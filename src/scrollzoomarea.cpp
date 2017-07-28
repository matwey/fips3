#include <QScrollBar>

#include <scrollzoomarea.h>

ScrollZoomArea::ScrollZoomArea(QWidget *parent, FITS *fits):
		QAbstractScrollArea(parent),
		open_gl_widget_(new OpenGLWidget(viewport(), fits)) {
	connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(translatePixelViewportX(int)));
	connect(verticalScrollBar(),   SIGNAL(valueChanged(int)), this, SLOT(translatePixelViewportY(int)));
}

void ScrollZoomArea::zoomViewport(double zoom_factor) {
	zoomViewport(ZoomParam(viewport()->rect().center(), zoom_factor));
}

void ScrollZoomArea::zoomViewport(const ZoomParam& zoom) {
	const auto old_viewrect = open_gl_widget_->viewrect();
	const auto new_size = old_viewrect.size() / zoom.factor;
	const auto new_top_left = old_viewrect.topLeft()
			+ QPointF(
				(old_viewrect.width() - new_size.width()) / 2,
				(old_viewrect.height() - new_size.height()) / 2
			);
	const QRectF new_viewrect(new_top_left, new_size);
	open_gl_widget_->setViewrect(new_viewrect);
	updateBars(new_viewrect);
}

void ScrollZoomArea::translatePixelViewport(int x, int y) {
	QRect new_pixel_viewrect(open_gl_widget_->pixelViewrect());
	new_pixel_viewrect.moveTopLeft({x, y});
	open_gl_widget_->setPixelViewrect(new_pixel_viewrect);
}

void ScrollZoomArea::updateBars(const QRectF &viewrect) {
	horizontalScrollBar()->setPageStep(open_gl_widget_->pixelViewrect().width());
	verticalScrollBar()  ->setPageStep(open_gl_widget_->pixelViewrect().height());
	horizontalScrollBar()->setRange(0, open_gl_widget_->fits_size().width()  - open_gl_widget_->pixelViewrect().width());
	verticalScrollBar()  ->setRange(0, open_gl_widget_->fits_size().height() - open_gl_widget_->pixelViewrect().height());
	horizontalScrollBar()->setValue(open_gl_widget_->pixelViewrect().left());
	verticalScrollBar()  ->setValue(open_gl_widget_->pixelViewrect().top());
}