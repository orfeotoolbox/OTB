#ifndef otbMinFilter_txx
#define otbMinFilter_txx


#include "otbMinFilter.h"

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
