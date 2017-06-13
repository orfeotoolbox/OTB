#ifndef otbFillOcclusionImageFilter_h
#define otbFillOcclusionImageFilter_h

/*===================================================================================================

Implementation of the Occlusion Zone Filling, Using 
Left or Right Disparity Maps and the Occlusion Mask

=====================================================================================================*/


#include <otbUnaryFunctorNeighborhoodVectorImageFilter.h>
#include <itkNumericTraits.h>
#include <itkConstNeighborhoodIterator.h>
#include <algorithm> //std::max_element


namespace otb
{

namespace Functor
{

 
/** \class FillOcclusionImageOperator

 */
template < class TInput, class TOutput >
class FillOcclusionImageOperator
{
private:


public:
  FillOcclusionImageOperator() { }
  virtual ~FillOcclusionImageOperator() { }
 
  // input(0) = Mask;
  // input(1) = disp
  
 TOutput operator() (const TInput & input)
  {	 
	  TOutput output (1 );
	  output.Fill(0);
	   
	  double temp(0.);
	
	std::vector<double> vect;
    vect.resize(input.Size(),0);
	   
	  	
				 for ( unsigned int i = 0; i < input.Size(); ++i ){	
					 vect[i] = input.GetPixel(i)[1];
				}	 
					 
					 
				for ( unsigned int i = 0; i < input.Size(); ++i ){			
							if(input.GetCenterPixel()[0] == 255 )		
										 
								temp =  input.GetCenterPixel()[1] ;
							
							else
							temp = *std::max_element(vect.begin(),vect.end())  ;             						
				 }
						
			output[0] = static_cast<typename TOutput::ValueType>(temp) ;
	
 return output;

	
  
  } // end operator ()

}; // end of functor class  FillOcclusionImageOperator

}  // end of fonctor 


/** \class FillOcclusionImageFilter
 *
 *
 * \ingroup OTBImageManipulation
 */

              
 
 
template < class TInputImage,class TOutputImage >
class ITK_EXPORT FillOcclusionImageFilter
 : public UnaryFunctorNeighborhoodVectorImageFilter<TInputImage, TOutputImage,
            Functor::FillOcclusionImageOperator<
              typename itk::ConstNeighborhoodIterator<TInputImage>,
              typename TOutputImage::PixelType > >
{
public:
  /** Standard class typedefs */
  typedef FillOcclusionImageFilter Self;
  typedef UnaryFunctorNeighborhoodVectorImageFilter< TInputImage, TOutputImage,
            Functor::FillOcclusionImageOperator<
               typename itk::ConstNeighborhoodIterator<TInputImage>,
               typename TOutputImage::PixelType > >  Superclass;
              
              
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

    /** Creation through object factory macro */
  itkTypeMacro(FillOcclusionImageFilter, ImageToImageFilter);


protected:

FillOcclusionImageFilter()
  {
  }
  ~FillOcclusionImageFilter() ITK_OVERRIDE { }

  FillOcclusionImageFilter( const Self & ); // Not implemented
  void operator=( const Self & ); // Not implemented
}; // end of class FillOcclusionImageFilter


} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#endif

#endif




