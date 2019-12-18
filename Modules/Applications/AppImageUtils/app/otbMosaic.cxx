/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2016-2019 IRSTEA
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "itkFixedArray.h"
#include "itkObjectFactory.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"
#include "otbWrapperApplicationFactory.h"

// Vector data rasterization
#include "otbVectorDataToLabelImageFilter.h"

// VectorData
#include "otbVectorDataIntoImageProjectionFilter.h"

// Application engine
#include "otbStandardFilterWatcher.h"
#include "itkFixedArray.h"

// Mosaic filters
#include "otbStreamingStatisticsMosaicFilter.h"
#include "otbStreamingLargeFeatherMosaicFilter.h"
#include "otbStreamingSimpleMosaicFilter.h"
#include "otbStreamingFeatherMosaicFilter.h"

// Resample filter
#include "otbStreamingResampleImageFilter.h"

// danielson distance image
#include "itkDanielssonDistanceMapImageFilter.h"

// Interpolators
#include "itkLinearInterpolateImageFunction.h"
#include "otbBCOInterpolateImageFunction.h"
#include "itkNearestNeighborInterpolateImageFunction.h"

// Functors for lab <--> rgb color spaces
#include "otbFunctorImageFilter.h"
#include "otbMosaicFunctors.h"

// Masks
#include "itkMaskImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "otbVectorImageToAmplitudeImageFilter.h"

// Solver for mosaic harmonization
#include "otbQuadraticallyConstrainedSimpleSolver.h"

// system tools
#include <itksys/SystemTools.hxx>

using namespace std;

namespace otb
{

enum
{
  Interpolator_NNeighbor,
  Interpolator_BCO,
  Interpolator_Linear
};

enum
{
  Composition_Method_none,
  Composition_Method_large,
  Composition_Method_slim
};

enum
{
  Harmonisation_Method_none,
  Harmonisation_Method_bands,
  Harmonisation_Method_rgb
};

namespace Wrapper
{

class Mosaic : public Application
{
public:
  /** Standard class typedefs. */
  typedef Mosaic                        Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);
  itkTypeMacro(Mosaic, Application);

  /** Solver (Cresson & St Geours) */
  typedef DoubleImageType::InternalPixelType                             SolverPrecisionType;
  typedef otb::QuadraticallyConstrainedSimpleSolver<SolverPrecisionType> SolverType;

  /* Mosaic Filters */
  typedef otb::StreamingMosaicFilterBase<FloatVectorImageType, FloatVectorImageType>   MosaicFilterType;
  typedef otb::StreamingSimpleMosaicFilter<FloatVectorImageType, FloatVectorImageType> SimpleMosaicFilterType;
  typedef otb::StreamingLargeFeatherMosaicFilter<FloatVectorImageType, FloatVectorImageType, DoubleImageType>   LargeFeatherMosaicFilterType;
  typedef otb::StreamingFeatherMosaicFilter<FloatVectorImageType, FloatVectorImageType, DoubleImageType>        SlimFeatherMosaicFilterType;
  typedef otb::StreamingStatisticsMosaicFilter<FloatVectorImageType, FloatVectorImageType, SolverPrecisionType> StatisticsMosaicFilterType;

  /* Binary masks */
  typedef otb::Image<bool>         MaskImageType;
  typedef otb::Image<unsigned int> LabelImageType;
  typedef itk::MaskImageFilter<FloatVectorImageType, MaskImageType, FloatVectorImageType> MaskImageFilterType;
  typedef otb::ImageFileReader<MaskImageType> MaskReaderType;

  /* UInt8 binary masks */
  typedef otb::Image<unsigned char>                UInt8MaskImageType;
  typedef otb::ImageFileWriter<UInt8MaskImageType> UInt8MaskWriterType;
  typedef otb::ImageFileReader<UInt8MaskImageType> UInt8MaskReaderType;
  typedef itk::BinaryThresholdImageFilter<LabelImageType, UInt8MaskImageType>          LabelImageThresholdFilterType;
  typedef otb::VectorImageToAmplitudeImageFilter<FloatVectorImageType, FloatImageType> VectorImageToAmplitudeFilterType;
  typedef itk::BinaryThresholdImageFilter<FloatImageType, UInt8MaskImageType>          ImageThresholdFilterType;

  /* Distance map image writer typedef */
  typedef otb::ImageFileReader<DoubleImageType> DistanceMapImageReaderType;

  /* Vector data filters typedefs */
  typedef otb::VectorDataIntoImageProjectionFilter<VectorDataType, FloatVectorImageType> VectorDataReprojFilterType;
  typedef otb::VectorDataToLabelImageFilter<VectorDataType, LabelImageType>              RasterizerType;

  /* Decorrelated color space <--> RGB functors typedefs */
  typedef otb::Functor::LAB2RGB<FloatVectorImageType::PixelType, FloatVectorImageType::PixelType> LAB2RGBFunctor;
  typedef otb::Functor::RGB2LAB<FloatVectorImageType::PixelType, FloatVectorImageType::PixelType> RGB2LABFunctor;
  typedef otb::FunctorImageFilter<RGB2LABFunctor> RGB2LABFilterType;
  typedef otb::FunctorImageFilter<LAB2RGBFunctor> LAB2RGBFilterType;

  /** Interpolators typedefs */
  typedef itk::LinearInterpolateImageFunction<FloatVectorImageType, double>          LinearInterpolationType;
  typedef itk::NearestNeighborInterpolateImageFunction<FloatVectorImageType, double> NearestNeighborInterpolationType;
  typedef otb::BCOInterpolateImageFunction<FloatVectorImageType>           BCOInterpolationType;
  typedef itk::NearestNeighborInterpolateImageFunction<UInt8MaskImageType> UInt8NNInterpolatorType;

