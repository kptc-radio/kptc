/***************************************************************************
		  kptc.cpp  -  description
			 -------------------
	begin				: Wed Feb 14 2001
	copyright			: (C) 2001 by Lars Schnake
	email				: mail@lars-schnake.de

	Ported to Qt5 by Sebastian Martin Dicke in 2017 (Sebastianmartindicke [@] gmx [.] de )
 ***************************************************************************/

/***************************************************************************
 *						 *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or	 *
 *   (at your option) any later version.				   *
 *						 *
 ***************************************************************************/

#include <iostream>

#include "kptc.h"

Kptc::Kptc(QWidget *parent) : QMainWindow()
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;

	setWindowTitle(tr("Kptc - the PTC-II for penguins "));
	// connect( kapp, SIGNAL(shutDown()), this, SLOT(shutdown()) );

	//TODO

	this->resize( 650, 450 );

	this->lefttoolbar = new QToolBar("", this);
	currentterm=1;
	bPromptInfoFollows = false;

	modecommander = new ModeCommander(this);
	cqdialog = new CQDialog(this,  modecommander);

		////////////////////////////////////////////////////////////////////////////

	this->initializePopUpMenues();

		/////////////////////////////////////////////////////////////////////////////
		// Toolbar for changing the mods:

		/*

	KToolBarRadioGroup *moderadiogroup = new KToolBarRadioGroup(modetoolbar,"modradiogroup");
	modetoolbar->setToggle(1);
	modetoolbar->setToggle(2);
	modetoolbar->setToggle(3);
	modetoolbar->setToggle(4);
	modetoolbar->setToggle(5);
	moderadiogroup->addButton(1);
	moderadiogroup->addButton(2);
	moderadiogroup->addButton(3);
	moderadiogroup->addButton(4);
	moderadiogroup->addButton(5);
	*/

	this->modetoolbar = new QToolBar(this);

	this->expandToolBar(" | Pactor | ", "changetoPactor", modecommander, modetoolbar);

	this->expandToolBar(" | Amtor |", "changetoAmtor", modecommander, modetoolbar);

	this->expandToolBar(" | RTTY | ", "changetoRTTY", modecommander, modetoolbar);

	this->expandToolBar(" | PSK31 | ", "changetoPSK31", modecommander, modetoolbar);

	this->expandToolBar(" | CW | ", "changetoCW", modecommander, modetoolbar);

	///////////////////////////////////////////////////////////////////////////////
	// toolbar on the left side

	lefttoolbar = new QToolBar(this);
	//lefttoolbar->setIconText(KToolBar::IconTextBottom);

	///////////////////////////////////////////////////////////////////////////////
	// statusbar

	this->initializeStatusBar();

//////////////////////////////////////////////////////////////////////////////
	// main window elements

	QSplitter *splitter = new QSplitter(this );
	splitter->setOrientation(Qt::Vertical);
	splitter->setOpaqueResize( true );
	setCentralWidget( splitter );

	// modebuttons in splitter window instead of modetoolbar ?! :
	// modebuttons = new ModeButtons(splitter);

