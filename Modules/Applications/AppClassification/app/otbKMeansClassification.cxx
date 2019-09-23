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

#include "otbWrapperCompositeApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbOGRDataToSamplePositionFilter.h"

namespace otb
{
namespace Wrapper
{

class KMeansApplicationBase : public CompositeApplication
{
public:
  /** Standard class typedefs. */
  typedef KMeansApplicationBase         Self;
  typedef CompositeApplication          Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkTypeMacro(KMeansApplicationBase, Superclass)

      protected : void InitKMParams()
  {
    AddApplication("ImageEnvelope", "imgenvelop", "mean shift smoothing");
    AddApplication("PolygonClassStatistics", "polystats", "Polygon Class Statistics");
    AddApplication("SampleSelection", "select", "Sample selection");
    AddApplication("SampleExtraction", "extraction", "Sample extraction");

    AddApplication("TrainVectorClassifier", "training", "Model training");
    AddApplication("ComputeImagesStatistics", "imgstats", "Compute Images second order statistics");
    AddApplication("ImageClassifier", "classif", "Performs a classification of the input image");

    ShareParameter("in", "imgenvelop.in");
    ShareParameter("out", "classif.out");

    InitKMSampling();
    InitKMClassification();

    // init at the end cleanup
    AddParameter(ParameterType_Bool, "cleanup", "Clean-up of temporary files");
    SetParameterDescription("cleanup", "If activated, the application will try to clean all temporary files it created");
    SetParameterInt("cleanup", 1);
  }

  void InitKMSampling()
  {
    AddParameter(ParameterType_Int, "nc", "Number of classes");
    SetParameterDescription("nc", "Number of modes, which will be used to generate class membership.");
    SetDefaultParameterInt("nc", 5);

    AddParameter(ParameterType_Int, "ts", "Training set size");
    SetParameterDescription("ts", "Size of the training set (in pixels).");
    SetDefaultParameterInt("ts", 100);
    MandatoryOff("ts");

    AddParameter(ParameterType_Int, "maxit", "Maximum number of iterations");
    SetParameterDescription("maxit",
                            "Maximum number of iterations for the learning step."
                            " If this parameter is set to 0, the KMeans algorithm will not stop until convergence");
    SetDefaultParameterInt("maxit", 1000);
    MandatoryOff("maxit");

    AddParameter(ParameterType_Group, "centroids", "Centroids IO parameters");
    SetParameterDescription("centroids", "Group of parameters for centroids IO.");

    AddParameter(ParameterType_InputFilename, "centroids.in", "input centroids text file");
    SetParameterDescription("centroids.in",
                            "Input text file containing centroid positions used to initialize the algorithm. "
                            "Each centroid must be described by p parameters, p being the number of bands in "
                            "the input image, and the number of centroids must be equal to the number of classes "
                            "(one centroid per line with values separated by spaces).");
    MandatoryOff("centroids.in");

    ShareKMSamplingParameters();
    ConnectKMSamplingParams();
  }

  void InitKMClassification()
  {
    ShareKMClassificationParams();
    ConnectKMClassificationParams();
  }

  void ShareKMSamplingParameters()
  {
    ShareParameter("ram", "polystats.ram");
    ShareParameter("sampler", "select.sampler");
    ShareParameter("centroids.out", "training.classifier.sharkkm.centroids.out");
    ShareParameter("vm", "polystats.mask", "Validity Mask", "Validity mask, only non-zero pixels will be used to estimate KMeans modes.");
  }

  void ShareKMClassificationParams()
  {
    ShareParameter("nodatalabel", "classif.nodatalabel", "Label mask value",
                   "By default, hidden pixels will have the assigned label 0 in the output image. "
                   "It's possible to define the label mask by another value, "
                   "but be careful to not take a label from another class. "
                   "This application initializes the labels from 0 to N-1, "
                   "N is the number of class (defined by 'nc' parameter).");
  }

  void ConnectKMSamplingParams()
  {
    Connect("polystats.in", "imgenvelop.in");

    Connect("select.in", "polystats.in");
    Connect("select.vec", "polystats.vec");
    Connect("select.ram", "polystats.ram");

    Connect("extraction.in", "select.in");
    Connect("extraction.field", "select.field");
    Connect("extraction.vec", "select.out");
    Connect("extraction.ram", "polystats.ram");
  }

