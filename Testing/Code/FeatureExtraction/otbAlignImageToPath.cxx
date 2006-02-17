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
#include <iostream>
#include "itkParametricPath.h"
#include "itkPolyLineParametricPath.h"
#include "itkVectorContainer.h"

#include "otbImageFileReader.h"
#include "otbImageToPathListAlignFilter.h"

#include "itkPath.h"
#include "itkPathSource.h"
#include "otbPathListSource.h"
#include "otbImageToPathListFilter.h"
#include "itkContinuousIndex.h"

#include <stdio.h>

int otbAlignImageToPath( int argc, char ** argv )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
       

        typedef unsigned char                                   InputPixelType;
        typedef unsigned char   	                        OutputPixelType;
//	typedef float						PathType;
	typedef float						RealPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >	InputImageType;
        typedef itk::Image< RealPixelType,  Dimension >		RealImageType;

//	typedef itk::Path< double, itk::ContinuousIndex< double , Dimension > , Dimension >			PathType;
	typedef itk::PolyLineParametricPath< Dimension >			PathType;
//	typedef itk::ParametricPath< Dimension >			PathType;
	typedef PathType::Pointer PathTypePointer;
//	PathType ltoto;
	PathType::Pointer ltoto = PathType::New();
	
/*	typedef otb::ImageToPathAlignExtract<itk::Image< InputPixelType,  Dimension > ,PathType>  OutputPathType;
        typedef  OutputPathType::InputImageType  InputImageType;
*/
//	typedef otb::PathListSource<PathType>			PathListSourceType;
//	PathListSourceType::Pointer PathSourceList = PathListSourceType::New();
	
	

        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;  

	typedef otb::ImageToPathListAlignFilter<InputImageType,PathType> ListAlignFilterType;
        typedef itk::ImageFileWriter< OutputImageType >         WriterType;

        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer = WriterType::New();
	InputImageType::Pointer ImageIn = InputImageType::New();
//	ListAlignFilterType::Pointer Align = ListAlignFilterType::New();

//        typedef itk::PathSource<PathType>  TestType;
//	TestType::Pointer test = TestType::New();
	
        typedef otb::ImageToPathListAlignFilter<InputImageType,PathType>  TestType;
	TestType::Pointer testList = TestType::New();
	
        reader->SetFileName( inputFilename  );

        testList->SetImageInput( reader->GetOutput() );
	
	typedef ListAlignFilterType::OutputPathListType   ListAlignFilterOutputPathListType;
	
	
	printf("Avant update\n");
	testList->Update(); 
	printf("Apres update\n");
	ListAlignFilterOutputPathListType * sortiePath = testList->GetOutput();
	
	printf("Phase d'écriture:\n");
	
	FILE *file = fopen(outputFilename,"w");
  	if (file == NULL) {
    		fprintf(stderr,"Erreur dans l'ouverture du fichier");
    		exit(-1);
  	}
//	typename InputImageType::PointType                   PointType;
	typedef itk::ContinuousIndex< double,2>              VertexType; 
	typedef itk::VectorContainer< unsigned,VertexType >  VertexListType;
	typedef VertexListType::ConstPointer                 VertexListTypePointer;
	VertexListTypePointer vertexList;
	VertexType cindex;
//	PointType::Pointer  point;
	double x1,y1,x2,y2;
	  
	int nbPath = sortiePath->size();
	printf("NbSegment: %d\n",nbPath);
	fprintf(file,"Nb Segment: %d\n",nbPath);
	for (int i =0 ; i<nbPath ;i++){
	   vertexList = (*sortiePath)[i]->GetVertexList();
	   cindex = vertexList->GetElement(0);
	   x1 = cindex[0];
	   y1 = cindex[1];
	   cindex = vertexList->GetElement(1);
	   x2 = cindex[0];
	   y2 = cindex[1];
	   
	   int size = fprintf(file,"%8.4f %8.3f\n",x1,x2);
	}
  	fclose(file);
        
//        writer->Update(); 

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


