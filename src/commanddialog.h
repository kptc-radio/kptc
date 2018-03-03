/***************************************************************************
						  commanddialog.h  -  description
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


#ifndef COMMANDDIALOG_H
#define COMMANDDIALOG_H

#include "modem.h"

#include <QtWidgets/QDialog>
#include <QtWidgets/QComboBox>
#include <QtGui/QKeyEvent>

/**
  *@author Lars Schnake
  */

class CommandDialog : public QDialog  {
public:
	CommandDialog(QWidget *parent = 0);
	~CommandDialog();

private:
	QComboBox *combo;
	void keyPressEvent(QKeyEvent *) ;

};
#endif
