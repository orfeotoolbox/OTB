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
#include "itkMacro.h"
#include "itkLabelMapToLabelImageFilter.h"

#include "itkTimeProbe.h"
#include "otbMacro.h"
#include "otbImage.h"
//#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorDataToLabelMapFilter.h"

#include "otbVectorDataFileReader.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "otbAttributesMapLabelObject.h"

#include "otbVectorDataProperties.h"
#include "itkNumericTraits.h"

int otbVectorDataToLabelMapFilter(int argc, char * argv[])
{

  /** Rasterize vectorData to LabelMap (only available for polygon yet)*/
  if (argc != 3)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputFile(shp) outputLabeledImagefile" << std::endl;
    return EXIT_FAILURE;
    }

  const char * infname = argv[1];
  const char * outfname = argv[2];
  //const char * outvectorfname = argv[3];

  // Labeled image type
  const unsigned int Dimension                 = 2;
  typedef unsigned short                         LabelType;
  typedef otb::Image<LabelType, Dimension>       LabeledImageType;
  typedef otb::ImageFileWriter<LabeledImageType> WriterType;

  // Label map typedef
  typedef otb::AttributesMapLabelObject<LabelType, Dimension, double> LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>                              LabelMapType;

  typedef otb::VectorData<double, Dimension>                            VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType>                     ReaderType;
  typedef otb::VectorDataToLabelMapFilter<VectorDataType, LabelMapType> VectorDataToLabelMapFilterType;

  typedef itk::LabelMapToLabelImageFilter<LabelMapType, LabeledImageType> LabelMapToLabelImageFilterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  //Update the reader
  reader->Update();

  VectorDataToLabelMapFilterType::Pointer myFilter = VectorDataToLabelMapFilterType::New();
  myFilter->SetInput(reader->GetOutput());

  //TODO Simplify the use of the class VectorDataProperties
  //VectorDataProperties is needed to compute the ROI of the vectorData
  typedef otb::VectorDataProperties <VectorDataType> VectorDataPropertiesType;
  VectorDataPropertiesType::Pointer p = VectorDataPropertiesType::New();
  //set the input vectordata to the properties properties
  p->SetVectorDataObject(reader->GetOutput());
  typedef VectorDataType::DataNodeType::PolygonType::RegionType RegionType;
  RegionType region;
  typedef RegionType::IndexType index;
  typedef RegionType::SizeType  size;

  p->SetBoundingRegion(region);

  //Compute the global bounding box of the vectordata
  p->ComputeBoundingRegion();

  LabelMapType::PointType origin;

  origin[0] = p->GetBoundingRegion().GetImageRegion().GetIndex(0);
  origin[1] = p->GetBoundingRegion().GetImageRegion().GetIndex(1);

  //Set size, origin and spacing of the output labelmap
  myFilter->SetSize(p->GetBoundingRegion().GetImageRegion().GetSize());
  myFilter->SetOrigin(origin);
  myFilter->SetSpacing(reader->GetOutput()->GetSpacing());

  // Translate the LabelMap in a labeld image
  LabelMapToLabelImageFilterType::Pointer labelMapToImageFilter = LabelMapToLabelImageFilterType::New();
  labelMapToImageFilter->SetInput(myFilter->GetOutput());

  //Write the result to an image file
  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName(outfname);
  writer->SetInput(labelMapToImageFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
