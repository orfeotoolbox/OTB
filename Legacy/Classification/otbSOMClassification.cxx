#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbImageFileWriter.h"
#include "otbSOMMap.h"
#include "otbSOM.h"
#include "otbSOMImageClassificationFilter.h"
#include "otbCommandLineArgumentParser.h"
#include "itkEuclideanDistance.h"
#include "itkImageRegionSplitter.h"
#include "otbStreamingTraits.h"
#include "itkImageRegionConstIterator.h"
#include "itkVariableSizeMatrix.h"
#include "itkListSample.h"
#include "itkVariableLengthVector.h"

int main(int argc, char * argv[])
{

  // Parse command line parameters
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();

  parser->SetProgramDescription("Unsupervised Self Organizing Map image classification");
  parser->AddInputImage();
  parser->AddOutputImage();
  parser->AddOption("--ValidityMask","Validity mask","-vm", 1, true);
  parser->AddOption("--MaxTrainingSetSize","Size of the training set","-ts", 1, true);
  parser->AddOption("--TrainingSetProbability","Probability for a sample to be selected in the training set","-tp", 1, true);
  parser->AddOption("--StreamingNumberOfLines","Number of lined for each streaming block","-sl", 1, true);
  parser->AddOption("--SOMMap","Output SOM map","-sm", 1, true);
  parser->AddOption("--SizeX","X size of the SOM map","-sx", 1, true);
  parser->AddOption("--SizeY","Y size of the SOM map","-sy", 1, true);
  parser->AddOption("--NeighborhoodInitX","X initial neighborhood of the SOM map","-nx", 1, true);
  parser->AddOption("--NeighborhoodInitY","Y initial neighborhood of the SOM map","-ny", 1, true);
  parser->AddOption("--NumberOfIterations","Number of iterations of the SOM learning","-ni", 1, true);
  parser->AddOption("--BetaInit","Initial beta value","-bi", 1, true);
  parser->AddOption("--BetaFinal","Final beta value","-be", 1, true);
  parser->AddOption("--InitValue","Initial value","-iv", 1, true);


  typedef otb::CommandLineArgumentParseResult ParserResultType;
  ParserResultType::Pointer  parseResult = ParserResultType::New();

  try
  {
    parser->ParseCommandLine(argc, argv, parseResult);
  }
  catch ( itk::ExceptionObject & err )
  {
    std::string descriptionException = err.GetDescription();
    if (descriptionException.find("ParseCommandLine(): Help Parser") != std::string::npos)
    {
      return EXIT_SUCCESS;
    }
    if (descriptionException.find("ParseCommandLine(): Version Parser") != std::string::npos)
    {
      return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
  }

  // initiating random number generation
  srand(time(NULL));

  std::string infname = parseResult->GetInputImage();
  std::string maskfname = parseResult->GetParameterString("--ValidityMask", 0);
  std::string outfname = parseResult->GetOutputImage();
  std::string somfname = parseResult->GetParameterString("--SOMMap", 0);
  const unsigned int nbsamples = parseResult->GetParameterUInt("--MaxTrainingSetSize");
  const double trainingProb =parseResult->GetParameterDouble("--TrainingSetProbability");
  const unsigned int nbLinesForStreaming = parseResult->GetParameterUInt("--StreamingNumberOfLines");
  const unsigned int sizeX = parseResult->GetParameterUInt("--SizeX");
  const unsigned int sizeY = parseResult->GetParameterUInt("--SizeY");
  const unsigned int neighInitX = parseResult->GetParameterUInt("--NeighborhoodInitX");
  const unsigned int neighInitY= parseResult->GetParameterUInt("--NeighborhoodInitY");
  const unsigned int nbIterations=parseResult->GetParameterUInt("--NumberOfIterations");
  const double betaInit = parseResult->GetParameterDouble("--BetaInit");
  const double betaEnd= parseResult->GetParameterDouble("--BetaFinal");
  const float initValue = parseResult->GetParameterFloat("--InitValue");

  typedef float PixelType;
  typedef unsigned short LabeledPixelType;

  typedef otb::VectorImage<PixelType, 2> ImageType;
  typedef otb::Image<LabeledPixelType, 2> LabeledImageType;
  typedef otb::ImageFileReader<ImageType> ImageReaderType;
  typedef otb::ImageFileReader<LabeledImageType> LabeledImageReaderType;
  typedef otb::StreamingImageFileWriter<LabeledImageType> WriterType;

  typedef itk::VariableLengthVector<double> SampleType;
  typedef itk::Statistics::EuclideanDistance<SampleType> DistanceType;
  typedef otb::SOMMap<SampleType, DistanceType, 2> SOMMapType;
  typedef itk::Statistics::ListSample<SampleType> ListSampleType;
  typedef otb::SOM<ListSampleType, SOMMapType> EstimatorType;
  typedef otb::ImageFileWriter<ImageType> SOMMapWriterType;

  typedef otb::StreamingTraits<ImageType> StreamingTraitsType;
  typedef itk::ImageRegionSplitter<2>  SplitterType;
  typedef ImageType::RegionType RegionType;

  typedef itk::ImageRegionConstIterator<ImageType> IteratorType;
  typedef itk::ImageRegionConstIterator<LabeledImageType> LabeledIteratorType;
  typedef itk::ImageRegionConstIterator<SOMMapType> SOMIteratorType;

  typedef otb::SOMImageClassificationFilter<ImageType, LabeledImageType, SOMMapType> ClassificationFilterType;


  ImageReaderType::Pointer reader = ImageReaderType::New();
  LabeledImageReaderType::Pointer maskReader = LabeledImageReaderType::New();

  reader->SetFileName(infname);
  maskReader->SetFileName(maskfname);

  /*******************************************/
  /*           Sampling data                 */
  /*******************************************/
  std::cout<<std::endl;
  std::cout<<"-- SAMPLING DATA --"<<std::endl;
  std::cout<<std::endl;

  // Update input images information
  reader->GenerateOutputInformation();
  maskReader->GenerateOutputInformation();

  if (reader->GetOutput()->GetLargestPossibleRegion()
      != maskReader->GetOutput()->GetLargestPossibleRegion()
     )
  {
    std::cerr<<"Mask image and input image have different sizes."<<std::endl;
    return EXIT_FAILURE;
  }

  RegionType largestRegion = reader->GetOutput()->GetLargestPossibleRegion();

  // Setting up local streaming capabilities
  SplitterType::Pointer splitter = SplitterType::New();
  unsigned int numberOfStreamDivisions = StreamingTraitsType::CalculateNumberOfStreamDivisions(reader->GetOutput(),
                                         largestRegion,
                                         splitter,
                                         otb::SET_BUFFER_NUMBER_OF_LINES,
                                         0, 0, nbLinesForStreaming);

  std::cout<<"The images will be streamed into "<<numberOfStreamDivisions<<" parts."<<std::endl;

  // Training sample lists
  ListSampleType::Pointer sampleList = ListSampleType::New();

  // Sample dimension and max dimension
  unsigned int sampleSize = reader->GetOutput()->GetNumberOfComponentsPerPixel();
  unsigned int totalSamples = 0;
  std::cout<<"The following sample size will be used: "<<sampleSize<<std::endl;
  std::cout<<std::endl;
  // local streaming variables
  unsigned int piece = 0;
  RegionType streamingRegion;

  while (totalSamples<nbsamples)
  {
    piece = static_cast<unsigned int>(static_cast<double>(numberOfStreamDivisions)*rand()/(RAND_MAX));

    streamingRegion = splitter->GetSplit(piece, numberOfStreamDivisions, largestRegion);

    std::cout<<"Processing region: "<<streamingRegion<<std::endl;

    reader->GetOutput()->SetRequestedRegion(streamingRegion);
    reader->GetOutput()->PropagateRequestedRegion();
    reader->GetOutput()->UpdateOutputData();

    maskReader->GetOutput()->SetRequestedRegion(streamingRegion);
    maskReader->GetOutput()->PropagateRequestedRegion();
    maskReader->GetOutput()->UpdateOutputData();

    IteratorType it(reader->GetOutput(), streamingRegion);
    LabeledIteratorType maskIt(maskReader->GetOutput(), streamingRegion);

    it.GoToBegin();
    maskIt.GoToBegin();

    unsigned int localNbSamples=0;

    // Loop on the image
    while (!it.IsAtEnd()&&!maskIt.IsAtEnd()&&(totalSamples<nbsamples))
    {
      // If the current pixel is labeled
      if (maskIt.Get()>0)
      {
        if ((rand()<trainingProb*RAND_MAX))
        {
          SampleType newSample;
          newSample.SetSize(sampleSize);
          // build the sample
          newSample.Fill(0);
          for (unsigned int i = 0; i<sampleSize; ++i)
          {
            newSample[i]=it.Get()[i];
          }
          // Update the the sample lists
          sampleList->PushBack(newSample);
          ++totalSamples;
          ++localNbSamples;
        }
      }
      ++it;
      ++maskIt;
    }
    std::cout<<localNbSamples<<" samples added to the training set."<<std::endl;
    std::cout<<std::endl;
  }

  std::cout<<"The final training set contains "<<totalSamples<<" samples."<<std::endl;


  std::cout<<std::endl;
  std::cout<<"Data sampling completed."<<std::endl;
  std::cout<<std::endl;

  /*******************************************/
  /*           Learning                      */
  /*******************************************/

  std::cout<<"-- LEARNING --"<<std::endl;

  EstimatorType::Pointer estimator = EstimatorType::New();

  estimator->SetListSample(sampleList);
  EstimatorType::SizeType size;
  size[0]=sizeX;
  size[1]=sizeY;
  estimator->SetMapSize(size);
  EstimatorType::SizeType radius;
  radius[0] = neighInitX;
  radius[1] = neighInitY;
  estimator->SetNeighborhoodSizeInit(radius);
  estimator->SetNumberOfIterations(nbIterations);
  estimator->SetBetaInit(betaInit);
  estimator->SetBetaEnd(betaEnd);
  estimator->SetMaxWeight(initValue);
//  estimator->SetRandomInit(true);
//   estimator->SetSeed(time(NULL));
  estimator->Update();

  ImageType::Pointer vectormap = ImageType::New();
  vectormap->SetRegions(estimator->GetOutput()->GetLargestPossibleRegion());
  vectormap->SetNumberOfComponentsPerPixel(108);
  vectormap->Allocate();

  ImageType::PixelType black;
  black.SetSize(108);
  black.Fill(0);
  vectormap->FillBuffer(black);

  SOMIteratorType somIt(estimator->GetOutput(), estimator->GetOutput()->GetLargestPossibleRegion());
  IteratorType vectorIt(vectormap, estimator->GetOutput()->GetLargestPossibleRegion());

  somIt.GoToBegin();
  vectorIt.GoToBegin();

  while (!somIt.IsAtEnd() && !vectorIt.IsAtEnd())
  {
    for (unsigned int i = 0; i<somIt.Get().GetSize(); ++i)
    {
      vectorIt.Get()[i]=somIt.Get()[i];
    }
    ++somIt;
    ++vectorIt;
  }

  SOMMapWriterType::Pointer somWriter = SOMMapWriterType::New();
  somWriter->SetFileName(somfname);
  somWriter->SetInput(vectormap);
  somWriter->Update();

  std::cout<<std::endl;
  std::cout<<"Learning completed."<<std::endl;
  std::cout<<std::endl;


  /*******************************************/
  /*           Classification                */
  /*******************************************/

  std::cout<<"-- CLASSIFICATION --"<<std::endl;
  std::cout<<std::endl;
  ClassificationFilterType::Pointer classifier = ClassificationFilterType::New();
  classifier->SetInput(reader->GetOutput());
  classifier->SetInputMask(maskReader->GetOutput());
  classifier->SetMap(estimator->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outfname);
  writer->SetInput(classifier->GetOutput());
  writer->SetNumberOfStreamDivisions(numberOfStreamDivisions);
  writer->Update();

  std::cout<<"Classification completed."<<std::endl;
  std::cout<<std::endl;

  return EXIT_SUCCESS;
}
