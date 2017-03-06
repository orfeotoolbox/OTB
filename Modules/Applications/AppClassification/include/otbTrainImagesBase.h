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
#ifndef otbTrainImagesBase_h
#define otbTrainImagesBase_h


#include "otbVectorDataFileWriter.h"
#include "otbWrapperCompositeApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbStatisticsXMLFileWriter.h"
#include "otbImageToEnvelopeVectorDataFilter.h"
#include "otbSamplingRateCalculator.h"
#include "otbOGRDataToSamplePositionFilter.h"

namespace otb
{
namespace Wrapper
{

/** \class TrainImagesBase
 * \brief Base class for the TrainImagesBaseClassifier and Clustering
 *
 * This class intends to hold common input/output parameters and
 * composite application connection for both supervised and unsupervised
 * model training.
 *
 * \ingroup OTBAppClassification
 */
class TrainImagesBase : public CompositeApplication
{
public:
  /** Standard class typedefs. */
  typedef TrainImagesBase Self;
  typedef CompositeApplication Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkTypeMacro( TrainImagesBase, Superclass )

  /** filters typedefs*/
  typedef otb::OGRDataToSamplePositionFilter<FloatVectorImageType, UInt8ImageType, otb::PeriodicSampler> PeriodicSamplerType;

  typedef otb::SamplingRateCalculator::MapRateType MapRateType;

protected:

  enum SamplingStrategy
  {
    CLASS, GEOMETRIC
  };

  struct SamplingRates;

  class TrainFileNamesHandler;

  void InitIO()
  {
    //Group IO
    AddParameter( ParameterType_Group, "io", "Input and output data" );
    SetParameterDescription( "io", "This group of parameters allows setting input and output data." );

    AddParameter( ParameterType_InputImageList, "io.il", "Input Image List" );
    SetParameterDescription( "io.il", "A list of input images." );
    AddParameter( ParameterType_InputVectorDataList, "io.vd", "Input Vector Data List" );
    SetParameterDescription( "io.vd", "A list of vector data to select the training samples." );

    AddParameter( ParameterType_Empty, "cleanup", "Temporary files cleaning" );
    EnableParameter( "cleanup" );
    SetParameterDescription( "cleanup",
                             "If activated, the application will try to clean all temporary files it created" );
    MandatoryOff( "cleanup" );
  }

  void InitSampling()
  {
    AddApplication( "PolygonClassStatistics", "polystat", "Polygon analysis" );
    AddApplication( "MultiImageSamplingRate", "rates", "Sampling rates" );
    AddApplication( "SampleSelection", "select", "Sample selection" );
    AddApplication( "SampleExtraction", "extraction", "Sample extraction" );

    // Sampling settings
    AddParameter( ParameterType_Group, "sample", "Training and validation samples parameters" );
    SetParameterDescription( "sample",
                             "This group of parameters allows you to set training and validation sample lists parameters." );
    AddParameter( ParameterType_Int, "sample.mt", "Maximum training sample size per class" );
    SetDefaultParameterInt( "sample.mt", 1000 );
    SetParameterDescription( "sample.mt", "Maximum size per class (in pixels) of "
            "the training sample list (default = 1000) (no limit = -1). If equal to -1,"
            " then the maximal size of the available training sample list per class "
            "will be equal to the surface area of the smallest class multiplied by the"
            " training sample ratio." );
    AddParameter( ParameterType_Int, "sample.mv", "Maximum validation sample size per class" );
    SetDefaultParameterInt( "sample.mv", 1000 );
    SetParameterDescription( "sample.mv", "Maximum size per class (in pixels) of "
            "the validation sample list (default = 1000) (no limit = -1). If equal to -1,"
            " then the maximal size of the available validation sample list per class "
            "will be equal to the surface area of the smallest class multiplied by the "
            "validation sample ratio." );
    AddParameter( ParameterType_Int, "sample.bm", "Bound sample number by minimum" );
    SetDefaultParameterInt( "sample.bm", 1 );
    SetParameterDescription( "sample.bm", "Bound the number of samples for each "
            "class by the number of available samples by the smaller class. Proportions "
            "between training and validation are respected. Default is true (=1)." );
    AddParameter( ParameterType_Float, "sample.vtr", "Training and validation sample ratio" );
    SetParameterDescription( "sample.vtr", "Ratio between training and validation samples (0.0 = all training, 1.0 = "
            "all validation) (default = 0.5)." );
    SetParameterFloat( "sample.vtr", 0.5, false );
    SetMaximumParameterFloatValue( "sample.vtr", 1.0 );
    SetMinimumParameterFloatValue( "sample.vtr", 0.0 );

    ShareSamplingParameters();
    ConnectSamplingParameters();
  }

