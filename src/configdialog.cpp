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

#define i18n

ConfigDialog::ConfigDialog(QWidget *parent, Qt::WindowFlags f) : QDialog(parent){

	this->createListBox();
	this->createButtons();
	this->createRadioButtons();
	this->createLabels();
	this->createLineEdits();

	resize( 500,340 );
	setMinimumSize( 500, 340 );
	setMaximumSize( 500, 340 );

	widgetstack= new QStackedWidget(this);
	widgetstack->setFrameStyle(QFrame::Sunken | QFrame::Box);
	widgetstack->setLineWidth(1);
	widgetstack->setMidLineWidth(0);
	widgetstack->setGeometry( 130, 10, 360, 290 );

 // ************************ port settings GUI ********************************

	personal_CheckBox_cmsg = new QCheckBox(personal_top);
	personal_CheckBox_cmsg->setText( i18n(QString("set \"connect message\"")) );
	personal_CheckBox_cmsg->setGeometry( 120, 150, 150, 20 );
	personal_MultiLineEdit_ctext = new QTextEdit(personal_top);
	personal_MultiLineEdit_ctext->setGeometry( 120, 170, 230, 100 );

	connect( personal_CheckBox_cmsg, SIGNAL(clicked()), this, SLOT( update_widgets()) );

  widgetstack->addWidget(personal_top);

	this->createBoxWidget();
	this->createLogOutWidget();
	this->createLogInWidget();

// final :
	widgetstack->setCurrentIndex(0);
	resetwidgets();
}

void ConfigDialog::createListBox() {
	QListWidget* ListBox = new QListWidget( this);
	ListBox->setGeometry( 10, 10, 110, 290 );
	ListBox->setMinimumSize( 110, 290 );
	ListBox->setMaximumSize( 110, 290 );
	ListBox->setFocusPolicy( Qt::StrongFocus );
	ListBox->setFrameStyle( 51 );
	ListBox->setLineWidth( 2 );
	ListBox->setMidLineWidth( 0 );
	ListBox->insertItem(0, "PORT" );
	ListBox->insertItem(1, "PERSONAL" );
	ListBox->insertItem(2, "FIX-TEXT" );
	ListBox->insertItem(3, "PACKET" );
	ListBox->insertItem(4, "BOX" );
	ListBox->insertItem(5,  "LOGIN" );
	ListBox->insertItem(6,  "LOGOUT" );
	ListBox->setAutoScroll( true );
	connect(ListBox, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectwidget(QListWidgetItem*)) );
}

void ConfigDialog::createButtons() {
	QPushButton *OkButton = new QPushButton( "OkButton", this );
	OkButton->setGeometry( 285, 310, 90, 20 );
	OkButton->setMinimumSize( 90, 20 );
	OkButton->setMaximumSize( 90, 20 );
	OkButton->setFocusPolicy( Qt::TabFocus );
	OkButton->setText( i18n( "Ok") );
	OkButton->setAutoRepeat( false );
	OkButton->setDefault( false );
	OkButton->setAutoDefault( false );

	QPushButton* CancelButton = new QPushButton("CancelButton", this );
	CancelButton->setGeometry( 395, 310, 90, 20 );
	CancelButton->setMinimumSize( 90, 20 );
	CancelButton->setMaximumSize( 90, 20 );
	CancelButton->setFocusPolicy( Qt::TabFocus );
	CancelButton->setText( i18n("Cancel") );
	CancelButton->setAutoRepeat( false );
	CancelButton->setDefault( false );
	CancelButton->setAutoDefault( false );

	QObject::connect (CancelButton, SIGNAL (clicked()), this, SLOT ( reject()));
	QObject::connect (OkButton, SIGNAL(clicked(bool) ), this, SLOT ( writeconfig()));
}

