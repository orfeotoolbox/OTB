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
#ifndef otbNRIBandImagesToOneNComplexBandsImage_h
#define otbNRIBandImagesToOneNComplexBandsImage_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"

/*
 * Inputs : N images made of two real bands 
 * Output : one single image made of N complex bands
 * 
 * */


namespace otb
{


template <class TInputImage, class TOutputImage>
class ITK_EXPORT NRIBandImagesToOneNComplexBandsImage :  public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /**   Extract input and output image dimension */
  itkStaticConstMacro(InputImageDimension,
                      unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension,
                      unsigned int,
                      TOutputImage::ImageDimension);

  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  /** standard class typedefs */
  typedef NRIBandImagesToOneNComplexBandsImage                                           Self;
  typedef itk::ImageToImageFilter<InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>                                  Pointer;
  typedef itk::SmartPointer<const Self>                            ConstPointer;

  /** Object factory management */
  itkNewMacro(Self);

  /** typemacro */
  itkTypeMacro(NRIBandImagesToOneNComplexBandsImage, ImageToImageFilter);

  typedef typename InputImageType::PixelType                    InputPixelType;
  typedef typename OutputImageType::PixelType                   OutputPixelType;
  typedef typename itk::NumericTraits<InputPixelType>::RealType InputRealType;
  typedef typename InputImageType::RegionType                   InputImageRegionType;
  typedef typename OutputImageType::RegionType                  OutputImageRegionType;


protected:
  NRIBandImagesToOneNComplexBandsImage();
  ~NRIBandImagesToOneNComplexBandsImage() ITK_OVERRIDE {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

  void GenerateOutputInformation(void) ITK_OVERRIDE;
  void BeforeThreadedGenerateData(void) ITK_OVERRIDE;
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId) ITK_OVERRIDE;

private:
  NRIBandImagesToOneNComplexBandsImage(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented


};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNRIBandImagesToOneNComplexBandsImage.txx"
#endif

#endif
