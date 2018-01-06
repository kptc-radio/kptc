/***************************************************************************
						  modem.cpp  -  description
							 -------------------
	begin				: Wed Feb 14 2001
	copyright			: (C) 2001 by Lars Schnake
													based on Bernd Johannes Wuebben´s version for (Kppp)
	email				: mail@lars-schnake.de

	Changed by Sebastian Martin Dicke
 ***************************************************************************/

#include "modem.h"
#include <iostream>

//  defaults for UUCP style lock files :
#define LF_PATH	"/var/lock"
#define LF_PREFIX	"LCK.."

Modem *Modem::modem = 0;

Modem::Modem() : modem_is_locked(false), qdev(""), modemfd(-1), sn(0L), data_mode(false), dataMask(0xFF)
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	assert(modem == 0);
	modem = this;
}

Modem::~Modem() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	modem = 0;
	delete modem;
}

speed_t Modem::modemspeed() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	// convert the string modem speed to a t_speed type
	// to set the modem.  The constants here should all be ifdef'd because
	// other systems may not have them
	const int i = configdata.getPortSpeed().toInt();

	switch(i) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		case 2400:
			return B2400;
			break;
		case 9600:
			return B9600;
			break;
		case 19200:
			return B19200;
			break;
		case 38400:
			return B38400;
			break;
		#ifdef B57600
		case 57600:
			return B57600;
			break;
		#endif
		#ifdef B115200
		case 115200:
			return B115200;
			break;
		#endif
		#ifdef B230400
			case 230400:
				return B230400;
			break;
		#endif
	#ifdef B460800
		case 460800:
			return 4608;
			break;
	#endif
		default:
			return B38400;
			break;
	}
}

/* open serial port with settings of ConfigData*/
bool Modem::opentty() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	// get device from config file
	qdev = configdata.getPort();
	// lock the device:
	if ( lock_device() == false ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		errmsg = tr("Error by device locking");
		return false;
	}

	// int flags;
 modemfd = open(qdev.toStdString().c_str(), O_RDWR | O_NOCTTY );
	if(modemfd  <0) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		errmsg = tr("Sorry, can't open modem.");
		return false;
	}

	tcdrain (modemfd);
	tcflush (modemfd, TCIOFLUSH);

	if(tcgetattr(modemfd, &tty) < 0){ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		// this helps in some cases
		tcsendbreak(modemfd, 0);
		sleep(1);
		if(tcgetattr(modemfd, &tty) < 0){ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
			errmsg = tr("Sorry, the modem is busy.");
			close(modemfd);
			modemfd = -1;
			return false;
		}
	}

	memset(&initial_tty,'\0',sizeof(initial_tty));

	initial_tty = tty;

	tty.c_cc[VMIN] = 0; // nonblocking
	tty.c_cc[VTIME] = 0;
	tty.c_oflag = 0;
	tty.c_lflag = 0;

	tty.c_cflag &= ~(CSIZE | CSTOPB | PARENB);
	tty.c_cflag |= CS8 | CREAD;
	tty.c_cflag |= CLOCAL;				   // ignore modem status lines
	tty.c_iflag = IGNBRK | IGNPAR /* | ISTRIP */ ;
	tty.c_lflag &= ~ICANON;				  // non-canonical mode
	tty.c_lflag &= ~(ECHO|ECHOE|ECHOK|ECHOKE);

	cfsetospeed(&tty, modemspeed());
	cfsetispeed(&tty, modemspeed());

	tcdrain(modemfd);

	if(tcsetattr(modemfd, TCSANOW, &tty) < 0){ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	errmsg = tr("Sorry, the modem is busy.");
	::close(modemfd);
	modemfd=-1;
	return false;
	}

	errmsg = tr("Modem Ready.");
	return true;
}

bool Modem::closetty() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	if(modemfd >=0 ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	stop();
	/* discard data not read or transmitted */
	tcflush(modemfd, TCIOFLUSH);

	if(tcsetattr(modemfd, TCSANOW, &initial_tty) < 0){ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		errmsg = tr("Can't restore tty settings: tcsetattr()\n");
		::close(modemfd);
		modemfd = -1;
		return false;
	}
	::close(modemfd);
	modemfd = -1;
	}

	if ( unlock_device() == false )  { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		errmsg = tr("cannot unlock device !");
		return false;
	}

	return true;
}

void Modem::readtty(int) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	char buffer[50];
	unsigned char c;
	int len;

	// read data in chunks of up to 50 bytes
	if((len = ::read(modemfd, buffer, 50)) > 0) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		// split buffer into single characters for further processing
		for(int i = 0; i < len; i++) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
			c = buffer[i] & dataMask;
			emit charWaiting(c);
		}
	}
}

