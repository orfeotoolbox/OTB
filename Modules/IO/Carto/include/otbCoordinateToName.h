/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbCoordinateToName_h
#define otbCoordinateToName_h

#include "itkPoint.h"
#include "itkMultiThreader.h"
#include "otbCurlHelperInterface.h"
#include "OTBCartoExport.h"
#include <string>

namespace otb
{

/**
 * \class CoordinateToName
 * \brief Retrieve geographical information for longitude and latitude coordinates
 *
 * This class can work in asynchronous mode using \code  MultithreadOn() \endcode. In this
 * case, the web request does not block the rest of the program.
 *
 *
 * \ingroup OTBCarto
 */

class OTBCarto_EXPORT CoordinateToName : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef CoordinateToName              Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef itk::Object Superclass;

  itkTypeMacro(CoordinateToName, itk::Object);
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef itk::Point<double, 2> PointType;

  itkGetMacro(Lon, double);
  itkGetMacro(Lat, double);

  itkSetMacro(Lon, double);
  itkSetMacro(Lat, double);

  /**
   * Set the lon/lat only if they are far enough from the current point to
   * avoid triggering too many updates
   */
  bool SetLonLat(PointType point)
  {
    if ((std::abs(point[0] - m_Lon) > m_UpdateDistance) || (std::abs(point[1] - m_Lat) > m_UpdateDistance))
    {
      //      std::cout << "Update lon/lat " << m_Lon << ", " << m_Lat << " -> " << point << std::endl;
      m_Lon = point[0];
      m_Lat = point[1];
      // TODO Check whether it is better to have something imprecise or nothing at all
      m_IsValid = false;
      return true;
    }
    else
    {
      //      std::cout << "Keeping lon/lat" << std::endl;
      return false;
    }
  }

  std::string GetPlaceName() const
  {
    if (m_IsValid)
    {
      return m_PlaceName;
    }
    else
    {
      return "";
    }
  }

  std::string GetCountryName() const
  {
    if (m_IsValid)
    {
      return m_CountryName;
    }
    else
    {
      return "";
    }
  }

  itkGetMacro(Multithread, bool);
  itkSetMacro(Multithread, bool);
  itkBooleanMacro(Multithread);

  itkSetObjectMacro(Curl, CurlHelperInterface);

  virtual bool Evaluate();

protected:
  CoordinateToName();
  ~CoordinateToName() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  void ParseXMLGeonames(std::string& placeName, std::string& countryName) const;

  virtual void DoEvaluate();

  static ITK_THREAD_RETURN_TYPE ThreadFunction(void*);

private:
  CoordinateToName(const Self&) = delete;
  void operator=(const Self&) = delete;

  double m_Lon;
  double m_Lat;

  bool m_Multithread;
  bool m_IsValid;

  // Minimum distance to trigger an update of the coordinates
  // specified in degrees
  double m_UpdateDistance;

  std::string m_PlaceName;
  std::string m_CountryName;
  std::string m_CurlOutput;

  CurlHelperInterface::Pointer m_Curl;

  itk::MultiThreader::Pointer m_Threader;
};

} // namespace otb

#endif
