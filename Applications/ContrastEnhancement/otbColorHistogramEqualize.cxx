
#include <iostream>
#include <sstream>

#include "itkRescaleIntensityImageFilter.h"
#include "itkVectorIndexSelectionCastImageFilter.h"

#include "otbImageToVectorImageCastFilter.h"
#include "otbImageList.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingHistogramVectorImageFilter.h"
#include "otbStreamingMinMaxVectorImageFilter.h"
#include "otbGlobalHistogramEqualizationFilter.h"
#include "otbVectorImageColorSpaceTransformFilter.h"
#include "otbImageFileWriter.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbRGBToYUVFunctor.h"
#include "otbYUVToRGBFunctor.h"

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

namespace otb
{
namespace Wrapper
{
class ColorHistogramEqualize : public Application
{
public:

  typedef ColorHistogramEqualize Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(Self, otb::Application);

  typedef unsigned int PixelType;
  // typedef unsigned int OutputPixelType;

  typedef otb::Image<PixelType, 2>  ImageType;
  typedef otb::VectorImage<PixelType, 2>   VectorType;
  typedef otb::VectorImage<float, 2>   FloatVectorType;
  typedef FloatVectorType::PixelType FloatVectorPixelType;
  typedef VectorType::PixelType VectorPixelType;
  typedef otb::Image<float, 2>  FloatImageType;

  typedef otb::StreamingMinMaxVectorImageFilter<VectorType> StreamingMinMaxFilterType;
  typedef itk::VariableLengthVector<PixelType> MinMaxPixelType;

  typedef otb::StreamingHistogramVectorImageFilter<VectorType> HistogramFilterType;
  typedef typename itk::NumericTraits< float >::RealType MeasurementType;
  typedef itk::Statistics::Histogram< MeasurementType, 1 > HistogramType;
  typedef otb::ObjectList< HistogramType > HistogramListType;
  typedef otb::GlobalHistogramEqualizationFilter<ImageType, FloatImageType> HistogramEqualizationFilterType;

  typedef otb::Functor::RGBToYUVFunctor<VectorPixelType,FloatVectorPixelType> YUVFunctorType;
  typedef otb::VectorImageColorSpaceTransformFilter<VectorType, FloatVectorType, YUVFunctorType> YUVFilterType;
  typedef itk::VectorIndexSelectionCastImageFilter<FloatVectorType, FloatImageType> VectorCastFilterType;
  typedef itk::RescaleIntensityImageFilter<FloatImageType, ImageType> FloatRescalerType;
  typedef otb::ImageToVectorImageCastFilter<ImageType,VectorType> ImageToVectorImageCastFilterType;
  typedef otb::ImageList<FloatImageType>  ImageListType;
  typedef otb::ImageListToVectorImageFilter<ImageListType, FloatVectorType > ListConcatenerFilterType;
  typedef otb::Functor::YUVToRGBFunctor<FloatVectorPixelType,VectorPixelType> YUVToRGBFunctorType;
  typedef otb::VectorImageColorSpaceTransformFilter<FloatVectorType, VectorType, YUVToRGBFunctorType> RGBFilterType;

private:

  void DoInit()
  {
    this->SetHaveInXML(false);
    this->SetHaveOutXML(false);
    SetName("ColorHistogramEqualize");
    SetDescription("Perform Histogram Equalization of Multi band raster image");

    SetDocName("ColorHistogramEqualize");
    SetDocLongDescription("Perform Histogram Equalization of Multi band raster image");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("ColorSpaceTransform");

    AddDocTag("Contrast Enhancement");
    AddDocTag("color histogram");

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    MandatoryOn("in");

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
    SetDocExampleParameterValue("type", "uint8");
    SetDocExampleParameterValue("out", "yuvOutput.tif");

  }

  void DoUpdateParameters()
  {
  }

