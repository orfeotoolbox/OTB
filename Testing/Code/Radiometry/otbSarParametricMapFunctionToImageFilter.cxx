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
#include "otbImage.h"
#include "otbFunctionToImageFilter.h"
#include "otbSarParametricMapFunction.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "itkPointSet.h"


int otbSarParametricMapFunctionToImageFilter(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef float                                                                     PixelType;
  typedef otb::Image<PixelType, Dimension>                                          InputImageType;
  typedef InputImageType::IndexType                                                 IndexType;
  typedef otb::Image<PixelType, Dimension>                                          OutputImageType;
  typedef otb::ImageFileReader<InputImageType>                                      ReaderType;
  typedef otb::StreamingImageFileWriter<OutputImageType>                            WriterType;
  typedef otb::SarParametricMapFunction<InputImageType>                             FunctionType;
  typedef otb::FunctionToImageFilter<InputImageType, OutputImageType, FunctionType> FilterType;
  typedef FunctionType::PointSetType                                                PointSetType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  PointSetType::Pointer points = PointSetType::New();

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);
  filter->SetInput(reader->GetOutput());

  FunctionType::Pointer function = FunctionType::New();
  function->SetInputImage(reader->GetOutput());

  /** Test on some indexes and some physical coordinates*/
  FunctionType::PointType  p0;
  PixelType                value;
 
  p0[0] = static_cast<unsigned int>(0);
  p0[1] = static_cast<unsigned int>(0);
  value = 10.0;
  points->SetPoint(0, p0);
  points->SetPointData(0, value);
  
  p0[0] = static_cast<unsigned int>(50);
  p0[1] = static_cast<unsigned int>(50);
  value = 100.0;
  points->SetPoint(1, p0);
  points->SetPointData(1, value);

  p0[0] = static_cast<unsigned int>(100);
  p0[1] = static_cast<unsigned int>(100);
  value = 50.0;
  points->SetPoint(3, p0);
  points->SetPointData(1, value);


  function->SetPointSet(points);

  IndexType       polynomalSize;
  polynomalSize[0] = 2;
  polynomalSize[1] = 0;
  function->SetPolynomalSize(polynomalSize);

  std::cout << function << std::endl;     


  filter->SetFunction(function);

  writer->SetInput(filter->GetOutput());
  writer->SetNumberOfStreamDivisions(2);
  writer->Update();

  return EXIT_SUCCESS;
}