  /** UInt8 filters typedefs */
  typedef otb::StreamingResampleImageFilter<UInt8MaskImageType, UInt8MaskImageType> UInt8ResampleImageFilterType;

private:
  /*
   * Create a reader and update its registry
   */
  template <class TReaderType>
  typename TReaderType::Pointer CreateReader(const std::string& inputfile, std::vector<typename TReaderType::Pointer>& registry)
  {
    typename TReaderType::Pointer reader = TReaderType::New();
    reader->SetFileName(inputfile);
    reader->SetReleaseDataFlag(true);
    reader->UpdateOutputInformation();
    registry.push_back(reader);
    return reader;
  }

  /*
   * Create a mask filter and update its registry
   */
  MaskImageFilterType::Pointer CreateMaskFilter(FloatVectorImageType::Pointer input, MaskImageType::Pointer mask,
                                                std::vector<MaskImageFilterType::Pointer>& registry)
  {
    MaskImageFilterType::Pointer maskFilter = MaskImageFilterType::New();
    maskFilter->SetInput(input);
    maskFilter->SetMaskImage(mask);
    maskFilter->SetReleaseDataFlag(true);
    maskFilter->UpdateOutputInformation();
    registry.push_back(maskFilter);
    return maskFilter;
  }

  /*
   * Delete a temporary file
   */
  void deleteFile(string filename)
  {
    if (remove(filename.c_str()) != 0)
      otbAppLogWARNING("Error deleting file " << filename);
  }

  /*
   * This function generates a filename that looks like:
   * <m_TempDirectory>/<tag>_<id>.tif
   */
  string GenerateFileName(string tag, int id)
  {
    string outputFile = m_TempFilesPrefix + "_" + tag + "_" + std::to_string(id) + ".tif";
    return outputFile;
  }

  void DoInit() override
  {
    SetName("Mosaic");
    SetDescription("Perform a mosaic of input images");

    // Documentation
    SetDocLongDescription("This application performs a mosaic of the input images");
    SetDocLimitations(
        "1. When \"comp\" parameter is different than \"none\", the sampling ratio for "
        "distance map computation can be adjusted to make input images fit into memory (distance map "
        "computation is not streamable)."
        "2. When \"harmo\" method is not \"none\", an algorithm performs the color harmonization of "
        "the input images using quadratic programming (QP). The objective function of the QP is a set "
        "of matrices, each one with size NxN (N being the number of input images). Hence, a large number "
        "of input images imply that those matrices fit into memory, and this is not related to the "
        "streaming capability of the application (ram parameter) but intrinsic to the harmonization process.");
    SetDocAuthors("Remi Cresson");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Manip);
    AddDocTag(Tags::Raster);

    // Input image
    AddParameter(ParameterType_InputImageList, "il", "Input Images");
    SetParameterDescription("il", "Input images to mosaic");

    // Input vector data (cutline)
    AddParameter(ParameterType_InputVectorDataList, "vdcut", "Input VectorData for composition");
    SetParameterDescription("vdcut",
                            "VectorData files to be used for cut input images. "
                            "Must be provided in the same order as the input images.");
    MandatoryOff("vdcut");

    // Input vector data (statistics masks)
    AddParameter(ParameterType_InputVectorDataList, "vdstats", "Input VectorData for statistics");
    SetParameterDescription("vdstats",
                            "VectorData files to be used for statistics computation (harmonization). "
                            "Must be provided in the same order as the input images.");
    MandatoryOff("vdstats");

    // comp (compositing)
    AddParameter(ParameterType_Group, "comp", "Mosaic compositing mode");
    SetParameterDescription("comp", "This group of parameters sets the mosaic compositing");

    // comp.feathering
    AddParameter(ParameterType_Choice, "comp.feather", "Feathering method");
    SetParameterDescription("comp.feather", "Set the feathering method for composition");

    // comp.feather.none
    AddChoice("comp.feather.none", "Simple compositing method");
    SetParameterDescription("comp.feather.none", "No feathering method is used. Copies the last image over earlier ones in areas of overlap");

    // comp.feather.large
    AddChoice("comp.feather.large", "The large blending composition mode");
    SetParameterDescription("comp.feather.large",
                            "Blends all images on the maximum overlapping areas (this produces seamless mosaics, but can cause blur effect where images are "
                            "not perfectly aligned)");

    // comp.feather.slim
    AddChoice("comp.feather.slim", "The slim blending composition mode");
    SetParameterDescription("comp.feather.slim",
                            "Blends the last image over earlier ones in areas of overlap, on a given transition distance (seam can be visible from a certain "
                            "zoom level, but it does not cause blur effect when images are not perfectly aligned)");

    // comp.feather.slim.exponent (i.e. blending smoothness)
    AddParameter(ParameterType_Float, "comp.feather.slim.exponent", "Transition smoothness (1.0 = linear transition)");
    SetDefaultParameterFloat("comp.feather.slim.exponent", 1.0);
    SetMinimumParameterFloatValue("comp.feather.slim.exponent", 0);
    MandatoryOff("comp.feather.slim.exponent");

    // comp.feather.slim.length (i.e. blending length)
    AddParameter(ParameterType_Float, "comp.feather.slim.length", "Transition length (In cartographic units)");
    MandatoryOn("comp.feather.slim.length");
    SetMinimumParameterFloatValue("comp.feather.slim.length", 0);
    MandatoryOff("comp.feather.slim.length");

    // harmo (harmonization)
    AddParameter(ParameterType_Group, "harmo", "Spectral bands harmonization mode");
    SetParameterDescription("harmo", "This group of parameters enables to set the mosaic harmonization method");

