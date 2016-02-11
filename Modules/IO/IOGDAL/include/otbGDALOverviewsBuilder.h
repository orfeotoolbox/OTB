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
#include "itkSize.h"

#include "otbGDALDatasetWrapper.h"
#include "otbConfigure.h"

namespace otb
{

/**
 */
enum GDALResampling
{
  GDAL_RESAMPLING_NONE = 0,
  //
  GDAL_RESAMPLING_NEAREST,
  GDAL_RESAMPLING_GAUSS,
  GDAL_RESAMPLING_CUBIC,
  GDAL_RESAMPLING_AVERAGE,
  GDAL_RESAMPLING_MODE,
  GDAL_RESAMPLING_AVERAGE_MAGPHASE,
  //
  GDAL_RESAMPLING_COUNT
};


// Compile-time compatibility alias.
typedef GDALResampling GDALResamplingType;

/**
 */
enum GDALCompression
{
  GDAL_COMPRESSION_NONE = 0,
  //
  GDAL_COMPRESSION_JPEG,
  GDAL_COMPRESSION_LZW,
  GDAL_COMPRESSION_PACKBITS,
  GDAL_COMPRESSION_DEFLATE,
  //
  GDAL_COMPRESSION_COUNT,
};


/**
 */
enum GDALFormat
{
  GDAL_FORMAT_ERDAS,
  GDAL_FORMAT_GEOTIFF,
  //
  GDAL_FORMAT_COUNT,
};


/**
 */
class GDALOverviewsBuilder : public itk::ProcessObject
{
public:
  typedef GDALOverviewsBuilder Self;
  typedef ProcessObject Superclass;
  typedef itk::SmartPointer< Self> Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  typedef itk::Size< 2 > Size;
  typedef std::vector< Size > SizeVector;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /**
   * \brief Count the number of resolution levels larger than
   * factor^n. 
   */
  unsigned int
  CountResolutions( unsigned int factor, unsigned int n =0 ) const;

  unsigned int
  CountResolutions( unsigned int n =0 ) const;

  void ListResolutions( SizeVector &, unsigned int factor, unsigned int count );

  void ListResolutions( SizeVector & );

  unsigned int GetOverviewsCount() const;

  GDALResampling GetResamplingMethod() const;

  void SetResamplingMethod( GDALResampling );


  GDALCompression GetCompressionMethod() const;

  void SetCompressionMethod( GDALCompression );


  GDALFormat GetFormat() const;

  void SetFormat( GDALFormat );


  unsigned int GetNbResolutions() const;

  void SetNbResolutions( unsigned int );

  unsigned int GetResolutionFactor() const;

  void SetResolutionFactor( unsigned int );


  const std::string & GetInputFileName() const;

  void SetInputFileName( const std::string & str );


  void Update();


protected:
  GDALOverviewsBuilder();

  virtual ~GDALOverviewsBuilder() {};

  void PrintSelf( std::ostream & os, itk::Indent indent ) const;


private:
  GDALOverviewsBuilder( const Self & ); //purposely not implemented

  void operator = ( const Self & ); //purposely not implemented

  // void GetGDALResamplingMethod( std::string & resamplingMethod );

  void OpenDataset( const std::string & filename );


  GDALDatasetWrapper::Pointer m_GdalDataset;
  std::string m_InputFileName;
  unsigned int m_NbResolutions;
  unsigned int m_ResolutionFactor;
  GDALResampling m_ResamplingMethod;
  GDALCompression m_CompressionMethod;
  GDALFormat m_Format;

}; // end of GDALOverviewsBuilder

} // end namespace otb

#endif // otbGDALOverviewsBuilder_h
