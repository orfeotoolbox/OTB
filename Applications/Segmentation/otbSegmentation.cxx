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
// Wrappers
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbWrapperChoiceParameter.h"

// Segmentation filters includes
#include "otbMeanShiftVectorImageFilter.h"
#include "otbMeanShiftSegmentationFilter.h"
#include "otbConnectedComponentMuParserFunctor.h"
#include "itkConnectedComponentFunctorImageFilter.h"
#include "otbMaskMuParserFilter.h"
#include "otbVectorImageToAmplitudeImageFilter.h"
#include "itkGradientMagnitudeImageFilter.h"
#include "otbWatershedSegmentationFilter.h"

// Large scale vectorization framework
#include "otbStreamingVectorizedSegmentationOGR.h"
#include "otbOGRDataSourceWrapper.h"

// Fusion filter
#include "otbFusionOGRTileFilter.h"

namespace otb
{
namespace Wrapper
{
class Segmentation : public Application
{
public:
  /** Standard class typedefs. */
  typedef Segmentation        Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Images typedefs */
  typedef UInt32ImageType               LabelImageType;
  typedef UInt32ImageType               MaskImageType;

  // Segmentation filters typedefs
  // Edison mean-shift
  typedef otb::MeanShiftVectorImageFilter
  <FloatVectorImageType,
   FloatVectorImageType,
   LabelImageType>                        EdisonSegmentationFilterType;

  // Home made mean-shift
  typedef otb::MeanShiftSegmentationFilter
  <FloatVectorImageType,
   LabelImageType,
   FloatVectorImageType>                  MeanShiftSegmentationFilterType;

  // Simple connected components
  typedef otb::Functor::ConnectedComponentMuParserFunctor
  <FloatVectorImageType::PixelType>       FunctorType;

  typedef itk::ConnectedComponentFunctorImageFilter
  <FloatVectorImageType,
   LabelImageType,
   FunctorType,
   MaskImageType >                        ConnectedComponentSegmentationFilterType;

  // Watershed
  typedef otb::VectorImageToAmplitudeImageFilter
  <FloatVectorImageType,
   FloatImageType>                        AmplitudeFilterType;

  typedef itk::GradientMagnitudeImageFilter
  <FloatImageType,FloatImageType>         GradientMagnitudeFilterType;

  typedef otb::WatershedSegmentationFilter
  <FloatImageType,LabelImageType>         WatershedSegmentationFilterType;


  // mask filter
  typedef otb::MaskMuParserFilter
  <FloatVectorImageType, MaskImageType>   MaskMuParserFilterType;

  // Vectorize filters
  // Edison mean-shift
  typedef otb::StreamingVectorizedSegmentationOGR
  <FloatVectorImageType,
   EdisonSegmentationFilterType>          EdisontreamingVectorizedSegmentationOGRType;

  // Home made mean-shift
  typedef otb::StreamingVectorizedSegmentationOGR
  <FloatVectorImageType,
   MeanShiftSegmentationFilterType>       MeanShiftVectorizedSegmentationOGRType;


  // Connected components
  typedef otb::StreamingVectorizedSegmentationOGR
  <FloatVectorImageType,
   ConnectedComponentSegmentationFilterType>
  ConnectedComponentStreamingVectorizedSegmentationOGRType;

  typedef otb::FusionOGRTileFilter
  <FloatVectorImageType>                  FusionFilterType;


  // Watershed
  typedef otb::StreamingVectorizedSegmentationOGR
  <FloatImageType,
   WatershedSegmentationFilterType>      StreamingVectorizedWatershedFilterType;

  /** Standard macro */
  itkNewMacro(Self);
  itkTypeMacro(Segmentation, otb::Application);

private:
  void DoInit()
  {
    SetName("Segmentation");
    SetDescription("Performs Large scale segmentation");

    // Documentation
    SetDocName("Large Scale segmentation");
    SetDocLongDescription("This application is dedicated to image segmentation. "
                          "By handling streaming and OGR framework, it has been optimized for processing large scale data."
                          "Numerous segmentations algorithms are available."
                          "The application stream a large image, and for each stream:"
                          "apply a segmentation algorithm, "
                          "vectorize the results in polygons and keep-it "
                          "This application allows to correct some errors due to streaming by stitching polygons."
    );
    SetDocLimitations(" .");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("MeanShiftSegmentation");

    AddDocTag(Tags::Segmentation);

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "The input image.");

