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
#ifndef __otbOGRDataToClassStatisticsFilter_txx
#define __otbOGRDataToClassStatisticsFilter_txx

namespace otb
{

// Members from otb::PersistentOGRDataToClassStatisticsFilter

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::SetOGRData(const otb::ogr::DataSource* vector)
{
  this->SetNthInput(1, const_cast<otb::ogr::DataSource *>( vector ));
}

template<class TInputImage, class TMaskImage>
const otb::ogr::DataSource*
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetOGRData()
{
  if (this->GetNumberOfInputs()<2)
    {
    return 0;
    }
  return static_cast<const otb::ogr::DataSource *>(this->itk::ProcessObject::GetInput(1));
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::SetMask(const TMaskImage* mask)
{
  this->SetNthInput(2, const_cast<TMaskImage *>( mask ));
}

template<class TInputImage, class TMaskImage>
const TMaskImage*
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetMask()
{
  if (this->GetNumberOfInputs()<3)
    {
    return 0;
    }
  return static_cast<const TMaskImage *>(this->itk::ProcessObject::GetInput(2));
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::Synthetize(void)
{
  // TODO
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::Reset(void)
{
  // TODO :
}

template<class TInputImage, class TMaskImage>
const typename PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>::ClassCountObjectType*
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetClassCountOutput() const
{
  if (this->GetNumberOfOutputs()<2)
    {
    return 0;
    }
  return static_cast<const ClassCountObjectType *>(this->itk::ProcessObject::GetOutput(1));
}


template<class TInputImage, class TMaskImage>
typename PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>::ClassCountObjectType* 
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetClassCountOutput()
{
  if (this->GetNumberOfOutputs()<2)
    {
    return 0;
    }
  return static_cast<ClassCountObjectType *>(this->itk::ProcessObject::GetOutput(1));
}

// TODO: store the polygon size map as output #3
template<class TInputImage, class TMaskImage>
const typename PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>::PolygonSizeObjectType*
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetPolygonSizeOutput() const
{
  if (this->GetNumberOfOutputs()<3)
    {
    return 0;
    }
  return static_cast<const PolygonSizeObjectType *>(this->itk::ProcessObject::GetOutput(2));
}


template<class TInputImage, class TMaskImage>
typename PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>::PolygonSizeObjectType*
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetPolygonSizeOutput()
{
    if (this->GetNumberOfOutputs()<3)
    {
    return 0;
    }
  return static_cast<PolygonSizeObjectType *>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage, class TMaskImage>
itk::DataObject::Pointer
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::MakeOutput(DataObjectPointerArraySizeType idx)
{
  // TODO
  return NULL;
}


template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GenerateOutputInformation()
{
  const MaskImageType *mask = this->GetMask();
  if (mask)
    {
    const InputImageType *input = this->GetInput();
    if (mask->GetLargestPossibleRegion() !=
        input->GetLargestPossibleRegion() )
      {
      itkGenericExceptionMacro("Mask and input image have a different size!");
      }
    }
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GenerateInputRequestedRegion()
{
  InputImageType *input = const_cast<InputImageType*>(this->GetInput());
  MaskImageType *mask = const_cast<MaskImageType*>(this->GetMask());

  RegionType requested = this->GetOutput()->GetRequestedRegion();
  RegionType emptyRegion = input->GetLargestPossibleRegion();
  emptyRegion.SetSize(0,0);
  emptyRegion.SetSize(1,0);

  input->SetRequestedRegion(emptyRegion);

  if (mask)
    {
    mask->SetRequestedRegion(requested);
    }
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::ThreadedGenerateData(const RegionType& outputRegionForThread,
                       itk::ThreadIdType threadId)
{
  // TODO
}

// Members from otb::OGRDataToClassStatisticsFilter

template<class TInputImage, class TMaskImage>
void
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::SetInput(const TInputImage* image)
{
  // TODO
}

template<class TInputImage, class TMaskImage>
const TInputImage*
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetInput()
{
  // TODO
  return NULL;
}

template<class TInputImage, class TMaskImage>
void
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::SetOGRData(const otb::ogr::DataSource* data)
{
  // TODO
}

template<class TInputImage, class TMaskImage>
const otb::ogr::DataSource*
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetOGRData()
{
  // TODO
  return NULL;
}

template<class TInputImage, class TMaskImage>
void
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::SetMask(const TMaskImage* mask)
{
  // TODO
}

template<class TInputImage, class TMaskImage>
const TMaskImage*
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetMask()
{
  // TODO
  return NULL;
}

template<class TInputImage, class TMaskImage>
void
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::SetClassKey(std::string &key)
{
  // TODO
}

template<class TInputImage, class TMaskImage>
std::string
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetClassKey()
{
  // TODO
  return NULL;
}


} // end of namespace otb

#endif
