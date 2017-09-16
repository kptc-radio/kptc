/***************************************************************************
						  modebuttons.cpp  -  description
							 -------------------
	begin                : Wed Mar 7 2001
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

#include "modebuttons.h"

ModeButtons::ModeButtons(QWidget *parent ) : QObject(parent) {
	buttongroup = new QButtonGroup(parent);
	pactorButton = new QPushButton("Pactor", parent);
	amtorButton = new QPushButton("Amtor", parent);
	rttyButton = new QPushButton("RTTY", parent);
	psk31Button = new QPushButton("PSK31", parent);
	cwButton = new QPushButton("CW", parent);
	buttongroup->addButton( pactorButton, 1);
	buttongroup->addButton( amtorButton, 2);
	buttongroup->addButton( rttyButton, 3);
	buttongroup->addButton( psk31Button, 4);
	buttongroup->addButton( cwButton, 5);
	buttongroup->setExclusive(true);
// TODO Make buttons to togglebuttons?
}
ModeButtons::~ModeButtons() {
	buttongroup->deleteLater();
}
