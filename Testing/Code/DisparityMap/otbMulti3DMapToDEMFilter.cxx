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

#include "otbMulti3DMapToDEMFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorImageToImageListFilter.h"
#include <string>
#include "otbObjectList.h"
#include "otbGeoInformationConversion.h"

typedef otb::Image<double, 2>                    ImageType;

typedef otb::VectorImage<double, 2>              VectorImageType;

typedef otb::Multi3DMapToDEMFilter
  <VectorImageType,
    ImageType,
    ImageType>                              Multi3DFilterType;


int otbMulti3DMapToDEMFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  // Instantiation
  Multi3DFilterType::Pointer multiFilter = Multi3DFilterType::New();

  return EXIT_SUCCESS;
}

int otbMulti3DMapToDEMFilterEPSG(int argc, char* argv[])
{


  typedef otb::ImageFileReader<ImageType>    ReaderType;

  typedef otb::ImageFileReader
    <VectorImageType>                        ReaderVectorType;
  typedef otb::ImageFileWriter
    <ImageType>                              WriterType;
  typedef otb::ObjectList<ReaderType>            MaskReaderListType;
  typedef otb::ObjectList<ReaderVectorType>      MapReaderListType;


  if ((argc-15)%2 !=0)
    {
    std::cout << "Usage: "<<argv[0]<<" 3DMapImage1 .mask1... 3DMapImageN maskN  DEMoutput DEMGridStep FusionMode ThreadNb StreamNb  <x start index> <y start index> <x origin> <y origin> <x size> <y size> <x spacing> <y spacing> <epsg code>" << std::endl;
    return EXIT_FAILURE;
    }

  unsigned int mapSize=(argc-15)/2;


  MapReaderListType::Pointer mapReaderList=MapReaderListType::New();
  MaskReaderListType::Pointer  maskReaderList=MaskReaderListType::New();
  for(unsigned int i=0; i<(mapSize); i++)
   {
    mapReaderList->PushBack(ReaderVectorType::New());
    maskReaderList->PushBack(ReaderType::New());
   }


  // unsigned int i=1;
  unsigned int index=0;
  for(unsigned int i=1; i<(2*mapSize+1); i=i+2)
  {
    mapReaderList->GetNthElement(index)->SetFileName(argv[i]);
    mapReaderList->GetNthElement(index)->UpdateOutputInformation();

    maskReaderList->GetNthElement(index)->SetFileName(argv[i+1]);
    maskReaderList->GetNthElement(index)->UpdateOutputInformation();
    index++;
  }

  float gridStep=atof(argv[argc-13]);

  std::cout<<"gridStep "<<gridStep<<std::endl;

  int fusionMode=atof(argv[argc-12]);

  std::cout<<"fusionMode "<<fusionMode<<std::endl;


  Multi3DFilterType::Pointer multiFilter = Multi3DFilterType::New();
  multiFilter->SetNumberOf3DMaps(mapSize);
  multiFilter->SetDEMGridStep(gridStep);
  multiFilter->SetCellFusionMode(fusionMode);

  for(unsigned int i=0; i<mapSize; i++)
   {
    multiFilter->Set3DMapInput(i,mapReaderList->GetNthElement(i)->GetOutput());
   // multiFilter->SetMapKeywordList(i,mapReaderList->GetNthElement(i)->GetOutput()->GetImageKeywordlist());
    multiFilter->SetMaskInput(i,maskReaderList->GetNthElement(i)->GetOutput());
   }

  //set output parameters
 // multiFilter->SetOutputParametersFrom3DMap();
 VectorImageType::IndexType start;
  start[0] =  atoi(argv[argc-9]);
  start[1] =  atoi(argv[argc-8]);
  multiFilter->SetOutputStartIndex(start);

  VectorImageType::SizeType size;
  size[0] = atoi(argv[argc-5]);      // X size
  size[1] = atoi(argv[argc-4]);            //Y size
  multiFilter->SetOutputSize(size);

  VectorImageType::SpacingType spacing;
  spacing[0] = atof(argv[argc-3]);
  spacing[1] = atof(argv[argc-2]);
  multiFilter->SetOutputSpacing(spacing);

  VectorImageType::PointType origin;
  origin[0] = strtod(argv[argc-7], NULL);
  origin[1] = strtod(argv[argc-6], NULL);
  multiFilter->SetOutputOrigin(origin);

  std::string projectionRef=otb::GeoInformationConversion::ToWKT( atoi(argv[argc-1]));

  std::cout<<"projection Reference :"<<std::endl<<projectionRef<<std::endl;

  multiFilter->SetProjectionRef(projectionRef);
  WriterType::Pointer writer = WriterType::New();

  multiFilter->SetNumberOfThreads(atoi(argv[argc-11]));
  //multiFilter->Update();

  writer->SetInput(multiFilter->GetOutput());

  writer->SetFileName(argv[argc-14]);
  writer->SetNumberOfDivisionsStrippedStreaming(atoi(argv[argc-10]));
  writer->Update();

  return EXIT_SUCCESS;

  /*
  if (argc != 12)
    {
    std::cout << argv[0] <<
    " <input filename> <output filename> <origin easting> <origin northing> <x size> <y size> <x spacing> <y spacing> <UTM zone> <UTM hemisphere>"
              << std::endl;

    return EXIT_FAILURE;
    }

  typedef otb::VectorImage<double, 2>                                               VectorImageType;
  typedef otb::ImageFileReader<VectorImageType>                                     ReaderType;
  typedef otb::ImageFileWriter<VectorImageType>                            WriterType;
  typedef otb::UtmInverseProjection                                                 UtmMapProjectionType;
  typedef otb::OrthoRectificationFilter<VectorImageType, VectorImageType, UtmMapProjectionType> OrthoRectifFilterType;

  //Allocate pointer
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  OrthoRectifFilterType::Pointer orthoRectifFilter = OrthoRectifFilterType::New();
  UtmMapProjectionType::Pointer  utmMapProjection = UtmMapProjectionType::New();

  // Set parameters ...
  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  reader->GenerateOutputInformation();
  std::cout << reader->GetOutput() << std::endl;

  orthoRectifFilter->SetInput(reader->GetOutput());

  VectorImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;
  orthoRectifFilter->SetOutputStartIndex(start);

  VectorImageType::SizeType size;
  size[0] = atoi(argv[5]);      // X size
  size[1] = atoi(argv[6]);            //Y size
  orthoRectifFilter->SetOutputSize(size);

  VectorImageType::SpacingType spacing;
  spacing[0] = atof(argv[7]);
  spacing[1] = atof(argv[8]);
  orthoRectifFilter->SetOutputSpacing(spacing);

  VectorImageType::PointType origin;
  origin[0] = strtod(argv[3], NULL);         //Origin easting
  origin[1] = strtod(argv[4], NULL);         //Origin northing
  orthoRectifFilter->SetOutputOrigin(origin);

  utmMapProjection->SetZone(atoi(argv[9]));
  utmMapProjection->SetHemisphere(argv[10][0]);
  orthoRectifFilter->SetMapProjection(utmMapProjection);

  // Deformation Field spacing
  VectorImageType::SpacingType  gridSpacing;
  gridSpacing[0] = atof(argv[11]);
  gridSpacing[1] = -atof(argv[11]);
  orthoRectifFilter->SetDeformationFieldSpacing(gridSpacing);

  VectorImageType::PixelType no_data(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  no_data.Fill(0);
  orthoRectifFilter->SetEdgePaddingValue(no_data);

  writer->SetInput(orthoRectifFilter->GetOutput());
  writer->SetNumberOfDivisionsTiledStreaming(4);
  writer->Update();

  return EXIT_SUCCESS; */




  /*
  if (argc != 12)
    {
    std::cout << argv[0] <<
    " <input filename> <output filename> <origin easting> <origin northing> <x size> <y size> <x spacing> <y spacing> <UTM zone> <UTM hemisphere>"
              << std::endl;

    return EXIT_FAILURE;
    }

  typedef otb::VectorImage<double, 2>                                               VectorImageType;
  typedef otb::ImageFileReader<VectorImageType>                                     ReaderType;
  typedef otb::ImageFileWriter<VectorImageType>                            WriterType;
  typedef otb::UtmInverseProjection                                                 UtmMapProjectionType;
  typedef otb::OrthoRectificationFilter<VectorImageType, VectorImageType, UtmMapProjectionType> OrthoRectifFilterType;

  //Allocate pointer
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  OrthoRectifFilterType::Pointer orthoRectifFilter = OrthoRectifFilterType::New();
  UtmMapProjectionType::Pointer  utmMapProjection = UtmMapProjectionType::New();

  // Set parameters ...
  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  reader->GenerateOutputInformation();
  std::cout << reader->GetOutput() << std::endl;

  orthoRectifFilter->SetInput(reader->GetOutput());

  VectorImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;
  orthoRectifFilter->SetOutputStartIndex(start);

  VectorImageType::SizeType size;
  size[0] = atoi(argv[5]);      // X size
  size[1] = atoi(argv[6]);            //Y size
  orthoRectifFilter->SetOutputSize(size);

  VectorImageType::SpacingType spacing;
  spacing[0] = atof(argv[7]);
  spacing[1] = atof(argv[8]);
  orthoRectifFilter->SetOutputSpacing(spacing);

  VectorImageType::PointType origin;
  origin[0] = strtod(argv[3], NULL);         //Origin easting
  origin[1] = strtod(argv[4], NULL);         //Origin northing
  orthoRectifFilter->SetOutputOrigin(origin);

  utmMapProjection->SetZone(atoi(argv[9]));
  utmMapProjection->SetHemisphere(argv[10][0]);
  orthoRectifFilter->SetMapProjection(utmMapProjection);

  // Deformation Field spacing
  VectorImageType::SpacingType  gridSpacing;
  gridSpacing[0] = atof(argv[11]);
  gridSpacing[1] = -atof(argv[11]);
  orthoRectifFilter->SetDeformationFieldSpacing(gridSpacing);

  VectorImageType::PixelType no_data(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  no_data.Fill(0);
  orthoRectifFilter->SetEdgePaddingValue(no_data);

  writer->SetInput(orthoRectifFilter->GetOutput());
  writer->SetNumberOfDivisionsTiledStreaming(4);
  writer->Update();

  return EXIT_SUCCESS; */
}

