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
#ifndef __otbLeastSquareAffineTransformEstimator_txx
#define __otbLeastSquareAffineTransformEstimator_txx

#include <vnl/algo/vnl_lsqr.h>
#include <vnl/vnl_sparse_matrix_linear_system.h>
#include <vnl/vnl_least_squares_function.h>

#include "otbMacro.h"
#include "otbLeastSquareAffineTransformEstimator.h"

namespace otb {

template <class TPoint>
LeastSquareAffineTransformEstimator<TPoint>
::LeastSquareAffineTransformEstimator() : m_TiePointsContainer(),
  m_RMSError(),
  m_RelativeResidual(),
  m_Matrix(),
  m_Offset(),
  m_AffineTransform()
{
  // Build the affine transform object
  m_AffineTransform = AffineTransformType::New();
}

template <class TPoint>
LeastSquareAffineTransformEstimator<TPoint>
::~LeastSquareAffineTransformEstimator()
{
  // Clear the tie points list
  this->ClearTiePoints();
}

template <class TPoint>
typename LeastSquareAffineTransformEstimator<TPoint>
::TiePointsContainerType&
LeastSquareAffineTransformEstimator<TPoint>
::GetTiePointsContainer()
{
  return m_TiePointsContainer;
}

template <class TPoint>
void
LeastSquareAffineTransformEstimator<TPoint>
::SetTiePointsContainer(const TiePointsContainerType& container)
{
  m_TiePointsContainer = container;
}

template <class TPoint>
void
LeastSquareAffineTransformEstimator<TPoint>
::ClearTiePoints()
{
  // Clear the container
  m_TiePointsContainer.clear();
}

template <class TPoint>
void
LeastSquareAffineTransformEstimator<TPoint>
::AddTiePoints(const PointType& src, const PointType& dst)
{
  // Build the tie point
  TiePointsType tpoints(src, dst);

  // Add it to the container
  m_TiePointsContainer.push_back(tpoints);
}

template <class TPoint>
void
LeastSquareAffineTransformEstimator<TPoint>
::Compute()
{
  // Get the number of tie points
  unsigned int nbPoints = m_TiePointsContainer.size();

  // Check if there are some points in
  if (nbPoints == 0)
    {
    itkExceptionMacro(<< "No tie points were given to the LeastSquareAffineTransformEstimator");
    }

  // Convenient typedefs
  typedef vnl_sparse_matrix<double> VnlMatrixType;
  typedef vnl_vector<double>        VnlVectorType;

  // Step 1: build linear systems
  // Build one linear system per dimension
  std::vector<VnlMatrixType> matrixPerDim(PointDimension, VnlMatrixType(nbPoints, PointDimension + 1));
  std::vector<VnlVectorType> vectorPerDim(PointDimension, VnlVectorType(nbPoints));

  // Iterate on points
  for (unsigned int pId = 0; pId < nbPoints; ++pId)
    {
    // Iterate on dimension
    for (unsigned int dim1 = 0; dim1 < PointDimension; ++dim1)
      {
      // Fill the vector
      vectorPerDim[dim1][pId] = static_cast<double>(m_TiePointsContainer[pId].second[dim1]);

      // Iterate on dimension (second loop)
      for (unsigned int dim2 = 0; dim2 < PointDimension; ++dim2)
        {
        matrixPerDim[dim1](pId, dim2) = static_cast<double>(m_TiePointsContainer[pId].first[dim2]);
        }

      // Fill the last column
      matrixPerDim[dim1](pId, PointDimension) = 1.;
      }
    }

  // Step 2: Solve linear systems

  for (unsigned int dim1 = 0; dim1 < PointDimension; ++dim1)
    {
    // Declare a linear system
    vnl_sparse_matrix_linear_system<double> linearSystem(matrixPerDim[dim1], vectorPerDim[dim1]);

    // Check if there are enough points to solve
    if (linearSystem.get_number_of_unknowns() > nbPoints * PointDimension)
      {
      itkExceptionMacro(<< "There are " << linearSystem.get_number_of_unknowns()
                        << " unknowns in the linear systems but only " << nbPoints
                        << " points are provided.");
      }
    otbMsgDebugMacro(<< "Number of unknowns: " << linearSystem.get_number_of_unknowns());
    otbMsgDebugMacro(<< "Number of equations: " << nbPoints);

    // A vector where the solution will be stored
    vnl_vector<double> solution(PointDimension + 1);

    // Declare the solver
    vnl_lsqr linearSystemSolver(linearSystem);

    // And solve it
    linearSystemSolver.minimize(solution);

    // Get the RMS Error for that dimension
    m_RMSError[dim1] = linearSystem.get_rms_error(solution);

    // Get the relative residual
    m_RelativeResidual[dim1] = linearSystem.get_relative_residual(solution);

    // Fill the affine transform matrix
    for (unsigned int dim2 = 0; dim2 < PointDimension; ++dim2)
      {
      m_Matrix(dim1, dim2) = static_cast<PrecisionType>(solution[dim2]);
      }
    // Fill the offset
    m_Offset[dim1] = static_cast<PrecisionType>(solution[PointDimension]);
    }

  // Set the affine transform parameters
  m_AffineTransform->SetMatrix(m_Matrix);
  m_AffineTransform->SetOffset(m_Offset);
}

template <class TPoint>
void
LeastSquareAffineTransformEstimator<TPoint>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Number of tie points: " << m_TiePointsContainer.size() << std::endl;
  os << indent << "RMS error: " << m_RMSError << std::endl;
  os << indent << "Relative residual: " << m_RelativeResidual << std::endl;
}

} // end namespace otb

#endif