void ConfigDialog::createRadioButtons() {
	port_top = new QWidget();

	port_ButtonGroup_1 = new QButtonGroup( port_top );
	port_ButtonGroup_1->setExclusive( true );

	port_RadioButton1 = new QRadioButton();
	port_ButtonGroup_1->addButton(port_RadioButton1);
	port_RadioButton1->setGeometry( 15, 20, 100, 20 );
	port_RadioButton1->setMinimumSize( 0, 0 );
	port_RadioButton1->setMaximumSize( 32767, 32767 );
	port_RadioButton1->setFocusPolicy( Qt::TabFocus );
	port_RadioButton1->setText( "ttyS0 (com 1)" );
	port_RadioButton1->setAutoRepeat( false );
	port_RadioButton1->setChecked( false );

	port_RadioButton2 = new QRadioButton();
	port_ButtonGroup_1->addButton(port_RadioButton2);
	port_RadioButton2->setGeometry( 15, 45, 100, 20 );
	port_RadioButton2->setMinimumSize( 0, 0 );
	port_RadioButton2->setMaximumSize( 32767, 32767 );
	port_RadioButton2->setFocusPolicy( Qt::TabFocus );
	port_RadioButton2->setText( "ttyS1 (com 2)" );
	port_RadioButton2->setAutoRepeat( false );
	port_RadioButton2->setChecked( false );

	port_RadioButton3 = new QRadioButton();
	port_ButtonGroup_1 ->addButton(port_RadioButton3);
	port_RadioButton3->setGeometry( 15, 70, 100, 20 );
	port_RadioButton3->setMinimumSize( 0, 0 );
	port_RadioButton3->setMaximumSize( 32767, 32767 );
	port_RadioButton3->setFocusPolicy( Qt::TabFocus );
	port_RadioButton3->setText( "ttyS2 (com 3)" );
	port_RadioButton3->setChecked( false );

	port_RadioButton4 = new QRadioButton();
	port_ButtonGroup_1->addButton(port_RadioButton4);
	port_RadioButton4->setGeometry( 15, 95, 100, 20 );
	port_RadioButton4->setMinimumSize( 0, 0 );
	port_RadioButton4->setMaximumSize( 32767, 32767 );
	port_RadioButton4->setFocusPolicy( Qt::TabFocus );
	port_RadioButton4->setText( "ttyS3 (com 4)" );
	port_RadioButton4->setAutoRepeat( false );
	port_RadioButton4->setChecked( false );

	port_RadioButton5 = new QRadioButton();
	port_ButtonGroup_1 ->addButton(port_RadioButton5);
	port_RadioButton5->setGeometry( 15, 120, 100, 20 );
	port_RadioButton5->setMinimumSize( 0, 0 );
	port_RadioButton5->setMaximumSize( 32767, 32767 );
	port_RadioButton5->setFocusPolicy( Qt::TabFocus );
	port_RadioButton5->setChecked( false );
}

