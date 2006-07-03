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

//
// Cet exemple permet de montrer l'utilisation des différentes bibliothèques utilisées par l'OTB
//
#include "otbBonjour.h"


/* Includes pour tester FLTK */
/*
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
*/


/* Includes pour tester VTK */
/*#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
*/

#include <iostream>
#include <string>
#include "otbGDALImageIO.h"

int main(int argc, char * argv[])
{
	//OTB
	otb::Bonjour lBonjour;
	
  	otb::GDALImageIO::Pointer lGDALImageIO = otb::GDALImageIO::New();
        bool lCanRead = lGDALImageIO->CanReadFile(argv[1]);
        if ( lCanRead == false)
        {
                std::cerr << "Cannot  open image file ["<<argv[1] <<"].");
                return EXIT_FAILURE;
        }

	
        return EXIT_SUCCESS;
}


