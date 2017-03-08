#ifndef _FITS_H_
#define _FITS_H_

#include <QFileDevice>

#include <abstractfitsstorage.h>
#include <exception.h>

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
public:
	FITS(AbstractFITSStorage* fits_storage);
	FITS(QFileDevice* file_device);
};

#endif // _FITS_H_
