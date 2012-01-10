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

#include "otbSOMMap.h"
#include "otbSOM.h"
#include "otbSOMImageClassificationFilter.h"
#include "itkEuclideanDistance.h"
#include "itkImageRegionSplitter.h"
#include "otbStreamingTraits.h"
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
  typedef itk::Statistics::EuclideanDistance<SampleType> DistanceType;
  typedef otb::SOMMap<SampleType, DistanceType, 2> SOMMapType;
  typedef itk::Statistics::ListSample<SampleType> ListSampleType;
  typedef otb::SOM<ListSampleType, SOMMapType> EstimatorType;

  typedef otb::StreamingTraits<FloatVectorImageType> StreamingTraitsType;
  typedef itk::ImageRegionSplitter<2>  SplitterType;
  typedef FloatVectorImageType::RegionType RegionType;

  typedef itk::ImageRegionConstIterator<FloatVectorImageType> IteratorType;
  typedef itk::ImageRegionConstIterator<LabeledImageType> LabeledIteratorType;
  typedef itk::ImageRegionConstIterator<SOMMapType> SOMIteratorType;

  typedef otb::SOMImageClassificationFilter
          <FloatVectorImageType, LabeledImageType, SOMMapType> ClassificationFilterType;

  
private:
  void DoInit()
  {
    SetName("SOMClassification");
    SetDescription("SOM image classification.");

    // Documentation
    SetDocName("SOM Classification");
    SetDocLongDescription("Unsupervised Self Organizing Map image classification.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Segmentation);
    AddDocTag(Tags::Learning);

    AddParameter(ParameterType_InputImage,  "in",   "InputImage");
    SetParameterDescription("in", "Input image.");
    
    AddParameter(ParameterType_OutputImage,  "out",   "OutputImage");
    SetParameterDescription("out", "Output classified image.");
    
    AddParameter(ParameterType_InputImage,  "vm",   "ValidityMask");
    SetParameterDescription("vm", "Validity mask");
    MandatoryOff("vm");
    
    AddParameter(ParameterType_Float, "tp", "TrainingProbability");
    SetParameterDescription("tp", "Probability for a sample to be selected in the training set");
    MandatoryOff("tp");
    
    AddParameter(ParameterType_Int,  "ts",   "TrainingSetSize");
    SetParameterDescription("ts", "Maximum training set size");
    MandatoryOff("ts");
    
    AddParameter(ParameterType_Int,  "sl",   "StreamingLines");
    SetParameterDescription("sl", "Number of lines in each streaming block (used during data sampling)");
    MandatoryOff("sl");
    
    AddParameter(ParameterType_OutputImage, "som", "SOM Map");
    SetParameterDescription("som","Self-Organizing Map map");
    MandatoryOff("som");
    
    AddParameter(ParameterType_Int,  "sx",   "SizeX");
    SetParameterDescription("sx", "X size of the SOM map");
    MandatoryOff("sx");
    
    AddParameter(ParameterType_Int,  "sy",   "SizeY");
    SetParameterDescription("sy", "Y size of the SOM map");
    MandatoryOff("sy");
    
    AddParameter(ParameterType_Int,  "nx",   "NeighborhoodX");
    SetParameterDescription("nx", "X initial neighborhood of the SOM map");
    MandatoryOff("nx");
    
    AddParameter(ParameterType_Int,  "ny",   "NeighborhoodY");
    SetParameterDescription("ny", "Y initial neighborhood of the SOM map");
    MandatoryOff("nx");
    
    AddParameter(ParameterType_Int,  "ni",   "NumberIteration");
    SetParameterDescription("ni", "Number of iterations of the SOM learning");
    MandatoryOff("ni");
    
    AddParameter(ParameterType_Float,  "bi",   "BetaInit");
    SetParameterDescription("bi", "Initial beta value");
    MandatoryOff("bi");
    
    AddParameter(ParameterType_Float,  "bf",   "BetaFinal");
    SetParameterDescription("bf", "Final beta value");
    MandatoryOff("bf");
    
    AddParameter(ParameterType_Float,  "iv",   "InitialValue");
    SetParameterDescription("iv", "Initial value (max weight)");
    MandatoryOff("iv");
    
    AddParameter(ParameterType_RAM, "ram", "Available RAM");
    SetDefaultParameterInt("ram", 256);
    MandatoryOff("ram");
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
    SetDocExampleParameterValue("sl", "32");
    SetDocExampleParameterValue("sx", "32");
    SetDocExampleParameterValue("sy", "32");
    SetDocExampleParameterValue("nx", "10");
    SetDocExampleParameterValue("ny", "10");
    SetDocExampleParameterValue("ni", "5");
    SetDocExampleParameterValue("bi", "1.0");
    SetDocExampleParameterValue("bf", "0.1");
    SetDocExampleParameterValue("iv", "0");
  }
  
  void DoUpdateParameters()
  {
    // Nothing to do
  }
  
  void DoExecute()
  {
    // initiating random number generation
    itk::Statistics::MersenneTwisterRandomVariateGenerator::Pointer
        randomGen = itk::Statistics::MersenneTwisterRandomVariateGenerator::New();
    
    FloatVectorImageType::Pointer input = GetParameterImage("in");
    LabeledImageType::Pointer mask;
    m_UseMask = false;
    if (HasValue("vm"))
    {
      mask = GetParameterImage<LabeledImageType>("vm");
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
    SplitterType::Pointer splitter = SplitterType::New();
    unsigned int numberOfStreamDivisions;
    if (HasValue("sl"))
    {
      numberOfStreamDivisions = StreamingTraitsType::CalculateNumberOfStreamDivisions(input,
                                          largestRegion,
                                          splitter,
                                          otb::SET_BUFFER_NUMBER_OF_LINES,
                                          0, 0, GetParameterInt("sl"));
    }
    else
    {
      numberOfStreamDivisions = StreamingTraitsType::CalculateNumberOfStreamDivisions(input,
                                          largestRegion,
                                          splitter,
                                          otb::SET_BUFFER_MEMORY_SIZE,
                                          0, 1048576*GetParameterInt("ram"), 0);
    }
    
    otbAppLogINFO("The images will be streamed into "<<numberOfStreamDivisions<<" parts.");
    
    // Training sample lists
    ListSampleType::Pointer sampleList = ListSampleType::New();
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
    
    // TODO : maybe change the approach: at the moment, the sampling process is able to pick a sample twice or more
    while (totalSamples < nbsamples)
    {
      unsigned int localNbSamples=0;
    
      piece = randPerm[index];
      streamingRegion = splitter->GetSplit(piece, numberOfStreamDivisions, largestRegion);
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
    
    m_Classifier = ClassificationFilterType::New();
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
