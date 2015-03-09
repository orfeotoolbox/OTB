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
#ifndef __otbLabelMapToVectorDataFilter_txx
#define __otbLabelMapToVectorDataFilter_txx

#include "otbLabelMapToVectorDataFilter.h"

namespace otb {

template<class TLabelMap, class TVectorData, class TFieldsFunctor>
LabelMapToVectorDataFilter<TLabelMap, TVectorData, TFieldsFunctor>
::LabelMapToVectorDataFilter()
{ /*
  m_BackgroundValue = NumericTraits<OutputImagePixelType>::NonpositiveMin(); */
}

template<class TLabelMap, class TVectorData, class TFieldsFunctor>
void
LabelMapToVectorDataFilter<TLabelMap, TVectorData, TFieldsFunctor>
::SetInput(const InputLabelMapType *input)
{
// Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<InputLabelMapType *>(input));
}

template<class TLabelMap, class TVectorData, class TFieldsFunctor>
void
LabelMapToVectorDataFilter<TLabelMap, TVectorData, TFieldsFunctor>
::SetInput(unsigned int idx, const InputLabelMapType *input)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(idx,
                                        const_cast<InputLabelMapType *>(input));
}

template<class TLabelMap, class TVectorData, class TFieldsFunctor>
const typename LabelMapToVectorDataFilter<TLabelMap, TVectorData, TFieldsFunctor>::InputLabelMapType *
LabelMapToVectorDataFilter<TLabelMap, TVectorData, TFieldsFunctor>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const TLabelMap *>
           (this->itk::ProcessObject::GetInput(0));
}

template<class TLabelMap, class TVectorData, class TFieldsFunctor>
const typename LabelMapToVectorDataFilter<TLabelMap, TVectorData, TFieldsFunctor>::InputLabelMapType *
LabelMapToVectorDataFilter<TLabelMap, TVectorData, TFieldsFunctor>
::GetInput(unsigned int idx)
{
  return static_cast<const TLabelMap *>
           (this->itk::ProcessObject::GetInput(idx));
}

template<class TLabelMap, class TVectorData, class TFieldsFunctor>
void
LabelMapToVectorDataFilter<TLabelMap, TVectorData, TFieldsFunctor>
::GenerateData()
{
  OutputVectorDataType *    output = this->GetOutput();
  const InputLabelMapType * input = this->GetInput();

  FunctorType functor;
  CorrectFunctorType correctFunctor;

  /**Create usual root elements of the output vectordata*/
  DataNodePointerType document = DataNodeType::New();
  DataNodePointerType folder1 = DataNodeType::New();

  document->SetNodeType(DOCUMENT);
  folder1->SetNodeType(FOLDER);
  DataNodePointerType root = output->GetDataTree()->GetRoot()->Get();
  output->GetDataTree()->Add(document, root);
  output->GetDataTree()->Add(folder1, document);

  // Lets begin by declaring the iterator for the objects in the image.
  ConstIteratorType it = ConstIteratorType( input );

  while ( !it.IsAtEnd() )
    {
    /**the label object*/
    const LabelObjectType * labelObject = it.GetLabelObject();

    /**Get the polygon image of the labelobject using the functor*/
    typename PolygonType::Pointer polygon = functor(const_cast <LabelObjectType *> (labelObject));

    /**correct polygon if necessary*/
    PolygonPointerType correctPolygon = correctFunctor(polygon);
    DataNodePointerType node = DataNodeType::New();
    node->SetNodeType(otb::FEATURE_POLYGON);

    /**Store the label in the NodeId*/
    std::ostringstream oss;
    oss << labelObject->GetLabel();
    node->SetNodeId(oss.str());
    //TODO hole in the polygon are not handle yet by the functor
    node->SetPolygonExteriorRing(correctPolygon);

    /** Store additionnal fields given by functor */
    std::map<std::string, std::string> fields = m_FieldsFunctor(const_cast <LabelObjectType *> (labelObject));
    std::map<std::string, std::string>::const_iterator it2;
    for ( it2 = fields.begin(); it2 != fields.end(); ++it2 )
      {
      node->SetFieldAsString(it2->first, it2->second);
      }

    /**Add the polygon to the VectorData*/
    output->GetDataTree()->Add(node, folder1);
    ++it;
    }
}

template<class TLabelMap, class TVectorData, class TFieldsFunctor>
void
LabelMapToVectorDataFilter<TLabelMap, TVectorData, TFieldsFunctor>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb
#endif
