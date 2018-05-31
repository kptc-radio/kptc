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
	return triples[type];
}

void ConfigData::setData(Types type, QVariant value) {
	Triple data = this->getTripleByType(type);
	if (value.toString() == "") {
		value = data.defaultvalue;
	}
	this->setValue(data.group, data.name.toString(), value);
}

QVariant ConfigData::getData(Types type) {
	Triple data = this->getTripleByType(type);
	return this->getValue(data.group, data.name, data.defaultvalue);
}

QString ConfigData::getGroupName(Group group)
{
	return groups[group];
}

void ConfigData::setValue(Group group, const QString &key, const QVariant &value) {
	this->settings->beginGroup(this->getGroupName(group));
	this->settings->setValue(key, value);
	this->settings->endGroup();
}

QVariant ConfigData::getValue(Group group, const QVariant &key, const QVariant &defaultvalue) {
	this->settings->beginGroup(this->getGroupName(group));
	QVariant data = this->settings->value(key.toString(), defaultvalue);
	this->settings->endGroup();
	return data;
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
	return this->getData(Types::PORTDEVICE).toString();
//	return this->getValue(Group::PORT, "PORTDEVICE", "/dev/ttyS0");
}

void ConfigData::setEditPort(const QString &dev) {
	this->setData(Types::PORTDEVICE_EDIT, dev);
//	this->setValue(Group::PORT, "PORTDEVICE_EDIT", dev);
}

QString ConfigData::getEditPort() {
	return this->getData(Types::PORTDEVICE_EDIT).toString();
//	return this->getValue(Group::PORT, "PORTDEVICE_EDIT", "/dev/ttyS11");
}

void ConfigData::setPortSpeed(const int &speed) {
	if (speed != 0L) {
		this->setData(Types::PORTSPEED, speed);
		//this->setValue(Group::PORT, "PORTSPEED", speed);
	}
}

int ConfigData::getPortSpeed() {
	return this->getData(Types::PORTSPEED).toInt();
//	return this->getValue(Group::PORT, "PORTSPEED", "38400");
}

void ConfigData::setCall(const QString &call) {
	this->setData(Types::CALL, call);
//	this->setValue(Group::PERSONAL, "CALL", call);
}


QString ConfigData::getCall() {
	return this->getData(Types::CALL).toString();
//	return this->getValue(Group::PERSONAL, "CALL", "DK0TUX");
}

void ConfigData::setSelCall(const QString &call) {
	this->setData(Types::SELCALL, call);
//	this->setValue(Group::PERSONAL, "SELCALL", call);
}


QString ConfigData::getSelCall() {
	return this->getData(Types::SELCALL).toString();
//	return this->getValue(Group::PERSONAL, "SELCALL", "DTUX");
}


void ConfigData::setfirststart(bool  first) {
	if (!first) {
		this->setData(Types::FIRSTSTART, false);
	}
//	if (!first) this->setValue(Group::GENERAL, "FIRSTSTART", "FALSE");
}

bool ConfigData::firststart() {
	return this->getData(Types::FIRSTSTART).toBool();
//	QString s=  this->getValue(Group::GENERAL, "FIRSTSTART", "TRUE");
	//qDebug() << "configdata::firststart() - entry:" << s;
}

void ConfigData::setAwayMsg(bool set, const QString &msg) {
	this->setData(Types::USEAWAYMSG, set);
	this->setData(Types::AWAYMSG, msg);
//	this->setValue(Group::LOGOUT, "USEAWAYMSG", this->boolToString(set));
//	this->setValue(Group::LOGOUT, "AWAYMSG", msg);
}

bool ConfigData::isAwayMsg() {
	return this->getData(Types::USEAWAYMSG).toBool();
//	QString s= this->getValue(Group::LOGOUT, "USEAWAYMSG", "TRUE");
}

QString ConfigData::getAwayMsg() {
	return this->getData(Types::AWAYMSG).toString();
//	return this->getValue(Group::LOGOUT, "AWAYMSG", "Here is %MYCALL% from %MYQTH%,\n\ncurrently terminal is not active!\n\n73 de %MYNAME%.");
}
/////
void ConfigData::setCMsg(bool set, const QString &msg) {
	this->setData(Types::USECMSG, set);
	this->setData(Types::CMSG, msg);
//	this->setValue(Group::PERSONAL, "USECMSG", this->boolToString(set));
//	this->setValue(Group::PERSONAL, "CMSG", msg);
}

bool ConfigData::isCMsg() {
	return this->getData(Types::USECMSG).toBool();
//	QString s= this->getValue(Group::PERSONAL, "USECMSG", "TRUE");
}

QString ConfigData::getCMsg() {
	return this->getData(Types::CMSG).toString();
//	return this->getValue(Group::PERSONAL, "CMSG", "Here is %MYCALL% from %MYQTH%!");
}

