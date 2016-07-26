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
#ifndef otbImageKeywordlist_h
#define otbImageKeywordlist_h

#include "itkMacro.h"
#include "itkIndent.h"
#include "gdal.h"
#include "ossimStringUtilities.h"

#include <iosfwd>
#include <map>
#include <string>

//forward declaration
class ossimKeywordlist;

namespace otb
{

namespace internal
{
  /**
   * Converts index coordinates from OTB into OSSIM. It is intended for sensor
   * images where the centre of the top-left pixel is expected to be :
   *   [0.5,0.5] in OTB physical space
   *   [ 0 , 0 ] in OSSIM
   * \param[in] val  coordinate value to modify (along X or Y)
   */
  inline double ConvertToOSSIMFrame(double val)
    {
    return (val - 0.5);
    }

  /**
   * Converts index coordinates from OSSIM into OTB. It is intended for sensor
   * images where the centre of the top-left pixel is expected to be :
   *   [0.5,0.5] in OTB physical space
   *   [ 0 , 0 ] in OSSIM
    * \param[in] val  coordinate value to modify (along X or Y)
   */
  inline double ConvertFromOSSIMFrame(double val)
    {
    return (val + 0.5);
    }

} // namespace internal

/** \class ImageKeywordlist
 * \brief Storage and conversion for OSSIM metadata
 *
 * \sa ImageSeriesReader
 * \sa ImageIOBase
 *
 * \ingroup Projections
 * \ingroup OTBOSSIMAdapters
 */
class ITK_EXPORT ImageKeywordlist
{
public:
  /** Standard class typedefs. */
  typedef ImageKeywordlist Self;

  //virtual const char *GetNameOfClass() const
  //{return "ImageKeywordlist"; }

  typedef std::map<std::string, std::string> KeywordlistMap;
  typedef KeywordlistMap::size_type KeywordlistMapSizeType;

  ImageKeywordlist() {}

  /** Get the internal map container */
  const KeywordlistMap& GetKeywordlist() const
  {
    return m_Keywordlist;
  }

  void SetKeywordlist(const ossimKeywordlist& kwl);

  void Clear(void)
  {
    m_Keywordlist.clear();
  }

  KeywordlistMapSizeType Empty() const
  {
    return m_Keywordlist.empty();
  }

  KeywordlistMapSizeType GetSize(void) const
  {
    return m_Keywordlist.size();
  }

  /** Get the Data object descriptor corresponding to the given key.
   * \throw itk::Exception if the `key` is not found
   */
  const std::string& GetMetadataByKey(const std::string& key) const;

  /** Get the Data object descriptor corresponding to the given key in the type
   * specified (/w exceptions).
   * \return the value associated to the `key` in the `T` type specified.
   * \throw itk::Exception if the `key` is not found
   * \throw std::runtime_error if the associated value cannot be converted
   */
  template <typename T>
  T GetMetadataByKeyAs(const std::string& key) const
    {
    std::string const& v = GetMetadataByKey(key);
    return ossimplugins::to<T>(v, " extracting " + key + " from KWL");
    }
  /** Get the Data object descriptor corresponding to the given key in the type
   * specified (w/ default value).
   * \return the value associated to the `key` in the `T` type specified.
   * \return `def` if the `key` is not found, or if the associated value cannot
   * be deserialized to a value of type `T`
   */
  template <typename T>
  T GetMetadataByKeyAs(const std::string& key, T const& def) const
    {
    const KeywordlistMap::const_iterator it = m_Keywordlist.find(key);
    return (it == m_Keywordlist.end()) ? def : ossimplugins::to_with_default<T>(it->second, def);
    }

  /** return true if the key is in the dictionary */
  bool HasKey(const std::string& key) const;

  /** Clear a given field of the keyword list */
  void ClearMetadataByKey(const std::string& key);

  void AddKey(const std::string& key, const std::string& value);

  void convertToOSSIMKeywordlist(ossimKeywordlist& kwl) const;

  /** try to convert the image keywordlist into a GDALRpcInfo structure
   *  return true if successful, false otherwise */
  bool convertToGDALRPC(GDALRPCInfo &rpc) const;

  void Print(std::ostream& os, itk::Indent indent = 0) const;

  bool operator ==(const Self&) const;
  inline bool operator != ( const Self & ) const;

private:
  /** Geo information are in this map */
  KeywordlistMap m_Keywordlist;
};

std::ostream & operator <<(std::ostream& os, const ImageKeywordlist& kwl);

// Free function to handle the keywordlist <-> files
ImageKeywordlist ReadGeometryFromImage(const std::string& filename, bool checkRpcTag=true);
ImageKeywordlist ReadGeometryFromGEOMFile(const std::string& filename);
ImageKeywordlist ReadGeometryFromRPCTag(const std::string& filename);
void WriteGeometry(const ImageKeywordlist& otb_kwl, const std::string& filename);

inline
bool
ImageKeywordlist
::operator !=( const Self & p ) const
{
  return !this->operator == ( p );
}

} //namespace otb

#endif // otbImageKeywordlist_h
