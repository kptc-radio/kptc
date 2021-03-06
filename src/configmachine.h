/***************************************************************************
						  configmachine.h  -  description
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

#ifndef CONFIGMACHINE_H
#define CONFIGMACHINE_H

#include "modem.h"
#include "configdata.h"

#include <QPair>
#include <QRegExp>
#include <QDebug>

/**
  *@author Lars Schnake
  * setting up the system with configdata
  *
  */

class ConfigMachine {

	public:
		typedef QPair<bool, QString> Pair;

		ConfigMachine();
		~ConfigMachine();
		void doconfig();
		Pair logout();
		Pair login();

};

#endif
