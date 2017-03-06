#ifndef _MMAPFITSSTORAGE_H_
#define _MMAPFITSSTORAGE_H_

#include <QFileDevice>

#include <memory>

#include <abstractfitsstorage.h>

class MMapFITSStorage: public AbstractFITSStorage {
private:
	std::unique_ptr<QFileDevice> file_device_;
	qint64 size_;
	quint8* data_;
public:
	MMapFITSStorage(QFileDevice* file_device);
	virtual ~MMapFITSStorage() override;

	virtual quint8* data() const override;
	virtual qint64 size() const override;
};

#endif // _MMAPFITSSTORAGE_H_
