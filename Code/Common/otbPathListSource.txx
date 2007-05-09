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

#ifndef _otbPathListSource_txx
#define _otbPathListSource_txx
#include "otbPathListSource.h"

namespace otb
{

/**
 * Constructor
 */
template<class TOutputPath>
PathListSource<TOutputPath>
::PathListSource()
{
  OutputPathListPointerType output
    = static_cast<OutputPathListType*>(this->MakeOutput(0).GetPointer()); 

  this->Superclass::SetNumberOfRequiredOutputs(1);
  this->Superclass::SetNthOutput(0,output.GetPointer());
}
/**
 * 
 */
template<class TOutputPath>
void
PathListSource<TOutputPath>
::GraftOutput(OutputPathListType *graft)
{
  this->GraftNthOutput(0, graft);
}

/**
 * 
 */
template<class TOutputPath>
void
PathListSource<TOutputPath>
::GraftNthOutput(unsigned int idx, OutputPathListType *graft)
{
/*  if (idx < this->GetNumberOfOutputs())
    {
    OutputPathListType * output = this->GetOutput(idx);

    if (output && graft)
      {
      // Paths do not have a generic pointer to their bulk data
      itkWarningMacro( << "Warning:  GraftNthOutput() is broken" );
      }
    }
*/
  if ( idx >= this->GetNumberOfOutputs() )
    {
    itkExceptionMacro(<<"Requested to graft output " << idx <<
        " but this filter only has " << this->GetNumberOfOutputs() << " Outputs.");
    }

  if ( !graft )
    {
    itkExceptionMacro(<<"Requested to graft output that is a NULL pointer" );
    }

  OutputPathListType * output = this->GetOutput( idx );

  // Call Graft on the Mesh in order to copy meta-information, and containers.
//  output->Graft( graft );
  output = graft;
  
}

/**
 *
 */
template<class TOutputPath>
typename PathListSource<TOutputPath>::DataObjectPointer
PathListSource<TOutputPath>
::MakeOutput(unsigned int)
{
  return (static_cast<itk::DataObject*>(OutputPathListType::New()));
}

/**
 *
 */
template<class TOutputPath>
typename PathListSource<TOutputPath>::OutputPathListType *
PathListSource<TOutputPath>
::GetOutput(void)
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  
  return static_cast<OutputPathListType*>
                     (this->Superclass::GetOutput(0));
}
  
/**
 *
 */
template<class TOutputPath>
typename PathListSource<TOutputPath>::OutputPathListType *
PathListSource<TOutputPath>
::GetOutput(unsigned int idx)
{
  return static_cast<OutputPathListType*>
                     (this->Superclass::GetOutput(idx));
}

template<class TOutputPath>
void 
PathListSource<TOutputPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

} // end namespace otb

#endif
