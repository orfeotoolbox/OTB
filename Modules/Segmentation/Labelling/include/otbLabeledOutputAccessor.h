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
#ifndef __otbLabeledOutputAccessor_h
#define __otbLabeledOutputAccessor_h

#include "itkMacro.h"

namespace otb
{

/**
 * \class LabeledOutputAccessor
 * \brief Accessor to the index of the labeled output image of the Template Filter.
 *
 * \ingroup OTBLabelling
 */
template <class TFilter>
class LabeledOutputAccessor
{
   public:
      typedef typename TFilter::OutputImageType    LabelImageType;
      itkStaticConstMacro(LabeledOutputIndex, unsigned int, 0);
};

}

#endif