//  termoutput = new MyTermout(splitter);
//	termoutput->setFont(QFont("courier",12,QFont::Normal));
	textedit = new MyLineEdit(splitter);
	textedit->notify(this, SLOT(sendchar(unsigned char)));

	splitter->show();

	textedit->setFocus();
	connect( textedit, SIGNAL(echoCommand(QString)), this, SLOT( echoText(QString)));
	connect( textedit, SIGNAL(sendit(QString)), this, SLOT(sendline(QString)));

	//////////////////////////////////////////////////////////////////

	cwspeedwidget = new CWSpeedWidget( 0 );
	rttyspeedwidget = new RTTYSpeedWidget( 0 );

	configmachine = new ConfigMachine(this);

	// Modem :
	(void)new Modem;
		//TODO possible memory leak
	bool bModemOk = false;

	//setAutoSaveSettings();

	if ( configdata.firststart() ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;

	QMessageBox::information( this, (QString(tr("Thank you for installing Kptc !\n")) +
	QString("This is a Ham Radio software for the SCS-PTC-II\nRemember: This sofware is still beta !\n\n") +
	QString("Connect the PTC to your computer\nand switch it on now.\nThen let´s continue with some configurations.\n") +
	QString("Have Fun ! 73 de Lars DL3YFC")), QString(tr("Welcome to Kptc !")));

	ConfigDialog configdialog ;
	if (configdialog.exec()== QDialog::Accepted) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		bModemOk = Modem::modem->opentty() ;
		Modem::modem->notify(this, SLOT(parseModemOut(unsigned char)));
		configdata.setfirststart( false );
		useconfigmachine();
		//lefttoolbar->setBarPos(KToolBar::Left);
		}
	}
	else { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;

	bModemOk = Modem::modem->opentty();
		Modem::modem->notify(this, SLOT(parseModemOut(unsigned char)));
		useconfigmachine();
		}
		configmachine->login();
	//qDebug() << Modem::modem->modemMessage();
	if ( ! bModemOk) QMessageBox::information( this,
		tr("Cannot open modem device !"), "Kptc" );
}

void Kptc::initializeToolBar() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	modetoolbar = new QToolBar(this);
	modebuttons = new ModeButtons(modetoolbar);

	modetoolbar->addWidget(modebuttons->pactorButton);
	modetoolbar->addWidget(modebuttons->amtorButton);
	modetoolbar->addWidget(modebuttons->rttyButton);
	modetoolbar->addWidget(modebuttons->psk31Button);
	modetoolbar->addWidget(modebuttons->cwButton);

	connect(modebuttons->pactorButton, SIGNAL(clicked(bool)), modecommander,SLOT(changetoPactor()));
	connect(modebuttons->amtorButton, SIGNAL(clicked(bool)), modecommander,SLOT(changetoAmtor()));
	connect(modebuttons->rttyButton, SIGNAL(clicked(bool)), modecommander,SLOT(changetoRTTY()));
	connect(modebuttons->psk31Button, SIGNAL(clicked(bool)), modecommander,SLOT(changetoPSK31()));
	connect(modebuttons->cwButton, SIGNAL(clicked(bool)), modecommander,SLOT(changetoCW()));
}

