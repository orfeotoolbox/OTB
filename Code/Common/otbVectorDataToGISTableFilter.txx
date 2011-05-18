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
#ifndef __otbVectorDataToGISTableFilter_txx
#define __otbVectorDataToGISTableFilter_txx

#include "otbVectorDataToGISTableFilter.h"
#include "itkProgressReporter.h"

namespace otb {

template<class TVectorData, class TGISTable>
VectorDataToGISTableFilter<TVectorData, TGISTable>
::VectorDataToGISTableFilter()
{
  m_InputGISConnection = InputGISConnectionType::New();
  m_DropExistingGISTable = false;
  m_GISTableName = "vectordata_to_gis";
}

template<class TVectorData, class TGISTable>
void
VectorDataToGISTableFilter<TVectorData, TGISTable>
::SetInput(const InputVectorDataType *input)
{
// Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<InputVectorDataType *>(input));
}

template<class TVectorData, class TGISTable>
void
VectorDataToGISTableFilter<TVectorData, TGISTable>
::SetInput(unsigned int idx, const InputVectorDataType *input)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(idx,
                                        const_cast<InputVectorDataType *>(input));
}

template<class TVectorData, class TGISTable>
const typename VectorDataToGISTableFilter<TVectorData, TGISTable>::InputVectorDataType *
VectorDataToGISTableFilter<TVectorData, TGISTable>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const TVectorData *>
           (this->itk::ProcessObject::GetInput(0));
}

template<class TVectorData, class TGISTable>
const typename VectorDataToGISTableFilter<TVectorData, TGISTable>::InputVectorDataType *
VectorDataToGISTableFilter<TVectorData, TGISTable>
::GetInput(unsigned int idx)
{
  return static_cast<const TVectorData *>
           (this->itk::ProcessObject::GetInput(idx));
}

template<class TVectorData, class TGISTable>
void
VectorDataToGISTableFilter<TVectorData, TGISTable>
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
template<class TInputImage, class TVectorData >
void
LabelMapToVectorDataFilter<TInputImage, TVectorData>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}
*/

template<class TVectorData, class TGISTable>
void
VectorDataToGISTableFilter<TVectorData, TGISTable>
::GenerateData()
{

  /** Allocate the output*/
  this->AllocateOutputs();

  OutputGISTableType * output = this->GetOutput();

  const InputVectorDataType * input = this->GetInput();

  /**Set the connection associated to the table*/
  output->SetConnection (this->GetInputGISConnection ());

  /**Set the projection of the GIS table with the vector data projection informations*/
  output->SetProjectionRef(input->GetProjectionRef());

  /**Connection to the database (create a transactor to manage the connection)*/
  output->GetConnection()->ConnectToDB();

  /**Name of the table is settedd automaticcaly to "vector_data_to_gis"*/
//   output->SetTableName ("vector_data_to_gis");
//output->SetTableName (this->GetGISTableName());
  output->SetTableName ("");
  /**Create the PostgreSQL table*/
  //output->CreateTable(m_DropExistingGISTable);

  /**Process filter for all inputs*/
  for (unsigned int idx = 0; idx < this->GetNumberOfInputs(); ++idx)
    {
    if (this->GetInput(idx))
      {
      InputVectorDataConstPointer input = this->GetInput(idx);
      InternalTreeNodeType *      inputRoot = const_cast<InternalTreeNodeType *>(input->GetDataTree()->GetRoot());

      //Old methods for processing translation to GIS table
      //ProcessNode(inputRoot);
      /**New method for the filter, call the OGR driver for writing data to the Db*/
      OGRVectorDataIOPointerType gisWriter = OGRVectorDataIOType::New();

      /**Transform connection parametrers to the DB to an OGR connection string (based on PG: for PostGIS)"*/
      const std::string outputOGRConnStr = output->GetOGRStrConnection();

      /**Test if the driver is available*/
      if (gisWriter->CanWriteFile(outputOGRConnStr.data()))
        {
        //Write VectorData to the GIS Table using OGR translation
        gisWriter->SetFileName(outputOGRConnStr);
        otbGenericMsgDebugMacro(<< "Write vector data to GIS table " << outputOGRConnStr);

        //Write with overwrite =true option
        std::string overStr("OVERWRITE=yes");
        char *overOptions[] = { const_cast <char *> (overStr.c_str()) };
//         std::cout << *overOptions << std::endl;
        gisWriter->Write(input, overOptions);

        output->SetTableName (inputRoot->Get()->GetNodeId());
        otbMsgDevMacro( "tablename " << output->GetTableName() );
        }
      else
        {
        itkGenericExceptionMacro(<< "Not valid connection string (PG:*) " << outputOGRConnStr);
        }
      }
    }

}

template<class TVectorData, class TGISTable>
void
VectorDataToGISTableFilter<TVectorData, TGISTable>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace otb
#endif