void ConfigDialog::createLabels() {
	QLabel* port_Label_3;
	port_Label_3 = new QLabel( port_top);
	port_Label_3->setGeometry( 220, 20, 100, 30 );
	port_Label_3->setMinimumSize( 0, 0 );
	port_Label_3->setMaximumSize( 32767, 32767 );
	port_Label_3->setFocusPolicy( Qt::NoFocus );
	port_Label_3->setFrameStyle( 0 );
	port_Label_3->setLineWidth( 1 );
	port_Label_3->setMidLineWidth( 0 );;
	port_Label_3->setText( i18n( "port speed") );
	port_Label_3->setMargin( -1 );

	port_ComboBox_speed = new QComboBox(port_top);
	port_ComboBox_speed->setGeometry( 200, 50, 100, 30 );
	port_ComboBox_speed->setMinimumSize( 0, 0 );
	port_ComboBox_speed->setMaximumSize( 32767, 32767 );
	port_ComboBox_speed->setFocusPolicy( Qt::StrongFocus );
	port_ComboBox_speed->addItem( "9600" );
	port_ComboBox_speed->addItem( "19200" );
	port_ComboBox_speed->addItem( "38400" );
	port_ComboBox_speed->addItem( "57600" );
	port_ComboBox_speed->addItem( "115200" );
	port_ComboBox_speed->setMaxCount( 10 );
	port_ComboBox_speed->setAutoCompletion( false );

	port_ButtonGroup_1->addButton(port_RadioButton1 );
	port_ButtonGroup_1->addButton( port_RadioButton2 );
	port_ButtonGroup_1->addButton( port_RadioButton3 );
	port_ButtonGroup_1->addButton( port_RadioButton4 );

	connect( port_ButtonGroup_1, SIGNAL(buttonClicked(int) ), this, SLOT( update_widgets()) );

	widgetstack->addWidget(port_top);

	personal_top = new QWidget();

	QLabel* personal_Label_8;
	personal_Label_8 = new QLabel( personal_top);
	personal_Label_8->setGeometry( 40, 110, 100, 20 );
	personal_Label_8->setMinimumSize( 0, 0 );
	personal_Label_8->setMaximumSize( 32767, 32767 );
	personal_Label_8->setFocusPolicy( Qt::NoFocus );
	personal_Label_8->setFrameStyle( 0 );
	personal_Label_8->setLineWidth( 1 );
	personal_Label_8->setMidLineWidth( 0 );
	personal_Label_8->setText( "SELCALL:" );
	personal_Label_8->setMargin( -1 );

	QLabel* personal_Label_7;
	personal_Label_7 = new QLabel( personal_top );
	personal_Label_7->setGeometry( 40, 80, 100, 20 );
	personal_Label_7->setMinimumSize( 0, 0 );
	personal_Label_7->setMaximumSize( 32767, 32767 );
	personal_Label_7->setFocusPolicy( Qt::NoFocus );
	personal_Label_7->setFrameStyle( 0 );
	personal_Label_7->setLineWidth( 1 );
	personal_Label_7->setMidLineWidth( 0 );
	personal_Label_7->setText( "CALL:" );
	personal_Label_7->setMargin( -1 );

	QLabel* personal_Label_qth;
	personal_Label_qth = new QLabel( personal_top);
	personal_Label_qth->setGeometry( 40, 50, 100, 20 );
	personal_Label_qth->setFrameStyle( 0 );
	personal_Label_qth->setLineWidth( 1 );
	personal_Label_qth->setMidLineWidth( 0 );
	personal_Label_qth->setText( "QTH:" );
	personal_Label_8->setMargin( -1 );

	QLabel* personal_Label_6;
	personal_Label_6 = new QLabel( personal_top );
	personal_Label_6->setGeometry( 40, 20, 100, 20 );
	personal_Label_6->setMinimumSize( 0, 0 );
	personal_Label_6->setMaximumSize( 32767, 32767 );
	personal_Label_6->setFocusPolicy( Qt::NoFocus );
	personal_Label_6->setFrameStyle( 0 );
	personal_Label_6->setLineWidth( 1 );
	personal_Label_6->setMidLineWidth( 0 );
	personal_Label_6->setText( i18n("Name:") );
	personal_Label_6->setMargin( -1 );
}

