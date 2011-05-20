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
#ifndef __otbMetaDataKey_h
#define __otbMetaDataKey_h

#include <string>
#include <vector>

#include "itkDataObject.h"
#include "itkVariableLengthVector.h"

//TODO: move the OTB_GCP definition out of here
#include "gdal.h"

namespace otb
{
/** \class MetaDataKey
 *
 * \brief Creation of an otb::MetaDataKey class that defines the
 * key of metadata used in the metadata dictionary.
 *
 */
class ITK_EXPORT MetaDataKey
{
public:

  typedef MetaDataKey Self;

  MetaDataKey() {};
  virtual ~MetaDataKey() {}

  static const std::string DriverShortNameKey;
  static const std::string DriverLongNameKey;

  static const std::string ProjectionRefKey;

  static const std::string GCPProjectionKey;
  static const std::string GCPParametersKey;
  static const std::string GCPCountKey;

  static const std::string GeoTransformKey;

  static const std::string MetadataKey;
  static const std::string SubMetadataKey;

  static const std::string UpperLeftCornerKey;
  static const std::string UpperRightCornerKey;
  static const std::string LowerLeftCornerKey;
  static const std::string LowerRightCornerKey;

  static const std::string ColorTableNameKey;
  static const std::string ColorEntryCountKey;
  static const std::string ColorEntryAsRGBKey;

  static const std::string OSSIMKeywordlistKey;
  static const std::string OSSIMKeywordlistDelimiterKey;

  static const std::string VectorDataKeywordlistKey;
  static const std::string VectorDataKeywordlistDelimiterKey;

  enum  KeyType
    {
    TSTRING,
    TENTIER,
    TDOUBLE,
    TOTB_GCP,
    TVECTOR,
    TOSSIMKEYWORDLIST,
    TVECTORDATAKEYWORDLIST,
    };
  /*
  typedef struct
  {
    std::string keyname;
    KeyType type;
  } KeyTypeDef; */

  struct KeyTypeDef
  {
    std::string keyname;
    KeyType type;

    KeyTypeDef() {}
    KeyTypeDef(const std::string& _keyname, const KeyType& _type)
    {
      keyname = _keyname;
      type = _type;
    }

  };

  KeyType GetKeyType(const std::string& name);

  typedef std::vector<double>               VectorType;
  typedef itk::VariableLengthVector<double> VariableLengthVectorType;

private:
  MetaDataKey(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

/** \class OTB_GCP
 *
 * \brief This OTB_GCP class is used to manage the GCP parameters
 * in OTB.
 *
 */
class ITK_EXPORT OTB_GCP
{
public:

  /** Unique identifier, often numeric */
  std::string m_Id;

  /** Informational message or "" */
  std::string m_Info;

  /** Pixel (x) location of GCP on raster */
  double m_GCPCol;

  /** Line (y) location of GCP on raster */
  double m_GCPRow;

  /** X position of GCP in georeferenced space */
  double m_GCPX;

  /** Y position of GCP in georeferenced space */
  double m_GCPY;

  /** Elevation of GCP, or zero if not known */
  double m_GCPZ;

  OTB_GCP();
  OTB_GCP(const GDAL_GCP * psGCP);
  ~OTB_GCP();

  OTB_GCP(const OTB_GCP &pGcp);
  
  void operator =(const OTB_GCP& pGcp);

  void Print(std::ostream& os) const;
};

} // end namespace otb

#endif
