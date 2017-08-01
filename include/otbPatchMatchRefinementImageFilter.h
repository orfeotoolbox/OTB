
#ifndef otbPatchMatchRefinementImageFilter_h
#define otbPatchMatchRefinementImageFilter_h


#include "itkImageToImageFilter.h"
#include "itkConstNeighborhoodIterator.h"
#include "otbImage.h"




namespace otb
{


template <class TInputImage,  class TOutputImage >
class ITK_EXPORT PatchMatchRefinementImageFilter :
    public itk::ImageToImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedef */
  typedef PatchMatchRefinementImageFilter       Self; 
  typedef itk::ImageToImageFilter<TInputImage, 
                                  TOutputImage>    Superclass; 
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(PatchMatchRefinementImageFilter, ImageToImageFilter); 


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
  typedef typename ConstNeighborhoodIteratorType::RadiusType      RadiusType;
  

 

// ====================== Geters ==============================================================================

  /** Get the inputs */
 
   TInputImage * GetPatchInputImage() ; 
  
  const TInputImage *  GetNormalAndZValueImage() const;

   /** Get the Outputs */  
  TInputImage * GetOutputPatchImage() ;
  TInputImage * GetOutputNormalAndZValueImage() ;
 
  
  SizeType GetRadius()
     {
       return m_Radius;
    } 

//=============== Seters ====================================================================================

  /** Set patch image input  */
  void SetPatchInputImage( TInputImage * image);

   /** Set Normal And ZValue of patch image input  */
  void SetNormalAndZValueImage(TInputImage * image);
  
   /** Set patch image input */
  void SetOutputPatchImage(const TInputImage * image);
   
   
  /** Set unsigned int radius */
  void SetRadius(unsigned int radius)
  {
    m_Radius.Fill(radius);
  }
  
int SetMaxdisp(int Maxdisp){
	   m_DispMax = Maxdisp;
	   return m_DispMax;
	}
	
 
    /** Set/Get the radius of the area on which metric is evaluated */
  itkSetMacro(Radius, SizeType);
  itkGetConstReferenceMacro(Radius, SizeType);

protected:
  /** Constructor */
  PatchMatchRefinementImageFilter();

  /** Destructor */
  ~PatchMatchRefinementImageFilter() ITK_OVERRIDE;

  /** Generate output information */
  void GenerateOutputInformation() ITK_OVERRIDE; 

  /** Generate input requested region */
  void GenerateInputRequestedRegion() ITK_OVERRIDE; 

  
  /** Threaded generate data */
  void ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId) ITK_OVERRIDE;
  //void BeforeThreadedGenerateData()ITK_OVERRIDE;
private:
 PatchMatchRefinementImageFilter(const Self&); //purposely not implemented
 void operator=(const Self&); //purposely not implemeFnted  
 


    /** The radius of the blocks */
  SizeType                   	 m_Radius; 

  
      /** The index of pixel in image*/
  IndexType						m_Index;
  int							m_DispMax;

  SizeType					m_PatchSize;
  
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPatchMatchRefinementImageFilter.txx"
#endif

#endif



















