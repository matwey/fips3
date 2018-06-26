/*
 *  Copyright (C) 2018  Matwey V. Kornilov <matwey.kornilov@gmail.com>
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

#include <openglfeatures.h>

constexpr const quint64 OpenGLFeatures::mask_hasOpenGL21;
constexpr const quint64 OpenGLFeatures::mask_hasOpenGL30;
constexpr const quint64 OpenGLFeatures::mask_hasOpenGL33;

bool OpenGLFeatures::checkHasOpenGLHelper(const QOpenGLContext& opengl_context, const int major, const int minor, const quint64 mask) {
	bool has = (opengl_context.format().majorVersion() > major
		|| (opengl_context.format().majorVersion() == major && opengl_context.format().minorVersion() >= minor));
	bitmask_ |= (has ? mask : static_cast<quint64>(0));
	return has;
}

bool OpenGLFeatures::checkHasOpenGL33(const QOpenGLContext& opengl_context) {
	return checkHasOpenGLHelper(opengl_context, 3, 3,
		mask_hasOpenGL33 | mask_hasOpenGL30 | mask_hasOpenGL21);
}

bool OpenGLFeatures::checkHasOpenGL30(const QOpenGLContext& opengl_context) {
	return checkHasOpenGLHelper(opengl_context, 3, 0, mask_hasOpenGL30 | mask_hasOpenGL21);
}

bool OpenGLFeatures::checkHasOpenGL21(const QOpenGLContext& opengl_context) {
	return checkHasOpenGLHelper(opengl_context, 2, 1, mask_hasOpenGL21);
}

OpenGLFeatures::OpenGLFeatures(const QOpenGLContext& opengl_context): bitmask_(0) {
	checkHasOpenGL33(opengl_context)
		|| checkHasOpenGL30(opengl_context)
		|| checkHasOpenGL21(opengl_context);
}
