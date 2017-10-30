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

#ifndef _ABSTRACTOPENGLTRANSFORM_H
#define _ABSTRACTOPENGLTRANSFORM_H

#include <QObject>
#include <QMatrix4x4>
#include <QVector4D>

class AbstractOpenGLTransform:
	public QObject {
public:
	AbstractOpenGLTransform(QObject* parent = Q_NULLPTR);
	virtual ~AbstractOpenGLTransform() = 0;

	virtual const QMatrix4x4& transformMatrix() const = 0;

	inline QVector4D transform(const QVector4D& vector) const {
		return transformMatrix().map(vector);
	}
	inline QVector4D transform(const float x, const float y) const {
		return transform(QVector4D(x, y, static_cast<float>(0), static_cast<float>(1)));
	}

	inline QVector4D inverseTransform(const QVector4D& vector) const {
		return transformMatrix().inverted().map(vector);
	}
	inline QVector4D inverseTransform(const float x, const float y) const {
		return inverseTransform(QVector4D(x, y, static_cast<float>(0), static_cast<float>(1)));
	}
};

#endif // _ABSTRACTOPENGLTRANSFORM_H
