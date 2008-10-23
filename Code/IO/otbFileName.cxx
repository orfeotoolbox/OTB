#include "otbFileName.h"

namespace otb
{

FileName::FileName() : ossimFilename()
{}

FileName::FileName(const FileName& src)
 : ossimFilename(src)
{}

FileName::FileName(const ossimFilename& src)
 : ossimFilename(src)
{}

FileName::FileName(const ossimString& src)
 : ossimFilename(src)
{}

FileName::FileName(const char* src)
 : ossimFilename(src)
{}


FileName FileName::ObtainFileNameWithNoExtension() const
{
	return this->fileNoExtension();
}

} // end namespace otb
