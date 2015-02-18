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
#include <iostream>
#include <iomanip>

#include "otbVectorDataKeywordlist.h"

namespace otb
{

VectorDataKeywordlist
::VectorDataKeywordlist()
{
  //Nothing to do here
}

VectorDataKeywordlist
::VectorDataKeywordlist(const Self& other)
{
  *this = other;
}

VectorDataKeywordlist
::~VectorDataKeywordlist()
{
  for (unsigned int i = 0; i < m_FieldList.size(); ++i)
    {
    if (m_FieldList[i].first->GetType() == OFTString)
      {
      VSIFree(m_FieldList[i].second.String);
      }
    delete (m_FieldList[i].first);
    }
}

void
VectorDataKeywordlist
::AddField(OGRFieldDefn* fieldDefn, OGRField* field)
{
  FieldType newField;
  newField.first = fieldDefn;
  newField.second = *field;
  //TODO: evaluate performance impact of fieldDefn copy
  // the object itself could be handled at the VectorData level
  // keeping only pointer here. (but it does not seem
  // necessary so far...)
  m_FieldList.push_back(CopyOgrField(newField));
}

void
VectorDataKeywordlist
::AddField(const std::string& key, const std::string& value)
{
  FieldType newField;

  OGRFieldDefn* fieldDefn =  new OGRFieldDefn(key.c_str(), OFTString);

  OGRField field;
  char *   cstr = (char*)VSIMalloc( (value.length() + 1) * sizeof(char) );
  strcpy(cstr, value.c_str());
  field.String = cstr;

  newField.first = fieldDefn;
  newField.second = field;
  m_FieldList.push_back(newField);
}

std::string
VectorDataKeywordlist
::GetFieldAsString(const std::string& key) const
{
  for (unsigned int i = 0; i < m_FieldList.size(); ++i)
    {
    if (key.compare(m_FieldList[i].first->GetNameRef()) == 0)
      {
      if (m_FieldList[i].first->GetType() == OFTString)
        {
        return m_FieldList[i].second.String;
        }
      if (m_FieldList[i].first->GetType() == OFTInteger)
        {
        std::ostringstream ss;
        ss << std::setprecision(15) << m_FieldList[i].second.Integer;
        return ss.str();
        }
      if (m_FieldList[i].first->GetType() == OFTReal)
        {
        std::ostringstream ss;
        ss << std::setprecision(15) << m_FieldList[i].second.Real;
        return ss.str();
        }
      itkExceptionMacro(
        << "This type (" << m_FieldList[i].first->GetType() <<
        ") is not handled (yet) by GetFieldAsString(), please request for it");
      }
    }
  return "";
}

double
VectorDataKeywordlist
::GetFieldAsDouble(const std::string& key) const
{
  for (unsigned int i = 0; i < m_FieldList.size(); ++i)
      {
      if (key.compare(m_FieldList[i].first->GetNameRef()) == 0)
        {
        if (m_FieldList[i].first->GetType() == OFTInteger)
          {
          return (double)(m_FieldList[i].second.Integer);
          }
        if (m_FieldList[i].first->GetType() == OFTReal)
          {
          return (double)(m_FieldList[i].second.Real);
          }
        if (m_FieldList[i].first->GetType() == OFTString)
          {
          std::istringstream is(m_FieldList[i].second.String);
          double value;
          is >> value;
          return value;
          }
        itkExceptionMacro(
          << "This type (" << m_FieldList[i].first->GetType() <<
          ") is not handled (yet) by GetFieldAsDouble(), please request for it");
        }
      }
    return 0.;
}

int
VectorDataKeywordlist
::GetFieldAsInt(const std::string& key) const
{
  for (unsigned int i = 0; i < m_FieldList.size(); ++i)
      {
      if (key.compare(m_FieldList[i].first->GetNameRef()) == 0)
        {
        if (m_FieldList[i].first->GetType() == OFTInteger)
          {
          return (int)(m_FieldList[i].second.Integer);
          }
        if (m_FieldList[i].first->GetType() == OFTReal)
          {
          return (int)(m_FieldList[i].second.Real);
          }
        if (m_FieldList[i].first->GetType() == OFTString)
          {
          std::istringstream is(m_FieldList[i].second.String);
          int value;
          is >> value;
          return value;
          }
        itkExceptionMacro(
          << "This type (" << m_FieldList[i].first->GetType() <<
          ") is not handled (yet) by GetFieldAsInt(), please request for it");
        }
      }
    return 0.;
}

void
VectorDataKeywordlist
::SetFieldAsDouble(const std::string& key, double value)
{
  if (HasField(key))
      {
      for (unsigned int i = 0; i < m_FieldList.size(); ++i)
        {
        if (key.compare(m_FieldList[i].first->GetNameRef()) == 0)
          {
          if (m_FieldList[i].first->GetType() == OFTReal)
            {
            OGRField field;
            field.Real = value;
            m_FieldList[i].second = field;
            }
          else
            {
            itkExceptionMacro(<< "This type is not of double type, can't add the element in it");
            }
          }
        }
      }
    else
      {
      FieldType newField;

      OGRFieldDefn* fieldDefn =  new OGRFieldDefn(key.c_str(), OFTReal);

      OGRField field;
      field.Real = value;
      newField.first = fieldDefn;
      newField.second = field;
      m_FieldList.push_back(newField);
      }
}

void
VectorDataKeywordlist
::SetFieldAsInt(const std::string& key, int value)
{
  if (HasField(key))
    {
    for (unsigned int i = 0; i < m_FieldList.size(); ++i)
      {
      if (key.compare(m_FieldList[i].first->GetNameRef()) == 0)
        {
        if (m_FieldList[i].first->GetType() == OFTInteger)
          {
          OGRField field;
          field.Integer = value;
          m_FieldList[i].second = field;
          }
        else
          if (m_FieldList[i].first->GetType() == OFTReal)
            {
            OGRField field;
            field.Real = static_cast<double>(value);
            m_FieldList[i].second = field;
            }
          else
            {
            itkExceptionMacro(<< "This type is not of integer type, can't add the element in it");
            }
        }
      }
    }
  else
    {
    FieldType newField;

    OGRFieldDefn* fieldDefn =  new OGRFieldDefn(key.c_str(), OFTInteger);
    OGRField field;
    field.Integer = value;
    newField.first = fieldDefn;
    newField.second = field;
    m_FieldList.push_back(newField);
    }
}

bool
VectorDataKeywordlist
::HasField(const std::string& key) const
{
  for (unsigned int i = 0; i < m_FieldList.size(); ++i)
    {
    if (key.compare(m_FieldList[i].first->GetNameRef()) == 0)
      {
      return true;
      }
    }
  return false;
}

void
VectorDataKeywordlist
::SetFieldAsString(const std::string& key, const std::string& value)
{
  if (HasField(key))
    {
    for (unsigned int i = 0; i < m_FieldList.size(); ++i)
      {
      if (key.compare(m_FieldList[i].first->GetNameRef()) == 0)
        {
        if (m_FieldList[i].first->GetType() == OFTString)
          {
          OGRField field;
          char *   cstr = (char*)VSIMalloc( (value.length() + 1) * sizeof(char) );
          strcpy(cstr, value.c_str());
          field.String = cstr;
          m_FieldList[i].second = field;
          }
        else
          {
          itkExceptionMacro(<< "This type is not of string type, can't add the element in it");
          }
        }
      }
    }
  else
    {
    AddField(key, value);
    }
}

VectorDataKeywordlist::FieldType
VectorDataKeywordlist
::GetNthField(unsigned int index) const
{
  if (index > m_FieldList.size())
    {
    itkExceptionMacro(<< " Accessing out-of-range metadata ");
    }
  return m_FieldList[index];
}

unsigned int
VectorDataKeywordlist
::GetNumberOfFields() const
{
  return m_FieldList.size();
}

std::vector<std::string>
VectorDataKeywordlist
::GetFieldList() const
{
  std::vector<std::string> res;
  for (unsigned int i = 0; i < m_FieldList.size(); ++i)
    {
    res.push_back((m_FieldList[i].first)->GetNameRef());
    }
  return res;
}

void
VectorDataKeywordlist
::operator =(const Self& p)
{
  for (unsigned int i = 0; i < p.m_FieldList.size(); ++i)
    {
    m_FieldList.push_back(CopyOgrField(p.m_FieldList[i]));
    }
}

void
VectorDataKeywordlist
::Print(std::ostream& os, itk::Indent indent) const
{
  this->PrintSelf(os, indent.GetNextIndent());
}

void
VectorDataKeywordlist
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << indent << " VectorData Keyword list: ";
  os << indent << " - Size: " << m_FieldList.size() << std::endl;
  for (unsigned int i = 0; i < m_FieldList.size(); ++i)
    {
    os << indent << "    " << PrintField(m_FieldList[i]);
    }
}