void ConfigDialog::createLineEdits() {
	port_LineEdit_dev = new QLineEdit( port_top );
	port_LineEdit_dev->setGeometry( 55, 140, 75, 20 );
	port_LineEdit_dev->setMinimumSize( 0, 0 );
	port_LineEdit_dev->setMaximumSize( 32767, 32767 );
	port_LineEdit_dev->setFocusPolicy( Qt::StrongFocus );
	port_LineEdit_dev->setText( "/dev/ttyS11" );
	port_LineEdit_dev->setMaxLength( 20 );
	port_LineEdit_dev->setFrame( QLineEdit::Normal );
	port_LineEdit_dev->setFrame( true );

	personal_LineEdit_name = new QLineEdit( personal_top);
	personal_LineEdit_name->setGeometry( 120, 20, 80, 20 );
	personal_LineEdit_name->setMinimumSize( 0, 0 );
	personal_LineEdit_name->setMaximumSize( 32767, 32767 );
	personal_LineEdit_name->setFocusPolicy( Qt::StrongFocus );
	personal_LineEdit_name->setText( "TUX" );
	personal_LineEdit_name->setMaxLength( 20 );
	personal_LineEdit_name->setFrame( QLineEdit::Normal );
	personal_LineEdit_name->setFrame( true );

	personal_LineEdit_qth = new QLineEdit( personal_top);
	personal_LineEdit_qth->setGeometry( 120, 50, 80, 20 );
	personal_LineEdit_qth->setMinimumSize( 0, 0 );
	personal_LineEdit_qth->setMaximumSize( 32767, 32767 );
	personal_LineEdit_qth->setText( "Nürnberg" );
	personal_LineEdit_qth->setMaxLength( 30 );
	personal_LineEdit_qth->setFrame( QLineEdit::Normal );
	personal_LineEdit_qth->setFrame( true );

	personal_LineEdit_call = new QLineEdit( personal_top );
	personal_LineEdit_call->setGeometry( 120, 80, 80, 20 );
	personal_LineEdit_call->setMinimumSize( 0, 0 );
	personal_LineEdit_call->setMaximumSize( 32767, 32767 );
	personal_LineEdit_call->setFocusPolicy( Qt::StrongFocus );
	personal_LineEdit_call->setText( "DK0TUX" );
	personal_LineEdit_call->setMaxLength( 12 );
	personal_LineEdit_call->setFrame( QLineEdit::Normal );
	personal_LineEdit_call->setFrame( true );

	personal_LineEdit_selcall = new QLineEdit( personal_top );
	personal_LineEdit_selcall->setGeometry( 120, 110, 80, 20 );
	personal_LineEdit_selcall->setMinimumSize( 0, 0 );
	personal_LineEdit_selcall->setMaximumSize( 32767, 32767 );
	personal_LineEdit_selcall->setFocusPolicy( Qt::StrongFocus );
	personal_LineEdit_selcall->setText( "DTUX" );
	personal_LineEdit_selcall->setMaxLength( 4 );
	personal_LineEdit_selcall->setFrame( QLineEdit::Normal );
	personal_LineEdit_selcall->setFrame( true );
}

void ConfigDialog::createLogOutWidget() {
	QWidget * logout_top = new QWidget();
	logout_CheckBox_cmsg = new QCheckBox(logout_top);
	logout_CheckBox_cmsg->setText( i18n("set \"away message\"") );
	logout_CheckBox_cmsg->setGeometry( 5, 5, 150, 20 );
	logout_MultiLineEdit_ctext = new QTextEdit(logout_top);
	logout_MultiLineEdit_ctext->setGeometry( 150, 5, 200, 100 );
	logout_CheckBox_script = new QCheckBox(logout_top);
	logout_CheckBox_script->setText( i18n("use personal logout script") );
	logout_CheckBox_script->setGeometry( 5, 130, 200, 20 );
	logout_LineEdit_path = new QLineEdit(logout_top);
	logout_LineEdit_path->setGeometry( 5, 150, 150, 20 );
	logout_PushButton_choosescript = new QPushButton(i18n("choose"), logout_top);
	logout_PushButton_choosescript->setGeometry( 170, 150, 50, 20 );
	connect( logout_PushButton_choosescript, SIGNAL(	clicked() ), this, SLOT( chooseLogoutFile()) );
	connect( logout_CheckBox_cmsg, SIGNAL(clicked(bool)), this, SLOT( update_widgets()) );
	connect( logout_CheckBox_script, SIGNAL(clicked(bool)), this, SLOT( update_widgets()) );
	widgetstack->addWidget(logout_top);
}

void ConfigDialog::createLogInWidget() {
	QWidget * login_top = new QWidget();
	login_CheckBox_script = new QCheckBox(login_top);
	login_CheckBox_script->setText( i18n("use personal login script") );
	login_CheckBox_script->setGeometry( 5, 5, 200, 20 );
	login_LineEdit_path = new QLineEdit(login_top);
	login_LineEdit_path->setGeometry( 5, 30, 150, 20 );
	login_PushButton_choosescript = new QPushButton(i18n("choose"), login_top);
	login_PushButton_choosescript->setGeometry( 170, 30, 50, 20 );
	connect( login_PushButton_choosescript, SIGNAL(	clicked() ), this, SLOT( chooseLoginFile()) );
	connect( login_CheckBox_script, SIGNAL(clicked()), this, SLOT( update_widgets()) );
	widgetstack->addWidget(login_top);
}

