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

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbVectorImage.h"
#include "otbImage.h"
#include "itkEuclideanDistance.h"
#include "otbStreamingTraits.h"
#include "itkImageRegionConstIterator.h"
#include "itkListSample.h"
#include "itkWeightedCentroidKdTreeGenerator.h"
#include "itkKdTreeBasedKmeansEstimator.h"
#include "otbStreamingShrinkImageFilter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "otbChangeLabelImageFilter.h"
#include "otbRAMDrivenTiledStreamingManager.h"
#include "otbRAMDrivenStrippedStreamingManager.h"

#include "otbChangeLabelImageFilter.h"
#include "itkLabelToRGBImageFilter.h"
#include "otbReliefColormapFunctor.h"
#include "itkScalarToRGBColormapImageFilter.h"
#include "otbWrapperTypes.h"


namespace otb
{


namespace Wrapper
{

namespace Functor
{
template <class TSample, class TLabel> class KMeansFunctor
{
public:
  /** operator */
  TLabel operator ()(const TSample& sample) const
  {
    typename CentroidMapType::const_iterator it = m_CentroidsMap.begin();

    if (it == m_CentroidsMap.end())
      {
      return 0;
      }

    TLabel resp = it->first;
    double minDist = m_Distance->Evaluate(sample, it->second);
    ++it;

    while (it != m_CentroidsMap.end())
      {
      double dist = m_Distance->Evaluate(sample, it->second);

      if (dist < minDist)
        {
        resp = it->first;
        minDist = dist;
        }
      ++it;
      }
    return resp;
  }

  /** Add a new centroid */
  void AddCentroid(const TLabel& label, const TSample& centroid)
  {
    m_CentroidsMap[label] = centroid;
  }

  /** Constructor */
  KMeansFunctor() : m_CentroidsMap(), m_Distance()
  {
    m_Distance = DistanceType::New();
  }

  bool operator !=(const KMeansFunctor& other) const
  {
    return m_CentroidsMap != other.m_CentroidsMap;
  }

private:
  typedef std::map<TLabel, TSample>                   CentroidMapType;
  typedef itk::Statistics::EuclideanDistance<TSample> DistanceType;

  CentroidMapType m_CentroidsMap;
  typename DistanceType::Pointer m_Distance;
};
}


typedef FloatImageType::PixelType PixelType;
typedef UInt8ImageType   LabeledImageType;

typedef UInt8VectorImageType        VectorImageType;
typedef VectorImageType::PixelType  VectorPixelType;
typedef UInt8RGBImageType           RGBImageType;
typedef RGBImageType::PixelType     RGBPixelType;



typedef LabeledImageType::PixelType LabelType;


typedef FloatVectorImageType::PixelType                               SampleType;
typedef itk::Statistics::ListSample<SampleType> ListSampleType;
typedef itk::Statistics::WeightedCentroidKdTreeGenerator<ListSampleType> TreeGeneratorType;
typedef TreeGeneratorType::KdTreeType TreeType;
typedef itk::Statistics::KdTreeBasedKmeansEstimator<TreeType> EstimatorType;
typedef RAMDrivenStrippedStreamingManager<FloatVectorImageType> RAMDrivenStrippedStreamingManagerType;


typedef itk::ImageRegionConstIterator<FloatVectorImageType> IteratorType;
typedef itk::ImageRegionConstIterator<LabeledImageType> LabeledIteratorType;

typedef otb::StreamingShrinkImageFilter<FloatVectorImageType,
     FloatVectorImageType>              ImageSamplingFilterType;

typedef otb::StreamingShrinkImageFilter<LabeledImageType,
    UInt8ImageType>              MaskSamplingFilterType;
typedef Functor::KMeansFunctor<SampleType, LabelType> KMeansFunctorType;
typedef itk::UnaryFunctorImageFilter<FloatVectorImageType,
    LabeledImageType, KMeansFunctorType>     KMeansFilterType;


// Manual label LUT
 typedef otb::ChangeLabelImageFilter
 <LabeledImageType, VectorImageType>    ChangeLabelFilterType;

