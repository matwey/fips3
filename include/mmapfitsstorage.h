#ifndef _MMAPFITSSTORAGE_H_
#define _MMAPFITSSTORAGE_H_

#include <QFileDevice>

#include <memory>

#include <abstractfitsstorage.h>

class MMapFITSStorage: public AbstractFITSStorage {
private:
	std::unique_ptr<QFileDevice> file_device_;
public:
	MMapFITSStorage(QFileDevice* file_device);
	virtual ~MMapFITSStorage() override;
};

#endif // _MMAPFITSSTORAGE_H_
