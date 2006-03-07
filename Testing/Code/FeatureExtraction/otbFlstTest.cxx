/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   20 fevrier 2006
  Version   :   
  Role      :   Tester le portage de la fonction FLST de mÃ©gawave vers l'OTB
  $Id: $

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
  
#include "itkExceptionObject.h"
#include "itkImageFileReader.h"
#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkPolyLineParametricPath.h"
#include "itkTreeNode.h"
#include "otbTreeSource.h"
#include "otbImageToTreeFilter.h"


#include "otbFlst.h"
#include "itkTreeContainer.h"

int otbFlstTest( int argc, char ** argv )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
       

        typedef unsigned char                                   InputPixelType;
        typedef unsigned char   	                        OutputPixelType;
	typedef float						RealPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >	InputImageType;
        typedef  itk::Image< OutputPixelType, Dimension >	OutputImageType;
        typedef itk::Image< RealPixelType,  Dimension >		RealImageType;
        typedef itk::ImageFileReader< InputImageType  >         ReaderType;
        typedef itk::ImageFileWriter< OutputImageType >         WriterType;
	typedef itk::PolyLineParametricPath<Dimension>          PathType;
//	typedef itk::TreeNode<PathType>                         TreeType;

        ReaderType::Pointer reader = ReaderType::New();	
        WriterType::Pointer writer = WriterType::New();	
        reader->SetFileName( inputFilename  );
        writer->SetFileName( outputFilename  );

	
        typedef otb::TreeSource<PathType>                       TreeSourceType;	
        typedef otb::ImageToTreeFilter<InputImageType,PathType> TreeFilterType;
	typedef otb::Flst<InputImageType,PathType>              FlstType;
	
        typedef  itk::TreeContainer< PathType >          OutputTreeType;
        typedef  OutputTreeType::Pointer        OutputTreePointerType;



	// Tester les constructeurs des différentes classes mises en oeuvre: 
//        TreeSourceType:: Pointer TreeSourceTest;
//        std::cout<<"AAAAAAAAAAAAAAA" <<std::endl;
//	TreeSourceTest = TreeSourceType::New();
	
//        std::cout<<"AAAAAAAAAAAAAAA" <<std::endl;
//	TreeSourceTest->AllocateShapeTree(100,1000,5.0);
//	TreeSourceTest->DeAllocateShapeTree();
    
//        TreeFilterType::Pointer TreeFilterTest;
//        TreeFilterTest = TreeFilterType::New();
	
	FlstType::Pointer FlstTest;

	FlstTest= FlstType::New();	
 
        // Test 2:  Pipeline de la Flst:
    
        std::cout<<"Tester le pipeline" <<std::endl;
	
	OutputTreeType  *FlstResult;
	reader->Update();
		
	FlstTest->SetInput( 0, reader->GetOutput() );	
	FlstTest->Update(); 
	
#if 0

	writer->SetInput(reader->GetOutput());
	writer->Update();
	
#endif
  
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
  // Software Guide : EndCodeSnippet

//#endif
  return EXIT_SUCCESS;
}
