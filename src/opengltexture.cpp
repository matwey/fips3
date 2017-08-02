#include <QDebug>
#include <QOpenGLContext>
#include <QOpenGLPixelTransferOptions>

#include <opengltexture.h>

namespace{
	template<std::size_t N> struct bswap_traits;
	template<> struct bswap_traits<1> {
		using type = quint8;
		static inline quint8 bswap(quint8 x) { return x; }
	};
	template<> struct bswap_traits<2> {
		using type = quint16;
		static inline quint16 bswap(quint16 x) { return __builtin_bswap16(x); }
	};
	template<> struct bswap_traits<4> {
		using type = quint32;
		static inline quint32 bswap(quint32 x) { return __builtin_bswap32(x); }
	};
	template<> struct bswap_traits<8> {
		using type = quint64;
		static inline quint64 bswap(quint64 x) { return __builtin_bswap64(x); }
	};

	template<class T> inline T bswap(T x) {
		using traits = bswap_traits<sizeof(T)>;
		auto value = traits::bswap(*reinterpret_cast<typename traits::type*>(&x));
		return *reinterpret_cast<T*>(&value);
	}

	template<class T> std::pair<T, T> swaped_minmax_element(T* begin, T* end) {
		auto elements = std::minmax_element(
				begin,
				end,
				[](T x, T y) { return bswap(x) < bswap(y); }
		);
		return std::make_pair(bswap(*elements.first), bswap(*elements.second));
	}
}

OpenGLTexture::OpenGLTexture(const FITS::HeaderDataUnit* hdu):
		QOpenGLTexture(QOpenGLTexture::Target2D),
		hdu_(hdu) {
}

void OpenGLTexture::initialize() {
	struct Loader {
		const FITS::HeaderDataUnit* hdu_;
		GLfloat* normalizer;
		QOpenGLTexture::TextureFormat* texture_format;
		QOpenGLTexture::PixelFormat* pixel_format;
		QOpenGLTexture::PixelType *pixel_type;
		bool* swap_bytes_enabled;
		double *minimum;
		double *maximum;

		void operator() (const FITS::DataUnit<quint8>& data) const {
			*normalizer = static_cast<GLfloat>(std::numeric_limits<quint8>::max());
			*texture_format = QOpenGLTexture::AlphaFormat;
			*pixel_format = QOpenGLTexture::Alpha;
			*pixel_type = QOpenGLTexture::UInt8;
			*swap_bytes_enabled = false;

			const auto minmax = swaped_minmax_element(data.data(), data.data() + data.length());
			*minimum = minmax.first  * hdu_->header().bscale() + hdu_->header().bzero();
			*maximum = minmax.second * hdu_->header().bscale() + hdu_->header().bzero();
		}

		void operator() (const FITS::DataUnit<qint16>& data) const {
			*normalizer = static_cast<GLfloat>(std::numeric_limits<qint16>::max()) - static_cast<GLfloat>(std::numeric_limits<qint16>::min());
			*texture_format = QOpenGLTexture::LuminanceAlphaFormat;
			*pixel_format = QOpenGLTexture::LuminanceAlpha;
			*pixel_type = QOpenGLTexture::UInt8;
			*swap_bytes_enabled = false;

			const auto minmax = swaped_minmax_element(data.data(), data.data() + data.length());
			*minimum = minmax.first  * hdu_->header().bscale() + hdu_->header().bzero();
			*maximum = minmax.second * hdu_->header().bscale() + hdu_->header().bzero();
		}
		void operator() (const FITS::DataUnit<qint32>& data) const {
			*normalizer = static_cast<GLfloat>(std::numeric_limits<qint32>::max()) - static_cast<GLfloat>(std::numeric_limits<qint32>::min());
			*texture_format = QOpenGLTexture::RGBAFormat;
			*pixel_format = QOpenGLTexture::RGBA;
			*pixel_type = QOpenGLTexture::UInt8;
			*swap_bytes_enabled = false;

			const auto minmax = swaped_minmax_element(data.data(), data.data() + data.length());
			*minimum = minmax.first  * hdu_->header().bscale() + hdu_->header().bzero();
			*maximum = minmax.second * hdu_->header().bscale() + hdu_->header().bzero();
		}
		void operator() (const FITS::DataUnit<qint64>& data) const {
			*normalizer = static_cast<GLfloat>(std::numeric_limits<qint64>::max()) - static_cast<GLfloat>(std::numeric_limits<qint64>::min());
			*texture_format = QOpenGLTexture::RGBA16_UNorm;
			*pixel_format = QOpenGLTexture::RGBA;
			*pixel_type = QOpenGLTexture::UInt16;
			*swap_bytes_enabled = true;

			const auto minmax = swaped_minmax_element(data.data(), data.data() + data.length());
			*minimum = minmax.first  * hdu_->header().bscale() + hdu_->header().bzero();
			*maximum = minmax.second * hdu_->header().bscale() + hdu_->header().bzero();
		}
		void operator() (const FITS::DataUnit<float>& data) const {
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

				const auto minmax = swaped_minmax_element(data.data(), data.data() + data.length());
				*minimum = minmax.first  * hdu_->header().bscale() + hdu_->header().bzero();
				*maximum = minmax.second * hdu_->header().bscale() + hdu_->header().bzero();
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
			hdu_,
			&normalizer_,
			&texture_format_,
			&pixel_format_,
			&pixel_type_,
			&swap_bytes_enabled_,
			&minimum_,
			&maximum_
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