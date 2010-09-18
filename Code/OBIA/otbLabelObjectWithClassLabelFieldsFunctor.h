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
#ifndef __otbLabelObjectWithClassLabelFieldsFunctor_h
#define __otbLabelObjectWithClassLabelFieldsFunctor_h

#include "itkLabelObject.h"
#include <map>

namespace otb
{

namespace Functor
{

/** \class LabelObjectWithClassLabelFieldsFunctor
 *  \brief Returns additionnal OGR fields to be saved in VectorData
 *
 * \sa LabelObjectFieldsFunctor
 */
template<class TLabelObject>
class ITK_EXPORT LabelObjectWithClassLabelFieldsFunctor
{
public:
  // The Label Object typedef
  typedef TLabelObject                                  LabelObjectType;
  typedef std::map<std::string, std::string>            FieldsMapType;

  FieldsMapType operator ()(LabelObjectType * labelObject) const
  {
    typename LabelObjectType::ClassLabelType classLabel = labelObject->GetClassLabel();
    std::ostringstream oss;
    oss << classLabel;

    FieldsMapType fields;
    fields["Class"] = oss.str();
    return fields;
  }
};

} // end namespace Functor

} // end namespace otb
#endif
