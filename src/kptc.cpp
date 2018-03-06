/***************************************************************************
		  kptc.cpp  -  description
			 -------------------
	begin				: Wed Feb 14 2001
	copyright			: (C) 2001 by Lars Schnake
	email				: mail@lars-schnake.de

	Ported to Qt5 by Sebastian Martin Dicke in 2017 (Sebastianmartindicke [@] gmx [.] de)
 ***************************************************************************/

/***************************************************************************
 *						 *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or	 *
 *   (at your option) any later version.				   *
 *						 *
 ***************************************************************************/

#include <QDebug>

#include "kptc.h"

Kptc::Kptc(QWidget *parent) : QMainWindow()
{
	//TODO
	this->initMainWindow();

	this->lefttoolbar = new QToolBar("");
	this->addToolBar(lefttoolbar);
	currentterm = 1;
	bPromptInfoFollows = false;

	modecommander = new ModeCommander(this);
	cqdialog = new CQDialog(this,  modecommander);

	this->initializeMenues();
	//lefttoolbar->setIconText(KToolBar::IconTextBottom);
	this->initializeStatusBar();
	this->initTextEdit();

	cwspeedwidget = new CWSpeedWidget();
	rttyspeedwidget = new RTTYSpeedWidget();
	configmachine = new ConfigMachine(this);

	//setAutoSaveSettings(); //TODO
	this->initModem();
	this->initializeMenues();
	this->initializeMenuBar();
	this->initializeToolBar();
}

bool Kptc::handleFirstStart() {
	bool bModemOk = false;
	QMessageBox::information(this, (QString(tr("Thank you for installing Kptc !\n")) +
	QString("This is a Ham Radio software for the SCS-PTC-II\nRemember: This sofware is still beta !\n\n") +
	QString("Connect the PTC to your computer\nand switch it on now.\nThen let?s continue with some configurations.\n") +
	QString("Have Fun ! 73 de Lars DL3YFC")), QString(tr("Welcome to Kptc !")));

	ConfigDialog configdialog ;
	if (configdialog.exec()== QDialog::Accepted) {
		bModemOk = Modem::modem->opentty() ;
		Modem::modem->notify(this, SLOT(parseModemOut(unsigned char)));
		configdata.setfirststart(false);
		useconfigmachine();
		//lefttoolbar->setBarPos(KToolBar::Left);
	}
	return bModemOk;
}

void Kptc::initModem() {
	(void) new Modem;
	bool bModemOk = false;

	if (configdata.firststart()) {
		bModemOk = this->handleFirstStart();
	}
	else {
		bModemOk = Modem::modem->opentty();
		Modem::modem->notify(this, SLOT(parseModemOut(unsigned char)));
		useconfigmachine();
	}
	configmachine->login();
	qDebug() << Modem::modem->modemMessage();
	if (!bModemOk) {
		QMessageBox::information(this,
			tr("Cannot open modem device !"), "Kptc");
	}
}

void Kptc::initMainWindow() {
	setWindowTitle(tr("Kptc - the PTC-II for penguins "));
	// connect(kapp, SIGNAL(shutDown()), this, SLOT(shutdown()));
	this->resize(650, 450);
}

void Kptc::initTextEdit() {
	QSplitter *splitter = new QSplitter(this);
	splitter->setOrientation(Qt::Vertical);
	splitter->setOpaqueResize(true);
	setCentralWidget(splitter);
	textedit = new MyLineEdit(splitter);
	textedit->notify(this, SLOT(sendchar(unsigned char)));
	splitter->show();
	textedit->setFocus();
	connect(textedit, SIGNAL(echoCommand(QString)), this, SLOT(echoText(QString)));
	connect(textedit, SIGNAL(sendit(QString)), this, SLOT(sendline(QString)));
	termoutput = new QTextEdit(this);
	termoutput->setGeometry(0, 4 * textedit->height(), this->width(), this->height() - 3 * textedit->height());
	termoutput->setEnabled(false);
	connect(this, SIGNAL(htmlString), termoutput, SLOT(insertHtml(QString)));
}

