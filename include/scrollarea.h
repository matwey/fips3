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
	void setViewportViewport(const QRect& viewport);
	inline const QRect& viewportViewport() const { return open_gl_widget_->viewport(); }
private:
	std::unique_ptr<OpenGLWidget> open_gl_widget_;

	void updateBars(const QRect& viewport);
};

#endif //_SCROLLAREA_H
