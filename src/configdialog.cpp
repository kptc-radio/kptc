/***************************************************************************
						  configdialog.cpp  -  description
							 -------------------
	begin                : Fri Jan 26 2001
	copyright            : (C) 2001 by Lars Schnake
	email                : lschnak@suse.de

	Ported to Qt5 by Sebastian Martin Dicke in 2017
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "configdialog.h"

#include <iostream>

#define i18n

ConfigDialog::ConfigDialog(QWidget *parent) : QDialog(parent){
	resize(500, 340);
	setMinimumSize(500, 340);
	setMaximumSize(500, 340);

	this->createTopWidgets();
	this->initWidgetStack();
	this->initLeftsiteEntries();
	this->createButtons();
	resetwidgets();
}

void ConfigDialog::createTopWidgets() {
	this->createPersonalWidget();
	this->createPortWidget();
	this->createBoxWidget();
	this->createPacketWidget();
	this->createLogOutWidget();
	this->createLogInWidget();
	this->createFixTextWidget();
}

void ConfigDialog::initWidgetStack() {
	widgetstack= new QStackedWidget(this);
	widgetstack->setFrameStyle(QFrame::Sunken | QFrame::Box);
	widgetstack->setLineWidth(1);
	widgetstack->setMidLineWidth(0);
	widgetstack->setGeometry(130, 10, 360, 290);
	widgetstack->addWidget(port_top);
	widgetstack->addWidget(personal_top);
	widgetstack->addWidget(fixtext_top);
	widgetstack->addWidget(packet_top);
	widgetstack->addWidget(box_top);
	widgetstack->addWidget(login_top);
	widgetstack->addWidget(logout_top);
// final :
	widgetstack->setCurrentIndex(0);
}

void ConfigDialog::initLeftsiteEntries() {
	QListWidget* ListBox = new QListWidget(this);
	ListBox->setGeometry(10, 10, 110, 290);
	ListBox->setMinimumSize(110, 290);
	ListBox->setMaximumSize(110, 290);
	ListBox->setFocusPolicy(Qt::StrongFocus);
	ListBox->setFrameStyle(51);
	ListBox->setLineWidth(2);
	ListBox->setMidLineWidth(0);
	ListBox->insertItem(0, "PORT");
	ListBox->insertItem(1, "PERSONAL");
	ListBox->insertItem(2, "FIX-TEXT");
	ListBox->insertItem(3, "PACKET");
	ListBox->insertItem(4, "BOX");
	ListBox->insertItem(5, "LOGIN");
	ListBox->insertItem(6, "LOGOUT");
	ListBox->setAutoScroll(true);

	connect(ListBox, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectwidget(QListWidgetItem*)));
}

void ConfigDialog::createButtons() {
	QPushButton *OkButton = this->createButton("OkButton", QRect(285, 310, 90, 20), "Ok");
	QPushButton* CancelButton = this->createButton("CancelButton", QRect(395, 310, 90, 20), "Cancel");

	QObject::connect (CancelButton, SIGNAL (clicked()), this, SLOT (reject()));
	QObject::connect (OkButton, SIGNAL(clicked(bool)), this, SLOT (writeconfig()));
}

QPushButton *ConfigDialog::createButton(QString text, const QRect dimensions, QString text2) {
	QPushButton* button = new QPushButton(text, this);
	button->setGeometry(dimensions);
	button->setMinimumSize(90, 20);
	button->setMaximumSize(90, 20);
	button->setFocusPolicy(Qt::TabFocus);
	button->setText(i18n(text2));
	button->setAutoRepeat(false);
	button->setDefault(false);
	button->setAutoDefault(false);
	return button;
}

void ConfigDialog::createPersonalWidget() {
	this->personal_top = new QWidget();
	this->createPersonalLabels();
	this->createPersonalMessageArea();
}

void ConfigDialog::createPersonalMessageArea() {
	personal_CheckBox_cmsg = new QCheckBox(personal_top);
	personal_CheckBox_cmsg->setText(i18n(QString("set \"connect message\"")));
	personal_CheckBox_cmsg->setGeometry(120, 150, 150, 20);
	personal_MultiLineEdit_ctext = new QTextEdit(personal_top);
	personal_MultiLineEdit_ctext->setGeometry(120, 170, 230, 100);

	connect(personal_CheckBox_cmsg, SIGNAL(clicked()), this, SLOT(update_widgets()));
}

void ConfigDialog::createPersonalLabels() {
	/*	QLabel* personal_Label_8 = */this->createPersonalLabel("SELCALL:", QRect(40, 110, 100, 20), true);
	/*	QLabel* personal_Label_7 =*/ this->createPersonalLabel("CALL:", QRect(40, 80, 100, 20), true);
	/*	QLabel* personal_Label_qth=*/  this->createPersonalLabel("QTH:", QRect(40, 50, 100, 20));
	/*	QLabel* personal_Label_6= */ this->createPersonalLabel("Name:", QRect(40, 20, 100, 20), true);
}

