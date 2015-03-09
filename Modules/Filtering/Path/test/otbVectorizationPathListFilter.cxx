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
#include "otbVectorizationPathListFilter.h"
#include "itkPolyLineParametricPath.h"
#include "otbImageFileReader.h"
#include <fstream>

#include "otbDrawPathListFilter.h"
#include "otbImageFileWriter.h"

int otbVectorizationPathListFilter(int itkNotUsed(argc), char * argv[])
{
  const char * modfname = argv[1];
  const char * dirfname = argv[2];
  const char * outfname = argv[3];
  const char * outImagefname = argv[4];
  const double thresh = atof(argv[5]);

  const unsigned int Dimension = 2;
  typedef double                                                              PixelType;
  typedef unsigned char                                                       OutputPixelType;
  typedef otb::Image<PixelType, Dimension>                                    ImageType;
  typedef otb::Image<OutputPixelType, Dimension>                              OutputImageType;
  typedef otb::ImageFileWriter<OutputImageType>                               WriterType;
  typedef otb::ImageFileReader<ImageType>                                     ReaderType;
  typedef itk::PolyLineParametricPath<Dimension>                              PathType;
  typedef otb::DrawPathListFilter<OutputImageType, PathType, OutputImageType> DrawFilterType;
  typedef otb::VectorizationPathListFilter<ImageType, ImageType, PathType>    VectorizationPathListFilterType;
  typedef VectorizationPathListFilterType::OutputPathListType                 PathListType;
  typedef PathListType::ConstIterator                                         PathListIteratorType;
  typedef PathType::VertexListType                                            VertexListType;
  typedef VertexListType::ConstIterator                                       VertexIteratorType;
  // Instantiating objects
  VectorizationPathListFilterType::Pointer filter = VectorizationPathListFilterType::New();
  ReaderType::Pointer                      modReader = ReaderType::New();
  ReaderType::Pointer                      dirReader = ReaderType::New();

  modReader->SetFileName(modfname);
  dirReader->SetFileName(dirfname);

  filter->SetInput(modReader->GetOutput());
  filter->SetInputDirection(dirReader->GetOutput());
  filter->SetAmplitudeThreshold(thresh);
  filter->Update();

  PathListType::Pointer pathList = filter->GetOutput();
  PathListIteratorType  pathListIt = pathList->Begin();

  std::ofstream file;
  file.open(outfname);
  unsigned int counter = 0;

  while (pathListIt != pathList->End())
    {
    file << "Path " << counter << ": ";
    for (VertexIteratorType vIt = pathListIt.Get()->GetVertexList()->Begin();
         vIt != pathListIt.Get()->GetVertexList()->End();
         ++vIt)
      {
      if (vIt != pathListIt.Get()->GetVertexList()->Begin())
        {
        file << ", ";
        }
      file << vIt.Value();
      }
    file << std::endl;
    ++pathListIt;
    ++counter;
    }
  file.close();

  OutputImageType::Pointer output = OutputImageType::New();
  output->SetRegions(modReader->GetOutput()->GetLargestPossibleRegion());
  output->Allocate();
  output->FillBuffer(255);

  DrawFilterType::Pointer drawer = DrawFilterType::New();
  drawer->SetInput(output);
  drawer->SetInputPath(filter->GetOutput());
  drawer->SetPathValue(0);

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outImagefname);
  writer->SetInput(drawer->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
