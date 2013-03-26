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

#include <fstream>

//Image
#include "otbListSampleGenerator.h"


// ListSample
#include "itkListSample.h"
#include "itkVariableLengthVector.h"
#include "itkFixedArray.h"

// SVM estimator
#include "otbSVMSampleListModelEstimator.h"

// Statistic XML Reader
#include "otbStatisticsXMLFileReader.h"

// Validation
#include "otbSVMClassifier.h"
#include "otbConfusionMatrixCalculator.h"

// Normalize the samples
#include "otbShiftScaleSampleListFilter.h"

// List sample concatenation
#include "otbConcatenateSampleListFilter.h"

// Classification filter
#include "otbSVMImageClassificationFilter.h"

// Extract a ROI of the vectordata
#include "otbVectorDataIntoImageProjectionFilter.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"

namespace otb
{
namespace Wrapper
{

class ValidateSVMImagesClassifier: public Application
{

public:
  /** Standard class typedefs. */
  typedef ValidateSVMImagesClassifier Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ValidateSVMImagesClassifier, otb::Application);

  typedef otb::Image<FloatVectorImageType::InternalPixelType, 2> ImageReaderType;

  typedef FloatVectorImageType::PixelType PixelType;
  typedef FloatVectorImageType VectorImageType;
  typedef FloatImageType ImageType;
  typedef Int32ImageType LabeledImageType;

  // Training vectordata
  typedef itk::VariableLengthVector<ImageType::PixelType> MeasurementType;

  // SampleList manipulation
  typedef otb::ListSampleGenerator<VectorImageType, VectorDataType> ListSampleGeneratorType;

  typedef ListSampleGeneratorType::ListSampleType ListSampleType;
  typedef ListSampleGeneratorType::LabelType LabelType;
  typedef ListSampleGeneratorType::ListLabelType LabelListSampleType;
  typedef otb::Statistics::ConcatenateSampleListFilter<ListSampleType> ConcatenateListSampleFilterType;
  typedef otb::Statistics::ConcatenateSampleListFilter<LabelListSampleType> ConcatenateLabelListSampleFilterType;

  // Statistic XML file Reader
  typedef otb::StatisticsXMLFileReader<MeasurementType> StatisticsReader;
  // Enhance List Sample
  typedef otb::Statistics::ShiftScaleSampleListFilter<ListSampleType, ListSampleType> ShiftScaleFilterType;

  /// Classification typedefs
  typedef otb::SVMImageClassificationFilter<VectorImageType, LabeledImageType> ClassificationFilterType;
  typedef ClassificationFilterType::Pointer ClassificationFilterPointerType;
  typedef ClassificationFilterType::ModelType ModelType;
  typedef ModelType::Pointer ModelPointerType;

  typedef otb::Functor::VariableLengthVectorToMeasurementVectorFunctor<MeasurementType> MeasurementVectorFunctorType;
  typedef otb::SVMSampleListModelEstimator<ListSampleType, LabelListSampleType, MeasurementVectorFunctorType>
      SVMEstimatorType;
  // Estimate performance on validation sample
  typedef otb::SVMClassifier<ListSampleType, LabelType::ValueType> ClassifierType;
  typedef otb::ConfusionMatrixCalculator<LabelListSampleType, LabelListSampleType> ConfusionMatrixCalculatorType;
  typedef ClassifierType::OutputType ClassifierOutputType;