void
VectorDataKeywordlist::CopyFieldList(const Self& kwl)
{
  for (unsigned int idx = 0; idx < kwl.GetNumberOfFields(); ++idx)
    {
    switch(kwl.GetNthField(idx).first->GetType())
      {
      case OFTString :
      {
      this->SetFieldAsString(kwl.GetNthField(idx).first->GetNameRef(),
                             kwl.GetNthField(idx).second.String);
      break;
      }
      case OFTInteger :
      {
      this->SetFieldAsInt(kwl.GetNthField(idx).first->GetNameRef(),
                          kwl.GetNthField(idx).second.Integer);
      break;
      }
      case OFTReal :
      {
      this->SetFieldAsDouble(kwl.GetNthField(idx).first->GetNameRef(),
                             kwl.GetNthField(idx).second.Real);
      break;
      }
      case OFTIntegerList:
      {
      std::cerr << "Type not handled for Integer conversion" <<std::endl;
      break;
      }
      case OFTRealList:
      {
      std::cerr << "Type not handled for Integer conversion"<<std::endl;
      break;
      }
      case OFTStringList:
      {
      std::cerr << "Type not handled for Integer conversion"<<std::endl;
      break;
      }
      case OFTWideString:
      {
      std::cerr << "Type not handled for Integer conversion"<<std::endl;
      break;
      }
      case OFTWideStringList:
      {
      std::cerr << "Type not handled for Integer conversion"<<std::endl;
      break;
      }
      case OFTBinary:
      {
      std::cerr << "Type not handled for Integer conversion"<<std::endl;
      break;
      }
      case OFTDate:
      {
      std::cerr << "Type not handled for Integer conversion"<<std::endl;
      break;
      }
      case OFTTime:
      {
      std::cerr << "Type not handled for Integer conversion"<<std::endl;
      break;
      }
      case OFTDateTime:
      {
      std::cerr << "Type not handled for Integer conversion"<<std::endl;
      break;
      }
      }
    }
}