QLabel *ConfigDialog::createPersonalLabel(QString text, QRect dimension, bool nofocus) {
	QLabel *label = new QLabel(personal_top);
	label->setGeometry(dimension);
	label->setMinimumSize(0, 0);
	label->setMaximumSize(32767, 32767);

	if (nofocus) {
		label->setFocusPolicy(Qt::NoFocus);
	}

	label->setFrameStyle(0);
	label->setLineWidth(1);
	label->setMidLineWidth(0);
	label->setText(i18n(text));
	label->setMargin(-1);

	return label;
}

void ConfigDialog::createPortWidget() {
	this->port_top = new QWidget();
	this->createPortSelectionRadioButtons();
	this->createPortLineEdits();
	this->createPortLabel();
	this->createSpeedComboBox();
}

void ConfigDialog::createPortLineEdits() {
	port_LineEdit_dev = this->createLineEdit(port_top, "/dev/ttyS11", QRect(40, 120, 75, 20), 20);
	personal_LineEdit_name = this->createLineEdit(personal_top, "TUX",  QRect(120, 20, 80, 20), 20);
	personal_LineEdit_qth = this->createLineEdit(personal_top, "Nürnberg", QRect(120, 50, 80, 20), 30);
	personal_LineEdit_call = this->createLineEdit(personal_top, "DK0TUX",  QRect(120, 80, 80, 20), 12);
	personal_LineEdit_selcall = this->createLineEdit(personal_top, "DTUX", QRect(120, 110, 80, 20), 4);
}

void ConfigDialog::createSpeedComboBox() {
	port_ComboBox_speed = new QComboBox(port_top);
	port_ComboBox_speed->setGeometry(200, 50, 100, 30);
	port_ComboBox_speed->setMinimumSize(0, 0);
	port_ComboBox_speed->setMaximumSize(32767, 32767);
	port_ComboBox_speed->setFocusPolicy(Qt::StrongFocus);
	port_ComboBox_speed->addItem("9600");
	port_ComboBox_speed->addItem("19200");
	port_ComboBox_speed->addItem("38400");
	port_ComboBox_speed->addItem("57600");
	port_ComboBox_speed->addItem("115200");
	port_ComboBox_speed->setMaxCount(10);
	port_ComboBox_speed->setAutoCompletion(false);
}

void ConfigDialog::createPortSelectionRadioButtons() {
	port_ButtonGroup = new QButtonGroup(port_top);
	port_ButtonGroup->setExclusive(true);

	port_RadioButton1 = this->createRadioButton("ttyS0 (com 1)", QRect(15, 20, 125, 20));
	port_ButtonGroup->addButton(port_RadioButton1);

	port_RadioButton2 = this->createRadioButton("ttyS1 (com 2)", QRect(15, 45, 125, 20));
	port_ButtonGroup->addButton(port_RadioButton2);

	port_RadioButton3 = this->createRadioButton("ttyS2 (com 3)", QRect(15, 70, 125, 20));
	port_ButtonGroup->addButton(port_RadioButton3);

	port_RadioButton4 = this->createRadioButton("ttyS3 (com 4)", QRect(15, 95, 125, 20));
	port_ButtonGroup->addButton(port_RadioButton4);

	port_RadioButton5 = this->createRadioButton("", QRect(15, 120, 100, 20));
	port_ButtonGroup->addButton(port_RadioButton5);

	connect(port_ButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(update_widgets()));
}

