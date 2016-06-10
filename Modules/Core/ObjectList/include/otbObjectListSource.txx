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
#ifndef __otbObjectListSource_txx
#define __otbObjectListSource_txx

#include "otbObjectListSource.h"
#include "itkProgressReporter.h"
#include "otbMath.h"

namespace otb
{

/**
   * Constructor
 */
template <class TOutputList>
ObjectListSource<TOutputList>
::ObjectListSource()
{
  // Create the output. We use static_cast<> here because we know the default
  // output must be of type TOutputImage
  typename TOutputList::Pointer output
    = static_cast<TOutputList*>(this->MakeOutput(0).GetPointer());
  this->Superclass::SetNumberOfRequiredOutputs(1);
  this->Superclass::SetNthOutput(0, output.GetPointer());

}

/**
 *
 */
template<class TOutputList>
typename ObjectListSource<TOutputList>::DataObjectPointer
ObjectListSource<TOutputList>
::MakeOutput(DataObjectPointerArraySizeType)
{
  return static_cast<itk::DataObject*>(TOutputList::New().GetPointer());
}

/**
 *
 */
template <class TOutputList>
typename ObjectListSource<TOutputList>::OutputListType *
ObjectListSource<TOutputList>
::GetOutput()
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }

  return static_cast<TOutputList*>
           (this->Superclass::GetOutput(0));
}

/**
 *
 */
template <class TOutputList>
typename ObjectListSource<TOutputList>::OutputListType *
ObjectListSource<TOutputList>
::GetOutput(DataObjectPointerArraySizeType idx)
{
  return static_cast<TOutputList*>
           (this->Superclass::GetOutput(idx));
}

/**
 *
 */
template<class TOutputList>
void
ObjectListSource<TOutputList>
::GraftOutput(itk::DataObject *graft)
{
  this->GraftNthOutput(0, graft);
}

/**
 *
 */
template<class TOutputList>
void
ObjectListSource<TOutputList>
::GraftNthOutput(DataObjectPointerArraySizeType idx, itk::DataObject *graft)
{
  if (idx >= this->GetNumberOfOutputs())
    {
    itkExceptionMacro(<< "Requested to graft output " << idx <<
                      " but this filter only has " << this->GetNumberOfOutputs() << " Outputs.");
    }

  if (!graft)
    {
    itkExceptionMacro(<< "Requested to graft output that is a NULL pointer");
    }

  itk::DataObject * output = this->GetOutput(idx);

  // Call GraftImage to copy meta-information, regions, and the pixel container
  output->Graft(graft);
}

//----------------------------------------------------------------------------

template <class TOutputList>
void
ObjectListSource<TOutputList>
::AllocateOutputs()
{
  OutputListPointer outputPtr;

  // Allocate the output memory
  for (unsigned int i = 0; i < this->GetNumberOfOutputs(); ++i)
    {
    outputPtr = this->GetOutput(i);
    outputPtr->Clear();
    }
}

/**
   * GenerateData
 */
template <class TOutputList>
void
ObjectListSource<TOutputList>
::GenerateData(void)
{
  itkExceptionMacro("subclass should ITK_OVERRIDE this method!!!");
}

/**
   * PrintSelf Method
 */
template <class TOutputList>
void
ObjectListSource<TOutputList>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace otb

#endif