  void ShareSamplingParameters()
  {
    // hide sampling parameters
    //ShareParameter("sample.strategy","rates.strategy");
    //ShareParameter("sample.mim","rates.mim");
    ShareParameter( "ram", "polystat.ram" );
    ShareParameter( "elev", "polystat.elev" );
    ShareParameter( "sample.vfn", "polystat.field" );
  }

  void ConnectSamplingParameters()
  {
    Connect( "extraction.field", "polystat.field" );
    Connect( "extraction.layer", "polystat.layer" );

    Connect( "select.ram", "polystat.ram" );
    Connect( "extraction.ram", "polystat.ram" );

    Connect( "select.field", "polystat.field" );
    Connect( "select.layer", "polystat.layer" );
    Connect( "select.elev", "polystat.elev" );

    Connect( "extraction.in", "select.in" );
    Connect( "extraction.vec", "select.out" );
  }

  void InitClassification(bool supervised)
  {
    if( supervised )
      AddApplication( "TrainVectorClassifier", "training", "Model training" );
    else
      AddApplication( "TrainVectorUnsupervised", "training", "Model training" );

    AddParameter( ParameterType_InputVectorDataList, "io.valid", "Validation Vector Data List" );
    SetParameterDescription( "io.valid", "A list of vector data to select the training samples." );
    MandatoryOff( "io.valid" );

    if( !supervised )
      MandatoryOff( "io.vd" );

    ShareClassificationParams( supervised );
    ConnectClassificationParams();
  };

  void ShareClassificationParams(bool supervised)
  {
    ShareParameter( "io.imstat", "training.io.stats" );
    ShareParameter( "io.out", "training.io.out" );

    ShareParameter( "classifier", "training.classifier" );
    ShareParameter( "rand", "training.rand" );

    if( supervised )
      ShareParameter( "io.confmatout", "training.io.confmatout" );
  }

  void ConnectClassificationParams()
  {
    Connect( "training.cfield", "polystat.field" );
    Connect( "select.rand", "training.rand" );
  }

  /**
   * Compute polygon statistics given provided strategy with PolygonClassStatistics class
   * \param imageList list of input images
   * \param vectorFileNames list of input vector file names
   * \param statisticsFileNames list of out
   */
  void ComputePolygonStatistics(FloatVectorImageListType *imageList, const std::vector<std::string> &vectorFileNames,
                                const std::vector<std::string> &statisticsFileNames)
  {
    unsigned int nbImages = static_cast<unsigned int>(imageList->Size());
    for( unsigned int i = 0; i < nbImages; i++ )
      {
      GetInternalApplication( "polystat" )->SetParameterInputImage( "in", imageList->GetNthElement( i ) );
      GetInternalApplication( "polystat" )->SetParameterString( "vec", vectorFileNames[i], false );
      GetInternalApplication( "polystat" )->SetParameterString( "out", statisticsFileNames[i], false );
      ExecuteInternal( "polystat" );
      }
  }

