#ifndef _SCROLLAREA_H
#define _SCROLLAREA_H

#include <QAbstractScrollArea>

#include <fits.h>
#include <openglwidget.h>

class ScrollArea: public QAbstractScrollArea {
protected:
	virtual void scrollContentsBy(int dx, int dy) override;
public:
	ScrollArea(QWidget *parent, FITS* fits);
	void setViewportViewrect(const QRect &viewrect);
	inline const QRect& viewportViewrect() const { return open_gl_widget_->viewrect(); }
private:
	std::unique_ptr<OpenGLWidget> open_gl_widget_;

	void updateBars(const QRect& viewrect);
};

#endif //_SCROLLAREA_H
