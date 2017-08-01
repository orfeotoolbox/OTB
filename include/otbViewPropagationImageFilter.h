
#ifndef otbViewPropagationImageFilter_h
#define otbViewPropagationImageFilter_h


#include "itkImageToImageFilter.h"
#include "itkContinuousIndex.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "otbImage.h"

#include "itkDataObject.h"

#include "otbAggregatedCostVectorPixelFilter.h"


namespace otb
{


template <class TInputImage,  class TOutputImage >
class ITK_EXPORT ViewPropagationImageFilter :
    public itk::ImageToImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedef */
  typedef ViewPropagationImageFilter       Self; 
  typedef itk::ImageToImageFilter<TInputImage, 
                                  TOutputImage>    Superclass; 
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ViewPropagationImageFilter, ImageToImageFilter); 


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
  
  typedef std::pair<PixelType,double> pairCord;
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
   TInputImage * GetLeftPatchInputImage() ; 
   TInputImage * GetRightPatchInputImage() ;
  const TInputImage *  GetLeftNormalAndZValueImage() const;
  const TInputImage *  GetRightNormalAndZValueImage() const;
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
  void SetLeftPatchInputImage( TInputImage * image);
  
    /** Set patch image input  */
  void SetRightPatchInputImage( TInputImage * image);
  
   /** Set Normal And ZValue of patch image input  */
  void SetLeftNormalAndZValueImage(TInputImage * image);
  
  void SetRightNormalAndZValueImage(TInputImage * image);
   /** Set patch image input */
  void SetOutputPatchImage(const TInputImage * image);
  
  
  
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
  ViewPropagationImageFilter();

  /** Destructor */
  ~ViewPropagationImageFilter() ITK_OVERRIDE;

  /** Generate output information */
  void GenerateOutputInformation() ITK_OVERRIDE; 

  /** Generate input requested region */
  void GenerateInputRequestedRegion() ITK_OVERRIDE; 
  
  
  typedef AggregatedCostVectorPixelFilter
    <TInputImage, TOutputImage>  AggregatedCostType;
  
  /** Threaded generate data */
  void ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId) ITK_OVERRIDE;
  void EnlargeOutputRequestedRegion(itk::DataObject */* image*/); //ITK_OVERRIDE;
private:
 ViewPropagationImageFilter(const Self&); //purposely not implemented
 void operator=(const Self&); //purposely not implemeFnted  
 
   static bool comparePair(pairCord i,pairCord j) { 
	if( i.first < j.first ) return true;
    if( j.first < i.first ) return false;
    return (i.first < j.first ); }


/** The Aggregated cost */
 typename AggregatedCostType::Pointer m_LeftCost;
 
 typename AggregatedCostType::Pointer m_RightCost;
  
    /** The radius of the blocks */
  SizeType                   	 m_Radius; 

    /** The offset of patch */
  unsigned int 					m_Iteration;
 

  
        /** The index of pixel in image*/
  SizeType						m_PatchSize;
     
    
  
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbViewPropagationImageFilter.txx"
#endif

#endif



















