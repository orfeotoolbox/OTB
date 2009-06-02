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
#ifndef __itkPostGISCreateTableTransactor_h
#define __itkPostGISCreateTableTransactor_h

#include <pqxx/pqxx>
#include <sstream>
#include <itkObject.h>


namespace otb
{


/** \class PostGISCreateTableTransactor
 * \brief PQXX-based transactor for creating PostGIS tables 
 *
 * 
 * 
 * 
 *
 * \ingroup GISTransactors
 */

class ITK_EXPORT PostGISCreateTableTransactor :
     public pqxx::transactor<pqxx::nontransaction>
{
  
public:

  typedef pqxx::result ResultType;

  typedef pqxx::transactor<pqxx::nontransaction> Superclass;

  PostGISCreateTableTransactor() : Superclass("CreateTable") {}

  void operator()(argument_type &T)
  {

    std::stringstream createCommand;

    createCommand << "CREATE TABLE "<< m_TableName
				   <<" (id serial PRIMARY KEY,genre text);";

    otbGenericMsgDebugMacro(<<"Create Command " << createCommand.str());
    m_Result = T.exec(createCommand.str());

    std::stringstream addGeometryCommand;

    addGeometryCommand << "SELECT AddGeometryColumn( '"<< m_TableName <<
      "', 'geom', "<< m_SRID <<", 'GEOMETRY',"<< m_Dimension <<" );";

    m_Result = T.exec(addGeometryCommand.str());

    
  }

  void on_commit()
  {
      std::cout << "\t Table is created \t"  << std::endl;

  }

  std::string GetTableName() const
  {
    return m_TableName;
  }

  void SetTableName(const std::string& aName)
  {
    m_TableName = aName;
  }

  int GetSRID() const
  {
    return m_SRID;
  }

  void SetSRID(int aSRID)
  {
    m_SRID = aSRID;
  }

  unsigned short GetDimension() const
  {
    return m_Dimension;
  }

  void SetDimension(unsigned short aDim)
  {
    m_Dimension = aDim;
  }

  ResultType GetResult() const
  {
    return m_Result;
  }

    
protected:
  ResultType m_Result;
  std::string m_TableName;
  int m_SRID;
  unsigned short m_Dimension;
};

} // end namespace otb



#endif
