#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbExtendedFilenameToReaderOptions);
  REGISTER_TEST(otbExtendedFilenameToWriterOptions);
  REGISTER_TEST(otbImageFileReaderWithExtendedFilename);
  REGISTER_TEST(otbImageFileWriterWithExtendedFilename);
}
