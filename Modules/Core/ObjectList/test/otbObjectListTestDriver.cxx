#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbObjectList);
  REGISTER_TEST(otbObjectListNew);
  REGISTER_TEST(otbImageList);
  REGISTER_TEST(otbImageListNew);
  REGISTER_TEST(otbImageListSourceNew);
  REGISTER_TEST(otbImageListToImageFilterNew);
  REGISTER_TEST(otbImageListToImageListApplyFilter);
  REGISTER_TEST(otbImageListToImageListApplyFilterNew);
  REGISTER_TEST(otbImageListToImageListFilterNew);
  REGISTER_TEST(otbImageListToVectorImageFilter2);
  REGISTER_TEST(otbImageListToVectorImageFilter);
  REGISTER_TEST(otbImageListToVectorImageFilterNew);
  REGISTER_TEST(otbObjectList2);
  REGISTER_TEST(otbObjectListToObjectListFilterNew);
  REGISTER_TEST(otbImageToImageListFilterNew);
  REGISTER_TEST(otbVectorImageToImageListFilter);
  REGISTER_TEST(otbVectorImageToImageListFilterNew);
  REGISTER_TEST(otbVectorImageToImageListFilterIterator);
}