void Modem::notify(const QObject *receiver, const char *member) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	connect(this, SIGNAL(charWaiting(unsigned char)), receiver, member);
	startNotifier();
}

void Modem::stop() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	disconnect(SIGNAL(charWaiting(unsigned char)));
	stopNotifier();
}

void Modem::startNotifier() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	if(modemfd >= 0) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		if(sn == 0) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
			sn = new QSocketNotifier(modemfd, QSocketNotifier::Read, this);
			connect(sn, SIGNAL(activated(int)), SLOT(readtty(int)));
			////qDebug() << "QSocketNotifier started!";
		} else { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
			sn->setEnabled(true);
		}
	}
}

void Modem::stopNotifier() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	if(sn != 0) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		sn->setEnabled(false);
		disconnect(sn);
		//TODO
		delete sn;
		sn = 0;
		////qDebug () << "QSocketNotifier stopped!" ;
	}
}

bool Modem::writeChar(unsigned char c) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return write(modemfd, &c, 1) == 1;
}

bool Modem::writeLine(QString data) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	const char *buf = data.toStdString().c_str();
	write(modemfd, buf, strlen(buf));

	//Let's send an "enter"
	write(modemfd, "\r", 1);
	return true;
}

void Modem::send_esc(){ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	writeChar(27);
}

bool Modem :: lock_device()
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	const char * device;
	device = qdev.toStdString().c_str();

	char	lckf[128];
	int	lfh;
	pid_t	lckpid;
	const char	*devicename;
	char	lckpidstr[20];
	int	nb;
	struct stat buf;
	devicename = strrchr(device, '/');
	sprintf(lckf, "%s/%s%s", LF_PATH, LF_PREFIX, (devicename ? (devicename + 1) : device));

	/*
	* Check if there's already a LCK..* file
	*/
	if (stat(lckf, &buf) == 0) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		/*
		 * we must now expend effort to learn if it's stale or not.
		 */
		if ((lfh = open(lckf, O_RDONLY)) != -1) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
			nb = read(lfh, &lckpidstr, std::min(20L, buf.st_size));
			if (nb > 0) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
				lckpidstr[nb] = 0;
				sscanf(lckpidstr, "%d", &lckpid);
				if (kill(lckpid, 0) == 0) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
					////qDebug ()<< "Device " << device <<" is locked by process " << lckpid;
					return false;
				}

				/*
				 * The lock file is stale. Remove it.
				 */
				if (unlink(lckf)) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
					////qDebug() << "Unable to unlink stale lock file: " << lckf ;
					return false;
				}
			} else { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
				////qDebug ()<< "Cannot read from lock file: "<< lckf ;
				return false;
			}
		} else { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
			////qDebug ()<< "Cannot open existing lock file: " << lckf ;
			return false;
		}
	}

	if ((lfh = open(lckf, O_WRONLY | O_CREAT | O_EXCL,  S_IWRITE | S_IREAD | S_IRGRP | S_IROTH)) < 0) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		////qDebug ()<< "Cannot create lockfile. Sorry.";
		return false;
	}
	sprintf(lckpidstr, "%10d\n", getpid());
	write(lfh, lckpidstr, strlen(lckpidstr));
	close(lfh);
	modem_is_locked = true;
	return true;
}

bool Modem :: unlock_device()
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	if(! modem_is_locked  && qdev == "") { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		////qDebug() << "confused by unlock device, sorry !";
		return false;
	}

	const char *device;
	device = qdev.toStdString().c_str();
	char	lckf[128];
	const char	*devicename;

	char device2[sizeof(device)];
	strcpy(device2, device);
	devicename = strrchr(device2, '/');
	sprintf(lckf, "%s/%s%s", LF_PATH, LF_PREFIX, (devicename ? (devicename + 1) : device));

	if (unlink(lckf)) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	////qDebug() << "Unable to unlink lock file: " << lckf ;
		return false;
	}
	return true;
}

int Modem::getFD() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return modemfd;
}

/*
 * Read from RS232 interface
 */
int Modem::rs232_read( void *bp, int maxlen)
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	fd_set set;
	struct timeval	timeout;
	int max;
	int endloop;
	int res;
	max = 0;
	endloop = 0;
	do { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		timeout.tv_sec = 0;
		timeout.tv_usec = 500000; /* 0.5 seconds */

		FD_ZERO(&set);
		FD_SET(modemfd, &set);
		res = select(FD_SETSIZE, &set, NULL, NULL, &timeout);
		if (0 == res) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
			fprintf(stderr, "ERROR: timed out!\n");
			endloop = 1;
		} else if (-1 == res) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
			perror("rs232_read select");
			exit(10);
		} else { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
			max += read(modemfd, bp, maxlen);
		}
	} while ((max < maxlen) && !endloop);
	return res;
}

const QString Modem::modemMessage() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
  return errmsg;
}
