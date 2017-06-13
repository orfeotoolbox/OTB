
#ifndef otbBoxImageFilter_h
#define otbBoxImageFilter_h

#include "itkConstantBoundaryCondition.h"
#include "itkImageToImageFilter.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "otbImage.h"



namespace otb
{


template <class TInputImage , class TOutputImage >  
class ITK_EXPORT BoxImageFilter :
    public itk::ImageToImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedef */
  typedef BoxImageFilter       Self; 
  typedef itk::ImageToImageFilter<TInputImage, 
                                  TOutputImage>    Superclass; 
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(BoxImageFilter, ImageToImageFilter); 

  /** Useful typedefs */
   
  typedef TInputImage                                       InputImageType;
  typedef typename InputImageType::SizeType                 SizeType;
  typedef typename InputImageType::IndexType                IndexType;
  typedef typename InputImageType::RegionType               RegionType;
  typedef typename InputImageType::PixelType                PixelType;
  
  typedef TOutputImage										OutputImageType;

  
  
  typedef typename itk::ConstantBoundaryCondition<TInputImage>  BoundaryConditionType;


  
/**  iterators */ 
  typedef typename itk::ConstNeighborhoodIterator<TInputImage> ConstNeighborhoodIteratorType;
  typedef typename itk::NeighborhoodIterator<TInputImage, BoundaryConditionType> NeighborhoodIteratorType; 
     
  typedef typename ConstNeighborhoodIteratorType::RadiusType      RadiusType;
  typedef typename ConstNeighborhoodIteratorType::OffsetType      OffsetType;
    
  



// ====================== Geters ==============================================================================

  /** Get the inputs */
  const TInputImage * GetImageInput() const;
  

  /** Get the metric output */
  TOutputImage * GetOutputImage();

  double GetRadius(double radius) const
     {
       return radius;
    }
//=============== Seters ====================================================================================
  /** Set Cost Volume input */
  void SetImageInput( const TInputImage * image);

    
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
  BoxImageFilter();

  /** Destructor */
  ~BoxImageFilter() ITK_OVERRIDE;

  /** Generate output information */
  void GenerateOutputInformation() ITK_OVERRIDE; 

  /** Threaded generate data */
  void ThreadedGenerateData(const RegionType & outputRegionForThread, itk::ThreadIdType threadId) ITK_OVERRIDE;

private:
  BoxImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemeFnted  


  
  /** The radius of the blocks */
  SizeType                      m_Radius; 
  

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBoxImageFilter.txx"
#endif

#endif
