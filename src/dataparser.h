#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <QObject>
#include <QString>

class DataParser : public QObject {
		Q_OBJECT
	public:
		enum Direction {SEND = true, RECEIVE = false};
		explicit DataParser(QObject *parent = nullptr);

	signals:
		void status(QString);
		void mode(QString);
		void currentmode(QString);
		void direction(Direction);
		void listen(bool);

	public slots:
		void parsePrompt(const char c);
		void parseStatus(const char c);
};

#endif // DATAPARSER_H
