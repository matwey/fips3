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

#ifndef _SCROLLZOOMAREA_H
#define _SCROLLZOOMAREA_H

#include <QAbstractScrollArea>
#include <QMessageBox>

#include <fits.h>
#include <openglwidget.h>

struct ZoomParam {
	QPoint point;
	double factor;

	inline ZoomParam(const QPoint& point, double factor): point(point), factor(factor) {}
	inline ZoomParam(int x, int y, double factor): point(x, y), factor(factor) {}
};

class ScrollZoomArea: public QAbstractScrollArea {
	Q_OBJECT
public:
	ScrollZoomArea(QWidget *parent, const FITS::HeaderDataUnit& hdu);

	void zoomViewport(double zoom_factor);
	void zoomViewport(const ZoomParam& zoom);
	void fitToViewport();
	inline OpenGLWidget* viewport() const { return static_cast<OpenGLWidget*>(QAbstractScrollArea::viewport()); }
	void reloadViewport(const FITS::HeaderDataUnit& hdu);
private slots:
	inline void translatePixelViewportX(int x) { translatePixelViewport(x, viewport()->pixelViewrect().top());  }
	inline void translatePixelViewportY(int y) { translatePixelViewport(viewport()->pixelViewrect().left(), y); }
	void updateBars();
protected:
	virtual bool viewportEvent(QEvent* event) override;
private:
	void translatePixelViewport(int x, int y);
};

#endif //_SCROLLZOOMAREA_H
