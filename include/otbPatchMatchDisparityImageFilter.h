#ifndef otbPatchMatchDisparityImageFilter_h
#define otbPatchMatchDisparityImageFilter_h

/*===================================================================================================

Compute disparit map

=====================================================================================================*/


#include <otbUnaryFunctorNeighborhoodVectorImageFilter.h>
#include <itkNumericTraits.h>
#include <itkConstNeighborhoodIterator.h>

namespace otb
{

namespace Functor
{

 
/** \class DispOperator

 */
template < class TInput, class TOutput >
class DispOperator
{
public:

typedef typename TInput::IndexType      IndexType;



public:
  DispOperator() { }
  virtual ~DispOperator() { }
 
  
 TOutput operator() (const TInput & input)
  {	 
	  TOutput output (1 );
	  output.Fill(0);
	   
	  double temp(0.);
	  IndexType index = input.GetIndex();

	 
		//~ double nx1 = input.GetCenterPixel()[0] ; 		
		//~ double ny1 = input.GetCenterPixel()[1] ;
		//~ double nz1 = input.GetCenterPixel()[2] ;
		
		 //~ double z1 = input.GetCenterPixel()[3];



		//~ double coeffs0 = -nx1 / nz1;
		//~ double coeffs1 = -ny1 / nz1;
		//~ double coeffs2 = (nx1*index[0] + ny1*index[1] + nz1*z1) / nz1 ;
	   
	   
	     //~ temp = coeffs0*index[0] + coeffs1*index[1] +coeffs2 ;
	     
	   temp = 	 input.GetCenterPixel()[0]*index[0] 
				+input.GetCenterPixel()[1]*index[1] 
				+input.GetCenterPixel()[2] ;
		
			output[0] = static_cast<typename TOutput::ValueType>(temp) ;
		
		
	
 return output;

	
  
  } // end operator ()

}; // end of functor class  DispOperator

}  // end of fonctor 


/** \class PatchMatchDisparityImageFilter
 *
 *
 * \ingroup OTBImageManipulation
 */

              
 
 
template < class TInputImage,class TOutputImage >
class ITK_EXPORT PatchMatchDisparityImageFilter
 : public UnaryFunctorNeighborhoodVectorImageFilter<TInputImage, TOutputImage,
            Functor::DispOperator<
              typename itk::ConstNeighborhoodIterator<TInputImage>,
              typename TOutputImage::PixelType > >
{
public:
  /** Standard class typedefs */
  typedef PatchMatchDisparityImageFilter Self;
  typedef UnaryFunctorNeighborhoodVectorImageFilter< TInputImage, TOutputImage,
            Functor::DispOperator<
               typename itk::ConstNeighborhoodIterator<TInputImage>,
               typename TOutputImage::PixelType > >  Superclass;
              
              
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

    /** Creation through object factory macro */
  itkTypeMacro(PatchMatchDisparityImageFilter, ImageToImageFilter);


protected:

PatchMatchDisparityImageFilter()
  {
  }
  ~PatchMatchDisparityImageFilter() ITK_OVERRIDE { }
  
void GenerateOutputInformation(void) ITK_OVERRIDE;

  PatchMatchDisparityImageFilter( const Self & ); // Not implemented
  void operator=( const Self & ); // Not implemented
}; // end of class OcclusionFilter

// surchage
template < class TInputImage,class TOutputImage >
void PatchMatchDisparityImageFilter<TInputImage, TOutputImage>
::GenerateOutputInformation(void){
 Superclass::GenerateOutputInformation();

 this->GetOutput()->SetNumberOfComponentsPerPixel(1);
 

}


} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION

#endif

#endif



