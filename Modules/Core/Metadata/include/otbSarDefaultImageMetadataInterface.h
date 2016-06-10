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
#ifndef __otbSarDefaultImageMetadataInterface_h
#define __otbSarDefaultImageMetadataInterface_h


#include "otbMacro.h"

#include "otbSarImageMetadataInterface.h"

namespace otb
{
/** \class SarDefaultImageMetadataInterface
 *
 * \brief Creation of an "otb" DefaultImageMetadataInterface that gets metadata.
 *
 *
 * \ingroup OTBMetadata
 */
class ITK_EXPORT SarDefaultImageMetadataInterface : public SarImageMetadataInterface
{
public:

  typedef SarDefaultImageMetadataInterface Self;
  typedef SarImageMetadataInterface    Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SarDefaultImageMetadataInterface, SarImageMetadataInterface);

  typedef Superclass::ImageType                      ImageType;
  typedef ImageType::IndexType                       IndexType;
  typedef Superclass::MetaDataDictionaryType         MetaDataDictionaryType;
  typedef Superclass::VectorType                     VectorType;
  typedef Superclass::VariableLengthVectorType       VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType           ImageKeywordlistType;
  typedef itk::PointSet<double, 2>                   PointSetType;
  typedef PointSetType::Pointer                      PointSetPointer;
  typedef double                                     RealType;
  typedef PointSetType::PointType                    PointType;

  RealType GetRadiometricCalibrationScale() const ITK_OVERRIDE
  {
    itkExceptionMacro("GetRadiometricCalibrationScale() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  PointSetPointer GetRadiometricCalibrationAntennaPatternNewGain() const ITK_OVERRIDE
  {
    itkExceptionMacro("GetRadiometricCalibrationAntennaPatternNewGain() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  PointSetPointer GetRadiometricCalibrationAntennaPatternOldGain() const ITK_OVERRIDE
  {
    itkExceptionMacro("GetRadiometricCalibrationAntennaPatternOldGain() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  PointSetPointer GetRadiometricCalibrationIncidenceAngle() const ITK_OVERRIDE
  {
    itkExceptionMacro("GetRadiometricCalibrationIncidenceAngle() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  PointSetPointer GetRadiometricCalibrationRangeSpreadLoss() const ITK_OVERRIDE
  {
    itkExceptionMacro("GetRadiometricCalibrationRangeSpreadLoss() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  PointSetPointer GetRadiometricCalibrationNoise() const ITK_OVERRIDE
  {
    itkExceptionMacro("GetRadiometricCalibrationNoise() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  IndexType GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree() const ITK_OVERRIDE
  {
    itkExceptionMacro("GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  IndexType GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree() const ITK_OVERRIDE
  {
    itkExceptionMacro("GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  IndexType GetRadiometricCalibrationIncidenceAnglePolynomialDegree() const ITK_OVERRIDE
  {
    itkExceptionMacro("GetRadiometricCalibrationIncidenceAnglePolynomialDegree() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  IndexType GetRadiometricCalibrationRangeSpreadLossPolynomialDegree() const ITK_OVERRIDE
  {
    itkExceptionMacro("GetRadiometricCalibrationRangeSpreadLossPolynomialDegree() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  IndexType GetRadiometricCalibrationNoisePolynomialDegree() const ITK_OVERRIDE
  {
    itkExceptionMacro("GetRadiometricCalibrationNoisePolynomialDegree() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition day from the ossim metadata */
  int GetDay() const ITK_OVERRIDE
  {
    itkExceptionMacro("GetDay not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition month from the ossim metadata */
  int GetMonth() const ITK_OVERRIDE
  {
    itkExceptionMacro("GetMonth not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition year from the ossim metadata */
  int GetYear() const ITK_OVERRIDE
  {
    itkExceptionMacro("GetYear not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition hour from the ossim metadata */
  int GetHour() const ITK_OVERRIDE
  {
    itkExceptionMacro("GetHour not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition minute from the ossim metadata */
  int GetMinute() const ITK_OVERRIDE
  {
    itkExceptionMacro("GetMinute not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging production day from the ossim metadata */
  int GetProductionDay() const ITK_OVERRIDE
  {
    itkExceptionMacro("GetProductionDay not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging production month from the ossim metadata */
  int GetProductionMonth() const ITK_OVERRIDE
  {
    itkExceptionMacro("GetProductionMonth not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging production year from the ossim metadata */
  int GetProductionYear() const ITK_OVERRIDE
  {
    itkExceptionMacro("GetProductionYear not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the PRF */
  double GetPRF() const ITK_OVERRIDE
  {
    itkExceptionMacro("GetPRF not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the RSF */
  double GetRSF() const ITK_OVERRIDE
  {
    itkExceptionMacro("GetRSF not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the radar frequency */
  double GetRadarFrequency() const ITK_OVERRIDE
  {
    itkExceptionMacro("GetRadarFrequency not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the center incidence angle */
  double GetCenterIncidenceAngle() const ITK_OVERRIDE
  {
    itkExceptionMacro("GetCenterIncidenceAngle not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  bool CanRead() const ITK_OVERRIDE
  {
    // This clas is the default one, it has to be able to call every metadata
    return false;
  }

  /** Get the 3 spectral band numbers corresponding to the default display for visualization,
   *  in the order R, G, B */
    UIntVectorType GetDefaultDisplay() const ITK_OVERRIDE
  {
    UIntVectorType rgb(3);
    rgb[0] = 0;
    rgb[1] = 0;
    rgb[2] = 0;
    return rgb;
  }

protected:
  SarDefaultImageMetadataInterface(){};
  ~SarDefaultImageMetadataInterface() ITK_OVERRIDE {}

private:

  SarDefaultImageMetadataInterface(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
