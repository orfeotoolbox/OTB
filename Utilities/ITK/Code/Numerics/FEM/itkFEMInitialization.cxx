/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFEMInitialization.cxx,v $
  Language:  C++
  Date:      $Date: 2009-01-29 21:28:16 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkFEMInitialization.h"

namespace itk {
namespace fem {

unsigned int FEMInitialization::count = 0;

/**
 * \brief Register all Load implementations of all Element classes.
 */
extern void LoadImplementationsRegister(void);

/**
 * Constructor of the FEMInitialization class does all
 * the initialization.
 */
FEMInitialization::FEMInitialization()
{ 
  if ( 0 == count++)
  {
    // Perform initialization


    // Register all loads with the VisitorDispatcher class
    LoadImplementationsRegister();

  }
}

/**
 * Destructor of the FEMInitialization class does all
 * the cleanup required by the FEM library.
 */
FEMInitialization::~FEMInitialization()
{
  if ( 0 == --count)
    {
    // perform the cleanup and housekeeping
    }
}

}} // end namespace itk::fem
