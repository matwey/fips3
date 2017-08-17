#ifndef _OPENGL_COLORMAP_H
#define _OPENGL_COLORMAP_H

#include <QDebug>
#include <QOpenGLTexture>

#include <array>

class OpenGLColorMap: public QOpenGLTexture {
private:
	bool initialized = false;
public:
	OpenGLColorMap();
	virtual ~OpenGLColorMap() = 0;

	void initialize();
	inline bool isInitialized() { return initialized; }

	virtual const char* name() const = 0;
protected:
	virtual int size() const = 0;
	virtual const quint8* data() const = 0;
};


class GrayscaleColorMap: public OpenGLColorMap {
private:
	std::array<quint8, 8> colormap_;
public:
	GrayscaleColorMap();
	virtual ~GrayscaleColorMap() override;
	virtual const char* name() const override;
protected:
	virtual int size() const override;
	virtual const quint8* data() const override;
};

class PurpleBlueColorMap: public OpenGLColorMap {
private:
	std::array<quint8, 12> colormap_;
public:
	PurpleBlueColorMap();
	virtual ~PurpleBlueColorMap() override;
	virtual const char* name() const override;
protected:
	virtual int size() const override;
	virtual const quint8* data() const override;
};

#endif //_OPENGL_COLORMAP_H
