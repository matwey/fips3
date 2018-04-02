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

#include <QString>

#include <mmapfitsstorage.h>
#include <fits.h>

namespace {

struct DataUnitCreateHelper {
	AbstractFITSStorage::Page& begin_;
	const AbstractFITSStorage::Page& end_;
	quint64 height_;
	quint64 width_;

	template<class T> FITS::ImageDataUnit* operator() (T*) {
		std::unique_ptr<FITS::DataUnit<T>> du{new FITS::DataUnit<T>(reinterpret_cast<const T*>(begin_.data()), height_, width_)};

		const auto length = du->element_size() * height_ * width_;

		if (begin_.distanceInBytes(end_) < length)
			throw FITS::UnexpectedEnd();
		begin_.advanceInBytes(length);

		return du.release();
	}
};

}

FITS::FITS(AbstractFITSStorage* fits_storage, AbstractFITSStorage::Page begin, const AbstractFITSStorage::Page& end):
	fits_storage_(fits_storage),
	primary_hdu_(begin, end) {

	const bool has_extension = (primary_hdu_.header().header("EXTEND","F") == "T");

	if (has_extension) {
		while (begin != end) {
			extensions_.emplace_back(begin, end);
		}
	}
}
FITS::FITS(AbstractFITSStorage* fits_storage): FITS(fits_storage, fits_storage->begin(), fits_storage->end()) {
}
FITS::FITS(QFileDevice* file_device): FITS(new MMapFITSStorage(file_device)) {
}
FITS::Exception::Exception(const QString& what): Utils::Exception(what) {
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
FITS::HeaderUnit::HeaderUnit(const std::map<QString, QString>& headers): headers_(headers) {
}
FITS::HeaderUnit::HeaderUnit(std::map<QString, QString>&& headers): headers_(std::move(headers)) {
}
FITS::HeaderUnit FITS::HeaderUnit::createFromPages(AbstractFITSStorage::Page& begin, AbstractFITSStorage::Page end) {
	std::map<QString, QString> headers;
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
			headers.emplace(key,value.trimmed());
		}
	}

	if (!foundEnd)
		throw FITS::UnexpectedEnd();

	return HeaderUnit(std::move(headers));
}
FITS::AbstractDataUnit::AbstractDataUnit() = default;

FITS::AbstractDataUnit::~AbstractDataUnit() = default;

FITS::AbstractDataUnit::VisitorBase::~VisitorBase() = default;

FITS::ImageDataUnit::ImageDataUnit(quint64 height, quint64 width, quint32 element_size):
	AbstractDataUnit(),
	height_(height),
	width_(width),
	element_size_(element_size) {
}

FITS::ImageDataUnit::~ImageDataUnit() = default;

FITS::ImageDataUnit* FITS::ImageDataUnit::createFromPages(AbstractFITSStorage::Page& begin, AbstractFITSStorage::Page end, const QString& bitpix, quint64 height, quint64 width) {
	DataUnitCreateHelper c{begin, end, height, width};
	return bitpixToType(bitpix, c);
}

FITS::EmptyDataUnit::EmptyDataUnit() = default;

FITS::EmptyDataUnit::~EmptyDataUnit() = default;

FITS::HeaderDataUnit::HeaderDataUnit(AbstractFITSStorage::Page& begin, const AbstractFITSStorage::Page& end):
	header_(new HeaderUnit(HeaderUnit::createFromPages(begin, end))) {
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

		data_.reset(ImageDataUnit::createFromPages(begin, end, bitpix, naxis2, naxis1));
	} else {
		data_.reset(new EmptyDataUnit());
	}
}

const FITS::HeaderDataUnit& FITS::first_hdu() const {
	const FITS::HeaderDataUnit* hdu = &primary_hdu();

	for (auto it = begin();
		 it != end() && !hdu->data().imageDataUnit();
		 ++it) {

		hdu = &(*it);
	}
	return *hdu;
}