    // harmo.method
    AddParameter(ParameterType_Choice, "harmo.method", "harmonization method");
    SetParameterDescription("harmo.method",
                            "Set the harmonization method. More information about the \"band\" and \"rgb\" methods can be found in the paper \"Natural Color "
                            "Satellite Image Mosaicking Using Quadratic Programming in Decorrelated Color Space\" Cresson and Saint-Geours, July 2015, IEEE "
                            "JSTARS Volume 8 Issue 8 (https://doi.org/10.1109/JSTARS.2015.2449233)");
    AddChoice("harmo.method.none", "None");
    SetParameterDescription("harmo.method.none", "No automatic harmonization is done");
    AddChoice("harmo.method.band", "Spectral bands");
    SetParameterDescription("harmo.method.band",
                            "Consists in minimizing a cost function based on images statistics in overlapping areas, for each band independently");
    AddChoice("harmo.method.rgb", "True colors");
    SetParameterDescription("harmo.method.rgb",
                            "Consists in minimizing a cost function based on images statistics in overlapping areas, in a decorreleted color space suitable "
                            "for true color processing (works only on true color images, i.e. RGB). Only the first 3 bands are processed by this method. ");

    // harmo.cost (harmonization cost function)
    AddParameter(ParameterType_Choice, "harmo.cost", "harmonization cost function");
    SetParameterDescription("harmo.cost", "Set the harmonization cost function");
    AddChoice("harmo.cost.rmse", "Root mean squared error based cost function");
    AddChoice("harmo.cost.musig", "Mean and Standard deviation based cost function");
    AddChoice("harmo.cost.mu", "Mean based cost function");

    // Output image
    AddParameter(ParameterType_OutputImage, "out", "Output image");
    SetParameterDescription("out", "Output image, resulting from the mosaicing process.");

    // Interpolators
    AddParameter(ParameterType_Choice, "interpolator", "Interpolation");
    SetParameterDescription("interpolator", "This group of parameters allows defining how the input image will be interpolated during resampling.");
    MandatoryOff("interpolator");

    // NN
    AddChoice("interpolator.nn", "Nearest Neighbor interpolation");
    SetParameterDescription("interpolator.nn", "Nearest neighbor interpolation leads to poor image quality, but it is fast.");

    // BCO
    AddChoice("interpolator.bco", "Bicubic interpolation");
    SetParameterDescription("interpolator.bco", "Bicubic interpolation leads to great image quality, but it is slow.");
    AddParameter(ParameterType_Radius, "interpolator.bco.radius", "Radius for bicubic interpolation");
    SetParameterDescription("interpolator.bco.radius",
                            "Size of the bicubic interpolation filter. If the target pixel size "
                            "is higher than the input pixel size, increasing this parameter will reduce aliasing artefacts.");

    // Linear
    AddChoice("interpolator.linear", "Linear interpolation");
    SetParameterDescription("interpolator.linear", "Linear interpolation leads to average image quality but is quite fast");
    SetDefaultParameterInt("interpolator.bco.radius", 2);

    // Spacing of the output image
    AddParameter(ParameterType_Group, "output", "Output Image Grid");
    SetParameterDescription("output", "Output mosaic pixel grid parameters");

    // Spacing X
    AddParameter(ParameterType_Float, "output.spacingx", "Pixel Size (X)");
    SetMinimumParameterFloatValue("output.spacingx", 0);
    SetParameterDescription("output.spacingx", "Physical size (X) of output image pixels, in cartographic units.");
    MandatoryOff("output.spacingx");

    // Spacing Y
    AddParameter(ParameterType_Float, "output.spacingy", "Pixel Size (Y)");
    SetMinimumParameterFloatValue("output.spacingy", 0);
    SetParameterDescription("output.spacingy", "Physical size (Y) of output image pixels, in cartographic units..");
    MandatoryOff("output.spacingy");

    // Directory for temporary files
    AddParameter(ParameterType_Directory, "tmpdir", "Directory for temporary files");
    SetParameterDescription("tmpdir", "Directory for temporary files. If not set, the output image parent directory is used.");
    MandatoryOff("tmpdir");

    AddParameter(ParameterType_Group, "distancemap", "Distance maps computation");
    AddParameter(ParameterType_Float, "distancemap.sr", "Distance maps sampling ratio");
    SetParameterDescription("distancemap.sr",
                            "Distance maps are computed when the compositing method uses feathering. "
                            "The distance maps sampling ratio is the ratio between the distance map physical spacing and the "
                            "physical spacing of the original image. It is used to change the distance maps physical spacing: "
                            "since the distance map computation is not a streamable pipeline, it can be useful to compute slightly "
                            "smaller distance maps. distancemap.sr can be hence increased if input images are too big to fit the RAM, "
                            "or in order to speed up the process");
    SetDefaultParameterFloat("distancemap.sr", 10);

    // no-data value
    AddParameter(ParameterType_Float, "nodata", "no-data value");
    SetParameterDescription("nodata",
                            "The no-data value is ignored during statistics computations (for color harmonisation) and "
                            "for the mosaic compositing.");
    SetDefaultParameterFloat("nodata", 0.0);
    MandatoryOff("nodata");

    AddRAMParameter();

    // Doc example
    SetDocExampleParameterValue("il", "SP67_FR_subset_1.tif SP67_FR_subset_2.tif");
    SetDocExampleParameterValue("out", "mosaicImage.tif");
  }

  void DoUpdateParameters() override
  {
    // TODO: update parameters
  }

