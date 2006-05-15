/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :  29 mars 2006
  Role      :  Associative Symetrical Sum of Line Detector Image Filter  
  $Id$ 

=========================================================================*/
#ifndef __otbAssymmetricFusionOfLineDetectorImageFilter_h
#define __otbAssymmetricFusionOfLineDetectorImageFilter_h

#include "itkImageToImageFilter.h"

#include "itkLinearInterpolateImageFunction.h"
#include "otbLineRatioDetectorImageFilter.h"
#include "otbLineCorrelationDetectorImageFilter.h"
#include "otbAssociativeSymmetricalSumImageFilter.h"


namespace otb
{

/** \class AssymmetricFusionOfLineDetectorImageFilter
 *
 * This class implements a composite filter that combines three filters:
 * two filters of line detector ( a line detector by ratio and a line
 * detector by croos-correlation) and a filter that produces a fusion of  
 * those two line detector filters.
 *
 */

template <class TInputImage, 
	  class TOutputImage, 
	  class InterpolatorType = itk::LinearInterpolateImageFunction<TInputImage> >
class ITK_EXPORT AssymmetricFusionOfLineDetectorImageFilter : 
    public LineDetectorImageFilterBase< TInputImage, TOutputImage, InterpolatorType >
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
  
  typedef TOutputImage InputImageType1;
  typedef TOutputImage InputImageType2;
  

  typedef AssymmetricFusionOfLineDetectorImageFilter Self;
  typedef LineDetectorImageFilterBase< TInputImage, TOutputImage, InterpolatorType > Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(AssymmetricFusionOfLineDetectorImageFilter, LineDetectorImageFilterBase);
  
  typedef typename InputImageType::PixelType  InputPixelType;
  typedef typename InputImageType::SizeType SizeType;

  typedef typename OutputImageType::PixelType OutputPixelType;
 

protected:
  AssymmetricFusionOfLineDetectorImageFilter();
  virtual ~AssymmetricFusionOfLineDetectorImageFilter() {};
  
  typedef otb::LineRatioDetectorImageFilter< InputImageType, OutputImageType, InterpolatorType > 	LineRatioType;
  typedef otb::LineCorrelationDetectorImageFilter< InputImageType, OutputImageType, InterpolatorType > LineCorrelationType;
  typedef otb::AssociativeSymmetricalSumImageFilter< InputImageType1, InputImageType2, OutputImageType > AssSymSumType;

  virtual void GenerateData();
  
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  AssymmetricFusionOfLineDetectorImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  
  typename LineRatioType::Pointer	m_LineRatio;
  typename LineCorrelationType::Pointer	m_LineCorrelation;
  typename AssSymSumType::Pointer	m_AssSymSum;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbAssymmetricFusionOfLineDetectorImageFilter.txx"
#endif

  
#endif
