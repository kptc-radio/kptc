 /* This file is part of the KDE libraries
	 Copyright (C) 1998 Jörg Habenicht (j.habenicht@europemail.com)

	 This library is free software; you can redistribute it and/or
	 modify it under the terms of the GNU Library General Public
	 License as published by the Free Software Foundation; either
	 version 2 of the License, or (at your option) any later version.

	 This library is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	 Library General Public License for more details.

	 You should have received a copy of the GNU Library General Public License
	 along with this library; see the file COPYING.LIB.  If not, write to
	 the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
	 Boston, MA 02110-1301, USA.

	 This file was originally part of the KDE libraries, but was not available in the current version in September 2017, so I separeted it.
	 Sebastian Martin Dicke
 */

#ifndef KLed_H
#define KLed_H

#include <QtGui/QPainter>
#include <QtGui/QImage>
#include <QtWidgets/QStyle>
#include <QtWidgets/QStyleOption>

class QColor;

class KLed : public QWidget {
	Q_OBJECT
	Q_ENUMS(State Shape Look)
	Q_PROPERTY(State state READ state WRITE setState)
	Q_PROPERTY(Shape shape READ shape WRITE setShape)
	Q_PROPERTY(Look look READ look WRITE setLook)
	Q_PROPERTY(QColor color READ color WRITE setColor)
	Q_PROPERTY(int darkFactor READ darkFactor WRITE setDarkFactor)

	public:
		enum State {Off, On};
		enum Shape {Rectangular, Circular};
		enum Look  {Flat, Raised, Sunken};

		explicit KLed(QWidget *parent = 0);
		explicit KLed(const QColor &color, QWidget *parent = 0);
		KLed(const QColor& color, KLed::State state, KLed::Look look, KLed::Shape shape, QWidget *parent = 0);
		~KLed();
		QColor color() const;
		State state() const;
		Look look() const;
		Shape shape() const;
		int darkFactor() const;
		void setColor(const QColor& color);
		void setState(State state);
		void setLook(Look look);
		void setShape(Shape shape);
		void setDarkFactor(int darkFactor);
		virtual QSize sizeHint() const;
		virtual QSize minimumSizeHint() const;

	public Q_SLOTS:
		void toggle();
		void on();
		void off();

	protected:
		virtual int KLedWidth() const;
		virtual void paintFlat();
		virtual void paintRaised();
		virtual void paintSunken();
		virtual void paintRect();
		virtual void paintRectFrame(bool raised);
		void paintEvent(QPaintEvent*);
		void resizeEvent(QResizeEvent*);
		bool paintCachedPixmap();
		void updateCachedPixmap();
		void paintKLed(Shape shape, Look look);

	private:
		class Private;
		Private *const d;
};

 #endif
