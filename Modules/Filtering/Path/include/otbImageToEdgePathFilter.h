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

#ifndef otbImageToEdgePathFilter_h
#define otbImageToEdgePathFilter_h

#include "otbImageToPathFilter.h"

namespace otb
{
/**
 * \class ImageToEdgePathFilter
 * \brief This filter compute the closed edge path of  a labelled object
 *  in an image (for example coming from a segmentation filter). Its output
 * is a Path.
 *
 * The input image is first surrounded by a 1 pixel wide border-line using the
 * ConstantPadImageFilter in order to deal with stuck-to-the-border
 * objects. The padding constant is chosen as regard to the ForegroundValue (label of
 * the object) to avoid confusion on foreground and background values.
 *
 * The edge computation is divided into two steps.
 *
 * First, a linear search is performed to detect a first pixel belonging to the edge of
 * the object.
 *
 * From that pixel and until the algorithm comes back to this position, the neighborhood
 * is iteratively and clock-wise searched to detect the next pixel in the edge.
 *
 * In case of linear object (in fact open edge), the algorithm will walk the object twice in
 * opposite directions, thus producing an edge with a null surface and twice the length of the
 * object as perimeter.
 *
 * This leads to consistent result for geometric descriptors (for instance compacity).
 * \sa ImageToPathFilter
 *
 * \ingroup OTBPath
 */

template <class TInputImage, class TOutputPath>
class ITK_EXPORT ImageToEdgePathFilter
  : public ImageToPathFilter<TInputImage, TOutputPath>
{
public:
  /** standards typedefs */
  typedef ImageToEdgePathFilter                       Self;
  typedef ImageToPathFilter<TInputImage, TOutputPath> Superclass;
  typedef itk::SmartPointer<Self>                     Pointer;
  typedef itk::SmartPointer<const Self>               ConstPointer;

  /// Creation through the object factory
  itkNewMacro(Self);
  /// Runtime information
  itkTypeMacro(ImageToEdgePathFilter, ImageToPathFilter);

  /// Template parameters typedef
  typedef typename Superclass::InputImageType          InputImageType;
  typedef typename Superclass::InputImagePointerType   InputImagePointerType;
  typedef typename Superclass::OutputPathType          OutputPathType;
  typedef typename Superclass::OutputPathPointerType   OutputPathPointerType;
  typedef typename OutputPathType::ContinuousIndexType ContinuousIndexType;

  typedef typename InputImageType::PixelType PixelType;
  typedef typename InputImageType::SizeType SizeType;

  /** Set and Get foreground value */
  itkSetMacro(ForegroundValue, PixelType);
  itkGetConstMacro(ForegroundValue, PixelType);

protected:
  ImageToEdgePathFilter();
  ~ImageToEdgePathFilter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  void GenerateOutputInformation() override {}  //does nothing
  void GenerateData() override;

private:
  ImageToEdgePathFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  PixelType m_ForegroundValue;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToEdgePathFilter.hxx"
#endif

#endif
