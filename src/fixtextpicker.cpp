/***************************************************************************
						  fixtextpicker.cpp  -  description
							 -------------------
	begin				: Wed Mar 7 2001
	copyright			: (C) 2001 by Lars Schnake
	email				: lschnak@suse.de

	Ported to Qt5 by Sebastian Martin Dicke in 2017 (Sebastianmartindicke [@] gmx [.] de )
 ***************************************************************************/

/***************************************************************************
 *																		 *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or	 *
 *   (at your option) any later version.								   *
 *																		 *
 ***************************************************************************/
#include <iostream>
#include "fixtextpicker.h"

FixTextPicker::FixTextPicker(QWidget *parent ) : QWidget(parent) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	choosebutton = new QPushButton( tr("choose..."), this );
	editpath = new QLineEdit(this);
	editlabel = new QLineEdit(this);
	numlabel = new QLabel("number", this);

	QHBoxLayout *layout = new QHBoxLayout( this );
	layout->addWidget( numlabel );
	layout->addWidget( editlabel );
	layout->addWidget( editpath );
	layout->addWidget( choosebutton );
}

QPushButton *FixTextPicker::getChooseButtom() {
	//Igitt!
	return this->choosebutton;
}

QLabel *FixTextPicker::getNumLabel() {
	return this->numlabel;
}

FixTextPicker::~FixTextPicker() { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;}
