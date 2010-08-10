/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "otbMacro.h"

#include "otbSarImageMetadataInterface.h"

#include "itkMetaDataObject.h"
#include "otbVectorDataKeywordlist.h"
#include "base/ossimKeywordlist.h"

namespace otb
{

SarImageMetadataInterface
::SarImageMetadataInterface()
{
}


SarImageMetadataInterface::RealType
SarImageMetadataInterface
::GetRadiometricCalibrationScale() const
{
	return static_cast<SarImageMetadataInterface::RealType>(1.0);
}

SarImageMetadataInterface::PointSetPointer
SarImageMetadataInterface
::GetConstantValuePointSet(const RealType& value) const
{
	PointSetPointer pointSet  = PointSetType::New();

	PointType  p0;

	pointSet->Initialize();

	p0[0] = static_cast<unsigned int>(0);
	p0[1] = static_cast<unsigned int>(0);
	pointSet->SetPoint(0, p0);
	pointSet->SetPointData(0, value);

	return pointSet;
}

SarImageMetadataInterface::PointSetPointer
SarImageMetadataInterface
::GetRadiometricCalibrationNoise() const
{
	return SarImageMetadataInterface::GetConstantValuePointSet(0.0);
}


SarImageMetadataInterface::PointSetPointer
SarImageMetadataInterface
::GetRadiometricCalibrationAntennaPatternNewGain() const
{
	return SarImageMetadataInterface::GetConstantValuePointSet(1.0);
}


SarImageMetadataInterface::PointSetPointer
SarImageMetadataInterface
::GetRadiometricCalibrationAntennaPatternOldGain() const
{
	return SarImageMetadataInterface::GetConstantValuePointSet(1.0);
}


SarImageMetadataInterface::PointSetPointer
SarImageMetadataInterface
::GetRadiometricCalibrationIncidenceAngle() const
{
	return SarImageMetadataInterface::GetConstantValuePointSet(M_PI/2.0);
}


SarImageMetadataInterface::PointSetPointer
SarImageMetadataInterface
::GetRadiometricCalibrationRangeSpreadLoss() const
{
	return SarImageMetadataInterface::GetConstantValuePointSet(1.0);
}


SarImageMetadataInterface::IndexType
SarImageMetadataInterface
::GetConstantPolynomialDegree() const
{
	IndexType polynomialDegree;
	polynomialDegree[0] = 0;
	polynomialDegree[1] = 0;
	return polynomialDegree;
}

SarImageMetadataInterface::IndexType
SarImageMetadataInterface
::GetRadiometricCalibrationNoisePolynomialDegree() const
{
	 return SarImageMetadataInterface::GetConstantPolynomialDegree();
}

SarImageMetadataInterface::IndexType
SarImageMetadataInterface
::GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree() const
{
	 return SarImageMetadataInterface::GetConstantPolynomialDegree();
}

SarImageMetadataInterface::IndexType
SarImageMetadataInterface
::GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree() const
{
	 return SarImageMetadataInterface::GetConstantPolynomialDegree();
}


SarImageMetadataInterface::IndexType
SarImageMetadataInterface
::GetRadiometricCalibrationIncidenceAnglePolynomialDegree() const
{
	 return SarImageMetadataInterface::GetConstantPolynomialDegree();
}


SarImageMetadataInterface::IndexType
SarImageMetadataInterface
::GetRadiometricCalibrationRangeSpreadLossPolynomialDegree() const
{
	 return SarImageMetadataInterface::GetConstantPolynomialDegree();
}



void
SarImageMetadataInterface
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  //os << indent << "GetSunElevation:     " << this->GetSunElevation() << std::endl;
}



} // end namespace otb
