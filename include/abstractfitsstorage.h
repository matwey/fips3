#ifndef _ABSTRACTFITSSTORAGE_H_
#define _ABSTRACTFITSSTORAGE_H_

#include <QtGlobal>

class AbstractFITSStorage {
private:
	quint8* data_;
	qint64 size_;
public:
	AbstractFITSStorage(quint8* data, qint64 size);
	virtual ~AbstractFITSStorage() = 0;

	inline quint8* data() const { return data_; }
	inline qint64 size() const { return size_; }
};

#endif // _ABSTRACTFITSSTORAGE_H_
