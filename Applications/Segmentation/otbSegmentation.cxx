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
#include "otbStreamingImageToOGRLayerSegmentationFilter.h"
#include "otbOGRDataSourceWrapper.h"

// Fusion filter
#include "otbOGRLayerStreamStitchingFilter.h"

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
  typedef otb::StreamingImageToOGRLayerSegmentationFilter
  <FloatVectorImageType,
   EdisonSegmentationFilterType>          EdisontreamingVectorizedSegmentationOGRType;

  // Home made mean-shift
  typedef otb::StreamingImageToOGRLayerSegmentationFilter
  <FloatVectorImageType,
   MeanShiftSegmentationFilterType>       MeanShiftVectorizedSegmentationOGRType;


  // Connected components
  typedef otb::StreamingImageToOGRLayerSegmentationFilter
  <FloatVectorImageType,
   ConnectedComponentSegmentationFilterType>
  ConnectedComponentStreamingVectorizedSegmentationOGRType;

  typedef otb::OGRLayerStreamStitchingFilter
  <FloatVectorImageType>                  FusionFilterType;


  // Watershed
  typedef otb::StreamingImageToOGRLayerSegmentationFilter
  <FloatImageType,
   WatershedSegmentationFilterType>      StreamingVectorizedWatershedFilterType;

  /** Standard macro */
  itkNewMacro(Self);
  itkTypeMacro(Segmentation, otb::Application);

