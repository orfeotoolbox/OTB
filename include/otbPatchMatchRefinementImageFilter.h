
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
  
 
  
/**  iterators */ 
  typedef itk::ConstNeighborhoodIterator<TInputImage> ConstNeighborhoodIteratorType;    
  typedef typename ConstNeighborhoodIteratorType::RadiusType      RadiusType;
  

 

// ====================== Geters ==============================================================================

  /** Get the inputs */
 

  
  const TInputImage *  GetNormalAndZValueImage() const;

   /** Get the Outputs */  
  TOutputImage * GetOutputPatchImage() ;
  TOutputImage * GetOutputNormalAndZValueImage() ;
 
  
  SizeType GetRadius()
     {
       return m_Radius;
    } 

//=============== Seters ====================================================================================

   /** Set Normal And ZValue of patch image input  */
  void SetNormalAndZValueImage(const TInputImage * image);
  
   
   
  /** Set unsigned int radius */
  void SetRadius(unsigned int radius)
  {
    m_Radius.Fill(radius);
  }
  
void SetMaxDz0(double MaxDz0){
	   m_MaxDz0 = MaxDz0;
	   
	}
	
void SetMaxDn(double  MaxDn){
	   m_MaxDn = MaxDn;
	  
	} 
	
void SetDispMinMax(double  DispMin, double DispMax){
	   m_DispMin = DispMin;
	   m_DispMax = DispMax;
	  
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



  
  /** Threaded generate data */
  void ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId) ITK_OVERRIDE;
  //void BeforeThreadedGenerateData()ITK_OVERRIDE;
private:
 PatchMatchRefinementImageFilter(const Self&); //purposely not implemented
 void operator=(const Self&); //purposely not implemeFnted  
 


    /** The radius of the blocks */
  SizeType                   	m_Radius; 
  
      /** The index of pixel in image*/
  IndexType						m_Index;
  double						m_MaxDn;
  double 						m_MaxDz0;
  SizeType					    m_PatchSize;
  double 						m_DispMin;
  double 						m_DispMax;
  
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPatchMatchRefinementImageFilter.txx"
#endif

#endif



















