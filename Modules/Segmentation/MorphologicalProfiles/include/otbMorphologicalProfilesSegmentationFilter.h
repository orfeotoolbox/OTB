/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbMorphologicalProfilesSegmentationFilter_h
#define otbMorphologicalProfilesSegmentationFilter_h

#include "otbMorphologicalOpeningProfileFilter.h"
#include "otbMorphologicalClosingProfileFilter.h"
#include "otbProfileToProfileDerivativeFilter.h"
#include "otbProfileDerivativeToMultiScaleCharacteristicsFilter.h"
#include "otbMultiScaleConvexOrConcaveClassificationFilter.h"
#include "otbImage.h"
#include "itkScalarConnectedComponentImageFilter.h"
#include "itkBinaryBallStructuringElement.h"

namespace otb
{
/** \class MorphologicalProfilesSegmentationFilter
*   \brief This class implements segmentation by morphological profiles
*
*   This filter implements segmentation based on the morphological
*   profiles, based on this work:
*   \par
*   Martino Pesaresi and Jon Alti Benediktsson, Member, IEEE:
*   A new approach for the morphological segmentation of high resolution
*   satellite imagery. IEEE Transactions on geoscience and remote sensing, vol. 39,
*   NO. 2, February 2001, p. 309-320.
*   \par
*
*   This filter is composite and creates the full pipeline to
*   implement the method. the
*   otb::MultiScaleConvexOrConcaveClassificationFilter is wired to an
*   itk::ScalarConnectedComponentImageFilter so as to get a labeled
*   raster output.
*   \sa otb::MorphologicalOpeningProfileFilter
*   \sa otb::MorphologicalClosingProfileFilter
*   \sa otb::ProfileToProfileDerivativeFilter
*   \sa otb::ProfileDerivativeToMultiScaleCharacteristicsFilter
*   \sa otb::MultiScaleConvexOrConcaveClassificationFilter
 *
 * \ingroup OTBMorphologicalProfiles
*/
template <class TInputImage, class TOutputImage, class TInternalPrecision = float,
          class TStructuringElement = itk::BinaryBallStructuringElement<typename TInputImage::PixelType, TInputImage::ImageDimension>>
class ITK_EXPORT MorphologicalProfilesSegmentationFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard Self typedef */
  typedef MorphologicalProfilesSegmentationFilter Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;

  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage                        InputImageType;
  typedef typename InputImageType::PixelType InputPixelType;
  typedef TOutputImage                       OutputImageType;
  typedef otb::Image<TInternalPrecision>     InternalImageType;

  typedef TStructuringElement StructuringElementType;

  typedef otb::MorphologicalOpeningProfileFilter<InputImageType, InternalImageType, StructuringElementType> OpeningProfileFilterType;
  typedef otb::MorphologicalClosingProfileFilter<InputImageType, InternalImageType, StructuringElementType> ClosingProfileFilterType;
  typedef otb::ProfileToProfileDerivativeFilter<InternalImageType, InternalImageType> DerivativeFilterType;
  typedef otb::ProfileDerivativeToMultiScaleCharacteristicsFilter<InternalImageType, InternalImageType, OutputImageType> MultiScaleCharacteristicsFilterType;
  typedef otb::MultiScaleConvexOrConcaveClassificationFilter<InternalImageType, OutputImageType> MultiScaleClassificationFilterType;
  typedef itk::ScalarConnectedComponentImageFilter<OutputImageType, OutputImageType>             ConnectedComponentsFilterType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(MorphologicalProfilesSegmentationFilter, itk::ImageToImageFilter);

  itkSetMacro(ProfileSize, unsigned int);
  itkGetConstReferenceMacro(ProfileSize, unsigned int);
  itkSetMacro(ProfileStart, unsigned int);
  itkGetConstReferenceMacro(ProfileStart, unsigned int);
  itkSetMacro(ProfileStep, unsigned int);
  itkGetConstReferenceMacro(ProfileStep, unsigned int);
  itkSetMacro(Sigma, double);
  itkGetConstReferenceMacro(Sigma, double);

protected:
  MorphologicalProfilesSegmentationFilter();

  ~MorphologicalProfilesSegmentationFilter() override;

  void GenerateData() override;

private:
  typename OpeningProfileFilterType::Pointer            m_OpeningProfile;
  typename ClosingProfileFilterType::Pointer            m_ClosingProfile;
  typename DerivativeFilterType::Pointer                m_OpeningDerivativeProfile;
  typename DerivativeFilterType::Pointer                m_ClosingDerivativeProfile;
  typename MultiScaleCharacteristicsFilterType::Pointer m_OpeningCharacteristicsFilter;
  typename MultiScaleCharacteristicsFilterType::Pointer m_ClosingCharacteristicsFilter;
  typename MultiScaleClassificationFilterType::Pointer  m_ClassificationFilter;
  typename ConnectedComponentsFilterType::Pointer       m_ConnectedComponentsFilter;

  unsigned int m_ProfileSize;
  unsigned int m_ProfileStart;
  unsigned int m_ProfileStep;
  double       m_Sigma;
};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMorphologicalProfilesSegmentationFilter.hxx"
#endif

#endif
