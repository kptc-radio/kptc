#include "dataparser.h"

DataParser::DataParser(QObject *parent) : QObject(parent) {
	bPromptInfoFollows = false;
	currentterm = Term::MessageWindow;
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

void DataParser::parseModemOut(unsigned char c) {
	int value = static_cast<int>(c);
	if (value == 6) {
		qDebug ()<< "Packet-STATUSINFO";
	}
	if (bStatusByteFollows) {
		bStatusByteFollows = false;
		parseStatus(c);
	}
	else if (value == 30) {
		bStatusByteFollows = true;
	}
	else if (c == 4) {
		bPromptInfoFollows = true; // command prompt info follows
	}
	else if (bPromptInfoFollows) {
		parsePrompt(c);
		bPromptInfoFollows = false;
		parsePromptText = 20;
	}
	else if (parsePromptText > 0) {
	if (value == 1) {
		parsePromptText = 0;
		currentterm = Term::MessageWindow;
		emit prompt();
	} // prompt end
		else {
			parsePromptText--;
			if (this->isendline(c)) {
				emit character(c);
			}
		}
	}
	else if (value == 3) {
			currentterm = Term::DelayedEcho;
	}	// delayed echo
	else if (value == 2) {
			currentterm = Term::RecWindow;
	}	// rx
	else if (value == 1) {
			currentterm = Term::MessageWindow;
	}	// prompt , errors , ...
	else if (value == 7) ; // klingeling :-) , changeover bell, do some ring ring here !?
	else {
		if ((currentterm == Term::RecWindow) || (currentterm == Term::DelayedEcho)) {
			QString color;
			if (currentterm == Term::DelayedEcho) {
				color = "#FF3333"; // red
			}
			else {
				color = "#336600"; // rx
			}
			emit line(c, color);
		}
		else if (currentterm == Term::MessageWindow) {
			bool endline = isendline(c);
			emit statusMessage(c, endline);
		}
	}
}

bool DataParser::isendline(char c) {
	return c == '\n' || c == '\r' ;
}
