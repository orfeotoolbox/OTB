#ifndef otbMultiplyImageAndCostImageFilter_h
#define otbMultiplyImageAndCostImageFilter_h

/*===================================================================================================
Implementation of the multiplication of each band of the input image with the cost volume


=====================================================================================================*/


#include <itkBinaryFunctorImageFilter.h>
#include <itkNumericTraits.h>
#include <itkConstNeighborhoodIterator.h>



namespace otb
{

namespace Functor
{

 
/** \class MOperator

 */
template < class TInput,class TInputCost, class TOutput >
class MOperator
{

public:
  MOperator() { }
  virtual ~MOperator() { }

  
  
 TOutput operator() (  TInput  input, TInputCost  Cost )
  {	 

TOutput Out(3);
 Out.Fill(0);
        
        	
        		Out[0] = static_cast<typename TOutput::ValueType>(input[0]*Cost[0]) ;
				Out[1] = static_cast<typename TOutput::ValueType>(input[1]*Cost[0]) ;
				Out[2] = static_cast<typename TOutput::ValueType>(input[2]*Cost[0]) ;
		

   	return Out;
  } // end operator ()

}; // end of functor class  MOperator

}  // end of fonctor 


/** \class MultiplyImageAndCostImageFilter
 *
 *
 * \ingroup OTBImageManipulation
 */
template < class TInputImage,class TInputCostImage, class TOutputImage >
class ITK_EXPORT MultiplyImageAndCostImageFilter
 : public itk::BinaryFunctorImageFilter< TInputImage, TInputCostImage, TOutputImage,
            Functor::MOperator<
              typename TInputImage::PixelType,typename TInputCostImage::PixelType,
              typename TOutputImage::PixelType > >
{
public:
  /** Standard class typedefs */
  typedef MultiplyImageAndCostImageFilter Self;
  typedef itk::BinaryFunctorImageFilter< TInputImage,TInputCostImage, TOutputImage,
            Functor::MOperator<
              typename TInputImage::PixelType,typename TInputCostImage::PixelType,
              typename TOutputImage::PixelType > >  Superclass;
              
              
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);


    /** Creation through object factory macro */
  itkTypeMacro(MultiplyImageAndCostImageFilter, ImageToImageFilter);


protected:

MultiplyImageAndCostImageFilter()
  {
 //this->SetNumberOfThreads(1);
  }
  ~MultiplyImageAndCostImageFilter() ITK_OVERRIDE { }
void GenerateOutputInformation(void) ITK_OVERRIDE;




  MultiplyImageAndCostImageFilter( const Self & ); // Not implemented
  void operator=( const Self & ); // Not implemented
}; // end of class MultiplyImageAndCostImageFilter


// Overload
template < class TInputImage, class TInputCostImage, class TOutputImage >
void MultiplyImageAndCostImageFilter<TInputImage,TInputCostImage, TOutputImage>
::GenerateOutputInformation(void){
 Superclass::GenerateOutputInformation();

 this->GetOutput()->SetNumberOfComponentsPerPixel(3);
 

}

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION

#endif

#endif




