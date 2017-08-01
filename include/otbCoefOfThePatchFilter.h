
#ifndef otbCoefOfThePatchFilter_h
#define otbCoefOfThePatchFilter_h


#include "itkImageToImageFilter.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageReverseConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "otbImage.h"

namespace otb
{


template <class TInputImage,  class TOutputImage >
class ITK_EXPORT CoefOfThePatchFilter :
    public itk::ImageToImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedef */
  typedef CoefOfThePatchFilter       Self; 
  typedef itk::ImageToImageFilter<TInputImage, 
                                  TOutputImage>    Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(CoefOfThePatchFilter, ImageToImageFilter); 

  /** Useful typedefs */
   
  typedef TInputImage                                       InputImageType;
  typedef typename InputImageType::SizeType                 SizeType;
  typedef typename InputImageType::IndexType                IndexType;
  typedef typename InputImageType::RegionType               RegionType;
  typedef typename InputImageType::PixelType                PixelType;
  
  typedef TOutputImage										OutputImageType;
  

 

// ====================== Geters ==============================================================================

  /** Get the inputs */
  const TInputImage * GetInputImage() const; 

  /** Get the metric output */

 TOutputImage * GetOutputCoefImage();
 TOutputImage * GetOutputNormalAndZValueImage();
 
     
      
//=============== Seters ====================================================================================
  /** Set  inputs */
  void SetInputImage( const TInputImage * image);
  
     /** Set int disparity max and min */
   int SetMindisp(int Mindisp){
	   m_DispMin = Mindisp;
	   return m_DispMin;
	} 
   int SetMaxdisp(int Maxdisp){
	   m_DispMax = Maxdisp;
	   return m_DispMax;
	}
   
     int SetIteration(int it){
		m_Iteration = it; 
		return m_Iteration;
	} 

protected:
  /** Constructor */
  CoefOfThePatchFilter();

  /** Destructor */
  ~CoefOfThePatchFilter() ITK_OVERRIDE;

  /** Generate output information */
  void GenerateOutputInformation() ITK_OVERRIDE; 

 
  /** Threaded generate data */
  void ThreadedGenerateData(const RegionType & outputRegionForThread, itk::ThreadIdType threadId) ITK_OVERRIDE;

private:
  CoefOfThePatchFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemeFnted  
   
  
  /** The radius of the blocks */

	int 								m_DispMax;
	int 								m_DispMin;
	int 								m_Iteration;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbCoefOfThePatchFilter.txx"
#endif

#endif