  void DoExecute()
  {
    std::string inputFilename = GetParameterAsString("in");
    std::string type = GetParameterString("type");
    itk::Index<3> index;
    index[0] = GetParameterInt("red");;
    index[1] = GetParameterInt("green");
    index[2] = GetParameterInt("blue");

    int delta = 128;
    if (type == "uint8")
      delta = 128;
    else if(type == "uint16")
      delta = 32768;
    else if(type == "float")
      delta =  0.5;
    else
      std::cerr << "Unknown pixel type\n";

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

    YUVFilterType::Pointer yuvFilter = YUVFilterType::New();
    yuvFilter->SetInput(vectorReader->GetOutput());
    yuvFilter->SetDelta(delta);
    yuvFilter->SetRGBIndex(index);
    yuvFilter->Update();

    VectorCastFilterType::Pointer vectorCastFilter1 = VectorCastFilterType::New();
    vectorCastFilter1->SetInput(yuvFilter->GetOutput());
    vectorCastFilter1->SetIndex(0);
    vectorCastFilter1->Update();

    FloatRescalerType::Pointer floatRescaler1 = FloatRescalerType::New();
    floatRescaler1->SetOutputMinimum(0);
    floatRescaler1->SetOutputMaximum(maxPixelValue );
    floatRescaler1->SetInput(vectorCastFilter1->GetOutput()); //Y
    floatRescaler1->Update();

    VectorCastFilterType::Pointer vectorCastFilter2 = VectorCastFilterType::New();
    vectorCastFilter2->SetInput(yuvFilter->GetOutput());
    vectorCastFilter2->SetIndex(1);
    vectorCastFilter2->Update();

    VectorCastFilterType::Pointer vectorCastFilter3 = VectorCastFilterType::New();
    vectorCastFilter3->SetInput(yuvFilter->GetOutput());
    vectorCastFilter3->SetIndex(2);
    vectorCastFilter3->Update();

    ImageToVectorImageCastFilterType::Pointer imageToVectorFilter = ImageToVectorImageCastFilterType::New();
    imageToVectorFilter->SetInput(floatRescaler1->GetOutput());
    imageToVectorFilter->Update();

    VectorType::Pointer histInput;
    ImageType::Pointer histEquInput;

    histInput = imageToVectorFilter->GetOutput(); //conversion of indexFilter->out
    histEquInput = floatRescaler1->GetOutput();

    filterMinMax->SetInput( histInput );
    filterMinMax->Update();
    MinMaxPixelType minPixel = filterMinMax->GetMinimum();
    maxPixel = filterMinMax->GetMaximum();

    unsigned int binestimate = maxPixel[0]- minPixel[0] + 1;
    unsigned int bins = (binestimate < 256)? 256: binestimate ;

    otbAppLogDEBUG( << "MaxPixelValue = " << maxPixelValue <<std::endl );
    otbAppLogDEBUG( << "Delta = " << delta << std::endl );
    otbAppLogDEBUG( << "Bins = " << bins << std::endl );

    HistogramFilterType::Pointer histogramFilter =  HistogramFilterType::New();
    histogramFilter->SetInput( histInput );
    histogramFilter->GetFilter()->SetHistogramMin( minPixel );
    histogramFilter->GetFilter()->SetHistogramMax( maxPixel );
    histogramFilter->GetFilter()->SetNumberOfBins( bins );
    histogramFilter->GetFilter()->SetSubSamplingRate( 1 );
    histogramFilter->Update();

    HistogramEqualizationFilterType::Pointer histEqualizeFilter = HistogramEqualizationFilterType::New();

    HistogramListType::Pointer histList = histogramFilter->GetHistogramList();
    for( HistogramListType::ConstIterator it( histList->Begin() );   it!=histList->End();  ++it )
      {

      HistogramType::Pointer hist = it.Get();
      histEqualizeFilter->SetInput( histEquInput );

      // histEqualizeFilter->SetMinimumRange(0);
      histEqualizeFilter->SetMinimumRange(minPixel[0]);
      histEqualizeFilter->SetHistogram(hist);
      histEqualizeFilter->Update();

      /*
      typedef  otb::ImageFileWriter< VectorType > FloatImageWriterType;
      typedef  otb::ImageFileWriter< FloatImageType > ImageWriterType;
      ImageWriterType::Pointer outImageWriter = ImageWriterType::New();
      outImageWriter->SetFileName(strm.str());
      outImageWriter->SetInput(histEqualizeFilter->GetOutput());
      outImageWriter->Update();
      std::cerr << strm.str() << "\n";
      */
      }

    ///YCbCr -> rgb
    ImageListType::Pointer  m_ImageList = ImageListType::New();
    m_ImageList->PushBack( histEqualizeFilter->GetOutput() ); //Y band equalized
    m_ImageList->PushBack( vectorCastFilter2->GetOutput() ); //Cb
    m_ImageList->PushBack( vectorCastFilter3->GetOutput() ); //Cr

    ListConcatenerFilterType::Pointer  m_Concatener = ListConcatenerFilterType::New();

    m_Concatener->SetInput( m_ImageList );
    m_Concatener->Update();

    RGBFilterType::Pointer rgbFilter = RGBFilterType::New();
    rgbFilter->SetMaxPixelValue(maxPixelValue);
    rgbFilter->SetDelta(delta);
    rgbFilter->SetInput(m_Concatener->GetOutput());
    rgbFilter->SetRGBIndex(index);
    rgbFilter->Update();

    SetParameterOutputImage("out", rgbFilter->GetOutput());
  }

};
} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::ColorHistogramEqualize)
