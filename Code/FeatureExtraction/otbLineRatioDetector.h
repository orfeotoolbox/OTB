/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   14 mars 2006
  Role      :   Filter of detection of linear features 
  $Id$ 

=========================================================================*/
#ifndef __otbLineRatioDetector_h
#define __otbLineRatioDetector_h

#include "itkBSplineInterpolateImageFunction.h"
#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"

#define MIN(_A,_B) ((_A) < (_B) ? (_A) : (_B))
#define MAX(_A,_B) ((_A) > (_B) ? (_A) : (_B))  
#define ROTATION(_x,_y,_theta,_xout,_yout) \
    (_xout) = (_x)*cos(_theta) - (_y)*sin(_theta); \
    (_yout) = (_x)*sin(_theta) + (_y)*cos(_theta)


namespace otb
{

/** \class LineRatioDetector
 * \brief Application of the filter of detection of linear features 
 *
 * This class implements the Tupin's detector D1 used to detect 
 * two parallel lines. This detector is derived from the coupling of two
 * ratio edge detectors (Touzi detector) on both side of a region.
 *
 * The region is devided in three zones to delimite two parallel lines. 
 * The size of one zone is defined by the product of the width 
 * of the linear feature by its length.
 *
 * For each vertical line, we calculate the intensity of detection 
 * R_{12}(\theta_{0}) between zones 1 and 2 and R_{13}(\theta_{0}) between  
 * zones 1 and 3 according to the principle of the Touzi's filter.
 *
 * The response of the edge detector between two zones i and j is:
 *	\[R_{ij}=1-\min (\fract{\mu_{i}}{\mu_{j}};\fract{\mu_{j}}{\mu_{i}}) \]
 *
 * The intensity of detection in the three other directions R(\theta_{i})
 * is determined by rotation of the pixels of each zone around the 
 * pixel central of the region considered. By default, the pixel location after
 * rotation is determined by the Spline interpolator.
 * 
 * Finally, the intensity of detection formed by the two parallel lines 
 * is determined by the minimum response of a ration edge detector on both sides
 * of the linear structure:
 *	\[R = \min (R_{12};R_{13}) \]
 * where R_{12} and R_{13} are the maximum response of the ratio edge 
 * detector of R(\theta_{i}).
 * 
 * The exit is an image of intensity of detection.
 *
 * 
 */

template <class TInputImage, 
	  class TOutputImage,
	  class InterpolatorType = itk::BSplineInterpolateImageFunction<TInputImage> >
class LineRatioDetector :  public itk::ImageToImageFilter< TInputImage, TOutputImage >
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
  typedef LineRatioDetector Self;
  typedef itk::ImageToImageFilter< InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for management of the "object factory". */
  itkNewMacro(Self);

  /** Return the nale of the class. */
  itkTypeMacro(LineRatioDetector, ImageToImageFilter);

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
  
  /** Set the length of the linear feature. */
  itkSetMacro(LengthLine, unsigned int);

  /** Get the length of the linear feature. */
  itkGetConstReferenceMacro(LengthLine, unsigned int);

  /** Set the width of the linear feature. */
  itkSetMacro(WidthLine, unsigned int);

  /** Get the length of the linear feature. */
  itkGetConstReferenceMacro(WidthLine, unsigned int);
  
  /** Set the radius of one zone. */
  itkSetMacro(Radius, SizeType);

  /** Get the radius of one zone. */
  itkGetConstReferenceMacro(Radius, SizeType);
 

  virtual void GenerateInputRequestedRegion() throw(itk::InvalidRequestedRegionError);

protected:
  LineRatioDetector();
  virtual ~LineRatioDetector() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;


  /** LineRatioDetector can be implemented for a treatment of filter multithreaded. 
   * Thus, the ThreadedGenerateData() method is called for each thread process. 
   * The data image are allocated automatically by the mother class by calling the 
   * ThreadedGenerateData() method. ThreadedGenerateData can only write the portion 
   * of the image specified by the parameter "outputRegionForThread" 
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );

private:
  LineRatioDetector(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
 
  /** Length of the linear feature = 2*m_LengthLine+1 */ 
  unsigned int m_LengthLine;
 
  /** Width of the linear feature = 2*m_WidthLine+1 */ 
  unsigned int m_WidthLine;
     
  /** Radius of the region*/
  SizeType m_Radius;
  
  /** Size of the facelist*/
  SizeType m_FaceList;
  
  InterpolatorPointer      m_Interpolator;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLineRatioDetector.txx"
#endif

  
#endif
