#ifndef otbCovarianceImageCostVolumeBoxFilter_h
#define otbCovarianceImageCostVolumeBoxFilter_h

/*===================================================================================================

InputMutilply: contains the multiplication of the input image of 3 bands and the cost volume
InputMean: Contains The local mean of the input image
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
 * \ingroup OTBImageManipulation
 */
template < class TInputMutilply, class TInputMean,class TOutput >
class CovarianceBoxOperator
{
public:
  CovarianceBoxOperator() { }
  virtual ~CovarianceBoxOperator() { }
 
    
 TOutput operator() ( const TInputMutilply & inputMul, const TInputMean & inputMean )
  {	TOutput OutCov(3); 
	  OutCov.Fill(0);   
        	
        		OutCov[0] = static_cast<typename TOutput::ValueType>(inputMul[0]- inputMean[0]*inputMean[3]) ; //Meam(R*Cost)-mean(R) *mean(Cost)
				OutCov[1] = static_cast<typename TOutput::ValueType>(inputMul[1]- inputMean[1]*inputMean[3]) ;
				OutCov[2] = static_cast<typename TOutput::ValueType>(inputMul[2]- inputMean[2]*inputMean[3]) ;
			

   	return OutCov;
  }

}; // end of functor class  CovarianceBoxOperator

}  // end of fonctor 







/** \class CovarianceImageCostVolumeBoxFilter
 *
 *
 * \ingroup OTBImageManipulation
 */
template < class TInputImageMultiply,class TInputImageMean, class TOutputImage >
class ITK_EXPORT CovarianceImageCostVolumeBoxFilter
 : public itk::BinaryFunctorImageFilter< TInputImageMultiply, TInputImageMean, TOutputImage,
            Functor::CovarianceBoxOperator<
              typename TInputImageMultiply::PixelType,
              typename TInputImageMean::PixelType,typename TOutputImage::PixelType > >
{
public:
  /** Standard class typedefs */
  typedef CovarianceImageCostVolumeBoxFilter Self;
  typedef itk::BinaryFunctorImageFilter< TInputImageMultiply, TInputImageMean, TOutputImage,
            Functor::CovarianceBoxOperator<
              typename TInputImageMultiply::PixelType,
              typename TInputImageMean::PixelType,typename TOutputImage::PixelType > >  Superclass;
              
              
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

 
    /** Creation through object factory macro */
  itkTypeMacro(CovarianceImageCostVolumeBoxFilter, ImageToImageFilter);


protected:

CovarianceImageCostVolumeBoxFilter()
  {
  }
  ~CovarianceImageCostVolumeBoxFilter() ITK_OVERRIDE { }
void GenerateOutputInformation(void) ITK_OVERRIDE;



  CovarianceImageCostVolumeBoxFilter( const Self & ); // Not implemented
  void operator=( const Self & ); // Not implemented
}; // end of class CovarianceImageCostVolumeBoxFilter




/*===========================Overload of GenerateOutputInformation ===========================*/
template < class TInputImageMultiply,class TInputImageMean, class TOutputImage >
void CovarianceImageCostVolumeBoxFilter< TInputImageMultiply, TInputImageMean, TOutputImage >
::GenerateOutputInformation(void){
 Superclass::GenerateOutputInformation();
 
 this->GetOutput()->SetNumberOfComponentsPerPixel(3);

}
/*==============================================================*/


} // end of namespace otb

#endif // otbCovarianceImageCostVolumeBoxFilter_h


