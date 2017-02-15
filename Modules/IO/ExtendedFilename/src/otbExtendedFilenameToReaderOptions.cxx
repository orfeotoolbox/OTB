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
#include "otbExtendedFilenameToReaderOptions.h"
#include "otb_boost_string_header.h"
#include "itksys/RegularExpression.hxx"

namespace otb
{

ExtendedFilenameToReaderOptions
::ExtendedFilenameToReaderOptions() : itk::Object()
{
  m_FilenameHelper = FNameHelperType::New();

  m_Options.simpleFileName.first  = false;
  m_Options.simpleFileName.second = "";

  m_Options.extGEOMFileName.first  = false;
  m_Options.extGEOMFileName.second = "";

  m_Options.subDatasetIndex.first  = false;
  m_Options.subDatasetIndex.second = 0;

  m_Options.resolutionFactor.first  = false;
  m_Options.resolutionFactor.second = 0;

  m_Options.skipCarto.first  = false;
  m_Options.skipCarto.second = false;

  m_Options.skipGeom.first  = false;
  m_Options.skipGeom.second = false;

  m_Options.skipRpcTag.first  = false;
  m_Options.skipRpcTag.second = false;

  m_Options.bandRange.first = false;
  m_Options.bandRange.second = "";

  m_Options.optionList.push_back("geom");
  m_Options.optionList.push_back("sdataidx");
  m_Options.optionList.push_back("resol");
  m_Options.optionList.push_back("skipcarto");
  m_Options.optionList.push_back("skipgeom");
  m_Options.optionList.push_back("skiprpctag");
  m_Options.optionList.push_back("band");
}

void
ExtendedFilenameToReaderOptions
::SetExtendedFileName(const char *extFname)
{
  this->m_FilenameHelper->SetExtendedFileName(extFname);
  MapType map = this->m_FilenameHelper->GetOptionMap();

  m_Options.simpleFileName.first  = true;
  m_Options.simpleFileName.second = this->m_FilenameHelper->GetSimpleFileName();

  if (!map["geom"].empty())
    {
    m_Options.extGEOMFileName.first  = true;
    m_Options.extGEOMFileName.second = map["geom"];
    }
  if (!map["sdataidx"].empty())
    {
    m_Options.subDatasetIndex.first  = true;
    m_Options.subDatasetIndex.second = atoi(map["sdataidx"].c_str());
    }
  if (!map["resol"].empty())
    {
    m_Options.resolutionFactor.first  = true;
    m_Options.resolutionFactor.second = atoi(map["resol"].c_str());
    }

  if (!map["skipcarto"].empty())
    {
    m_Options.skipCarto.first = true;
    if (   map["skipcarto"] == "On"
        || map["skipcarto"] == "on"
        || map["skipcarto"] == "ON"
        || map["skipcarto"] == "true"
        || map["skipcarto"] == "True"
        || map["skipcarto"] == "1"   )
      {
      m_Options.skipCarto.second = true;
      }
    }

  if (!map["skipgeom"].empty())
    {
    m_Options.skipGeom.first = true;
    if (   map["skipgeom"] == "On"
        || map["skipgeom"] == "on"
        || map["skipgeom"] == "ON"
        || map["skipgeom"] == "true"
        || map["skipgeom"] == "True"
        || map["skipgeom"] == "1"   )
      {
      m_Options.skipGeom.second = true;
      }
    }

  if (!map["skiprpctag"].empty())
    {
    m_Options.skipRpcTag.first = true;
    if (   map["skiprpctag"] == "On"
        || map["skiprpctag"] == "on"
        || map["skiprpctag"] == "ON"
        || map["skiprpctag"] == "true"
        || map["skiprpctag"] == "True"
        || map["skiprpctag"] == "1"   )
      {
      m_Options.skipRpcTag.second = true;
      }
    }

  if (!map["band"].empty())
    {
    // Basic check on bandRange (using regex)
    itksys::RegularExpression reg;
    reg.compile("^((\\-?[0-9]+)?(:(\\-?[0-9]+)?)?)(,(\\-?[0-9]+)?(:(\\-?[0-9]+)?)?)*$");
    if (reg.find(map["band"]))
      {
      m_Options.bandRange.first = true;
      m_Options.bandRange.second = map["band"];
      }
    else
      {
      itkWarningMacro("Unkwown value "<<map["band"]<<" for band range. Expect a list of tokens separated with comma (each token being a single band index or a range in the form x:y)");
      }
    }

  //Option Checking
  MapIteratorType it;
  for ( it=map.begin(); it != map.end(); it++ )
    {
    bool isKnown=false;
    for (unsigned int i=0; i<m_Options.optionList.size(); i++)
      {
      if (m_Options.optionList[i]==it->first)
        isKnown=true;
      }
    if(!isKnown)
      itkWarningMacro("Unknown option detected: " << it->first << ".");
    }
}

bool
ExtendedFilenameToReaderOptions
::SimpleFileNameIsSet () const
{
  return m_Options.simpleFileName.first;
}
const char*
ExtendedFilenameToReaderOptions
::GetSimpleFileName () const
{
  return m_Options.simpleFileName.second.c_str();
}

bool
ExtendedFilenameToReaderOptions
::ExtGEOMFileNameIsSet () const
{
  return m_Options.extGEOMFileName.first;
}
const char*
ExtendedFilenameToReaderOptions
::GetExtGEOMFileName () const
{
  return m_Options.extGEOMFileName.second.c_str();
}

bool
ExtendedFilenameToReaderOptions
::SubDatasetIndexIsSet () const
{
  return m_Options.subDatasetIndex.first;
}
unsigned int
ExtendedFilenameToReaderOptions
::GetSubDatasetIndex () const
{
  return m_Options.subDatasetIndex.second;
}

bool
ExtendedFilenameToReaderOptions
::ResolutionFactorIsSet () const
{
  return m_Options.resolutionFactor.first;
}
unsigned int
ExtendedFilenameToReaderOptions
::GetResolutionFactor () const
{
  return m_Options.resolutionFactor.second;
}

bool
ExtendedFilenameToReaderOptions
::SkipCartoIsSet () const
{
  return m_Options.skipCarto.first;
}
bool
ExtendedFilenameToReaderOptions
::GetSkipCarto () const
{
  return m_Options.skipCarto.second;
}

bool
ExtendedFilenameToReaderOptions
::SkipGeomIsSet () const
{
  return m_Options.skipGeom.first;
}
bool
ExtendedFilenameToReaderOptions
::GetSkipGeom () const
{
  return m_Options.skipGeom.second;
}

bool
ExtendedFilenameToReaderOptions
::SkipRpcTagIsSet () const
{
  return m_Options.skipRpcTag.first;
}
bool
ExtendedFilenameToReaderOptions
::GetSkipRpcTag () const
{
  return m_Options.skipRpcTag.second;
}

bool
ExtendedFilenameToReaderOptions
::BandRangeIsSet () const
{
  return m_Options.bandRange.first;
}

std::vector<ExtendedFilenameHelper::GenericBandRange>
ExtendedFilenameToReaderOptions
::GetBandRange () const
{
  //Parse string to return vector of band range
  std::vector<ExtendedFilenameHelper::GenericBandRange> vBands;
  size_t start = 0;
  size_t pos;
  while (start != std::string::npos)
    {
    pos = m_Options.bandRange.second.find(',',start);
    if (pos > start)
      {
      ExtendedFilenameHelper::GenericBandRange range;
      size_t size = (pos == std::string::npos ? pos : pos - start);
      bool ret = range.SetString(m_Options.bandRange.second, start, size);
      if (ret) vBands.push_back(range);
      }
    if (pos != std::string::npos) pos++;
    start = pos;
    }
  return vBands;
}

bool
ExtendedFilenameToReaderOptions
::ResolveBandRange(const unsigned int &nbBands, std::vector<unsigned int> &output) const
{
  output.clear();
  std::vector<ExtendedFilenameHelper::GenericBandRange> bandRangeList =
    this->GetBandRange();
  for (unsigned int i=0 ; i<bandRangeList.size() ; i++)
    {
    int a = bandRangeList[i].first;
    int b = bandRangeList[i].second;
    if (a<0) a+= nbBands+1;
    if (b<0) b+= nbBands+1;
    if (a==0) a=1;
    if (b==0) b=nbBands;

    if (1<=a && a<=b && b<=(int)nbBands)
      {
      for (unsigned int k=a ; k <= (unsigned int)b ; k++)
        {
        output.push_back((int)k -1);
        }
      }
    else
      {
      // Invalid range wrt. the given number of bands
      return false;
      }
    }
  return true;
}

} // end namespace otb
