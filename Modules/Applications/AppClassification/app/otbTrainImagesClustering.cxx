#include "otbTrainImagesBase.h"

namespace otb
{
namespace Wrapper
{

class TrainImagesClustering : public TrainImagesBase
{
public:
  typedef TrainImagesClustering         Self;
  typedef TrainImagesBase               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  itkNewMacro( Self )
  itkTypeMacro( Self, Superclass )

  void DoInit() ITK_OVERRIDE
  {
    SetName( "TrainImagesClustering" );
    SetDescription( "Train a classifier from multiple pairs of images and optional input training vector data." );

    // Documentation
    SetDocName( "Train a classifier from multiple images" );
    SetDocLongDescription( "TODO" );
    SetDocLimitations( "None" );
    SetDocAuthors( "OTB-Team" );
    SetDocSeeAlso( "OpenCV documentation for machine learning http://docs.opencv.org/modules/ml/doc/ml.html " );

    AddDocTag( Tags::Learning );

    ClearApplications();
    InitIO();
    InitSampling();
    InitClassification( false );

    AddParameter( ParameterType_Float, "sample.percent", "Percentage of samples extract in images for "
            "training and validation when only images are provided." );
    SetParameterDescription( "sample.percent", "Percentage of samples extract in images for "
            "training and validation when only images are provided. This parameter is disable when vector data are provided" );
    SetDefaultParameterFloat( "sample.percent", 1.0 );
    SetMinimumParameterFloatValue( "sample.percent", 0.0 );
    SetMaximumParameterFloatValue( "sample.percent", 1.0 );

    // Doc example parameter settings
    SetDocExampleParameterValue( "io.il", "QB_1_ortho.tif" );
    SetDocExampleParameterValue( "io.vd", "VectorData_QB1.shp" );
    SetDocExampleParameterValue( "io.imstat", "EstimateImageStatisticsQB1.xml" );
    SetDocExampleParameterValue( "sample.mv", "100" );
    SetDocExampleParameterValue( "sample.mt", "100" );
    SetDocExampleParameterValue( "sample.vtr", "0.5" );
    SetDocExampleParameterValue( "sample.vfn", "Class" );
    SetDocExampleParameterValue( "classifier", "sharkkm" );
    SetDocExampleParameterValue( "classifier.sharkkm.k", "2" );
    SetDocExampleParameterValue( "io.out", "sharkKMModelQB1.txt" );
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
    if( HasValue( "io.vd" ) )
      {
      MandatoryOff( "sample.percent" );
      UpdatePolygonClassStatisticsParameters();
      }
    else
      {
      MandatoryOn( "sample.percent" );
      }
  }

  void DoExecute() ITK_OVERRIDE
  {
    TrainFileNamesHandler fileNames;
    FloatVectorImageListType *imageList = GetParameterImageList( "io.il" );
    bool HasInputVector = IsParameterEnabled( "io.vd" ) && HasValue( "io.vd" );
    std::vector<std::string> vectorFileList = GetParameterStringList( "io.vd" );


    unsigned long nbInputs = imageList->Size();

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

    if( HasInputVector )
      {
      // Select and Extract samples for training with computed statistics and rates
      ComputePolygonStatistics( imageList, vectorFileList, fileNames.polyStatTrainOutputs );
      ComputeSamplingRate( fileNames.polyStatTrainOutputs, fileNames.rateTrainOut, rates.fmt );
      SelectAndExtractTrainSamples( fileNames, imageList, vectorFileList, SamplingStrategy::CLASS );
      }
    else
      {
      SelectAndExtractTrainSamples( fileNames, imageList, vectorFileList, SamplingStrategy::GEOMETRIC, "fid" );
      }

    // Select and Extract samples for validation with computed statistics and rates.
    // Validation samples could be empty if sample.vrt == 0 and if no dedicated validation are provided
    // If no dedicated validation is provided the training is split corresponding to the sample.vtr parameter
    // In this case if no vector data have been provided, the training rates and statistics are computed
    // on the selection and extraction training result.
    if( dedicatedValidation )
      {
      ComputePolygonStatistics( imageList, validationVectorFileList, fileNames.polyStatValidOutputs );
      ComputeSamplingRate( fileNames.polyStatValidOutputs, fileNames.rateValidOut, rates.fmv );
      }
    else if(!HasInputVector)
      {
      ComputePolygonStatistics( imageList, fileNames.sampleOutputs, fileNames.polyStatTrainOutputs );
      ComputeSamplingRate( fileNames.polyStatTrainOutputs, fileNames.rateTrainOut, rates.fmt );
      }


    // Extract or split validation vector data.
    SelectAndExtractValidationSamples( fileNames, imageList, validationVectorFileList );

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

OTB_APPLICATION_EXPORT( otb::Wrapper::TrainImagesClustering )