 // Continuous LUT mapping
  typedef itk::ScalarToRGBColormapImageFilter<LabeledImageType, RGBImageType>      ColorMapFilterType;


  typedef otb::Functor::ReliefColormapFunctor
   <LabelType, RGBPixelType>           ReliefColorMapFunctorType;

  typedef otb::ImageMetadataInterfaceBase ImageMetadataInterfaceType;


class KMeansClassification: public Application
{
public:
  /** Standard class typedefs. */
  typedef KMeansClassification Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(KMeansClassification, otb::Application);

private:
  KMeansClassification()
  {
    SetName("KMeansClassification");
    SetDescription("Unsupervised KMeans image classification");

    SetDocName("Unsupervised KMeans image classification Application");
    SetDocLongDescription("Performs Unsupervised KMeans image classification.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
  
    // Build lut map

      m_LutMap["Red"]=ColorMapFilterType::Red;
      m_LutMap["Green"]=ColorMapFilterType::Green;
      m_LutMap["Blue"]=ColorMapFilterType::Blue;
      m_LutMap["Grey"]=ColorMapFilterType::Grey;
      m_LutMap["Hot"]=ColorMapFilterType::Hot;
      m_LutMap["Cool"]=ColorMapFilterType::Cool;
      m_LutMap["Spring"]=ColorMapFilterType::Spring;
      m_LutMap["Summer"]=ColorMapFilterType::Summer;
      m_LutMap["Autumn"]=ColorMapFilterType::Autumn;
      m_LutMap["Winter"]=ColorMapFilterType::Winter;
      m_LutMap["Copper"]=ColorMapFilterType::Copper;
      m_LutMap["Jet"]=ColorMapFilterType::Jet;
      m_LutMap["HSV"]=ColorMapFilterType::HSV;
      m_LutMap["OverUnder"]=ColorMapFilterType::OverUnder;



    AddDocTag(Tags::Segmentation);
    AddDocTag(Tags::Learning);
  }

  virtual ~KMeansClassification()
  {
  }

  void DoCreateParameters()
  {

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "Input image filename.");
    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out", "Output image filename.");
    AddParameter(ParameterType_RAM, "ram", "Available RAM");
    SetDefaultParameterInt("ram", 256);
    MandatoryOff("ram");
    AddParameter(ParameterType_InputImage, "vm", "Validity Mask");
    SetParameterDescription("vm", "Validity mask. Only non-zero pixels will be used to estimate KMeans modes.");
    AddParameter(ParameterType_Int, "ts", "Training set size");
    SetParameterDescription("ts", "Size of the training set.");
    SetDefaultParameterInt("ts", 100);
    MandatoryOff("ts");
    AddParameter(ParameterType_Int, "nc", "Number of classes");
    SetParameterDescription("nc", "number of modes, which will be used to generate class membership.");
    SetDefaultParameterInt("nc", 5);
    AddParameter(ParameterType_Int, "maxit", "Maximum number of iterations");
    SetParameterDescription("maxit", "Maximum number of iterations.");
    SetDefaultParameterInt("maxit", 1000);
    MandatoryOff("maxit");
    AddParameter(ParameterType_Float, "ct", "Convergence threshold");
    SetParameterDescription("ct", "Convergence threshold for class centroid  (L2 distance, by default 0.01).");
    SetDefaultParameterFloat("ct", 0.0001);
    MandatoryOff("ct");
    AddParameter(ParameterType_Filename, "outmeans", "Centroid filename");
    SetParameterDescription("outmeans", "save label centroid in txt file.");
    MandatoryOff("outmeans");
    SetParameterRole("outmeans", Role_Output);

    AddParameter(ParameterType_Group, "mapping", "Color mapping");
    SetParameterDescription("mapping", "Save colorized image and LUT in txt file.");
    AddParameter(ParameterType_Filename, "mapping.labelout","Output Colorized Image");
    SetParameterDescription("mapping.labelout", "Output colorized image filename.");
    SetParameterRole("mapping.labelout", Role_Output);
    AddParameter(ParameterType_Filename, "mapping.lutout", "Output LUT filename.");

    AddParameter(ParameterType_Choice, "mapping.lut", "Look-up tables");
    SetParameterDescription("mapping.lut", "Available look-up tables. RGBIntensity remap label with class centroids, which are reweighted into [0 255]");

    AddChoice("mapping.lut.rgbintensity", "RGBIntensity");
    AddChoice("mapping.lut.red", "Red");
    AddChoice("mapping.lut.green", "Green");
    AddChoice("mapping.lut.blue", "Blue");
    AddChoice("mapping.lut.grey", "Grey");
    AddChoice("mapping.lut.hot", "Hot");
    AddChoice("mapping.lut.cool", "Cool");
    AddChoice("mapping.lut.spring", "Spring");
    AddChoice("mapping.lut.summer", "Summer");
    AddChoice("mapping.lut.autumn", "Autumn");
    AddChoice("mapping.lut.winter", "Winter");
    AddChoice("mapping.lut.copper", "Copper");
    AddChoice("mapping.lut.jet", "Jet");
    AddChoice("mapping.lut.hsv", "HSV");
    AddChoice("mapping.lut.overunder", "OverUnder");
    AddChoice("mapping.lut.relief", "Relief");
    SetParameterInt("mapping.lut", 0);

    MandatoryOff("mapping");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "qb_RoadExtract.img");
    SetDocExampleParameterValue("vm", "qb_RoadExtract_mask.png");
    SetDocExampleParameterValue("ts", "1000");
    SetDocExampleParameterValue("nc", "5");
    SetDocExampleParameterValue("maxit", "1000");
    SetDocExampleParameterValue("ct", "0.0001");
    SetDocExampleParameterValue("out", "ClassificationFilterOuptut.tif");
  }

