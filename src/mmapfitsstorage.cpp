#include <mmapfitsstorage.h>

MMapFITSStorage::MMapFITSStorage(QFileDevice* file_device):
	AbstractFITSStorage(
		static_cast<quint8*>(file_device->map(static_cast<qint64>(0), file_device->size(), QFileDevice::MapPrivateOption)),
		file_device->size()),
	file_device_(file_device) {
}
MMapFITSStorage::~MMapFITSStorage() {
	file_device_->unmap(static_cast<uchar*>(data()));
}
