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
#ifndef __otbCloudEstimatorFilter_h
#define __otbCloudEstimatorFilter_h

#include "otbSpectralAngleFunctor.h"
#include "itkUnaryFunctorImageFilter.h"

namespace otb
{
/** \class CloudEstimatorFilter
 *  \brief Filter to help with the cloud detection
 *
 * Applies spectral angle functor to an image.
 * Multiplies by a Gaussian coefficient
 * Applies a color reversal.
 *
 * \example FeatureExtraction/CloudDetectionExample.cxx
 *
 * \ingroup OTBCloudDetection
 */
template <class TInputImage, class TOutputImage, class TFunction = Functor::SpectralAngleFunctor<
        typename TInputImage::PixelType, typename TOutputImage::PixelType> >
class ITK_EXPORT CloudEstimatorFilter : public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction>
{
public:
  /** Standard class typedefs. */
  typedef CloudEstimatorFilter Self;
  typedef typename itk::UnaryFunctorImageFilter <TInputImage, TOutputImage, TFunction>
  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(CloudEstimatorFilter, UnaryFunctorImageFilter);

  /** Some convenient typedefs. */
  typedef          TInputImage                 InputImageType;
  typedef typename InputImageType::Pointer     InputImagePointer;
  typedef typename InputImageType::PixelType   InputPixelType;
  typedef          TOutputImage                OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointer;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::PixelType  OutputPixelType;

  /** Getters/Setters */
  void SetReferencePixel(InputPixelType ref);
  InputPixelType GetReferencePixel() const;
  void SetVariance(double var);
  double GetVariance() const;

protected:
  CloudEstimatorFilter();

  virtual ~CloudEstimatorFilter() {}

  virtual void BeforeThreadedGenerateData();

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  CloudEstimatorFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbCloudEstimatorFilter.txx"
#endif

#endif
