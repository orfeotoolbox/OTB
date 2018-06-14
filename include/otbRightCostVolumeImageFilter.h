
#ifndef otbRightCostVolumeImageFilter_h
#define otbRightCostVolumeImageFilter_h


#include "itkImageToImageFilter.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "otbImage.h"

namespace otb
{


template <class TInputImage, class TGradientImage, class TOutputImage >
class ITK_EXPORT RightCostVolumeImageFilter :
    public itk::ImageToImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedef */
  typedef RightCostVolumeImageFilter       Self; 
  typedef itk::ImageToImageFilter<TInputImage, 
                                  TOutputImage>    Superclass; 
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(RightCostVolumeImageFilter, ImageToImageFilter); 

  /** Useful typedefs */
   
  typedef TInputImage                                       InputImageType;
  typedef typename InputImageType::SizeType                 SizeType;
  typedef typename InputImageType::IndexType                IndexType;
  typedef typename InputImageType::RegionType               RegionType;
  typedef typename InputImageType::PixelType               PixelType;
  
  typedef TOutputImage										OutputImageType;
  
/**  iterators */ 
  typedef itk::ConstNeighborhoodIterator<TInputImage> NeighborhoodIteratorType;
    
  typedef typename NeighborhoodIteratorType::RadiusType      RadiusType;
 

// ====================== Geters ==============================================================================

  /** Get the inputs */
  const TInputImage * GetLeftInputImage() const;
  const TInputImage * GetRightInputImage() const;
  const TGradientImage * GetLeftGradientXInput() const;
  const TGradientImage * GetLeftGradientYInput() const;
  const TGradientImage * GetRightGradientXInput() const;
  const TGradientImage * GetRightGradientYInput() const;

  /** Get the metric output */
  const TOutputImage * GetOutputImage() const;
  TOutputImage * GetOutputImage();

	int GetHorizontalMinDisparity() const ;
  int GetHorizontalMaxDisparity() const ;



      
//=============== Seters ====================================================================================
  /** Set left input */
  void SetLeftInputImage( const TInputImage * image);

  /** Set right input */
  void SetRightInputImage( const TInputImage * image);

  /** Set mask input (optional) */
  void SetLeftGradientXInput(const TGradientImage * image);
  void SetLeftGradientYInput(const TGradientImage * image);

  /** Set right mask input (optional) */
  void SetRightGradientXInput(const TGradientImage * image);
  void SetRightGradientYInput(const TGradientImage * image);
 
  void SetMinDisp(int disp)
  {
    m_HorizontalMinDisparity  = disp;
  }


  void SetMaxDisp(int disp)
  {
    m_HorizontalMaxDisparity  = disp;
  }

  void SetDisp(int disp)
  {
    m_HorizontalMaxDisparity  = disp;
  }


  int GetBandNumber()
  {
    unsigned int b = m_HorizontalMaxDisparity - m_HorizontalMinDisparity +1;
    return b ;
  }

  


protected:
  /** Constructor */
  RightCostVolumeImageFilter();

  /** Destructor */
  ~RightCostVolumeImageFilter() ITK_OVERRIDE;

  /** Generate output information */
  void GenerateOutputInformation() ITK_OVERRIDE; 

  /** Generate input requested region */
  void GenerateInputRequestedRegion() ITK_OVERRIDE; 


  /** Threaded generate data */
  void ThreadedGenerateData(const RegionType & outputRegionForThread, itk::ThreadIdType threadId) ITK_OVERRIDE;

private:
  RightCostVolumeImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemeFnted  
 void ComputeInputRegions(const RegionType& outputRegion, RegionType& LeftRegion,RegionType& RightRegion, int iteration_disp); 

   /** The min disparity to explore */
  int                           m_HorizontalMinDisparity;
  int                           m_HorizontalMaxDisparity;


   /** The min disparity to explore */
  int                           m_VerticalDisparity;


     

  


};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRightCostVolumeImageFilter.txx"
#endif

#endif
