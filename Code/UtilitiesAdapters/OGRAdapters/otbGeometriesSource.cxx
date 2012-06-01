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

/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#include "otbGeometriesSource.h"
#include "otbGeometriesSet.h"

/*===========================================================================*/
/*==============================[ other stuff ]==============================*/
/*===========================================================================*/

otb::GeometriesSource::GeometriesSource()
{
  Superclass::SetNumberOfRequiredOutputs(1);
  // The default behaviour is to prepare a in-memory OGR datasource in case
  // filters are piped.
  // In the filter is meant to produce a file, use SetOutput, or the New(string)
  // function to built the GeometriesSource and filters
  ogr::DataSource::Pointer inMemoryDS = ogr::DataSource::New();
  Superclass::SetNthOutput(0, inMemoryDS);
}

/*virtual*/  otb::GeometriesSource::~GeometriesSource()
{
}

/*virtual*/
otb::GeometriesSource::OutputGeometriesType* otb::GeometriesSource::GetOutput(void )
{
  return static_cast<GeometriesSet*> (Superclass::GetOutput(0));
}

/*virtual*/
otb::GeometriesSource::OutputGeometriesType* otb::GeometriesSource::GetOutput(unsigned int idx)
{
  return static_cast<GeometriesSet*> (Superclass::GetOutput(idx));
}

/*virtual*/
void otb::GeometriesSource::SetOutput(OutputGeometriesType* output, unsigned int idx/* = 0 */)
{
  Superclass::SetNthOutput(idx, output);
}
