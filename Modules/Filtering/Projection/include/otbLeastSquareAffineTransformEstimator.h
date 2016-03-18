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
#ifndef __otbLeastSquareAffineTransformEstimator_h
#define __otbLeastSquareAffineTransformEstimator_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkMatrix.h"
#include "itkVector.h"
#include "itkAffineTransform.h"

namespace otb {

/** \class LeastSquareAffineTransformEstimator
 * \brief This class provide the affine transform LSQR estimation
 *
 * This class uses the classical least square optimisation to estimate
 * an affine transfrom from a set of tie points.
 *
 * This implementation supports points of any dimension.
 *
 * Tie points can be added through the AddTiePoints() method.
 *
 * The ClearTiePoints() method allows removing all the tie points
 * that has been previously set.
 *
 * Once all the tie points have been feeded into the estimator, the
 * Compute() method will perform the optimization.
 *
 * Matrix of the estimated affine transform can be retrieved using the
 * GetMatrix() method.
 *
 * Offset of the estimated affine transform can be retrieved using the
 * GetOffset() method.
 *
 * Alternatively, the GetAffineTransfrom() methods return a pointer to
 * a fully set-up forward affine transfrom (\sa AffineTransform).
 *
 * Estimation errors are available per dimension using the
 * GetRMSError() (root mean square location error) or
 * GetRelativeResidual() (relative residual error).
 *
 * NOTE: All computation are performed in double. Results are casted
 * back to TPoint::CoordRepType.
 *
 *
 * \ingroup OTBProjection
 */
template <class TPoint>
class ITK_EXPORT LeastSquareAffineTransformEstimator :
  public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef LeastSquareAffineTransformEstimator Self;
  typedef itk::Object                         Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LeastSquareAffineTransformEstimator, Object);

  /** Extract dimension from input and output image. */
  itkStaticConstMacro(PointDimension, unsigned int,
                      TPoint::PointDimension);

  /** Points typedefs */
  typedef TPoint                           PointType;
  typedef typename PointType::CoordRepType PrecisionType;
  typedef itk::CovariantVector<PrecisionType,
      PointDimension>  CovariantVectorType;
  typedef std::pair<PointType, PointType> TiePointsType;
  typedef std::vector<TiePointsType>      TiePointsContainerType;

  /** Affine transform components typedefs */
  typedef itk::Matrix<PrecisionType,
      PointDimension,
      PointDimension>           MatrixType;
  typedef itk::Vector<PrecisionType,
      PointDimension>           VectorType;
  typedef itk::AffineTransform<PrecisionType,
      PointDimension>  AffineTransformType;
  typedef typename AffineTransformType::Pointer AffineTransformPointerType;

  /** Get the affine transform matrix */
  itkGetConstReferenceMacro(Matrix, MatrixType);

  /** Get the affine transform offset */
  itkGetConstReferenceMacro(Offset, VectorType);

  /** Get the estimated affine transfrom */
  itkGetObjectMacro(AffineTransform, AffineTransformType);

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
  void AddTiePoints(const PointType& src, const PointType& dst);

  /** Clear all tie points */
  void ClearTiePoints();

  /** The Compute method does the affine least square estimation */
  void Compute();

protected:
  /** Constructor */
  LeastSquareAffineTransformEstimator();
  /** Destructor */
  virtual ~LeastSquareAffineTransformEstimator();

  /** The PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  LeastSquareAffineTransformEstimator (const Self &);   // purposely not implemented
  void operator =(const Self&);    // purposely not implemented

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

  /** Affine transfrom */
  AffineTransformPointerType m_AffineTransform;

}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLeastSquareAffineTransformEstimator.txx"
#endif

#endif
