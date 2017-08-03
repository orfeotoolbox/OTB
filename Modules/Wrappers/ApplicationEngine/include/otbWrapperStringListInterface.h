/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
  typedef std::vector< std::string > StringVector;

  /** Set file form a list of filenames */
  virtual bool SetListFromFileName( const StringVector & filenames ) = 0;

  /** Add null element to lists. */
  virtual void AddNullElement() = 0;

  /** Add a filename from a filename */
  virtual bool AddFromFileName( const std::string & filename ) = 0;

  /** Set one specific stored filename. */
  virtual bool SetNthFileName( const unsigned int id, const std::string & filename ) = 0;

  /** Get the stored filename list */
  virtual StringVector GetFileNameList() const = 0;

  /** Get one specific stored filename. */
  virtual const std::string & GetNthFileName( unsigned int i ) const = 0;

  /** Erase one element of the list. */
  virtual void Erase( unsigned int id ) = 0;

  /** Retrieve number of elements */
  virtual unsigned int Size() const = 0;


protected:
  /** Constructor */
  StringListInterface() {};


private:

};

} // End namespace Wrapper

} // End namespace otb


#endif // otbWrapperStringListInterface_h
