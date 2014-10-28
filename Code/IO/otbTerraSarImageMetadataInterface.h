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

#ifndef __otbTerraSarImageMetadataInterface_h
#define __otbTerraSarImageMetadataInterface_h

#include <string>

#include "otbSarImageMetadataInterface.h"
#include "otbMetaDataKey.h"
#include "itkImageBase.h"

namespace otb
{
/** \class TerraSarImageMetadataInterface
 *
 * \brief Creation of an "otb" TerraSarImageMetadataInterface that gets metadata.
 *
 */
class ITK_EXPORT TerraSarImageMetadataInterface : public SarImageMetadataInterface
{
public:

  typedef TerraSarImageMetadataInterface Self;
  typedef SarImageMetadataInterface      Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(TerraSarImageMetadataInterface, otbSarImageMetadataInterface);

  typedef itk::ImageBase<2>                     ImageType;
  typedef ImageType::IndexType                  IndexType;
  typedef std::vector<IndexType>                IndexVectorType;
  typedef itk::MetaDataDictionary               MetaDataDictionaryType;
  typedef MetaDataKey::VectorType               VectorType;
  typedef MetaDataKey::VariableLengthVectorType VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType      ImageKeywordlistType;
  typedef std::vector<double>                   DoubleVectorType;
  typedef std::vector<DoubleVectorType>         DoubleVectorVectorType;
  typedef std::vector<unsigned int>             UIntVectorType;

  typedef Superclass::PointSetType              PointSetType;
  typedef Superclass::PointSetPointer           PointSetPointer;
  typedef double                                RealType;

  /** Get the imaging start acquisition day from the ossim metadata */
  int GetDay() const;

  /** Get the imaging start acquisition month from the ossim metadata */
  int GetMonth() const;

  /** Get the imaging start acquisition year from the ossim metadata */
  int GetYear() const;

  /** Get the imaging start acquisition hour from the ossim metadata */
  int GetHour() const;

  /** Get the imaging start acquisition minute from the ossim metadata */
  int GetMinute() const;

  /** Get the imaging production day from the ossim metadata : generationTime variable */
  int GetProductionDay() const;

  /** Get the imaging production month from the ossim metadata : generationTime variable */
  int GetProductionMonth() const;

  /** Get the imaging production year from the ossim metadata : generationTime variable */
  int GetProductionYear() const;

  /** Get the calibration.calFactor : generationTime variable */
  double GetCalibrationFactor() const;

  /** Get the number of noise records */
  unsigned int GetNumberOfNoiseRecords() const;

  /** Get the polynomial degree list */
  UIntVectorType GetNoisePolynomialDegrees() const;

  /** Get the polynomial coefficient list */
  DoubleVectorVectorType GetNoisePolynomialCoefficientsList() const;

  /** Get timeUTC noise acquisition list in Julian day */
  DoubleVectorType GetNoiseTimeUTCList() const;

  /** Get noise minimum validity range list */
  DoubleVectorType GetNoiseValidityRangeMinList() const;

  /** Get noise maximum validity range list */
  DoubleVectorType GetNoiseValidityRangeMaxList() const;

  /** Get noise reference point list */
  DoubleVectorType GetNoiseReferencePointList() const;

  /** Get the radar frequency */
  double GetRadarFrequency() const;

  /** Get the PRF */
  double GetPRF() const;

  /** Get the RSF */
  double GetRSF() const;

  /** Get the number of corner incidence angles */
  unsigned int GetNumberOfCornerIncidenceAngles() const;

  /** Get the Mean Incidence angles */
  double GetMeanIncidenceAngles() const;

  /** Get the center incidence angle */
  double GetCenterIncidenceAngle() const;

  /** Get the center index */
  IndexType GetCenterIncidenceAngleIndex() const;

  /** Get the corners incidence angles */
  DoubleVectorType GetCornersIncidenceAngles() const;

  /** Get the corners index */
  IndexVectorType GetCornersIncidenceAnglesIndex() const;

  /** Get the constant calibration factor */
  RealType   GetRadiometricCalibrationScale() const;

  PointSetPointer GetRadiometricCalibrationNoise() const;
  IndexType GetRadiometricCalibrationNoisePolynomialDegree() const;

  //PointSetPointer GetRadiometricCalibrationAntennaPatternOldGain() const;
  PointSetPointer GetRadiometricCalibrationIncidenceAngle() const;
  IndexType GetRadiometricCalibrationIncidenceAnglePolynomialDegree() const;

  bool CanRead() const;

  /** Get the 3 spectral band numbers corresponding to the default display for visualization,
   *  in the order R, G, B */
  std::vector<unsigned int> GetDefaultDisplay() const;

protected:
  TerraSarImageMetadataInterface();
  virtual ~TerraSarImageMetadataInterface() {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Evaluate polynom with Horner scheme*/

  inline double Horner(std::vector<double>& coefficients, const double tauMinusTauRef) const;

  double GetStartTimeUTC() const;
  double GetStopTimeUTC() const;
  RealType GetRangeTimeFirstPixel() const;

  RealType GetRangeTimeLastPixel() const;

  /** convert a TimeUTC string to a julian day */
  double ConvertStringTimeUTCToJulianDay(const std::string& value) const;

  /** Get the polynomial degree for a given noise record */
  unsigned int GetNoisePolynomialDegrees(unsigned int noiseRecord) const;

  /** Get the polynomial coefficient for a given noise record */
  DoubleVectorType GetNoisePolynomialCoefficients(unsigned int noiseRecord) const;

  /** Get timeUTC noise acquisition in Julian day for a given noise record*/
  double GetNoiseTimeUTC(unsigned int noiseRecord) const;

  /** Get noise reference point for a given noise record */
  double GetNoiseReferencePoint(unsigned int noiseRecord) const;

private:
  TerraSarImageMetadataInterface(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
