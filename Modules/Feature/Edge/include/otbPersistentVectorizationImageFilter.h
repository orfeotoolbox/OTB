/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbPersistentVectorizationImageFilter_h
#define otbPersistentVectorizationImageFilter_h

#include "otbPersistentImageFilter.h"

#include "otbObjectList.h"
#include "otbImageToEdgePathFilter.h"
#include "itkMinimumMaximumImageFilter.h"

namespace otb
{

/** \class PersistentVectorizationImageFilter
 *  \brief Perform vectorization in a persistent way.
 *
 * This filter allows vectorizing input labeled image while it is streamed
 * through it. The GetPathList() method allows getting the vectorized boundaries,
 * while the GetOutput() method allows getting the input image itself.
 *
 * The Reset() method allows clearing the result path list.
 *
 * \sa PersistentImageFilter
 *
 *
 * \ingroup OTBEdge
 */
template<class TInputImage, class TOutputPath>
class ITK_EXPORT PersistentVectorizationImageFilter :
  public PersistentImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentVectorizationImageFilter              Self;
  typedef PersistentImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentVectorizationImageFilter, PersistentImageFilter);

  /** Image related typedefs. */
  // First Input
  typedef TInputImage                             ImageType;
  typedef typename TInputImage::Pointer           InputImagePointer;
  typedef typename TInputImage::RegionType        RegionType;
  typedef typename TInputImage::SizeType          SizeType;
  typedef typename TInputImage::IndexType         IndexType;
  typedef typename TInputImage::PixelType         PixelType;
  typedef typename TInputImage::InternalPixelType InternalPixelType;

  // Output path type
  typedef TOutputPath                                     PathType;
  typedef otb::ObjectList<PathType>                       PathListType;
  typedef typename PathListType::Pointer                  PathListPointerType;
  typedef typename PathType::Pointer                      PathPointerType;
  typedef itk::MinimumMaximumImageFilter<ImageType>       MinMaxFilterType;
  typedef typename MinMaxFilterType::Pointer              MinMaxFilterPointerType;
  typedef otb::ImageToEdgePathFilter<ImageType, PathType> ImageToEdgePathFilterType;
  typedef typename ImageToEdgePathFilterType::Pointer     ImageToEdgePathFilterPointerType;

  void Reset(void) override;
  void Synthetize(void) override;

  itkGetObjectMacro(PathList, PathListType);

protected:
  PersistentVectorizationImageFilter();
  ~PersistentVectorizationImageFilter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  void GenerateData() override;

private:
  PersistentVectorizationImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  // Min max filter (the mini-pipeline)
  MinMaxFilterPointerType m_MinMaxFilter;

  // Where to store the vectorized objects
  PathListPointerType m_PathList;

}; // end of class
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPersistentVectorizationImageFilter.hxx"
#endif

#endif