  /*
   * Write a binary mask to disk from a vector data
   */
  void RasterizeBinaryMask(VectorDataType* vd, FloatVectorImageType* reference, string outputFileName, double spacingRatio, bool invert = false)
  {

    // Reproject VectorData
    VectorDataReprojFilterType::Pointer vdReproj = VectorDataReprojFilterType::New();
    vdReproj->SetInputVectorData(vd);
    vdReproj->SetInputImage(reference);

    // Rasterize vector data
    GDALAllRegister();
    RasterizerType::Pointer rasterizer = RasterizerType::New();
    if (vd->GetProjectionRef().empty())
    {
      otbAppLogWARNING("No CRS in the vector data. Using image CRS instead!") rasterizer->AddVectorData(vd);
    }
    else
    {
      vdReproj->Update();
      rasterizer->AddVectorData(vdReproj->GetOutput());
    }

    rasterizer->SetOutputOrigin(reference->GetOrigin());
    LabelImageType::SizeType outputSize = reference->GetLargestPossibleRegion().GetSize();
    outputSize[0] /= spacingRatio;
    outputSize[1] /= spacingRatio;
    rasterizer->SetOutputSize(outputSize);
    LabelImageType::SpacingType outputSpacing = reference->GetSignedSpacing();
    outputSpacing[0] *= spacingRatio;
    outputSpacing[1] *= spacingRatio;
    rasterizer->SetOutputSpacing(outputSpacing);
    rasterizer->SetOutputProjectionRef(reference->GetProjectionRef());
    rasterizer->SetBurnAttribute("________");
    rasterizer->SetGlobalWarningDisplay(false);

    // Threshold background
    LabelImageThresholdFilterType::Pointer labelThreshold = LabelImageThresholdFilterType::New();
    labelThreshold->SetInput(rasterizer->GetOutput());
    if (invert)
    {
      labelThreshold->SetInsideValue(itk::NumericTraits<UInt8MaskImageType::InternalPixelType>::max());
      labelThreshold->SetOutsideValue(itk::NumericTraits<UInt8MaskImageType::InternalPixelType>::Zero);
    }
    else
    {
      labelThreshold->SetInsideValue(itk::NumericTraits<UInt8MaskImageType::InternalPixelType>::Zero);
      labelThreshold->SetOutsideValue(itk::NumericTraits<UInt8MaskImageType::InternalPixelType>::max());
    }
    labelThreshold->SetLowerThreshold(1);
    labelThreshold->SetUpperThreshold(itk::NumericTraits<LabelImageType::InternalPixelType>::max());

    // Write file
    UInt8MaskWriterType::Pointer writer = UInt8MaskWriterType::New();
    writer->SetInput(labelThreshold->GetOutput());
    writer->SetFileName(outputFileName);
    AddProcess(writer, "Writing binary mask (from vector data) " + outputFileName);
    writer->Update();
  }

  /*
   * Write on disk the distance image
   * Inputs:
   * -input filename (binary mask)
   * -output filename (distance image)
   */
  void WriteDistanceImage(string inputBinaryMaskFileName, string outputDistanceImageFileName)
  {
    /** Typedefs */
    typedef itk::DanielssonDistanceMapImageFilter<UInt8MaskImageType, DoubleImageType, DoubleImageType> ApproximateSignedDistanceMapImageFilterType;
    typedef otb::ImageFileWriter<DoubleImageType> WriterType;
    typedef otb::StreamingResampleImageFilter<UInt8MaskImageType, UInt8MaskImageType> PadFilterType;

    // Read the binary mask image
    UInt8MaskReaderType::Pointer reader = UInt8MaskReaderType::New();
    reader->SetFileName(inputBinaryMaskFileName);

    // Pad the image
    const unsigned int paddingRadius = 2;
    reader->UpdateOutputInformation();
    UInt8MaskImageType::SizeType size = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
    size[0] += 2 * paddingRadius;
    size[1] += 2 * paddingRadius;
    UInt8MaskImageType::SpacingType spacing = reader->GetOutput()->GetSignedSpacing();
    UInt8MaskImageType::PointType   origin  = reader->GetOutput()->GetOrigin();
    origin[0] -= static_cast<UInt8MaskImageType::PointType::ValueType>(paddingRadius) * spacing[0];
    origin[1] -= static_cast<UInt8MaskImageType::PointType::ValueType>(paddingRadius) * spacing[1];

    PadFilterType::Pointer padFilter = PadFilterType::New();
    padFilter->SetInput(reader->GetOutput());
    padFilter->SetOutputOrigin(origin);
    padFilter->SetOutputSpacing(spacing);
    padFilter->SetOutputSize(size);
    padFilter->SetEdgePaddingValue(itk::NumericTraits<UInt8MaskImageType::InternalPixelType>::max());

    // Compute the approximate signed distance image
    ApproximateSignedDistanceMapImageFilterType::Pointer approximateSignedDistanceMapImageFilter = ApproximateSignedDistanceMapImageFilterType::New();
    approximateSignedDistanceMapImageFilter->SetInput(padFilter->GetOutput());
    approximateSignedDistanceMapImageFilter->SetInputIsBinary(true);
    approximateSignedDistanceMapImageFilter->SetUseImageSpacing(true);
    approximateSignedDistanceMapImageFilter->Update();

    // Write the distance image
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(outputDistanceImageFileName);
    writer->SetInput(approximateSignedDistanceMapImageFilter->GetOutput());
    AddProcess(writer, "Writing distance map image " + outputDistanceImageFileName);
    writer->Update();
  }

