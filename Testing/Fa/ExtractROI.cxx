#include "otbImageFileReader.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbLeeImageFilter.h"
#include "otbFrostImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbImageFileWriter.h"



int main(int argc, char** argv)
{

  if( argc < 7 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile  x0 y0 b0 xsize ysize outputImageFile " << std::endl;
    return -1;
    }


  typedef double InputPixelType;
  typedef unsigned char OutputPixelType;
  const unsigned int Dimension=2;
  
  typedef itk::VectorImage< InputPixelType, Dimension > InputImageType;

  typedef itk::Image< OutputPixelType, Dimension > OutputImageType;

    typedef itk::Image< InputPixelType, Dimension > InternalImageType;

  typedef otb::ImageFileReader< InputImageType >       ReaderType;

  typedef otb::ImageFileWriter< OutputImageType >           WriterType;
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  

  typedef otb::MultiToMonoChannelExtractROI< InputPixelType, 
                                             InputPixelType >  ExtractROIFilterType;


  ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();



  typedef itk::RescaleIntensityImageFilter< InternalImageType,
                                            OutputImageType > RescalerType; 



  
  reader->SetFileName( argv[1]  );
  writer->SetFileName( argv[7] );


  extractROIFilter->SetInput( reader->GetOutput() );


  RescalerType::Pointer rescaler = RescalerType::New();


  extractROIFilter->SetStartX(atoi(argv[2]));
  extractROIFilter->SetStartY(atoi(argv[3]));

  extractROIFilter->SetSizeX(atoi(argv[5]));//reader->GetOutput()->GetBufferedRegion().GetSize()[0]);
  extractROIFilter->SetSizeY(atoi(argv[6]));//reader->GetOutput()->GetBufferedRegion().GetSize()[1]);

  extractROIFilter->SetChannel(atoi(argv[4]));


  rescaler->SetOutputMinimum( itk::NumericTraits< OutputPixelType >::min());
  rescaler->SetOutputMaximum( itk::NumericTraits< OutputPixelType >::max());

  rescaler->SetInput( extractROIFilter->GetOutput() );
  writer->SetInput( rescaler->GetOutput() );

  writer->Update(); 

	

  try
    {
        writer->Update(); 
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 

return 0;

}
