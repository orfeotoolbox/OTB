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

namespace otb
{

// only two states : the Pointer is Null or GetDataSet() returns a
// valid dataset
class GDALDatasetWrapper : public itk::LightObject
{
  friend class GDALDriverManagerWrapper;

public:
  typedef GDALDatasetWrapper      Self;
  typedef itk::LightObject        Superclass;
  typedef itk::SmartPointer<Self> Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GDALImageIO, itk::LightObject);

  /** Easy access to the internal GDALDataset object.
   *  Don't close it, it will be automatic */
  GDALDataset* GetDataSet() const;
  
  /** Test if the dataset corresponds to a Jpeg2000 file format
   *  Return true if the dataset exists and has a JPEG2000 driver
   *  Return false in all other cases */
  bool IsJPEG2000() const;

protected :
  GDALDatasetWrapper();

  ~GDALDatasetWrapper();


private:
  GDALDataset* m_Dataset;
}; // end of GDALDatasetWrapper

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

typedef enum {NONE, NEAREST, GAUSS, CUBIC, AVERAGE, MODE, AVERAGE_MAGPHASE } GDALResamplingType;


class GDALOverviewsBuilder : public itk::ProcessObject
{
public:
  typedef GDALOverviewsBuilder            Self;
  typedef ProcessObject                   Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  void SetResamplingMethod(GDALResamplingType resampMethod)
  {
    m_ResamplingMethod = resampMethod;
  };

  void SetNbOfResolutions(unsigned int nbResol)
  {
    m_NbOfResolutions = nbResol;
  };

  void SetResolutionFactor(unsigned int factor)
  {
    m_ResolutionFactor = factor;
  }

  void SetInputFileName(std::string str)
  {
    m_InputFileName = str;
  };

  void Update();

protected:
  GDALOverviewsBuilder();
  virtual ~GDALOverviewsBuilder() {};

  void PrintSelf(std::ostream& os,itk::Indent indent) const;

private:

  GDALOverviewsBuilder(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  std::string m_InputFileName;
  unsigned int m_NbOfResolutions;
  unsigned int m_ResolutionFactor;
  GDALResamplingType m_ResamplingMethod;

  void GetGDALResamplingMethod(std::string &resamplingMethod);

}; // end of GDALOverviewsBuilder

} // end namespace otb

