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

#ifndef _OPENGLERRORS_H
#define _OPENGLERRORS_H

#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include <exception.h>

class OpenGLException: public Exception {
public:
	OpenGLException(const QString &reason, GLenum gl_error_code);

	virtual void raise() const override;
	virtual QException* clone() const override;
	static QString glErrorString(GLenum gl_error_code);
};

template<class T> void throwIfGLError() {
	const auto gl_error_code = QOpenGLContext::currentContext()->functions()->glGetError();
	if (gl_error_code) {
		throw T(gl_error_code);
	}
}

#endif //_OPENGLERRORS_H