void Kptc::initializePopUpMenues() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	// Popup Menu:

	QMenu *filemenu = new QMenu();
	QAction *action = new QAction(tr("&Quit"), this);
	connect(action, SIGNAL(triggered(bool)), this, SLOT(fileQuit()));
	filemenu->addAction(action);

	QMenu *actionmenu = new QMenu ;

	QAction *reload = new QAction(tr( "Change&over"), actionmenu);
	reload->setShortcut(QKeySequence(Qt::CTRL, 'Y'));
	connect(reload, SIGNAL(triggered(bool)), this, SLOT(initchangeover()));
	actionmenu->addAction(reload);

	QAction *exit = new QAction(tr( "QR&T"), actionmenu);
	exit->setShortcut(QKeySequence(Qt::CTRL, 'D'));
	connect(exit, SIGNAL(triggered(bool)), this, SLOT(initQRT()));
	actionmenu->addAction(exit);

	QAction *stop = new QAction(tr( "&Stand by"), actionmenu);
	stop->setShortcut(QKeySequence(Qt::CTRL, 'S'));
	connect(stop, SIGNAL(triggered(bool)), this, SLOT(Standby()));
	actionmenu->addAction(stop);

	QAction *konqueror = new QAction(tr( "&CQ..."), actionmenu);
	konqueror->setShortcut(QKeySequence(Qt::CTRL, 'Q'));
	connect(konqueror, SIGNAL(triggered(bool)), this, SLOT(openDialog()));
	actionmenu->addAction(konqueror);

	QAction *command = new QAction(tr( "&Command..."), actionmenu);
	command->setShortcut(QKeySequence(Qt::CTRL, 19)); //Ctrl + Esc
	connect(command, SIGNAL(triggered(bool)), this, SLOT(openCommandDialog()));
	actionmenu->addAction(command);

	QMenu *optionmenu = new QMenu();
	QAction *configure = new QAction(tr( "&Config..." ), optionmenu);
	optionmenu->addAction(configure);
	connect(configure, SIGNAL(triggered(bool)), this, SLOT(openconfigdialog()));

	QAction *updateFirmware = new QAction(tr("Firmware Update..."), optionmenu);
	optionmenu->addAction(updateFirmware);
	connect(updateFirmware, SIGNAL(triggered(bool)), this, SLOT(openUpdateDialog()));

	// window menu
	QMenu *clearwindow = new QMenu("Clear");
	QAction *traffic = new QAction(tr("&traffic window"), clearwindow);
	connect(traffic, SIGNAL(triggered(bool)), this, SLOT(clearTrafficWindow()));
	clearwindow->addAction(traffic);
	QAction *edit = new QAction(tr("&edit window"), clearwindow);
	connect(edit, SIGNAL(triggered(bool)), this, SLOT(clearEditWindow()));
	clearwindow->addAction(edit);

	//clearwindow->insertItem (tr("&traffic window"), this, SLOT(clearTrafficWindow()));
	//clearwindow->insertItem (tr("&edit window"), this, SLOT(clearEditWindow()));
	//->insertItem (tr("&clear window"), clearwindow );

	QString about =
			tr("Kptc 0.2\n"
				 "user interface for the SCS-PTC-II\n\n"
				 "(C) 2001 Lars Schnake\nmail@lars-schnake.de\n"
				 );

	QMenu *helpmenu = new QMenu();
	helpmenu->setTitle(about);

		fixmenu = new QMenu ;

		QString s;
	for ( int i = 1; i <= 8; i++ ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;

	  s.setNum(i);

	  //QAction *actionSendText = new QAction(+ "&" + s + ". " + configdata.getFixLabel( s ), fixmenu);

		//connect(actionSendText, SIGNAL(triggered(bool)),this, SLOT (sendFixText( int )) );

	   // actionSendText->setShortcut(QKeySequence(Qt::CTRL, Qt::Key_F1, i));

	  //fixmenu->addAction(actionSendText);

//	  fixmenu->insertItem ( "&" + s + ". " + configdata.getFixLabel( s ),
//			  this, SLOT (sendFixText( int )), CTRL + SHIFT + (Key_F1 +(i)) , i);
//			((fixmenu->setItemParameter ( i, i);

		}
}

void Kptc::initializeStatusBar() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	KLed *sendled = new KLed( statusBar() );
	sendled->setColor(QColor("#CC0000b")); //red
	sendled->off();

	//statusBar()->setInsertOrder( KStatusBar::RightToLeft );	//
	//statusBar()->insertWidget(sendled, sendled->width(),1);
	statusBar()->addPermanentWidget(sendled, 1);

//statusinfo = new StatusInfo(statusBar());
statusinfo = new StatusInfo((QWidget *) statusBar());
	statusBar()->addWidget(statusinfo);
statusinfo->show();
}

void Kptc::initializeMenuBar() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	QMenuBar *menu = new QMenuBar( this );
	QMenu *fileMenu = new QMenu(tr( "&File" ), menu);
	menu->addMenu(fileMenu);
	QMenu *actionMenu = new QMenu(tr( "&Actions" ), menu);
	menu->addMenu(actionMenu);
	QMenu *fixmenuMenu = new QMenu( tr( "Fi&xtext" ), menu);
	menu->addMenu(fixmenuMenu);
	QMenu *optionMenu = new QMenu(tr("&Options"), menu);
	menu->addMenu(optionMenu);
}

