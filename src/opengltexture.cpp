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

#include <QOpenGLContext>
#include <QOpenGLPixelTransferOptions>
#include <QtGlobal>

#include <opengltexture.h>
#include <utils/swapbytes.h>
#include <utils/openglrowalign.h>

namespace {
	inline void setSizeArray(QOpenGLTexture& tex, const FITS::ImageDataUnit& du) {
		tex.setLayers(du.depth());
		tex.setSize(du.width(), du.height());
	}
}

Uint8OpenGLTexture::Uint8OpenGLTexture(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu, QOpenGLTexture::Target target):
	AbstractOpenGLTexture(target), hdu_(&hdu) {
}

void Uint8OpenGLTexture::setFormat() {
	QOpenGLTexture::setFormat(QOpenGLTexture::AlphaFormat);
}

void Uint8OpenGLTexture::allocateStorage() {
	QOpenGLTexture::allocateStorage(QOpenGLTexture::Alpha, QOpenGLTexture::UInt8);
}

void Uint8OpenGLTexture::setData() {
	QOpenGLPixelTransferOptions pto{};
	pto.setAlignment(Utils::row_align(hdu_->data().width()));
	pto.setSwapBytesEnabled(false);
	AbstractOpenGLTexture::setData(QOpenGLTexture::Alpha, QOpenGLTexture::UInt8, hdu().data(), &pto);
}

Uint8OpenGLTextureArray::Uint8OpenGLTextureArray(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu):
	Uint8OpenGLTexture(hdu, QOpenGLTexture::Target2DArray) {
}

void Uint8OpenGLTextureArray::setSize() {
	setSizeArray(*this, hdu().data());
}

Uint8OpenGL3Texture::Uint8OpenGL3Texture(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu, QOpenGLTexture::Target target):
	AbstractOpenGLTexture(target), hdu_(&hdu) {
}

void Uint8OpenGL3Texture::setFormat() {
	QOpenGLTexture::setFormat(QOpenGLTexture::R8_UNorm);
}

void Uint8OpenGL3Texture::allocateStorage() {
	QOpenGLTexture::allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::UInt8);
}

void Uint8OpenGL3Texture::setData() {
	QOpenGLPixelTransferOptions pto{};
	pto.setAlignment(Utils::row_align(hdu_->data().width()));
	pto.setSwapBytesEnabled(false);
	AbstractOpenGLTexture::setData(QOpenGLTexture::Red, QOpenGLTexture::UInt8, hdu().data(), &pto);
}

Uint8OpenGL3TextureArray::Uint8OpenGL3TextureArray(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu):
	Uint8OpenGL3Texture(hdu, QOpenGLTexture::Target2DArray) {
}

void Uint8OpenGL3TextureArray::setSize() {
	setSizeArray(*this, hdu().data());
}

Uint8OpenGLES3Texture::Uint8OpenGLES3Texture(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu, QOpenGLTexture::Target target):
	AbstractOpenGLTexture(target), hdu_(&hdu) {
}

void Uint8OpenGLES3Texture::setFormat() {
	QOpenGLTexture::setFormat(QOpenGLTexture::R8_UNorm);
}

void Uint8OpenGLES3Texture::allocateStorage() {
	QOpenGLTexture::allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::UInt8);
}

void Uint8OpenGLES3Texture::setData() {
	QOpenGLPixelTransferOptions pto{};
	pto.setAlignment(Utils::row_align(hdu_->data().width()));
	AbstractOpenGLTexture::setData(QOpenGLTexture::Red, QOpenGLTexture::UInt8, hdu().data(), &pto);
}

Uint8OpenGLES3TextureArray::Uint8OpenGLES3TextureArray(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu):
	Uint8OpenGLES3Texture(hdu, QOpenGLTexture::Target2DArray) {
}

void Uint8OpenGLES3TextureArray::setSize() {
	setSizeArray(*this, hdu().data());
}

Int16OpenGLTexture::Int16OpenGLTexture(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu, QOpenGLTexture::Target target):
	AbstractOpenGLTexture(target), hdu_(&hdu) {
}

void Int16OpenGLTexture::setFormat() {
	QOpenGLTexture::setFormat(QOpenGLTexture::LuminanceAlphaFormat);
}

void Int16OpenGLTexture::allocateStorage() {
	QOpenGLTexture::allocateStorage(QOpenGLTexture::LuminanceAlpha, QOpenGLTexture::UInt8);
}

void Int16OpenGLTexture::setData() {
	QOpenGLPixelTransferOptions pto{};
	pto.setAlignment(Utils::row_align(hdu_->data().width() * 2));
	pto.setSwapBytesEnabled(false);
	AbstractOpenGLTexture::setData(QOpenGLTexture::LuminanceAlpha, QOpenGLTexture::UInt8, hdu().data(), &pto);
}

Int16OpenGLTextureArray::Int16OpenGLTextureArray(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu):
	Int16OpenGLTexture(hdu, QOpenGLTexture::Target2DArray) {
}

