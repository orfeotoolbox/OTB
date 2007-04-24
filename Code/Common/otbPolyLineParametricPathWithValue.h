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
#ifndef _otbPolyLineParametricPathWithValue_h
#define _otbPolyLineParametricPathWithValue_h

#include "itkPolyLineParametricPath.h"
#include "itkMetaDataObject.h"
#include "otbMacro.h"

namespace otb
{
/** \class PolyLineParametricPathWithValue
 *  \brief 
 *
 * \ingroup 
 * \ingroup
 */
template < class TValue,unsigned int VDimension=2>
class ITK_EXPORT PolyLineParametricPathWithValue
  : public itk::PolyLineParametricPath<VDimension>
{
 public:
  /** Standard typedefs */
  typedef PolyLineParametricPathWithValue          Self;
  typedef itk::PolyLineParametricPath<VDimension>  Superclass;
  typedef itk::SmartPointer<Self>                  Pointer;
  typedef itk::SmartPointer<const Self>            ConstPointer;
  
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(PolyLineParametricPath, MySuperclass);
  
  /** Template parameters typedefs */
 typedef TValue ValueType;
 
 itkGetMacro(Key,std::string);

 void SetValue(ValueType value)
 {
   itk::MetaDataDictionary & dict = this->GetMetaDataDictionary();
   itk::EncapsulateMetaData<ValueType>(dict,m_Key,value);
 }

 ValueType GetValue(void)
 {
   itk::MetaDataDictionary & dict = this->GetMetaDataDictionary();
   if(dict.HasKey(m_Key))
   {
     ValueType resp;
     itk::ExposeMetaData<ValueType>(dict,m_Key,resp);
   }
   else
   {
     itkGenericExceptionMacro(<<"Key "<<m_Key<<" not found in metadata dictionary !");
   }
 }

protected:
  /** Constructor */
  PolyLineParametricPathWithValue()
 {
   itk::MetaDataDictionary & dict = this->GetMetaDataDictionary();
   m_Key = "Value";
   itk::EncapsulateMetaData<ValueType>(dict,m_Key,0);
 };
  /** Destructor */
  virtual ~PolyLineParametricPathWithValue() 
 { }
 /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
 {
   Superclass::PrintSelf(os,indent);
 }
 private:
 PolyLineParametricPathWithValue(const Self&); //purposely not implemented
 void operator=(const Self&); //purposely not implemented
 std::string m_Key;
};
}// End namespace otb
#endif