std::string
VectorDataKeywordlist
::PrintField(FieldType field) const
{
  std::stringstream output;
  output << std::setprecision(15);
  output << field.first->GetNameRef() << " (";
  output << field.first->GetFieldTypeName(field.first->GetType()) << "): ";
  switch (field.first->GetType())
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
      std::cerr << "Type not handled for printing" << std::endl;
      break;
      }
    case OFTString:
      {
      if (field.second.String != NULL)
        {
        output << field.second.String;
        }
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

VectorDataKeywordlist::FieldType
VectorDataKeywordlist
::CopyOgrField(FieldType field)
{
  FieldType outField;
  outField.first = new OGRFieldDefn(field.first);
  switch (field.first->GetType())
    {
    case OFTInteger:
      {
      outField.second.Integer = field.second.Integer;
      break;
      }
    case OFTIntegerList:
      {
      std::cerr << "OGR type not handled" << std::endl;
      break;
      }
    case OFTReal:
      {
      outField.second.Real = field.second.Real;
      break;
      }
    case OFTRealList:
      {
      std::cerr << "OGR type not handled" << std::endl;
      break;
      }
    case OFTString:
      {
      if (field.second.String != NULL)
        {
        CPLFree(outField.second.String);
        outField.second.String = CPLStrdup(field.second.String);
        }
      break;
      }
    case OFTStringList:
      {
      std::cerr << "OGR type not handled" << std::endl;
      break;
      }
    case OFTWideString:
      {
      std::cerr << "OGR type not handled" << std::endl;
      break;
      }
    case OFTWideStringList:
      {
      std::cerr << "OGR type not handled" << std::endl;
      break;
      }
    case OFTBinary:
      {
      std::cerr << "OGR type not handled" << std::endl;
      break;
      }
    case OFTDate:
      {
      outField.second.Date.Year = field.second.Date.Year;
      outField.second.Date.Month = field.second.Date.Month;
      outField.second.Date.Day = field.second.Date.Day;
      break;
      }
    case OFTTime:
      {
      outField.second.Date.Hour = field.second.Date.Hour;
      outField.second.Date.Minute = field.second.Date.Minute;
      outField.second.Date.Second = field.second.Date.Second;
      break;
      }
    case OFTDateTime:
      {
      outField.second.Date.Year = field.second.Date.Year;
      outField.second.Date.Month = field.second.Date.Month;
      outField.second.Date.Day = field.second.Date.Day;
      outField.second.Date.Hour = field.second.Date.Hour;
      outField.second.Date.Minute = field.second.Date.Minute;
      outField.second.Date.Second = field.second.Date.Second;
      break;
      }
    }
  return outField;
}

std::ostream &
operator <<(std::ostream& os, const VectorDataKeywordlist& kwl)
{
  kwl.Print(os);
  return os;
}

}
