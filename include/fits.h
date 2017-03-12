#ifndef _FITS_H_
#define _FITS_H_

#include <QFileDevice>

#include <abstractfitsstorage.h>
#include <exception.h>

#include <map>
#include <memory>

class FITS {
public:
	class Exception: public ::Exception {
	public:
		virtual void raise() const override;
		virtual QException* clone() const override;
	};

	class UnexpectedEnd: public Exception {
	public:
		virtual void raise() const override;
		virtual QException* clone() const override;
	};

	class WrongHeaderValue: public Exception {
	public:
		WrongHeaderValue(const QString& key, const QString& value);

		virtual void raise() const override;
		virtual QException* clone() const override;
	};

	class UnsupportedBitpix: public WrongHeaderValue {
	public:
		UnsupportedBitpix(const QString& bitpix);

		virtual void raise() const override;
		virtual QException* clone() const override;
	};

	class HeaderUnit {
	private:
		std::map<QString, QString> headers_;
	public:
		HeaderUnit(AbstractFITSStorage::Page& begin, const AbstractFITSStorage::Page& end);

		inline const QString& header(const QString& key) const {
			return headers_.at(key);
		}
	};

	class AbstractDataUnit {
	private:
		const quint8* data_;
		quint32 size_;
		quint64 height_;
		quint64 width_;
	public:
		AbstractDataUnit(AbstractFITSStorage::Page& begin, const AbstractFITSStorage::Page& end, quint32 size, quint64 height, quint64 width);
		virtual ~AbstractDataUnit() = 0;

		inline const quint8* data() const { return data_; }
		inline quint64 height() const { return height_; }
		inline quint64 width()  const { return width_; }

		template<class F> static void bitpixToType(const QString& bitpix, F fun);
		static AbstractDataUnit* createFromBitpix(const QString& bitpix, AbstractFITSStorage::Page& begin, const AbstractFITSStorage::Page& end, quint64 height, quint64 width);
	};

	template<class T> class DataUnit: public AbstractDataUnit {
	public:
		inline DataUnit(AbstractFITSStorage::Page& begin, const AbstractFITSStorage::Page& end, quint64 height, quint64 width):
			AbstractDataUnit(begin, end, sizeof(T), height, width) {}
		inline const T* data() const {
			return static_cast<const T*>(AbstractDataUnit::data());
		}
	};
private:
	std::unique_ptr<AbstractFITSStorage> fits_storage_;
	std::unique_ptr<HeaderUnit> header_unit_;
	std::unique_ptr<AbstractDataUnit> data_unit_;
public:
	FITS(AbstractFITSStorage* fits_storage);
	FITS(QFileDevice* file_device);

	inline const HeaderUnit& header_unit() const { return *header_unit_; }
	inline const AbstractDataUnit& data_unit() const { return *data_unit_; } 
};

template<class F> void FITS::AbstractDataUnit::bitpixToType(const QString& bitpix, F fun) {
	if (bitpix == "8") {
		fun(static_cast<quint8*>(0));
	} else if (bitpix == "16") {
		fun(static_cast<qint16*>(0));
	} else if (bitpix == "32") {
		fun(static_cast<qint32*>(0));
	} else if (bitpix == "64") {
		fun(static_cast<qint64*>(0));
	} else if (bitpix == "-32") {
		fun(static_cast<float*>(0));
	} else if (bitpix == "-64") {
		fun(static_cast<double*>(0));
	} else {
		throw FITS::UnsupportedBitpix(bitpix);
	}
}

#endif // _FITS_H_
