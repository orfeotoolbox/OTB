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

#include "otbAeronetFileReader.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include "otbMath.h"
#include "otbAeronetData.h"

#include "base/ossimDate.h"

namespace otb
{

namespace internal
{
/**
 * Generate date method
 */
ossimLocalTm
ParseDate(const std::string& date, const std::string& time)
{
  ossimLocalTm currentDate;
  ossimString  word("");
  word = date[0];
  word += date[1];
  currentDate.setDay(word.toInt());
  word = date[3];
  word += date[4];
  currentDate.setMonth(word.toInt());
  word =  date[6];
  word += date[7];
  word += date[8];
  word += date[9];
  currentDate.setYear(word.toInt());

  word = time[0];
  word += time[1];
  currentDate.setHour(word.toInt());
  word = time[3];
  word += time[4];
  currentDate.setMin(word.toInt());
  word =  time[6];
  word += time[7];
  currentDate.setSec(word.toInt());

  return currentDate;
}
}

/**
 * Constructor
 */
AeronetFileReader
::AeronetFileReader() :
  m_FileName(""),
  m_Day(1),
  m_Month(1),
  m_Year(2000),
  m_Hour(12),
  m_Minute(0),
  m_Epsilon(0.4)
{
  this->Superclass::SetNumberOfRequiredOutputs(1);
  this->Superclass::SetNthOutput(0, AeronetData::New().GetPointer());
}

/**
 * Destructor
 */
AeronetFileReader
::~AeronetFileReader()
{
}

/**
 * Get the output aeronet data
 * \return The aeronet data extracted.
 */
AeronetData *
AeronetFileReader
::GetOutput(void)
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  return static_cast<AeronetData *> (this->ProcessObject::GetOutput(0));
}

/**
 * Parse line method
 */
AeronetFileReader::VectorString
AeronetFileReader
::ParseLine(const std::string& line) const
{
  VectorString strVector;
  std::string  word("");

  strVector.clear();
  for (unsigned int i = 0; i < line.size(); ++i)
    {
    if (line[i] == ',')
      {
      strVector.push_back(word);
      word = "";
      }
    else if (i == (line.size() - 1))
      {
      word.push_back(line[i]);
      strVector.push_back(word);
      word = "";
      }
    else
      {
      word.push_back(line[i]);
      }
    }
  return strVector;
}

/**
 * Parse a valid line method
 */
void
AeronetFileReader
::ParseValidLine(const double& ref_date,
                 const VectorString& line,
                 const double& epsilon,
                 VectorDouble& water,
                 VectorDouble& angst,
                 VectorDouble& tau_day,
                 VectorDouble& solarZenithAngle) const
{
  unsigned int col_date             = 0;
  unsigned int col_time             = 1;
  unsigned int col_670              = 6;
  unsigned int col_440              = 15;
  unsigned int col_angst            = 37;
  unsigned int col_vapor            = 19;
  unsigned int col_solarZenithAngle = 44;

  ossimLocalTm current_date = internal::ParseDate(line[col_date], line[col_time]);
  double       dcurrent_date = current_date.getJulian();
  // Check hour +/- epsilon
  if (vcl_abs(dcurrent_date - ref_date) < epsilon)
    {
    double dwater = atof(line[col_vapor].c_str());
    double dangst = atof(line[col_angst].c_str());
    double dsolarZenithAngle = atof(line[col_solarZenithAngle].c_str());
    double dtau_670 = atof(line[col_670].c_str());
    double dtau_440 = atof(line[col_440].c_str());
    double dtau_550 = ((670. - 550.) * dtau_440 + (550 - 440) * dtau_670) / (670. - 440.);
    tau_day.push_back(dtau_550);
    angst.push_back(dangst);
    water.push_back(dwater);
    solarZenithAngle.push_back(dsolarZenithAngle);
    }
}

/**
 * Compute statistics method
 */

void
AeronetFileReader
::GetStatistics(const VectorDouble& vec, double& mean, double& stddev) const
{
  double sum(0.);
  double sumOfSquares(0.);
  mean = 0.;
  stddev = 0.;
  if (vec.size() <= 0) return;
  for (unsigned int i = 0; i < vec.size(); i++)
    {
    sum += vec[i];
    sumOfSquares += vec[i] * vec[i];
    }

  // get the mean value
  const double num = static_cast<double>(vec.size());
  if (num == 1)
    {
    mean = sum;
    stddev = 0.;
    }
  else if (num > 1)
    {
    mean = sum / num;
    stddev = (sumOfSquares - (sum * sum / num)) / (num - 1.);
    }
}

/**
 * Generate data method
 */

