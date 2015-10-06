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
#ifndef __otbSentinel1ImageMetadataInterface_h
#define __otbSentinel1ImageMetadataInterface_h

#include "otbSarImageMetadataInterface.h"
#include "itkMetaDataObject.h"
#include "itkMetaDataDictionary.h"

namespace otb
{
/** \class CosmoSkymedImageMetadataInterface
 *
 * \brief Creation of an "otb" CosmoSkymedImageMetadataInterface that gets metadata.
 *
 *
 * \ingroup OTBMetadata
 */

class ITK_ABI_EXPORT CosmoSkymedImageMetadataInterface : public SarImageMetadataInterface
{
public:

  typedef CosmoSkymedImageMetadataInterface    Self;
  typedef SarImageMetadataInterface         Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);


  /** Run-time type information (and related methods). */
  itkTypeMacro(CosmoSkymedImageMetadataInterface, SarImageMetadataInterface);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType     ImageKeywordlistType;
  typedef Superclass::RealType                  RealType;


  std::string GetSensorID() const;

  /** check sensor ID */
  bool CanRead() const;

  int GetYear() const;
  int GetMonth() const;
  int GetDay() const;
  int GetHour() const;
  int GetMinute() const;

  /** Get the imaging production year from the ossim metadata : DATASET_PRODUCTION_DATE metadata variable */
  int GetProductionYear() const;
  /** Get the imaging production month from the ossim metadata : DATASET_PRODUCTION_DATE metadata variable */
  int GetProductionMonth() const;
  /** Get the imaging production day from the ossim metadata : DATASET_PRODUCTION_DATE metadata variable */
  int GetProductionDay() const;

  UIntVectorType GetDefaultDisplay() const;

  /*SarImageMetadataInterface pure virutals rituals */
  double GetPRF() const;

  double GetRSF() const;

  double GetRadarFrequency() const;

  double GetCenterIncidenceAngle() const;

  std::string GetImageID() const;

  void GetValueFromMetadataDictionary(std::string const key, std::string& result) const;

  template<typename T>
    void GetValueFromMetadataDictionary(std::string const key, T& result) const
  {
    const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
    std::string const k = MetaDataKey::MetadataKeyPrefix + key;
    if ( dict.HasKey(k) )
      {
      std::string v;
      //using Template type T directly is not working. So I need to get it as
      //string and use Utils::LexicalCast.
      itk::ExposeMetaData<std::string>(dict, k, v);
      result = Utils::LexicalCast<T>(v, k);
      }
    else
      {
      itkExceptionMacro( << "No metadata entry with  key: '" << k << "' found in GetMetaDataDictionary");
      }
  }

  /*GDALImageIO must export number of bands to have the correct value */
  unsigned int GetNumberOfBands() const
  {
  return 2;
  }

  PointSetPointer GetRadiometricCalibrationIncidenceAngle() const;

  PointSetPointer GetRadiometricCalibrationRangeSpreadLoss() const;

  double GetRescalingFactor() const;


/* Helper function to parse date and time into a std::vector<std::string>
 * using boost::split() expect date time in yyyy-mm-ddThh:mm:ss.ms
 * the date-time string is to be found in keywordlist with key 'key'
 * fills argument dateFields of type std::vector<std::string> which is mutable!
 * TODO: move this method into base class
 */
  void ParseDateTime(const char* key, std::vector<int>& dateFields) const;

protected:

  /* class ctor */
  CosmoSkymedImageMetadataInterface();

  /* class dtor */
  virtual ~CosmoSkymedImageMetadataInterface() {}

private:

  CosmoSkymedImageMetadataInterface(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  mutable std::vector<int> m_ProductionDateFields;
  mutable std::vector<int> m_AcquisitionDateFields;
};

} // end namespace otb

#endif
