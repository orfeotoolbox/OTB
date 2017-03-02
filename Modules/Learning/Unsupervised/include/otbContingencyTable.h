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

#ifndef otbContingencyTable_h
#define otbContingencyTable_h

#include <vector>
#include <iostream>
#include <iomanip>

namespace otb
{
template<class TClassLabel>
class ContingencyTable
{
public:
  typedef itk::VariableSizeMatrix<unsigned long> MatrixType;
  typedef std::vector<TClassLabel> LabelList;

  ContingencyTable(LabelList referenceLabels, LabelList producedLabels) : refLabels( referenceLabels ),
                                                                          prodLabels( producedLabels )
  {
    unsigned int rows = static_cast<unsigned int>(refLabels.size());
    unsigned int cols = static_cast<unsigned int>(prodLabels.size());
    matrix.SetSize( rows, cols );
    matrix.Fill( 0 );
  }

  MatrixType matrix;

  friend std::ostream &operator<<(std::ostream &o, const ContingencyTable<TClassLabel> &contingencyTable)
  {

    // Retrieve the maximal width from the matrix and the labels
    size_t maxWidth = 6;
    for( size_t i = 0; i << contingencyTable.prodLabels.size(); ++i )
      {
      std::ostringstream oss;
      oss << contingencyTable.prodLabels[i];
      size_t length = oss.str().length();
      if( length > maxWidth )
        maxWidth = length;
      }

    for( size_t i = 0; i << contingencyTable.refLabels.size(); ++i )
      {
      std::ostringstream oss;
      oss << contingencyTable.refLabels[i];
      size_t length = oss.str().length();
      if( length > maxWidth )
        maxWidth = length;
      }

    for( unsigned int i = 0; i < contingencyTable.matrix.Rows(); ++i )
      {
      for( unsigned int j = 0; j < contingencyTable.matrix.Cols(); ++j )
        {
        std::ostringstream oss;
        oss << contingencyTable.matrix( i, j );
        size_t length = oss.str().length();
        if( length > maxWidth )
          maxWidth = length;
        }
      }

    int width = static_cast<int>(maxWidth);

    // Write the first line of the matrix (produced labels)
    o << std::setfill(' ') << std::setw( width ) << "labels";
    for( size_t i = 0; i < contingencyTable.prodLabels.size(); ++i )
      {
      o << std::setfill(' ') << std::setw( width ) << contingencyTable.prodLabels[i];
      }
    o << std::endl;

    // For each line write the reference label, then the count value
    for( unsigned int i = 0; i < contingencyTable.matrix.Rows(); ++i )
      {
      o << std::setfill(' ') << std::setw( width ) << contingencyTable.refLabels[i];
      for( unsigned int j = 0; j < contingencyTable.matrix.Cols(); ++j )
        {
        o << std::setfill(' ') << std::setw( width ) << contingencyTable.matrix( i, j );
        }
      o << std::endl;
      }

    return o;
  }

  std::string to_csv() const
  {
    const char separator = ',';

    std::ostringstream oss;
    oss << "labels";
    for( size_t i = 0; i < prodLabels.size(); ++i )
      {
      oss << separator << prodLabels[i];
      }
    oss << std::endl;

    // For each line write the reference label, then the count value
    for( unsigned int i = 0; i < matrix.Rows(); ++i )
      {
      oss << refLabels[i];
      for( unsigned int j = 0; j < matrix.Cols(); ++j )
        {
        oss << separator << matrix( i, j );
        }
      oss << std::endl;
      }
    oss << std::endl;

    return oss.str();
  }

private:
  LabelList refLabels;
  LabelList prodLabels;


};
}

#endif //otbContingencyTable_h
