#include "otbTrainImagesBase.h"

namespace otb
{
namespace Wrapper
{

class TrainImagesClassifier : public TrainImagesBase
{
public:
  typedef TrainImagesClassifier         Self;
  typedef TrainImagesBase               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  itkNewMacro( Self )
  itkTypeMacro( Self, Superclass )

  void DoInit() ITK_OVERRIDE
  {
    SetName( "TrainImagesClassifier" );
    SetDescription( "Train a classifier from multiple pairs of images and training vector data." );

    // Documentation
    SetDocName( "Train a classifier from multiple images" );
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
                    "(2.3.1 and later)." );
    SetDocLimitations( "None" );
    SetDocAuthors( "OTB-Team" );
    SetDocSeeAlso( "OpenCV documentation for machine learning http://docs.opencv.org/modules/ml/doc/ml.html " );

    AddDocTag( Tags::Learning );

    // Perform initialization
    ClearApplications();
    InitIO();
    InitSampling();
    InitClassification();

    AddDocTag( Tags::Learning );

    // Doc example parameter settings
    SetDocExampleParameterValue( "io.il", "QB_1_ortho.tif" );
    SetDocExampleParameterValue( "io.vd", "VectorData_QB1.shp" );
    SetDocExampleParameterValue( "io.imstat", "EstimateImageStatisticsQB1.xml" );
    SetDocExampleParameterValue( "sample.mv", "100" );
    SetDocExampleParameterValue( "sample.mt", "100" );
    SetDocExampleParameterValue( "sample.vtr", "0.5" );
    SetDocExampleParameterValue( "sample.vfn", "Class" );
    SetDocExampleParameterValue( "classifier", "libsvm" );
    SetDocExampleParameterValue( "classifier.libsvm.k", "linear" );
    SetDocExampleParameterValue( "classifier.libsvm.c", "1" );
    SetDocExampleParameterValue( "classifier.libsvm.opt", "false" );
    SetDocExampleParameterValue( "io.out", "svmModelQB1.txt" );
    SetDocExampleParameterValue( "io.confmatout", "svmConfusionMatrixQB1.csv" );
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
    if( HasValue( "io.vd" ) && IsParameterEnabled( "io.vd" ))
      {
      UpdatePolygonClassStatisticsParameters();
      }


    // Change mandatory of input vector depending on supervised and unsupervised mode.
    if( HasValue( "classifier" ) )
      {
      UpdateInternalParameters( "training" );
      switch( trainVectorBase->GetClassifierCategory() )
        {
        case TrainVectorBase::Unsupervised:
          MandatoryOff( "io.vd" );
          break;
        default:
        case TrainVectorBase::Supervised:
          MandatoryOn( "io.vd" );
          break;
        }
      }

  }

  /**
   * Select and Extract samples for validation with computed statistics and rates.
   * Validation samples could be empty if sample.vrt == 0 and if no dedicated validation are provided.
   * If no dedicated validation is provided the training is split corresponding to the sample.vtr parameter,
   * in this case if no vector data have been provided, the training rates and statistics are computed
   * on the selection and extraction training result.
   * fileNames.sampleOutputs contains training data and after an ExtractValidationData training data will
   * be split to fileNames.sampleTrainOutputs.
   * \param imageList
   * \param fileNames
   * \param validationVectorFileList
   * \param rates
   * \param HasInputVector
   */
  void ExtractValidationData(FloatVectorImageListType *imageList, TrainFileNamesHandler& fileNames,
                             std::vector<std::string> validationVectorFileList,
                             const SamplingRates& rates, bool HasInputVector )
  {
    if( !validationVectorFileList.empty() ) // Compute class statistics and sampling rate of validation data if provided.
      {
      ComputePolygonStatistics( imageList, validationVectorFileList, fileNames.polyStatValidOutputs );
      ComputeSamplingRate( fileNames.polyStatValidOutputs, fileNames.rateValidOut, rates.fmv );
      SelectAndExtractValidationSamples( fileNames, imageList, validationVectorFileList );
      if( HasInputVector ) // if input vector is provided the sampleTrainOutputs is the previously extracted sampleOutputs
        fileNames.sampleTrainOutputs = fileNames.sampleOutputs;
      }
    else if(GetParameterFloat("sample.vtr") != 0.0)// Split training data to validation
      {
      if( !HasInputVector ) // Compute one class statistics and sampling rate for the generated vector.
        ComputePolygonStatistics( imageList, fileNames.sampleOutputs, fileNames.polyStatTrainOutputs );
        ComputeSamplingRate( fileNames.polyStatTrainOutputs, fileNames.rateTrainOut, rates.fmt );
      SplitTrainingToValidationSamples( fileNames, imageList );
      }
    else // nothing to do, except update fileNames
      {
      fileNames.sampleTrainOutputs = fileNames.sampleOutputs;
      }
  }

