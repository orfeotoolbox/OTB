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

namespace otb
{
/** \namespace MetaDataKey
 * Definition of the key of metadata used in the metadata dictionary.
 */
namespace MetaDataKey
{
  extern char const* DriverShortNameKey;
  extern char const* DriverLongNameKey;

  extern char const* ProjectionRefKey;

  extern char const* GCPProjectionKey;
  extern char const* GCPParametersKey;
  extern char const* GCPCountKey;

  extern char const* GeoTransformKey;

  extern char const* MetadataKey;
  extern char const* SubMetadataKey;

  extern char const* UpperLeftCornerKey;
  extern char const* UpperRightCornerKey;
  extern char const* LowerLeftCornerKey;
  extern char const* LowerRightCornerKey;

  extern char const* ColorTableNameKey;
  extern char const* ColorEntryCountKey;
  extern char const* ColorEntryAsRGBKey;

  extern char const* OSSIMKeywordlistKey;
  extern char const* OSSIMKeywordlistDelimiterKey;

  extern char const* VectorDataKeywordlistKey;
  extern char const* VectorDataKeywordlistDelimiterKey;

  extern char const* ResolutionFactor;
  extern char const* CacheSizeInBytes;

  extern char const* TileHintX;
  extern char const* TileHintY;


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
}

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
  ~OTB_GCP();

  void Print(std::ostream& os) const;
};

} // end namespace otb

#endif
