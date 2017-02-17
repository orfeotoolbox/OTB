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
#include "otbTrainVectorBase.h"

// Validation
#include "otbConfusionMatrixCalculator.h"

namespace otb
{
namespace Wrapper
{

class TrainVectorClassifier : public TrainVectorBase
{
public:
  typedef TrainVectorClassifier Self;
  typedef TrainVectorBase Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  itkNewMacro( Self )

  itkTypeMacro( Self, Superclass )

  typedef Superclass::SampleType SampleType;
  typedef Superclass::ListSampleType ListSampleType;
  typedef Superclass::TargetListSampleType TargetListSampleType;

  // Estimate performance on validation sample
  typedef otb::ConfusionMatrixCalculator<TargetListSampleType, TargetListSampleType> ConfusionMatrixCalculatorType;
  typedef ConfusionMatrixCalculatorType::ConfusionMatrixType ConfusionMatrixType;
  typedef ConfusionMatrixCalculatorType::MapOfIndicesType MapOfIndicesType;
  typedef ConfusionMatrixCalculatorType::ClassLabelType ClassLabelType;

protected :
  TrainVectorClassifier() : TrainVectorBase()
  {
    m_ClassifierCategory = Supervised;
  }

private:
  void DoTrainInit()
  {
    SetName( "TrainVectorClassifier" );
    SetDescription( "Train a classifier based on labeled geometries and a list of features to consider." );

    SetDocName( "Train Vector Classifier" );
    SetDocLongDescription( "This application trains a classifier based on "
                                   "labeled geometries and a list of features to consider for classification." );
    SetDocLimitations( " " );
    SetDocAuthors( "OTB Team" );
    SetDocSeeAlso( " " );

    // Add a new parameter to compute confusion matrix
    AddParameter( ParameterType_OutputFilename, "io.confmatout", "Output confusion matrix" );
    SetParameterDescription( "io.confmatout", "Output file containing the confusion matrix (.csv format)." );
    MandatoryOff( "io.confmatout" );

    // Doc example parameter settings
    SetDocExampleParameterValue( "io.vd", "vectorData.shp" );
    SetDocExampleParameterValue( "io.stats", "meanVar.xml" );
    SetDocExampleParameterValue( "io.out", "svmModel.svm" );
    SetDocExampleParameterValue( "feat", "perimeter  area  width" );
    SetDocExampleParameterValue( "cfield", "predicted" );

  }

  void DoTrainUpdateParameters()
  {
    // Nothing to do here
  }

  void DoBeforeTrainExecute()
  {
    // Enforce the need of class field name in supervised mode
    featuresInfo.SetClassFieldNames( GetChoiceNames( "cfield" ), GetSelectedItems( "cfield" ) );

    if( featuresInfo.m_SelectedCFieldIdx.empty() && m_ClassifierCategory == Supervised )
      {
      otbAppLogFATAL( << "No field has been selected for data labelling!" );
      }
  }

  void DoAfterTrainExecute()
  {
    ConfusionMatrixCalculatorType::Pointer confMatCalc = ComputeConfusionmatrix( predictedList,
                                                                                 classificationListSamples.labeledListSample );
    WriteConfusionMatrix( confMatCalc );
  }


  ListSamples ExtractClassificationListSamples(const StatisticsMeasurement &measurement)
  {
    ListSamples performanceSample;
    ListSamples validationListSamples = ExtractListSamples( "valid.vd", "valid.layer", measurement );
    //Test the input validation set size
    if( validationListSamples.labeledListSample->Size() != 0 )
      {
      performanceSample.listSample = validationListSamples.listSample;
      performanceSample.labeledListSample = validationListSamples.labeledListSample;
      }
    else
      {
      otbAppLogWARNING(
              "The validation set is empty. The performance estimation is done using the input training set in this case." );
      performanceSample.listSample = trainingListSamples.listSample;
      performanceSample.labeledListSample = trainingListSamples.labeledListSample;
      }

    return performanceSample;
  }


  ConfusionMatrixCalculatorType::Pointer
  ComputeConfusionmatrix(const TargetListSampleType::Pointer &predictedListSample,
                         const TargetListSampleType::Pointer &performanceLabeledListSample)
  {
    ConfusionMatrixCalculatorType::Pointer confMatCalc = ConfusionMatrixCalculatorType::New();

    otbAppLogINFO( "Predicted list size : " << predictedListSample->Size() );
    otbAppLogINFO( "ValidationLabeledListSample size : " << performanceLabeledListSample->Size() );
    confMatCalc->SetReferenceLabels( performanceLabeledListSample );
    confMatCalc->SetProducedLabels( predictedListSample );
    confMatCalc->Compute();

    otbAppLogINFO( "training performances" );
    LogConfusionMatrix( confMatCalc );

    for( unsigned int itClasses = 0; itClasses < confMatCalc->GetNumberOfClasses(); itClasses++ )
      {
      ConfusionMatrixCalculatorType::ClassLabelType classLabel = confMatCalc->GetMapOfIndices()[itClasses];

      otbAppLogINFO( "Precision of class [" << classLabel << "] vs all: " << confMatCalc->GetPrecisions()[itClasses] );
      otbAppLogINFO( "Recall of class    [" << classLabel << "] vs all: " << confMatCalc->GetRecalls()[itClasses] );
      otbAppLogINFO(
              "F-score of class   [" << classLabel << "] vs all: " << confMatCalc->GetFScores()[itClasses] << "\n" );
      }
    otbAppLogINFO( "Global performance, Kappa index: " << confMatCalc->GetKappaIndex() );
    return confMatCalc;
  }

