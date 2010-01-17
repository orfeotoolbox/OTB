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


namespace otb
{


ImageKeywordlist
::ImageKeywordlist()
{
}

ImageKeywordlist
::ImageKeywordlist(const Self& p): m_Keywordlist(p.m_Keywordlist) //m_Keywordlist(Self.m_Keywordlist), m_Delimiter(Self.m_Delimiter)
{
}


ImageKeywordlist
::~ImageKeywordlist()
{
}

void
ImageKeywordlist::
operator=(const Self& p)
{
  m_Keywordlist = p.m_Keywordlist;
}


void
ImageKeywordlist::
convertToOSSIMKeywordlist(ossimKeywordlist& kwl) const
{
  kwl.getMap() = m_Keywordlist;
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
  os << indent << " Ossim Keyword list:"<<std::endl;
  os << indent << kwl;
}

std::ostream &
operator<<(std::ostream &os, const ImageKeywordlist &kwl)
{
  kwl.Print(os);
  return os;
}


} //namespace otb
