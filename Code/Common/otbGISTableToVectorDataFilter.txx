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
#ifndef __otbGISTableToVectorDataFilter_txx
#define __otbGISTableToVectorDataFilter_txx

#include "otbGISTableToVectorDataFilter.h"
//#include "itkNumericTraits.h"
//#include "itkProgressReporter.h"
//#include "itkImageRegionConstIteratorWithIndex.h"
//#include "otbGISTable.h"

namespace otb {

template<class TGISTable, class TVectorData>
GISTableToVectorDataFilter<TGISTable, TVectorData>
::GISTableToVectorDataFilter()
{ /*
  m_BackgroundValue = NumericTraits<OutputImagePixelType>::NonpositiveMin(); */
  m_Reader = VectorDataFileReaderType::New();
}

template<class TGISTable, class TVectorData>
void
GISTableToVectorDataFilter<TGISTable, TVectorData>
::SetInput(const InputGISTableType *input)
{
// Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<InputGISTableType *>(input));
}

template<class TGISTable, class TVectorData>
void
GISTableToVectorDataFilter<TGISTable, TVectorData>
::SetInput(unsigned int idx, const InputGISTableType *input)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(idx,
                                        const_cast<InputGISTableType *>(input));
}

template<class TGISTable, class TVectorData>
const typename GISTableToVectorDataFilter<TGISTable, TVectorData>::InputGISTableType *
GISTableToVectorDataFilter<TGISTable, TVectorData>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const TGISTable *>
           (this->itk::ProcessObject::GetInput(0));
}

template<class TGISTable, class TVectorData>
const typename GISTableToVectorDataFilter<TGISTable, TVectorData>::InputGISTableType *
GISTableToVectorDataFilter<TGISTable, TVectorData>
::GetInput(unsigned int idx)
{
  return static_cast<const TGISTable *>
           (this->itk::ProcessObject::GetInput(idx));
}

template<class TGISTable, class TVectorData>
void
GISTableToVectorDataFilter<TGISTable, TVectorData>
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

/*
template<class TGISTable, class TVectorData >
void
GISTableToVectorDataFilter<TGISTable, TVectorData>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}
*/

template<class TGISTable, class TVectorData>
void
GISTableToVectorDataFilter<TGISTable, TVectorData>
::GenerateData()
{

  // Allocate the output
  //this->AllocateOutputs();

  OutputVectorDataType *    output = this->GetOutput();
  const InputGISTableType * input = this->GetInput();

  /**Create usual root elements of the output vectordata*/
//   DataNodePointerType document = DataNodeType::New();
//   DataNodePointerType folder1 = DataNodeType::New();
//
//   document->SetNodeType(DOCUMENT);
//   folder1->SetNodeType(FOLDER);
//
//   DataNodePointerType root = output->GetDataTree()->GetRoot()->Get();
//
//   output->GetDataTree()->Add(document, root);
//   output->GetDataTree()->Add(folder1, document);

  OGRVectorDataIOPointerType gisReader = OGRVectorDataIOType::New();

  const std::string inputOGRConnStr = input->GetOGRStrConnection();
  //Try  if the Db is readable
//   std::cout << "ogr connection: " << inputOGRConnStr[0] << std::endl;
  if (gisReader->CanReadFile(inputOGRConnStr.data()))
    {
    //Read GISTable data and copy in the output VectorData

    gisReader->SetFileName(inputOGRConnStr);
//     std::cout << "read ogr!!: " << inputOGRConnStr << std::endl;
    gisReader->Read(output);
    }
  else
    {
    itkExceptionMacro(<< "The OGR connection is not valid; ogrconnection = " << inputOGRConnStr);
    }

//   const std::string inputOGRConnStr=input->GetOGRStrConnection();
//Try  if the Db is readable
//std::cout << "ogr connection: " << inputOGRConnStr[0] << std::endl;

//   m_Reader->SetFileName( inputOGRConnStr.data() );
//   m_Reader->Update();

  output = m_Reader->GetOutput();
}

template<class TGISTable, class TVectorData>
void
GISTableToVectorDataFilter<TGISTable, TVectorData>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
/*
  os << indent << "BackgroundValue: "  << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_BackgroundValue) << std::endl;
  */
}

} // end namespace otb
#endif
