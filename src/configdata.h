/***************************************************************************
						  configdata.h  -  description
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


#ifndef CONFIGDATA_H
#define CONFIGDATA_H

#include <unistd.h>
#include <sys/types.h>

#include <QSettings>
#include <QStringList>
#include <QtWidgets/QMessageBox>
#include <QRegExp>

/**read/write the config data from/to the config file
  *@author Lars Schnake
  */

class ConfigData {

	public:
		ConfigData();
		~ConfigData();
		QString	parseMacroText( QString );
	// function to read/write date to configuration file
		void setPort(const QString dev);
		QString getPort();
		void setEditPort(const QString dev);
		QString getEditPort();
		void setPortSpeed(const QString speed);
		QString getPortSpeed();
		void setCall(QString);
		QString getCall();
		void setSelCall(QString);
		QString getSelCall();
		void setCMsg(bool, QString);
		bool isCMsg();
		QString getCMsg();
		void setfirststart(bool);
		bool firststart();
		void setAwayMsg(bool, QString);
		bool isAwayMsg();
		QString getAwayMsg();
		void setCQPactor( QString );
		QString	getCQPactor();
		void setCQAmtor( QString );
		QString	getCQAmtor();
		void setCQRTTY( QString );
		QString	getCQRTTY();
		void setCQPSK31( QString );
		QString	getCQPSK31();
		void setCQCW( QString );
		QString	getCQCW();
		void setFixLabel( QString , QString  );
		QString getFixLabel( QString  );
		void setFixPath( QString , QString  );
		QString getFixPath( QString  ) ;
		void setName(  QString  );
		QString getName() ;
		void setQTH(  QString  );
		QString getQTH() ;
		void setLogoutPath( QString );
		QString getLogoutPath();
		void setLoginPath( QString );
		QString getLoginPath();
		void setLogoutScript(bool);
		bool isLogoutScript();
		void setLoginScript(bool);
		bool isLoginScript();

	private:
		enum class Group {PORT, PERSONAL, GENERAL, LOGOUT, CQTEXT, FIXTEXT, LOGIN};
		QSettings *settings;

		QString getGroupName(Group group);
		void setValue(Group group, QString key, QString value);
		QString getValue(Group group, QString key, QString defaultvalue);
		bool stringIsTrue (const QString &string);
		QString boolToString(bool value);

};

extern ConfigData configdata;

#endif
