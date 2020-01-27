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

	connect(&viewport()->viewrect(), SIGNAL(virtualSizeChanged(const QSize&)), this, SLOT(updateScrollBars()));
	connect(&viewport()->viewrect(), SIGNAL(virtualPosChanged(const QPoint&)), this, SLOT(setVirtualPos(const QPoint&)));
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

QPoint ScrollArea::virtualPos() const {
	const auto vsize = viewport()->viewrect().virtualSize();
	const auto psize = viewport()->size();
	const auto pw = psize.width();
	const auto ph = psize.height();
	const auto vw = vsize.width();
	const auto vh = vsize.height();
	const auto vx = -horizontalScrollBar()->value();
	const auto vy = -verticalScrollBar()->value();
	const auto ax = (pw - vw) / 2;
	const auto ay = (ph - vh) / 2;

	return QPoint{pw > vw ? ax : vx, ph > vh ? ay : vy};
}

void ScrollArea::scrollContentsBy(int dx, int dy) {
	const auto vpos = virtualPos();

	if (dx) {
		updateViewportHorizontalPosition(vpos);
	}

	if (dy) {
		updateViewportVerticalPosition(vpos);
	}
}

void ScrollArea::viewportResizeEvent(QResizeEvent* event) {
	const auto& new_viewport_size = event->size();
	const auto& old_viewport_size = event->oldSize();
	const QPoint delta_vpos{
		(new_viewport_size.width() - old_viewport_size.width()) / 2,
		(new_viewport_size.height() - old_viewport_size.height()) / 2};
	const auto new_vpos = virtualPos() + delta_vpos;

	updateScrollBars();
	setVirtualPos(new_vpos);
}

bool ScrollArea::viewportEvent(QEvent* event) {
	switch (event->type()) {
	/* The following three guys are the special beasts. */
	case QEvent::Resize:
		viewportResizeEvent(static_cast<QResizeEvent*>(event));
		return false;
	case QEvent::Paint:
	case QEvent::Wheel:
		return false;
	default:
		return QAbstractScrollArea::viewportEvent(event);
	}
}

void ScrollArea::setVirtualPos(const QPoint& vpos) {
	horizontalScrollBar()->setValue(-vpos.x());
	verticalScrollBar()->setValue(-vpos.y());
	updateViewportPosition();
}

void ScrollArea::updateScrollBars() {
	const auto vsize = viewport()->viewrect().virtualSize();
	const auto psize = viewport()->size();

	updateScrollBar(horizontalScrollBar(), psize.width(), vsize.width());
	updateScrollBar(verticalScrollBar(), psize.height(), vsize.height());
	updateViewportPosition();
}

void ScrollArea::updateScrollBar(QScrollBar* bar, int phys, int virt) {
	bar->setPageStep(phys);
	bar->setRange(0, virt - phys);
}

void ScrollArea::updateViewportPosition() const {
	const auto vpos = virtualPos();

	updateViewportHorizontalPosition(vpos);
	updateViewportVerticalPosition(vpos);
}

void ScrollArea::updateViewportHorizontalPosition(const QPoint& vpos) const {
	const auto& vx = vpos.x();
	viewport()->viewrect().setHorizontalVirtualPos(vx);
}

void ScrollArea::updateViewportVerticalPosition(const QPoint& vpos) const {
	const auto& vy = vpos.y();
	viewport()->viewrect().setVerticalVirtualPos(vy);
}