void ConfigDialog::selectwidget( QListWidgetItem * lbi) {
	if (lbi == NULL){
		return;
	}
	if (lbi->text() == "PORT")  widgetstack->setCurrentIndex(0);
	else if (lbi->text() == "PERSONAL")  widgetstack->setCurrentIndex(1);
	else if (lbi->text() == "LOGOUT")  widgetstack->setCurrentIndex(2);
	else if (lbi->text() == "FIX-TEXT")  widgetstack->setCurrentIndex(3);
	else if (lbi->text() == "BOX")  widgetstack->setCurrentIndex(4);
	else if (lbi->text() == "LOGIN") widgetstack->setCurrentIndex(5);
	else  widgetstack->setCurrentIndex(4);
	// else
	 //qDebug () <<"item: "<< lbi->text();
}

void ConfigDialog::createFixTextWidget() {
	QWidget * fixtext_top = new QWidget(0);

	list.clear();
	oblist.clear();

	QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom, fixtext_top);

	QString number;
	for ( int i = 1 ; i <= 8; i++ ) {
		number.setNum(i);
		FixTextPicker *tp = new FixTextPicker(fixtext_top);
		tp->setNumlabelText( number + ".");
		layout->addWidget( tp );
		list.append( tp );
		oblist.append( *tp->choosebutton );
		connect ( tp->choosebutton, SIGNAL (clicked()), this, SLOT ( chooseFile()));
	}
		layout->activate();
		widgetstack->addWidget(fixtext_top);
}

void ConfigDialog::createBoxWidget() {
	QWidget * box_top = new QWidget();
	QLabel* box_Label;
	box_Label = new QLabel( box_top);
	box_Label->setText(" coming soon ... ");
	widgetstack->addWidget(box_top);
}

void ConfigDialog::resetwidgets() {
	QString port = configdata.getPort();
	if ( port == "/dev/ttyS0" ) port_RadioButton1->setChecked( true );
	else if ( port == "/dev/ttyS1" ) port_RadioButton2->setChecked( true );
	else if ( port == "/dev/ttyS2" ) port_RadioButton3->setChecked( true );
	else if ( port == "/dev/ttyS3" ) port_RadioButton4->setChecked( true );
	else  port_RadioButton5->setChecked( true );
	port_LineEdit_dev->setText(configdata.getEditPort());

	QString portspeed = configdata.getPortSpeed();

	if ( portspeed == "9600" ) port_ComboBox_speed->setCurrentIndex(0);
	else if ( portspeed == "19200" ) port_ComboBox_speed->setCurrentIndex(1);
	else if ( portspeed == "38400" ) port_ComboBox_speed->setCurrentIndex(2);
	else if ( portspeed == "57600" ) port_ComboBox_speed->setCurrentIndex(3);
	else if ( portspeed == "115200" ) port_ComboBox_speed->setCurrentIndex(4);
	else  port_ComboBox_speed->setCurrentIndex(4);

	// reset personal widget
	personal_LineEdit_call->setText(configdata.getCall());
	personal_LineEdit_selcall->setText(configdata.getSelCall());
	personal_CheckBox_cmsg->setChecked( configdata.isCMsg() );
	personal_MultiLineEdit_ctext->setText( configdata.getCMsg() );
	personal_LineEdit_qth->setText(configdata.getQTH());
	personal_LineEdit_name->setText(configdata.getName());

	// reset logout widget
	logout_CheckBox_cmsg->setChecked( configdata.isAwayMsg() );
	logout_MultiLineEdit_ctext->setText( configdata.getAwayMsg() ) ;

	logout_LineEdit_path->setText( configdata.getLogoutPath() );
	logout_CheckBox_script->setChecked( configdata.isLogoutScript());

	// reset login widget
	login_LineEdit_path->setText( configdata.getLoginPath() );
	login_CheckBox_script->setChecked( configdata.isLoginScript());

	// reset fix text widget
	FixTextPicker  *FixDummy;
//  for ( FixDummy=list.first(); FixDummy != 0; FixDummy=list.next() )   {

//		QString num;
//		num.setNum( list.findRef ( FixDummy ) + 1 );


//		QString s = configdata.getFixLabel( num );
//		FixDummy->editlabel->setText( s );

//		s = configdata.getFixPath( num );
//		FixDummy->editpath->setText( s );

//  }
	update_widgets();
}

