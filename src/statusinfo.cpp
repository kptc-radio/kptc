/***************************************************************************
						  statusinfo.cpp  -  description
							 -------------------
	begin                : Mon Feb 19 2001
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

#include "statusinfo.h"

StatusInfo::StatusInfo(QWidget *parent) : QWidget(parent) {

	this->setFont(QFont("courier",10,QFont::Normal));
setMinimumSize( 400, 20 );

//TODO
	led = new KLed( QColor("#FF0000"), this);
	led->setGeometry ( 2, 2, 16, 16);
	led->setFixedSize(18,18);
	led->off();
	led->setToolTip(tr("SEND-LED"));
	led->setWhatsThis(tr("The Send LED show the direction\nof the information flow.\nOn means: sending text\noff means: receiving text"));

	send = new QLabel(this);
	send->setGeometry( 20, 0, 30, 20 );
	send->setFixedSize(30,20);
	send->setText("SEND");

	prompt = new QLabel(this);
	prompt->setGeometry( 50, 0, 80, 20 );
	prompt->setFixedSize(80,20);
	prompt->setFrameStyle(QFrame::Sunken | QFrame::Box);
	prompt->setToolTip(tr("prompt"));
	prompt->setWhatsThis(tr("Here is the current prompt showen."));

	mode = new QLabel(this);
	mode->setGeometry( 130, 0, 80, 20 );
	mode->setFixedSize(80,20);
	mode->setFrameStyle(QFrame::Sunken | QFrame::Box);
	mode->setText("NO RESPONSE !");
	mode->setToolTip(tr("mode"));
	mode->setWhatsThis(tr("Here are information about\nthe current mode showen."));

	status = new QLabel(this);
	status->setGeometry( 210, 0, 80, 20 );
	status->setFixedSize(80,20);
	status->setFrameStyle(QFrame::Sunken | QFrame::Box);
	status->setToolTip(tr("status"));
	status->setWhatsThis( tr("Here are some status information showen."));

	statusmessage = new QLabel(this);
	statusmessage->setMaximumHeight(20);
	statusmessage->setFrameStyle(QFrame::Sunken | QFrame::Box);
	statusmessage->setToolTip(tr("connect message"));
	statusmessage->setWhatsThis(tr("Here is the current connect message showen."));

	call = new QLabel(this);
	call->setFrameStyle(QFrame::Sunken | QFrame::Box);
	call->setToolTip(tr("call/selcall"));
	call->setWhatsThis(tr("This is your call and your Amtor Selcall\nhow you have entered it in the config dialog"));
	QHBoxLayout *layout = new QHBoxLayout (this);

	layout->addWidget (led);
	layout->addWidget (send);
	layout->addWidget (prompt);
	layout->addWidget (mode);
	layout->addWidget (status);
	layout->addWidget (statusmessage,1);
	layout->addStretch(1);
	layout->addWidget (call);

}
StatusInfo::~StatusInfo() {
}
