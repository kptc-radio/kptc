#include "dataparser.h"

DataParser::DataParser(QObject *parent) : QObject(parent) {

}

void DataParser::parsePrompt(const char c) {
	// analyze prompt
	QString modestring;
	switch(c) {
		case 32:
			modestring = ("cmd:");
			break;
		case 33:
			modestring = ("AMTOR");
			break;
		case 34:
			modestring = ("AMTOR-MONITOR");
			break;
		case 35:
			modestring = ("RTTY");
			break;
		case 36:
			modestring = ("CW");
			break;
		case 37:
			modestring = ("Pactor");
			break;
		default:
			modestring = ("??????");
			break;
	}
	emit currentmode(modestring);
}

void DataParser::parseStatus(const char c) {
	// check DIRECTION bit (SEND-LED)
	if ((c & 0x08) > 0) {
		emit direction(SEND);
//		statusinfo->setLED(true);
	}
	else {
		emit direction(RECEIVE);
//		statusinfo->setLED(false);
	}

	//TODO
	// check STATUS bits
	QString statusstring;
	switch (c & 0x07) {
		case 0:
			statusstring = "ERROR";
			break;
		case 1:
			statusstring = "REQUEST";
			break;
		case 2:
			statusstring = "TRAFFIC";
			break;
		case 3:
			statusstring = "IDLE";
			break;
		case 4:
			statusstring = "OVER";
			break;
		case 5:
			statusstring = "PHASE";
			break;
		case 6:
			statusstring = "SYNCH";
			break;
		case 7:
			statusstring = "";
			break;
		default:
			statusstring = "??????";
			break;
	}

	emit status(statusstring);
//	statusinfo->setStatus(status);

	// check MODE bits
	QString modestring;
	switch ((c & 112) >> 4) {
		case 0:
			modestring = "STAND BY";
			break;
		case 1:
			modestring = "AMTOR-ARQ";
			break;
		case 2:
			modestring = "PACTOR-ARQ";
			break;
		case 3:
			modestring = "AMTOR-FEC";
			break;
		case 4:
			modestring = "PACTOR-FEC";
			break;
		case 5:
			modestring = "RTTY / CW";
			break;
		case 6:
			modestring = "LISTEN";
			break;
		case 7:
			modestring = "channel busy";
			break;
	}

	emit mode(modestring);
//	statusinfo->setMode(mode);

	// read listen mode from status byte ??
	if (modestring == "LISTEN") {
		emit listen(true);
	}
	else {
		emit listen(false);
	}
}
