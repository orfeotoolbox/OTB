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
#include "otbImageKeywordlist.h"

#include "base/ossimKeywordlist.h"
#include "base/ossimString.h"

namespace otb
{

ImageKeywordlist
::ImageKeywordlist()
{
}

ImageKeywordlist
::ImageKeywordlist(const Self& p) : m_Keywordlist(p.m_Keywordlist)
{
}

ImageKeywordlist
::~ImageKeywordlist()
{
}

void
ImageKeywordlist::
operator =(const Self& p)
{
  m_Keywordlist = p.m_Keywordlist;
}

void
ImageKeywordlist::
SetKeywordlist(const ossimKeywordlist& kwl)
{
  m_Keywordlist.clear();
  for (ossimKeywordlist::KeywordMap::const_iterator it = kwl.getMap().begin();
       it != kwl.getMap().end();
       ++it)
    {
    std::string first(it->first.stringDup());
    std::string second(it->second.stringDup());
    m_Keywordlist[first] = second;
    }
}

const std::string&
ImageKeywordlist::
GetMetadataByKey(const std::string& key) const
{
  // Search for the key in the output map
  KeywordlistMap::const_iterator it = m_Keywordlist.find(key);

  // If the key can not be found, throw an exception
  if (it == m_Keywordlist.end())
    {
    itkExceptionMacro(<< "Keywordlist has no output with key " << key);
    }

  // Then if everything is ok, return the ossinString
  return it->second;
}
void
ImageKeywordlist::
convertToOSSIMKeywordlist(ossimKeywordlist& kwl) const
{
  ossimKeywordlist::KeywordMap ossimMap;
  for(KeywordlistMap::const_iterator it = m_Keywordlist.begin();
      it != m_Keywordlist.end();
      ++it)
    {
    ossimString first(it->first);
    ossimString second(it->second);
    ossimMap[first] = second;
    }
  kwl.getMap() = ossimMap;
}

void
ImageKeywordlist::
Print(std::ostream& os, itk::Indent indent) const
{
  this->PrintSelf(os, indent.GetNextIndent());
}

void
ImageKeywordlist::
PrintSelf(std::ostream& os, itk::Indent indent) const
{
  ossimKeywordlist kwl;
  convertToOSSIMKeywordlist(kwl);
  os << indent << " Ossim Keyword list:" << std::endl;
  os << indent << kwl;
}

std::ostream &
operator <<(std::ostream& os, const ImageKeywordlist& kwl)
{
  kwl.Print(os);
  return os;
}

} //namespace otb
