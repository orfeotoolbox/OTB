#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRGBPixel.h"
#include "itkImageRegionIterator.h"


int main(int argc, char * argv[] )
{

  if( argc < 3 )
  {
    std::cerr << "VWBlueRemoval  inputFile outputFile" << std::endl;
    return -1;
  }
  
  typedef unsigned char                           PixelComponentType;
  typedef itk::RGBPixel<PixelComponentType>       ImagePixelType;

  typedef itk::Image< ImagePixelType, 3 >         ImageType;

  typedef itk::ImageFileReader< ImageType  >      ImageReaderType;
  typedef itk::ImageFileWriter< ImageType >       ImageWriterType;


  ImageType::Pointer image;

  { // Local scoope for destroying the reader
    ImageReaderType::Pointer imageReader = ImageReaderType::New();
    imageReader->SetFileName( argv[1] );
    try
      {
      imageReader->Update();
      }
    catch( itk::ExceptionObject & excp )
      {
      std::cout << excp << std::endl;
      return -1;
      }

    image = imageReader->GetOutput();
  }

  typedef itk::ImageRegionIterator< ImageType > IteratorType;

  IteratorType it( image, image->GetBufferedRegion() );
  it.GoToBegin();


  //
  // Separatrix Plane Coefficients
  //
  const double A = -48.0;
  const double B =   0.0;
  const double C =  59.0;
  const double D = 106.0;


  //
  // RGB color to replace blue
  //
  ImagePixelType replaceValue;
  replaceValue[0] = 0;
  replaceValue[1] = 0;
  replaceValue[2] = 0;


  //
  //  In place replacement
  //
  while( !it.IsAtEnd() )
    {
    ImagePixelType pixel = it.Get();
    const PixelComponentType red   = pixel.GetRed();
    const PixelComponentType green = pixel.GetGreen();
    const PixelComponentType blue  = pixel.GetBlue();

    const double distanceToPlane =
                    A * red + B * green + C * blue + D;
      
    const bool pixelIsOnBlueSide = ( distanceToPlane > 0 );

    if( pixelIsOnBlueSide )
      {
      it.Set( replaceValue );
      }
    ++it;
    }


  ImageWriterType::Pointer imageWriter = ImageWriterType::New();

  imageWriter->SetFileName( argv[2] );
  imageWriter->SetInput( image );


  try
    {
    imageWriter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cout << excp << std::endl;
    return -1;
    }

  
  return 0;
}

