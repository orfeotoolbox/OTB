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
#ifndef otbGeometricMomentPathFunction_h
#define otbGeometricMomentPathFunction_h

#include "otbPathFunction.h"

namespace otb
{

/**
 * \class GeometricMomentPathFunction
 * \brief Virtual class for the Geometric moments for an path function
 *
 * \ingroup PathFunctions
 *
 * \ingroup OTBMoments
 */

template <class TInputPath,
    class TOutput,
    class TPrecision = double>
class ITK_EXPORT GeometricMomentPathFunction :
  public PathFunction<TInputPath, TOutput>
{
public:
  /** Standard class typedefs. */
  typedef GeometricMomentPathFunction       Self;
  typedef PathFunction<TInputPath, TOutput> Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(GeometricMomentPathFunction, PathFunction);

  /** InputPathType typedef support. */
  typedef typename Superclass::InputPathType         PathType;
  typedef typename Superclass::InputPathConstPointer PathConstPointer;

  /** OutputPathType typedef support. */
  typedef typename Superclass::OutputType OutputType;

  /** Type for calculation precision */
  typedef TPrecision PrecisionType;

protected:
  GeometricMomentPathFunction() {};
  ~GeometricMomentPathFunction() ITK_OVERRIDE {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  GeometricMomentPathFunction(const Self &);  //purposely not implemented
  void operator =(const Self&);                //purposely not implemented
};

} // namespace otb

#endif
