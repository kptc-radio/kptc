/***************************************************************************
						  cqdialog.cpp  -  description
							 -------------------
	begin                : Mon Feb 26 2001
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

#include "cqdialog.h"
#include <iostream>

CQDialog::CQDialog( QWidget *parent, ModeCommander  *_modecommander) : QObject( parent) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	modecommander = _modecommander;
	TabDialog = new QTabWidget();
	TabDialog->setWindowTitle("CQ");

	//TODO implement buttons in the widget

//    TabDialog->setOkButton( tr("CALL C&Q"));
//    //TabDialog->setApplyButton( tr("CALL C&Q"));
//    TabDialog->setDefaultButton( tr("&Save"));
//    TabDialog->setCancelButton( tr("&Cancel"));

	CQText_Pactor = new QTextEdit("Pactor-text", TabDialog);
	CQText_Amtor = new QTextEdit("Amtor-text", TabDialog);
	CQText_RTTY = new QTextEdit("RTTY-text", TabDialog);
	CQText_PSK31 = new QTextEdit("PSK31-text", TabDialog);
	CQText_CW = new QTextEdit("CW-text", TabDialog);

	TabDialog->addTab( CQText_Pactor, "Pactor");
	TabDialog->addTab( CQText_Amtor, "Amtor");
	TabDialog->addTab( CQText_RTTY, "RTTY");
	TabDialog->addTab( CQText_PSK31, "PSK31");
	TabDialog->addTab( CQText_CW, "CW");

   //TODO
  //connect ( TabDialog, SIGNAL ( applyButtonPressed () ), this, SLOT (startCall()) );
  //connect ( TabDialog, SIGNAL ( defaultButtonPressed () ), this, SLOT (saveText()) );
	connect ( TabDialog, SIGNAL ( tabBarClicked(int)  ), this, SLOT (selectTab(int)) );
}

void CQDialog::openDialog(  ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	CQText_Pactor->setText(configdata.getCQPactor());
	CQText_Amtor->setText(configdata.getCQAmtor());
	CQText_RTTY->setText(configdata.getCQRTTY());
	CQText_PSK31->setText(configdata.getCQPSK31());
	CQText_CW->setText(configdata.getCQCW());

	//TODO
	QString mod;
	if (modecommander != 0) mod = modecommander->currendmod();
	if (mod == "cmd:") TabDialog->setCurrentWidget(CQText_Pactor);
	else if (mod == "AMTOR") TabDialog->setCurrentWidget(CQText_Amtor);
	else if (mod == "AMTOR-MONITOR") TabDialog->setCurrentWidget(CQText_Amtor);
	else if (mod == "RTTY") TabDialog->setCurrentWidget(CQText_RTTY);
	else if (mod == "PSK31") TabDialog->setCurrentWidget(CQText_PSK31);
	else if (mod == "CW") TabDialog->setCurrentWidget(CQText_CW);
	TabDialog->show();
}


void CQDialog::saveText() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	configdata.setCQPactor( CQText_Pactor->toPlainText());
	configdata.setCQAmtor( CQText_Amtor->toPlainText());
	configdata.setCQRTTY( CQText_RTTY->toPlainText());
	configdata.setCQPSK31( CQText_PSK31->toPlainText());
	configdata.setCQCW( CQText_CW->toPlainText());

}

void CQDialog::selectTab(int Tab) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	currenttab = static_cast<Tabs>(Tab);
	//qDebug ()<<"CQDialog-currenttab: "<< Tab ;
}

void CQDialog::startCall() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	////qDebug() <<"CQDialog:startCall";

	QString qs;
	if ( currenttab == Tabs::Pactor) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;//"Pactor"
		modecommander->changetoPactor();
		modecommander->Unproto();
		qs = qs = this->processString(CQText_Pactor);
		Modem::modem->writeLine(qs);
		modecommander->QRT();
	}
	else if ( currenttab == Tabs::Amtor) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;//"Amtor"
		modecommander->changetoAmtor();
		modecommander->FEC();
		qs = qs = this->processString(CQText_Amtor);
		Modem::modem->writeLine(qs);
		modecommander->QRT();
	}
	else if ( currenttab == Tabs::RTTY ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;//"RTTY"
		modecommander->changetoRTTY();
		modecommander->Changeover();
		qs = this->processString(CQText_RTTY);
		Modem::modem->writeLine(qs);
		modecommander->QRT();
	}
	else if ( currenttab == Tabs::PSK31 ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;//"PSK31"
		modecommander->changetoPSK31();
		modecommander->Changeover();
		qs = this->processString(CQText_PSK31);
		Modem::modem->writeLine(qs);
		modecommander->QRT();
	}
	else if ( currenttab == Tabs::CW ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;//"CW"
		modecommander->changetoCW();
		qs = this->processString(CQText_CW);
		sleep(1);
		Modem::modem->writeLine(qs);
	}

}

QString CQDialog::processString(QTextEdit *edit) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	QString string = edit->toPlainText();
	string.replace( QRegExp("\n"), "\r" );
	return configdata.parseMacroText(string);
}

CQDialog::~CQDialog() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;}
