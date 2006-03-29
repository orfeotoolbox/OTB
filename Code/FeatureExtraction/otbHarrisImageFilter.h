/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :  28 mars 2006
  Role      :   Harris Filter 
  $Id$ 

=========================================================================*/
#ifndef __otbHarrisFilter_h
#define __otbHarrisFilter_h

#include "itkImageToImageFilter.h"
#include "itkRecursiveGaussianImageFilter.h"
#include "itkHessianRecursiveGaussianImageFilter.h"
#include "itkSymmetricSecondRankTensor.h"
#include "otbHessianToScalarImageFilter.h"
#include "otbMultiplyByScalarImageFilter.h"

namespace otb
{

/** \class HarrisFilter
 *
 */

template <class TInputImage, class TOutputImage>
class HarrisImageFilter :  public itk::ImageToImageFilter< TInputImage, TOutputImage >
{
public:

  itkStaticConstMacro(		InputImageDimension,
  				unsigned int,
                      		TInputImage::ImageDimension);
  itkStaticConstMacro(		OutputImageDimension, 
  				unsigned int,
                      		TOutputImage::ImageDimension);

  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;

  typedef HarrisImageFilter Self;
  typedef itk::ImageToImageFilter< InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(HarrisImageFilter, ImageToImageFilter);
  
  typedef typename InputImageType::PixelType  InputPixelType;
  typedef typename InputImageType::SizeType SizeType;

  typedef typename OutputImageType::PixelType OutputPixelType;

  typedef typename InputImageType::SizeType SizeType;

  typedef itk::Image< itk::SymmetricSecondRankTensor< 
                      typename itk::NumericTraits< InputPixelType>::RealType,
                                      ::itk::GetImageDimension<InputImageType>::ImageDimension >,
                      ::itk::GetImageDimension<InputImageType>::ImageDimension >  TensorType;
		      
  typedef itk::HessianRecursiveGaussianImageFilter<InputImageType,TensorType >  HessianFilterType;
  
  typedef itk::RecursiveGaussianImageFilter<TensorType,
                                            TensorType>                     GaussianFilterType;  
  typedef otb::HessianToScalarImageFilter<TensorType,OutputImageType >      HessianToScalarFilterType;
  typedef otb::MultiplyByScalarImageFilter<OutputImageType,OutputImageType> MultiplyScalarFilterType;

  itkSetMacro(SigmaD,double);
  itkGetConstReferenceMacro(SigmaD, double);
  itkSetMacro(SigmaI,double);
  itkGetConstReferenceMacro(SigmaI, double);
  itkSetMacro(Alpha,double);
  itkGetConstReferenceMacro(Alpha, double);


protected:
  HarrisImageFilter();
  virtual ~HarrisImageFilter() {};

  virtual void GenerateData();
  
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  HarrisImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  double m_SigmaD;
  double m_SigmaI;
  double m_Alpha;
  
  typename HessianFilterType::Pointer           m_HessianFilter;
  typename GaussianFilterType::Pointer          m_GaussianFilter;
  typename HessianToScalarFilterType::Pointer   m_HessianToScalarFilter;
  typename MultiplyScalarFilterType::Pointer    m_MultiplyScalarFilter; 
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbHarrisImageFilter.txx"
#endif

  
#endif