void ConfigDialog::writeconfig() {
	configdata.setPortSpeed( port_ComboBox_speed->currentText() );
	QString port;

	if (port_RadioButton1->isChecked()) port = "/dev/ttyS0";
	else if (port_RadioButton2->isChecked()) port = "/dev/ttyS1";
	else if (port_RadioButton3->isChecked()) port = "/dev/ttyS2";
	else if (port_RadioButton4->isChecked()) port = "/dev/ttyS3";
	else port =port_LineEdit_dev->text();
	configdata.setPort(port);
	configdata.setEditPort(port_LineEdit_dev->text());

	configdata.setCall( personal_LineEdit_call->text());
	configdata.setSelCall( personal_LineEdit_selcall->text());
	configdata.setCMsg( personal_CheckBox_cmsg->isChecked(), personal_MultiLineEdit_ctext->toPlainText() );
	configdata.setAwayMsg( logout_CheckBox_cmsg->isChecked(), logout_MultiLineEdit_ctext->toPlainText() );
	configdata.setLogoutPath( logout_LineEdit_path->text());
	configdata.setLogoutScript( logout_CheckBox_script->isChecked() );
	configdata.setLoginPath( login_LineEdit_path->text());
	configdata.setLoginScript( login_CheckBox_script->isChecked() );
	configdata.setName( personal_LineEdit_name->text() );
	configdata.setQTH(  personal_LineEdit_qth->text() );

	// fixtext
//  FixTextPicker  *FixDummy;
//	QString num;
//  for ( int i = 0; i < list.size(); ++i )   {
//      FixDummy = &list[i];
//    //FixDummy=list.first(); FixDummy != 0; FixDummy=list.next()

//      FixTextPicker *next = &list[i+1];
//        //TODO
//        //configdata.setFixLabel( FixDummy->editlabel->text(), num );
//        //configdata.setFixPath( FixDummy->editpath->text(), num );


//  }
	accept();
}

void ConfigDialog::chooseFile() {
//    QString s( QFileDialog::getOpenFileName(this, QString::null, "" ) );
//  if ( s.isEmpty() )
//        return;

////    for (int i = 0; i < oblist.size(); ++i) {
////        if (oblist[i] == (QObject *) sender()) {
////            break;
////        }
////    }
//    int i  = 0; // TODO
//    //TODO
//    FixTextPicker * FixDummy = &list[i];
//	QString num;
//	num.setNum( i + 1 );

//	FixDummy->editpath->setText( s );
//	configdata.setFixPath( s, num);

}

void ConfigDialog::chooseLogoutFile() {
	QString s( QFileDialog::getOpenFileName(this, QString::null, "" ) );
	if ( s.isEmpty() ) {
		return;
	}
	logout_LineEdit_path->setText(s);
}

void ConfigDialog::chooseLoginFile() {
	QString s( QFileDialog::getOpenFileName(this, QString::null, "" ) );
	if ( s.isEmpty() ) {
		return;
	}
	login_LineEdit_path->setText(s);
}


void ConfigDialog::update_widgets() {
	port_LineEdit_dev->setEnabled( port_RadioButton5->isChecked() );
	personal_MultiLineEdit_ctext->setEnabled(  personal_CheckBox_cmsg->isChecked() );
	logout_MultiLineEdit_ctext->setEnabled( logout_CheckBox_cmsg->isChecked() );
	logout_LineEdit_path->setEnabled( logout_CheckBox_script->isChecked() );
	login_LineEdit_path->setEnabled(  login_CheckBox_script->isChecked() );
}


ConfigDialog::~ConfigDialog(){
	deleteLater();
}
