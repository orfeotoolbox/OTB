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
#include "otbLearningApplicationBase.h"
#include "otbWrapperApplicationFactory.h"

#include "otbListSampleGenerator.h"

#include "otbImageToEnvelopeVectorDataFilter.h"
#include "itkPreOrderTreeIterator.h"

// Statistic XML Reader
#include "otbStatisticsXMLFileReader.h"

#include "itkTimeProbe.h"
#include "otbStandardFilterWatcher.h"

// Normalize the samples
#include "otbShiftScaleSampleListFilter.h"

// List sample concatenation
#include "otbConcatenateSampleListFilter.h"

// Balancing ListSample
#include "otbListSampleToBalancedListSampleFilter.h"

#include "itkMersenneTwisterRandomVariateGenerator.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"

namespace otb
{
namespace Wrapper
{

class TrainRegression: public LearningApplicationBase<float,float>
{
public:
  /** Standard class typedefs. */
  typedef TrainRegression Self;
  typedef LearningApplicationBase<float,float> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self)

  itkTypeMacro(TrainRegression, otb::Wrapper::LearningApplicationBase)

  typedef Superclass::SampleType              SampleType;
  typedef Superclass::ListSampleType          ListSampleType;
  typedef Superclass::TargetSampleType        TargetSampleType;
  typedef Superclass::TargetListSampleType    TargetListSampleType;

  typedef Superclass::SampleImageType         SampleImageType;
  typedef SampleImageType::PixelType          PixelType;

  // SampleList manipulation
  typedef otb::ListSampleGenerator<SampleImageType, VectorDataType> ListSampleGeneratorType;

  typedef otb::Statistics::ConcatenateSampleListFilter<ListSampleType> ConcatenateListSampleFilterType;
  typedef otb::Statistics::ConcatenateSampleListFilter<TargetListSampleType> ConcatenateLabelListSampleFilterType;

  // Statistic XML file Reader
  typedef otb::StatisticsXMLFileReader<SampleType> StatisticsReader;

  // Enhance List Sample  typedef otb::Statistics::ListSampleToBalancedListSampleFilter<ListSampleType, LabelListSampleType>      BalancingListSampleFilterType;
  typedef otb::Statistics::ShiftScaleSampleListFilter<ListSampleType, ListSampleType> ShiftScaleFilterType;

  typedef otb::ImageToEnvelopeVectorDataFilter<SampleImageType,VectorDataType> EnvelopeFilterType;

