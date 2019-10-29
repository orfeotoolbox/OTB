/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbComplexMomentPathFunction_h
#define otbComplexMomentPathFunction_h

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
 *
 * \ingroup OTBMoments
 */
template <class TInputPath, class TOutput = std::complex<double>, class TPrecision = double>
class ITK_EXPORT ComplexMomentPathFunction : public GeometricMomentPathFunction<TInputPath, TOutput, TPrecision>
{
public:
  /** Standard class typedefs. */
  typedef ComplexMomentPathFunction Self;
  typedef GeometricMomentPathFunction<TInputPath, TOutput, TPrecision> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ComplexMomentPathFunction, GeometricMomentPathFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputPathType typedef support. */
  typedef typename Superclass::PathType          PathType;
  typedef typename Superclass::PathConstPointer  PathConstPointer;
  typedef typename PathType::ContinuousIndexType VertexType;
  typedef itk::VectorContainer<unsigned, VertexType> VertexListType;
  typedef typename VertexListType::ConstPointer VertexListPointer;

  /** Complex Type */
  typedef typename Superclass::OutputType ComplexType;

  typedef typename Superclass::OutputType OutputType;

  typedef double RealType;

  /** Type for calculation precision */
  typedef typename Superclass::PrecisionType PrecisionType;

  /** ComplexType for calculation precision */
  typedef std::complex<PrecisionType> ComplexPrecisionType;

  /** Evalulate the function */
  OutputType Evaluate(const PathType& path) const override;
  virtual OutputType Evaluate() const;

  itkSetMacro(P, unsigned int);
  itkGetConstReferenceMacro(P, unsigned int);
  itkSetMacro(Q, unsigned int);
  itkGetConstReferenceMacro(Q, unsigned int);

protected:
  ComplexMomentPathFunction();
  ~ComplexMomentPathFunction() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ComplexMomentPathFunction(const Self&) = delete;
  void                 operator=(const Self&) = delete;
  ComplexPrecisionType EvaluateComplexMomentAtIndex(VertexType index) const;

  unsigned int m_P;
  unsigned int m_Q;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbComplexMomentPathFunction.hxx"
#endif

#endif
