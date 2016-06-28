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
#ifndef otbLabelObjectFieldsFunctor_h
#define otbLabelObjectFieldsFunctor_h

#include "itkLabelObject.h"
#include <map>

namespace otb
{

namespace Functor
{

/** \class LabelObjectFieldsFunctor
 *  \brief Returns additionnal OGR fields to be saved in VectorData
 *
 * \sa LabelObjectWithClassLabelFieldsFunctor
 *
 * \ingroup OTBLabelMap
 */
template<class TLabelObject>
class ITK_EXPORT LabelObjectFieldsFunctor
{
public:
  // The Label Object typedef
  typedef TLabelObject                                  LabelObjectType;
  typedef std::map<std::string, std::string>            FieldsMapType;

  FieldsMapType operator ()(LabelObjectType * itkNotUsed(labelObject)) const
  {
    // don't return anything for standard LabelObject (label is already handled by LabelmapToVectorData)
    return FieldsMapType();
  }
};

} // end namespace Functor

} // end namespace otb
#endif
