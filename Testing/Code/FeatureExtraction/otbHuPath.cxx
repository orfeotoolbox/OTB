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

#include "otbImageFileReader.h"
#include "otbHuPathFunction.h"
#include "itkPolyLineParametricPath.h"
#include "itkExceptionObject.h"

int otbHuPath( int argc, char * argv[] )
{
  try 
    { 
        unsigned int              Number;
        const   unsigned int      Dimension = 2;
	typedef itk::PolyLineParametricPath< Dimension >     PathType;
	typedef otb::HuPathFunction<PathType>                FunctionType;
	typedef FunctionType::RealType                       RealType;
  
		
        // Dessiner un carré:
	PathType::ContinuousIndexType cindex;
	PathType::Pointer pathElt = PathType::New();

 	pathElt->Initialize();

        cindex[0]=30;
        cindex[1]=30;
        pathElt->AddVertex(cindex);
        cindex[0]= 30;
        cindex[1]=130;
        pathElt->AddVertex(cindex);
        cindex[0]=130;
        cindex[1]=130;
        pathElt->AddVertex(cindex);
        cindex[0]=130;
        cindex[1]= 30;
        pathElt->AddVertex(cindex);

	FunctionType::Pointer function =FunctionType::New();

	RealType Result;
	function->SetInputPath( pathElt );
	
	for (Number = 1 ;Number<8;Number++)
	  {
           //OTB-FA-00024-CS
	   function->SetMomentNumber(Number);
           Result = function->Evaluate();
	   std::cout << "Hu("<<Number<<") = "<< Result <<std::endl;
	  }
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "Exception levee inconnue !" << std::endl; 
    return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;
}

