
#ifndef _otb_VectorData_txx
#define _otb_VectorData_txx

#include "otbVectorData.h"

namespace otb
{

template <class TPrecision, unsigned int VDimension>
VectorData<TPrecision,VDimension>
::VectorData()
{
  m_DataTree = DataTreeType::New();
  m_DataTree->SetRoot(DataNodeType::New());
}

template <class TPrecision, unsigned int VDimension>
void
VectorData<TPrecision,VDimension>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os<<indent<<m_DataTree;
}
} // end namespace otb


#endif
