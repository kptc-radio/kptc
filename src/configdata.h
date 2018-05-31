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

#include <unordered_map>
#include <utility>
#include <iostream>

#include <QSettings>
#include <QRegExp>
#include <QHash>

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
		void setPortSpeed(const int &speed);
		int getPortSpeed();
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
		enum class Group;
		struct Triple {
				Triple(Group group = Group::NO, QVariant name = "", QVariant defaultvalue = "") :
				group{group}, name{name}, defaultvalue{defaultvalue} {}
			Group group;
			QVariant name;
			QVariant defaultvalue;
		};
		enum class Group {PORT, PERSONAL, GENERAL, LOGOUT, CQTEXT, FIXTEXT, LOGIN, NO};
		enum class Types {PORTDEVICE, PORTDEVICE_EDIT, PORTSPEED, CALL,
							 SELCALL, FIRSTSTART, USEAWAYMSG, AWAYMSG,
							 USECMSG, CMSG, PACTOR, TEXT, AMTOR, RTTY,
							 PSK31, CW, TEXT_NR, PATH_LOGIN, PATH_LOGOUT, NAME, QTH,
							 LOGOUTSCRIPT, LOGINSCRIPT};

		QSettings *settings;
		std::unordered_map<Types, Triple> triples = {
		std::make_pair(Types::PORTDEVICE, Triple(Group::PORT, QString("PORTDEVICE_EDIT"), QString("/dev/ttyS0"))),
		std::make_pair(Types::PORTDEVICE_EDIT, Triple(Group::PORT, "PORTDEVICE_EDIT", "/dev/ttyS11")),
		std::make_pair(Types::PORTSPEED, Triple(Group::PORT, "PORTSPEED", "38400")),
		std::make_pair(Types::CALL, Triple(Group::PERSONAL, "CALL", "DK0TUX")),
		std::make_pair(Types::SELCALL, Triple(Group::PERSONAL, "SELCALL", "DTUX")),
		std::make_pair(Types::FIRSTSTART, Triple(Group::GENERAL, "FIRSTSTART", "TRUE")),
		std::make_pair(Types::AWAYMSG, Triple(Group::LOGOUT,"AWAYMSG", "Here is %MYCALL% from %MYQTH%,\n\ncurrently terminal is not active!\n\n73 de %MYNAME%.")),
		std::make_pair(Types::USEAWAYMSG, Triple(Group::LOGOUT, "USEAWAYMSG", "TRUE")),
		std::make_pair(Types::CMSG, Triple(Group::PERSONAL, "CMSG", "Here is %MYCALL% from %MYQTH%!")),
		std::make_pair(Types::PACTOR, Triple(Group::CQTEXT, "PACTOR", "CQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\npse K\n")),
		std::make_pair(Types::AMTOR, Triple(Group::CQTEXT, "AMTOR", "CQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\npse K\n")),
		std::make_pair(Types::RTTY, Triple(Group::CQTEXT, "RTTY", "ryryryryryry\nCQ CQ CQ de %MYSELCALL%\nCQ CQ CQ de %MYSELCALL%\npse K\n")),
		std::make_pair(Types::PSK31, Triple(Group::CQTEXT, "PSK31", "CQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\npse K\n")),
		std::make_pair(Types::CW, Triple(Group::CQTEXT, "PSK31", "CQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\npse K\n")),
		std::make_pair(Types::PATH_LOGIN, Triple(Group::LOGIN, "PATH", "")),
		std::make_pair(Types::PATH_LOGOUT, Triple(Group::LOGOUT, "PATH", "")),
		std::make_pair(Types::NAME, Triple(Group::PERSONAL, "NAME", "Tux")),
		std::make_pair(Types::QTH, Triple(Group::PERSONAL, "QTH", "Antarctic")),
		std::make_pair(Types::LOGOUTSCRIPT, Triple(Group::LOGOUT, "LOGOUTSCRIPT", "FALSE	")),
		std::make_pair(Types::LOGINSCRIPT, Triple(Group::LOGIN, "LOGINSCRIPT", "FALSE"))
		};
		std::unordered_map<Group, QString> groups = {
		std::make_pair(Group::PORT, "PORT"),
		std::make_pair(Group::PERSONAL, "PERSONAL"),
		std::make_pair(Group::GENERAL, "GENERAL"),
		std::make_pair(Group::LOGIN, "LOGIN"),
		std::make_pair(Group::LOGOUT, "LOGOUT"),
		std::make_pair(Group::CQTEXT, "CQTEXT"),
		};

		QString getGroupName(Group group);
		void setValue(Group group, const QString &key, const QVariant &value);
		QVariant getValue(Group group, const QVariant &key, const QVariant &defaultvalue);
		void setData(Types type, QVariant value = "");
		QVariant getData(Types type);
		Triple getTripleByType(Types type);
};

extern ConfigData configdata;

#endif