  void ConnectKMClassificationParams()
  {
    Connect("training.cfield", "extraction.field");
    Connect("training.io.stats", "imgstats.out");

    Connect("classif.in", "imgenvelop.in");
    Connect("classif.model", "training.io.out");
    Connect("classif.ram", "polystats.ram");
    Connect("classif.imstat", "imgstats.out");
  }

  void ConnectKMClassificationMask()
  {
    otbAppLogINFO("Using input mask ...");
    Connect("select.mask", "polystats.mask");
    Connect("classif.mask", "select.mask");
  }

  void ComputeImageEnvelope(const std::string& vectorFileName)
  {
    GetInternalApplication("imgenvelop")->SetParameterString("out", vectorFileName);
    GetInternalApplication("imgenvelop")->ExecuteAndWriteOutput();
  }

  void ComputeAddField(const std::string& vectorFileName, const std::string& fieldName)
  {
    otbAppLogINFO("add field in the layer ...");
    otb::ogr::DataSource::Pointer ogrDS;
    ogrDS                 = otb::ogr::DataSource::New(vectorFileName, otb::ogr::DataSource::Modes::Update_LayerUpdate);
    otb::ogr::Layer layer = ogrDS->GetLayer(0);

    OGRFieldDefn classField(fieldName.c_str(), OFTInteger);
    classField.SetWidth(classField.GetWidth());
    classField.SetPrecision(classField.GetPrecision());
    ogr::FieldDefn classFieldDefn(classField);
    layer.CreateField(classFieldDefn);

    otb::ogr::Layer::const_iterator it    = layer.cbegin();
    otb::ogr::Layer::const_iterator itEnd = layer.cend();
    for (; it != itEnd; ++it)
    {
      ogr::Feature dstFeature(layer.GetLayerDefn());
      dstFeature.SetFrom(*it, TRUE);
      dstFeature.SetFID(it->GetFID());
      dstFeature[fieldName].SetValue<int>(0);
      layer.SetFeature(dstFeature);
    }
    const OGRErr err = layer.ogr().CommitTransaction();
    if (err != OGRERR_NONE)
      itkExceptionMacro(<< "Unable to commit transaction for OGR layer " << layer.ogr().GetName() << ".");
    ogrDS->SyncToDisk();
  }

  void ComputePolygonStatistics(const std::string& statisticsFileName, const std::string& fieldName)
  {
    std::vector<std::string> fieldList = {fieldName};

    GetInternalApplication("polystats")->SetParameterStringList("field", fieldList);
    GetInternalApplication("polystats")->SetParameterString("out", statisticsFileName);

    ExecuteInternal("polystats");
  }

  void SelectAndExtractSamples(const std::string& statisticsFileName, const std::string& fieldName, const std::string& sampleFileName, int NBSamples)
  {
    /* SampleSelection */
    GetInternalApplication("select")->SetParameterString("out", sampleFileName);

    UpdateInternalParameters("select");
    GetInternalApplication("select")->SetParameterString("instats", statisticsFileName);
    GetInternalApplication("select")->SetParameterString("field", fieldName);

    GetInternalApplication("select")->SetParameterString("strategy", "constant");
    GetInternalApplication("select")->SetParameterInt("strategy.constant.nb", NBSamples);

    if (IsParameterEnabled("rand"))
      GetInternalApplication("select")->SetParameterInt("rand", GetParameterInt("rand"));

    // select sample positions
    ExecuteInternal("select");

    /* SampleExtraction */
    UpdateInternalParameters("extraction");

    GetInternalApplication("extraction")->SetParameterString("outfield", "prefix");
    GetInternalApplication("extraction")->SetParameterString("outfield.prefix.name", "value_");

    // extract sample descriptors
    GetInternalApplication("extraction")->ExecuteAndWriteOutput();
  }

