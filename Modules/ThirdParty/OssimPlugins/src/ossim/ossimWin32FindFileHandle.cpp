
#include <cassert>
#include "ossimStringUtilities.h"
#include "ossimWin32FindFileHandle.h"

namespace ossimplugins {

ossimWin32FindFileHandle::ossimWin32FindFileHandle(std::string const& path)  
{
	memset(&m_search_data, 0, sizeof(WIN32_FIND_DATA));
    m_h = FindFirstFile(path.c_str(), &m_search_data);
    if (!is_valid() && (GetLastError() != ERROR_FILE_NOT_FOUND)) 
	{
		char err_msg[1024];
    
    s_printf(
       err_msg,
       "ossimSentinel1Model: cannot list files in directory '%s' : '%d'",
       path,
       GetLastError()
      );
		throw std::runtime_error( err_msg );
	}
}

bool ossimWin32FindFileHandle::is_valid() const 
{ 
	return m_h != INVALID_HANDLE_VALUE; 
}

ossimWin32FindFileHandle::~ossimWin32FindFileHandle() 
{ 
	if (is_valid())	
		FindClose(m_h);  
}

bool ossimWin32FindFileHandle::next() 
{
	assert(is_valid());
    return (FindNextFile(m_h, &m_search_data) == true);
}

std::string ossimWin32FindFileHandle::crt_filename() const 
{ 
return m_search_data.cFileName; 
}

}
