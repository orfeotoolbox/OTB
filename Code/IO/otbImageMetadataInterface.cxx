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


#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include <typeinfo>
#include <cassert>

#include "otbImageMetadataInterface.h"

#include "otbMacro.h"

#include "otbSpotImageMetadataInterface.h"
#include "otbIkonosImageMetadataInterface.h"
#include "otbQuickBirdImageMetadataInterface.h"

namespace otb
{


ImageMetadataInterface::ImageMetadataInterface()
{
}


ImageMetadataInterface::VariableLengthVectorType
ImageMetadataInterface::GetPhysicalBias( const MetaDataDictionaryType & dict ) const
{
  VariableLengthVectorType output;
  
  SpotImageMetadataInterface::Pointer spotIMI = SpotImageMetadataInterface::New();
  if(spotIMI->IsSpot(dict))
  {
    output = spotIMI->GetPhysicalBias(dict);
  }
  
  IkonosImageMetadataInterface::Pointer ikIMI = IkonosImageMetadataInterface::New();
  if(ikIMI->IsIkonos(dict))
  {
    output = ikIMI->GetPhysicalBias(dict);
  }
  
  QuickBirdImageMetadataInterface::Pointer qbIMI = QuickBirdImageMetadataInterface::New();
  if(qbIMI->IsQuickBird(dict))
  {
    output = qbIMI->GetPhysicalBias(dict);
  }

  return output;
}


ImageMetadataInterface::VariableLengthVectorType
ImageMetadataInterface::GetPhysicalGain( const MetaDataDictionaryType & dict ) const
{
  VariableLengthVectorType output;
	  
  SpotImageMetadataInterface::Pointer spotIMI = SpotImageMetadataInterface::New();
  if(spotIMI->IsSpot(dict))
  {
    output = spotIMI->GetPhysicalGain(dict);
  }
  
  IkonosImageMetadataInterface::Pointer ikIMI = IkonosImageMetadataInterface::New();
  if(ikIMI->IsIkonos(dict))
  {
    output = ikIMI->GetPhysicalGain(dict);
  }

  QuickBirdImageMetadataInterface::Pointer qbIMI = QuickBirdImageMetadataInterface::New();
  if(qbIMI->IsQuickBird(dict))
  {
    output = qbIMI->GetPhysicalGain(dict);
  }
  
  return output;
}


ImageMetadataInterface::VariableLengthVectorType
ImageMetadataInterface::GetSolarIrradiance( const MetaDataDictionaryType & dict ) const
{
  VariableLengthVectorType outputValuesVariableLengthVector;
	  
  SpotImageMetadataInterface::Pointer spotIMI = SpotImageMetadataInterface::New();
  if(spotIMI->IsSpot(dict))
  {
    outputValuesVariableLengthVector = spotIMI->GetSolarIrradiance(dict);
  }
  
  IkonosImageMetadataInterface::Pointer ikIMI = IkonosImageMetadataInterface::New();
  if(ikIMI->IsIkonos(dict))
  {
    outputValuesVariableLengthVector = ikIMI->GetSolarIrradiance(dict);
  }

  QuickBirdImageMetadataInterface::Pointer qbIMI = QuickBirdImageMetadataInterface::New();
  if(qbIMI->IsQuickBird(dict))
  {
    outputValuesVariableLengthVector = qbIMI->GetSolarIrradiance(dict);
  }
  
  return outputValuesVariableLengthVector;
}


int 
ImageMetadataInterface::GetDay( const MetaDataDictionaryType & dict ) const
{
  int day = 0;
  
  SpotImageMetadataInterface::Pointer spotIMI = SpotImageMetadataInterface::New();
  if(spotIMI->IsSpot(dict))
  {
    day = spotIMI->GetDay(dict);
  }
  
  IkonosImageMetadataInterface::Pointer ikIMI = IkonosImageMetadataInterface::New();
  if(ikIMI->IsIkonos(dict))
  {
    day = ikIMI->GetDay(dict);
  }

  QuickBirdImageMetadataInterface::Pointer qbIMI = QuickBirdImageMetadataInterface::New();
  if(qbIMI->IsQuickBird(dict))
  {
    day = qbIMI->GetDay(dict);
  }
  
  return day;
}


int
ImageMetadataInterface::GetMonth( const MetaDataDictionaryType & dict ) const
{
  int month = 0;

  SpotImageMetadataInterface::Pointer spotIMI = SpotImageMetadataInterface::New();
  if(spotIMI->IsSpot(dict))
  {
    month = spotIMI->GetMonth(dict);
  }
  
  IkonosImageMetadataInterface::Pointer ikIMI = IkonosImageMetadataInterface::New();
  if(ikIMI->IsIkonos(dict))
  {
    month = ikIMI->GetMonth(dict);
  }

  QuickBirdImageMetadataInterface::Pointer qbIMI = QuickBirdImageMetadataInterface::New();
  if(qbIMI->IsQuickBird(dict))
  {
    month = qbIMI->GetMonth(dict);
  }
  
  return month;
}

int
ImageMetadataInterface::GetYear( const MetaDataDictionaryType & dict ) const
{
  int year = 0;

  SpotImageMetadataInterface::Pointer spotIMI = SpotImageMetadataInterface::New();
  if(spotIMI->IsSpot(dict))
  {
    year = spotIMI->GetYear(dict);
  }
  
  IkonosImageMetadataInterface::Pointer ikIMI = IkonosImageMetadataInterface::New();
  if(ikIMI->IsIkonos(dict))
  {
    year = ikIMI->GetYear(dict);
  }

  QuickBirdImageMetadataInterface::Pointer qbIMI = QuickBirdImageMetadataInterface::New();
  if(qbIMI->IsQuickBird(dict))
  {
    year = qbIMI->GetYear(dict);
  }
  
  return year;
}

void 
ImageMetadataInterface::PrintSelf(std::ostream& os, itk::Indent indent, const MetaDataDictionaryType & dict) const
{
	Superclass::PrintSelf(os, indent, dict);
}

} // end namespace otb
