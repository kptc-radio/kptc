/***************************************************************************
						  statusinfo.cpp  -  description
							 -------------------
	begin                : Mon Feb 19 2001
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
#include <iostream>

#include "statusinfo.h"

StatusInfo::StatusInfo(QWidget *parent) : QStatusBar(parent) {
	this->setFocusPolicy(Qt::NoFocus);
	this->setFont(QFont("courier", 10, QFont::Normal));
	setMinimumSize(400, 20);

	//TODO
	led = new KLed(QColor("#FF0000"), this);
	led->off();
	led->setToolTip(tr("SEND-LED"));
	led->setWhatsThis(tr("The Send LED show the direction\nof the information flow.\nOn means: sending text\noff means: receiving text"));

	send = new QLabel(this);
	send->setText("SEND");

	prompt = new QLabel(this);
	prompt->setFrameStyle(QFrame::Sunken | QFrame::Box);
	prompt->setToolTip(tr("prompt"));
	prompt->setWhatsThis(tr("Here is the current prompt showen."));

	mode = new QLabel(this);
	mode->setFrameStyle(QFrame::Sunken | QFrame::Box);
	mode->setText("NO RESPONSE !");
	mode->setToolTip(tr("mode"));
	mode->setWhatsThis(tr("Here are information about\nthe current mode showen."));

	status = new QLabel(this);
	status->setFrameStyle(QFrame::Sunken | QFrame::Box);
	status->setToolTip(tr("status"));
	status->setWhatsThis(tr("Here are some status information showen."));

	statusmessage = new QLabel(this);
	statusmessage->setFrameStyle(QFrame::Sunken | QFrame::Box);
	statusmessage->setToolTip(tr("connect message"));
	statusmessage->setWhatsThis(tr("Here is the current connect message showen."));

	call = new QLabel(this);
	call->setFrameStyle(QFrame::Sunken | QFrame::Box);
	call->setToolTip(tr("call/selcall"));
	call->setWhatsThis(tr("This is your call and your Amtor Selcall\nhow you have entered it in the config dialog"));
//	QHBoxLayout layout(this);

//	layout.addWidget(led);
//	layout.addWidget(send);
//	layout.addWidget(prompt);
//	layout.addWidget(mode);
//	layout.addWidget(status);
//	layout.addWidget(statusmessage,1);
//	layout.addStretch(1);
//	layout.addWidget (call);

	this->setSize();

}

void StatusInfo::setSize() {
	QFontMetrics metric(this->font());
	constexpr int promptWidth = 80;
	constexpr int distance = 0;
	int modeWidth = metric.width(mode->text());
	int statusWidth = metric.width(mode->text());
	int statusMessageWidth = metric.width(mode->text());
	int callWidth = metric.width(call->text());
	constexpr int ledwidth = 16;
	constexpr int sendwidth = 30;
	constexpr int y = 0;
	constexpr int height = 20;
	int wholewidth = modeWidth + statusWidth + statusMessageWidth + promptWidth +
					 ledwidth + sendwidth + callWidth;
	int x = ((width() - wholewidth) / 2) + 2;
	int ledx = x + ledwidth + distance;
	int sendx = ledx + ledwidth + distance;
	int promptx = sendx + sendwidth + distance;
	int modex = promptx + promptWidth + distance;
	int statusx = modex + modeWidth + distance;
	int callx = statusx + statusWidth + distance;

	led->setGeometry (x, y, ledwidth, height);
	send->setGeometry(sendx, y, sendwidth, height);
	prompt->setGeometry(promptx, y, promptWidth, height);
	mode->setGeometry(modex, y, modeWidth, height);
	status->setGeometry(statusx, y, statusWidth, height);
	statusmessage->resize(statusMessageWidth, height);
	call->setGeometry(callx, y, callWidth, height);
}

void StatusInfo::resizeEvent(QResizeEvent *event) {
	event->accept();
	setSize();
}

void StatusInfo::setLED(bool state) {
	if (state) {
		led->on();
	}
	else {
		led->off();
	}
}

void StatusInfo::setPrompt(QString prompt) {
	this->prompt->setText(prompt);
}

void StatusInfo::setCall(QString call) {
	this->call->setText(call);
}

void StatusInfo::setStatus(QString status) {
	this->status->setText(status);
}

void StatusInfo::setMode(QString mode) {
	this->mode->setText(mode);
}

void StatusInfo::setStatusMessage(QString message) {
	this->statusmessage->setText(message);
}

StatusInfo::~StatusInfo() {

}