void
AeronetFileReader
::GenerateData()
{

  std::ifstream fin;
  std::string   line;

  // open file input stream
  fin.open(m_FileName.c_str());

  // Test if the file has been opened correctly
  if (!fin)
    {
    AeronetFileReaderException e(__FILE__, __LINE__);
    std::ostringstream         msg;
    msg << " Could not open IO object for file ";
    msg << m_FileName << "." << std::endl;
    e.SetDescription(msg.str().c_str());
    throw e;
    return;
    }

  // Read informations lines (5 lines)
  std::getline(fin, line);
  std::getline(fin, line);
  std::getline(fin, line);
  std::getline(fin, line);
  std::getline(fin, line);

  unsigned int col_date             = 0;
  unsigned int col_time             = 1;
  unsigned int col_670              = 6;
  unsigned int col_440              = 15;
  unsigned int col_angst            = 37;
  unsigned int col_vapor            = 19;
  unsigned int col_solarZenithAngle = 44;

  VectorString list_str;
  list_str = ParseLine(line);
  if ((list_str[col_date] != "Date(dd-mm-yy)") ||
      (list_str[col_time] != "Time(hh:mm:ss)") ||
      (list_str[col_670] != "AOT_675") ||
      (list_str[col_440] != "AOT_440") ||
      (list_str[col_angst] != "440-870Angstrom") ||
      (list_str[col_vapor] != "Water(cm)") ||
      (list_str[col_solarZenithAngle] != "Solar_Zenith_Angle")
      )
    {
    AeronetFileReaderException e(__FILE__, __LINE__);
    std::ostringstream         msg;
    msg << " The file ";
    msg << m_FileName << " is not conform." << std::endl;
    e.SetDescription(msg.str().c_str());
    throw e;
    return;
    }

  //Compute input date
  ossimLocalTm inputDate;
  inputDate.setDay(m_Day);
  inputDate.setMonth(m_Month);
  inputDate.setYear(m_Year);
  inputDate.setHour(m_Hour);
  inputDate.setMin(m_Minute);
  inputDate.setSec(0);
  double dinputDate = inputDate.getJulian();

  MatrixString tabStr;
  // if so, parse it and select only valid lines for the input date
  while (!fin.eof())
    {
    std::getline(fin, line);
    std::string  word("");
    VectorString listStr;

    listStr = ParseLine(line);
    // Select only valid lines: good day
    if (listStr.size() > 0)
      {
      ossimLocalTm currentDate = internal::ParseDate(listStr[col_date], listStr[col_time]);
      if ((listStr[col_670] != "N/A") &&
          (listStr[col_440] != "N/A") &&
          (static_cast<int>(currentDate.getJulian()) == static_cast<int>(dinputDate))
          )
        {
        tabStr.push_back(listStr);
        }
      }
    }
  fin.close();

  if (tabStr.size() <= 0)
    {
    itkExceptionMacro(
      << "The aeronet file (" << m_FileName << ") doesn't contain valid data for the day (dd/mm/yyyy) " << m_Day <<
      "/" << m_Month << "/" << m_Year << ".");
    }

  //Compute time for one hour
  ossimLocalTm temp;
  temp.setDay(m_Day);
  temp.setMonth(m_Month);
  temp.setYear(m_Year);
  temp.setMin(0);
  temp.setSec(0);

  temp.setHour(10);
  double dhour1 = temp.getJulian();
  temp.setHour(11);
  double dhour2 = temp.getJulian();
  // Update epsilon for one hour
  double epsilon = m_Epsilon * (dhour2 - dhour1);

  VectorDouble water;
  VectorDouble tau_day;
  VectorDouble angst;
  VectorDouble solarZenithAngle;
  for (unsigned int idCurrentLine = 0; idCurrentLine < tabStr.size(); idCurrentLine++)
    {
    VectorString current_line2 = tabStr[idCurrentLine];
    ossimLocalTm currentDate = internal::ParseDate(current_line2[col_date], current_line2[col_time]);
    ParseValidLine(dinputDate, current_line2, epsilon, water, angst, tau_day, solarZenithAngle);
    }

  if (tau_day.size() <= 0)
    {
    itkExceptionMacro(
      << "The aeronet file (" << m_FileName << ") doesn't contain valid data for the time (hh:mm:ss) " << m_Hour <<
      ":" << m_Minute << ":00 with a tolerance of " << m_Epsilon <<
      ". Select an other file or increase the epsilon value.");
    }

  double tau_mean(0.);
  double tau_stddev(0.);

  GetStatistics(tau_day, tau_mean, tau_stddev);
  if (tau_stddev > 0.8)
    {
    itkExceptionMacro(<< "The stddev of the Aerosol Optical Thickness selected is upper than 0.8.");
    }

  double angst_mean(0.);
  double angst_stddev(0.);
  double water_mean(0.);
  double water_stddev(0.);
  double solarZenithAngle_mean(0.);
  double solarZenithAngle_stddev(0.);
//  double stddev = (0.02 >  tau_stddev ? 0.02 :  tau_stddev );
  double stddev(tau_stddev);
  GetStatistics(angst, angst_mean, angst_stddev);
  GetStatistics(water, water_mean, water_stddev);
  GetStatistics(solarZenithAngle, solarZenithAngle_mean, solarZenithAngle_stddev);

  AeronetData * aeronetData = this->GetOutput();
  aeronetData->SetAerosolOpticalThickness(tau_mean);
  aeronetData->SetStdDev(stddev);
  aeronetData->SetAngstromCoefficient(angst_mean);
  aeronetData->SetWater(water_mean);
  aeronetData->SetSolarZenithAngle(solarZenithAngle_mean);
  std::ostringstream msg;
  msg << "(hh/mm/yyyy hh:mm:ss) " << m_Day << "/" << m_Month << "/" << m_Year << " " << m_Hour << ":" << m_Minute <<
  ":00";
  std::string str(msg.str());
  aeronetData->SetDataDate(str);
  aeronetData->SetNumberOfDateUsed(tau_day.size());
  aeronetData->SetEpsilonDate(m_Epsilon);
  // Cloud estimation
  if (stddev > 0.05)
    {
    aeronetData->SetCloudEstimation(true);
    }
  else
    {
    aeronetData->SetCloudEstimation(false);
    }

}
/**
 * PrintSelf method
 */
void
AeronetFileReader
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace otb
