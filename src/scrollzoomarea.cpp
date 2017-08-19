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

#include <scrollzoomarea.h>

ScrollZoomArea::ScrollZoomArea(QWidget *parent, const FITS::HeaderDataUnit& hdu):
	QAbstractScrollArea(parent) {

	std::unique_ptr<OpenGLWidget> open_gl_widget{new OpenGLWidget(this, hdu)};
	/* setViewport promises to take ownership */
	setViewport(open_gl_widget.release());

	connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(translatePixelViewportX(int)));
	connect(verticalScrollBar(),   SIGNAL(valueChanged(int)), this, SLOT(translatePixelViewportY(int)));
	connect(viewport(), SIGNAL(pixelViewrectChanged(const QRect&)), this, SLOT(updateBars()));
}

void ScrollZoomArea::zoomViewport(double zoom_factor) {
	zoomViewport(ZoomParam(viewport()->rect().center(), zoom_factor));
}

void ScrollZoomArea::zoomViewport(const ZoomParam& zoom) {
	const auto old_viewrect = viewport()->viewrect();
	const auto new_size = old_viewrect.size() / zoom.factor;
	const auto new_top_left =
			old_viewrect.topLeft()
			+ QPointF(
				(old_viewrect.width() - new_size.width()) / 2,
				(old_viewrect.height() - new_size.height()) / 2
			);
	const QRectF new_viewrect(new_top_left, new_size);
	viewport()->setViewrect(new_viewrect);
}

void ScrollZoomArea::fitToViewport() {
	const auto old_viewrect = viewport()->viewrect();
	const auto new_size = old_viewrect.size().scaled(1, 1, Qt::KeepAspectRatioByExpanding);
	const QRectF new_viewrect{QPointF(0,0), new_size};

	// Hide scroll bars while do fit
	const auto h_policy = horizontalScrollBarPolicy();
	const auto v_policy = verticalScrollBarPolicy();
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy  (Qt::ScrollBarAlwaysOff);
	viewport()->setViewrect(new_viewrect);
	setHorizontalScrollBarPolicy(h_policy);
	setVerticalScrollBarPolicy  (v_policy);
}

void ScrollZoomArea::translatePixelViewport(int x, int y) {
	QRect new_pixel_viewrect(viewport()->pixelViewrect());
	new_pixel_viewrect.moveTopLeft({x, y});
	viewport()->setPixelViewrect(new_pixel_viewrect);
}

void ScrollZoomArea::updateBars() {
	horizontalScrollBar()->setPageStep(viewport()->pixelViewrect().width());
	verticalScrollBar()  ->setPageStep(viewport()->pixelViewrect().height());
	horizontalScrollBar()->setRange(0, viewport()->image_size().width()  - viewport()->pixelViewrect().width()  - 1);
	verticalScrollBar()  ->setRange(0, viewport()->image_size().height() - viewport()->pixelViewrect().height() - 1);
	horizontalScrollBar()->setValue(viewport()->pixelViewrect().left());
	verticalScrollBar()  ->setValue(viewport()->pixelViewrect().top());
}

bool ScrollZoomArea::viewportEvent(QEvent* event) {
	switch (event->type()) {
	/* The following two guys are the special beasts. */
	case QEvent::Resize:
	case QEvent::Paint:
		return false;
	default:
		return QAbstractScrollArea::viewportEvent(event);
	}
}
