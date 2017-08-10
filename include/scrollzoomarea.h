#ifndef _SCROLLZOOMAREA_H
#define _SCROLLZOOMAREA_H

#include <QAbstractScrollArea>

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
