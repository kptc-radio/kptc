/***************************************************************************
						  rttyspeedwidget.h  -  description
							 -------------------
	begin                : Mon Mar 12 2001
	copyright            : (C) 2001 by Lars Schnake
	email                : mail@lars-schnake.de

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

#ifndef RTTYSPEEDWIDGET_H
#define RTTYSPEEDWIDGET_H

#include "modem.h"

#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLayout>


/**change baude rate for RTTY
  *@author Lars Schnake
  */

class RTTYSpeedWidget : public QDialog  {

		Q_OBJECT

public:
	QPushButton * okbutton;
	QSlider * slider;
	QLineEdit * lineedit;

	RTTYSpeedWidget(QWidget *parent=0);
	~RTTYSpeedWidget();

	private:
		int speed;

	private slots:
		void updateText(int);
		void updateSlider(const QString &);
		void change();
};

#endif
