#include <QDebug>
#include <QOpenGLContext>
#include <QOpenGLPixelTransferOptions>
#include <QtGlobal>

#include <opengltexture.h>

namespace {
	template<std::size_t N> struct bswap_traits;
	template<> struct bswap_traits<1> {
		using type = quint8;
		static inline quint8 bswap(quint8 x) { return x; }
	};
	template<> struct bswap_traits<2> {
		using type = quint16;
		static inline quint16 bswap(quint16 x) {
#if _MSC_VER
			return _byteswap_ushort(x);
#else
			return __builtin_bswap16(x);
#endif
		}
	};
	template<> struct bswap_traits<4> {
		using type = quint32;
		static inline quint32 bswap(quint32 x) {
#if _MSC_VER
			return _byteswap_ulong(x);
#else
			return __builtin_bswap32(x);
#endif
		}
	};
	template<> struct bswap_traits<8> {
		using type = quint64;
		static inline quint64 bswap(quint64 x) {
#if _MSC_VER
			return _byteswap_uint64(x);
#else
			return __builtin_bswap64(x);
#endif
		}
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
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
				[](T x, T y) { return bswap(x) < bswap(y); }
#else
				[](T x, T y) { return x < y; }
#endif
		);
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
		return std::make_pair(bswap(*elements.first), bswap(*elements.second));
#else
		return std::make_pair(*elements.first, *elements.second);
#endif
	}
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

	hdu_->data().apply(Loader{
			hdu_,
			&texture_format_,
			&pixel_format_,
			&pixel_type_,
			&channels_, &channel_size_,
			&swap_bytes_enabled_,
			&minmax_,
			&instrumental_minmax_
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
