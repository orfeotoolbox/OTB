/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   28 fevrier 2006
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
#include "otbDrawPathFilter.h"
#include "otbImageFileWriter.h"


int otbDrawPathDessinCarre( int argc, char ** argv )
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
	typedef InputImageType::PointType                       InputImagePointType;

	typedef itk::PolyLineParametricPath< Dimension >	PathType;

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;  
        typedef itk::ImageFileWriter< OutputImageType >         WriterType;

	
	typedef otb::DrawPathFilter<InputImageType,PathType,OutputImageType> DrawPathFilterType;

        ReaderType::Pointer reader         = ReaderType::New();
        WriterType::Pointer writer         = WriterType::New();
	PathType::Pointer   VertexList     = PathType::New();
	
        reader->SetFileName( inputFilename  );
	writer->SetFileName( outputFilename );
	
	InputImageType::ConstPointer image = reader->GetOutput();
		
	InputImagePointType   pos;
	PathType::ContinuousIndexType cindex;
	
	VertexList->Initialize();
	
	/*1er Segment*/
        pos[0]=30;
        pos[1]=30;
	image->TransformPhysicalPointToContinuousIndex(pos,cindex);
        VertexList->AddVertex(cindex);
        pos[0]= 30;
        pos[1]=130;
	image->TransformPhysicalPointToContinuousIndex(pos,cindex);
        VertexList->AddVertex(cindex);
        
	/*2eme Segment*/	
        pos[0]=130;
        pos[1]=130;
	image->TransformPhysicalPointToContinuousIndex(pos,cindex);
        VertexList->AddVertex(cindex);


        /*3eme Segment*/	
        pos[0]=130;
        pos[1]= 30;
	image->TransformPhysicalPointToContinuousIndex(pos,cindex);
        VertexList->AddVertex(cindex);
        
	/*4eme Segment*/	
        pos[0]=30;
        pos[1]=30;
	image->TransformPhysicalPointToContinuousIndex(pos,cindex);
        VertexList->AddVertex(cindex);
        
	
	DrawPathFilterType::Pointer DrawPath = DrawPathFilterType::New();

	DrawPath->SetImageInput( reader->GetOutput()  );
	DrawPath->SetPathInput( VertexList     );
	writer->SetInput(DrawPath->GetOutput());
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
  return EXIT_SUCCESS;
}

