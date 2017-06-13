#ifndef otbMinFilter_h
#define otbMinFilter_h

/*===================================================================================================

implémentation du minimum entre deux pixels de deux images et calcul de la carte de disparité 

=====================================================================================================*/


#include <itkBinaryFunctorImageFilter.h>
#include <itkNumericTraits.h>
#include <itkConstNeighborhoodIterator.h>

#include "otbCostVolumeImageFilter.h"

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
  int SetDisp(int disp){ m_disp = disp; return m_disp;} //-disp pour avoir l ameme norme avec les resulats de ipol
  
  
 TOutput operator() (TInput  input,const TInputCost & Cost )
  {	 

	double temp0(0.);
	double temp1(0.);
    TOutput OutDisp(2);
   
    // 1st band = inDisp
     // 2nd band = min_Cost
    
		 		if((double)Cost[0] < (double)input[1]){
		 		
		 		temp0 = m_disp;
		 		temp1 = Cost[0]; 
		 		//std::cout << " temp1= "<< temp1; 
				}
				
				else{
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
  
  //typedef typename InputImageType::RegionType               RegionType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Creation through object factory macro */
 // itkTypeMacro(MinFilter, BinaryFunctorImageFilter);
    /** Creation through object factory macro */
  itkTypeMacro(MinFilter, ImageToImageFilter);


protected:

MinFilter()
  {

  }
  ~MinFilter() ITK_OVERRIDE { }
void GenerateOutputInformation(void) ITK_OVERRIDE;



  MinFilter( const Self & ); // Not implemented
  void operator=( const Self & ); // Not implemented
}; // end of class MinFilter






} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMinFilter.txx"
#endif

#endif




