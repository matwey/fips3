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

#include <QDebug>
#include <QOpenGLContext>
#include <QOpenGLPixelTransferOptions>
#include <QtGlobal>

#include <opengltexture.h>
#include <utils/swapbytes.h>
#include <utils/openglrowalign.h>

namespace {
	template<class T> std::pair<T, T> swaped_minmax_element(T* begin, T* end) {
		using Utils::swap_bytes;

		auto elements = std::minmax_element(
				begin,
				end,
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
				[](T x, T y) { return swap_bytes(x) < swap_bytes(y); }
#else
				[](T x, T y) { return x < y; }
#endif
		);
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
		return std::make_pair(swap_bytes(*elements.first), swap_bytes(*elements.second));
#else
		return std::make_pair(*elements.first, *elements.second);
#endif
	}
}

OpenGLTexture::TextureCreateError::TextureCreateError(GLenum gl_error_code):
		OpenGLException("Cannot create texture", gl_error_code) {
}
void OpenGLTexture::TextureCreateError::raise() const {
	throw *this;
}
QException* OpenGLTexture::TextureCreateError::clone() const {
	return new OpenGLTexture::TextureCreateError(*this);
}

OpenGLTexture::OpenGLTexture(const FITS::HeaderDataUnit* hdu):
		QOpenGLTexture(QOpenGLTexture::Target2D),
		hdu_(hdu) {
}

