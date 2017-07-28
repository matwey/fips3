#ifndef _SCROLLZOOMAREA_H
#define _SCROLLZOOMAREA_H

#include <QAbstractScrollArea>

#include <fits.h>
#include <openglwidget.h>

struct ZoomParam {
	QPoint point;
	double factor;

	ZoomParam(const QPoint& point, double factor):
			point(point),
			factor(factor) {}

	ZoomParam(int x, int y, double factor):
			point(x, y),
			factor(factor) {}
};

class ScrollZoomArea: public QAbstractScrollArea {
	Q_OBJECT
public:
	ScrollZoomArea(QWidget *parent, FITS* fits);

	void zoomViewport(double zoom_factor);
	void zoomViewport(const ZoomParam& zoom);
private slots:
	inline void translatePixelViewportX(int x) { translatePixelViewport(x, open_gl_widget_->pixelViewrect().top());  }
	inline void translatePixelViewportY(int y) { translatePixelViewport(open_gl_widget_->pixelViewrect().left(), y); }
private:
	std::unique_ptr<OpenGLWidget> open_gl_widget_;

	void updateBars(const QRectF &viewrect);
	void translatePixelViewport(int x, int y);
};

#endif //_SCROLLZOOMAREA_H
