#include <mmapfitsstorage.h>

MMapFITSStorage::MMapFITSStorage(QFileDevice* file_device):
	file_device_(file_device),
	size_(file_device_->size()),
	data_(static_cast<quint8*>(file_device_->map(static_cast<qint64>(0), size_, QFileDevice::MapPrivateOption))),
	AbstractFITSStorage() {
}
MMapFITSStorage::~MMapFITSStorage() {
	file_device_->unmap(static_cast<uchar*>(data_));
}
quint8* MMapFITSStorage::data() const {
	return data_;
}
qint64 MMapFITSStorage::size() const {
	return size_;
}
