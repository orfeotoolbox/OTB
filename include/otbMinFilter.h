#ifndef otbMinFilter_h
#define otbMinFilter_h

/*===================================================================================================

Implementation of the minimum between two pixels of two images and calculation of the disparity map

=====================================================================================================*/


#include <itkBinaryFunctorImageFilter.h>

namespace otb
{

namespace Functor
{

 
/** \class MinImageOperator

 */
template < class TInput,class TInputCost, class TOutput >
class MinImageOperator
{
private:
int m_disp;

public:
  MinImageOperator() { }
  virtual ~MinImageOperator() { }
  int SetDisp(int disp){ m_disp = disp; return m_disp;} //-disp To have the same standard with the results of ipol
  
  
 TOutput operator() (TInput  input,const TInputCost & Cost )
  {	 

	double temp0(0.);
	double temp1(0.);
  TOutput OutDisp(2);
   
  // 1st band = inDisp
  // 2nd band = min_Cost
    
	if( (double)Cost[0] < (double)input[1] )
    {
		temp0 = m_disp;
		temp1 = Cost[0]; 
		//std::cout << " temp1= "<< temp1; 
		}		
	else
    {
		temp0 = input[0] ;
		temp1 = input[1];
		}

    OutDisp[0] = static_cast<typename TOutput::ValueType>(temp0) ;
		OutDisp[1] = static_cast<typename TOutput::ValueType>(temp1) ;
  
   	return OutDisp;
  } // end operator ()

}; // end of functor class  MinImageOperator

}  // end of fonctor 


/** \class MinFilter
 *
 *
 * \ingroup OTBImageManipulation
 */
template < class TInputImage,class TInputImageCost, class TOutputImage >
class ITK_EXPORT MinFilter
 : public itk::BinaryFunctorImageFilter< TInputImage, TInputImageCost, TOutputImage,
            Functor::MinImageOperator<
              typename TInputImage::PixelType,
              typename TInputImageCost::PixelType,typename TOutputImage::PixelType > >
{
public:
  /** Standard class typedefs */
  typedef MinFilter Self;
  typedef itk::BinaryFunctorImageFilter< TInputImage, TInputImageCost, TOutputImage,
            Functor::MinImageOperator<
              typename TInputImage::PixelType,
              typename TInputImageCost::PixelType,typename TOutputImage::PixelType > >  Superclass;
              
              
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

    /** Creation through object factory macro */
  itkTypeMacro(MinFilter, ImageToImageFilter);


protected:

MinFilter()
  {

  }
  ~MinFilter() override { }
void GenerateOutputInformation(void) override;



  MinFilter( const Self & ) = delete ;
  void operator=( const Self & ) = delete ;
}; // end of class MinFilter






} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMinFilter.txx"
#endif

#endif




