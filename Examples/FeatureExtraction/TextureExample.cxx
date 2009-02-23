
#include "itkExceptionObject.h"

#include "otbUnaryFunctorNeighborhoodWithOffsetImageFilter.h"
#include "otbVectorImage.h"
#include "itkConstNeighborhoodIterator.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"


#include "otbTextureFunctors.h"


template<class TInputImage, class TOutputImage, class TFunctor>
int generic_TextureFunctor(std::string inputFileName, std::string outputFileName, unsigned int radius, unsigned int xOffset, unsigned int yOffset)
{


}

int main(int argc, char * argv[])
{
  // Parse command line parameters
  
  const char* infname   = argv[1];
  const char* outfname  = argv[2];

  const unsigned int radius =   static_cast<unsigned int>(atoi(argv[3]));
  const unsigned int xOffset =  static_cast<unsigned int>(atoi(argv[4]));
  const unsigned int yOffset =  static_cast<unsigned int>(atoi(argv[5]));

 
  typedef double InputPixelType;
  const int Dimension = 2;
  typedef otb::VectorImage<InputPixelType,Dimension> ImageType;
  typedef ImageType::PixelType                       PixelType;
  typedef itk::ConstNeighborhoodIterator<ImageType>  IteratorType;


  typedef otb::Functor::ContrastTextureFunctor<IteratorType, PixelType>
                                                             FunctorType;
  
  typedef ImageType::OffsetType OffsetType;
  typedef otb::ImageFileReader<ImageType>  ReaderType;
  typedef otb::StreamingImageFileWriter<ImageType> WriterType;
  
  typedef otb::UnaryFunctorNeighborhoodWithOffsetImageFilter<ImageType,
                                         ImageType, FunctorType> FilterType;

  // Instantiating object
  FilterType::Pointer object = FilterType::New();
  ReaderType::Pointer reader  = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  
  reader->SetFileName(infname);
  writer->SetFileName(outfname);

  object->SetInput(reader->GetOutput());
  object->SetRadius(radius);
  OffsetType offset;
  offset[0] =  xOffset;
  offset[1] =  yOffset;

  object->SetOffset(offset);
  writer->SetInput(object->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
