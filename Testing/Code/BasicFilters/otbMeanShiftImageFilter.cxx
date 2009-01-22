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
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbImageFileWriter.h"
#include "otbMeanShiftImageFilter.h"

int otbMeanShiftImageFilter(int argc, char * argv[])
{
 if(argc != 13)
    {
      std::cerr<<"Usage: "<<argv[0]<<" infname filteredfname clusteredfname labeledclusteredfname clusterboundariesfname vectclusterboundariesfname spatialRadius rangeRadius minregionsize scale streamed threaded"<<std::endl;
      return EXIT_FAILURE;
    }

  const char *       infname                = argv[1];
  const char *       filteredfname          = argv[2];
  const char *       clusteredfname         = argv[3];
  const char *       labeledclusteredfname  = argv[4];
  const char *       clusterboundariesfname = argv[5];
  const char *       vectclusterboundariesfname = argv[6];
  const unsigned int spatialRadius          = atoi(argv[7]);
  const double       rangeRadius            = atof(argv[8]);
  const unsigned int minRegionSize          = atoi(argv[9]);
  const double       scale                  = atoi(argv[10]);
  bool               streamed               = atoi(argv[11]);
  bool               threaded               = atoi(argv[12]);

  const unsigned int Dimension = 2;
  typedef float PixelType;
  typedef otb::Image<PixelType,Dimension>                 ImageType;
  typedef otb::ImageFileReader<ImageType>                 ReaderType;
  typedef otb::StreamingImageFileWriter<ImageType>        StreamingWriterType;
  typedef otb::ImageFileWriter<ImageType>                 WriterType;
  typedef otb::MeanShiftImageFilter<ImageType,ImageType>  FilterType;
  typedef FilterType::LabeledOutputType                   LabeledImageType;
  typedef otb::StreamingImageFileWriter<LabeledImageType> LabeledStreamingWriterType;
  typedef otb::ImageFileWriter<LabeledImageType>          LabeledWriterType;

  typedef FilterType::PolygonListType                     PolygonListType;
  typedef FilterType::PolygonType                         PolygonType;
  typedef PolygonType::VertexListType                     VertexListType;
  typedef PolygonType::VertexListType::ConstIterator      VertexListConstIterator;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  


  reader->SetFileName(infname);
  

  filter->SetSpatialRadius(spatialRadius);
  filter->SetRangeRadius(rangeRadius);
  filter->SetMinimumRegionSize(minRegionSize);
  filter->SetScale(scale);

  if(!threaded)
    {
    filter->SetNumberOfThreads(1);
    }

  filter->SetInput(reader->GetOutput());

  if(streamed)
    {

    StreamingWriterType::Pointer writer1 = StreamingWriterType::New();
    StreamingWriterType::Pointer writer2 = StreamingWriterType::New();
    LabeledStreamingWriterType::Pointer writer3 = LabeledStreamingWriterType::New();
    LabeledStreamingWriterType::Pointer writer4 = LabeledStreamingWriterType::New();
    writer1->SetFileName(filteredfname);
    writer2->SetFileName(clusteredfname);
    writer3->SetFileName(labeledclusteredfname);
    writer4->SetFileName(clusterboundariesfname);
    writer1->SetInput(filter->GetOutput());
    writer2->SetInput(filter->GetClusteredOutput());
    writer3->SetInput(filter->GetLabeledClusteredOutput());
    writer4->SetInput(filter->GetClusterBoundariesOutput());
    writer1->Update();
    writer2->Update();
    writer3->Update();
    writer4->Update();

    }

  else
    {

    WriterType::Pointer writer1 = WriterType::New();
    WriterType::Pointer writer2 = WriterType::New();
    LabeledWriterType::Pointer writer3 = LabeledWriterType::New();
    LabeledWriterType::Pointer writer4 = LabeledWriterType::New();
    writer1->SetFileName(filteredfname);
    writer2->SetFileName(clusteredfname);
    writer3->SetFileName(labeledclusteredfname);
    writer4->SetFileName(clusterboundariesfname);
    writer1->SetInput(filter->GetOutput());
    writer2->SetInput(filter->GetClusteredOutput());
    writer3->SetInput(filter->GetLabeledClusteredOutput());
    writer4->SetInput(filter->GetClusterBoundariesOutput());
    writer1->Update();
    writer2->Update();
    writer3->Update();
    writer4->Update();

    }

  std::ofstream f;
  f.open(vectclusterboundariesfname);
  unsigned int index = 0;
  for(PolygonListType::Iterator it = filter->GetVectorizedClusterBoundariesOutput()->Begin();
      it != filter->GetVectorizedClusterBoundariesOutput()->End();
      ++it,++index)
    {
    f<<"Polygon: "<<index;
    f<<" Mode: "<<it.Get()->GetValue();
    f<<" Points: ";
    VertexListType::ConstPointer ptrVertexList = it.Get()->GetVertexList();

    for(unsigned int cpt=0 ; cpt<  ptrVertexList->Size() ; cpt++)
      {
      f << "["<<ptrVertexList->GetElement(cpt)[0]<<", "<<ptrVertexList->GetElement(cpt)[1]<<"] ";
      }
    f << std::endl;
    }
  f.close();

  return EXIT_SUCCESS;
}
