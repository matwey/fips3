#include <QScrollBar>

#include <scrollarea.h>

ScrollArea::ScrollArea(QWidget *parent, FITS *fits):
		QAbstractScrollArea(parent),
		open_gl_widget_(new OpenGLWidget(viewport(), fits)) {
	// TODO: use Qt::ScrollBarAsNeeded
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

void ScrollArea::scrollContentsBy(int dx, int dy) {
	QAbstractScrollArea::scrollContentsBy(dx, dy);
	setViewportViewrect(viewportViewrect().translated(dx, dy));
}

void ScrollArea::setViewportViewrect(const QRect &viewrect) {
	open_gl_widget_->setViewrect(viewrect);
	updateBars(viewrect);
}

void ScrollArea::updateBars(const QRect &viewrect) {
	verticalScrollBar()->setPageStep(viewrect.height());
	horizontalScrollBar()->setPageStep(viewrect.width());
	verticalScrollBar()->setRange(0, open_gl_widget_->fits_size().height() - viewrect.height());
	horizontalScrollBar()->setRange(0, open_gl_widget_->fits_size().width() - viewrect.width());
//	verticalScrollBar()->setValue(viewport.top());
//	horizontalScrollBar()->setValue(viewport.left());
}