#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbImageFileWriter.h"
#include "otbCommandLineArgumentParser.h"
#include "itkEuclideanDistance.h"
#include "itkImageRegionSplitter.h"
#include "otbStreamingTraits.h"
#include "otbKMeansImageClassificationFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkListSample.h"
#include "itkWeightedCentroidKdTreeGenerator.h"
#include "itkKdTreeBasedKmeansEstimator.h"

int main(int argc, char * argv[])
{

  // Parse command line parameters
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();

  parser->SetProgramDescription("Unsupervised KMeans image classification");
  parser->AddInputImage();
  parser->AddOutputImage();
  parser->AddOption("--ValidityMask","Validity mask","-vm", 1, true);
  parser->AddOption("--MaxTrainingSetSize","Size of the training set","-ts", 1, true);
  parser->AddOption("--TrainingSetProbability","Probability for a sample to be selected in the training set","-tp", 1, true);
  parser->AddOption("--NumberOfClasses","Number of classes","-nc", 1, true);
  parser->AddOption("--InitialCentroidProbability","Probability for a pixel to be selected as an initial class centroid","-cp", 1, true);
  parser->AddOption("--StreamingNumberOfLines","Number of lined for each streaming block","-sl", 1, true);


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
  const unsigned int nbsamples = parseResult->GetParameterUInt("--MaxTrainingSetSize");
  const double trainingProb =parseResult->GetParameterDouble("--TrainingSetProbability");
  const double initProb = parseResult->GetParameterDouble("--InitialCentroidProbability");
  const unsigned int nbLinesForStreaming = parseResult->GetParameterUInt("--StreamingNumberOfLines");
  const unsigned int nb_classes = parseResult->GetParameterUInt("--NumberOfClasses");

  typedef unsigned short PixelType;
  typedef unsigned short LabeledPixelType;

  typedef otb::VectorImage<PixelType, 2> ImageType;
  typedef otb::Image<LabeledPixelType, 2> LabeledImageType;
  typedef otb::ImageFileReader<ImageType> ImageReaderType;
  typedef otb::ImageFileReader<LabeledImageType> LabeledImageReaderType;
  typedef otb::StreamingImageFileWriter<LabeledImageType> WriterType;

  typedef itk::FixedArray<PixelType, 108> SampleType;
  typedef itk::Statistics::ListSample<SampleType> ListSampleType;
  typedef itk::Statistics::WeightedCentroidKdTreeGenerator<ListSampleType>   TreeGeneratorType;
  typedef  TreeGeneratorType::KdTreeType                                     TreeType;
  typedef itk::Statistics::KdTreeBasedKmeansEstimator<TreeType>              EstimatorType;

  typedef otb::StreamingTraits<ImageType> StreamingTraitsType;
  typedef itk::ImageRegionSplitter<2>  SplitterType;
  typedef ImageType::RegionType RegionType;

  typedef itk::ImageRegionConstIterator<ImageType> IteratorType;
  typedef itk::ImageRegionConstIterator<LabeledImageType> LabeledIteratorType;

  typedef otb::KMeansImageClassificationFilter<ImageType, LabeledImageType, 108> ClassificationFilterType;


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
  EstimatorType::ParametersType initialMeans(108*nb_classes);
  initialMeans.Fill(0);
  unsigned int init_means_index = 0;

  // Sample dimension and max dimension
  unsigned int maxDimension = SampleType::Dimension;
  unsigned int sampleSize = std::min(reader->GetOutput()->GetNumberOfComponentsPerPixel(), maxDimension);
  unsigned int totalSamples = 0;
  std::cout<<"Sample max possible dimension: "<<maxDimension<<std::endl;
  std::cout<<"The following sample size will be used: "<<sampleSize<<std::endl;
  std::cout<<std::endl;
  // local streaming variables
  unsigned int piece = 0;
  RegionType streamingRegion;

  while ((totalSamples<nbsamples)&&(init_means_index<108*nb_classes))
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
    while (!it.IsAtEnd()&&!maskIt.IsAtEnd()&&(totalSamples<nbsamples)&&(init_means_index<108*nb_classes))
    {
      // If the current pixel is labeled
      if (maskIt.Get()>0)
      {
        if ((rand()<trainingProb*RAND_MAX))
        {
          SampleType newSample;

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
        else if ((init_means_index<108*nb_classes)&&(rand()<initProb*RAND_MAX))
        {
          for (unsigned int i = 0; i<sampleSize; ++i)
          {
            initialMeans[init_means_index+i]=it.Get()[i];
          }
          init_means_index += 108;
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
  std::cout<<std::endl;

  std::cout<<"Initial centroids are: "<<std::endl;

  for (unsigned int i=0; i<nb_classes; ++i)
  {
    std::cout<<"Class "<<i<<": ";
    for (unsigned int j = 0; j<sampleSize; ++j)
    {
      std::cout<<initialMeans[i*108+j]<<"\t";
    }
    std::cout<<std::endl;
  }
  std::cout<<std::endl;

  std::cout<<"Starting optimization."<<std::endl;
  std::cout<<std::endl;
  EstimatorType::Pointer estimator = EstimatorType::New();

  TreeGeneratorType::Pointer treeGenerator = TreeGeneratorType::New();
  treeGenerator->SetSample(sampleList);
  treeGenerator->SetBucketSize(100);
  treeGenerator->Update();

  estimator->SetParameters(initialMeans);
  estimator->SetKdTree(treeGenerator->GetOutput());
  estimator->SetMaximumIteration(100000000);
  estimator->SetCentroidPositionChangesThreshold(0.001);
  estimator->StartOptimization();

  EstimatorType::ParametersType estimatedMeans = estimator->GetParameters();

  std::cout<<"Optimization completed."<<std::endl;
  std::cout<<std::endl;
  std::cout<<"Estimated centroids are: "<<std::endl;

  for (unsigned int i=0; i<nb_classes; ++i)
  {
    std::cout<<"Class "<<i<<": ";
    for (unsigned int j = 0; j<sampleSize; ++j)
    {
      std::cout<<estimatedMeans[i*108+j]<<"\t";
    }
    std::cout<<std::endl;
  }

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
  classifier->SetCentroids(estimator->GetParameters());

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outfname);
  writer->SetInput(classifier->GetOutput());
  writer->SetNumberOfStreamDivisions(numberOfStreamDivisions);
  writer->Update();

  std::cout<<"Classification completed."<<std::endl;
  std::cout<<std::endl;

  return EXIT_SUCCESS;
}
