/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkPolygonGroupOrientation.h,v $
  Language:  C++
  Date:      $Date: 2005/12/08 19:47:44 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkPolygonGroupOrientation_h
#define _itkPolygonGroupOrientation_h
namespace itk {
/**
 * \enum PolygonGroupOrientation
 * \brief enumerates the possible spatial orientations
 */
typedef enum {
  Axial = 0,
  Coronal = 1,
  Sagittal = 2,
  UserPlane = 3,
  Unknown = 4
} PolygonGroupOrientation;
}

#endif
