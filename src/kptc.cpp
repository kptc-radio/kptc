/***************************************************************************
		 kptc.cpp - description
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
 *   the Free Software Foundation; either version 2 of the License, or	   *
 *   (at your option) any later version.				                   *
 *						 *
 ***************************************************************************/

#include <QDebug>

#include "kptc.h"

Kptc::Kptc(QWidget *parent) : QMainWindow(parent)
{
	//TODO
	this->initMainWindow();

	this->lefttoolbar = new QToolBar();
	this->addToolBar(lefttoolbar);
	this->addToolBarBreak();

	modecommander = new ModeCommander(this);
	cqdialog = new CQDialog(this, modecommander);

	this->initializeMenues();
	//lefttoolbar->setIconText(KToolBar::IconTextBottom);
	this->initializeStatusBar();
	this->initTextEdit();

	cwspeedwidget = new CWSpeedWidget();
	rttyspeedwidget = new RTTYSpeedWidget();
	configmachine = new ConfigMachine();

	//setAutoSaveSettings(); //TODO
	this->initModem();
	this->initializeMenues();
	this->initializeMenuBar();
	this->initializeToolBar();
	this->resizeElements();

	connect(this, &Kptc::status, statusinfo, &StatusInfo::setStatus);
	connect(this, &Kptc::mode, statusinfo, &StatusInfo::setMode);
	connect(this, &Kptc::direction, statusinfo, &StatusInfo::setLED);
	connect(this, &Kptc::changePrompt, statusinfo, &StatusInfo::setPrompt);
	connect(this, &Kptc::changeCall, statusinfo, &StatusInfo::setCall);
	connect(this, &Kptc::changeStatusMessage, statusinfo, &StatusInfo::setStatusMessage);

	connect(&dataparser, &DataParser::statusMessage, this, &Kptc::setStatusMessage);
	connect(&dataparser, &DataParser::character, this, &Kptc::appendToPrompt);
	connect(&dataparser, &DataParser::prompt, this, &Kptc::showPrompt);
	connect(&dataparser, &DataParser::direction, statusinfo, &StatusInfo::setLED);
	connect(&dataparser, &DataParser::status, statusinfo, &StatusInfo::setStatus);
	connect(&dataparser, &DataParser::mode, statusinfo, &StatusInfo::setMode);
	connect(&dataparser, &DataParser::listen, modecommander, &ModeCommander::setListen);
	connect(&dataparser, &DataParser::line, this, &Kptc::apppendToTermoutput);
	connect(&dataparser, &DataParser::currentmode, this, &Kptc::updateStatusBar);

	statusinfo->setCall(configdata.getCall() + " (" + configdata.getSelCall() + ") ");
}

bool Kptc::handleFirstStart() {
	bool bModemOk = false;
	QMessageBox::information(this, QString(tr("Welcome to Kptc !")),
		QString(tr("Thank you for installing Kptc !\n")) +
		QString(tr("This is a Ham Radio software for the SCS-PTC-II\nRemember: This sofware is still beta !\n\n")) +
		QString(tr("Connect the PTC to your computer\nand switch it on now.\nThen let?s continue with some configurations.\n")) +
		QString(tr("Have Fun! 73 de Lars DL3YFC")));

	ConfigDialog configdialog ;
	if (configdialog.exec() == QDialog::Accepted) {
		bModemOk = Modem::modem()->opentty() ;
		Modem::modem()->notify(&dataparser, SLOT(parseModemOut(unsigned char)));
		configdata.setfirststart(false);
		useconfigmachine();
	}
	return bModemOk;
}

void Kptc::initModem() {
	bool bModemOk = false;

	if (configdata.firststart()) {
		bModemOk = this->handleFirstStart();
	}
	else {
		bModemOk = Modem::modem()->opentty();
		Modem::modem()->notify(&dataparser, SLOT(parseModemOut(unsigned char)));
		useconfigmachine();
	}
	ConfigMachine::Pair result = configmachine->login();
	if (!result.first) {
		QMessageBox::critical(this, "",
			("Cannot open your personal login script file !\n Error by opening \"" + result.second +"\"" ));	 // error by opening text file
	}
	qDebug() << Modem::modem()->modemMessage();
	if (!bModemOk) {
		QMessageBox::information(this,
			tr("Cannot open modem device !"), "Kptc");
	}
}

void Kptc::initMainWindow() {
	setWindowTitle(tr("Kptc - the PTC-II for penguins "));
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
	connect(textedit, &MyLineEdit::echoCommand, this, &Kptc::echoText);
	connect(textedit, SIGNAL(sendit(QString)), this, SLOT(sendline(QString)));
	termoutput = new QTextEdit(this);
	termoutput->setEnabled(false);
	connect(this, &Kptc::htmlString, termoutput, &QTextEdit::insertHtml);
}

