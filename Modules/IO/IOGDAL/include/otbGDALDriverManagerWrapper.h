/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef otbGDALDriverManagerWrapper_h
#define otbGDALDriverManagerWrapper_h


#include "itkLightObject.h"
#include "itkProcessObject.h"
#include "otbConfigure.h"

class GDALDataset;
class GDALDriver;

/* GDAL Libraries */
#include "gdal.h"
#include "gdaljp2metadata.h"
#include "gdal_priv.h"
#include "gdal_alg.h"

#include "otbGDALDatasetWrapper.h"
// otb::GDALOverviewsBuilder moved to self header & body files.
// Including its header file here for compile time compatibility.
#include "otbGDALOverviewsBuilder.h"

namespace otb
{

/** \class GDALDriverManagerWrapper
 *
 * \brief Provide an unique instance of a GDALDataSet
 *
 * This class provides an unique instance of GDALDataSet which remain
 * available during all the program lifetime. This class automatically
 * allocate and destroy the available gdal drivers.
 *
 * \ingroup IOFilters
 *
 *
 * \ingroup OTBIOGDAL
 */
// Wraps the GdalDriverManager so that GDALAllRegister is called automatically
class GDALDriverManagerWrapper
{
public:
  // GetInstance returns a reference to a GDALDriverManagerWrapper
  // This is the only entry point to interact with this class
  static GDALDriverManagerWrapper& GetInstance()
  {

    // Declare a static method variable of type GDALDriverManagerWrapper
    // so that it is constructed and initialized only on the first call
    // to GetInstance(), and so try to avoid static initialization order problems

    static GDALDriverManagerWrapper theUniqueInstance;
    return theUniqueInstance;
  }

  // Open the file for reading and returns a smart dataset pointer
  GDALDatasetWrapper::Pointer Open( std::string filename ) const;

  // Open the new  file for writing and returns a smart dataset pointer
  GDALDatasetWrapper::Pointer Create( std::string driverShortName, std::string filename,
                                      int nXSize, int nYSize, int nBands,
                                      GDALDataType eType, char ** papszOptions ) const;


  GDALDriver* GetDriverByName( std::string driverShortName ) const;

private :
// private constructor so that this class is allocated only inside GetInstance
  GDALDriverManagerWrapper();

  ~GDALDriverManagerWrapper();
}; // end of GDALDriverManagerWrapper


} // end namespace otb


#endif // otbGDALDriverManagerWrapper_h
