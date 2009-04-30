#include "otbVectorDataFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorData.h"
#include "otbVectorDataProjectionFilter.h"
#include <fstream>
#include <iostream>

#include "itkRGBAPixel.h"
#include "otbImage.h"
#include "otbVectorDataToImageFilter.h"

//  ./mapnikOTBClasses /home/christop/OTB/trunk/OTB-Data/Input/waterways.shp output.png
//  ./mapnikOTBClasses ~/OTB/trunk/OTB-Data/Input/ToulouseRoad-examples.shp output.png

int main(int argc, char * argv[])
{

  //Read the vector data
  typedef otb::VectorData<> VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;
  VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();

  reader->SetFileName(argv[1]);

  //Reproject the vector data in the proper projection
  typedef otb::VectorDataProjectionFilter<VectorDataType, VectorDataType> ProjectionFilterType;
  ProjectionFilterType::Pointer projection = ProjectionFilterType::New();
  projection->SetInput(reader->GetOutput());
//   projection->Update();

  //Convert the vector data into an image
  typedef itk::RGBAPixel< unsigned char > PixelType;
  typedef otb::Image<PixelType,2> ImageType;
  typedef otb::VectorDataToImageFilter<VectorDataType, ImageType> VectorDataToImageFilterType;
  VectorDataToImageFilterType::Pointer vectorDataRendering = VectorDataToImageFilterType::New();
  vectorDataRendering->SetInput(projection->GetOutput());
  ImageType::SizeType size;
  size[0]=1000;
  size[1]=1000;
  vectorDataRendering->SetSize(size);


  //Save the image in a file
  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(vectorDataRendering->GetOutput());
  writer->SetFileName(argv[2]);
  writer->Update();



  return EXIT_SUCCESS;
}