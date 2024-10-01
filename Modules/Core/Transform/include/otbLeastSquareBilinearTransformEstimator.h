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

#ifndef otbLeastSquareBilinearTransformEstimator_h
#define otbLeastSquareBilinearTransformEstimator_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkMatrix.h"
#include "itkVector.h"
#include "itkAffineTransform.h"

namespace otb
{

/** \class LeastSquareBilinearTransformEstimator
 * \brief This class provide the 2D Bilinear transform LSQR estimation
 *
 * z(x,y) = a + b*x + c*y + d*x*y
 *
 * \ingroup OTBTransform
 */
template <class TPoint>
class ITK_EXPORT LeastSquareBilinearTransformEstimator : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef LeastSquareBilinearTransformEstimator Self;
  typedef itk::Object                         Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LeastSquareBilinearTransformEstimator, Object);

  /** Extract dimension from input and output image. */
  itkStaticConstMacro(PointDimension, unsigned int, TPoint::PointDimension);

  /** Points typedefs */
  typedef TPoint                           PointType;
  typedef typename PointType::CoordRepType PrecisionType;
  typedef itk::CovariantVector<PrecisionType, PointDimension> CovariantVectorType;
  typedef std::pair<PointType, PrecisionType>                     TiePointsType;
  typedef std::vector<TiePointsType> TiePointsContainerType;

  /** Affine transform components typedefs */
  typedef itk::Matrix<PrecisionType, 4, 4> MatrixType;
  typedef itk::Matrix<PrecisionType, 4, 1> MatrixVecType;
  typedef itk::Matrix<PrecisionType, 1, 4> MatrixVecTransposeType;
  typedef itk::Vector<PrecisionType, 4>    VectorType;

    /** Constructor */
  LeastSquareBilinearTransformEstimator();
  /** Destructor */
  ~LeastSquareBilinearTransformEstimator() = default;

  /** Get the bilinear transform matrix */
  itkGetConstReferenceMacro(Matrix, MatrixType);

  /** Get the bilinear transform offset */
  itkGetConstReferenceMacro(Offset, VectorType);

  /** Get the RMS error */
  itkGetConstReferenceMacro(RMSError, CovariantVectorType);

  /** Get the relative residual */
  itkGetConstReferenceMacro(RelativeResidual, CovariantVectorType);

  /** Get the GCPsContainer
   * \return The GCPs container */
  TiePointsContainerType& GetTiePointsContainer();

  /** Set the GCP container */
  void SetTiePointsContainer(const TiePointsContainerType& container);

  /** Add a pair of tie points */
  void AddTiePoints(const PointType& src, const PrecisionType& dst);

  /** Clear all tie points */
  void ClearTiePoints();

  /** The Compute method does the affine least square estimation */
  void Compute();

  /**
  * interpolate LS-fit value at location (xx,yy) - returns z(xx,yy).
  *
  * @param xx "x" coordinate at which to interpolate.
  * @param yy "y" coordinate at which to interpolate.
  * 
  */
  void lsFitValue(const PointType& point, PrecisionType& dst) const;

protected:
  /** The PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  LeastSquareBilinearTransformEstimator(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Container of GCPs */
  TiePointsContainerType m_TiePointsContainer;

  /** RMS error */
  CovariantVectorType m_RMSError;

  /** Relative residual */
  CovariantVectorType m_RelativeResidual;

  /** Affine transform matrix */
  MatrixType m_Matrix;

  /** Affine transform offset */
  VectorType m_Offset;


  // BILINEAR 
  // Respectibely: constant, linear-X, linear-Y, cross-XY term
  double bl_a, bl_b, bl_c, bl_d;

  // Normal system coefficient matrix.
  vnl_matrix_fixed<double, 4, 4>  Ata;
  // Normal system RHS vector
  vnl_matrix_fixed<double, 4, 1>  Atb;


}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLeastSquareBilinearTransformEstimator.hxx"
#endif

#endif
