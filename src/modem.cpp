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
constexpr char LF_PATH[10] = "/var/lock";
constexpr char LF_PREFIX[5] = "LCK	";

Modem *Modem::modem()
{
	static Modem instance;
	return &instance;
}

Modem::Modem() :
	modemfd(-1),
	notifier(0L),
	data_mode(false),
	dataMask(0xFF)

{
	modem_is_locked = false;
	qdev = "";
}

Modem::~Modem() {}

speed_t Modem::modemspeed() {
	// convert the string modem speed to a t_speed type
	// to set the modem.	The constants here should all be ifdef'd because
	// other systems may not have them
 int speed = configdata.getPortSpeed() / 100;
	switch(speed) {
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
	if (lock_device() == false) {
		errmsg = /*i18n*/tr("Error by device locking");
		return false;
	}
	//	int flags;
	const char *const devString = qdev.toStdString().c_str();
	modemfd = open(devString, O_RDWR | O_NOCTTY);
	if(modemfd	< 0) {
		errmsg = /*i18n*/tr("Sorry, can't open modem.");
		return false;
	}
	tcdrain (modemfd);
	tcflush (modemfd, TCIOFLUSH);
	const auto getResult = tcgetattr(modemfd, &tty);
	if(getResult < 0){
		// this helps in some cases
		tcsendbreak(modemfd, 0);
		sleep(1);
		if(tcgetattr(modemfd, &tty) < 0){
			errmsg = /*i18n*/tr("Sorry, the modem is busy.");
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
	const auto setResult = tcsetattr(modemfd, TCSANOW, &tty);
	if(setResult < 0){
		errmsg = /*i18n*/tr("Sorry, the modem is busy.");
		::close(modemfd);
		modemfd=-1;
		return false;
	}
	errmsg = /*i18n*/tr("Modem Ready.");
	return true;
}

bool Modem::closetty() {
	if(modemfd >= 0) {
		stop();
		/* discard data not read or transmitted */
		tcflush(modemfd, TCIOFLUSH);
		const int setResult = tcsetattr(modemfd, TCSANOW, &initial_tty);
		if(setResult < 0){
			errmsg = /*i18n*/tr("Can't restore tty settings: tcsetattr()\n");
			::close(modemfd);
			modemfd = -1;
			return false;
		}
		::close(modemfd);
		modemfd = -1;
	}
	if (unlock_device() == false) {
		errmsg = /*i18n*/tr("cannot unlock device !"); return false;
	}
	return true;
}

void Modem::readtty(int) {
	char buffer[50];
	unsigned char c;
	int len;
	// read data in chunks of up to 50 bytes
	int charsRead = len = ::read(modemfd, buffer, 50);
	if(charsRead > 0) {
	// split buffer into single characters for further processing
		for (auto &current : buffer) {
			c = current & dataMask;
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
		if(notifier == 0) {
			notifier = new QSocketNotifier(modemfd, QSocketNotifier::Read, this);
			connect(notifier, SIGNAL(activated(int)), SLOT(readtty(int)));
			qDebug() << tr("QSocketNotifier started!") << endl;
		} else {
			notifier->setEnabled(true);
		}
	}
}

void Modem::stopNotifier() {
	if(notifier != 0) {
		notifier->setEnabled(false);
		disconnect(notifier);
		delete notifier;
		notifier = 0;
		qDebug() << tr("QSocketNotifier stopped!") << endl;
	}
}

bool Modem::writeChar(unsigned char c) {
	auto result = write(modemfd, &c, 1);
	return result == 1;
}

bool Modem::writeString(QString data) {
	auto buf = data.toStdString().c_str();
	write(modemfd, buf, strlen(buf));
	//Let's send an "enter"
	write(modemfd, "\r", 1);
	return true;
}

bool Modem::writeLine(QString data) {
	auto result = this->writeString(data);
	this->send_esc();
	return result;
}

bool Modem::writeLine2(QString data) {
	this->send_esc();
	return this->writeString(data);
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
	const auto statResult = stat(lckf, &buf);
	if (statResult == 0) {
		/*
		 * we must now expend effort to learn if it's stale or not.
		 */
		lfh = open(lckf, O_RDONLY);
		if (lfh != -1) {
			const auto byteToRead = std::min(static_cast<__off_t>(20), buf.st_size);
			nb = read(lfh, &lckpidstr, byteToRead);
			if (nb > 0) {
				lckpidstr[nb] = 0;
				sscanf(lckpidstr, "%d", &lckpid);
				const auto killResult = kill(lckpid, 0);
				if (killResult == 0) {
					qDebug() << tr("Device ") << device << tr(" is locked by process ") << lckpid << endl;
					return false;
				}

				/*
				 * The lock file is stale. Remove it.
				 */
				if (unlink(lckf)) {
					qDebug() << tr("Unable to unlink stale lock file: ") << lckf << endl;
					return false;
				}
			} else {
				qDebug() << tr("Cannot read from lock file: ") << lckf << endl ;
				return false;
			}
		} else {
			qDebug() << tr("Cannot open existing lock file: ") << lckf	<< endl;
			return false;
		}
	}
	lfh = open(lckf, O_WRONLY | O_CREAT | O_EXCL,	S_IWRITE | S_IREAD | S_IRGRP | S_IROTH);
	if (lfh < 0) {
		qDebug() << tr("Cannot create lockfile. Sorry.") << endl;
		return false;
	}
	sprintf(lckpidstr, "%10d\n", getpid());
	const auto lckpidstrLength = strlen(lckpidstr);
	write(lfh, lckpidstr, lckpidstrLength);
	close(lfh);
	modem_is_locked = true;
	return true;
}

bool Modem :: unlock_device()
{
	if(!modem_is_locked && qdev == "") {
		qDebug() << tr("confused by unlock device, sorry !") << endl;
		return false;
	}
	char *device;
	device = (char *) qdev.toStdString().c_str();
	char lckf[128];
	char *devicename;
	devicename = strrchr(device, '/');
	sprintf(lckf, "%s/%s%s", LF_PATH, LF_PREFIX, (devicename ? (devicename + 1) : device));
	if (unlink(lckf)) {
		qDebug() << tr("Unable to unlink lock file: ") << lckf << endl;
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
int Modem::rs232_read(void *bp, int maxlen, bool breakonerror)
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
		if (res == 0) {
			fprintf(stderr, "ERROR: timed out!\n");
			endloop = 1;
		} else if (res == -1) {
			if (breakonerror) {
				perror("rs232_read select");
				exit(10);
			}
			else {
				break;
			}
		} else {
			max += read(modemfd, bp, maxlen);
		}
	} while ((max < maxlen) && !endloop);
	return res;
}

const QString Modem::modemMessage() {
	return errmsg;
}
