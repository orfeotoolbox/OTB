/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbSOMMap.h"
#include "otbSOM.h"
#include "otbSOMImageClassificationFilter.h"
#include "otbRAMDrivenAdaptativeStreamingManager.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRandomNonRepeatingConstIteratorWithIndex.h"

namespace otb
{
namespace Wrapper
{

class SOMClassification : public Application
{
public:
  /** Standard class typedefs. */
  typedef SOMClassification             Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(SOMClassification, otb::Application);

  /** Filters typedef */
  typedef UInt16ImageType LabeledImageType;

  typedef itk::VariableLengthVector<double> SampleType;
  typedef itk::Statistics::EuclideanDistanceMetric<SampleType> DistanceType;
  typedef otb::SOMMap<SampleType, DistanceType, 2> SOMMapType;
  typedef itk::Statistics::ListSample<SampleType> ListSampleType;
  typedef otb::SOM<ListSampleType, SOMMapType> EstimatorType;

  typedef RAMDrivenAdaptativeStreamingManager
    <FloatVectorImageType>                            RAMDrivenAdaptativeStreamingManagerType;

  typedef FloatVectorImageType::RegionType RegionType;

  typedef itk::ImageRegionConstIterator<FloatVectorImageType> IteratorType;
  typedef itk::ImageRegionConstIterator<LabeledImageType> LabeledIteratorType;
  typedef itk::ImageRegionConstIterator<SOMMapType> SOMIteratorType;

  typedef otb::SOMImageClassificationFilter
          <FloatVectorImageType, LabeledImageType, SOMMapType> ClassificationFilterType;


private:
  SOMClassification()
    {
    m_UseMask = false;
    m_Classifier = ClassificationFilterType::New();
    }

