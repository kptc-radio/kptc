/***************************************************************************
						  cqdialog.h  -  description
							 -------------------
	begin                : Mon Feb 26 2001
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

#ifndef CQDIALOG_H
#define CQDIALOG_H

#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QPushButton>
#include <QVector>
#include <QString>
#include <QRegExp>

#include "configdata.h"
#include "modem.h"
#include "modecommander.h"

/**
  *@author Lars Schnake
  */

class CQDialog : public QObject  {

Q_OBJECT

	public:
		CQDialog(QWidget *parent=0, ModeCommander *modecommander = 0);
		~CQDialog();

	private:
		enum class Tabs {Pactor = 0, Amtor = 1, RTTY = 2, PSK31 = 3, CW = 4};

		QTabWidget *TabDialog;
		QTextEdit *CQText_Pactor ;
		QTextEdit *CQText_Amtor;
		QTextEdit *CQText_RTTY ;
		QTextEdit *CQText_PSK31 ;
		QTextEdit *CQText_CW ;
		ModeCommander *modecommander;
		Tabs currenttab;

		QString processString(QTextEdit *edit);

	public slots:
		void openDialog();
		void saveText();
		void startCall();
		void selectTab(int);
};

#endif
