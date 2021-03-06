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

#include <abstractopengltexture.h>

AbstractOpenGLTexture::TextureCreateError::TextureCreateError(GLenum gl_error_code):
		OpenGLException("Cannot create texture", gl_error_code) {
}
void AbstractOpenGLTexture::TextureCreateError::raise() const {
	throw *this;
}
QException* AbstractOpenGLTexture::TextureCreateError::clone() const {
	return new AbstractOpenGLTexture::TextureCreateError(*this);
}

AbstractOpenGLTexture::AbstractOpenGLTexture(QOpenGLTexture::Target target):
	QOpenGLTexture(target) {
}

AbstractOpenGLTexture::~AbstractOpenGLTexture() = default;

void AbstractOpenGLTexture::setSize() {
	const auto& im_du = hdu().data().imageDataUnit();

	QOpenGLTexture::setSize(im_du->width(), im_du->height());
}

void AbstractOpenGLTexture::initialize() {
	setMinificationFilter(QOpenGLTexture::Nearest);
	setMagnificationFilter(QOpenGLTexture::Nearest);
	setFormat();
	throwIfGLError<TextureCreateError>();
	setSize();
	throwIfGLError<TextureCreateError>();
	allocateStorage();
	throwIfGLError<TextureCreateError>();
	setData();
	throwIfGLError<TextureCreateError>();
}

constexpr const bool AbstractOpenGLTexture::is_little_endian;
