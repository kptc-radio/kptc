/***************************************************************************
						  kptc.h  -  description
							 -------------------
	begin                : Wed Feb 14 2001
	copyright            : (C) 2001 by Lars Schnake
	email                : mail@lars-schnake.de

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

#ifndef KPTC_H
#define KPTC_H

#include "modem.h"
#include "modecommander.h"
#include "cwspeedwidget.h"
#include "rttyspeedwidget.h"
#include "mylineedit.h"
#include "commanddialog.h"
#include "modecommander.h"
#include "kled.h"
#include "configdialog.h"
#include "configdata.h"
#include "configmachine.h"
#include "statusinfo.h"
#include "cqdialog.h"
#include "modebuttons.h"
#include "cwspeedwidget.h"
#include "updatedialog.h"

#include <QPair>
#include <QFile>
#include <QtWidgets/QComboBox>
#include <QtGui/QKeySequence>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>

class Kptc : public QMainWindow {
	Q_OBJECT
	public:
		/** construtor */
			Kptc(QWidget* parent = 0);
		/** destructor */
		~Kptc();

	protected:
		 void resizeEvent(QResizeEvent *event) override;

	private:
		const QString standby= tr("Stand by");
		const QString qrt = tr("QRT");
		const QString changeover = tr("changeover");
		const QString cq = tr("CQ");
		const QString cqws = tr(" CQ ");
		const QString prescribe = tr("prescribe");
		const QString flush = tr("flush");
		const QString fix_speed = tr("fix speed");
		const QString speed_up = tr("speed up");
		const QString speed_down = tr("speed down");
		const QString tx_speed = tr("tx speed");
		const QString baudrate = tr("baudrate");
		int currentterm;
		// meaning of currentterm :
		// 1: (ctrl-A) : prescripe/message window
		// 2: (ctrl-B) : rec window
		// 3: (ctrl-C) : delayed echo
		bool bPromptInfoFollows;
		bool bStatusByteFollows;
		int parsePromptText;
		CommandDialog commanddialog;
		ModeCommander *modecommander;
		StatusInfo *statusinfo;
		CWSpeedWidget  *cwspeedwidget;
		RTTYSpeedWidget * rttyspeedwidget;
		QString statusmessage;
		QString prompt;
		KLed led;
		QMenu *fixmenu;
		QStatusBar *mystatusBar;
		QToolBar *modetoolbar;
		QToolBar *lefttoolbar;
		QTextEdit *termoutput;
		MyLineEdit *textedit;
		CQDialog *cqdialog;
		ModeButtons *modebuttons;
		ConfigMachine *configmachine;
		QMenu *filemenu;
		QMenu *actionmenu;
		QMenu *clearwindow;
		QMenu *optionmenu;
		QMenu *helpmenu;

		void useconfigmachine();
		void parsePrompt(const char);
		void parseStatus(const char);
		bool queryClose ();
		void expandToolBar(QString text, char *slot, QObject *obj,  QToolBar *bar);
		void initializeToolBar();
		void initializeMenues();
		void initFileMenu();
		void initOptionMenu();
		void initActionMenu();
		void initHelpMenu();
		void initFixMenu();
		void initClearwindowMenu();
		void initializeStatusBar();
		void initializeMenuBar();
		bool isendline(char c);
		bool handleFirstStart();
		void initModem();
		void initMainWindow();
		void initTextEdit();
		QString makeHTML(QString text, QString color);
		void setHTML(QString text);
		void resizeElements();

	signals:
		void htmlString(QString);

	public slots:
		void parseModemOut(unsigned char);
		void sendline(QString);
		void sendchar(unsigned char);
		void openconfigdialog();
		void openCommandDialog();
		void showPactor();
		void showAmtor();
		void showRTTY();
		void showPSK31();
		void showCW();
		void initchangeover();
		void initQRT();
		void updateStatusBar();
		void clearTrafficWindow();
		void clearEditWindow();
		void sendFixText (int);
		void shutdown();
		void showcwspeeddialog();
		void showrttyspeeddialog();
		void echoText(QString);
		void openUpdateDialog();
//		void fileQuit();
		void closeEvent(QCloseEvent* event);
};

#endif