void ConfigDialog::createPortLabel() {
	QLabel* port_Label_3;
	port_Label_3 = new QLabel(port_top);
	port_Label_3->setGeometry(220, 12, 100, 30);
	port_Label_3->setMinimumSize(0, 0);
	port_Label_3->setMaximumSize(32767, 32767);
	port_Label_3->setFocusPolicy(Qt::NoFocus);
	port_Label_3->setFrameStyle(0);
	port_Label_3->setLineWidth(1);
	port_Label_3->setMidLineWidth(0);;
	port_Label_3->setText(i18n("port speed"));
	port_Label_3->setMargin(-1);
}

QRadioButton *ConfigDialog::createRadioButton(QString text, QRect dimensions) {
	QRadioButton *radiobuttom = new QRadioButton(port_top);
	radiobuttom->setGeometry(dimensions);
	radiobuttom->setMinimumSize(0, 0);
	radiobuttom->setMaximumSize(32767, 32767);
	radiobuttom->setFocusPolicy(Qt::TabFocus);
	radiobuttom->setText(text);
	radiobuttom->setChecked(false);
	return radiobuttom;
}

void ConfigDialog::createPacketWidget() {
	this->packet_top = new QWidget();
}

QLineEdit *ConfigDialog::createLineEdit(QWidget *parent, QString text, QRect dimensions, int maxLength, bool strongfocus) {
	QLineEdit *edit = new QLineEdit(parent);
	edit->setGeometry(dimensions);
	edit->setMinimumSize(0, 0);
	edit->setMaximumSize(32767, 32767);

	if (strongfocus) {
		edit->setFocusPolicy(Qt::StrongFocus);
	}

	edit->setText(text);
	edit->setMaxLength(maxLength);
	edit->setFrame(QLineEdit::Normal);
	edit->setFrame(true);

	return edit;
}

void ConfigDialog::createLogOutWidget() {
	logout_top = new QWidget();
	logout_CheckBox_cmsg = new QCheckBox(logout_top);
	logout_CheckBox_cmsg->setText(i18n("set \"away message\""));
	logout_CheckBox_cmsg->setGeometry(5, 5, 150, 20);
	logout_MultiLineEdit_ctext = new QTextEdit(logout_top);
	logout_MultiLineEdit_ctext->setGeometry(150, 5, 200, 100);
	logout_CheckBox_script = new QCheckBox(logout_top);
	logout_CheckBox_script->setText(i18n("use personal logout script"));
	logout_CheckBox_script->setGeometry(5, 130, 200, 20);
	logout_LineEdit_path = new QLineEdit(logout_top);
	logout_LineEdit_path->setGeometry(5, 150, 150, 20);
	logout_PushButton_choosescript = new QPushButton(i18n("choose"), logout_top);
	logout_PushButton_choosescript->setGeometry(170, 150, 50, 20);
	connect(logout_PushButton_choosescript, SIGNAL(	clicked()), this, SLOT(chooseLogoutFile()));
	connect(logout_CheckBox_cmsg, SIGNAL(clicked(bool)), this, SLOT(update_widgets()));
	connect(logout_CheckBox_script, SIGNAL(clicked(bool)), this, SLOT(update_widgets()));
}

void ConfigDialog::createLogInWidget() {
	login_top = new QWidget();
	login_CheckBox_script = new QCheckBox(login_top);
	login_CheckBox_script->setText(i18n("use personal login script"));
	login_CheckBox_script->setGeometry(5, 5, 200, 20);
	login_LineEdit_path = new QLineEdit(login_top);
	login_LineEdit_path->setGeometry(5, 30, 150, 20);
	login_PushButton_choosescript = new QPushButton(i18n("choose"), login_top);
	login_PushButton_choosescript->setGeometry(170, 30, 50, 20);

	connect(login_PushButton_choosescript, SIGNAL(clicked()), this, SLOT(chooseLoginFile()));
	connect(login_CheckBox_script, SIGNAL(clicked()), this, SLOT(update_widgets()));
}

