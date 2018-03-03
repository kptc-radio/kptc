/***************************************************************************
						  fixtextpicker.h  -  description
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

#ifndef FIXTEXTPICKER_H
#define FIXTEXTPICKER_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>

/**Lineedits and button to setup a fixtext
  *@author Lars Schnake
  */

class FixTextPicker : public QWidget  {

	Q_OBJECT

	public:

		FixTextPicker(QWidget *parent = 0);
		~FixTextPicker();
		QPushButton *getChooseButtom();
		QLabel *getNumLabel();

	private:
		QPushButton *choosebutton;
		QLineEdit *editlabel;
		QLineEdit *editpath;
		QLabel *numlabel;

};

#endif
