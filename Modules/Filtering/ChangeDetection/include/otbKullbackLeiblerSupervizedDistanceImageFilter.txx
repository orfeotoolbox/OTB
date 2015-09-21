/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Mines-Telecom. All rights reserved.
  See IMTCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbKullbackLeiblerSupervizedDistanceImageFilter_txx
#define __otbKullbackLeiblerSupervizedDistanceImageFilter_txx

#include "otbKullbackLeiblerSupervizedDistanceImageFilter.h"

namespace otb
{

namespace Functor
{

template <class TInput1, class TInput2, class TInputROIImage, class TOutput>
KullbackLeiblerSupervizedDistance<TInput1, TInput2, TInputROIImage, TOutput>
::KullbackLeiblerSupervizedDistance ()
{
  m_CumROI1 = NULL;
  m_CumROI2 = NULL;
}

template <class TInput1, class TInput2, class TInputROIImage, class TOutput>
KullbackLeiblerSupervizedDistance<TInput1, TInput2, TInputROIImage, TOutput>
::~KullbackLeiblerSupervizedDistance ()
{
  delete m_CumROI1;
  m_CumROI1 = NULL;

  delete m_CumROI2;
  m_CumROI2 = NULL;
}

template <class TInput1, class TInput2, class TInputROIImage, class TOutput>
void
KullbackLeiblerSupervizedDistance<TInput1, TInput2, TInputROIImage, TOutput>
::Evaluate(const typename TInput1::ImageType * img1,
           const typename TInput2::ImageType * img2,
           const TInputROIImage * imgROI)
{
  typedef ROIdataConversion<typename TInput1::ImageType, TInputROIImage>
  ROIConversionType1;

  typedef itk::ConstNeighborhoodIterator<
      typename ROIConversionType1::OutputImageType> ROIInputType1;

  typename ROIConversionType1::Pointer convertion1 = ROIConversionType1::New();
  convertion1->SetInputImage(img1);
  convertion1->SetROIImage(imgROI);
  convertion1->Update();

  if (m_CumROI1 != NULL) delete m_CumROI1;

  m_CumROI1 = new CumulantsForEdgeworth<ROIInputType1> (convertion1->GetOutput());

  if (!m_CumROI1->IsDataAvailable())
    {
    throw itk::ExceptionObject(__FILE__,
                               __LINE__,
                               "Cumulants estimated from ROI in image 1 are not usable",
                               ITK_LOCATION);
    }

  typedef ROIdataConversion<typename TInput2::ImageType, TInputROIImage>
  ROIConversionType2;

  typedef itk::ConstNeighborhoodIterator<
      typename ROIConversionType2::OutputImageType> ROIInputType2;

  typename ROIConversionType2::Pointer convertion2 = ROIConversionType2::New();
  convertion2->SetInputImage(img2);
  convertion2->SetROIImage(imgROI);
  convertion2->Update();

  if (m_CumROI2 != NULL) delete m_CumROI2;

  m_CumROI2 = new CumulantsForEdgeworth<ROIInputType2> (convertion2->GetOutput());

  if (!m_CumROI2->IsDataAvailable())
    {
    throw itk::ExceptionObject(__FILE__,
                               __LINE__,
                               "Cumulants estimated from ROI in image 2 are not usable",
                               ITK_LOCATION);
    }
}

template <class TInput1, class TInput2, class TInputROIImage, class TOutput>
TOutput
KullbackLeiblerSupervizedDistance<TInput1, TInput2, TInputROIImage, TOutput>
::operator() (const TInput1 &it1, const TInput2 &it2)
  {
  CumulantsForEdgeworth<TInput1> cum1(it1);

  if (!cum1.IsDataAvailable())
    {
    return static_cast<TOutput> (0.);
    }

  CumulantsForEdgeworth<TInput2> cum2(it2);

  if (!cum2.IsDataAvailable())
    {
    return static_cast<TOutput> (0.);
    }

  return static_cast<TOutput> (m_CumROI1->Divergence(cum1)
                               + m_CumROI2->Divergence(cum2));
  }

} // end of namespace Functor

/**
 *  Connect the training area to build the reference pdfs,
 *  with parameters to be hold by
 *  Functor::KullbackLeiblerSupervizedDistance.
 *
 *  Images 1 & 2 are supposed to be already connected.
 */
template <class TInputImage1, class TInputImage2, class TInputROIImage, class TOutputImage>
void
KullbackLeiblerSupervizedDistanceImageFilter<TInputImage1, TInputImage2, TInputROIImage, TOutputImage>
::SetTrainingArea(const TInputROIImage * trainingImage)
{
  this->itk::ProcessObject::SetNthInput(2, const_cast<TInputROIImage *>(trainingImage));
}

template <class TInputImage1, class TInputImage2, class TInputROIImage, class TOutputImage>
void
KullbackLeiblerSupervizedDistanceImageFilter<TInputImage1, TInputImage2, TInputROIImage, TOutputImage>
::BeforeThreadedGenerateData(void)
{
  typename TInputImage1::ConstPointer inputPtr1
    = dynamic_cast<const TInputImage1*>(this->GetInput(0));
  typename TInputImage2::ConstPointer inputPtr2
    = dynamic_cast<const TInputImage2*>(this->GetInput(1));
  typename TInputROIImage::ConstPointer trainingImage
    = static_cast<const TInputROIImage *>(this->itk::ProcessObject::GetInput(2));

  this->GetFunctor().Evaluate(inputPtr1, inputPtr2, trainingImage);
}

} // end of namespace otb

#endif
