/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <cassert>
#include "ossim/ossimStringUtilities.h"
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
       path.c_str(),
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
