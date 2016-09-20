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
#ifndef otbImageMetadataInterfaceBase_h
#define otbImageMetadataInterfaceBase_h

#include <string>

#include "otbMacro.h"
#include "otbImageKeywordlist.h"
#include "itkMetaDataDictionary.h"
#include "otbMetaDataKey.h"
#include "itkImageBase.h"

#include "OTBMetadataExport.h"

namespace otb
{

/** \class ImageMetadataInterfaceBase
 *
 * \brief Base class for captor metadata reading.
 *
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT ImageMetadataInterfaceBase : public itk::Object
{
public:

  typedef ImageMetadataInterfaceBase    Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageMetadataInterfaceBase, itk::Object);

  typedef itk::ImageBase<2>                     ImageType;
  typedef itk::MetaDataDictionary               MetaDataDictionaryType;
  typedef MetaDataKey::VectorType               VectorType;
  typedef MetaDataKey::VariableLengthVectorType VariableLengthVectorType;
  typedef ImageKeywordlist                      ImageKeywordlistType;
  typedef unsigned int                          UnsignedIntType;
  typedef std::vector<std::string>              StringVectorType;
  typedef std::vector<unsigned int>             UIntVectorType;


  /** Set the image used to get the metadata */
  void SetImage (ImageType* image)
  {
    this->SetMetaDataDictionary(image->GetMetaDataDictionary());
  }

  /** Set the MetadataDictionary  */
  void SetMetaDataDictionary(const MetaDataDictionaryType& dict)
  {
    m_MetaDataDictionary = dict;
  }

  /** Get the MetadataDictionary  */
  const MetaDataDictionaryType&  GetMetaDataDictionary() const
  {
    return m_MetaDataDictionary;
  }

  /** Get the projection coordinate system of the image. */
  std::string const& GetProjectionRef() const;

  /** Get the GCP projection coordinates of the image. */
  std::string const& GetGCPProjection() const;

  UnsignedIntType GetGCPCount() const;

  OTB_GCP& GetGCPs(unsigned int GCPnum);

  std::string const& GetGCPId(unsigned int GCPnum) const;

  std::string const& GetGCPInfo(unsigned int GCPnum) const;

  double GetGCPRow(unsigned int GCPnum) const;

  double GetGCPCol(unsigned int GCPnum) const;

  double GetGCPX(unsigned int GCPnum) const;

  double GetGCPY(unsigned int GCPnum) const;

  double GetGCPZ(unsigned int GCPnum) const;

  /**
   * Get The no data flags if existing
   * return False otherwise
   */
  bool GetNoDataFlags(std::vector<bool> & flags, std::vector<double> & values) const;

  /** Get the six coefficients of affine geoTtransform. */
  VectorType GetGeoTransform() const;

  /** Get image corners. */
  VectorType GetUpperLeftCorner() const;
  VectorType GetUpperRightCorner() const;
  VectorType GetLowerLeftCorner() const;
  VectorType GetLowerRightCorner() const;

  /** Get the ImageKeywordlist */
  const ImageKeywordlistType GetImageKeywordlist() const;

  /** This method is deprecated. Please use bool GetSensorID(std::string& ) **/
  /** OTB_DEPRECATED( ) **/
  std::string const GetSensorID() const;
  
  /** Get the sensor ID from the ossim metadata */
  bool GetSensorID(std::string & sensorId) const;
  
  /** Get the number of bands from the ossim metadata */
  UnsignedIntType GetNumberOfBands() const;

  /** Get the band name from the ossim metadata */
  StringVectorType GetBandName() const;

  /** Get the x pixel spacing*/
  double GetXPixelSpacing() const;

  /** Get the y pixel spacing*/
  double GetYPixelSpacing() const;

  /** Get the imaging acquisition day from the ossim metadata */
  virtual int GetDay() const = 0;

  /** Get the imaging acquisition month from the ossim metadata */
  virtual int GetMonth() const = 0;

  /** Get the imaging acquisition year from the ossim metadata */
  virtual int GetYear() const = 0;

  /** Get the imaging acquisition hour from the ossim metadata */
  virtual int GetHour() const = 0;

  /** Get the imaging acquisition minute from the ossim metadata */
  virtual int GetMinute() const = 0;

  /** Get the imaging production day from the ossim metadata */
  virtual int GetProductionDay() const = 0;

  /** Get the imaging production month from the ossim metadata */
  virtual int GetProductionMonth() const = 0;

  /** Get the imaging production year from the ossim metadata */
  virtual int GetProductionYear() const = 0;

  /** Convert the band names provided by ossim to the official band names  */
  virtual StringVectorType GetEnhancedBandNames () const = 0;

  /** Get the 3 spectral band numbers corresponding to the default display for visualization,
   *  in the order R, G, B */
  virtual UIntVectorType GetDefaultDisplay() const = 0;

  virtual bool CanRead() const = 0;

  bool IsEmpty(std::string          const& s)   const;
  bool IsEmpty(ImageKeywordlistType const& kwl) const;

  static void PrintMetadata(std::ostream& os, itk::Indent indent, const MetaDataDictionaryType& dict);

protected:
  std::string const& GetMetadataByKey(std::string const& key, std::string const& context) const;
  template <typename T> T GetMetadataByKeyAs(std::string const& key, std::string const& context, T const& def) const
    {
    std::string const& sValue = GetMetadataByKey(key, context);
    if (IsEmpty(sValue))
      {
      return def;
      }
    else
      {
      return ossimplugins::to_with_default<T>(sValue, def);
      }
    }

  ImageMetadataInterfaceBase();
  ~ImageMetadataInterfaceBase() ITK_OVERRIDE {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

  MetaDataDictionaryType m_MetaDataDictionary;

private:
  ImageMetadataInterfaceBase(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  OTB_GCP            m_GCP;
};

} // end namespace otb

#endif
