#ifndef otbMultiplyImagebandsFilter_h
#define otbMultiplyImagebandsFilter_h

/*===================================================================================================

Implementation of the multiplication between the bands of the image itself
=====================================================================================================*/


#include <itkUnaryFunctorImageFilter.h>
#include <itkNumericTraits.h>
#include <itkConstNeighborhoodIterator.h>



namespace otb
{

namespace Functor
{

 
/** \class MultiplyOperator

 */
template < class TInput, class TOutput >
class MultiplyOperator
{

public:
  MultiplyOperator() { }
  virtual ~MultiplyOperator() { }

  
  
 TOutput operator() (  TInput  input )
  {	 


    TOutput Out(9); 
    Out.Fill(0);   
        	
        Out[0] = static_cast<typename TOutput::ValueType>(input[0]*input[0]) ;
				Out[1] = static_cast<typename TOutput::ValueType>(input[0]*input[1]) ;
				Out[2] = static_cast<typename TOutput::ValueType>(input[0]*input[2]) ;
				Out[3] = Out[1];
				Out[4] = static_cast<typename TOutput::ValueType>(input[1]*input[1]) ;
				Out[5] = static_cast<typename TOutput::ValueType>(input[1]*input[2]) ;
				Out[6] = static_cast<typename TOutput::ValueType>(input[2]*input[0]) ;
				Out[7] = Out[5];
				Out[8] = static_cast<typename TOutput::ValueType>(input[2]*input[2]) ;

       // std::cout << " input[0] = " << input[0] << std::endl;
        // std::cout << " out[0] = " << Out[0] << std::endl;

   	return Out;
  } // end operator ()

}; // end of functor class  MultiplyOperator

}  // end of fonctor 


/** \class MultiplyImagebandsFilter
 *
 *
 * \ingroup OTBImageManipulation
 */
template < class TInputImage, class TOutputImage >
class ITK_EXPORT MultiplyImagebandsFilter
 : public itk::UnaryFunctorImageFilter< TInputImage,  TOutputImage,
            Functor::MultiplyOperator<
              typename TInputImage::PixelType,
              typename TOutputImage::PixelType > >
{
public:
  /** Standard class typedefs */
  typedef MultiplyImagebandsFilter Self;
  typedef itk::UnaryFunctorImageFilter< TInputImage, TOutputImage,
            Functor::MultiplyOperator<
              typename TInputImage::PixelType,
              typename TOutputImage::PixelType > >  Superclass;
              
              
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);


    /** Creation through object factory macro */
  itkTypeMacro(MultiplyImagebandsFilter, ImageToImageFilter);


protected:

MultiplyImagebandsFilter()
  {

  }
  ~MultiplyImagebandsFilter() ITK_OVERRIDE { }
void GenerateOutputInformation(void) ITK_OVERRIDE;




  MultiplyImagebandsFilter( const Self & ); // Not implemented
  void operator=( const Self & ); // Not implemented
}; // end of class MultiplyImagebandsFilter


// surchage
template < class TInputImage, class TOutputImage >
void MultiplyImagebandsFilter<TInputImage,TOutputImage>
::GenerateOutputInformation(void){
 Superclass::GenerateOutputInformation();

 this->GetOutput()->SetNumberOfComponentsPerPixel(9);

}

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION

#endif

#endif




