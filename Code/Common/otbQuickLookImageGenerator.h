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

#ifndef __otbQuickLookImageGenerator_h
#define __otbQuickLookImageGenerator_h

#include "otbMath.h"
#include "itkImageToImageFilter.h"
#include "otbImage.h"
#include "otbPerBandVectorImageFilter.h"
#include "otbStreamingShrinkImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"


namespace otb
{
  /** \class QuickLookImageGenerator
   *  \brief This functor computes a quicklook using discrete gaussian transform
   */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT QuickLookImageGenerator :
public itk::ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** Standard class typedefs. */
  typedef QuickLookImageGenerator                                        Self;
  typedef TInputImage                                                 InputImageType;
  typedef TOutputImage                                                OutputImageType;
  typedef typename InputImageType::PixelType                          InputPixelType;
  typedef typename OutputImageType::PixelType                         OutputPixelType;
  typedef typename itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                                     Pointer;
  typedef itk::SmartPointer<const Self>                               ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  typedef Image<double, 2>                                                              InternSingleImageType;
  typedef itk::DiscreteGaussianImageFilter<InternSingleImageType,InternSingleImageType> GaussianFilterType;
  typedef typename GaussianFilterType::Pointer                                          GaussianFilterPointerType;
  typedef PerBandVectorImageFilter<InputImageType, InputImageType, GaussianFilterType>  PerBandFilterType;
  typedef typename PerBandFilterType::Pointer                                           PerBandFilterPointerType;
  typedef StreamingShrinkImageFilter<InputImageType, InputImageType>                    ShrinkImageFilterType;
  typedef typename ShrinkImageFilterType::Pointer                                       ShrinkImageFilterPointerType;


  void SetVariance(double var)
    {
      m_Variance = var;
      m_GaussianFilter->SetVariance(m_Variance);
      this->Modified();
    };
  itkGetMacro(Variance, double);
  
  void UseImageSpacing(bool boo)
    {
      m_UseImageSpacing = boo;
      if (boo)
        m_GaussianFilter->SetUseImageSpacingOn();
      else
        m_GaussianFilter->SetUseImageSpacingOff();
    
      this->Modified();
    }
  itkGetMacro(UseImageSpacing, bool);

  void SetMaximumKernelWidth(unsigned int width)
    {
      m_MaximumKernelWidth = width;
      m_GaussianFilter->SetMaximumKernelWidth( m_MaximumKernelWidth );
      this->Modified();
    }
  itkGetMacro(MaximumKernelWidth, unsigned int);

 void SetMaximumError(double error)
    {
      m_MaximumError = error;
      m_GaussianFilter->SetMaximumError( m_MaximumError );
      this->Modified();
    }
  itkGetMacro(MaximumError, double);

  void SetSampleRatio(unsigned int sr)
    {
      m_SampleRatio = sr;
      m_ShrinkFilter->SetShrinkFactor( m_SampleRatio );
      this->Modified();
    }
  itkGetMacro(SampleRatio, unsigned int);

 
protected:
  QuickLookImageGenerator();
  virtual ~QuickLookImageGenerator(){};

  /** Actually process the input */
  virtual void GenerateData();
  virtual void GenerateOutputInformation();
 
  
  /** Display */
  void PrintSelf( std::ostream& os, itk::Indent indent ) const;

private:
  QuickLookImageGenerator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Internal filters*/
  PerBandFilterPointerType     m_PerBandFilter;
  ShrinkImageFilterPointerType m_ShrinkFilter;
  GaussianFilterPointerType    m_GaussianFilter;

  /** Variance value for Gaussian filter*/
  double m_Variance;
  /** Use spacing to compute Gaussain filter*/
  bool m_UseImageSpacing;
  /** Maximum kernel width for Gaussian filter*/
 double m_MaximumError;
 /** Maximum error tolerate for Gaussian filter*/
  unsigned int m_MaximumKernelWidth;

  /** Sample Ratio*/
  unsigned int m_SampleRatio;

  
};

}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbQuickLookImageGenerator.txx"
#endif

#endif