  typedef itk::PreOrderTreeIterator<VectorDataType::DataTreeType> TreeIteratorType;

  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator RandomGeneratorType;

protected:
  TrainRegression()
    {
    this->m_RegressionFlag = true;
    }

private:

void DoInit()
{
  SetName("TrainRegression");
  SetDescription(
    "Train a classifier from multiple images to perform regression.");

  // Documentation
  SetDocName("Train a regression model");
  SetDocLongDescription(
    "This application trains a classifier from multiple input images or a csv "
    "file, in order to perform regression. Predictors are composed of pixel "
    "values in each band optionally centered and reduced using an XML "
    "statistics file produced by the ComputeImagesStatistics application.\n "
    "The output value for each predictor is assumed to be the last band "
    "(or the last column for CSV files). Training and validation predictor "
    "lists are built such that their size is inferior to maximum bounds given "
    "by the user, and the proportion corresponds to the balance parameter. "
    "Several classifier parameters can be set depending on the chosen "
    "classifier. In the validation process, the mean square error is computed\n"
    " This application is based on LibSVM and on OpenCV Machine Learning "
    "classifiers, and is compatible with OpenCV 2.3.1 and later.");
  SetDocLimitations("None");
  SetDocAuthors("OTB-Team");
  SetDocSeeAlso("OpenCV documentation for machine learning http://docs.opencv.org/modules/ml/doc/ml.html ");

  //Group IO
  AddParameter(ParameterType_Group, "io", "Input and output data");
  SetParameterDescription("io", "This group of parameters allows setting input and output data.");
  AddParameter(ParameterType_InputImageList, "io.il", "Input Image List");
  SetParameterDescription("io.il", "A list of input images. First (n-1) bands should contain the predictor. The last band should contain the output value to predict.");
  AddParameter(ParameterType_InputFilename, "io.csv", "Input CSV file");
  SetParameterDescription("io.csv","Input CSV file containing the predictors, and the output values in last column. Only used when no input image is given");
  MandatoryOff("io.csv");

  AddParameter(ParameterType_InputFilename, "io.imstat", "Input XML image statistics file");
  MandatoryOff("io.imstat");
  SetParameterDescription("io.imstat",
                          "Input XML file containing the mean and the standard deviation of the input images.");
  AddParameter(ParameterType_OutputFilename, "io.out", "Output regression model");
  SetParameterDescription("io.out", "Output file containing the model estimated (.txt format).");

  AddParameter(ParameterType_Float,"io.mse","Mean Square Error");
  SetParameterDescription("io.mse","Mean square error computed with the validation predictors");
  SetParameterRole("io.mse",Role_Output);
  DisableParameter("io.mse");

  //Group Sample list
  AddParameter(ParameterType_Group, "sample", "Training and validation samples parameters");
  SetParameterDescription("sample",
                          "This group of parameters allows you to set training and validation sample lists parameters.");

  AddParameter(ParameterType_Int, "sample.mt", "Maximum training predictors");
  //MandatoryOff("mt");
  SetDefaultParameterInt("sample.mt", 1000);
  SetParameterDescription("sample.mt", "Maximum number of training predictors (default = 1000) (no limit = -1).");

  AddParameter(ParameterType_Int, "sample.mv", "Maximum validation predictors");
  // MandatoryOff("mv");
  SetDefaultParameterInt("sample.mv", 1000);
  SetParameterDescription("sample.mv", "Maximum number of validation predictors (default = 1000) (no limit = -1).");

  AddParameter(ParameterType_Float, "sample.vtr", "Training and validation sample ratio");
  SetParameterDescription("sample.vtr",
                          "Ratio between training and validation samples (0.0 = all training, 1.0 = all validation) (default = 0.5).");
  SetParameterFloat("sample.vtr", 0.5);

  Superclass::DoInit();

  AddRANDParameter();

  // Doc example parameter settings
  SetDocExampleParameterValue("io.il", "training_dataset.tif");
  SetDocExampleParameterValue("io.out", "regression_model.txt");
  SetDocExampleParameterValue("io.imstat", "training_statistics.xml");
  SetDocExampleParameterValue("classifier", "libsvm");
}

void DoUpdateParameters()
{
  if (HasValue("io.csv") && IsParameterEnabled("io.csv"))
    {
    MandatoryOff("io.il");
    }
  else
    {
    MandatoryOn("io.il");
    }
}

void ParseCSVPredictors(std::string path, ListSampleType* outputList)
{
  std::ifstream ifs;
  ifs.open(path.c_str());
  unsigned int nbCols = 0;
  char sep = '\t';
  std::istringstream iss;
  SampleType elem;
  while(!ifs.eof())
    {
    std::string line;
    std::getline(ifs,line);
    // filter current line
    while (!line.empty() && (line[0] == ' ' || line[0] == '\t'))
      {
      line.erase(line.begin());
      }
    while (!line.empty() && ( *(line.end()-1) == ' ' || *(line.end()-1) == '\t' || *(line.end()-1) == '\r'))
      {
      line.erase(line.end()-1);
      }

    // Avoid commented lines or too short ones
    if (!line.empty() && line[0] != '#')
      {
      std::vector<itksys::String> words = itksys::SystemTools::SplitString(line.c_str(),sep);
      if (nbCols == 0)
        {
        // detect separator and feature size
        if (words.size() < 2)
          {
          sep = ' ';
          words = itksys::SystemTools::SplitString(line.c_str(),sep);
          }
        if (words.size() < 2)
          {
          sep = ';';
          words = itksys::SystemTools::SplitString(line.c_str(),sep);
          }
        if (words.size() < 2)
          {
          sep = ',';
          words = itksys::SystemTools::SplitString(line.c_str(),sep);
          }
        if (words.size() < 2)
          {
          otbAppLogFATAL(<< "Can't parse CSV file : less than 2 columns or invalid separator (valid separators are tab, space, comma and semi-colon)");
          }
        nbCols = words.size();
        elem.SetSize(nbCols,false);
        outputList->SetMeasurementVectorSize(nbCols);
        }
      else if (words.size() != nbCols )
        {
        otbAppLogWARNING(<< "Skip CSV line, wrong number of columns : got "<<words.size() << ", expected "<<nbCols);
        continue;
        }
      elem.Fill(0.0);
      for (unsigned int i=0 ; i<nbCols ; ++i)
        {
        iss.str(words[i]);
        iss >> elem[i];
        }
      outputList->PushBack(elem);
      }
    }
  ifs.close();
}

void DoExecute()
{
  GetLogger()->Debug("Entering DoExecute\n");
  //Create training and validation for list samples and label list samples
  ConcatenateListSampleFilterType::Pointer concatenateTrainingSamples = ConcatenateListSampleFilterType::New();
  ConcatenateListSampleFilterType::Pointer concatenateValidationSamples = ConcatenateListSampleFilterType::New();

  SampleType meanMeasurementVector;
  SampleType stddevMeasurementVector;

  //--------------------------
  // Load measurements from images
  unsigned int nbBands = 0;
  unsigned int nbFeatures = 0;
  //Iterate over all input images

  FloatVectorImageListType* imageList = GetParameterImageList("io.il");

  //Iterate over all input images
  for (unsigned int imgIndex = 0; imgIndex < imageList->Size(); ++imgIndex)
    {
    FloatVectorImageType::Pointer image = imageList->GetNthElement(imgIndex);
    image->UpdateOutputInformation();

    if (imgIndex == 0)
      {
      nbBands = image->GetNumberOfComponentsPerPixel();
      nbFeatures = static_cast<unsigned int>(static_cast<int>(nbBands) - 1);
      if (nbBands < 2)
        {
        otbAppLogFATAL(<< "Need at least two bands per image, got "<<nbBands);
        }
      else
        {
        if (nbBands != image->GetNumberOfComponentsPerPixel())
          {
          otbAppLogFATAL(<< "Image has a different number of components than "
            "the first one, expected "<<nbBands<<", got "<< image->GetNumberOfComponentsPerPixel());
          }
        }
      }

    // Extract image envelope to feed in sampleGenerator
    EnvelopeFilterType::Pointer envelopeFilter = EnvelopeFilterType::New();
    envelopeFilter->SetInput(image);
    envelopeFilter->SetSamplingRate(0);
    if (!image->GetProjectionRef().empty())
      {
      envelopeFilter->SetOutputProjectionRef(image->GetProjectionRef());
      }

    // Setup the DEM Handler
    // otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"elev");

    envelopeFilter->Update();

    VectorDataType::Pointer envelope = envelopeFilter->GetOutput();

    TreeIteratorType itVector(envelope->GetDataTree());
    for (itVector.GoToBegin(); !itVector.IsAtEnd(); ++itVector)
      {
      if (itVector.Get()->IsPolygonFeature())
        {
        itVector.Get()->SetFieldAsInt(std::string("class"),1);
        }
      }


    //Sample list generator
    ListSampleGeneratorType::Pointer sampleGenerator = ListSampleGeneratorType::New();

    sampleGenerator->SetInput(image);
    sampleGenerator->SetInputVectorData(envelope);

    sampleGenerator->SetClassKey("class");
    sampleGenerator->SetMaxTrainingSize(GetParameterInt("sample.mt"));
    sampleGenerator->SetMaxValidationSize(GetParameterInt("sample.mv"));
    sampleGenerator->SetValidationTrainingProportion(GetParameterFloat("sample.vtr"));
    sampleGenerator->SetBoundByMin(false);
    sampleGenerator->SetPolygonEdgeInclusion(true);

    sampleGenerator->Update();

    //Concatenate training and validation samples from the image
    concatenateTrainingSamples->AddInput(sampleGenerator->GetTrainingListSample());
    concatenateValidationSamples->AddInput(sampleGenerator->GetValidationListSample());
    }

  // if no input image, try CSV
  if (imageList->Size() == 0)
    {
    if (HasValue("io.csv") && IsParameterEnabled("io.csv"))
      {
      ListSampleType::Pointer csvListSample = ListSampleType::New();
      this->ParseCSVPredictors(this->GetParameterString("io.csv"), csvListSample);
      unsigned int totalCSVSize = csvListSample->Size();
      if (totalCSVSize == 0)
        {
        otbAppLogFATAL("No input image and empty CSV file. Missing input data");
        }
      nbBands = csvListSample->GetMeasurementVectorSize();
      nbFeatures = static_cast<unsigned int>(static_cast<int>(nbBands) - 1);
      ListSampleType::Pointer csvTrainListSample = ListSampleType::New();
      ListSampleType::Pointer csvValidListSample = ListSampleType::New();
      csvTrainListSample->SetMeasurementVectorSize(nbBands);
      csvValidListSample->SetMeasurementVectorSize(nbBands);
      double ratio = this->GetParameterFloat("sample.vtr");
      int trainSize = static_cast<int>(static_cast<double>(totalCSVSize)*(1.0-ratio));
      int validSize = static_cast<int>(static_cast<double>(totalCSVSize)*(ratio));
      if (trainSize > this->GetParameterInt("sample.mt"))
        {
        trainSize = this->GetParameterInt("sample.mt");
        }
      if (validSize > this->GetParameterInt("sample.mv"))
        {
        validSize = this->GetParameterInt("sample.mv");
        }
      double probaTrain = static_cast<double>(trainSize)/static_cast<double>(totalCSVSize);
      double probaValid = static_cast<double>(validSize)/static_cast<double>(totalCSVSize);

      RandomGeneratorType::Pointer randomGenerator = RandomGeneratorType::GetInstance();
      for (unsigned int i=0; i<totalCSVSize; ++i)
        {
        double random = randomGenerator->GetUniformVariate(0.0, 1.0);
        if (random < probaTrain)
          {
          csvTrainListSample->PushBack(csvListSample->GetMeasurementVector(i));
          }
        else if (random < probaTrain + probaValid)
          {
          csvValidListSample->PushBack(csvListSample->GetMeasurementVector(i));
          }
        }
      concatenateTrainingSamples->AddInput(csvTrainListSample);
      concatenateValidationSamples->AddInput(csvValidListSample);
      }
    }

  // Update
  concatenateTrainingSamples->Update();
  concatenateValidationSamples->Update();

  if (concatenateTrainingSamples->GetOutput()->Size() == 0)
    {
    otbAppLogFATAL("No training samples, cannot perform training.");
    }

  if (concatenateValidationSamples->GetOutput()->Size() == 0)
    {
    otbAppLogWARNING("No validation samples.");
    }

  if (IsParameterEnabled("io.imstat"))
    {
    StatisticsReader::Pointer statisticsReader = StatisticsReader::New();
    statisticsReader->SetFileName(GetParameterString("io.imstat"));
    meanMeasurementVector = statisticsReader->GetStatisticVectorByName("mean");
    stddevMeasurementVector = statisticsReader->GetStatisticVectorByName("stddev");
    // handle stat file without output normalization
    if (meanMeasurementVector.Size() == nbFeatures)
      {
      meanMeasurementVector.SetSize(nbBands,false);
      meanMeasurementVector[nbFeatures] = 0.0;
      stddevMeasurementVector.SetSize(nbBands,false);
      stddevMeasurementVector[nbFeatures] = 1.0;
      }
    }
  else
    {
    meanMeasurementVector.SetSize(nbBands);
    meanMeasurementVector.Fill(0.);
    stddevMeasurementVector.SetSize(nbBands);
    stddevMeasurementVector.Fill(1.);
    }

  // Shift scale the samples
  ShiftScaleFilterType::Pointer trainingShiftScaleFilter = ShiftScaleFilterType::New();
  trainingShiftScaleFilter->SetInput(concatenateTrainingSamples->GetOutput());
  trainingShiftScaleFilter->SetShifts(meanMeasurementVector);
  trainingShiftScaleFilter->SetScales(stddevMeasurementVector);
  trainingShiftScaleFilter->Update();

  ListSampleType::Pointer rawValidationListSample=ListSampleType::New();

  //Test if the validation test is empty
  if ( concatenateValidationSamples->GetOutput()->Size() != 0 )
    {
    ShiftScaleFilterType::Pointer validationShiftScaleFilter = ShiftScaleFilterType::New();
    validationShiftScaleFilter->SetInput(concatenateValidationSamples->GetOutput());
    validationShiftScaleFilter->SetShifts(meanMeasurementVector);
    validationShiftScaleFilter->SetScales(stddevMeasurementVector);
    validationShiftScaleFilter->Update();
    rawValidationListSample = validationShiftScaleFilter->GetOutput();
    }

  // Split between predictors and output values
  ListSampleType::Pointer rawlistSample = trainingShiftScaleFilter->GetOutput();
  ListSampleType::Pointer listSample = ListSampleType::New();
  listSample->SetMeasurementVectorSize(nbFeatures);
  listSample->Resize(rawlistSample->Size());
  TargetListSampleType::Pointer labelListSample = TargetListSampleType::New();
  labelListSample->SetMeasurementVectorSize(1);
  labelListSample->Resize(rawlistSample->Size());

  ListSampleType::Pointer validationListSample = ListSampleType::New();
  validationListSample->SetMeasurementVectorSize(nbFeatures);
  validationListSample->Resize(rawValidationListSample->Size());
  TargetListSampleType::Pointer validationLabeledListSample = TargetListSampleType::New();
  validationLabeledListSample->SetMeasurementVectorSize(1);
  validationLabeledListSample->Resize(rawValidationListSample->Size());

  ListSampleType::MeasurementVectorType elem;
  TargetListSampleType::MeasurementVectorType outElem;
  for (ListSampleType::InstanceIdentifier i=0; i<rawlistSample->Size() ; ++i)
    {
    elem = rawlistSample->GetMeasurementVector(i);
    outElem[0] = elem[nbFeatures];
    labelListSample->SetMeasurementVector(i,outElem);
    elem.SetSize(nbFeatures,false);
    listSample->SetMeasurementVector(i,elem);
    }
  for (ListSampleType::InstanceIdentifier i=0; i<rawValidationListSample->Size() ; ++i)
    {
    elem = rawValidationListSample->GetMeasurementVector(i);
    outElem[0] = elem[nbFeatures];
    validationLabeledListSample->SetMeasurementVector(i,outElem);
    elem.SetSize(nbFeatures,false);
    validationListSample->SetMeasurementVector(i,elem);
    }


  otbAppLogINFO("Number of training samples: " << concatenateTrainingSamples->GetOutput()->Size());
  //--------------------------
  // Split the data set into training/validation set
  ListSampleType::Pointer trainingListSample = listSample;
  TargetListSampleType::Pointer trainingLabeledListSample = labelListSample;

  otbAppLogINFO("Size of training set: " << trainingListSample->Size());
  otbAppLogINFO("Size of validation set: " << validationListSample->Size());

  //--------------------------
  // Estimate model
  //--------------------------
  this->Train(trainingListSample,trainingLabeledListSample,GetParameterString("io.out"));

  //--------------------------
  // Performances estimation
  //--------------------------
  ListSampleType::Pointer performanceListSample;
  TargetListSampleType::Pointer predictedList = TargetListSampleType::New();
  predictedList->SetMeasurementVectorSize(1);
  TargetListSampleType::Pointer performanceLabeledListSample;

  //Test the input validation set size
  if(validationLabeledListSample->Size() != 0)
    {
    performanceListSample = validationListSample;
    performanceLabeledListSample = validationLabeledListSample;
    }
  else
    {
    otbAppLogWARNING("The validation set is empty. The performance estimation is done using the input training set in this case.");
    performanceListSample = trainingListSample;
    performanceLabeledListSample = trainingLabeledListSample;
    }

  this->Classify(performanceListSample, predictedList, GetParameterString("io.out"));

  otbAppLogINFO("Training performances");
  double mse=0.0;
  TargetListSampleType::MeasurementVectorType predictedElem;
  for (TargetListSampleType::InstanceIdentifier i=0; i<performanceListSample->Size() ; ++i)
    {
    outElem = performanceLabeledListSample->GetMeasurementVector(i);
    predictedElem = predictedList->GetMeasurementVector(i);
    mse += (outElem[0] - predictedElem[0]) * (outElem[0] - predictedElem[0]);
    }
  mse /= static_cast<double>(performanceListSample->Size());
  otbAppLogINFO("Mean Square Error = "<<mse);
  this->SetParameterFloat("io.mse",mse);
}

};

} // end namespace Wrapper
} // end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::TrainRegression)
