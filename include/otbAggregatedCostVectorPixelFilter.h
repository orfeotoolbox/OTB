
#ifndef otbAggregatedCostVectorPixelFilter_h
#define otbAggregatedCostVectorPixelFilter_h


#include "itkImageToImageFilter.h"
#include "itkContinuousIndex.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "otbImage.h"



namespace otb
{


template <class TInputImage,  class TOutputImage >
class ITK_EXPORT AggregatedCostVectorPixelFilter :
    public itk::ImageToImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedef */
  typedef AggregatedCostVectorPixelFilter       Self; 
  typedef itk::ImageToImageFilter<TInputImage, 
                                  TOutputImage>    Superclass; 
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(AggregatedCostVectorPixelFilter, ImageToImageFilter); 


  /** Useful typedefs */
   
  typedef TInputImage                                       InputImageType;
  typedef typename InputImageType::SizeType                 SizeType;
  typedef typename InputImageType::IndexType                IndexType;

  typedef typename InputImageType::OffsetType               OffsetType;
  typedef typename InputImageType::RegionType               RegionType;
  typedef typename InputImageType::PixelType                PixelType;
  
  typedef TOutputImage										OutputImageType;
  
  typedef typename itk::ContinuousIndex<double, 1> 			ContinuousIndextype;
  

 

// ====================== Geters ==============================================================================

  /** Get the inputs */
  const TInputImage * GetLeftInputImage() const;
  const TInputImage * GetRightInputImage() const;
  const TInputImage * GetLeftGradientXInput() const;
  const TInputImage * GetRightGradientXInput() const;
  TInputImage * GetPatchInputImage();

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

  /** Set patch image input (optional) */
  void SetPatchInputImage(TInputImage * image);

 
  void SetIndex(unsigned int x, unsigned int y)
  {
    m_Index[0]  = x;
    m_Index[1]  = y;
  }

    /** Set unsigned int Patch offset */
  void SetOffsetPatch(OffsetType Offset0, OffsetType Offset1, OffsetType Offset2)
  {
    m_PatchOffset0[0] = Offset0[0];
    m_PatchOffset0[1] = Offset0[1];
    
    m_PatchOffset1[0] = Offset1[0];
    m_PatchOffset1[1] = Offset1[1];
    
    m_PatchOffset2[0] = Offset2[0];
    m_PatchOffset2[1] = Offset2[1];
  }
  
  /** Set unsigned int Patch Size */
 void SetPatchSize(int XSize, int YSize)
  {
    m_PatchSize[0] = XSize;
    m_PatchSize[1] = YSize;
  }
 

protected:
  /** Constructor */
  AggregatedCostVectorPixelFilter();

  /** Destructor */
  ~AggregatedCostVectorPixelFilter() ITK_OVERRIDE;

  /** Generate output information */
  void GenerateOutputInformation() ITK_OVERRIDE; 

  /** Generate input requested region */
  void GenerateInputRequestedRegion() ITK_OVERRIDE; 
  
  /** Threaded generate data */
  void ThreadedGenerateData(const RegionType & outputRegionForThread, itk::ThreadIdType threadId) ITK_OVERRIDE;

private:
 AggregatedCostVectorPixelFilter(const Self&); //purposely not implemented
 void operator=(const Self&); //purposely not implemeFnted  

    /** The offset of patch */
  OffsetType					m_PatchOffset0;
  OffsetType					m_PatchOffset1;
  OffsetType					m_PatchOffset2;
  
      /** The index of pixel in image*/
  IndexType						m_Index;
  
        /** The index of pixel in image*/
  SizeType						m_PatchSize;
     
      
  
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbAggregatedCostVectorPixelFilter.txx"
#endif

#endif



















