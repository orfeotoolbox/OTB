/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#include "otbTrainVectorBase.h"

// Validation
#include "otbConfusionMatrixCalculator.h"
#include "otbContingencyTableCalculator.h"

namespace otb
{
namespace Wrapper
{

class TrainVectorClassifier : public TrainVectorBase<float, int>
{
public:
  typedef TrainVectorClassifier Self;
  typedef TrainVectorBase<float, int> Superclass;
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
  
  typedef ContingencyTable<ClassLabelType> ContingencyTableType;
  typedef ContingencyTableType::Pointer    ContingencyTablePointerType;

protected:
  void DoInit() override
  {
    SetName( "TrainVectorClassifier" );
    SetDescription( "Train a classifier based on labeled geometries and a "
      "list of features to consider." );

    SetDocLongDescription( "This application trains a classifier based on "
      "labeled geometries and a list of features to consider for "
      "classification.\nThis application is based on LibSVM, OpenCV Machine "
      "Learning (2.3.1 and later), and Shark ML The output of this application "
      "is a text model file, whose format corresponds to the ML model type "
      "chosen. There is no image nor vector data output.");
    SetDocLimitations("None");
    SetDocAuthors( "OTB Team" );
    SetDocSeeAlso( " " );

    SetOfficialDocLink();

    Superclass::DoInit();
    
    // Add a new parameter to compute confusion matrix / contingency table
    this->AddParameter( ParameterType_OutputFilename, "io.confmatout", 
      "Output confusion matrix or contingency table" );
    this->SetParameterDescription( "io.confmatout", 
      "Output file containing the confusion matrix or contingency table (.csv format)."
      "The contingency table is output when we unsupervised algorithms is used otherwise the confusion matrix is output." );
    this->MandatoryOff( "io.confmatout" );

  }

  void DoUpdateParameters() override
  {
    Superclass::DoUpdateParameters();
  }

  void DoExecute() override
  {
    m_FeaturesInfo.SetClassFieldNames( GetChoiceNames( "cfield" ), GetSelectedItems( "cfield" ) );

    if( m_FeaturesInfo.m_SelectedCFieldIdx.empty() && GetClassifierCategory() == Supervised )
      {
      otbAppLogFATAL( << "No field has been selected for data labelling!" );
      }

    Superclass::DoExecute();

    if (GetClassifierCategory() == Supervised)
      {
      ConfusionMatrixCalculatorType::Pointer confMatCalc = ComputeConfusionMatrix( m_PredictedList,
                                                                                   m_ClassificationSamplesWithLabel.labeledListSample );
      WriteConfusionMatrix( confMatCalc );
      }
    else
      {
      ContingencyTablePointerType table = ComputeContingencyTable( m_PredictedList,
                                                                   m_ClassificationSamplesWithLabel.labeledListSample );
      WriteContingencyTable( table );
      }
  }

  ContingencyTablePointerType ComputeContingencyTable(const TargetListSampleType::Pointer &predictedListSample,
                                                      const TargetListSampleType::Pointer &performanceLabeledListSample)
  {
    typedef ContingencyTableCalculator<ClassLabelType> ContigencyTableCalcutaltorType;

    ContigencyTableCalcutaltorType::Pointer contingencyTableCalculator = ContigencyTableCalcutaltorType::New();
    contingencyTableCalculator->Compute(performanceLabeledListSample->Begin(),
                                        performanceLabeledListSample->End(),predictedListSample->Begin(), predictedListSample->End());

    if(GetParameterInt("v"))
    {
      otbAppLogINFO( "Training performances:" );
      otbAppLogINFO(<<"Contingency table: reference labels (rows) vs. produced labels (cols)\n"
        <<contingencyTableCalculator->BuildContingencyTable());
    }

    return contingencyTableCalculator->BuildContingencyTable();
  }


  void WriteContingencyTable(const ContingencyTablePointerType& table)
  {
    if(IsParameterEnabled("io.confmatout"))
      {
      // Write contingency table
      std::ofstream outFile;
      outFile.open( this->GetParameterString( "io.confmatout" ) );
      outFile << table->ToCSV();
      }
  }


  ConfusionMatrixCalculatorType::Pointer
  ComputeConfusionMatrix(const TargetListSampleType::Pointer &predictedListSample,
                         const TargetListSampleType::Pointer &performanceLabeledListSample)
  {
    ConfusionMatrixCalculatorType::Pointer confMatCalc = ConfusionMatrixCalculatorType::New();

    otbAppLogINFO( "Predicted list size : " << predictedListSample->Size() );
    otbAppLogINFO( "ValidationLabeledListSample size : " << performanceLabeledListSample->Size() );
    confMatCalc->SetReferenceLabels( performanceLabeledListSample );
    confMatCalc->SetProducedLabels( predictedListSample );
    confMatCalc->Compute();

    otbAppLogINFO( "Training performances:" );
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
      outFile.open( this->GetParameterString( "io.confmatout" ) );
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
