/***************************************************************************
						  modecommander.h  -  description
							 -------------------
	begin                : Wed Feb 14 2001
	copyright            : (C) 2001 by Lars Schnake
	email                : mail@lars-schnake.de

	Ported to Qt5 by Sebastian Martin Dicke in 2017 (Sebastianmartindicke [@] gmx [.] de)
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef MODECOMMANDER_H
#define MODECOMMANDER_H

#include "modem.h"

#include <qobject.h>

/**
  *@author Lars Schnake
  */

class ModeCommander : public QObject  {

	Q_OBJECT

	public:
		ModeCommander(QObject *parent = 0);
		~ModeCommander();
		void setcurrendmod(QString);
		QString currendmod();

	public slots:
		void changetoPactor();
		void changetoAmtor();
		void changetoAmtorMon();
		void changetoRTTY();
		void changetoPSK31();
		void changetoCW();
		void Standby();
		void QRT();
		void Changeover();
		// PACTOR
		void Unproto();
		void Listen();
		bool isListen();
		void setListen(bool);
		// Amtor
		void Monitor();
		void FEC();
		// cw
		void cwprescribe();
		void cwflush();
		void increaseCWSpeed();
		void decreaseCWSpeed();
		void cwautospeed();

	private:
		QString qscurrendmod; // for statusbar message
		bool blisten;
};

#endif
