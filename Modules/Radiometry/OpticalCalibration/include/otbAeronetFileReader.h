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
#ifndef __otbAeronetFileReader_h
#define __otbAeronetFileReader_h

#include <string>
#include <vector>

#include "itkProcessObject.h"

namespace otb
{

class AeronetData;

/** \class AeronetFileReaderException
 * \brief Base exception class for Aeronet problems during reading.
 *
 * \ingroup OTBOpticalCalibration
 */
class ITK_EXPORT AeronetFileReaderException
  : public itk::ExceptionObject
{
public:
  /** Run-time information. */
  itkTypeMacro(AeronetFileReaderException, ExceptionObject);

  /** Constructor. */
  AeronetFileReaderException(const char *file, unsigned int line,
                             const char* message = "Error in Radiometry IO",
                             const char* loc = "Unknown") :
    ExceptionObject(file, line, message, loc) {}
  /** Constructor. */
  AeronetFileReaderException(const std::string & file, unsigned int line,
                             const char* message = "Error in Radiometry IO",
                             const char* loc = "Unknown") :
    ExceptionObject(file, line, message, loc) {}
};

/**
 * \class AeronetFileReader
 * \brief This class reads a Aeronet data from an Aeronet file.
 *
 *  This class reads a Aeronet data from an Aeronet file.
 *
 *  This file must contain the following fields (contains in the line 5)
 *    column 1  : The date "Date (dd/mm/yyyy)"
 *           2  : The time "Time (hh:mm:ss)"
 *           6  : Aerosol Optical for 670 nm "AOT_675"
 *           15 : Aerosol Optical for 440 nm "AOT_440"
 *           19 : Water "Water (cm)"
 *           37 : Angstrom value "440-870Angstrom"
 *           44 :  Solar zenith angle "Solar_Zenith_Angle"
 *  The datas start in the line number 6.
 *
 * \sa AeronetData
 *
 * \ingroup OTBOpticalCalibration
 */
class ITK_EXPORT AeronetFileReader : public itk::ProcessObject
{
public:
  /** Standards typedef */
  typedef AeronetFileReader             Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  /** Creation through the object factory */
  itkNewMacro(Self);
  /** Runtime type information */
  itkTypeMacro(AeronetFileReader, itk::ProcessObject);

  /** Overiding of the GetOutput() method */
  virtual AeronetData * GetOutput(void);

  /** Set the filename  */
  itkSetStringMacro(FileName);
  /** Get the filename */
  itkGetStringMacro(FileName);

  /** Set/Get the day value */
  itkSetMacro(Day, int);
  itkGetMacro(Day, int);

  /** Set/Get the month value */
  itkSetMacro(Month, int);
  itkGetMacro(Month, int);

  /** Set/Get the year value */
  itkSetMacro(Year, int);
  itkGetMacro(Year, int);

  /** Set/Get the hour value */
  itkSetMacro(Hour, int);
  itkGetMacro(Hour, int);

  /** Set/Get the minute value */
  itkSetMacro(Minute, int);
  itkGetMacro(Minute, int);

  /** Set/Get the epsilon for the time (tolerance for one hour) */
  itkSetMacro(Epsilon, double);
  itkGetMacro(Epsilon, double);

protected:
  /** Constructor */
  AeronetFileReader();
  /** Destructor */
  virtual ~AeronetFileReader();
  /** Main computation method */
  virtual void GenerateData();

  /** PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:

  /** Standards privates typedefs */
  typedef std::vector<std::string>  VectorString;
  typedef std::vector<double>       VectorDouble;
  typedef std::vector<VectorString> MatrixString;

  /** Parse the string and return a list of strings which separated by ',' char */
  VectorString ParseLine(const std::string& line) const;

/**
 * Parse valid line method
 */
  void ParseValidLine(const double& ref_date,
                      const VectorString& line,
                      const double& epsilon,
                      VectorDouble& water,
                      VectorDouble& angst,
                      VectorDouble& tau_day,
                      VectorDouble& solarZenithAngle) const;

/**
 * Compute statistics method (mean and stddev)
 */
  void GetStatistics(const VectorDouble& vec, double& mean, double& stddev) const;

  /** File name */
  std::string m_FileName;
  /** Day value */
  int m_Day;
  /** Month value */
  int m_Month;
  /** Year value */
  int m_Year;
  /** Hour value */
  int m_Hour;
  /** Minute value */
  int m_Minute;

  /** Epsilon to select time */
  double m_Epsilon;
};
}

#endif