  /*
   * Write a binary mask from an input image
   */
  void WriteBinaryMask(FloatVectorImageType* referenceImage, string outputFileName, double spacingRatio = 1.0)
  {
    // Vector image to amplitude image
    VectorImageToAmplitudeFilterType::Pointer ampFilter = VectorImageToAmplitudeFilterType::New();

    ampFilter->SetInput(referenceImage);

    // Threshold image
    ImageThresholdFilterType::Pointer thresholdFilter = ImageThresholdFilterType::New();
    thresholdFilter->SetInput(ampFilter->GetOutput());
    thresholdFilter->SetOutsideValue(itk::NumericTraits<UInt8MaskImageType::InternalPixelType>::Zero);
    thresholdFilter->SetInsideValue(itk::NumericTraits<UInt8MaskImageType::InternalPixelType>::max());
    thresholdFilter->SetLowerThreshold(GetParameterFloat("nodata"));
    thresholdFilter->SetUpperThreshold(GetParameterFloat("nodata"));
    thresholdFilter->UpdateOutputInformation();

    // Resample image
    UInt8ResampleImageFilterType::Pointer resampler = UInt8ResampleImageFilterType::New();
    resampler->SetInput(thresholdFilter->GetOutput());
    LabelImageType::SizeType outputSize = thresholdFilter->GetOutput()->GetLargestPossibleRegion().GetSize();
    outputSize[0]                       = outputSize[0] / spacingRatio + 1;
    outputSize[1]                       = outputSize[1] / spacingRatio + 1;
    resampler->SetOutputSize(outputSize);
    LabelImageType::SpacingType outputSpacing = thresholdFilter->GetOutput()->GetSignedSpacing();
    outputSpacing[0] *= spacingRatio;
    outputSpacing[1] *= spacingRatio;
    resampler->SetOutputSpacing(outputSpacing);
    resampler->SetOutputOrigin(thresholdFilter->GetOutput()->GetOrigin());

    // Write image
    UInt8MaskWriterType::Pointer writer = UInt8MaskWriterType::New();
    writer->SetInput(resampler->GetOutput());
    writer->SetFileName(outputFileName);
    AddProcess(writer, "Writing binary mask (from image boundaries) " + outputFileName);
    writer->Update();
  }

  /*
   * Write the distance image of the input image #id
   */
  void WriteDistanceImageFromCutline(FloatVectorImageType* image, VectorDataType* vd, string outputFileName)
  {

    // Generate a temporary filenames for the resampled mask
    string temporaryFileName = GenerateFileName("tmp_binary_rasterized_mask", 0);

    // Write a binary mask
    RasterizeBinaryMask(vd, image, temporaryFileName, GetParameterFloat("distancemap.sr"));

    // Create distance image
    WriteDistanceImage(temporaryFileName, outputFileName);

    // Delete the temporary file
    deleteFile(temporaryFileName);
  }

  /*
   * Write the distance image of the input image #id
   */
  string WriteDistanceImageFromBoundaries(FloatVectorImageType* image, string outputFileName)
  {

    // Generate a temporary filenames for the resampled mask
    string temporaryFileName = GenerateFileName("tmp_binary_mask", 0);

    // Write a temporary binary mask
    WriteBinaryMask(image, temporaryFileName, GetParameterFloat("distancemap.sr"));

    // Create distance image
    WriteDistanceImage(temporaryFileName, outputFileName);

    // Delete the temporary binary mask file
    deleteFile(temporaryFileName);

    // Prepare image for new data
    image->PrepareForNewData();

    return outputFileName;
  }

  /*
   * Set the correction model to the mosaic filter
   */
  template <class TMosaicFilterType>
  void SetCorrectionModel(typename TMosaicFilterType::Pointer& filter)
  {
    // If no harmonization is required, we return
    if (this->GetParameterInt("harmo.method") == Harmonisation_Method_none)
    {
      return;
    }

    // Set the solver objective function (aka "correction cost function")
    SolverType::Pointer solver = SolverType::New();
    switch (GetParameterInt("harmo.cost"))
    {
    case SolverType::Cost_Function_mu:
      solver->SetMeanBased();
      otbAppLogINFO("Correction cost function: mean based cost function");
      break;
    case SolverType::Cost_Function_musig:
      solver->SetMeanAndStandardDeviationBased();
      otbAppLogINFO("Correction cost function: mean-stdev based cost function");
      break;
    case SolverType::Cost_Function_rmse:
      solver->SetRMSEBased();
      otbAppLogINFO("Correction cost function: RMSE based cost function");
      break;
    default:
      otbWarningMacro("Unknown correction cost function. Setting to RMSE");
      solver->SetRMSEBased();
      break;
    }

    // Colorimetric correction (Cresson & St Geours)
    filter->UpdateOutputInformation();
    const unsigned int              nImages = this->GetParameterImageList("il")->Size();
    const unsigned int              nBands  = filter->GetOutput()->GetNumberOfComponentsPerPixel();
    vnl_matrix<SolverPrecisionType> scales(nImages, nBands, 1);
    solver->SetAreaInOverlaps(m_StatsFilter->GetAreas());
    for (unsigned int band = 0; band < nBands; band++)
    {
      otbAppLogINFO("computing correction model for band " << band);
      solver->SetMeanInOverlaps(m_StatsFilter->GetMeans()[band]);
      solver->SetStandardDeviationInOverlaps(m_StatsFilter->GetStds()[band]);
      solver->SetMeanOfProductsInOverlaps(m_StatsFilter->GetMeansOfProducts()[band]);
      solver->Solve();

      // Keep scales
      scales.set_column(band, solver->GetOutputCorrectionModel());

      otbAppLogINFO("\n\t[ Band " << band << " ]"
                                  << "\n\tGains  : " << solver->GetOutputCorrectionModel());
    }

    // Set filter correction model
    filter->SetScaleMatrix(scales);
    filter->SetShiftScaleInputImages(true);
  }

