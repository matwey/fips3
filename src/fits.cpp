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

struct HeaderDataUnitCreateHelper {
	FITS::HeaderUnit header;

	template<class T> FITS::AbstractHeaderDataUnit* operator() (const T& data) {
		return new FITS::HeaderDataUnit<T>(std::move(header), data);
	}
};

}

FITS::FITS(AbstractFITSStorage* fits_storage, AbstractFITSStorage::Page begin, const AbstractFITSStorage::Page& end):
	fits_storage_(fits_storage),
	primary_hdu_{AbstractHeaderDataUnit::createFromPages(begin, end)} {

	const bool has_extension = (primary_hdu_->header().header("EXTEND","F") == "T");

	if (has_extension) {
		while (begin != end) {
			extensions_.emplace_back(AbstractHeaderDataUnit::createFromPages(begin, end));
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

FITS::AbstractDataUnit* FITS::AbstractDataUnit::createFromPages(AbstractFITSStorage::Page& begin, AbstractFITSStorage::Page end, const HeaderUnit& header) {
	const auto bitpix = header.header("BITPIX");
	const auto naxis = header.header_as<unsigned int>("NAXIS");

	if (naxis == 0) {
		return new EmptyDataUnit{};
	} else if (naxis != 2) {
		throw FITS::WrongHeaderValue("NAXIS", header.header("NAXIS"));
	}

	const auto width = header.header_as<quint64>("NAXIS1");
	const auto height = header.header_as<quint64>("NAXIS2");

	return ImageDataUnit::createFromPages(begin, end, bitpix, height, width);
}

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

FITS::AbstractHeaderDataUnit::AbstractHeaderDataUnit(const HeaderUnit& header):
	header_(header) {
}
FITS::AbstractHeaderDataUnit::AbstractHeaderDataUnit(HeaderUnit&& header):
	header_(std::move(header)) {
}
FITS::AbstractHeaderDataUnit::~AbstractHeaderDataUnit() = default;
FITS::AbstractHeaderDataUnit* FITS::AbstractHeaderDataUnit::createFromPages(AbstractFITSStorage::Page& begin, const AbstractFITSStorage::Page& end) {
	FITS::HeaderUnit hdr{HeaderUnit::createFromPages(begin, end)};
	auto dataunit = FITS::AbstractDataUnit::createFromPages(begin, end, hdr);
	HeaderDataUnitCreateHelper c{std::move(hdr)};
	return dataunit->apply(c);
}

const FITS::AbstractHeaderDataUnit& FITS::first_hdu() const {
	const FITS::AbstractHeaderDataUnit* hdu = &primary_hdu();

	for (auto it = begin();
		 it != end() && !hdu->data().imageDataUnit();
		 ++it) {

		hdu = &(**it);
	}
	return *hdu;
}
