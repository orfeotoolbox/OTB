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

#include <vector>

namespace otb
{
/** \class MetaImageFunction
 *  \brief Concatenate results from multiple ImageFunction
 *
 *  The MetaImageFunction class allows calling multiple ImageFunction at the same location
 *  and to concatenate their result into a single VariableLengthVector.
 *
 *  In the case of ImageFunction which do not produce VariableLengthVector, one can wrap these
 *  function using the ImageFunctionAdaptor class, which will translate the image function output to
 *  a VariableLengthVector.
 *
 *  \ingroup ImageFunction
 *
 * \ingroup OTBImageBase
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
  itkTypeMacro(MetaImageFunction, itk::FunctionBase);

  // Input and output typedef
  typedef TCoordRep                                   CoordRepType;
  typedef itk::Point<CoordRepType, 2>                  PointType;
  typedef TOutputPrecision                            ValueType;
  typedef itk::VariableLengthVector<ValueType>        OutputType;

  // Compatible functions typedefs
  typedef Superclass                                  FunctionType;
  typedef typename FunctionType::Pointer              FunctionPointerType;
  typedef std::vector<FunctionPointerType>            FunctionContainerType;

  /** Evaluate the function at the given location */
  OutputType Evaluate(const PointType & point) const;

  /** Add a new function to the functions vector */
  void AddFunction(FunctionType * function);

  /** Add a new function the functions vector (compatibility via adapters) */
//  template <typename T1, typename T2> void AddFunction(itk::ImageFunction<T1, T2, TCoordRep> * function);

  /** Clear functions vector */
  void ClearFunctions();

  /** Get the number of function registered */
  unsigned int GetNumberOfFunctions() const;

  /** Retrieve the nth function */
  FunctionType * GetNthFunction(unsigned int index);

  /** Remove the nth function */
  void RemoveNthFunction(unsigned int index);

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

  FunctionContainerType m_FunctionContainer;
};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMetaImageFunction.txx"
#endif

#endif