    AddParameter(ParameterType_Choice, "filter", "Segmentation algorithm");
    SetParameterDescription("filter", "Choose your segmentation method (threaded mean-shift by default).");

    AddChoice("filter.meanshift", "Threaded mean-shift");
    SetParameterDescription(
      "filter.meanshift",
      "Home-made threaded mean-shift filter.");
    // MeanShift Parameters
    AddParameter(ParameterType_Int, "filter.meanshift.spatialr", "Spatial radius");
    SetParameterDescription("filter.meanshift.spatialr", "Spatial radius defining neighborhood.");
    AddParameter(ParameterType_Float, "filter.meanshift.ranger", "Range radius");
    SetParameterDescription("filter.meanshift.ranger", "Range radius defining the interval in the color space.");
    AddParameter(ParameterType_Float, "filter.meanshift.thres", "convergence threshold");
    SetParameterDescription("filter.meanshift.thres", "convergence threshold. iterative scheme will stop if mean-shift "
                            "vector is below this threshold (1e-3 by default) or iteration number reached maximum iteration number.");
    AddParameter(ParameterType_Int, "filter.meanshift.maxiter", "maximum iteration number");
    SetParameterDescription("filter.meanshift.maxiter",
                            "iteration process is stopped if convergence hasn't been reached after this number of iteration (10 by default).");

    //AddParameter(ParameterType_Empty, "filter.meanshift.useoptim", "use optimization");
    //SetParameterDescription("filter.meanshift.useoptim", "Use mode optimization.");
    //MandatoryOff("filter.meanshift.useoptim");

    SetDefaultParameterInt("filter.meanshift.spatialr", 5);
    SetDefaultParameterFloat("filter.meanshift.ranger", 15.0);
    SetDefaultParameterFloat("filter.meanshift.thres", 0.1);
    SetMinimumParameterFloatValue("filter.meanshift.thres", 0.);
    SetDefaultParameterInt("filter.meanshift.maxiter", 100);
    SetMinimumParameterIntValue("filter.meanshift.maxiter", 1);

    AddChoice("filter.meanshiftedison", "EDISON mean-shift");
    SetParameterDescription("filter.meanshiftedison",
                            "EDISON based Mean-shift filter. (is going to be replaced by new framework and will be deprecated).");

    // EDISON Meanshift Parameters
    AddParameter(ParameterType_Int, "filter.meanshiftedison.spatialr", "Spatial radius");
    SetParameterDescription("filter.meanshiftedison.spatialr", "Spatial radius defining neighborhood.");
    AddParameter(ParameterType_Float, "filter.meanshiftedison.ranger", "Range radius");
    SetParameterDescription("filter.meanshiftedison.ranger", "Range radius defining the interval in the color space.");
    AddParameter(ParameterType_Int, "filter.meanshiftedison.minsize", "Min region size");
    SetParameterDescription("filter.meanshiftedison.minsize", "Minimun size of a region to be kept after clustering.");
    AddParameter(ParameterType_Float, "filter.meanshiftedison.scale", "Scale");
    SetParameterDescription("filter.meanshiftedison.scale", "Scale to stretch the image before processing.");
    SetDefaultParameterInt("filter.meanshiftedison.spatialr", 5);
    SetDefaultParameterFloat("filter.meanshiftedison.ranger", 15.0);
    SetDefaultParameterInt("filter.meanshiftedison.minsize", 100);
    SetDefaultParameterFloat("filter.meanshiftedison.scale", 100000.);

    //Connected component segmentation parameters
    AddChoice("filter.connectedcomponent", "Connected component Segmentation");
    SetParameterDescription("filter.connectedcomponent", "Connected component segmentation based on mathematical condition.");

    AddParameter(ParameterType_String, "filter.connectedcomponent.expr", "Condition");
    SetParameterDescription("filter.connectedcomponent.expr", "User defined criteria based on mathematical condition used for connected component segmentation.");
    MandatoryOff("filter.connectedcomponent.expr");

    // Watershed
    AddChoice("filter.watershed","Watershed segmentation");
    SetParameterDescription("filter.watershed","The traditionnal watershed algorithm. The height function is the gradient magnitude of the amplitude (square root of the sum of squared bands)");

