#include <QResizeEvent>
#include <QScrollBar>

#include <scrollzoomarea.h>

ScrollZoomArea::ScrollZoomArea(QWidget *parent, FITS *fits):
	QAbstractScrollArea(parent) {

	std::unique_ptr<OpenGLWidget> open_gl_widget{new OpenGLWidget(this, fits)};
	/* setViewport promises to take ownership */
	setViewport(open_gl_widget.release());

	connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(translatePixelViewportX(int)));
	connect(verticalScrollBar(),   SIGNAL(valueChanged(int)), this, SLOT(translatePixelViewportY(int)));
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
	updateBars(new_viewrect);
}

void ScrollZoomArea::translatePixelViewport(int x, int y) {
	QRect new_pixel_viewrect(viewport()->pixelViewrect());
	new_pixel_viewrect.moveTopLeft({x, y});
	viewport()->setPixelViewrect(new_pixel_viewrect);
}

void ScrollZoomArea::updateBars(const QRectF &viewrect) {
	horizontalScrollBar()->setPageStep(viewport()->pixelViewrect().width());
	verticalScrollBar()  ->setPageStep(viewport()->pixelViewrect().height());
	horizontalScrollBar()->setRange(0, viewport()->fits_size().width()  - viewport()->pixelViewrect().width()  - 1);
	verticalScrollBar()  ->setRange(0, viewport()->fits_size().height() - viewport()->pixelViewrect().height() - 1);
	horizontalScrollBar()->setValue(viewport()->pixelViewrect().left());
	verticalScrollBar()  ->setValue(viewport()->pixelViewrect().top());
}

bool ScrollZoomArea::viewportEvent(QEvent* event) {
	return false;
}
