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

#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"
#include "otbImageKeywordlist.h"

#include <vnl/algo/vnl_svd.h>

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
SarParametricMapFunction<TInputImage, TCoordRep>
::SarParametricMapFunction()
 : m_PointSet(PointSetType::New()),
   m_IsInitialize(false),
   m_ProductWidth(0),
   m_ProductHeight(0)
{
  m_Coeff.SetSize(1, 1);
  m_Coeff.Fill(0);
}

template <class TInputImage, class TCoordRep>
void
SarParametricMapFunction<TInputImage, TCoordRep>
::SetConstantValue(const RealType& value)
{
  PointType  p0;
  p0.Fill(0);
  m_ProductWidth = 1;
  m_ProductHeight = 1;
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
  point[0] /= m_ProductWidth;
  point[1] /= m_ProductHeight;

  double result = 0;
  for (unsigned int ycoeff = m_Coeff.Rows(); ycoeff > 0; --ycoeff)
     {
     double intermediate = 0;
     for (unsigned int xcoeff = m_Coeff.Cols(); xcoeff > 0; --xcoeff)
       {
       //std::cout << "m_Coeff(" << ycoeff-1 << "," << xcoeff-1 << ") = " << m_Coeff(ycoeff-1, xcoeff-1) << std::endl;
       intermediate = intermediate * point[0] + m_Coeff(ycoeff-1, xcoeff-1);
       }
     result += vcl_pow( static_cast<double>(point[1]), static_cast<double>(ycoeff-1) ) * intermediate;
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
    const itk::MetaDataDictionary& dict = this->GetInputImage()->GetMetaDataDictionary();
    if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
      {
      ImageKeywordlist imageKeywordlist;
      itk::ExposeMetaData<ImageKeywordlist>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
      std::string nbLinesValue = imageKeywordlist.GetMetadataByKey("number_lines");
      std::string nbSamplesValue = imageKeywordlist.GetMetadataByKey("number_samples");
      m_ProductWidth = atof(nbSamplesValue.c_str());
      m_ProductHeight = atof(nbLinesValue.c_str());
      }
    else
      {
      m_ProductHeight = this->GetInputImage()->GetLargestPossibleRegion().GetSize()[0];
      m_ProductWidth  = this->GetInputImage()->GetLargestPossibleRegion().GetSize()[1];
      }

    // Perform the plane least square estimation
    unsigned int nbRecords = pointSet->GetNumberOfPoints();
    unsigned int nbCoef = m_Coeff.Rows() * m_Coeff.Cols();

    vnl_matrix<double> a(nbRecords, nbCoef);
    vnl_vector<double> b(nbRecords), bestParams(nbCoef);
    a.fill(0);
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
        double xpart = vcl_pow( static_cast<double>(point[0]) / m_ProductWidth, static_cast<double>(xcoeff));
        for (unsigned int ycoeff = 0; ycoeff < m_Coeff.Rows(); ++ycoeff)
          {
          double ypart = vcl_pow( static_cast<double>(point[1]) / m_ProductHeight, static_cast<double>(ycoeff));
          a(i, xcoeff * m_Coeff.Rows() + ycoeff) = xpart * ypart;
          //std::cout << "a(" << i << "," << xcoeff * m_Coeff.Rows() + ycoeff << ") = " <<  xpart * ypart << std::endl;
          }
        }
      }

    // Solve linear system with SVD decomposition
    vnl_svd<double> svd(a);
    bestParams = svd.solve(b);


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
::Evaluate(const PointType& point) const
{
  RealType result = itk::NumericTraits<RealType>::Zero;

  if (!m_IsInitialize)
    {
    itkExceptionMacro(<< "Must call EvaluateParametricCoefficient before evaluating");
    }
  else if (m_Coeff.Rows() * m_Coeff.Cols() == 1)
    {
    result = m_Coeff(0, 0);
    }
  else
    {
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
