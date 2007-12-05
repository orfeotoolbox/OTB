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
#include "otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms.h"

namespace otb
{
/** 
 * Constructor.
 */

AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
::AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms()
{
  this->ProcessObject::SetNumberOfRequiredInputs(1);
  this->ProcessObject::SetNumberOfRequiredOutputs(1);
  // Create the output. We use static_cast<> here because we know the default
  // output must be of type TOutputPointSet
  AtmosphericRadiativeTermsPointer output
    = static_cast<AtmosphericRadiativeTermsType*>(this->MakeOutput(0).GetPointer()); 

  this->ProcessObject::SetNthOutput( 0, output.GetPointer() );

}
/**
 *
 */
AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms::DataObjectPointer
AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
::MakeOutput(unsigned int)
{
  return static_cast<itk::DataObject*>(AtmosphericRadiativeTermsType::New().GetPointer());
}

/**
 * 
 */
void
AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
::GraftOutput(itk::DataObject *graft)
{
  this->GraftNthOutput(0, graft);
}


/**
 * 
 */
void
AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
::GraftNthOutput(unsigned int idx, itk::DataObject *graft)
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

  itk::DataObject * output = this->GetOutput( idx );

  // Call Graft on the PointSet in order to copy meta-information, and containers.
  output->Graft( graft );
}

/**
 * Get the output data
 * \return The data produced.
 */

AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms::AtmosphericRadiativeTermsType *
AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
::GetOutput(void)
{
  if(this->GetNumberOfOutputs()<1)
    {
      return 0;
    }
  return static_cast<AtmosphericRadiativeTermsType *> (this->ProcessObject::GetOutput(0));
}

/**
 *
 */
AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms::AtmosphericRadiativeTermsType *
AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
::GetOutput(unsigned int idx)
{
  return static_cast<AtmosphericRadiativeTermsType*>
    (this->itk::ProcessObject::GetOutput(idx));
}


void
AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
::SetInput(const AtmosphericCorrectionParametersType *object)
{
  // A single input image
  this->itk::ProcessObject::SetNthInput(0,const_cast<AtmosphericCorrectionParametersType*>(object));
}

AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms::AtmosphericCorrectionParametersType *
AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
::GetInput(void)
{
  // If there is no input 
  if (this->GetNumberOfInputs()!=1)
    {
    // exit
    return 0;
    }
  // else return the first input
  return static_cast<AtmosphericCorrectionParametersType * >
    (this->itk::ProcessObject::GetInput(0) );
}


/**
 * PrintSelf Method
 */
void
AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

}// end namespace otb
