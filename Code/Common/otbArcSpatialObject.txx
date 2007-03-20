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
#ifndef __otbArcSpatialObject_txx
#define __otbArcSpatialObject_txx

#include "otbArcSpatialObject.h"

#define M_PI 4*vcl_atan(1.)

namespace otb
{
/** 
 * Constructor.
 */
template< unsigned int VDimension >
ArcSpatialObject< VDimension >
::ArcSpatialObject()
{
  this->SetTypeName("ArcSpatialObject");
  m_Radius=1.0;
  this->SetDimension(VDimension);
  m_Angle1=0.0;
  m_Angle2=0.0;
}
/** 
 * Test whether a point is inside or outside the object
 * For computational speed purposes, it is faster if the method does not
 * check the name of the class and the current depth 
 */
template< unsigned int VDimension >
bool
ArcSpatialObject< VDimension >
::IsInside( const PointType & point) const
{
  this->ComputeLocalBoundingBox();
  if(!this->GetIndexToWorldTransform()->GetInverse(const_cast<TransformType *>(this->GetInternalInverseTransform())))
    {
    return false;
    }

  PointType transformedPoint = this->GetInternalInverseTransform()->TransformPoint(point);
  double r = 0;
  for(unsigned int i=0;i<VDimension;i++)
    {
    if(m_Radius!=0.0)
      {
      r +=  ((transformedPoint[i]*transformedPoint[i])/(m_Radius*m_Radius));
      }
    else if(transformedPoint[i]>0.0)  // Degenerate ellipse
      {
      r = 2; // Keeps function from returning true here
     break;
      }
    }
  if(r<=1)
  {
        double angle;
        if(transformedPoint[0]!=0)
        {
                angle=vcl_atan(transformedPoint[1]/transformedPoint[0])*180/M_PI;
        }
        else if(transformedPoint[1]>0)
                {
                angle=90;
                }
        else if(transformedPoint[1]<0)
                {
                angle=-90;
                }
        printf("\n angle=%6.3f ",angle);
        if (angle>=m_Angle1 && angle<=m_Angle2)
                {
                printf("----> OK");
                return true;
                }
        else return false;

        }
  return false;
}
/** 
 * Test if the given point is inside the arc 
 */
template< unsigned int VDimension >
bool
ArcSpatialObject< VDimension >
::IsInside( const PointType & point, unsigned int depth, char * name ) const
{
  itkDebugMacro( "Checking the point [" << point << "] is inside the Arc" );

  if(name == NULL)
    {
    if(IsInside(point))
      {
      return true;
      }
    }
  else if(strstr(typeid(Self).name(), name))
    {
    if(IsInside(point))
      {
      return true;
      }
    }
  return Superclass::IsInside(point, depth, name);
}
/** 
 * Compute the bounds of the arc 
 */
template< unsigned int VDimension >
bool
ArcSpatialObject< VDimension >
::ComputeLocalBoundingBox() const
{
  itkDebugMacro( "Computing arc bounding box" );
  if( this->GetBoundingBoxChildrenName().empty()
      || strstr(typeid(Self).name(), this->GetBoundingBoxChildrenName().c_str()) )
    {
    // we need to set the minimum and maximum of the bounding box
    // the center is always inside the bounding box.
    PointType center;
    center.Fill(0);
    center = this->GetIndexToWorldTransform()->TransformPoint(center);
    const_cast<BoundingBoxType *>(this->GetBounds())->SetMinimum(center);
    const_cast<BoundingBoxType *>(this->GetBounds())->SetMaximum(center);

    // First we compute the bounding box in the index space
    typename BoundingBoxType::Pointer bb = BoundingBoxType::New();

    PointType pntA;
    PointType pntB;
        PointType pntC;
        PointType pntD;
        
        double alpha,longueur,largeur;
        alpha=(m_Angle2-m_Angle1) ; //différence entre les deux angles.

        if (alpha<=180)
                {
                pntA[0]=m_Radius*vcl_cos(m_Angle1*M_PI/180);
                pntA[1]=m_Radius*vcl_sin(m_Angle1*M_PI/180);
                pntB[0]=m_Radius*vcl_cos(m_Angle2*M_PI/180);
                pntB[1]=m_Radius*vcl_sin(m_Angle2*M_PI/180);

                longueur=vcl_sqrt((pntB[0]-pntA[0])*(pntB[0]-pntA[0])+(pntB[1]-pntA[1])*(pntB[1]-pntA[1]));
                largeur=m_Radius*(1-vcl_cos((alpha/2)*M_PI/180));
                pntC[0]=pntB[0]+largeur*(pntB[1]-pntA[1])/longueur;
                pntC[1]=pntB[1]-largeur*(pntB[0]-pntA[0])/longueur;
                pntD[0]=pntA[0]+largeur*(pntB[1]-pntA[1])/longueur;
                pntD[1]=pntA[1]-largeur*(pntB[0]-pntA[0])/longueur;
        }
        else
                {
                pntA[0]=m_Radius*vcl_cos(m_Angle1*M_PI/180);
                pntA[1]=m_Radius*vcl_sin(m_Angle1*M_PI/180);
                pntB[0]=m_Radius*vcl_cos(m_Angle2*M_PI/180);
                pntB[1]=m_Radius*vcl_sin(m_Angle2*M_PI/180);
                longueur=vcl_sqrt((pntB[0]-pntA[0])*(pntB[0]-pntA[0])+(pntB[1]-pntA[1])*(pntB[1]-pntA[1]));
                largeur=m_Radius*(1-vcl_cos((alpha/2)*M_PI/180));
                pntA[0]=(m_Radius/2)*(vcl_cos(m_Angle1*M_PI/180)+vcl_cos(m_Angle2*M_PI/180))-(m_Radius*m_Radius/longueur)*(vcl_cos(m_Angle1*M_PI/180)-vcl_cos(m_Angle2*M_PI/180));
                pntA[1]=(m_Radius/2)*(vcl_sin(m_Angle1*M_PI/180)+vcl_sin(m_Angle2*M_PI/180))-(m_Radius*m_Radius/longueur)*(vcl_sin(m_Angle1*M_PI/180)-vcl_sin(m_Angle2*M_PI/180));
                pntB[0]=(m_Radius/2)*(vcl_cos(m_Angle1*M_PI/180)+vcl_cos(m_Angle2*M_PI/180))+(m_Radius*m_Radius/longueur)*(vcl_cos(m_Angle1*M_PI/180)-vcl_cos(m_Angle2*M_PI/180));
                pntB[1]=(m_Radius/2)*(vcl_sin(m_Angle1*M_PI/180)+vcl_sin(m_Angle2*M_PI/180))+(m_Radius*m_Radius/longueur)*(vcl_sin(m_Angle1*M_PI/180)-vcl_sin(m_Angle2*M_PI/180));
                pntC[0]=pntB[0]-(largeur/(2*m_Radius))*(pntB[1]-pntA[1]);
                pntC[1]=pntB[1]+(largeur/(2*m_Radius))*(pntB[0]-pntA[0]);
                pntD[0]=pntA[0]-(largeur/(2*m_Radius))*(pntB[1]-pntA[1]);
                pntD[1]=pntA[1]+(largeur/(2*m_Radius))*(pntB[0]-pntA[0]);
                }
        bb->ConsiderPoint(pntA);
    bb->ConsiderPoint(pntB);
    bb->ConsiderPoint(pntC);
    bb->ConsiderPoint(pntD);
        bb->ComputeBoundingBox();
        PointType pnt = this->GetIndexToWorldTransform()->TransformPoint(pntA);
        const_cast<BoundingBoxType *>(this->GetBounds())->ConsiderPoint(pnt);
        pnt = this->GetIndexToWorldTransform()->TransformPoint(pntB);
        const_cast<BoundingBoxType *>(this->GetBounds())->ConsiderPoint(pnt);
        pnt = this->GetIndexToWorldTransform()->TransformPoint(pntC);
        const_cast<BoundingBoxType *>(this->GetBounds())->ConsiderPoint(pnt);
        pnt = this->GetIndexToWorldTransform()->TransformPoint(pntD);
        const_cast<BoundingBoxType *>(this->GetBounds())->ConsiderPoint(pnt);
    }
  return true;
}
/**
 *  Returns if the arc is evaluable at one point 
 */
template< unsigned int VDimension >
bool
ArcSpatialObject< VDimension >
::IsEvaluableAt( const PointType & point, unsigned int depth, char * name ) const
{
  itkDebugMacro( "Checking if the arc is evaluable at " << point );
  return IsInside(point, depth, name);
}
/** 
 * Returns the value at one point 
 */
template< unsigned int VDimension >
bool
ArcSpatialObject< VDimension >
::ValueAt( const PointType & point, double & value, unsigned int depth,
           char * name ) const
{
  itkDebugMacro( "Getting the value of the arc at " << point );
  if( IsInside(point, 0, name) )
    {
    value = this->GetDefaultInsideValue();
    return true;
    }
  else
    {
    if( Superclass::IsEvaluableAt(point, depth, name) )
      {
      Superclass::ValueAt(point, value, depth, name);
      return true;
      }
    else
      {
      value = this->GetDefaultOutsideValue();
      return false;
      }
    }
  return false;
}
/** 
 * Print Self function 
 */
template< unsigned int VDimension >
void
ArcSpatialObject< VDimension >
::PrintSelf( std::ostream& os, itk::Indent indent ) const
{
  Superclass::PrintSelf(os, indent);
  os << "Radius: " << m_Radius << std::endl;
}
/** 
 * Copy the information from another spatial object 
 */
template< unsigned int VDimension >
void  ArcSpatialObject< VDimension >
::CopyInformation(const itk::DataObject *data)
{
  // check if we are the same type
  const Self* source = dynamic_cast<const Self*>(data);
  if(!source)
    {
    std::cout << "CopyInformation: objects are not of the same type" << std::endl;
    return;
    }
  // copy the properties
  Superclass::CopyInformation(data);

  // copy the internal info
  this->SetRadius(source->GetRadius());
}
} // end namespace otb

#endif
