/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbDataObjectListInterface_h
#define otbDataObjectListInterface_h

#include "itkDataObject.h"
#include <boost/noncopyable.hpp>

namespace otb
{

class DataObjectListInterface : private boost::noncopyable
{
/** \class DataObjectListInterface
 *  \brief This non template class is an interface that wrapp ObjectList
 *  
 *  New method that do not need the template parameter of ObjectList 
 *  should be declare here.
 *
 * \ingroup OTBObjectList
 */
public:
  /**
  Get the nth element of the list as a DataObject *.
  */
  virtual itk::DataObject * GetNthDataObject(unsigned int index) const = 0;

  virtual std::size_t Size(void) const = 0;

protected:
  DataObjectListInterface() = default ;
  virtual ~DataObjectListInterface() = default ;
};

} // end of otb namespace

#endif
