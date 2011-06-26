//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: This class give the capability to access tiles from an
//              vpf file.
//
//********************************************************************
// $Id: ossimVpfTable.cpp 19636 2011-05-24 16:48:45Z gpotts $
#include <ossim/vec/ossimVpfTable.h>
#include <ossim/vec/vpf.h>
#include <ossim/base/ossimErrorCodes.h>
#include <cstdlib>
#include <cstring>

std::ostream& operator <<(std::ostream& out,
                     const ossimVpfTable& data)
{
   data.print(out);
   return out;
}

ossimVpfTable::ossimVpfTable()
   :theTableInformation(NULL)
{
}

ossimVpfTable::~ossimVpfTable()
{
   if(theTableInformation)
   {
      closeTable();
      delete theTableInformation;
      theTableInformation = NULL;
   }
}

bool ossimVpfTable::openTable(const ossimFilename& tableName)
{
   closeTable();

   if(is_vpf_table(const_cast<char*>(tableName.c_str())))
   {
      if(theTableInformation)
      {
         delete theTableInformation;
         theTableInformation = NULL;
      }
      theTableInformation = new vpf_table_type;
      memset(theTableInformation, 0, sizeof(vpf_table_type));

      theTableName = tableName;
      *theTableInformation = vpf_open_table(const_cast<char*>(tableName.c_str()),
                                            disk,
                                            "rb",
                                            NULL);
   }
   else
   {
      delete theTableInformation;
      theTableInformation = NULL;

      return false;
   }

   return true;
}

void ossimVpfTable::closeTable()
{
   if(theTableInformation&&(theTableInformation->status!=CLOSED))
   {
      vpf_close_table(theTableInformation);
   }
}

int ossimVpfTable::getNumberOfRows()const
{
  if(theTableInformation) return theTableInformation->nrows;

  return 0;
}

int ossimVpfTable::getNumberOfColumns()const
{
  if(theTableInformation) return theTableInformation->nfields;

  return 0;
}

ossimString ossimVpfTable::getColumnName(int idx)const
{
  if((idx < 0) || idx > getNumberOfColumns())
    {
      return ossimString("");
    }
  return ossimString(theTableInformation->header[idx].name);
}

bool ossimVpfTable::isClosed()const
{
   return (theTableInformation &&
           (theTableInformation->status==CLOSED));
}

void ossimVpfTable::reset()const
{
   if(theTableInformation &&
      (theTableInformation->status!=CLOSED))
   {
      long fpos;

      fpos = index_pos(0, *theTableInformation);
      fseek(theTableInformation->fp, fpos, SEEK_SET);
   }
}

bool ossimVpfTable::goToRow(long row)const
{
   if(theTableInformation &&
      (theTableInformation->status!=CLOSED))
   {
      long fpos;

      fpos = index_pos(row, *theTableInformation);
      if(fpos < 0)
      {
         return false;
      }
      fseek(theTableInformation->fp, fpos, SEEK_SET);
   }

   return true;
}

std::vector<ossimString> ossimVpfTable::getColumnValues(const ossimString& columnName)const
{
	std::vector<ossimString> result;

   if(theTableInformation &&
      theTableInformation->status == OPENED)
   {
      long int columnNumber = table_pos(const_cast<char*>(columnName.c_str()), *theTableInformation);

      if(columnNumber >=0 &&
         (columnNumber < theTableInformation->nfields))
      {
         row_type row;
         // start at the first row of the table
         reset();
         for (int rowIndex = 1;
              rowIndex <= theTableInformation->nrows;
              ++rowIndex)
         {
            row = read_next_row(*theTableInformation);
            ossimString value = getColumnValueAsString(row,
                                                       columnNumber);
	    value.trim();
            result.push_back(value);
            free_row(row, *theTableInformation);
         }
      }
   }

   return result;
}

std::vector<ossimString> ossimVpfTable::getColumnValues(long columnNumber)const
{
	std::vector<ossimString> result;

   if(theTableInformation &&
      theTableInformation->status == OPENED)
   {
      if(columnNumber >=0 &&
         (columnNumber < theTableInformation->nfields))
      {
         row_type row;
         // start at the first row of the table
         reset();
         for (int rowIndex = 0;
              rowIndex < theTableInformation->nrows;
              ++rowIndex)
         {
            row = read_next_row(*theTableInformation);
            ossimString value = getColumnValueAsString(row,
                                                       columnNumber);
//            value = value.downcase();
            value = value.trim();
            result.push_back(value);
            free_row(row, *theTableInformation);
         }
      }
   }

   return result;
}



