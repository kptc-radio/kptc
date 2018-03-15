/***************************************************************************
						  modem.h  -  description
							 -------------------
	begin                : Wed Feb 14 2001
	copyright            : (C) 2001 by Lars Schnake
	email                : mail@lars-schnake.de

	Changed by Sebastian Martin Dicke
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef MODEM_H
#define MODEM_H

#include "configdata.h"

#include <algorithm>

#include <qobject.h>

#include <qdir.h>
#include <qregexp.h>
#include <qtimer.h>
#include <qsocketnotifier.h>

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <assert.h>
#include <signal.h>

#include <sys/types.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <setjmp.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>

/**modem class for serial communication
  *@author Lars Schnake
  */

class Modem : public QObject  {

	Q_OBJECT

	public:
		static Modem *modem;

		Modem();
		~Modem();
		/** lock and open serial port with settings of ConfigData*/
		bool opentty();
		/** unlock and close serial port */
		bool closetty();
		/** write a char to port */
		bool writeChar(unsigned char);
		/** write a char[] + Enter to port*/
		bool writeString(QString data);
		bool writeLine(QString data);
		bool writeLine2(QString data);
        bool dataMode() const {return data_mode; }
        void setDataMode(bool set) {data_mode = set; }
		/** returns the last modem (error?)message */
		const QString modemMessage();
		/** set up the connection to get notified if data was received */
		void notify(const QObject *, const char *);
		/** disconnect the notify connection (see notify)*/
		void stop();
		/** send escape to change to command mode*/
		void send_esc();
		/** simple read from port */
		int rs232_read(void *, int);
		int getFD();

	signals:
		/** is emitted if data was received */
		void charWaiting(unsigned char);

	public slots:
		/** notify if chars arriving from ptc */
		void startNotifier();
		/** stop the notifier */
		void stopNotifier();
		/** read from port */
		void readtty(int);

	private:
		bool modem_is_locked;
		int modemfd;
		unsigned char dataMask;
		bool data_mode;
		QSocketNotifier *sn;
		QString errmsg;
		struct termios initial_tty;
		struct termios tty;
		QString qdev;

		speed_t modemspeed();  // parse modem speed
		bool lock_device();
		bool unlock_device();
};

#endif

