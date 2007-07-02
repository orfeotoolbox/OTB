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
/*!otbImageGeometryHandler.h
 *
 * 
 * PURPOSE:
 *
 * 
 */
// iostream is used for general output
//#include <iostream>
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <iterator>

#include "base/ossimKeywordlist.h"
#include "base/ossimFilename.h"
#include "base/ossimString.h"
#include "base/ossimDrect.h"
#include "support_data/ossimFfL7.h"
#include "imaging/ossimImageHandlerRegistry.h"
#include "imaging/ossimImageHandler.h"
#include "projection/ossimMapProjectionInfo.h"
#include "projection/ossimMapProjection.h"
#include "projection/ossimAdjMapModel.h"

// this is the most important class and is called as the first line of all applications.
// without this alll the important factories are not created.
#include "init/ossimInit.h"
#include "itkExceptionObject.h"
#include "itkMacro.h"
#include "itkSmartPointer.h"
#include "itkObject.h"

namespace otb
{	         
class ITKCommon_EXPORT ImageGeometryHandler: public itk::Object
{          
public :
//Déclaration des types:
  typedef ImageGeometryHandler                       Self;
  typedef itk::Object                                Superclass;

  typedef itk::SmartPointer<Self>                    Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;

static Pointer New()
{ Pointer n = new Self; n->UnRegister(); return n; }

itkTypeMacro( ImageGeometryHandler, Object);
//itkNewMacro( Self );

/***********************************/
/*   Déclaration des méthodes      */
/***********************************/
//Ouverture d'une image à partir d'un nom de fichier
void SetFileName(char *src);

//Récupération de la keywordlist de l'image:
ossimKeywordlist GetGeometryKeywordlist();

// //Récupération du model de projection(spécifique à AdjMapModel):
// const ossimMapProjection* ImageGeometryHandler::GetProjection() const;
// 
// //Récupération de ImageSize(spécifique à AdjMapModel):
// const ossimIpt ImageGeometryHandler::GetImageSize() const;
// 
// //Récupération du GeoFilename:
// const ossimFilename ImageGeometryHandler::GetGeometryFileName() const;
// 
// //Récupération du HeaderFile de type Ffl7:
// const ossimFfL7 ImageGeometryHandler::GetFfL7();
// 
// //Récupération du SpotDimapSupportData:

protected:
ImageGeometryHandler();
//ImageGeometryHandler(const char *src);
virtual ~ImageGeometryHandler();

ossimImageHandler *handler;
ossimKeywordlist m_geom_kwl;
ossimFilename m_filename;
};//Fin définition de la classe

}//fin namespace


#endif