  /**
   * Compute final maximum training and validation
   * \param dedicatedValidation
   * \return SamplingRates final maximum training and final maximum validation
   */
  SamplingRates ComputeFinalMaximumSamplingRates(bool dedicatedValidation)
  {
    SamplingRates rates;
    GetInternalApplication( "rates" )->SetParameterString( "mim", "proportional", false );
    double vtr = GetParameterFloat( "sample.vtr" );
    long mt = GetParameterInt( "sample.mt" );
    long mv = GetParameterInt( "sample.mv" );
    // compute final maximum training and final maximum validation
    // By default take all samples (-1 means all samples)
    rates.fmt = -1;
    rates.fmv = -1;
    if( GetParameterInt( "sample.bm" ) == 0 )
      {
      if( dedicatedValidation )
        {
        // fmt and fmv will be used separately
        rates.fmt = mt;
        rates.fmv = mv;
        if( mt > -1 && mv <= -1 && vtr < 0.99999 )
          {
          rates.fmv = static_cast<long>(( double ) mt * vtr / ( 1.0 - vtr ));
          }
        if( mt <= -1 && mv > -1 && vtr > 0.00001 )
          {
          rates.fmt = static_cast<long>(( double ) mv * ( 1.0 - vtr ) / vtr);
          }
        }
      else
        {
        // only fmt will be used for both training and validation samples
        // So we try to compute the total number of samples given input
        // parameters mt, mv and vtr.
        if( mt > -1 && mv > -1 )
          {
          rates.fmt = mt + mv;
          }
        if( mt > -1 && mv <= -1 && vtr < 0.99999 )
          {
          rates.fmt = static_cast<long>(( double ) mt / ( 1.0 - vtr ));
          }
        if( mt <= -1 && mv > -1 && vtr > 0.00001 )
          {
          rates.fmt = static_cast<long>(( double ) mv / vtr);
          }
        }
      }
    return rates;
  }


  /**
   * Compute rates using MultiImageSamplingRate application
   * \param statisticsFileNames
   * \param ratesFileName
   * \param maximum final maximum value computed by ComputeFinalMaximumSamplingRates
   * \sa ComputeFinalMaximumSamplingRates
   */
  void ComputeSamplingRate(const std::vector<std::string> &statisticsFileNames, const std::string &ratesFileName,
                           long maximum)
  {
    // Sampling rates
    GetInternalApplication( "rates" )->SetParameterStringList( "il", statisticsFileNames, false );
    GetInternalApplication( "rates" )->SetParameterString( "out", ratesFileName, false );
    if( GetParameterInt( "sample.bm" ) != 0 )
      {
      GetInternalApplication( "rates" )->SetParameterString( "strategy", "smallest", false );
      }
    else
      {
      if( maximum > -1 )
        {
        GetInternalApplication( "rates" )->SetParameterString( "strategy", "constant", false );
        GetInternalApplication( "rates" )->SetParameterInt( "strategy.constant.nb", static_cast<int>(maximum), false );
        }
      else
        {
        GetInternalApplication( "rates" )->SetParameterString( "strategy", "all", false );
        }
      }
    ExecuteInternal( "rates" );
  }

  /**
   * Train the model with training and optional validation data samples
   * \param imageList list of input images
   * \param sampleTrainFileNames files names of the training samples
   * \param sampleValidationFileNames file names of the validation sample
   */
  void TrainModel(FloatVectorImageListType *imageList, const std::vector<std::string> &sampleTrainFileNames,
                  const std::vector<std::string> &sampleValidationFileNames)
  {
    GetInternalApplication( "training" )->SetParameterStringList( "io.vd", sampleTrainFileNames, false );
    if( !sampleValidationFileNames.empty() )
      GetInternalApplication( "training" )->SetParameterStringList( "valid.vd", sampleValidationFileNames, false );

    UpdateInternalParameters( "training" );
    // set field names
    FloatVectorImageType::Pointer image = imageList->GetNthElement( 0 );
    unsigned int nbBands = image->GetNumberOfComponentsPerPixel();
    std::vector<std::string> selectedNames;
    for( unsigned int i = 0; i < nbBands; i++ )
      {
      std::ostringstream oss;
      oss << i;
      selectedNames.push_back( "value_" + oss.str() );
      }
    GetInternalApplication( "training" )->SetParameterStringList( "feat", selectedNames, false );
    ExecuteInternal( "training" );
  }

