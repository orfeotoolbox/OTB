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

#include "itkExceptionObject.h"
#include "itkImage.h"

#include "otbImageFileReader.h"
#include "otbComplexMomentPathFunction.h"
#include "itkPolylineParametricPath.h"

int otbComplexMomentPath( int argc, char ** argv )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        unsigned int  p((unsigned int)::atoi(argv[2]));
        unsigned int  q((unsigned int)::atoi(argv[3]));
       
        typedef unsigned char     InputPixelType;
        const   unsigned int      Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >  InputImageType;
	typedef InputImageType::PointType                 ImagePointType;
        typedef otb::ImageFileReader< InputImageType  >   ReaderType;
	  
	typedef itk::PolyLineParametricPath< Dimension >	          PathType;
	typedef std::complex<double>                                      ComplexType
	typedef otb::ComplexMomentPathFunction<InputImageType,PathType>   CMType;
  
        ReaderType::Pointer reader         = ReaderType::New();	
        reader->SetFileName( inputFilename  );

        // Dessiner un carré:
	ImagePointType                pos;
	PathType::ContinuousIndexType cindex;
	PathType::Pointer pathElt = PathType::New();
        pos[0]=30;
        pos[1]=30;
	image->TransformPhysicalPointToContinuousIndex(pos,cindex);
        pathElt->AddVertex(cindex);
        pos[0]= 30;
        pos[1]=130;
	image->TransformPhysicalPointToContinuousIndex(pos,cindex);
        pathElt->AddVertex(cindex);
        pos[0]=130;
        pos[1]=130;
	image->TransformPhysicalPointToContinuousIndex(pos,cindex);
        pathElt->AddVertex(cindex);
        pos[0]=130;
        pos[1]= 30;
	image->TransformPhysicalPointToContinuousIndex(pos,cindex);
        pathElt->AddVertex(cindex);



	CMType::Pointer function =CMType::New();
	function->SetInputImage( reader->GetInput() );
	function->SetInputPath( pathElt );

	function->SetQ(q);
	function->SetP(p);
	
	InputImageType::IndexType index;
	index[0]=10;
	index[1]=10;
	
	ComplexType Result;
	
        Result = function->EvaluateAtIndex( index );
        std::cout << "function->EvaluateAtIndex( index ): " << Result << std::endl;
	
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

