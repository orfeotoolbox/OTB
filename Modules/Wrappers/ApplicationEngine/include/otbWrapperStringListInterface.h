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

#ifndef otbWrapperStringListInterface_h
#define otbWrapperStringListInterface_h


#include <string>
#include <vector>

#include "OTBApplicationEngineExport.h"

#include "otbWrapperTypes.h"


namespace otb
{

namespace Wrapper
{

/** \class StringListInterface
 *  \brief This class represents a list of InputFilename parameter
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT StringListInterface
{
public:
  /** */
  typedef std::vector<std::string> StringVector;

  /** Set file form a list of filenames */
  virtual void SetListFromFileName(const StringVector&) = 0;

  /** Add null element to lists. */
  virtual void AddNullElement();

  /** */
  virtual void InsertNullElement(std::size_t = -1) = 0;

  /** Add a filename from a filename */
  virtual void AddFromFileName(const std::string&) = 0;

  /** */
  virtual void Insert(const std::string& filename, std::size_t = -1) = 0;

  /** Set one specific stored filename. */
  virtual void SetNthFileName(std::size_t, const std::string&) = 0;

  /** */
  virtual std::size_t SetStrings(const StringVector&);

  /** */
  virtual std::size_t GetStrings(StringVector&) const;

  /** Get the stored filename list */
  virtual StringVector GetFileNameList() const = 0;

  /** Get one specific stored filename. */
  virtual std::string GetNthFileName(std::size_t i) const = 0;

  /** Erase one element of the list. */
  virtual void Erase(std::size_t id);

  /** */
  virtual void Erase(std::size_t start, std::size_t count) = 0;

  /** Retrieve number of elements */
  virtual std::size_t Size() const = 0;

  /** */
  virtual bool IsActive(std::size_t) const = 0;

  /** */
  virtual const std::string& GetToolTip(std::size_t) const = 0;

  /** */
  virtual void Swap(std::size_t, std::size_t) = 0;

  /** */
  virtual Role GetDirection(std::size_t) const;

  /** */
  virtual Role GetDirection() const = 0;

  /** */
  virtual const std::string& GetFilenameFilter(std::size_t) const;

  /** */
  virtual const std::string& GetFilenameFilter() const;

  /** */
  virtual bool IsFilename() const;

protected:
  /** Constructor */
  StringListInterface(){};

private:
};

} // End namespace Wrapper

} // End namespace otb


#endif // otbWrapperStringListInterface_h
