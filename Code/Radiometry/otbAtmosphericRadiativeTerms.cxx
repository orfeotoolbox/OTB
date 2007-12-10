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

#include "otbAtmosphericRadiativeTerms.h"

namespace otb 
{
/*********************************** AtmosphericRadiativeTermsSingleChannel***********************************************/ 
/**PrintSelf method */
void 
AtmosphericRadiativeTermsSingleChannel
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "Intrinsic Atmospheric Reflectance: " << m_IntrinsicAtmosphericReflectance << std::endl;
  os << indent << "Shperical Albedo of the Atmosphere: " << m_SphericalAlbedo << std::endl;
  os << indent << "Total Gaseous Transmission: " << m_TotalGaseousTransmission << std::endl;
  os << indent << "Downward Transmittance of the Atmospher: " << m_DownwardTransmittance << std::endl;
  os << indent << "Upward Transmittance of the Atmospher: " << m_UpwardTransmittance << std::endl;
}
  



/*********************************** AtmosphericRadiativeTerms **********************************************************/ 
/**CONSTRUCTOR. */
AtmosphericRadiativeTerms
::AtmosphericRadiativeTerms()
{
  m_Values.clear();
  m_IsInitialized = false;
}

void 
AtmosphericRadiativeTerms
::ValuesInitialization(unsigned int nbChannel)
{
  ValueType temp = AtmosphericRadiativeTermsSingleChannel::New();
  for (unsigned int j=0; j<nbChannel; j++)
    {
      m_Values.push_back(temp);
    }
  m_IsInitialized = true;
}

/** SET ACCESSORS WITH VECTORS. */
void 
AtmosphericRadiativeTerms
::SetIntrinsicAtmosphericReflectances(const DataVectorType & vect)
{
  if ( !m_IsInitialized)
    { 
      this->ValuesInitialization(vect.size());
      
    }
  for (unsigned int nbChannel=0; nbChannel<vect.size(); nbChannel++)
    { 
      m_Values[nbChannel]->SetIntrinsicAtmosphericReflectance(vect[nbChannel]);
    }
}
void 
AtmosphericRadiativeTerms
::SetSphericalAlbedos(const DataVectorType & vect)
{
  if ( !m_IsInitialized)
    { 
      this->ValuesInitialization(vect.size());
    }
  for (unsigned int nbChannel=0; nbChannel<vect.size(); nbChannel++)
    {
      m_Values[nbChannel]->SetSphericalAlbedo(vect[nbChannel]);
    } 
}
void 
AtmosphericRadiativeTerms
::SetTotalGaseousTransmissions(const DataVectorType & vect)
{
  if ( !m_IsInitialized)
    { 
      this->ValuesInitialization(vect.size());
    }
  for (unsigned int nbChannel=0; nbChannel<vect.size(); nbChannel++)
    {
      m_Values[nbChannel]->SetTotalGaseousTransmission(vect[nbChannel]);
    }  
  
}
void 
AtmosphericRadiativeTerms
::SetDownwardTransmittances(const DataVectorType & vect)
{
  if ( !m_IsInitialized)
    { 
      this->ValuesInitialization(vect.size());
    }
  for (unsigned int nbChannel=0; nbChannel<vect.size(); nbChannel++)
    {
      m_Values[nbChannel]->SetDownwardTransmittance(vect[nbChannel]);
    }
}
void 
AtmosphericRadiativeTerms
::SetUpwardTransmittances(const DataVectorType & vect)
{
  if ( !m_IsInitialized)
    { 
      this->ValuesInitialization(vect.size());
    }
  for (unsigned int nbChannel=0; nbChannel<vect.size(); nbChannel++)
    {
      m_Values[nbChannel]->SetUpwardTransmittance(vect[nbChannel]);
    }  
}

