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

typedef otb::Image<double, 2>                    FloatImageType;
  
typedef otb::VectorImage<double, 2>              FloatVectorImageType;
  
typedef otb::Multi3DMapToDEMFilter
  <FloatVectorImageType,
    FloatImageType,
    FloatImageType>                              Multi3DFilterType;

    
int otbMulti3DMapToDEMFilterNew(int argc, char* argv[])
{
  // Instantiation
  Multi3DFilterType::Pointer multiFilter = Multi3DFilterType::New();
  
  return EXIT_SUCCESS;
}

int otbMulti3DMapToDEMFilter(int argc, char* argv[])
{
  typedef otb::ImageFileReader<FloatImageType>    ReaderType;
  
  typedef otb::ImageFileReader
    <FloatVectorImageType>                        ReaderVectorType;
    

  typedef otb::ImageFileWriter
    <FloatImageType>                              WriterType;
  
  typedef otb::ImageList<FloatImageType>          ImageListType;
  
  typedef otb::VectorImageToImageListFilter
    <FloatVectorImageType,
     ImageListType>                               VectorToListFilterType;
  
  typedef otb::ObjectList<ReaderType>            MaskReaderListType;
  typedef otb::ObjectList<ReaderVectorType>      MapReaderListType;


  if ((argc-6)%2 !=0)
    {
    std::cout << "Usage: "<<argv[0]<<" 3DMapImage1 .mask1... 3DMapImageN maskN  DEMoutput DEMGridStep FusionMode ThreadNb StreamNb" << std::endl;
    return EXIT_FAILURE;
    }
  
  int mapSize=(argc-6)/2;

  
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
    multiFilter->SetMapKeywordList(i,mapReaderList->GetNthElement(i)->GetOutput()->GetImageKeywordlist());
    multiFilter->SetMaskInput(i,maskReaderList->GetNthElement(i)->GetOutput());
   }

  WriterType::Pointer writer = WriterType::New();

  multiFilter->SetNumberOfThreads(atoi(argv[argc-2]));
  multiFilter->Update();

  writer->SetInput(multiFilter->GetOutput());

  writer->SetFileName(argv[argc-5]);
  writer->SetNumberOfDivisionsStrippedStreaming(atoi(argv[argc-1]));
  writer->Update();

  
  return EXIT_SUCCESS;
}
