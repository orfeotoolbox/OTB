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
#ifndef __otbCompacityPathFunction_h
#define __otbCompacityPathFunction_h

#include "otbPathFunction.h"
#include "itkVectorContainer.h"

namespace otb
{

/**
 * \class CompacityPathFunction
 * \brief Calculate the compacity of a path.
 * The formula of the compacity is :
 * \f[ compacity = 4 \pi Surface / Perimeter^{2} \f]
 *
 * The path must contain at least 3 points.
 * The result value is comprise between 0.0 and 1.0
 *
 * \ingroup PathFunctions
 */

template <class TInputPath,
    class TOutput      = double>
class ITK_EXPORT CompacityPathFunction :
  public PathFunction<TInputPath, TOutput>
{
public:
  /** Standard class typedefs. */
  typedef CompacityPathFunction             Self;
  typedef PathFunction<TInputPath, TOutput> Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(CompacityPathFunction, PathFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputPathType typedef support. */
  typedef typename Superclass::InputPathType         PathType;
  typedef typename Superclass::InputPathConstPointer PathConstPointer;
  typedef typename PathType::ContinuousIndexType     VertexType;
  typedef itk::VectorContainer<unsigned, VertexType> VertexListType;
  typedef typename VertexListType::ConstPointer      VertexListPointer;
  typedef TOutput                                    OutputType;

  typedef double RealType;

  /** Evaluate the function at non-integer positions */
  virtual OutputType Evaluate(const PathType& path) const;
  virtual OutputType Evaluate() const;

protected:
  CompacityPathFunction() {};
  virtual ~CompacityPathFunction() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  CompacityPathFunction(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbCompacityPathFunction.txx"
#endif

#endif
