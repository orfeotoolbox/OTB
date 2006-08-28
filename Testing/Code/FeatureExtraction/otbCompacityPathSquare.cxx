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

#include "otbCompacityPathFunction.h"
#include "itkPolyLineParametricPath.h"
#include "itkExceptionObject.h"

int otbCompacityPathSquare( int argc, char * argv[] )
{
  try 
    { 
        double        A ((double)::atof(argv[1]));

        unsigned int                                           Number;
        const   unsigned int                                   Dimension = 2;
	typedef itk::PolyLineParametricPath< Dimension >       PathType;
	typedef otb::CompacityPathFunction<PathType>           FunctionType;
	typedef FunctionType::RealType                         RealType;
  
	PathType::ContinuousIndexType cindex;
	PathType::Pointer pathElt = PathType::New();

        if(A<0)
	{
             std::cout << "square must be greater than 0.0 !" << std::endl; 
             return EXIT_FAILURE;
	} 

	
 	pathElt->Initialize();

       cindex[0]= 100;
       cindex[1]= 100;
       pathElt->AddVertex(cindex);	
       cindex[0]= 100+A;
       cindex[1]= 100;
       pathElt->AddVertex(cindex);	
       cindex[0]= 100+A;
       cindex[1]= 100+A;
       pathElt->AddVertex(cindex);	
       cindex[0]= 100;
       cindex[1]= 100+A;
       pathElt->AddVertex(cindex);	
        

	FunctionType::Pointer function =FunctionType::New();
        function->SetInputPath( pathElt );

	RealType Result = function->Evaluate();
	std::cout << "Compacity result: " << Result <<std::endl;

	RealType Error;
	Error = fabs(Result - static_cast<RealType>(acos(-1.0)/4.) );
	
	if(  Error > 1.E-9)
	{
		std::cout << "Error in Theta estimation :" << std::endl;
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