  /**
   * Write the confidence matrix into a file if output is provided.
   * \param confMatCalc the input matrix to write.
   */
  void WriteConfusionMatrix(const ConfusionMatrixCalculatorType::Pointer &confMatCalc)
  {
    if( this->HasValue( "io.confmatout" ) )
      {
      // Writing the confusion matrix in the output .CSV file

      MapOfIndicesType::iterator itMapOfIndicesValid, itMapOfIndicesPred;
      ClassLabelType labelValid = 0;

      ConfusionMatrixType confusionMatrix = confMatCalc->GetConfusionMatrix();
      MapOfIndicesType mapOfIndicesValid = confMatCalc->GetMapOfIndices();

      unsigned long nbClassesPred = mapOfIndicesValid.size();

      /////////////////////////////////////////////
      // Filling the 2 headers for the output file
      const std::string commentValidStr = "#Reference labels (rows):";
      const std::string commentPredStr = "#Produced labels (columns):";
      const char separatorChar = ',';
      std::ostringstream ossHeaderValidLabels, ossHeaderPredLabels;

      // Filling ossHeaderValidLabels and ossHeaderPredLabels for the output file
      ossHeaderValidLabels << commentValidStr;
      ossHeaderPredLabels << commentPredStr;

      itMapOfIndicesValid = mapOfIndicesValid.begin();

      while( itMapOfIndicesValid != mapOfIndicesValid.end() )
        {
        // labels labelValid of mapOfIndicesValid are already sorted in otbConfusionMatrixCalculator
        labelValid = itMapOfIndicesValid->second;

        otbAppLogINFO( "mapOfIndicesValid[" << itMapOfIndicesValid->first << "] = " << labelValid );

        ossHeaderValidLabels << labelValid;
        ossHeaderPredLabels << labelValid;

        ++itMapOfIndicesValid;

        if( itMapOfIndicesValid != mapOfIndicesValid.end() )
          {
          ossHeaderValidLabels << separatorChar;
          ossHeaderPredLabels << separatorChar;
          }
        else
          {
          ossHeaderValidLabels << std::endl;
          ossHeaderPredLabels << std::endl;
          }
        }

      std::ofstream outFile;
      outFile.open( this->GetParameterString( "io.confmatout" ).c_str() );
      outFile << std::fixed;
      outFile.precision( 10 );

      /////////////////////////////////////
      // Writing the 2 headers
      outFile << ossHeaderValidLabels.str();
      outFile << ossHeaderPredLabels.str();
      /////////////////////////////////////

      unsigned int indexLabelValid = 0, indexLabelPred = 0;

      for( itMapOfIndicesValid = mapOfIndicesValid.begin();
           itMapOfIndicesValid != mapOfIndicesValid.end(); ++itMapOfIndicesValid )
        {
        indexLabelPred = 0;

        for( itMapOfIndicesPred = mapOfIndicesValid.begin();
             itMapOfIndicesPred != mapOfIndicesValid.end(); ++itMapOfIndicesPred )
          {
          // Writing the confusion matrix (sorted in otbConfusionMatrixCalculator) in the output file
          outFile << confusionMatrix( indexLabelValid, indexLabelPred );
          if( indexLabelPred < ( nbClassesPred - 1 ) )
            {
            outFile << separatorChar;
            }
          else
            {
            outFile << std::endl;
            }
          ++indexLabelPred;
          }

        ++indexLabelValid;
        }

      outFile.close();
      }
  }

  /**
   * Display the log of the confusion matrix computed with
   * \param confMatCalc the input confusion matrix to display
   */
  void LogConfusionMatrix(ConfusionMatrixCalculatorType *confMatCalc)
  {
    ConfusionMatrixCalculatorType::ConfusionMatrixType matrix = confMatCalc->GetConfusionMatrix();

    // Compute minimal width
    size_t minwidth = 0;

    for( unsigned int i = 0; i < matrix.Rows(); i++ )
      {
      for( unsigned int j = 0; j < matrix.Cols(); j++ )
        {
        std::ostringstream os;
        os << matrix( i, j );
        size_t size = os.str().size();

        if( size > minwidth )
          {
          minwidth = size;
          }
        }
      }

    MapOfIndicesType mapOfIndices = confMatCalc->GetMapOfIndices();

    MapOfIndicesType::const_iterator it = mapOfIndices.begin();
    MapOfIndicesType::const_iterator end = mapOfIndices.end();

    for( ; it != end; ++it )
      {
      std::ostringstream os;
      os << "[" << it->second << "]";

      size_t size = os.str().size();
      if( size > minwidth )
        {
        minwidth = size;
        }
      }

    // Generate matrix string, with 'minwidth' as size specifier
    std::ostringstream os;

    // Header line
    for( size_t i = 0; i < minwidth; ++i )
      os << " ";
    os << " ";

    it = mapOfIndices.begin();
    end = mapOfIndices.end();
    for( ; it != end; ++it )
      {
      os << "[" << it->second << "]" << " ";
      }

    os << std::endl;

    // Each line of confusion matrix
    for( unsigned int i = 0; i < matrix.Rows(); i++ )
      {
      ConfusionMatrixCalculatorType::ClassLabelType label = mapOfIndices[i];
      os << "[" << std::setw( minwidth - 2 ) << label << "]" << " ";
      for( unsigned int j = 0; j < matrix.Cols(); j++ )
        {
        os << std::setw( minwidth ) << matrix( i, j ) << " ";
        }
      os << std::endl;
      }

    otbAppLogINFO( "Confusion matrix (rows = reference labels, columns = produced labels):\n" << os.str() );
  }

};
}
}

OTB_APPLICATION_EXPORT( otb::Wrapper::TrainVectorClassifier )
