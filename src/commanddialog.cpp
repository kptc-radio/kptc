/***************************************************************************
						  commanddialog.cpp  -  description
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

#include <iostream>
#include "commanddialog.h"

CommandDialog::CommandDialog(QWidget *parent, Qt::WindowFlags f) : QDialog (parent){
    std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	resize( 180,50 );
	setMinimumSize( 180, 50 );
	setMaximumSize( 180, 50 );

	combo = new QComboBox (this);
	combo->setGeometry( 15,10, 150, 30 );
	combo->setMinimumSize( 0, 0 );
	combo->setMaximumSize( 32767, 32767 );
	combo->setFocusPolicy( Qt::StrongFocus );
	combo->setMaxCount( 20 );
	combo->setAutoCompletion( true );
	combo->setInsertPolicy(QComboBox::InsertPolicy::InsertAtTop);

}

CommandDialog::~CommandDialog() {
    std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	deleteLater();
}

void CommandDialog::keyPressEvent(QKeyEvent *ke) {
    std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
    ke->accept();
	switch(ke->key()) {
		case QKeyEvent::Enter:   ;
		//////////////////////////////////////////////
		// send the command to the ptc:

			////qDebug() << "CommandDialog::keyEvent" <<combo->currentText();
			Modem::modem->send_esc();
			Modem::modem->writeLine(combo->currentText().toStdString().c_str());
			combo->clear();
			break;

		default: ke->ignore();
	}
}
