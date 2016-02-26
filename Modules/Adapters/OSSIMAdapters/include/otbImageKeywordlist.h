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
#ifndef __otbImageKeywordlist_h
#define __otbImageKeywordlist_h

#include <iostream>
#include <map>

#include "gdal.h"

#include "itkObject.h"
#include "itkObjectFactory.h"

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
 *
 *
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
 
  KeywordlistMapSizeType GetSize(void) const
  {
    return m_Keywordlist.size();
  }

  /** Get the Data object descriptor corresponding to the given key */
  const std::string& GetMetadataByKey(const std::string& key) const;

  /** return true if the key is in the dictionary */
  bool HasKey(const std::string& key) const;

  /** Clear a given field of the keyword list */
  virtual void ClearMetadataByKey(const std::string& key);

  virtual void AddKey(const std::string& key, const std::string& value);

  virtual void convertToOSSIMKeywordlist(ossimKeywordlist& kwl) const;
  
  /** try to convert the image keywordlist into a GDALRpcInfo structure
   *  return true if successful, false otherwise */
  virtual bool convertToGDALRPC(GDALRPCInfo &rpc) const;

  virtual void Print(std::ostream& os, itk::Indent indent = 0) const;

  ImageKeywordlist();
  virtual ~ImageKeywordlist();

  ImageKeywordlist(const Self &);
  void operator =(const Self&);

  bool operator ==(const Self&) const;
  inline bool operator != ( const Self & ) const;

protected:
  /** Methods invoked by Print() to print information about the object
   * Typically not called by the user (use Print()
   * instead) but used in the hierarchical print process to combine the
   * output of several classes.  */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  /** Geo information are in this map */
  KeywordlistMap m_Keywordlist;

//  char m_Delimiter;

//  void operator=(const Self&); //purposely not implemented

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

#endif // __otbImageKeywordlist_h