  void TrainKMModel(FloatVectorImageType* image, const std::string& sampleTrainFileName, const std::string& modelFileName)
  {
    std::vector<std::string> extractOutputList = {sampleTrainFileName};
    GetInternalApplication("training")->SetParameterStringList("io.vd", extractOutputList);
    UpdateInternalParameters("training");

    // set field names
    std::string              selectPrefix = GetInternalApplication("extraction")->GetParameterString("outfield.prefix.name");
    unsigned int             nbBands      = image->GetNumberOfComponentsPerPixel();
    std::vector<std::string> selectedNames;
    for (unsigned int i = 0; i < nbBands; i++)
    {
      std::ostringstream oss;
      oss << i;
      selectedNames.push_back(selectPrefix + oss.str());
    }
    GetInternalApplication("training")->SetParameterStringList("feat", selectedNames);

    GetInternalApplication("training")->SetParameterString("classifier", "sharkkm");
    GetInternalApplication("training")->SetParameterInt("classifier.sharkkm.maxiter", GetParameterInt("maxit"));
    GetInternalApplication("training")->SetParameterInt("classifier.sharkkm.k", GetParameterInt("nc"));
    if (IsParameterEnabled("centroids.in") && HasValue("centroids.in"))
    {
      GetInternalApplication("training")->SetParameterString("classifier.sharkkm.centroids.in", GetParameterString("centroids.in"));

      GetInternalApplication("training")
          ->SetParameterString("classifier.sharkkm.centroids.stats", GetInternalApplication("imgstats")->GetParameterString("out"));
    }


    if (IsParameterEnabled("rand"))
      GetInternalApplication("training")->SetParameterInt("rand", GetParameterInt("rand"));
    GetInternalApplication("training")->GetParameterByKey("v")->SetActive(false);

    GetInternalApplication("training")->SetParameterString("io.out", modelFileName);

    ExecuteInternal("training");
    otbAppLogINFO("output model: " << GetInternalApplication("training")->GetParameterString("io.out"));
  }

  void ComputeImageStatistics(ImageBaseType* img, const std::string& imagesStatsFileName)
  {
    // std::vector<std::string> imageFileNameList = {imageFileName};
    GetInternalApplication("imgstats")->SetParameterImageBase("il", img);
    GetInternalApplication("imgstats")->SetParameterString("out", imagesStatsFileName);

    ExecuteInternal("imgstats");
    otbAppLogINFO("image statistics file: " << GetInternalApplication("imgstats")->GetParameterString("out"));
  }


  void KMeansClassif()
  {
    ExecuteInternal("classif");
  }

  class KMeansFileNamesHandler
  {
  public:
    KMeansFileNamesHandler(const std::string& outPath)
    {
      tmpVectorFile  = outPath + "_imgEnvelope.shp";
      polyStatOutput = outPath + "_polyStats.xml";
      sampleOutput   = outPath + "_sampleSelect.shp";
      modelFile      = outPath + "_model.txt";
      imgStatOutput  = outPath + "_imgstats.xml";
    }

    void clear()
    {
      RemoveFile(tmpVectorFile);
      RemoveFile(polyStatOutput);
      RemoveFile(sampleOutput);
      RemoveFile(modelFile);
      RemoveFile(imgStatOutput);
    }

    std::string tmpVectorFile;
    std::string polyStatOutput;
    std::string sampleOutput;
    std::string modelFile;
    std::string imgStatOutput;

