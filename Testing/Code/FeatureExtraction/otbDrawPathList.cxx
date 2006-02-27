/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   06 fevrier 2006
  Version   :   
  Role      :   
  $Id: $

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkPolyLineParametricPath.h"

#include "otbImageFileReader.h"
#include "otbPathListSource.h"
#include "otbImageToPathListAlignFilter.h"
#include "otbDrawPathListFilter.h"
#include "otbImageFileWriter.h"


int otbDrawPathListTracerLignes( int argc, char ** argv )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
       
        typedef unsigned char                                   InputPixelType;
        typedef unsigned char   	                        OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >	InputImageType;
        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

	typedef itk::PolyLineParametricPath< Dimension >	PathType;
	typedef PathType::Pointer                               PathPointerType;	

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;  
        typedef itk::ImageFileWriter< OutputImageType >         WriterType;
	typedef otb::PathListSource<PathType>                   PathListType;
	
	typedef otb::DrawPathListFilter<InputImageType,PathType,OutputImageType> DrawPathListFilterType;

        ReaderType::Pointer reader         = ReaderType::New();
        WriterType::Pointer writer         = WriterType::New();
	
        reader->SetFileName( inputFilename  );
	writer->SetFileName( outputFilename );
	
	InputImageType::ConstPointer image = reader->GetOutput();
	typedef InputImageType::PointType ImagePointType;
		
	PathListType::Pointer testList = PathListType::New();
        PathListType::OutputPathListType * path = testList->GetOutput();

	
	ImagePointType   pos;
	PathType::ContinuousIndexType cindex;
	
        
	/*1er Segment*/
	PathType::Pointer pathElt1 = PathType::New();	
	pathElt1->Initialize();

        pos[0]=30;
        pos[1]=30;
	image->TransformPhysicalPointToContinuousIndex(pos,cindex);
        pathElt1->AddVertex(cindex);
        pos[0]= 30;
        pos[1]=130;
	image->TransformPhysicalPointToContinuousIndex(pos,cindex);
        pathElt1->AddVertex(cindex);
    
        path->push_back(pathElt1);
	
        /*2eme Segment*/	
	PathType::Pointer pathElt2 = PathType::New();
	pathElt2->Initialize();
        pos[0]= 30;
        pos[1]=130;
	image->TransformPhysicalPointToContinuousIndex(pos,cindex);
        pathElt2->AddVertex(cindex);
        pos[0]=130;
        pos[1]=130;
	image->TransformPhysicalPointToContinuousIndex(pos,cindex);
        pathElt2->AddVertex(cindex);
    
        path->push_back(pathElt2);

        /*3eme Segment*/	
	PathType::Pointer pathElt3 = PathType::New();
	pathElt3->Initialize();
        pos[0]=130;
        pos[1]=130;
	image->TransformPhysicalPointToContinuousIndex(pos,cindex);
        pathElt3->AddVertex(cindex);
        pos[0]=130;
        pos[1]= 30;
	image->TransformPhysicalPointToContinuousIndex(pos,cindex);
        pathElt3->AddVertex(cindex);
    
        path->push_back(pathElt3);

        /*4eme Segment*/	
	PathType::Pointer pathElt4 = PathType::New();

	pathElt4->Initialize();
        pos[0]=130;
        pos[1]= 30;
	image->TransformPhysicalPointToContinuousIndex(pos,cindex);
        pathElt4->AddVertex(cindex);
        pos[0]=30;
        pos[1]=30;
	image->TransformPhysicalPointToContinuousIndex(pos,cindex);
        pathElt4->AddVertex(cindex);
    
        path->push_back(pathElt4);

        /*5eme Segment*/	
	PathType::Pointer pathElt5 = PathType::New();
	pathElt5->Initialize();
        pos[0]= 30;
        pos[1]= 30;
	image->TransformPhysicalPointToContinuousIndex(pos,cindex);
        pathElt5->AddVertex(cindex);
        pos[0]=130;
        pos[1]=130;
	image->TransformPhysicalPointToContinuousIndex(pos,cindex);
        pathElt5->AddVertex(cindex);
    
        path->push_back(pathElt5);
		
        /*6eme Segment*/	
	PathType::Pointer pathElt6 = PathType::New();

	pathElt6->Initialize();
        pos[0]=130;
        pos[1]= 30;
	image->TransformPhysicalPointToContinuousIndex(pos,cindex);
        pathElt6->AddVertex(cindex);
        pos[0]= 30;
        pos[1]=130;
	image->TransformPhysicalPointToContinuousIndex(pos,cindex);
        pathElt6->AddVertex(cindex);
    
        path->push_back(pathElt6);


	
#if 0	
	testList->SetImageInput( reader->GetOutput() );
	testList->Update();
#endif	
        DrawPathListFilterType::Pointer DrawPath = DrawPathListFilterType::New();

	DrawPath->SetImageInput(reader->GetOutput()  );
	DrawPath->SetPathInput( testList->GetOutput());
//	DrawPath->SetImageOutput();
	DrawPath->Update();
	
	writer->SetInput(DrawPath->GetImageOutput());
        writer->Update(); 

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


int otbDrawPathList( int argc, char ** argv )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
       

        typedef unsigned char                                   InputPixelType;
        typedef unsigned char   	                        OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >	InputImageType;
        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

	typedef itk::PolyLineParametricPath< Dimension >	PathType;
	typedef PathType::Pointer                               PathPointerType;	

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;  
        typedef itk::ImageFileWriter< OutputImageType >         WriterType;
	
//	typedef otb::ImageToPathListAlignFilter<InputImageType,PathType> ListAlignFilterType;
//	typedef ListAlignFilterType::OutputPathListType   ListAlignFilterOutputPathListType;	
	typedef otb::DrawPathListFilter<InputImageType,PathType,OutputImageType> DrawPathListFilterType;

        typedef otb::ImageToPathListAlignFilter<InputImageType,PathType>  PathListType;

        ReaderType::Pointer reader         = ReaderType::New();
        WriterType::Pointer writer         = WriterType::New();
//	InputImageType::Pointer  ImageIn   = InputImageType::New();
//	OutputImageType::Pointer ImageOut  = OutputImageType::New();
	
	DrawPathListFilterType::Pointer DrawPath = DrawPathListFilterType::New();

//        typedef itk::PathSource<PathType>  TestType;
//	TestType::Pointer test = TestType::New();
	
	PathListType::Pointer testList = PathListType::New();
	
        reader->SetFileName( inputFilename  );
	writer->SetFileName( outputFilename );
	
	testList->SetImageInput( reader->GetOutput() );
	testList->Update();
	
	DrawPath->SetImageInput(reader->GetOutput()  );
	DrawPath->SetPathInput( testList->GetOutput());
//	DrawPath->SetImageOutput();
	DrawPath->Update();
	
	writer->SetInput(DrawPath->GetImageOutput());
        writer->Update(); 

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
