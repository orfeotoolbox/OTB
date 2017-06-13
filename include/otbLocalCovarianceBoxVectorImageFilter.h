#ifndef otbLocalCovarianceBoxVectorImageFilters_h
#define otbLocalCovarianceBoxVectorImageFilters_h

/*===================================================================================================

Implémentation of local variance and covariance using Summed areas 

=====================================================================================================*/


#include "otbUnaryFunctorNeighborhoodVectorImageFilter.h"
#include "otbBinaryFunctorNeighborhoodVectorImageFilter.h"
#include <itkNumericTraits.h>
#include <itkConstNeighborhoodIterator.h>

namespace otb
{

namespace Functor
{
 
/** \class LocalCovarianceBoxOperator
 * \ingroup 
 */
template < class TInputMutilply, class TInputMean,class TOutput >
class LocalCovarianceBoxOperator
{
public:
  LocalCovarianceBoxOperator() { }
  virtual ~LocalCovarianceBoxOperator() { }
 
    
 TOutput operator() ( const TInputMutilply & inputMul, const TInputMean & inputMean )
  {	TOutput OutCov(9); 
	  OutCov.Fill(0);   
        	
        		OutCov[0] = static_cast<typename TOutput::ValueType>(inputMul[0]- inputMean[0]*inputMean[0]) ;
				OutCov[1] = static_cast<typename TOutput::ValueType>(inputMul[1]- inputMean[0]*inputMean[1]) ;
				OutCov[2] = static_cast<typename TOutput::ValueType>(inputMul[2]- inputMean[0]*inputMean[2]) ;
				OutCov[3] = OutCov[1] ; //static_cast<typename TOutput::ValueType>(inputMul[3]- inputMean[1]*inputMean[0]) ;
				OutCov[4] = static_cast<typename TOutput::ValueType>(inputMul[4]- inputMean[1]*inputMean[1]) ;
				OutCov[5] = static_cast<typename TOutput::ValueType>(inputMul[5]- inputMean[1]*inputMean[2]) ;
				OutCov[6] = OutCov[2];//static_cast<typename TOutput::ValueType>(inputMul[6]- inputMean[2]*inputMean[0]) ;
				OutCov[7] = OutCov[5];//static_cast<typename TOutput::ValueType>(inputMul[7]- inputMean[2]*inputMean[1]) ;
				OutCov[8] = static_cast<typename TOutput::ValueType>(inputMul[8]- inputMean[2]*inputMean[2]) ;

   	return OutCov;
  }

}; // end of functor class  LocalCovarianceBoxOperator

}  // end of fonctor 


/** \class LocalCovarianceBoxVectorImageFilter
 *
 *
 * \ingroup 
 */
template < class TInputImageMultiply,class TInputImageMean, class TOutputImage >
class ITK_EXPORT LocalCovarianceBoxVectorImageFilter
 : public itk::BinaryFunctorImageFilter< TInputImageMultiply, TInputImageMean, TOutputImage,
            Functor::LocalCovarianceBoxOperator<
              typename TInputImageMultiply::PixelType,
              typename TInputImageMean::PixelType,typename TOutputImage::PixelType > >
{
public:
  /** Standard class typedefs */
  typedef LocalCovarianceBoxVectorImageFilter Self;
  typedef itk::BinaryFunctorImageFilter< TInputImageMultiply, TInputImageMean, TOutputImage,
            Functor::LocalCovarianceBoxOperator<
              typename TInputImageMultiply::PixelType,
              typename TInputImageMean::PixelType,typename TOutputImage::PixelType > >  Superclass;
              
              
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

 
    /** Creation through object factory macro */
  itkTypeMacro(LocalCovarianceBoxVectorImageFilter, ImageToImageFilter);


protected:

LocalCovarianceBoxVectorImageFilter()
  {

  }
  ~LocalCovarianceBoxVectorImageFilter() ITK_OVERRIDE { }
void GenerateOutputInformation(void) ITK_OVERRIDE;



  LocalCovarianceBoxVectorImageFilter( const Self & ); // Not implemented
  void operator=( const Self & ); // Not implemented
}; // end of class LocalCovarianceBoxVectorImageFilter




/*===========================Overload of GenerateOutputInformation ===========================*/
template < class TInputImageMultiply,class TInputImageMean, class TOutputImage >
void LocalCovarianceBoxVectorImageFilter < TInputImageMultiply, TInputImageMean, TOutputImage >
::GenerateOutputInformation(void){
 Superclass::GenerateOutputInformation();
 
 this->GetOutput()->SetNumberOfComponentsPerPixel(9);

}
/*==============================================================*/




} // end of namespace otb

#endif // LocalCovarianceBoxVectorImageFilter_h




