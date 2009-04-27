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
    delete(m_FieldList[i].first);
  }
}

void VectorDataKeywordlist::
    AddField(OGRFieldDefn* fieldDefn, OGRField* field)
{
  FieldType newField;
  newField.first = new OGRFieldDefn(fieldDefn);
  newField.second = *field;
  m_FieldList.push_back(newField);
};

void
VectorDataKeywordlist::
    operator=(const Self& p)
{
  for (unsigned int i = 0; i < p.m_FieldList.size(); ++i)
  {
    FieldType newField;
    newField.first = new OGRFieldDefn(p.m_FieldList[i].first);
    newField.second = p.m_FieldList[i].second;
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
  os << indent << " VectorData Keyword list: ";
  os << indent << " - Size: " << m_FieldList.size() << std::endl;
  for (unsigned int i = 0; i < m_FieldList.size(); ++i)
  {
    os << indent << "    " << PrintField(m_FieldList[i]);
  }
}

std::string
VectorDataKeywordlist::
    PrintField(FieldType field) const
{
  std::stringstream output;
  output << field.first->GetNameRef() << " (";
  output << field.first->GetFieldTypeName(field.first->GetType()) << "): ";
  switch(field.first->GetType())
  {
    case OFTInteger:
    {
      output << field.second.Integer;
      break;
    }
    case OFTIntegerList:
    {
      output << "Type not handled for printing";
      break;
    }
    case OFTReal:
    {
      output << field.second.Real;
      break;
    }
    case OFTRealList:
    {
      output << "Type not handled for printing";
      break;
    }
    case OFTString:
    {
      output << field.second.String;
      break;
    }
    case OFTStringList:
    {
      output << "Type not handled for printing";
      break;
    }
    case OFTWideString:
    {
      output << "Type not handled for printing";
      break;
    }
    case OFTWideStringList:
    {
      output << "Type not handled for printing";
      break;
    }
    case OFTBinary:
    {
      output << "Type not handled for printing";
      break;
    }
    case OFTDate:
    {
      output << field.second.Date.Year << field.second.Date.Month << field.second.Date.Day;
      break;
    }
    case OFTTime:
    {
      output << field.second.Date.Hour << field.second.Date.Minute << field.second.Date.Second;
      break;
    }
    case OFTDateTime:
    {
      output << field.second.Date.Year << field.second.Date.Month << field.second.Date.Day << "-"
          << field.second.Date.Hour << field.second.Date.Minute << field.second.Date.Second;
      break;
    }
  }
  output << std::endl;
  return output.str();
}

std::ostream &
    operator<<(std::ostream &os, const VectorDataKeywordlist &kwl)
{
  kwl.Print(os);
  return os;
}

}
