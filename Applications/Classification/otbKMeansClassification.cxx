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
#include "itkImageRegionSplitter.h"
#include "otbStreamingTraits.h"
#include "otbKMeansImageClassificationFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkListSample.h"
#include "itkWeightedCentroidKdTreeGenerator.h"
#include "itkKdTreeBasedKmeansEstimator.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"

namespace otb
{
namespace Wrapper
{

typedef FloatImageType::PixelType PixelType;

typedef itk::FixedArray<PixelType, 108> SampleType;
typedef itk::Statistics::ListSample<SampleType> ListSampleType;
typedef itk::Statistics::WeightedCentroidKdTreeGenerator<ListSampleType> TreeGeneratorType;
typedef TreeGeneratorType::KdTreeType TreeType;
typedef itk::Statistics::KdTreeBasedKmeansEstimator<TreeType> EstimatorType;

typedef otb::StreamingTraits<FloatVectorImageType> StreamingTraitsType;
typedef itk::ImageRegionSplitter<2> SplitterType;
typedef FloatImageType::RegionType RegionType;

typedef itk::ImageRegionConstIterator<FloatVectorImageType> IteratorType;
typedef itk::ImageRegionConstIterator<UInt8ImageType> LabeledIteratorType;

typedef otb::KMeansImageClassificationFilter<FloatVectorImageType, UInt8ImageType, 108> ClassificationFilterType;

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
    SetDocCLExample("otbApplicationLauncherCommandLine KMeansClassification "
      "--in ${OTB-Data}/Input/poupees_sub.png --vm ${OTB-Data}/Input/mask_KMeans.png "
      "--ts 100 --tp 0.6 --nc 5 --cp 0.9 --sl 100 --out ClassificationFilterOuptut.tif ");
    AddDocTag("Classification");

  }

  virtual ~KMeansClassification()
  {
  }

