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
#include "otbWrapperApplicationFactory.h"

#include "otbStreamingLineSegmentDetector.h"

#include "otbVectorImageToAmplitudeImageFilter.h"
#include "otbStreamingStatisticsImageFilter.h"
#include "itkShiftScaleImageFilter.h"

#include "otbVectorDataProjectionFilter.h"
#include "otbVectorDataTransformFilter.h"
#include "itkAffineTransform.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"

namespace otb
{
namespace Wrapper
{

class LineSegmentDetection: public Application
{
public:
  /** Standard class typedefs. */
  typedef LineSegmentDetection          Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(LineSegmentDetection, otb::Wrapper::Application);

private:
  void DoInit()
  {
    SetName("LineSegmentDetection");
    SetDescription("Detect line segments in raster");

    // Documentation
    SetDocName("Line segment detection");
    SetDocLongDescription("This application detects locally straight contours in a image. It is based on Burns, Hanson, and Riseman method and use an a contrario validation approach (Desolneux, Moisan, and Morel). The algorithm was published by Rafael Gromponevon Gioi, Jérémie Jakubowicz, Jean-Michel Morel and Gregory Randall.\n The given approach computes gradient and level lines of the image and detects aligned points in line support region. The application allows exporting the detected lines in a vector data.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("On Line demonstration of the LSD algorithm is available here: http://www.ipol.im/pub/algo/gjmr_line_segment_detector/\n");

    AddDocTag(Tags::FeatureExtraction);

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in"," Input image on which lines will be detected.");

    AddParameter(ParameterType_OutputVectorData, "out", "Output Detected lines");
    SetParameterDescription("out"," Output detected line segments (vector data).");

    // Elevation
    ElevationParametersHandler::AddElevationParameters(this, "elev");

    AddParameter(ParameterType_Empty, "norescale", "No rescaling in [0, 255]");
    SetParameterDescription("norescale","By default, the input image amplitude is rescaled between [0,255]. Turn on this parameter to skip rescaling");
    MandatoryOff("norescale");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_Suburb.png");
    SetDocExampleParameterValue("out", "LineSegmentDetection.shp");
  }

  void DoUpdateParameters()
  {
  }

  void DoExecute()
  {
    typedef otb::VectorImageToAmplitudeImageFilter<FloatVectorImageType, FloatImageType>
      VectorImageToAmplitudeImageFilterType;

    typedef otb::StreamingStatisticsImageFilter<FloatImageType>
      StreamingStatisticsImageFilterType;

    typedef itk::ShiftScaleImageFilter<FloatImageType, FloatImageType>
      ShiftScaleImageFilterType;

    typedef otb::StreamingLineSegmentDetector
      < FloatImageType >::FilterType LSDFilterType;

    VectorImageToAmplitudeImageFilterType::Pointer amplitudeConverter
      = VectorImageToAmplitudeImageFilterType::New();

    amplitudeConverter->SetInput( GetParameterImage("in") );

    FloatImageType::Pointer image = amplitudeConverter->GetOutput();

    StreamingStatisticsImageFilterType::Pointer stats
       = StreamingStatisticsImageFilterType::New();

    ShiftScaleImageFilterType::Pointer shiftScale
      = ShiftScaleImageFilterType::New();

    // Default behavior is to do the rescaling
    if ( !IsParameterEnabled("norescale") )
      {
      stats->SetInput(amplitudeConverter->GetOutput());

      AddProcess(stats->GetStreamer(), "Image statistics");
      stats->Update();
      FloatImageType::PixelType min = stats->GetMinimum();
      FloatImageType::PixelType max = stats->GetMaximum();

      shiftScale->SetInput(amplitudeConverter->GetOutput());
      shiftScale->SetShift( -min );
      shiftScale->SetScale( 255.0 / (max - min) );

      image = shiftScale->GetOutput();
      }

    LSDFilterType::Pointer lsd
      = LSDFilterType::New();
    lsd->GetFilter()->SetInput(image);

    AddProcess(lsd->GetStreamer(), "Running Line Segment Detector");
    lsd->Update();

    /*
     * Reprojection of the output VectorData
     *
     * The output of LSDFilterType is in image physical coordinates,
     * projection WKT applied if the input image has one
     *
     * We need to reproject in WGS84 if the input image is in sensor model geometry
     */

    std::string projRef = GetParameterImage("in")->GetProjectionRef();
    ImageKeywordlist kwl = GetParameterImage("in")->GetImageKeywordlist();

    VectorDataType::Pointer vd = lsd->GetFilter()->GetOutputVectorData();

    VectorDataType::Pointer projectedVD = vd;

    if ( projRef.empty() && kwl.GetSize() > 0 )
      {
      // image is in sensor model geometry

      // Reproject VectorData in image projection
      typedef otb::VectorDataProjectionFilter
        <VectorDataType, VectorDataType>                     VectorDataProjectionFilterType;

      VectorDataProjectionFilterType::Pointer vproj = VectorDataProjectionFilterType::New();
      vproj->SetInput(vd);
      vproj->SetInputKeywordList(GetParameterImage("in")->GetImageKeywordlist());
      //vproj->SetInputOrigin(GetParameterImage("in")->GetOrigin());
      //vproj->SetInputSpacing(GetParameterImage("in")->GetSpacing());

      // Setup the DEM Handler
      otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"elev");

      AddProcess(vproj, "Reprojecting output vector data");
      vproj->Update();

      projectedVD = vproj->GetOutput();
      }

    SetParameterOutputVectorData("out", projectedVD);
  }

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::LineSegmentDetection)