void Kptc::parseModemOut(unsigned char c) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;

	if ((int) c == 6)  //qDebug ()<< "Packet-STATUSINFO";
	if (bStatusByteFollows) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		bStatusByteFollows = false;
		parseStatus( c );
	}
	else if ((int) c == 30 ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		bStatusByteFollows = true;
	}

	else if (c == 4) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		bPromptInfoFollows = true; // command prompt info follows
	}
	else if (bPromptInfoFollows) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		parsePrompt( c );
		  bPromptInfoFollows = false;
		  parsePromptText = 20;
	}
	else if (parsePromptText > 0) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	if ((int)c == 1) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl; parsePromptText = 0; currentterm = 1; textedit->setPrompt(prompt.trimmed()); prompt.clear();} // prompt end
		else
		{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
			parsePromptText--;
			if (this->isendline(c)) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
				prompt.append(c);
			}
		}
	}
	else if ((int)c == 3 ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl; currentterm = 3;   }	// delayed echo
	else if ((int)c == 2 ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl; currentterm = 2;   }	// rx
	else if ((int)c == 1 ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl; currentterm = 1;   }	// prompt , errors , ...
	else if ((int)c == 7 ) ; // klingeling :-) , changeover bell, do some ring ring here !?
	else { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		if ((currentterm == 2) || (currentterm == 3)) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
//			if (currentterm == 3) termoutput->setNewLineColor(QColor("#FF3333"));   // echo //red
//			else termoutput->setNewLineColor(QColor("#336600"));			  // rx

//			termoutput->appendChar(c);#
			//TODO
			show();
		}
		else if (currentterm == 1) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;

			//termoutput->setNewLineColor(QColor("#000000")); //black
		//termoutput->appendChar(c);
			if (this->isendline(c)) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;

				if (statusmessage.contains("*** ") == 1 ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;

					if (statusmessage.contains("CONNECTED") || statusmessage.contains("CALLING")) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
						statusmessage.replace( QRegExp("[*]"), "" );
						statusmessage = statusmessage.trimmed();
						statusinfo->statusmessage->setText(statusmessage);
					}
					else if ( statusmessage.contains("STBY")) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
						statusinfo->statusmessage->setText("");
					}
				}
				statusmessage = "";
			}
			else { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
				statusmessage.append(c);
			}
		}
	}
}

void Kptc :: sendline(QString qs) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	Modem::modem->writeLine( qs );
	termoutput->append(qs);
	termoutput->append("\n");
	show();
}

void Kptc :: sendchar(unsigned char c){ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	if (Modem::modem->writeChar(c));
}

void Kptc :: echoText(QString qtext) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	//	termoutput->setNewLineColor(QColor("#001933")); //darkblue
	termoutput->append(qtext);
}

void Kptc :: openconfigdialog(){ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	ConfigDialog configdialog ;
	if (configdialog.exec() == QDialog::Accepted) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		useconfigmachine();
	}
}

void Kptc :: useconfigmachine(){ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	configmachine->doconfig();
	statusinfo->call->setText(configdata.getCall() + " (" + configdata.getSelCall() + ") ");
	updateStatusBar();

	QString s;
	fixmenu->clear();
	for ( int i = 1; i <= 8; i++ ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		s.setNum(i);
		QAction *action = new QAction("&" + s + ". " + configdata.getFixLabel( s ), fixmenu);
		connect(action, SIGNAL(triggered(bool)), this, SLOT (sendFixText(int)));
		action->setShortcut(QKeySequence(Qt::Key_Shift, Qt::Key_F1 + i, i));
		fixmenu->addAction(action);
	}
}

void Kptc :: openCommandDialog() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		commanddialog.close();
		commanddialog.show();
		commanddialog.setFocus();
}

void Kptc :: clearTrafficWindow() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	//termoutput->repaint(false);
}

void Kptc :: clearEditWindow() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	textedit->clear();
}

//TODO Check which correct QObject has to been called

void Kptc :: showPactor() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	lefttoolbar->clear();
	std::cout << __LINE__ << " " << __FUNCTION__ << std::endl;
	this->expandToolBar("Stand by", "Standby", modecommander, lefttoolbar);
	this->expandToolBar("QRT", "initQRT", this, lefttoolbar);
	this->expandToolBar("changeover", "initchangeover", this, lefttoolbar);
	this->expandToolBar(" CQ ", "openDialog", cqdialog, lefttoolbar);
	this->expandToolBar("Unproto TX", "Unproto", modecommander, lefttoolbar);
	this->expandToolBar("Listen", "Listen", modecommander, lefttoolbar);
