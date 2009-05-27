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
#ifndef __otbGISConnection_cxx
#define __otbGISConnection_cxx

#include "otbGISConnection.h"
#include <sstream>

namespace otb
{
GISConnection::GISConnection()
{
  m_Host = "localhost";
  m_DBName = "";
  m_User = "postgres";
  m_Password = "";
  m_Port = "";
  m_Options = "";

  
}

GISConnection::~GISConnection()
{
 
}

void GISConnection::ConnectToDB()
{
  std::stringstream connstring;

  connstring << "host=" << m_Host << " dbname=" << m_DBName << " user=" << m_User ;

  if(m_Password!="")
    connstring << " password=" << m_Password;

  if(m_Port!="")
    connstring << " port=" << m_Port;

  if(m_Options!="")
    connstring << " options=" << m_Options;

  delete m_PostGISConnection;

  m_PostGISConnection = new BasicConnectionType(connstring.str().c_str());

}

void GISConnection::PerformTransaction(const TransactorType& theTransaction)
{
  //m_PostGISConnection->perform( theTransaction() );
}



void GISConnection::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os<<std::endl;

  os<<"DB name: " << m_DBName <<std::endl;

}

} // end namespace otb

#endif