  void DoInit() override
  {
    SetName("SOMClassification");
    SetDescription("SOM image classification.");

    // Documentation
    SetDocName("SOM Classification");
    SetDocLongDescription("Unsupervised Self Organizing Map image classification.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

	AddDocTag(Tags::Learning);
    AddDocTag(Tags::Segmentation);

    AddParameter(ParameterType_InputImage,  "in",   "InputImage");
    SetParameterDescription("in", "Input image to classify.");

    AddParameter(ParameterType_OutputImage,  "out",   "OutputImage");
    SetParameterDescription("out", "Output classified image (each pixel contains the index of its corresponding vector in the SOM).");
    SetDefaultOutputPixelType("out",ImagePixelType_uint8);

    AddParameter(ParameterType_InputImage,  "vm",   "ValidityMask");
    SetParameterDescription("vm", "Validity mask (only pixels corresponding to a mask value greater than 0 will be used for learning)");
    MandatoryOff("vm");

    AddParameter(ParameterType_Float, "tp", "TrainingProbability");
    SetParameterDescription("tp", "Probability for a sample to be selected in the training set");
    MandatoryOff("tp");

    AddParameter(ParameterType_Int,  "ts",   "TrainingSetSize");
    SetParameterDescription("ts", "Maximum training set size (in pixels)");
    MandatoryOff("ts");

    AddParameter(ParameterType_OutputImage, "som", "SOM Map");
    SetParameterDescription("som","Output image containing the Self-Organizing Map");
    MandatoryOff("som");

    AddParameter(ParameterType_Int,  "sx",   "SizeX");
    SetParameterDescription("sx", "X size of the SOM map");
    MandatoryOff("sx");

    AddParameter(ParameterType_Int,  "sy",   "SizeY");
    SetParameterDescription("sy", "Y size of the SOM map");
    MandatoryOff("sy");

    AddParameter(ParameterType_Int,  "nx",   "NeighborhoodX");
    SetParameterDescription("nx", "X size of the initial neighborhood in the SOM map");
    MandatoryOff("nx");

    AddParameter(ParameterType_Int,  "ny",   "NeighborhoodY");
    SetParameterDescription("ny", "Y size of the initial neighborhood in the SOM map");
    MandatoryOff("nx");

    AddParameter(ParameterType_Int,  "ni",   "NumberIteration");
    SetParameterDescription("ni", "Number of iterations for SOM learning");
    MandatoryOff("ni");

    AddParameter(ParameterType_Float,  "bi",   "BetaInit");
    SetParameterDescription("bi", "Initial learning coefficient");
    MandatoryOff("bi");

    AddParameter(ParameterType_Float,  "bf",   "BetaFinal");
    SetParameterDescription("bf", "Final learning coefficient");
    MandatoryOff("bf");

    AddParameter(ParameterType_Float,  "iv",   "InitialValue");
    SetParameterDescription("iv", "Maximum initial neuron weight");
    MandatoryOff("iv");

    AddRANDParameter();

    AddRAMParameter();
    // TODO : replace StreamingLines by RAM param ?

    // Default parameters
    SetDefaultParameterFloat("tp", 1.0);
    SetDefaultParameterInt("sx", 32);
    SetDefaultParameterInt("sy", 32);
    SetDefaultParameterInt("nx", 10);
    SetDefaultParameterInt("ny", 10);
    SetDefaultParameterInt("ni", 5);
    SetDefaultParameterFloat("bi", 1.0);
    SetDefaultParameterFloat("bf", 0.1);
    SetDefaultParameterFloat("iv", 0.0);

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_1_ortho.tif");
    SetDocExampleParameterValue("out","SOMClassification.tif");
    SetDocExampleParameterValue("tp", "1.0");
    SetDocExampleParameterValue("ts","16384");
    SetDocExampleParameterValue("sx", "32");
    SetDocExampleParameterValue("sy", "32");
    SetDocExampleParameterValue("nx", "10");
    SetDocExampleParameterValue("ny", "10");
    SetDocExampleParameterValue("ni", "5");
    SetDocExampleParameterValue("bi", "1.0");
    SetDocExampleParameterValue("bf", "0.1");
    SetDocExampleParameterValue("iv", "0");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do
  }

  void DoExecute() override
  {
    // initiating random number generation
    itk::Statistics::MersenneTwisterRandomVariateGenerator::Pointer
        randomGen = itk::Statistics::MersenneTwisterRandomVariateGenerator::GetInstance();

    FloatVectorImageType::Pointer input = GetParameterImage("in");
    LabeledImageType::Pointer mask;
    m_UseMask = false;
    if (HasValue("vm"))
    {
      mask = GetParameterUInt16Image("vm");
      if (input->GetLargestPossibleRegion()
        != mask->GetLargestPossibleRegion())
        {
        otbAppLogFATAL("Mask image and input image have different sizes.");
        }
      m_UseMask = true;
    }

    /*******************************************/
    /*           Sampling data                 */
    /*******************************************/
    otbAppLogINFO("-- SAMPLING DATA --");

    RegionType largestRegion = input->GetLargestPossibleRegion();

    // Setting up local streaming capabilities
    
    RAMDrivenAdaptativeStreamingManagerType::Pointer
        streamingManager = RAMDrivenAdaptativeStreamingManagerType::New();
    int availableRAM = GetParameterInt("ram");
    streamingManager->SetAvailableRAMInMB(availableRAM);
    float bias = 2.0; // empiric value;
    streamingManager->SetBias(bias);
    
    streamingManager->PrepareStreaming(input, largestRegion);
      
    unsigned long numberOfStreamDivisions = streamingManager->GetNumberOfSplits();
    

    otbAppLogINFO("The images will be streamed into "<<numberOfStreamDivisions<<" parts.");

    // Training sample lists
    ListSampleType::Pointer sampleList = ListSampleType::New();
    sampleList->SetMeasurementVectorSize(input->GetNumberOfComponentsPerPixel());

    const double trainingProb = static_cast<double>(GetParameterFloat("tp"));
    unsigned int nbsamples;
    if (HasValue("ts"))
    {
      nbsamples = GetParameterInt("ts");
    }
    else
    {
      nbsamples = largestRegion.GetNumberOfPixels();
    }

    // Sample dimension and max dimension
    unsigned int sampleSize = input->GetNumberOfComponentsPerPixel();
    unsigned int totalSamples = 0;
    otbAppLogINFO("The following sample size will be used: "<<sampleSize);

    // local streaming variables
    unsigned int piece = 0;
    RegionType streamingRegion;

    // create a random permutation to explore
    itk::RandomPermutation randPerm(numberOfStreamDivisions);
    unsigned int index = 0;

    // reset seed and step once (itk::RandomPermutation may have used it)
    randomGen->SetSeed(GetParameterInt("rand"));
    randomGen->GetVariateWithClosedRange();

    // TODO : maybe change the approach: at the moment, the sampling process is able to pick a sample twice or more
    while (totalSamples < nbsamples)
    {
      unsigned int localNbSamples=0;

      piece = randPerm[index];
      streamingRegion = streamingManager->GetSplit(piece);
      //otbAppLogINFO("Processing region: "<<streamingRegion);

      input->SetRequestedRegion(streamingRegion);
      input->PropagateRequestedRegion();
      input->UpdateOutputData();

      IteratorType it(input, streamingRegion);
      it.GoToBegin();

      if (m_UseMask)
      {
        mask->SetRequestedRegion(streamingRegion);
        mask->PropagateRequestedRegion();
        mask->UpdateOutputData();

        LabeledIteratorType maskIt(mask, streamingRegion);
        maskIt.GoToBegin();

        // Loop on the image and the mask
        while ( !it.IsAtEnd()
            && !maskIt.IsAtEnd()
            && (totalSamples<nbsamples))
        {
          // If the current pixel is labeled
          if (maskIt.Get()>0)
          {
            if (randomGen->GetVariateWithClosedRange() < trainingProb)
            {
              SampleType newSample;
              newSample.SetSize(sampleSize);
              // build the sample
              newSample.Fill(0);
              for (unsigned int i = 0; i<sampleSize; ++i)
              {
                newSample[i]=it.Get()[i];
              }
              // Update the sample lists
              sampleList->PushBack(newSample);
              ++totalSamples;
              ++localNbSamples;
            }
          }
          ++it;
          ++maskIt;
        }
        index++;
        // we could break out of the while loop here, once the entire image has been streamed once
        if (index == numberOfStreamDivisions) index = 0;
      }
      else
      {
        // Loop on the image
        while ( !it.IsAtEnd()
            && (totalSamples<nbsamples))
        {
          if (randomGen->GetVariateWithClosedRange() < trainingProb)
          {
            SampleType newSample;
            newSample.SetSize(sampleSize);
            // build the sample
            newSample.Fill(0);
            for (unsigned int i = 0; i<sampleSize; ++i)
            {
              newSample[i]=it.Get()[i];
            }
            // Update the sample lists
            sampleList->PushBack(newSample);
            ++totalSamples;
            ++localNbSamples;
          }
          ++it;
        }
        index++;
        // we could break out of the while loop here, once the entire image has been streamed once
        if (index == numberOfStreamDivisions) index = 0;
      }
    }

    otbAppLogINFO("The final training set contains "<<totalSamples<<" samples.");


      /*******************************************/
      /*           Learning                      */
      /*******************************************/
      otbAppLogINFO("-- LEARNING --");

      EstimatorType::Pointer estimator = EstimatorType::New();

      estimator->SetListSample(sampleList);
      EstimatorType::SizeType size;
      size[0]=GetParameterInt("sx");
      size[1]=GetParameterInt("sy");
      estimator->SetMapSize(size);
      EstimatorType::SizeType radius;
      radius[0] = GetParameterInt("nx");
      radius[1] = GetParameterInt("ny");
      estimator->SetNeighborhoodSizeInit(radius);
      estimator->SetNumberOfIterations(GetParameterInt("ni"));
      estimator->SetBetaInit(GetParameterFloat("bi"));
      estimator->SetBetaEnd(GetParameterFloat("bf"));
      estimator->SetMaxWeight(GetParameterFloat("iv"));

    AddProcess(estimator,"Learning");
    estimator->Update();

    m_SOMMap = estimator->GetOutput();
    if (HasValue("som"))
      {
      otbAppLogINFO("-- Using Leaning image --");
      SetParameterOutputImage<DoubleVectorImageType>("som", m_SOMMap);
      }

    /*******************************************/
    /*           Classification                */
    /*******************************************/
    otbAppLogINFO("-- CLASSIFICATION --");

    m_Classifier->SetInput(input);
    m_Classifier->SetMap(m_SOMMap);
    if (m_UseMask) m_Classifier->SetInputMask(mask);

    AddProcess(m_Classifier,"Classification");

    SetParameterOutputImage<LabeledImageType>("out", m_Classifier->GetOutput());
  }

  bool m_UseMask;
  SOMMapType::Pointer m_SOMMap;
  ClassificationFilterType::Pointer m_Classifier;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::SOMClassification)
