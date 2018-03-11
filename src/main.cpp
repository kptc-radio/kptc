/***************************************************************************
						  main.cpp  -  description
							 -------------------
	begin				: Wed Feb 14 2001
	copyright			: (C) 2001 by Lars Schnake
	email				: mail@lars-schnake.de

	Ported to Qt5 by Sebastian Martin Dicke in 2017 (Sebastianmartindicke [@] gmx [.] de)
 ***************************************************************************/

/***************************************************************************
 *																		 *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or	 *
 *   (at your option) any later version.								   *
 *																		 *
 ***************************************************************************/

#include <QtWidgets/QApplication>
#include <QGuiApplication>
#include <QDir>

#include "kptc.h"

static const char *description = ("Kptc - User Interface for the SCS-PTC-II");

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName("Kptc");
	QGuiApplication::setApplicationName("Kptc");
	QGuiApplication::setApplicationVersion("0.1 alpha");
	//Set the current working directory to the user home directory
	QDir::setCurrent(QDir::homePath());
	Kptc *kptc = new Kptc();
	QObject::connect(&app, &QApplication::lastWindowClosed, &app, &QApplication::quit);
	kptc->show();
	//QObject :: connect (&app, SIGNAL (lastWindowClosed ()), &app, SLOT (quit()));
	return app.exec();
//	VERSION, description, KAboutData::License_GPL,
//	"(c) 2001, Lars Schnake");
//	aboutData.addAuthor("Lars Schnake",0, "mail@lars-schnake.de");
//	KCmdLineArgs::init(argc, argv, &aboutData);
//	KCmdLineArgs::addCmdLineOptions(options); // Add our own options.
}
