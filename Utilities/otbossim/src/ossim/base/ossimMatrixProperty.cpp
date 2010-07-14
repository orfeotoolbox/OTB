//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimMatrixProperty.cpp 17074 2010-04-14 18:49:11Z dburken $
#include <sstream>
#include <ossim/base/ossimMatrixProperty.h>
#include <ossim/base/ossimCommon.h>

RTTI_DEF1(ossimMatrixProperty, "ossimMatrixProperty", ossimProperty);

ossimMatrixProperty::ossimMatrixProperty(const ossimString& name,
                                         const std::vector<double>& values,
                                         int numberOfRows,
                                         int numberOfCols)
   :ossimProperty(name),
    theMinNumberOfCols(-1),
    theMaxNumberOfCols(-1),
    theMinNumberOfRows(-1),
    theMaxNumberOfRows(-1)
{
   resize(numberOfRows, numberOfCols);

   if((int)values.size() == numberOfRows*numberOfCols)
   {
      int rowIdx = 0;
      int colIdx = 0;
      int linearIdx = 0;
      for(rowIdx = 0; rowIdx < getNumberOfRows(); ++rowIdx)
      {
         for(colIdx = 0; colIdx < getNumberOfCols(); ++colIdx)
         {
            theValueArray[rowIdx][colIdx] = values[linearIdx];
            ++linearIdx;
         }
      }
   }
}


ossimMatrixProperty::ossimMatrixProperty(const ossimMatrixProperty& rhs)
   :ossimProperty(rhs),
    theValueArray(rhs.theValueArray),
    theMinNumberOfCols(rhs.theMinNumberOfCols),
    theMaxNumberOfCols(rhs.theMaxNumberOfCols),
    theMinNumberOfRows(rhs.theMaxNumberOfCols),
    theMaxNumberOfRows(rhs.theMaxNumberOfRows)
{
}

ossimMatrixProperty::~ossimMatrixProperty()
{
   
}

ossimObject* ossimMatrixProperty::dup()const
{
   return new ossimMatrixProperty(*this);
}


const ossimProperty& ossimMatrixProperty::assign(const ossimProperty& rhs)
{
   ossimMatrixProperty* rhsPtr = PTR_CAST(ossimMatrixProperty,
                                          &rhs);
   if(rhsPtr)
   {
      theValueArray      = rhsPtr->theValueArray;
      theMinNumberOfCols = rhsPtr->theMinNumberOfCols;
      theMaxNumberOfCols = rhsPtr->theMaxNumberOfCols;
      theMinNumberOfRows = rhsPtr->theMinNumberOfRows;
      theMaxNumberOfRows = rhsPtr->theMaxNumberOfRows;
   }

   return ossimProperty::assign(rhs);
}

bool ossimMatrixProperty::setValue(const ossimString& value)
{
   std::istringstream in(value);
   
   ossimString numberOfRows;
   ossimString numberOfCols;
   ossimString tempValue;
   
   in>>numberOfRows >> numberOfCols;

   if(!in.bad())
   {
      resize(numberOfRows.toInt32(), numberOfCols.toInt32());

      int rowIdx = 0;
      int colIdx = 0;
      for(rowIdx = 0; ((rowIdx < getNumberOfRows())&&(!in.bad())); ++rowIdx)
      {
         for(colIdx = 0; ((colIdx < getNumberOfCols())&&(!in.bad()));++ colIdx)
         {
            in >> tempValue;
            
            theValueArray[rowIdx][colIdx] = tempValue.toDouble();
         }
      }
   }

   return !in.bad();
}

void ossimMatrixProperty::valueToString(ossimString& valueResult)const
{
   if(theValueArray.size()>0)
   {
      if(theValueArray[0].size() > 0)
      {
         valueResult = "";
         
         int rowIdx = 0;
         int colIdx = 0;
         valueResult += ossimString::toString(getNumberOfRows()) + " " +
                        ossimString::toString(getNumberOfCols()) + " ";
         for(rowIdx = 0; rowIdx < getNumberOfRows();++rowIdx)
         {
            for(colIdx = 0; colIdx < getNumberOfCols(); ++colIdx)
            {
               valueResult += ossimString::toString(theValueArray[rowIdx][colIdx]);
               valueResult += " ";
            }
         }
      }
   }
}

