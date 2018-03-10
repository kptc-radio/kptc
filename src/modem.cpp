
/***************************************************************************
						  modem.cpp  -  description
							 -------------------
	begin                : Wed Feb 14 2001
	copyright            : (C) 2001 by Lars Schnake
													based on Bernd Johannes Wuebben´s version for (Kppp)
	email                : mail@lars-schnake.de
	Changed slightly by Sebastian Martin Dicke
 ***************************************************************************/

#include "modem.h"
#include <QDebug>

//	defaults for UUCP style lock files :
constexpr char *LF_PATH = "/var/lock";
constexpr char *LF_PREFIX = "LCK	";

Modem *Modem::modem = 0;

Modem::Modem() :
	modemfd(-1),
	sn(0L),
	data_mode(false),
	dataMask(0xFF)

{
	assert(modem == 0);
	modem = this;
	modem_is_locked = false;
	qdev = "";
}

Modem::~Modem() {
	modem = 0;
}

speed_t Modem::modemspeed() {
	// convert the string modem speed to a t_speed type
	// to set the modem.	The constants here should all be ifdef'd because
	// other systems may not have them
 int i = configdata.getPortSpeed().toInt()/100;
	switch(i) {
	case 24:
	return B2400;
	break;
	case 96:
	return B9600;
	break;
	case 192:
	return B19200;
	break;
	case 384:
	return B38400;
	break;
#ifdef B57600
	case 576:
	return B57600;
	break;
#endif

#ifdef B115200
	case 1152:
	return B115200;
	break;
#endif

#ifdef B230400
	case 2304:
	return B230400;
	break;
#endif

#ifdef B460800
	case 4608:
	return 4608;
	break;
#endif

	default:
	return B38400;
	break;
	}
}

/* open serial port with settings of ConfigData*/
bool Modem::opentty() {

	// get device from config file
	qdev = configdata.getPort();
	// lock the device:
	if ( lock_device() == false ) {
		errmsg = /*i18n*/("Error by device locking");
		return false;
	}
	//	int flags;
	modemfd = open(qdev.toStdString().c_str(), O_RDWR | O_NOCTTY );
	if(modemfd	<0) {
		errmsg = /*i18n*/("Sorry, can't open modem.");
		return false;
	}
	tcdrain (modemfd);
	tcflush (modemfd, TCIOFLUSH);
	if(tcgetattr(modemfd, &tty) < 0){
		// this helps in some cases
		tcsendbreak(modemfd, 0);
		sleep(1);
		if(tcgetattr(modemfd, &tty) < 0){
			errmsg = /*i18n*/("Sorry, the modem is busy.");
			::close(modemfd);
			modemfd = -1;
			return false;
		}
	}
	memset(&initial_tty, '\0', sizeof(initial_tty));
	initial_tty = tty;
	tty.c_cc[VMIN] = 0; // nonblocking
	tty.c_cc[VTIME] = 0;
	tty.c_oflag = 0;
	tty.c_lflag = 0;
	tty.c_cflag &= ~(CSIZE | CSTOPB | PARENB);
	tty.c_cflag |= CS8 | CREAD;
	tty.c_cflag |= CLOCAL;					 // ignore modem status lines
	tty.c_iflag = IGNBRK | IGNPAR /* | ISTRIP */ ;
	tty.c_lflag &= ~ICANON;					// non-canonical mode
	tty.c_lflag &= ~(ECHO|ECHOE|ECHOK|ECHOKE);
	cfsetospeed(&tty, modemspeed());
	cfsetispeed(&tty, modemspeed());
	tcdrain(modemfd);
	if(tcsetattr(modemfd, TCSANOW, &tty) < 0){
		errmsg = /*i18n*/("Sorry, the modem is busy.");
		::close(modemfd);
		modemfd=-1;
		return false;
	}
	errmsg = /*i18n*/("Modem Ready.");
	return true;
}

bool Modem::closetty() {
	if(modemfd >=0 ) {
		stop();
		/* discard data not read or transmitted */
		tcflush(modemfd, TCIOFLUSH);
		if(tcsetattr(modemfd, TCSANOW, &initial_tty) < 0){
			errmsg = /*i18n*/("Can't restore tty settings: tcsetattr()\n");
			::close(modemfd);
			modemfd = -1;
			return false;
		}
		::close(modemfd);
		modemfd = -1;
	}
	if ( unlock_device() == false )	{
		errmsg = /*i18n*/("cannot unlock device !"); return false;
	}
	return true;
}

void Modem::readtty(int) {
	char buffer[50];
	unsigned char c;
	int len;
	// read data in chunks of up to 50 bytes
	if((len = ::read(modemfd, buffer, 50)) > 0) {
	// split buffer into single characters for further processing
		for(int i = 0; i < len; i++) {
			c = buffer[i] & dataMask;
			emit charWaiting(c);
		}
	}
}

