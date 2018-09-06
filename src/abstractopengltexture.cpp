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

#include <chrono>

#include <QDebug>

AbstractOpenGLTexture::TextureCreateError::TextureCreateError(GLenum gl_error_code):
		OpenGLException("Cannot create texture", gl_error_code) {
}
void AbstractOpenGLTexture::TextureCreateError::raise() const {
	throw *this;
}
QException* AbstractOpenGLTexture::TextureCreateError::clone() const {
	return new AbstractOpenGLTexture::TextureCreateError(*this);
}

AbstractOpenGLTexture::AbstractOpenGLTexture():
	QOpenGLTexture(QOpenGLTexture::Target2D) {
}

AbstractOpenGLTexture::~AbstractOpenGLTexture() = default;

void AbstractOpenGLTexture::initialize() {
	setMinificationFilter(QOpenGLTexture::Nearest);
	setMagnificationFilter(QOpenGLTexture::Nearest);
	setFormat();
	throwIfGLError<TextureCreateError>();
	setSize(hdu().data().imageDataUnit()->width(), hdu().data().imageDataUnit()->height());
	throwIfGLError<TextureCreateError>();
	allocateStorage();
	throwIfGLError<TextureCreateError>();
	auto t1 = std::chrono::high_resolution_clock::now();
	setData();
	auto t2 = std::chrono::high_resolution_clock::now();
	throwIfGLError<TextureCreateError>();
	std::chrono::duration<double> diff{t2-t1};
	qDebug() << "Load " << diff.count();
}

constexpr const bool AbstractOpenGLTexture::is_little_endian;
