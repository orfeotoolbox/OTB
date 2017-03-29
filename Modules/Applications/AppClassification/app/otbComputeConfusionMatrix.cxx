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

#include "otbOGRDataSourceToLabelImageFilter.h"
#include "itkImageRegionConstIterator.h"

#include "otbRAMDrivenAdaptativeStreamingManager.h"

#include "otbConfusionMatrixMeasurements.h"
#include "otbContingencyTableCalculator.h"
#include "otbContingencyTable.h"

namespace otb
{
namespace Wrapper
{
/** Utility function to negate std::isalnum */
bool IsNotAlphaNum(char c)
  {
  return !std::isalnum(c);
  }

class ComputeConfusionMatrix : public Application
{
public:
  /** Standard class typedefs. */
  typedef ComputeConfusionMatrix Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ComputeConfusionMatrix, otb::Application);

  typedef itk::ImageRegionConstIterator<Int32ImageType> ImageIteratorType;

  typedef otb::OGRDataSourceToLabelImageFilter<Int32ImageType> RasterizeFilterType;
  
  typedef RAMDrivenAdaptativeStreamingManager
    <Int32ImageType>                            RAMDrivenAdaptativeStreamingManagerType;

  typedef Int32ImageType::RegionType RegionType;

  typedef int                                              ClassLabelType;
  typedef unsigned long                                    ConfusionMatrixEltType;
  typedef itk::VariableSizeMatrix<ConfusionMatrixEltType>  ConfusionMatrixType;

  typedef std::map<
                   ClassLabelType,
                   std::map<ClassLabelType, ConfusionMatrixEltType>
                  > OutputConfusionMatrixType;


  // filter type
  typedef otb::ConfusionMatrixMeasurements<ConfusionMatrixType, ClassLabelType> ConfusionMatrixMeasurementsType;
  typedef ConfusionMatrixMeasurementsType::MapOfClassesType                     MapOfClassesType;
  typedef ConfusionMatrixMeasurementsType::MeasurementType                      MeasurementType;

  typedef ContingencyTable<ClassLabelType>  ContingencyTableType;


private:
  Int32ImageType* input;
  Int32ImageType::Pointer reference;
  RAMDrivenAdaptativeStreamingManagerType::Pointer streamingManager;
  otb::ogr::DataSource::Pointer ogrRef;
  RasterizeFilterType::Pointer rasterizeReference = RasterizeFilterType::New();

  struct StreamingInitializationData
  {
    int nodata;
    unsigned long numberOfStreamDivisions;
  };


