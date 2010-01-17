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
#ifndef __otbCrossGlComponent_h
#define __otbCrossGlComponent_h

#include "otbGlComponent.h"
#include "itkPreOrderTreeIterator.h"

#  ifdef __APPLE__
#    include <OpenGL/glu.h>
#  else
#    include <GL/glu.h>
#  endif

// There are function prototype conflits under cygwin between standard w32 API
// and standard C ones
#ifndef CALLBACK
#if defined(_WINDOWS) || defined(__CYGWIN__)
#define CALLBACK __stdcall
#else
#define CALLBACK
#endif
#endif


namespace otb
{
/** \class CrossGlComponent
*   \brief This Gl Component to render a Cross.
*   No checking is done upon the adequation between the Cross
*   projection and the underlying image projection.
*
*   Origin and Spacing allows to fit to the image axis.
*  \ingroup Visualization
 */

class CrossGlComponent : public GlComponent
{
public:
  /** Standard class typedefs */
  typedef CrossGlComponent              Self;
  typedef GlComponent                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef Superclass::RegionType        RegionType;

  // affine transform
  typedef Superclass::AffineTransformType       AffineTransformType;
  typedef AffineTransformType::InputPointType   PointType;
  typedef AffineTransformType::InputVectorType  VectorType;
  typedef Superclass::ColorType                 ColorType;

  typedef itk::Index<> IndexType;
  typedef std::vector<IndexType> IndexListType;
  typedef std::vector<ColorType> ColorListType;

  /** Runtime information */
  itkTypeMacro(CrossGlComponent,GlComponent);

  /** New macro */
  itkNewMacro(Self);

  /// Render the vector data
  virtual void  Render(const RegionType& extent,const AffineTransformType * space2ScreenTransform);

  /** Set/Get the grid spacing */
  itkSetMacro(Spacing,VectorType);
  itkGetConstReferenceMacro(Spacing,VectorType);

  /** Set/Get the grid origin */
  itkSetMacro(Origin,PointType);
  itkGetConstReferenceMacro(Origin,PointType);

  /** Set/Get the index to render */
  void SetIndexList(IndexListType idList) { m_IndexList = idList; };
  IndexListType GetIndexList() { return m_IndexList; };
  void AddIndex(IndexType id) { m_IndexList.push_back(id); m_ColorList.push_back(m_RedColor); };
  void RemoveIndex(unsigned int id)
  {
    if( id >= m_IndexList.size() )
      itkExceptionMacro(<<"Index out of size ");

    m_IndexList.erase(m_IndexList.begin()+id);
  };
  
  /** Set/Get the color */
  void SetColorList(ColorListType colorList) { m_ColorList = colorList; };
  ColorListType GetColorList() { return m_ColorList; };
  void ChangeColor(ColorType color, unsigned int id)
  {
    if( id >= m_ColorList.size() )
      itkExceptionMacro(<<"Index out of size ");

    m_ColorList[id] = color;
  };
  void RemoveColor(unsigned int id)
  {
    if( id >= m_ColorList.size() )
      itkExceptionMacro(<<"Index out of size ");

    m_ColorList.erase(m_ColorList.begin()+id);
  };

  /** Clear all*/
  void Clear() { m_IndexList.clear(); m_ColorList.clear(); };
  void ClearIndex(unsigned int id)
  {
    this->RemoveIndex(id);
    this->RemoveColor(id);
  };

  /** Set/Get the line width */
  itkSetMacro(LineWidth,double);
  itkGetMacro(LineWidth,double);

  /** Set/Get the cross width */
  itkSetMacro(CrossWidth,double);
  itkGetMacro(CrossWidth,double);

protected:
  /** Constructor */
  CrossGlComponent();
  /** Destructor */
  virtual ~CrossGlComponent();
  /** Printself method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
  }

  // Recursive rendering method
  virtual void Render(unsigned int id, const RegionType & extent, const AffineTransformType * space2ScreenTransform);


private:
  CrossGlComponent(const Self&); // purposely not implemented
  void operator=(const Self&);        // purposely not implemented

  /// Index point to render
  IndexListType m_IndexList;

  /// Spacing of the image grid
  VectorType m_Spacing;

  /// Origin of the image
  PointType  m_Origin;

  /// The GluTesselator object to render complex polygons
  GLUtesselator * m_GluTesselator;

  /// Color of the vector layer
  ColorListType m_ColorList;

  /** The line width */
  double m_LineWidth;

  /** The cross width for points */
  double m_CrossWidth;
  
  /** Default color : red*/
  ColorType m_RedColor;

}; // end class
} // end namespace otb

#endif



