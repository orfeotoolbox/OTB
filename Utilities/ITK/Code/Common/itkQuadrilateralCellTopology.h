/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkQuadrilateralCellTopology.h,v $
  Language:  C++
  Date:      $Date: 2009-03-03 15:08:20 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkQuadrilateralCellTopology_h
#define __itkQuadrilateralCellTopology_h
#include "itkWin32Header.h"

namespace itk
{

/** \class QuadrilateralCellTopology
 * QuadrilateralCellTopology holds data defining the topological 
 * connections of the vertices and edges of an Quadrilateral Cell.
 *
 * This class is used to localize static variables out of .txx 
 * files. This prevents multiple definition of static variables.
 *
 * \ingroup MeshObjects
 */

class  ITKCommon_EXPORT QuadrilateralCellTopology
{
public:
  QuadrilateralCellTopology();
  ~QuadrilateralCellTopology();
  
protected:
  /** Quadrilateral topology data. */
  static const int m_Edges[4][2];
   
};

} // end namespace itk


#endif
