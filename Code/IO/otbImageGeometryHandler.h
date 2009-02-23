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
#ifndef __otbImageGeometryHandler_h
#define __otbImageGeometryHandler_h


#include "base/ossimKeywordlist.h"
#include "base/ossimFilename.h"

#include "imaging/ossimImageHandler.h"

#include "itkExceptionObject.h"
#include "itkMacro.h"
#include "itkObject.h"

/**
 * \class ImageGeometryHandler
 * \brief used for orthorectification
 *
 */

namespace otb
{
class ITK_EXPORT ImageGeometryHandler: public itk::Object
{
public :

  typedef ImageGeometryHandler                       Self;
  typedef itk::Object                                Superclass;

  typedef itk::SmartPointer<Self>                    Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;

  static Pointer New()
  {
    Pointer n = new Self;
    n->UnRegister();
    return n;
  }

  itkTypeMacro( ImageGeometryHandler, Object);


  /** Open image from filename*/
  void SetFileName(char *src);

  /** Get the image keyword list*/
  ossimKeywordlist GetGeometryKeywordlist();


protected:
  ImageGeometryHandler() {};
  virtual ~ImageGeometryHandler() {};

  ossimImageHandler *handler;
  ossimKeywordlist m_geom_kwl;
  ossimFilename m_filename;
};

}


#endif