//  lefttoolbar->setToggle(6);
//	lefttoolbar->setButton(6,modecommander->isListen());
}

void Kptc :: showAmtor() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	//modebuttons->buttongroup->setButton(2);
	lefttoolbar->clear();
	std::cout << __LINE__ << " " << __FUNCTION__ << std::endl;
	this->expandToolBar("Stand by", "Standby", modecommander, lefttoolbar);
	this->expandToolBar("QRT", "initQRT", this, lefttoolbar);
	this->expandToolBar("changeover", "initchangeover", this, lefttoolbar);
	this->expandToolBar(" CQ ", "openDialog", cqdialog, lefttoolbar);
	this->expandToolBar("FEC TX", "FEC", modecommander, lefttoolbar);
	this->expandToolBar("Monitor", "Monitor", modecommander, lefttoolbar);
}

void Kptc :: showRTTY() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	// modetoolbar->toggleButton(3);
	// ;
	std::cout << __LINE__ << " " << __FUNCTION__ << std::endl;
	lefttoolbar->clear();
	this->expandToolBar(standby, "Standby", modecommander, lefttoolbar);
	this->expandToolBar(qrt, "initQRT", this, lefttoolbar);
	this->expandToolBar(changeover, "initchangeover", this, lefttoolbar);
	this->expandToolBar(baudrate, "showrttyspeeddialog", this, lefttoolbar);
	this->expandToolBar(cqws, "openDialog", cqdialog, lefttoolbar);
}

void Kptc :: showPSK31() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	//modetoolbar->toggleButton(4);
	//modebuttons->buttongroup->setButton(4);
	std::cout << __LINE__ << " " << __FUNCTION__ << std::endl;
	lefttoolbar->clear();
	this->expandToolBar(standby, "Standby", modecommander, lefttoolbar);
	this->expandToolBar(qrt, "initQRT", this, lefttoolbar);
	this->expandToolBar(changeover, "initchangeover", this, lefttoolbar);
	this->expandToolBar(cq, "openDialog", cqdialog, lefttoolbar);
}

void Kptc :: showCW() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	//modebuttons->buttongroup->setButton(5);
	lefttoolbar->clear();
	std::cout << __LINE__ << " " << __FUNCTION__ << std::endl;
	this->expandToolBar(standby, "StandBy", modecommander, lefttoolbar);
	this->expandToolBar(prescribe, "cwprescribe", modecommander, lefttoolbar);
	this->expandToolBar(flush, "cwflush", modecommander, lefttoolbar);
	this->expandToolBar(fix_speed, "cwautospeed", modecommander, lefttoolbar);
	this->expandToolBar(speed_up, "increaseCWSpeed", modecommander, lefttoolbar);
	this->expandToolBar(speed_down, "decreaseCWSpeed", modecommander, lefttoolbar);
	this->expandToolBar(tx_speed, "showcwspeeddialog", this, lefttoolbar);
	this->expandToolBar(cqws, "openDialog", cqdialog, lefttoolbar);
}

void Kptc::expandToolBar(QString text, char *slot, QObject *obj, QToolBar *bar) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	QAction *temp = new QAction(text, this->lefttoolbar);
	connect(temp, SIGNAL(triggered(bool)), obj, slot);
	bar->addAction(temp);
}

void Kptc :: showcwspeeddialog() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	cwspeedwidget->close();
	cwspeedwidget->show();
}

void Kptc :: showrttyspeeddialog() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	rttyspeedwidget->close();
	rttyspeedwidget->show();
}
void Kptc :: initchangeover() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	textedit->myinsert("[changeover]\n");
	termoutput->append("\n[changeover]\n");
	modecommander->Changeover();
}

void Kptc :: initQRT() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	textedit->myinsert("[QRT]\n");
	termoutput->append("\n[QRT]\n");
	modecommander->QRT();
}

