/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMeshRegion.cxx,v $
  Language:  C++
  Date:      $Date: 2009-02-05 22:04:06 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkMeshRegion.h"

namespace itk
{

/**
 * Instantiate object.
 */
MeshRegion
::MeshRegion()
{
  m_NumberOfRegions = 0;
  m_Region = 0;
}

/**
 * Destructor for the MeshRegion class.
 */
MeshRegion
::~MeshRegion()
{
}

} // end namespace itk
