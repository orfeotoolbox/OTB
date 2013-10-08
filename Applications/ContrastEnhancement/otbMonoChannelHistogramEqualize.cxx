/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <iostream>
#include <sstream>

#include "itkVectorIndexSelectionCastImageFilter.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImageToVectorImageCastFilter.h"
#include "otbStreamingMinMaxVectorImageFilter.h"
#include "otbStreamingHistogramVectorImageFilter.h"
#include "otbGlobalHistogramEqualizationFilter.h"
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

namespace otb
{
namespace Wrapper
{
class MonoChannelHistogramEqualize : public Application
{
public:

  typedef MonoChannelHistogramEqualize Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(Self, otb::Application);

  typedef unsigned int PixelType;

  //typedef unsigned int OutputPixelType;
  typedef otb::Image<PixelType, 2>  ImageType;

  typedef otb::VectorImage<PixelType, 2>   VectorType;
  typedef VectorType::PixelType VectorPixelType;

  typedef otb::VectorImage<float, 2>   FloatVectorType;
  typedef FloatVectorType::PixelType FloatVectorPixelType;

  typedef otb::ImageFileReader<VectorType> VectorReaderType;

  typedef itk::VectorIndexSelectionCastImageFilter<VectorType, ImageType> IndexFilterType;
  typedef otb::ImageToVectorImageCastFilter<ImageType,VectorType> ImageToVectorImageCastFilterType;

  typedef itk::VariableLengthVector<PixelType> MinMaxPixelType;
  typedef otb::StreamingMinMaxVectorImageFilter<VectorType> StreamingMinMaxFilterType;

  typedef otb::StreamingHistogramVectorImageFilter<VectorType> HistogramFilterType;
  typedef typename itk::NumericTraits< PixelType >::RealType MeasurementType;
  typedef itk::Statistics::Histogram< MeasurementType, 1 > HistogramType;
  typedef otb::ObjectList< HistogramType > HistogramListType;

  typedef otb::GlobalHistogramEqualizationFilter<ImageType, ImageType> HistogramEqualizationFilterType;

private:

  void DoInit()
  {
    this->SetHaveInXML(false);
    this->SetHaveOutXML(false);

    SetName("MonoChannelHistogramEqualize");
    SetDescription("Perform Histogram Equalization of single band raster image");

    SetDocName("MonoChannelHistogramEqualize");
    SetDocLongDescription("Perform Histogram Equalization of single band raster image");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag("Histogram");
    AddDocTag("Contrast Enhancement");

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    MandatoryOn("in");
    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    MandatoryOn("out");

    SetDocExampleParameterValue("in", "grayInput.tif");
    SetDocExampleParameterValue("out", "grayIntput_HE_.tif");

  }

  void DoUpdateParameters()
  {
  }

  void DoExecute()
  {
    std::string inputFilename = GetParameterString("in");

    VectorType::Pointer histInput; //needs vector
    ImageType::Pointer histEquInput;

    VectorReaderType::Pointer vectorReader = VectorReaderType::New();
    vectorReader->SetFileName(inputFilename);
    vectorReader->Update();

    IndexFilterType::Pointer indexFilter = IndexFilterType::New();
    indexFilter->SetInput(vectorReader->GetOutput());
    indexFilter->SetIndex(0);
    indexFilter->Update(); //got otb::Image

    ImageToVectorImageCastFilterType::Pointer imageToVectorFilter = ImageToVectorImageCastFilterType::New();
    imageToVectorFilter->SetInput(indexFilter->GetOutput());
    imageToVectorFilter->Update();

    histInput = imageToVectorFilter->GetOutput(); //convert to otbvectorimage
    histEquInput = indexFilter->GetOutput(); //need otbimage

    StreamingMinMaxFilterType::Pointer filterMinMax = StreamingMinMaxFilterType::New();
    filterMinMax->SetInput( histInput );
    filterMinMax->Update();
    MinMaxPixelType minPixel = filterMinMax->GetMinimum();
    MinMaxPixelType maxPixel = filterMinMax->GetMaximum();

    HistogramFilterType::Pointer histogramFilter =  HistogramFilterType::New();
    histogramFilter->SetInput( histInput );

    unsigned int binestimate = maxPixel[0]- minPixel[0] + 1;
    unsigned int bins = (binestimate < 256)? 256: binestimate;
    otbAppLogDEBUG( << "bins = " << bins <<std::endl );
    //otbAppLogDEBUG( << "MinPixel = " << minPixel[0] << std::endl );
    //should also skip minmax setting since the bin size is default

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
      histEqualizeFilter->SetMinimumRange(minPixel[0]);
      histEqualizeFilter->SetHistogram(hist);
      histEqualizeFilter->Update();

      SetParameterOutputImage("out", histEqualizeFilter->GetOutput());
      }
  }

};

} //end of namespace Wrapper
} //end of namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::MonoChannelHistogramEqualize)
