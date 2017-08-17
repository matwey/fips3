#include <openglcolormap.h>

OpenGLColorMap::OpenGLColorMap():
	QOpenGLTexture(QOpenGLTexture::Target1D) {}

OpenGLColorMap::~OpenGLColorMap() = default;

void OpenGLColorMap::initialize() {
	setMinificationFilter(QOpenGLTexture::Linear);
	setMagnificationFilter(QOpenGLTexture::Linear);
	setWrapMode(QOpenGLTexture::MirroredRepeat);
	setFormat(QOpenGLTexture::RGBAFormat);
	setSize(size()/4);
	allocateStorage();
	setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, data());
	initialized = true;
}


GrayscaleColorMap::GrayscaleColorMap():
	OpenGLColorMap(),
	colormap_{{0,0,0,255,255,255,255,255}} {}

GrayscaleColorMap::~GrayscaleColorMap() = default;

int GrayscaleColorMap::size() const {
	return colormap_.size();
}

const quint8* GrayscaleColorMap::data() const {
	return colormap_.data();
}

const char* GrayscaleColorMap::name() const {
	return "Grayscale";
}


PurpleBlueColorMap::PurpleBlueColorMap():
		OpenGLColorMap(),
		colormap_{{255,0,255,255, 0,0,255,255, 0,255,255,255}} {}

PurpleBlueColorMap::~PurpleBlueColorMap() = default;

int PurpleBlueColorMap::size() const {
	return colormap_.size();
}

const quint8* PurpleBlueColorMap::data() const {
	return colormap_.data();
}

const char* PurpleBlueColorMap::name() const {
	return "Purple—Blue";
}