    AddParameter(ParameterType_Float,"filter.watershed.threshold","Depth Threshold");
    SetParameterDescription("filter.watershed.threshold","Depth threshold Units in percentage of the maximum depth in the image.");
    SetDefaultParameterFloat("filter.watershed.threshold",0.01);
    SetMinimumParameterFloatValue("filter.watershed.threshold",0);
    SetMaximumParameterFloatValue("filter.watershed.threshold",1);

    AddParameter(ParameterType_Float,"filter.watershed.level","Flood Level");
    SetParameterDescription("filter.watershed.level","flood level for generating the merge tree from the initial segmentation (between 0 and 1)");
    SetDefaultParameterFloat("filter.watershed.level",0.1);
    SetMinimumParameterFloatValue("filter.watershed.level",0);
    SetMaximumParameterFloatValue("filter.watershed.level",1);

    AddParameter(ParameterType_Choice, "mode", "Segmentation mode (large scale or normal");
    SetParameterDescription("mode", "Choose your segmentation mode (large scalse with vector output or label image.");

    AddChoice("mode.largescale", "vector data output");
    SetParameterDescription("mode.largescale","Large scale segmentation paradigm.");

    AddChoice("mode.normal", "label image output");
    SetParameterDescription("mode.normal","Classic segmentation paradigm.");

    //Normal mode parameters
    AddParameter(ParameterType_OutputImage,  "mode.normal.lout",    "Label output");
    SetParameterDescription( "mode.normal.lout", "The label output image.");

    //Streaming vectorization parameters
    AddParameter(ParameterType_OutputFilename, "mode.largescale.outvd", "Output vector data");
    SetParameterDescription("mode.largescale.outvd", "The name of segmentation output. Vector Data (polygons).");
    MandatoryOff("mode.largescale.outvd");

    AddParameter(ParameterType_InputImage, "mode.largescale.inmask", "Mask Image");
    SetParameterDescription("mode.largescale.inmask", "Mask image. (Pixel with 0 will not be processed).");
    MandatoryOff("mode.largescale.inmask");

    AddParameter(ParameterType_Empty, "mode.largescale.neighbor", "8-neighbor vect. strategy");
    SetParameterDescription("mode.largescale.neighbor",
                            "Pixel neighborhood vectorization strategy. 4 or 8 neighborhood .(4 neighborhood by default.)");
    MandatoryOff("mode.largescale.neighbor");

    AddParameter(ParameterType_Empty,"mode.largescale.stitch","Stitch polygons");
    SetParameterDescription("mode.largescale.stitch", "Scan segments on each side of tiles and append polygons which have almost one picel in common.");
    MandatoryOff("mode.largescale.stitch");
    EnableParameter("mode.largescale.stitch");

    AddParameter(ParameterType_Int, "mode.largescale.minsize", "Minimum object size");
    SetParameterDescription("mode.largescale.minsize",
                            "Object with size under this threshold(area in pixels) will be replaced by the background value.");
    SetDefaultParameterInt("mode.largescale.minsize", 1);
    SetMinimumParameterIntValue("mode.largescale.minsize", 1);
    MandatoryOff("mode.largescale.minsize");


    AddParameter(ParameterType_Float, "mode.largescale.simplify", "Simplify geometry");
    SetParameterDescription("mode.largescale.simplify",
                            "Simplify polygons according to a given tolerance (in pixel?).");
    SetDefaultParameterFloat("mode.largescale.simplify",0.1);
    MandatoryOff("mode.largescale.simplify");
    DisableParameter("mode.largescale.simplify");

    AddParameter(ParameterType_String, "mode.largescale.layername", "Layer name");
    SetParameterDescription("mode.largescale.layername", "Layer Name.(by default : Layer )");
    SetParameterString("mode.largescale.layername", "layer");
    MandatoryOff("mode.largescale.layername");

    AddParameter(ParameterType_String, "mode.largescale.fieldname", "Field name");
    SetParameterDescription("mode.largescale.fieldname", "field Name.(by default : DN )");
    SetParameterString("mode.largescale.fieldname", "DN");
    MandatoryOff("mode.largescale.fieldname");

    AddParameter(ParameterType_Int, "mode.largescale.tilesize", "Tiles size");
    SetParameterDescription("mode.largescale.tilesize",
                            "user defined streaming tiles size  to compute segmentation.Automatic tile dimension is set "
                            "   if tile size set to (0 by default)");
    SetDefaultParameterInt("mode.largescale.tilesize", 0);
    MandatoryOff("mode.largescale.tilesize");
    EnableParameter("mode.largescale.tilesize");

