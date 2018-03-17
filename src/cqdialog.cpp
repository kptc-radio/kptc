/***************************************************************************
						  cqdialog.cpp  -  description
							 -------------------
	begin                : Mon Feb 26 2001
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

#include "cqdialog.h"

CQDialog::CQDialog(QWidget *parent, ModeCommander  *_modecommander) : QObject(parent) {
	modecommander = _modecommander;
	tabDialog = new QTabWidget();
	tabDialog->setWindowTitle("CQ");

	//TODO implement buttons in the widget

	tabDialog->addTab(new QPushButton(tabDialog), tr("CALL C&Q")); //okay
	tabDialog->addTab(new QPushButton(tabDialog), tr("CALL C&Q")); //apply
	tabDialog->addTab(new QPushButton(tabDialog), tr("&Save")); //save
	tabDialog->addTab(new QPushButton(tabDialog), tr("&Cancel")); //cancel

	cqtext_Pactor = new QTextEdit("Pactor-text", tabDialog);
	CQText_Amtor = new QTextEdit("Amtor-text", tabDialog);
	CQText_RTTY = new QTextEdit("RTTY-text", tabDialog);
	CQText_PSK31 = new QTextEdit("PSK31-text", tabDialog);
	CQText_CW = new QTextEdit("CW-text", tabDialog);

	tabDialog->addTab(cqtext_Pactor, "Pactor");
	tabDialog->addTab(CQText_Amtor, "Amtor");
	tabDialog->addTab(CQText_RTTY, "RTTY");
	tabDialog->addTab(CQText_PSK31, "PSK31");
	tabDialog->addTab(CQText_CW, "CW");

   //TODO
	connect (static_cast<QPushButton*>(tabDialog->widget(0)), &QPushButton::clicked, this, &CQDialog::startCall);
	connect (static_cast<QPushButton*>(tabDialog->widget(2)), &QPushButton::clicked, this, &CQDialog::saveText);
	connect (tabDialog, &QTabWidget::tabBarClicked, this, &CQDialog::selectTab);
}

void CQDialog::openDialog() {
	cqtext_Pactor->setText(configdata.getCQPactor());
	CQText_Amtor->setText(configdata.getCQAmtor());
	CQText_RTTY->setText(configdata.getCQRTTY());
	CQText_PSK31->setText(configdata.getCQPSK31());
	CQText_CW->setText(configdata.getCQCW());

	//TODO
	QString mod;
	if (modecommander != 0) {
		mod = modecommander->currendmod();
	}
	if (mod == "cmd:") {
		tabDialog->setCurrentWidget(cqtext_Pactor);
	}
	else if (mod == "AMTOR") {
		tabDialog->setCurrentWidget(CQText_Amtor);
	}
	else if (mod == "AMTOR-MONITOR") {
		tabDialog->setCurrentWidget(CQText_Amtor);
	}
	else if (mod == "RTTY") {
		tabDialog->setCurrentWidget(CQText_RTTY);
	}
	else if (mod == "PSK31") {
		tabDialog->setCurrentWidget(CQText_PSK31);
	}
	else if (mod == "CW") {
		tabDialog->setCurrentWidget(CQText_CW);
	}
	tabDialog->show();
}


void CQDialog::saveText() {
	configdata.setCQPactor(cqtext_Pactor->toPlainText());
	configdata.setCQAmtor(CQText_Amtor->toPlainText());
	configdata.setCQRTTY(CQText_RTTY->toPlainText());
	configdata.setCQPSK31(CQText_PSK31->toPlainText());
	configdata.setCQCW(CQText_CW->toPlainText());
}

void CQDialog::selectTab(int Tab) {
	currenttab = static_cast<Tabs>(Tab);
	qDebug()<<"CQDialog-currenttab: "<< Tab ;
}

void CQDialog::startCall() {
	////qDebug() <<"CQDialog:startCall";

	QString string;
	if (currenttab == Tabs::Pactor) {//"Pactor"
		modecommander->changetoPactor();
		modecommander->Unproto();
		string = string = this->processString(cqtext_Pactor);
		Modem::modem->writeString(string);
		modecommander->QRT();
	}
	else if (currenttab == Tabs::Amtor) {//"Amtor"
		modecommander->changetoAmtor();
		modecommander->FEC();
		string = string = this->processString(CQText_Amtor);
		Modem::modem->writeString(string);
		modecommander->QRT();
	}
	else if (currenttab == Tabs::RTTY) {//"RTTY"
		modecommander->changetoRTTY();
		modecommander->Changeover();
		string = this->processString(CQText_RTTY);
		Modem::modem->writeString(string);
		modecommander->QRT();
	}
	else if (currenttab == Tabs::PSK31) {//"PSK31"
		modecommander->changetoPSK31();
		modecommander->Changeover();
		string = this->processString(CQText_PSK31);
		Modem::modem->writeString(string);
		modecommander->QRT();
	}
	else if (currenttab == Tabs::CW) {//"CW"
		modecommander->changetoCW();
		string = this->processString(CQText_CW);
		sleep(1);
		Modem::modem->writeString(string);
	}

}

QString CQDialog::processString(QTextEdit *edit) {
	QString string = edit->toPlainText();
	string.replace(QRegExp("\n"), "\r");
	return configdata.parseMacroText(string);
}

CQDialog::~CQDialog() {}
