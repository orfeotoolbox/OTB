/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbFlusserPathFunction_h
#define otbFlusserPathFunction_h

#include "otbRealMomentPathFunction.h"

namespace otb
{

/**
 * \class FlusserPathFunction
 * \brief Calculate the Flusser's invariant parameters.
 *
 * Calculate the Flusser's invariant over a path and defined as:
 *
 * - \f$ \psi_{1} = c_{11} \f$
 * - \f$ \psi_{2} = c_{21} c_{12} \f$
 * - \f$ \psi_{3} = Re (c_{20} c_{12}^{2} )\f$
 * - \f$ \psi_{4} = Im (c_{20} c_{12}^{2} )\f$
 * - \f$ \psi_{5} = Re (c_{30} c_{12}^{3} )\f$
 * - \f$ \psi_{6} = Im (c_{30} c_{12}^{3} )\f$
 * - \f$ \psi_{7} = c_{22} \f$
 * - \f$ \psi_{8} = Re (c_{31} c_{12}^{2} )\f$
 * - \f$ \psi_{9} = Im (c_{31} c_{12}^{2} )\f$
 * - \f$ \psi_{10} = Re (c_{40} c_{12}^{4} )\f$
 * - \f$ \psi_{11} = Im (c_{40} c_{12}^{4} )\f$
 *
 * With :
 *
 *  \f[  c_{p, q}=\int \int (x+iy)^{p} \cdot (x-iy)^{q} \cdot f(x, y) \cdot
 dx \cdot dy \f]
 *
 * And:
 *  - \f$(x, y)\f$ pixel localization;
 *  - \f$ f(x, y)\f$ the pixel value over the \f$(x, y)\f$ coordinate.
 *
 * This class is templated over the input image type and the
 * coordinate representation type (e.g. float or double).
 *
 * \ingroup PathFunctions
 *
 * \ingroup OTBMoments
 */

template <class TInputPath, class TOutput = double, class TPrecision = double>
class ITK_EXPORT FlusserPathFunction : public RealMomentPathFunction<TInputPath, TOutput, TPrecision>
{
public:
  /** Standard class typedefs. */
  typedef FlusserPathFunction Self;
  typedef RealMomentPathFunction<TInputPath, TOutput, TPrecision> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(FlusserPathFunction, RealMomentPathFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputPathType typedef support. */
  typedef typename Superclass::PathType          PathType;
  typedef typename Superclass::PathConstPointer  PathConstPointer;
  typedef typename PathType::ContinuousIndexType VertexType;
  typedef itk::VectorContainer<unsigned, VertexType> VertexListType;
  typedef typename VertexListType::ConstPointer VertexListPointer;

  typedef typename Superclass::RealType RealType;

  /** Type for calculation precision */
  typedef typename Superclass::PrecisionType PrecisionType;

  /** Evaluate the function at non-integer positions */
  RealType Evaluate(const PathType& path) const override;
  virtual RealType Evaluate() const;
  /** Get/Set the radius of the neighborhood over which the
      statistics are evaluated */
  itkSetMacro(MomentNumber, short);
  itkGetConstReferenceMacro(MomentNumber, short);

protected:
  FlusserPathFunction();
  ~FlusserPathFunction() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  FlusserPathFunction(const Self&) = delete;
  void operator=(const Self&) = delete;

  short m_MomentNumber;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFlusserPathFunction.hxx"
#endif

#endif
