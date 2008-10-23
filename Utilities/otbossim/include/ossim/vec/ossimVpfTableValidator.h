//-----------------------------------------------------------------
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//-----------------------------------------------------------------
#ifndef ossimVpfTableValidator_HEADER
#define ossimVpfTableValidator_HEADER

class ossimVpfTable;

class ossimVpfTableValidator
{
public:
   virtual ~ossimVpfTableValidator(){}
   virtual bool isValid(ossimVpfTable& aTable)const=0;
};

#endif
