#include <QtTest/QtTest>
#include <utils/swapbytes.h>

using namespace Utils;

class TestSwapBytes: public QObject
{
	Q_OBJECT
private slots:
	void swap8();
	void swap16();
	void swap16_signed();
	void swap32();
	void swap32_signed();
	void swap32_float();
	void swap64();
	void swap64_signed();
	void swap64_float();
};

void TestSwapBytes::swap8() {
	quint8 x = 1;
	QCOMPARE(swap_bytes(x), x);

}
void TestSwapBytes::swap16() {
	QCOMPARE(swap_bytes(static_cast<quint16>(0x1234)), static_cast<quint16>(0x3412));
}
void TestSwapBytes::swap16_signed() {
	QCOMPARE(swap_bytes(static_cast<qint16>(0x1234)), static_cast<qint16>(0x3412));
	QCOMPARE(swap_bytes(static_cast<qint16>(0x8234)), static_cast<qint16>(0x3482));
}
void TestSwapBytes::swap32() {
	QCOMPARE(swap_bytes(static_cast<quint32>(0x12345678u)), static_cast<quint32>(0x78563412u));
}
void TestSwapBytes::swap32_signed() {
	QCOMPARE(swap_bytes(static_cast<qint32>(0x12345678)), static_cast<qint32>(0x78563412));
	QCOMPARE(swap_bytes(static_cast<qint32>(0x82345678)), static_cast<qint32>(0x78563482));
}
void TestSwapBytes::swap32_float() {
	const float zero = 0.0;
	const float minus_zero = -0.0;
	QCOMPARE(swap_bytes(zero), zero);
	QCOMPARE(swap_bytes(minus_zero), 1.79366203433576585078237386661E-43f);
}
void TestSwapBytes::swap64() {
	QCOMPARE(swap_bytes(static_cast<quint64>(0x123456789abcdef0u)), static_cast<quint64>(0xf0debc9a78563412u));
}
void TestSwapBytes::swap64_signed() {
	QCOMPARE(swap_bytes(static_cast<qint64>(0x123456789abcdef0)), static_cast<qint64>(0xf0debc9a78563412));
	QCOMPARE(swap_bytes(static_cast<qint64>(0x823456789abcdef0)), static_cast<qint64>(0xf0debc9a78563482));
}
void TestSwapBytes::swap64_float() {
	const double zero = 0.0;
	const double minus_zero = -0.0;
	QCOMPARE(swap_bytes(zero), zero);
	QCOMPARE(swap_bytes(minus_zero), 6.32404026676795576546008054871E-322);
}

QTEST_MAIN(TestSwapBytes)
#include "swapbytes.moc"