  /**
   * Select samples by class or by geographic strategy
   * \param image
   * \param vectorFileName
   * \param sampleFileName
   * \param statisticsFileName
   * \param ratesFileName
   * \param strategy
   */
  void SelectAndExtractSamples(FloatVectorImageType *image, std::string vectorFileName, std::string sampleFileName,
                               std::string statisticsFileName, std::string ratesFileName, SamplingStrategy strategy,
                               std::string selectedField = "")
  {
    GetInternalApplication( "select" )->SetParameterInputImage( "in", image );
    GetInternalApplication( "select" )->SetParameterString( "out", sampleFileName, false );

    // Change the selection strategy based on selected sampling strategy
    switch( strategy )
      {
      case GEOMETRIC:
        GetInternalApplication( "select" )->SetParameterString( "sampler", "random", false );
        GetInternalApplication( "select" )->SetParameterString( "strategy", "percent", false );
        GetInternalApplication( "select" )->SetParameterFloat( "strategy.percent.p",
                                                               GetParameterFloat( "sample.percent" ), false );
        break;
      case CLASS:
      default:
        GetInternalApplication( "select" )->SetParameterString( "vec", vectorFileName, false );
        GetInternalApplication( "select" )->SetParameterString( "instats", statisticsFileName, false );
        GetInternalApplication( "select" )->SetParameterString( "sampler", "periodic", false );
        GetInternalApplication( "select" )->SetParameterInt( "sampler.periodic.jitter", 50 );
        GetInternalApplication( "select" )->SetParameterString( "strategy", "byclass", false );
        GetInternalApplication( "select" )->SetParameterString( "strategy.byclass.in", ratesFileName, false );
        break;
      }

    // select sample positions
    ExecuteInternal( "select" );

    GetInternalApplication( "extraction" )->SetParameterString( "vec", sampleFileName, false );
    UpdateInternalParameters( "extraction" );
    if( !selectedField.empty() )
      GetInternalApplication( "extraction" )->SetParameterString( "field", selectedField, false );

    GetInternalApplication( "extraction" )->SetParameterString( "outfield", "prefix", false );
    GetInternalApplication( "extraction" )->SetParameterString( "outfield.prefix.name", "value_", false );

    // extract sample descriptors
    ExecuteInternal( "extraction" );
  }

  /**
   * Select and extract samples with the SampleSelection and SampleExtraction application.
   */
  void SelectAndExtractTrainSamples(const TrainFileNamesHandler &fileNames, FloatVectorImageListType *imageList,
                                    std::vector<std::string> vectorFileNames, SamplingStrategy strategy,
                                    std::string selectedFieldName = "")
  {

    for( unsigned int i = 0; i < imageList->Size(); ++i )
      {
      std::string vectorFileName = vectorFileNames.empty() ? "" : vectorFileNames[i];
      SelectAndExtractSamples( imageList->GetNthElement( i ), vectorFileName, fileNames.sampleOutputs[i],
                               fileNames.polyStatTrainOutputs[i], fileNames.ratesTrainOutputs[i], strategy,
                               selectedFieldName );
      }
  }