  /**
   * Extract Training data depending if input vector is provided
   * \param imageList list of the image
   * \param fileNames handler that contain filenames
   * \param vectorFileList input vector file list (if provided
   * \param rates
   */
  void ExtractTrainData(FloatVectorImageListType *imageList, const TrainFileNamesHandler& fileNames,
                        std::vector<std::string> vectorFileList,
                        const SamplingRates& rates)
  {
    if( !vectorFileList.empty() ) // Select and Extract samples for training with computed statistics and rates
      {
      ComputePolygonStatistics( imageList, vectorFileList, fileNames.polyStatTrainOutputs );
      ComputeSamplingRate( fileNames.polyStatTrainOutputs, fileNames.rateTrainOut, rates.fmt );
      SelectAndExtractTrainSamples( fileNames, imageList, vectorFileList, SamplingStrategy::CLASS );
      }
    else // Select training samples base on geometric sampling if no input vector is provided
      {
      SelectAndExtractTrainSamples( fileNames, imageList, vectorFileList, SamplingStrategy::GEOMETRIC, "fid" );
      }
  }


  void DoExecute()
  {
    TrainFileNamesHandler fileNames;
    std::vector<std::string> vectorFileList;
    FloatVectorImageListType *imageList = GetParameterImageList( "io.il" );
    bool HasInputVector = IsParameterEnabled( "io.vd" ) && HasValue( "io.vd" );
    if(HasInputVector)
      vectorFileList = GetParameterStringList( "io.vd" );


    unsigned long nbInputs = imageList->Size();

    if( !HasInputVector && trainVectorBase->GetClassifierCategory() == TrainVectorBase::Supervised )
      {
      otbAppLogFATAL( "Missing input vector data files" );
      }

    if( !vectorFileList.empty() && nbInputs > vectorFileList.size() )
      {
      otbAppLogFATAL( "Missing input vector data files to match number of images (" << nbInputs << ")." );
      }

    // check if validation vectors are given
    std::vector<std::string> validationVectorFileList;
    bool dedicatedValidation = false;
    if( IsParameterEnabled( "io.valid" ) && HasValue( "io.valid" ) )
      {
      validationVectorFileList = GetParameterStringList( "io.valid" );
      if( nbInputs > validationVectorFileList.size() )
        {
        otbAppLogFATAL( "Missing validation vector data files to match number of images (" << nbInputs << ")." );
        }

      dedicatedValidation = true;
      }

    fileNames.CreateTemporaryFileNames( GetParameterString( "io.out" ), nbInputs, dedicatedValidation );

    // Compute final maximum sampling rates for both training and validation samples
    SamplingRates rates = ComputeFinalMaximumSamplingRates( dedicatedValidation );

    ExtractTrainData(imageList, fileNames, vectorFileList, rates);
    ExtractValidationData(imageList, fileNames, validationVectorFileList, rates, HasInputVector);

    // Then train the model with extracted samples
    TrainModel( imageList, fileNames.sampleTrainOutputs, fileNames.sampleValidOutputs );

    // cleanup
    if( IsParameterEnabled( "cleanup" ) )
      {
      otbAppLogINFO( <<"Final clean-up ..." );
      fileNames.clear();
      }
  }

private :

  void UpdatePolygonClassStatisticsParameters()
  {
    std::vector<std::string> vectorFileList = GetParameterStringList( "io.vd" );
    GetInternalApplication( "polystat" )->SetParameterString( "vec", vectorFileList[0], false );
    UpdateInternalParameters( "polystat" );
  }

};

}
}

OTB_APPLICATION_EXPORT( otb::Wrapper::TrainImagesClassifier )