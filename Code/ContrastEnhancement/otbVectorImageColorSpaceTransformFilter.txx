/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbVectorImageColorSpaceTransformFilter_txx
#define __otbVectorImageColorSpaceTransformFilter_txx


#include "otbVectorImageColorSpaceTransformFilter.h"

namespace otb
{

template <class TInput, class TOutput, class TFunctor>
void
VectorImageColorSpaceTransformFilter<TInput, TOutput, TFunctor>
::GenerateOutputInformation()
{

  Superclass::GenerateOutputInformation();
  this->GetOutput()->SetNumberOfComponentsPerPixel(3);

}

template <class TInput, class TOutput, class TFunctor>
void
VectorImageColorSpaceTransformFilter<TInput,TOutput, TFunctor>
::BeforeThreadedGenerateData()
{

//  std::cerr << "before thdata end\n";

}

} // end namespace otb
#endif
