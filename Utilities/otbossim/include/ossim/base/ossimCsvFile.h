#ifndef ossimCsvFile_HEADER
#define ossimCsvFile_HEADER
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimString.h>
#include <map>
#include <vector>
#include <fstream>

class OSSIM_DLL ossimCsvFile : public ossimReferenced
{
public:
   typedef std::vector<ossimString> StringListType;
   class OSSIM_DLL Record : public ossimReferenced
      {
      public:
         typedef std::map<ossimString, ossimString, ossimStringLtstr> FieldMapType;
         typedef std::vector<ossimString> FieldValueListType;
                  
         Record(ossimCsvFile* csvFile);
         void setCsvFile(ossimCsvFile* file)
         {
            theCsvFile = file;
         }
         ossimCsvFile* csvFile()
         {
            return theCsvFile;
         }
         const ossimCsvFile* csvFile()const
         {
            return theCsvFile;
         }
         
         StringListType& values()
         {
            return theValues;
         }
         const StringListType& values()const
         {
            return theValues;
         }
         
         /**
          * Allows access to a field as read only.
          */
         bool valueAt(const ossimString& fieldName,
                      ossimString& value)const;
         
         bool valueAt(ossim_uint32 idx,
                      ossimString& value)const;
         /**
          * Allows one to access and write to the field.
          */
         ossimString& operator [](const ossimString& fieldName);
         const ossimString& operator [](const ossimString& fieldName)const;
         ossimString& operator [](ossim_uint32 idx);
         const ossimString& operator [](ossim_uint32 idx)const;
      protected:
         // used in out of bounds returns on the operators []
         //
         ossimString theDummyValue;
         StringListType theValues;
         ossimCsvFile* theCsvFile;
      };
   friend class Record;
   
   ossimCsvFile(const ossimString& separatorList=",");
   ~ossimCsvFile();
   
   /**
    * For now we will only support the read flag and open existing 
    * csv files.  Later we can add support for creating and writing csv files
    *
    */
   bool open(const ossimFilename& file, const ossimString& flags="r");
   void close();
   
   bool readHeader();
   /**
    * Read one record and returns null if no more records exist or returns a valid pointer
    * if a record exists.
    */
   ossimRefPtr<ossimCsvFile::Record> nextRecord();
      
   /**
    * Returns the header of the CSV file.
    */
   const StringListType& fieldHeaderList()const;
   
   ossim_int32 indexOfField(const ossimString& fieldName)const;
   
   static ossim_int32 INVALID_INDEX;
protected:
   bool readCsvLine(std::istream& inStream,
                    ossimCsvFile::StringListType& tokens)const;
   StringListType theFieldHeaderList;
   ossimRefPtr<ossimCsvFile::Record> theRecordBuffer;
   std::istream* theInputStream;
   ossimString theSeparatorList;
   bool theOpenFlag;
};
#endif
