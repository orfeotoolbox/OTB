#include <ossim/base/ossimByteStreamBuffer.h>
#include <cstring> /* for memcpy */

ossimByteStreamBuffer::ossimByteStreamBuffer()
   :m_buffer(0),
    m_bufferSize(0),
    m_sharedBuffer(false)
{
   setBuf(m_buffer, m_bufferSize, m_sharedBuffer);
}

ossimByteStreamBuffer::ossimByteStreamBuffer(char_type* buf, 
                                             ossim_int64 bufSize, 
                                             bool shared)
   :m_buffer(0),
    m_bufferSize(0),
    m_sharedBuffer(false)    
{
   setBuf(buf, bufSize, shared);
}

ossimByteStreamBuffer::ossimByteStreamBuffer(const ossimByteStreamBuffer& src)
   :m_buffer(0),
    m_bufferSize(0),
    m_sharedBuffer(false)    
{
   setBuf(src.m_buffer, src.m_bufferSize, src.m_sharedBuffer);
   
   if(src.m_buffer&&src.m_bufferSize)
   {
      // now align pointers to the same offset locations
      //
      ossim_int64 goff = src.gptr()-src.eback();
      ossim_int64 poff = src.pptr()-src.pbase();
      setg(m_buffer, m_buffer+goff, m_buffer+m_bufferSize);
      setp(m_buffer, m_buffer+m_bufferSize);
      pbump(poff);
   }
}

ossimByteStreamBuffer::~ossimByteStreamBuffer()
{
   deleteBuffer();
}
std::streambuf* ossimByteStreamBuffer::setbuf ( char_type * s, std::streamsize n )
{
   return setBuf(s, n, false);
}

void ossimByteStreamBuffer::clear()
{
   setbuf(0,0);
}
// added so we can set a buffer and make it shared
std::streambuf* ossimByteStreamBuffer::setBuf(char* buf, std::streamsize bufSize, bool shared)
{
   
   deleteBuffer();
   setp(0,0);
   setg(0,0,0);
   char_type* tempBuf = buf;
   if(!shared&&bufSize&&buf)
   {
      tempBuf = new char_type[bufSize];
      memcpy(tempBuf, buf, bufSize);
   }
   m_buffer = tempBuf;
   m_sharedBuffer = shared;
   m_bufferSize = bufSize;
   setp(m_buffer, m_buffer+bufSize);
   if(m_buffer)
   {
      setg(m_buffer, m_buffer, m_buffer+bufSize);
   }
    
   return this;
}

int ossimByteStreamBuffer::overflow( int c)
{
   if(m_sharedBuffer)
   {
      return EOF;
   }
   else
   {            
      ossim_uint32 oldSize = m_bufferSize;
      extendBuffer(1);
      pbump(1);
      m_buffer[oldSize] = (char_type)c;
   }
    
   return c;
}

ossimByteStreamBuffer::char_type* ossimByteStreamBuffer::buffer(){return m_buffer;}
const ossimByteStreamBuffer::char_type* ossimByteStreamBuffer::buffer()const{return m_buffer;}

/**
 * The buffer is no longer managed by this stream buffer and is removed.
 */
ossimByteStreamBuffer::char_type* ossimByteStreamBuffer::takeBuffer()
{
   char_type* result = m_buffer;
   setp(0,0);
   setg(0,0,0);
   m_bufferSize = 0;
   m_buffer     = 0;
    
   return result;
}

ossim_uint64 ossimByteStreamBuffer::bufferSize()const{return m_bufferSize;}


