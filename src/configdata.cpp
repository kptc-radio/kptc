/***************************************************************************
						  configdata.cpp  -  description
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


#include "configdata.h"

ConfigData configdata;

QString ConfigData::getGroupName(Group group)
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

void ConfigData::setValue(Group group, QString key, QString value) {
	this->settings->beginGroup(this->getGroupName(group));
	this->settings->setValue(key, value);
	this->settings->endGroup();
}

QString ConfigData::getValue(Group group, QString key, QString defaultvalue) {
	this->settings->beginGroup(this->getGroupName(group));
	QString data = (QString) this->settings->value(key, defaultvalue).toString();
	this->settings->endGroup();
	return data;
}

ConfigData::ConfigData()
{
	this->settings = new QSettings("kptc", "kptc");
}

ConfigData::~ConfigData() {}

QString ConfigData::parseMacroText( QString text ) {
	text.replace( QRegExp("%MYCALL%"), configdata.getCall() );
	text.replace( QRegExp("%MYSELCALL%"), configdata.getSelCall() );
	text.replace( QRegExp("%MYQTH%"), configdata.getQTH() );
	text.replace( QRegExp("%MYNAME%"), configdata.getName() );
	return text;
}

// change and read keys

void ConfigData::setPort(const QString dev) {
	if (dev != 0L) {
		this->setValue(Group::PORT, "PORTDEVICE", dev);
	}
}

QString ConfigData::getPort() {
	return this->getValue(Group::PORT, "PORTDEVICE", "/dev/ttyS0");
}

void ConfigData::setEditPort(const QString dev) {
	this->setValue(Group::PORT, "PORTDEVICE_EDIT", dev);
}

QString ConfigData::getEditPort() {
	return this->getValue(Group::PORT, "PORTDEVICE_EDIT", "/dev/ttyS11");
}

void ConfigData::setPortSpeed(const QString speed) {
	if (speed != 0L){
		this->setValue(Group::PORT, "PORTSPEED", speed);
	}
}

QString ConfigData::getPortSpeed() {
	return this->getValue(Group::PORT,  "PORTSPEED", "38400");
}

void ConfigData::setCall( QString call ){
	this->setValue(Group::PERSONAL, "CALL", call);
}


QString ConfigData::getCall(){
	return this->getValue(Group::PERSONAL,  "CALL", "DK0TUX");
}

void ConfigData::setSelCall( QString call ){
 this->setValue(Group::PERSONAL, "SELCALL", call);
}


QString ConfigData::getSelCall(){
return this->getValue(Group::PERSONAL, "SELCALL", "DTUX");
}


void ConfigData::setfirststart( bool  first) {
	if (!first) this->setValue(Group::GENERAL, "FIRSTSTART", "FALSE");
}

bool ConfigData::firststart() {
	QString s=  this->getValue(Group::GENERAL, "FIRSTSTART", "TRUE");
	//qDebug () << "configdata::firststart() - entry:" << s;
	return ( s == "TRUE" );
}

void ConfigData::setAwayMsg(bool set, QString msg) {
	if (set) {
		this->setValue(Group::LOGOUT, "USEAWAYMSG", "TRUE");
	}
	else {
		this->setValue(Group::LOGOUT, "USEAWAYMSG", "TRUE");
	}
	this->setValue(Group::LOGOUT, "AWAYMSG", msg);
}

bool ConfigData::isAwayMsg() {
	QString s= this->getValue(Group::LOGOUT, "USEAWAYMSG", "TRUE");
	return (s == "TRUE");
}

QString ConfigData::getAwayMsg() {
	return this->getValue(Group::LOGOUT, "AWAYMSG", "Here is %MYCALL% from %MYQTH%,\n\ncurrently terminal is not active!\n\n73 de %MYNAME%.");
}
/////
void ConfigData::setCMsg(bool set, QString msg) {
	if (set) {
		this->setValue(Group::PERSONAL, "USECMSG", "TRUE");
	}
	else  {
		this->setValue(Group::PERSONAL, "USECMSG", "FALSE");
	}
	this->setValue(Group::PERSONAL, "CMSG", msg);
}

bool ConfigData::isCMsg() {
	QString s= this->getValue(Group::PERSONAL, "USECMSG", "TRUE");
	return  s == "TRUE";
}

QString ConfigData::getCMsg() {
	return this->getValue(Group::PERSONAL, "CMSG", "Here is %MYCALL% from %MYQTH% !");
}

void ConfigData::setCQPactor( QString qtext ) {
	this->setValue(Group::CQTEXT, "PACTOR", qtext );
}

QString ConfigData::getCQPactor() {
	return this->getValue(Group::CQTEXT, "PACTOR", "CQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\npse K\n");
}

void ConfigData::setCQAmtor( QString qtext ) {
   this->setValue(Group::CQTEXT, "AMTOR", qtext);
}

QString ConfigData::getCQAmtor() {
	return this->getValue(Group::CQTEXT, "AMTOR", "CQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\npse K\n");
}

void ConfigData::setCQRTTY( QString qtext ) {
	this->setValue(Group::CQTEXT, "RTTY", qtext );
}

QString ConfigData::getCQRTTY() {
	return this->getValue(Group::CQTEXT, "RTTY", "ryryryryryry\nCQ CQ CQ de %MYSELCALL%\nCQ CQ CQ de %MYSELCALL%\npse K\n");
}
void ConfigData::setCQPSK31( QString qtext ) {
	this->setValue(Group::CQTEXT, "PSK31", qtext );
}

QString ConfigData::getCQPSK31() {
	return this->getValue(Group::CQTEXT, "PSK31", "CQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\nCQ CQ CQ de %MYCALL%\npse K\n");
}

void ConfigData::setCQCW( QString qtext ) {
	this->setValue(Group::CQTEXT, "CW", qtext );
}

QString ConfigData::getCQCW() {
	return this->getValue(Group::CQTEXT, "CW", "cq cq cq de %MYCALL%\ncq cq cq de %MYCALL%\npse k\n");
}

void ConfigData::setFixLabel( QString label, QString number) {
	this->setValue(Group::FIXTEXT, number, label);
}

QString ConfigData::getFixLabel ( QString number ) {
	return this->getValue(Group::FIXTEXT, number, "TEXT NR. " + number );
}

void ConfigData::setFixPath( QString path, QString number) {
	this->setValue(Group::FIXTEXT, number + "PATH" , path );
}

QString ConfigData::getFixPath ( QString number ) {
	return this->getValue(Group::FIXTEXT, number + "PATH" , "");
}

void ConfigData::setName( QString name ) {
	this->setValue(Group::PERSONAL, "NAME", name);
}

QString ConfigData::getName(){
	return this->getValue(Group::PERSONAL, "NAME", "Tux");
}

void ConfigData::setQTH( QString name ) {
	this->setValue(Group::PERSONAL, "QTH", name);
}

QString ConfigData::getQTH(){
	return this->getValue(Group::PERSONAL, "QTH", "Antarctic");
}

void ConfigData::setLogoutPath( QString path) {
	this->setValue(Group::LOGOUT, "PATH" , path);
}

QString ConfigData::getLogoutPath () {
	return this->getValue(Group::LOGOUT, "PATH" , "" );
}


void ConfigData::setLoginPath( QString path) {
	this->setValue(Group::LOGIN,  "PATH" , path);
}

QString ConfigData::getLoginPath () {
	return this->getValue(Group::LOGIN, "PATH" , "" );
}

void ConfigData::setLogoutScript( bool set ) {
	QString qset;
	if (set) qset = "TRUE";
	else qset = "FALSE";
	this->setValue(Group::LOGOUT,  "LOGOUTSCRIPT" , qset);
}

bool ConfigData::isLogoutScript() {
	QString s = this->getValue(Group::LOGOUT, "LOGOUTSCRIPT" , "FALSE" );
	if ( s == "TRUE" ) return true;
	else return false;

}

void ConfigData::setLoginScript( bool set ) {
	QString qset;
	if (set) qset = "TRUE";
	else qset = "FALSE";
	this->setValue(Group::LOGIN,  "LOGINSCRIPT" , qset);
}

bool ConfigData::isLoginScript() {
	QString s = this->getValue(Group::LOGIN, "LOGINSCRIPT" , "FALSE" );
	if ( s == "TRUE" ) return true;
	else return false;

}
