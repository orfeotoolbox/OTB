
#include <iomanip>

#include <ossim/vec/ossimShapeDatabase.h>
#include <ossim/vec/ossimShapeFile.h>



ossimString ossimShapeDatabaseField::fieldTypeAsString()const
{
   switch(theFieldType)
   {
   case FTString:
   {
      return "String";
   }
   case FTInteger:
   {
      return "Integer";
   }
   case FTDouble:
   {
      return "Double";
   }
   default:
   {
      return "Unknown";
   }
   };

   return "";
   
}


ossim_int32 ossimShapeDatabaseRecord::getFieldIdx(const ossimString& name,
                                                  bool caseInsensitive)const
{
   ossimString searchString = name;
   if(caseInsensitive) searchString = searchString.downcase();
   ossim_int32 idx = 0;
   for(idx = 0; idx < (int)theFieldArray.size(); ++idx)
   {
      if(caseInsensitive)
      {
         if(ossimString(theFieldArray[idx].theName).downcase() == searchString)
         {
            return idx;
         }
      }
      else
      {
         if(theFieldArray[idx].theName == searchString)
         {
            return idx;
         }
      }
   }
   return -1;
}

ossimShapeDatabase::ossimShapeDatabase()
   :theHandle(NULL),
    theFilename("")
{
   theRecordNumber = -1;
}

ossimShapeDatabase::~ossimShapeDatabase()
{
   close();
}

bool ossimShapeDatabase::open(const ossimFilename& file,
                              const ossimString& flags)
{
   if(isOpen()) close();

   theHandle = ossim_DBFOpen(file.c_str(), flags.c_str());
   if(theHandle)
   {
      theFilename = file;
      theRecordNumber = -1;
   }

   return (theHandle != NULL);
}

void ossimShapeDatabase::close()
{
   if(isOpen())
   {
      ossim_DBFClose(theHandle);
      theHandle = NULL;
      theRecordNumber = -1;
   }
}
int ossimShapeDatabase::getNumberOfRecords()const
{
   if(isOpen())
   {
      return theHandle->nRecords;
   }

   return 0;
}
bool ossimShapeDatabase::getRecord(ossimShapeDatabaseRecord& result)
{
   if(isOpen()&&( (theRecordNumber < theHandle->nRecords) ))
   {
      if(result.getNumberOfFields() != theHandle->nFields)
      {
         result.setNumberOfFields(theHandle->nFields);
      }
      
      char name[1024] = {'\0'};
      int width       = 0;
      int decimals    = 0;
      int iField      = 0;
      std::vector<int>         fieldWidths;
      
      for(iField = 0; iField < theHandle->nFields; ++iField)
      {   
         ossim_DBFFieldType fieldType = ossim_DBFGetFieldInfo(theHandle,
                                                  iField,
                                                  name,
                                                  &width,
                                                  &decimals);
         ossimShapeDatabaseField field;
         field.theName = name;
         field.theWidth = width;
         field.theDecimals = decimals;
         field.theFieldType = fieldType;
            
         ossimString key = "field";
         key+=ossimString::toString(iField+1);
         key+=(ossimString(".")+name+":");
         
         switch(fieldType)
         {
         case FTString:
         {
            field.theValue = ossim_DBFReadStringAttribute(theHandle, theRecordNumber, iField);
            break;
         }
         case FTInteger:
         {
            field.theValue = ossimString::toString(ossim_DBFReadIntegerAttribute(theHandle, theRecordNumber, iField));
            break;
         }
         case FTDouble:
         {
            field.theValue = ossimString::toString(ossim_DBFReadDoubleAttribute(theHandle, theRecordNumber, iField));
            break;
         }
		 case FTLogical:
		 {
			 break;
		 }
		 case FTInvalid:
		 {
			 break;
		 }
         }

         result.setField(field,
                         iField);
      }
      return true;
   }

   return false;
}

void ossimShapeDatabase::print(std::ostream& out)const
{
   if(isOpen())
   {
      out << std::setw(15)<<setiosflags(std::ios::left)
          <<"DB filename:" << theFilename << std::endl
          << std::setw(15)<<setiosflags(std::ios::left)
          <<"records:" << theHandle->nRecords << std::endl
          << std::setw(15)<<setiosflags(std::ios::left)
          <<"fields:" << theHandle->nFields << std::endl;

      char name[1024] = {'\0'};
      int width       = 0;
      int decimals    = 0;
      int iField      = 0;
      std::vector<int>         fieldWidths;

      
      for(iField = 0; iField < theHandle->nFields; ++iField)
      {
         ossim_DBFFieldType fieldType = ossim_DBFGetFieldInfo(theHandle,
                                                  iField,
                                                  name,
                                                  &width,
                                                  &decimals);
         ossimString s = "field " + ossimString::toString(iField+1) + " name:";
         switch(fieldType)
         {
         case FTString:
         case FTInteger:
         case FTDouble:
         {
            out << std::setw(15) << setiosflags(std::ios::left) << s.c_str() << name << std::endl;
            break;
         }
         default:
         {
            out << std::setw(15) << setiosflags(std::ios::left) << s.c_str() << "INVALID"<<std::endl;
            break;
         }
         }
      }
      for(int iShape = 0; iShape < theHandle->nRecords; ++iShape)
      {
         for(iField = 0; iField < theHandle->nFields; ++iField)
         {   
            ossim_DBFFieldType fieldType = ossim_DBFGetFieldInfo(theHandle,
                                                     iField,
                                                     name,
                                                     &width,
                                                     &decimals);
            
            ossimString key = "field";
            key+=ossimString::toString(iField+1);
            key+=(ossimString(".")+name+":");
            
            switch(fieldType)
            {
            case FTString:
            {
               
               out << std::setw(25) << setiosflags(std::ios::left) << key.c_str()
                   << ossim_DBFReadStringAttribute(theHandle, iShape, iField) <<std::endl;
               
               break;
            }
            case FTInteger:
            {
               out << std::setw(25) << setiosflags(std::ios::left) << key.c_str()
                   << ossim_DBFReadIntegerAttribute(theHandle, iShape, iField) << std::endl;
               
               break;
            }
            case FTDouble:
            {
               out << std::setw(25) << setiosflags(std::ios::left) << key.c_str()
                   << ossim_DBFReadDoubleAttribute(theHandle, iShape, iField) << std::endl;
               
               break;
            }
			case FTLogical:
			{
				break;
			}
			case FTInvalid:
			{
				break;
			}
            }
         }
         out << "_________________________________"<<std::endl;
      }
   }
}
