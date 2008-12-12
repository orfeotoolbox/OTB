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
#ifndef __otbNCCRegistrationFilter_txx
#define __otbNCCRegistrationFilter_txx
#include "otbNCCRegistrationFilter.h"

namespace otb {

/*
 * Default constructor
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
NCCRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::NCCRegistrationFilter()
{

  typename NCCRegistrationFunctionType::Pointer drfp;
  drfp = NCCRegistrationFunctionType::New();

  drfp->SetDeformationField( this->GetDeformationField() );

  this->SetDifferenceFunction( static_cast<FiniteDifferenceFunctionType *>(
                                 drfp.GetPointer() ) );

}


template <class TFixedImage, class TMovingImage, class TDeformationField>
void
NCCRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
   os << indent << "NCC Radius: " <<
    this->GetNCCRadius() << std::endl;
}


/*
 * Set the function state values before each iteration
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
NCCRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::InitializeIteration()
{

  // call the superclass  implementation
  Superclass::InitializeIteration();

  // set the gradient selection flag
  NCCRegistrationFunctionType *drfp =
    dynamic_cast<NCCRegistrationFunctionType *>
      (this->GetDifferenceFunction().GetPointer());

  if( !drfp )
   {
   itkExceptionMacro( <<
     "Could not cast difference function to NCCRegistrationFunction" );
   }


  /*
   * Smooth the deformation field

  if ( this->GetSmoothDeformationField() )
    {
    this->SmoothDeformationField();
    }
   */
}


/*
 * Get the metric value from the difference function
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
double
NCCRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::GetMetric() const
{

  NCCRegistrationFunctionType *drfp =
    dynamic_cast<NCCRegistrationFunctionType *>
      (this->GetDifferenceFunction().GetPointer());

  if( !drfp )
   {
   itkExceptionMacro( <<
     "Could not cast difference function to NCCRegistrationFunction" );
   }

  return drfp->GetEnergy();

}


/*
 *
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
typename NCCRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>::RadiusType
NCCRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::GetNCCRadius() const
{

  NCCRegistrationFunctionType *drfp =
    dynamic_cast<NCCRegistrationFunctionType *>
      (this->GetDifferenceFunction().GetPointer());

  if( !drfp )
   {
   itkExceptionMacro( <<
     "Could not cast difference function to NCCRegistrationFunction" );
   }

  return drfp->GetRadius();

}

/*
 *
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
NCCRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::SetNCCRadius(RadiusType radius)
{

  NCCRegistrationFunctionType *drfp =
    dynamic_cast<NCCRegistrationFunctionType *>
      (this->GetDifferenceFunction().GetPointer());

  if( !drfp )
   {
   itkExceptionMacro( <<
     "Could not cast difference function to NCCRegistrationFunction" );
   }

  drfp->SetRadius(radius);

  this->Modified();
}

template <class TFixedImage, class TMovingImage, class TDeformationField>
void
NCCRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::GenerateInputRequestedRegion()
{
   // get pointers to the input and output
  typename Superclass::FixedImagePointer fixedPtr =
      const_cast< TFixedImage * >( this->GetFixedImage() );
  typename Superclass::MovingImagePointer movingPtr =
      const_cast< TMovingImage * >( this->GetMovingImage() );
  typename TDeformationField::Pointer outputPtr = this->GetOutput();

  if ( !fixedPtr || !movingPtr || !outputPtr )
  {
    return;
  }

      // get a copy of the input requested region (should equal the output
      // requested region)
  typename TDeformationField::RegionType requestedRegion;
  requestedRegion = outputPtr->GetRequestedRegion();

      // pad the input requested region by the operator radius
  requestedRegion.PadByRadius( this->GetNCCRadius() );

      // crop the input requested region at the input's largest possible region
  if ( requestedRegion.Crop(fixedPtr->GetLargestPossibleRegion()))
  {
    if ( requestedRegion.Crop(movingPtr->GetLargestPossibleRegion()))
    {
      fixedPtr->SetRequestedRegion( requestedRegion );
      movingPtr->SetRequestedRegion( requestedRegion );
      return;
    }
    else
    {
        // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
      movingPtr->SetRequestedRegion( requestedRegion );

    // build an exception
      itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
      e.SetLocation(ITK_LOCATION);
      e.SetDescription("Requested region is (at least partially) outside the largest possible region of the moving image.");
      e.SetDataObject(movingPtr);
      throw e;

    }
  }
  else
  {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    fixedPtr->SetRequestedRegion( requestedRegion );

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is (at least partially) outside the largest possible region of the fixed image.");
    e.SetDataObject(fixedPtr);
    throw e;
  }
}

/*
 * Get the metric value from the difference function
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
NCCRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::ApplyUpdate(TimeStepType dt)
{
  // If we smooth the update buffer before applying it, then the are
  // approximating a viscuous problem as opposed to an elastic problem
/*  if ( this->GetSmoothUpdateField() )
    {
    this->SmoothUpdateField();
    }
  */
  this->Superclass::ApplyUpdate(dt);

  NCCRegistrationFunctionType *drfp =
    dynamic_cast<NCCRegistrationFunctionType *>
      (this->GetDifferenceFunction().GetPointer());

  if( !drfp )
   {
   itkExceptionMacro( <<
     "Could not cast difference function to NCCRegistrationFunction" );
   }

//  this->SetRMSChange( drfp->GetRMSChange() );

}



} // end namespace otb

#endif