    AddParameter(ParameterType_Int, "mode.largescale.startlabel", "start label");
    SetParameterDescription("mode.largescale.startlabel", "Start label (1 by default)");
    SetDefaultParameterInt("mode.largescale.startlabel", 1);
    MandatoryOff("mode.largescale.startlabel");

    // Doc example parameter settings

    SetDocExampleParameterValue("in", "QB_Toulouse_Ortho_PAN.tif");
    SetDocExampleParameterValue("outvd", "SegmentationVectorData.sqlite");
    SetDocExampleParameterValue("filter", "meanshift");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  template<class TInputImage, class TSegmentationFilter>
  FloatVectorImageType::SizeType
  GenericApplySegmentation(otb::StreamingVectorizedSegmentationOGR<TInputImage,
                                                                   TSegmentationFilter> * streamingVectorizedFilter, TInputImage * inputImage ,otb::ogr::DataSource::Pointer ogrDS, const unsigned int outputNb)
  {
    typedef  TSegmentationFilter             SegmentationFilterType;
    typedef  typename SegmentationFilterType::Pointer SegmentationFilterPointerType;
    typedef otb::StreamingVectorizedSegmentationOGR
      <FloatVectorImageType,
       SegmentationFilterType>          StreamingVectorizedSegmentationOGRType;

    // Retrieve tile size parameter
    const unsigned int tileSize = static_cast<unsigned int> (this->GetParameterInt("mode.largescale.tilesize"));
    // Retrieve the 8-connected option
    bool use8connected = IsParameterEnabled("mode.largescale.neighbor");
    // Retrieve min object size parameter
    const unsigned int minSize = static_cast<unsigned int> (this->GetParameterInt("mode.largescale.minsize"));

    // Switch on segmentation mode
    const std::string segModeType = (dynamic_cast <ChoiceParameter *> (this->GetParameterByKey("mode")))->GetChoiceKey(GetParameterInt("mode"));

    streamingVectorizedFilter->SetInput(inputImage);

    if (HasValue("mode.largescale.inmask"))
      {
      streamingVectorizedFilter->SetInputMask(this->GetParameterUInt32Image("mode.largescale.inmask"));
      otbAppLogINFO(<<"Use a mask as input." << std::endl);
      }
    streamingVectorizedFilter->SetOGRDataSource(ogrDS);

    if (tileSize != 0)
      {
      streamingVectorizedFilter->GetStreamer()->SetTileDimensionTiledStreaming(tileSize);
      }
    else
      {
      streamingVectorizedFilter->GetStreamer()->SetAutomaticTiledStreaming();
      }

    if (use8connected)
      {
      otbAppLogINFO(<<"Use 8 connected neighborhood."<<std::endl);
      }
    streamingVectorizedFilter->SetUse8Connected(use8connected);

    if (minSize > 1)
      {
      otbAppLogINFO(<<"Object with size under "<< minSize <<" will be suppressed."<<std::endl);
      streamingVectorizedFilter->SetFilterSmallObject(true);
      streamingVectorizedFilter->SetMinimumObjectSize(minSize);
      }

    const std::string layerName = this->GetParameterString("mode.largescale.layername");
    const std::string fieldName = this->GetParameterString("mode.largescale.fieldname");

    // Retrieve start label parameter
    const unsigned int startLabel = this->GetParameterInt("mode.largescale.startlabel");

    streamingVectorizedFilter->SetLayerName(layerName);
    streamingVectorizedFilter->SetFieldName(fieldName);
    streamingVectorizedFilter->SetStartLabel(startLabel);

    if(IsParameterEnabled("mode.largescale.simplify"))
      {
      streamingVectorizedFilter->SetSimplify(true);
      streamingVectorizedFilter->SetSimplificationTolerance(GetParameterFloat("mode.largescale.simplify"));
      otbAppLogINFO(<<"Simplify the geometry." << std::endl);
      }
    else
      {
      streamingVectorizedFilter->SetSimplify(false);
      }

    if (segModeType == "largescale")
      {
      otbAppLogINFO(<<"Large scale segmentation mode which output vector data" << std::endl);
      AddProcess(streamingVectorizedFilter->GetStreamer(), "Computing " + (dynamic_cast <ChoiceParameter *> (this->GetParameterByKey("filter")))->GetChoiceKey(GetParameterInt("filter")) + " segmentation");

      streamingVectorizedFilter->Initialize(); //must be called !
      streamingVectorizedFilter->Update(); //must be called !
      }
    else if (segModeType == "normal")
      {
      otbAppLogINFO(<<"Segmentation mode which output label image" << std::endl);

      streamingVectorizedFilter->GetSegmentationFilter()->SetInput(inputImage);
      SetParameterOutputImage<UInt32ImageType> ("mode.normal.lout", dynamic_cast<UInt32ImageType *> (streamingVectorizedFilter->GetSegmentationFilter()->GetOutputs().at(outputNb).GetPointer()));
      //TODO add progress reporting in normal mode
      // AddProcess(dynamic_cast <OutputImageParameter *> (GetParameterByKey("mode.normal.lout"))->GetWriter(),
      //            "Computing " + (dynamic_cast <ChoiceParameter *>
      //                            (this->GetParameterByKey("filter")))->GetChoiceKey(GetParameterInt("filter"))
      //            + " segmentation");
      streamingVectorizedFilter->GetSegmentationFilter()->Update();
      }
    return streamingVectorizedFilter->GetStreamSize();
  }

