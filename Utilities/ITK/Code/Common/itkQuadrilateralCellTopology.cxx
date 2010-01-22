/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkQuadrilateralCellTopology.cxx,v $
  Language:  C++
  Date:      $Date: 2009-03-03 15:08:12 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkQuadrilateralCellTopology.h"

namespace itk
{


/**
 * The quadrilateral's topology data: Edges.
 */
const int
QuadrilateralCellTopology
::m_Edges[4][2] = { {0,1}, {1,2}, {2,3}, {3,0} };

QuadrilateralCellTopology
::QuadrilateralCellTopology()
{
}

QuadrilateralCellTopology
::~QuadrilateralCellTopology()
{
}

} // end namespace itk
