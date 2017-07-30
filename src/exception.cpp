#include <exception.h>

Exception::Exception(const QString& what): what_(what) {
}
void Exception::raise() const {
	throw *this;
}
QException* Exception::clone() const {
	return new Exception(*this);
}
const char* Exception::what() const noexcept {
	return what_.toLatin1().constData();
}
