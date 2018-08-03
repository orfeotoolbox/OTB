
#ifndef otbCostVolumeFilter_h
#define otbCostVolumeFilter_h


#include "itkImageToImageFilter.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "otbImage.h"

namespace otb
{

  /** \class CostVolumeFilter
 *  \brief Perform the disparity-space range  of the reference image
 *
 *  Pixel-to-pixel comparison is done using the input images (left and right) and the luminence gradient of each 
 *  ones.
 *
 *
 * \ingroup OTBDisparityMap
 */


template <class TInputImage, class TGradientImage, class TOutputImage >
class ITK_EXPORT CostVolumeFilter :
    public itk::ImageToImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedef */
  typedef CostVolumeFilter       Self; 
  typedef itk::ImageToImageFilter<TInputImage, 
                                  TOutputImage>    Superclass; 
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(CostVolumeFilter, ImageToImageFilter); 

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

  float GetAlpha() const ;
  float GetTau1() const ;
  float GetTau2() const ;
  char GetSide() ;


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

  void SetAlpha(float param)
    {
    m_Alpha = param ;
    }

  void SetTau1(float param)
    {
    m_Tau1 = param ;
    }

  void SetTau2(float param)
    {
    m_Tau2 = param ;
    }

  void SetSide(char side)
    {
    m_Side = side ;
    }
    

protected:
  /** Constructor */
  CostVolumeFilter();

  /** Destructor */
  ~CostVolumeFilter() override;

  /** Generate output information */
  void GenerateOutputInformation() override; 

  /** Generate input requested region */
  void GenerateInputRequestedRegion() override; 


  /** Threaded generate data */
  void ThreadedGenerateData(const RegionType & outputRegionForThread, itk::ThreadIdType threadId) override;

private:
  CostVolumeFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemeFnted  
 void ComputeInputRegions(const RegionType& outputRegion, RegionType& LeftRegion,RegionType& RightRegion, int iteration_disp); 

   /** The min disparity to explore */
  int                           m_HorizontalMinDisparity;
  int                           m_HorizontalMaxDisparity;

   /** The min disparity to explore */
  int                             m_VerticalDisparity;
  float                           m_Alpha ;
  float                           m_Tau1 ;
  float                           m_Tau2 ;
  char                            m_Side ;        


};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbCostVolumeFilter.hxx"
#endif

#endif
