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
#ifndef otbPlaceNameToLonLat_h
#define otbPlaceNameToLonLat_h

#include "otbCurlHelperInterface.h"
#include "OTBCartoExport.h"
#include <string>

namespace otb
{

/**
 * \class PlaceNameToLonLat
 * \brief Retrieve Longitude and Latitude coordinates from a place name
 *
 *
 * \ingroup OTBCarto
 */

class OTBCarto_EXPORT PlaceNameToLonLat : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef PlaceNameToLonLat             Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef itk::Object Superclass;

  itkTypeMacro(PlaceNameToLonLat, Object);
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  itkGetMacro(Lon, double);
  itkGetMacro(Lat, double);
  itkGetMacro(PlaceName, std::string);

  itkSetMacro(PlaceName, std::string);

  virtual bool Evaluate();

protected:
  PlaceNameToLonLat();
  ~PlaceNameToLonLat() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  void RetrieveXML(const std::ostringstream& urlStream);
  void ParseXMLYahoo();
  void ParseXMLGoogle();
  void ParseXMLGeonames();

private:
  PlaceNameToLonLat(const Self&) = delete;
  void operator=(const Self&) = delete;

  double      m_Lon;
  double      m_Lat;
  std::string m_PlaceName;

  CurlHelperInterface::Pointer m_Curl;
  std::string                  m_CurlOutput;
  bool                         m_RequestSucceed;
};

} // namespace otb

#endif