void OpenGLTexture::initialize() {
	struct Loader {
		const FITS::HeaderDataUnit* hdu_;
		QOpenGLTexture::TextureFormat* texture_format;
		QOpenGLTexture::PixelFormat* pixel_format;
		QOpenGLTexture::PixelType *pixel_type;
		quint8* channels;
		quint8* channel_size;
		int* alignment;
		bool* swap_bytes_enabled;
		std::pair<double, double>* minmax;
		std::pair<double, double>* instrumental_minmax;

		void operator() (const FITS::DataUnit<quint8>& data) const {
			*texture_format = QOpenGLTexture::AlphaFormat;
			*pixel_format = QOpenGLTexture::Alpha;
			*pixel_type = QOpenGLTexture::UInt8;
			*swap_bytes_enabled = false;
			*channels = 1;
			*channel_size = 1;
			*alignment = Utils::row_align(data.width());

			*minmax = swaped_minmax_element(data.data(), data.data() + data.length());
			minmax->first = minmax->first  * hdu_->header().bscale() + hdu_->header().bzero();
			minmax->second = minmax->second * hdu_->header().bscale() + hdu_->header().bzero();

			instrumental_minmax->first  = hdu_->header().bzero();
			instrumental_minmax->second = static_cast<double>(std::numeric_limits<quint8>::max()) * hdu_->header().bscale() + hdu_->header().bzero();
		}

		void operator() (const FITS::DataUnit<qint16>& data) const {
			*texture_format = QOpenGLTexture::LuminanceAlphaFormat;
			*pixel_format = QOpenGLTexture::LuminanceAlpha;
			*pixel_type = QOpenGLTexture::UInt8;
			*swap_bytes_enabled = false;
			*channels = 2;
			*channel_size = 1;
			*alignment = Utils::row_align(data.width() * *channels);

			*minmax = swaped_minmax_element(data.data(), data.data() + data.length());
			minmax->first = minmax->first  * hdu_->header().bscale() + hdu_->header().bzero();
			minmax->second = minmax->second * hdu_->header().bscale() + hdu_->header().bzero();

			instrumental_minmax->first  = static_cast<double>(std::numeric_limits<qint16>::min()) * hdu_->header().bscale() + hdu_->header().bzero();
			instrumental_minmax->second = static_cast<double>(std::numeric_limits<qint16>::max()) * hdu_->header().bscale() + hdu_->header().bzero();
		}
		void operator() (const FITS::DataUnit<qint32>& data) const {
			*texture_format = QOpenGLTexture::RGBAFormat;
			*pixel_format = QOpenGLTexture::RGBA;
			*pixel_type = QOpenGLTexture::UInt8;
			*swap_bytes_enabled = false;
			*channels = 4;
			*channel_size = 1;
			*alignment = Utils::row_align(data.width() * *channels);

			*minmax = swaped_minmax_element(data.data(), data.data() + data.length());
			minmax->first = minmax->first  * hdu_->header().bscale() + hdu_->header().bzero();
			minmax->second = minmax->second * hdu_->header().bscale() + hdu_->header().bzero();

			instrumental_minmax->first  = static_cast<double>(std::numeric_limits<qint32>::min()) * hdu_->header().bscale() + hdu_->header().bzero();
			instrumental_minmax->second = static_cast<double>(std::numeric_limits<qint32>::max()) * hdu_->header().bscale() + hdu_->header().bzero();
		}
		void operator() (const FITS::DataUnit<qint64>& data) const {
			*texture_format = QOpenGLTexture::RGBA16_UNorm;
			*pixel_format = QOpenGLTexture::RGBA;
			*pixel_type = QOpenGLTexture::UInt16;
			*swap_bytes_enabled = true;
			*channels = 4;
			*channel_size = 2;
			*alignment = Utils::row_align(data.width() * *channels * *channel_size);

			*minmax = swaped_minmax_element(data.data(), data.data() + data.length());
			minmax->first = minmax->first  * hdu_->header().bscale() + hdu_->header().bzero();
			minmax->second = minmax->second * hdu_->header().bscale() + hdu_->header().bzero();

			instrumental_minmax->first  = static_cast<double>(std::numeric_limits<qint64>::min()) * hdu_->header().bscale() + hdu_->header().bzero();
			instrumental_minmax->second = static_cast<double>(std::numeric_limits<qint64>::max()) * hdu_->header().bscale() + hdu_->header().bzero();
		}
		void operator() (const FITS::DataUnit<float>& data) const {
			// TODO: Check GL_ARB_color_buffer_float, GL_OES_texture_float.
			if (! QOpenGLContext::currentContext()->hasExtension("GL_ARB_texture_float")) {
				// TODO: recode data from float into (u)int32
				qDebug() << "BITPIX==-32 is not implemented for this hardware";
			} else {
				// Constant from GL_ARB_texture_float extension documentation:
				// https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_texture_float.txt
				static const quint64 alpha32f_arb = 0x8816;
				*texture_format = static_cast<QOpenGLTexture::TextureFormat>(alpha32f_arb);
				*pixel_format = QOpenGLTexture::Alpha;
				*pixel_type = QOpenGLTexture::Float32;
				*swap_bytes_enabled = true;
				*channels = 1;
				*channel_size = 0;  // special value for float channel
				*alignment = Utils::row_align(data.width() * 4);

				*minmax = swaped_minmax_element(data.data(), data.data() + data.length());
				minmax->first = minmax->first  * hdu_->header().bscale() + hdu_->header().bzero();
				minmax->second = minmax->second * hdu_->header().bscale() + hdu_->header().bzero();

				instrumental_minmax->first  = minmax->first;
				instrumental_minmax->second = minmax->second;
			}
		}
		void operator() (const FITS::DataUnit<double>&) const {
			qDebug() << "BITPIX==-64 is not implemented";
		}
		void operator() (const FITS::EmptyDataUnit&) const {
			Q_ASSERT(0);
		}
	};

	int alignment;
	hdu_->data().apply(Loader{
			hdu_,
			&texture_format_,
			&pixel_format_,
			&pixel_type_,
			&channels_, &channel_size_,
			&alignment,
			&swap_bytes_enabled_,
			&minmax_,
			&instrumental_minmax_
	});

	setMinificationFilter(QOpenGLTexture::Nearest);
	setMagnificationFilter(QOpenGLTexture::Nearest);
	setFormat(texture_format_);
	throwIfGLError<TextureCreateError>();
	setSize(hdu_->data().imageDataUnit()->width(), hdu_->data().imageDataUnit()->height());
	throwIfGLError<TextureCreateError>();
	// We use this overloading to provide a possibility to use texture internal format unsupported by QT
	allocateStorage(pixel_format_, pixel_type_);
	throwIfGLError<TextureCreateError>();
	QOpenGLPixelTransferOptions pixel_transfer_options;
	pixel_transfer_options.setAlignment(alignment);
	pixel_transfer_options.setSwapBytesEnabled(swap_bytes_enabled_);
	this->setData(pixel_format_, pixel_type_, hdu_->data().data(), &pixel_transfer_options);
	throwIfGLError<TextureCreateError>();
}
