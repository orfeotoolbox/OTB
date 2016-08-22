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
#ifndef otbOrientationPathFunction_txx
#define otbOrientationPathFunction_txx

#include "otbOrientationPathFunction.h"
#include "otbPathFunction.h"
#include "itkNumericTraits.h"
#include "otbMacro.h"
#include "otbMath.h"

namespace otb
{

template <class TInputPath, class TOutput>
void
OrientationPathFunction<TInputPath, TOutput>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

template <class TInputPath, class TOutput>
typename OrientationPathFunction<TInputPath,
    TOutput>::OutputType
OrientationPathFunction<TInputPath, TOutput>
::Evaluate(const PathType& path) const
{
  typedef double RealType;

  VertexListPointer vertexList;
  VertexType        cindex;
  VertexType        IndexOut;
  int               nbPath;
  RealType          Theta;

  vertexList = path.GetVertexList();
  nbPath = vertexList->Size();

  if (nbPath == 2)
    {
    cindex = vertexList->GetElement(0);
    RealType x1 = cindex[0];
    RealType y1 = cindex[1];
    cindex = vertexList->GetElement(1);
    RealType x2 = cindex[0];
    RealType y2 = cindex[1];

    Theta = vcl_atan2(y2 - y1, x2 - x1);
    } // IF loop
  else
    {
    itkExceptionMacro(<< "OrientationPathFunction::Evaluate() FAILED -- path must have 2 points");
    }
  return (static_cast<OutputType>(Theta));

}

template <class TInputPath, class TOutput>
typename OrientationPathFunction<TInputPath,
    TOutput>::OutputType
OrientationPathFunction<TInputPath, TOutput>
::Evaluate() const
{
  if (!this->GetInputPath())
    {
    otbMsgDevMacro(<< "Problem with GetInputPath");
    return static_cast<OutputType>(itk::NumericTraits<OutputType>::max());
    }

  OutputType Result =  Evaluate(*(this->GetInputPath()));

  return Result;
}

} // namespace otb

#endif
