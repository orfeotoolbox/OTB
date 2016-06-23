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
#ifndef otbVectorDataToSpecificDescriptionFilterBase_txx
#define otbVectorDataToSpecificDescriptionFilterBase_txx

#include "otbVectorDataToSpecificDescriptionFilterBase.h"

namespace otb
{

// Constructor
template <class TVectorData>
VectorDataToSpecificDescriptionFilterBase<TVectorData>
::VectorDataToSpecificDescriptionFilterBase()
{
  this->SetNumberOfRequiredInputs(1);
}

template <class TVectorData>
void
VectorDataToSpecificDescriptionFilterBase<TVectorData>
::AddSupport(const DataObject * support, unsigned int idx)
{
  // Process object is not const-correct so the const_cast is required here
  // input(0) is dedicated to the input vector data, support #i is
  // stored in input(i+1)
  this->itk::ProcessObject::SetNthInput(idx+1,
                                        const_cast<DataObject *>(support));
}

template <class TVectorData>
const typename VectorDataToSpecificDescriptionFilterBase<TVectorData>
::DataObject *
VectorDataToSpecificDescriptionFilterBase<TVectorData>
::GetSupport(unsigned int idx)
{
  if (this->GetNumberOfInputs() < idx+2)
    {
    return ITK_NULLPTR;
    }

  return static_cast<const DataObject *>
           (this->itk::ProcessObject::GetInput(idx+1));
}

// PrintSelf Method
template <class TVectorData>
void
VectorDataToSpecificDescriptionFilterBase<TVectorData>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os<<indent<<"Number of Support: "<<(int)(this->GetNumberOfInputs())-1<<std::endl;
}

} // end namespace otb

#endif
