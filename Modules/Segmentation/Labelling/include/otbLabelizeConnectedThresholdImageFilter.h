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
#ifndef __otbLabelizeConnectedThresholdImageFilter_h
#define __otbLabelizeConnectedThresholdImageFilter_h

#include "itkConnectedThresholdImageFilter.h"
#include "otbLabelizeImageFilterBase.h"

namespace otb
{

/** \class LabelizeConnectedThresholdImageFilter
 *  \brief TODO
 *
 *
 * \ingroup OTBLabelling
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT LabelizeConnectedThresholdImageFilter
  : public otb::LabelizeImageFilterBase<TInputImage, TOutputImage,
      itk::ConnectedThresholdImageFilter<TInputImage, TOutputImage> >
{
public:
  /** typedef for standard classes. */
  typedef LabelizeConnectedThresholdImageFilter
  Self;
  typedef otb::LabelizeImageFilterBase<TInputImage, TOutputImage,
      itk::ConnectedThresholdImageFilter<TInputImage, TOutputImage> > Superclass;
  typedef itk::SmartPointer<Self>
  Pointer;
  typedef itk::SmartPointer<const Self>
  ConstPointer;

  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  typedef typename TInputImage::PixelType  InputPixelType;
  typedef typename TOutputImage::PixelType OutputPixelType;
  typedef typename TInputImage::IndexType  IndexType;

  /** "object factory" management method. */
  itkNewMacro(Self);

  /** Return the class name. */
  itkTypeMacro(LabelizeConnectedThresholdImageFilter, LabelizeImageFilterBase);

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

protected:
  LabelizeConnectedThresholdImageFilter();
  ~LabelizeConnectedThresholdImageFilter() ITK_OVERRIDE {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

  /** Region growing */
  void RegionGrowing(const IndexType indexSeed) ITK_OVERRIDE;

private:
  LabelizeConnectedThresholdImageFilter(const Self &); //purposely not implemented
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
#include "otbLabelizeConnectedThresholdImageFilter.txx"
#endif

#endif
