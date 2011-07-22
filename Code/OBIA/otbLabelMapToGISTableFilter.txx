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
#ifndef __otbLabelMapToGISTableFilter_txx
#define __otbLabelMapToGISTableFilter_txx

#include "otbLabelMapToGISTableFilter.h"
#include "itkProgressReporter.h"

namespace otb {

template<class TLabelMap, class TGISTable>
LabelMapToGISTableFilter<TLabelMap, TGISTable>
::LabelMapToGISTableFilter()
{
  m_InputGISConnection = InputGISConnectionType::New();
  m_DropExistingGISTable = false;
  //default value of the gis table name
  m_GISTableName = "labelmap_to_gis_table";
}

template<class TLabelMap, class TGISTable>
void
LabelMapToGISTableFilter<TLabelMap, TGISTable>
::SetInput(const InputLabelMapType *input)
{
// Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<InputLabelMapType *>(input));
}

template<class TLabelMap, class TGISTable>
void
LabelMapToGISTableFilter<TLabelMap, TGISTable>
::SetInput(unsigned int idx, const InputLabelMapType *input)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(idx,
                                        const_cast<InputLabelMapType *>(input));
}

template<class TLabelMap, class TGISTable>
const typename LabelMapToGISTableFilter<TLabelMap, TGISTable>::InputLabelMapType *
LabelMapToGISTableFilter<TLabelMap, TGISTable>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const TLabelMap *>
           (this->itk::ProcessObject::GetInput(0));
}

template<class TLabelMap, class TGISTable>
const typename LabelMapToGISTableFilter<TLabelMap, TGISTable>::InputLabelMapType *
LabelMapToGISTableFilter<TLabelMap, TGISTable>
::GetInput(unsigned int idx)
{
  return static_cast<const TLabelMap *>
           (this->itk::ProcessObject::GetInput(idx));
}

template<class TLabelMap, class TGISTable>
void
LabelMapToGISTableFilter<TLabelMap, TGISTable>
::GenerateInputRequestedRegion()
{ /*
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  if ( !input )
    { return; }
  input->SetRequestedRegion( input->GetLargestPossibleRegion() ); */
}

/*Distance: 5.19615
 -> Test EXIT SUCCESS.
template<class TInputImage, class TLabelMap >
void
LabelMapToLabelMapFilter<TInputImage, TLabelMap>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}
*/

template<class TLabelMap, class TGISTable>
void
LabelMapToGISTableFilter<TLabelMap, TGISTable>
::GenerateData()
{

  // Allocate the output
  this->AllocateOutputs();
  //std::cout << "before GetOutput" << std::endl;
  OutputGISTableType * output = this->GetOutput();
  //std::cout << "after GetOutput" << std::endl;
  const InputLabelMapType * input = this->GetInput();

  //Set the filter's Postgres connection
  output->SetConnection (this->GetInputGISConnection ());

  //Connection to the database
  output->GetConnection()->ConnectToDB();

  //Name of the table is set automatically to "vector_data_to_gis"
  output->SetTableName (this->GetGISTableName());

  //Create the PostgreSQL table
  output->CreateTable(m_DropExistingGISTable);
  FunctorType functor;
//   SimplifyFunctorType simplifyFunctor;
//   simplifyFunctor.SetTolerance (0.0);
//
//   CloseFunctorType closeFunctor;

  CorrectFunctorType correctFunctor;
  // Lets begin by declaring the iterator for the objects in the image.
  typename InputLabelMapType::LabelObjectContainerType::const_iterator it;
  // And get the object container to reuse it later
  const typename InputLabelMapType::LabelObjectContainerType& labelObjectContainer = input->GetLabelObjectContainer();
  for (it = labelObjectContainer.begin(); it != labelObjectContainer.end(); ++it)
    {
    // the label is there if we need it, but it can also be found at labelObject->GetLabel().
    // const PType & label = it->first;

    // the label object
    LabelObjectType *  labelObject = it->second;
    PolygonPointerType polygon = functor(labelObject);
    /**Store the label in column 'genre' of the Db*/
    std::ostringstream oss;
    oss << labelObject->GetLabel();
    //std::cout << "label : " << oss.str() << std::endl;
    //std::cout << "polygon : " << polygon << std::endl;

    //Simply polygon (erase aligned points)
//     PolygonPointerType simplifyPolygon = simplifyFunctor(polygon);
    //std::cout << "simplify polygon : " << simplifyPolygon << std::endl;

    //Close polygon if necessary
//     PolygonPointerType closePolygon = closeFunctor(simplifyPolygon);
    PolygonPointerType correctPolygon = correctFunctor(polygon);
    //std::cout << "simplify polygon : " << closePolygon << std::endl;

    this->GetOutput()->InsertPolygons(static_cast<typename TGISTable::PolygonConstPointerType> (
                                        correctPolygon),
                                      static_cast<typename TGISTable::PolygonListConstPointerType> (0), oss.str());
    //Add polygon to the gis table
    }

}

template<class TLabelMap, class TGISTable>
void
LabelMapToGISTableFilter<TLabelMap, TGISTable>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  //this->GetInputGISConnection()->PrintSelf (os, indent);
/*
  os << indent << "BackgroundValue: "  << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_BackgroundValue) << std::endl;
  */
}

} // end namespace otb
#endif
