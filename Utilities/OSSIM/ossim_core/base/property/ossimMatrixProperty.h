#ifndef ossimMatrixProperty_HEADER
//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimMatrixProperty.h,v 1.6 2004/05/19 18:29:38 dburken Exp $
#define ossimMatrixProperty_HEADER
#include <vector>
#include "ossimProperty.h"


class OSSIMDLLEXPORT ossimMatrixProperty : public ossimProperty
{
public:
   ossimMatrixProperty(const ossimString& name=ossimString(""),
                       const std::vector<double>& values = std::vector<double>(),
                       int numberOfRows    = 0,
                       int numberOfColumns = 0);
   ossimMatrixProperty(const ossimMatrixProperty& rhs);

   virtual ~ossimMatrixProperty();

   virtual ossimObject* dup()const;
   virtual const ossimProperty& assign(const ossimProperty& rhs);
  virtual bool setValue(const ossimString& value);
   virtual void valueToString(ossimString& valueResult)const;
   
   void resize(int numberOfRows,
               int numberOfColumns);

   double& operator ()(int rowIdx,
                       int colIdx);
   
   const double& operator()(int rowIdx,
                            int colIdx)const;

   int getNumberOfRows()const;
   int getNumberOfCols()const;

   void clearConstraints();
   
   /*!
    * If any of the contraints are set to something < 0 then it means there
    * isn't a constraint for that value.  So if you give a min value of -1
    * and a max value of 10 then the column can grow from 10 downto 0.
    */
   void setColContraints(int minNumberOfColumns,
                         int maxNumberOfColumns);

   /*!
    * If any of the contraints are set to something < 0 then it means there
    * isn't a constraint for that value.  So if you give a min value of -1
    * and a max value of 10 then the row can grow from 10 downto 0.
    */
   void setRowContraints(int minNumberOfRows,
                         int maxNumberOfRows);

   void getColContraints(int& minNumberOfColumns,
                         int& maxNumberOfColumns);
   
   void getRowContraints(int& minNumberOfRows,
                         int& maxNumberOfRows);


   void normalize();
   void zero();
   
protected:
   std::vector< std::vector<double> > theValueArray;
   int theMinNumberOfCols;
   int theMaxNumberOfCols;
   int theMinNumberOfRows;
   int theMaxNumberOfRows;

   double density()const;
TYPE_DATA
};

#endif