  /*
   * Set interpolator to the mosaic filter
   */
  template <class TMosaicFilterType>
  void SetInterpolator(typename TMosaicFilterType::Pointer& filter)
  {
    // Get Interpolator
    switch (GetParameterInt("interpolator"))
    {
    case Interpolator_Linear:
    {
      typedef itk::LinearInterpolateImageFunction<FloatVectorImageType, double> LinearInterpolationType;
      LinearInterpolationType::Pointer interpolator = LinearInterpolationType::New();
      filter->SetInterpolator(interpolator);
    }
    break;
    case Interpolator_NNeighbor:
    {
      typedef itk::NearestNeighborInterpolateImageFunction<FloatVectorImageType, double> NearestNeighborInterpolationType;
      NearestNeighborInterpolationType::Pointer interpolator = NearestNeighborInterpolationType::New();
      filter->SetInterpolator(interpolator);
    }
    break;
    case Interpolator_BCO:
    {
      typedef otb::BCOInterpolateImageFunction<FloatVectorImageType> BCOInterpolationType;
      BCOInterpolationType::Pointer                                  interpolator = BCOInterpolationType::New();
      interpolator->SetRadius(GetParameterInt("interpolator.bco.radius"));
      filter->SetInterpolator(interpolator);
    }
    break;
    }
  }

  /*
   * Change a mosaic filter spacing
   */
  template <class TMosaicFilterType>
  void SetSpacing(typename TMosaicFilterType::Pointer& filter)
  {
    if (!this->HasValue("output.spacingx") && !this->HasValue("output.spacingy"))
      return;

    // Retrieve original spacing and size
    filter->SetAutomaticOutputParametersComputation(true);
    filter->UpdateOutputInformation();
    FloatVectorImageType::SpacingType spacing = filter->GetOutput()->GetSignedSpacing();
    FloatVectorImageType::SizeType    size    = filter->GetOutputSize();

    // Compute new spacing and size
    if (this->HasValue("output.spacingx"))
    {
      size[0] *= std::abs(spacing[0] / GetParameterFloat("output.spacingx"));
      spacing[0] = GetParameterFloat("output.spacingx");
    }
    if (this->HasValue("output.spacingy"))
    {
      size[1] *= std::abs(spacing[1] / GetParameterFloat("output.spacingy"));
      spacing[1] = -1.0 * GetParameterFloat("output.spacingy");
    }

    // Change spacing and size
    filter->SetOutputSpacing(spacing);
    filter->SetOutputSize(size);
    filter->SetAutomaticOutputParametersComputation(false);
    filter->UpdateOutputInformation();
  }

  /*
   * Change a mosaic filter no-data value
   */
  template <class TMosaicFilterType>
  void SetNoDataValue(typename TMosaicFilterType::Pointer& filter)
  {
    if (this->HasValue("nodata"))
    {
      FloatVectorImageType::PixelType nodatapix;
      filter->UpdateOutputInformation();
      nodatapix.SetSize(filter->GetOutput()->GetNumberOfComponentsPerPixel());
      nodatapix.Fill(GetParameterFloat("nodata"));
      filter->SetNoDataInputPixel(nodatapix);
      filter->SetNoDataOutputPixel(nodatapix);
    }
  }

  /*
   * Prepare the mosaic filter:
   * -set interpolator
   * -set output spacing
   * -set correction model
   * -set no data
   */
  template <class TMosaicFilterType>
  void PrepareMosaicFilter(typename TMosaicFilterType::Pointer& filter)
  {
    SetInterpolator<TMosaicFilterType>(filter);
    SetCorrectionModel<TMosaicFilterType>(filter);
    SetSpacing<TMosaicFilterType>(filter);
    SetNoDataValue<TMosaicFilterType>(filter);
    SetCorrectionModel<TMosaicFilterType>(filter);
  }

  /*
   * Set a mosaic filter its distance offset, given the sampling ratio (sr)
   */
  template <class TMosaicFilterType>
  void ComputeDistanceOffset(typename TMosaicFilterType::Pointer& filter)
  {
    filter->UpdateOutputInformation();
    typename TMosaicFilterType::OutputImageSpacingType spacing       = filter->GetOutputSpacing();
    const float                                        multiplicator = GetParameterFloat("distancemap.sr");
    const float                                        abs_spc_x     = multiplicator * vnl_math_abs(spacing[0]);
    const float                                        abs_spc_y     = multiplicator * vnl_math_abs(spacing[1]);
    const float                                        maxSpacing    = vnl_math_max(abs_spc_x, abs_spc_y);
    filter->SetDistanceOffset(maxSpacing);
  }

  /*
   * Check that the number of inputs (images, vector data) are consistent
   */
  void CheckNbOfInputs()
  {

    const unsigned int nImages  = GetParameterImageList("il")->Size();
    const unsigned int nMasks   = GetParameterVectorDataList("vdstats")->Size();
    const unsigned int nCutline = GetParameterVectorDataList("vdcut")->Size();

    if (GetParameterByKey("vdcut")->HasValue() && nCutline != nImages)
    {
      otbAppLogFATAL("Number of input cutlines (" << nCutline << ") should be equal to number of images (" << nImages << ")");
    }
    if (GetParameterByKey("vdstats")->HasValue() && nMasks != nImages)
    {
      otbAppLogFATAL("Number of input masks (" << nMasks << ") should be equal to number of images (" << nImages << ")");
    }
  }

  /*
   * Resolve the temporary directory.
   * If the "tmpdir" parameter is not set, the output image parent directory is used.
   */
  void ResolveTemporaryDirectory()
  {
    // Get output filename (without extension)
    const std::string outfname  = GetParameterString("out");
    const std::string outbfname = itksys::SystemTools::GetFilenameWithoutExtension(outfname.c_str());

    // Get specified temporary directory
    std::string tmpdir = GetParameterAsString("tmpdir");
    if (tmpdir.empty())
    {
      // If tmpdir is empty, we use the same output directory as for the output image
      tmpdir = itksys::SystemTools::GetFilenamePath(outfname.c_str());
    }

    // Check that it ends with a POSIX separator
    if (tmpdir[tmpdir.size() - 1] != '/')
    {
      tmpdir.append("/");
    }

    m_TempFilesPrefix = tmpdir + outbfname;
    otbAppLogINFO(<< "Temporary files prefix is: " << m_TempFilesPrefix);
  }

