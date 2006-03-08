/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkGlDrawer.h,v $
  Language:  C++
  Date:      $Date: 2002/01/15 20:24:56 $
  Version:   $Revision: 1.4 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef fltkGLDRAWERCLASS
#define fltkGLDRAWERCLASS


namespace fltk {

class ITK_EXPORT GlDrawer 
{

public:
  GlDrawer() {};
  virtual ~GlDrawer() {};
  virtual void glDraw(void) const =0;
};

typedef GlDrawer * GlDrawerPtr;

}


#endif
