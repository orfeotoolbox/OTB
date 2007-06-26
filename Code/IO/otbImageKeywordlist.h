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

#include "itkObject.h"
#include "itkObjectFactory.h"
//#include "itkSize.h"
//#include "itkImageRegion.h"
//#include "itkDefaultConvertPixelTraits.h"

#include "base/ossimString.h"
#include "base/ossimKeywordlist.h"

#include <map>

namespace otb
{

/** \class ImageKeywordlist
 * \brief Stockage/Conversion des MetaDonnees OSSIM
 *
 * \sa ImageSeriesReader
 * \sa ImageIOBase
 * 
 * \ingroup Projections
 *
 */
class ITKCommon_EXPORT ImageKeywordlist
{
public:
  /** Standard class typedefs. */
  typedef ImageKeywordlist         Self;
  
  typedef std::map<ossimString, ossimString> KeywordlistMap ;

  void SetKeywordlist(const ossimKeywordlist& kwl)
  { 
  	m_Keywordlist = kwl.getMap();
  }
  
  virtual void convertToOSSIMKeywordlist(ossimKeywordlist& kwl) const;

  ImageKeywordlist() ;
  virtual ~ImageKeywordlist() ;

  void Print(std::ostream& os) const ;
  
  ImageKeywordlist(const Self&);
  void operator=(const Self&);
  
private:

  /** Geo informations are in this map */
  KeywordlistMap m_Keywordlist;
  
//  char m_Delimiter;

//  void operator=(const Self&); //purposely not implemented
  
};


} //namespace otb

#endif // __otbImageKeywordlist_h
