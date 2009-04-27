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
#ifndef __otbVectorDataKeywordlist_h
#define __otbVectorDataKeywordlist_h

#include "itkLightObject.h"
#include "itkObjectFactory.h"
#include <vector>
#include "ogr_feature.h"


namespace otb
{
/** \class VectorDataKeywordlist
 * \brief this class handle the metadata of vector data.
 *
 * This class is used internaly to handle the information associated with
 * a vector object. This information is retrieved from the input file (a
 * shapefile for example) and propagated along the pipeline with the object.
 *
 * This is the equivalent of the otbOssimKeywordlist class but for OGR information.
 *
 * \todo add the accessor to enable modifying/updating the data.
 *
 */

class VectorDataKeywordlist
{
  public:
    /** Smart pointer typedef support. */
    typedef VectorDataKeywordlist        Self;
//     typedef itk::LightObject               Superclass;
//     typedef itk::SmartPointer<Self>        Pointer;
//     typedef itk::SmartPointer<const Self>  ConstPointer;
//
//     /** Method for creation through the object factory. */
//     itkNewMacro(Self);
//
//     /** Run-time type information (and related methods). */
//     itkTypeMacro(VectorDataKeywordlist, LightObject);

    typedef std::pair<OGRFieldDefn*,OGRField> FieldType;
    typedef std::vector< FieldType > FieldListType;


    void AddField(OGRFieldDefn* fieldDefn, OGRField* field);

    virtual void Print(std::ostream& os, itk::Indent indent=0) const;

    VectorDataKeywordlist();
    virtual ~VectorDataKeywordlist();

    VectorDataKeywordlist(const Self&);
    void operator=(const Self&);

  protected:

    virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  private:

    std::string PrintField(FieldType field) const;
    FieldType CopyOgrField(FieldType field);
    FieldListType m_FieldList;


};

extern std::ostream & operator<<(std::ostream &os, const VectorDataKeywordlist &kwl);

}

#endif

