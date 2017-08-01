#ifndef _OPENGLTEXTURE_H
#define _OPENGLTEXTURE_H

#include <QOpenGLTexture>
#include <QOpenGLWidget>

#include <fits.h>

class OpenGLTexture: public QOpenGLTexture {
private:
	const FITS::HeaderDataUnit* hdu_;
	GLfloat normalizer_;
	QOpenGLTexture::TextureFormat texture_format_;
	QOpenGLTexture::PixelFormat pixel_format_;
	QOpenGLTexture::PixelType pixel_type_;
	bool swap_bytes_enabled_;

public:
	explicit OpenGLTexture(const FITS::HeaderDataUnit* hdu);

	void initialize();
	inline GLfloat normalizer() { return normalizer_; };
};

#endif //FIPS_OPENGLTEXTURE_H