int otbMulti3DMapToDEMFilterManual(int argc, char* argv[])
{


typedef otb::ImageFileReader<ImageType>    ReaderType;

typedef otb::ImageFileReader
  <VectorImageType>                        ReaderVectorType;


typedef otb::ImageFileWriter
  <ImageType>                              WriterType;

typedef otb::ObjectList<ReaderType>            MaskReaderListType;
typedef otb::ObjectList<ReaderVectorType>      MapReaderListType;


if ((argc-14)%2 !=0)
  {
  std::cout << "Usage: "<<argv[0]<<" 3DMapImage1 .mask1... 3DMapImageN maskN  DEMoutput DEMGridStep FusionMode ThreadNb StreamNb  <x start index> <y start index> <x origin> <y origin> <x size> <y size> <x spacing> <y spacing>" << std::endl;
  return EXIT_FAILURE;
  }

unsigned int mapSize=(argc-14)/2;


MapReaderListType::Pointer mapReaderList=MapReaderListType::New();
MaskReaderListType::Pointer  maskReaderList=MaskReaderListType::New();
for(unsigned int i=0; i<(mapSize); i++)
 {
  mapReaderList->PushBack(ReaderVectorType::New());
  maskReaderList->PushBack(ReaderType::New());
 }


// unsigned int i=1;
unsigned int index=0;
for(unsigned int i=1; i<(2*mapSize+1); i=i+2)
{
  mapReaderList->GetNthElement(index)->SetFileName(argv[i]);
  mapReaderList->GetNthElement(index)->UpdateOutputInformation();

  maskReaderList->GetNthElement(index)->SetFileName(argv[i+1]);
  maskReaderList->GetNthElement(index)->UpdateOutputInformation();
  index++;
}

float gridStep=atof(argv[argc-12]);

std::cout<<"gridStep "<<gridStep<<std::endl;

int fusionMode=atof(argv[argc-11]);

std::cout<<"fusionMode "<<fusionMode<<std::endl;


Multi3DFilterType::Pointer multiFilter = Multi3DFilterType::New();
multiFilter->SetNumberOf3DMaps(mapSize);
multiFilter->SetDEMGridStep(gridStep);
multiFilter->SetCellFusionMode(fusionMode);

for(unsigned int i=0; i<mapSize; i++)
 {
  multiFilter->Set3DMapInput(i,mapReaderList->GetNthElement(i)->GetOutput());
 // multiFilter->SetMapKeywordList(i,mapReaderList->GetNthElement(i)->GetOutput()->GetImageKeywordlist());
  multiFilter->SetMaskInput(i,maskReaderList->GetNthElement(i)->GetOutput());
 }

//set output parameters
//multiFilter->SetOutputParametersFrom3DMap();
VectorImageType::IndexType start;
start[0] =  atoi(argv[argc-7]);
start[1] =  atoi(argv[argc-8]);
multiFilter->SetOutputStartIndex(start);

VectorImageType::SizeType size;
size[0] = atoi(argv[argc-4]);      // X size
size[1] = atoi(argv[argc-3]);            //Y size
multiFilter->SetOutputSize(size);

VectorImageType::SpacingType spacing;
spacing[0] = atof(argv[argc-2]);
spacing[1] = atof(argv[argc-1]);
multiFilter->SetOutputSpacing(spacing);

VectorImageType::PointType origin;
origin[0] = strtod(argv[argc-6], NULL);
origin[1] = strtod(argv[argc-5], NULL);
multiFilter->SetOutputOrigin(origin);



WriterType::Pointer writer = WriterType::New();

multiFilter->SetNumberOfThreads(atoi(argv[argc-10]));
//multiFilter->Update();

writer->SetInput(multiFilter->GetOutput());

writer->SetFileName(argv[argc-13]);
writer->SetNumberOfDivisionsStrippedStreaming(atoi(argv[argc-15]));
writer->Update();


return EXIT_SUCCESS;

/*
if (argc != 12)
  {
  std::cout << argv[0] <<
  " <input filename> <output filename> <origin easting> <origin northing> <x size> <y size> <x spacing> <y spacing> <UTM zone> <UTM hemisphere>"
            << std::endl;

  return EXIT_FAILURE;
  }

typedef otb::VectorImage<double, 2>                                               VectorImageType;
typedef otb::ImageFileReader<VectorImageType>                                     ReaderType;
typedef otb::ImageFileWriter<VectorImageType>                            WriterType;
typedef otb::UtmInverseProjection                                                 UtmMapProjectionType;
typedef otb::OrthoRectificationFilter<VectorImageType, VectorImageType, UtmMapProjectionType> OrthoRectifFilterType;

//Allocate pointer
ReaderType::Pointer reader = ReaderType::New();
WriterType::Pointer writer = WriterType::New();

OrthoRectifFilterType::Pointer orthoRectifFilter = OrthoRectifFilterType::New();
UtmMapProjectionType::Pointer  utmMapProjection = UtmMapProjectionType::New();

// Set parameters ...
reader->SetFileName(argv[1]);
writer->SetFileName(argv[2]);

reader->GenerateOutputInformation();
std::cout << reader->GetOutput() << std::endl;

orthoRectifFilter->SetInput(reader->GetOutput());

VectorImageType::IndexType start;
start[0] = 0;
start[1] = 0;
orthoRectifFilter->SetOutputStartIndex(start);

VectorImageType::SizeType size;
size[0] = atoi(argv[5]);      // X size
size[1] = atoi(argv[6]);            //Y size
orthoRectifFilter->SetOutputSize(size);

VectorImageType::SpacingType spacing;
spacing[0] = atof(argv[7]);
spacing[1] = atof(argv[8]);
orthoRectifFilter->SetOutputSpacing(spacing);

VectorImageType::PointType origin;
origin[0] = strtod(argv[3], NULL);         //Origin easting
origin[1] = strtod(argv[4], NULL);         //Origin northing
orthoRectifFilter->SetOutputOrigin(origin);

utmMapProjection->SetZone(atoi(argv[9]));
utmMapProjection->SetHemisphere(argv[10][0]);
orthoRectifFilter->SetMapProjection(utmMapProjection);

// Deformation Field spacing
VectorImageType::SpacingType  gridSpacing;
gridSpacing[0] = atof(argv[11]);
gridSpacing[1] = -atof(argv[11]);
orthoRectifFilter->SetDeformationFieldSpacing(gridSpacing);

VectorImageType::PixelType no_data(reader->GetOutput()->GetNumberOfComponentsPerPixel());
no_data.Fill(0);
orthoRectifFilter->SetEdgePaddingValue(no_data);

writer->SetInput(orthoRectifFilter->GetOutput());
writer->SetNumberOfDivisionsTiledStreaming(4);
writer->Update();

return EXIT_SUCCESS; */

}


