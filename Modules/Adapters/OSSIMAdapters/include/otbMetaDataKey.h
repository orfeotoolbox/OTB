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
#ifndef otbMetaDataKey_h
#define otbMetaDataKey_h

#include <string>
#include <vector>

#include "itkDataObject.h"
#include "itkVariableLengthVector.h"
#include "OTBOSSIMAdaptersExport.h"

namespace otb
{
/** \namespace MetaDataKey
 * Definition of the key of metadata used in the metadata dictionary.
 */
namespace MetaDataKey
{
  extern OTBOSSIMAdapters_EXPORT char const* DriverShortNameKey;
  extern OTBOSSIMAdapters_EXPORT char const* DriverLongNameKey;

  extern OTBOSSIMAdapters_EXPORT char const* ProjectionRefKey;

  extern OTBOSSIMAdapters_EXPORT char const* GCPProjectionKey;
  extern OTBOSSIMAdapters_EXPORT char const* GCPParametersKey;
  extern OTBOSSIMAdapters_EXPORT char const* GCPCountKey;

  extern OTBOSSIMAdapters_EXPORT char const* GeoTransformKey;

  extern OTBOSSIMAdapters_EXPORT char const* MetadataKey;
  extern OTBOSSIMAdapters_EXPORT char const* SubMetadataKey;

  extern OTBOSSIMAdapters_EXPORT char const* UpperLeftCornerKey;
  extern OTBOSSIMAdapters_EXPORT char const* UpperRightCornerKey;
  extern OTBOSSIMAdapters_EXPORT char const* LowerLeftCornerKey;
  extern OTBOSSIMAdapters_EXPORT char const* LowerRightCornerKey;

  extern OTBOSSIMAdapters_EXPORT char const* ColorTableNameKey;
  extern OTBOSSIMAdapters_EXPORT char const* ColorEntryCountKey;
  extern OTBOSSIMAdapters_EXPORT char const* ColorEntryAsRGBKey;

  extern OTBOSSIMAdapters_EXPORT char const* OSSIMKeywordlistKey;
  extern OTBOSSIMAdapters_EXPORT char const* OSSIMKeywordlistDelimiterKey;

  extern OTBOSSIMAdapters_EXPORT char const* VectorDataKeywordlistKey;
  extern OTBOSSIMAdapters_EXPORT char const* VectorDataKeywordlistDelimiterKey;

  extern OTBOSSIMAdapters_EXPORT char const* ResolutionFactor;
  extern OTBOSSIMAdapters_EXPORT char const* SubDatasetIndex;
  extern OTBOSSIMAdapters_EXPORT char const* CacheSizeInBytes;

  extern OTBOSSIMAdapters_EXPORT char const* TileHintX;
  extern OTBOSSIMAdapters_EXPORT char const* TileHintY;

  extern OTBOSSIMAdapters_EXPORT char const * NoDataValueAvailable;
  extern OTBOSSIMAdapters_EXPORT char const * NoDataValue;


  enum  KeyType
    {
    TSTRING,
    TENTIER,
    TDOUBLE,
    TOTB_GCP,
    TVECTOR,
    TOSSIMKEYWORDLIST,
    TVECTORDATAKEYWORDLIST,
    TBOOLVECTOR
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
  typedef std::vector<bool>                 BoolVectorType;
  typedef itk::VariableLengthVector<double> VariableLengthVectorType;
}

/** \class OTB_GCP
 *
 * \brief This OTB_GCP class is used to manage the GCP parameters
 * in OTB.
 *
 *
 * \ingroup OTBOSSIMAdapters
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
