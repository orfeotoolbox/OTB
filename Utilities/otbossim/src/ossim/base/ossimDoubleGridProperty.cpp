#include <sstream>
#include <ossim/base/ossimDoubleGridProperty.h>

RTTI_DEF1(ossimDoubleGridProperty, "ossimDoubleGridProperty", ossimProperty);

ossimDoubleGridProperty::ossimDoubleGridProperty(const ossimString& name,
						 int /* numberOfRows */,
						 int /* numberOfCols */,
						 const std::vector<double>& /* values*/ )
  :ossimProperty(name),
   theMinNumberOfCols(-1),
   theMaxNumberOfCols(-1),
   theMinNumberOfRows(-1),
   theMaxNumberOfRows(-1)
{

}

ossimDoubleGridProperty::ossimDoubleGridProperty(const ossimDoubleGridProperty& rhs)
  :ossimProperty(rhs),
  theMinNumberOfCols(rhs.theMinNumberOfCols),
  theMaxNumberOfCols(rhs.theMaxNumberOfCols),
  theMinNumberOfRows(rhs.theMinNumberOfRows),
  theMaxNumberOfRows(rhs.theMaxNumberOfRows),
  theValues(rhs.theValues)
{
}

ossimDoubleGridProperty::~ossimDoubleGridProperty()
{
}

ossimObject* ossimDoubleGridProperty::dup()const
{
  return new ossimDoubleGridProperty(*this);
}

const ossimProperty& ossimDoubleGridProperty::assign(const ossimProperty& rhs)
{
  ossimProperty::assign(rhs);
  ossimDoubleGridProperty* rhsPtr = PTR_CAST(ossimDoubleGridProperty, &rhs);

  if(rhsPtr)
    {
      theMinNumberOfCols = rhsPtr->theMinNumberOfCols;
      theMaxNumberOfCols = rhsPtr->theMaxNumberOfCols;
      theMinNumberOfRows = rhsPtr->theMinNumberOfRows;
      theMaxNumberOfRows = rhsPtr->theMaxNumberOfRows;
      theValues          = rhsPtr->theValues;
    }
  else
    {
      setValue(rhs.valueToString());
      
    }
  
  return *this;
}

  
bool ossimDoubleGridProperty::setValue(const ossimString& value)
{
   std::istringstream in(value.c_str());
   ossimString nRows, nCols, v;
   int numberOfRows=0;
   int numberOfCols=0;
   int rowIdx = 0;
   int colIdx = 0;
   in >> nRows >> nCols;
   numberOfRows = nRows.toInt32();
   numberOfCols = nCols.toInt32();
   theValues.resize(numberOfRows);
   
   for(rowIdx = 0; rowIdx < numberOfRows; ++rowIdx)
   {
      theValues[rowIdx].resize(numberOfCols);
      for(colIdx = 0; colIdx < numberOfCols; ++ colIdx)
      {
         in >> v;
         theValues[rowIdx][colIdx] = v.toDouble();
      }
   }
   
   return true;
}

void ossimDoubleGridProperty::valueToString(ossimString& valueResult)const
{
  std::ostringstream out;
  int rowIdx = 0;
  int colIdx = 0;
  out << getNumberOfRows() << " " << getNumberOfCols() << " ";

  for(rowIdx = 0; rowIdx < (int)getNumberOfRows(); ++rowIdx)
    {
      for(colIdx = 0; colIdx < (int)getNumberOfCols(); ++colIdx)
	{
	  out << ossimString::toString(getValue(rowIdx, colIdx)) << " ";
	}
    }
  valueResult = out.str();
}

void ossimDoubleGridProperty::clearConstraints()
{
  theMinNumberOfCols = -1;
  theMaxNumberOfCols = -1;
  theMinNumberOfRows = -1;
  theMaxNumberOfRows = -1;
}

void ossimDoubleGridProperty::setColConstraints(int minNumberOfCols,
		  int maxNumberOfCols)
{
  theMinNumberOfCols = minNumberOfCols;
  theMaxNumberOfCols = maxNumberOfCols;
}

void ossimDoubleGridProperty::setRowConstraints(int minNumberOfRows,
						int maxNumberOfRows)
{
  theMinNumberOfRows = minNumberOfRows;
  theMaxNumberOfRows = maxNumberOfRows;
}

void ossimDoubleGridProperty::setContraints(int minNumberOfRows,
					    int maxNumberOfRows,
					    int minNumberOfCols,
					    int maxNumberOfCols)
{
  theMinNumberOfRows = minNumberOfRows;
  theMaxNumberOfRows = maxNumberOfRows;
  theMinNumberOfCols = minNumberOfCols;
  theMaxNumberOfCols = maxNumberOfCols;
}

ossim_uint32 ossimDoubleGridProperty::getNumberOfRows()const
{
  return ((int)theValues.size());
}

ossim_uint32 ossimDoubleGridProperty::getNumberOfCols()const
{
  if(getNumberOfRows())
    {
      return (ossim_uint32)theValues[0].size();
    }
  return 0;
}

double ossimDoubleGridProperty::getValue(ossim_uint32 row, 
					 ossim_uint32 col)const
{
  if((row < getNumberOfRows())&&
     (col < getNumberOfCols()))
    {
      return theValues[(int)row][(int)col];
    }

  return 0.0;
}
