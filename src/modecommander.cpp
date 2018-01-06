/***************************************************************************
						  modecommander.cpp  -  description
							 -------------------
	begin				: Wed Feb 14 2001
	copyright			: (C) 2001 by Lars Schnake
	email				: lschnak@suse.de

	Ported to Qt5 by Sebastian Martin Dicke in 2017 (Sebastianmartindicke [@] gmx [.] de )
 ***************************************************************************/

/***************************************************************************
 *																		 *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or	 *
 *   (at your option) any later version.								   *
 *																		 *
 ***************************************************************************/

#include <iostream>
#include "modecommander.h"

ModeCommander::ModeCommander(QObject *parent ) : blisten(false), QObject(parent) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;}

ModeCommander::~ModeCommander(){ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;}

QString ModeCommander::currendmod() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return qscurrendmod;
}

void ModeCommander::setcurrendmod( QString _qscurrendmod )
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	qscurrendmod = _qscurrendmod;
}

void ModeCommander::changetoPactor() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	Standby() ;
	Modem::modem->send_esc();
	Modem::modem->writeLine( "PT" );
}

void ModeCommander::changetoAmtor() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	Standby() ;
	Modem::modem->send_esc();
	Modem::modem->writeLine( "Amtor" );
}

void ModeCommander::changetoAmtorMon() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	Standby()	;
	Modem::modem->send_esc();
	Modem::modem->writeLine( "Amtor" );
}

void ModeCommander::changetoRTTY() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	Standby();
	Modem::modem->send_esc();
	Modem::modem->writeLine( "Baudot" );
}

void ModeCommander::changetoPSK31() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	Standby() ;
	Modem::modem->send_esc();
	Modem::modem->writeLine( "PSKTerm" );
}

void ModeCommander::changetoCW() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	Standby() ;
	Modem::modem->send_esc();
	Modem::modem->writeLine( "cwt" );
}

void ModeCommander::Standby() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	Modem::modem->send_esc();
	Modem::modem->writeLine( "dd" );
}

void ModeCommander::Changeover() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	char c = 25;
	Modem::modem->writeChar( c );
}


void ModeCommander::QRT() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	char c = 4;
	Modem::modem->writeChar( c );
}

void ModeCommander::Unproto() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	Modem::modem->send_esc();
	Modem::modem->writeLine( "Unproto" );
}


void ModeCommander::Listen() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	Modem::modem->send_esc();
	if ( !blisten ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		Modem::modem->writeLine( "l 1" );
		blisten = true;
	}
	else { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		Modem::modem->writeLine( "l 0" );
		blisten = false;
	}
}

bool ModeCommander::isListen() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return blisten;
}

void ModeCommander::setListen(bool _blisten) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	blisten = _blisten; }

void ModeCommander::Monitor() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	Modem::modem->send_esc();
	Modem::modem->writeLine( "mon" );
}

void ModeCommander::FEC() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	Modem::modem->send_esc();
	Modem::modem->writeLine( "fec" );
}

void ModeCommander::cwprescribe() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	char c = 25; // CTRL + Key_Y
	Modem::modem->writeChar( c );
	sleep(1);
	Modem::modem->writeChar( c );
}

void ModeCommander::cwflush() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	char c = 25; // CTRL + Key_Y
	Modem::modem->writeChar( c );
}

void ModeCommander::increaseCWSpeed() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	char c = 21;  // CTRL + Key_U
	Modem::modem->writeChar( c );
}

void ModeCommander::decreaseCWSpeed() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	char c = 4;  // CTRL + Key_D
	Modem::modem->writeChar( c );
}

void ModeCommander::cwautospeed() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	Modem::modem->writeChar( (char) 6 );	// CTRL + Key_F
}
