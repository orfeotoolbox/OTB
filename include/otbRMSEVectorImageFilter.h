
#ifndef otbRMSEVectorImageFilter_h
#define otbRMSEVectorImageFilter_h


#include "itkImageToImageFilter.h"
#include "itkContinuousIndex.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "otbImage.h"



namespace otb
{


template <class TInputImage,  class TOutputImage >
class ITK_EXPORT RMSEVectorImageFilter :
    public itk::ImageToImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedef */
  typedef RMSEVectorImageFilter       Self; 
  typedef itk::ImageToImageFilter<TInputImage, 
                                  TOutputImage>    Superclass; 
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(RMSEVectorImageFilter, ImageToImageFilter); 


  /** Useful typedefs */
   
  typedef TInputImage                                       InputImageType;
  typedef typename InputImageType::SizeType                 SizeType;
  typedef typename InputImageType::IndexType                IndexType;

  typedef typename InputImageType::OffsetType               OffsetType;
  typedef typename InputImageType::RegionType               RegionType;
  typedef typename InputImageType::PixelType                PixelType;
  
  typedef TOutputImage										OutputImageType;
  
  

  
// ====================== Geters ==============================================================================

  /** Get the inputs */
   TInputImage * GetEstimatedInputImage() ;
  TInputImage * GetInputImage() ;
     
 
//=============== Seters ====================================================================================
  /** Set image input */
  void SetEstimatedInputImage( TInputImage * image);
  void SetInputImage( TInputImage * image);



protected:
  /** Constructor */
  RMSEVectorImageFilter();

  /** Destructor */
  ~RMSEVectorImageFilter() ITK_OVERRIDE;

  /** Generate output information */
  void GenerateOutputInformation() ITK_OVERRIDE; 

  
  /** Threaded generate data */
  void ThreadedGenerateData(const RegionType & outputRegionForThread, itk::ThreadIdType threadId) ITK_OVERRIDE;

private:
 RMSEVectorImageFilter(const Self&); //purposely not implemented
 void operator=(const Self&); //purposely not implemeFnted  
 
 

     
      
  
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRMSEVectorImageFilter.txx"
#endif

#endif



















