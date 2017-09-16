/***************************************************************************
						  update.h  -  description
							 -------------------
	begin                : Tue Mar 20 2001
	copyright            : (C) 2001 by Peter Mack / Lars Schnake
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

#ifndef UPDATE_H
#define UPDATE_H

#include <QtWidgets/QMessageBox>

#include "modem.h"

#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

constexpr int MAX_SECT_SIZE = 512;

#define ACK '\006'
#define ESC '\033'

/**PTC Firmware Update
	*based on the update tool from Peter Mack, changes by Lars Schnake
  *@author Peter Mack / Lars Schnake
  */

/*
 * Flash Device Data
 */
typedef struct {
	char *Manufacturer;				/* Manufacturer string */
	char *Type;						/* Flash type string */
	unsigned short usSectSize;		/* Sector size of flash */
	unsigned long ulFlashSize;		/* Size of flash */
} FLASH, *PFLASH;

/*
 * 68k data structures
 */
typedef union {
	unsigned long	l;
	unsigned char	c[4];
} ULDATA;

typedef union {
	unsigned short	s;
	unsigned char	c[2];
} USDATA;

/*
 * PT2 date and time
 */
typedef struct _FDATE {
	unsigned int day = 5;
	unsigned int month = 4;
	unsigned int year = 7; /* = year - 1980 */
} FDATE;


typedef struct _FTIME {
	unsigned int twosecs = 5;
	unsigned int minutes = 6;
	unsigned int hours = 5;
} FTIME;


typedef struct _FDTIME {
	unsigned int twosecs = 5;
	unsigned int minutes = 6;
	unsigned int hours = 5;
	unsigned int day = 5;
	unsigned int month = 4;
	unsigned int year = 7; /* = year - 1980 */
} FDTIME;

class Update : public QObject  {

	Q_OBJECT

	public:
		int runUpdate(QString);

		Update(QWidget *mywidget=0);
		~Update();


	private:
		QWidget *updatewidget;
		time_t convtime(FDTIME);
		 int GetFlash (char, char, FLASH *);
		int readflush(int);

	signals:
		void progress(int);
		void message(QString);
};

#endif