ossimByteStreamBuffer::int_type ossimByteStreamBuffer::pbackfail(int_type __c )
{
   int_type result = __c;
   ossim_int64 delta = gptr()-eback();
   if(delta!=0)
   {
      setg(m_buffer, m_buffer+(delta-1), m_buffer+m_bufferSize);
      if(__c != traits_type::eof())
      {
         *gptr() = static_cast<char_type>(__c);
      }
   }
   else
   {
      result = traits_type::eof();
   }
   return result;
}
ossimByteStreamBuffer::pos_type ossimByteStreamBuffer::seekoff(off_type offset, std::ios_base::seekdir dir,
                                                               std::ios_base::openmode __mode)
{ 
   pos_type result = pos_type(off_type(-1));
   if((__mode & std::ios_base::in)&&
      (__mode & std::ios_base::out))
   {
      // we currently will not support both input and output stream at the same time
      //
      return result;
   }
   switch(dir)
   {
      case std::ios_base::beg:
      {
         // if we are determing an absolute position from the beginning then 
         // just make sure the offset is within range of the current buffer size
         //
         if((offset < m_bufferSize)&&
            (offset >=0))
         {
            result = pos_type(offset);
         }
         if(__mode  & std::ios_base::in)
         {
            gbump(offset - (gptr() - eback()));
         }
         else if(__mode & std::ios_base::out)
         {
            pbump(offset - (pptr() - pbase()));
         }
         break;
      }
      case std::ios_base::cur:
      {
         // if we are determing an absolute position from the current pointer then 
         // add the offset as a relative displacment
         //
         pos_type newPosition = result;
         ossim_int64 delta = 0;
         if(__mode & std::ios_base::in)
         {
            delta = gptr()-eback();
         }
         else if(__mode & std::ios_base::out)
         {
            delta = pptr()-pbase();
         }
         newPosition = pos_type(delta + offset);
         if((newPosition >= 0)&&(newPosition < m_bufferSize))
         {
            result = newPosition;
            if(__mode  & std::ios_base::in)
            {
               gbump(offset);
            }
            else if(__mode & std::ios_base::out)
            {
               pbump(offset);
            }
         }
         break;
      }
      case std::ios_base::end:
      {
         pos_type newPosition = result;
         ossim_int64 delta = 0;
         if(__mode & std::ios_base::in)
         {
            delta = egptr()-eback();
         }
         else if(__mode & std::ios_base::out)
         {
            delta = epptr()-pbase();
         }
         newPosition = pos_type(delta + offset);
         if((newPosition >= 0)&&(newPosition < m_bufferSize))
         {
            result = newPosition;
            if(__mode  & std::ios_base::in)
            {
               gbump(offset - (gptr() - eback()));
            }
            else if(__mode & std::ios_base::out)
            {
               pbump(offset - (epptr() - pptr()));
            }
         }
         break;
      }
      default:
      {
         break;
      }
   }
   return result; 
} 

ossimByteStreamBuffer::pos_type ossimByteStreamBuffer::seekpos(pos_type pos, std::ios_base::openmode __mode)
{
   pos_type result = pos_type(off_type(-1));
    
   if(__mode & std::ios_base::in)
   {
      if(pos >= 0)
      {
         if(pos < m_bufferSize)
         {
            setg(m_buffer, m_buffer + pos, m_buffer + m_bufferSize);
            result = pos;
         }
      }
   }
   else if(__mode & std::ios_base::out)
   {
      if(pos >=0)
      {
         setp(m_buffer, m_buffer+m_bufferSize);
         if(pos < m_bufferSize)
         {
            pbump(pos);
            result = pos;
         }
         else if(!m_sharedBuffer)
         {
            ossim_int64 delta = ossim_int64(pos) - m_bufferSize;
            if(delta > 0)
            {
               extendBuffer(delta+1);
               pbump(pos);
               result = pos;
            }
         }
      }
   }
    
   return result;
}

std::streamsize ossimByteStreamBuffer::xsgetn(char_type* __s, std::streamsize __n)
{
   ossim_uint64 bytesLeftToRead = egptr()-gptr();
   ossim_uint64 bytesToRead = __n;
    
   if(bytesToRead > bytesLeftToRead)
   {
      bytesToRead = bytesLeftToRead;
   }
   if(bytesToRead)
   {
      std::memcpy(__s, gptr(), bytesToRead);
      gbump(bytesToRead); // bump the current get pointer
   }
   return std::streamsize(bytesToRead);
}

std::streamsize ossimByteStreamBuffer::xsputn(const char_type* __s, std::streamsize __n)
{
   ossim_int64 bytesLeftToWrite = epptr()-pptr();
   ossim_int64 bytesToWrite = __n;
   if(__n > bytesLeftToWrite)
   {
      if(!m_sharedBuffer)
      {
         extendBuffer(__n-bytesLeftToWrite);
      }
      else
      {
         bytesToWrite = bytesLeftToWrite;
      }
   }
   if(bytesToWrite)
   {
      std::memcpy(pptr(), __s, bytesToWrite);
      pbump(bytesToWrite);
   }
    
   return bytesToWrite;
}

void ossimByteStreamBuffer::deleteBuffer()
{
   if(!m_sharedBuffer&&m_buffer)
   {
      delete [] m_buffer;
   }
   m_buffer = 0;
   m_bufferSize=0;
}
void ossimByteStreamBuffer::extendBuffer(ossim_uint64 bytes)
{
   // ossim_uint32 oldSize = m_bufferSize;
   char* inBegin = eback();
   char* inCur   = gptr();
   ossim_uint64 pbumpOffset = pptr()-pbase();
    
   ossim_int64 relativeInCur = inCur-inBegin;
    
    
   if(!m_buffer)
   {
      if(bytes>0)
      {
         m_buffer = new char[m_bufferSize + bytes];
      }
   }
   else 
   {
      if(bytes>0)
      {
         char* newBuf = new char[m_bufferSize + bytes];
         std::memcpy(newBuf, m_buffer, m_bufferSize);
         delete [] m_buffer;
         m_buffer = newBuf;
      }
   }
   m_bufferSize += bytes;
    
   setp(m_buffer, m_buffer+m_bufferSize);
   setg(m_buffer, m_buffer+relativeInCur, m_buffer + m_bufferSize);
   pbump(pbumpOffset); // reallign to the current location
}
