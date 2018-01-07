/***************************************************************************
						  configdialog.h  -  description
							 -------------------
	begin                : Wed Feb 14 2001
	copyright            : (C) 2001 by Lars Schnake
	email                : mail@lars-schnake.de

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

#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QCheckBox>
#include <qpixmap.h>
#include <QtWidgets/QLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLayout>
#include <qlist.h>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QDialog>
#include <vector>
#include <array>

#include "configdata.h"
#include "configmachine.h"
#include "fixtextpicker.h"

/**
  *@author Lars Schnake
  */

class ConfigDialog : public QDialog  {

	Q_OBJECT

	public:

		ConfigDialog(QWidget *parent=0);
		~ConfigDialog();

	private:
		QStackedWidget *widgetstack;
		QRadioButton *port_RadioButton1;
		QRadioButton *port_RadioButton2;
		QRadioButton *port_RadioButton3;
		QRadioButton *port_RadioButton4;
		QRadioButton *port_RadioButton5;
		QLineEdit *port_LineEdit_dev;
		QComboBox *port_ComboBox_speed;
		QLineEdit *personal_LineEdit_call;
		QLineEdit *personal_LineEdit_selcall;
		QLineEdit *personal_LineEdit_name;
		QLineEdit *personal_LineEdit_qth ;
		QTextEdit *personal_MultiLineEdit_ctext;
		QCheckBox *personal_CheckBox_cmsg ;
		QTextEdit *logout_MultiLineEdit_ctext;
		QCheckBox *logout_CheckBox_cmsg ;
		QList <FixTextPicker *> list;
		QList <QObject*> oblist;
		QLineEdit *logout_LineEdit_path;
		QCheckBox *logout_CheckBox_script;
		QPushButton *logout_PushButton_choosescript;
		QLineEdit *login_LineEdit_path;
		QCheckBox *login_CheckBox_script;
		QPushButton *login_PushButton_choosescript;
		QWidget *port_top;
		QWidget *personal_top;
		QWidget *box_top;
		QWidget *logout_top;
		QWidget *login_top;
		QWidget *fixtext_top;
		QWidget *packet_top;
		QButtonGroup *port_ButtonGroup;

		void initLeftsiteEntries();
		void createTopWidgets();
		void initWidgetStack();
		void createButtons();
		QPushButton *createButton(QString text, const QRect dimensions, QString text2);
		void createPortSelectionRadioButtons();
		QRadioButton *createRadioButton(QString text, QRect dimensions);
		void createPortLabel();
		QLabel *createPersonalLabel(QString text, QRect dimension, bool nofocus = false);
		void createPersonalWidget();
		void createPersonalMessageArea();
		void createPortWidget();
		void createPersonalLabels();
		void createPortLineEdits();
		QLineEdit *createLineEdit(QWidget *parent, QString text, QRect dimensions, int maxLength, bool strongfocus = true);
		void createPacketWidget();
		void createLogInWidget();
		void createLogOutWidget();
		void createFixTextWidget();
		void createBoxWidget();
		void createSpeedComboBox();
		void resetDeviceSelection();
		void resetPortSpeed();
		void resetPersonalWidget();
		void resetLogOutWidget();
		void resetLogInWidget();
		void resetFixTextWidget();

	private slots:
		void resetwidgets();
		void selectwidget(QListWidgetItem *lbi) ;
		void writeconfig();
		void chooseLogoutFile();
		void chooseLoginFile();
		void update_widgets();

	public slots:
		void chooseFile();

};

#endif
