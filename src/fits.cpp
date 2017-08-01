#include <QString>

#include <mmapfitsstorage.h>
#include <fits.h>

namespace {

struct DataUnitCreateHelper {
	FITS::AbstractDataUnit** data_unit_ref_;
	AbstractFITSStorage::Page& begin_;
	const AbstractFITSStorage::Page& end_;
	quint64 height_;
	quint64 width_;

	template<class T> void operator() (T*) {
		*data_unit_ref_ = new FITS::DataUnit<T>(begin_, end_, height_, width_);
	}
};

}

FITS::FITS(AbstractFITSStorage* fits_storage, AbstractFITSStorage::Page begin, const AbstractFITSStorage::Page& end):
	fits_storage_(fits_storage),
	primary_hdu_(begin, end) {
}
FITS::FITS(AbstractFITSStorage* fits_storage): FITS(fits_storage, fits_storage->begin(), fits_storage->end()) {
}
FITS::FITS(QFileDevice* file_device): FITS(new MMapFITSStorage(file_device)) {
}
FITS::Exception::Exception(const QString& what): ::Exception(what) {
}
void FITS::Exception::raise() const {
	throw *this;
}
QException* FITS::Exception::clone() const {
	return new FITS::Exception(*this);
}
FITS::UnexpectedEnd::UnexpectedEnd():
	FITS::Exception(QString("Unexpected end of file reached")) {
}
void FITS::UnexpectedEnd::raise() const {
	throw *this;
}
QException* FITS::UnexpectedEnd::clone() const {
	return new FITS::UnexpectedEnd(*this);
}
FITS::WrongHeaderValue::WrongHeaderValue(const QString& key, const QString& value):
	FITS::Exception(QString("Wrong header pair found: '" + key + "' = '" + value + "'")) {
}
void FITS::WrongHeaderValue::raise() const {
	throw *this;
}
QException* FITS::WrongHeaderValue::clone() const {
	return new FITS::WrongHeaderValue(*this);
}
FITS::UnsupportedBitpix::UnsupportedBitpix(const QString& bitpix): WrongHeaderValue("BITPIX", bitpix) {
}
void FITS::UnsupportedBitpix::raise() const {
	throw *this;
}
QException* FITS::UnsupportedBitpix::clone() const {
	return new FITS::UnsupportedBitpix(*this);
}
FITS::HeaderUnit::HeaderUnit(AbstractFITSStorage::Page& begin, const AbstractFITSStorage::Page& end) {
	bool foundEnd = false;

	for (; begin != end && !foundEnd; ++begin) {
		for (const quint8* record = begin.data(); record != begin.data() + 2880 && !foundEnd; record += 80) {
			QString key = QString::fromLatin1(reinterpret_cast<const char*>(record), 8).trimmed();
			if (key == QString("END")) {
				foundEnd = true;
				continue;
			}
			QString value = QString::fromLatin1(reinterpret_cast<const char*>(record) + 10, 70);
			int comment = value.indexOf(QChar('/'));
			if (comment != -1) {
				value.resize(comment);
			}
			headers_.emplace(key,value.trimmed());
		}
	}

	if (!foundEnd)
		throw FITS::UnexpectedEnd();
}
FITS::AbstractDataUnit::AbstractDataUnit(AbstractFITSStorage::Page& begin, const AbstractFITSStorage::Page& end, quint64 length):
	data_(begin.data()), length_(length) {

	if (begin.distanceInBytes(end) < length)
		throw FITS::UnexpectedEnd();
	begin.advanceInBytes(length);
}
FITS::AbstractDataUnit::~AbstractDataUnit() = default;

FITS::AbstractDataUnit::VisitorBase::~VisitorBase() = default;

FITS::AbstractDataUnit* FITS::AbstractDataUnit::createFromBitpix(const QString& bitpix, AbstractFITSStorage::Page& begin, const AbstractFITSStorage::Page& end, quint64 height, quint64 width) {
	FITS::AbstractDataUnit* data_unit;
	DataUnitCreateHelper c {&data_unit, begin, end, height, width};
	bitpixToType(bitpix, c);
	return data_unit;
}
FITS::ImageDataUnit::ImageDataUnit(AbstractFITSStorage::Page& begin, const AbstractFITSStorage::Page& end, quint32 element_size, quint64 height, quint64 width):
	AbstractDataUnit(begin, end, element_size * height * width),
	height_(height),
	width_(width) {
}
FITS::ImageDataUnit::~ImageDataUnit() = default;

FITS::HeaderDataUnit::HeaderDataUnit(AbstractFITSStorage::Page& begin, const AbstractFITSStorage::Page& end):
	header_(new HeaderUnit(begin, end)) {
	bool ok = false;

	auto bitpix = header_->header("BITPIX");

	auto naxis = header_->header("NAXIS").toInt(&ok);
	if (!ok || (naxis != 0 && naxis != 2)) {
		throw FITS::WrongHeaderValue("NAXIS", header_->header("NAXIS"));
	}

	if (naxis) {
		// width
		auto naxis1 = header_->header("NAXIS1").toULongLong(&ok);
		if (!ok) {
			throw FITS::WrongHeaderValue("NAXIS1", header_->header("NAXIS1"));
		}

		// height
		auto naxis2 = header_->header("NAXIS2").toULongLong(&ok);
		if (!ok) {
			throw FITS::WrongHeaderValue("NAXIS2", header_->header("NAXIS2"));
		}

		data_.reset(AbstractDataUnit::createFromBitpix(bitpix, begin, end, naxis2, naxis1));
	}
}