  void DoInit() ITK_OVERRIDE
  {
  SetName("ComputeConfusionMatrix");
  SetDescription("Computes the confusion matrix of a classification");

  // Documentation
  SetDocName("Confusion matrix Computation");
  SetDocLongDescription("This application computes the confusion matrix of a classification map relatively to a ground truth. "
      "This ground truth can be given as a raster or a vector data. Only reference and produced pixels with values different "
      "from NoData are handled in the calculation of the confusion matrix. The confusion matrix is organized the following way: "
      "rows = reference labels, columns = produced labels. In the header of the output file, the reference and produced class labels "
      "are ordered according to the rows/columns of the confusion matrix.");
  SetDocLimitations("None");
  SetDocAuthors("OTB-Team");
  SetDocSeeAlso(" ");

  AddDocTag(Tags::Learning);

  AddParameter(ParameterType_InputImage, "in", "Input Image");
  SetParameterDescription( "in", "The input classification image." );

  AddParameter(ParameterType_OutputFilename, "out", "Matrix output");
  SetParameterDescription("out", "Filename to store the output matrix (csv format)");

  AddParameter(ParameterType_Choice,"format","set the output format to contingency table or confusion matrix");
  SetParameterDescription("format","Choice of the output format as a contingency table for unsupervised algorithms"
          "or confusion matrix for supervised ones.");
  AddChoice("format.confusionmatrix","Choice of a confusion matrix as output.");
  AddChoice("format.contingencytable","Choice of a contingency table as output.");

  AddParameter(ParameterType_Choice,"ref","Ground truth");
  SetParameterDescription("ref","Choice of ground truth format");

  AddChoice("ref.raster","Ground truth as a raster image");

  AddChoice("ref.vector","Ground truth as a vector data file");

  AddParameter(ParameterType_InputImage,"ref.raster.in","Input reference image");
  SetParameterDescription("ref.raster.in","Input image containing the ground truth labels");

  AddParameter(ParameterType_InputFilename,"ref.vector.in","Input reference vector data");
  SetParameterDescription("ref.vector.in", "Input vector data of the ground truth");

  AddParameter(ParameterType_ListView,"ref.vector.field","Field name");
  SetParameterDescription("ref.vector.field","Field name containing the label values");
  SetListViewSingleSelectionMode("ref.vector.field",true);
  
  AddParameter(ParameterType_Int,"nodatalabel","Value for nodata pixels");
  SetParameterDescription("nodatalabel", "Label for the NoData class. Such input pixels will be discarded from the "
      "ground truth and from the input classification map. By default, 'nodatalabel = 0'.");
  SetDefaultParameterInt("nodatalabel",0);
  MandatoryOff("nodatalabel");
  DisableParameter("nodatalabel");

  AddRAMParameter();

  // Doc example parameter settings
  SetDocExampleParameterValue("in", "clLabeledImageQB1.tif");
  SetDocExampleParameterValue("out", "ConfusionMatrix.csv");
  SetDocExampleParameterValue("ref", "vector");
  SetDocExampleParameterValue("ref.vector.in","VectorData_QB1_bis.shp");
  SetDocExampleParameterValue("ref.vector.field","Class");
  SetDocExampleParameterValue("nodatalabel","255");
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
    if ( HasValue("ref.vector.in") )
      {
      std::string vectorFile = GetParameterString("ref.vector.in");
      ogr::DataSource::Pointer ogrDS =
        ogr::DataSource::New(vectorFile, ogr::DataSource::Modes::Read);
      ogr::Layer layer = ogrDS->GetLayer(0);
      ogr::Feature feature = layer.ogr().GetNextFeature();
      
      ClearChoices("ref.vector.field");
      
      for(int iField=0; iField<feature.ogr().GetFieldCount(); iField++)
        {
        std::string key, item = feature.ogr().GetFieldDefnRef(iField)->GetNameRef();
        key = item;
        std::string::iterator end = std::remove_if(key.begin(),key.end(),IsNotAlphaNum);
        std::transform(key.begin(), end, key.begin(), tolower);
        
        OGRFieldType fieldType = feature.ogr().GetFieldDefnRef(iField)->GetType();
        
        if(fieldType == OFTString || fieldType == OFTInteger || ogr::version_proxy::IsOFTInteger64(fieldType))
          {
          std::string tmpKey="ref.vector.field."+key.substr(0, end - key.begin());
          AddChoice(tmpKey,item);
          }
        }
      }    
  }

  void LogContingencyTable(const ContingencyTableType& contingencyTable)
  {
    otbAppLogINFO("Contingency table : :\n" << contingencyTable);
  }

  void writeContingencyTable(const ContingencyTableType& contingencyTable)
  {
    std::ofstream outFile;
    outFile.open( this->GetParameterString( "out" ).c_str() );
    outFile << contingencyTable.to_csv();
    outFile.close();
  }

