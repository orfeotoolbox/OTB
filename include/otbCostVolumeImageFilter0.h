
#ifndef otbCostVolumeImageFilter0_h
#define otbCostVolumeImageFilter0_h


#include "itkImageToImageFilter.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "otbImage.h"

namespace otb
{


template <class TInputImage, class TGradientImage, class TOutputImage >
class ITK_EXPORT CostVolumeImageFilter0 :
    public itk::ImageToImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedef */
  typedef CostVolumeImageFilter0       Self; //*
  typedef itk::ImageToImageFilter<TInputImage, 
                                  TOutputImage>    Superclass; //*
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(CostVolumeImageFilter0, ImageToImageFilter); //*

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

	int GetHDisparity() const ;
     
      
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
 
  void SetDisp(int disp)
  {
    m_HorizontalDisparity  = disp;
  }
  
    //~ void SetMinMAxDisp(int Mindisp,int Maxdisp)
  //~ {
    //~ m_MinHDisp  = Mindisp;
    //~ m_MaxHDisp = Maxdisp;
  //~ }

  //~ /** Set/Get the radius of the area on which metric is evaluated */
  //~ itkSetMacro(Radius, SizeType);
  //~ itkGetConstReferenceMacro(Radius, SizeType);


protected:
  /** Constructor */
  CostVolumeImageFilter0();

  /** Destructor */
  ~CostVolumeImageFilter0() ITK_OVERRIDE;

  /** Generate output information */
  void GenerateOutputInformation() ITK_OVERRIDE; // meme taille de sortie, 1 composante

  /** Generate input requested region */
  void GenerateInputRequestedRegion() ITK_OVERRIDE; // voir dans unaryneighborhood


  /** Threaded generate data */
  void ThreadedGenerateData(const RegionType & outputRegionForThread, itk::ThreadIdType threadId) ITK_OVERRIDE;

private:
  CostVolumeImageFilter0(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemeFnted  
 void ComputeInputRegions(const RegionType& outputRegion, RegionType& LeftRegion,RegionType& RightRegion); 

   /** The min disparity to explore */
  int                           m_HorizontalDisparity;
 // int                           m_MinHDisp;
 // int                           m_MaxHDisp;

   /** The min disparity to explore */
  int                           m_VerticalDisparity;
  
  
  //~ /** The radius of the blocks */
  //~ SizeType                      	  m_Radius;


};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbCostVolumeImageFilter0.txx"
#endif

#endif
