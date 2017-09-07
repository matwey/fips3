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

#include <QtTest/QtTest>
#include <utils/optional.h>

using namespace Utils;

class TestOptional: public QObject {
	Q_OBJECT
private:
	static constexpr const double x = 3.14;
private slots:
	void emptyOperatorBool();
	void filledOperatorBool();
	void emptyValue();
	void filledValue();
	void emptyValueOr();
	void filledValueOr();
	void operatorAssignmentEmptyOther();
	void operatorAssignmentFilledOther();
	void operatorAssignmentT();
	void makeOptional();
};

constexpr const double TestOptional::x;

void TestOptional::emptyOperatorBool() {
	Optional<double> op;
	QVERIFY(! op);
}

void TestOptional::filledOperatorBool() {
	Optional<double> op(x);
	QVERIFY(static_cast<bool>(op));
}

void TestOptional::emptyValue() {
	Optional<double> op;
	QVERIFY_EXCEPTION_THROWN(op.value(), BadOptionalAccess);
}

void TestOptional::filledValue() {
	Optional<double> op(x);
	QCOMPARE(x, op.value());
}

void TestOptional::emptyValueOr() {
	Optional<double> op;
	QCOMPARE(x, op.value_or(x));
}

void TestOptional::filledValueOr() {
	Optional<double> op(x);
	QCOMPARE(x, op.value_or(0));
}

void TestOptional::operatorAssignmentEmptyOther() {
	Optional<double> op1;
	Optional<double> op2(x);
	op2 = op1;
	QVERIFY(! op2);
}

void TestOptional::operatorAssignmentFilledOther() {
	Optional<double> op1(x);
	Optional<double> op2;
	op2 = op1;
	QCOMPARE(op1.value(), op2.value());
}

void TestOptional::operatorAssignmentT() {
	Optional<double> op;
	op = x;
	QCOMPARE(x, op.value());
}

void TestOptional::makeOptional() {
	auto op = make_optional(x);
	QCOMPARE(x, op.value_or(0));
}

QTEST_MAIN(TestOptional)
#include "optional.moc"