int otbMulti3DMapToDEMFilter(int argc, char* argv[])
{
  typedef otb::ImageFileReader<ImageType>    ReaderType;

  typedef otb::ImageFileReader
    <VectorImageType>                        ReaderVectorType;
  typedef otb::ImageFileWriter
    <ImageType>                              WriterType;
  typedef otb::ObjectList<ReaderType>            MaskReaderListType;
  typedef otb::ObjectList<ReaderVectorType>      MapReaderListType;

  if ((argc-6)%2 !=0)
    {
    std::cout << "Usage: "<<argv[0]<<" 3DMapImage1 .mask1... 3DMapImageN maskN  DEMoutput DEMGridStep FusionMode ThreadNb StreamNb" << std::endl;
    return EXIT_FAILURE;
    }

  unsigned int mapSize=(argc-6)/2;


  MapReaderListType::Pointer mapReaderList=MapReaderListType::New();
  MaskReaderListType::Pointer  maskReaderList=MaskReaderListType::New();
  for(unsigned int i=0; i<(mapSize); i++)
   {
    mapReaderList->PushBack(ReaderVectorType::New());
    maskReaderList->PushBack(ReaderType::New());
   }


  // unsigned int i=1;
  unsigned int index=0;
  for(unsigned int i=1; i<(2*mapSize+1); i=i+2)
  {
    mapReaderList->GetNthElement(index)->SetFileName(argv[i]);
    mapReaderList->GetNthElement(index)->UpdateOutputInformation();

    maskReaderList->GetNthElement(index)->SetFileName(argv[i+1]);
    maskReaderList->GetNthElement(index)->UpdateOutputInformation();
    index++;
  }

  float gridStep=atof(argv[argc-4]);

  std::cout<<"gridStep "<<gridStep<<std::endl;

  int fusionMode=atof(argv[argc-3]);

  std::cout<<"fusionMode "<<fusionMode<<std::endl;


  Multi3DFilterType::Pointer multiFilter = Multi3DFilterType::New();
  multiFilter->SetNumberOf3DMaps(mapSize);
  multiFilter->SetDEMGridStep(gridStep);
  multiFilter->SetCellFusionMode(fusionMode);

  for(unsigned int i=0; i<mapSize; i++)
   {
    multiFilter->Set3DMapInput(i,mapReaderList->GetNthElement(i)->GetOutput());
   // multiFilter->SetMapKeywordList(i,mapReaderList->GetNthElement(i)->GetOutput()->GetImageKeywordlist());
    multiFilter->SetMaskInput(i,maskReaderList->GetNthElement(i)->GetOutput());
   }
  multiFilter->SetOutputParametersFrom3DMap();

  WriterType::Pointer writer = WriterType::New();

  multiFilter->SetNumberOfThreads(atoi(argv[argc-2]));
  //multiFilter->Update();

  writer->SetInput(multiFilter->GetOutput());

  writer->SetFileName(argv[argc-5]);
  writer->SetNumberOfDivisionsStrippedStreaming(atoi(argv[argc-1]));
  writer->Update();


  return EXIT_SUCCESS;
}
