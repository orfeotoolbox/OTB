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

#ifndef _otbShapeFileDataSource_txx
#define _otbShapeFileDataSource_txx

#include "otbShapeFileDataSource.h"

namespace otb
{
/**
 *   Constructor
 */
template <class TOutputShapeFile>
ShapeFileDataSource<TOutputShapeFile>
::ShapeFileDataSource()
{
  this->Superclass::SetNumberOfRequiredOutputs(1);
  this->Superclass::SetNthOutput(0,OutputShapeFileType::New().GetPointer());
}

template <class TOutputShapeFile>
ShapeFileDataSource<TOutputShapeFile>
::~ShapeFileDataSource()
{
}
/**
 * Get the output image list
 * \return The image list produced.
 */
template <class TOutputShapeFile>
typename ShapeFileDataSource<TOutputShapeFile>::OutputShapeFileType *
ShapeFileDataSource<TOutputShapeFile>
::GetOutput(void)
{
  if(this->GetNumberOfOutputs()<1)
    {
    return 0;
    }
  return static_cast<OutputShapeFileType *> (this->ProcessObject::GetOutput(0));
}
/**
 * PrintSelf Method
 */
template<class TOutputShapeFile>
void
ShapeFileDataSource<TOutputShapeFile>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}
}// End namespace otb

#endif