  void DoExecute()
  {
    // Switch on segmentation mode
    const std::string segModeType = (dynamic_cast <ChoiceParameter *> (this->GetParameterByKey("mode")))->GetChoiceKey(GetParameterInt("mode"));
    // Switch on segmentation filter case
    const std::string segType = (dynamic_cast <ChoiceParameter *> (this->GetParameterByKey("filter")))->GetChoiceKey(GetParameterInt("filter"));

    otb::ogr::DataSource::Pointer ogrDS;

    if(segModeType=="largescale")
      {
      // Retrieve output filename as well as layer names
      std::string dataSourceName = GetParameterString("mode.largescale.outvd");
      ogrDS = otb::ogr::DataSource::New(dataSourceName, otb::ogr::DataSource::Modes::write);
      }

    // The actual stream size used
    FloatVectorImageType::SizeType streamSize;

    if (segType == "connectedcomponent")
      {
      otbAppLogINFO(<<"Use connected component segmentation."<<std::endl);
      ConnectedComponentStreamingVectorizedSegmentationOGRType::Pointer
        ccVectorizationFilter = ConnectedComponentStreamingVectorizedSegmentationOGRType::New();

      if (HasValue("mode.largescale.inmask"))
        {
        ccVectorizationFilter->GetSegmentationFilter()->SetMaskImage(this->GetParameterUInt32Image("mode.largescale.inmask"));
        }

      ccVectorizationFilter->GetSegmentationFilter()->GetFunctor().SetExpression(
        GetParameterString(
          "filter.connectedcomponent.expr"));
      streamSize = GenericApplySegmentation<FloatVectorImageType,ConnectedComponentSegmentationFilterType>(ccVectorizationFilter,this->GetParameterFloatVectorImage("in"),ogrDS, 0);
      }
    else if (segType == "meanshiftedison")
      {
      otbAppLogINFO(<<"Use Edison Mean-shift segmentation."<<std::endl);

      //segmentation parameters
      const unsigned int
        spatialRadius = static_cast<unsigned int> (this->GetParameterInt("filter.meanshiftedison.spatialr"));
      const unsigned int
        rangeRadius = static_cast<unsigned int> (this->GetParameterInt("filter.meanshiftedison.ranger"));
      const unsigned int
        minimumObjectSize = static_cast<unsigned int> (this->GetParameterInt("filter.meanshiftedison.minsize"));
      const float scale = this->GetParameterFloat("filter.meanshiftedison.scale");

      EdisontreamingVectorizedSegmentationOGRType::Pointer
        edisonVectorizationFilter = EdisontreamingVectorizedSegmentationOGRType::New();

      edisonVectorizationFilter->GetSegmentationFilter()->SetSpatialRadius(spatialRadius);
      edisonVectorizationFilter->GetSegmentationFilter()->SetRangeRadius(rangeRadius);
      edisonVectorizationFilter->GetSegmentationFilter()->SetMinimumRegionSize(minimumObjectSize);
      edisonVectorizationFilter->GetSegmentationFilter()->SetScale(scale);

      streamSize = GenericApplySegmentation<FloatVectorImageType,EdisonSegmentationFilterType>(edisonVectorizationFilter, this->GetParameterFloatVectorImage("in"), ogrDS,2);
      }
    else if (segType == "meanshift")
      {
      otbAppLogINFO(<<"Use threaded Mean-shift segmentation."<<std::endl);

      MeanShiftVectorizedSegmentationOGRType::Pointer
        meanShiftVectorizationFilter = MeanShiftVectorizedSegmentationOGRType::New();

      //segmentation parameters
      const unsigned int
        spatialRadius = static_cast<unsigned int> (this->GetParameterInt("filter.meanshift.spatialr"));
      const unsigned int
        rangeRadius = static_cast<unsigned int> (this->GetParameterInt("filter.meanshift.ranger"));
      //const unsigned int
      //minimumObjectSize = static_cast<unsigned int> (this->GetParameterInt("minsize"));
      const float
        threshold = static_cast<float> (this->GetParameterInt("filter.meanshift.thres"));
      const unsigned int
        maxIterNumber = static_cast<unsigned int> (this->GetParameterInt("filter.meanshift.maxiter"));

      meanShiftVectorizationFilter->GetSegmentationFilter()->SetSpatialBandwidth(spatialRadius);
      meanShiftVectorizationFilter->GetSegmentationFilter()->SetRangeBandwidth(rangeRadius);
      meanShiftVectorizationFilter->GetSegmentationFilter()->SetMaxIterationNumber(maxIterNumber);
      meanShiftVectorizationFilter->GetSegmentationFilter()->SetThreshold(threshold);

      streamSize = this->GenericApplySegmentation<FloatVectorImageType,MeanShiftSegmentationFilterType>(meanShiftVectorizationFilter, this->GetParameterFloatVectorImage("in"), ogrDS, 0);
      }
    else if(segType == "watershed")
      {
      otbAppLogINFO(<<"Using watershed segmentation."<<std::endl);

      AmplitudeFilterType::Pointer amplitudeFilter = AmplitudeFilterType::New();

      amplitudeFilter->SetInput(this->GetParameterFloatVectorImage("in"));

      GradientMagnitudeFilterType::Pointer gradientMagnitudeFilter = GradientMagnitudeFilterType::New();
      gradientMagnitudeFilter->SetInput(amplitudeFilter->GetOutput());

      StreamingVectorizedWatershedFilterType::Pointer watershedVectorizedFilter = StreamingVectorizedWatershedFilterType::New();

      watershedVectorizedFilter->GetSegmentationFilter()->SetThreshold(GetParameterFloat("filter.watershed.threshold"));
      watershedVectorizedFilter->GetSegmentationFilter()->SetLevel(GetParameterFloat("filter.watershed.level"));

      streamSize = this->GenericApplySegmentation<FloatImageType,WatershedSegmentationFilterType>(watershedVectorizedFilter,gradientMagnitudeFilter->GetOutput(),ogrDS,0);
      }
    else
      {
      otbAppLogFATAL(<<"non defined filtering method "<<GetParameterInt("filter")<<std::endl);
      }

    if (segModeType == "largescale" )
      {
      ogrDS->SyncToDisk();

      // Stitching mode
      if(IsParameterEnabled("mode.largescale.stitch"))
        {
        otbAppLogINFO(<<"Segmentation done, stiching polygons ...");
        const std::string layerName = this->GetParameterString("mode.largescale.layername");

        FusionFilterType::Pointer fusionFilter = FusionFilterType::New();
        fusionFilter->SetInput(GetParameterFloatVectorImage("in"));
        fusionFilter->SetOGRDataSource(ogrDS);
        fusionFilter->SetStreamSize(streamSize);
        fusionFilter->SetLayerName(layerName);

        AddProcess(fusionFilter, "Stitching polygons");
        fusionFilter->GenerateData();
        }
      }
    else if (segModeType == "normal")
      {
      otbAppLogINFO(<<"implementation in progress..." << std::endl);
      }
    else
      {
      otbAppLogFATAL(<<"non defined segmentation mode method "<<GetParameterInt("mode")<<std::endl);
      }
  }
  EdisonSegmentationFilterType::Pointer m_Filter;
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::Segmentation)
