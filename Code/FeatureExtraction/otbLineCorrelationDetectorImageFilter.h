/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   14 mars 2006
  Role      :   Filter of detection of linear features 
  $Id$ 

=========================================================================*/
#ifndef __otbLineCorrelationDetectorImageFilter_h
#define __otbLineCorrelationDetectorImageFilter_h

#include "itkNearestNeighborInterpolateImageFunction.h"
#include "itkLinearInterpolateImageFunction.h"
#include "otbLineDetectorImageFilterBase.h"


namespace otb
{


template <class TInputImage, 
	  class TOutputImage,
	  class InterpolatorType = itk::LinearInterpolateImageFunction<TInputImage> >
class ITK_EXPORT LineCorrelationDetectorImageFilter :  public LineDetectorImageFilterBase< TInputImage, TOutputImage, InterpolatorType >
{
public:
  /** 	Extract dimensions as well of the images of entry of exit. */
  itkStaticConstMacro(		InputImageDimension,
  				unsigned int,
                      		TInputImage::ImageDimension);
  itkStaticConstMacro(		OutputImageDimension, 
  				unsigned int,
                      		TOutputImage::ImageDimension);

  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;

  /** typedef for the classes standards. */
  typedef LineCorrelationDetectorImageFilter Self;
  typedef LineDetectorImageFilterBase< InputImageType, OutputImageType, InterpolatorType> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for management of the "object factory". */
  itkNewMacro(Self);

  /** Return the name of the class. */
  itkTypeMacro(LineCorrelationDetectorImageFilter, LineDetectorImageFilterBase);

  /** Typedefs to describe and access Interpolator */
  typedef typename InterpolatorType::Pointer InterpolatorPointer;
  typedef typename InterpolatorType::CoordRepType CoordRepType;
  
  typedef typename InputImageType::PointType TPoint;


  /** Definition of the input and output images */
  typedef typename InputImageType::PixelType InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;

  
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  /** Definition of the size of the images. */
  typedef typename InputImageType::SizeType SizeType;

  //virtual void GenerateInputRequestedRegion() throw(itk::InvalidRequestedRegionError);
  
//  const OutputImageType * GetOutputDirection();

protected:
  LineCorrelationDetectorImageFilter();
  virtual ~LineCorrelationDetectorImageFilter() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  //void BeforeThreadedGenerateData();

  /** LineCorrelationDetectorImageFilter can be implemented for a treatment of filter multithreaded. 
   * Thus, the ThreadedGenerateData() method is called for each thread process. 
   * The data image are allocated automatically by the mother class by calling the 
   * ThreadedGenerateData() method. ThreadedGenerateData can only write the portion 
   * of the image specified by the parameter "outputRegionForThread" 
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
/*  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );
*/

  virtual double ComputeMeasure(std::vector<double>* m1, std::vector<double>* m2, std::vector<double>* m3);

  /** Length of the linear feature = 2*m_LengthLine+1 */ 
  //unsigned int m_LengthLine;
 
  /** Width of the linear feature = 2*m_WidthLine+1 */ 
  //unsigned int m_WidthLine;
     
  /** Radius of the region*/
  //SizeType m_Radius;
  
  /** Size of the facelist*/
  //SizeType m_FaceList;
  
  //InterpolatorPointer      m_Interpolator;

  //typename OutputImageType::Pointer m_DirectionOuputImage;

  
private:
  LineCorrelationDetectorImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
 

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLineCorrelationDetectorImageFilter.txx"
#endif

  
#endif
