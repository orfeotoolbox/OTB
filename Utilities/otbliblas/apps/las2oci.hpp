#ifndef LAS2OCI_HPP_INCLUDED
#define LAS2OCI_HPP_INCLUDED

#include "oci_wrapper.h"

#include <stdlib.h>


// god-awful hack because of GDAL/GeoTIFF's shitty include structure
#define CPL_SERV_H_INCLUDED


#include <liblas/liblas.hpp>
#include <boost/cstdint.hpp>
#include <boost/concept_check.hpp>

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <exception>
#include <algorithm>
#include <vector>
#include <cctype>
#include <cmath>

#include <sys/stat.h>


using namespace std;
using namespace liblas;


#ifdef _WIN32
#define compare_no_case(a,b,n)  _strnicmp( (a), (b), (n) )
#else
#define compare_no_case(a,b,n)  strncasecmp( (a), (b), (n) )
#endif


#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>

typedef std::vector<boost::uint32_t> IDVector;
typedef boost::shared_ptr< IDVector > IDVectorPtr;

typedef struct
{
    long* pc_ids;
    long* block_ids;
    long* num_points;
    OCILobLocator** locators; // =(OCILobLocator**) VSIMalloc( sizeof(OCILobLocator*) * 1 );

    std::vector<boost::uint8_t>** blobs;

    long* srids;
    long* gtypes;
    OCIArray** element_arrays;
    OCIArray** coordinate_arrays;
    
    long size;
        
} blocks;

// typedef struct
// {
//     double x0;
//     double x1;
//     double y0;
//     double y1;
//     double z0;
//     double z1;
//     bool bUse3d;
//    
// } extent;


#include "kdx_util.hpp"
#include "oci_util.hpp"

#endif // LAS2OCI

