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
#ifndef otbPointSetSource_txx
#define otbPointSetSource_txx

#include "otbPointSetSource.h"

namespace otb
{

/**
 *
 */
template<class TOutputPointSet>
PointSetSource<TOutputPointSet>
::PointSetSource()
{
  // Create the output. We use static_cast<> here because we know the default
  // output must be of type TOutputPointSet
  OutputPointSetPointer output
    = static_cast<TOutputPointSet*>(this->MakeOutput(0).GetPointer());

  this->ProcessObject::SetNumberOfRequiredOutputs(1);
  this->ProcessObject::SetNthOutput(0, output.GetPointer());

  m_GenerateDataRegion = 0;
  m_GenerateDataNumberOfRegions = 0;
}

/**
 *
 */
template<class TOutputPointSet>
typename PointSetSource<TOutputPointSet>::DataObjectPointer
PointSetSource<TOutputPointSet>
::MakeOutput(DataObjectPointerArraySizeType)
{
  return static_cast<itk::DataObject*>(TOutputPointSet::New().GetPointer());
}

/**
 *
 */
template<class TOutputPointSet>
typename PointSetSource<TOutputPointSet>::OutputPointSetType *
PointSetSource<TOutputPointSet>
::GetOutput(void)
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return ITK_NULLPTR;
    }

  return static_cast<TOutputPointSet*>
           (this->ProcessObject::GetOutput(0));
}

/**
 *
 */
template<class TOutputPointSet>
typename PointSetSource<TOutputPointSet>::OutputPointSetType *
PointSetSource<TOutputPointSet>
::GetOutput(DataObjectPointerArraySizeType idx)
{
  return static_cast<TOutputPointSet*>
           (this->ProcessObject::GetOutput(idx));
}

/**
 *
 */
template<class TOutputPointSet>
void
PointSetSource<TOutputPointSet>
::SetOutput(OutputPointSetType *output)
{
  itkWarningMacro(
    <<
    "SetOutput(): This method is slated to be removed from ITK.  Please use GraftOutput() in possible combination with DisconnectPipeline() instead.");
  this->ProcessObject::SetNthOutput(0, output);
}

/**
 *
 */
template<class TOutputPointSet>
void
PointSetSource<TOutputPointSet>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
}

/**
 *
 */
template<class TOutputPointSet>
void
PointSetSource<TOutputPointSet>
::GraftOutput(itk::DataObject *graft)
{
  this->GraftNthOutput(0, graft);
}

/**
 *
 */
template<class TOutputPointSet>
void
PointSetSource<TOutputPointSet>
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

  // Call Graft on the PointSet in order to copy meta-information, and containers.
  output->Graft(graft);
}

/**
 *
 */
template<class TOutputPointSet>
void
PointSetSource<TOutputPointSet>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
