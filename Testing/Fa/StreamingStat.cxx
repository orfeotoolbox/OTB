#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingStatisticsImageFilter.h"
#include "otbVectorImageTo3DScalarImageFilter.h"

int main(int argc, char ** argv)
{
  typedef otb::Image<double, 3>                 Image3DType;
  typedef otb::VectorImage<double, 2>           VectorImageType;
  typedef otb::ImageFileReader<VectorImageType> ReaderType;

  ReaderType::Pointer reader1 = ReaderType::New();
  reader1->SetFileName(argv[1]);

  typedef otb::VectorImageTo3DScalarImageFilter<VectorImageType, Image3DType>
  VectorImageTo3DScalarImageFilterType;
  VectorImageTo3DScalarImageFilterType::Pointer filter1 =
    VectorImageTo3DScalarImageFilterType::New();

  typedef otb::StreamingStatisticsImageFilter<Image3DType> statFilterType;
  statFilterType::Pointer statFilter = statFilterType::New();

  filter1->SetInput(reader1->GetOutput());
//   filter1->Update();
  statFilter->SetInput(filter1->GetOutput());
  statFilter->Update();

  std::cout << statFilter->GetMean() << std::endl;

}
