#include "wcsdata.h"

WcsData::WcsData(const FITS::HeaderUnit& unit) : matrix_{} {
	try {
		if (unit.count("NAXIS") > 0)
			if (unit.header("NAXIS") == "2") {
				std::array<float, 4> m{};
                const std::array<float, 2> ref_px{ unit.header_as<float>("CRPIX1"), unit.header_as<float>("CRPIX2") };
				if (unit.count("CD1_1") > 0 && unit.count("CD1_2") > 0 &&
					unit.count("CD2_1") > 0 && unit.count("CD2_2") > 0) {
					m.at(0) = unit.header_as<float>("CD1_1");
					m.at(1) = unit.header_as<float>("CD1_2");
					m.at(2) = unit.header_as<float>("CD2_1");
					m.at(3) = unit.header_as<float>("CD2_2");
				}
				else if (unit.count("PC1_1") > 0 && unit.count("PC1_2") > 0 &&
					unit.count("PC2_1") > 0 && unit.count("PC2_2") > 0) {
                    const std::array<float, 2> c_delt{ unit.header_as<float>("CDELT1"), unit.header_as<float>("CDELT2") };

					m.at(0) = unit.header_as<float>("PC1_1") * c_delt.at(0);
					m.at(1) = unit.header_as<float>("PC1_2") * c_delt.at(0);
					m.at(2) = unit.header_as<float>("PC2_1") * c_delt.at(1);
					m.at(3) = unit.header_as<float>("PC2_2") * c_delt.at(1);
				}
				else FITS::Exception("WCS data is not present or unsupported. Using identity transformation.").raise();

				matrix_ = QMatrix4x4
				{  
					m.at(0), m.at(1), 0.f, unit.header_as<float>("CRVAL1") - (m.at(0) * ref_px.at(0) + m.at(1) * ref_px.at(1)),
                    m.at(2), m.at(3), 0.f, unit.header_as<float>("CRVAL2") - (m.at(2) * ref_px.at(0) + m.at(3) * ref_px.at(1)),
					0.f, 0.f, 0.f, 0.f,
					0.f, 0.f, 0.f, 1.f
				};
			}
			else qDebug() << "Only two dimensional transformation is supported. Using identity transformation.";
		else qDebug() << "Number of axes is not present. Using identity transformation.";
	}
	catch (std::exception& e) {
		qDebug() << "Error occurred during processing WCS data. See details: " << e.what();
	}
}

const QMatrix4x4& WcsData::matrix() const noexcept {
	return matrix_;
}