void Kptc :: updateStatusBar() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	statusinfo->prompt->setText(modecommander->currendmod());
	if (modecommander->currendmod() == "cmd:")  showPactor();
	else if (modecommander->currendmod() == "Amtor")  showAmtor();
	else if (modecommander->currendmod() == "AMTOR-MONITOR")  showAmtor();
	else if (modecommander->currendmod() == "RTTY")  showRTTY();
	else if (modecommander->currendmod() == "PSK31")  showPSK31();
	else if (modecommander->currendmod() == "CW")  showCW();
	else showPactor();
}

bool Kptc::isendline(char c) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return c == '\n' || c == '\r' ;
}

void Kptc :: parsePrompt(const char c) {
	//  analyze prompt
	switch( c ) {
		case 32:
			modecommander->setcurrendmod("cmd:");
			break;
		case 33:
			modecommander->setcurrendmod("AMTOR");
			break;
		case 34:
			modecommander->setcurrendmod("AMTOR-MONITOR");
			break;
		case 35:
			modecommander->setcurrendmod("RTTY");
			break;
		case 36:
			modecommander->setcurrendmod("CW");
			break;
		case 37:
			modecommander->setcurrendmod("Pactor");
		break;
		default:
			modecommander->setcurrendmod( "??????" );
	}

	updateStatusBar();
}

void Kptc :: parseStatus(const char c) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	// check DIRECTION bit ( SEND-LED )
	if (( c & 0x08 ) > 0 ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		statusinfo->led->on();
	}
	else { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		statusinfo->led->off();
	}

	//TODO
	// check STATUS bits
	QString status;
	switch ( c & 0x07 ) {
		case 0:
			status =  "ERROR";
			break;
		case 1:
			status =  "REQUEST";
			break;
		case 2:
			status =  "TRAFFIC";
			break;
		case 3:
			status =  "IDLE";
			break;
		case 4:
			status =  "OVER";
			break;
		case 5:
			status =  "PHASE";
			break;
		case 6:
			status =  "SYNCH";
			break;
		case 7:
			status =  "";
			break;
		default:
			status =  "??????";
			break;
	}

	statusinfo->status->setText(status);

	// check MODE bits
	QString mode;
	switch ( (c & 112) >> 4 ) {
		case 0:
			mode =  "STAND BY";
			break;
		case 1:
			mode =  "AMTOR-ARQ";
			break;
		case 2:
			mode =  "PACTOR-ARQ";
			break;
		case 3:
			mode =  "AMTOR-FEC";
			break;
		case 4:
			mode =  "PACTOR-FEC";
			break;
		case 5:
			mode =  "RTTY / CW";
			break;
		case 6:
			mode =  "LISTEN";
			break;
		case 7:
			mode =  "channel busy";
			break;
	}
	statusinfo->mode->setText(mode);

	// read listen mode from status byte ??
	if ( mode == "LISTEN" ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		modecommander->setListen(true);
	}
	else { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		modecommander->setListen(false);
	}
}

void Kptc::sendFixText( int id ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	QString qsid;
	qsid.setNum( id );
	QString filename = configdata.getFixPath(qsid);
	QFile file ( filename );
	if ( ! file.open (QIODevice::ReadOnly)) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		QMessageBox::critical( this, "",
		("Cannot open fix text file !\n Error by opening \"" + filename +"\""  ) );	 // error by opening text file
		return;
	}
	QString buffer;

	while (!file.atEnd()) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	buffer = file.readLine(1024);
		Modem::modem->writeLine(buffer);
	}
	file.close() ;
}

void Kptc::openUpdateDialog() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	UpdateDialog updatedialog;
	if (updatedialog.exec()==QDialog::Accepted) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		Modem::modem->startNotifier();
	}
}

void Kptc::shutdown() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	modecommander->changetoPactor();
	sleep(1) ;
	configmachine->logout();
	if ( ! Modem::modem->closetty()) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		//qDebug () << Modem::modem->modemMessage();
	}
}

void Kptc::closeEvent( QCloseEvent* ce ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	emit shutdown();
	ce->accept();
}

void Kptc::fileQuit() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	close();
}

bool Kptc::queryClose() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	shutdown();
	return true;
}

Kptc::~Kptc(){ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	//TODO
}
