/* This file is part of the KDE libraries
	 Copyright (C) 1998 Jörg Habenicht (j.habenicht@europemail.com)
	 Copyright (C) 2010 Christoph Feck <christoph@maxiom.de>

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

	 This file was originally part of the KDE libraries, but was not available in the current version in September 2017, so I separeted and changed it a bit.
	 Sebastian Martin Dicke

 */

#include "kled.h"
#include <iostream>

class KLed::Private
{
	public:
        Private() : darkFactor( 300 ), state( On ), look( Raised ), shape( Circular ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;}

		int darkFactor;
		QColor color;
		KLed::State state;
		KLed::Look look;
		KLed::Shape shape;

		QPixmap cachedPixmap[2]; // for both states
		QStyle::ControlElement ce_indicatorKLedCircular;
	QStyle::ControlElement ce_indicatorKLedRectangular;
};



KLed::KLed( QWidget *parent ) : QWidget( parent ), d( new Private )
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	setColor( Qt::green );
}


KLed::KLed( const QColor& color, QWidget *parent ) : QWidget( parent ), d( new Private )
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	setColor( color );
}

KLed::KLed( const QColor& color, State state, Look look, Shape shape, QWidget *parent ) : QWidget( parent ), d( new Private )
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	d->state = (state == Off ? Off : On);
	d->look = look;
	d->shape = shape;
	setColor( color );
	}

KLed::~KLed()
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	delete d;
}

void KLed::paintEvent( QPaintEvent* )
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
    switch( d->shape ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		case Rectangular:
            switch ( d->look ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
				case Sunken:
					paintRectFrame( false );
					break;
				case Raised:
					paintRectFrame( true );
					break;
				case Flat:
					paintRect();
					break;
			}
			break;
		case Circular:
            switch ( d->look ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
				case Flat:
					paintFlat();
					break;
				case Raised:
					paintRaised();
					break;
				case Sunken:
					paintSunken();
					break;
			}
			break;
	}
}

int KLed::KLedWidth() const
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	// Make sure the KLed is round!
	int size = qMin(width(), height());

	// leave one pixel border
	size -= 2;

	return qMax(0, size);
}

bool KLed::paintCachedPixmap()
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
    if (d->cachedPixmap[d->state].isNull()) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		return false;
	}
	QPainter painter(this);
		painter.drawPixmap(1, 1, d->cachedPixmap[d->state]);
	return true;
}

void KLed::paintFlat()
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	paintKLed(Circular, Flat);
}

void KLed::paintRaised()
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	paintKLed(Circular, Raised);
}

void KLed::paintSunken()
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	paintKLed(Circular, Sunken);
}

void KLed::paintRect()
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	paintKLed(Rectangular, Flat);
}

void KLed::paintRectFrame( bool raised )
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	paintKLed(Rectangular, raised ? Raised : Sunken);
}

KLed::State KLed::state() const
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return d->state;
}

KLed::Shape KLed::shape() const
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return d->shape;
}

QColor KLed::color() const
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return d->color;
}

KLed::Look KLed::look() const
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return d->look;
}

void KLed::setState( State state )
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
    if ( d->state == state) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		return;
	}
	d->state = (state == Off ? Off : On);
	updateCachedPixmap();
}

void KLed::setShape( Shape shape )
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
    if ( d->shape == shape ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		return;
	}

	d->shape = shape;
	updateCachedPixmap();
}

void KLed::setColor( const QColor &color )
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
    if ( d->color == color ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		return;
	}
	d->color = color;
	updateCachedPixmap();
	}

void KLed::setDarkFactor( int darkFactor )
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
    if ( d->darkFactor == darkFactor ) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	 return;
	}
	d->darkFactor = darkFactor;
	updateCachedPixmap();
}

int KLed::darkFactor() const
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return d->darkFactor;
}

void KLed::setLook( Look look )
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
    if ( d->look == look) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	 return;
	}

	d->look = look;
	updateCachedPixmap();
}

void KLed::toggle()
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	d->state = (d->state == On ? Off : On);
	updateCachedPixmap();
}

void KLed::on()
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	setState( On );
}

void KLed::off()
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	setState( Off );
}

void KLed::resizeEvent( QResizeEvent * )
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	updateCachedPixmap();
}

QSize KLed::sizeHint() const
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	QStyleOption option;
	option.initFrom(this);
	int iconSize = style()->pixelMetric(QStyle::PM_SmallIconSize, &option, this);
	return QSize( iconSize,  iconSize );
}

QSize KLed::minimumSizeHint() const
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	return QSize( 16, 16 );
}

void KLed::updateCachedPixmap()
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
	d->cachedPixmap[Off] = QPixmap();
	d->cachedPixmap[On] = QPixmap();
	update();
}

void KLed::paintKLed(Shape shape, Look look)
{ std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
    if (paintCachedPixmap()) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		 return;
	}

	QSize size(width() - 2, height() - 2);
    if (shape == Circular) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		const int width = KLedWidth();
		size = QSize(width, width);
	}
	QPointF center(size.width() / 2.0, size.height() / 2.0);
	const int smallestSize = qMin(size.width(), size.height());
	QPainter painter;

	QImage image(size, QImage::Format_ARGB32_Premultiplied);
	image.fill(0);

	QRadialGradient fillGradient(center, smallestSize / 2.0, QPointF(center.x(), size.height() / 3.0));
	const QColor fillColor = d->state != Off ? d->color : d->color.dark(d->darkFactor);
	fillGradient.setColorAt(0.0, fillColor.light(250));
	fillGradient.setColorAt(0.5, fillColor.light(130));
	fillGradient.setColorAt(1.0, fillColor);

	QConicalGradient borderGradient(center, look == Sunken ? 90 : -90);
	QColor borderColor = palette().color(QPalette::Dark);
    if (d->state == On) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		QColor glowOverlay = fillColor;
		glowOverlay.setAlpha(80);
		//borderColor = KColorUtils::overlayColors(borderColor, glowOverlay);
	}
	borderGradient.setColorAt(0.2, borderColor);
	borderGradient.setColorAt(0.5, palette().color(QPalette::Light));
	borderGradient.setColorAt(0.8, borderColor);

	painter.begin(&image);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(look == Flat ? QBrush(fillColor) : QBrush(fillGradient));
	const QBrush penBrush = (look == Flat) ? QBrush(borderColor) : QBrush(borderGradient);
	const qreal penWidth = smallestSize / 8.0;
	painter.setPen(QPen(penBrush, penWidth));
	QRectF r(penWidth / 2.0, penWidth / 2.0, size.width() - penWidth, size.height() - penWidth);
    if (shape == Rectangular) { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		painter.drawRect(r);
    } else { std::cout << __FILE__ << __FUNCTION__ << __LINE__  << std::endl;
		painter.drawEllipse(r);
	}
	painter.end();

	d->cachedPixmap[d->state] = QPixmap::fromImage(image);
	painter.begin(this);
	painter.drawPixmap(1, 1, d->cachedPixmap[d->state]);
	painter.end();
}
