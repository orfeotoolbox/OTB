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

#ifndef __otbPathListToHistogramGenerator_txx
#define __otbPathListToHistogramGenerator_txx

#include "otbPathListToHistogramGenerator.h"

namespace otb
{

template<class TPath, class TFunction>
PathListToHistogramGenerator<TPath, TFunction>
::PathListToHistogramGenerator()
{
  m_HistogramGenerator = GeneratorType::New();

  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);

  this->itk::ProcessObject::SetNthOutput(0, this->MakeOutput(0).GetPointer());
}

template<class TPath, class TFunction>
void
PathListToHistogramGenerator<TPath, TFunction>
::SetInput(const PathListType* path)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<PathListType*>(path));
}

template<class TPath, class TFunction>
const typename PathListToHistogramGenerator<TPath, TFunction>::PathListType*
PathListToHistogramGenerator<TPath, TFunction>
::GetInput() const
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }
  return static_cast<const PathListType* >
    (this->itk::ProcessObject::GetInput(0) );
}

/**
 * //FIXME : should be done in a HistogramSource
 */
template<class TPath, class TFunction>
void
PathListToHistogramGenerator<TPath, TFunction>
::GraftOutput(itk::DataObject *graft)
{
  this->GraftNthOutput(0, graft);
}

/**
 * //FIXME : should be done in a HistogramSource
 */

template<class TPath, class TFunction>
void
PathListToHistogramGenerator<TPath, TFunction>
::GraftNthOutput(unsigned int idx, itk::DataObject *graft)
{
  if (idx >= this->GetNumberOfOutputs())
    {
    itkExceptionMacro(<< "Requested to graft output " << idx <<
                      " but this filter only has " << this->GetNumberOfOutputs() << " Outputs.");
    }

  if (!graft)
    {
    itkExceptionMacro(<< "Requested to graft output that is a NULL pointer");
    }

  itk::DataObject * output = const_cast<HistogramType*>(this->GetOutput());

  // Call GraftImage to copy meta-information, regions, and the pixel container
  output->Graft(graft);
}

template<class TPath, class TFunction>
typename PathListToHistogramGenerator<TPath, TFunction>::DataObjectPointer
PathListToHistogramGenerator<TPath, TFunction>
::MakeOutput(unsigned int)
{
  DataObjectPointer output;
  output = static_cast<itk::DataObject*>(HistogramType::New().GetPointer());
  return output;
}


template<class TPath, class TFunction>
const typename PathListToHistogramGenerator<TPath, TFunction>::HistogramType *
PathListToHistogramGenerator<TPath, TFunction>
::GetOutput() const
{
  return dynamic_cast<const HistogramType*>(this->itk::ProcessObject::GetOutput(0));
}

template<class TPath, class TFunction>
void
PathListToHistogramGenerator<TPath, TFunction>
::GenerateData()
{
  // Get the input pathlist
  typename PathListType::ConstPointer pathList = this->GetInput();

  // Path pointer
  PathPointer vertexList;

  ListSamplePointer listSample = ListSampleType::New();
  int               nbElementPathList = pathList->Size();

  ListSampleVectorType ResultFunction;

  typename TFunction::Pointer function = TFunction::New();

  for (int noPathList = 0; noPathList < nbElementPathList; noPathList++)
    {
    vertexList = pathList->GetNthElement(noPathList); //(*pathList)[noPathList];
    function->SetInputPath(vertexList);

    ResultFunction[0] = static_cast<MeasurementType>(function->Evaluate());
    // Set the ListSample MeasurementVectorSize once before the PushBack
    if(noPathList==0)
      listSample->SetMeasurementVectorSize(ResultFunction.Size());

    listSample->PushBack(ResultFunction);
    }

  m_HistogramGenerator->SetInput(listSample);
  m_HistogramGenerator->GraftOutput(const_cast<HistogramType*>(this->GetOutput()));
  m_HistogramGenerator->Update();
  this->GraftOutput(const_cast<HistogramType*>(m_HistogramGenerator->GetOutput()));
}

template<class TPath, class TFunction>
void
PathListToHistogramGenerator<TPath, TFunction>
::SetNumberOfBins(const SizeType& size)
{
  m_HistogramGenerator->SetHistogramSize(size);
}

template<class TPath, class TFunction>
void
PathListToHistogramGenerator<TPath, TFunction>
::SetMarginalScale(double marginalScale)
{
  m_HistogramGenerator->SetMarginalScale(marginalScale);
}

template<class TPath, class TFunction>
void
PathListToHistogramGenerator<TPath, TFunction>
::SetHistogramMin(const MeasurementVectorType& histogramMin)
{
  m_HistogramGenerator->SetHistogramMin(histogramMin);
}

template<class TPath, class TFunction>
void
PathListToHistogramGenerator<TPath, TFunction>
::SetHistogramMax(const MeasurementVectorType& histogramMax)
{
  m_HistogramGenerator->SetHistogramMax(histogramMax);
}

template<class TPath, class TFunction>
void
PathListToHistogramGenerator<TPath, TFunction>
::SetAutoMinMax(bool autoMinMax)
{
  m_HistogramGenerator->SetAutoMinMax(autoMinMax);
}

template<class TPath, class TFunction>
void
PathListToHistogramGenerator<TPath, TFunction>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << "PathList = " << this->GetInput() << std::endl;
  os << "HistogramGenerator = " << m_HistogramGenerator << std::endl;
}

} // end of namespace otb

#endif
