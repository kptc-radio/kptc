/***************************************************************************
						  configdata.h  -  description
							 -------------------
	begin                : Wed Feb 14 2001
	copyright            : (C) 2001 by Lars Schnake
	email                : lschnak@suse.de

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

#ifndef CONFIGDATA_H
#define CONFIGDATA_H

#include <QSettings>
#include <QRegExp>

/**read/write the config data from/to the config file
  *@author Lars Schnake
  */

class ConfigData {

	public:
		ConfigData();
		~ConfigData();
		QString parseMacroText(QString);
	// function to read/write date to configuration file
		void setPort(const QString &dev);
		QString getPort();
		void setEditPort(const QString &dev);
		QString getEditPort();
		void setPortSpeed(const QString &speed);
		QString getPortSpeed();
		void setCall(const QString&);
		QString getCall();
		void setSelCall(const QString&);
		QString getSelCall();
		void setCMsg(bool, const QString&);
		bool isCMsg();
		QString getCMsg();
		void setfirststart(bool);
		bool firststart();
		void setAwayMsg(bool, const QString&);
		bool isAwayMsg();
		QString getAwayMsg();
		void setCQPactor(const QString &);
		QString	getCQPactor();
		void setCQAmtor(const QString &);
		QString getCQAmtor();
		void setCQRTTY(const QString &);
		QString getCQRTTY();
		void setCQPSK31(const QString &);
		QString getCQPSK31();
		void setCQCW(const QString &);
		QString getCQCW();
		void setFixLabel(const QString&, const QString&);
		QString getFixLabel(const QString&);
		void setFixPath(const QString&, const QString&);
		QString getFixPath(const QString&) ;
		void setName(const QString &);
		QString getName();
		void setQTH(const QString &);
		QString getQTH() ;
		void setLogoutPath(const QString &);
		QString getLogoutPath();
		void setLoginPath(const QString &);
		QString getLoginPath();
		void setLogoutScript(bool);
		bool isLogoutScript();
		void setLoginScript(bool);
		bool isLoginScript();

	private:
		enum class Group {PORT, PERSONAL, GENERAL, LOGOUT, CQTEXT, FIXTEXT, LOGIN};
		QSettings *settings;

		QString getGroupName(Group group) const;
		void setValue(Group group, const QString &key, const QString &value);
		QString getValue(Group group, const QString &key, const QString &defaultvalue) const;
		bool stringIsTrue (const QString &string);
		QString boolToString(bool value) const;
};

extern ConfigData configdata;

#endif
