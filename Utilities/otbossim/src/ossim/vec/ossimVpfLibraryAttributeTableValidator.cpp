#include <ossim/vec/ossimVpfLibraryAttributeTableValidator.h>
#include <ossim/vec/ossimVpfTable.h>

bool ossimVpfLibraryAttributeTableValidator::isValid(ossimVpfTable& aTable)const
{
   // make sure the table is not null
   if(&aTable == NULL)
   {
      return false;
   }

   // and make sure that the table is not closed
   if(aTable.isClosed())
   {
      return false;
   }

   // get the table data defined in vpf_util/vpftable.h"
   const vpf_table_type* data = aTable.getVpfTableData();

   if(!data)
   {
      return false;
   }
   // if the file is not open then we can't validate
   if(!data->fp)
   {
      return false;
   }

   // The way that we will validate is to see if the columns of a dht
   // exist.  if the column didn't exist then the position
   // will be negative
   long column = table_pos("ID", *data);
   if(column < 0)
   {
      return false;
   }
   column = table_pos("LIBRARY_NAME", *data);
   if(column < 0)
   {
      return false;
   }
   column = table_pos("XMIN", *data);
   if(column < 0)
   {
      return false;
   }
   column = table_pos("YMIN", *data);
   if(column < 0)
   {
      return false;
   }
   column = table_pos("XMAX", *data);
   if(column < 0)
   {
      return false;
   }
   column = table_pos("YMAX", *data);
   if(column < 0)
   {
      return false;
   }

   return true;
}
