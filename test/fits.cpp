#include <QtTest/QtTest>
#include <QFile>
#include <fits.h>

#define DATA_ROOT PROJECT_ROOT "/test/data"

class TestFits: public QObject
{
	Q_OBJECT
private slots:
	void pageAdvance1();
	void pageAdvanceInBytes1();
	void pageAbstractFITSStorage1();
	void parseHeaderUnit1();
	void parseHeaderUnit2();
	void parseHeaderUnit3();
	void parseHeaderUnit4();
	void parseHeaderBzero1();
	void parseHeaderBscale1();
	void parseDataUnitShape();
	void visitDataUnit1();
};

void TestFits::pageAdvance1() {
	uint8_t x;

	AbstractFITSStorage::Page page(&x);
	QCOMPARE(page.data(), &x);
	page.advance(0);
	QCOMPARE(page.data(), &x);
	page.advance(1);
	QCOMPARE(page.data(), &x + 2880);
	page.advance(1);
	QCOMPARE(page.data(), &x + 2*2880);
}
void TestFits::pageAdvanceInBytes1() {
	uint8_t x;

	AbstractFITSStorage::Page page(&x);
	QCOMPARE(page.data(), &x);
	page.advanceInBytes(0);
	QCOMPARE(page.data(), &x);
	page.advanceInBytes(1);
	QCOMPARE(page.data(), &x + 2880);
	page.advanceInBytes(2880);
	QCOMPARE(page.data(), &x + 2*2880);
}
void TestFits::pageAbstractFITSStorage1() {
	uint8_t x;

	struct S: public AbstractFITSStorage {
		inline S(quint8* data, qint64 size): AbstractFITSStorage(data, size) {}
	};

	S s1(&x, 0);
	QCOMPARE(s1.cbegin().data(), s1.cend().data());

	S s2(&x, 1);
	QCOMPARE(s2.cbegin().data(), s2.cend().data());

	S s3(&x, 2880);
	QCOMPARE(s3.cbegin().data() + 2880, s3.cend().data());

	S s4(&x, 2881);
	QCOMPARE(s4.cbegin().data() + 2880, s4.cend().data());
}
void TestFits::parseHeaderUnit1() {
	QFile* file = new QFile(DATA_ROOT "/header_unexpected_end.fits");
	file->open(QIODevice::ReadOnly);
	QVERIFY_EXCEPTION_THROWN(FITS fits(file), FITS::UnexpectedEnd);
}
void TestFits::parseHeaderUnit2() {
	QFile* file = new QFile(DATA_ROOT "/header_end.fits");
	file->open(QIODevice::ReadOnly);
	FITS fits(file);
	QTRY_COMPARE(fits.header_unit().header("SIMPLE"), QString("T"));
	QTRY_COMPARE(fits.header_unit().header("BITPIX"), QString("16"));
	QTRY_COMPARE(fits.header_unit().header("NAXIS"), QString("2"));
	QTRY_COMPARE(fits.header_unit().header("NAXIS1"), QString("10"));
	QTRY_COMPARE(fits.header_unit().header("NAXIS2"), QString("10"));
}
void TestFits::parseHeaderUnit3() {
	quint8 end[] = "END     ";

	AbstractFITSStorage::Page page_begin(end);
	AbstractFITSStorage::Page page_end(end + 2880);

	FITS::HeaderUnit hdr(page_begin, page_end);
	QTRY_COMPARE(page_begin, page_end);
}
void TestFits::parseHeaderUnit4() {
	quint8 end[] = "END     ";

	AbstractFITSStorage::Page page_begin(end);
	AbstractFITSStorage::Page header_end(end + 2880);
	AbstractFITSStorage::Page page_end(end + 2 * 2880);

	FITS::HeaderUnit hdr(page_begin, page_end);
	QTRY_COMPARE(page_begin, header_end);
}
void TestFits::parseHeaderBzero1() {
	QFile* file = new QFile(DATA_ROOT "/header_end.fits");
	file->open(QIODevice::ReadOnly);
	FITS fits(file);
	QCOMPARE(fits.header_unit().bzero(), 0.0);
}
void TestFits::parseHeaderBscale1() {
	QFile* file = new QFile(DATA_ROOT "/header_end.fits");
	file->open(QIODevice::ReadOnly);
	FITS fits(file);
	QCOMPARE(fits.header_unit().bscale(), 1.0);
}
void TestFits::parseDataUnitShape() {
	QFile* file = new QFile(DATA_ROOT "/sombrero8.fits");
	file->open(QIODevice::ReadOnly);
	const FITS fits(file);
	auto du = fits.data_unit().imageDataUnit();
	QTRY_COMPARE(du->height(), static_cast<quint64>(448));
	QTRY_COMPARE(du->width (), static_cast<quint64>(800));
}
void TestFits::visitDataUnit1() {
	QFile* file = new QFile(DATA_ROOT "/header_end.fits");
	file->open(QIODevice::ReadOnly);
	FITS fits(file);
	struct test_fun {
		void operator() (const FITS::DataUnit<quint8>&) const {
			QFAIL("Wrong overloading");
		}
		void operator() (const FITS::DataUnit<qint16>&) const {
		}
		void operator() (const FITS::DataUnit<qint32>&) const {
			QFAIL("Wrong overloading");
		}
		void operator() (const FITS::DataUnit<qint64>&) const {
			QFAIL("Wrong overloading");
		}
		void operator() (const FITS::DataUnit<float>&) const {
			QFAIL("Wrong overloading");
		}
		void operator() (const FITS::DataUnit<double>&) const {
			QFAIL("Wrong overloading");
		}
	};
	fits.data_unit().apply(test_fun{});
}

QTEST_MAIN(TestFits)
#include "fits.moc"
