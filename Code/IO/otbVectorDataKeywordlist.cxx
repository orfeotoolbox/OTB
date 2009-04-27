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

#include "otbVectorDataKeywordlist.h"

namespace otb
{


VectorDataKeywordlist
  ::VectorDataKeywordlist()
{
  //Nothing to do here
}

VectorDataKeywordlist
  ::~VectorDataKeywordlist()
{
  for (unsigned int i = 0; i < m_FieldList.size(); ++i)
  {
    delete(m_FieldList[i]);
  }
}

void VectorDataKeywordlist::
    AddField(OGRFieldDefn* field)
{
  OGRFieldDefn* newField = new OGRFieldDefn(field);//this ogr construction does a clone
  m_FieldList.push_back(newField);
};

void
VectorDataKeywordlist::
    operator=(const Self& p)
{
  for (unsigned int i = 0; i < p.m_FieldList.size(); ++i)
  {
    OGRFieldDefn* newField = new OGRFieldDefn(p.m_FieldList[i]);//this ogr construction does a clone
    m_FieldList.push_back(newField);
  }
}

void
VectorDataKeywordlist::
    Print(std::ostream& os, itk::Indent indent) const
{
  this->PrintSelf(os, indent.GetNextIndent());
}

void
VectorDataKeywordlist::
    PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << indent << " VectorData Keyword list:"<<std::endl;
  os << indent << "  - Size: " << m_FieldList.size() << std::endl;
  for (unsigned int i = 0; i < m_FieldList.size(); ++i)
  {
    os << indent << "    " << m_FieldList[i]->GetNameRef ();
    os << " : " << (*(m_FieldList[i]->GetDefaultRef())).Integer;
  }
}

std::ostream &
    operator<<(std::ostream &os, const VectorDataKeywordlist &kwl)
{
  kwl.Print(os);
  return os;
}

}
