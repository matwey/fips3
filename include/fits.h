#ifndef _FITS_H_
#define _FITS_H_

#include <QFileDevice>

#include <abstractfitsstorage.h>

#include <memory>

class FITS {
private:
	std::unique_ptr<AbstractFITSStorage> fits_storage_;
public:
	FITS(AbstractFITSStorage* fits_storage);
	FITS(QFileDevice* file_device);
};

#endif // _FITS_H_
