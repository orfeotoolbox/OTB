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
#ifndef __otbWrapperOutputImageListParameter_h
#define __otbWrapperOutputImageListParameter_h

#include "otbVectorImage.h"
#include "otbWrapperParameter.h"
#include "otbStreamingImageFileWriter.h"

namespace otb
{
namespace Wrapper
{
/** \class OutputImageListParameter
 *  \brief This class represents a list of OutputImage parameter
 */

class ITK_EXPORT OutputImageListParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef OutputImageListParameter      Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef otb::ObjectList<VectorImageType>               VectorImageListType;
  typedef otb::StreamingImageFileWriter<VectorImageType> ImageFileWriterType;
  typedef otb::ObjectList<ImageFileWriterType>           ImageFileWriterListType;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(OutputImageListParameter, Parameter);

  /** Set the value */
  itkSetObjectMacro(ImageList, VectorImageListType);

  /** Get the value */
  itkGetObjectMacro(ImageList, VectorImageListType);

  void AddImage( VectorImageType * image )
  {
    m_ImageList->PushBack( image );
    this->Modified();
  }

  VectorImageType * GetNthImage( unsigned int i )
  {
    return m_ImageList->GetNthElement(i);
  }

  void SetFileNameList( const std::vector<std::string> & fileList )
  {
    m_FileNameList = fileList;
    this->Modified();
  }

  void AddFileName( std::string fileName )
  {
    m_FileNameList.push_back( fileName );
    this->Modified();
  }

  std::vector<std::string> GetFileNameList()
  {
    return m_FileNameList;
  }


  void Write()
  {
    if(m_ImageList->Size() != m_FileNameList.size())
      {
        itkExceptionMacro(<< "Image list and filename list size dismatch...");
      }
    
    for (unsigned int i=0; i<m_ImageList->Size(); i++)
      {
        if (m_ImageList->GetNthElement( i ).IsNotNull())
          {
            ImageFileWriterType::Pointer writer = ImageFileWriterType::New();
            writer->SetInput(m_ImageList->GetNthElement( i ));
            writer->SetFileName( m_FileNameList[i]);
            writer->Update();
          }
      }
  }

protected:
  /** Constructor */
  OutputImageListParameter()
  {
    this->SetName("Output Image");
    this->SetKey("out");
    m_ImageList = VectorImageListType::New();
    m_FileNameList.clear();
  }

  /** Destructor */
  virtual ~OutputImageListParameter()
  {}

  VectorImageListType::Pointer m_ImageList;
  std::vector<std::string>     m_FileNameList;

private:
  OutputImageListParameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

}; // End class OutputImage Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
