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

#include "otbLineRatioDetector.h"
#include "otbLineCorrelationDetector.h"
#include "otbAssociativeSymmetricalSumImageFilter.h"


namespace otb
{

/** \class AssymmetricFusionOfLineDetectorImageFilter
 *
 */

template <class TInputImage, 
	  class TOutputImage, 
	  class InterpolatorType = itk::BSplineInterpolateImageFunction<TInputImage> >
class ITK_EXPORT AssymmetricFusionOfLineDetectorImageFilter : 
public itk::ImageToImageFilter< TInputImage, TOutputImage >
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
  typedef itk::ImageToImageFilter< InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(AssymmetricFusionOfLineDetectorImageFilter, ImageToImageFilter);
  
  typedef typename InputImageType::PixelType  InputPixelType;
  typedef typename InputImageType::SizeType SizeType;

  typedef typename OutputImageType::PixelType OutputPixelType;

  typedef typename InputImageType::SizeType SizeType;
 

  /** Set the length of the linear feature. */
  itkSetMacro(LengthLine, unsigned int);

  /** Get the length of the linear feature. */
  itkGetConstReferenceMacro(LengthLine, unsigned int);

  /** Set the width of the linear feature. */
  itkSetMacro(WidthLine, unsigned int);

  /** Get the length of the linear feature. */
  itkGetConstReferenceMacro(WidthLine, unsigned int);

protected:
  AssymmetricFusionOfLineDetectorImageFilter();
  virtual ~AssymmetricFusionOfLineDetectorImageFilter() {};
  
  typedef otb::LineRatioDetector< InputImageType, OutputImageType, InterpolatorType > 	LineRatioType;
  typedef otb::LineCorrelationDetector< InputImageType, OutputImageType, InterpolatorType > LineCorrelationType;
  typedef otb::AssociativeSymmetricalSumImageFilter< InputImageType1, InputImageType2, OutputImageType > AssSymSumType;

  virtual void GenerateData();
  
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  AssymmetricFusionOfLineDetectorImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Length of the linear feature = 2*m_LengthLine+1 */ 
  unsigned int m_LengthLine;
 
  /** Width of the linear feature = 2*m_WidthLine+1 */ 
  unsigned int m_WidthLine;
  
  typename LineRatioType::Pointer	m_LineRatio;
  typename LineCorrelationType::Pointer	m_LineCorrelation;
  typename AssSymSumType::Pointer	m_AssSymSum;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbAssymmetricFusionOfLineDetectorImageFilter.txx"
#endif

  
#endif
