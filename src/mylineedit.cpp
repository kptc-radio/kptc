/***************************************************************************
						  mylineedit.cpp  -  description
							 -------------------
	begin                : Tue Mar 13 2001
	copyright            : (C) 2001 by lschnak@suse.de
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

#include "mylineedit.h"
#include <iostream>

MyLineEdit::MyLineEdit(QWidget *parent) : QLineEdit(parent){ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	this->setFont(QFont("courier",12,QFont::Normal));
	setReadOnly (false);
	setMinimumSize( 400, 30 );
	setMaximumSize( 1690, 30 );
	setFocusPolicy( Qt::StrongFocus );
	connect ( this, SIGNAL(returnPressed()), this, SLOT(clear()) );
	commandmode = true;
	cursorpos = 0;
	promptlength = 0;
	commandlist.clear();
	commandlist.append((""));
}

void MyLineEdit::notify(const QObject *receiver, const char *member) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	connect(this, SIGNAL(sendit(unsigned char)), receiver, member);
}

void MyLineEdit::setPrompt(QString prompt) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	qsprompt = prompt;
	setText( prompt );
	promptlength = prompt.length();
	if ( prompt == "") commandmode = false;
	else commandmode = true;
}

void MyLineEdit::keyPressEvent(QKeyEvent *ke) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	auto it = commandlist.begin();
	ke->accept();
	switch ( ke->key() ) {
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
			if (commandmode) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
				QString pprev = *(it -1);
			if (pprev.size()) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
					QString updummy = qsprompt;
					updummy.append(pprev);
					setText(updummy);
					end(false);
				}
				else commandlist.first();
			}
			return;
		case Qt::Key_Down:
			if (commandmode) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
				if ( *(it) == "" )
					return;
				QString pnext;
				pnext = *(it++);
				if (pnext.size()) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
					QString downdummy = qsprompt;
					downdummy.append(pnext);
					setText(downdummy);
					end(false);
				}
			}
			return;
		case Qt::Key_Enter:
		case Qt::Key_Return:
		if ( commandmode ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
			emit echoCommand(text().prepend("\n"));
			if (promptlength > 0) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
				QString qtext = text();
				qtext.remove(0,(promptlength));
				qtext = qtext.trimmed();
				sendit(qtext);
				commandlist.last();
			if (qtext != "") { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;commandlist.removeOne(*(it)); commandlist.append(QString(qtext)); commandlist.append(QString(""));}
			if (commandlist.count() > 10 ) commandlist.removeFirst();
					commandlist.last();
				}
			}
			setText("");
			commandmode = false;
		break;
		}

	char a = ke->key();

	if (!commandmode) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		sendit(a);
	}
	if ( ke->key() == Qt::CTRL + Qt::Key_Z || ke->key()== Qt::CTRL + Qt::Key_U ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		return;
	}
	QLineEdit::keyPressEvent(ke);       // lokale Echo ??

}


void MyLineEdit::insertChar(unsigned char c) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	myinsert(QChar(c));
}

void MyLineEdit::myinsert( QString qs) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
//TODO
	/* // get cursor position
  int line, col;
  getCursorPosition( &line, &col );

  // write text
  insertAt( qs, line, col );
	 */
}

void MyLineEdit::focusInEvent( QFocusEvent * ev )
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	QLineEdit::focusInEvent( ev );
	setCursorPosition(cursorpos);
}

void MyLineEdit::focusOutEvent ( QFocusEvent * ev )
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	cursorpos = cursorPosition();
	QLineEdit::focusInEvent( ev );
}


MyLineEdit::~MyLineEdit(){ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
}
