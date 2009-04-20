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
#ifndef __otbVectorDataMetaData_h
#define __otbVectorDataMetaData_h

#include "itkLightObject.h"
#include "itkObjectFactory.h"
#include <vector>
#include "ogr_feature.h"


namespace otb
{
/** \class VectorDataMetaData
   * \brief this class represents the metadata of vector data.
   *
 */

class VectorDataMetaData : public itk::LightObject
{
  public:
    /** Smart pointer typedef support. */
    typedef VectorDataMetaData        Self;
    typedef itk::LightObject               Superclass;
    typedef itk::SmartPointer<Self>        Pointer;
    typedef itk::SmartPointer<const Self>  ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(VectorDataMetaData, LightObject);


    typedef std::vector<OGRFieldDefn> FeatureListType;


    virtual void Print(std::ostream& os) const;

  protected:

    VectorDataMetaData();
    virtual ~VectorDataMetaData();

  private:
    VectorDataMetaData(const Self &);//purposely not implemented
    void operator=(const Self&); //purposely not implemented

    FeatureListType m_FeatureList;


};

}

#endif