ossimString ossimVpfTable::getColumnValueAsString(row_type& row,
                                                  long columnNumber)const
{
   ossimString result;
   ossim_int32 n=1;
   switch(theTableInformation->header[columnNumber].type)
   {
   case 'T': // it's of type text so
   {
      char c;
      char *buf = (char *)get_table_element(columnNumber,
                                            row,
                                            *theTableInformation,
                                            &c,
                                            &n);
      if(buf)
      {
         result = buf;
         free(buf);
      }
      else
      {
         result = c;
      }
      break;
   }
   case 'I':
   {
      ossim_int32 value;
      ossim_int32* v = (ossim_int32*)get_table_element(columnNumber,
                                                       row,
                                                       *theTableInformation,
                                                       &value,
                                                       &n);
      if(v)
      {
         result = ossimString::toString(v[0]);

         free(v);
      }
      else
      {
         result = ossimString::toString(value);
      }
      break;
   }
   case 'S':
   {
      short int value;
      short int* v = (short int*)get_table_element(columnNumber,
                                                   row,
                                                   *theTableInformation,
                                                   &value,
                                                   &n);
      if(v)
      {
         result = ossimString::toString(v[0]);

         free(v);
      }
      else
      {
         result = ossimString::toString(value);
      }
      break;
   }
   case 'F':
   {
      float value;
      float* v = (float*)get_table_element(columnNumber,
                                           row,
                                           *theTableInformation,
                                           &value,
                                           &n);
      if(v)
      {
         value = v[0];
         free(v);
      }
      if (!is_vpf_null_float(value))
      {
         result = ossimString::toString(value);
      }
      else
      {
         result = "nan";
      }
      break;
   }
   case 'B':
   {
      double value;
      double* tempBuf = (double*)get_table_element(columnNumber,
                                                 row,
                                                 *theTableInformation,
                                                 &value,
                                                 &n);
      if(tempBuf)
      {
         value = tempBuf[0];
         free(tempBuf);
      }
      if (!is_vpf_null_double(value))
      {
         result = ossimString::toString(value);
      }
      else
      {
         result = "nan";
      }

      break;
   }
   case 'C':
   {
      coordinate_type coordType;
      coordinate_type* temp = (coordinate_type*)get_table_element(columnNumber,
                                                                  row,
                                                                  *theTableInformation,
                                                                  &coordType,
                                                                  &n);
      if(temp)
      {
         coordType = temp[0];
         free(temp);
      }
      result = ossimString::toString(coordType.x)+
               ossimString(" ") +
               ossimString::toString(coordType.y);
      break;
   }
   case 'K':
   {
      id_triplet_type idTripletType;
      id_triplet_type* tempType = (id_triplet_type*)get_table_element(columnNumber,
                                                                      row,
                                                                      *theTableInformation,
                                                                      &idTripletType,
                                                                      &n);
      if(tempType)
      {
         idTripletType = tempType[0];
         free (tempType);
      }
      result = ossimString(idTripletType.type) +
         ossimString(" ") +
         ossimString::toString(static_cast<ossim_int32>(idTripletType.id)) +
         ossimString(" ") +
         ossimString::toString(static_cast<ossim_int32>(idTripletType.tile))+
         ossimString(" ") +
         ossimString::toString(static_cast<ossim_int32>(idTripletType.exid));
      break;
   }
   case 'D':
   {
      date_type date, formatDate;
      date_type* temp = (date_type*)get_table_element(columnNumber,
                                                      row,
                                                      *theTableInformation,
                                                      &date,
                                                      &n);
      if(temp)
      {
         free(temp);
      }
      format_date(date, formatDate);
      result = formatDate;
      break;
   }
   case 'Z': // tri coordinate types x, y, z
   {
      tri_coordinate_type  *tcptr=NULL;
      tcptr = (tri_coordinate_type *) get_table_element(columnNumber,row,
                                                        *theTableInformation, NULL,&n);
      result = "";
      for (int k=0;k<n;k++)
      {
         
         result = result + " (" +
            ossimString::toString(tcptr[k].x) +
            ossimString(" ") +
            ossimString::toString(tcptr[k].y) +
            ossimString(" ") +
            ossimString::toString(tcptr[k].z) +
            ossimString(") ") ;
      }
      
      if (tcptr)
      {
         free(tcptr);
      }
      break;
   }   
   }
   result.trim();
   return result;
}

