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

#ifndef otbMeanShiftConnectedComponentSegmentationFilter_h
#define otbMeanShiftConnectedComponentSegmentationFilter_h

#include "itkMacro.h"

#include "otbConnectedComponentMuParserFunctor.h"
#include "itkConnectedComponentFunctorImageFilter.h"
#include "otbMaskMuParserFilter.h"
#include "itkRelabelComponentImageFilter.h"

#include "otbMeanShiftSmoothingImageFilter.h"
#include <string>

namespace otb
{

/** \class MeanShiftConnectedComponentSegmentationFilter
*   \brief [internal] Helper class to perform connected component segmentation on an input image,
*
*
*  The whole chain is described in :
*  http://wiki.orfeo-toolbox.org/index.php/Connected_component_segmentation_module
*
*  This class wraps a processing chain based on meanshift filtering followed by a connected component segmentation,
*
*  An optional mask can be applied to segment only the pixels inside the mask.
*
*  Parameters of the chain are :
*  - MaskExpression : mathematical expression to apply on the input image to make a mask
*  - ConnectedComponentExpression : mathematical expression which connects two pixels
*  - MinimumObjectSize : minimum object size kept after segmentation
*
 *
 * \ingroup OTBMeanShift
*/


template <class TVInputImage, class TMaskImage, class TLabelImage>
class MeanShiftConnectedComponentSegmentationFilter : public itk::ImageToImageFilter<TVInputImage, TLabelImage>
{
public:
  /** Standard Self typedef */

  typedef MeanShiftConnectedComponentSegmentationFilter Self;
  typedef itk::ImageToImageFilter<TVInputImage, TLabelImage> Superclass;

  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef unsigned int ObjectSizeType;

  /** Some convenient typedefs. */
  typedef TVInputImage VectorImageType;
  typedef TMaskImage   MaskImageType;
  typedef TLabelImage  LabelImageType;

  typedef typename VectorImageType::Pointer   VectorImagePointerType;
  typedef typename VectorImageType::PixelType VectorImagePixelType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(MeanShiftConnectedComponentSegmentationFilter, ImageToImageFilter);

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int, TVInputImage::ImageDimension);

  // Mask generation
  typedef Functor::ConnectedComponentMuParserFunctor<VectorImagePixelType> FunctorType;
  typedef itk::ConnectedComponentFunctorImageFilter<VectorImageType, LabelImageType, FunctorType, MaskImageType> ConnectedComponentFilterType;

  // mask typedef
  typedef otb::MaskMuParserFilter<VectorImageType, MaskImageType> MaskMuParserFilterType;

  // Labelization
  typedef itk::RelabelComponentImageFilter<LabelImageType, LabelImageType> RelabelComponentFilterType;

  typedef double KernelType;
  typedef otb::MeanShiftSmoothingImageFilter<VectorImageType, VectorImageType> MeanShiftFilterType;

  typedef typename MeanShiftFilterType::Pointer MeanShiftFilterPointerType;

  // ** // meanshift filter
  /* Set/Get mean shift filter */
  itkSetObjectMacro(MeanShiftFilter, MeanShiftFilterType);
  itkGetObjectMacro(MeanShiftFilter, MeanShiftFilterType);

  /* Set the mathematical expression used for the mask */
  itkSetStringMacro(MaskExpression);

  /* Get the mathematical expression used for the mask */
  itkGetStringMacro(MaskExpression);

  /* Set the mathematical expression used during connected component segmentation */
  itkSetStringMacro(ConnectedComponentExpression);

  /* Get the mathematical expression used during connected component segmentation */
  itkGetStringMacro(ConnectedComponentExpression);

  /* Set the minimum object size */
  itkSetMacro(MinimumObjectSize, ObjectSizeType);

  /* Get the minimum object size */
  itkGetMacro(MinimumObjectSize, ObjectSizeType);

protected:
  MeanShiftConnectedComponentSegmentationFilter();

  ~MeanShiftConnectedComponentSegmentationFilter() override;

  void GenerateInputRequestedRegion() override;
  void GenerateData() override;

private:
  MeanShiftFilterPointerType m_MeanShiftFilter;

  /* CC parameters */
  ObjectSizeType m_MinimumObjectSize;
  std::string    m_MaskExpression;
  std::string    m_ConnectedComponentExpression;
  /* */
};


} // end namespace itk

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMeanShiftConnectedComponentSegmentationFilter.hxx"
#endif

#endif
