#ifndef ossimWin32FindFileHandle_HEADER
#define ossimWin32FindFileHandle_HEADER

#include <windows.h>
#include <string>

namespace ossimplugins {

struct ossimWin32FindFileHandle {
	explicit ossimWin32FindFileHandle(std::string const& path); 

	bool is_valid() const;

    ~ossimWin32FindFileHandle();

    bool next();

    std::string crt_filename() const;
private:

    HANDLE m_h;
    WIN32_FIND_DATA m_search_data;
};

}
#endif