/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTimeStamp.cxx,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:29:27 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkTimeStamp.h"
#include "itkFastMutexLock.h"

namespace itk
{

/**
 * Instance creation.
 */
TimeStamp*
TimeStamp
::New()
{
  return new Self;
}
  
/** Used for mutex locking */
static SimpleFastMutexLock TimeStampMutex;
  
/**
 * Make sure the new time stamp is greater than all others so far.
 */
void 
TimeStamp
::Modified()
{
  /**
   * Initialize static member
   */
  static unsigned long itkTimeStampTime = 0;
  
  TimeStampMutex.Lock();
  m_ModifiedTime = ++itkTimeStampTime;
  TimeStampMutex.Unlock();
}

} // end namespace itk
