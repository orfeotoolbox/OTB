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

#ifndef __otbMIRegistrationFilter_txx
#define __otbMIRegistrationFilter_txx
#include "otbMIRegistrationFilter.h"

namespace otb {

/*
 * Default constructor
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
MIRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::MIRegistrationFilter()
{
 
  typename MIRegistrationFunctionType::Pointer drfp;
  drfp = MIRegistrationFunctionType::New();

  drfp->SetDeformationField( this->GetDeformationField() );
  
  this->SetDifferenceFunction( static_cast<FiniteDifferenceFunctionType *>(
                                 drfp.GetPointer() ) );

}


template <class TFixedImage, class TMovingImage, class TDeformationField>
void
MIRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{ 
  Superclass::PrintSelf( os, indent );
   os << indent << "MI Radius: " <<
    this->GetMIRadius() << std::endl;
}


/*
 * Set the function state values before each iteration
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
MIRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::InitializeIteration()
{
 
  // call the superclass  implementation
  Superclass::InitializeIteration();

  // set the gradient selection flag
  MIRegistrationFunctionType *drfp = 
    dynamic_cast<MIRegistrationFunctionType *>
      (this->GetDifferenceFunction().GetPointer());
 
  if( !drfp )
   {
   itkExceptionMacro( << 
     "Could not cast difference function to MIRegistrationFunction" );
   }
 
  /*
   * Smooth the deformation field
   */
  if ( this->GetSmoothDeformationField() )
    {
    this->SmoothDeformationField();
    }

}


/*
 * Get the metric value from the difference function
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
double
MIRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::GetMetric() const
{
 
  MIRegistrationFunctionType *drfp = 
    dynamic_cast<MIRegistrationFunctionType *>
      (this->GetDifferenceFunction().GetPointer());
 
  if( !drfp )
   {
   itkExceptionMacro( << 
     "Could not cast difference function to MIRegistrationFunction" );
   }
   
  return drfp->GetEnergy();

}


/*
 * 
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
typename MIRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>::RadiusType
MIRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::GetMIRadius() const
{
 
  MIRegistrationFunctionType *drfp = 
    dynamic_cast<MIRegistrationFunctionType *>
      (this->GetDifferenceFunction().GetPointer());
 
  if( !drfp )
   {
   itkExceptionMacro( << 
     "Could not cast difference function to MIRegistrationFunction" );
   }
   
  return drfp->GetRadius();

}

/*
 * 
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
MIRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::SetMIRadius(RadiusType radius) 
{
 
  MIRegistrationFunctionType *drfp = 
    dynamic_cast<MIRegistrationFunctionType *>
      (this->GetDifferenceFunction().GetPointer());
 
  if( !drfp )
   {
   itkExceptionMacro( << 
     "Could not cast difference function to MIRegistrationFunction" );
   }
   
  drfp->SetRadius(radius);

}

/*
 * Get the metric value from the difference function
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
MIRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::ApplyUpdate(TimeStepType dt)
{
  // If we smooth the update buffer before applying it, then the are
  // approximating a viscuous problem as opposed to an elastic problem
  if ( this->GetSmoothUpdateField() )
    {
    this->SmoothUpdateField();
    }
  
  this->Superclass::ApplyUpdate(dt);

  MIRegistrationFunctionType *drfp = 
    dynamic_cast<MIRegistrationFunctionType *>
      (this->GetDifferenceFunction().GetPointer());
 
  if( !drfp )
   {
   itkExceptionMacro( << 
     "Could not cast difference function to MIRegistrationFunction" );
   }

//  this->SetRMSChange( drfp->GetRMSChange() );
   
}



} // end namespace otb

#endif
