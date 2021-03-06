/***************************************************************************
						  modecommander.cpp  -  description
							 -------------------
	begin				: Wed Feb 14 2001
	copyright			: (C) 2001 by Lars Schnake
	email				: lschnak@suse.de

	Ported to Qt5 by Sebastian Martin Dicke in 2017 (Sebastianmartindicke [@] gmx [.] de)
 ***************************************************************************/

/***************************************************************************
 *																		 *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or	 *
 *   (at your option) any later version.								   *
 *																		 *
 ***************************************************************************/

#include "modecommander.h"

ModeCommander::ModeCommander(QObject *parent) : blisten(false), QObject(parent) {}

ModeCommander::~ModeCommander() {}

QString ModeCommander::currendmod() {
	return qscurrendmod;
}

void ModeCommander::setcurrendmod(QString _qscurrendmod)
{
	qscurrendmod = _qscurrendmod;
}

void ModeCommander::changetoPactor() {
	Standby();
//	Modem::modem()->send_esc();
//	Modem::modem()->writeString("PT");
	Modem::modem()->writeLine2("PT");
}

void ModeCommander::changetoAmtor() {
	Standby();
//	Modem::modem()->send_esc();
//	Modem::modem()->writeString("Amtor");
	Modem::modem()->writeLine2("Amtor");
}

void ModeCommander::changetoAmtorMon() {
	Standby();
//	Modem::modem()->send_esc();
//	Modem::modem()->writeString("Amtor");
	Modem::modem()->writeLine2("Amtor");
}

void ModeCommander::changetoRTTY() {
	Standby();
//	Modem::modem()->send_esc();
//	Modem::modem()->writeString("Baudot");
	Modem::modem()->writeLine2("Baudot");
}

void ModeCommander::changetoPSK31() {
	Standby();
//	Modem::modem()->send_esc();
//	Modem::modem()->writeString("PSKTerm");
	Modem::modem()->writeLine2("PSKTerm");
}

void ModeCommander::changetoCW() {
	Standby();
//	Modem::modem()->send_esc();
//	Modem::modem()->writeString("cwt");
	Modem::modem()->writeLine2("cwt");
}

void ModeCommander::Standby() {
//	Modem::modem()->send_esc();
//	Modem::modem()->writeString("dd");
	Modem::modem()->writeLine2("dd");
}

void ModeCommander::Changeover() {
	char c = 25;
	Modem::modem()->writeChar(c);
}


void ModeCommander::QRT() {
	char c = 4;
	Modem::modem()->writeChar(c);
}

void ModeCommander::Unproto() {
//	Modem::modem()->send_esc();
//	Modem::modem()->writeString("Unproto");
	Modem::modem()->writeLine2("Unproto");
}


void ModeCommander::Listen() {
	Modem::modem()->send_esc();
	if (!blisten) {
		Modem::modem()->writeString("l 1");
		blisten = true;
	}
	else {
		Modem::modem()->writeString("l 0");
		blisten = false;
	}
}

bool ModeCommander::isListen() {
	return blisten;
}

void ModeCommander::setListen(bool _blisten) {
	blisten = _blisten; }

void ModeCommander::Monitor() {
//	Modem::modem()->send_esc();
//	Modem::modem()->writeString("mon");
	Modem::modem()->writeLine2("mon");
}

void ModeCommander::FEC() {
//	Modem::modem()->send_esc();
//	Modem::modem()->writeString("fec");
	Modem::modem()->writeLine2("fec");
}

void ModeCommander::cwprescribe() {
	char c = 25; // CTRL + Key_Y
	Modem::modem()->writeChar(c);
	sleep(1);
	Modem::modem()->writeChar(c);
}

void ModeCommander::cwflush() {
	char c = 25; // CTRL + Key_Y
	Modem::modem()->writeChar(c);
}

void ModeCommander::increaseCWSpeed() {
	char c = 21;  // CTRL + Key_U
	Modem::modem()->writeChar(c);
}

void ModeCommander::decreaseCWSpeed() {
	char c = 4;  // CTRL + Key_D
	Modem::modem()->writeChar(c);
}

void ModeCommander::cwautospeed() {
	Modem::modem()->writeChar((char) 6);	// CTRL + Key_F
}