void Kptc::initializeToolBar() {
	modebuttons = new ModeButtons(modecommander, this);
	this->addToolBar(modebuttons);

	// modebuttons in splitter window instead of modetoolbar ?! :
	// modebuttons = new ModeButtons(splitter);	
}

void Kptc::resizeElements() {
	termoutput->setGeometry(0, 4 * textedit->height(), this->width() - 1, this->height() - 4 * textedit->height() - statusBar()->height());
	textedit->setGeometry(2, 0, width() - 4, 30);
	statusBar()->resize(width(), 20);
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
	connect(action, &QAction::triggered, this, [this](){close();});
	filemenu->addAction(action);
}

void Kptc::initClearwindowMenu() {
	clearwindow = new QMenu(tr("Clear"), menuBar());

	QAction *traffic = new QAction(tr("&traffic window"), clearwindow);
	connect(traffic, &QAction::triggered, this, &Kptc::clearTrafficWindow);
	clearwindow->addAction(traffic);

	QAction *edit = new QAction(tr("&edit window"), clearwindow);
	connect(edit, &QAction::triggered, this, &Kptc::clearEditWindow);
	clearwindow->addAction(edit);
}

void Kptc::initOptionMenu() {
	optionmenu = new QMenu(tr("&Options"), menuBar());

	QAction *configure = new QAction(tr("&Config..."), optionmenu);
	optionmenu->addAction(configure);
	connect(configure, &QAction::triggered, this, &Kptc::openconfigdialog);

	QAction *updateFirmware = new QAction(tr("Firmware Update..."), optionmenu);
	optionmenu->addAction(updateFirmware);
	connect(updateFirmware, &QAction::triggered, this, &Kptc::openUpdateDialog);
}

void Kptc::initActionMenu() {
	actionmenu = new QMenu(tr("&Actions"), this->menuBar());

	QAction *reload = new QAction(tr("Change&over"), actionmenu);
	reload->setShortcut(QKeySequence(Qt::CTRL, 'Y'));
	connect(reload, &QAction::triggered, this, &Kptc::initchangeover);
	actionmenu->addAction(reload);

	QAction *exit = new QAction(tr("QR&T"), actionmenu);
	exit->setShortcut(QKeySequence(Qt::CTRL, 'D'));
	connect(exit, &QAction::triggered, this, &Kptc::initQRT);
	actionmenu->addAction(exit);

	QAction *stop = new QAction(tr("&Stand by"), actionmenu);
	stop->setShortcut(QKeySequence(Qt::CTRL, 'S'));
	connect(stop, &QAction::triggered, modecommander, &ModeCommander::Standby);
	actionmenu->addAction(stop);

	//TODO
//	QAction *konqueror = new QAction(tr("&CQ..."), actionmenu);
//	konqueror->setShortcut(QKeySequence(Qt::CTRL, 'Q'));
//	connect(konqueror, &QAction::triggered, this, );
//	actionmenu->addAction(konqueror);

	QAction *command = new QAction(tr("&Command..."), actionmenu);
	command->setShortcut(QKeySequence(Qt::CTRL, 19)); //Ctrl + Esc
	connect(command, &QAction::triggered, this, &Kptc::openCommandDialog);
	actionmenu->addAction(command);
}

void Kptc::initHelpMenu() {
	helpmenu = new QMenu(tr("Help"), menuBar());

	QAction *helpaction = new QAction(tr("About"), helpmenu);
	helpmenu->addAction(helpaction);

	static QString about = tr("Kptc 0.2\n user interface for the SCS-PTC-II\n\n (C) 2001 Lars Schnake\nmail@lars-schnake.de\n");
	static QLabel *label = new QLabel(about);
	connect(helpaction, &QAction::triggered, this, [label](){
		label->show();
	});
	connect(this, &Kptc::closing, label, [&label](){delete label;});
}

void Kptc::initFixMenu() {
	fixmenu = new QMenu(tr("Fi&xtext"), menuBar());

	QString number;
	for (int i = 1; i <= 8; i++) {
		number.setNum(i);
		QAction *actionSendText = new QAction(+ "&" + number + ". " + configdata.getFixLabel(number), fixmenu);
		//TODO
		connect(actionSendText, &QAction::triggered, this, &Kptc::sendFixText);

		actionSendText->setShortcut(QKeySequence(Qt::CTRL, Qt::Key_F1, i));

		fixmenu->addAction(actionSendText);
	}
}

void Kptc::initializeStatusBar() {
	statusinfo = new StatusInfo(this);
	setStatusBar(statusinfo);
}

void Kptc::initializeMenuBar() {
	menuBar()->addMenu(filemenu);
	menuBar()->addMenu(actionmenu);
	menuBar()->addMenu(fixmenu);
	menuBar()->addMenu(optionmenu);
	menuBar()->addMenu(clearwindow);
	menuBar()->addMenu(helpmenu);
}

