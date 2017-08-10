#ifndef _OPENGLTEXTURE_H
#define _OPENGLTEXTURE_H

#include <QOpenGLTexture>
#include <QOpenGLWidget>

#include <algorithm>

#include <fits.h>

class OpenGLTexture: public QOpenGLTexture {
private:
	const FITS::HeaderDataUnit* hdu_;
	quint8 channels_;  // Number of color channels
	quint8 channel_size_;  // Bytes per channel for integral texture, 0 for float one
	std::pair<double, double> minmax_;
	std::pair<double, double> instrumental_minmax_;
	QOpenGLTexture::TextureFormat texture_format_;
	QOpenGLTexture::PixelFormat pixel_format_;
	QOpenGLTexture::PixelType pixel_type_;
	bool swap_bytes_enabled_;

public:
	explicit OpenGLTexture(const FITS::HeaderDataUnit* hdu);

	void initialize();
	inline const std::pair<double, double>& hdu_minmax() const { return minmax_; }
	inline const std::pair<double, double>& instrumental_minmax() const { return instrumental_minmax_; }
	inline quint8 channels() const { return channels_; }
	inline quint8 channel_size() const { return channel_size_; };
};

#endif //_OPENGLTEXTURE_H
