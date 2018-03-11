
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

ConfigDialog::ConfigDialog(QWidget *parent) : QDialog(parent){
	resize(500, 340);
	setMinimumSize(size());
	setMaximumSize(size());

	this->devices.reserve(4);
	this->devices.append(QString("/dev/ttyS0"));
	this->devices.append(QString("/dev/ttyS1"));
	this->devices.append(QString("/dev/ttyS2"));
	this->devices.append(QString("/dev/ttyS3"));

	this->categories.reserve(7);
	this->categories.append(QString("PORT"));
	this->categories.append(QString("PERSONAL"));
	this->categories.append(QString("FIX-TEXT"));
	this->categories.append(QString("PACKET"));
	this->categories.append(QString("BOX"));
	this->categories.append(QString("LOGIN"));
	this->categories.append(QString("LOGOUT"));

	this->createTopWidgets();
	this->initWidgetStack();
	this->initLeftsiteEntries();
	this->createButtons();
	this->resetwidgets();
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
	constexpr int width = 110;
	constexpr int height = 290;
	QListWidget* ListBox = new QListWidget(this);
	ListBox->setGeometry(10, 10, width, height);
	ListBox->setMinimumSize(width, height);
	ListBox->setMaximumSize(width, height);
	ListBox->setFocusPolicy(Qt::StrongFocus);
	ListBox->setFrameStyle(51);
	ListBox->setLineWidth(2);
	ListBox->setMidLineWidth(0);
	for (int i = 0; i < categories.size(); ++i) {
		ListBox->insertItem(i, categories[i]);
	}
	ListBox->setAutoScroll(true);

	connect(ListBox, &QListWidget::itemClicked, this, &ConfigDialog::selectwidget);
}

void ConfigDialog::createButtons() {
	constexpr int width = 90;
	constexpr int y = 310;
	QPushButton *OkButton = this->createButton("OkButton", QRect(285, y, width, standardheight), "Ok");
	QPushButton *CancelButton = this->createButton("CancelButton", QRect(395, y, width, standardheight), "Cancel");

	QObject::connect (CancelButton, &QPushButton::clicked, this, &ConfigDialog::rect);
	QObject::connect (OkButton, &QPushButton::clicked, this, &ConfigDialog::writeconfig);
}

QPushButton *ConfigDialog::createButton(QString text, const QRect dimensions, QString text2) {
	QPushButton* button = new QPushButton(text, this);
	constexpr int x = 90;
	button->setGeometry(dimensions);
	button->setMinimumSize(x, standardheight);
	button->setMaximumSize(x, standardheight);
	button->setFocusPolicy(Qt::TabFocus);
	button->setText((text2));
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
	constexpr int x = 120;
	personal_CheckBox_cmsg = new QCheckBox(personal_top);
	personal_CheckBox_cmsg->setText((QString("set \"connect message\"")));
	personal_CheckBox_cmsg->setGeometry(x, 150, 150, standardheight);
	personal_MultiLineEdit_ctext = new QTextEdit(personal_top);
	personal_MultiLineEdit_ctext->setGeometry(x, 170, 230, 100);

	connect(personal_CheckBox_cmsg, &QCheckBox::clicked, this, &ConfigDialog::update_widgets);
}

void ConfigDialog::createPersonalLabels() {
	constexpr int width = 100;
	constexpr int x = 40;
	/*	QLabel* personal_Label_8 = */this->createPersonalLabel("SELCALL:", QRect(x, 110, width, standardheight), true);
	/*	QLabel* personal_Label_7 =*/ this->createPersonalLabel("CALL:", QRect(x, 80, width, standardheight), true);
	/*	QLabel* personal_Label_qth=*/  this->createPersonalLabel("QTH:", QRect(x, 50, width, standardheight));
	/*	QLabel* personal_Label_6= */ this->createPersonalLabel("Name:", QRect(x, 20, width, standardheight), true);
}

