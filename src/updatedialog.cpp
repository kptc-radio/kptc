/***************************************************************************
						  updatedialog.cpp  -  description
							 -------------------
	begin                : Tue Mar 20 2001
	copyright            : (C) 2001 by Lars Schnake
	email                : lschnak@suse.de

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

#include "updatedialog.h"

UpdateDialog::UpdateDialog(QWidget *parent) : QDialog(parent) {
	updaterunning = false;
	update = new Update();

	this->setWindowTitle(tr("PTC Firmware Update"));
	this->initGUIElements();
	this->initConnections();
}

void UpdateDialog::initConnections() {
	auto fileerror = [this]  (QString string) {
		if (string.isEmpty()) {
			string = tr("ERROR: No file selected.");
		}
		else {
			string = tr("ERROR: Cannot open file: ") + string;
		}
		QMessageBox::warning(this, "Kptc", string);
	};
	auto flashinfoerror = [this] () {
		QMessageBox::warning(this, "Kptc", tr("ERROR: Receiving Flash information!"));
	};
	auto wrontsectorsize = [this] () {
		QMessageBox::warning(this, "Kptc", tr("ERROR: Wrong sector size!") );
	};
	auto wrongtimestamp = [this] () {
		QMessageBox::warning(this, "Kptc", tr("WARNING: Invalid Flash time stamp.\nPossibly no firmware installed.\nUpdate will proceed."));
	};
	auto filetoolarge = [this] (unsigned long flashFree) {
		QMessageBox::warning(this, "Kptc", tr("ERROR: File too large!\nFile should not be longer than ") + flashFree + QString(" bytes."));
	};
	auto handshakefailed = [this] () {
		QMessageBox::warning(this, "Kptc", tr("ERROR: Handshake failed!"));
	};

	QObject::connect(cancelbutton, &QPushButton::clicked, this, &UpdateDialog::reject);
	QObject::connect(choosebutton, &QPushButton::clicked, this, &UpdateDialog::myfileDialog);
	QObject::connect(okbutton, &QPushButton::clicked, this, &UpdateDialog::initUpdate);
	QObject::connect(update, &Update::progress, progressbar, &QProgressBar::setValue);
	QObject::connect(update, &Update::message, this, &UpdateDialog::updateMessage);
	QObject::connect(update, &Update::fileopenerror, this,  fileerror);
	QObject::connect(update, &Update::flashinfoerror, this, flashinfoerror );
	QObject::connect(update, &Update::wrongsectorsize, this,  wrontsectorsize);
	QObject::connect(update, &Update::wrongtimestamp, this,  wrongtimestamp);
	QObject::connect(update, &Update::filetoolarge, this,  filetoolarge);
	QObject::connect(update, &Update::handshakefailed, this,  handshakefailed);
}

void UpdateDialog::initGUIElements() {
	okbutton = new QPushButton(tr("Start Update"), this);
	cancelbutton = new QPushButton(tr("Exit"), this);
	lineedit = new QLineEdit(this);
	choosebutton = new QPushButton(tr("Choose"), this);
	progressbar = new QProgressBar(this);

	infolabel = new QLabel(this);
	infolabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	infolabel->setText(tr("Choose the new Firmware"));

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
}

void UpdateDialog::myfileDialog() {
	//"*.pt*\n*.pt2\n*.pte\n*"
	QString filter = tr("Firmware Files (*.pt* *.pt2* *.pte*)");
	QString caption = tr("Choose new Firmware");
	QString filename = QFileDialog::getOpenFileName(this, caption, "", filter);
	if (filename.isEmpty()) {
		return;
	}
	lineedit->setText(filename);
}

void UpdateDialog::initUpdate() {
	updaterunning = true;

	QObject::disconnect (cancelbutton, &QPushButton::clicked, this,  &UpdateDialog::reject);
	QObject::disconnect (choosebutton, &QPushButton::clicked, this, &UpdateDialog::myfileDialog);
	QObject::disconnect (okbutton, &QPushButton::clicked, this, &UpdateDialog::initUpdate);

	int result;
	result = update->runUpdate(lineedit->text());
	if (result == -2) {
		QMessageBox::critical(this, "Kptc", tr("ERROR: Update failed!"));
	}

	QObject::connect (cancelbutton, &QPushButton::clicked, this, &UpdateDialog::reject);
	QObject::connect (choosebutton, &QPushButton::clicked, this, &UpdateDialog::myfileDialog);
	QObject::connect (okbutton, &QPushButton::clicked, this, &UpdateDialog::initUpdate);

	updaterunning = false;
}

void UpdateDialog::updateMessage(QString text) {
	infolabel->setText(text);
}

void UpdateDialog::closeEvent(QCloseEvent* event)
{
	if (!updaterunning) {
		event->accept();
	}
	else {
		event->ignore();
	}
}

UpdateDialog::~UpdateDialog() {
	deleteLater();
}
