#ifndef ossimShapeDatabase_HEADER
#define ossimShapeDatabase_HEADER
#include <iostream>
#include <ossim/vec/shapefil.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimString.h>


class OSSIM_DLL ossimShapeDatabaseField
{
public:
   ossimString fieldTypeAsString()const;
   
   ossimString  theName;
   int         theWidth;
   int         theDecimals;
   ossim_DBFFieldType theFieldType;
   ossimString  theValue;
};

class OSSIM_DLL ossimShapeDatabaseRecord
{
   
public:
   bool getField(ossimShapeDatabaseField& result,
                 ossim_uint32 i)
      {
         if(i < theFieldArray.size())
         {
            result = theFieldArray[i];
            return true;
         }
         return false;
      }
   bool setField(const ossimShapeDatabaseField& field,
                 ossim_uint32 i)
      {
         if(i < theFieldArray.size())
         {
            theFieldArray[i] = field;
            return true;
         }
         return false;
      }
   int getNumberOfFields()const
      {
         return theFieldArray.size();
      }
   void setNumberOfFields(int n)
      {
         if(n)
         {
            theFieldArray.resize(n);
         }
         else
         {
            theFieldArray.clear();
         }
      }
   ossim_int32 getFieldIdx(const ossimString& name,
                           bool caseInsensitive=true)const;
protected:
	std::vector<ossimShapeDatabaseField> theFieldArray;
};

class OSSIM_DLL ossimShapeDatabase
{
public:
   friend std::ostream& operator <<(std::ostream& out, const ossimShapeDatabase& rhs)
      {
         rhs.print(out);
         
         return out;
      }
   
   ossimShapeDatabase();
   virtual ~ossimShapeDatabase();
   
   virtual bool open(const ossimFilename& file,
                     const ossimString& flags=ossimString("rb"));
   
   virtual void close();
   
   bool getRecord(ossimShapeDatabaseRecord& result);
   
   bool getRecord(ossimShapeDatabaseRecord& result,
                  int recordNumber)
      {
         if(isOpen())
         {
            if(recordNumber < getNumberOfRecords())
            {
               theRecordNumber = recordNumber;
               return getRecord(result);
            }
         }

         return false;
      }
   
   bool getNextRecord(ossimShapeDatabaseRecord& result)
      {
         if(isOpen() && ((theRecordNumber+1) < getNumberOfRecords()))
         {
            ++theRecordNumber;
            return getRecord(result);
         }

         return false;
      }

   int getNumberOfRecords()const;
   bool isOpen()const
      {
         return (theHandle!=NULL);
      }
   virtual ossim_DBFHandle getHandle()
      {
         return theHandle;
      }

   virtual const ossim_DBFHandle getHandle()const
      {
         return theHandle;
      }
   
   virtual void print(std::ostream& out)const;

protected:
   ossim_DBFHandle theHandle;
   ossimFilename theFilename;

   int theRecordNumber;
};

#endif
