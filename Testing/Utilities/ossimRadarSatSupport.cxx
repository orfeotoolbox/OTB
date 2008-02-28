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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

/*!
 *
 * PURPOSE:
 *
 * Application pour projeter une r�gion d'une image en coordonn�es g�ographiques 
 * en utilisant un Interpolator+regionextractor et un Iterator. 
 * 
 */

// iostream is used for general output
#include <iostream>
#include <iterator>
#include <stdlib.h>

#include "base/ossimKeywordlist.h"
#include "init/ossimInit.h"
#include "imaging/ossimImageHandler.h"
#include "projection/ossimProjection.h"
#include "projection/ossimProjectionFactoryRegistry.h"
#include "imaging/ossimImageHandlerRegistry.h"
#include "imaging/ossimImageHandlerSarFactory.h"
// this is the most important class and is called as the first line of all applications.
// without this all the important factories are not created.
//#include "init/ossimInit.h"



int ossimRadarSatSupport( int argc, char* argv[] )
{
  try 
    {        

        ossimInit::instance()->initialize(argc, argv);

        if(argc<2)
        {
			/*
			 * Verification que l'utilisateur passe bien un fichier en parametre de l'application
			 */
                std::cout << argv[0] <<" <input filename> " << std::endl;

                return EXIT_FAILURE;
        }
  
		ossimImageHandlerRegistry::instance()->addFactory(ossimImageHandlerSarFactory::instance());
		/*
		 * Lecture du fichier passé en parametre
		 */
        ossimImageHandler *handler = ossimImageHandlerRegistry::instance()->open(ossimFilename(argv[1])); 
		/*
		 * Verification que la lecture est effectuée
		 */
        if(!handler)
        {
                std::cout<<"Unable to open input image "<<argv[1]<< std::endl;
        }  
   
		/*
		 * Recuperation des métadonnées 
		 */
        ossimKeywordlist geom;
        std::cout<<"Read ossim Keywordlist..."<< std::endl;
        handler->getImageGeometry(geom);
	
	std::cout<<geom<<std::endl;

        ossimGpt ossimGPoint(0,0);
        ossimDpt ossimDPoint;
        std::cout<<"Creating projection..."<< std::endl;
        ossimProjection * model = NULL;
		/*
		 * Creation d'un modèle de projection à partir des métadonnées
		 */
        model = ossimProjectionFactoryRegistry::instance()->createProjection(geom);
		/*
		 * Verification de l'existance du modèke de projection
		 */
        if( model == NULL)
        {
                std::cout<<"Invalid Model * == NULL !"<< std::endl;
        }

        std::cout<<"Creating RefPtr of projection..."<< std::endl;
        ossimRefPtr<ossimProjection> ptrmodel = model;
        if( ptrmodel.valid() == false )
        {
                std::cout<<"Invalid Model pointer .valid() == false !"<< std::endl;
        }
		
		/*
		 * Localisation d'un point de la donnée passé en parametre
		 */
		//if (argc = 4)
		{
			int i = 7040;
			int j = 19641;

			if(model != NULL)
			{
				ossimDpt image(i,j);
				ossimDpt imageret;
				ossimGpt world;
				double height = 0;
				model->lineSampleHeightToWorld(image, height, world);
				std::cout<<"longitude = "<<world.lon<<"latitude = "<<world.lat<<std::endl;

				model->worldToLineSample(world,imageret);
				std::cout<<"x = "<<imageret.x<<"y = "<<imageret.y<<std::endl;
			}
		}
    } 
  catch( std::bad_alloc & err ) 
    { 
    std::cout << "Exception bad_alloc : "<<(char*)err.what()<< std::endl; 
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "Exception levee inconnue !" << std::endl; 
    return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;

 }//Fin main()

