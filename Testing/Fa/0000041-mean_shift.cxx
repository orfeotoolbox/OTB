#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbMeanShiftVectorImageFilter.h"

//Code adapted from submission from Christophe Simler
// http://bugs.orfeo-toolbox.org/view.php?id=41

int main(int argc, char *argv[])
{

  if (argc < 9)
    {
    std::cout <<
    "Usage : inputImage rangeRadius spatialRadius minRegionSize outfilenamefiltered outfilenamesegmented outfilenamelabeled outfilenameboundary"
              << std::endl;

    return EXIT_FAILURE;
    }

  char * filename = argv[1];
  int    rangeRadius = atoi(argv[2]);
  int    spatialRadius = atoi(argv[3]);
  int    minRegionSize = atoi(argv[4]);
  char * outfilenamefiltered = argv[5];
  char * outfilenamesegmented = argv[6];
  char * outfilenamelabeled = argv[7];
  char * outfilenameboundary = argv[8];

  typedef otb::VectorImage<unsigned char, 2> ImageType;               // image d'entree, image filtree et image segmente
  typedef otb::Image<int, 2>                 TLabeledOutput;          // image labelisee, image des contours (de l'image labellisee)

  // lecture de l'image d'entree a partir d'un fichier
  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(filename);

  // traitement avec le filtre
  typedef otb::MeanShiftVectorImageFilter<ImageType, ImageType, TLabeledOutput> FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetRangeRadius(rangeRadius);
  filter->SetSpatialRadius(spatialRadius);
  filter->SetMinimumRegionSize(minRegionSize);

  // sauvegarde de l'image filtree,
  typedef otb::StreamingImageFileWriter<ImageType> WriterType1;
  WriterType1::Pointer writer1 = WriterType1::New();
  writer1->SetFileName(outfilenamefiltered);

  // sauvegarde de l'image segmente,
  typedef otb::StreamingImageFileWriter<ImageType> WriterType2;
  WriterType2::Pointer writer2 = WriterType2::New();
  writer2->SetFileName(outfilenamesegmented);

  // sauvegarde de l'image labelisee
  typedef otb::StreamingImageFileWriter<TLabeledOutput> WriterType3;
  WriterType3::Pointer writer3 = WriterType3::New();
  writer3->SetFileName(outfilenamelabeled);

  // sauvegarde de l'image de contours
  typedef otb::StreamingImageFileWriter<TLabeledOutput> WriterType4;
  WriterType4::Pointer writer4 = WriterType4::New();
  writer4->SetFileName(outfilenameboundary);

  // construction du pipeline
  filter->SetInput(reader->GetOutput());

  writer1->SetInput(filter->GetOutput());                  // image filtree (*)
  writer2->SetInput(filter->GetClusteredOutput());         // image segmente (clusterisee avec coherence spaciale ?) (*)
  writer3->SetInput(filter->GetLabeledClusteredOutput());  // image des labels des clusters
  writer4->SetInput(filter->GetClusterBoundariesOutput()); // image des contours des clusters (contours de l'image labelisee)

  writer1->Update();
  writer2->Update();
  writer3->Update();
  writer4->Update();

  return 0;
}
