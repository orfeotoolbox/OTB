/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTetrahedronCellTopology.cxx,v $
  Language:  C++
  Date:      $Date: 2009-03-03 15:09:44 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkTetrahedronCellTopology.h"

namespace itk
{


/**
 * The tetrahedron's topology data: Faces
 */
const int
TetrahedronCellTopology
::m_Faces[4][3] = { {0,1,3}, {1,2,3}, {2,0,3}, {0,2,1} };


/**
 * The tetrahedron's topology data: Faces
 */
const int
TetrahedronCellTopology
::m_Edges[6][2] = { {0,1}, {1,2}, {2,0}, {0,3}, {1,3}, {2,3} };

TetrahedronCellTopology
::TetrahedronCellTopology()
{
}

TetrahedronCellTopology
::~TetrahedronCellTopology()
{
}

} // end namespace itk
