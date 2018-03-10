/***************************************************************************
						  configdata.cpp  -  description
							 -------------------
	begin                : Wed Feb 14 2001
	copyright            : (C) 2001 by Lars Schnake
	email                : lschnak@suse.de

	Ported to Qt5 and restructured storage access by Sebastian Martin Dicke in 2017 (Sebastianmartindicke [@] gmx [.] de)
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "configdata.h"

ConfigData configdata;


ConfigData::ConfigData()
{
	this->settings = new QSettings("kptc", "kptc");
}

ConfigData::Triple ConfigData::getTripleByType(Types type) {
		switch (type) {
			case Types::PORTDEVICE:
				return Triple{Group::PORT, "PORTDEVICE_EDIT", "/dev/ttyS0"};
				break;
			case Types::PORTDEVICE_EDIT:
				return Triple{Group::PORT, "PORTDEVICE_EDIT", "/dev/ttyS11"};
				break;
			case Types::PORTSPEED:
				return Triple{Group::PORT, "PORTSPEED", "38400"};
				break;
			case Types::CALL:
				return Triple{Group::PERSONAL, "CALL", "DK0TUX"};
				break;
			case Types::SELCALL:
				return Triple{Group::PERSONAL, "SELCALL", "DTUX"};
				break;
			case Types::FIRSTSTART:
				return Triple{Group::GENERAL, "FIRSTSTART", "TRUE"};
				break;
			case Types::AWAYMSG:
				return Triple{Group::LOGOUT,"AWAYMSG", "Here is %MYCALL% from %MYQTH%,\n\ncurrently terminal is not active!\n\n73 de %MYNAME%."};
				break;
			case Types::USEAWAYMSG:
				return Triple{Group::LOGOUT, "USEAWAYMSG", "TRUE"};
				break;
			case Types::CMSG:
				return Triple{Group::PERSONAL, "CMSG", "Here is %MYCALL% from %MYQTH%!"};
				break;
			case Types::PACTOR:
				return Triple{Group::CQTEXT, "PACTOR", "CQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\npse K\n"};
				break;
			case Types::AMTOR:
				return Triple{Group::CQTEXT, "AMTOR", "CQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\npse K\n"};
				break;
			case Types::RTTY:
				return Triple{Group::CQTEXT, "RTTY", "ryryryryryry\nCQ CQ CQ de %MYSELCALL%\nCQ CQ CQ de %MYSELCALL%\npse K\n"};
				break;
			case Types::PSK31:
				return Triple{Group::CQTEXT, "PSK31", "CQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\npse K\n"};
				break;
			case Types::CW:
				return Triple{Group::CQTEXT, "CW", "cq cq cq de %MYCALL%\ncq cq cq de %MYCALL%\npse k\n"};
				break;
			case Types::PATH_LOGIN:
				return Triple{Group::LOGIN, "PATH", ""};
				break;
			case Types::PATH_LOGOUT:
				return  Triple{Group::LOGOUT, "PATH", ""};
				break;
			case Types::NAME:
				return Triple{Group::PERSONAL, "NAME", "Tux"};
				break;
			case Types::QTH:
				return Triple{Group::PERSONAL, "QTH", "Antarctic"};
				break;
			case Types::LOGOUTSCRIPT:
				return Triple{Group::LOGOUT, "LOGOUTSCRIPT", "FALSE	"};
				break;
			case Types::LOGINSCRIPT:
				return Triple{Group::LOGIN, "LOGINSCRIPT", "FALSE"};
				break;
			default:
				return Triple{Group::NO, "", ""};
		}
}

void ConfigData::setData(Types type, QString value) {
	Triple data = this->getTripleByType(type);
	if (value.isEmpty()) {
		value = data.defaultvalue;
	}
	this->setValue(data.group, data.name, value);
}

QString ConfigData::getData(Types type) {
	Triple data = this->getTripleByType(type);
	return this->getValue(data.group, data.name, data.defaultvalue);
}

QString ConfigData::getGroupName(Group group) const
{
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

QString ConfigData::boolToString(bool value) const {
	if (value) {
		return "TRUE";
	}
	return "FALSE";
}

void ConfigData::setValue(Group group, const QString &key ,const QString &value) {
	this->settings->beginGroup(this->getGroupName(group));
	this->settings->setValue(key, value);
	this->settings->endGroup();
}

QString ConfigData::getValue(Group group,const QString &key,const QString &defaultvalue) const {
	this->settings->beginGroup(this->getGroupName(group));
	QString data = (QString) this->settings->value(key, defaultvalue).toString();
	this->settings->endGroup();
	return data;
}

bool ConfigData::stringIsTrue(const QString &string) {
	return string == "TRUE";
}

ConfigData::~ConfigData() {
	this->settings->deleteLater();
}

QString ConfigData::parseMacroText(QString text) {
	text.replace(QRegExp("%MYCALL%"), configdata.getCall());
	text.replace(QRegExp("%MYSELCALL%"), configdata.getSelCall());
	text.replace(QRegExp("%MYQTH%"), configdata.getQTH());
	text.replace(QRegExp("%MYNAME%"), configdata.getName());
	return text;
}

// change and read keys

void ConfigData::setPort(const QString &dev) {
	if (dev != 0L) {
		this->setData(Types::PORTDEVICE, dev);
		//this->setValue(Group::PORT, "PORTDEVICE", dev);
	}
}

QString ConfigData::getPort() {
	return this->getData(Types::PORTDEVICE);
//	return this->getValue(Group::PORT, "PORTDEVICE", "/dev/ttyS0");
}

void ConfigData::setEditPort(const QString &dev) {
	this->setData(PORTDEVICE_EDIT, dev);
//	this->setValue(Group::PORT, "PORTDEVICE_EDIT", dev);
}

QString ConfigData::getEditPort() {
	return this->getData(PORTDEVICE_EDIT);
//	return this->getValue(Group::PORT, "PORTDEVICE_EDIT", "/dev/ttyS11");
}

void ConfigData::setPortSpeed(const QString &speed) {
	if (speed != 0L) {
		this->setData(PORTSPEED, speed);
		//this->setValue(Group::PORT, "PORTSPEED", speed);
	}
}

QString ConfigData::getPortSpeed() {
	return this->getData(PORTSPEED);
//	return this->getValue(Group::PORT, "PORTSPEED", "38400");
}

void ConfigData::setCall(const QString &call) {
	this->setData(CALL, call);
//	this->setValue(Group::PERSONAL, "CALL", call);
}


QString ConfigData::getCall() {
	return this->getData(CALL);
//	return this->getValue(Group::PERSONAL, "CALL", "DK0TUX");
}

void ConfigData::setSelCall(const QString &call) {
	this->setData(SELCALL, call);
//	this->setValue(Group::PERSONAL, "SELCALL", call);
}


QString ConfigData::getSelCall() {
	return this->getData(SELCALL);
//	return this->getValue(Group::PERSONAL, "SELCALL", "DTUX");
}


void ConfigData::setfirststart(bool  first) {
	if (!first) {
		this->setData(FIRSTSTART, "FALSE");
	}
//	if (!first) this->setValue(Group::GENERAL, "FIRSTSTART", "FALSE");
}

bool ConfigData::firststart() {
	QString s = this->getData(FIRSTSTART);
//	QString s=  this->getValue(Group::GENERAL, "FIRSTSTART", "TRUE");
	//qDebug () << "configdata::firststart() - entry:" << s;
	return this->stringIsTrue(s);
}

void ConfigData::setAwayMsg(bool set,const QString &msg) {
	this->setData(USEAWAYMSG, this->boolToString(set));
	this->setData(AWAYMSG, msg);
//	this->setValue(Group::LOGOUT, "USEAWAYMSG", this->boolToString(set));
//	this->setValue(Group::LOGOUT, "AWAYMSG", msg);
}

bool ConfigData::isAwayMsg() {
	QString string = this->getData(USEAWAYMSG);
//	QString s= this->getValue(Group::LOGOUT, "USEAWAYMSG", "TRUE");
	return this->stringIsTrue(string);
}

QString ConfigData::getAwayMsg() {
	return this->getData(AWAYMSG);
//	return this->getValue(Group::LOGOUT, "AWAYMSG", "Here is %MYCALL% from %MYQTH%,\n\ncurrently terminal is not active!\n\n73 de %MYNAME%.");
}
/////
void ConfigData::setCMsg(bool set,const QString &msg) {
	this->setData(USECMSG, this->boolToString(set));
	this->setData(CMSG, msg);
//	this->setValue(Group::PERSONAL, "USECMSG", this->boolToString(set));
//	this->setValue(Group::PERSONAL, "CMSG", msg);
}

bool ConfigData::isCMsg() {
	QString string = this->getData(USECMSG);
//	QString s= this->getValue(Group::PERSONAL, "USECMSG", "TRUE");
	return this->stringIsTrue(string);
}

QString ConfigData::getCMsg() {
	return this->getData(CMSG);
//	return this->getValue(Group::PERSONAL, "CMSG", "Here is %MYCALL% from %MYQTH%!");
}

void ConfigData::setCQPactor(const QString &qtext) {
	this->setData(PACTOR, qtext);
//	this->setValue(Group::CQTEXT, "PACTOR", qtext);
}

QString ConfigData::getCQPactor() {
	return this->getData(PACTOR);
//	return this->getValue(Group::CQTEXT, "PACTOR", "CQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\npse K\n");
}

void ConfigData::setCQAmtor(const QString &qtext) {
	this->setData(AMTOR, qtext);
	this->setValue(Group::CQTEXT, "AMTOR", qtext);
}

QString ConfigData::getCQAmtor() {
	return this->getData(AMTOR);
//	return this->getValue(Group::CQTEXT, "AMTOR", "CQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\npse K\n");
}

void ConfigData::setCQRTTY(const QString &qtext) {
	this->setData(RTTY, qtext);
//	this->setValue(Group::CQTEXT, "RTTY", qtext);
}

QString ConfigData::getCQRTTY() {
	return this->getData(RTTY);
//	return this->getValue(Group::CQTEXT, "RTTY", "ryryryryryry\nCQ CQ CQ de %MYSELCALL%\nCQ CQ CQ de %MYSELCALL%\npse K\n");
}
void ConfigData::setCQPSK31(const QString &qtext) {
	this->setData(PSK31, qtext);
//	this->setValue(Group::CQTEXT, "PSK31", qtext);
}

QString ConfigData::getCQPSK31() {
	return this->getData(PSK31);
//	return this->getValue(Group::CQTEXT, "PSK31", "CQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\npse K\n");
}

void ConfigData::setCQCW(const QString &qtext) {
	this->setData(CW, qtext);
//	this->setValue(Group::CQTEXT, "CW", qtext);
}

QString ConfigData::getCQCW() {
	return this->getData(CW);
//	return this->getValue(Group::CQTEXT, "CW", "cq cq cq de %MYCALL%\ncq cq cq de %MYCALL%\npse k\n");
}

void ConfigData::setFixLabel(const QString &label,const QString &number) {
	this->setValue(Group::FIXTEXT, number, label);
}

QString ConfigData::getFixLabel (const QString &number) {
	return this->getValue(Group::FIXTEXT, number, "TEXT NR. " + number);
}

void ConfigData::setFixPath(const QString &path,const QString &number) {
	this->setValue(Group::FIXTEXT, number + "PATH", path);
}

QString ConfigData::getFixPath (const QString &number) {
	return this->getValue(Group::FIXTEXT, number + "PATH", "");
//	return this->getValue(Group::FIXTEXT, number + "PATH", "");
}

void ConfigData::setName(const QString &name) {
	this->setData(NAME, name);
//	this->setValue(Group::PERSONAL, "NAME", name);
}

QString ConfigData::getName() {
	return this->getData(NAME);
//	return this->getValue(Group::PERSONAL, "NAME", "Tux");
}

void ConfigData::setQTH(const QString &name) {
	this->setData(QTH, name);
//	this->setValue(Group::PERSONAL, "QTH", name);
}

QString ConfigData::getQTH() {
	return this->getData(QTH);
//	return this->getValue(Group::PERSONAL, "QTH", "Antarctic");
}

void ConfigData::setLogoutPath(const QString &path) {
	this->setData(PATH_LOGOUT, path);
//	this->setValue(Group::LOGOUT, "PATH", path);
}

QString ConfigData::getLogoutPath () {
	return this->getData(PATH_LOGOUT);
//	return this->getValue(Group::LOGOUT, "PATH", "");
}


void ConfigData::setLoginPath(const QString &path) {
	this->setData(PATH_LOGIN, path);
//	this->setValue(Group::LOGIN, "PATH", path);
}

QString ConfigData::getLoginPath () {
	return this->getData(PATH_LOGIN);
//	return this->getValue(Group::LOGIN, "PATH", "");
}

void ConfigData::setLogoutScript(bool set) {
	QString qset = this->boolToString(set);
	this->setData(LOGOUTSCRIPT, qset);
//	this->setValue(Group::LOGOUT, "LOGOUTSCRIPT", qset);
}

bool ConfigData::isLogoutScript() {
	QString temp = this->getData(LOGOUTSCRIPT);
	return this->stringIsTrue(temp);
}

void ConfigData::setLoginScript(bool set) {
	QString qset = this->boolToString(set);
	this->setData(LOGINSCRIPT, qset);
//	this->setValue(Group::LOGIN, "LOGINSCRIPT", qset);
}

bool ConfigData::isLoginScript() {
	QString temp = this->getData(LOGINSCRIPT);
	return this->stringIsTrue(temp);
}