void Int16OpenGLTextureArray::setSize() {
	setSizeArray(*this, hdu().data());
}

Int16OpenGL3Texture::Int16OpenGL3Texture(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu, QOpenGLTexture::Target target):
	AbstractOpenGLTexture(target), hdu_(&hdu) {
}

void Int16OpenGL3Texture::setFormat() {
	QOpenGLTexture::setFormat(QOpenGLTexture::R16_UNorm);
}

void Int16OpenGL3Texture::allocateStorage() {
	QOpenGLTexture::allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::UInt16);
}

void Int16OpenGL3Texture::setData() {
	QOpenGLPixelTransferOptions pto{};
	pto.setAlignment(Utils::row_align(hdu_->data().width() * 2));
	pto.setSwapBytesEnabled(is_little_endian);
	AbstractOpenGLTexture::setData(QOpenGLTexture::Red, QOpenGLTexture::UInt16, hdu().data(), &pto);
}

Int16OpenGL3TextureArray::Int16OpenGL3TextureArray(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu):
	Int16OpenGL3Texture(hdu, QOpenGLTexture::Target2DArray) {
}

void Int16OpenGL3TextureArray::setSize() {
	setSizeArray(*this, hdu().data());
}

Int16OpenGLES3Texture::Int16OpenGLES3Texture(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu, QOpenGLTexture::Target target):
	AbstractOpenGLTexture(target), hdu_(&hdu) {
}

void Int16OpenGLES3Texture::setFormat() {
	QOpenGLTexture::setFormat(QOpenGLTexture::RG8_UNorm);
}

void Int16OpenGLES3Texture::allocateStorage() {
	QOpenGLTexture::allocateStorage(QOpenGLTexture::RG, QOpenGLTexture::UInt8);
}

void Int16OpenGLES3Texture::setData() {
	QOpenGLPixelTransferOptions pto{};
	pto.setAlignment(Utils::row_align(hdu_->data().width() * 2));
	AbstractOpenGLTexture::setData(QOpenGLTexture::RG, QOpenGLTexture::UInt8, hdu().data(), &pto);
}

Int16OpenGLES3TextureArray::Int16OpenGLES3TextureArray(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu):
	Int16OpenGLES3Texture(hdu, QOpenGLTexture::Target2DArray) {
}

void Int16OpenGLES3TextureArray::setSize() {
	setSizeArray(*this, hdu().data());
}

Int32OpenGLTexture::Int32OpenGLTexture(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu, QOpenGLTexture::Target target):
	AbstractOpenGLTexture(target), hdu_(&hdu) {
}

void Int32OpenGLTexture::setFormat() {
	QOpenGLTexture::setFormat(QOpenGLTexture::RGBAFormat);
}

void Int32OpenGLTexture::allocateStorage() {
	QOpenGLTexture::allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8);
}

void Int32OpenGLTexture::setData() {
	QOpenGLPixelTransferOptions pto{};
	pto.setAlignment(Utils::row_align(hdu_->data().width() * 4));
	pto.setSwapBytesEnabled(false);
	AbstractOpenGLTexture::setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, hdu().data(), &pto);
}

Int32OpenGLTextureArray::Int32OpenGLTextureArray(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu):
	Int32OpenGLTexture(hdu, QOpenGLTexture::Target2DArray) {
}

void Int32OpenGLTextureArray::setSize() {
	setSizeArray(*this, hdu().data());
}

Int32OpenGL3Texture::Int32OpenGL3Texture(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu, QOpenGLTexture::Target target):
	AbstractOpenGLTexture(target), hdu_(&hdu) {
}

void Int32OpenGL3Texture::setFormat() {
	QOpenGLTexture::setFormat(QOpenGLTexture::RG16_UNorm);
}

void Int32OpenGL3Texture::allocateStorage() {
	QOpenGLTexture::allocateStorage(QOpenGLTexture::RG, QOpenGLTexture::UInt16);
}

void Int32OpenGL3Texture::setData() {
	QOpenGLPixelTransferOptions pto{};
	pto.setAlignment(Utils::row_align(hdu_->data().width() * 4));
	pto.setSwapBytesEnabled(is_little_endian);
	AbstractOpenGLTexture::setData(QOpenGLTexture::RG, QOpenGLTexture::UInt16, hdu().data(), &pto);
}

Int32OpenGL3TextureArray::Int32OpenGL3TextureArray(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu):
	Int32OpenGL3Texture(hdu, QOpenGLTexture::Target2DArray) {
}

void Int32OpenGL3TextureArray::setSize() {
	setSizeArray(*this, hdu().data());
}

Int32OpenGLES3Texture::Int32OpenGLES3Texture(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu, QOpenGLTexture::Target target):
	AbstractOpenGLTexture(target), hdu_(&hdu) {
}

void Int32OpenGLES3Texture::setFormat() {
	QOpenGLTexture::setFormat(QOpenGLTexture::RGBA8_UNorm);
}

void Int32OpenGLES3Texture::allocateStorage() {
	QOpenGLTexture::allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8);
}

