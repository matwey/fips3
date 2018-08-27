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

Uint8OpenGLTexture::Uint8OpenGLTexture(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu):
	AbstractOpenGLTexture(), hdu_(&hdu) {
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
	QOpenGLTexture::setData(QOpenGLTexture::Alpha, QOpenGLTexture::UInt8, hdu_->data().data(), &pto);
}

Uint8OpenGL3Texture::Uint8OpenGL3Texture(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu):
	AbstractOpenGLTexture(), hdu_(&hdu) {
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
	QOpenGLTexture::setData(QOpenGLTexture::Red, QOpenGLTexture::UInt8, hdu_->data().data(), &pto);
}

Int16OpenGLTexture::Int16OpenGLTexture(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu):
	AbstractOpenGLTexture(), hdu_(&hdu) {
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
	QOpenGLTexture::setData(QOpenGLTexture::LuminanceAlpha, QOpenGLTexture::UInt8, hdu_->data().data(), &pto);
}

Int16OpenGL3Texture::Int16OpenGL3Texture(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu):
	AbstractOpenGLTexture(), hdu_(&hdu) {
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
	QOpenGLTexture::setData(QOpenGLTexture::Red, QOpenGLTexture::UInt16, hdu_->data().data(), &pto);
}

Int32OpenGLTexture::Int32OpenGLTexture(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu):
	AbstractOpenGLTexture(), hdu_(&hdu) {
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
	QOpenGLTexture::setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, hdu_->data().data(), &pto);
}

Int32OpenGL3Texture::Int32OpenGL3Texture(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu):
	AbstractOpenGLTexture(), hdu_(&hdu) {
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
	QOpenGLTexture::setData(QOpenGLTexture::RG, QOpenGLTexture::UInt16, hdu_->data().data(), &pto);
}

Int64OpenGLTexture::Int64OpenGLTexture(const FITS::HeaderDataUnit<FITS::DataUnit<qint64>>& hdu):
	AbstractOpenGLTexture(), hdu_(&hdu) {
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
	QOpenGLTexture::setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt16, hdu_->data().data(), &pto);
}

FloatOpenGL3Texture::FloatOpenGL3Texture(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu):
	AbstractOpenGLTexture(), hdu_(&hdu) {
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
	QOpenGLTexture::setData(QOpenGLTexture::Red, QOpenGLTexture::Float32, hdu_->data().data(), &pto);
}

DoubleOpenGL3Texture::DoubleOpenGL3Texture(const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu):
	AbstractOpenGLTexture(), hdu_(&hdu) {
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
	QOpenGLTexture::setData(QOpenGLTexture::RG_Integer, QOpenGLTexture::UInt32, hdu_->data().data(), &pto);
}
