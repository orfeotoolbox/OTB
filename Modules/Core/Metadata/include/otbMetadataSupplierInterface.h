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

#ifndef otbMetadataSupplierInterface_h
#define otbMetadataSupplierInterface_h

#include "OTBMetadataExport.h"
#include "itkExceptionObject.h"
#include <vector>
#include <string>
#include "otbStringUtils.h"
#include "otbMacro.h"
#include "otbStringUtilities.h"

namespace otb
{


class MissingMetadataException : public itk::ExceptionObject
{
public:
  /** Run-time information. */
  itkTypeMacro(MissingMetadataException, itk::ExceptionObject);

  /** Constructor. */
  MissingMetadataException(const char* file, unsigned int line, const char* message = "Missing metadata", const char* loc = "Unknown")
    : itk::ExceptionObject(file, line, message, loc)
  {
  }

  /** Constructor. */
  MissingMetadataException(const std::string& file, unsigned int line, const char* message = "Missing metadata", const char* loc = "Unknown")
    : itk::ExceptionObject(file, line, message, loc)
  {
  }

  MissingMetadataException(const std::string& file, unsigned int line, const std::string& message = "Missing metadata", const char* loc = "Unknown")
    : itk::ExceptionObject(file, line, message, loc)
  {
  }
};

/** \class MetadataSupplierInterface
 *
 * \brief Base class to access metadata information in files/images
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT MetadataSupplierInterface
{
public:

  virtual std::string GetResourceFile() = 0;

  // Maybe not needed
//  virtual std::vector<std::string> GetResourceFiles() = 0;

  //~ virtual bool HasValue(const char * path) = 0;

  /** Get the metadata value corresponding to a given path (meaning of this path
   * depends on the specific implementation. Returns NULL when path is not found */
  virtual const char * GetMetadataValue(const char * path) const = 0;

  // probably not needed
  //~ virtual std::vector<std::string> GetValuesList(const std::string& path) = 0;

  // utility functions
  template <typename T> T GetAs(const char *path) const
    {
    const char * ret = GetMetadataValue(path);
    if (ret == nullptr)
      {
      otbGenericExceptionMacro(MissingMetadataException,<<"Missing metadata '"<<path<<"'")
      }
    try
      {
      return boost::lexical_cast<T>(ret);
      }
    catch (boost::bad_lexical_cast&)
      {
      otbGenericExceptionMacro(MissingMetadataException,<<"Bad metadata value for '"<<path<<"', got :"<<ret)
      }
    }

  /** Parse a metadata value to a std::vector,
   *  If size>=0, then the final std::vector size is checked and an exception
   *  is raised if it doesn't match the given size.*/
  template < typename T> std::vector<T> GetAsVector(const char *path, const char sep=' ', int size=-1) const
    {
    const char * ret = GetMetadataValue(path);
    if (ret == nullptr)
      {
      otbGenericExceptionMacro(MissingMetadataException,<<"Missing metadata '"<<path<<"'")
      }
    string_view value(ret);
    string_view filt_value = rstrip(lstrip(value,"[ "), "] ");
    std::vector<T> output;
    typedef part_range<splitter_on_delim> range_type;
    const range_type parts = split_on(filt_value, sep);
    for (auto const& part : parts)
      {
      // TODO: check if we can use lexical_cast on a string_view
      std::string strPart = to<std::string>(part, "casting string_view to std::string");
      if (strPart.empty())
        {
        continue;
        }
      try
        {
        output.push_back(boost::lexical_cast<T>(strPart));
        }
      catch (boost::bad_lexical_cast&)
        {
        otbGenericExceptionMacro(MissingMetadataException,<<"Bad metadata vector element in '"<<path<<"', got :"<<part)
        }
      }
    if ((size >= 0) and (output.size() != (size_t)size))
      {
      otbGenericExceptionMacro(MissingMetadataException,<<"Bad number of elements in vector '"<<path<<"', expected "<<size<< ", got "<<output.size())
      }
    return output;
    }

};

// TODO : for complex types ...

} // end namespace otb

#endif

