#ifndef otbGuidedImageFilter_h
#define otbGuidedImageFilter_h

/*===================================================================================================

Implementation of the last step of the guided Filter
eq 15 mean(ai).I(i)+mean(bk)
 b += aR.boxFilter(r)*im1R+aG.boxFilter(r)*im1G+aB.boxFilter(r)*im1B;	

=====================================================================================================*/


#include <itkBinaryFunctorImageFilter.h>
#include <itkNumericTraits.h>
#include <itkConstNeighborhoodIterator.h>



namespace otb
{

namespace Functor
{

 
/** \class GuidedOperator

 */
template < class TInput,class TInputCoeff, class TOutput >
class GuidedOperator
{

public:
  GuidedOperator() { }
  virtual ~GuidedOperator() { }

  
  
 TOutput operator() (  TInput  input, TInputCoeff  Coeff )
  {	 

	TOutput OutCost(1);
     OutCost.Fill(0);
 
     
     OutCost[0]	 = static_cast<typename TOutput::ValueType>( Coeff[0]*input[0]+Coeff[1]*input[1]+Coeff[2]*input[2] +Coeff[3] );
         //eq 15 mean(ai).I(i)+mean(bk)
        // b += aR.boxFilter(r)*im1R+aG.boxFilter(r)*im1G+aB.boxFilter(r)*im1B;	


   	return OutCost;
  } // end operator ()

}; // end of functor class  GuidedOperator

}  // end of fonctor 


/** \class otbGuidedImageFilter
 *
 *
 * \ingroup OTBImageManipulation
 */
template < class TInputImage,class TInputCoeffImage, class TOutputImage >
class ITK_EXPORT GuidedImageFilter
 : public itk::BinaryFunctorImageFilter< TInputImage, TInputCoeffImage, TOutputImage,
            Functor::GuidedOperator<
              typename TInputImage::PixelType,typename TInputCoeffImage::PixelType,
              typename TOutputImage::PixelType > >
{
public:
  /** Standard class typedefs */
  typedef GuidedImageFilter Self;
  typedef itk::BinaryFunctorImageFilter< TInputImage,TInputCoeffImage, TOutputImage,
            Functor::GuidedOperator<
              typename TInputImage::PixelType,typename TInputCoeffImage::PixelType,
              typename TOutputImage::PixelType > >  Superclass;
              
              
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);


    /** Creation through object factory macro */
  itkTypeMacro(GuidedImageFilter, ImageToImageFilter);


protected:

GuidedImageFilter()
  {

  }
  ~GuidedImageFilter() ITK_OVERRIDE { }
void GenerateOutputInformation(void) ITK_OVERRIDE;




  GuidedImageFilter( const Self & ); // Not implemented
  void operator=( const Self & ); // Not implemented
}; // end of class GuidedImageFilter


// surchage
template < class TInputImage, class TInputCostImage, class TOutputImage >
void GuidedImageFilter<TInputImage,TInputCostImage, TOutputImage>
::GenerateOutputInformation(void){
 Superclass::GenerateOutputInformation();

 this->GetOutput()->SetNumberOfComponentsPerPixel(1);
 

}

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION

#endif

#endif




