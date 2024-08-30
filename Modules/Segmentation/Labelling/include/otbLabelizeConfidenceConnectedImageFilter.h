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

#ifndef otbLabelizeConfidenceConnectedImageFilter_h
#define otbLabelizeConfidenceConnectedImageFilter_h

#include "itkConfidenceConnectedImageFilter.h"
#include "otbLabelizeImageFilterBase.h"

namespace otb
{

/** \class LabelizeConfidenceConnectedImageFilter
 *  \brief Labels pixels with similar statistics using connectivity
 *
 *  This class implements abstract class otb::LabelizeImageFilterBase
 *  \see itk::ConfidenceConnectedImageFilter
 *
 * \ingroup OTBLabelling
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT LabelizeConfidenceConnectedImageFilter
    : public otb::LabelizeImageFilterBase<TInputImage, TOutputImage, itk::ConfidenceConnectedImageFilter<TInputImage, TOutputImage>>
{
public:
  /** typedef for standard classes. */
  typedef LabelizeConfidenceConnectedImageFilter Self;
  typedef otb::LabelizeImageFilterBase<TInputImage, TOutputImage, itk::ConfidenceConnectedImageFilter<TInputImage, TOutputImage>> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** typedef to simplify variables definition and declaration. */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  typedef typename TInputImage::PixelType  InputPixelType;
  typedef typename TOutputImage::PixelType OutputPixelType;
  typedef typename TInputImage::IndexType  IndexType;

  /** "object factory" management method. */
  itkNewMacro(Self);

  /** Return the class name. */
  itkTypeMacro(LabelizeConfidenceConnectedImageFilter, LabelizeImageFilterBase);

  /** Get multiplier */
  double& GetMultiplier() const
  {
    return this->m_RegionGrowingFilter->GetMultiplier();
  }

  /** Set multiplier */
  void SetMultiplier(const double multiplier)
  {
    this->m_RegionGrowingFilter->SetMultiplier(multiplier);
  }

  /** Get number of iterations */
  unsigned int& GetNumberOfIterations() const
  {
    return this->m_RegionGrowingFilter->GetNumberOfIterations();
  }

  /** Set number of iterations */
  void SetNumberOfIterations(const unsigned int iteration)
  {
    this->m_RegionGrowingFilter->SetNumberOfIterations(iteration);
  }

  /** Get replace value */
  itkGetMacro(ReplaceValue, OutputPixelType);

  /** Set replace value */
  itkSetMacro(ReplaceValue, OutputPixelType);

  /** Set initial neighborhood radius */
  const unsigned int& GetInitialNeighborhoodRadius()
  {
    return this->m_RegionGrowingFilter->GetInitialNeighborhoodRadius();
  }

  /** Set initial neighborhood radius */
  void SetInitialNeighborhoodRadius(const unsigned int initial)
  {
    this->m_RegionGrowingFilter->SetInitialNeighborhoodRadius(initial);
  }

protected:
  LabelizeConfidenceConnectedImageFilter();
  ~LabelizeConfidenceConnectedImageFilter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Region growing */
  void RegionGrowing(const IndexType indexSeed) override;

private:
  LabelizeConfidenceConnectedImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Initial replace value*/
  OutputPixelType m_ReplaceValue;

}; // end class LabelizeconnectedThresholdImageFilter

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelizeConfidenceConnectedImageFilter.hxx"
#endif

#endif
