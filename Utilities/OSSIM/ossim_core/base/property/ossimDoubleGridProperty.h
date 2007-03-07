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
// $Id: ossimDoubleGridProperty.h,v 1.4 2004/05/19 18:29:38 dburken Exp $
#ifndef ossimDoubleGridProperty_HEADER
#define ossimDoubleGridProperty_HEADER
#include "ossimProperty.h"

class OSSIMDLLEXPORT ossimDoubleGridProperty : public ossimProperty
{
public:
  ossimDoubleGridProperty(const ossimString& name = ossimString(""),
			  int numberOfRows = 0,
			  int numberOfCols = 0,
			  const std::vector<double>& values = std::vector<double>());
  ossimDoubleGridProperty(const ossimDoubleGridProperty& rhs);
  
  virtual ~ossimDoubleGridProperty();

  ossimObject* dup()const;

  virtual const ossimProperty& assign(const ossimProperty& rhs);

  virtual bool setValue(const ossimString& value);
  virtual void valueToString(ossimString& valueResult)const;

  void clearConstraints();
  /*!
   * Setting both values to -1 will say no constraints.
   * if min is constrained and max not then it will not allow
   * the cols to grow any larger than the passed in max.
   *
   * If the min is -1 and max not then it will not exceed the 
   * max number of cols
   */
  void setColConstraints(int minNumberOfCols,
			 int maxNumberOfCols);

  void setRowConstraints(int minNumberOfRows,
			 int maxNumberOfRows);
  
  void setContraints(int minNumberOfRows,
		     int maxNumberOfRows,
		     int minNumberOfCols,
		     int maxNumberOfCols);

  ossim_uint32 getNumberOfRows()const;
  ossim_uint32 getNumberOfCols()const;

  double getValue(ossim_uint32 row, ossim_uint32 col)const;
protected:
  int theMinNumberOfCols;
  int theMaxNumberOfCols;
  int theMinNumberOfRows;
  int theMaxNumberOfRows;
  std::vector< std::vector<double> > theValues;
    

TYPE_DATA
};
#endif 