  /*
   * Compute images statistics
   */
  void ComputeImagesStatistics()
  {

    // Statistics filter
    m_StatsFilter = StatisticsMosaicFilterType::New();

    if (GetParameterByKey("vdstats")->HasValue())
    // Use vector data as mask
    {
      otbAppLogINFO("Using vector data for statistics computation");

      // Create mini-pipelines to mask the input images
      m_MaskReaderForStats.clear();
      m_MaskImageFilterForStats.clear();
      for (unsigned int i = 0; i < GetParameterImageList("il")->Size(); i++)
      {
        // 1. Rasterize the vector data in a binary mask
        const string outputFileName = GenerateFileName("tmp_binary_mask_for_stats", i);
        RasterizeBinaryMask(GetParameterVectorDataList("vdstats")->GetNthElement(i), GetParameterImageList("il")->GetNthElement(i), outputFileName, 1.0, true);

        // 2. Add a new reader
        MaskReaderType::Pointer maskReader = CreateReader<MaskReaderType>(outputFileName, m_MaskReaderForStats);

        // 3. Mask the input
        MaskImageFilterType::Pointer maskFilter = CreateMaskFilter(m_InputImagesSources->GetNthElement(i), maskReader->GetOutput(), m_MaskImageFilterForStats);

        m_StatsFilter->PushBackInput(maskFilter->GetOutput());
        m_TemporaryFiles.push_back(outputFileName);
      }
    }
    else
      // No vector data as mask
      for (auto input = m_InputImagesSources->Begin(); input != m_InputImagesSources->End(); ++input)
        m_StatsFilter->PushBackInput(input.Get());

    // Compute statistics
    m_StatsFilter->GetStreamer()->SetAutomaticAdaptativeStreaming(GetParameterInt("ram"));
    AddProcess(m_StatsFilter->GetStreamer(), "Computing statistics");
    m_StatsFilter->Update();
  }

  /*
   * Prepare distance maps
   */
  void ComputeDistanceMaps()
  {

    // Compute distance images
    otbAppLogINFO("Computing distance maps");

    m_DistanceMapImageReader.clear();
    for (unsigned int i = 0; i < GetParameterImageList("il")->Size(); i++)
    {
      const string outputFileName = GenerateFileName("tmp_distance_image", i);
      if (GetParameterByKey("vdcut")->HasValue())
      {
        WriteDistanceImageFromCutline(GetParameterImageList("il")->GetNthElement(i), GetParameterVectorDataList("vdcut")->GetNthElement(i), outputFileName);
      }
      else // use images boundaries
      {
        WriteDistanceImageFromBoundaries(GetParameterImageList("il")->GetNthElement(i), outputFileName);
      }

      m_TemporaryFiles.push_back(outputFileName);

      // Instantiate a reader
      DistanceMapImageReaderType::Pointer reader = CreateReader<DistanceMapImageReaderType>(outputFileName, m_DistanceMapImageReader);
    }
  }

  /*
   * Prepare the sources for compositing.
   * In the specific case of no feathering + cutlines, crop the input images with
   * the cutlines.
   */
  void PrepareSourcesForCompositing()
  {
    if ((GetParameterInt("comp.feather") == Composition_Method_none) && (GetParameterByKey("vdcut")->HasValue()))
    {
      // Use a mask filter for the cutline
      m_MaskReaderForCutline.clear();
      m_MaskImageFilterForCutline.clear();

      m_SourcesForCompositing = FloatVectorImageListType::New();
      for (unsigned int i = 0; i < GetParameterImageList("il")->Size(); i++)
      {
        const string outputFileName = GenerateFileName("tmp_cutline_image", i);
        RasterizeBinaryMask(GetParameterVectorDataList("vdcut")->GetNthElement(i), GetParameterImageList("il")->GetNthElement(i), outputFileName, 1.0, true);

        m_TemporaryFiles.push_back(outputFileName);

        // Mask reader
        MaskReaderType::Pointer maskReader = CreateReader<MaskReaderType>(outputFileName, m_MaskReaderForCutline);

        // Mask filter
        MaskImageFilterType::Pointer maskFilter =
            CreateMaskFilter(m_InputImagesSources->GetNthElement(i), maskReader->GetOutput(), m_MaskImageFilterForCutline);

        // Update source
        m_SourcesForCompositing->PushBack(maskFilter->GetOutput());
      }
    }
    else
      m_SourcesForCompositing = m_InputImagesSources;
  }

