/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbImageMetadataInterfaceBase_h
#define otbImageMetadataInterfaceBase_h

#include <string>

#include "otbImageKeywordlist.h"
#include "itkMetaDataDictionary.h"
#include "otbMetaDataKey.h"
#include "itkImageBase.h"
#include "otbImageMetadata.h"
#include "otbMetadataSupplierInterface.h"

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
  void SetImage(ImageType* image);

  /** Set the MetadataDictionary  */
  void SetMetaDataDictionary(const MetaDataDictionaryType& dict);

  /** Get the MetadataDictionary  */
  const MetaDataDictionaryType& GetMetaDataDictionary() const;

  void SetImageMetadata(ImageMetadata imd);

  const ImageMetadata& GetImageMetadata() const;

  /** Get the projection coordinate system of the image. */
  std::string GetProjectionRef() const;

  /** Get the GCP projection coordinates of the image. */
  std::string GetGCPProjection() const;
  //  otbMetadataGetMacro(GCPProjection, std::string);

  UnsignedIntType GetGCPCount() const;
  //  otbMetadataGetMacro(GCPCount, unsigned int);

  OTB_GCP& GetGCPs(unsigned int GCPnum);
  // otbMetadataGetGCPnumMacro(GCPs, OTB_GCP&, GCPnum, unsigned int);

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

  /**
   * Get The no data flags if existing
   * return False otherwise
   */
  bool GetNoDataFlags(std::vector<bool>& flags, std::vector<double>& values) const;

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

  /** This method is less performant and has extra copy.
  Please use bool GetSensorID(std::string& ) **/
  std::string const GetSensorID() const;

  /** Get the sensor ID from the ossim metadata */
  bool GetSensorID(std::string& sensorId) const;

  // otbMetadataGetMacro(SensorID, std::string);

  /** Get the number of bands from the ossim metadata */
  UnsignedIntType GetNumberOfBands() const;
  // otbMetadataGetMacro(NumberOfBands, unsigned int);

  /** Get the band name from the ossim metadata */
  StringVectorType GetBandName() const;
  // otbMetadataGetMacro(BandName, std::vector<std::string>);

  /** Get the x pixel spacing*/
  double GetXPixelSpacing() const;
  // otbMetadataGetMacro(XPixelSpacing, double);

  /** Get the y pixel spacing*/
  double GetYPixelSpacing() const;
  // otbMetadataGetMacro(YPixelSpacing, double);

  /** Get the imaging acquisition day from the ossim metadata */
  virtual int GetDay() const {return -1;}
  //  otbMetadataGetMacro(Day, int);

  /** Get the imaging acquisition month from the ossim metadata */
  virtual int GetMonth() const {return -1;}
  // otbMetadataGetMacro(Month, int);

  /** Get the imaging acquisition year from the ossim metadata */
  virtual int GetYear() const {return -1;}
  // otbMetadataGetMacro(Year, int);

  /** Get the imaging acquisition hour from the ossim metadata */
  virtual int GetHour() const {return -1;}
  // otbMetadataGetMacro(Hour, int);

  /** Get the imaging acquisition minute from the ossim metadata */
  virtual int GetMinute() const {return -1;}
  // otbMetadataGetMacro(Minute, int);

  /** Get the imaging production day from the ossim metadata */
  virtual int GetProductionDay() const {return -1;}
  // otbMetadataGetMacro(ProductionDay, int);

  /** Get the imaging production month from the ossim metadata */
  virtual int GetProductionMonth() const {return -1;}
  // otbMetadataGetMacro(ProductionMonth, int);

  /** Get the imaging production year from the ossim metadata */
  virtual int GetProductionYear() const {return -1;}
  // otbMetadataGetMacro(ProductionYear, int);

  /** Convert the band names provided by ossim to the official band names  */
  virtual StringVectorType GetEnhancedBandNames() const {return StringVectorType();}

  /** Get the 3 spectral band numbers corresponding to the default display for visualization,
   *  in the order R, G, B */
  virtual UIntVectorType GetDefaultDisplay() const {return {0,1,2}; }

  virtual bool CanRead() const {return false;}

  virtual void Parse(const MetadataSupplierInterface *)
    {
    otbGenericExceptionMacro(MissingMetadataException,<<"Metadata parsing not implemented")
    }

  static void PrintMetadata(std::ostream& os, itk::Indent indent, const MetaDataDictionaryType& dict);

  const std::string& Fetch(MDStr key, const MetadataSupplierInterface * mds, const char *path, int band=-1);

  const double& Fetch(MDNum key, const MetadataSupplierInterface * mds, const char *path, int band=-1);

  const MetaData::Time& Fetch(MDTime key, const MetadataSupplierInterface * mds, const char *path, int band=-1);

  const boost::any& FetchRPC(const MetadataSupplierInterface * mds);

  /** Reads into the MetaDataDictionary to find an OSSIM ImageKeywordlist,
   * then translate it into ImageMetadata.
   * Returns true if succeed. */
  virtual bool ConvertImageKeywordlistToImageMetadata();

protected:
  ImageMetadataInterfaceBase();
  ~ImageMetadataInterfaceBase() override
  {
  }

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  MetaDataDictionaryType m_MetaDataDictionary;

  ImageMetadata m_Imd;

private:
  ImageMetadataInterfaceBase(const Self&) = delete;
  void operator=(const Self&) = delete;

  OTB_GCP m_GCP;
};

} // end namespace otb

#endif
