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
#ifndef otbGDALOverviewsBuilder_h
#define otbGDALOverviewsBuilder_h


// #include "itkLightObject.h"
#include "itkProcessObject.h"
#include "otbConfigure.h"

// class GDALDataset;
// class GDALDriver;

/* GDAL Libraries */
// #include "gdal.h"
// #include "gdaljp2metadata.h"
// #include "gdal_priv.h"
// #include "gdal_alg.h"

namespace otb
{

/**
 */
enum GdalResampling
{
  NONE = 0,
  NEAREST,
  GAUSS,
  CUBIC,
  AVERAGE,
  MODE,
  AVERAGE_MAGPHASE,
};


// Compile-time compatibility alias.
typedef GdalResampling GDALResamplingType;


/**
 */
class GDALOverviewsBuilder : public itk::ProcessObject
{
public:
  typedef GDALOverviewsBuilder Self;
  typedef ProcessObject Superclass;
  typedef itk::SmartPointer< Self> Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  void SetResamplingMethod( GDALResamplingType resampMethod );

  void SetNbOfResolutions( unsigned int nbResol );

  void SetResolutionFactor( unsigned int factor );

  void SetInputFileName( const std::string & str );

  void Update();

protected:
  GDALOverviewsBuilder();
  virtual ~GDALOverviewsBuilder() {};

  void PrintSelf( std::ostream & os, itk::Indent indent ) const;

private:

  GDALOverviewsBuilder( const Self & ); //purposely not implemented
  void operator = ( const Self & ); //purposely not implemented

  std::string m_InputFileName;
  unsigned int m_NbOfResolutions;
  unsigned int m_ResolutionFactor;
  GDALResamplingType m_ResamplingMethod;

  void GetGDALResamplingMethod( std::string & resamplingMethod );

}; // end of GDALOverviewsBuilder

} // end namespace otb

#endif // otbGDALOverviewsBuilder_h