  void DoUpdateParameters()
  {

    // test of input image //
    if (HasValue("in"))
      {
      // input image
      FloatVectorImageType::Pointer inImage = GetParameterImage("in");

      RAMDrivenStrippedStreamingManagerType::Pointer streamingManager = RAMDrivenStrippedStreamingManagerType::New();
      int availableRAM = GetParameterInt("ram");
      streamingManager->SetAvailableRAMInMB(availableRAM);
      float bias = 1.5; // empric value;
      streamingManager->SetBias(bias);
      FloatVectorImageType::RegionType largestRegion = inImage->GetLargestPossibleRegion();
      FloatVectorImageType::SizeType largestRegionSize = largestRegion.GetSize();
      streamingManager->PrepareStreaming(inImage, largestRegion);

      unsigned long nbDivisions = streamingManager->GetNumberOfSplits();
      unsigned long largestPixNb = largestRegionSize[0] * largestRegionSize[1];

      unsigned long maxPixNb = largestPixNb / nbDivisions;

      if (GetParameterInt("ts") > static_cast<int> (maxPixNb))
        {
        otbAppLogWARNING(" available RAM is too small to process this sample size is.Sample size will be reduced to "<<maxPixNb<<std::endl);
        this->SetParameterInt("ts", maxPixNb);
        }

      this->SetMaximumParameterIntValue("ts", maxPixNb);

      }

  }

