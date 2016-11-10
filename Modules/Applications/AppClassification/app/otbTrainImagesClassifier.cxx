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
#include "otbWrapperCompositeApplication.h"
#include "otbWrapperApplicationFactory.h"


namespace otb
{
namespace Wrapper
{

class TrainImagesClassifier: public CompositeApplication
{
public:
  /** Standard class typedefs. */
  typedef TrainImagesClassifier Self;
  typedef CompositeApplication Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self)

  itkTypeMacro(TrainImagesClassifier, otb::Wrapper::CompositeApplication)

protected:

private:

void DoInit() ITK_OVERRIDE
{
  SetName("TrainImagesClassifier");
  SetDescription(
    "Train a classifier from multiple pairs of images and training vector data.");

  // Documentation
  SetDocName("Train a classifier from multiple images");
  SetDocLongDescription(
    "This application performs a classifier training from multiple pairs of input images and training vector data. "
    "Samples are composed of pixel values in each band optionally centered and reduced using an XML statistics file produced by "
    "the ComputeImagesStatistics application.\n The training vector data must contain polygons with a positive integer field "
    "representing the class label. The name of this field can be set using the \"Class label field\" parameter. Training and validation "
    "sample lists are built such that each class is equally represented in both lists. One parameter allows controlling the ratio "
    "between the number of samples in training and validation sets. Two parameters allow managing the size of the training and "
    "validation sets per class and per image.\n Several classifier parameters can be set depending on the chosen classifier. In the "
    "validation process, the confusion matrix is organized the following way: rows = reference labels, columns = produced labels. "
    "In the header of the optional confusion matrix output file, the validation (reference) and predicted (produced) class labels"
    " are ordered according to the rows/columns of the confusion matrix.\n This application is based on LibSVM and OpenCV Machine Learning "
    "(2.3.1 and later).");
  SetDocLimitations("None");
  SetDocAuthors("OTB-Team");
  SetDocSeeAlso("OpenCV documentation for machine learning http://docs.opencv.org/modules/ml/doc/ml.html ");

  AddApplication("PolygonClassStatistics", "polystat","Polygon analysis");
  AddApplication("MultiImageSamplingRate", "rates", "Sampling rates");
  AddApplication("SampleSelection", "select", "Sample selection");
  AddApplication("SampleExtraction","extraction", "Sample extraction");
  AddApplication("TrainVectorClassifier", "training", "Model training");

  //Group IO
  AddParameter(ParameterType_Group, "io", "Input and output data");
  SetParameterDescription("io", "This group of parameters allows setting input and output data.");

  AddParameter(ParameterType_InputImageList, "io.il", "Input Image List");
  SetParameterDescription("io.il", "A list of input images.");
  AddParameter(ParameterType_InputVectorDataList, "io.vd", "Input Vector Data List");
  SetParameterDescription("io.vd", "A list of vector data to select the training samples.");

  ShareParameter("io.imstat","training.io.stats");
  ShareParameter("io.confmatout","training.io.confmatout");
  ShareParameter("io.out","training.io.out");

  // TODO : handle elev parameters ?

  // Sampling settings
  AddParameter(ParameterType_Group, "sample", "Training and validation samples parameters");
  SetParameterDescription("sample",
    "This group of parameters allows you to set training and validation sample lists parameters.");
  AddParameter(ParameterType_Int, "sample.mt", "Maximum training sample size per class");
  SetDefaultParameterInt("sample.mt", 1000);
  SetParameterDescription("sample.mt", "Maximum size per class (in pixels) of "
    "the training sample list (default = 1000) (no limit = -1). If equal to -1,"
    " then the maximal size of the available training sample list per class "
    "will be equal to the surface area of the smallest class multiplied by the"
    " training sample ratio.");
  AddParameter(ParameterType_Int, "sample.mv", "Maximum validation sample size per class");
  SetDefaultParameterInt("sample.mv", 1000);
  SetParameterDescription("sample.mv", "Maximum size per class (in pixels) of "
    "the validation sample list (default = 1000) (no limit = -1). If equal to -1,"
    " then the maximal size of the available validation sample list per class "
    "will be equal to the surface area of the smallest class multiplied by the "
    "validation sample ratio.");
  AddParameter(ParameterType_Int, "sample.bm", "Bound sample number by minimum");
  SetDefaultParameterInt("sample.bm", 1);
  SetParameterDescription("sample.bm", "Bound the number of samples for each "
    "class by the number of available samples by the smaller class. Proportions "
    "between training and validation are respected. Default is true (=1).");
  AddParameter(ParameterType_Float, "sample.vtr", "Training and validation sample ratio");
  SetParameterDescription("sample.vtr",
    "Ratio between training and validation samples (0.0 = all training, 1.0 = "
    "all validation) (default = 0.5).");
  SetParameterFloat("sample.vtr", 0.5);

  ShareParameter("sample.vfn","training.cfield");
  ShareParameter("sample.strategy","rates.strategy");
  ShareParameter("sample.mim","rates.mim");

  // Classifier settings
  ShareParameter("classifier","training.classifier");

  ShareParameter("rand","training.rand");

  // Synchronization between applications
  Connect("select.field", "polystat.field");
  Connect("select.layer", "polystat.layer");

  Connect("extraction.in",    "select.in");
  Connect("extraction.vec",   "select.vec");
  Connect("extraction.field", "select.field");
  Connect("extraction.layer", "select.layer");

  Connect("select.rand", "training.rand");

  // Doc example parameter settings
  SetDocExampleParameterValue("io.il", "QB_1_ortho.tif");
  SetDocExampleParameterValue("io.vd", "VectorData_QB1.shp");
  SetDocExampleParameterValue("io.imstat", "EstimateImageStatisticsQB1.xml");
  SetDocExampleParameterValue("sample.mv", "100");
  SetDocExampleParameterValue("sample.mt", "100");
  SetDocExampleParameterValue("sample.vtr", "0.5");
  SetDocExampleParameterValue("sample.edg", "false");
  SetDocExampleParameterValue("sample.vfn", "Class");
  SetDocExampleParameterValue("classifier", "libsvm");
  SetDocExampleParameterValue("classifier.libsvm.k", "linear");
  SetDocExampleParameterValue("classifier.libsvm.c", "1");
  SetDocExampleParameterValue("classifier.libsvm.opt", "false");
  SetDocExampleParameterValue("io.out", "svmModelQB1.txt");
  SetDocExampleParameterValue("io.confmatout", "svmConfusionMatrixQB1.csv");
}

void DoUpdateParameters() ITK_OVERRIDE
{
  // TODO : if an image is present, check number of bands and fill training.field ListView
  if (HasValue("io.il"))
    {
    FloatVectorImageListType* imageList = GetParameterImageList("io.il");
    FloatVectorImageType::Pointer image = imageList->GetNthElement(imgIndex);
    //image->UpdateOutputInformation();
    unsigned int nbBands = image->GetNumberOfComponentsPerPixel();
    GetInternalApplication("training")->ClearChoices("feat");
    std::string key("feat.value");
    std::string value("value_");
    for (unsigned int i=0 ; i<nbBands ; i++)
      {
      std::ostringstream oss;
      oss << i;
      GetInternalApplication("training")->AddChoices(key+oss.str(),value+oss.str());
      }
    }
}

void DoExecute() ITK_OVERRIDE
{
  FloatVectorImageListType* imageList = GetParameterImageList("io.il");
  std::vector<std::string> vectorFileList = GetParameterStringList("io.vd");
  unsigned int nbInputs = imageList->Size();
  std::string outModel(GetParameterString("io.out"));
  std::vector<std::string> polyStatOutputs;
  std::vector<std::string> ratesOutputs;

  // Polygons stats
  for (unsigned int i=0 ; i<nbInputs ; i++)
    {
    std::ostringstream oss;
    oss << outModel << "_stats"<<i<<".xml";
    polyStatOutputs.push_back(oss.str());
    GetInternalApplication("polystat")->SetParameterInputImage("in",imageList->GetNthElement(i));
    GetInternalApplication("polystat")->SetParameterString("vec",vectorFileList[i]);
    GetInternalApplication("polystat")->SetParameterString("out",oss.str());
    }
}

};

} // end namespace Wrapper
} // end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::TrainImagesClassifier)
