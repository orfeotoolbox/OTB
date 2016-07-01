/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef otbSpatialObjectSource_txx
#define otbSpatialObjectSource_txx

#include "otbSpatialObjectSource.h"

namespace otb
{
/**
 * Constructor.
 */
template <class TSpatialObject>
SpatialObjectSource<TSpatialObject>
::SpatialObjectSource()
{
  this->Superclass::SetNumberOfRequiredOutputs(1);
  this->Superclass::SetNthOutput(0, TSpatialObject::New().GetPointer());
}
/**
 * Get the output image list
 * \return The image list produced.
 */
template <class TSpatialObject>
typename SpatialObjectSource<TSpatialObject>::SpatialObjectType *
SpatialObjectSource<TSpatialObject>
::GetOutput(void)
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return ITK_NULLPTR;
    }
  return static_cast<SpatialObjectType *> (this->ProcessObject::GetOutput(0));
}
/**
 * PrintSelf Method
 */
template<class TSpatialObject>
void
SpatialObjectSource<TSpatialObject>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace otb
#endif
