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
#ifndef __otbRealMomentPathFunction_h
#define __otbRealMomentPathFunction_h

#include "otbGeometricMomentPathFunction.h"
#include "itkVectorContainer.h"


namespace otb
{

/**
 * \class RealMomentPathFunction
 * \brief Virtual class for the Real moments for a path
 *
 * \ingroup PathFunctions
 */

template < class TInputPath,
class TOutput      = double,
class TPrecision   = double>
class ITK_EXPORT RealMomentPathFunction :
      public GeometricMomentPathFunction< TInputPath, TOutput, TPrecision>
{
public:
  /** Standard class typedefs. */
  typedef RealMomentPathFunction                                      Self;
  typedef GeometricMomentPathFunction< TInputPath, TOutput, TPrecision> Superclass;
  typedef itk::SmartPointer<Self>                                     Pointer;
  typedef itk::SmartPointer<const Self>                               ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(RealMomentPathFunction, GeometricMomentPathFunction);

  /** InputPathType typedef support. */
  typedef typename Superclass::PathType                 PathType;
  typedef typename Superclass::PathConstPointer         PathConstPointer;
  typedef typename PathType::ContinuousIndexType        VertexType;
  typedef itk::VectorContainer< unsigned,VertexType >   VertexListType;
  typedef typename VertexListType::ConstPointer         VertexListPointer;

  /** Type for calculation precision */
  typedef typename Superclass::PrecisionType            PrecisionType;

  /** OutputType typedef support. */
  typedef typename Superclass::OutputType               RealType;


  itkSetMacro(Step, float);
  itkGetConstReferenceMacro(Step, float);

protected:
  RealMomentPathFunction()
  {
    m_Step = 1.0;
  }
  ~RealMomentPathFunction() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf( os, indent );
  }

private:
  RealMomentPathFunction( const Self& ); //purposely not implemented
  void operator=( const Self& );               //purposely not implemented

  float        m_Step;

};

} // namespace otb

#endif

