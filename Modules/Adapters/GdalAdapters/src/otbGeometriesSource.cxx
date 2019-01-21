/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


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
  // The following line has for side effect to set of DataSource with the wrong
  // type as default value instead of a plain nil
  // smartpointer... => NO
  // Superclass::SetNthOutput(0, 0);

  // Moreover, the default, and correct, value will be set in DoAllocateOutputs if
  // nothing is set by then => nothing more to do.
}

/*virtual*/
otb::GeometriesSource::~GeometriesSource()
{
}

/*virtual*/
otb::GeometriesSource::OutputGeometriesType* otb::GeometriesSource::GetOutput(void )
{
  return static_cast<GeometriesSet*> (Superclass::GetOutput(0));
}

/*virtual*/
otb::GeometriesSource::OutputGeometriesType* otb::GeometriesSource::GetOutput(DataObjectPointerArraySizeType idx)
{
  return static_cast<GeometriesSet*> (Superclass::GetOutput(idx));
}

/*virtual*/
void otb::GeometriesSource::SetOutput(OutputGeometriesType* output, DataObjectPointerArraySizeType idx/* = 0 */)
{
  Superclass::SetNthOutput(idx, output);
}

/*virtual*/
void otb::GeometriesSource::DoAllocateOutputs()
{
  // The default behaviour is to prepare a in-memory OGR datasource in case
  // filters are piped.
  // In the filter is meant to produce a file, use SetOutput, or the New(string)
  // function to built the GeometriesSource and filters
  if (!GetOutput() || !GetOutput()->IsSet())
    {
    GeometriesSet::Pointer gs = GeometriesSet::New(); // in-memory DataSource
    assert(gs);
    this->SetOutput(gs);
    ///\todo Check whether we need to increase the count of the \c GeometriesSet as
    ///it'll get reelased at the end of function the scope.
    }
}

/*virtual*/
void otb::GeometriesSource::PrepareOutputs()
{
  DoAllocateOutputs();
  Superclass::PrepareOutputs();
}
