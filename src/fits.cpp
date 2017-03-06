#include <mmapfitsstorage.h>
#include <fits.h>

FITS::FITS(AbstractFITSStorage* fits_storage):
	fits_storage_(fits_storage) {
}
FITS::FITS(QFileDevice* file_device):
	fits_storage_(new MMapFITSStorage(file_device)) {
}
