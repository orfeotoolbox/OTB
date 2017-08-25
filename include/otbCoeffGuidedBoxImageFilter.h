
#ifndef otbCoeffGuidedBoxImageFilter_h
#define otbCoeffGuidedBoxImageFilter_h


#include "itkImageToImageFilter.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "otbImage.h"



namespace otb
{

template <class TInputImage , class TOutputImage >  
class ITK_EXPORT CoeffGuidedBoxImageFilter :
    public itk::ImageToImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedef */
  typedef CoeffGuidedBoxImageFilter       Self; 
  typedef itk::ImageToImageFilter<TInputImage, 
                                  TOutputImage>    			Superclass; 
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(CoeffGuidedBoxImageFilter, ImageToImageFilter); 

  /** Useful typedefs */
   
  typedef TInputImage                                       InputImageType;
  typedef typename InputImageType::SizeType                 SizeType;
  typedef typename InputImageType::IndexType                IndexType;
  typedef typename InputImageType::RegionType               RegionType;
  typedef typename InputImageType::PixelType                PixelType;
  
  typedef TOutputImage										OutputImageType;

  
/**  iterators */ 
  typedef typename itk::ConstNeighborhoodIterator<TInputImage> ConstNeighborhoodIteratorType;   

// ====================== Geters ==============================================================================

  /** Get the inputs */
  const TInputImage * GetCostVolumeImageInput() const;
  const TInputImage * GetMeanCostVolumeImageInput() const;
  const TInputImage * GetMeanImageInput() const;
  const TInputImage * GetCovarianceImageInput() const;
  const TInputImage * GetCovIm1CostImageInput() const;
  const TInputImage * GetLeftImageInput() const;
 

  /** Get the metric output */
  TOutputImage * GetOutputImage();

  double GetRadius(double radius) const
     {
       return radius;
    }
//=============== Seters ====================================================================================
  /** Set Cost Volume input */
  void SetCostVolumeImageInput( const TInputImage * image);
  void SetMeanCostVolumeImageInput(const TInputImage * image);

  /** Set Mean image input */
  void SetMeanImageInput( const TInputImage * image);

  /** Set Covariance image input (optional) */
  void SetCovarianceImageInput(const TInputImage * image);
  
  void SetCovIm1CostImageInput(const TInputImage * image);
  /** Set Left image input (optional) */  
  void SetLeftImageInput(const TInputImage * image);
  
  /** Set unsigned int radius */
  void SetRadius(unsigned int radius)
  {
    m_Radius.Fill(radius);
  }

  /** Set/Get the radius of the area on which metric is evaluated */
  itkSetMacro(Radius, SizeType);
  itkGetConstReferenceMacro(Radius, SizeType);


protected:
  /** Constructor */
  CoeffGuidedBoxImageFilter();

  /** Destructor */
  ~CoeffGuidedBoxImageFilter() ITK_OVERRIDE;

  /** Generate output information */
  void GenerateOutputInformation() ITK_OVERRIDE; 

  /** Threaded generate data */
  void ThreadedGenerateData(const RegionType & outputRegionForThread, itk::ThreadIdType threadId) ITK_OVERRIDE;

private:
  CoeffGuidedBoxImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemeFnted  




   /** The epsilon of the filter weights */
  float                           m_Epsilon;
  
  
  /** The radius of the blocks */
  SizeType                      m_Radius; 
  

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbCoeffGuidedBoxImageFilter.txx"
#endif

#endif
