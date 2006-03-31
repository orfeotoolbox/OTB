/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   30 mars 2006
  Role      :   Filtre de suppression par direction des pixels isoles  
  $Id$ 

=========================================================================*/
#ifndef __otbPixelSuppressionByDirectionImageFilter_h
#define __otbPixelSuppressionByDirectionImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"

namespace otb
{

/** \class PixelSuppressionByDirectionImageFilter
 * \brief 
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

  
  SizeType m_Radius;
  InputRealType m_AngularBeam;
  
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPixelSuppressionByDirectionImageFilter.txx"
#endif

  
#endif
