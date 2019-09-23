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

#ifndef otbImageFittingPolygonListFilter_h
#define otbImageFittingPolygonListFilter_h

#include "otbPathListToPathListFilter.h"
#include "otbMacro.h"
#include "itkLineConstIterator.h"

namespace otb
{
/** \class ImageFittingPolygonListFilter
 *  \brief Slightly deform polygon to reach higher enery from the image
 *
 * <b>Limitations:</b> This filter is currently working with integer position
 * for the polygon vertices. It should be optimized for continuous positions.
 *
 *
 * \ingroup OTBPath
 */
template <class TPath, class TImage>
class ITK_EXPORT ImageFittingPolygonListFilter : public PathListToPathListFilter<TPath>
{
public:
  /** Standard typedefs */
  typedef ImageFittingPolygonListFilter   Self;
  typedef PathListToPathListFilter<TPath> Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ImageFittingPolygonListFilter, PathListToPathListFilter);

  /** Template parameters typedefs */
  typedef typename Superclass::PathType          PathType;
  typedef typename Superclass::PathListType      PathListType;
  typedef typename Superclass::PathPointerType   PathPointerType;
  typedef typename PathListType::Pointer         PathListPointerType;
  typedef typename PathListType::ConstIterator   IteratorType;
  typedef typename PathType::VertexType          VertexType;
  typedef typename PathType::VertexListType      VertexListType;
  typedef typename VertexListType::ConstIterator VertexListConstIteratorType;
  typedef double                                 RealType;

  typedef TImage                           ImageType;
  typedef typename ImageType::Pointer      ImagePointerType;
  typedef typename ImageType::ConstPointer ImageConstPointerType;

  typedef itk::LineConstIterator<ImageType> LineConstIteratorType;

  /**
   * Set the input Likelihood image.
   * \param image The Likelihood image.
   */
  void SetInputImage(const ImageType* image);

  /**
   * Get the input Likelihood image.
   * \return The input Likelihood image.
   */
  const ImageType* GetInputImage(void);

  /** Set/Get the search radius. */
  itkSetMacro(Radius, unsigned int);
  itkGetMacro(Radius, unsigned int);

  /** Set/Get the number of iteration. */
  itkSetMacro(NumberOfIterations, unsigned int);
  itkGetMacro(NumberOfIterations, unsigned int);

protected:
  /** Constructor */
  ImageFittingPolygonListFilter();
  /** Destructor */
  ~ImageFittingPolygonListFilter() override
  {
  }
  /** GenerateData method */
  void GenerateData() override;
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  virtual double computeValue(ImageConstPointerType image, VertexType middlePoint, VertexType previousPoint, VertexType nextPoint) const;

  unsigned int m_Radius;
  unsigned int m_NumberOfIterations;

private:
  ImageFittingPolygonListFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageFittingPolygonListFilter.hxx"
#endif

#endif
