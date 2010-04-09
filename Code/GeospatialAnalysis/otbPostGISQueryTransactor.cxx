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
#include "otbPostGISQueryTransactor.h"
#include <sstream>
#include "otbMacro.h"

namespace otb
{

PostGISQueryTransactor::PostGISQueryTransactor() : PostGISQueryTransactor::Superclass()
{
  m_ViewName = "default_view";
}

PostGISQueryTransactor::PostGISQueryTransactor(const PostGISQueryTransactor& pgt)
  : PostGISQueryTransactor::Superclass()
{
  m_TransactionString = pgt.GetTransactionString();
  m_ViewName = pgt.GetViewName();
}

PostGISQueryTransactor & PostGISQueryTransactor::operator =(const PostGISQueryTransactor& pgt)
throw()
{
  m_TransactionString = pgt.GetTransactionString();
  m_ViewName = pgt.GetViewName();
  return *this;
}

void PostGISQueryTransactor::operator ()(pqxx::nontransaction& T)
{

  if (m_RemoveExistingView)
    {
    std::stringstream dropCommand;

    //dropCommand << "DROP TABLE " << m_ViewName;
    dropCommand << "DROP VIEW IF EXISTS " << m_ViewName;

    otbGenericMsgDebugMacro(<< "Drop Command " << dropCommand.str());

    m_Result = T.exec(dropCommand.str());
    }

  std::stringstream createCommand;

  createCommand << "CREATE VIEW  " << m_ViewName
                << " AS " << m_TransactionString;

  otbGenericMsgDebugMacro(<< "Create Command " << createCommand.str());
  m_Result = T.exec(createCommand.str());

}

/*
void PostGISQueryTransactor::on_commit()
{
  std::cout << "\t Transaction \t"  << std::endl;
  std::cout << "\t "<< m_TransactionString  << std::endl;

}

std::string PostGISQueryTransactor::GetTransactionString() const
{
  return m_TransactionString;
}

void PostGISQueryTransactor::SetTransactionString(const std::string& aName)
{
  m_TransactionString = aName;
}
*/
std::string PostGISQueryTransactor::GetViewName() const
{
  return m_ViewName;
}

void PostGISQueryTransactor::SetRemoveExistingView(bool val)
{
  m_RemoveExistingView = val;
}

bool PostGISQueryTransactor::GetRemoveExistingView() const
{
  return m_RemoveExistingView;
}

void PostGISQueryTransactor::SetViewName(const std::string& aName)
{
  m_ViewName = aName;
}
/*
void PostGISQueryTransactor::CreateView(pqxx::nontransaction &T)
{
  if(m_RemoveExistingView)
  {
    std::stringstream dropCommand;

    //dropCommand << "DROP TABLE " << m_ViewName;
    dropCommand << "DROP VIEW IF EXISTS " << m_ViewName;

    otbGenericMsgDebugMacro(<<"Drop Command " << dropCommand.str());

    m_Result = T.exec(dropCommand.str());
  }

  std::stringstream createCommand;

  createCommand << "CREATE VIEW  "<< m_ViewName
      <<" AS " << m_TransactionString;

  otbGenericMsgDebugMacro(<<"Create Command " << createCommand.str());
  m_Result = T.exec(createCommand.str());

}
*/
} // end namespace otb
