#include <exception.h>

void Exception::raise() const {
	throw *this;
}
QException* Exception::clone() const {
	return new Exception(*this);
}
