//-----------------------------------------------------------------
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//-----------------------------------------------------------------
#ifndef ossimVpfDatabaseHeaderTableValidator_HEADER
#define ossimVpfDatabaseHeaderTableValidator_HEADER
#include <ossim/vec/ossimVpfTableValidator.h>
class ossimVpfTable;
class ossimVpfDatabaseHeaderTableValidator : public ossimVpfTableValidator
{
public:
   virtual ~ossimVpfDatabaseHeaderTableValidator();
   virtual bool isValid(ossimVpfTable& aTable)const;
};

#endif
