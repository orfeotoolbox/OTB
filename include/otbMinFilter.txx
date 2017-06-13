#ifndef otbMinFilter_txx
#define otbMinFilter_txx


#include "otbMinFilter.h"

#include "otbCostVolumeImageFilter.h"
#include "itkProgressReporter.h"
#include "itkConstantBoundaryCondition.h"
#include "itkConstNeighborhoodIterator.h"


#include "itkImageToImageFilter.h" 
#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

namespace otb
{




template < class TInputImage,class TInputImageCost, class TOutputImage >
void MinFilter<TInputImage,TInputImageCost,TOutputImage>
::GenerateOutputInformation(void){
 Superclass::GenerateOutputInformation();

 this->GetOutput()->SetNumberOfComponentsPerPixel(2);
 

}


} // end of otb
#endif