QLabel *ConfigDialog::createPersonalLabel(QString text, QRect dimension, bool nofocus) {
	QLabel *label = new QLabel(personal_top);
	label->setGeometry(dimension);

	if (nofocus) {
		label->setFocusPolicy(Qt::NoFocus);
	}

	label->setFrameStyle(0);
	label->setLineWidth(1);
	label->setMidLineWidth(0);
	label->setText((text));
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
	constexpr int maxlength = 20;
	constexpr int length = 80;
	constexpr int x = 120;
	port_LineEdit_dev = this->createLineEdit(port_top, "/dev/ttyS11", QRect(40, 120, 75, standardheight), maxlength);
	personal_LineEdit_name = this->createLineEdit(personal_top, "TUX",  QRect(x, 20, length, standardheight), maxlength);
	personal_LineEdit_qth = this->createLineEdit(personal_top, "Nürnberg", QRect(x, 50, length, standardheight), 30);
	personal_LineEdit_call = this->createLineEdit(personal_top, "DK0TUX",  QRect(x, 80, length, standardheight), 12);
	personal_LineEdit_selcall = this->createLineEdit(personal_top, "DTUX", QRect(x, 110, length, standardheight), 4);
}

void ConfigDialog::createSpeedComboBox() {
	port_ComboBox_speed = new QComboBox(port_top);
	port_ComboBox_speed->setGeometry(200, 50, 100, 30);
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
	constexpr int x = 15;
	constexpr int width = 125;
	port_ButtonGroup = new QButtonGroup(port_top);
	port_ButtonGroup->setExclusive(true);

	port_RadioButton1 = this->createRadioButton("ttyS0 (com 1)", QRect(x, 20, width, standardheight));
	port_ButtonGroup->addButton(port_RadioButton1);

	port_RadioButton2 = this->createRadioButton("ttyS1 (com 2)", QRect(x, 45, width, standardheight));
	port_ButtonGroup->addButton(port_RadioButton2);

	port_RadioButton3 = this->createRadioButton("ttyS2 (com 3)", QRect(x, 70, width, standardheight));
	port_ButtonGroup->addButton(port_RadioButton3);

	port_RadioButton4 = this->createRadioButton("ttyS3 (com 4)", QRect(x, 95, width, standardheight));
	port_ButtonGroup->addButton(port_RadioButton4);

	port_RadioButton5 = this->createRadioButton("", QRect(x, 120, width, standardheight));
	port_ButtonGroup->addButton(port_RadioButton5);

	connect(port_ButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(update_widgets()));
}

void ConfigDialog::createPortLabel() {
	QLabel *port_Label_3 = new QLabel(port_top);
	port_Label_3->setGeometry(220, 12, 100, 30);
	port_Label_3->setFocusPolicy(Qt::NoFocus);
	port_Label_3->setFrameStyle(0);
	port_Label_3->setLineWidth(1);
	port_Label_3->setMidLineWidth(0);;
	port_Label_3->setText(tr("port speed"));
	port_Label_3->setMargin(-1);
}

QRadioButton *ConfigDialog::createRadioButton(QString text, QRect dimensions) {
	QRadioButton *radiobuttom = new QRadioButton(port_top);
	radiobuttom->setGeometry(dimensions);
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
	constexpr int x = 4;
	logout_top = new QWidget();

	logout_CheckBox_cmsg = new QCheckBox(logout_top);
	logout_CheckBox_cmsg->setText(tr("set \"away message\""));
	logout_CheckBox_cmsg->setGeometry(x, 5, 150, standardheight);

	logout_MultiLineEdit_ctext = new QTextEdit(logout_top);
	logout_MultiLineEdit_ctext->setGeometry(150, 5, 200, 100);

	logout_CheckBox_script = new QCheckBox(logout_top);
	logout_CheckBox_script->setText(tr("use personal logout script"));
	logout_CheckBox_script->setGeometry(x, 130, 200, standardheight);

	logout_LineEdit_path = new QLineEdit(logout_top);
	logout_LineEdit_path->setGeometry(x, 150, 150, standardheight);
	logout_PushButton_choosescript = new QPushButton(tr("choose"), logout_top);
	logout_PushButton_choosescript->setGeometry(170, 150, 50, standardheight);

	connect(logout_PushButton_choosescript, &QPushButton::clicked, this, &ConfigDialog::chooseLogoutFile);
	connect(logout_CheckBox_cmsg, &QCheckBox::clicked, this, &ConfigDialog::update_widgets);
	connect(logout_CheckBox_script, &QCheckBox::clicked, this, &ConfigDialog::update_widgets);
}

