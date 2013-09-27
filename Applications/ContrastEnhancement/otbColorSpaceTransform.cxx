
#include <iostream>
#include <sstream>

#include "itkVectorIndexSelectionCastImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

#include "otbVectorImageColorSpaceTransformFilter.h"
#include "otbStreamingMinMaxVectorImageFilter.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbImageToVectorImageCastFilter.h"
#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"
#include "otbVectorImage.h"
#include "otbImageList.h"
#include "otbImage.h"
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

namespace otb
{
namespace Wrapper
{
class ColorSpaceTransform : public Application
{
public:

  typedef ColorSpaceTransform Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(ColorSpaceTransform, otb::Application);


private:

  void DoInit()
  {
    this->SetHaveInXML(false);
    this->SetHaveOutXML(false);
    SetName("ColorSpaceTransform");
    SetDescription("This application changes the colorspace of image. Valid color spaces are YCbCr, RGB, HSL, HSI");

    SetDocName("TransformColorSpace");
    SetDocLongDescription("This application changes the colorspace of input image. Input Image is an N-band image and -red,-green,-blue parameters specify R,G,B channels in the image. Default is 0,1,2. Valid color spaces are YCbCr, RGB, HSL, HSI");
    SetDocLimitations("only YCbCr and RGB available for now");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Analysis);
    AddDocTag("color");
    AddDocTag("histogram");

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    MandatoryOn("in");

    AddParameter(ParameterType_String, "cs", "Color Space");
    MandatoryOn("cs");

    AddParameter(ParameterType_String, "type", "Pixel type of input image(eg: uint8/uint16)");
    SetParameterString("type", "uint8");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    MandatoryOn("out");

    AddParameter(ParameterType_Int, "red", "Red channel");
    SetDefaultParameterInt("red", 0);
    AddParameter(ParameterType_Int, "green", "Green channel");
    SetDefaultParameterInt("green", 1);
    AddParameter(ParameterType_Int, "blue", "Blue channel");
    SetDefaultParameterInt("blue", 2);

    SetDocExampleParameterValue("in", "rgbInput.tif");
    SetDocExampleParameterValue("cs", "rgb2yuv");
    SetDocExampleParameterValue("type", "uint8");
    SetDocExampleParameterValue("out", "yuvOutput.tif");

  }


  void DoUpdateParameters()
  {

  }

  void DoExecute()
  {
    std::string inputFilename = GetParameterAsString("in");
    std::string colorSpace = GetParameterString("cs");
    std::string type = GetParameterString("type");


    itk::Index<3> index;
    index[0] = GetParameterInt("red");;
    index[1] = GetParameterInt("green");
    index[2] = GetParameterInt("blue");

    int delta;
    if (type == "uint8")
      delta = 128;
    else if(type == "uint16")
      delta = 32768;
    else if(type == "float")
      delta =  0.5;
    else
      std::cerr << "Unknown pixel type\n";

    typedef unsigned int PixelType;
    const unsigned int Dimension = 2;
    //typedef unsigned int OutputPixelType;
    //typedef otb::Image<PixelType, Dimension>  ImageType;

    typedef otb::VectorImage<PixelType, Dimension>   VectorType;
    typedef VectorType::PixelType VectorPixelType;

    typedef otb::VectorImage<float, Dimension>   FloatVectorType;
    typedef FloatVectorType::PixelType FloatVectorPixelType;

    typedef itk::VariableLengthVector<PixelType> MinMaxPixelType;
    typedef otb::StreamingMinMaxVectorImageFilter<VectorType> StreamingMinMaxFilterType;
    typedef otb::ImageFileReader<VectorType> VectorReaderType;

    VectorReaderType::Pointer vectorReader = VectorReaderType::New();
    vectorReader->SetFileName(inputFilename);
    vectorReader->Update();

    StreamingMinMaxFilterType::Pointer filterMinMax = StreamingMinMaxFilterType::New();
    filterMinMax->SetInput( vectorReader->GetOutput() );
    filterMinMax->Update();

    MinMaxPixelType maxPixel = filterMinMax->GetMaximum();
    MinMaxPixelType::ValueType maxPixelValue;



    maxPixelValue = maxPixel[0]; //must get the maximum of all bands
    maxPixelValue = (maxPixelValue < 255) ? 255 : maxPixelValue;  //shouldnt go below 255 in case of uint8

    std::ostringstream strm;
    std::string outputFilename = GetParameterAsString("out");
    strm << outputFilename << std::endl;

    otbAppLogDEBUG( << "MaxPixelValue = " << maxPixelValue <<std::endl );
    otbAppLogDEBUG( << "Delta = " << delta << std::endl );

    if( colorSpace == "rgb2yuv")
      {

      typedef otb::Functor::RGBToYUVFunctor<VectorPixelType,FloatVectorPixelType> RGBToYUVFunctorType;
      typedef otb::VectorImageColorSpaceTransformFilter<VectorType, FloatVectorType, RGBToYUVFunctorType> RGBToYUVFilterType;
      typedef otb::ImageFileReader<VectorType> ReaderType;

      ReaderType::Pointer reader = ReaderType::New();
      RGBToYUVFilterType::Pointer yuvFilter = RGBToYUVFilterType::New();
      //  VectorWriterType::Pointer outVectorWriter = VectorWriterType::New();

      reader->SetFileName(inputFilename);
      reader->Update();

      yuvFilter->SetInput(reader->GetOutput());
      yuvFilter->SetRGBIndex(index);
      yuvFilter->SetDelta(delta);
      yuvFilter->Update();

      SetParameterOutputImage("out", yuvFilter->GetOutput());

      }
    else if(colorSpace == "yuv2rgb")
      {
      typedef otb::Functor::YUVToRGBFunctor<FloatVectorPixelType,VectorPixelType> YUVToRGBFunctorType;
      typedef otb::VectorImageColorSpaceTransformFilter<FloatVectorType, VectorType, YUVToRGBFunctorType> YUVToRGBFilterType;
      // typedef  otb::ImageFileWriter< VectorType > VectorWriterType;
      typedef otb::ImageFileReader<FloatVectorType> ReaderType;

      ReaderType::Pointer reader = ReaderType::New();
      YUVToRGBFilterType::Pointer rgbFilter = YUVToRGBFilterType::New();

      reader->SetFileName(inputFilename);
      reader->Update();

      rgbFilter->SetInput(reader->GetOutput());
      rgbFilter->SetRGBIndex(index);
      rgbFilter->SetMaxPixelValue(maxPixelValue);
      rgbFilter->SetDelta(delta);
      rgbFilter->Update();
      SetParameterOutputImage("out", rgbFilter->GetOutput());

      }
  }
};

} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::ColorSpaceTransform)
