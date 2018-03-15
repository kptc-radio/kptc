/***************************************************************************
						  configmachine.cpp  -  description
							 -------------------
	begin                : Wed Feb 14 2001
	copyright            : (C) 2001 by Lars Schnake
	email                : lschnak@suse.de

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

#include "configmachine.h"

ConfigMachine::ConfigMachine() {}

void ConfigMachine::doconfig() {
	Modem::modem->closetty();
	if (! Modem::modem->opentty()) {

	// TODO errorhandling !
	}

	Modem::modem->startNotifier();
	//qDebug << Modem::modem->modemMessage()<< endl;

	Modem::modem->closetty();

//	Modem::modem->writeString("");
//	Modem::modem->send_esc();
	Modem::modem->writeLine("");
//	Modem::modem->writeString("dd");
//	Modem::modem->send_esc();
	Modem::modem->writeLine("dd");
//	Modem::modem->writeString("pt");
//	Modem::modem->send_esc();
	Modem::modem->writeLine("pt");
//	Modem::modem->writeString("term 5"); // terminal setup
//	Modem::modem->send_esc();
	Modem::modem->writeLine("term 5"); // terminal setup)
	Modem::modem->writeString("status 2"); // auto status dump
	Modem::modem->writeChar(static_cast<char>(30)); // poll for first status info
	Modem::modem->writeString("listen 0"); // set listen mode off at startup
	Modem::modem->writeString("lfignore 1"); // linefeed after <CR> ?
	sleep(1);
	// set mycall:
	QString qs = configdata.getCall();
	qs.prepend("mycall ");
	//Modem::modem->send_esc();
	Modem::modem->writeString(qs);
	// set myselcall:
	qs = configdata.getSelCall();
	qs.prepend("myselc ");
	//Modem::modem->send_esc();
	Modem::modem->writeString(qs);

	if (configdata.isCMsg()) {
		Modem::modem->writeString("cmsg 1");
	}
	else {
		Modem::modem->writeString("cmsg 0");
	}

	QString cmsg = configdata.getCMsg();
	cmsg = cmsg.replace(QRegExp("\n"), "#");
	cmsg = configdata.parseMacroText (cmsg);
	if (cmsg !="") {
		Modem::modem->writeString("ctext " + cmsg);
	}
}

ConfigMachine::Pair ConfigMachine::logout() {
	if (configdata.isAwayMsg()) {
		QString ctext = configdata.getAwayMsg();
		ctext = configdata.parseMacroText(ctext);
		ctext = ctext.replace(QRegExp("\n"), "#");
		ctext.prepend("cte ");
		ctext.append("\r");
		Modem::modem->send_esc();
		Modem::modem->writeString(ctext);
		//qDebug () << "configmachine::logout- "<< ctext;
	}
	Modem::modem->send_esc();
	Modem::modem->writeString("term 0"); // terminal setup

	if (configdata.isLogoutScript()) {

		QString qpath = configdata.getLogoutPath();
		QFile file (qpath);
		if (! file.open (QIODevice::ReadOnly)) {
			return Pair(false, qpath);
		}
		else {
			while (!file.atEnd()) {
				QString buffer = file.readLine(1024);
				Modem::modem->writeString(QString(buffer));
			}
			file.close() ;
			Modem::modem->writeString("");
		}
	}
	return Pair(true, "");
}

ConfigMachine::Pair ConfigMachine::login() {
	if (configdata.isLoginScript()) {
		QString qpath = configdata.getLoginPath();
		QFile file (qpath);
		if (! file.open (QIODevice::ReadOnly)) {
			return Pair(false, qpath);
		}
		else {
			while (!file.atEnd()) {
				QByteArray buffer = file.readLine(1024);
				Modem::modem->writeString(QString(buffer));
			}
			file.close() ;
			Modem::modem->writeString("");
		}
	}
	return Pair(true, "");
}

ConfigMachine::~ConfigMachine() {
}