  void DoExecute()
  {
    GetLogger()->Debug("Entering DoExecute\n");

    m_InImage = GetParameterImage("in");
    m_InImage->UpdateOutputInformation();

    UInt8ImageType::Pointer maskImage = GetParameterUInt8Image("vm");
    maskImage->UpdateOutputInformation();
    std::ostringstream message("");

    int nbsamples = GetParameterInt("ts");
    const unsigned int nbClasses = GetParameterInt("nc");

    /*******************************************/
    /*           Sampling data                 */
    /*******************************************/otbAppLogINFO("-- SAMPLING DATA --"<<std::endl);

    // Update input images information
    m_InImage->UpdateOutputInformation();
    maskImage->UpdateOutputInformation();

    if (m_InImage->GetLargestPossibleRegion() != maskImage->GetLargestPossibleRegion())
      {
      GetLogger()->Error("Mask image and input image have different sizes.");
      }

    // Training sample lists
    ListSampleType::Pointer sampleList = ListSampleType::New();

    unsigned int init_means_index = 0;

    // Sample dimension and max dimension
    const unsigned int nbComp = m_InImage->GetNumberOfComponentsPerPixel();
    unsigned int sampleSize = nbComp;
    unsigned int totalSamples = 0;

    // sampleSize = std::min(nbComp, maxDim);

    EstimatorType::ParametersType initialMeans(nbComp * nbClasses);
    initialMeans.Fill(0);

    // use image and mask shrink

    ImageSamplingFilterType::Pointer imageSampler = ImageSamplingFilterType::New();
    imageSampler->SetInput(m_InImage);
    MaskSamplingFilterType::Pointer maskSampler = MaskSamplingFilterType::New();
    maskSampler->SetInput(maskImage);

    double theoricNBSamplesForKMeans = nbsamples;

    const double upperThresholdNBSamplesForKMeans = 1000 * 1000;
    const double actualNBSamplesForKMeans = std::min(theoricNBSamplesForKMeans, upperThresholdNBSamplesForKMeans);

    otbAppLogINFO(<<actualNBSamplesForKMeans<<" is the maximum sample size that will be used."<<std::endl);

    const double shrinkFactor = vcl_floor(
                                          vcl_sqrt(
                                                   m_InImage->GetLargestPossibleRegion().GetNumberOfPixels()
                                                       / actualNBSamplesForKMeans));
    imageSampler->SetShrinkFactor(shrinkFactor);
    imageSampler->Update();
    maskSampler->SetShrinkFactor(shrinkFactor);
    maskSampler->Update();

    // Then, build the sample list

    IteratorType it(imageSampler->GetOutput(), imageSampler->GetOutput()->GetLargestPossibleRegion());
    LabeledIteratorType m_MaskIt(maskSampler->GetOutput(), maskSampler->GetOutput()->GetLargestPossibleRegion());

    it.GoToBegin();
    m_MaskIt.GoToBegin();

    SampleType min;
    SampleType max;
    SampleType sample;
    //first sample
    while (!it.IsAtEnd() && !m_MaskIt .IsAtEnd() && (m_MaskIt.Get() <= 0))
      {
      ++it;
      ++m_MaskIt;
      }

    min = it.Get();
    max = it.Get();
    sample = it.Get();

    sampleList->PushBack(sample);

    ++it;
    ++m_MaskIt;

    totalSamples = 1;
    while (!it.IsAtEnd() && !m_MaskIt .IsAtEnd())
      {
      if (m_MaskIt.Get() > 0)
        {
        totalSamples++;

        sample = it.Get();

        sampleList->PushBack(sample);

        for (unsigned int i = 0; i < nbComp; ++i)
          {
          if (min[i] > sample[i])
            {
            min[i] = sample[i];
            }
          if (max[i] < sample[i])
            {
            max[i] = sample[i];
            }
          }
        }
      ++it;
      ++m_MaskIt;
      }

    // Next, initialize centroids

    for (unsigned int classIndex = 0; classIndex < nbClasses; ++classIndex)
      {
      for (unsigned int compIndex = 0; compIndex < sampleSize; ++compIndex)
        {
        initialMeans[compIndex + classIndex * sampleSize] = min[compIndex] + (max[compIndex] - min[compIndex]) * rand()
            / (RAND_MAX + 1.0);

        }
      }
    otbAppLogINFO(<<totalSamples <<" samples will be used as estimator input."<<std::endl);

    /*******************************************/
    /*           Learning                      */
    /*******************************************/

    otbAppLogINFO("-- LEARNING --" << std::endl);
    otbAppLogINFO("Initial centroids are: " << std::endl);

    message.clear();
    message << std::endl;
    for (unsigned int i = 0; i < nbClasses; i++)
      {
      message << "Class " << i << ": ";
      for (unsigned int j = 0; j < sampleSize; j++)
        {
        message << std::setw(8) << initialMeans[i * sampleSize + j] << "   ";
        }
      message << std::endl;
      }
    message << std::endl;
    GetLogger()->Info(message.str());
    message.clear();
    otbAppLogINFO("Starting optimization." << std::endl);
    EstimatorType::Pointer estimator = EstimatorType::New();

    TreeGeneratorType::Pointer treeGenerator = TreeGeneratorType::New();
    treeGenerator->SetSample(sampleList);

    treeGenerator->SetBucketSize(10000);
    treeGenerator->Update();

    estimator->SetParameters(initialMeans);
    estimator->SetKdTree(treeGenerator->GetOutput());
    int maxIt = GetParameterInt("maxit");
    estimator->SetMaximumIteration(maxIt);
    estimator->SetCentroidPositionChangesThreshold(GetParameterFloat("ct"));
    estimator->StartOptimization();

    EstimatorType::ParametersType estimatedMeans = estimator->GetParameters();

    otbAppLogINFO("Optimization completed." );
    if (estimator->GetCurrentIteration() == maxIt)
      {
      otbAppLogWARNING("estimator reached maximum iteration number."<<std::endl);
      }
    message.clear();
    message << "Estimated centroids are: " << std::endl;
    message << std::endl;
    for (unsigned int i = 0; i < nbClasses; i++)
      {
      message << "Class " << i << ": ";
      for (unsigned int j = 0; j < sampleSize; j++)
        {
        message << std::setw(8) << estimatedMeans[i * sampleSize + j] << "   ";
        }
      message << std::endl;
      }

    message << std::endl;
    message << "Learning completed." << std::endl;
    message << std::endl;
    GetLogger()->Info(message.str());

    /*******************************************/
    /*           Classification                */
    /*******************************************/otbAppLogINFO("-- CLASSIFICATION --" << std::endl);

    // Finally, update the KMeans filter
    KMeansFunctorType functor;

    for (unsigned int classIndex = 0; classIndex < nbClasses; ++classIndex)
      {
      SampleType centroid(sampleSize);

      for (unsigned int compIndex = 0; compIndex < sampleSize; ++compIndex)
        {
        centroid[compIndex] = estimatedMeans[compIndex + classIndex * sampleSize];
        }
      functor.AddCentroid(classIndex, centroid);
      }

    m_KMeansFilter = KMeansFilterType::New();
    m_KMeansFilter->SetFunctor(functor);
    m_KMeansFilter->SetInput(m_InImage);

    // optional saving option -> lut

    if (IsParameterEnabled("outmeans"))
      {
      std::ofstream file;
      file.open(GetParameterString("outmeans").c_str());
      for (unsigned int i = 0; i < nbClasses; i++)
        {

        for (unsigned int j = 0; j < sampleSize; j++)
          {
          file << std::setw(8) << estimatedMeans[i * sampleSize + j] << " ";
          }
        file << std::endl;
        }

      file.close();
      }
    // colormapping
    if (IsParameterEnabled("mapping"))
      {
      m_ParamOut = OutputImageParameter::New();

      m_ParamOut->SetPixelType(ImagePixelType_uint8);
      m_ParamOut->SetFileName(GetParameterString("mapping.labelout"));

      std::string lut = GetParameterString("mapping.lut");
      if (lut == "RGBIntensity")
        {
        // find RGB
        ImageMetadataInterfaceType::Pointer
            metadataInterface = ImageMetadataInterfaceFactory::CreateIMI(m_InImage->GetMetaDataDictionary());

        std::vector<unsigned int> RGBIndex = metadataInterface->GetDefaultDisplay();

        otbAppLogINFO(" RGB index are "<<RGBIndex[0]<<" "<<RGBIndex[1]<<" "<<RGBIndex[2]<<std::endl);
        // map intensity
        m_CustomMapper = ChangeLabelFilterType::New();
        m_CustomMapper->SetInput(m_KMeansFilter->GetOutput());
        const unsigned int nbCompRGB = 3;
        m_CustomMapper->SetNumberOfComponentsPerPixel(nbCompRGB);

        EstimatorType::ParametersType weithedMeans = estimatedMeans;

        std::vector<double> minVal(nbCompRGB);
        std::vector<double> maxVal(nbCompRGB);

        for (unsigned int i = 0; i < nbCompRGB; i++)
          {
          minVal[i] = estimatedMeans[i];
          maxVal[i] = estimatedMeans[i];
          for (unsigned int j = 0; j < nbClasses; j++)
            {
            double value = estimatedMeans[j * sampleSize + i];
            std::cout << value << " " << std::endl;
            if (value > maxVal[i]) maxVal[i] = value;
            if (value < minVal[i]) minVal[i] = value;
            }
          }

        for (unsigned int j = 0; j < nbClasses; j++)
          {
          VectorPixelType color(nbCompRGB);
          color.Fill(0);

          for (unsigned int i = 0; i < nbCompRGB; i++)
            {
            color[RGBIndex[i]] = static_cast<PixelType> (((estimatedMeans[j * sampleSize + i] - minVal[i]) / (maxVal[i]
                - minVal[i])) * 255.0);
            }
          otbAppLogINFO("Adding color mapping " << j << " -> [" << (int) color[0] << " " << (int) color[1] << " "<< (int) color[2] << " ]" << std::endl);

          m_CustomMapper->SetChange(j, color);
          }

        // Create an output parameter to write the current output image

        m_ParamOut->SetValue(m_CustomMapper->GetOutput());
        m_ParamOut->InitializeWriters();
        AddProcess(m_ParamOut->GetWriter(), GetParameterString("mapping.labelout"));
        m_ParamOut->Write();

        }
      else
        {

        m_ColorMapper = ColorMapFilterType::New();
        m_ColorMapper->SetInput(m_KMeansFilter->GetOutput());

        m_ColorMapper->UseInputImageExtremaForScalingOn();
        if (lut == "Relief")
          {
          ReliefColorMapFunctorType::Pointer reliefFunctor = ReliefColorMapFunctorType::New();
          m_ColorMapper->SetColormap(reliefFunctor);
          }
        else
          {
          m_ColorMapper->SetColormap((ColorMapFilterType::ColormapEnumType) m_LutMap[lut]);
          }

        // reweight label into [0 255]
        m_ColorMapper->GetColormap()->SetMinimumInputValue(0.0);
        m_ColorMapper->GetColormap()->SetMaximumInputValue(255.0);

        m_ParamOut->SetValue(m_ColorMapper->GetOutput());

        m_ParamOut->InitializeWriters();
        AddProcess(m_ParamOut->GetWriter(), GetParameterString("mapping.labelout"));
        m_ParamOut->Write();

        }

      }
    SetParameterOutputImage("out", m_KMeansFilter->GetOutput());

  }

  // KMeans filter
  OutputImageParameter::Pointer       m_ParamOut;
  KMeansFilterType::Pointer           m_KMeansFilter;
  FloatVectorImageType::Pointer       m_InImage;
  ChangeLabelFilterType::Pointer      m_CustomMapper;
  ColorMapFilterType::Pointer         m_ColorMapper;
  std::map<std::string, unsigned int> m_LutMap;

};



}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::KMeansClassification)


