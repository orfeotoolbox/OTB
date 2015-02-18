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


  RealType   GetRadiometricCalibrationScale() const
  {
    itkExceptionMacro("GetRadiometricCalibrationScale() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  PointSetPointer GetRadiometricCalibrationAntennaPatternNewGain() const
  {
    itkExceptionMacro("GetRadiometricCalibrationAntennaPatternNewGain() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  PointSetPointer GetRadiometricCalibrationAntennaPatternOldGain() const
  {
    itkExceptionMacro("GetRadiometricCalibrationAntennaPatternOldGain() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  PointSetPointer GetRadiometricCalibrationIncidenceAngle() const
  {
    itkExceptionMacro("GetRadiometricCalibrationIncidenceAngle() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  PointSetPointer GetRadiometricCalibrationRangeSpreadLoss() const
  {
    itkExceptionMacro("GetRadiometricCalibrationRangeSpreadLoss() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  PointSetPointer GetRadiometricCalibrationNoise() const
  {
    itkExceptionMacro("GetRadiometricCalibrationNoise() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  IndexType GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree() const
  {
    itkExceptionMacro("GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  IndexType GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree() const
  {
    itkExceptionMacro("GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  IndexType GetRadiometricCalibrationIncidenceAnglePolynomialDegree() const
  {
    itkExceptionMacro("GetRadiometricCalibrationIncidenceAnglePolynomialDegree() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  IndexType GetRadiometricCalibrationRangeSpreadLossPolynomialDegree() const
  {
    itkExceptionMacro("GetRadiometricCalibrationRangeSpreadLossPolynomialDegree() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  IndexType GetRadiometricCalibrationNoisePolynomialDegree() const
  {
    itkExceptionMacro("GetRadiometricCalibrationNoisePolynomialDegree() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition day from the ossim metadata */
  int GetDay() const
  {
    itkExceptionMacro("GetDay not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition month from the ossim metadata */
  int GetMonth() const
  {
    itkExceptionMacro("GetMonth not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition year from the ossim metadata */
  int GetYear() const
  {
    itkExceptionMacro("GetYear not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition hour from the ossim metadata */
  int GetHour() const
  {
    itkExceptionMacro("GetHour not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition minute from the ossim metadata */
  int GetMinute() const
  {
    itkExceptionMacro("GetMinute not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging production day from the ossim metadata */
  int GetProductionDay() const
  {
    itkExceptionMacro("GetProductionDay not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging production month from the ossim metadata */
  int GetProductionMonth() const
  {
    itkExceptionMacro("GetProductionMonth not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging production year from the ossim metadata */
  int GetProductionYear() const
  {
    itkExceptionMacro("GetProductionYear not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the PRF */
  double GetPRF() const
  {
    itkExceptionMacro("GetPRF not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the RSF */
  double GetRSF() const
  {
    itkExceptionMacro("GetRSF not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the radar frequency */
  double GetRadarFrequency() const
  {
    itkExceptionMacro("GetRadarFrequency not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the center incidence angle */
  double GetCenterIncidenceAngle() const
  {
    itkExceptionMacro("GetCenterIncidenceAngle not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  bool CanRead() const
  {
    // This clas is the default one, it has to be able to call every metadata
    return false;
  }

  /** Get the 3 spectral band numbers corresponding to the default display for visualization,
   *  in the order R, G, B */
    std::vector<unsigned int> GetDefaultDisplay() const
  {
    std::vector<unsigned int> rgb(3);
    rgb[0] = 0;
    rgb[1] = 0;
    rgb[2] = 0;
    return rgb;
  }

protected:
  SarDefaultImageMetadataInterface(){};
  virtual ~SarDefaultImageMetadataInterface() {}

private:

  SarDefaultImageMetadataInterface(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
