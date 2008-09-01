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
#ifndef __otbRealMomentImageFunction_h
#define __otbRealMomentImageFunction_h

#include "otbGeometricMomentImageFunction.h"


namespace otb
{

/**
 * \class RealMomentImageFunction
 * \brief Virtual class for the Real moments for an image function
 *
 * \ingroup ImageFunctions
 */

template < class TInput, 
           class TOutput = float,
           class TPrecision = double,
	   class TCoordRep = float >
class ITK_EXPORT RealMomentImageFunction :
  public GeometricMomentImageFunction<TInput, TOutput, TPrecision, TCoordRep >
{
public:
  /** Standard class typedefs. */
  typedef RealMomentImageFunction                                     Self;
  typedef GeometricMomentImageFunction< TInput, TOutput, TPrecision, TCoordRep >   Superclass;
  typedef itk::SmartPointer<Self>                                     Pointer;
  typedef itk::SmartPointer<const Self>                               ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(RealMomentImageFunction, GeometricMomentImageFunction);


  /** InputImageType typedef support. */
  typedef typename Superclass::InputType            InputType;
  typedef typename Superclass::IndexType            IndexType;
  typedef typename Superclass::ContinuousIndexType  ContinuousIndexType;
  typedef typename Superclass::PointType            PointType;
 
  /** Type for calculation precision */
  typedef typename Superclass::PrecisionType        PrecisionType;

  typedef typename Superclass::OutputType           OutputType;
  typedef OutputType                                RealType;


  

protected:
  RealMomentImageFunction() {};
  ~RealMomentImageFunction(){};
  void PrintSelf(std::ostream& os, itk::Indent indent) const 
     {
      Superclass::PrintSelf( os, indent );
     }
     
private:
  RealMomentImageFunction( const Self& ); //purposely not implemented
  void operator=( const Self& );               //purposely not implemented

};

} // namespace otb

#endif

