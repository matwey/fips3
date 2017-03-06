#ifndef _ABSTRACTFITSSTORAGE_H_
#define _ABSTRACTFITSSTORAGE_H_

#include <QtGlobal>

class AbstractFITSStorage {
private:
public:
	virtual ~AbstractFITSStorage() = default;

	virtual quint8* data() const = 0;
	virtual qint64 size() const = 0;
};

#endif // _ABSTRACTFITSSTORAGE_H_
