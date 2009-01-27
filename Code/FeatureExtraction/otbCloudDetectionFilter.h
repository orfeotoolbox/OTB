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
#ifndef __otbCloudDetectionFilter_h
#define __otbCloudDetectionFilter_h

#include "otbSpectralAngleFunctor.h"
#include "itkUnaryFunctorImageFilter.h"

namespace otb
{
/** \class CloudDetectionFilter
 * \brief Apply spectral angle functor to an image.
 * \brief Apply a threshold.
 * \brief Apply a color reversal.
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT CloudDetectionFilter :  public itk::UnaryFunctorImageFilter< TInputImage, TOutputImage,                                       /* TFunction =*/ Functor::SpectralAngleFunctor< ITK_TYPENAME TInputImage::PixelType, 
                                                    ITK_TYPENAME TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef CloudDetectionFilter                           Self;
  typedef typename itk::UnaryFunctorImageFilter < TInputImage,
                                                  TOutputImage,
                                                    Functor::SpectralAngleFunctor < typename TInputImage::PixelType, typename TOutputImage::PixelType > > Superclass;

  typedef itk::SmartPointer<Self>                       Pointer;
  typedef itk::SmartPointer<const Self>                 ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(CloudDetectionFilter,UnaryFunctorImageFilter);

  /** Some convenient typedefs. */
  typedef          TInputImage                    InputImageType;
  typedef typename InputImageType::Pointer        InputImagePointer;
  typedef typename InputImageType::PixelType      InputPixelType;
  typedef          TOutputImage                   OutputImageType;
  typedef typename OutputImageType::Pointer       OutputImagePointer;
  typedef typename OutputImageType::RegionType    OutputImageRegionType;
  typedef typename OutputImageType::PixelType     OutputPixelType;

 
  itkSetMacro(Variance, double);
  itkGetMacro(Variance, double);
  itkSetMacro(Threshold, double);
  itkGetMacro(Threshold, double);

  void SetReferencePixel( InputPixelType ref ){ this->GetFunctor().SetReferencePixel( ref ); };
  InputPixelType GetReferencePixel(){ return this->GetFunctor().GetReferencePixel(); };
  
/*  void SetVariance(double var){ this->GetFunctor().SetVariance( var ); };
  double GetVariance(){ return this->GetFunctor().GetVariance();
*/

protected:
  CloudDetectionFilter();

  virtual ~CloudDetectionFilter(){};

  virtual void BeforeThreadedGenerateData();

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  CloudDetectionFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Gaussian parameter */
  double m_Variance;

  /** Threshold for the binary image */
  double m_Threshold;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbCloudDetectionFilter.txx"
#endif

#endif

