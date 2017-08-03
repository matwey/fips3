#ifndef _OPENGLTEXTURE_H
#define _OPENGLTEXTURE_H

#include <QOpenGLTexture>
#include <QOpenGLWidget>

#include <fits.h>

class OpenGLTexture: public QOpenGLTexture {
private:
	const FITS::HeaderDataUnit* hdu_;
	GLfloat normalizer_;
	double minimum_, maximum_;
	QOpenGLTexture::TextureFormat texture_format_;
	QOpenGLTexture::PixelFormat pixel_format_;
	QOpenGLTexture::PixelType pixel_type_;
	bool swap_bytes_enabled_;

public:
	explicit OpenGLTexture(const FITS::HeaderDataUnit* hdu);

	void initialize();
	inline GLfloat normalizer() const { return normalizer_; }
	inline double hdu_minimum() const { return minimum_; }
	inline double hdu_maximum() const { return maximum_; }
	inline std::pair<double, double> hdu_minmax() const { return std::make_pair(minimum_, maximum_); }
};

#endif //FIPS_OPENGLTEXTURE_H