void ConfigDialog::selectwidget(QListWidgetItem * lbi) {
	if (lbi == NULL) {
		return;
	}
	std::array<QString, 7> possibleTextes = {"PORT", "PERSONAL", "FIX-TEXT", "PACKET", "BOX",  "LOGIN", "LOGOUT"};
	int selectedIndex = 4;

	for (auto i = 0u; i < possibleTextes.size(); ++i) {
		if (lbi->text() == possibleTextes[i]) {
			selectedIndex = i;
			break;
		}
	}

	widgetstack->setCurrentIndex(selectedIndex);
	// else
	 //qDebug () <<"item: "<< lbi->text();
}

void ConfigDialog::createFixTextWidget() {
	fixtext_top = new QWidget(0);
	list.clear();
	oblist.clear();

	QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom, fixtext_top);
	QString number;
	for (int i = 1 ; i <= 8; i++) {
		number.setNum(i);
		FixTextPicker *tp = new FixTextPicker(fixtext_top);
		//TODO
		tp->getNumLabel()->setText(number + ".");
		//tp->setNumlabelText(number + ".");
		layout->addWidget(tp);
		list.append(tp);
		oblist.append(tp->getChooseButtom());
		connect (tp->getChooseButtom(), SIGNAL (clicked()), this, SLOT (chooseFile()));
	}
	layout->activate();
}

void ConfigDialog::createBoxWidget() {
	//TODO implement more
	box_top = new QWidget();
	QLabel* box_Label;
	box_Label = new QLabel(box_top);
	box_Label->setText(" coming soon ... ");
}

void ConfigDialog::resetwidgets() {
	this->resetDeviceSelection();
	this->resetPortSpeed();
	this->resetPersonalWidget();
	this->resetLogOutWidget();
	this->resetLogInWidget();
	this->resetFixTextWidget();

	update_widgets();
}

void ConfigDialog::resetDeviceSelection() {
	QString port = configdata.getPort();
	if (port == "/dev/ttyS0") {
		port_RadioButton1->setChecked(true);
	}
	else if (port == "/dev/ttyS1") {
		port_RadioButton2->setChecked(true);
	}
	else if (port == "/dev/ttyS2") {
		port_RadioButton3->setChecked(true);
	}
	else if (port == "/dev/ttyS3") {
		port_RadioButton4->setChecked(true);
	}
	else  {
		port_RadioButton5->setChecked(true);
	}
	port_LineEdit_dev->setText(configdata.getEditPort());
}

void ConfigDialog::resetPortSpeed() {
	QString portspeed = configdata.getPortSpeed();
	constexpr std::array<int, 5> speeds = {9600, 19200, 38400, 57600,115200};
	int selectedIndex = 4;
	int speed = portspeed.toInt();
	for (size_t i = 0; i < speeds.size(); ++i) {
		if (speed == speeds[i]) {
			selectedIndex = i;
			break;
		}
	}
	port_ComboBox_speed->setCurrentIndex(selectedIndex);
}

void ConfigDialog::resetPersonalWidget() {
	personal_LineEdit_call->setText(configdata.getCall());
	personal_LineEdit_selcall->setText(configdata.getSelCall());
	personal_CheckBox_cmsg->setChecked(configdata.isCMsg());
	personal_MultiLineEdit_ctext->setText(configdata.getCMsg());
	personal_LineEdit_qth->setText(configdata.getQTH());
	personal_LineEdit_name->setText(configdata.getName());
}

void ConfigDialog::resetLogOutWidget() {
	logout_CheckBox_cmsg->setChecked(configdata.isAwayMsg());
	logout_MultiLineEdit_ctext->setText(configdata.getAwayMsg()) ;
}

void ConfigDialog::resetLogInWidget() {
	login_LineEdit_path->setText(configdata.getLoginPath());
	login_CheckBox_script->setChecked(configdata.isLoginScript());
}

