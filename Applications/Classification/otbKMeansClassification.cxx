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
#include "itkEuclideanDistanceMetric.h"
#include "otbStreamingTraits.h"
#include "itkImageRegionConstIterator.h"
#include "itkListSample.h"
#include "itkWeightedCentroidKdTreeGenerator.h"
#include "itkKdTreeBasedKmeansEstimator.h"
#include "otbStreamingShrinkImageFilter.h"
#include "otbChangeLabelImageFilter.h"
#include "otbRAMDrivenStrippedStreamingManager.h"

#include "otbChangeLabelImageFilter.h"
#include "itkLabelToRGBImageFilter.h"
#include "otbReliefColormapFunctor.h"
#include "itkScalarToRGBColormapImageFilter.h"


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
  typedef itk::Statistics::EuclideanDistanceMetric<TSample> DistanceType;

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
  void DoInit()
  {
    SetName("KMeansClassification");
    SetDescription("Unsupervised KMeans image classification");

    SetDocName("Unsupervised KMeans image classification");
    SetDocLongDescription("Performs unsupervised KMeans image classification.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Segmentation);
    AddDocTag(Tags::Learning);
    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "Input image to classify.");
    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out", "Output image containing the class indexes.");

    AddRAMParameter();

    AddParameter(ParameterType_InputImage, "vm", "Validity Mask");
    SetParameterDescription("vm", "Validity mask. Only non-zero pixels will be used to estimate KMeans modes.");
    MandatoryOff("vm");
    AddParameter(ParameterType_Int, "ts", "Training set size");
    SetParameterDescription("ts", "Size of the training set (in pixels).");
    SetDefaultParameterInt("ts", 100);
    MandatoryOff("ts");
    AddParameter(ParameterType_Int, "nc", "Number of classes");
    SetParameterDescription("nc", "Number of modes, which will be used to generate class membership.");
    SetDefaultParameterInt("nc", 5);
    AddParameter(ParameterType_Int, "maxit", "Maximum number of iterations");
    SetParameterDescription("maxit", "Maximum number of iterations for the learning step.");
    SetDefaultParameterInt("maxit", 1000);
    MandatoryOff("maxit");
    AddParameter(ParameterType_Float, "ct", "Convergence threshold");
    SetParameterDescription("ct", "Convergence threshold for class centroid  (L2 distance, by default 0.0001).");
    SetDefaultParameterFloat("ct", 0.0001);
    MandatoryOff("ct");
    AddParameter(ParameterType_OutputFilename, "outmeans", "Centroid filename");
    SetParameterDescription("outmeans", "Output text file containing centroid positions");
    MandatoryOff("outmeans");

    AddRANDParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_1_ortho.tif");
    SetDocExampleParameterValue("ts", "1000");
    SetDocExampleParameterValue("nc", "5");
    SetDocExampleParameterValue("maxit", "1000");
    SetDocExampleParameterValue("ct", "0.0001");
    SetDocExampleParameterValue("out", "ClassificationFilterOutput.tif");
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
      float bias = 1.5; // empirical value
      streamingManager->SetBias(bias);
      FloatVectorImageType::RegionType largestRegion = inImage->GetLargestPossibleRegion();
      FloatVectorImageType::SizeType largestRegionSize = largestRegion.GetSize();
      streamingManager->PrepareStreaming(inImage, largestRegion);

      unsigned long nbDivisions = streamingManager->GetNumberOfSplits();
      unsigned long largestPixNb = largestRegionSize[0] * largestRegionSize[1];

      unsigned long maxPixNb = largestPixNb / nbDivisions;

      if (GetParameterInt("ts") > static_cast<int> (maxPixNb))
        {
        otbAppLogWARNING("The available RAM is too small to process this sample size of " << GetParameterInt("ts") <<
            " pixels. The sample size will be reduced to " << maxPixNb << " pixels." << std::endl);
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
    UInt8ImageType::Pointer maskImage;

    std::ostringstream message("");

    int nbsamples = GetParameterInt("ts");
    const unsigned int nbClasses = GetParameterInt("nc");

    /*******************************************/
    /*           Sampling data                 */
    /*******************************************/

    otbAppLogINFO("-- SAMPLING DATA --"<<std::endl);

    // Update input images information
    m_InImage->UpdateOutputInformation();

    bool maskFlag = IsParameterEnabled("vm");
    if (maskFlag)
      {
      otbAppLogINFO("sample choice using mask "<<std::endl);
      maskImage = GetParameterUInt8Image("vm");
      maskImage->UpdateOutputInformation();
      if (m_InImage->GetLargestPossibleRegion() != maskImage->GetLargestPossibleRegion())
        {
        GetLogger()->Error("Mask image and input image have different sizes.");
        return;
        }
      }

    // Training sample lists
    ListSampleType::Pointer sampleList = ListSampleType::New();
    sampleList->SetMeasurementVectorSize(m_InImage->GetNumberOfComponentsPerPixel());

    //unsigned int init_means_index = 0;

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

    double theoricNBSamplesForKMeans = nbsamples;

    const double upperThresholdNBSamplesForKMeans = 1000 * 1000;
    const double actualNBSamplesForKMeans = std::min(theoricNBSamplesForKMeans, upperThresholdNBSamplesForKMeans);

    otbAppLogINFO(<< actualNBSamplesForKMeans << " is the maximum sample size that will be used." << std::endl);

    const double shrinkFactor = vcl_floor(
                                          vcl_sqrt(
                                                   m_InImage->GetLargestPossibleRegion().GetNumberOfPixels()
                                                       / actualNBSamplesForKMeans));
    imageSampler->SetShrinkFactor(shrinkFactor);
    imageSampler->Update();

    MaskSamplingFilterType::Pointer maskSampler;
    LabeledIteratorType m_MaskIt;
    if (maskFlag)
      {
      maskSampler = MaskSamplingFilterType::New();
      maskSampler->SetInput(maskImage);
      maskSampler->SetShrinkFactor(shrinkFactor);
      maskSampler->Update();
      m_MaskIt = LabeledIteratorType(maskSampler->GetOutput(), maskSampler->GetOutput()->GetLargestPossibleRegion());
      m_MaskIt.GoToBegin();
      }
    // Then, build the sample list

    IteratorType it(imageSampler->GetOutput(), imageSampler->GetOutput()->GetLargestPossibleRegion());

    it.GoToBegin();

    SampleType min;
    SampleType max;
    SampleType sample;
    //first sample

    itk::Statistics::MersenneTwisterRandomVariateGenerator::Pointer randGen=itk::Statistics::MersenneTwisterRandomVariateGenerator::GetInstance();

    //randGen->Initialize();

    if (maskFlag)
      {
      while (!it.IsAtEnd() && !m_MaskIt.IsAtEnd() && (m_MaskIt.Get() <= 0))
        {
        ++it;
        ++m_MaskIt;
        }

      // If the mask is empty after the subsampling
      if (m_MaskIt.IsAtEnd())
        {
        GetLogger()->Error("The mask image is empty after subsampling. Please increase the training set size.");
        return;
        }
      }

    min = it.Get();
    max = it.Get();
    sample = it.Get();

    sampleList->PushBack(sample);

    ++it;

    if (maskFlag)
      {
      ++m_MaskIt;
      }

    totalSamples = 1;
    bool selectSample;
    while (!it.IsAtEnd())
      {
      if (maskFlag)
        {
        selectSample = (m_MaskIt.Get() > 0);
        ++m_MaskIt;
        }
      else selectSample = true;

      if (selectSample)
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
      }

    // Next, initialize centroids by random sampling in the generated
    // list of samples

    for (unsigned int classIndex = 0; classIndex < nbClasses; ++classIndex)
      {
      SampleType newCentroid = sampleList->GetMeasurementVector(randGen->GetIntegerVariate(sampleList->Size()-1));

      for (unsigned int compIndex = 0; compIndex < sampleSize; ++compIndex)
        {
        initialMeans[compIndex + classIndex * sampleSize] = newCentroid[compIndex];
        }
      }
    otbAppLogINFO(<< totalSamples << " samples will be used as estimator input." << std::endl);

    /*******************************************/
    /*           Learning                      */
    /*******************************************/

    otbAppLogINFO("-- LEARNING --" << std::endl);
    otbAppLogINFO("Initial centroids are: " << std::endl);

    message.str("");
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
    message.str("");
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
      otbAppLogWARNING("The estimator reached the maximum iteration number." << std::endl);
      }
    message.str("");
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
    /*******************************************/
    otbAppLogINFO("-- CLASSIFICATION --" << std::endl);

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

    SetParameterOutputImage("out", m_KMeansFilter->GetOutput());

  }

  // KMeans filter
  KMeansFilterType::Pointer           m_KMeansFilter;
  FloatVectorImageType::Pointer       m_InImage;

};


}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::KMeansClassification)


