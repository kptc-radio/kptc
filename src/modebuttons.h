/***************************************************************************
						  modebuttons.h  -  description
							 -------------------
	begin                : Wed Mar 7 2001
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

#ifndef MODEBUTTONS_H
#define MODEBUTTONS_H

#include <qobject.h>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QPushButton>
#include "QtWidgets/QLayout"

/**buttons for changeing the mode
  *@author Lars Schnake
  */

class ModeButtons : public QObject  {

	Q_OBJECT

	public:
		QPushButton *pactorButton;
		QPushButton *amtorButton;
		QPushButton *rttyButton;
		QPushButton *psk31Button;
		QPushButton *cwButton;
		QButtonGroup *buttongroup;

		ModeButtons(QWidget *parent = 0);
		~ModeButtons();

};

#endif