private:
  void DoInit()
  {
    SetName("Segmentation");
    SetDescription("Performs segmentation of an image, and output either a raster or a vector file. In vector mode, large input datasets are supported.");

    // Documentation
    SetDocName("Segmentation");
    SetDocLongDescription("This application allows to perform various segmentation algorithms on an multispectral image."
                          "Available segmentation algorithms are two different versions of Mean-Shift segmentation algorithm (one being multi-threaded),"
                          " simple pixel based connected components according to a user-defined criterion, and watershed from the gradient of the intensity"
                          " (norm of spectral bands vector). The application has two different modes that affects the nature of its output.\n\nIn raster mode,"
                          " the output of the application is a classical image of unique labels identifying the segmented regions. The labeled output can be passed to the"
                          " ColorMapping application to render regions with contrasted colours. Please note that this mode loads the whole input image into memory, and as such"
                          " can not handle large images. \n\nTo segment large data, one can use the vector mode. In this case, the output of the application is a"
                          " vector file or database. The input image is split into tiles (whose size can be set using the tilesize parameter), and each tile is loaded, segmented"
                          " with the chosen algorithm, vectorized, and written into the output file or database. This piece-wise behaviour ensure that memory will never get overloaded,"
                          " and that images of any size can be processed. There are few more options in the vector mode. The simplify option allows to simplify the geometry"
                          " (i.e. remove nodes in polygons) according to a user-defined tolerance. The stitch option allows to application to try to stitch together polygons corresponding"
                          " to segmented region that may have been splitted by the tiling scheme. ");

    SetDocLimitations("In raster mode, the application can not handle large input images. Stitching step of vector mode might become slow with very large input images."
                     " MeanShift filter results depends on threads number.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("MeanShiftSegmentation");

    AddDocTag(Tags::Segmentation);

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "The input image to segment");

    AddParameter(ParameterType_Choice, "filter", "Segmentation algorithm");
    SetParameterDescription("filter", "Choice of segmentation algorithm (mean-shift by default)");

    AddChoice("filter.meanshift", "Mean-Shift");
    SetParameterDescription(
      "filter.meanshift","OTB implementation of the Mean-Shift algorithm (multi-threaded).");
    // MeanShift Parameters
    AddParameter(ParameterType_Int, "filter.meanshift.spatialr", "Spatial radius");
    SetParameterDescription("filter.meanshift.spatialr", "Spatial radius of the neighborhood.");
    AddParameter(ParameterType_Float, "filter.meanshift.ranger", "Range radius");
    SetParameterDescription("filter.meanshift.ranger", "Range radius defining the radius (expressed in radiometry unit) in the multi-spectral space.");
    AddParameter(ParameterType_Float, "filter.meanshift.thres", "Mode convergence threshold");
    SetParameterDescription("filter.meanshift.thres", "Algorithm iterative scheme will stop if mean-shift "
                            "vector is below this threshold or if iteration number reached maximum number of iterations.");
    AddParameter(ParameterType_Int, "filter.meanshift.maxiter", "Maximum number of iterations");
    SetParameterDescription("filter.meanshift.maxiter", "Algorithm iterative scheme will stop if convergence hasn't been reached after the maximum number of iterations.");
    AddParameter(ParameterType_Int, "filter.meanshift.minsize", "Mininum region size");
    SetParameterDescription("filter.meanshift.minsize", "Minimun size of a region (in pixel unit) in segmentation. Smaller clusters will be merged to the neighbouring cluster with the closest radiometry."
      " If set to 0 no pruning is done.");

    //AddParameter(ParameterType_Empty, "filter.meanshift.useoptim", "use optimization");
    //SetParameterDescription("filter.meanshift.useoptim", "Use mode optimization.");
    //MandatoryOff("filter.meanshift.useoptim");

    SetDefaultParameterInt("filter.meanshift.spatialr", 5);
    SetDefaultParameterFloat("filter.meanshift.ranger", 15.0);
    SetDefaultParameterFloat("filter.meanshift.thres", 0.1);
    SetMinimumParameterFloatValue("filter.meanshift.thres", 0.1);
    SetDefaultParameterInt("filter.meanshift.minsize", 100);
    SetMinimumParameterIntValue("filter.meanshift.minsize", 0);
    SetDefaultParameterInt("filter.meanshift.maxiter", 100);
    SetMinimumParameterIntValue("filter.meanshift.maxiter", 1);

    AddChoice("filter.edison", "Edison mean-shift");
    SetParameterDescription("filter.edison",
                            "Edison implementation of mean-shift algorithm, by its authors.");

    // EDISON Meanshift Parameters
    AddParameter(ParameterType_Int, "filter.edison.spatialr", "Spatial radius");
    SetParameterDescription("filter.edison.spatialr", "Spatial radius defining neighborhood.");
    AddParameter(ParameterType_Float, "filter.edison.ranger", "Range radius");
    SetParameterDescription("filter.edison.ranger", "Range radius defining the radius (expressed in radiometry unit) in the multi-spectral space.");
    AddParameter(ParameterType_Int, "filter.edison.minsize", "Mininum region size");
    SetParameterDescription("filter.edison.minsize", "Minimun size of a region in segmentation. Smaller clusters will be merged to the neighbouring cluster with the closest radiometry.");
    AddParameter(ParameterType_Float, "filter.edison.scale", "Scale factor");
    SetParameterDescription("filter.edison.scale", "Scaling of the image before processing. This is useful for images with narrow decimal ranges (like [0,1] for instance). ");
    SetDefaultParameterInt("filter.edison.spatialr", 5);
    SetDefaultParameterFloat("filter.edison.ranger", 15.0);
    SetDefaultParameterInt("filter.edison.minsize", 100);
    SetMinimumParameterIntValue("filter.edison.minsize", 0);
    SetDefaultParameterFloat("filter.edison.scale", 1.);

    //Connected component segmentation parameters
    AddChoice("filter.cc", "Connected components");
    SetParameterDescription("filter.cc", "Simple pixel-based connected-components algorithm with a user-defined connection condition.");

    AddParameter(ParameterType_String, "filter.cc.expr", "Condition");
    SetParameterDescription("filter.cc.expr", "User defined connection condition, written as a mathematical expression. Available variables are p(i)b(i), intensity_p(i) and distance (example of expression : distance < 10 )");

    // Watershed
    AddChoice("filter.watershed","Watershed");
    SetParameterDescription("filter.watershed","The traditional watershed algorithm. The height function is the gradient magnitude of the amplitude (square root of the sum of squared bands).");

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

    AddParameter(ParameterType_Choice, "mode", "Processing mode");
    SetParameterDescription("mode", "Choice of processing mode, either raster or large-scale.");

    AddChoice("mode.vector", "Tile-based large-scale segmentation with vector output");
    SetParameterDescription("mode.vector","In this mode, the application will output a vector file or database, and process the input image piecewise. This allows to perform segmentation of very large images.");

    AddChoice("mode.raster", "Standard segmentation with labeled raster output");
    SetParameterDescription("mode.raster","In this mode, the application will output a standard labeled raster. This mode can not handle large data.");

    //Raster mode parameters
    AddParameter(ParameterType_OutputImage,  "mode.raster.out",    "Output labeled image");
    SetParameterDescription( "mode.raster.out", "The output labeled image.");

    //Streaming vectorization parameters
    AddParameter(ParameterType_OutputFilename, "mode.vector.out", "Output vector file");
    SetParameterDescription("mode.vector.out", "The output vector file or database (name can be anything understood by OGR)");

    AddParameter(ParameterType_Choice,"mode.vector.outmode","Writing mode for the output vector file");
    SetParameterDescription("mode.vector.outmode","This allows to set the writing behaviour for the output vector file. Please note that the actual behaviour depends on the file format.");

    AddChoice("mode.vector.outmode.ovw","Overwrite output vector file if existing.");
    SetParameterDescription("mode.vector.outmode.ovw","If the output vector file already exists, it is completely destroyed (including all its layers) and recreated from scratch.");

    AddChoice("mode.vector.outmode.ulovw","Update output vector file, overwrite existing layer");
    SetParameterDescription("mode.vector.outmode.ulovw","The output vector file is opened in update mode if existing. If the output layer already exists, it si completely destroyed and recreated from scracth.");
    
    AddChoice("mode.vector.outmode.ulu","Update output vector file, update existing layer");
    SetParameterDescription("mode.vector.outmode.ulu","The output vector file is opened in update mode if existing. If the output layer already exists, the new geometries are appened to the layer.");

    AddChoice("mode.vector.outmode.ulco","Update output vector file, only allow to create new layers");
    SetParameterDescription("mode.vector.outmode.ulu","The output vector file is opened in update mode if existing. If the output layer already exists, the application stops, leaving it untouched.");
    
    AddParameter(ParameterType_InputImage, "mode.vector.inmask", "Mask Image");
    SetParameterDescription("mode.vector.inmask", "Only pixels whose mask value is strictly positive will be segmented.");
    MandatoryOff("mode.vector.inmask");

    AddParameter(ParameterType_Empty, "mode.vector.neighbor", "8-neighbor connectivity");
    SetParameterDescription("mode.vector.neighbor",
                            "Activate 8-Neighborhood connectivity (default is 4).");
    MandatoryOff("mode.vector.neighbor");
    DisableParameter("mode.vector.neighbor");


    AddParameter(ParameterType_Empty,"mode.vector.stitch","Stitch polygons");
    SetParameterDescription("mode.vector.stitch", "Scan polygons on each side of tiles and stitch polygons which connect by more than one pixel.");
    MandatoryOff("mode.vector.stitch");
    EnableParameter("mode.vector.stitch");

    AddParameter(ParameterType_Int, "mode.vector.minsize", "Minimum object size");
    SetParameterDescription("mode.vector.minsize",
                            "Objects whose size is below the minimum object size (area in pixels) will be ignored during vectorization.");
    SetDefaultParameterInt("mode.vector.minsize", 1);
    SetMinimumParameterIntValue("mode.vector.minsize", 1);
    MandatoryOff("mode.vector.minsize");
    DisableParameter("mode.vector.minsize");

    AddParameter(ParameterType_Float, "mode.vector.simplify", "Simplify polygons");
    SetParameterDescription("mode.vector.simplify",
                            "Simplify polygons according to a given tolerance (in pixel). This option allows to reduce the size of the output file or database.");
    SetDefaultParameterFloat("mode.vector.simplify",0.1);
    MandatoryOff("mode.vector.simplify");
    DisableParameter("mode.vector.simplify");

    AddParameter(ParameterType_String, "mode.vector.layername", "Layer name");
    SetParameterDescription("mode.vector.layername", "Name of the layer in the vector file or database (default is Layer).");
    SetParameterString("mode.vector.layername", "layer");

    AddParameter(ParameterType_String, "mode.vector.fieldname", "Geometry index field name");
    SetParameterDescription("mode.vector.fieldname", "Name of the field holding the geometry index in the output vector file or database.");
    SetParameterString("mode.vector.fieldname", "DN");

    AddParameter(ParameterType_Int, "mode.vector.tilesize", "Tiles size");
    SetParameterDescription("mode.vector.tilesize",
                            "User defined tiles size for tile-based segmentation. Optimal tile size is selected according to available RAM if null.");
    SetDefaultParameterInt("mode.vector.tilesize",1024);
    SetMinimumParameterIntValue("mode.vector.tilesize",0);

    AddParameter(ParameterType_Int, "mode.vector.startlabel", "Starting geometry index");
    SetParameterDescription("mode.vector.startlabel", "Starting value of the geometry index field");
    SetDefaultParameterInt("mode.vector.startlabel", 1);
    SetMinimumParameterIntValue("mode.vector.startlabel", 1);

    AddParameter(ParameterType_StringList,"mode.vector.ogroptions","OGR options for layer creation");
    SetParameterDescription("mode.vector.ogroptions","A list of layer creation options in the form KEY=VALUE that will be passed directly to OGR without any validity checking. Options may depend on the file format, and can be found in OGR documentation.");
    MandatoryOff("mode.vector.ogroptions");

    // Doc example parameter settings
    SetExampleComment("Example of use with vector mode and watershed segmentation",0);
    SetDocExampleParameterValue("in", "QB_Toulouse_Ortho_PAN.tif");
    SetDocExampleParameterValue("mode","vector");
    SetDocExampleParameterValue("mode.vector.out", "SegmentationVector.sqlite");
    SetDocExampleParameterValue("filter", "watershed");

    AddExample("Example of use with raster mode and mean-shift segmentation");
    SetDocExampleParameterValue("in", "QB_Toulouse_Ortho_PAN.tif",1);
    SetDocExampleParameterValue("mode","raster",1);
    SetDocExampleParameterValue("mode.raster.out", "SegmentationRaster.tif uint16",1);
    SetDocExampleParameterValue("filter", "meanshift",1);
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  template<class TInputImage, class TSegmentationFilter>
  FloatVectorImageType::SizeType
  GenericApplySegmentation(otb::StreamingImageToOGRLayerSegmentationFilter<TInputImage,
                           TSegmentationFilter> * streamingVectorizedFilter, TInputImage * inputImage, const otb::ogr::Layer& layer, const unsigned int outputNb)
  {
    typedef  TSegmentationFilter             SegmentationFilterType;
    typedef  typename SegmentationFilterType::Pointer SegmentationFilterPointerType;
    typedef otb::StreamingImageToOGRLayerSegmentationFilter
      <FloatVectorImageType,
       SegmentationFilterType>          StreamingVectorizedSegmentationOGRType;

    // Retrieve tile size parameter
    const unsigned int tileSize = static_cast<unsigned int> (this->GetParameterInt("mode.vector.tilesize"));
    // Retrieve the 8-connected option
    bool use8connected = IsParameterEnabled("mode.vector.neighbor");
    // Retrieve min object size parameter
    const unsigned int minSize = static_cast<unsigned int> (this->GetParameterInt("mode.vector.minsize"));

    // Switch on segmentation mode
    const std::string segModeType = GetParameterString("mode");

    streamingVectorizedFilter->SetInput(inputImage);

    if (segModeType == "vector" && HasValue("mode.vector.inmask"))
      {
      streamingVectorizedFilter->SetInputMask(this->GetParameterUInt32Image("mode.vector.inmask"));
      otbAppLogINFO(<<"Use a mask as input." << std::endl);
      }
    streamingVectorizedFilter->SetOGRLayer(layer);

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
    
    const std::string fieldName = this->GetParameterString("mode.vector.fieldname");

    // Retrieve start label parameter
    const unsigned int startLabel = this->GetParameterInt("mode.vector.startlabel");

    streamingVectorizedFilter->SetFieldName(fieldName);
    streamingVectorizedFilter->SetStartLabel(startLabel);

    if(IsParameterEnabled("mode.vector.simplify") && GetParameterString("mode") == "vector")
      {
      streamingVectorizedFilter->SetSimplify(true);
      streamingVectorizedFilter->SetSimplificationTolerance(GetParameterFloat("mode.vector.simplify"));
      otbAppLogINFO(<<"Simplify the geometry." << std::endl);
      }
    else
      {
      streamingVectorizedFilter->SetSimplify(false);
      }

    if (segModeType == "vector")
      {      
      otbAppLogINFO(<<"Large scale segmentation mode which output vector data" << std::endl);
      AddProcess(streamingVectorizedFilter->GetStreamer(), "Computing " + (dynamic_cast <ChoiceParameter *> (this->GetParameterByKey("filter")))->GetChoiceKey(GetParameterInt("filter")) + " segmentation");

      streamingVectorizedFilter->Initialize(); //must be called !
      streamingVectorizedFilter->Update(); //must be called !
      }
    else if (segModeType == "raster")
      {
      otbAppLogINFO(<<"Segmentation mode which output label image" << std::endl);

      streamingVectorizedFilter->GetSegmentationFilter()->SetInput(inputImage);
      SetParameterOutputImage<UInt32ImageType> ("mode.raster.out", dynamic_cast<UInt32ImageType *> (streamingVectorizedFilter->GetSegmentationFilter()->GetOutputs().at(outputNb).GetPointer()));
      //TODO add progress reporting in raster mode
      // AddProcess(dynamic_cast <OutputImageParameter *> (GetParameterByKey("mode.raster.out"))->GetWriter(),
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
    const std::string segModeType = GetParameterString("mode");
    // Switch on segmentation filter case
    const std::string segType = GetParameterString("filter");

    otb::ogr::DataSource::Pointer ogrDS;
    otb::ogr::Layer layer(NULL, false);

    OGRSpatialReference oSRS(GetParameterFloatVectorImage("in")->GetProjectionRef().c_str());

    if(segModeType=="vector")
      {
      // Retrieve output filename as well as layer names
      std::string dataSourceName = GetParameterString("mode.vector.out");

      // Create the DataSource in the appropriate mode
      if(GetParameterString("mode.vector.outmode") == "ovw")
        {
        // Create the datasource
        ogrDS = otb::ogr::DataSource::New(dataSourceName, otb::ogr::DataSource::Modes::Update_LayerOverwrite);

        // and create the layer since we are in overwrite mode, the
        // datasource is blank
        layer = ogrDS->CreateLayer(GetParameterString("mode.vector.layername"),
                                   &oSRS,wkbMultiPolygon,
                                   GetParameterStringList("mode.vector.ogroptions"));
        // And create the field
        OGRFieldDefn field(this->GetParameterString("mode.vector.fieldname").c_str(),OFTInteger);
        layer.CreateField(field,true);
        }
      else
        {
        otbAppLogFATAL(<<"outmode not handled yet");
        }
      }

    // The actual stream size used
    FloatVectorImageType::SizeType streamSize;

    if (segType == "cc")
      {
      otbAppLogINFO(<<"Use connected component segmentation."<<std::endl);
      ConnectedComponentStreamingVectorizedSegmentationOGRType::Pointer
        ccVectorizationFilter = ConnectedComponentStreamingVectorizedSegmentationOGRType::New();

      if (HasValue("mode.vector.inmask"))
        {
        ccVectorizationFilter->GetSegmentationFilter()->SetMaskImage(this->GetParameterUInt32Image("mode.vector.inmask"));
        }

      ccVectorizationFilter->GetSegmentationFilter()->GetFunctor().SetExpression(
        GetParameterString(
          "filter.cc.expr"));
      streamSize = GenericApplySegmentation<FloatVectorImageType,ConnectedComponentSegmentationFilterType>(ccVectorizationFilter,this->GetParameterFloatVectorImage("in"),layer, 0);
      }
    else if (segType == "edison")
      {
      otbAppLogINFO(<<"Use Edison Mean-shift segmentation."<<std::endl);

      //segmentation parameters
      const unsigned int
        spatialRadius = static_cast<unsigned int> (this->GetParameterInt("filter.edison.spatialr"));
      const unsigned int
        rangeRadius = static_cast<unsigned int> (this->GetParameterInt("filter.edison.ranger"));
      const unsigned int
        minimumObjectSize = static_cast<unsigned int> (this->GetParameterInt("filter.edison.minsize"));
      const float scale = this->GetParameterFloat("filter.edison.scale");

      EdisontreamingVectorizedSegmentationOGRType::Pointer
        edisonVectorizationFilter = EdisontreamingVectorizedSegmentationOGRType::New();

      edisonVectorizationFilter->GetSegmentationFilter()->SetSpatialRadius(spatialRadius);
      edisonVectorizationFilter->GetSegmentationFilter()->SetRangeRadius(rangeRadius);
      edisonVectorizationFilter->GetSegmentationFilter()->SetMinimumRegionSize(minimumObjectSize);
      edisonVectorizationFilter->GetSegmentationFilter()->SetScale(scale);

      streamSize = GenericApplySegmentation<FloatVectorImageType,EdisonSegmentationFilterType>(edisonVectorizationFilter, this->GetParameterFloatVectorImage("in"), layer,2);
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
       const unsigned int
            minimumObjectSize = static_cast<unsigned int> (this->GetParameterInt("filter.meanshift.minsize"));

      const float
        threshold = this->GetParameterFloat("filter.meanshift.thres");
      const unsigned int
        maxIterNumber = static_cast<unsigned int> (this->GetParameterInt("filter.meanshift.maxiter"));

      meanShiftVectorizationFilter->GetSegmentationFilter()->SetSpatialBandwidth(spatialRadius);
      meanShiftVectorizationFilter->GetSegmentationFilter()->SetRangeBandwidth(rangeRadius);
      meanShiftVectorizationFilter->GetSegmentationFilter()->SetMaxIterationNumber(maxIterNumber);
      meanShiftVectorizationFilter->GetSegmentationFilter()->SetThreshold(threshold);
      meanShiftVectorizationFilter->GetSegmentationFilter()->SetMinRegionSize(minimumObjectSize);

      streamSize = this->GenericApplySegmentation<FloatVectorImageType,MeanShiftSegmentationFilterType>(meanShiftVectorizationFilter, this->GetParameterFloatVectorImage("in"), layer, 0);
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

      streamSize = this->GenericApplySegmentation<FloatImageType,WatershedSegmentationFilterType>(watershedVectorizedFilter,gradientMagnitudeFilter->GetOutput(), layer,0);
      }
    else
      {
      otbAppLogFATAL(<<"non defined filtering method "<<GetParameterInt("filter")<<std::endl);
      }

    if (segModeType == "vector" )
      {
      ogrDS->SyncToDisk();

      // Stitching mode
      if(IsParameterEnabled("mode.vector.stitch"))
        {
        otbAppLogINFO(<<"Segmentation done, stiching polygons ...");

        FusionFilterType::Pointer fusionFilter = FusionFilterType::New();
        fusionFilter->SetInput(GetParameterFloatVectorImage("in"));
        fusionFilter->SetOGRLayer(layer);
        fusionFilter->SetStreamSize(streamSize);

        AddProcess(fusionFilter, "Stitching polygons");
        fusionFilter->GenerateData();
        }
      }
  }
  EdisonSegmentationFilterType::Pointer m_Filter;
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::Segmentation)