void ConfigDialog::createLogInWidget() {
	login_top = new QWidget();
	login_CheckBox_script = new QCheckBox(login_top);
	login_CheckBox_script->setText(tr("use personal login script"));
	login_CheckBox_script->setGeometry(5, 5, 200, standardheight);
	login_LineEdit_path = new QLineEdit(login_top);
	login_LineEdit_path->setGeometry(5, 30, 150, standardheight);
	login_PushButton_choosescript = new QPushButton(tr("choose"), login_top);
	login_PushButton_choosescript->setGeometry(170, 30, 50, standardheight);

	connect(login_PushButton_choosescript, &QPushButton::clicked, this, &ConfigDialog::chooseLoginFile);
	connect(login_CheckBox_script, &QCheckBox::clicked, this, &ConfigDialog::update_widgets);
}

void ConfigDialog::selectwidget(QListWidgetItem * lbi) {
	if (lbi == nullptr) {
		return;
	}
	int selectedIndex = 4;

	for (auto i = 0u; i < categories.size(); ++i) {
		if (lbi->text() == categories[i]) {
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
		tp->setNumlabelText(number + ".");
		layout->addWidget(tp);
		list.append(tp);
		oblist.append(tp->getChooseButtom());
		connect (tp->getChooseButtom(), &QPushButton::clicked, this, &ConfigDialog::chooseFile);
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
	if (port == devices[0]) {
		port_RadioButton1->setChecked(true);
	}
	else if (port == devices[1]) {
		port_RadioButton2->setChecked(true);
	}
	else if (port == devices[2]) {
		port_RadioButton3->setChecked(true);
	}
	else if (port == devices[3]) {
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
	//TODO

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

	if (port_RadioButton1->isChecked()) {
		port = devices[0];
	}
	else if (port_RadioButton2->isChecked()) {
		port = devices[1];
	}
	else if (port_RadioButton3->isChecked()) {
		port = devices[2];
	}
	else if (port_RadioButton4->isChecked()) {
		port = devices[3];
	}
	else {
		port =port_LineEdit_dev->text();
	}
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

		  FixTextPicker *next = list[i + 1];
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
	this->selectFile(*logout_LineEdit_path);
}

void ConfigDialog::chooseLoginFile() {
	this->selectFile(*login_LineEdit_path);
}

void ConfigDialog::selectFile(QLineEdit &edit) {
	QString string(QFileDialog::getOpenFileName(this, QString::null, ""));
	if (string.isEmpty()) {
		return;
	}
	edit.setText(string);
}

void ConfigDialog::update_widgets() {
	port_LineEdit_dev->setEnabled(port_RadioButton5->isChecked());
	personal_MultiLineEdit_ctext->setEnabled(personal_CheckBox_cmsg->isChecked());
	logout_MultiLineEdit_ctext->setEnabled(logout_CheckBox_cmsg->isChecked());
	logout_LineEdit_path->setEnabled(logout_CheckBox_script->isChecked());
	login_LineEdit_path->setEnabled(login_CheckBox_script->isChecked());
}

ConfigDialog::~ConfigDialog(){
	deleteLater();
}
