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

#include "otbSarImageMetadataInterface.h"

#include "otbMath.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"

namespace otb
{

SarImageMetadataInterface
::SarImageMetadataInterface()
{

}

const std::string
SarImageMetadataInterface
::GetProductType() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata");
    }

    ImageKeywordlistType imageKeywordlist;

    if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
      {
      itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
      }

    if (imageKeywordlist.HasKey("support_data.product_type"))
      {
      const std::string product_type = imageKeywordlist.GetMetadataByKey("support_data.product_type");
      return product_type;
      }
    return "";
}

const std::string
SarImageMetadataInterface
::GetAcquisitionMode() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata");
    }

    ImageKeywordlistType imageKeywordlist;

    if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
      {
      itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
      }

    if (imageKeywordlist.HasKey("support_data.acquisition_mode"))
      {
      const std::string acquisition_mode = imageKeywordlist.GetMetadataByKey("support_data.acquisition_mode");
      return acquisition_mode;
      }
    return "";
}

void
SarImageMetadataInterface
::CreateCalibrationLookupData(const short itkNotUsed(t))
{

}

const SarImageMetadataInterface::LookupDataPointerType
SarImageMetadataInterface
::GetCalibrationLookupData(const short type)
{
  if (HasCalibrationLookupDataFlag())
    {
    CreateCalibrationLookupData(type);
    }

  return m_SarLut;
}

bool
SarImageMetadataInterface
::HasCalibrationLookupDataFlag() const
{
  const ImageKeywordlist imageKeywordlist = this->GetImageKeywordlist();
  /* checking if the key exist is more than enough */
  if (imageKeywordlist.HasKey("support_data.calibration_lookup_flag"))
    {
    return true;
    }
  return false;
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
  return SarImageMetadataInterface::GetConstantValuePointSet(CONST_PI_2);
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

double
SarImageMetadataInterface
::GetRescalingFactor() const
{
  return 1.0;
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

  if (this->CanRead())
    {
    os << indent << "GetRadiometricCalibrationScale:                 " << this->GetRadiometricCalibrationScale() << std::endl;
    os << indent << "GetRadiometricCalibrationNoise:                 " << this->GetRadiometricCalibrationNoise() << std::endl;
    os << indent << "GetRadiometricCalibrationAntennaPatternNewGain: " << this->GetRadiometricCalibrationAntennaPatternNewGain() << std::endl;
    os << indent << "GetRadiometricCalibrationAntennaPatternOldGain: " << this->GetRadiometricCalibrationAntennaPatternOldGain() << std::endl;
    os << indent << "GetRadiometricCalibrationIncidenceAngle:        " << this->GetRadiometricCalibrationIncidenceAngle() << std::endl;
    os << indent << "GetRadiometricCalibrationRangeSpreadLoss:       " << this->GetRadiometricCalibrationRangeSpreadLoss() << std::endl;
    os << indent << "GetConstantPolynomialDegree:                    " << this->GetConstantPolynomialDegree() << std::endl;
    os << indent << "GetRadiometricCalibrationNoisePolynomialDegree: " << this->GetRadiometricCalibrationNoisePolynomialDegree() << std::endl;
    os << indent << "GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree: " << this->GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree() << std::endl;
    os << indent << "GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree: " << this->GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree() << std::endl;
    os << indent << "GetRadiometricCalibrationIncidenceAnglePolynomialDegree:        " << this->GetRadiometricCalibrationIncidenceAnglePolynomialDegree() << std::endl;
    os << indent << "GetRadiometricCalibrationRangeSpreadLossPolynomialDegree:       " << this->GetRadiometricCalibrationRangeSpreadLossPolynomialDegree() << std::endl;
    os << indent << "GetPRF:                  " << this->GetPRF() << std::endl;
    os << indent << "GetRSF:                  " << this->GetRSF() << std::endl;
    os << indent << "GetRadarFrequency:       " << this->GetRadarFrequency() << std::endl;
    os << indent << "GetCenterIncidenceAngle: " << this->GetCenterIncidenceAngle() << std::endl;
    }
}


} // end namespace otb
