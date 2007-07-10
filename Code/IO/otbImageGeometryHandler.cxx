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
/*!
 *
 * PURPOSE:
 *2 m�thodes n�cessaires pour l'ortho (au niveau de l'instanciation des mod�les de capteurs).
 * 
 * 
 */
#include "otbImageGeometryHandler.h"
#include "otbMacro.h"

namespace otb

{     /***************************/
      /*     Constructeurs       */
      /***************************/
      
// Constructeur par d�fault
ImageGeometryHandler::ImageGeometryHandler()
{
//handler = new ImageGeometryHandler();
}

// //Constructeur en connaissant le nom de l'image
// ImageHandler::ImageHandler(const char *src)
//  { 
//  handler   = ossimImageHandlerRegistry::instance()->open(ossimFilename(src));
//       if(!handler)
//       {
//          cout << "Unable to open input image: " << src << endl;
//       }
//  }
 
      /***************************/
      /*     Destructeurs        */
      /***************************/
ImageGeometryHandler::~ImageGeometryHandler()
{
delete handler;
}
      
      /***************************/
      /*     M�thodes            */
      /***************************/
//Ouvrir le fichier: On sp�cifie le nom de l'image en param�tres
//Encapsulation de la m�thode "open"
void ImageGeometryHandler::SetFileName(char *src)
{
	otbDebugMacro(<<"Creation handler... ");
 	handler = ossimImageHandlerRegistry::instance()->open(ossimFilename(src));
    if(!handler)
    {
		itkExceptionMacro(<< "Unable to open input image: " << src);
    }
}
        
// R�cup�ration de la g�om�trie de l'image: 
//Encapsulation de la m�thode "getImageGeometry"
ossimKeywordlist ImageGeometryHandler::GetGeometryKeywordlist()
{
otbDebugMacro( << "Get geometry handler " );
handler->getImageGeometry(m_geom_kwl);
return m_geom_kwl;
}    

}//fin namespace



