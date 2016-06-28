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
#ifndef otbLabelizeNeighborhoodConnectedImageFilter_h
#define otbLabelizeNeighborhoodConnectedImageFilter_h

#include "itkNeighborhoodConnectedImageFilter.h"
#include "otbLabelizeImageFilterBase.h"

namespace otb
{

/** \class LabelizeNeighborhoodConnectedImageFilter
 *  \brief Labels pixels that are connected to a seed and lie
 *         within a neighborhood
 *
 *  This class implements base class otb::LabelizeImageFilterBase
 *
 *  First seeds are obtained with a theshold filter
 *  For each seed, itk::NeighborhoodConnectedImageFilter labels pixel
 *  Finally, filter produce a regions map
 *
 * \ingroup OTBLabelling
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT LabelizeNeighborhoodConnectedImageFilter
  : public otb::LabelizeImageFilterBase<TInputImage, TOutputImage,
      itk::NeighborhoodConnectedImageFilter<TInputImage, TOutputImage> >
{
public:
  /** typedef for standard classes. */
  typedef LabelizeNeighborhoodConnectedImageFilter
  Self;
  typedef otb::LabelizeImageFilterBase<TInputImage, TOutputImage,
      itk::NeighborhoodConnectedImageFilter<TInputImage, TOutputImage> > Superclass;
  typedef itk::SmartPointer<Self>
  Pointer;
  typedef itk::SmartPointer<const Self>
  ConstPointer;

  /** typedef to simplify variables definition and declaration. */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  typedef typename TInputImage::PixelType InputPixelType;
  typedef typename TInputImage::SizeType  InputSizeType;
  typedef typename TInputImage::IndexType IndexType;

  typedef typename TOutputImage::PixelType OutputPixelType;

  /** "object factory" management method. */
  itkNewMacro(Self);

  /** Return the class name. */
  itkTypeMacro(LabelizeNeighborhoodConnectedImageFilter, LabelizeImageFilterBase);

  /** Get delta lower threshold */
  itkGetMacro(LowerThresholdDelta, InputPixelType);

  /** Set delta lower threshold */
  itkSetMacro(LowerThresholdDelta, InputPixelType);

  /** Get delta upper threshold */
  itkGetMacro(UpperThresholdDelta, InputPixelType);

  /** Set delta upper threshold */
  itkSetMacro(UpperThresholdDelta, InputPixelType);

  /** Get replace value */
  itkGetMacro(ReplaceValue, OutputPixelType);

  /** Set replace value */
  itkSetMacro(ReplaceValue, OutputPixelType);

  /** Get radius */
  const InputSizeType& GetRadius() const
  {
    return this->m_RegionGrowingFilter->GetRadius();
  }

  /** Set radius */
  void SetRadius(const InputSizeType radius)
  {
    this->m_RegionGrowingFilter->SetRadius(radius);
    this->Modified();
  }

protected:
  LabelizeNeighborhoodConnectedImageFilter();
  ~LabelizeNeighborhoodConnectedImageFilter() ITK_OVERRIDE {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

  /** Region growing */
  void RegionGrowing(const IndexType indexSeed) ITK_OVERRIDE;

private:
  LabelizeNeighborhoodConnectedImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Delta + threshold for growing region */
  InputPixelType m_UpperThresholdDelta;

  /** Delta - threshold for growing region */
  InputPixelType m_LowerThresholdDelta;

  /** Intial replace value*/
  OutputPixelType m_ReplaceValue;

}; // end class LabelizeconnectedThresholdImageFilter

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelizeNeighborhoodConnectedImageFilter.txx"
#endif

#endif
