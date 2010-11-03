#ifndef OCI_UTIL_HPP_INCLUDED
#define OCI_UTIL_HPP_INCLUDED


#include "oci_wrapper.h"


#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>

#include <boost/cstdint.hpp>
#include <boost/concept_check.hpp>


bool EnableTracing(OWConnection* connection);
bool IsGeographic(OWConnection* connection, long srid);
OWStatement* RunSQL(OWConnection* connection, std::ostringstream& command);
bool Cleanup(OWConnection* connection, std::string tableName);
bool CreateBlockTable(OWConnection* connection, std::string tableName);
bool BlockTableExists(OWConnection* connection, std::string tableName);
bool CreateBlockIndex(  OWConnection* connection, 
                        std::string tableName, 
                        long srid, 
                        bool bUse3d);

bool WipeBlockTable(   OWConnection* connection, 
                    std::string tableName, 
                    std::string cloudTableName, 
                    std::string cloudColumnName);
std::string to_upper(const std::string&);
std::string ReadSQLData(std::string filename);
std::istream* OpenInput(std::string filename, bool bEnd) ;

#ifdef _WIN32
#define compare_no_case(a,b,n)  _strnicmp( (a), (b), (n) )
#else
#define compare_no_case(a,b,n)  strncasecmp( (a), (b), (n) )
#endif

#endif // OCI_UTIL_HPP_INCLUDED

