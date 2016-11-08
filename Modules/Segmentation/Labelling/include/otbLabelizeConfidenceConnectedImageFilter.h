/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
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
  : public otb::LabelizeImageFilterBase<TInputImage, TOutputImage,
      itk::ConfidenceConnectedImageFilter<TInputImage, TOutputImage> >
{
public:
  /** typedef for standard classes. */
  typedef LabelizeConfidenceConnectedImageFilter
  Self;
  typedef otb::LabelizeImageFilterBase<TInputImage, TOutputImage,
      itk::ConfidenceConnectedImageFilter<TInputImage, TOutputImage> > Superclass;
  typedef itk::SmartPointer<Self>
  Pointer;
  typedef itk::SmartPointer<const Self>
  ConstPointer;

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
  ~LabelizeConfidenceConnectedImageFilter() ITK_OVERRIDE {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

  /** Region growing */
  void RegionGrowing(const IndexType indexSeed) ITK_OVERRIDE;

private:
  LabelizeConfidenceConnectedImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Initial replace value*/
  OutputPixelType m_ReplaceValue;

}; // end class LabelizeconnectedThresholdImageFilter

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelizeConfidenceConnectedImageFilter.txx"
#endif

#endif