ossimString ossimVpfTable::getColumnValueAsString(ossim_int32 rowNumber,
                                                  long columnNumber)const
{
   row_type row = read_row( rowNumber, *theTableInformation);
   ossimString result = getColumnValueAsString(row,
                                               columnNumber);
   free_row(row, *theTableInformation);

   return result;
}

ossimString ossimVpfTable::getColumnValueAsString(const ossimString& columnName)
{
   if(theTableInformation &&
      (theTableInformation->status!=CLOSED))
   {
      long int columnNumber = table_pos(const_cast<char*>(columnName.c_str()),
                                        *theTableInformation);
      row_type row = read_next_row(*theTableInformation);
      ossimString result = getColumnValueAsString(row,
                                                  columnNumber);
      free_row(row, *theTableInformation);
      return result;
   }

   return "";
}

ossim_int32 ossimVpfTable::getColumnPosition(const ossimString& columnName)const
{
   return ((ossim_int32)table_pos(const_cast<char*>(columnName.c_str()), *theTableInformation));
}

void ossimVpfTable::print(std::ostream& out)const
{
   if(theTableInformation &&
      (theTableInformation->status!=CLOSED))
   {
      // make sure we are at the beginning of the table.
      this->reset();


      // the rest of this code is from the vpfutil.  The vpfutil
      // was grabbed from the vhcl map server software.
      vpf_table_type& table = *theTableInformation;

      ossim_int32       i,j,k,n;
      ossim_int16  ival=0,*iptr=NULL;
      ossim_int32       lval=0,*lptr=NULL;
      ossim_float32      fval=0,*fptr=NULL;
      date_type  dval,*dptr=NULL;
      id_triplet_type kval={0,0,0}, *kptr=NULL;
      coordinate_type cval={0,0}, *cptr=NULL;
      char       *buf,  ch, date[40];
      row_type   row;


	  out << "table name:        " << theTableName << std::endl
		  << "desciption:        " << table.description << std::endl
		  << std::endl;

	  out << "Definition:" << std::endl;
      for (i=0;i<table.nfields;i++) {
         if (table.header[i].count < 0)
         {

            out << table.header[i].name << " ("
                << table.header[i].type << ",*)  "
				<< table.header[i].description << std::endl;
         }
         else
         {
            out << table.header[i].name << " ("
                << table.header[i].type << ","
                << table.header[i].count << ")  "
				<< table.header[i].description << std::endl;
         }
      }

	  out << "\nContents:" << std::endl;
      for (i=1;i<=table.nrows;i++)
      {
         row = read_next_row(table);
         for (j=0;j<table.nfields;j++)
         {
            out << table.header[j].name << ": ";
            switch (table.header[j].type) {
	    case 'T':
	       if (table.header[j].count==1)
               {
		  get_table_element(j,row,table,&ch,&n);

		  out << ch << std::endl;
	       }
               else
               {
		  buf = (char *)get_table_element(j,row,table,NULL,&n);
		  n = (long)strlen(table.header[j].name) + 2;
		  for (k=0;k<(long)strlen(buf);k++)
                  {
                     out << buf[k];
		     n++;
		     if (n>80)
                     {
						 out << std::endl;
			n = 0;
		     }
		  }
				  out << std::endl;
		  free(buf);
	       }
	       break;
	    case 'I': // long
	       if (table.header[j].count==1)
               {
		  get_table_element(j,row,table,&lval,&n);
		  if (!is_vpf_null_float(lval))
                  {
					  out << lval << std::endl;
                  }
		  else
                  {
					  out << "null" << std::endl;
                  }
	       }
               else
               {
		  lptr = (ossim_int32*)get_table_element(j,row,table,NULL,&n);
		  for (k=0;k<n;k++)
                  {
		     if (!is_vpf_null_float(lptr[k]))
                     {
                        out << lptr[k];
                     }
		     else
                     {
                        out << "null";
                     }
		  }
				  out << std::endl;
		  free(lptr);
	       }
	       break;
            case 'R': // for double
            {
               double *dptr=NULL;
               double value = 0;
	       if (table.header[j].count==1)
               {
		  get_table_element(j,row,table,&value,&n);
                  if(!is_vpf_null_double(value))
                  {
					  out << "null" << std::endl;
                  }
                  else
                  {
					  out << value << std::endl;
                  }
               }
               else
               {
		  dptr = (double*)get_table_element(j,row,table,NULL,&n);
		  for (k=0;k<n;k++)
                  {
		     if (!is_vpf_null_double(dptr[k]))
                     {
                        out << dptr[k];
                     }
		     else
                     {
                        out << "null ";
                     }
		  }
				  out << std::endl;
		  free(dptr);
               }
            }
	    case 'S': // integer
	       if (table.header[j].count==1)
               {
		  get_table_element(j,row,table,&ival,&n);
		  if (ival != (short int)INT_MAX)
                  {
					  out << ival << std::endl;
                  }
		  else
                  {
					  out << "null" << std::endl;
                  }
	       }
               else
               {
		  iptr = (short int*)get_table_element(j,row,table,NULL,&n);
		  for (k=0;k<n;k++)
                  {
		     if (iptr[k] != (short int)INT_MAX)
                     {
                        out << iptr[k];
                     }
		     else
                     {
                        out << "null ";
                     }
		  }
				  out << std::endl;
		  free(iptr);
	       }
	       break;
	    case 'F': // float type
	       if (table.header[j].count==1)
               {
		  get_table_element(j,row,table,&fval,&n);
		  if (!is_vpf_null_float(fval))
                  {
					  out << fval << std::endl;
                  }
		  else
                  {
					  out << "null" << std::endl;
                  }
	       }
               else
               {
		  fptr = (float*)get_table_element(j,row,table,NULL,&n);
		  for (k=0;k<n;k++)
                  {
		     if (!is_vpf_null_float(fptr[k]))
                     {
                        out << fptr[k] << " ";
                     }
		     else
                     {
                        out << "null ";
                     }
		  }
				  out << std::endl;
		  free(fptr);
	       }
	       break;
            case 'Y':
            {
               double_tri_coordinate_type dtctValue, *dtctPtr;
	       if (table.header[j].count==1)
               {
                  get_table_element(j,row,table,&dtctValue,&n);
                  out << "(";
                  if(is_vpf_null_double(dtctValue.x))
                  {
                     out << "null, ";
                  }
                  else
                  {
                     out << dtctValue.x << ", ";
                  }
                  if(is_vpf_null_double(dtctValue.y))
                  {
                     out << "null, ";
                  }
                  else
                  {
                     out << dtctValue.y << ", ";
                  }
                  if(is_vpf_null_double(dtctValue.z))
                  {
                     out << "null, ";
                  }
                  else
                  {
                     out << dtctValue.z << ", ";
                  }
               }
               else
               {
		  dtctPtr = (double_tri_coordinate_type*)get_table_element(j,row,table,NULL,&n);
		  for (k=0;k<n;k++)
                  {
                     out << "(";
                     if(is_vpf_null_double(dtctPtr[k].x))
                     {
                        out << "null, ";
                     }
                     else
                     {
                        out << dtctPtr[k].x  << ", ";
                     }
                     if(is_vpf_null_double(dtctPtr[k].y))
                     {
                        out << "null, ";
                     }
                     else
                     {
                        out << dtctPtr[k].y  << ", ";
                     }
                     if(is_vpf_null_double(dtctPtr[k].z))
                     {
                        out << "null, ";
                     }
                     else
                     {
                        out << dtctPtr[k].z  << ", ";
                     }
                  }
                  free(dtctPtr);
               }
               break;
            }
            case 'Z': // tri coordinate types x, y, z
            {
               tri_coordinate_type tcval={0,0,0}, *tcptr=NULL;
	       if (table.header[j].count==1)
               {
		  get_table_element(j,row,table,&tcval,&n);
                  out << "(";
                  if(is_vpf_null_float(tcval.x))
                  {
                     out << "null" << ",";
                  }
                  else
                  {
                     out << tcval.x << ", ";
                  }
                  if(is_vpf_null_float(tcval.y))
                  {
                     out << "null" << ", ";
                  }
                  else
                  {
                     out << tcval.y << ", ";
                  }
                  if(is_vpf_null_float(tcval.z))
                  {
					  out << "null)" << std::endl;
                  }
                  else
                  {
					  out << tcval.z << ")" << std::endl;
                  }
	       }
               else
               {
		  tcptr = (tri_coordinate_type*)get_table_element(j,row,table,NULL,&n);
		  for (k=0;k<n;k++)
                  {
                     out << "(";
                     if(is_vpf_null_float(tcptr[k].x))
                     {
                        out << "null, ";
                     }
                     else
                     {
                        out << tcptr[k].x << ", ";
                     }
                     if(is_vpf_null_float(tcptr[k].y))
                     {
                        out << "null, ";
                     }
                     else
                     {
                        out << tcptr[k].y << ", ";
                     }
                     if(is_vpf_null_float(tcptr[k].z))
                     {
                        out << "null)";
                     }
                     else
                     {
                        out << tcptr[k].z << ")";
                     }
                  }
				  out << std::endl;
		  free(cptr);
	       }
	       break;
            }
	    case 'C': // coordinate type  x, y
	       if (table.header[j].count==1)
               {
		  get_table_element(j,row,table,&cval,&n);
                  out << "(";
                  if(is_vpf_null_float(cval.x))
                  {
                     out << "null, ";
                  }
                  else
                  {
                     out << cval.x << ", ";
                  }
                  if(is_vpf_null_float(cval.y))
                  {
                     out << "null, ";
                  }
                  else
                  {
                     out << cval.y << ")";
                  }
	       }
               else
               {
		  cptr = (coordinate_type*)get_table_element(j,row,table,NULL,&n);
		  for (k=0;k<n;k++)
                  {

                     out << "(" << cptr[k].x << "," << cptr[k].y << ") ";
                  }
				  out << std::endl;
		  free(cptr);
	       }
	       break;
            case 'B': // double coordinate type
            {
               double_coordinate_type dct, *dctPtr;

	       if (table.header[j].count==1)
               {
		  get_table_element(j,row,table,&dct,&n);

                  out << "(";
                  if(is_vpf_null_double(dct.x))
                  {
                     out << "null, ";
                  }
                  else
                  {
                     out << dct.x << ", ";
                  }
                  if(is_vpf_null_double(dct.y))
                  {
                     out << "null, ";
                  }
                  else
                  {
                     out << dct.y << ")";
                  }
	       }
               else
               {
		  dctPtr = (double_coordinate_type*)get_table_element(j,row,table,NULL,&n);
		  for (k=0;k<n;k++)
                  {
                     out << "(";
                     if(is_vpf_null_double(dctPtr[k].x))
                     {
                        out << "null,";
                     }
                     else
                     {
                        out << dctPtr[k].x << ", ";
                     }
                     if(is_vpf_null_double(dctPtr[k].y))
                     {
                        out << "null,";
                     }
                     else
                     {
                        out << dctPtr[k].y << ", ";
                     }
                  }
				  out << std::endl;
		  free(dctPtr);
	       }

            }
	    case 'K': //id triplet
	       if (table.header[j].count==1)
               {
		  get_table_element(j,row,table,&kval,&n);
                  out <<  "(" << kval.id
                      << ", " << kval.tile
                      << ", " << kval.exid
					  << ")" << std::endl;
	       }
               else
               {
		  kptr = (id_triplet_type*)get_table_element(j,row,table,NULL,&n);
		  for (k=0;k<n;k++)
                  {
                     out <<  "(" << kptr[k].id
                         << ", " << kptr[k].tile
                         << ", " << kptr[k].exid
						 << ")  " << std::endl;

                  }
				  out << std::endl;
		  free(kptr);
	       }
	       break;
	    case 'D':   /* Date */
	       if (table.header[j].count==1)
               {
		  get_table_element(j,row,table,&dval,&n);
		  format_date(dval,date);
		  out << date << std::endl;
	       }
               else
               {
		  dptr = (date_type*)get_table_element(j,row,table,NULL,&n);
		  for (k=0;k<n;k++)
                  {
		     format_date((char*)(&dptr[k]),date);
                     out << dptr[k] << " ";
		  }
				  out << std::endl;
		  free(dptr);
	       }
	       break;
            }
         }
		 out << std::endl;
         free_row( row, table );
      }
   }
}
