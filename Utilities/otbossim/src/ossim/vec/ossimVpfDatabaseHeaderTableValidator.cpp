//-----------------------------------------------------------------
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//-----------------------------------------------------------------
#include <ossim/vec/ossimVpfDatabaseHeaderTableValidator.h>
#include <ossim/vec/ossimVpfTable.h>

ossimVpfDatabaseHeaderTableValidator::~ossimVpfDatabaseHeaderTableValidator()
{
}

bool ossimVpfDatabaseHeaderTableValidator::isValid(ossimVpfTable& aTable)const
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
   column = table_pos("VPF_VERSION", *data);
   if(column < 0)
   {
      return false;
   }
   column = table_pos("DATABASE_NAME", *data);
   if(column < 0)
   {
      return false;
   }
   column = table_pos("DATABASE_DESC", *data);
   if(column < 0)
   {
      return false;
   }
   column = table_pos("MEDIA_STANDARD", *data);
   if(column < 0)
   {
      return false;
   }
   column = table_pos("ORIGINATOR", *data);
   if(column < 0)
   {
      return false;
   }
   column = table_pos("ADDRESSEE", *data);
   if(column < 0)
   {
      return false;
   }
   column = table_pos("MEDIA_VOLUMES", *data);
   if(column < 0)
   {
      return false;
   }
   column = table_pos("SEQ_NUMBERS", *data);
   if(column < 0)
   {
      return false;
   }
   column = table_pos("NUM_DATA_SETS", *data);
   if(column < 0)
   {
      return false;
   }
   column = table_pos("SECURITY_CLASS", *data);
   if(column < 0)
   {
      return false;
   }
   column = table_pos("DOWNGRADING", *data);
   if(column < 0)
   {
      return false;
   }
   column = table_pos("DOWNGRADE_DATE", *data);
   if(column < 0)
   {
      return false;
   }
   column = table_pos("RELEASABILITY", *data);
   if(column < 0)
   {
      return false;
   }
   column = table_pos("TRANSMITTAL_ID", *data);
   if(column < 0)
   {
      return false;
   }
   column = table_pos("EDITION_NUMBER", *data);
   if(column < 0)
   {
      return false;
   }
   column = table_pos("EDITION_DATE", *data);
   if(column < 0)
   {
      return false;
   }
   
   return true;

}
