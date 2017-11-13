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

#ifndef FLIPWIDGET_H
#define FLIPWIDGET_H

#include <QCheckBox>
#include <QWidget>

#include <memory>

class FlipWidget: public QWidget {
	Q_OBJECT
private:
	QCheckBox* h_flip_check_box_;
	QCheckBox* v_flip_check_box_;
public:
	explicit FlipWidget(QWidget *parent = Q_NULLPTR);
	inline const QCheckBox* horizontalFlipCheckBox() const { return h_flip_check_box_; }
	inline const QCheckBox* verticalFlipCheckBox()   const { return v_flip_check_box_; }

public slots:
	inline void setHorizontalFlip(bool flipped) { h_flip_check_box_->setChecked(flipped); }
	inline void setVerticalFlip(bool flipped)   { v_flip_check_box_->setChecked(flipped); }
};

#endif //FLIPWIDGET_H
