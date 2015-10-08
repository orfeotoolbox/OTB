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
#ifndef __otbTwoNRIBandsImageToNComplexBandsImage_h
#define __otbTwoNRIBandsImageToNComplexBandsImage_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"

/*
 * Inputs : one single image made of 2N real bands 
 * Output : one single image made of N complex bands
 * 
 * */


namespace otb
{


template <class TInputImage, class TOutputImage>
class ITK_EXPORT TwoNRIBandsImageToNComplexBandsImage :  public itk::ImageToImageFilter<TInputImage, TOutputImage>
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
  typedef TwoNRIBandsImageToNComplexBandsImage                                           Self;
  typedef itk::ImageToImageFilter<InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>                                  Pointer;
  typedef itk::SmartPointer<const Self>                            ConstPointer;

  /** Object factory management */
  itkNewMacro(Self);

  /** typemacro */
  itkTypeMacro(TwoNRIBandsImageToNComplexBandsImage, ImageToImageFilter);

  typedef typename InputImageType::PixelType                    InputPixelType;
  typedef typename OutputImageType::PixelType                   OutputPixelType;
  typedef typename itk::NumericTraits<InputPixelType>::RealType InputRealType;
  typedef typename InputImageType::RegionType                   InputImageRegionType;
  typedef typename OutputImageType::RegionType                  OutputImageRegionType;


protected:
  TwoNRIBandsImageToNComplexBandsImage();
  virtual ~TwoNRIBandsImageToNComplexBandsImage() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void GenerateOutputInformation(void);
  void BeforeThreadedGenerateData(void);
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId);

private:
  TwoNRIBandsImageToNComplexBandsImage(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented


};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTwoNRIBandsImageToNComplexBandsImage.txx"
#endif

#endif
