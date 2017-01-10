#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRGBPixel.h"
#include "itkImageRegionConstIterator.h"


//
// Taken from VTK : Imaging/vtkImageRGBToHSV
//
void ConvertRGBToHSV( float R, float G, float B, float & H, float & S, float & V )
{
  
  const float max = 255.0;

  // Saturation
  float temp = R;
  if (G < temp)
    {
    temp = G;
    }
  if (B < temp)
    {
    temp = B;
    }
  float sumRGB = R+G+B;
  if(sumRGB == 0.0)
    {
    S = 0.0;
    }
  else
    {
    S = max * (1.0 - (3.0 * temp / sumRGB));
    }
  
  temp = (float)(R + G + B);
  // Value is easy
  V = temp / 3.0;
  
  // Hue
  temp = sqrt((R-G)*(R-G) + (R-B)*(G-B));
  if(temp != 0.0)
    {
    temp = acos((0.5 * ((R-G) + (R-B))) / temp);
    }
  if (G >= B)
    {
    H = max * (temp / 6.2831853);
    }
  else
    {
    H = max * (1.0 - (temp / 6.2831853));
    }
  
}




int main(int argc, char * argv[] )
{

  if( argc < 3 )
  {
    std::cerr << "VWSegmentation  inputFile histogramHSVFile" << std::endl;
    return -1;
  }
  
  typedef unsigned char                        PixelComponentType;
  typedef itk::RGBPixel<PixelComponentType>    InputPixelType;
  typedef unsigned short                       OutputPixelType;

  typedef itk::Image< InputPixelType,  3 >   InputImageType;
  typedef itk::Image< OutputPixelType, 3 >   OutputImageType;

  typedef itk::ImageFileReader< InputImageType  > ReaderType;
  typedef itk::ImageFileWriter< OutputImageType > WriterType;

  typedef OutputImageType::RegionType    RegionType;
  typedef OutputImageType::SizeType      SizeType;
  typedef OutputImageType::IndexType     IndexType;

  InputImageType::Pointer inputImage;

  { // local scope for destroying the reader
    
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( argv[1] );
    try
      {
      reader->Update();
      }
    catch( itk::ExceptionObject & excp )
      {
      std::cout << excp << std::endl;
      return -1;
      }
    inputImage = reader->GetOutput();
  }  

  RegionType region;
  SizeType   size;
  IndexType  start;

  start.Fill( 0 );

  size[0] = 256;
  size[1] = 256;
  size[2] = 256;

  region.SetSize( size );
  region.SetIndex( start );

  OutputImageType::Pointer histogramImage = OutputImageType::New();
  histogramImage->SetRegions( region );
  histogramImage->Allocate();
  histogramImage->FillBuffer( 0 );


  typedef itk::ImageRegionConstIterator< InputImageType > IteratorType;

  IndexType index;

  IteratorType it( inputImage, inputImage->GetBufferedRegion() );

  std::cout << "Starting first pass for computing max HSV values " << std::endl;
  it.GoToBegin();

  float hue;
  float saturation;
  float value;

  float hueMax = 0;
  float saturationMax = 0;
  float valueMax = 0;

  while( !it.IsAtEnd() )
    {
    InputPixelType pixel = it.Get();
    PixelComponentType red   = pixel.GetRed();
    PixelComponentType green = pixel.GetGreen();
    PixelComponentType blue  = pixel.GetBlue();

    ConvertRGBToHSV( red, green, blue, hue, saturation, value );

    if( hue > hueMax ) 
      {
      hueMax = hue;
      }
    if( saturation > saturationMax )
      {
      saturationMax = saturation;
      }
    if( value > valueMax )
      {
      valueMax = value;
      }

    ++it;
    }


  std::cout << "Maximum values found: " << std::endl;
  std::cout << "H: " << hueMax << std::endl;
  std::cout << "S: " << saturationMax << std::endl;
  std::cout << "V: " << valueMax << std::endl;


  it.GoToBegin();

  while( !it.IsAtEnd() )
    {
    InputPixelType pixel = it.Get();
    PixelComponentType red   = pixel.GetRed();
    PixelComponentType green = pixel.GetGreen();
    PixelComponentType blue  = pixel.GetBlue();

    ConvertRGBToHSV( red, green, blue, hue, saturation, value );

    index[0] = static_cast<int>( 255.0 * hue/ hueMax );  
    index[1] = static_cast<int>( 255.0 * saturation / saturationMax );
    index[2] = static_cast<int>( 255.0 * value / valueMax );

    OutputPixelType count = histogramImage->GetPixel( index );
    count++;
    histogramImage->SetPixel( index, count );
    ++it;
    }

  std::cout << "End of HSV histogram computation" << std::endl;

  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName( argv[2] );
  writer->SetInput( histogramImage );


  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cout << excp << std::endl;
    return -1;
    }

  
  return 0;
}

