/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   20 mars 2006
  Version   :   
  Role      :   
  $Id$

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbImageFileReader.h"
#include "otbComplexMomentPathFunction.h"
#include "itkExceptionObject.h"
#include "itkPolyLineParametricPath.h"

int otbComplexMomentPath( int argc, char ** argv )
{
  try 
    { 
        unsigned int  p((unsigned int)::atoi(argv[1]));
        unsigned int  q((unsigned int)::atoi(argv[2]));
       
        const   unsigned int      Dimension = 2;
	  
	typedef itk::PolyLineParametricPath< Dimension >	        PathType;
	typedef otb::ComplexMomentPathFunction< PathType >              CMType;
	typedef CMType::ComplexType                                     ComplexType;

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

	CMType::Pointer function =CMType::New();

	function->SetQ(q);
	function->SetP(p);
	
	ComplexType Result;
	
        Result = function->Evaluate( *pathElt);
        std::cout << "function->Evaluate(Path)"<< Result << std::endl;	
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