/** SET ACCESSORS WITH INDEX. */
void 
AtmosphericRadiativeTerms
::SetValueByIndex(unsigned int id, const ValueType & val)
{
  if ( m_IsInitialized )
    {
      if ( m_Values.size()<id+1 )
	{ 
	  for(unsigned int j=0; j<(id+1-m_Values.size());j++)
	    {
	      ValueType temp = AtmosphericRadiativeTermsSingleChannel::New();
	      m_Values.push_back(temp);
	    }
	}
      m_Values[id] = val;
    }
  else
    {
      itkExceptionMacro(<< "Can't insert value before iniatilizing vector value..."<<std::endl);
    }
}
void 
AtmosphericRadiativeTerms
::SetIntrinsicAtmosphericReflectances(unsigned int id, const double & val)
{
  if ( m_Values.size()<id+1 )
    { 
      for(unsigned int j=0; j<(id+1-m_Values.size());j++)
	{
	  ValueType temp = AtmosphericRadiativeTermsSingleChannel::New();
	  m_Values.push_back(temp);
	}
    }
  m_Values[id]->SetIntrinsicAtmosphericReflectance(val);
} 
void
AtmosphericRadiativeTerms
::SetSphericalAlbedos(unsigned int id, const double & val)
{
  if ( m_Values.size()<id+1 )
    {
      for(unsigned int j=0; j<(id+1-m_Values.size());j++)
	{
	  ValueType temp = AtmosphericRadiativeTermsSingleChannel::New();
	  m_Values.push_back(temp);
	}
    }
  m_Values[id]->SetSphericalAlbedo(val);
} 
void 
AtmosphericRadiativeTerms
::SetTotalGaseousTransmissions(unsigned int id, const double & val)
{
  if ( m_Values.size()<id+1 )
    { 
      for(unsigned int j=0; j<(id+1-m_Values.size());j++)
	{
	  ValueType temp = AtmosphericRadiativeTermsSingleChannel::New();
	  m_Values.push_back(temp);
	}
    }
  m_Values[id]->SetTotalGaseousTransmission(val);
} 
void 
AtmosphericRadiativeTerms
::SetDownwardTransmittances(unsigned int id, const double & val ) 
{
 if ( m_Values.size()<id+1 )
    { 
      for(unsigned int j=0; j<(id+1-m_Values.size());j++)
	{
	  ValueType temp = AtmosphericRadiativeTermsSingleChannel::New();
	  m_Values.push_back(temp);
	}
    }
  m_Values[id]->SetDownwardTransmittance(val);
} 
void 
AtmosphericRadiativeTerms
::SetUpwardTransmittances(unsigned int id, const  double & val ) 
{
   if ( m_Values.size()<id+1 )
    { 
      for(unsigned int j=0; j<(id+1-m_Values.size());j++)
	{
	  ValueType temp = AtmosphericRadiativeTermsSingleChannel::New();
	  m_Values.push_back(temp);
	}
    }
  m_Values[id]->SetUpwardTransmittance(val);
} 

/** GET ACCESSORS WITH VECTORS. */
AtmosphericRadiativeTerms::DataVectorType 
AtmosphericRadiativeTerms
::GetIntrinsicAtmosphericReflectances()
{
  DataVectorType vect(m_Values.size(), 0);
  for (unsigned int nbChannel=0; nbChannel<m_Values.size(); nbChannel++)
    {
      vect[nbChannel] = m_Values[nbChannel]->GetIntrinsicAtmosphericReflectance();
    } 
  return vect;
}
AtmosphericRadiativeTerms::DataVectorType
AtmosphericRadiativeTerms
::GetSphericalAlbedos()
{
  DataVectorType vect(m_Values.size(), 0);
  for (unsigned int nbChannel=0; nbChannel<m_Values.size(); nbChannel++)
    {
      vect[nbChannel] = m_Values[nbChannel]->GetIntrinsicAtmosphericReflectance();
    } 
  return vect;
}
AtmosphericRadiativeTerms::DataVectorType
AtmosphericRadiativeTerms
::GetTotalGaseousTransmissions()
{
  DataVectorType vect(m_Values.size(), 0);
  for (unsigned int nbChannel=0; nbChannel<m_Values.size(); nbChannel++)
    {
      vect[nbChannel] = m_Values[nbChannel]->GetIntrinsicAtmosphericReflectance();
    } 
  return vect;
}
AtmosphericRadiativeTerms::DataVectorType
AtmosphericRadiativeTerms
::GetDownwardTransmittances() 
{
  DataVectorType vect(m_Values.size(), 0);
  for (unsigned int nbChannel=0; nbChannel<m_Values.size(); nbChannel++)
    {
      vect[nbChannel] = m_Values[nbChannel]->GetIntrinsicAtmosphericReflectance();
    } 
  return vect;
}
AtmosphericRadiativeTerms::DataVectorType
AtmosphericRadiativeTerms
::GetUpwardTransmittances()
{
  DataVectorType vect(m_Values.size(), 0);
  for (unsigned int nbChannel=0; nbChannel<m_Values.size(); nbChannel++)
    {
      vect[nbChannel] = m_Values[nbChannel]->GetIntrinsicAtmosphericReflectance();
    } 
  return vect;
}
/** GET ACCESSORS WITH INDEX. */
double
AtmosphericRadiativeTerms
::GetIntrinsicAtmosphericReflectances(unsigned int id)
{
  return m_Values[id]->GetIntrinsicAtmosphericReflectance();
}
double
AtmosphericRadiativeTerms
::GetSphericalAlbedos(unsigned int id) 
{
  return m_Values[id]->GetSphericalAlbedo();
}
double
AtmosphericRadiativeTerms
::GetTotalGaseousTransmissions(unsigned int id)
{
  return m_Values[id]->GetTotalGaseousTransmission();
}
double 
AtmosphericRadiativeTerms
::GetDownwardTransmittances(unsigned int id) 
{
  return m_Values[id]->GetDownwardTransmittance();
}
double 
AtmosphericRadiativeTerms
::GetUpwardTransmittances(unsigned int id)
{
  return m_Values[id]->GetUpwardTransmittance();
}

const AtmosphericRadiativeTerms::ValueType 
AtmosphericRadiativeTerms
::GetValueByIndex(unsigned int id) const
{
  return m_Values[id];
}

/**PrintSelf method */
void 
AtmosphericRadiativeTerms
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  for (unsigned int i=0; i<m_Values.size(); i++)
    {
      os << indent << "Channel "<< i << " : "<< std::endl;
      //ValueType::(os,indent);
    }
}


} // end namespace otb

