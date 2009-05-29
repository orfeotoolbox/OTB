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

  CreateTable() : pqxx::transactor<pqxx::nontransaction>("CreateTable") {}

  void operator()(argument_type &T)
  {
    std::sstream createCommand;

    createCommand = "CREATE TABLE "<< m_TableName
				   <<" (id serial PRIMARY KEY,genre text);"
      
    m_Result = T.exec(createCommand.str());

    std::sstream addGeometryCommand;

    addGeometryCommand = "SELECT AddGeometryColumn( '"<< m_TableName <<
      "', 'geom', "<< m_SRID <<", 'GEOMETRY',"<< m_Dimension <<" );"
    m_Result = T.exec();

    
  }

  void on_commit()
  {
      std::cout << "\t Table is created \t"  << std::endl;

  }

  itkGetMacro(TableName, std::string);
  itkSetMacro(TableName, std::string);

  itkGetMacro(SRID, int);
  itkSetMacro(SRID, int);

  itkGetMacro(Dimension, unisgned short);
  itkSetMacro(Dimension, unisgned short);

  itkGetMacro(Result, ResultType);
  
protected:
  ResultType m_Result;
  std::string m_TableName;
  int m_SRID;
  unsigned short m_Dimension:
};

} // end namespace otb



#endif
