#include <ossim/support_data/ossimNmeaMessageSequencer.h>
#include <fstream>
#include <sstream>

ossimNmeaMessageSequencer::ossimNmeaMessageSequencer()
:m_inputStream(0)
{
   
}

ossimNmeaMessageSequencer::ossimNmeaMessageSequencer(const ossimFilename& file)
:m_inputStream(0)
{
   initialize(file);
}

ossimNmeaMessageSequencer::ossimNmeaMessageSequencer(const std::string& str)
:m_inputStream(0)
{
   initialize(str);
}


ossimNmeaMessageSequencer::~ossimNmeaMessageSequencer()
{
   destroy();
}

void ossimNmeaMessageSequencer::initialize(const ossimFilename& file)
{
   destroy();
   m_inputStream = new std::ifstream(file.c_str());
}

void ossimNmeaMessageSequencer::initialize(const std::string& str)
{
   destroy();
   m_inputStream = new std::istringstream(str);
}

void ossimNmeaMessageSequencer::destroy()
{
   if(m_inputStream)
   {
      delete m_inputStream;
      m_inputStream = 0;
   }
}

bool ossimNmeaMessageSequencer::next(ossimNmeaMessage& msg)
{
   bool result = false;
   
   try {
      msg.parseMessage(*m_inputStream);
      result = true;
   } 
   catch (...) 
   {
      result = false;
   }
   
   return result;
}

bool ossimNmeaMessageSequencer::valid()const
{
   bool result = false;
   
   if(m_inputStream)
   {
      result = m_inputStream->good();
   }
   
   return result;
}

void ossimNmeaMessageSequencer::reset()
{
   if(m_inputStream)
   {
      m_inputStream->clear();
      m_inputStream->seekg(0);
   }
}
