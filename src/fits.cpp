#include <mmapfitsstorage.h>
#include <fits.h>

FITS::FITS(AbstractFITSStorage* fits_storage):
	fits_storage_(fits_storage) {
}
FITS::FITS(QFileDevice* file_device):
	fits_storage_(new MMapFITSStorage(file_device)) {
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
