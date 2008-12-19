/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkHexahedronCellTopology.cxx,v $
  Language:  C++
  Date:      $Date: 2008-10-17 01:21:45 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkHexahedronCellTopology.h"

namespace itk
{


/**
 * The hexahedron's topology data: Edges.
 */
const int
HexahedronCellTopology
::m_Edges[12][2] = 
{ {0,1}, {1,2}, {3,2}, {0,3}, 
  {4,5}, {5,6}, {7,6}, {4,7},
  {0,4}, {1,5}, {3,7}, {2,6}
};

/**
 * The hexahedron's topology data: Faces.
 */
const int
HexahedronCellTopology
::m_Faces[6][4] = 
{ {0,4,7,3}, {1,2,6,5},
  {0,1,5,4}, {3,7,6,2},
  {0,3,2,1}, {4,5,6,7} 
};


HexahedronCellTopology
::HexahedronCellTopology()
{
}

HexahedronCellTopology
::~HexahedronCellTopology()
{
}


} // end namespace itk
