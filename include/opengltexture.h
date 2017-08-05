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
	double minimum_, maximum_;
	double instrumental_minimum_, instrumental_maximum_;
	QOpenGLTexture::TextureFormat texture_format_;
	QOpenGLTexture::PixelFormat pixel_format_;
	QOpenGLTexture::PixelType pixel_type_;
	bool swap_bytes_enabled_;

public:
	explicit OpenGLTexture(const FITS::HeaderDataUnit* hdu);

	void initialize();
	inline double hdu_minimum() const { return minimum_; }
	inline double hdu_maximum() const { return maximum_; }
	inline std::pair<double, double> hdu_minmax() const { return std::make_pair(minimum_, maximum_); }
	inline quint8 channels() const { return channels_; }
	inline quint8 channel_size() const { return channel_size_; };
	inline std::pair<double, double> instrumental_minmax() const { return std::make_pair(instrumental_minimum_, instrumental_maximum_); };
};

#endif //FIPS_OPENGLTEXTURE_H
