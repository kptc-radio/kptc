/***************************************************************************
						  update.cpp  -  description
							 -------------------
	begin                : Tue Mar 20 2001
	copyright            : (C) 2001 by Lars Schnake
	email                : lschnak@suse.de

	Ported to Qt5 by Sebastian Martin Dicke in 2017
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "update.h"

Update::Update() : QObject() {
}

Update::~Update() {}

int Update::runUpdate(QString qsfilename ) {

	char buffer[MAX_SECT_SIZE];
	char ch;
	char manCode;
	char devID;
	FLASH flash;
	unsigned long flashFree;
	unsigned short sectors;
	unsigned long sectorsWritten = 0;
	unsigned long bytesRead;

	int hFile;
	unsigned long fileLength;

	int res;

	FDTIME fileStamp;
	FDTIME flashStamp;

	hFile = open((const char *)qsfilename.data(), O_RDONLY);

	if(-1 == hFile) {
		//QMessageBox::warning( updatewidget, "Kptc", tr("ERROR: opening file : ") + qsfilename );
		emit fileopenerror(qsfilename);
		//fprintf(stderr, "ERROR: opening file: %s\n", (const char *)qsfilename.data());
		return -1;
	}

	lseek(hFile, 12, SEEK_SET);
	read(hFile, &fileStamp, 4);

	fileLength = lseek(hFile, 0, SEEK_END);
	Modem::modem->writeLine("Q");
	usleep(200);

	Modem::modem->stopNotifier(); // close connection, skip data
	Modem::modem->writeLine("UPDATE");

	readflush(Modem::modem->getFD()); /* read and ignore the UPDATE message */

	Modem::modem->writeChar((char) 6 );	/* send ack */

	Modem::modem->rs232_read(&manCode, 1);
	Modem::modem->rs232_read(&devID, 1);
	Modem::modem->rs232_read(&flashStamp, 4);

	if (GetFlash(manCode, devID, &flash) != 0) {
		//QMessageBox::warning( updatewidget, "Kptc", tr("ERROR: receiving Flash information !") );
		emit flashinfoerror();
		// fprintf(stderr, "ERROR: receiving Flash information!\n");
		Modem::modem->send_esc();	/* send esc */
		close(hFile);
		return -1;
	}
	//qDebug() << "update::runupdate- manufaxturer, type: " << Flash.Manufacturer<<" / "<< Flash.Type;
	//printf("Flash: %s - %s\n\n", Flash.Manufacturer, Flash.Type);

	flashFree = flash.ulFlashSize - 16384 - flash.usSectSize;

	if(flash.usSectSize == 0) {
		//QMessageBox::warning( updatewidget, "Kptc", tr("ERROR: wrong sector size !") );
		emit wrongsectorsize();
		//fprintf(stderr, "ERROR: wrong sector size!\n");
		Modem::modem->send_esc();	/* send esc */
		close(hFile);
		return -1;
	}

	sectors = fileLength / flash.usSectSize;

	if(fileLength % flash.usSectSize) {
		++sectors;
	}

	if(fileLength > flashFree) {
		//QMessageBox::warning( updatewidget, "Kptc", tr("ERROR: File too large !\nFile should not be longer than ") + flashFree + QString(" bytes.") );
		emit filetoolarge(flashFree);
		//fprintf(stderr, "ERROR: File too large!\n       File should not be longer than %ld bytes.\n", FlashFree);
		Modem::modem->send_esc();	/* send esc */
		close(hFile);
		return -1;
	}

	if(flashStamp.day == 0 || flashStamp.month == 0 || (flashStamp.day == 0x1f && flashStamp.month == 0xf && flashStamp.year == 0x7f)) {
	//QMessageBox::warning( updatewidget, "Kptc", tr("WARNING: Invalid Flash time stamp.\nPossibly no firmware installed.\n Update will proceed.") );
	emit wrongtimestamp();
		//fprintf(stderr, "WARNING: Invalid Flash time stamp.\n         Possibly no firmware installed.\n\n");
	}

	if ( convtime(fileStamp) <= convtime(flashStamp)) {
		//TODO wtf?
		//qDebug() << "Update: same or newer time stamp";
		switch( QMessageBox::warning( new QWidget, "Kptc", tr("The current firmware has the same or a newer time stamp!\n"
		"Do you whant to proceed or quit the update ?\n"),
		tr("Proceed"), tr("Quit"), 0, 0, 1 )) {
			case 1: // Quit or escape
			// exit
			Modem::modem->send_esc();
			close(hFile);
			return -2;
			break;
		}
	}

	Modem::modem->writeChar((char) 6);	/* send ack */

	ch = (char) (sectors >> 8);
	Modem::modem->writeChar (ch);
	ch = (char) sectors;
	Modem::modem->writeChar(ch);

	Modem::modem->rs232_read(&ch, 1);

	if (ch != ack) {
		//QMessageBox::warning( updatewidget, "Kptc", tr("ERROR: Handshake failed !") );
		emit handshakefailed();
		//fprintf(stderr, "\a\aERROR: Handshake failed!\n");
		Modem::modem->send_esc();	/* send esc */
		close(hFile);
		return -1;
	}

	QString text(QString("Writing %1 byte in %2 sectors.").arg(fileLength).arg(sectors));
	emit message(text);
	//kapp->processEvents();
	lseek(hFile, 0, SEEK_SET);

	do {
		bytesRead = read(hFile, &buffer, flash.usSectSize);
		if(bytesRead < flash.usSectSize) {
			memset(buffer + bytesRead, 0, flash.usSectSize-bytesRead);
		}
		write(Modem::modem->getFD(), &buffer, flash.usSectSize);

		sectorsWritten++;

		Modem::modem->rs232_read(&ch, 1);
		if (ch != ack) {
			//QMessageBox::warning( new QWidget, "Kptc", tr("ERROR: Handshake failed !") );
			emit handshakefailed();
			//fprintf(stderr, "\a\aERROR: Handshake failed!\n");
			Modem::modem->send_esc();	/* send esc */
			close(hFile);
			return -1;
		}

		//sprintf(Buffer, "Written: %3ld%%\r", SectorsWritten * 100 / Sectors);
		//write(STDOUT_FILENO, Buffer, strlen(Buffer));

		emit progress((int) (sectorsWritten * 100 / sectors));
		//kapp->processEvents();
		//qDebug() << "update:sectorsWritten:" << SectorsWritten <<" / sectors total:" << Sectors;
		//qDebug ()<< "update:progress:" << (SectorsWritten * 100 / Sectors) ;
		//qDebug ()<< "update:progress(int):" << (int) (SectorsWritten * 100 / Sectors);

	} while (bytesRead == flash.usSectSize);

	Modem::modem->writeLine("");
	message(tr("Update complete. Press Exit now."));
	Modem::modem->startNotifier();
	close(hFile);
	return 0;
}