 // Extract ROI and Project vectorData
 typedef otb::VectorDataIntoImageProjectionFilter<VectorDataType, VectorImageType> VectorDataReprojectionType;

private:
  void DoInit()
  {
    SetName("ValidateSVMImagesClassifier");
    SetDescription("Estimate the performance of the SVM model with a set of images and validation samples.");

    SetDocName("Validate SVM Images Classifier");
    SetDocLongDescription("Estimate the performance of the SVM model obtained by the TrainSVMImagesClassifier with a new set of images and validation samples.\n The application asks for images statistics as input (XML file generated with the ComputeImagesStatistics application) and a SVM model (text file) generated with the TrainSVMImagesClassifier application.\n It will compute the global confusion matrix, kappa index and also the precision, recall and F-score of each class. In the validation process, the confusion matrix is organized the following way: rows = reference labels, columns = produced labels.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Learning);

    AddParameter(ParameterType_InputImageList, "il", "Input Image List");
    SetParameterDescription("il", "Input image list filename.");
    AddParameter(ParameterType_InputVectorDataList, "vd", "Vector Data List");
    SetParameterDescription("vd", "List of vector data to select validation samples.");
    AddParameter(ParameterType_InputFilename, "imstat", "XML image statistics file");
    MandatoryOff("imstat");
    SetParameterDescription("imstat", "Filename of an XML file containing mean and standard deviation of input images.");

    // Elevation
    ElevationParametersHandler::AddElevationParameters(this, "elev");

    AddParameter(ParameterType_OutputFilename, "out", "Output filename");
    SetParameterDescription("out", "Output file, which contains the performances of the SVM model.");
    MandatoryOff("out");
    AddParameter(ParameterType_InputFilename, "svm", "SVM validation filename");
    SetParameterDescription("svm",
                            "Input SVM model to validate (given by TrainSVMImagesClassification for instance).");

    AddRANDParameter();
   // Doc example parameter settings
   SetDocExampleParameterValue("il", "QB_1_ortho.tif");
   SetDocExampleParameterValue("vd", "VectorData_QB1.shp");
   SetDocExampleParameterValue("imstat", "EstimateImageStatisticsQB1.xml");
   SetDocExampleParameterValue("svm", "clsvmModelQB1.svm");
   SetDocExampleParameterValue("out", "PerformanceEstimationQB1.txt");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  std::string LogConfusionMatrix(ConfusionMatrixCalculatorType* confMatCalc)
  {
    ConfusionMatrixCalculatorType::ConfusionMatrixType matrix = confMatCalc->GetConfusionMatrix();

    // Compute minimal width
    size_t minwidth = 0;

    for (unsigned int i = 0; i < matrix.Rows(); i++)
      {
      for (unsigned int j = 0; j < matrix.Cols(); j++)
        {
        std::ostringstream os;
        os << matrix(i,j);
        size_t size = os.str().size();

        if (size > minwidth)
          {
          minwidth = size;
          }
        }
      }

    typedef std::map<int, ConfusionMatrixCalculatorType::ClassLabelType> MapOfIndicesType;
    MapOfIndicesType mapOfIndices = confMatCalc->GetMapOfIndices();

    MapOfIndicesType::const_iterator it = mapOfIndices.begin();
    MapOfIndicesType::const_iterator end = mapOfIndices.end();

    for(; it != end; ++it)
      {
      std::ostringstream os;
      os << "[" << it->second << "]";

      size_t size = os.str().size();
      if (size > minwidth)
        {
        minwidth = size;
        }
      }


    // Generate matrix string, with 'minwidth' as size specifier
    std::ostringstream os;

    // Header line
    for (size_t i = 0; i < minwidth; ++i)
      os << " ";
    os << " ";

    it = mapOfIndices.begin();
    end = mapOfIndices.end();
    for(; it != end; ++it)
      {
      os << "[" << it->second << "]" << " ";
      }

    os << std::endl;

    // Each line of confusion matrix
    for (unsigned int i = 0; i < matrix.Rows(); i++)
      {
      ConfusionMatrixCalculatorType::ClassLabelType label = mapOfIndices[i];
      os << "[" << std::setw(minwidth - 2) << label << "]" << " ";
      for (unsigned int j = 0; j < matrix.Cols(); j++)
        {
        os << std::setw(minwidth) << matrix(i,j) << " ";
        }
      os << std::endl;
      }

    otbAppLogINFO("Confusion matrix (rows = reference labels, columns = produced labels):\n" << os.str());
    return os.str();
  }

  void DoExecute()
  {
    GetLogger()->Debug("Entering DoExecute\n");
    //Create training and validation for list samples and label list samples
    ConcatenateLabelListSampleFilterType::Pointer
        concatenateTrainingLabels = ConcatenateLabelListSampleFilterType::New();
    ConcatenateListSampleFilterType::Pointer concatenateTrainingSamples = ConcatenateListSampleFilterType::New();
    ConcatenateLabelListSampleFilterType::Pointer
        concatenateValidationLabels = ConcatenateLabelListSampleFilterType::New();
    ConcatenateListSampleFilterType::Pointer concatenateValidationSamples = ConcatenateListSampleFilterType::New();

    MeasurementType meanMeasurementVector;
    MeasurementType stddevMeasurementVector;

    //--------------------------
    // Load measurements from images
    unsigned int nbBands = 0;
    //Iterate over all input images

    FloatVectorImageListType* imageList = GetParameterImageList("il");
    VectorDataListType* vectorDataList = GetParameterVectorDataList("vd");

    //Iterate over all input images
    for (unsigned int imgIndex = 0; imgIndex < imageList->Size(); ++imgIndex)
      {
      FloatVectorImageType::Pointer image = imageList->GetNthElement(imgIndex);
      image->UpdateOutputInformation();

      if (imgIndex == 0)
        {
        nbBands = image->GetNumberOfComponentsPerPixel();
        }

      // read the Vectordata
      VectorDataType::Pointer vectorData = vectorDataList->GetNthElement(imgIndex);
      vectorData->Update();

      VectorDataReprojectionType::Pointer vdreproj = VectorDataReprojectionType::New();
      vdreproj->SetInputImage(image);
      vdreproj->SetInput(vectorData);
      vdreproj->SetUseOutputSpacingAndOriginFromImage(false);

      // Setup the DEM Handler
      otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"elev");
      vdreproj->Update();

      //Sample list generator
      ListSampleGeneratorType::Pointer sampleGenerator = ListSampleGeneratorType::New();

      //Set inputs of the sample generator
      //TODO the ListSampleGenerator perform UpdateOutputData over the input image (need a persistent implementation)
      sampleGenerator->SetInput(image);
      sampleGenerator->SetInputVectorData(vdreproj->GetOutput());
      sampleGenerator->SetValidationTrainingProportion(1.0); // All in validation

      sampleGenerator->SetClassKey("Class");

      sampleGenerator->Update();

      //Concatenate training and validation samples from the image
      concatenateValidationLabels->AddInput(sampleGenerator->GetValidationListLabel());
      concatenateValidationSamples->AddInput(sampleGenerator->GetValidationListSample());
      }

    // Update
    concatenateValidationSamples->Update();
    concatenateValidationLabels->Update();

    if (IsParameterEnabled("imstat"))
      {
      StatisticsReader::Pointer statisticsReader = StatisticsReader::New();
      statisticsReader->SetFileName(GetParameterString("imstat"));
      meanMeasurementVector = statisticsReader->GetStatisticVectorByName("mean");
      stddevMeasurementVector = statisticsReader->GetStatisticVectorByName("stddev");
      }
    else
      {
      meanMeasurementVector.SetSize(nbBands);
      meanMeasurementVector.Fill(0.);
      stddevMeasurementVector.SetSize(nbBands);
      stddevMeasurementVector.Fill(1.);
      }

    ShiftScaleFilterType::Pointer validationShiftScaleFilter = ShiftScaleFilterType::New();
    validationShiftScaleFilter->SetInput(concatenateValidationSamples->GetOutput());
    validationShiftScaleFilter->SetShifts(meanMeasurementVector);
    validationShiftScaleFilter->SetScales(stddevMeasurementVector);
    validationShiftScaleFilter->Update();
    //--------------------------
    // split the data set into training/validation set
    ListSampleType::Pointer validationListSample = validationShiftScaleFilter->GetOutputSampleList();
    LabelListSampleType::Pointer validationLabeledListSample = concatenateValidationLabels->GetOutputSampleList();

    otbAppLogINFO("Size of validation set: " << validationListSample->Size());
    otbAppLogINFO("Size of labeled validation set: " << validationLabeledListSample->Size());

    //--------------------------
    // Load svm model
    ModelPointerType modelSVM = ModelType::New();
    modelSVM->LoadModel(GetParameterString("svm").c_str());

    //--------------------------
    // Performances estimation
    ClassifierType::Pointer validationClassifier = ClassifierType::New();
    validationClassifier->SetSample(validationListSample);
    validationClassifier->SetNumberOfClasses(modelSVM->GetNumberOfClasses());
    validationClassifier->SetModel(modelSVM);
    validationClassifier->Update();

    // Estimate performances
    ClassifierOutputType::ConstIterator it = validationClassifier->GetOutput()->Begin();
    ClassifierOutputType::ConstIterator itEnd = validationClassifier->GetOutput()->End();

    LabelListSampleType::Pointer classifierListLabel = LabelListSampleType::New();

    while (it != itEnd)
      {
      // Due to a bug in SVMClassifier, outlier in one-class SVM are labeled with unsigned int max
      classifierListLabel->PushBack(
                                    it.GetClassLabel() == itk::NumericTraits<unsigned int>::max() ? 2
                                                                                                  : it.GetClassLabel());
      ++it;
      }

    ConfusionMatrixCalculatorType::Pointer confMatCalc = ConfusionMatrixCalculatorType::New();

    confMatCalc->SetReferenceLabels(validationLabeledListSample);
    confMatCalc->SetProducedLabels(classifierListLabel);

    confMatCalc->Compute();

    otbAppLogINFO("SVM training performances");
    std::string confMatString;
    confMatString = LogConfusionMatrix(confMatCalc);

    for (unsigned int itClasses = 0; itClasses < modelSVM->GetNumberOfClasses(); itClasses++)
      {
      otbAppLogINFO("Precision of class [" << itClasses << "] vs all: " << confMatCalc->GetPrecisions()[itClasses] << std::endl);
      otbAppLogINFO("Recall of class [" << itClasses << "] vs all: " << confMatCalc->GetRecalls()[itClasses] << std::endl);
      otbAppLogINFO("F-score of class [" << itClasses << "] vs all: " << confMatCalc->GetFScores()[itClasses] << "\n" << std::endl);
      }
    otbAppLogINFO("Global performance, Kappa index: " << confMatCalc->GetKappaIndex() << std::endl);
    otbAppLogINFO("Global performance, Overall accuracy: " << confMatCalc->GetOverallAccuracy() << std::endl);
    //--------------------------
    // Save output in a ascii file (if needed)
    if (IsParameterEnabled("out"))
      {
      std::ofstream file;
      file.open(GetParameterString("out").c_str());
      file << "Confusion matrix (rows = reference labels, columns = produced labels):\n" << std::endl;
      file << confMatString << std::endl;
      file << "Precision of the different class: " << confMatCalc->GetPrecisions() << std::endl;
      file << "Recall of the different class: " << confMatCalc->GetRecalls() << std::endl;
      file << "F-score of the different class: " << confMatCalc->GetFScores() << std::endl;
      file << "Kappa index: " << confMatCalc->GetKappaIndex() << std::endl;
      file << "Overall accuracy index: " << confMatCalc->GetOverallAccuracy() << std::endl;
      file.close();
      }
  }
};

} // end of namespace Wrapper
} // end of namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::ValidateSVMImagesClassifier)
