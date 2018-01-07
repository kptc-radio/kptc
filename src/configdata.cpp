/***************************************************************************
						  configdata.cpp  -  description
							 -------------------
	begin                : Wed Feb 14 2001
	copyright            : (C) 2001 by Lars Schnake
	email                : lschnak@suse.de

	Ported to Qt5 and restructured storage access by Sebastian Martin Dicke in 2017 (Sebastianmartindicke [@] gmx [.] de )
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <iostream>
#include "configdata.h"

ConfigData configdata;

QString ConfigData::getGroupName(Group group) const
{std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	switch (group) {
		case  Group::PORT:
			return "PORT";
		case Group::PERSONAL:
			return "PERSONAL";
		case Group::GENERAL:
			return "GENERAL";
		case Group::LOGIN:
			return "LOGIN";
		case Group::LOGOUT:
			return "LOGOUT";
		case Group::CQTEXT:
			return "CQTEXT";
		default:
			return "";
	}
}

QString ConfigData::boolToString(bool value) const {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	if (value) {
		return "TRUE";
	}
	return "FALSE";
}

void ConfigData::setValue(Group group, QString key, QString value) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	this->settings->beginGroup(this->getGroupName(group));
	this->settings->setValue(key, value);
	this->settings->endGroup();
}

QString ConfigData::getValue(Group group, QString key, QString defaultvalue) const {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;this->settings->beginGroup(this->getGroupName(group));
	QString data = (QString) this->settings->value(key, defaultvalue).toString();
	this->settings->endGroup();
	return data;
}

bool ConfigData::stringIsTrue(const QString &string) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return string == "TRUE";
}

ConfigData::ConfigData()
{std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;

	std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	this->settings = new QSettings("kptc", "kptc");
}

ConfigData::~ConfigData() {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	this->settings->deleteLater();
}

QString ConfigData::parseMacroText( QString text ) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	text.replace( QRegExp("%MYCALL%"), configdata.getCall() );
	text.replace( QRegExp("%MYSELCALL%"), configdata.getSelCall() );
	text.replace( QRegExp("%MYQTH%"), configdata.getQTH() );
	text.replace( QRegExp("%MYNAME%"), configdata.getName() );
	return text;
}

// change and read keys

void ConfigData::setPort(const QString dev) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	if (dev != 0L) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		this->setValue(Group::PORT, "PORTDEVICE", dev);
	}
}

QString ConfigData::getPort() {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return this->getValue(Group::PORT, "PORTDEVICE", "/dev/ttyS0");
}

void ConfigData::setEditPort(const QString dev) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	this->setValue(Group::PORT, "PORTDEVICE_EDIT", dev);
}

QString ConfigData::getEditPort() {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return this->getValue(Group::PORT, "PORTDEVICE_EDIT", "/dev/ttyS11");
}

void ConfigData::setPortSpeed(const QString speed) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	if (speed != 0L){std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		this->setValue(Group::PORT, "PORTSPEED", speed);
	}
}

QString ConfigData::getPortSpeed() {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return this->getValue(Group::PORT,  "PORTSPEED", "38400");
}

void ConfigData::setCall( QString call ){std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	this->setValue(Group::PERSONAL, "CALL", call);
}


QString ConfigData::getCall(){std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return this->getValue(Group::PERSONAL,  "CALL", "DK0TUX");
}

void ConfigData::setSelCall( QString call ){std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	this->setValue(Group::PERSONAL, "SELCALL", call);
}


QString ConfigData::getSelCall(){std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return this->getValue(Group::PERSONAL, "SELCALL", "DTUX");
}


void ConfigData::setfirststart( bool  first) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	if (!first) this->setValue(Group::GENERAL, "FIRSTSTART", "FALSE");
}

bool ConfigData::firststart() {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	QString s=  this->getValue(Group::GENERAL, "FIRSTSTART", "TRUE");
	//qDebug () << "configdata::firststart() - entry:" << s;
	return this->stringIsTrue(s);
}

void ConfigData::setAwayMsg(bool set, QString msg) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	this->setValue(Group::LOGOUT, "USEAWAYMSG", this->boolToString(set));
	this->setValue(Group::LOGOUT, "AWAYMSG", msg);
}

bool ConfigData::isAwayMsg() {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	QString s= this->getValue(Group::LOGOUT, "USEAWAYMSG", "TRUE");
	return this->stringIsTrue(s);
}

QString ConfigData::getAwayMsg() {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return this->getValue(Group::LOGOUT, "AWAYMSG", "Here is %MYCALL% from %MYQTH%,\n\ncurrently terminal is not active!\n\n73 de %MYNAME%.");
}
/////
void ConfigData::setCMsg(bool set, QString msg) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	this->setValue(Group::PERSONAL, "USECMSG", this->boolToString(set));
	this->setValue(Group::PERSONAL, "CMSG", msg);
}

bool ConfigData::isCMsg() {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	QString s= this->getValue(Group::PERSONAL, "USECMSG", "TRUE");
	return this->stringIsTrue(s);
}

QString ConfigData::getCMsg() {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return this->getValue(Group::PERSONAL, "CMSG", "Here is %MYCALL% from %MYQTH% !");
}

void ConfigData::setCQPactor( QString qtext ) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	this->setValue(Group::CQTEXT, "PACTOR", qtext );
}

QString ConfigData::getCQPactor() {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return this->getValue(Group::CQTEXT, "PACTOR", "CQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\npse K\n");
}

void ConfigData::setCQAmtor( QString qtext ) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	this->setValue(Group::CQTEXT, "AMTOR", qtext);
}

QString ConfigData::getCQAmtor() {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return this->getValue(Group::CQTEXT, "AMTOR", "CQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\npse K\n");
}

void ConfigData::setCQRTTY( QString qtext ) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	this->setValue(Group::CQTEXT, "RTTY", qtext );
}

QString ConfigData::getCQRTTY() {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return this->getValue(Group::CQTEXT, "RTTY", "ryryryryryry\nCQ CQ CQ de %MYSELCALL%\nCQ CQ CQ de %MYSELCALL%\npse K\n");
}
void ConfigData::setCQPSK31( QString qtext ) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	this->setValue(Group::CQTEXT, "PSK31", qtext );
}

QString ConfigData::getCQPSK31() {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return this->getValue(Group::CQTEXT, "PSK31", "CQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\npse K\n");
}

void ConfigData::setCQCW( QString qtext ) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	this->setValue(Group::CQTEXT, "CW", qtext );
}

QString ConfigData::getCQCW() {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return this->getValue(Group::CQTEXT, "CW", "cq cq cq de %MYCALL%\ncq cq cq de %MYCALL%\npse k\n");
}

void ConfigData::setFixLabel( QString label, QString number) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	this->setValue(Group::FIXTEXT, number, label);
}

QString ConfigData::getFixLabel ( QString number ) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return this->getValue(Group::FIXTEXT, number, "TEXT NR. " + number );
}

void ConfigData::setFixPath( QString path, QString number) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	this->setValue(Group::FIXTEXT, number + "PATH" , path );
}

QString ConfigData::getFixPath ( QString number ) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return this->getValue(Group::FIXTEXT, number + "PATH" , "");
}

void ConfigData::setName( QString name ) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	this->setValue(Group::PERSONAL, "NAME", name);
}

QString ConfigData::getName(){std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return this->getValue(Group::PERSONAL, "NAME", "Tux");
}

void ConfigData::setQTH( QString name ) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	this->setValue(Group::PERSONAL, "QTH", name);
}

QString ConfigData::getQTH(){std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return this->getValue(Group::PERSONAL, "QTH", "Antarctic");
}

void ConfigData::setLogoutPath( QString path) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	this->setValue(Group::LOGOUT, "PATH" , path);
}

QString ConfigData::getLogoutPath () {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return this->getValue(Group::LOGOUT, "PATH" , "" );
}


void ConfigData::setLoginPath( QString path) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	this->setValue(Group::LOGIN,  "PATH" , path);
}

QString ConfigData::getLoginPath () {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return this->getValue(Group::LOGIN, "PATH" , "" );
}

void ConfigData::setLogoutScript( bool set ) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	QString qset = this->boolToString(set);
	this->setValue(Group::LOGOUT,  "LOGOUTSCRIPT" , qset);
}

bool ConfigData::isLogoutScript() {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	QString s = this->getValue(Group::LOGOUT, "LOGOUTSCRIPT" , "FALSE" );
	return this->stringIsTrue(s);
}

void ConfigData::setLoginScript( bool set ) {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	QString qset = this->boolToString(set);
	this->setValue(Group::LOGIN,  "LOGINSCRIPT" , qset);
}

bool ConfigData::isLoginScript() {std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	QString s = this->getValue(Group::LOGIN, "LOGINSCRIPT" , "FALSE" );
	return this->stringIsTrue(s);
}
