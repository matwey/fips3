#include <QString>

#include <mmapfitsstorage.h>
#include <fits.h>

FITS::FITS(AbstractFITSStorage* fits_storage):
	fits_storage_(fits_storage) {

	AbstractFITSStorage::Page begin = fits_storage->begin();
	AbstractFITSStorage::Page end = fits_storage->end();
	header_unit_ = std::unique_ptr<HeaderUnit>(new HeaderUnit(begin, end));
	data_unit_
}
FITS::FITS(QFileDevice* file_device): FITS(new MMapFITSStorage(file_device)) {
}
void FITS::Exception::raise() const {
	throw *this;
}
QException* FITS::Exception::clone() const {
	return new FITS::Exception(*this);
}
void FITS::UnexpectedEnd::raise() const {
	throw *this;
}
QException* FITS::UnexpectedEnd::clone() const {
	return new FITS::UnexpectedEnd(*this);
}
FITS::HeaderUnit::HeaderUnit(AbstractFITSStorage::Page& begin, const AbstractFITSStorage::Page& end) {
	bool foundEnd = false;

	for (; begin != end && !foundEnd; ++begin) {
		for (const quint8* record = begin.data(); record != begin.data() + 2880; record += 80) {
			QString key = QString::fromLatin1(reinterpret_cast<const char*>(record), 8).trimmed();
			if (key == QString("END")) {
				foundEnd = true;
				break;
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
FITS::AbstractDataUnit::AbstractDataUnit(AbstractFITSStorage::Page& begin, const AbstractFITSStorage::Page& end, quint32 size, quint64 height, quint64 width):
	data_(begin.data()),
	size_(size),
	height_(height),
	width_(width) {

	quint64 full_size = height * width * size;
	if (begin.distanceInBytes(end) < full_size)
		throw FITS::UnexpectedEnd();
	begin.advanceInBytes(full_size);
}
FITS::AbstractDataUnit::~AbstractDataUnit() = default;
