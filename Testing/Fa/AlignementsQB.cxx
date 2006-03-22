#include "itkImage.h"
#include "itkImageFileWriter.h"
#include <iostream>
#include "itkPolyLineParametricPath.h"

#include "otbImageFileReader.h"
#include "otbExtractROI.h"
#include "otbImageToPathListAlignFilter.h"
#include "otbDrawPathFilter.h"
//#include "otbColorImageViewer.h"


#include <stdio.h>

int main( int argc, char ** argv )
{
  if(argc!=3)
    {

    std::cout << "Usage : " << argv[0] << " inputImage outputImage" << std::endl;
    return 1;
    
    }

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];
       

  typedef unsigned char                                   InputPixelType;
  typedef unsigned char   	                        OutputPixelType;

  const   unsigned int        	                        Dimension = 2;

  typedef itk::Image< InputPixelType,  Dimension >	InputImageType;
  typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

  typedef otb::ImageFileReader< InputImageType  >         ReaderType;
  typedef itk::ImageFileWriter< OutputImageType >         WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName( inputFilename  );
  writer->SetFileName( outputFilename  );

  reader->Update();

  std::cout << "Lecture terminee" << std::endl;

  typedef otb::ExtractROI<InputPixelType,InputPixelType> ROIFilterType;

  ROIFilterType::Pointer roiFilter = ROIFilterType::New();

  roiFilter->SetInput( reader->GetOutput() );
  roiFilter->SetStartX( 10 );
  roiFilter->SetStartY( 0 );
  roiFilter->SetSizeX( 256 );
  roiFilter->SetSizeY( 256 );


  roiFilter->Update();

  std::cout << "Extraction ROI" << std::endl;
  
  typedef itk::PolyLineParametricPath< Dimension >		  PathType;
  typedef otb::ImageToPathListAlignFilter<InputImageType,PathType> ListAlignFilterType;

  ListAlignFilterType::Pointer alignFilter = ListAlignFilterType::New();

  alignFilter->SetInput( roiFilter->GetOutput() );

  alignFilter->Update();

  std::cout << "Alignements termines" << std::endl;


  typedef ROIFilterType::OutputImageType BackgroundImageType;
  
  typedef otb::DrawPathFilter< BackgroundImageType, PathType, OutputImageType >  DrawPathFilterType;

  DrawPathFilterType::Pointer drawPathFilter = DrawPathFilterType::New();

  typedef ListAlignFilterType::OutputPathListType ListType;

  
  ListType* listePaths = alignFilter->GetOutput();

  ListType::iterator listIt = listePaths->begin();

  BackgroundImageType::Pointer backgroundImage = roiFilter->GetOutput();

  roiFilter->Update();

  unsigned int color = 0;
  
  while( listIt != listePaths->end())
    {

    drawPathFilter->SetImageInput( backgroundImage );
    drawPathFilter->SetPathInput( *(listIt) );
    //drawPathFilter->SetPathValue( color );

    drawPathFilter->Update();

    backgroundImage = drawPathFilter->GetOutput();
    
    ++listIt;
    ++color;

    


    }
  

  
  writer->SetInput( drawPathFilter->GetOutput() );

  writer->Update();

  
/*  typedef otb::ColorImageViewer<unsigned char,double>    ViewerType;
  ViewerType                      viewer;

  viewer.SetLabel( "Input Image" );
  viewer.SetImage( drawPathFilter->GetOutput() );  

  viewer.Show();
  Fl::run();
*/

  

  return EXIT_SUCCESS;
}


