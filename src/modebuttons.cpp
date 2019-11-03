/***************************************************************************
						  modebuttons.cpp  -  description
							 -------------------
	begin				: Wed Mar 7 2001
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

#include "modebuttons.h"

ModeButtons::ModeButtons(ModeCommander *commander, QWidget *parent) : commander(commander), QToolBar(parent) {
	buttongroup.setParent(parent);
	pactorButton.setText(tr("Pactor"));
	pactorButton.setParent(parent);
	amtorButton.setText(tr("Amtor"));
	amtorButton.setParent(parent);
	rttyButton.setText(tr("RTTY"));
	rttyButton.setParent(parent);
	psk31Button.setText(tr("PSK31"));
	psk31Button.setParent(parent);
	cwButton.setText(tr("CW"));
	cwButton.setParent(parent);

	buttongroup.addButton(&pactorButton, 1);
	buttongroup.addButton(&amtorButton, 2);
	buttongroup.addButton(&rttyButton, 3);
	buttongroup.addButton(&psk31Button, 4);
	buttongroup.addButton(&cwButton, 5);
	buttongroup.setExclusive(true);

	addWidget(&pactorButton);
	addWidget(&amtorButton);
	addWidget(&rttyButton);
	addWidget(&psk31Button);
	addWidget(&cwButton);

	connect(&pactorButton, &QRadioButton::clicked, commander, &ModeCommander::changetoPactor);
	connect(&amtorButton, &QRadioButton::clicked, commander, &ModeCommander::changetoAmtor);
	connect(&rttyButton, &QRadioButton::clicked, commander, &ModeCommander::changetoRTTY);
	connect(&psk31Button, &QRadioButton::clicked, commander, &ModeCommander::changetoPSK31);
	connect(&cwButton, &QRadioButton::clicked, commander, &ModeCommander::changetoCW);
}

ModeButtons::~ModeButtons() {
	deleteLater();
}
