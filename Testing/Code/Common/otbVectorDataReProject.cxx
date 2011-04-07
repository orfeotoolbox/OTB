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
#include "itkExceptionObject.h"
#include "otbMacro.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"

#include "otbVectorDataReProjectionFilter.h"

int otbVectorDataReProject(int argc, char * argv[])
{
  typedef float                                           PixelType;
  typedef otb::VectorImage<PixelType,2>                   VectorImageType;
  typedef otb::Image<PixelType,2>                         ImageType;
  typedef otb::ImageFileReader<VectorImageType>           ReaderType;

  // Vector Data
  typedef otb::VectorData<>                               VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType>       VectorDataReaderType;

  //
  typedef otb::VectorDataReProjectionFilter<VectorDataType, VectorImageType>
                                                          VectorDataReProjFilter;

  typedef itk::PreOrderTreeIterator<VectorDataType::DataTreeType> TreeIteratorType;

  // Read the image
  ReaderType::Pointer    reader  = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();

  std::cout<<"Processing image: "<<reader->GetFileName()<<std::endl;

  // read the Vector Data
  VectorDataReaderType::Pointer vdReader = VectorDataReaderType::New();
  vdReader->SetFileName(argv[2]);
  vdReader->Update();

  std::cout<<"Processing vectorData: "<<vdReader->GetFileName()<<std::endl;

  VectorDataReProjFilter::Pointer vdReProjFilter = VectorDataReProjFilter::New();

  //----------
  // Display input
  std::cout << "--- TEST INPUT ---" << std::endl;
  TreeIteratorType itVectorInput(vdReader->GetOutput()->GetDataTree());
  itVectorInput.GoToBegin();
  while (!itVectorInput.IsAtEnd())
    {
    if (itVectorInput.Get()->IsPolygonFeature())
      {
      std::cout << "----------\n" <<itVectorInput.Get()->GetNodeTypeAsString() << std::endl;
      std::cout << "List of vertices: " << ": " <<std::endl;
      for (unsigned int itPoints = 0; itPoints < itVectorInput.Get()->GetPolygonExteriorRing()->GetVertexList()->Size(); itPoints++)
        {
        std::cout << "vertex[" << itPoints << "]: " << itVectorInput.Get()->GetPolygonExteriorRing()->GetVertexList()->GetElement(itPoints) <<std::endl;
        }
      std::cout << "Polygon region: " << itVectorInput.Get()->GetPolygonExteriorRing()->GetBoundingRegion() <<  std::endl;
      }
    ++itVectorInput;
    }

  //----------
  // Set input of the filter
  vdReProjFilter->SetInputImage(reader->GetOutput());

  vdReProjFilter->SetInputVectorData(vdReader->GetOutput());

  //----------
  // UPDATE
  vdReProjFilter->Update();

  //----------
  // check output
  std::cout << "--- TEST OUTPUT ---" << std::endl;
  TreeIteratorType itVector(vdReProjFilter->GetOutput()->GetDataTree());
  itVector.GoToBegin();
  while (!itVector.IsAtEnd())
    {
    if (itVector.Get()->IsPolygonFeature())
      {
      std::cout << "----------\n" <<itVector.Get()->GetNodeTypeAsString() << std::endl;
      std::cout << "List of vertices: " << ": " <<std::endl;
      for (unsigned int itPoints = 0; itPoints < itVector.Get()->GetPolygonExteriorRing()->GetVertexList()->Size(); itPoints++)
        {
        std::cout << "vertex[" << itPoints << "]: " << itVector.Get()->GetPolygonExteriorRing()->GetVertexList()->GetElement(itPoints) <<std::endl;
        }
      std::cout << "Polygon region: " << itVector.Get()->GetPolygonExteriorRing()->GetBoundingRegion() <<  std::endl;
      }
    ++itVector;
    }

  return EXIT_SUCCESS;
}