void ConfigData::setCQPactor(const QString &qtext) {
	this->setData(Types::PACTOR, qtext);
//	this->setValue(Group::CQTEXT, "PACTOR", qtext);
}

QString ConfigData::getCQPactor() {
	return this->getData(Types::PACTOR).toString();
//	return this->getValue(Group::CQTEXT, "PACTOR", "CQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\npse K\n");
}

void ConfigData::setCQAmtor(const QString &qtext) {
	this->setData(Types::AMTOR, qtext);
//	this->setValue(Group::CQTEXT, "AMTOR", qtext);
}

QString ConfigData::getCQAmtor() {
	return this->getData(Types::AMTOR).toString();
//	return this->getValue(Group::CQTEXT, "AMTOR", "CQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\npse K\n");
}

void ConfigData::setCQRTTY(const QString &qtext) {
	this->setData(Types::RTTY, qtext);
//	this->setValue(Group::CQTEXT, "RTTY", qtext);
}

QString ConfigData::getCQRTTY() {
	return this->getData(Types::RTTY).toString();
//	return this->getValue(Group::CQTEXT, "RTTY", "ryryryryryry\nCQ CQ CQ de %MYSELCALL%\nCQ CQ CQ de %MYSELCALL%\npse K\n");
}
void ConfigData::setCQPSK31(const QString &qtext) {
	this->setData(Types::PSK31, qtext);
//	this->setValue(Group::CQTEXT, "PSK31", qtext);
}

QString ConfigData::getCQPSK31() {
	return this->getData(Types::PSK31).toString();
//	return this->getValue(Group::CQTEXT, "PSK31", "CQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\npse K\n");
}

void ConfigData::setCQCW(const QString &qtext) {
	this->setData(Types::CW, qtext);
//	this->setValue(Group::CQTEXT, "CW", qtext);
}

QString ConfigData::getCQCW() {
	return this->getData(Types::CW).toString();
//	return this->getValue(Group::CQTEXT, "CW", "cq cq cq de %MYCALL%\ncq cq cq de %MYCALL%\npse k\n");
}

void ConfigData::setFixLabel(const QString &label, const QString &number) {
	this->setValue(Group::FIXTEXT, number, label);
}

QString ConfigData::getFixLabel (const QString &number) {
	return this->getValue(Group::FIXTEXT, number, "TEXT NR. " + number).toString();
}

void ConfigData::setFixPath(const QString &path, const QString &number) {
	this->setValue(Group::FIXTEXT, number + "PATH", path);
}

QString ConfigData::getFixPath (const QString &number) {
	return this->getValue(Group::FIXTEXT, number + "PATH", "").toString();
//	return this->getValue(Group::FIXTEXT, number + "PATH", "");
}

void ConfigData::setName(const QString &name) {
	this->setData(Types::NAME, name);
//	this->setValue(Group::PERSONAL, "NAME", name);
}

QString ConfigData::getName() {
	return this->getData(Types::NAME).toString();
//	return this->getValue(Group::PERSONAL, "NAME", "Tux");
}

void ConfigData::setQTH(const QString &name) {
	this->setData(Types::QTH, name);
//	this->setValue(Group::PERSONAL, "QTH", name);
}

QString ConfigData::getQTH() {
	return this->getData(Types::QTH).toString();
//	return this->getValue(Group::PERSONAL, "QTH", "Antarctic");
}

void ConfigData::setLogoutPath(const QString &path) {
	this->setData(Types::PATH_LOGOUT, path);
//	this->setValue(Group::LOGOUT, "PATH", path);
}

QString ConfigData::getLogoutPath() {
	return this->getData(Types::PATH_LOGOUT).toString();
//	return this->getValue(Group::LOGOUT, "PATH", "");
}


void ConfigData::setLoginPath(const QString &path) {
	this->setData(Types::PATH_LOGIN, path);
//	this->setValue(Group::LOGIN, "PATH", path);
}

QString ConfigData::getLoginPath() {
	return this->getData(Types::PATH_LOGIN).toString();
//	return this->getValue(Group::LOGIN, "PATH", "");
}

void ConfigData::setLogoutScript(bool set) {
	this->setData(Types::LOGOUTSCRIPT,  set);
//	this->setValue(Group::LOGOUT, "LOGOUTSCRIPT", qset);
}

bool ConfigData::isLogoutScript() {
	return this->getData(Types::LOGOUTSCRIPT).toBool();
}

void ConfigData::setLoginScript(bool set) {
	this->setData(Types::LOGINSCRIPT, set);
//	this->setValue(Group::LOGIN, "LOGINSCRIPT", qset);
}

bool ConfigData::isLoginScript() {
	return this->getData(Types::LOGINSCRIPT).toBool();
}
