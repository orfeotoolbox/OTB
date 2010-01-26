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
#include "otbPostGISFromStringTransactor.h"
#include <sstream>
#include "otbMacro.h"


namespace otb
{

PostGISFromStringTransactor::PostGISFromStringTransactor() : PostGISFromStringTransactor::Superclass("FromStringTransactor") {
  m_TransactionString = "";
   };

PostGISFromStringTransactor::PostGISFromStringTransactor(const PostGISFromStringTransactor& pgt)
  : PostGISFromStringTransactor::Superclass("FromStringTransactor")
{
  m_TransactionString = pgt.GetTransactionString();
}

PostGISFromStringTransactor& PostGISFromStringTransactor::operator=(const PostGISFromStringTransactor& pgt) throw() {
    m_TransactionString = pgt.GetTransactionString();
    return *this;
}
  
void PostGISFromStringTransactor::operator()(pqxx::nontransaction &T)
{

  if(m_TransactionString!="")
    {
  
    otbGenericMsgDebugMacro(<<"Transaction Command " << m_TransactionString);
  
    m_Result = T.exec(m_TransactionString);
    }
  
}

void PostGISFromStringTransactor::on_commit()
{
  std::cout << "\t Transaction \t"  << std::endl;
  std::cout << "\t "<< m_TransactionString  << std::endl;
  
}

std::string PostGISFromStringTransactor::GetTransactionString() const
{
  return m_TransactionString;
}

void PostGISFromStringTransactor::SetTransactionString(const std::string& aName)
{
  m_TransactionString = aName;
}

PostGISFromStringTransactor::ResultType PostGISFromStringTransactor::GetResult() const
{
  return m_Result;
}


} // end namespace otb


