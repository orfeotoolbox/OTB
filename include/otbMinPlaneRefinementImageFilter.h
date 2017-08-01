

#ifndef otbMinPlaneRefinementImageFilter_h
#define otbMinPlaneRefinementImageFilter_h


#include "itkImageToImageFilter.h"
#include "itkContinuousIndex.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "otbImage.h"


#include "otbAggregatedCostVectorPixelFilter.h"


namespace otb
{


template <class TInputImage,  class TOutputImage >
class ITK_EXPORT MinPlaneRefinementImageFilter :
    public itk::ImageToImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedef */
  typedef MinPlaneRefinementImageFilter       Self; 
  typedef itk::ImageToImageFilter<TInputImage, 
                                  TOutputImage>    Superclass; 
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MinPlaneRefinementImageFilter, ImageToImageFilter); 


  /** Useful typedefs */
   
  typedef TInputImage                                       InputImageType;
  typedef typename InputImageType::SizeType                 SizeType;
  typedef typename InputImageType::IndexType                IndexType;

  typedef typename InputImageType::OffsetType               OffsetType;
  typedef typename InputImageType::RegionType               RegionType;
  typedef typename InputImageType::PixelType                PixelType;
  typedef typename InputImageType::InternalPixelType        InternalPixelType;
  
  typedef TOutputImage										OutputImageType;
  
  typedef typename itk::ContinuousIndex<double, 1> 			ContinuousIndextype;
  
/**  iterators */ 
  typedef itk::ConstNeighborhoodIterator<TInputImage> ConstNeighborhoodIteratorType;
  
    typedef itk::ImageConstIterator<TInputImage> ConstIteratorType;
    
  typedef typename ConstNeighborhoodIteratorType::RadiusType      RadiusType;
  
  typedef typename ConstNeighborhoodIteratorType::NeighborIndexType        NeighborIndexType;
 

// ====================== Geters ==============================================================================

  /** Get the inputs */
  const TInputImage * GetLeftInputImage() const;
  const TInputImage * GetRightInputImage() const;
  const TInputImage * GetLeftGradientXInput() const;
  const TInputImage * GetRightGradientXInput() const;
  TInputImage * GetPatchInputImage();
 const TInputImage *  GetNormalAndZValueImage() const;
 const TInputImage *  GetCostInputImage() const;

 TInputImage *  GetCoefRefinedInputImage() ;
const TInputImage *  GetRefinedNormalAndZValueImage() const;
  /** Get the metric output */
   
  TOutputImage * GetOutputCostImage();
  
  TOutputImage * GetOutputPatchImage();
  
  TOutputImage * GetOutputNormalAndZValueImage();
  
     
  SizeType GetRadius()
     {
       return m_Radius;
    } 
    
 /** Get  Patch Size */
 SizeType GetPatchSize() const
  {
    return m_PatchSize;
  }  
//=============== Seters ====================================================================================
  /** Set image input */
  void SetLeftInputImage( const TInputImage * image);
  void SetRightInputImage( const TInputImage * image);

  /** Set Gradient input */
  void SetLeftGradientXInput(const TInputImage * image);
  void SetRightGradientXInput(const TInputImage * image);

  /** Set patch image input  */
  void SetPatchInputImage(TInputImage * image);
    /** Set cost image input  */
  void SetCostInputImage( TInputImage * image);
  
  void SetNormalAndZValueImage(TInputImage * image);
  
 
   /** Set coef patch image input  */
  void SetCoefRefinedInputImage( TInputImage * image);
  
  void SetRefinedNormalAndZValueImage(TInputImage * image);

  void SetIteration(unsigned int x)
  {
    m_Iteration  = x;
  }
  
  unsigned int GetIteration()
  {
    return m_Iteration;
  }
  
  /** Set unsigned int radius */
  void SetRadius(unsigned int radius)
  {
    m_Radius.Fill(radius);
  }
  
    /** Set unsigned int Patch offset */
  void SetOffsetPatch(OffsetType Offset0, OffsetType Offset1, OffsetType Offset2)
  {
    m_Offset0[0] = Offset0[0];
    m_Offset0[1] = Offset0[1];
    
    m_Offset1[0] = Offset1[0];
    m_Offset1[1] = Offset1[1];
    
    m_Offset2[0] = Offset2[0];
    m_Offset2[1] = Offset2[1];
  }
  
  /** Set unsigned int Patch Size */
 void SetPatchSize(int XSize, int YSize)
  {
    m_PatchSize[0] = XSize;
    m_PatchSize[1] = YSize;
  }
    /** Set/Get the radius of the area on which metric is evaluated */
  itkSetMacro(Radius, SizeType);
  itkGetConstReferenceMacro(Radius, SizeType);

protected:
  /** Constructor */
  MinPlaneRefinementImageFilter();

  /** Destructor */
  ~MinPlaneRefinementImageFilter() ITK_OVERRIDE;

  /** Generate output information */
  void GenerateOutputInformation() ITK_OVERRIDE; 

  /** Generate input requested region */
  void GenerateInputRequestedRegion() ITK_OVERRIDE; 
  
  
  typedef AggregatedCostVectorPixelFilter
    <TInputImage, TOutputImage>  AggregatedCostType;
  
  /** Threaded generate data */
  void ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId) ITK_OVERRIDE;

private:
 MinPlaneRefinementImageFilter(const Self&); //purposely not implemented
 void operator=(const Self&); //purposely not implemeFnted  
 


/** The Aggregated cost */
 typename AggregatedCostType::Pointer m_Cost;
  
    /** The radius of the blocks */
  SizeType                   	 m_Radius; 

    /** The offset of patch */
  unsigned int 						 	 m_Iteration;
 
  
      /** The index of pixel in image*/
  IndexType						m_Index;
  
        /** The index of pixel in image*/
  SizeType						m_PatchSize;
     
  OffsetType					m_Offset0;
  OffsetType					m_Offset1;
  OffsetType					m_Offset2;    
  
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMinPlaneRefinementImageFilter.txx"
#endif

#endif



















