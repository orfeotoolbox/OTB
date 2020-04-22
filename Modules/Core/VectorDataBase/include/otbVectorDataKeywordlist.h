/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbVectorDataKeywordlist_h
#define otbVectorDataKeywordlist_h

#include <iosfwd>
#include <vector>
#include <string>

#include "itkLightObject.h"
#include "itkObjectFactory.h"
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include "ogr_feature.h"
#pragma GCC diagnostic pop
#else
#include "ogr_feature.h"
#endif

namespace otb
{
/** \class VectorDataKeywordlist
 * \brief this class handle the metadata of vector data.
 *
 * This class is used internaly to handle the information associated with
 * a vector object. This information is retrieved from the input file (a
 * shapefile for example) and propagated along the pipeline with the object.
 *
 * This is the equivalent of the otbImageKeywordlist class but for OGR information.
 *
 * \todo add the accessor to enable modifying/updating the data.
 *
 *
 * \ingroup OTBVectorDataBase
 */
#include "OTBVectorDataBaseExport.h"

class OTBVectorDataBase_EXPORT VectorDataKeywordlist
{
public:
  /** Smart pointer typedef support. */
  typedef VectorDataKeywordlist Self;

  typedef std::pair<OGRFieldDefn*, OGRField> FieldType;
  typedef std::vector<FieldType> FieldListType;

  virtual const char* GetNameOfClass() const
  {
    return "VectorDataKeywordlist";
  }

  void AddField(OGRFieldDefn* fieldDefn, OGRField* field);

  /**
    * \param key The name of the field.
    * \param value The value of the field.
    */
  void AddField(const std::string& key, const std::string& value);

  /**
    * Returns the value associated with a field name.
    * \param key The name of the field.
    * \return The value of the field. A default value is returned if the key was not found.
    */
  std::string GetFieldAsString(const std::string& key) const;

  /**
    * \return True if the node contains the field named after the given key.
    * \param key The name of the field.
    */
  bool HasField(const std::string& key) const;

  /**
    * \param key The name of the field.
    * \param value The value of the field.
    */
  void SetFieldAsString(const std::string& key, const std::string& value);

  /**
    * Returns the value associated with a field name.
    * \param key The name of the field.
    * \return The value of the field. A default value is returned if the key was not found.
    */
  double GetFieldAsDouble(const std::string& key) const;

  /**
    * \param key The name of the field.
    * \param value The value of the field.
    */
  void SetFieldAsDouble(const std::string& key, double value);

  /**
   * Set the field as an integer
   * \param key The name of the field.
   * \param value The value of the field.
   */
  void SetFieldAsInt(const std::string& key, int value);

  /**
    * Returns the value associated with a field name.
    * \param key The name of the field.
    * \return The value of the field. A default value is returned if the key was not found.
    */
  int GetFieldAsInt(const std::string& key) const;

  /**
    * \return the nth field of the node as a std::pair of (key, value).
    * \param index the index of the field to return.
    */
  FieldType GetNthField(unsigned int index) const;

  /**
    * \return the number of fields in the node.
    */
  unsigned int GetNumberOfFields() const;

  /**
    * \return the name of fields in the node.
    */
  std::vector<std::string> GetFieldList() const;

  /**
    * Copy all the fields from another kwl
    */
  void CopyFieldList(const Self& kwl);

  /**
   * Print the keyword list
   */
  virtual void Print(std::ostream& os, itk::Indent indent = 0) const;

  /** Constructor */
  VectorDataKeywordlist();

  /** Destructor */
  ~VectorDataKeywordlist();

  /** Constructor by copy (deep copy)*/
  VectorDataKeywordlist(const Self& other);

  /** Deep copy operator*/
  void operator=(const Self&);

protected:
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  std::string PrintField(FieldType field) const;
  FieldType CopyOgrField(FieldType field);
  FieldListType m_FieldList;
};
OTBVectorDataBase_EXPORT extern std::ostream& operator<<(std::ostream& os, const VectorDataKeywordlist& kwl);
}

#endif
