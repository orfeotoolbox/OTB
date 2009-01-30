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
#ifndef __otbTreeSource_txx
#define __otbTreeSource_txx

#include "otbTreeSource.h"

namespace otb
{

/**
 *
 */
template<class TOutputTree>
TreeSource<TOutputTree>
::TreeSource()
{
  // Create the output. We use static_cast<> here because we know the default
  // output must be of type TOutputMesh
  OutputTreePointer output
  = static_cast<TOutputTree*>(this->MakeOutput(0).GetPointer());

  this->itk::ProcessObject::SetNumberOfRequiredOutputs(1);
  this->itk::ProcessObject::SetNthOutput( 0, output.GetPointer() );

  m_GenerateDataRegion = 0;
  m_GenerateDataNumberOfRegions = 0;
}

/**
 *
 */
template<class TOutputTree>
typename TreeSource<TOutputTree>::DataObjectPointer
TreeSource<TOutputTree>
::MakeOutput(unsigned int)
{
  return static_cast<DataObject*>(TOutputTree::New().GetPointer());
}

/**
 *
 */
template<class TOutputTree>
typename TreeSource<TOutputTree>::OutputTreeType *
TreeSource<TOutputTree>
::GetOutput(void)
{
  if (this->GetNumberOfOutputs() < 1)
  {
    return 0;
  }

  return static_cast<TOutputTree*>
         (this->itk::ProcessObject::GetOutput(0));
}


/**
 *
 */
template<class TOutputTree>
typename TreeSource<TOutputTree>::OutputTreeType *
TreeSource<TOutputTree>
::GetOutput(unsigned int idx)
{
  return static_cast<TOutputTree*>
         (this->itk::ProcessObject::GetOutput(idx));
}


/**
 *
 */
template<class TOutputTree>
void
TreeSource<TOutputTree>
::SetOutput(TOutputTree *output)
{
  itkWarningMacro(<< "SetOutput(): This method is slated to be removed from ITK.  Please use GraftOutput() in possible combination with DisconnectPipeline() instead." );
  this->itk::ProcessObject::SetNthOutput(0, output);
}


/**
 *
 */
template<class TOutputTree>
void
TreeSource<TOutputTree>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
}


/**
 *
 */
template<class TOutputTree>
void
TreeSource<TOutputTree>
::GraftOutput(DataObject *graft)
{
  this->GraftNthOutput(0, graft);
}


/**
 *
 */
template<class TOutputTree>
void
TreeSource<TOutputTree>
::GraftNthOutput(unsigned int idx, DataObject *graft)
{
  if ( idx >= this->GetNumberOfOutputs() )
  {
    itkExceptionMacro(<<"Requested to graft output " << idx <<
                      " but this filter only has " << this->GetNumberOfOutputs() << " Outputs.");
  }

  if ( !graft )
  {
    itkExceptionMacro(<<"Requested to graft output that is a NULL pointer" );
  }

  DataObject * output = this->GetOutput( idx );

  // Call Graft on the Mesh in order to copy meta-information, and containers.
  output->Graft( graft );
}


/**
 *
 */
template<class TOutputTree>
void
TreeSource<TOutputTree>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

} // end namespace otb

#endif
