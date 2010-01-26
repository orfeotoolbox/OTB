#ifndef ossimVpfLibraryAttributeTableValidator_HEADER
#define ossimVpfLibraryAttributeTableValidator_HEADER
#include <ossim/vec/ossimVpfTableValidator.h>

class ossimVpfTable;
class ossimVpfLibraryAttributeTableValidator :public ossimVpfTableValidator
{
public:
   virtual bool isValid(ossimVpfTable& aTable)const;
};

#endif
