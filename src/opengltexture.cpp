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

#include <QDebug>
#include <QOpenGLContext>
#include <QOpenGLPixelTransferOptions>
#include <QtGlobal>

#include <opengltexture.h>
#include <utils/swapbytes.h>
#include <utils/openglrowalign.h>

Uint8OpenGLTexture::Uint8OpenGLTexture(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu):
	AbstractOpenGLTexture(1, 1), hdu_(&hdu) {

	const auto bzero = hdu.header().bzero();

	minmax_ = makeMinMax(hdu);
	instrumental_minmax_ = makeInstrumentalMinMax(hdu);
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

Int16OpenGLTexture::Int16OpenGLTexture(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu):
	AbstractOpenGLTexture(2, 1), hdu_(&hdu) {

	minmax_ = makeMinMax(hdu);
	instrumental_minmax_ = makeInstrumentalMinMax(hdu);
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

Int32OpenGLTexture::Int32OpenGLTexture(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu):
	AbstractOpenGLTexture(4, 1), hdu_(&hdu) {

	minmax_ = makeMinMax(hdu);
	instrumental_minmax_ = makeInstrumentalMinMax(hdu);
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

Int64OpenGLTexture::Int64OpenGLTexture(const FITS::HeaderDataUnit<FITS::DataUnit<qint64>>& hdu):
	AbstractOpenGLTexture(4, 2), hdu_(&hdu) {

	minmax_ = makeMinMax(hdu);
	instrumental_minmax_ = makeInstrumentalMinMax(hdu);
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
	pto.setSwapBytesEnabled(true);
	QOpenGLTexture::setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt16, hdu_->data().data(), &pto);
}

FloatOpenGLTexture::FloatOpenGLTexture(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu):
	AbstractOpenGLTexture(1, 0), hdu_(&hdu) {

	minmax_ = makeMinMax(hdu);
	instrumental_minmax_ = minmax_;
}

void FloatOpenGLTexture::setFormat() {
	static const quint64 alpha32f_arb = 0x8816;
	QOpenGLTexture::setFormat(static_cast<QOpenGLTexture::TextureFormat>(alpha32f_arb));
}

void FloatOpenGLTexture::allocateStorage() {
	QOpenGLTexture::allocateStorage(QOpenGLTexture::Alpha, QOpenGLTexture::Float32);
}

void FloatOpenGLTexture::setData() {
	QOpenGLPixelTransferOptions pto{};
	pto.setAlignment(Utils::row_align(hdu_->data().width() * 4));
	pto.setSwapBytesEnabled(true);
	QOpenGLTexture::setData(QOpenGLTexture::Alpha, QOpenGLTexture::Float32, hdu_->data().data(), &pto);
}
