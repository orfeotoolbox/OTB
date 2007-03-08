#ifndef ossimVpfBoundingRecordTable_HEADER
#define ossimVpfBoundingRecordTable_HEADER
#include <ossim/vec/ossimVpfTable.h>
#include <ossim/vec/ossimVpfExtent.h>

class ossimVpfBoundingRecordTable : public ossimVpfTable
{
public:
   ossimVpfBoundingRecordTable();
   virtual bool openTable(const ossimFilename& tableName);

   void getExtent(ossimVpfExtent& extent)const;

protected:
   ossimVpfExtent theExtent;
};

#endif