void Int32OpenGLES3Texture::setData() {
	QOpenGLPixelTransferOptions pto{};
	pto.setAlignment(Utils::row_align(hdu_->data().width() * 4));
	AbstractOpenGLTexture::setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, hdu().data(), &pto);
}

Int32OpenGLES3TextureArray::Int32OpenGLES3TextureArray(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu):
	Int32OpenGLES3Texture(hdu, QOpenGLTexture::Target2DArray) {
}

void Int32OpenGLES3TextureArray::setSize() {
	setSizeArray(*this, hdu().data());
}

Int64OpenGLTexture::Int64OpenGLTexture(const FITS::HeaderDataUnit<FITS::DataUnit<qint64>>& hdu, QOpenGLTexture::Target target):
	AbstractOpenGLTexture(target), hdu_(&hdu) {
}

void Int64OpenGLTexture::setFormat() {
	QOpenGLTexture::setFormat(QOpenGLTexture::RGBA16_UNorm);
}

void Int64OpenGLTexture::allocateStorage() {
	QOpenGLTexture::allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::UInt16);
}

void Int64OpenGLTexture::setData() {
	QOpenGLPixelTransferOptions pto{};
	pto.setAlignment(Utils::row_align(hdu_->data().width() * 8));
	pto.setSwapBytesEnabled(is_little_endian);
	AbstractOpenGLTexture::setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt16, hdu().data(), &pto);
}

Int64OpenGLTextureArray::Int64OpenGLTextureArray(const FITS::HeaderDataUnit<FITS::DataUnit<qint64>>& hdu):
	Int64OpenGLTexture(hdu, QOpenGLTexture::Target2DArray) {
}

void Int64OpenGLTextureArray::setSize() {
	setSizeArray(*this, hdu().data());
}

FloatOpenGL3Texture::FloatOpenGL3Texture(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu, QOpenGLTexture::Target target):
	AbstractOpenGLTexture(target), hdu_(&hdu) {
}

void FloatOpenGL3Texture::setFormat() {
	QOpenGLTexture::setFormat(QOpenGLTexture::R32F);
}

void FloatOpenGL3Texture::allocateStorage() {
	QOpenGLTexture::allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::Float32);
}

void FloatOpenGL3Texture::setData() {
	QOpenGLPixelTransferOptions pto{};
	pto.setAlignment(Utils::row_align(hdu_->data().width() * 4));
	pto.setSwapBytesEnabled(is_little_endian);
	AbstractOpenGLTexture::setData(QOpenGLTexture::Red, QOpenGLTexture::Float32, hdu().data(), &pto);
}

FloatOpenGL3TextureArray::FloatOpenGL3TextureArray(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu):
	FloatOpenGL3Texture(hdu, QOpenGLTexture::Target2DArray) {
}

void FloatOpenGL3TextureArray::setSize() {
	setSizeArray(*this, hdu().data());
}

FloatOpenGLES3Texture::FloatOpenGLES3Texture(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu, QOpenGLTexture::Target target):
	AbstractOpenGLTexture(target), hdu_(&hdu) {
}

void FloatOpenGLES3Texture::setFormat() {
	QOpenGLTexture::setFormat(QOpenGLTexture::RGBA8U);
}

void FloatOpenGLES3Texture::allocateStorage() {
	QOpenGLTexture::allocateStorage(QOpenGLTexture::RGBA_Integer, QOpenGLTexture::UInt8);
}

void FloatOpenGLES3Texture::setData() {
	QOpenGLPixelTransferOptions pto{};
	pto.setAlignment(Utils::row_align(hdu_->data().width() * 4));
	AbstractOpenGLTexture::setData(QOpenGLTexture::RGBA_Integer, QOpenGLTexture::UInt8, hdu().data(), &pto);
}

DoubleOpenGL3Texture::DoubleOpenGL3Texture(const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu, QOpenGLTexture::Target target):
	AbstractOpenGLTexture(target), hdu_(&hdu) {
}

void DoubleOpenGL3Texture::setFormat() {
	QOpenGLTexture::setFormat(QOpenGLTexture::RG32U);
}

void DoubleOpenGL3Texture::allocateStorage() {
	QOpenGLTexture::allocateStorage(QOpenGLTexture::RG_Integer, QOpenGLTexture::UInt32);
}

void DoubleOpenGL3Texture::setData() {
	QOpenGLPixelTransferOptions pto{};
	pto.setAlignment(Utils::row_align(hdu_->data().width() * 8));
	pto.setSwapBytesEnabled(is_little_endian);
	AbstractOpenGLTexture::setData(QOpenGLTexture::RG_Integer, QOpenGLTexture::UInt32, hdu().data(), &pto);
}

DoubleOpenGL3TextureArray::DoubleOpenGL3TextureArray(const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu):
	DoubleOpenGL3Texture(hdu, QOpenGLTexture::Target2DArray) {
}

void DoubleOpenGL3TextureArray::setSize() {
	setSizeArray(*this, hdu().data());
}