void Modem::notify(const QObject *receiver, const char *member) {
	connect(this, SIGNAL(charWaiting(unsigned char)), receiver, member);
	startNotifier();
}

void Modem::stop() {
	disconnect(SIGNAL(charWaiting(unsigned char)));
	stopNotifier();
}

void Modem::startNotifier() {
	if(modemfd >= 0) {
		if(sn == 0) {
			sn = new QSocketNotifier(modemfd, QSocketNotifier::Read, this);
			connect(sn, SIGNAL(activated(int)), SLOT(readtty(int)));
			qDebug() << "QSocketNotifier started!" << endl;
		} else {
			sn->setEnabled(true);
		}
	}
}

void Modem::stopNotifier() {
	if(sn != 0) {
		sn->setEnabled(false);
		disconnect(sn);
		delete sn;
		sn = 0;
		qDebug() << "QSocketNotifier stopped!" << endl;
	}
}

bool Modem::writeChar(unsigned char c) {
	return write(modemfd, &c, 1) == 1;
}

bool Modem::writeLine(QString data) {
	auto buf = data.toStdString().c_str();
	write(modemfd, buf, strlen(buf));
	//Let's send an "enter"
	write(modemfd, "\r", 1);
	return true;
}

void Modem::send_esc(){
	constexpr char esc = 27;
	writeChar(esc);
}

bool Modem :: lock_device()
{
	char *device;
	device = (char *) qdev.toStdString().data();
	char lckf[128];
	int lfh;
	pid_t lckpid;
	char *devicename;
	char lckpidstr[20];
	int nb;
	struct stat buf;
	devicename = strrchr(device, '/');
	sprintf(lckf, "%s/%s%s", LF_PATH, LF_PREFIX, (devicename ? (devicename + 1) : device));
	/*
	 * Check if there's already a LCK	* file
	 */
	if (stat(lckf, &buf) == 0) {
		/*
		 * we must now expend effort to learn if it's stale or not.
		 */
		if ((lfh = open(lckf, O_RDONLY)) != -1) {
			nb = read(lfh, &lckpidstr, std::min(static_cast<__off_t>(20), buf.st_size));
			if (nb > 0) {
				lckpidstr[nb] = 0;
				sscanf(lckpidstr, "%d", &lckpid);
				if (kill(lckpid, 0) == 0) {
					qDebug() << "Device " << device <<" is locked by process " << lckpid << endl;
					return false;
				}

				/*
				 * The lock file is stale. Remove it.
				 */
				if (unlink(lckf)) {
					qDebug() << "Unable to unlink stale lock file: " << lckf << endl;
					return false;
				}
			} else {
				qDebug() << "Cannot read from lock file: "<< lckf << endl ;
				return false;
			}
		} else {
			qDebug() << "Cannot open existing lock file: " << lckf	<< endl;
			return false;
		}
	}
	if ((lfh = open(lckf, O_WRONLY | O_CREAT | O_EXCL,	S_IWRITE | S_IREAD | S_IRGRP | S_IROTH)) < 0) {
		qDebug() << "Cannot create lockfile. Sorry." << endl;
		return false;
	}
	sprintf(lckpidstr, "%10d\n", getpid());
	write(lfh, lckpidstr, strlen(lckpidstr));
	close(lfh);
		modem_is_locked = true;
	return true;
}

bool Modem :: unlock_device()
{
	if(!modem_is_locked && qdev == "") {
		qDebug() << "confused by unlock device, sorry !"<< endl;
		return false;
	}
	char *device;
	device = (char *) qdev.toStdString().c_str();
	char lckf[128];
	char *devicename;
	devicename = strrchr(device, '/');
	sprintf(lckf, "%s/%s%s", LF_PATH, LF_PREFIX, (devicename ? (devicename + 1) : device));
	if (unlink(lckf)) {
		qDebug() << "Unable to unlink lock file: " << lckf << endl;
		return false;
	}
	return true;
}

int Modem::getFD() {
	return modemfd;
}

/*
 * Read from RS232 interface
 */
int Modem::rs232_read( void *bp, int maxlen)
{
	fd_set set;
	struct timeval timeout;
	int  max;
	int endloop;
	int res;
	max = 0;
	endloop = 0;
	do {
		timeout.tv_sec = 0;
		timeout.tv_usec = 500000; /* 0.5 seconds */

		FD_ZERO(&set);
		FD_SET(modemfd, &set);
		res = select(FD_SETSIZE, &set, NULL, NULL, &timeout);
		if (0 == res) {
			fprintf(stderr, "ERROR: timed out!\n");
			endloop = 1;
		} else if (-1 == res) {
			perror("rs232_read select");
			exit(10);
		} else {
			max += read(modemfd, bp, maxlen);
		}
	} while ((max < maxlen) && !endloop);
	return res;
}

const QString Modem::modemMessage() {
	return errmsg;
}
