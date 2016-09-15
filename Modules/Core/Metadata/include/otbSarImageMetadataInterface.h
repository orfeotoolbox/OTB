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
#ifndef otbSarImageMetadataInterface_h
#define otbSarImageMetadataInterface_h

#include <string>
#include <vector>
#include "OTBMetadataExport.h"
#include "otbImageMetadataInterfaceBase.h"
#include "itkPointSet.h"
#include "otbSarCalibrationLookupData.h"
#include "otbStringUtils.h"

namespace otb
{
/** \class SarImageMetadataInterface
 *
 * \brief Class for SAR captor metadata reading.
 *
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT SarImageMetadataInterface : public ImageMetadataInterfaceBase
{
public:

  typedef SarImageMetadataInterface     Self;
  typedef ImageMetadataInterfaceBase    Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(SarImageMetadataInterface, ImageMetadataInterfaceBase);

  typedef Superclass::ImageType                      ImageType;
  typedef ImageType::IndexType                       IndexType;
  typedef Superclass::MetaDataDictionaryType         MetaDataDictionaryType;
  typedef Superclass::VectorType                     VectorType;
  typedef Superclass::VariableLengthVectorType       VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType           ImageKeywordlistType;
  typedef Superclass::UIntVectorType                 UIntVectorType;
  typedef Superclass::StringVectorType               StringVectorType;
  typedef itk::PointSet<double, 2>                   PointSetType;
  typedef PointSetType::Pointer                      PointSetPointer;
  typedef double                                     RealType;
  typedef PointSetType::PointType                    PointType;
  typedef SarCalibrationLookupData                   LookupDataType;
  typedef LookupDataType::Pointer                    LookupDataPointerType;

  virtual void CreateCalibrationLookupData(const short t);

  const LookupDataPointerType GetCalibrationLookupData(const short type);

  bool HasCalibrationLookupDataFlag() const;

  void SetCalibrationLookupData(LookupDataType* lut)
  {
    m_SarLut = lut;
  }

  virtual RealType   GetRadiometricCalibrationScale() const;

  virtual PointSetPointer GetRadiometricCalibrationAntennaPatternNewGain() const;
  virtual PointSetPointer GetRadiometricCalibrationAntennaPatternOldGain() const;
  virtual PointSetPointer GetRadiometricCalibrationIncidenceAngle() const;
  virtual PointSetPointer GetRadiometricCalibrationRangeSpreadLoss() const;
  virtual PointSetPointer GetRadiometricCalibrationNoise() const;

  virtual IndexType GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree() const;
  virtual IndexType GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree() const;
  virtual IndexType GetRadiometricCalibrationIncidenceAnglePolynomialDegree() const;
  virtual IndexType GetRadiometricCalibrationRangeSpreadLossPolynomialDegree() const;
  virtual IndexType GetRadiometricCalibrationNoisePolynomialDegree() const;

  virtual double GetPRF() const = 0;
  virtual double GetRSF() const = 0;
  virtual double GetRadarFrequency () const = 0;
  virtual double GetCenterIncidenceAngle() const = 0;



  virtual double GetRescalingFactor() const;

  virtual const std::string GetProductType() const;

  virtual const std::string GetAcquisitionMode() const;

  /** Get the enhanced band names (No enhanced band name support for SAR) */
  StringVectorType GetEnhancedBandNames() const ITK_OVERRIDE
  {
    StringVectorType nothing;
    return nothing;
  }

protected:
  SarImageMetadataInterface();
  ~SarImageMetadataInterface() ITK_OVERRIDE {}

  PointSetPointer GetConstantValuePointSet(const RealType& value) const;
  IndexType GetConstantPolynomialDegree() const;

  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

  LookupDataPointerType m_SarLut;


private:
  SarImageMetadataInterface(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
