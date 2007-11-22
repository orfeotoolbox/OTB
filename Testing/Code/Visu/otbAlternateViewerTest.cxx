#include "otbVectorImage.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageAlternateViewer.h"
#include "Fl/Fl.h"


int otbAlternateViewerTest(int argc, char* argv[])
{
  const unsigned int Dimension = 2;
  typedef double PixelType;
  typedef otb::VectorImage<PixelType,Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageAlternateViewer<PixelType> ViewerType;
  
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->GenerateOutputInformation();
  
  ImageType::PixelType min,max;
  min.SetSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  max.SetSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());
//   min[0]=195;
//   min[1]=241;
//   min[2]=127;
//   min[3]=130;
//   max[0]=387;
//   max[1]=602;
//   max[2]=469;
//   max[3]=740;
    min.Fill(0);
    max.Fill(255);
  
 //  RescalerType::Pointer rescaler = RescalerType::New();
//   rescaler->SetOutputMinimum(min);
//   rescaler->SetOutputMaximum(max);
//   rescaler->SetInput(reader->GetOutput());
//   rescaler->SetClampThreshold(atof(argv[2]));
//   rescaler->GenerateOutputInformation();

  Fl_Window window(512,512);
  ViewerType::Pointer viewer = ViewerType::New();
  viewer->SetImage(reader->GetOutput());
  viewer->SetMinComponentValues(min);
  viewer->SetMaxComponentValues(max);
  viewer->SetRedChannelIndex(atoi(argv[2]));
  viewer->SetGreenChannelIndex(atoi(argv[3]));
  viewer->SetBlueChannelIndex(atoi(argv[4]));
  window.end();
  window.resizable(viewer.GetPointer());
  viewer->Init("test de la nouvelle visu");
  window.show();
  viewer->Show();
  

   Fl::check();

//    window.resize(0,0,600,400);
//   Fl::check();
//    window.resize(0,0,600,600);
//   Fl::check();
//   window.resize(0,0,650,500);
//   Fl::check();
//   window.resize(0,0,700,500);
//   Fl::check();
//   window.resize(0,0,750,500);
//   Fl::check();
//   window.resize(0,0,800,500);
//   Fl::check();


//   window.resize(0,0,521,481);

  Fl::run();
  

  return EXIT_SUCCESS;
}
