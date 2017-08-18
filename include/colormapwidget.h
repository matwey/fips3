#ifndef _COLORMAPWIDGET_H
#define _COLORMAPWIDGET_H

#include <QtGui/qopengl.h>
#include <QButtonGroup>
#include <QDebug>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QWidget>

#include <array>
#include <memory>

#include <openglcolormap.h>
#include <openglwidget.h>

class ColorMapWidget: public QWidget {
	Q_OBJECT
private:
	std::unique_ptr<QButtonGroup> button_group_;
public:
	ColorMapWidget(QWidget* parent, const OpenGLWidget& open_gl_widget);
	inline QButtonGroup* buttonGroup() const { return button_group_.get(); }
};

#endif //_COLORMAPWIDGET_H