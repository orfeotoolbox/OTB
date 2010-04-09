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
#ifndef __otbPostGISQueryTransactor_h
#define __otbPostGISQueryTransactor_h

//#include <pqxx/pqxx>
#include <string>

#include "otbPostGISFromStringTransactor.h"

namespace otb
{

/** \class PostGISQueryTransactor
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

class PostGISQueryTransactor :
  public otb::PostGISFromStringTransactor
{

public:

  typedef PostGISFromStringTransactor Superclass;
  //typedef pqxx::transactor<pqxx::nontransaction> Superclass;
  //typedef pqxx::result ResultType;
  PostGISQueryTransactor();

  PostGISQueryTransactor(const PostGISQueryTransactor& pgt);

  PostGISQueryTransactor& operator =(const PostGISQueryTransactor& pgt)
    throw();

  void operator ()(pqxx::nontransaction& T);

  void SetRemoveExistingView(bool val);

  bool GetRemoveExistingView() const;

  void SetViewName(const std::string& aName);

  std::string GetViewName() const;

  //void CreateView(pqxx::nontransaction &T);

protected:
  std::string m_ViewName;
  bool        m_RemoveExistingView;
};

} // end namespace otb

#endif
