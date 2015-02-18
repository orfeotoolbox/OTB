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
#ifndef __otbHuPathFunction_h
#define __otbHuPathFunction_h

#include "otbRealMomentPathFunction.h"

namespace otb
{

/**
 * \class HuPathFunction
 * \brief Calculate the Hu's invariant parameter.
 *
 * Calculate the Hu's invariant defined as:
 *
 * - \f$ \phi_{1} = c_{11} \f$
 * - \f$ \phi_{2} = c_{20} c_{02} \f$
 * - \f$ \phi_{3} = c_{30} c_{03} \f$
 * - \f$ \phi_{4} = c_{21} c_{12} \f$
 * - \f$ \phi_{5} = Re (c_{30} c_{12}^{3}) \f$
 * - \f$ \phi_{6} = Re (c_{20} c_{12}^{2}) \f$
 * - \f$ \phi_{7} = Im (c_{30} c_{12}^{3}) \f$
 *
 * With :
 *
 *  \f[  c_{p, q}=\int \int (x+iy)^{p} \cdot (x-iy)^{q} \cdot f(x, y) \cdot
 dx \cdot dy \f]
 *
 * And:
 *  - \f$(x, y)\f$ pixel localization;
 *  - \f$ f(x, y)\f$  the pixel value over the \f$(x, y)\f$ coordinate.
 *
 * This class is templated over the input image type and the
 * path type (e.g. PolyLineParametricPath<2> ).
 *
 * \ingroup FunctionFunctions
 *
 * \ingroup OTBMoments
 */

template <class TInputPath,
    class TOutput    = double,
    class TPrecision = double>
class ITK_EXPORT HuPathFunction :
  public RealMomentPathFunction<TInputPath, TOutput, TPrecision>
{
public:
  /** Standard class typedefs. */
  typedef HuPathFunction                                          Self;
  typedef RealMomentPathFunction<TInputPath, TOutput, TPrecision> Superclass;
  typedef itk::SmartPointer<Self>                                 Pointer;
  typedef itk::SmartPointer<const Self>                           ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(HuPathFunction, RealMomentPathFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputPathType typedef support. */
  typedef typename Superclass::PathType              PathType;
  typedef typename Superclass::PathConstPointer      PathConstPointer;
  typedef typename PathType::ContinuousIndexType     VertexType;
  typedef itk::VectorContainer<unsigned, VertexType> VertexListType;
  typedef typename VertexListType::ConstPointer      VertexListPointer;

  /** OutputType typedef support. */
  typedef typename Superclass::RealType RealType;

  /** Type for calculation precision */
  typedef typename Superclass::PrecisionType PrecisionType;

  /** Evaluate the function at non-integer positions */
  virtual RealType Evaluate(const PathType& path) const;
  virtual RealType Evaluate() const;
  /** Get/Set the radius of the neighborhood over which the
      statistics are evaluated */
  itkSetMacro(MomentNumber, short);
  itkGetConstReferenceMacro(MomentNumber, short);

protected:
  HuPathFunction();
  virtual ~HuPathFunction() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  HuPathFunction(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

  short m_MomentNumber;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbHuPathFunction.txx"
#endif

#endif
