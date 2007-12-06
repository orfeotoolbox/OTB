
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbSimpleRcsPanSharpeningFusionImageFilter.h"


int main( int argc, char* argv[] )
{
  typedef otb::Image<double, 2>     ImageType;
  typedef otb::VectorImage<double, 2>     VectorImageType;
  typedef otb::ImageFileReader<ImageType>  ReaderType;
  typedef otb::ImageFileReader<VectorImageType>  ReaderVectorType;
  
  typedef otb::VectorImage<unsigned int, 2>     VectorIntImageType;


					
  ReaderVectorType::Pointer     	readerXS=ReaderVectorType::New();
  ReaderType::Pointer     	readerPAN=ReaderType::New();

  
  readerPAN->SetFileName(argv[1]);
  readerXS->SetFileName(argv[2]);
  
  typedef otb::SimpleRcsPanSharpeningFusionImageFilter
      <ImageType,VectorImageType,VectorIntImageType> FusionFilterType;
  FusionFilterType::Pointer fusion = FusionFilterType::New();
  fusion->SetPanInput(readerPAN->GetOutput());
  fusion->SetXsInput(readerXS->GetOutput());
      
      
  typedef otb::StreamingImageFileWriter<VectorIntImageType>  WriterType;
  WriterType::Pointer	    	writer=WriterType::New();
  writer->SetFileName(argv[3]);
  writer->SetInput(fusion->GetOutput());
  writer->Update();

  return 0;
  
}