void Kptc::sendline(QString qs) {
	Modem::modem()->writeString(qs);
	termoutput->append(qs);
	termoutput->append("\n");
	show();
}

void Kptc::sendchar(unsigned char c) {
	Modem::modem()->writeChar(c);
}

void Kptc::echoText(QString qtext) {
	setHTML(qtext, Kptc::darkblue);
	termoutput->append(qtext);
}

void Kptc::openconfigdialog() {
	ConfigDialog configdialog ;
	if (configdialog.exec() == QDialog::Accepted) {
		useconfigmachine();
	}
}

void Kptc::useconfigmachine() {
	configmachine->doconfig();
	updateStatusBar();

	QString number;
	fixmenu->clear();
	for (int i = 1; i <= 8; i++) {
		number.setNum(i);
		QAction *action = new QAction("&" + number + ". " + configdata.getFixLabel(number), fixmenu);
		//TODO
		connect(action, &QAction::triggered, this, &Kptc::sendFixText);
		action->setShortcut(QKeySequence(Qt::Key_Shift, Qt::Key_F1 + i, i));
		fixmenu->addAction(action);
	}
}

void Kptc::openCommandDialog() {
	commanddialog.close();
	commanddialog.show();
	commanddialog.setFocus();
}

void Kptc::clearTrafficWindow() {
	termoutput->clear();
}

void Kptc::clearEditWindow() {
	textedit->clear();
}

//TODO Check which correct QObject has to been called

void Kptc::showPactor() {
//	lefttoolbar->clear();
	this->expandToolBar(tr("Stand by"), &ModeCommander::Standby, modecommander, *lefttoolbar);
	this->expandToolBar(tr("QRT"), &Kptc::initQRT, this, *lefttoolbar);
	this->expandToolBar(tr("changeover"), &Kptc::initchangeover, this, *lefttoolbar);
	this->expandToolBar(tr(" CQ "), &CQDialog::openDialog, cqdialog, *lefttoolbar);
	this->expandToolBar(tr("Unproto TX"), &ModeCommander::Unproto, modecommander, *lefttoolbar);
	this->expandToolBar(tr("Listen"), &ModeCommander::Listen, modecommander, *lefttoolbar);
// lefttoolbar->setToggle(6);
//	lefttoolbar->setButton(6,modecommander->isListen());
}

void Kptc::showAmtor() {
	//modebuttons->buttongroup->setButton(2);
//	lefttoolbar->clear();
	this->expandToolBar(tr("Stand by"), &ModeCommander::Standby, modecommander, *lefttoolbar);
	this->expandToolBar(tr("QRT"), &Kptc::initQRT, this, *lefttoolbar);
	this->expandToolBar(tr("changeover"), &Kptc::initchangeover, this, *lefttoolbar);
	this->expandToolBar(tr(" CQ "), &CQDialog::openDialog, cqdialog, *lefttoolbar);
	this->expandToolBar(tr("FEC TX"), &ModeCommander::FEC, modecommander, *lefttoolbar);
	this->expandToolBar(tr("Monitor"), &ModeCommander::Monitor, modecommander, *lefttoolbar);
}

void Kptc::showRTTY() {
	// modetoolbar->toggleButton(3);
//	lefttoolbar->clear();
	this->expandToolBar(standby, &ModeCommander::Standby, modecommander, *lefttoolbar);
	this->expandToolBar(qrt, &Kptc::initQRT, this, *lefttoolbar);
	this->expandToolBar(changeover, &Kptc::initchangeover, this, *lefttoolbar);
	this->expandToolBar(baudrate, &Kptc::showrttyspeeddialog, this, *lefttoolbar);
	this->expandToolBar(cqws, &CQDialog::openDialog, cqdialog, *lefttoolbar);
}

void Kptc::showPSK31() {
	//modetoolbar->toggleButton(4);
//	modebuttons->buttongroup->setButton(4);
	lefttoolbar->clear();
	this->expandToolBar(standby, &ModeCommander::Standby, modecommander, *lefttoolbar);
	this->expandToolBar(qrt, &Kptc::initQRT, this, *lefttoolbar);
	this->expandToolBar(changeover, &Kptc::initchangeover, this, *lefttoolbar);
	this->expandToolBar(cq, &CQDialog::openDialog, cqdialog, *lefttoolbar);
}

