/***************************************************************************
						  updatedialog.cpp  -  description
							 -------------------
	begin                : Tue Mar 20 2001
	copyright            : (C) 2001 by Lars Schnake
	email                : lschnak@suse.de

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
#include <iostream>
#include "updatedialog.h"

UpdateDialog::UpdateDialog(QWidget *parent) : QDialog(parent) {
	updaterunning = false;
	update = new Update();

	this->setWindowTitle("PTC Firmware Update");
	okbutton = new QPushButton(tr("start Update"), this);
	cancelbutton = new QPushButton(tr("Exit"),this);
	lineedit = new QLineEdit(this);
	choosebutton = new QPushButton(tr("choose"),this);
	progressbar = new QProgressBar(this);
	infolabel = new QLabel(this);
	infolabel->setFrameStyle( QFrame::Panel | QFrame::Sunken );
	infolabel->setText("choose the new Firmware");

	QBoxLayout *toplayout = new QBoxLayout(QBoxLayout::TopToBottom, this);
	QBoxLayout *buttonlayout = new QBoxLayout(QBoxLayout::LeftToRight, this);
	QBoxLayout *filepicklayout = new QBoxLayout(QBoxLayout::LeftToRight, this);

	toplayout->addLayout(filepicklayout);
	toplayout->addWidget(infolabel);
	toplayout->addWidget(progressbar);
	toplayout->addLayout(buttonlayout);

	buttonlayout->addStretch();
	buttonlayout->addWidget(okbutton);
	buttonlayout->addWidget(cancelbutton);

	filepicklayout->addWidget(lineedit);
	filepicklayout->addWidget(choosebutton);

	QObject::connect (cancelbutton, SIGNAL (clicked()), this, SLOT ( reject()));
	QObject::connect (choosebutton, SIGNAL (clicked()), this, SLOT ( myfileDialog()));
	QObject::connect (okbutton, SIGNAL (clicked()), this, SLOT ( initUpdate()));
	QObject::connect (update, SIGNAL (progress(int)), progressbar, SLOT ( setProgress(int)));
	QObject::connect (update, SIGNAL (message(QString)), this, SLOT ( updateMessage(QString)));
}

void UpdateDialog::myfileDialog() {
	QString filename = QFileDialog::getOpenFileName( this, QString::null, "*.pt*\n*.pt2\n*.pte\n*", tr("choose new Firmware:") );
	if (filename == NULL) {
		return;
	}
	lineedit->setText(filename);
}

void UpdateDialog::initUpdate() {
	updaterunning = true;
	QObject::disconnect (cancelbutton, SIGNAL (clicked()), this, SLOT ( reject()));
	QObject::disconnect (choosebutton, SIGNAL (clicked()), this, SLOT ( fileDialog()));
	QObject::disconnect (okbutton, SIGNAL (clicked()), this, SLOT ( initUpdate()));

	update->runUpdate(lineedit->text());

	QObject::connect (cancelbutton, SIGNAL (clicked()), this, SLOT ( reject()));
	QObject::connect (choosebutton, SIGNAL (clicked()), this, SLOT ( fileDialog()));
	QObject::connect (okbutton, SIGNAL (clicked()), this, SLOT ( initUpdate()));

	updaterunning = false;
}

void UpdateDialog::updateMessage(QString text) {
	infolabel->setText(text);
}

void UpdateDialog::closeEvent( QCloseEvent* event )
{
	if (!updaterunning) {
		event->accept();
		return;
	}
	else {
		event->ignore();
	}
}

UpdateDialog::~UpdateDialog() {}