  void DoCreateParameters()
  {

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in","Input image filename.");
    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out","Output image filename.");
    AddParameter(ParameterType_InputImage, "vm", "Validity Mask");
    SetParameterDescription("vm","Validity mask. Only non-zero pixels will be used to estimate KMeans modes.");
    AddParameter(ParameterType_Int, "ts", "Training set size");
    SetParameterDescription("ts", "Size of the training set.");
    SetParameterInt("ts", 100);
    AddParameter(ParameterType_Float, "tp", "Training set sample selection probability");
    SetParameterDescription("tp", "Probability for a sample to be selected in the training set.");
    SetParameterFloat("tp", 0.5);
    AddParameter(ParameterType_Int, "nc", "Number of classes");
    SetParameterDescription("nc","number of modes, which will be used to generate class membership.");
    SetParameterInt("nc", 3);
    AddParameter(ParameterType_Float, "cp", "Initial class centroid probability");
    SetParameterDescription("cp", "Probability for a pixel to be selected as an initial class centroid");
    SetParameterFloat("cp", 0.8);
    AddParameter(ParameterType_Int, "sl", "Number of lines for each streaming block");
    SetParameterDescription("sl","input image will be divided into sl lines.");
    SetParameterInt("sl", 1000);
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    GetLogger()->Debug("Entering DoExecute\n");

    // initiating random number generation
    itk::Statistics::MersenneTwisterRandomVariateGenerator::Pointer
        randomGen = itk::Statistics::MersenneTwisterRandomVariateGenerator::New();
    m_InImage = GetParameterImage("in");
    std::cout<<"mask in progress"<<std::endl;

    UInt8ImageType::Pointer maskImage = GetParameterUInt8Image("vm");

    std::cout<<"mask in progress done"<<std::endl;
    std::ostringstream message("");

    const unsigned int nbsamples = GetParameterInt("ts");
    const double trainingProb = GetParameterFloat("tp");
    const double initProb = GetParameterFloat("cp");
    const unsigned int nb_classes = GetParameterInt("nc");
    const unsigned int nbLinesForStreaming = GetParameterInt("sl");

    /*******************************************/
    /*           Sampling data                 */
    /*******************************************/
    GetLogger()->Info("-- SAMPLING DATA --");

    // Update input images information
    m_InImage->UpdateOutputInformation();
    maskImage->UpdateOutputInformation();

    if (m_InImage->GetLargestPossibleRegion() != maskImage->GetLargestPossibleRegion())
      {
      GetLogger()->Error("Mask image and input image have different sizes.");
      }

    RegionType largestRegion = m_InImage->GetLargestPossibleRegion();

    // Setting up local streaming capabilities
    SplitterType::Pointer splitter = SplitterType::New();
    unsigned int
        numberOfStreamDivisions = StreamingTraitsType::CalculateNumberOfStreamDivisions(
                                                                                        m_InImage,
                                                                                        largestRegion,
                                                                                        splitter,
                                                                                        otb::SET_BUFFER_NUMBER_OF_LINES,
                                                                                        0, 0, nbLinesForStreaming);

    message.clear();
    message << "The images will be streamed into " << numberOfStreamDivisions << " parts.";
    GetLogger()->Info(message.str());

    // Training sample lists
    ListSampleType::Pointer sampleList = ListSampleType::New();
    EstimatorType::ParametersType initialMeans(108 * nb_classes);
    initialMeans.Fill(0);
    unsigned int init_means_index = 0;

    // Sample dimension and max dimension
    unsigned int maxDimension = SampleType::Dimension;
    unsigned int sampleSize = std::min(m_InImage->GetNumberOfComponentsPerPixel(), maxDimension);
    unsigned int totalSamples = 0;

    message.clear();
    message << "Sample max possible dimension: " << maxDimension << std::endl;
    GetLogger()->Info(message.str());
    message.clear();
    message << "The following sample size will be used: " << sampleSize << std::endl;
    GetLogger()->Info(message.str());
    // local streaming variables
    unsigned int piece = 0;
    RegionType streamingRegion;

    while ((totalSamples < nbsamples) && (init_means_index < 108 * nb_classes))
      {
      double random = randomGen->GetVariateWithClosedRange();
      piece = static_cast<unsigned int> (random * numberOfStreamDivisions);

      streamingRegion = splitter->GetSplit(piece, numberOfStreamDivisions, largestRegion);

      message.clear();
      message << "Processing region: " << streamingRegion << std::endl;
      GetLogger()->Info(message.str());

      m_InImage->SetRequestedRegion(streamingRegion);
      m_InImage->PropagateRequestedRegion();
      m_InImage->UpdateOutputData();

      maskImage->SetRequestedRegion(streamingRegion);
      maskImage->PropagateRequestedRegion();
      maskImage->UpdateOutputData();

      IteratorType it(m_InImage, streamingRegion);
      LabeledIteratorType m_MaskIt(maskImage, streamingRegion);

      it.GoToBegin();
      m_MaskIt.GoToBegin();

      unsigned int localNbSamples = 0;

      // Loop on the image
      while (!it.IsAtEnd() && !m_MaskIt.IsAtEnd() && (totalSamples < nbsamples)
          && (init_means_index < (108 * nb_classes)))
        {
        // If the current pixel is labeled
        if (m_MaskIt.Get() > 0)
          {
          if ((rand() < trainingProb * RAND_MAX))
            {
            SampleType newSample;

            // build the sample
            newSample.Fill(0);
            for (unsigned int i = 0; i < sampleSize; ++i)
              {
              newSample[i] = it.Get()[i];
              }
            // Update the the sample lists
            sampleList->PushBack(newSample);
            ++totalSamples;
            ++localNbSamples;
            }
          else
            if ((init_means_index < 108 * nb_classes) && (rand() < initProb * RAND_MAX))
              {
              for (unsigned int i = 0; i < sampleSize; ++i)
                {
                initialMeans[init_means_index + i] = it.Get()[i];
                }
              init_means_index += 108;
              }
          }
        ++it;
        ++m_MaskIt;
        }

      message.clear();
      message << localNbSamples << " samples added to the training set." << std::endl;
      GetLogger()->Info(message.str());

      }

    message.clear();
    message << "The final training set contains " << totalSamples << " samples." << std::endl;
    GetLogger()->Info(message.str());

    message.clear();
    message << "Data sampling completed." << std::endl;
    GetLogger()->Info(message.str());

    /*******************************************/
    /*           Learning                      */
    /*******************************************/
    message.clear();
    message << "-- LEARNING --" << std::endl;
    message << "Initial centroids are: " << std::endl;
    GetLogger()->Info(message.str());
    message.clear();
    for (unsigned int i = 0; i < nb_classes; ++i)
      {
      message << "Class " << i << ": ";
      for (unsigned int j = 0; j < sampleSize; ++j)
        {
        message << initialMeans[i * 108 + j] << "\t";
        }
      message << std::endl;
      }
    message << std::endl;

    message.clear();
    message << "Starting optimization." << std::endl;
    message << std::endl;
    GetLogger()->Info(message.str());

    EstimatorType::Pointer estimator = EstimatorType::New();

    TreeGeneratorType::Pointer treeGenerator = TreeGeneratorType::New();
    treeGenerator->SetSample(sampleList);
    treeGenerator->SetBucketSize(100);
    treeGenerator->Update();

    estimator->SetParameters(initialMeans);
    estimator->SetKdTree(treeGenerator->GetOutput());
    estimator->SetMaximumIteration(100000000);
    estimator->SetCentroidPositionChangesThreshold(0.001);
    estimator->StartOptimization();

    EstimatorType::ParametersType estimatedMeans = estimator->GetParameters();
    message.clear();
    message << "Optimization completed." << std::endl;
    message << std::endl;
    message << "Estimated centroids are: " << std::endl;

    for (unsigned int i = 0; i < nb_classes; ++i)
      {
      message << "Class " << i << ": ";
      for (unsigned int j = 0; j < sampleSize; ++j)
        {
        message << estimatedMeans[i * 108 + j] << "\t";
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
    message.clear();
    message << "-- CLASSIFICATION --" << std::endl;
    message << std::endl;
    GetLogger()->Info(message.str());

    m_Classifier = ClassificationFilterType::New();

    m_Classifier->SetInput(m_InImage);
    m_Classifier->SetInputMask(maskImage);

    m_Classifier->SetCentroids(estimator->GetParameters());

    SetParameterOutputImage<UInt8ImageType> ("out", m_Classifier->GetOutput());

  }

  ClassificationFilterType::Pointer m_Classifier;
  FloatVectorImageType::Pointer m_InImage;


};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::KMeansClassification)



