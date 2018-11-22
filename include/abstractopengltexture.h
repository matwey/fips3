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

#ifndef _ABSTRACTOPENGLTEXTURE_H
#define _ABSTRACTOPENGLTEXTURE_H

#include <QtGlobal>
#include <QOpenGLTexture>

#include <fits.h>
#include <openglerrors.h>
#include <utils/swapbytes.h>
#include <utility>

class AbstractOpenGLTexture: public QOpenGLTexture {
public:
	class TextureCreateError: public OpenGLException {
	public:
		TextureCreateError(GLenum gl_error_code);

		virtual void raise() const override;
		virtual QException* clone() const override;
	};

protected:
	template<class T>
	void setData(PixelFormat sourceFormat, PixelType sourceType, const FITS::DataUnit<T>& dataUnit, const QOpenGLPixelTransferOptions *const options) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
		QOpenGLTexture::setData(0, 0, layers(), QOpenGLTexture::CubeMapPositiveX, sourceFormat, sourceType, dataUnit.data(), options);
#else
		const auto layer_size = width() * height();

		auto data = dataUnit.data();
		for (std::size_t i = 0; i < layers(); ++i, data += layer_size) {
			QOpenGLTexture::setData(0, i, sourceFormat, sourceType, data, options);
		}
#endif // QT_VERSION
	}

	virtual void setSize();
	virtual void setFormat() = 0;
	virtual void allocateStorage() = 0;
	virtual void setData() = 0;

#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
	constexpr static const bool is_little_endian = true;
#else
	constexpr static const bool is_little_endian = false;
#endif

public:
	explicit AbstractOpenGLTexture(QOpenGLTexture::Target target = QOpenGLTexture::Target2D);
	virtual ~AbstractOpenGLTexture() = 0;

	virtual void initialize();
	virtual const FITS::AbstractHeaderDataUnit& hdu() const = 0;
};

#endif // _ABSTRACTOPENGLTEXTURE_H
