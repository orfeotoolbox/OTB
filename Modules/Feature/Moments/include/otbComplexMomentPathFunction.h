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
#ifndef __otbComplexMomentPathFunction_h
#define __otbComplexMomentPathFunction_h

#include "otbGeometricMomentPathFunction.h"
#include "itkVectorContainer.h"

namespace otb
{

/**
 * \class ComplexMomentPathFunction
 * \brief Calculate the complex moment value over a path.
 *
 * The implemented equation is:
 *
 *  \f[  c_{p, q}=\int\int_{D} (x+iy)^{p} \cdot (x-iy)^{q} \cdot
 dx \cdot dy \f]
 *
 * With:
 *
 *   - \f$ (x, y) \f$ pixel localization;
 *   - \f$ D \f$ the contour domain.
 *
 * This class is templated over :
 *   - the input image type
 *   - the path type
 *   - and the coordinate representation type (e.g. float or double).
 *
 * \ingroup ImageFunctions
 */
template <class TInputPath,
    class TOutput = std::complex<double>,
    class TPrecision = double>
class ITK_EXPORT ComplexMomentPathFunction :
  public GeometricMomentPathFunction<TInputPath,
      TOutput,
      TPrecision>
{
public:
  /** Standard class typedefs. */
  typedef ComplexMomentPathFunction                                    Self;
  typedef GeometricMomentPathFunction<TInputPath, TOutput, TPrecision> Superclass;
  typedef itk::SmartPointer<Self>                                      Pointer;
  typedef itk::SmartPointer<const Self>                                ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ComplexMomentPathFunction, GeometricMomentPathFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputPathType typedef support. */
  typedef typename Superclass::PathType              PathType;
  typedef typename Superclass::PathConstPointer      PathConstPointer;
  typedef typename PathType::ContinuousIndexType     VertexType;
  typedef itk::VectorContainer<unsigned, VertexType> VertexListType;
  typedef typename VertexListType::ConstPointer      VertexListPointer;

  /** Complex Type */
  typedef typename Superclass::OutputType ComplexType;

  typedef typename Superclass::OutputType OutputType;

  typedef double RealType;

  /** Type for calculation precision */
  typedef typename Superclass::PrecisionType PrecisionType;

  /** ComplexType for calculation precision */
  typedef std::complex<PrecisionType> ComplexPrecisionType;

  /** Evalulate the function */
  virtual OutputType Evaluate(const PathType& path) const;
  virtual OutputType Evaluate() const;

  itkSetMacro(P, unsigned int);
  itkGetConstReferenceMacro(P, unsigned int);
  itkSetMacro(Q, unsigned int);
  itkGetConstReferenceMacro(Q, unsigned int);

protected:
  ComplexMomentPathFunction();
  virtual ~ComplexMomentPathFunction() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ComplexMomentPathFunction(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented
  ComplexPrecisionType EvaluateComplexMomentAtIndex(VertexType index) const;

  unsigned int m_P;
  unsigned int m_Q;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbComplexMomentPathFunction.txx"
#endif

#endif
