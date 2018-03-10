/***************************************************************************
						  updatedialog.h  -  description
							 -------------------
	begin                : Tue Mar 20 2001
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

#ifndef UPDATEDIALOG_H
#define UPDATEDIALOG_H

#include <QtGui/QCloseEvent>
#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QLayout>
#include "update.h"

/**
  *@author Lars Schnake
  */

class UpdateDialog : public QDialog  {

	Q_OBJECT

	public:
		UpdateDialog(QWidget *parent = 0);
		~UpdateDialog();

	private:
		bool updaterunning;
		QPushButton *okbutton;
		QPushButton *cancelbutton;
		QLineEdit *lineedit;
		QPushButton *choosebutton;
		QProgressBar *progressbar;
		QLabel *infolabel;
		Update *update;

		void initConnections();
		void initGUIElements();

	protected:
		void closeEvent(QCloseEvent*);

	private slots:
		void myfileDialog();
		void initUpdate();
		void updateMessage(QString);
};

#endif
