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
#ifndef __otbPostGISCreateTableTransactor_h
#define __otbPostGISCreateTableTransactor_h

#include <pqxx/pqxx>
#include <string>


namespace otb
{


/** \class PostGISCreateTableTransactor
 * \brief PQXX-based transactor for creating PostGIS tables
 *
 *
 * The copy constructor and the "=" operator have to be defined for
 * deep copy, so that libpqxx is able to get the values set for the
 * class variables.
 *
 *
 * \ingroup GISTransactors
 */

class PostGISCreateTableTransactor :
     public pqxx::transactor<pqxx::nontransaction>
{
  
public:

  typedef pqxx::result ResultType;

  typedef pqxx::transactor<pqxx::nontransaction> Superclass;

  PostGISCreateTableTransactor();

  PostGISCreateTableTransactor(const PostGISCreateTableTransactor& pgt);

  PostGISCreateTableTransactor& operator=(const PostGISCreateTableTransactor& pgt) throw();
  
  void operator()(pqxx::nontransaction &T);
  
  void on_commit();

  std::string GetTableName() const;

  void SetTableName(const std::string& aName);

  int GetSRID() const;

  void SetSRID(int aSRID);

  unsigned short GetDimension() const;

  void SetDimension(unsigned short aDim);

  void SetRemoveExistingTable(bool val);

  bool GetRemoveExistingTable() const;

  ResultType GetResult() const;
  
  void CreateGISTIndex (pqxx::nontransaction &T);
    
protected:
  
  ResultType m_Result;
  std::string m_TableName;
  int m_SRID;
  unsigned short m_Dimension;
  bool m_RemoveExistingTable;
};

} // end namespace otb


#endif
