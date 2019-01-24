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

#ifndef otbBreakAngularPathListFilter_h
#define otbBreakAngularPathListFilter_h

#include "otbPathListToPathListFilter.h"
#include "otbMacro.h"

namespace otb
{
/** \class BreakAngularPathListFilter
 *  \brief CBase class for breaking angulars the input PathList, returning a PathList.
   *
   * \example FeatureExtraction/ExtractRoadByStepsExample.cxx
   *
 *
 * \ingroup OTBRoadExtraction
 */
template <class TPath>
class ITK_EXPORT BreakAngularPathListFilter
  : public PathListToPathListFilter<TPath>
{
public:
  /** Standard typedefs */
  typedef BreakAngularPathListFilter      Self;
  typedef PathListToPathListFilter<TPath> Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(BreakAngularPathListFilter, PathListToPathListFilter);

  /** Template parameters typedefs */
  typedef typename Superclass::PathType        PathType;
  typedef typename Superclass::PathListType    PathListType;
  typedef typename Superclass::PathPointerType PathPointerType;
  typedef typename PathListType::Pointer       PathListPointerType;

  typedef double MaxAngleType;

  /** Set/Get the max angle */
  itkSetMacro(MaxAngle, MaxAngleType);
  itkGetConstMacro(MaxAngle, MaxAngleType);

protected:
  /** Constructor */
  BreakAngularPathListFilter();
  /** Destructor */
  ~BreakAngularPathListFilter() override {}
  /** GenerateData method */
  void GenerateData() override;
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  BreakAngularPathListFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  /** Calculate break angular for a path */
  void BreakAngularPath(const MaxAngleType maxAngle,
                        const PathPointerType inputPath,
                        PathListPointerType outputPathList);

  /** Max angle value */
  MaxAngleType m_MaxAngle;

};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBreakAngularPathListFilter.hxx"
#endif

#endif
