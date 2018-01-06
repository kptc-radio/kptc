/***************************************************************************
						  configmachine.cpp  -  description
							 -------------------
	begin                : Wed Feb 14 2001
	copyright            : (C) 2001 by Lars Schnake
	email                : lschnak@suse.de

	Ported to Qt5 by Sebastian Martin Dicke in 2017 (Sebastianmartindicke [@] gmx [.] de )
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <iostream>
#include "configmachine.h"

ConfigMachine::ConfigMachine(QWidget *_top) : top(_top) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;}

void ConfigMachine::doconfig(){ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	Modem::modem->closetty();
	if ( ! Modem::modem->opentty() ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;

	// todo errorhandling !
	}

	Modem::modem->startNotifier();
	//qDebug << Modem::modem->modemMessage()<< endl;

	Modem::modem->closetty();

	Modem::modem->writeLine("");
	Modem::modem->send_esc();
	Modem::modem->writeLine("dd");
	Modem::modem->send_esc();
	Modem::modem->writeLine("pt");
	Modem::modem->send_esc();
	Modem::modem->writeLine("term 5"); // terminal setup
	Modem::modem->send_esc();
	Modem::modem->writeLine("status 2"); // auto status dump
	Modem::modem->writeChar((char)30); // poll for first status info
	Modem::modem->writeLine("listen 0"); // set listen mode off at startup
	Modem::modem->writeLine("lfignore 1"); // linefeed after <CR> ?
	sleep(1);
	// set mycall:
	QString qs = configdata.getCall();
	qs.prepend("mycall ");
	//Modem::modem->send_esc();
	Modem::modem->writeLine(qs);
	// set myselcall:
	qs = configdata.getSelCall();
	qs.prepend("myselc ");
	//Modem::modem->send_esc();
	Modem::modem->writeLine(qs);

	if ( configdata.isCMsg() ) {
		Modem::modem->writeLine( "cmsg 1");
	}
	else {
		Modem::modem->writeLine( "cmsg 0");
	}

	QString cmsg = configdata.getCMsg();
	cmsg = cmsg.replace( QRegExp("\n"), "#" );
	cmsg = configdata.parseMacroText ( cmsg );
	if (cmsg !="") {
		Modem::modem->writeLine( "ctext " + cmsg );
	}
}

void ConfigMachine::logout() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	if ( configdata.isAwayMsg() ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		QString ctext = configdata.getAwayMsg();
		ctext = configdata.parseMacroText( ctext );
		ctext = ctext.replace( QRegExp("\n"), "#" );
		ctext.prepend("cte ");
		ctext.append("\r");
		Modem::modem->send_esc();
		Modem::modem->writeLine( ctext );
		//qDebug () << "configmachine::logout- "<< ctext;
	}
	Modem::modem->send_esc();
	Modem::modem->writeLine("term 0"); // terminal setup

	if ( configdata.isLogoutScript() ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;

		QString qpath = configdata.getLogoutPath();
		QFile file ( qpath );
		if ( ! file.open (QIODevice::ReadOnly)) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
			QMessageBox::critical( top, "",
	("Cannot open your personal logout script file !\n Error by opening \"" + qpath +"\""  ) );	 // error by opening text file
		}
		else { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
			while (!file.atEnd()) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
				QString buffer = file.readLine(1024);
				Modem::modem->writeLine( QString(buffer) );
			}
			file.close() ;
			Modem::modem->writeLine("");
		}
	}
}

void ConfigMachine::login() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	if ( configdata.isLoginScript() ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		QString qpath = configdata.getLoginPath();
		QFile file ( qpath );
		if ( ! file.open (QIODevice::ReadOnly)) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
			QMessageBox::critical( top, "",
			("Cannot open your personal login script file !\n Error by opening \"" + qpath +"\""  ) );	 // error by opening text file
		}
		else { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
			while (!file.atEnd()) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
				QByteArray buffer = file.readLine(1024 );
				Modem::modem->writeLine( QString(buffer) );
			}
			file.close() ;
			Modem::modem->writeLine("");
		}
	}
}

ConfigMachine::~ConfigMachine(){ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
}
