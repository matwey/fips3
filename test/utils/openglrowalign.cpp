#include <QtTest/QtTest>
#include <utils/openglrowalign.h>

using namespace Utils;

class TestOpenGLRowAlign: public QObject
{
	Q_OBJECT
private slots:
	void align32();
	void align64();
};

void TestOpenGLRowAlign::align32() {
	QCOMPARE(row_align(0x1), 1);
	QCOMPARE(row_align(0x2), 2);
	QCOMPARE(row_align(0x4), 4);
	QCOMPARE(row_align(0x8), 8);
	QCOMPARE(row_align(0x10), 8);
	QCOMPARE(row_align(0x20), 8);
	QCOMPARE(row_align(0x40), 8);
	QCOMPARE(row_align(0x80), 8);
	QCOMPARE(row_align(0x10000000), 8);
	QCOMPARE(row_align(0x20000000), 8);
	QCOMPARE(row_align(0x40000000), 8);
	QCOMPARE(row_align(0x80000000ull), 8);
}

void TestOpenGLRowAlign::align64() {
	QCOMPARE(row_align(0x100000000ull), 8);
	QCOMPARE(row_align(0x200000000ull), 8);
	QCOMPARE(row_align(0x400000000ull), 8);
	QCOMPARE(row_align(0x800000000ull), 8);
	QCOMPARE(row_align(0x1000000000ull), 8);
	QCOMPARE(row_align(0x2000000000ull), 8);
	QCOMPARE(row_align(0x4000000000ull), 8);
	QCOMPARE(row_align(0x8000000000ull), 8);
	QCOMPARE(row_align(0x1000000000000000ull), 8);
	QCOMPARE(row_align(0x2000000000000000ull), 8);
	QCOMPARE(row_align(0x4000000000000000ull), 8);
	QCOMPARE(row_align(0x8000000000000000ull), 8);
}

QTEST_MAIN(TestOpenGLRowAlign)
#include "openglrowalign.moc"
