#ifndef _WCSDATA_H_
#define _WCSDATA_H_

#include <QMatrix4x4>
#include <QString>

#include <array>

#include <fits.h>

class WcsData
{
public:
	explicit WcsData(const FITS::HeaderUnit&);
	~WcsData() = default;

	const QMatrix4x4& matrix() const noexcept;

private:
	QMatrix4x4 matrix_;
};
#endif
