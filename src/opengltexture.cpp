#include <QDebug>
#include <QOpenGLContext>
#include <QOpenGLPixelTransferOptions>

#include <opengltexture.h>

OpenGLTexture::OpenGLTexture(const FITS::HeaderDataUnit* hdu):
		QOpenGLTexture(QOpenGLTexture::Target2D),
		hdu_(hdu) {
}

void OpenGLTexture::initialize() {
	struct Loader {
		GLfloat* normalizer;
		QOpenGLTexture::TextureFormat* texture_format;
		QOpenGLTexture::PixelFormat* pixel_format;
		QOpenGLTexture::PixelType *pixel_type;
		bool* swap_bytes_enabled;

		void operator() (const FITS::DataUnit<quint8>&) const {
			*normalizer = static_cast<GLfloat>(std::numeric_limits<quint8>::max());
			*texture_format = QOpenGLTexture::AlphaFormat;
			*pixel_format = QOpenGLTexture::Alpha;
			*pixel_type = QOpenGLTexture::UInt8;
			*swap_bytes_enabled = false;
		}

		void operator() (const FITS::DataUnit<qint16>&) const {
			*normalizer = static_cast<GLfloat>(std::numeric_limits<qint16>::max()) - static_cast<GLfloat>(std::numeric_limits<qint16>::min());
			*texture_format = QOpenGLTexture::LuminanceAlphaFormat;
			*pixel_format = QOpenGLTexture::LuminanceAlpha;
			*pixel_type = QOpenGLTexture::UInt8;
			*swap_bytes_enabled = false;
		}
		void operator() (const FITS::DataUnit<qint32>&) const {
			*normalizer = static_cast<GLfloat>(std::numeric_limits<qint32>::max()) - static_cast<GLfloat>(std::numeric_limits<qint32>::min());
			*texture_format = QOpenGLTexture::RGBAFormat;
			*pixel_format = QOpenGLTexture::RGBA;
			*pixel_type = QOpenGLTexture::UInt8;
			*swap_bytes_enabled = false;
		}
		void operator() (const FITS::DataUnit<qint64>&) const {
			*normalizer = static_cast<GLfloat>(std::numeric_limits<qint64>::max()) - static_cast<GLfloat>(std::numeric_limits<qint64>::min());
			*texture_format = QOpenGLTexture::RGBA16_UNorm;
			*pixel_format = QOpenGLTexture::RGBA;
			*pixel_type = QOpenGLTexture::UInt16;
			*swap_bytes_enabled = true;
		}
		void operator() (const FITS::DataUnit<float>&) const {
			// TODO: Check GL_ARB_color_buffer_float, GL_OES_texture_float.
			if (! QOpenGLContext::currentContext()->hasExtension("GL_ARB_texture_float")) {
				// TODO: recode data from float into (u)int32
				qDebug() << "BITPIX==-32 is not implemented for this hardware";
			} else {
				// Constant from GL_ARB_texture_float extension documentation:
				// https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_texture_float.txt
				*normalizer = 1.0f;
				static const quint64 alpha32f_arb = 0x8816;
				*texture_format = static_cast<QOpenGLTexture::TextureFormat>(alpha32f_arb);
				*pixel_format = QOpenGLTexture::Alpha;
				*pixel_type = QOpenGLTexture::Float32;
				*swap_bytes_enabled = true;
			}
		}
		void operator() (const FITS::DataUnit<double>&) const {
			qDebug() << "BITPIX==-64 is not implemented";
		}
		void operator() (const FITS::EmptyDataUnit&) const {
			Q_ASSERT(0);
		}
	};

	hdu_->data().apply(Loader{
			&normalizer_,
			&texture_format_,
			&pixel_format_,
			&pixel_type_,
			&swap_bytes_enabled_
	});

	setMinificationFilter(QOpenGLTexture::Nearest);
	setMagnificationFilter(QOpenGLTexture::Nearest);
	setFormat(texture_format_);
//	throwIfGLError<TextureCreateError>();
	setSize(hdu_->data().imageDataUnit()->width(), hdu_->data().imageDataUnit()->height());
//	throwIfGLError<TextureCreateError>();
	// We use this overloading to provide a possibility to use texture internal format unsupported by QT
	allocateStorage(pixel_format_, pixel_type_);
//	throwIfGLError<TextureCreateError>();
	QOpenGLPixelTransferOptions pixel_transfer_options;
	pixel_transfer_options.setSwapBytesEnabled(swap_bytes_enabled_);
	this->setData(pixel_format_, pixel_type_, hdu_->data().data(), &pixel_transfer_options);
//	throwIfGLError<TextureCreateError>();
}