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
#ifndef otbWrapperInputImageListParameter_h
#define otbWrapperInputImageListParameter_h

#include "otbWrapperParameter.h"
#include "otbWrapperInputImageParameter.h"

namespace otb
{
namespace Wrapper
{
/** \class InputImageListParameter
 *  \brief This class represents a list of InputImage parameter
 *
 * \ingroup OTBApplicationEngine
 */

class ITK_ABI_EXPORT InputImageListParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef InputImageListParameter           Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef std::vector<InputImageParameter::Pointer> InputImageParameterVectorType;

  typedef itk::ImageBase<2> ImageBaseType;
  
  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(InputImageListParameter, Parameter);

  /** Set image form a list of filename */
  bool SetListFromFileName(const std::vector<std::string> & filenames);

  /** Add null element to lists. */
  void AddNullElement();

  /** Add an image from a filename */
  bool AddFromFileName(const std::string & filename);

  /** Set one specific stored image filename. */
  bool SetNthFileName( const unsigned int id, const std::string & filename );

  /** Get the stored image filename list */
  std::vector<std::string> GetFileNameList() const;

 /** Get one specific stored image filename. */
  std::string GetNthFileName( unsigned int i ) const;

  /** Get one list of the stored image. */
  FloatVectorImageListType* GetImageList() const;

  /** Get one specific stored image. */
  FloatVectorImageType* GetNthImage(unsigned int i) const;

  /** Set the list of image. */
  void SetImageList(FloatVectorImageListType* imList);

  /** Add an image to the list. */
  void AddImage(ImageBaseType* image);

  bool HasValue() const ITK_OVERRIDE;


  /** Erase one element of the list. */
  void Erase( unsigned int id );

 /** Clear all the list. */
  void ClearValue() ITK_OVERRIDE;
  
protected:
  /** Constructor */
  InputImageListParameter();

  /** Destructor */
  ~InputImageListParameter() ITK_OVERRIDE;


private:
  InputImageListParameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

  InputImageParameterVectorType m_InputImageParameterVector;
  FloatVectorImageListType::Pointer m_ImageList;
  
  
}; // End class InputImage Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
