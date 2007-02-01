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

#include <math.h>

#include "otbOrientationPathFunction.h"
#include "itkPolyLineParametricPath.h"
#include "itkExceptionObject.h"

int otbOrientationPath( int argc, char * argv[] )
{
  try 
    { 
        double        Theta((double)::atof(argv[1]));

        const   unsigned int                                   Dimension = 2;
	typedef itk::PolyLineParametricPath< Dimension >       PathType;
	typedef otb::OrientationPathFunction<PathType>         FunctionType;
	typedef FunctionType::RealType                         RealType;
  
	PathType::ContinuousIndexType cindex;
	PathType::Pointer pathElt = PathType::New();

        Theta *= acos(-1.0)/180.;
	
 	pathElt->Initialize();

        cindex[0]=30;
        cindex[1]=30;
        pathElt->AddVertex(cindex);
        cindex[0] += 100 * cos(Theta);
        cindex[1] += 100 * sin(Theta);
        pathElt->AddVertex(cindex);

	FunctionType::Pointer function =FunctionType::New();
        function->SetInputPath( pathElt );

	RealType ResultTheta = function->Evaluate();
	std::cout.precision(10);
	std::cout << "Orientation found : " << ResultTheta <<std::endl;
	if( fabs(static_cast<double>(ResultTheta-Theta))>=10e-15)
	{
		std::cout << "Error in Theta estimation:" <<(ResultTheta-Theta)<<std::endl;
		return EXIT_FAILURE;
	}
	
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "itk::ExceptionObject catch !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "unknown Exception catch !" << std::endl; 
    return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;
}