  private:
    bool RemoveFile(const std::string& filePath)
    {
      bool res = true;
      if (itksys::SystemTools::FileExists(filePath))
      {
        size_t posExt = filePath.rfind('.');
        if (posExt != std::string::npos && filePath.compare(posExt, std::string::npos, ".shp") == 0)
        {
          std::string shxPath = filePath.substr(0, posExt) + std::string(".shx");
          std::string dbfPath = filePath.substr(0, posExt) + std::string(".dbf");
          std::string prjPath = filePath.substr(0, posExt) + std::string(".prj");
          RemoveFile(shxPath);
          RemoveFile(dbfPath);
          RemoveFile(prjPath);
        }
        res = itksys::SystemTools::RemoveFile(filePath);
        if (!res)
        {
          // otbAppLogINFO( <<"Unable to remove file  "<<filePath );
        }
      }
      return res;
    }
  };
};


class KMeansClassification : public KMeansApplicationBase
{
public:
  /** Standard class typedefs. */
  typedef KMeansClassification          Self;
  typedef KMeansApplicationBase         Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(Self, Superclass);

private:
  void DoInit() override
  {
    SetName("KMeansClassification");
    SetDescription("Unsupervised KMeans image classification");

    SetDocLongDescription(
        "Unsupervised KMeans image classification. "
        "This is a composite application, using existing training and classification applications. "
        "The SharkKMeans model is used.\n\n"
        "This application is only available if OTB is compiled with Shark support"
        "(CMake option :code:`OTB_USE_SHARK=ON`).\n\n"

        "The steps of this composite application:\n\n"
        "1) ImageEnvelope: create a shapefile (1 polygon),\n"
        "2) PolygonClassStatistics: compute the statistics,\n"
        "3) SampleSelection: select the samples by constant strategy in the shapefile "
        "(1000000 samples max),\n"
        "4) SampleExtraction: extract the samples descriptors (update of SampleSelection output file),\n"
        "5) ComputeImagesStatistics: compute images second order statistics,\n"
        "6) TrainVectorClassifier: train the SharkKMeans model,\n"
        "7) ImageClassifier: perform the classification of the input image "
        "according to a model file.\n\n"
        "It is possible to choose random/periodic modes of the SampleSelection application.\n"
        "If you do not want to keep the temporary files (sample selected, model file, ...), "
        "initialize cleanup parameter.\n"
        "For more information on shark KMeans algorithm [1].");

    SetDocLimitations("The application does not support NaN in the input image");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(
        "ImageEnvelope, PolygonClassStatistics, SampleSelection, SampleExtraction, "
        "PolygonClassStatistics, TrainVectorClassifier, ImageClassifier.\n\n"
        "[1] http://image.diku.dk/shark/sphinx_pages/build/html/rest_sources/tutorials/algorithms/kmeans.html");

    AddDocTag(Tags::Learning);
    AddDocTag(Tags::Segmentation);

    // Perform initialization
    ClearApplications();

    // initialisation parameters and synchronizes parameters
    Superclass::InitKMParams();

    AddRANDParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_1_ortho.tif");
    SetDocExampleParameterValue("ts", "1000");
    SetDocExampleParameterValue("nc", "5");
    SetDocExampleParameterValue("maxit", "1000");
    SetDocExampleParameterValue("out", "ClassificationFilterOutput.tif uint8");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
  }

  void DoExecute() override
  {
    if (IsParameterEnabled("vm") && HasValue("vm"))
      Superclass::ConnectKMClassificationMask();

    KMeansFileNamesHandler fileNames(GetParameterString("out"));

    const std::string fieldName = "field";

    // Create an image envelope
    Superclass::ComputeImageEnvelope(fileNames.tmpVectorFile);
    // Add a new field at the ImageEnvelope output file
    Superclass::ComputeAddField(fileNames.tmpVectorFile, fieldName);

    // Compute PolygonStatistics app
    UpdateKMPolygonClassStatisticsParameters(fileNames.tmpVectorFile);
    Superclass::ComputePolygonStatistics(fileNames.polyStatOutput, fieldName);

    // Compute number of sample max for KMeans
    const int theoricNBSamplesForKMeans        = GetParameterInt("ts");
    const int upperThresholdNBSamplesForKMeans = 1000 * 1000;
    const int actualNBSamplesForKMeans         = std::min(theoricNBSamplesForKMeans, upperThresholdNBSamplesForKMeans);
    otbAppLogINFO(<< actualNBSamplesForKMeans << " is the maximum sample size that will be used." << std::endl);

    // Compute SampleSelection and SampleExtraction app
    Superclass::SelectAndExtractSamples(fileNames.polyStatOutput, fieldName, fileNames.sampleOutput, actualNBSamplesForKMeans);

    // Compute Images second order statistics
    Superclass::ComputeImageStatistics(GetParameterImageBase("in"), fileNames.imgStatOutput);

    // Compute a train model with TrainVectorClassifier app
    Superclass::TrainKMModel(GetParameterImage("in"), fileNames.sampleOutput, fileNames.modelFile);

    // Compute a classification of the input image according to a model file
    Superclass::KMeansClassif();

    // Remove all tempory files
    if (GetParameterInt("cleanup"))
    {
      otbAppLogINFO(<< "Final clean-up ...");
      fileNames.clear();
    }
  }

  void UpdateKMPolygonClassStatisticsParameters(const std::string& vectorFileName)
  {
    GetInternalApplication("polystats")->SetParameterString("vec", vectorFileName);
    UpdateInternalParameters("polystats");
  }
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::KMeansClassification)
