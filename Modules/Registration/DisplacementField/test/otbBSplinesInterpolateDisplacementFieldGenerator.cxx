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
#include "itkPointSet.h"
#include "otbVectorImage.h"
#include "otbBSplinesInterpolateDisplacementFieldGenerator.h"
#include "otbImageFileWriter.h"

int otbBSplinesInterpolateDisplacementFieldGenerator(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  const char *       outfname = argv[1];
  typedef double                                                                     PixelType;
  typedef otb::VectorImage<PixelType, Dimension>                                     ImageType;
  typedef itk::Array<double>                                                         ParamType;
  typedef itk::PointSet<ParamType, Dimension>                                        PointSetType;
  typedef PointSetType::PointType                                                    PointType;
  typedef otb::BSplinesInterpolateDisplacementFieldGenerator<PointSetType, ImageType> FilterType;
  typedef otb::ImageFileWriter<ImageType>                                            WriterType;

  ImageType::SizeType size;
  size.Fill(100);
  double thresh = 0.9;

  // Preparing point set
  PointSetType::Pointer ps = PointSetType::New();
  PointType             p1, p2, p3, p4, p5;
  ParamType             pd1(3), pd2(3), pd3(3), pd4(3), pd5(3);

  p1[0] = 10;
  p1[1] = 10;
  p2[0] = 75;
  p2[1] = 10;
  p3[0] = 50;
  p3[1] = 50;
  p4[0] = 10;
  p4[1] = 60;
  p5[0] = 85;
  p5[1] = 70;

  pd1[0] = 0.95;
  pd1[1] = 10;
  pd1[2] = -5;
  pd2[0] = 0.98;
  pd2[1] = 2;
  pd2[2] = 5;
  pd3[0] = 0.5;
  pd3[1] = 20;
  pd3[2] = -20;
  pd4[0] = 0.91;
  pd4[1] = 15;
  pd4[2] = -5;
  pd5[0] = 0.91;
  pd5[1] = 5;
  pd5[2] = 5;

  ps->SetPoint(0, p1);
  ps->SetPointData(0, pd1);
  ps->SetPoint(1, p2);
  ps->SetPointData(1, pd2);
  ps->SetPoint(2, p3);
  ps->SetPointData(2, pd3);
  ps->SetPoint(3, p4);
  ps->SetPointData(3, pd4);
  ps->SetPoint(4, p5);
  ps->SetPointData(4, pd5);

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();
  filter->SetOutputSize(size);
  filter->SetMetricThreshold(thresh);
  filter->SetPointSet(ps);

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();

  return EXIT_SUCCESS;
}
