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

#ifndef _SCROLLAREA_H
#define _SCROLLAREA_H

#include <QAbstractScrollArea>

#include <fits.h>
#include <openglwidget.h>

class ScrollArea: public QAbstractScrollArea {
	Q_OBJECT
public:
	ScrollArea(QWidget *parent, const FITS::AbstractHeaderDataUnit& hdu);

	void fitToViewport();
	inline OpenGLWidget* viewport() const { return static_cast<OpenGLWidget*>(QAbstractScrollArea::viewport()); }
	QPoint virtualPos() const;
protected:
	virtual void scrollContentsBy(int dx, int dy) override;
	virtual bool viewportEvent(QEvent* event) override;
	void viewportResizeEvent(QResizeEvent* event);
private slots:
	void setVirtualPos(const QPoint& vpos);
	void updateScrollBars();
	void updateScrollBar(QScrollBar* bar, int phys, int virt);
	void updateViewportPosition() const;
	void updateViewportHorizontalPosition(const QPoint& vpos) const;
	void updateViewportVerticalPosition(const QPoint& vpos) const;
};

#endif //_SCROLLAREA_H
