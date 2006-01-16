//
// Cet exemple permet de montrer l'utilisation des différentes bibliothèques utilisées par l'OTB
//
#include "otbBonjour.h"

#define MAIN
extern "C"
{
#include "cai_image.h"
}

/*
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
*/


#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkRenderer.h"



#include <iostream>
#include <iostream>
#include <string>

int main()
{
	//OTB
	otb::Bonjour lBonjour;
	
	//CAI
	CAI_IMAGE * lCai=NULL;
	int lNbCanaux,lNbOctPix,lNbLignes,lNbColonnes;
	char lImage[1024]="image.lum";
	char lFormatImage[1024]="AUTO";
	lCai = cai_ouvre_lecture_image(lImage,lFormatImage,&lNbCanaux,&lNbOctPix,&lNbColonnes,&lNbLignes);

	//FLTK
//	Fl_Window lWindow(400, 400);

	//VTK - Code extrait de VTK\Examples\Tutorial\Step1\Cxx\Cone.cxx
  	vtkConeSource *cone = vtkConeSource::New();
  	cone->Delete();

	
	return 0;
}


