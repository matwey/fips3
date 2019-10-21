#include <QRectF>
#include <QtTest/QtTest>
#include <borderrectf.h>

class TestBorderRectF: public QObject
{
	Q_OBJECT
private slots:
	void alignRect1();
	void alignRect2();
	void alignRect3();
	void alignRect4();
	void alignRect5();
	void alignRect6();
	void alignRect7();
	void alignRect8();
	void circumRect1();
	void circumRect2();
	void circumRect3();
	void circumRect4();
	void circumRect5();
	void circumRect6();
};

void TestBorderRectF::alignRect1() {
	BorderRectF border{0.0, 0.0, 1.0, 1.0};
	QRectF rect{-0.5, 0.0, 1.0, 1.0};
	const QRectF expected{0.0, 0.0, 1.0, 1.0};

	rect = border.alignRect(rect);
	QCOMPARE(rect, expected);
}

void TestBorderRectF::alignRect2() {
	BorderRectF border{0.0, 0.0, 1.0, 1.0};
	QRectF rect{+0.5, 0.0, 1.0, 1.0};
	const QRectF expected{0.0, 0.0, 1.0, 1.0};

	rect = border.alignRect(rect);
	QCOMPARE(rect, expected);
}

void TestBorderRectF::alignRect3() {
	BorderRectF border{0.0, 0.0, 1.0, 1.0};
	QRectF rect{0.0, -0.5, 1.0, 1.0};
	const QRectF expected{0.0, 0.0, 1.0, 1.0};

	rect = border.alignRect(rect);
	QCOMPARE(rect, expected);
}

void TestBorderRectF::alignRect4() {
	BorderRectF border{0.0, 0.0, 1.0, 1.0};
	QRectF rect{0.0, 0.5, 1.0, 1.0};
	const QRectF expected{0.0, 0.0, 1.0, 1.0};

	rect = border.alignRect(rect);
	QCOMPARE(rect, expected);
}

void TestBorderRectF::alignRect5() {
	BorderRectF border{0.0, 0.0, 1.0, 1.0};
	QRectF rect{-0.5, -0.5, 1.0, 1.0};
	const QRectF expected{0.0, 0.0, 1.0, 1.0};

	rect = border.alignRect(rect);
	QCOMPARE(rect, expected);
}

void TestBorderRectF::alignRect6() {
	BorderRectF border{0.0, 0.0, 1.0, 1.0};
	QRectF rect{0.5, 0.5, 1.0, 1.0};
	const QRectF expected{0.0, 0.0, 1.0, 1.0};

	rect = border.alignRect(rect);
	QCOMPARE(rect, expected);
}

void TestBorderRectF::alignRect7() {
	BorderRectF border{0.0, 0.0, 1.0, 1.0};
	QRectF rect{0.0, 0.0, 2.0, 2.0};
	const QRectF expected{-0.5, -0.5, 2.0, 2.0};

	rect = border.alignRect(rect);
	QCOMPARE(rect, expected);
}

void TestBorderRectF::alignRect8() {
	BorderRectF border{0.0, 0.0, 1.0, 1.0};
	QRectF rect{0.0, 0.0, 1.0, 1.0};
	const QRectF expected{0.0, 0.0, 1.0, 1.0};

	rect = border.alignRect(rect);
	QCOMPARE(rect, expected);
}

void TestBorderRectF::circumRect1() {
	BorderRectF border{0.0, 0.0, 1.0, 1.0};
	QRectF rect{0.25, 0.25, 0.5, 0.5};
	const QRectF expected{0.0, 0.0, 1.0, 1.0};

	rect = border.circumRect(rect);
	QCOMPARE(rect, expected);
}

void TestBorderRectF::circumRect2() {
	BorderRectF border{0.0, 0.0, 1.0, 1.0};
	QRectF rect{0.0, 0.25, 1.0, 0.5};
	const QRectF expected{-0.5, 0.0, 2.0, 1.0};

	rect = border.circumRect(rect);
	QCOMPARE(rect, expected);
}

void TestBorderRectF::circumRect3() {
	BorderRectF border{0.0, 0.0, 1.0, 1.0};
	QRectF rect{0.25, 0.0, 0.5, 1.0};
	const QRectF expected{0.0, -0.5, 1.0, 2.0};

	rect = border.circumRect(rect);
	QCOMPARE(rect, expected);
}

void TestBorderRectF::circumRect4() {
	BorderRectF border{0.0, 0.0, 1.0, 1.0};
	QRectF rect{0.0, 0.0, 2.0, 2.0};
	const QRectF expected{0.0, 0.0, 1.0, 1.0};

	rect = border.circumRect(rect);
	QCOMPARE(rect, expected);
}

void TestBorderRectF::circumRect5() {
	BorderRectF border{0.0, 0.0, 1.0, 1.0};
	QRectF rect{0.0, 0.0, 4.0, 2.0};
	const QRectF expected{-0.5, 0.0, 2.0, 1.0};

	rect = border.circumRect(rect);
	QCOMPARE(rect, expected);
}

void TestBorderRectF::circumRect6() {
	BorderRectF border{0.0, 0.0, 1.0, 1.0};
	QRectF rect{0.0, 0.0, 2.0, 4.0};
	const QRectF expected{0.0, -0.5, 1.0, 2.0};

	rect = border.circumRect(rect);
	QCOMPARE(rect, expected);
}

QTEST_MAIN(TestBorderRectF)
#include "borderrectf.moc"