  std::string LogConfusionMatrix(MapOfClassesType* mapOfClasses, ConfusionMatrixType* matrix)
    {
      // Compute minimal width
      size_t minwidth = 0;

      for (unsigned int i = 0; i < matrix->Rows(); i++)
        {
        for (unsigned int j = 0; j < matrix->Cols(); j++)
          {
          std::ostringstream os;
          os << (*matrix)(i,j);
          size_t size = os.str().size();

          if (size > minwidth)
            {
            minwidth = size;
            }
          }
        }

      MapOfClassesType::const_iterator it = mapOfClasses->begin();
      MapOfClassesType::const_iterator end = mapOfClasses->end();

      for(; it != end; ++it)
        {
        std::ostringstream os;
        os << "[" << it->first << "]";

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

      it = mapOfClasses->begin();
      end = mapOfClasses->end();
      for(; it != end; ++it)
        {
        //os << "[" << it->first << "]" << " ";
        os << "[" << std::setw(minwidth - 2) << it->first << "]" << " ";
        }

      os << std::endl;

      // Each line of confusion matrix
      it = mapOfClasses->begin();
      for (unsigned int i = 0; i < matrix->Rows(); i++)
        {
        ClassLabelType label = it->first;
        os << "[" << std::setw(minwidth - 2) << label << "]" << " ";
        for (unsigned int j = 0; j < matrix->Cols(); j++)
          {
          os << std::setw(minwidth) << (*matrix)(i,j) << " ";
          }
        os << std::endl;
        ++it;
        }

      otbAppLogINFO("Confusion matrix (rows = reference labels, columns = produced labels):\n" << os.str());
      return os.str();
    }


  StreamingInitializationData InitStreamingData()
  {

    StreamingInitializationData sid;

    input = this->GetParameterInt32Image("in");

    std::string field;

    rasterizeReference = RasterizeFilterType::New();
    sid.nodata = this->GetParameterInt("nodatalabel");

    if (GetParameterString("ref") == "raster")
      {
      reference = this->GetParameterInt32Image("ref.raster.in");
      }
    else
      {
      ogrRef = otb::ogr::DataSource::New(GetParameterString("ref.vector.in"), otb::ogr::DataSource::Modes::Read);

      // Get field name
      std::vector<int> selectedCFieldIdx = GetSelectedItems("ref.vector.field");

      if(selectedCFieldIdx.empty())
        {
        otbAppLogFATAL(<<"No field has been selected for data labelling!");
        }

      std::vector<std::string> cFieldNames = GetChoiceNames("ref.vector.field");
      field = cFieldNames[selectedCFieldIdx.front()];

      rasterizeReference->AddOGRDataSource(ogrRef);
      rasterizeReference->SetOutputParametersFromImage(input);
      rasterizeReference->SetBackgroundValue(sid.nodata);
      rasterizeReference->SetBurnAttribute(field.c_str());

      reference = rasterizeReference->GetOutput();
      reference->UpdateOutputInformation();
      }

    // Prepare local streaming


    streamingManager = RAMDrivenAdaptativeStreamingManagerType::New();
    int availableRAM = GetParameterInt("ram");
    streamingManager->SetAvailableRAMInMB( static_cast<unsigned int>( availableRAM ) );
    float bias = 2.0; // empiric value;
    streamingManager->SetBias(bias);

    streamingManager->PrepareStreaming(input, input->GetLargestPossibleRegion());

    sid.numberOfStreamDivisions = streamingManager->GetNumberOfSplits();

    otbAppLogINFO("Number of stream divisions : "<<sid.numberOfStreamDivisions);

    return sid;
  }

  void DoExecute() ITK_OVERRIDE
  {
    StreamingInitializationData sid = InitStreamingData();

    if(GetParameterString("format") == "contingencytable")
      {
      DoExecuteContingencyTable( sid );
      }
    else
      {
      DoExecuteConfusionMatrix( sid );
      }
  }

  void DoExecuteContingencyTable(const StreamingInitializationData& sid)
  {
    typedef ContingencyTableCalculator<ClassLabelType> ContingencyTableCalculatorType;
    ContingencyTableCalculatorType::Pointer calculator = ContingencyTableCalculatorType::New();

    for (unsigned int index = 0; index < sid.numberOfStreamDivisions; index++)
      {
      RegionType streamRegion = streamingManager->GetSplit( index );

      input->SetRequestedRegion( streamRegion );
      input->PropagateRequestedRegion();
      input->UpdateOutputData();

      reference->SetRequestedRegion( streamRegion );
      reference->PropagateRequestedRegion();
      reference->UpdateOutputData();

      ImageIteratorType itInput( input, streamRegion );
      itInput.GoToBegin();

      ImageIteratorType itRef( reference, streamRegion );
      itRef.GoToBegin();

      calculator->Compute( itRef, itInput );
      }

    ContingencyTableType contingencyTable = calculator->GetContingencyTable();
    LogContingencyTable(contingencyTable);
    writeContingencyTable(contingencyTable);
  }

  void DoExecuteConfusionMatrix(const StreamingInitializationData& sid)
  {

    // Extraction of the Class Labels from the Reference image/rasterized vector data + filling of m_Matrix
    MapOfClassesType  mapOfClassesRef, mapOfClassesProd;
    MapOfClassesType::iterator  itMapOfClassesRef, itMapOfClassesProd;
    ClassLabelType labelRef = 0, labelProd = 0;
    int itLabelRef = 0, itLabelProd = 0;

    for (unsigned int index = 0; index < sid.numberOfStreamDivisions; index++)
      {
      RegionType streamRegion = streamingManager->GetSplit(index);

      input->SetRequestedRegion(streamRegion);
      input->PropagateRequestedRegion();
      input->UpdateOutputData();

      reference->SetRequestedRegion(streamRegion);
      reference->PropagateRequestedRegion();
      reference->UpdateOutputData();

      ImageIteratorType itInput(input, streamRegion);
      itInput.GoToBegin();

      ImageIteratorType itRef(reference, streamRegion);
      itRef.GoToBegin();

      while (!itRef.IsAtEnd())
        {
        labelRef = static_cast<ClassLabelType> (itRef.Get());
        labelProd = static_cast<ClassLabelType> (itInput.Get());

        // Extraction of the reference/produced class labels
        if ((labelRef != sid.nodata) && (labelProd != sid.nodata))
          {
          // If the current labels have not been added to their respective mapOfClasses yet
          if (mapOfClassesRef.insert(MapOfClassesType::value_type(labelRef, itLabelRef)).second)
            {
            ++itLabelRef;
            }
          if (mapOfClassesProd.insert(MapOfClassesType::value_type(labelProd, itLabelProd)).second)
            {
            ++itLabelProd;
            }

          // Filling of m_Matrix
          m_Matrix[labelRef][labelProd]++;
          } // END if ((labelRef != nodata) && (labelProd != nodata))
        ++itRef;
        ++itInput;
        }
      } // END of for (unsigned int index = 0; index < numberOfStreamDivisions; index++)


    /////////////////////////////////////////////
    // Filling the 2 headers for the output file
    const std::string commentRefStr = "#Reference labels (rows):";
    const std::string commentProdStr = "#Produced labels (columns):";
    const char separatorChar = ',';
    std::ostringstream ossHeaderRefLabels, ossHeaderProdLabels;

    // Filling ossHeaderRefLabels for the output file
    ossHeaderRefLabels << commentRefStr;

    MapOfClassesType::iterator itMapOfClassesRefEnd = mapOfClassesRef.end();
    itMapOfClassesRef = mapOfClassesRef.begin();
    int indexLabelRef = 0;
    while (itMapOfClassesRef != itMapOfClassesRefEnd)
      {
      // labels labelRef of mapOfClassesRef are already sorted
      labelRef = itMapOfClassesRef->first;

      // SORTING the itMapOfClassesRef->second items of mapOfClassesRef
      mapOfClassesRef[labelRef] = indexLabelRef;
      otbAppLogINFO("mapOfClassesRef[" << labelRef << "] = " << mapOfClassesRef[labelRef]);

      ossHeaderRefLabels << labelRef;
      ++itMapOfClassesRef;
      if (itMapOfClassesRef != itMapOfClassesRefEnd)
        {
        ossHeaderRefLabels << separatorChar;
        }
      else
        {
        ossHeaderRefLabels << std::endl;
        }

      ++indexLabelRef;
      }

    // Filling ossHeaderProdLabels for the output file
    ossHeaderProdLabels << commentProdStr;
    MapOfClassesType::iterator itMapOfClassesProdEnd = mapOfClassesProd.end();
    itMapOfClassesProd = mapOfClassesProd.begin();
    int indexLabelProd = 0;
    while (itMapOfClassesProd != itMapOfClassesProdEnd)
      {
      // labels labelProd of mapOfClassesProd are already sorted
      labelProd = itMapOfClassesProd->first;

      // SORTING the itMapOfClassesProd->second items of mapOfClassesProd
      mapOfClassesProd[labelProd] = indexLabelProd;
      otbAppLogINFO("mapOfClassesProd[" << labelProd << "] = " << mapOfClassesProd[labelProd]);

      ossHeaderProdLabels << labelProd;
      ++itMapOfClassesProd;
      if (itMapOfClassesProd != itMapOfClassesProdEnd)
        {
        ossHeaderProdLabels << separatorChar;
        }
      else
        {
        ossHeaderProdLabels << std::endl;
        }

      ++indexLabelProd;
      }


    std::ofstream outFile;
    outFile.open(this->GetParameterString("out").c_str());
    outFile << std::fixed;
    outFile.precision(10);

    /////////////////////////////////////
    // Writing the 2 headers
    outFile << ossHeaderRefLabels.str();
    outFile << ossHeaderProdLabels.str();
    /////////////////////////////////////


    // Initialization of the Confusion Matrix for the application LOG and for measurements
    int nbClassesRef = static_cast<int>(mapOfClassesRef.size());
    int nbClassesProd = static_cast<int>(mapOfClassesProd.size());

    // Formatting m_MatrixLOG from m_Matrix in order to make m_MatrixLOG a square matrix
    // from the reference labels in mapOfClassesRef
    indexLabelRef = 0;
    int indexLabelProdInRef = 0;

    // Initialization of m_MatrixLOG
    m_MatrixLOG.SetSize(nbClassesRef, nbClassesRef);
    m_MatrixLOG.Fill(0);
    for (itMapOfClassesRef = mapOfClassesRef.begin(); itMapOfClassesRef != itMapOfClassesRefEnd; ++itMapOfClassesRef)
      {
      // labels labelRef of mapOfClassesRef are already sorted
      labelRef = itMapOfClassesRef->first;

      indexLabelProd = 0;
      for (itMapOfClassesProd = mapOfClassesProd.begin(); itMapOfClassesProd != itMapOfClassesProdEnd; ++itMapOfClassesProd)
        {
        // labels labelProd of mapOfClassesProd are already sorted
        labelProd = itMapOfClassesProd->first;

        // If labelProd is present in mapOfClassesRef
        if (mapOfClassesRef.count(labelProd) != 0)
          {
          // Index of labelProd in mapOfClassesRef; itMapOfClassesRef->second elements are now SORTED
          indexLabelProdInRef = mapOfClassesRef[labelProd];
          m_MatrixLOG(indexLabelRef, indexLabelProdInRef) = m_Matrix[labelRef][labelProd];
          }

        ///////////////////////////////////////////////////////////
        // Writing the ordered confusion matrix in the output file
        outFile << m_Matrix[labelRef][labelProd];
        if (indexLabelProd < (nbClassesProd - 1))
          {
          outFile << separatorChar;
          }
        else
          {
          outFile << std::endl;
          }
        ///////////////////////////////////////////////////////////
        ++indexLabelProd;
        }

      m_Matrix[labelRef].clear();
      ++indexLabelRef;
      }

    // m_Matrix is cleared in order to remove old results in case of successive runs of the GUI application
    m_Matrix.clear();
    outFile.close();

    otbAppLogINFO("Reference class labels ordered according to the rows of the output confusion matrix: " << ossHeaderRefLabels.str());
    otbAppLogINFO("Produced class labels ordered according to the columns of the output confusion matrix: " << ossHeaderProdLabels.str());
    //otbAppLogINFO("Output confusion matrix (rows = reference labels, columns = produced labels):\n" << m_MatrixLOG);

    LogConfusionMatrix(&mapOfClassesRef, &m_MatrixLOG);


    // Measurements of the Confusion Matrix parameters
    ConfusionMatrixMeasurementsType::Pointer confMatMeasurements = ConfusionMatrixMeasurementsType::New();

    confMatMeasurements->SetMapOfClasses(mapOfClassesRef);
    confMatMeasurements->SetConfusionMatrix(m_MatrixLOG);
    confMatMeasurements->Compute();

    for (itMapOfClassesRef = mapOfClassesRef.begin(); itMapOfClassesRef != itMapOfClassesRefEnd; ++itMapOfClassesRef)
      {
      labelRef = itMapOfClassesRef->first;
      indexLabelRef = itMapOfClassesRef->second;

      otbAppLogINFO("Precision of class [" << labelRef << "] vs all: " << confMatMeasurements->GetPrecisions()[indexLabelRef]);
      otbAppLogINFO("Recall of class [" << labelRef << "] vs all: " << confMatMeasurements->GetRecalls()[indexLabelRef]);
      otbAppLogINFO("F-score of class [" << labelRef << "] vs all: " << confMatMeasurements->GetFScores()[indexLabelRef] << std::endl);
      }

    otbAppLogINFO("Precision of the different classes: " << confMatMeasurements->GetPrecisions());
    otbAppLogINFO("Recall of the different classes: " << confMatMeasurements->GetRecalls());
    otbAppLogINFO("F-score of the different classes: " << confMatMeasurements->GetFScores() << std::endl);

    otbAppLogINFO("Kappa index: " << confMatMeasurements->GetKappaIndex());
    otbAppLogINFO("Overall accuracy index: " << confMatMeasurements->GetOverallAccuracy());

  }// END Execute()

  ConfusionMatrixType m_MatrixLOG;
  OutputConfusionMatrixType m_Matrix;
};

}
}
OTB_APPLICATION_EXPORT(otb::Wrapper::ComputeConfusionMatrix)
