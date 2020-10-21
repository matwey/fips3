/*
 *  Copyright (C) 2017  Matwey V. Kornilov <matwey.kornilov@gmail.com>
 *                      Konstantin Malanchev <hombit@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or (at
 *  your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _OPENGLTEXTURE_H
#define _OPENGLTEXTURE_H

#include <abstractopengltexture.h>

class Uint8OpenGLTexture:
	public AbstractOpenGLTexture {
private:
	const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>* hdu_;
protected:
	virtual const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu() const override {
		return *hdu_;
	}

	virtual void setFormat() override;
	virtual void allocateStorage() override;
	virtual void setData() override;
public:
	Uint8OpenGLTexture(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu, QOpenGLTexture::Target target = QOpenGLTexture::Target2D);
	virtual ~Uint8OpenGLTexture() override = default;
};

class Uint8OpenGLTextureArray:
	public Uint8OpenGLTexture {
protected:
	virtual void setSize() override;
public:
	explicit Uint8OpenGLTextureArray(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu);
	virtual ~Uint8OpenGLTextureArray() override = default;
};

class Uint8OpenGL3Texture:
	public AbstractOpenGLTexture {
private:
	const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>* hdu_;
protected:
	virtual const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu() const override {
		return *hdu_;
	}

	virtual void setFormat() override;
	virtual void allocateStorage() override;
	virtual void setData() override;
public:
	Uint8OpenGL3Texture(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu, QOpenGLTexture::Target target = QOpenGLTexture::Target2D);
	virtual ~Uint8OpenGL3Texture() override = default;
};

class Uint8OpenGL3TextureArray:
	public Uint8OpenGL3Texture {
protected:
	virtual void setSize() override;
public:
	explicit Uint8OpenGL3TextureArray(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu);
	virtual ~Uint8OpenGL3TextureArray() override = default;
};

class Uint8OpenGLES3Texture:
	public AbstractOpenGLTexture {
private:
	const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>* hdu_;
protected:
	virtual const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu() const override {
		return *hdu_;
	}

	virtual void setFormat() override;
	virtual void allocateStorage() override;
	virtual void setData() override;
public:
	Uint8OpenGLES3Texture(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu, QOpenGLTexture::Target target = QOpenGLTexture::Target2D);
	virtual ~Uint8OpenGLES3Texture() override = default;
};

class Int16OpenGLTexture:
	public AbstractOpenGLTexture {
private:
	const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>* hdu_;
protected:
	virtual const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu() const override {
		return *hdu_;
	}

	virtual void setFormat() override;
	virtual void allocateStorage() override;
	virtual void setData() override;
public:
	Int16OpenGLTexture(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu, QOpenGLTexture::Target target = QOpenGLTexture::Target2D);
	virtual ~Int16OpenGLTexture() override = default;
};

class Int16OpenGLTextureArray:
	public Int16OpenGLTexture {
protected:
	virtual void setSize() override;
public:
	explicit Int16OpenGLTextureArray(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu);
	virtual ~Int16OpenGLTextureArray() override = default;
};

class Int16OpenGL3Texture:
	public AbstractOpenGLTexture {
private:
	const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>* hdu_;
protected:
	virtual const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu() const override {
		return *hdu_;
	}

	virtual void setFormat() override;
	virtual void allocateStorage() override;
	virtual void setData() override;
public:
	Int16OpenGL3Texture(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu, QOpenGLTexture::Target target = QOpenGLTexture::Target2D);
	virtual ~Int16OpenGL3Texture() override = default;
};

class Int16OpenGL3TextureArray:
	public Int16OpenGL3Texture {
protected:
	virtual void setSize() override;
public:
	explicit Int16OpenGL3TextureArray(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu);
	virtual ~Int16OpenGL3TextureArray() override = default;
};

class Int16OpenGLES3Texture:
	public AbstractOpenGLTexture {
private:
	const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>* hdu_;
protected:
	virtual const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu() const override {
		return *hdu_;
	}

	virtual void setFormat() override;
	virtual void allocateStorage() override;
	virtual void setData() override;
public:
	Int16OpenGLES3Texture(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu, QOpenGLTexture::Target target = QOpenGLTexture::Target2D);
	virtual ~Int16OpenGLES3Texture() override = default;
};

class Int32OpenGLTexture:
	public AbstractOpenGLTexture {
private:
	const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>* hdu_;
protected:
	virtual const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu() const override {
		return *hdu_;
	}

	virtual void setFormat() override;
	virtual void allocateStorage() override;
	virtual void setData() override;
public:
	Int32OpenGLTexture(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu, QOpenGLTexture::Target target = QOpenGLTexture::Target2D);
	virtual ~Int32OpenGLTexture() override = default;
};

class Int32OpenGLTextureArray:
	public Int32OpenGLTexture {
protected:
	virtual void setSize() override;
public:
	explicit Int32OpenGLTextureArray(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu);
	virtual ~Int32OpenGLTextureArray() override = default;
};

class Int32OpenGL3Texture:
	public AbstractOpenGLTexture {
private:
	const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>* hdu_;
protected:
	virtual const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu() const override {
		return *hdu_;
	}

	virtual void setFormat() override;
	virtual void allocateStorage() override;
	virtual void setData() override;
public:
	Int32OpenGL3Texture(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu, QOpenGLTexture::Target target = QOpenGLTexture::Target2D);
	virtual ~Int32OpenGL3Texture() override = default;
};

class Int32OpenGL3TextureArray:
	public Int32OpenGL3Texture {
protected:
	virtual void setSize() override;
public:
	explicit Int32OpenGL3TextureArray(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu);
	virtual ~Int32OpenGL3TextureArray() override = default;
};

class Int32OpenGLES3Texture:
	public AbstractOpenGLTexture {
private:
	const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>* hdu_;
protected:
	virtual const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu() const override {
		return *hdu_;
	}

	virtual void setFormat() override;
	virtual void allocateStorage() override;
	virtual void setData() override;
public:
	Int32OpenGLES3Texture(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu, QOpenGLTexture::Target target = QOpenGLTexture::Target2D);
	virtual ~Int32OpenGLES3Texture() override = default;
};

class Int64OpenGLTexture:
	public AbstractOpenGLTexture {
private:
	const FITS::HeaderDataUnit<FITS::DataUnit<qint64>>* hdu_;
protected:
	virtual const FITS::HeaderDataUnit<FITS::DataUnit<qint64>>& hdu() const override {
		return *hdu_;
	}

	virtual void setFormat() override;
	virtual void allocateStorage() override;
	virtual void setData() override;
public:
	Int64OpenGLTexture(const FITS::HeaderDataUnit<FITS::DataUnit<qint64>>& hdu, QOpenGLTexture::Target target = QOpenGLTexture::Target2D);
	virtual ~Int64OpenGLTexture() override = default;
};

class Int64OpenGLTextureArray:
	public Int64OpenGLTexture {
protected:
	virtual void setSize() override;
public:
	explicit Int64OpenGLTextureArray(const FITS::HeaderDataUnit<FITS::DataUnit<qint64>>& hdu);
	virtual ~Int64OpenGLTextureArray() override = default;
};


class FloatOpenGL3Texture:
	public AbstractOpenGLTexture {
private:
	const FITS::HeaderDataUnit<FITS::DataUnit<float>>* hdu_;

	virtual void setFormat() override;
	virtual void allocateStorage() override;
	virtual void setData() override;
protected:
	virtual const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu() const override {
		return *hdu_;
	}
public:
	FloatOpenGL3Texture(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu, QOpenGLTexture::Target target = QOpenGLTexture::Target2D);
	virtual ~FloatOpenGL3Texture() override = default;
};

class FloatOpenGL3TextureArray:
	public FloatOpenGL3Texture {
protected:
	virtual void setSize() override;
public:
	explicit FloatOpenGL3TextureArray(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu);
	virtual ~FloatOpenGL3TextureArray() override = default;
};

class DoubleOpenGL3Texture:
	public AbstractOpenGLTexture {
private:
	const FITS::HeaderDataUnit<FITS::DataUnit<double>>* hdu_;

	virtual void setFormat() override;
	virtual void allocateStorage() override;
	virtual void setData() override;
protected:
	virtual const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu() const override {
		return *hdu_;
	}
public:
	DoubleOpenGL3Texture(const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu, QOpenGLTexture::Target target = QOpenGLTexture::Target2D);
	virtual ~DoubleOpenGL3Texture() override = default;
};

class DoubleOpenGL3TextureArray:
	public DoubleOpenGL3Texture {
protected:
	virtual void setSize() override;
public:
	explicit DoubleOpenGL3TextureArray(const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu);
	virtual ~DoubleOpenGL3TextureArray() override = default;
};

#endif //_OPENGLTEXTURE_H