  /*
   * Set-up the compositing pipeline
   */
  void BuildCompositingPipeline()
  {

    MosaicFilterType::Pointer mosaicFilter;

    //----------------------------------------------------------------
    // No feathering
    //----------------------------------------------------------------
    if (GetParameterInt("comp.feather") == Composition_Method_none)
    {
      otbAppLogINFO("No feathering");

      m_SimpleMosaicFilter = SimpleMosaicFilterType::New();
      for (auto img = m_SourcesForCompositing->Begin(); img != m_SourcesForCompositing->End(); ++img)
      {
        m_SimpleMosaicFilter->PushBackInput(img.Get());
      }
      mosaicFilter = static_cast<MosaicFilterType*>(m_SimpleMosaicFilter);
    }

    //----------------------------------------------------------------
    // Large feathering
    //----------------------------------------------------------------
    else if (GetParameterInt("comp.feather") == Composition_Method_large)
    {
      otbAppLogINFO("Large feathering");

      m_LargeFeatherMosaicFilter = LargeFeatherMosaicFilterType::New();
      for (unsigned int i = 0; i < m_SourcesForCompositing->Size(); i++)
      {
        m_LargeFeatherMosaicFilter->PushBackInputs(m_SourcesForCompositing->GetNthElement(i), m_DistanceMapImageReader[i]->GetOutput());
      }
      ComputeDistanceOffset<LargeFeatherMosaicFilterType>(m_LargeFeatherMosaicFilter);
      mosaicFilter = static_cast<MosaicFilterType*>(m_LargeFeatherMosaicFilter);
    }

    //----------------------------------------------------------------
    // Slim feathering
    //----------------------------------------------------------------
    else if (GetParameterInt("comp.feather") == Composition_Method_slim)
    {
      otbAppLogINFO("Slim feathering");

      m_SlimFeatherMosaicFilter = SlimFeatherMosaicFilterType::New();
      for (unsigned int i = 0; i < m_SourcesForCompositing->Size(); i++)
      {
        m_SlimFeatherMosaicFilter->PushBackInputs(m_SourcesForCompositing->GetNthElement(i), m_DistanceMapImageReader[i]->GetOutput());
      }
      ComputeDistanceOffset<SlimFeatherMosaicFilterType>(m_SlimFeatherMosaicFilter);

      // Set transition length and smoothness
      m_SlimFeatherMosaicFilter->SetFeatheringTransitionDistance(GetParameterFloat("comp.feather.slim.length"));
      m_SlimFeatherMosaicFilter->SetFeatheringSmoothness(GetParameterFloat("comp.feather.slim.exponent"));

      mosaicFilter = static_cast<MosaicFilterType*>(m_SlimFeatherMosaicFilter);
    }

    //----------------------------------------------------------------
    // Unknown (throw error)
    //----------------------------------------------------------------
    else
    {
      otbAppLogFATAL("Unknown compositing mode");
    }

    // Setup mosaic filter
    PrepareMosaicFilter<MosaicFilterType>(mosaicFilter);

    // Setup output color space
    if (GetParameterInt("harmo.method") == Harmonisation_Method_rgb)
    {
      // Output image LAB --> RGB
      m_LAB2RGBFilter = LAB2RGBFilterType::New();
      m_LAB2RGBFilter->SetInput(mosaicFilter->GetOutput());
      SetParameterOutputImage("out", m_LAB2RGBFilter->GetOutput());
    }
    else
    {
      SetParameterOutputImage("out", mosaicFilter->GetOutput());
    }
  }

  /*
   * Setup input color space (LAB or original)
   */
  void PrepareInputImagesSource()
  {
    if (GetParameterInt("harmo.method") == Harmonisation_Method_rgb)
    {
      otbAppLogINFO("Using LAB color space for harmonization");

      // Input images RGB --> LAB
      m_RGB2LABFilters.clear();
      m_InputImagesSources = FloatVectorImageListType::New();
      for (auto img = GetParameterImageList("il")->Begin(); img != GetParameterImageList("il")->End(); ++img)
      {
        RGB2LABFilterType::Pointer rgb2labFilter = RGB2LABFilterType::New();
        rgb2labFilter->SetInput(img.Get());
        rgb2labFilter->UpdateOutputInformation();
        m_RGB2LABFilters.push_back(rgb2labFilter);

        m_InputImagesSources->PushBack(rgb2labFilter->GetOutput());
      }
    }
    else
    {
      m_InputImagesSources = GetParameterImageList("il");
    }
  }

  void DoExecute() override
  {
    GDALAllRegister();
    m_TemporaryFiles.clear();
    CheckNbOfInputs();

    ResolveTemporaryDirectory();

    PrepareInputImagesSource();
    PrepareSourcesForCompositing();

    // Compute distance maps if needed
    if (GetParameterInt("comp.feather") != Composition_Method_none)
    {
      ComputeDistanceMaps();
    }

    // Compute statistics if needed
    if (GetParameterInt("harmo.method") != Harmonisation_Method_none)
    {
      ComputeImagesStatistics();
    }

    BuildCompositingPipeline();

  } // DoExecute()

  void AfterExecuteAndWriteOutputs() override
  {
    if (m_TemporaryFiles.size() > 0)
    {
      otbAppLogINFO("Clean temporary files");
      for (const auto& file : m_TemporaryFiles)
        deleteFile(file);
    }
  }

  // Sources
  FloatVectorImageListType::Pointer m_SourcesForCompositing;
  FloatVectorImageListType::Pointer m_InputImagesSources;

  // Mosaic filters
  SimpleMosaicFilterType::Pointer       m_SimpleMosaicFilter;
  LargeFeatherMosaicFilterType::Pointer m_LargeFeatherMosaicFilter;
  SlimFeatherMosaicFilterType::Pointer  m_SlimFeatherMosaicFilter;
  StatisticsMosaicFilterType::Pointer   m_StatsFilter;

  // RGB<-->LAB functors filters
  vector<RGB2LABFilterType::Pointer> m_RGB2LABFilters;
  LAB2RGBFilterType::Pointer         m_LAB2RGBFilter;

  // Mask image filters
  vector<MaskImageFilterType::Pointer> m_MaskImageFilterForStats;
  vector<MaskReaderType::Pointer>      m_MaskReaderForStats;
  vector<MaskImageFilterType::Pointer> m_MaskImageFilterForCutline;
  vector<MaskReaderType::Pointer>      m_MaskReaderForCutline;

  // Distance images reader
  vector<DistanceMapImageReaderType::Pointer> m_DistanceMapImageReader;

  // Parameters
  string         m_TempFilesPrefix; // Temp. directory
  vector<string> m_TemporaryFiles;  // Temp. filenames for distance images, masks, etc.
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::Mosaic)