int Update::GetFlash(char manCode, char devID, FLASH *flash)
{
	int result;
	switch((unsigned char)manCode) {
		/* AMD Flash wird zwar erkannt aber für Programmierung nicht unterstützt */
		case 0x01:
			flash->Manufacturer = "AMD";
			switch((unsigned char)devID) {
				case 0xa4:
					flash->Type = "Am29F040";
					flash->usSectSize = 65535;
					flash->ulFlashSize = 524288;
					result = 0;
					break;
				default:
					flash->Type = "UNKNOWN";
					flash->usSectSize = 0;
					flash->ulFlashSize = 0;
					result = -1;
					break;
				}
				break;

		case 0x1f:
			flash->Manufacturer = "Atmel";
			switch((unsigned char)devID) {
				case 0x5d:
					flash->Type = "AT29C512";
					flash->usSectSize = 128;
					flash->ulFlashSize = 65535;
					result = 0;
					break;
				case 0xd5:
					flash->Type = "AT29C010";
					flash->usSectSize = 128;
					flash->ulFlashSize = 131072;
					result = 0;
					break;
				case 0xda:
					flash->Type = "AT29C020";
					flash->usSectSize = 256;
					flash->ulFlashSize = 262144;
					result = 0;
					break;
				case 0x5b:
					flash->Type = "AT29C040";
					flash->usSectSize = 512;
					flash->ulFlashSize = 524288;
					result = 0;
					break;
				case 0xa4:
					flash->Type = "AT29C040A";
					flash->usSectSize = 256;
					flash->ulFlashSize = 524288;
					result = 0;
					break;
				default:
					flash->Type = "UNKNOWN";
					flash->usSectSize = 0;
					flash->ulFlashSize = 0;
					result = -1;
					break;
				}
				break;

		default:
			flash->Manufacturer = "INVALID";
			flash->Type = "INVALID";
			flash->usSectSize = 0;
			flash->ulFlashSize = 0;
			result = -1;
			break;
		}

	return result;
}

time_t Update::convtime(FDTIME PTC_Time)
{
	struct tm	btime;

	btime.tm_sec = PTC_Time.twosecs * 2;
	btime.tm_min = PTC_Time.minutes;
	btime.tm_hour = PTC_Time.hours;
	btime.tm_mday = PTC_Time.day;
	btime.tm_mon = PTC_Time.month;
	btime.tm_year = PTC_Time.year + 80;
	btime.tm_wday = 0;
	btime.tm_yday = 0;
	btime.tm_isdst = 0;

	return mktime(&btime);
}

int Update::readflush(int handle)
{
	fd_set set;
	struct timeval timeout;
	int max;
	int endloop;
	int res;
	char tmp;

	max = 0;
	endloop = 0;
	do {
		timeout.tv_sec = 0;
		timeout.tv_usec = 100000; /* 0.1 seconds */

		FD_ZERO(&set);
		FD_SET(handle, &set);
		res = select(FD_SETSIZE, &set, NULL, NULL, &timeout);
		if (0 == res) {
			//fprintf(stderr, "ERROR: timed out!\n");
			endloop = 1;
		} else if (-1 == res) {
			perror("rs232_read select");
			//exit(10);
			return -1;
		} else {
			max += read(handle, &tmp, 1);

		}
	} while (!endloop);

	return res;
}