  void SelectAndExtractValidationSamples(const TrainFileNamesHandler &fileNames, FloatVectorImageListType *imageList,
                                         const std::vector<std::string> &validationVectorFileList = std::vector<std::string>())
  {
    // In dedicated validation mode the by class sampling strategy and statistics are used.
    // Otherwise simply split training to validation samples corresponding to sample.vtr percentage.
    if( !validationVectorFileList.empty() )
      {
      for( unsigned int i = 0; i < imageList->Size(); ++i )
        {
        SelectAndExtractSamples( imageList->GetNthElement( i ), validationVectorFileList[i],
                                 fileNames.sampleValidOutputs[i], fileNames.polyStatValidOutputs[i],
                                 fileNames.ratesValidOutputs[i], SamplingStrategy::CLASS );
        }
      }
    else
      {
      for( unsigned int i = 0; i < imageList->Size(); ++i )
        {
        SplitTrainingAndValidationSamples( imageList->GetNthElement( i ), fileNames.sampleOutputs[i],
                                           fileNames.sampleTrainOutputs[i], fileNames.sampleValidOutputs[i],
                                           fileNames.ratesTrainOutputs[i] );
        }
      }
  }

private:
  void SplitTrainingAndValidationSamples(FloatVectorImageType *image, std::string sampleFileName,
                                         std::string sampleTrainFileName, std::string sampleValidFileName,
                                         std::string ratesTrainFileName)
  {
    // Split between training and validation
    ogr::DataSource::Pointer source = ogr::DataSource::New( sampleFileName, ogr::DataSource::Modes::Read );
    ogr::DataSource::Pointer destTrain = ogr::DataSource::New( sampleTrainFileName, ogr::DataSource::Modes::Overwrite );
    ogr::DataSource::Pointer destValid = ogr::DataSource::New( sampleValidFileName, ogr::DataSource::Modes::Overwrite );
    // read sampling rates from ratesTrainOutputs
    SamplingRateCalculator::Pointer rateCalculator = SamplingRateCalculator::New();
    rateCalculator->Read( ratesTrainFileName );
    // Compute sampling rates for train and valid
    const MapRateType &inputRates = rateCalculator->GetRatesByClass();
    MapRateType trainRates;
    MapRateType validRates;
    otb::SamplingRateCalculator::TripletType tpt;
    for( MapRateType::const_iterator it = inputRates.begin(); it != inputRates.end(); ++it )
      {
      double vtr = GetParameterFloat( "sample.vtr" );
      unsigned long total = std::min( it->second.Required, it->second.Tot );
      unsigned long neededValid = static_cast<unsigned long>(( double ) total * vtr );
      unsigned long neededTrain = total - neededValid;
      tpt.Tot = total;
      tpt.Required = neededTrain;
      tpt.Rate = ( 1.0 - vtr );
      trainRates[it->first] = tpt;
      tpt.Tot = neededValid;
      tpt.Required = neededValid;
      tpt.Rate = 1.0;
      validRates[it->first] = tpt;
      }

    // Use an otb::OGRDataToSamplePositionFilter with 2 outputs
    PeriodicSamplerType::SamplerParameterType param;
    param.Offset = 0;
    param.MaxJitter = 0;
    PeriodicSamplerType::Pointer splitter = PeriodicSamplerType::New();
    splitter->SetInput( image );
    splitter->SetOGRData( source );
    splitter->SetOutputPositionContainerAndRates( destTrain, trainRates, 0 );
    splitter->SetOutputPositionContainerAndRates( destValid, validRates, 1 );
    splitter->SetFieldName( this->GetParameterStringList( "sample.vfn" )[0] );
    splitter->SetLayerIndex( 0 );
    splitter->SetOriginFieldName( std::string( "" ) );
    splitter->SetSamplerParameters( param );
    splitter->GetStreamer()->SetAutomaticTiledStreaming( static_cast<unsigned int>(this->GetParameterInt( "ram" )) );
    AddProcess( splitter->GetStreamer(), "Split samples between training and validation..." );
    splitter->Update();
  }


protected:

  struct SamplingRates
  {
    long int fmt;
    long int fmv;
  };

