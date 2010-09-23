#include <ossim/base/ossimCsvFile.h>
#include <iostream>
#include <iterator>
static const ossim_uint32 DEFAULT_BUFFER_SIZE = 1024;

ossim_int32 ossimCsvFile::INVALID_INDEX = -1;
static std::istream& csvSkipWhiteSpace(std::istream& in)
{
   int c = in.peek();
   while(!in.fail()&& ( (c == ' ')||(c == '\t')||(c == '\n')||(c == '\r') ) )
   {
      in.ignore(1);
      c = in.peek();
   }
   
   return in;
}

ossimCsvFile::Record::Record(ossimCsvFile* csvFile)
:theCsvFile(csvFile)
{
}

bool ossimCsvFile::Record::valueAt(const ossimString& fieldName,
                                   ossimString& value)const
{
   bool result = false;
   if(theCsvFile)
   {
      ossim_int32 idx = theCsvFile->indexOfField(fieldName);
      if((idx > 0)&&(idx < (ossim_int32)theValues.size()))
      {
         value = theValues[idx];
         result = true;
      }
   }
   
   return result;
}

bool ossimCsvFile::Record::valueAt(ossim_uint32 idx,
                                   ossimString& value)const
{
   bool result = false;
   if(idx < theValues.size())
   {
      value = theValues[idx];
      result = true;
   }
   
   return result;
}

ossimString& ossimCsvFile::Record::operator [](const ossimString& fieldName)
{
   if(theCsvFile)
   {
      ossim_int32 idx = theCsvFile->indexOfField(fieldName);
      if((idx >= 0)&&(idx < (ossim_int32)theValues.size()))
      {
         return theValues[idx];
      }
   }      
   
   return theDummyValue;
}

const ossimString& ossimCsvFile::Record::operator [](const ossimString& fieldName)const
{
   if(theCsvFile)
   {
      ossim_int32 idx = theCsvFile->indexOfField(fieldName);
      if((idx >= 0)&&(idx < (ossim_int32)theValues.size()))
      {
         return theValues[idx];
      }
   }      
   
   return theDummyValue;
}

ossimString& ossimCsvFile::Record::operator [](ossim_uint32 idx)
{
   if(idx < theValues.size())
   {
      return theValues[idx];
   }
   
   return theDummyValue;
}

const ossimString& ossimCsvFile::Record::operator [](ossim_uint32 idx)const
{
   if(idx < theValues.size())
   {
      return theValues[idx];
   }
   
   return theDummyValue;
}

ossimCsvFile::ossimCsvFile(const ossimString& separatorList)
:theInputStream(0),
theSeparatorList(separatorList),
 theOpenFlag(false)
{
}

ossimCsvFile::~ossimCsvFile()
{
   close();
}

bool ossimCsvFile::readCsvLine(std::istream& inStream,
                               ossimCsvFile::StringListType& tokens)const
{
   tokens.clear();
   bool done = false;
   char c;
   const char quote = '\"';
   bool inQuotedString = false;
   bool inDoubleQuote    = false;
   ossimString currentToken;
   inStream >> csvSkipWhiteSpace;
   while(!done&&inStream.get(c)&&inStream.good())
   {
      if(c > 0x7e )
      {
         return false;
      }
      if((c!='\n')&&
         (c!='\r'))
      {
         // check to see if we are quoted and check to see if double quoted
         if(c == quote)
         {
            // check if at a double quote
            if(inStream.peek() == quote)
            {
               currentToken += quote;
               inStream.ignore(1);
               if(!inDoubleQuote)
               {
                  inDoubleQuote = true;
               }
               else
               {
                  inDoubleQuote = false;
               }
            }
            else
            {
               if(!inQuotedString)
               {
                  inQuotedString = true;
               }
               else
               {
                  inQuotedString = false;
               }
            }
         }
         // if we are at a separator then check to see if we are inside a quoted string
         else if(theSeparatorList.contains(c)) 
         {
            // ignore token separator if quoted
            if(inQuotedString||inDoubleQuote)
            {
               currentToken += c;
            }
            else
            {
               // push the current token.
               currentToken = currentToken.trim();
               tokens.push_back(currentToken);
               currentToken = "";
               inStream >> csvSkipWhiteSpace;
            }
         }
         else
         {
            currentToken += c;
         }
      }
      else if(!inQuotedString||inDoubleQuote)
      {
         currentToken = currentToken.trim();
         tokens.push_back(currentToken);
         done = true;
      }
      else
      {
         currentToken += c;
      }
   }
   
   return (inStream.good()&&(tokens.size()>0)); 
}

bool ossimCsvFile::open(const ossimFilename& file, const ossimString& flags)
{
   close();
   
   if((*flags.begin()) == 'r')
   {
      theInputStream = new std::ifstream(file.c_str(), std::ios::in|std::ios::binary);
      theOpenFlag = true;
      theRecordBuffer = new ossimCsvFile::Record(this);
   }
   else
   {
      return theOpenFlag;
   }
   
   return theOpenFlag;
}

bool ossimCsvFile::readHeader()
{
   if(theOpenFlag)
   {
      theFieldHeaderList.clear();
      return readCsvLine(*theInputStream, theFieldHeaderList);
   }
   
   return false;
}

void ossimCsvFile::close()
{
   if(theOpenFlag)
   {
      theFieldHeaderList.clear();
      if(theInputStream)
      {
         delete theInputStream;
         theInputStream = 0;
      }
      theOpenFlag = false;
      theRecordBuffer = 0;
   }
}
ossimRefPtr<ossimCsvFile::Record> ossimCsvFile::nextRecord()
{
   if(!theOpenFlag) return ossimRefPtr<ossimCsvFile::Record>(0);
   
   if(theFieldHeaderList.empty())
   {
      if(!readHeader())
      {
         return ossimRefPtr<ossimCsvFile::Record>();
      }
   }
   if(!readCsvLine(*theInputStream,  theRecordBuffer->values()))
   {
      return ossimRefPtr<ossimCsvFile::Record>();
   }
   return theRecordBuffer;
}

const ossimCsvFile::StringListType& ossimCsvFile::fieldHeaderList()const
{
   return theFieldHeaderList;
}

ossim_int32 ossimCsvFile::indexOfField(const ossimString& fieldName)const
{
   ossim_int32 result = ossimCsvFile::INVALID_INDEX;
   ossim_uint32 idx = 0;
   for(;idx < theFieldHeaderList.size(); ++idx)
   {
      if(theFieldHeaderList[idx] == fieldName)
      {
         result = (ossim_int32)idx;
         break;
      }
   }
   
   return result;
}
