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
	setViewportViewport( viewportViewport().translated(dx, dy) );
}

void ScrollArea::setViewportViewport(const QRect& viewport) {
	open_gl_widget_->setViewport(viewport);
	updateBars(viewport);
}

void ScrollArea::updateBars(const QRect &viewport) {
	verticalScrollBar()->setPageStep(viewport.height());
	horizontalScrollBar()->setPageStep(viewport.width());
	verticalScrollBar()->setRange(0, open_gl_widget_->fits_size().height() - viewport.height());
	horizontalScrollBar()->setRange(0, open_gl_widget_->fits_size().width() - viewport.width());
//	verticalScrollBar()->setValue(viewport.top());
//	horizontalScrollBar()->setValue(viewport.left());
}