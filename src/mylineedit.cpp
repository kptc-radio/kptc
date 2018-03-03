/***************************************************************************
						  mylineedit.cpp  -  description
							 -------------------
	begin                : Tue Mar 13 2001
	copyright            : (C) 2001 by lschnak@suse.de
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

#include "mylineedit.h"
#include <iostream>

MyLineEdit::MyLineEdit(QWidget *parent) : QLineEdit(parent) {
	this->setFont(QFont("courier",12,QFont::Normal));
	setReadOnly (false);
	setMinimumSize(400, 30);
	setMaximumSize(1690, 30);
	setFocusPolicy(Qt::StrongFocus);
	connect (this, SIGNAL(returnPressed()), this, SLOT(clear()));
	commandmode = true;
	cursorpos = 0;
	promptlength = 0;
	commandlist.clear();
	commandlist.append("");
}

void MyLineEdit::notify(const QObject *receiver, const char *member) {
	connect(this, SIGNAL(sendit(unsigned char)), receiver, member);
}

void MyLineEdit::setPrompt(QString prompt) {
	qsprompt = prompt;
	setText(prompt);
	promptlength = prompt.length();
	if (prompt == "") commandmode = false;
	else commandmode = true;
}

void MyLineEdit::keyPressEvent(QKeyEvent *key) {
	auto iterator = commandlist.begin();
	key->accept();
	switch (key->key()) {
		case Qt::Key_Left:
			if (!commandmode) return;
			if (cursorPosition()<= promptlength) return;
			break;
		case Qt::Key_Right:
			if (!commandmode) return;
			break;
		case Qt::Key_Backspace:
			if (commandmode && cursorPosition()<= promptlength) return;
			break;
		case Qt::Key_Up:
			if (commandmode) {
				QString pprev = *(iterator -1);
			if (pprev.size()) {
					QString updummy = qsprompt;
					updummy.append(pprev);
					setText(updummy);
					end(false);
				}
				else commandlist.first();
			}
			return;
		case Qt::Key_Down:
			if (commandmode) {
				if (*(iterator) == "")
					return;
				QString pnext;
				pnext = *(iterator++);
				if (pnext.size()) {
					QString downdummy = qsprompt;
					downdummy.append(pnext);
					setText(downdummy);
					end(false);
				}
			}
			return;
		case Qt::Key_Enter:
		case Qt::Key_Return:
		if (commandmode) {
			//emit echoCommand(text() + "\n"); //BUG
			if (promptlength > 0) {
				QString qtext = text();
				qtext.remove(0,(promptlength));
				qtext = qtext.trimmed();
				sendit(qtext);
				commandlist.last();
			if (qtext != "") {commandlist.removeOne(*(iterator)); commandlist.append(QString(qtext)); commandlist.append(QString(""));}
			if (commandlist.count() > 10) commandlist.removeFirst();
					commandlist.last();
				}
			}
			setText("");
			commandmode = false;
		break;
		}

	char value = key->key();

	if (!commandmode) {
		sendit(value);
	}
	if (key->key() == Qt::CTRL + Qt::Key_Z || key->key()== Qt::CTRL + Qt::Key_U) {
		return;
	}
	QLineEdit::keyPressEvent(key);       // lokale Echo ??

}


void MyLineEdit::insertChar(unsigned char character) {
	myinsert(QChar(character));
}

void MyLineEdit::myinsert(QString string) {
//TODO
	/* // get cursor position
  int line, col;
  getCursorPosition(&line, &col);

  // write text
  insertAt(qs, line, col);
	 */
}

void MyLineEdit::focusInEvent(QFocusEvent * event)
{
	QLineEdit::focusInEvent(event);
	setCursorPosition(cursorpos);
}

void MyLineEdit::focusOutEvent (QFocusEvent * event)
{
	cursorpos = cursorPosition();
	QLineEdit::focusInEvent(event);
}


MyLineEdit::~MyLineEdit() {
}
