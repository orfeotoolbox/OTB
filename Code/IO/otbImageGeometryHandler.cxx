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
 *2 méthodes nécessaires pour l'ortho (au niveau de l'instanciation des modèles de capteurs).
 * 
 * 
 */
#include "otbImageGeometryHandler.h"

namespace otb

{     /***************************/
      /*     Constructeurs       */
      /***************************/
      
// Constructeur par défault
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
      /*     Méthodes            */
      /***************************/
//Ouvrir le fichier: On spécifie le nom de l'image en paramètres
//Encapsulation de la méthode "open"
void ImageGeometryHandler::SetFileName(char *src)
{
std::cout << "Creation handler " << std::endl; std::cout.flush();
 handler = ossimImageHandlerRegistry::instance()->open(ossimFilename(src));
      if(!handler)
      {
         cout << "Unable to open input image: " << src << endl;
      }
}
        
// Récupération de la géométrie de l'image: 
//Encapsulation de la méthode "getImageGeometry"
ossimKeywordlist ImageGeometryHandler::GetGeometryKeywordlist()
{
std::cout << "Get geometry handler " << std::endl; std::cout.flush();
handler->getImageGeometry(m_geom_kwl);
return m_geom_kwl;
}    

}//fin namespace



