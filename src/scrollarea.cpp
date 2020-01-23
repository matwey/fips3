/*
 *  Copyright (C) 2017  Matwey V. Kornilov <matwey.kornilov@gmail.com>
 *                      Konstantin Malanchev <hombit@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or (at
 *  your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <QResizeEvent>
#include <QScrollBar>
#include <QMessageBox>

#include <scrollarea.h>

ScrollArea::ScrollArea(QWidget *parent, const FITS::AbstractHeaderDataUnit& hdu):
	QAbstractScrollArea(parent) {

	std::unique_ptr<OpenGLWidget> open_gl_widget{new OpenGLWidget(this, hdu)};
	/* setViewport promises to take ownership */
	setViewport(open_gl_widget.release());

	connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(translateScrollRectX(int)));
	connect(verticalScrollBar(),   SIGNAL(valueChanged(int)), this, SLOT(translateScrollRectY(int)));
	connect(&viewport()->viewrect(), SIGNAL(scrollChanged(const QRect&)), this, SLOT(updateBars()));
}

void ScrollArea::fitToViewport() {
	// Hide scroll bars while do fit
	const auto h_policy = horizontalScrollBarPolicy();
	const auto v_policy = verticalScrollBarPolicy();
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy  (Qt::ScrollBarAlwaysOff);
	viewport()->fitViewrect();
	setHorizontalScrollBarPolicy(h_policy);
	setVerticalScrollBarPolicy  (v_policy);
}

void ScrollArea::translateScrollRect(int x, int y) {
	QRect new_scroll_rect(viewport()->viewrect().scroll());
	new_scroll_rect.moveTopLeft({x, y});
	viewport()->viewrect().setScroll(new_scroll_rect);
}

void ScrollArea::updateBars() {
	const auto scroll_rect  = viewport()->viewrect().scroll();
	const auto scroll_range = viewport()->viewrect().scrollRange();
	horizontalScrollBar()->setPageStep(scroll_rect.width());
	verticalScrollBar()  ->setPageStep(scroll_rect.height());
	horizontalScrollBar()->setRange(0, scroll_range - scroll_rect.width());
	verticalScrollBar()  ->setRange(0, scroll_range - scroll_rect.height());
	horizontalScrollBar()->setValue(scroll_rect.left());
	verticalScrollBar()  ->setValue(scroll_rect.top());
}

bool ScrollArea::viewportEvent(QEvent* event) {
	switch (event->type()) {
	/* The following three guys are the special beasts. */
	case QEvent::Paint:
	case QEvent::Resize:
	case QEvent::Wheel:
		return false;
	default:
		return QAbstractScrollArea::viewportEvent(event);
	}
}
