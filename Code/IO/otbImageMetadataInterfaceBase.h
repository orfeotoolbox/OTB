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
#ifndef __otbImageMetadataInterfaceBase_h
#define __otbImageMetadataInterfaceBase_h

#include <string>

#include "otbImageKeywordlist.h"
#include "itkMetaDataDictionary.h"
#include "otbMetaDataKey.h"
#include "itkImageBase.h"

namespace otb
{

/** \class ImageMetadataInterfaceBase
 *
 * \brief Base class for captor metadata reading.
 *
 */
class ITK_EXPORT ImageMetadataInterfaceBase : public itk::Object
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
  std::string GetProjectionRef() const;

  /** Get the GCP projection coordinates of the image. */
  std::string GetGCPProjection() const;
//  otbMetadataGetMacro(GCPProjection, std::string);

  unsigned int GetGCPCount() const;
//  otbMetadataGetMacro(GCPCount, unsigned int);

  OTB_GCP& GetGCPs(unsigned int GCPnum);
  //otbMetadataGetGCPnumMacro(GCPs, OTB_GCP&, GCPnum, unsigned int);

  std::string GetGCPId(unsigned int GCPnum) const;
//  otbMetadataGetGCPnumMacro(GCPId, std::string, GCPnum, unsigned int);

  std::string GetGCPInfo(unsigned int GCPnum) const;
//  otbMetadataGetGCPnumMacro(GCPInfo, std::string, GCPnum, unsigned int);

  double GetGCPRow(unsigned int GCPnum) const;
//  otbMetadataGetGCPnumMacro(GCPRow, double, GCPnum, unsigned int);

  double GetGCPCol(unsigned int GCPnum) const;
//  otbMetadataGetGCPnumMacro(GCPCol, double, GCPnum, unsigned int);

  double GetGCPX(unsigned int GCPnum) const;
//  otbMetadataGetGCPnumMacro(GCPX, double, GCPnum, unsigned int);

  double GetGCPY(unsigned int GCPnum) const;
//  otbMetadataGetGCPnumMacro(GCPY, double, GCPnum, unsigned int);

  double GetGCPZ(unsigned int GCPnum) const;
//  otbMetadataGetGCPnumMacro(GCPZ, double, GCPnum, unsigned int);

  /** Get the six coefficients of affine geoTtransform. */

  VectorType GetGeoTransform() const;
//  otbMetadataGetMacro(GeoTransform, VectorType);

  /** Get image corners. */

  VectorType GetUpperLeftCorner() const;
//  otbMetadataGetMacro(UpperLeftCorner, VectorType);

  VectorType GetUpperRightCorner() const;
//  otbMetadataGetMacro(UpperRightCorner, VectorType);

  VectorType GetLowerLeftCorner() const;
//  otbMetadataGetMacro(LowerLeftCorner, VectorType);

  VectorType GetLowerRightCorner() const;
//  otbMetadataGetMacro(LowerRightCorner, VectorType);

  /** Get the ImageKeywordlist */
  ImageKeywordlistType GetImageKeywordlist();
  const ImageKeywordlistType GetImageKeywordlist() const;

  /** Get the sensor ID from the ossim metadata */
  std::string GetSensorID() const;
  //otbMetadataGetMacro(SensorID, std::string);

  /** Get the number of bands from the ossim metadata */
  unsigned int GetNumberOfBands() const;
  //otbMetadataGetMacro(NumberOfBands, unsigned int);

  /** Get the band name from the ossim metadata */
  std::vector<std::string> GetBandName() const;
  //otbMetadataGetMacro(BandName, std::vector<std::string>);

  /** Get the x pixel spacing*/
  double GetXPixelSpacing() const;
  //otbMetadataGetMacro(XPixelSpacing, double);

  /** Get the y pixel spacing*/
  double GetYPixelSpacing() const;
  //otbMetadataGetMacro(YPixelSpacing, double);


  /** Get the imaging acquisition day from the ossim metadata */
  virtual int GetDay() const = 0;
//  otbMetadataGetMacro(Day, int);

  /** Get the imaging acquisition month from the ossim metadata */
  virtual int GetMonth() const = 0;
  //otbMetadataGetMacro(Month, int);

  /** Get the imaging acquisition year from the ossim metadata */
  virtual int GetYear() const = 0;
  //otbMetadataGetMacro(Year, int);

  /** Get the imaging acquisition hour from the ossim metadata */
  virtual int GetHour() const = 0;
  //otbMetadataGetMacro(Hour, int);

  /** Get the imaging acquisition minute from the ossim metadata */
  virtual int GetMinute() const = 0;
  //otbMetadataGetMacro(Minute, int);

  /** Get the imaging production day from the ossim metadata */
  virtual int GetProductionDay() const = 0;
  //otbMetadataGetMacro(ProductionDay, int);

  /** Get the imaging production month from the ossim metadata */
  virtual int GetProductionMonth() const = 0;
  //otbMetadataGetMacro(ProductionMonth, int);

  /** Get the imaging production year from the ossim metadata */
  virtual int GetProductionYear() const = 0;
  //otbMetadataGetMacro(ProductionYear, int);

  /** Convert the band names provided by ossim to the official band names  */
  virtual std::vector<std::string> GetEnhancedBandNames () const = 0;

  /** Get the 3 spectral band numbers corresponding to the default display for visualization,
   *  in the order R, G, B */
  virtual std::vector<unsigned int> GetDefaultDisplay() const = 0;

  virtual bool CanRead() const = 0;

  virtual void PrintSelf(std::ostream& os, itk::Indent indent, const MetaDataDictionaryType& dict) const;

protected:
  ImageMetadataInterfaceBase();
  virtual ~ImageMetadataInterfaceBase() {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  MetaDataDictionaryType m_MetaDataDictionary;

private:
  ImageMetadataInterfaceBase(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  OTB_GCP            m_GCP;
};

} // end namespace otb

#endif
