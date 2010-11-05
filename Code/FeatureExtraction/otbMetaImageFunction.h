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
#ifndef __otbMetaImageFunction_h
#define __otbMetaImageFunction_h

#include "itkFunctionBase.h"
#include "itkPoint.h"
#include "itkVariableLengthVector.h"

namespace otb
{
/** \class MetaImageFunction
 *  \brief TODO
 *
 */
template <class TOutputPrecision = double, class TCoordRep = double>
class ITK_EXPORT MetaImageFunction
: public itk::FunctionBase<itk::Point<TCoordRep, 2>,
  itk::VariableLengthVector<TOutputPrecision> >
{
public:
  // Standard class typedefs
  typedef MetaImageFunction                           Self;
  typedef itk::FunctionBase<
        itk::Point<TCoordRep, 2>,
        itk::VariableLengthVector<TOutputPrecision> > Superclass;
  typedef itk::SmartPointer<Self>                     Pointer;
  typedef itk::SmartPointer<const Self>               ConstPointer;

  // New macro
  itkNewMacro(Self);

  // RTTI typeinfo
  itkTypeMacro(MetaImageFunction,itk::FunctionBase);

  // Input and output typedef
  typedef TCoordRep                                   CoordRepType;
  typedef itk::Point<CoordRepType,2>                  PointType;
  typedef TOutputPrecision                            ValueType;
  typedef itk::VariableLengthVector<ValueType>        OutputType;

  /** Evaluate the function at the given location */
  OutputType Evaluate(const PointType & point) const;

protected:
  /** Constructor */
  MetaImageFunction();

  /** Destructor */
  ~MetaImageFunction();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  MetaImageFunction(const Self& ); //purposely not implemented
  void operator=(const Self& ); //purposely not implemented
};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMetaImageFunction.txx"
#endif

#endif
