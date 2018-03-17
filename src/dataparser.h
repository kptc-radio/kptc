#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <QObject>
#include <QString>
#include <QDebug>

class DataParser : public QObject {
		Q_OBJECT
	public:
		enum Direction {SEND = true, RECEIVE = false};
		explicit DataParser(QObject *parent = nullptr);

	private:
		enum class Term {MessageWindow, RecWindow, DelayedEcho};

		bool bPromptInfoFollows;
		bool bStatusByteFollows;
		int parsePromptText;
		Term currentterm;
		// meaning of currentterm :
		// 1: (ctrl-A) : prescripe/message window
		// 2: (ctrl-B) : rec window
		// 3: (ctrl-C) : delayed echo

		void parsePrompt(const char c);
		void parseStatus(const char c);
		bool isendline(char c);

	signals:
		void status(QString);
		void mode(QString);
		void currentmode(QString);
		void direction(Direction);
		void listen(bool);
		void prompt();
		void character(char);
		void line(char, QString);
		void statusMessage(char, bool);

	public slots:
		void parseModemOut(unsigned char c);
};

#endif // DATAPARSER_H
