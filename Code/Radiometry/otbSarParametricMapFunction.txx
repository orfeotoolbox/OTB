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

#ifndef __otbSarParametricMapFunction_txx
#define __otbSarParametricMapFunction_txx

#include "otbSarParametricMapFunction.h"
#include "itkNumericTraits.h"


#include <vnl/algo/vnl_lsqr.h>
#include <vnl/vnl_sparse_matrix_linear_system.h>
#include <vnl/vnl_least_squares_function.h>



namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
SarParametricMapFunction<TInputImage, TCoordRep>
::SarParametricMapFunction()
{
  m_PointSet = PointSetType::New();
  m_Coeff.SetSize(1,1);
  m_Coeff.Fill(0);
  m_IsInitialize = false;
  m_UsingClosestPointMethod = false;
}

template <class TInputImage, class TCoordRep>
void
SarParametricMapFunction<TInputImage, TCoordRep>
::SetConstantValue(const RealType& value)
{
  PointType  p0;
  p0.Fill(0);
  m_IsInitialize = false;
  m_PointSet->Initialize();
  m_PointSet->SetPoint(0, p0);
  m_PointSet->SetPointData(0, value);
  EvaluateParametricCoefficient();
  this->Modified();
}


template <class TInputImage, class TCoordRep>
void
SarParametricMapFunction<TInputImage, TCoordRep>
::SetPolynomalSize(const IndexType polynomalSize)
{
  m_Coeff.SetSize(polynomalSize[1] + 1, polynomalSize[0] + 1);
  m_Coeff.Fill(0);
  this->Modified();
}

template <class TInputImage, class TCoordRep>
double
SarParametricMapFunction<TInputImage, TCoordRep>
::Horner(PointType point) const
{
  // Implementation of a Horner scheme evaluation for bivariate polynomial
  //std::cout << "Horner " << std::endl;

  const typename InputImageType::RegionType& region = this->GetInputImage()->GetLargestPossibleRegion();
  const typename InputImageType::IndexType&  origin = region.GetIndex();
  const typename InputImageType::SizeType&   size   = region.GetSize();

  point[0] = (point[0] - origin[0]) / size[0];
  point[1] = (point[1] - origin[1]) / size[1];

  //std::cout << "point = " << point << std::endl;

  double result = 0;
  for (unsigned int ycoeff = m_Coeff.Rows(); ycoeff > 0 ; --ycoeff)
     {
     double intermediate = 0;
     for (unsigned int xcoeff = m_Coeff.Cols(); xcoeff > 0 ; --xcoeff)
       {
       //std::cout << "m_Coeff(" << ycoeff-1 << "," << xcoeff-1 << ") = " << m_Coeff(ycoeff-1, xcoeff-1) << std::endl;
       intermediate = intermediate * point[0] + m_Coeff(ycoeff-1, xcoeff-1);
       }
     result += vcl_pow( point[1], ycoeff-1 ) * intermediate;
     }

  return result;
}

template <class TInputImage, class TCoordRep>
void
SarParametricMapFunction<TInputImage, TCoordRep>
::EvaluateParametricCoefficient()
{
  PointSetPointer pointSet = this->GetPointSet();

  PointType coef;
  PointType point;
  coef.Fill(0);
  point.Fill(0);
  typename PointSetType::PixelType pointValue;
  pointValue = itk::NumericTraits<PixelType>::Zero;

  if (pointSet->GetNumberOfPoints() == 0)
    {
    itkExceptionMacro(<< "PointSet must be set before evaluating the parametric coefficient (at least one value)");
    }
  else if (pointSet->GetNumberOfPoints() == 1)
    {
    pointSet->GetPointData(0, &pointValue);
    m_Coeff(0, 0) = pointValue;
    }
  else
    {
    // Get input region for normalization of coordinates
    const typename InputImageType::RegionType& region = this->GetInputImage()->GetLargestPossibleRegion();
    const typename InputImageType::IndexType&  origin = region.GetIndex();
    const typename InputImageType::SizeType&   size   = region.GetSize();

    //std::cout << "size = " << size << std::endl;
    //std::cout << "origin = " << origin << std::endl;

    // Perform the plane least square estimation
    unsigned int nbRecords = pointSet->GetNumberOfPoints();
    unsigned int nbCoef = m_Coeff.Rows() * m_Coeff.Cols();

    vnl_sparse_matrix<double> a(nbRecords, nbCoef);
    vnl_vector<double> b(nbRecords), bestParams(nbCoef);
    b.fill(0);
    bestParams.fill(0);

    // Fill the linear system
    for (unsigned int i = 0; i < nbRecords; ++i)
      {
      this->GetPointSet()->GetPoint(i, &point);
      this->GetPointSet()->GetPointData(i, &pointValue);
      b(i) = pointValue;
      //std::cout << "point = " << point << std::endl;
      //std::cout << "b(" << i << ") = " << pointValue << std::endl;

      for (unsigned int xcoeff = 0; xcoeff < m_Coeff.Cols(); ++xcoeff)
        {
        double xpart = vcl_pow( (point[0] - origin[0]) / size[0], xcoeff);
        for (unsigned int ycoeff = 0; ycoeff < m_Coeff.Rows(); ++ycoeff)
          {
          double ypart = vcl_pow( (point[1] - origin[1]) / size[1], ycoeff);
          a(i, xcoeff * m_Coeff.Rows() + ycoeff) = xpart * ypart;
          //std::cout << "a(" << i << "," << xcoeff * m_Coeff.Rows() + ycoeff << ") = " <<  xpart * ypart << std::endl;
          }
        }
      }

    // Create the linear system
    vnl_sparse_matrix_linear_system<double> linearSystem(a, b);

    // And solve it
    vnl_lsqr linearSystemSolver(linearSystem);
    linearSystemSolver.minimize(bestParams);

    for (unsigned int xcoeff = 0; xcoeff < m_Coeff.Cols(); ++xcoeff)
      {
      for (unsigned int ycoeff = 0; ycoeff < m_Coeff.Rows(); ++ycoeff)
        {
        m_Coeff(ycoeff, xcoeff) = bestParams(xcoeff * m_Coeff.Rows() + ycoeff);
        //std::cout << "m_Coeff(" << ycoeff << "," << xcoeff << ") = " << m_Coeff(ycoeff, xcoeff) << std::endl;
        }
      }
    }
  m_IsInitialize = true;
}

/**
 *
 */
template <class TInputImage, class TCoordRep>
typename SarParametricMapFunction<TInputImage, TCoordRep>
::RealType
SarParametricMapFunction<TInputImage, TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{
  RealType result = itk::NumericTraits<RealType>::Zero;

  if (!this->GetInputImage())
    {
    itkExceptionMacro(<< "No input image specified");
    }
  else if (!this->GetInputImage()->GetLargestPossibleRegion().IsInside(index))
    {
    result = itk::NumericTraits<RealType>::max();
    }
  else if (!m_IsInitialize)
    {
    itkExceptionMacro(<< "Must call EvaluateParametricCoefficient before evaluating");
    }
  else if(!m_UsingClosestPointMethod)
    {
    PointType point;
    point[0] = static_cast<typename PointType::ValueType>(index[0]);
    point[1] = static_cast<typename PointType::ValueType>(index[1]);
    result = this->Horner(point);
    }
  return result;
}


/**
 *
 */
template <class TInputImage, class TCoordRep>
void
SarParametricMapFunction<TInputImage, TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "Polynom coefficients: "  << m_Coeff << std::endl;
}


} // end namespace otb

#endif
