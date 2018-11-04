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

#ifndef _FITS_H_
#define _FITS_H_

#include <QFileDevice>
#include <QSize>
#include <QVariant>

#include <abstractfitsstorage.h>
#include <utils/exception.h>
#include <utils/result.h>

#include <map>
#include <memory>

class FITS {
public:
	class Exception: public Utils::Exception {
	public:
		explicit Exception(const QString& what);

		virtual void raise() const override;
		virtual QException* clone() const override;
	};

	class UnexpectedEnd: public Exception {
	public:
		UnexpectedEnd();

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
		HeaderUnit() = default;
		explicit HeaderUnit(const std::map<QString, QString>& headers);
		explicit HeaderUnit(std::map<QString, QString>&& headers);
		HeaderUnit(const HeaderUnit&) = default;
		HeaderUnit(HeaderUnit&&) = default;

		HeaderUnit& operator=(const HeaderUnit&) = default;
		HeaderUnit& operator=(HeaderUnit&&) = default;

		~HeaderUnit() = default;

		static HeaderUnit createFromPages(AbstractFITSStorage::Page& begin, AbstractFITSStorage::Page end);

		inline const QString& header(const QString& key) const {
			return headers_.at(key);
		}
		inline const QString& header(const QString& key, const QString& def) const {
			auto it = headers_.find(key);
			return (it != headers_.end() ? it->second : def);
		}
		template<class T> inline T header_as(const QString& key) const {
			QVariant v(headers_.at(key));
			if (!v.canConvert<T>())
				throw WrongHeaderValue(key, v.value<QString>());
			return v.value<T>();
		}
		template<class T> inline T header_as(const QString& key, const T& def) const {
			auto it = headers_.find(key);
			return (it != headers_.end() && QVariant(it->second).canConvert<T>() ?
				qvariant_cast<T>(it->second) : def);
		}

		typedef std::map<QString, QString>::const_iterator const_iterator;

		const_iterator begin() const { return headers_.cbegin(); }
		const_iterator end() const { return headers_.cend(); }
		const_iterator cbegin() const { return headers_.cbegin(); }
		const_iterator cend() const { return headers_.cend(); }
		std::size_t size() const { return headers_.size(); }

		inline double bscale() const { return header_as<double>("BSCALE", 1.0); }
		inline double bzero()  const { return header_as<double>("BZERO", 0.0); }
	};

	class AbstractDataUnit;
	class ImageDataUnit;
	class EmptyDataUnit;
	template<class T> class DataUnit;

	class AbstractDataUnit {
	protected:
		struct VisitorBase {
			virtual ~VisitorBase() = 0;
			virtual void visit(const EmptyDataUnit&) = 0;
			virtual void visit(const DataUnit<quint8>&) = 0;
			virtual void visit(const DataUnit<qint16>&) = 0;
			virtual void visit(const DataUnit<qint32>&) = 0;
			virtual void visit(const DataUnit<qint64>&) = 0;
			virtual void visit(const DataUnit<float>&) = 0;
			virtual void visit(const DataUnit<double>&) = 0;
		};

		virtual void do_apply(VisitorBase& visitor) const = 0;
	public:
		AbstractDataUnit();
		virtual ~AbstractDataUnit() = 0;

		template<class F> auto apply(F fun) const ->
			decltype(fun(std::declval<const EmptyDataUnit&>())) {

			using result_type = decltype(fun(std::declval<const EmptyDataUnit&>()));

			struct Visitor: public VisitorBase {
				Utils::Result<result_type, F> fun_;

				inline Visitor(F fun): VisitorBase(), fun_(std::move(fun)) {}

				virtual void visit(const EmptyDataUnit& x) override { fun_(x); };
				virtual void visit(const DataUnit<quint8>& x) override { fun_(x); };
				virtual void visit(const DataUnit<qint16>& x) override { fun_(x); };
				virtual void visit(const DataUnit<qint32>& x) override { fun_(x); };
				virtual void visit(const DataUnit<qint64>& x) override { fun_(x); };
				virtual void visit(const DataUnit<float>& x)  override { fun_(x); };
				virtual void visit(const DataUnit<double>& x) override { fun_(x); };

				result_type do_apply(const AbstractDataUnit& du) {
					du.do_apply(*this);
					return fun_.result();
				}
			};

			return Visitor(std::move(fun)).do_apply(*this);
		}

		static AbstractDataUnit* createFromPages(AbstractFITSStorage::Page& begin, AbstractFITSStorage::Page end, const HeaderUnit& header);

		inline       ImageDataUnit* imageDataUnit()       { return dynamic_cast<ImageDataUnit*>(this); }
		inline const ImageDataUnit* imageDataUnit() const { return dynamic_cast<const ImageDataUnit*>(this); }
	};

	class ImageDataUnit: public AbstractDataUnit {
	private:
		quint64 height_;
		quint64 width_;
		quint32 element_size_;
	public:
		ImageDataUnit(quint64 height, quint64 width, quint32 element_size);
		virtual ~ImageDataUnit() override = 0;

		template<class F> static auto bitpixToType(const QString& bitpix, F fun) -> decltype(std::declval<F>()(static_cast<quint8*>(0)));
		static ImageDataUnit* createFromPages(AbstractFITSStorage::Page& begin, AbstractFITSStorage::Page end, const QString& bitpix, quint64 height, quint64 width);

		inline quint64 height() const { return height_; }
		inline quint64 width()  const { return width_; }
		inline quint32 element_size() const { return element_size_; }
		inline QSize size() const { return QSize(width_, height_); }
	};

	class EmptyDataUnit: public AbstractDataUnit {
	protected:
		virtual void do_apply(VisitorBase& visitor) const override {
			visitor.visit(*this);
		}
	public:
		EmptyDataUnit();
		virtual ~EmptyDataUnit() override;
	};

