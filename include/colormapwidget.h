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
	QButtonGroup* button_group_;
public:
	ColorMapWidget(QWidget* parent, const OpenGLWidget& open_gl_widget) noexcept;
	inline QButtonGroup* buttonGroup() const { return button_group_; }
};

#endif //_COLORMAPWIDGET_H