void Kptc::initializeToolBar() {
	modetoolbar = new QToolBar();
	this->addToolBar(modetoolbar);
	//modetoolbar->setGeometry(0, 2 * textedit->height(), this->width(), textedit->height());

//	this->expandToolBar(" | Pactor | ", "changetoPactor", modecommander, modetoolbar);
//	this->expandToolBar(" | Amtor |", "changetoAmtor", modecommander, modetoolbar);
//	this->expandToolBar(" | RTTY | ", "changetoRTTY", modecommander, modetoolbar);
//	this->expandToolBar(" | PSK31 | ", "changetoPSK31", modecommander, modetoolbar);
//	this->expandToolBar(" | CW | ", "changetoCW", modecommander, modetoolbar);

	modebuttons = new ModeButtons(modetoolbar);
	/*	KToolBarRadioGroup *moderadiogroup = new KToolBarRadioGroup(modetoolbar,"modradiogroup");
	modetoolbar->setToggle(1);
	modetoolbar->setToggle(2);
	modetoolbar->setToggle(3);
	modetoolbar->setToggle(4);
	modetoolbar->setToggle(5);
	moderadiogroup->addButton(1);
	moderadiogroup->addButton(2);
	moderadiogroup->addButton(3);
	moderadiogroup->addButton(4);
	moderadiogroup->addButton(5);*/

	// modebuttons in splitter window instead of modetoolbar ?! :
	// modebuttons = new ModeButtons(splitter);

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

void Kptc::initializeMenues() {
	initFileMenu();
	initClearwindowMenu();
	initOptionMenu();
	initActionMenu();
	initHelpMenu();
	initFixMenu();
}

void Kptc::initFileMenu() {
	filemenu = new QMenu(tr("&File"), this->menuBar());
	QAction *action = new QAction(tr("&Quit"), this);
	connect(action, SIGNAL(triggered(bool)), this, SLOT(fileQuit()));
	filemenu->addAction(action);
}

void Kptc::initClearwindowMenu() {
	clearwindow = new QMenu(tr("Clear"), menuBar());
	QAction *traffic = new QAction(tr("&traffic window"), clearwindow);
	connect(traffic, SIGNAL(triggered(bool)), this, SLOT(clearTrafficWindow()));
	clearwindow->addAction(traffic);
	QAction *edit = new QAction(tr("&edit window"), clearwindow);
	connect(edit, SIGNAL(triggered(bool)), this, SLOT(clearEditWindow()));
	clearwindow->addAction(edit);

	//clearwindow->insertItem (tr("&traffic window"), this, SLOT(clearTrafficWindow()));
	//clearwindow->insertItem (tr("&edit window"), this, SLOT(clearEditWindow()));
	//->insertItem (tr("&clear window"), clearwindow);
}

void Kptc::initOptionMenu() {
	optionmenu = new QMenu(tr("&Options"), menuBar());
	QAction *configure = new QAction(tr("&Config..."), optionmenu);
	optionmenu->addAction(configure);
	connect(configure, SIGNAL(triggered(bool)), this, SLOT(openconfigdialog()));

	QAction *updateFirmware = new QAction(tr("Firmware Update..."), optionmenu);
	optionmenu->addAction(updateFirmware);
	connect(updateFirmware, SIGNAL(triggered(bool)), this, SLOT(openUpdateDialog()));
}

void Kptc::initActionMenu() {
	actionmenu = new QMenu(tr("&Actions"), this->menuBar());

	QAction *reload = new QAction(tr("Change&over"), actionmenu);
	reload->setShortcut(QKeySequence(Qt::CTRL, 'Y'));
	connect(reload, SIGNAL(triggered(bool)), this, SLOT(initchangeover()));
	actionmenu->addAction(reload);

	QAction *exit = new QAction(tr("QR&T"), actionmenu);
	exit->setShortcut(QKeySequence(Qt::CTRL, 'D'));
	connect(exit, SIGNAL(triggered(bool)), this, SLOT(initQRT()));
	actionmenu->addAction(exit);

	QAction *stop = new QAction(tr("&Stand by"), actionmenu);
	stop->setShortcut(QKeySequence(Qt::CTRL, 'S'));
	connect(stop, SIGNAL(triggered(bool)), this, SLOT(Standby()));
	actionmenu->addAction(stop);

	QAction *konqueror = new QAction(tr("&CQ..."), actionmenu);
	konqueror->setShortcut(QKeySequence(Qt::CTRL, 'Q'));
	connect(konqueror, SIGNAL(triggered(bool)), this, SLOT(openDialog()));
	actionmenu->addAction(konqueror);

	QAction *command = new QAction(tr("&Command..."), actionmenu);
	command->setShortcut(QKeySequence(Qt::CTRL, 19)); //Ctrl + Esc
	connect(command, SIGNAL(triggered(bool)), this, SLOT(openCommandDialog()));
	actionmenu->addAction(command);
}

void Kptc::initHelpMenu() {
	helpmenu = new QMenu(tr("Help"), menuBar());
	QAction *helpaction = new QAction("About", helpmenu);
	helpmenu->addAction(helpaction);
	connect(helpaction, &QAction::triggered, this, [](){
		QString about = tr("Kptc 0.2\n user interface for the SCS-PTC-II\n\n (C) 2001 Lars Schnake\nmail@lars-schnake.de\n");
		QLabel *label = new QLabel(about);
		label->show();
	});
}

void Kptc::initFixMenu() {
	fixmenu = new QMenu(tr("Fi&xtext"), menuBar());

	QString number;
	for (int i = 1; i <= 8; i++) {
		number.setNum(i);
		QAction *actionSendText = new QAction(+ "&" + number + ". " + configdata.getFixLabel(number), fixmenu);
		connect(actionSendText, SIGNAL(triggered(bool)),this, SLOT (sendFixText(int)));

		actionSendText->setShortcut(QKeySequence(Qt::CTRL, Qt::Key_F1, i));

		fixmenu->addAction(actionSendText);
		//	  fixmenu->insertItem ("&" + s + ". " + configdata.getFixLabel(s),
		//			  this, SLOT (sendFixText(int)), CTRL + SHIFT + (Key_F1 +(i)) , i);
		//			((fixmenu->setItemParameter (i, i);

	}
}

void Kptc::initializeStatusBar() {
	KLed *sendled = new KLed(statusBar());
	sendled->setColor(QColor("#CC0000b")); //red
	sendled->off();

	statusBar()->setLayoutDirection(Qt::RightToLeft);
	statusBar()->insertWidget(1, sendled, sendled->width());
	statusBar()->addPermanentWidget(sendled, 1);

	statusinfo = new StatusInfo((QWidget *) statusBar());
	statusBar()->addWidget(statusinfo);
	statusinfo->show();
}

void Kptc::initializeMenuBar() {
	menuBar()->addMenu(filemenu);
	menuBar()->addMenu(actionmenu);
	menuBar()->addMenu(fixmenu);
	menuBar()->addMenu(optionmenu);
	menuBar()->addMenu(clearwindow);
	menuBar()->addMenu(helpmenu);
}

void Kptc::parseModemOut(unsigned char c) {
	if ((int) c == 6) {
		qDebug ()<< "Packet-STATUSINFO";
	}
	if (bStatusByteFollows) {
		bStatusByteFollows = false;
		parseStatus(c);
	}
	else if ((int) c == 30) {
		bStatusByteFollows = true;
	}
	else if (c == 4) {
		bPromptInfoFollows = true; // command prompt info follows
	}
	else if (bPromptInfoFollows) {
		parsePrompt(c);
		  bPromptInfoFollows = false;
		  parsePromptText = 20;
	}
	else if (parsePromptText > 0) {
	if ((int)c == 1) { parsePromptText = 0; currentterm = 1; textedit->setPrompt(prompt.trimmed()); prompt.clear();} // prompt end
		else {
			parsePromptText--;
			if (this->isendline(c)) {
				prompt.append(c);
			}
		}
	}
	else if ((int)c == 3) {
			currentterm = 3;
	}	// delayed echo
	else if ((int)c == 2) {
			currentterm = 2;
	}	// rx
	else if ((int)c == 1) {
			currentterm = 1;
	}	// prompt , errors , ...
	else if ((int)c == 7) ; // klingeling :-) , changeover bell, do some ring ring here !?
	else {
		if ((currentterm == 2) || (currentterm == 3)) {
			QString color;
			if (currentterm == 3) {
				color = "#FF3333"; // red
			}
			else {
				color = "#336600"; // rx
			}
			setHTML(makeHTML(QString(c), color));
			show();
		}
		else if (currentterm == 1) {
			setHTML(makeHTML(QString(c), "#000000")); //black
			if (this->isendline(c)) {
				if (statusmessage.contains("*** ") == 1) {
					if (statusmessage.contains("CONNECTED") || statusmessage.contains("CALLING")) {
						statusmessage.replace(QRegExp("[*]"), "");
						statusmessage = statusmessage.trimmed();
						statusinfo->statusmessage->setText(statusmessage);
					}
					else if (statusmessage.contains("STBY")) {
						statusinfo->statusmessage->setText("");
					}
				}
				statusmessage = "";
			}
			else {
				statusmessage.append(c);
			}
		}
	}
}

void Kptc :: sendline(QString qs) {
	Modem::modem->writeLine(qs);
	termoutput->append(qs);
	termoutput->append("\n");
	show();
}

void Kptc :: sendchar(unsigned char c) {
	if (Modem::modem->writeChar(c));//TODO Why?
}

void Kptc :: echoText(QString qtext) {
	setHTML(makeHTML(qtext, "#001933")); //darkblue
	termoutput->append(qtext);
}

void Kptc :: openconfigdialog() {
	ConfigDialog configdialog ;
	if (configdialog.exec() == QDialog::Accepted) {
		useconfigmachine();
	}
}

void Kptc :: useconfigmachine() {
	configmachine->doconfig();
	statusinfo->call->setText(configdata.getCall() + " (" + configdata.getSelCall() + ") ");
	updateStatusBar();

	QString number;
	fixmenu->clear();
	for (int i = 1; i <= 8; i++) {
		number.setNum(i);
		QAction *action = new QAction("&" + number + ". " + configdata.getFixLabel(number), fixmenu);
		connect(action, SIGNAL(triggered(bool)), this, SLOT (sendFixText(int)));
		action->setShortcut(QKeySequence(Qt::Key_Shift, Qt::Key_F1 + i, i));
		fixmenu->addAction(action);
	}
}

void Kptc :: openCommandDialog() {
	commanddialog.close();
	commanddialog.show();
	commanddialog.setFocus();
}

void Kptc :: clearTrafficWindow() {
	termoutput->repaint();
}

void Kptc :: clearEditWindow() {
	textedit->clear();
}

//TODO Check which correct QObject has to been called

void Kptc :: showPactor() {
	lefttoolbar->clear();
	this->expandToolBar("Stand by", "Standby", modecommander, lefttoolbar);
	this->expandToolBar("QRT", "initQRT", this, lefttoolbar);
	this->expandToolBar("changeover", "initchangeover", this, lefttoolbar);
	this->expandToolBar(" CQ ", "openDialog", cqdialog, lefttoolbar);
	this->expandToolBar("Unproto TX", "Unproto", modecommander, lefttoolbar);
	this->expandToolBar("Listen", "Listen", modecommander, lefttoolbar);
//  lefttoolbar->setToggle(6);
//	lefttoolbar->setButton(6,modecommander->isListen());
}

void Kptc :: showAmtor() {
	//modebuttons->buttongroup->setButton(2);
	lefttoolbar->clear();
	this->expandToolBar("Stand by", "Standby", modecommander, lefttoolbar);
	this->expandToolBar("QRT", "initQRT", this, lefttoolbar);
	this->expandToolBar("changeover", "initchangeover", this, lefttoolbar);
	this->expandToolBar(" CQ ", "openDialog", cqdialog, lefttoolbar);
	this->expandToolBar("FEC TX", "FEC", modecommander, lefttoolbar);
	this->expandToolBar("Monitor", "Monitor", modecommander, lefttoolbar);
}

void Kptc :: showRTTY() {
	// modetoolbar->toggleButton(3);
	lefttoolbar->clear();
	this->expandToolBar(standby, "Standby", modecommander, lefttoolbar);
	this->expandToolBar(qrt, "initQRT", this, lefttoolbar);
	this->expandToolBar(changeover, "initchangeover", this, lefttoolbar);
	this->expandToolBar(baudrate, "showrttyspeeddialog", this, lefttoolbar);
	this->expandToolBar(cqws, "openDialog", cqdialog, lefttoolbar);
}

void Kptc :: showPSK31() {
	//modetoolbar->toggleButton(4);
	//modebuttons->buttongroup->setButton(4);
	lefttoolbar->clear();
	this->expandToolBar(standby, "Standby", modecommander, lefttoolbar);
	this->expandToolBar(qrt, "initQRT", this, lefttoolbar);
	this->expandToolBar(changeover, "initchangeover", this, lefttoolbar);
	this->expandToolBar(cq, "openDialog", cqdialog, lefttoolbar);
}

void Kptc :: showCW() {
	//modebuttons->buttongroup->setButton(5);
	lefttoolbar->clear();
	this->expandToolBar(standby, "StandBy", modecommander, lefttoolbar);
	this->expandToolBar(prescribe, "cwprescribe", modecommander, lefttoolbar);
	this->expandToolBar(flush, "cwflush", modecommander, lefttoolbar);
	this->expandToolBar(fix_speed, "cwautospeed", modecommander, lefttoolbar);
	this->expandToolBar(speed_up, "increaseCWSpeed", modecommander, lefttoolbar);
	this->expandToolBar(speed_down, "decreaseCWSpeed", modecommander, lefttoolbar);
	this->expandToolBar(tx_speed, "showcwspeeddialog", this, lefttoolbar);
	this->expandToolBar(cqws, "openDialog", cqdialog, lefttoolbar);
}

void Kptc::expandToolBar(QString text, char *slot, QObject *obj, QToolBar *bar) {
	QAction *temp = new QAction(text, this->lefttoolbar);
	connect(temp, SIGNAL(triggered(bool)), obj, slot);
	bar->addAction(temp);
}

void Kptc :: showcwspeeddialog() {
	cwspeedwidget->close();
	cwspeedwidget->show();
}

void Kptc :: showrttyspeeddialog() {
	rttyspeedwidget->close();
	rttyspeedwidget->show();
}
void Kptc :: initchangeover() {
	textedit->myinsert("[changeover]\n");
	termoutput->append("\n[changeover]\n");
	modecommander->Changeover();
}

void Kptc :: initQRT() {
	textedit->myinsert("[QRT]\n");
	termoutput->append("\n[QRT]\n");
	modecommander->QRT();
}

QString Kptc::makeHTML(QString text, QString color) {
	text = "<p style=\"color: " + color +
	"; font-family: courier; font-size: 12; font-style: normal;\">"
	+ text + "</p>";
	return text;
}

void Kptc::setHTML(QString text) {
	text = termoutput->toHtml() + text;
	termoutput->setHtml(text);
}

void Kptc :: updateStatusBar() {
	statusinfo->prompt->setText(modecommander->currendmod());
	if (modecommander->currendmod() == "cmd:")  showPactor();
	else if (modecommander->currendmod() == "Amtor")  showAmtor();
	else if (modecommander->currendmod() == "AMTOR-MONITOR")  showAmtor();
	else if (modecommander->currendmod() == "RTTY")  showRTTY();
	else if (modecommander->currendmod() == "PSK31")  showPSK31();
	else if (modecommander->currendmod() == "CW")  showCW();
	else showPactor();
}

bool Kptc::isendline(char c) {
	return c == '\n' || c == '\r' ;
}

void Kptc :: parsePrompt(const char c) {
	//  analyze prompt
	switch(c) {
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
			modecommander->setcurrendmod("??????");
	}

	updateStatusBar();
}

void Kptc :: parseStatus(const char c) {
	// check DIRECTION bit (SEND-LED)
	if ((c & 0x08) > 0) {
		statusinfo->led->on();
	}
	else {
		statusinfo->led->off();
	}

	//TODO
	// check STATUS bits
	QString status;
	switch (c & 0x07) {
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
	switch ((c & 112) >> 4) {
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
	if (mode == "LISTEN") {
		modecommander->setListen(true);
	}
	else {
		modecommander->setListen(false);
	}
}

void Kptc::sendFixText(int id) {
	QString qsid;
	qsid.setNum(id);
	QString filename = configdata.getFixPath(qsid);
	QFile file (filename);
	if (!file.open(QIODevice::ReadOnly)) {
		QMessageBox::critical(this, "",
		("Cannot open fix text file !\n Error by opening \"" + filename +"\"" ));	 // error by opening text file
		return;
	}
	QString buffer;

	while (!file.atEnd()) {
	buffer = file.readLine(1024);
		Modem::modem->writeLine(buffer);
	}
	file.close() ;
}

void Kptc::openUpdateDialog() {
	UpdateDialog updatedialog;
	if (updatedialog.exec()==QDialog::Accepted) {
		Modem::modem->startNotifier();
	}
}

void Kptc::shutdown() {
	modecommander->changetoPactor();
	sleep(1) ;
	configmachine->logout();
	if (! Modem::modem->closetty()) {
		qDebug () << Modem::modem->modemMessage();
	}
}

void Kptc::closeEvent(QCloseEvent* ce) {
	emit shutdown();
	ce->accept();
}

void Kptc::fileQuit() {
	close();
}

bool Kptc::queryClose() {
	shutdown();
	return true;
}

Kptc::~Kptc() {
	deleteLater();
}
