/***************************************************************************
						  statusinfo.h  -  description
							 -------------------
	begin                : Mon Feb 19 2001
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

#ifndef STATUSINFO_H
#define STATUSINFO_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QWhatsThis>

#include "kled.h"

/**This Widget shows the status Info of the PTC-II
like "Stand by" or "over" and the command prompt Info
  *@author Lars Schnake
  */

class StatusInfo : public QWidget  {

		Q_OBJECT

	public:
		QLabel *prompt;
		QLabel *status;
		QLabel *mode;
		KLed *led;
		QLabel *send;
		QLabel *statusmessage;
		QLabel *call;

		StatusInfo(QWidget *parent=0);
		~StatusInfo();
};

#endif
