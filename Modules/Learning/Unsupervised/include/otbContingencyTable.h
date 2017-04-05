/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbContingencyTable_h
#define otbContingencyTable_h

#include <vector>
#include <iostream>
#include <iomanip>
#include <itkVariableSizeMatrix.h>

namespace otb
{
template<class TClassLabel>
class ContingencyTable
{
public:
  typedef itk::VariableSizeMatrix<unsigned long> MatrixType;
  typedef std::vector<TClassLabel> LabelList;

  MatrixType matrix;

  ContingencyTable(LabelList referenceLabels, LabelList producedLabels) : refLabels( referenceLabels ),
                                                                          prodLabels( producedLabels )
  {
    unsigned int rows = static_cast<unsigned int>(refLabels.size());
    unsigned int cols = static_cast<unsigned int>(prodLabels.size());
    matrix.SetSize( rows, cols );
    matrix.Fill( 0 );
  }

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

    int width = static_cast<int>(maxWidth)+1;

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
