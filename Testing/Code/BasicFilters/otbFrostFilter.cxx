/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   26 janvier 2005
  Version   :   
  Role      :   Test la classe qui filtre un image a partir de l'algorithme de FROST
  $Id$

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

//#define MAIN

#include "otbImageFileReader.h"
#include "otbFrostImageFilter.h"

#include "itkExceptionObject.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"
#include "itkRandomImageSource.h"
#include "itkMeanImageFilter.h"


int otbFrostFilterNew( int argc, char ** argv )
{
  try 
    { 
        typedef unsigned char                                   InputPixelType;
        typedef unsigned char   	                        OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >        InputImageType;
        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::FrostImageFilter< InputImageType,OutputImageType >   FilterType;

        FilterType::Pointer frost = FilterType::New();
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


int otbFrostFilterTest( int argc, char ** argv )
{

  typedef itk::Image<float,2> FloatImage2DType;

  itk::RandomImageSource<FloatImage2DType>::Pointer random;
  random = itk::RandomImageSource<FloatImage2DType>::New();
  random->SetMin(0.0);
  random->SetMax(1000.0);

  unsigned long randomSize[2];
  randomSize[0] = randomSize[1] = 8;
  random->SetSize(randomSize);
  
  float spacing[2] = {0.7, 2.1};
  random->SetSpacing( spacing );
  float origin[2] = {15, 400};
  random->SetOrigin( origin );
    
  // Create a mean image
  otb::FrostImageFilter<FloatImage2DType, FloatImage2DType>::Pointer frost;
  frost = otb::FrostImageFilter<FloatImage2DType,FloatImage2DType>::New();
  frost->SetInput(random->GetOutput());

  // define the neighborhood size used for the mean filter (5x5)
  FloatImage2DType::SizeType neighRadius;
  neighRadius[0] = 1;
  neighRadius[1] = 1;
  frost->SetRadius(neighRadius);

  // run the algorithm
  frost->Update();

  itk::ImageRegionIterator<FloatImage2DType> it;
  it = itk::ImageRegionIterator<FloatImage2DType>(random->GetOutput(),
                               random->GetOutput()->GetBufferedRegion());
  std::cout << "Input image" << std::endl;
  unsigned int i;
  for (i=1; !it.IsAtEnd(); ++i, ++it)
    {
    std::cout << "\t" << it.Get();
    if ((i % 8) == 0)
      {
      std::cout << std::endl;
      }
    }

  std::cout << "Output image" << std::endl;
  it = itk::ImageRegionIterator<FloatImage2DType>(frost->GetOutput(),
                               frost->GetOutput()->GetBufferedRegion());
  for (i=1; !it.IsAtEnd(); ++i, ++it)
    {
    std::cout << "\t" << it.Get();
    if ((i % 8) == 0)
      {
      std::cout << std::endl;
      }
    }
  
  // Test the itkGetConstReferenceMacro
  const FloatImage2DType::SizeType & radius = frost->GetRadius();
  std::cout << "frost->GetRadius():" << radius << std::endl;

  return EXIT_SUCCESS;

}



int otbFrostFilter( int argc, char ** argv )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
       
        unsigned int  RadiusX((unsigned int)::atoi(argv[3]));
        unsigned int  RadiusY((unsigned int)::atoi(argv[4]));
        double        Deramp ((double)::atof(argv[5]));

        typedef unsigned char                                   InputPixelType;
        typedef unsigned char   	                        OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >        InputImageType;
        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;
        typedef itk::ImageFileWriter< OutputImageType >         WriterType;

        typedef otb::FrostImageFilter< InputImageType,OutputImageType >   FilterType;
	
	FilterType::SizeType Radius;
	Radius[0]= RadiusX;
	Radius[1]= RadiusY;

        FilterType::Pointer filtreFrost = FilterType::New();
        
	filtreFrost->SetRadius( Radius );
	filtreFrost->SetDeramp( Deramp );

        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer = WriterType::New();

        reader->SetFileName( inputFilename  );
        writer->SetFileName( outputFilename );
        
        filtreFrost->SetInput( reader->GetOutput() );
        writer->SetInput( filtreFrost->GetOutput() );
        
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


