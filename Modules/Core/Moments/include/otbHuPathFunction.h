/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbHuPathFunction_h
#define otbHuPathFunction_h

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

template <class TInputPath, class TOutput = double, class TPrecision = double>
class ITK_EXPORT HuPathFunction : public RealMomentPathFunction<TInputPath, TOutput, TPrecision>
{
public:
  /** Standard class typedefs. */
  typedef HuPathFunction Self;
  typedef RealMomentPathFunction<TInputPath, TOutput, TPrecision> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(HuPathFunction, RealMomentPathFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputPathType typedef support. */
  typedef typename Superclass::PathType          PathType;
  typedef typename Superclass::PathConstPointer  PathConstPointer;
  typedef typename PathType::ContinuousIndexType VertexType;
  typedef itk::VectorContainer<unsigned, VertexType> VertexListType;
  typedef typename VertexListType::ConstPointer VertexListPointer;

  /** OutputType typedef support. */
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
  HuPathFunction();
  ~HuPathFunction() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  HuPathFunction(const Self&) = delete;
  void operator=(const Self&) = delete;

  short m_MomentNumber;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbHuPathFunction.hxx"
#endif

#endif