void ConfigDialog::resetFixTextWidget() {
	FixTextPicker  *FixDummy;
//  for (FixDummy=list.first(); FixDummy != 0; FixDummy=list.next())   {

//		QString num;
//		num.setNum(list.findRef (FixDummy) + 1);


//		QString s = configdata.getFixLabel(num);
//		FixDummy->editlabel->setText(s);

//		s = configdata.getFixPath(num);
//		FixDummy->editpath->setText(s);

//  }
}

void ConfigDialog::writeconfig() {
	configdata.setPortSpeed(port_ComboBox_speed->currentText());
	QString port;

	if (port_RadioButton1->isChecked()) port = "/dev/ttyS0";
	else if (port_RadioButton2->isChecked()) port = "/dev/ttyS1";
	else if (port_RadioButton3->isChecked()) port = "/dev/ttyS2";
	else if (port_RadioButton4->isChecked()) port = "/dev/ttyS3";
	else port =port_LineEdit_dev->text();
	configdata.setPort(port);
	configdata.setEditPort(port_LineEdit_dev->text());

	configdata.setCall(personal_LineEdit_call->text());
	configdata.setSelCall(personal_LineEdit_selcall->text());
	configdata.setCMsg(personal_CheckBox_cmsg->isChecked(), personal_MultiLineEdit_ctext->toPlainText());
	configdata.setAwayMsg(logout_CheckBox_cmsg->isChecked(), logout_MultiLineEdit_ctext->toPlainText());
	configdata.setLogoutPath(logout_LineEdit_path->text());
	configdata.setLogoutScript(logout_CheckBox_script->isChecked());
	configdata.setLoginPath(login_LineEdit_path->text());
	configdata.setLoginScript(login_CheckBox_script->isChecked());
	configdata.setName(personal_LineEdit_name->text());
	configdata.setQTH( personal_LineEdit_qth->text());

	// fixtext
	FixTextPicker  *FixDummy;
	QString num;
	for (auto i = 0; i < list.size(); ++i)   {
		FixDummy = list[i];
	//	FixDummy=list.first(); FixDummy != 0; FixDummy=list.next();

		  FixTextPicker *next = list[i+1];
			//TODO
	//		configdata.setFixLabel(FixDummy->editlabel->text(), num);
	//		configdata.setFixPath(FixDummy->editpath->text(), num);


	}
	accept();
}

void ConfigDialog::chooseFile() {

QString s(QFileDialog::getOpenFileName(this, QString::null, ""));
	  if (s.isEmpty()) {
		return;
	}

////	for (int i = 0; i < oblist.size(); ++i) {
////		if (oblist[i] == (QObject *) sender()) {
////			break;
////		}
////	}
//	int i  = 0; // TODO
//	//TODO
//	FixTextPicker * FixDummy = &list[i];
//	QString num;
//	num.setNum(i + 1);

//	FixDummy->editpath->setText(s);
//	configdata.setFixPath(s, num);

}

void ConfigDialog::chooseLogoutFile() {
	QString s(QFileDialog::getOpenFileName(this, QString::null, ""));
	if (s.isEmpty()) {
		return;
	}
	logout_LineEdit_path->setText(s);
}

void ConfigDialog::chooseLoginFile() {
	QString s(QFileDialog::getOpenFileName(this, QString::null, ""));
	if (s.isEmpty()) {
		return;
	}
	login_LineEdit_path->setText(s);
}

void ConfigDialog::update_widgets() {
	port_LineEdit_dev->setEnabled(port_RadioButton5->isChecked());
	personal_MultiLineEdit_ctext->setEnabled( personal_CheckBox_cmsg->isChecked());
	logout_MultiLineEdit_ctext->setEnabled(logout_CheckBox_cmsg->isChecked());
	logout_LineEdit_path->setEnabled(logout_CheckBox_script->isChecked());
	login_LineEdit_path->setEnabled( login_CheckBox_script->isChecked());
}

ConfigDialog::~ConfigDialog(){
	deleteLater();
}
