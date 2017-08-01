#ifndef _ABSTRACTFITSSTORAGE_H_
#define _ABSTRACTFITSSTORAGE_H_

#include <QtGlobal>

class AbstractFITSStorage {
public:
	class Page {
	private:
		quint8* data_;
	public:
		Page(quint8* data);

		inline const quint8* data() const { return data_; }
		inline const quint8* operator* () const { return data(); }

		inline quint64 distanceInBytes(const Page& other) const {
			return other.data_ - data_;
		}
		inline quint64 distance(const Page& other) const {
			return distanceInBytes(other) / 2880;
		}

		inline void advance(quint64 pages) {
			data_ += pages * 2880;
		}
		inline void advanceInBytes(quint64 bytes) {
			advance(bytes ? (bytes - 1) / 2880 + 1 : 0);
		}

		inline Page& operator++ () {
			data_ += 2880;
			return *this;
		}
		inline Page operator++ (int) {
			Page ret(*this);
			data_ += 2880;
			return ret;
		}
		inline bool operator==(const Page& other) const {
			return data_ == other.data_;
		}
		inline bool operator!=(const Page& other) const {
			return data_ != other.data_;
		}
	};
private:
	quint8* data_;
	qint64 size_;
public:
	AbstractFITSStorage(quint8* data, qint64 size);
	virtual ~AbstractFITSStorage() = 0;

	inline quint8* data() const { return data_; }
	inline qint64 size() const { return size_; }

	inline Page cbegin() const { return Page(data_); }
	inline Page cend()   const { return Page(data_ + size_ / 2880 * 2880); }
	inline Page begin() const { return cbegin(); }
	inline Page end()   const { return cend(); }
};

#endif // _ABSTRACTFITSSTORAGE_H_