	template<class T> class DataUnit: public ImageDataUnit {
	private:
		T const* data_;
		quint64  length_;
	protected:
		virtual void do_apply(VisitorBase& visitor) const override {
			visitor.visit(*this);
		}
	public:
		DataUnit(T const* data, quint64 height, quint64 width):
			ImageDataUnit(height, width, sizeof(T)),
			data_(data), length_(height*width) {}
		DataUnit(const DataUnit&) = default;
		DataUnit(DataUnit&&) = default;
		DataUnit& operator=(const DataUnit&) = default;
		DataUnit& operator=(DataUnit&&) = default;

		virtual ~DataUnit() override = default;

		inline const T* data() const { return data_; }
		inline quint64 length() const { return length_; }
	};

	class AbstractHeaderDataUnit;
	template<class T> class HeaderDataUnit;

	class AbstractHeaderDataUnit {
	private:
		HeaderUnit header_;

		template<class F>
		struct ApplyWrap {
			using result_type = decltype(std::declval<F>()(std::declval<const HeaderDataUnit<EmptyDataUnit>&>()));

			const AbstractHeaderDataUnit* self;
			F& fun_;

			template<class T> auto operator()(const T&) ->
			decltype(std::declval<F>()(std::declval<const HeaderDataUnit<T>&>())) {
				return fun_(*static_cast<const HeaderDataUnit<T>*>(self));
			}
		};
	public:
		explicit AbstractHeaderDataUnit(const HeaderUnit& header);
		explicit AbstractHeaderDataUnit(HeaderUnit&& header);

		AbstractHeaderDataUnit(AbstractHeaderDataUnit&&) = default;
		AbstractHeaderDataUnit(const AbstractHeaderDataUnit&) = default;
		AbstractHeaderDataUnit& operator=(AbstractHeaderDataUnit&&) = default;
		AbstractHeaderDataUnit& operator=(const AbstractHeaderDataUnit&) = default;

		virtual ~AbstractHeaderDataUnit() = 0;

		static AbstractHeaderDataUnit* createFromPages(AbstractFITSStorage::Page& begin, const AbstractFITSStorage::Page& end);

		inline const HeaderUnit&       header() const { return header_; }
		virtual const AbstractDataUnit& data() const = 0;

		template<class F> auto apply(F fun) const ->
			decltype(std::declval<F>()(std::declval<const HeaderDataUnit<EmptyDataUnit>&>())) {

			return data().apply(ApplyWrap<F>{this, fun});
		}

		template<class T> auto FITSToInstrumental(const T& x) const -> decltype(std::declval<T>() * 1.0 + 1.0) {
			const auto bscale = header().bscale();
			const auto bzero  = header().bzero();
			return x * bscale + bzero;
		}

		template<class T> auto InstrumentalToFITS(const T& x) const -> decltype((std::declval<T>() - 1.0)/1.0) {
			const auto bzero  = header().bzero();
			const auto bscale = header().bscale();
			return (x - bzero) / bscale;
		}
	};

	template<class T> class HeaderDataUnit: public AbstractHeaderDataUnit {
	public:
		typedef T data_unit_type;
	private:
		data_unit_type data_;
	public:
		HeaderDataUnit(const HeaderUnit& header, const T& dataunit):
			AbstractHeaderDataUnit(header),
			data_(dataunit) {}
		HeaderDataUnit(HeaderUnit&& header, T&& dataunit):
			AbstractHeaderDataUnit(std::move(header)),
			data_(std::move(dataunit)) {}

		HeaderDataUnit(const HeaderDataUnit&) = default;
		HeaderDataUnit(HeaderDataUnit&&) = default;
		HeaderDataUnit& operator=(const HeaderDataUnit&) = default;
		HeaderDataUnit& operator=(HeaderDataUnit&&) = default;

		virtual ~HeaderDataUnit() override = default;

		virtual const T& data() const override {
			return data_;
		}
	};
private:
	std::unique_ptr<AbstractFITSStorage> fits_storage_;
	std::unique_ptr<AbstractHeaderDataUnit> primary_hdu_;

	std::list<std::unique_ptr<AbstractHeaderDataUnit>> extensions_;

	FITS(AbstractFITSStorage* fits_storage, AbstractFITSStorage::Page begin, const AbstractFITSStorage::Page& end);
public:
	typedef decltype(extensions_)::const_iterator const_iterator;

	FITS(AbstractFITSStorage* fits_storage);
	FITS(QFileDevice* file_device);

	inline const AbstractHeaderDataUnit& primary_hdu() const { return *primary_hdu_; }
	inline const HeaderUnit&       header_unit() const { return primary_hdu_->header(); }
	inline const AbstractDataUnit& data_unit()   const { return primary_hdu_->data(); }
	const AbstractHeaderDataUnit& first_hdu() const;

	const_iterator begin() const { return extensions_.begin(); }
	const_iterator end()   const { return extensions_.end(); }
};

template<class F> auto FITS::ImageDataUnit::bitpixToType(const QString& bitpix, F fun) -> decltype(std::declval<F>()(static_cast<quint8*>(0))) {
	if (bitpix == "8") {
		return fun(static_cast<quint8*>(0));
	} else if (bitpix == "16") {
		return fun(static_cast<qint16*>(0));
	} else if (bitpix == "32") {
		return fun(static_cast<qint32*>(0));
	} else if (bitpix == "64") {
		return fun(static_cast<qint64*>(0));
	} else if (bitpix == "-32") {
		return fun(static_cast<float*>(0));
	} else if (bitpix == "-64") {
		return fun(static_cast<double*>(0));
	}

	throw FITS::UnsupportedBitpix(bitpix);
}

#endif // _FITS_H_