void ossimMatrixProperty::resize(int numberOfRows,
                                 int numberOfCols)
{
   int tempNumberOfRows = numberOfRows;
   int tempNumberOfCols = numberOfCols;

   if(theMinNumberOfCols > 0)
   {
      if(tempNumberOfCols < theMinNumberOfCols)
      {
         tempNumberOfCols = theMinNumberOfCols;
      }
   }
   if(theMinNumberOfRows > 0)
   {
      if(tempNumberOfRows < theMinNumberOfRows)
      {
         tempNumberOfRows = theMinNumberOfRows;
      }
   }
   if(theMaxNumberOfCols > 0)
   {
      if(tempNumberOfCols > theMaxNumberOfCols)
      {
         tempNumberOfCols = theMaxNumberOfCols;
      }
   }
   if(theMaxNumberOfRows > 0)
   {
      if(tempNumberOfRows > theMaxNumberOfRows)
      {
         tempNumberOfRows = theMaxNumberOfRows;
      }
   }

   int currentNumberOfRows = getNumberOfRows();
   int currentNumberOfCols = getNumberOfCols();
   
   if((currentNumberOfRows != tempNumberOfRows) ||
      (currentNumberOfCols != tempNumberOfCols))
   {
      std::vector< std::vector<double> > tempValue = theValueArray;
      
      theValueArray.resize(tempNumberOfRows);

      int minCols = ossim::min((int)currentNumberOfCols,
                             (int)tempNumberOfCols);
      int rowIdx = 0;
      int colIdx = 0;
      
      for(rowIdx = 0 ; rowIdx < tempNumberOfRows; ++rowIdx)
      {
         theValueArray[rowIdx].resize(tempNumberOfCols);
         std::fill(theValueArray[rowIdx].begin(), theValueArray[rowIdx].end(), 0);

         if(tempNumberOfRows < currentNumberOfRows)
         {
            for(colIdx = 0 ; colIdx < minCols; ++colIdx)
            {
               theValueArray[rowIdx][colIdx] = tempValue[rowIdx][colIdx];
            }
         }
      }
   }
}

double& ossimMatrixProperty::operator()(int rowIdx,
                                        int colIdx)
{
   return theValueArray[rowIdx][colIdx];
}

const double& ossimMatrixProperty::operator()(int rowIdx,
                                              int colIdx)const
{
   return theValueArray[rowIdx][colIdx];
}

int ossimMatrixProperty::getNumberOfRows()const
{
   return (int)theValueArray.size();
}

int ossimMatrixProperty::getNumberOfCols()const
{
   if(getNumberOfRows())
   {
      return (int)theValueArray[0].size();
   }

   return 0;
}

void ossimMatrixProperty::clearConstraints()
{
   theMinNumberOfCols = -1;
   theMaxNumberOfCols = -1;
   theMinNumberOfRows = -1;
   theMaxNumberOfRows = -1;
}

void ossimMatrixProperty::setColConstraints(int minNumberOfCols,
                                            int maxNumberOfCols)
{
   theMinNumberOfCols = minNumberOfCols;
   theMaxNumberOfCols = maxNumberOfCols;
}

void ossimMatrixProperty::setRowConstraints(int minNumberOfRows,
                                            int maxNumberOfRows)
{
   theMinNumberOfRows = minNumberOfRows;
   theMaxNumberOfRows = maxNumberOfRows;
}

void ossimMatrixProperty::getColConstraints(int& minNumberOfCols,
                                            int& maxNumberOfCols) const
{
   minNumberOfCols = theMinNumberOfCols;
   maxNumberOfCols = theMaxNumberOfCols;
}

void ossimMatrixProperty::getRowConstraints(int& minNumberOfRows,
                                            int& maxNumberOfRows) const
{
   minNumberOfRows = theMinNumberOfRows;
   maxNumberOfRows = theMaxNumberOfRows;
}

void ossimMatrixProperty::normalize()
{
   double densityValue = density();

   if(fabs(densityValue) <= DBL_EPSILON)
   {
      return;
   }
   int rowIdx = 0;
   int colIdx = 0;
   for(rowIdx = 0 ; rowIdx < (int)theValueArray.size(); ++rowIdx)
   {
      for(colIdx = 0 ; colIdx < (int)theValueArray[rowIdx].size(); ++colIdx)
      {
         theValueArray[rowIdx][colIdx]/=densityValue;
      }
   }
   
}

void ossimMatrixProperty::zero()
{
   int rowIdx = 0;
   int colIdx = 0;
   for(rowIdx = 0 ; rowIdx < (int)theValueArray.size(); ++rowIdx)
   {
      for(colIdx = 0 ; colIdx < (int)theValueArray[rowIdx].size(); ++colIdx)
      {
         theValueArray[rowIdx][colIdx] = 0.0;
      }
   }
}

double ossimMatrixProperty::density()const
{
   double result = 0.0;

   if((getNumberOfRows() > 0)&&
      (getNumberOfCols() > 0))
   {
      int rowIdx = 0;
      int colIdx = 0;
      
      for(rowIdx = 0 ; rowIdx < (int)theValueArray.size(); ++rowIdx)
      {
         for(colIdx = 0 ; colIdx < (int)theValueArray[rowIdx].size(); ++colIdx)
         {
            result += theValueArray[rowIdx][colIdx];
         }
      }
   }

   return result;
}