  /**
   * \class TrainFileNamesHandler
   * This class is used to store file names requires for the application's input and output.
   * And to clear temporary files generated by the applications
   * \ingroup OTBAppClassification
   */
  class TrainFileNamesHandler
  {
  public :
    void CreateTemporaryFileNames(std::string outModel, size_t nbInputs, bool dedicatedValidation)
    {

      if( dedicatedValidation )
        {
        rateTrainOut = outModel + "_ratesTrain.csv";
        }
      else
        {
        rateTrainOut = outModel + "_rates.csv";
        }

      rateValidOut = outModel + "_ratesValid.csv";
      for( unsigned int i = 0; i < nbInputs; i++ )
        {
        std::ostringstream oss;
        oss << i + 1;
        std::string strIndex( oss.str() );
        if( dedicatedValidation )
          {
          polyStatTrainOutputs.push_back( outModel + "_statsTrain_" + strIndex + ".xml" );
          polyStatValidOutputs.push_back( outModel + "_statsValid_" + strIndex + ".xml" );
          ratesTrainOutputs.push_back( outModel + "_ratesTrain_" + strIndex + ".csv" );
          ratesValidOutputs.push_back( outModel + "_ratesValid_" + strIndex + ".csv" );
          sampleOutputs.push_back( outModel + "_samplesTrain_" + strIndex + ".shp" );
          }
        else
          {
          polyStatTrainOutputs.push_back( outModel + "_stats_" + strIndex + ".xml" );
          ratesTrainOutputs.push_back( outModel + "_rates_" + strIndex + ".csv" );
          sampleOutputs.push_back( outModel + "_samples_" + strIndex + ".shp" );
          }
        sampleTrainOutputs.push_back( outModel + "_samplesTrain_" + strIndex + ".shp" );
        sampleValidOutputs.push_back( outModel + "_samplesValid_" + strIndex + ".shp" );
        }

    }

    void clear()
    {
      for( unsigned int i = 0; i < polyStatTrainOutputs.size(); i++ )
        RemoveFile( polyStatTrainOutputs[i] );
      for( unsigned int i = 0; i < polyStatValidOutputs.size(); i++ )
        RemoveFile( polyStatValidOutputs[i] );
      for( unsigned int i = 0; i < ratesTrainOutputs.size(); i++ )
        RemoveFile( ratesTrainOutputs[i] );
      for( unsigned int i = 0; i < ratesValidOutputs.size(); i++ )
        RemoveFile( ratesValidOutputs[i] );
      for( unsigned int i = 0; i < sampleOutputs.size(); i++ )
        RemoveFile( sampleOutputs[i] );
      for( unsigned int i = 0; i < sampleTrainOutputs.size(); i++ )
        RemoveFile( sampleTrainOutputs[i] );
      for( unsigned int i = 0; i < sampleValidOutputs.size(); i++ )
        RemoveFile( sampleValidOutputs[i] );
      for( unsigned int i = 0; i < tmpVectorFileList.size(); i++ )
        RemoveFile( tmpVectorFileList[i] );
    }

  public:
    std::vector<std::string> polyStatTrainOutputs;
    std::vector<std::string> polyStatValidOutputs;
    std::vector<std::string> ratesTrainOutputs;
    std::vector<std::string> ratesValidOutputs;
    std::vector<std::string> sampleOutputs;
    std::vector<std::string> sampleTrainOutputs;
    std::vector<std::string> sampleValidOutputs;
    std::vector<std::string> tmpVectorFileList;
    std::string rateValidOut;
    std::string rateTrainOut;

  private:
    bool RemoveFile(std::string &filePath)
    {
      bool res = true;
      if( itksys::SystemTools::FileExists( filePath.c_str() ) )
        {
        size_t posExt = filePath.rfind( '.' );
        if( posExt != std::string::npos && filePath.compare( posExt, std::string::npos, ".shp" ) == 0 )
          {
          std::string shxPath = filePath.substr( 0, posExt ) + std::string( ".shx" );
          std::string dbfPath = filePath.substr( 0, posExt ) + std::string( ".dbf" );
          std::string prjPath = filePath.substr( 0, posExt ) + std::string( ".prj" );
          RemoveFile( shxPath );
          RemoveFile( dbfPath );
          RemoveFile( prjPath );
          }
        res = itksys::SystemTools::RemoveFile( filePath.c_str() );
        if( !res )
          {
          //otbAppLogINFO( <<"Unable to remove file  "<<filePath );
          }
        }
      return res;
    }
  };

};

} // end namespace Wrapper
} // end namespace otb


#endif //otbTrainImagesBase_h
