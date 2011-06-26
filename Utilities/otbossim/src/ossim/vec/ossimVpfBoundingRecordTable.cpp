#include <ossim/vec/ossimVpfBoundingRecordTable.h>

ossimVpfBoundingRecordTable::ossimVpfBoundingRecordTable()
   :ossimVpfTable()
{
}

bool ossimVpfBoundingRecordTable::openTable(const ossimFilename& tableName)
{
   bool result = false;
   theExtent = ossimVpfExtent(0,0,0,0);
   bool firstOneSetFlag = false;
   
   if(ossimVpfTable::openTable(tableName))
   {
      ossim_int32 xminIdx = getColumnPosition("XMIN");
      ossim_int32 yminIdx = getColumnPosition("YMIN");
      ossim_int32 xmaxIdx = getColumnPosition("XMAX");
      ossim_int32 ymaxIdx = getColumnPosition("YMAX");
      
      if((xminIdx < 0)||
         (yminIdx < 0)||
         (xmaxIdx < 0)||
         (ymaxIdx < 0))
      {
         closeTable();
      }
      else
      {
         if(getNumberOfRows() > 0)
         {
            result = true;
            reset();
            ossim_int32 n = 1;
            
            ossim_float32 xmin;
            ossim_float32 ymin;
            ossim_float32 xmax;
            ossim_float32 ymax;
            
            row_type row;
            for(int rowIdx = 1; rowIdx < getNumberOfRows(); ++rowIdx)
            {
               if(rowIdx == 1)
               {
                  row = read_row(rowIdx,
                                 *theTableInformation);
               }
               else
               {
                  row = read_next_row(*theTableInformation);
               }
               get_table_element(xminIdx,
                                 row,
                                 *theTableInformation,
                                 &xmin,
                                 &n);
               get_table_element(yminIdx,
                                 row,
                                 *theTableInformation,
                                 &ymin,
                                 &n);
               get_table_element(xmaxIdx,
                                 row,
                                 *theTableInformation,
                                 &xmax,
                                 &n);
               get_table_element(ymaxIdx,
                                 row,
                                 *theTableInformation,
                                 &ymax,
                                 &n);
               if(!is_vpf_null_float(xmin)&&
                  !is_vpf_null_float(ymin)&&
                  !is_vpf_null_float(xmax)&&
                  !is_vpf_null_float(ymax))
               {
                  if(!firstOneSetFlag)
                  {
                     theExtent = ossimVpfExtent(xmin,
                                                ymin,
                                                xmax,
                                                ymax);
                     firstOneSetFlag = true;
                  }
                  else
                  {
                     theExtent = theExtent + ossimVpfExtent(xmin,
                                                            ymin,
                                                            xmax,
                                                            ymax);
                  }                  
               }
               free_row(row, *theTableInformation);
            }
         }
      }
   }
   return result;
}

void ossimVpfBoundingRecordTable::getExtent(ossimVpfExtent& extent)const
{
   extent = theExtent;
}
