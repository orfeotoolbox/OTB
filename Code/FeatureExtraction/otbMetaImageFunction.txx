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
#ifndef __otbMetaImageFunction_txx
#define __otbMetaImageFunction_txx

#include "otbMetaImageFunction.h"

namespace otb
{
template <class TOutputPrecision, class TCoordRep>
MetaImageFunction<TOutputPrecision,TCoordRep>
::MetaImageFunction()
 {

 }

template <class TOutputPrecision, class TCoordRep>
MetaImageFunction<TOutputPrecision,TCoordRep>
::~MetaImageFunction()
 {

 }

template <class TOutputPrecision, class TCoordRep>
typename MetaImageFunction<TOutputPrecision,TCoordRep>
::OutputType
MetaImageFunction<TOutputPrecision,TCoordRep>
::Evaluate(const PointType & point) const
 {
  OutputType resp;

  return resp;
 }


template <class TOutputPrecision, class TCoordRep>
void
MetaImageFunction<TOutputPrecision,TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
