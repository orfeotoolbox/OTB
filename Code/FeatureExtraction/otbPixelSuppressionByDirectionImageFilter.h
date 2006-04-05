/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   30 mars 2006
  Role      :   Filtre de suppression des pixels isoles par direction  
  $Id$ 

=========================================================================*/
#ifndef __otbPixelSuppressionByDirectionImageFilter_h
#define __otbPixelSuppressionByDirectionImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"

#define TRANSITION_MATRIX(_x,_y,_theta,_xout,_yout) \
    (_xout) = (_x)*cos(_theta) + (_y)*sin(_theta); \
    (_yout) = - (_x)*sin(_theta) + (_y)*cos(_theta)
    

namespace otb
{

/** \class PixelSuppressionByDirectionImageFilter
 * \brief Application of a filter of suppression of isolated pixels,
 * not belonging to a line, strating from the directions of pixels.
 *
 * This class implements an image filter which detects isolated pixels
 * that have little chance of belonging to a raod and performs a pixel
 * supression. For each pixel kept with direction \theta_{i}, we look
 * for other pixels with a direction close to \theta_i in an angular
 * beam around it. If none is found, the pixel is suppressed.
 *
 * 
 */

template <class TInputImage, class TOutputImage>
class PixelSuppressionByDirectionImageFilter :  public itk::ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** 	Extract input and output image dimensions */
  itkStaticConstMacro(		InputImageDimension,
  				unsigned int,
                      		TInputImage::ImageDimension);
  itkStaticConstMacro(		OutputImageDimension, 
  				unsigned int,
                      		TOutputImage::ImageDimension);


  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;

  /** typedef for the classes standards. */
  typedef PixelSuppressionByDirectionImageFilter Self;
  typedef itk::ImageToImageFilter< InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for management of the "object factory". */
  itkNewMacro(Self);

  /** Return the nale of the class. */
  itkTypeMacro(PixelSuppressionByDirectionImageFilter, ImageToImageFilter);
  
  /** Definition of the input and output images */
  typedef typename InputImageType::PixelType InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;

  typedef typename itk::NumericTraits<InputPixelType>::RealType InputRealType;
  
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  typedef typename InputImageType::SizeType SizeType;

  /** Set the radius of one zone. */
  itkSetMacro(Radius, SizeType);
  /** Get the radius of one zone. */
  itkGetConstReferenceMacro(Radius, SizeType);
  
  /** Set the angular beam. */
  itkSetMacro(AngularBeam, InputRealType);
  /** Get the angular beam. */
  itkGetConstReferenceMacro(AngularBeam, InputRealType);
  
  /** Set/Get the image input of this process object.  */
  void SetInputImage( const InputImageType *image);
  const InputImageType * GetInputImage(void);  
  
  void SetInputImageDirection( const InputImageType *image);
  const InputImageType * GetInputImageDirection(void);

  
  virtual void GenerateInputRequestedRegion() throw(itk::InvalidRequestedRegionError);

protected:
  PixelSuppressionByDirectionImageFilter();
  virtual ~PixelSuppressionByDirectionImageFilter() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );

private:
  PixelSuppressionByDirectionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  // Radius of the region
  SizeType m_Radius;
  // Angular Accuracy on the direction of the central pixel
  double m_AngularBeam;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPixelSuppressionByDirectionImageFilter.txx"
#endif

  
#endif
