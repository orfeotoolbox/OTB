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
#ifndef __otbLineOfSightOptimizer_txx
#define __otbLineOfSightOptimizer_txx

#include "otbLineOfSightOptimizer.h"

namespace otb
{

template <class TPrecision, class TLabel>
LineOfSightOptimizer<TPrecision,TLabel>
::LineOfSightOptimizer()
{
  m_Residues.clear();
  
  m_GlobalResidue = 0;
  
  m_InvCumul = vnl_matrix<PrecisionType>(3,3);
  
  m_Identity = vnl_matrix<PrecisionType>(3,3);
  m_Identity.fill(0);
  m_Identity.fill_diagonal(1.);
  
  m_SecCumul = vnl_vector<PrecisionType>(3);
}

template <class TPrecision, class TLabel>
typename LineOfSightOptimizer<TPrecision,TLabel>::PointType
LineOfSightOptimizer<TPrecision,TLabel>
::Compute(PointSetPointerType pointA, PointSetPointerType pointB)
{
  // First, empty the cumulators and residues
  m_InvCumul.fill(0);
  m_SecCumul.fill(0);
  m_Residues.clear();
  
  vnl_matrix<PrecisionType> idMinusViViT(3,3);
  vnl_matrix<PrecisionType> vi(3,1);
  vnl_vector<PrecisionType> si(3);
  
  PointType result;
  
  //check inputs
  if (pointA->GetNumberOfPoints() != pointB->GetNumberOfPoints() ||
      pointA->GetNumberOfPoints() < 2)
    {
    itkExceptionMacro(<<"Points are missing in at least one of the input point sets.");
    return result;
    }
  
  // iterate over lines of sight
  PointSetConstIteratorType itPointA = pointA->GetPoints()->Begin();
  PointSetConstIteratorType itPointB = pointB->GetPoints()->Begin();
  
  while (itPointA != pointA->GetPoints()->End() &&
         itPointB != pointB->GetPoints()->End())
    {
    vi(0,0) = itPointB.Value()[0] - itPointA.Value()[0];
    vi(1,0) = itPointB.Value()[1] - itPointA.Value()[1];
    vi(2,0) = itPointB.Value()[2] - itPointA.Value()[2];
    
    PrecisionType norm_inv = 1. / vcl_sqrt(vi(0,0)*vi(0,0)+vi(1,0)*vi(1,0)+vi(2,0)*vi(2,0));
    
    vi(0,0) *= norm_inv;
    vi(1,0) *= norm_inv;
    vi(2,0) *= norm_inv;
    
    si(0) = itPointA.Value()[0];
    si(1) = itPointA.Value()[1];
    si(2) = itPointA.Value()[2];
    
    idMinusViViT = m_Identity - (vi * vi.transpose());
    
    m_InvCumul+=idMinusViViT;
    m_SecCumul+=(idMinusViViT * si);
    
    ++itPointA;
    ++itPointB;
    }
  
  vnl_matrix_inverse<PrecisionType> inv(m_InvCumul);
  vnl_vector<PrecisionType> intersection = inv.inverse() * m_SecCumul;
  
  result[0] = intersection[0];
  result[1] = intersection[1];
  result[2] = intersection[2];
  
  // Compute residues
  m_GlobalResidue = 0;
  
  vnl_vector<PrecisionType> AB(3);
  vnl_vector<PrecisionType> AC(3);
  PrecisionType res2;
  itPointA = pointA->GetPoints()->Begin();
  itPointB = pointB->GetPoints()->Begin();
  while (itPointA != pointA->GetPoints()->End() &&
         itPointB != pointB->GetPoints()->End())
  {
    AB[0] = itPointB.Value()[0] - itPointA.Value()[0];
    AB[1] = itPointB.Value()[1] - itPointA.Value()[1];
    AB[2] = itPointB.Value()[2] - itPointA.Value()[2];
    
    AC[0] = intersection[0]   - itPointA.Value()[0];
    AC[1] = intersection[1]   - itPointA.Value()[1];
    AC[2] = intersection[2]   - itPointA.Value()[2];
    
    res2 = dot_product(AC,AC) - dot_product(AB,AC) / (dot_product(AB,AB));
    
    m_Residues.push_back( vcl_sqrt( res2 ) );
    m_GlobalResidue += res2;
    
    ++itPointA;
    ++itPointB;
  }
  
  m_GlobalResidue = vcl_sqrt(m_GlobalResidue);
   
  return result;
}

}

#endif
