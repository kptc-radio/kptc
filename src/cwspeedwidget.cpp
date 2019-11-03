/***************************************************************************
						  CWSpeedWidget.cpp  -  description
							 -------------------
	begin                : Mon Mar 12 2001
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

#include "cwspeedwidget.h"

CWSpeedWidget::CWSpeedWidget(QWidget *parent) : QDialog(parent) {
	setWindowTitle(tr("CW TX speed"));
	resize(200,80);
	setMinimumSize(200, 80);

	QVBoxLayout *vbox = new QVBoxLayout(this);
	slider.setParent(this);
	slider.setOrientation(Qt::Horizontal);
	slider.setGeometry(40,1200, 1, 160);

	lineedit.setParent(this);
	lineedit.setText("160");
	okbutton.setText(tr("Ok"));
	okbutton.setParent(this);
	vbox->addWidget(&slider);
	vbox->addWidget(&lineedit);
	vbox->addWidget(&okbutton);
	connect(&okbutton, &QPushButton::clicked, this, &CWSpeedWidget::change);
	connect(&slider, &QSlider::valueChanged, this, &CWSpeedWidget::updateText);
	connect(&lineedit, &QLineEdit::textChanged, this, &CWSpeedWidget::updateSlider);
}

void CWSpeedWidget::updateText(int i) {
	QString qi;
	qi.setNum(i);
	speed = i;
	lineedit.setText(qi);
}

void CWSpeedWidget::updateSlider(const QString &qs) {
	speed = qs.toUInt();
	slider.setValue(speed);
}

void CWSpeedWidget::change() {
	QString qs;
	qs.setNum(speed);
	qs.prepend("cws ");
	Modem::modem()->send_esc();
	Modem::modem()->writeString(qs);
	accept();
}

CWSpeedWidget::~CWSpeedWidget() {
}
