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

#ifndef otbLikelihoodPathListFilter_h
#define otbLikelihoodPathListFilter_h

#include "otbPathListToPathListFilter.h"
#include "otbMacro.h"
#include <string>

namespace otb
{
/** \class LikelihoodPathListFilter
 *  \brief Affect a value to each path of the list based on the image
 *
 * PathList and image are given as an input, the filter affect a value to each
 * path: this value is equal to the sum of the pixels crossed by the path
 * (using a PolyLineImageConstIterator) divided by the number of points in the
 * path
 *
 *
 * \ingroup OTBRoadExtraction
 */
template <class TPath, class TImage>
class ITK_EXPORT LikelihoodPathListFilter
  : public PathListToPathListFilter<TPath>
{
public:
  /** Standard typedefs */
  typedef LikelihoodPathListFilter        Self;
  typedef PathListToPathListFilter<TPath> Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(LikelihoodPathListFilter, PathListToPathListFilter);

  /** Template parameters typedefs */
  typedef typename Superclass::PathType          PathType;
  typedef typename Superclass::PathListType      PathListType;
  typedef typename Superclass::PathPointerType   PathPointerType;
  typedef typename PathListType::Pointer         PathListPointerType;
  typedef typename PathListType::ConstIterator   IteratorType;
  typedef typename PathType::VertexType          VertexType;
  typedef typename PathType::VertexListType      VertexListType;
  typedef typename VertexListType::ConstIterator VertexIteratorType;
  typedef double                                 RealType;

  typedef TImage                           ImageType;
  typedef typename ImageType::Pointer      ImagePointerType;
  typedef typename ImageType::ConstPointer ImageConstPointerType;

  /**
   * Set the input Likelihood image.
   * \param image The Likelihood image.
   */
  void SetInputImage(const ImageType * image);

  /**
   * Get the input Likelihood image.
   * \return The input Likelihood image.
   */
  const ImageType * GetInputImage(void);

protected:
  /** Constructor */
  LikelihoodPathListFilter();
  /** Destructor */
  ~LikelihoodPathListFilter() override {}
  /** GenerateData method */
  void GenerateData() override;
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  LikelihoodPathListFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  std::string m_Key;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLikelihoodPathListFilter.hxx"
#endif

#endif
