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
      UpdatePolygonClassStatisticsParameters();
      }
  }

  void DoExecute() ITK_OVERRIDE
  {
    TrainFileNamesHandler fileNames;
    FloatVectorImageListType *imageList = GetParameterImageList( "io.il" );
    bool HasInputVector = IsParameterEnabled( "io.vd" ) && HasValue( "io.vd" );
    std::vector<std::string> vectorFileList = GetVectorFileList( GetParameterString( "io.out" ), fileNames );


    unsigned long nbInputs = imageList->Size();

    if( nbInputs > vectorFileList.size() )
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
      SelectAndExtractTrainSamples( fileNames, imageList, vectorFileList, SamplingStrategy::GEOMETRIC );
    }

    // Select and Extract samples for validation with computed statistics and rates
    // Validation samples could be empty if sample.vrt == 0 and if no dedicated validation are provided
    if( dedicatedValidation ) {
      ComputePolygonStatistics(imageList, validationVectorFileList, fileNames.polyStatValidOutputs);
      ComputeSamplingRate(fileNames.polyStatValidOutputs, fileNames.rateValidOut, rates.fmv);
      }
    SelectAndExtractValidationSamples(fileNames, imageList, validationVectorFileList, dedicatedValidation);


    // Then train the model with extracted samples
    TrainModel( imageList, fileNames.sampleTrainOutputs, fileNames.sampleValidOutputs);

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


  /**
   * Retrieve input vector data if provided otherwise generate a default vector shape file for each image.
   * \param output vector file path
   * \param fileNames
   * \return list of input vector data file names
   */
  std::vector<std::string> GetVectorFileList(std::string output, TrainFileNamesHandler &fileNames)
  {
    std::vector<std::string> vectorFileList;
    bool HasInputVector = IsParameterEnabled( "io.vd" ) && HasValue( "io.vd" );

    // Retrieve provided input vector data if available.
    if( !HasInputVector )
      {
      FloatVectorImageListType *imageList = GetParameterImageList( "io.il" );
      unsigned int nbInputs = static_cast<unsigned int>(imageList->Size());

      for( unsigned int i = 0; i < nbInputs; ++i )
        {
        std::string name = output + "_vector_" + std::to_string( i ) + ".shp";
        GenerateVectorDataFile( imageList->GetNthElement( i ), name );
        fileNames.tmpVectorFileList.push_back( name );
        }
      vectorFileList = fileNames.tmpVectorFileList;
      SetParameterStringList( "io.vd", vectorFileList, false );
      UpdatePolygonClassStatisticsParameters();
      GetInternalApplication( "polystat" )->SetParameterString( "field", "fid" );
      }
    else
      {
      vectorFileList = GetParameterStringList( "io.vd" );
      }

    return vectorFileList;
  }



  void GenerateVectorDataFile(const FloatVectorImageListType::ObjectPointerType &floatVectorImage, std::string name)
  {
    typedef otb::ImageToEnvelopeVectorDataFilter<FloatVectorImageType, VectorDataType> ImageToEnvelopeFilterType;
    typedef ImageToEnvelopeFilterType::OutputVectorDataType OutputVectorData;
    typedef otb::VectorDataFileWriter<OutputVectorData> VectorDataWriter;

    ImageToEnvelopeFilterType::Pointer imageToEnvelopeVectorData = ImageToEnvelopeFilterType::New();
    imageToEnvelopeVectorData->SetInput( floatVectorImage );
    imageToEnvelopeVectorData->SetOutputProjectionRef( floatVectorImage->GetProjectionRef().c_str() );
    OutputVectorData::Pointer vectorData = imageToEnvelopeVectorData->GetOutput();

    // write temporary generated vector file to disk.
    VectorDataWriter::Pointer vectorDataFileWriter = VectorDataWriter::New();
    vectorDataFileWriter->SetInput( vectorData );
    vectorDataFileWriter->SetFileName( name.c_str() );
    vectorDataFileWriter->Write();
  }

};

}
}

OTB_APPLICATION_EXPORT( otb::Wrapper::TrainImagesClustering )