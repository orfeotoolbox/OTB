/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   6 avril 2006
  Version   :   
  Role      :   Fill Gaps source Class
  $Id$

=========================================================================*/
#ifndef _otbFillGapsFilter_txx
#define _otbFillGapsFilter_txx

#include "otbFillGapsFilter.h"

namespace otb
{

/**
 *
 */
FillGapsFilter::FillGapsFilter()
{
        this->itk::ProcessObject::SetNumberOfRequiredInputs(1);
        this->itk::ProcessObject::SetNumberOfRequiredOutputs(1);

        LineSpatialObjectListPointer output
                = dynamic_cast<LineSpatialObjectListType*>(this->MakeOutput(0).GetPointer()); 

        this->itk::ProcessObject::SetNthOutput(0, output.GetPointer());
}

/**
 *
 */
FillGapsFilter::~FillGapsFilter()
{
}

/**
 *
 */
itk::DataObject::Pointer
FillGapsFilter
::MakeOutput(unsigned int)
{
  LineSpatialObjectListPointer  outputList = LineSpatialObjectListType::New();
  return dynamic_cast< DataObject *>( outputList.GetPointer() );
}

/**
 *
 */

void 
FillGapsFilter
::GenerateData()
{
        //Algo
}


/**
 *
 */

void 
FillGapsFilter
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

} // end namespace otb

#endif
