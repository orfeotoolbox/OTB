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

#ifndef otbPathListToPathListFilter_h
#define otbPathListToPathListFilter_h

#include "otbObjectListToObjectListFilter.h"
#include "otbObjectList.h"

namespace otb
{
/** \class PathListToPathListFilter
 *  \brief Base class for filter taking a PathList as input a returning a PathList.
 *
 * \ingroup OTBPath
 */
template <class TPath>
class ITK_EXPORT PathListToPathListFilter : public ObjectListToObjectListFilter<ObjectList<TPath>, ObjectList<TPath>>
{
public:
  /** Standard typedefs */
  typedef PathListToPathListFilter Self;
  typedef ObjectListToObjectListFilter<ObjectList<TPath>, ObjectList<TPath>> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(PathListToPathListFilter, ObjectListToObjectListFilter);

  /** Template parameters typedefs */
  typedef typename Superclass::InputObjectType   PathType;
  typedef typename Superclass::InputListType     PathListType;
  typedef typename PathType::Pointer             PathPointerType;
  typedef typename PathListType::Pointer         PathListPointerType;
  typedef typename PathListType::ConstPointer    PathListConstPointerType;
  typedef typename Superclass::OutputObjectType  OutputPathType;
  typedef typename PathType::Pointer             OutputPathPointerType;
  typedef typename Superclass::OutputListType    OutputPathListType;
  typedef typename Superclass::OutputListPointer OutputPathListPointerType;

protected:
  /** Constructor */
  PathListToPathListFilter(){};
  /** Destructor */
  ~PathListToPathListFilter() override
  {
  }

private:
  PathListToPathListFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};
} // End namespace otb

#endif
