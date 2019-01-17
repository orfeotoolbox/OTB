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

#ifndef otbLineSpatialObjectList_h
#define otbLineSpatialObjectList_h


#include "itkDataObject.h"
#include "itkLineSpatialObject.h"

#include <list>

namespace otb
{
/** \class LineSpatialObjectList
 * \brief TODO
 *
 * \ingroup OTBSpatialObjects
 */

class ITK_EXPORT LineSpatialObjectList :        public std::list<itk::LineSpatialObject<2>::Pointer>,
  public itk::DataObject

{
public:
  /** Standard class typedefs. */
  typedef LineSpatialObjectList Self;
  typedef itk::DataObject       Superclass;

  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LineSpatialObjectList, itk::DataObject);

  /** Some convenient typedefs. */
  typedef itk::LineSpatialObject<2> LineType;

protected:
  LineSpatialObjectList() {};
  ~LineSpatialObjectList() override {}

private:
  LineSpatialObjectList(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // end namespace otb

#endif