void Kptc::showCW() {
	//modebuttons->buttongroup->setButton(5);
	lefttoolbar->clear();
	this->expandToolBar(standby, &ModeCommander::Standby, modecommander, *lefttoolbar);
	this->expandToolBar(prescribe, &ModeCommander::cwprescribe, modecommander, *lefttoolbar);
	this->expandToolBar(flush, &ModeCommander::cwflush, modecommander, *lefttoolbar);
	this->expandToolBar(fix_speed, &ModeCommander::cwautospeed, modecommander, *lefttoolbar);
	this->expandToolBar(speed_up, &ModeCommander::increaseCWSpeed, modecommander, *lefttoolbar);
	this->expandToolBar(speed_down, &ModeCommander::decreaseCWSpeed, modecommander, *lefttoolbar);
	this->expandToolBar(tx_speed, &Kptc::showcwspeeddialog, this, *lefttoolbar);
	this->expandToolBar(cqws, &CQDialog::openDialog, cqdialog, *lefttoolbar);
}

void Kptc::expandToolBar(QString text, auto slot, auto obj, QToolBar &bar) {
	QAction *temp = new QAction(text, &bar);
	bar.addAction(temp);

	connect(temp, &QAction::triggered, obj, slot);
}

void Kptc::showcwspeeddialog() {
	cwspeedwidget->close();
	cwspeedwidget->show();
}

void Kptc::showrttyspeeddialog() {
	rttyspeedwidget->close();
	rttyspeedwidget->show();
}

void Kptc::initchangeover() {
	textedit->myinsert("[changeover]\n");
	termoutput->append("\n[changeover]\n");
	modecommander->Changeover();
}

void Kptc::initQRT() {
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

void Kptc::setHTML(QString text, QString color) {
	text = makeHTML(text, color);
	text = termoutput->toHtml() + text;
	termoutput->setHtml(text);
}

void Kptc::updateStatusBar() {
	statusinfo->setPrompt(modecommander->currendmod());
//	statusinfo->setPrompt(modecommander->currendmod());
	if (modecommander->currendmod() == "cmd:") {
		showPactor();
	}
	else if (modecommander->currendmod() == "Amtor") {
		showAmtor();
	}
	else if (modecommander->currendmod() == "AMTOR-MONITOR") {
		showAmtor();
	}
	else if (modecommander->currendmod() == "RTTY") {
		showRTTY();
	}
	else if (modecommander->currendmod() == "PSK31") {
		showPSK31();
	}
	else if (modecommander->currendmod() == "CW") {
		showCW();
	}
	else {
		showPactor();
	}
}

void Kptc::sendFixText(int id) {
	QString idstring;
	idstring.setNum(id);
	QString filename = configdata.getFixPath(idstring);
	QFile file (filename);
	if (!file.open(QIODevice::ReadOnly)) {
		QMessageBox::critical(this, "",
			("Cannot open fix text file !\n Error by opening \"" + filename +"\"" ));	 // error by opening text file
		return;
	}
	QString buffer;

	while (!file.atEnd()) {
		buffer = file.readLine(1024);
		Modem::modem()->writeString(buffer);
	}
	file.close() ;
}

void Kptc::openUpdateDialog() {
	UpdateDialog updatedialog;
	if (updatedialog.exec() == QDialog::Accepted) {
		Modem::modem()->startNotifier();
	}
}

void Kptc::shutdown() {
	modecommander->changetoPactor();
	sleep(1);
	ConfigMachine::Pair result = configmachine->logout();
	if (!result.first) {
		QMessageBox::critical(this, "",
			("Cannot open your personal logout script file !\n Error by opening \"" + result.second +"\"" ));	 // error by opening text file
	}
	if (!Modem::modem()->closetty()) {
		qDebug () << Modem::modem()->modemMessage();
	}
}

void Kptc::closeEvent(QCloseEvent *event) {
	emit shutdown();
	event->accept();
}

bool Kptc::queryClose() {
	shutdown();
	return true;
}

void Kptc::showPrompt() {
	textedit->setPrompt(prompt.trimmed());
	prompt.clear();
}

void Kptc::appendToPrompt(char c) {
	prompt.append(c);
}

void Kptc::apppendToTermoutput(char c, QString color) {
	setHTML(QString(c), color);
	show();
}

void Kptc::setStatusMessage(char c, bool endline) {
	setHTML(QString(c), Kptc::black); //black
	if (endline) {
		if (statusmessage.contains("*** ") == 1) {
			if (statusmessage.contains("CONNECTED") || statusmessage.contains("CALLING")) {
				statusmessage.replace(QRegExp("[*]"), "");
				statusmessage = statusmessage.trimmed();
				emit changeStatusMessage(statusmessage);
			}
			else if (statusmessage.contains("STBY")) {
				emit changeStatusMessage("");
			}
		}
		statusmessage = "";
	}
	else {
		statusmessage.append(c);
	}
}

void Kptc::resizeEvent(QResizeEvent *event) {
	event->accept();
	this->resizeElements();
}

Kptc::~Kptc() {
	deleteLater();
}
