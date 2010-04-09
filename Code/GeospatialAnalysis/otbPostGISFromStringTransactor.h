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
#ifndef __otbPostGISFromStringTransactor_h
#define __otbPostGISFromStringTransactor_h

#include <pqxx/pqxx>
#include <string>

namespace otb
{

/** \class PostGISFromStringTransactor
 * \brief PQXX-based transactor for executing PostGIS queries.
 *
 *
 * The copy constructor and the "=" operator have to be defined for
 * deep copy, so that libpqxx is able to get the values set for the
 * class variables.
 *
 *
 * \ingroup GISTransactors
 */

class PostGISFromStringTransactor :
  public pqxx::transactor<pqxx::nontransaction>
{

public:

  typedef pqxx::result ResultType;

  typedef pqxx::transactor<pqxx::nontransaction> Superclass;

  PostGISFromStringTransactor();

  PostGISFromStringTransactor(const PostGISFromStringTransactor& pgt);

  PostGISFromStringTransactor& operator =(const PostGISFromStringTransactor& pgt)
    throw();

  void operator ()(pqxx::nontransaction& T);

  void on_commit();

  std::string GetTransactionString() const;

  void SetTransactionString(const std::string& trans);

  ResultType GetResult() const;

protected:

  ResultType  m_Result;
  std::string m_TransactionString;
};

} // end namespace otb

#endif
