/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   24 janvier 2006
  Role      :    
  $Id$ 

=========================================================================*/
#ifndef __otbFlst_h
#define __otbFlst_h
 
#include "itkImage.h"
#include "otbImageToTreeFilter.h"
#include "otbTreeNeighborhood.h"
//#include "otbTreeSource.h"
#include "itkPolyLineParametricPath.h"

namespace otb
{

/** \class Flst
 * \brief Algorithme de Fast Level Sets Transform of an image
 *
 */
 
 
template < class TInputImage, class TOutputTree /*= itk::PolyLineParametricPath<2>*/ >
class ITK_EXPORT Flst :  public ImageToTreeFilter< TInputImage, TOutputTree >
{
public:
  /** Standard class typedefs. */
  typedef Flst                                              Self;
  typedef ImageToTreeFilter<TInputImage,TOutputTree>        Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Methode pour la gestion "object factory". */
  itkNewMacro(Self);

  /** Retourne le nom de la classe. */
  itkTypeMacro(Flst, ImageToTreeFilter);
  
  itkStaticConstMacro(InputImageDimension,
  		      unsigned int,
                      TInputImage::ImageDimension);
   
  /** "typedef" pour simplifier la définition et la déclaration de variables. */
  
  typedef typename Superclass::InputImageType        InputImageType;
  typedef typename Superclass::InputImagePointer     InputImagePointer;
  typedef typename Superclass::InputImageRegionType  InputImageRegionType; 
  typedef typename Superclass::InputImagePixelType   InputImagePixelType; 
  typedef typename Superclass::InputImageSizeType    InputImageSizeType;                 
  typedef typename Superclass::InputImageValueType   InputImageValueType;
  typedef typename Superclass::InputImageIndexType   InputImageIndexType;

  typedef typename Superclass::OutputTreeType             OutputTreeType;
  typedef typename Superclass::OutputTreePointerType      OutputTreePointerType;
  typedef typename Superclass::OutputTreeConstPointerType OutputTreeConstPointerType;
  
  typedef itk::Image<int,2>                     IntImageType;
  typedef typename IntImageType::Pointer        IntImagePointerType;
  typedef typename IntImageType::ConstPointer   IntImageConstPointerType;
  
  typedef itk::Image<float,2>                   RealImageType;
  typedef typename RealImageType::Pointer       RealImagePointerType;
  typedef typename RealImageType::ConstPointer  RealImageConstPointerType;
  
  typedef typename Superclass::PathType  	        PathType;
  typedef typename Superclass::PathPointerType  	PathPointerType;

  typedef typename Superclass::ShapeType     	        ShapeType;
  typedef typename Superclass::ShapePointerType     	ShapePointerType;
  typedef typename Superclass::ShapeConstPointerType   	ShapeConstPointerType;
  
  typedef typename Superclass::ShapeTreeType 	          ShapeTreeType;  
  typedef typename Superclass::ShapeTreePointerType       ShapePointerType;
  typedef typename Superclass::ShapeTreeConstPointerType  ShapeConstPointerType;

  typedef typename Superclass::PointPlaneType              PointPlaneType;

  typedef typename otb::TreeNeighborhood::NeighborhoodType   NeighborhoodType;

  typedef struct
     {
       ShapeType shape;
       float     level;
     } ConnectionType;
  
protected:
  Flst();
  virtual ~Flst() {}
  
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

 /** Définition des paramètres d'optimisation. 
 * Ils sont dépendant de la taille des images mais ces valeurs semblent 
 * convenir à  la majoritée des images. */
  static const int INIT_MAX_AREA; 
  static const int STEP_MAX_AREA;
  
  /** configuration locale des pixels */
  static const int EAST; 
  static const int NORTH_EAST;
  static const int NORTH;
  static const int NORTH_WEST;
  static const int WEST;
  static const int SOUTH_WEST;
  static const int SOUTH;
  static const int SOUTH_EAST;    
  
  static const char tabPatterns[2][256];
  
  itkSetMacro(MinArea, int);
  itkGetConstReferenceMacro(MinArea, int);
//  inline int NEIGHBOR_NOT_STORED(int x,int y)
//      {m_VisitedPixels[y][x]<m_Exploration;}
      
#if 0
  char is_local_min(RealImagePointerType ou, int x, int y, char b8Connected);
  char is_local_max(RealImagePointerType ou, int x, int y, char b8Connected);

  void UpdateSmallestShapes(PointPlaneType tabPoints, int NbPoints);  
  void Connect(PointPlaneType tabPoints,int iNbPoints,
               ConnectionType *tabConnections,ShapeType pSmallestShape);
  void NewConnection(PointPlaneType tabPoints,float level,
                     ConnectionType *tabConnections); 
  
  unsigned char configuration(IntImagePointerType VisitedPixels, int x, int y);

  ShapeType NewShape(int iCurrentArea, float currentGrayLevel, 
                     char bOfInferiorType, ShapeType pChild);

  void Store4Neighbors(RealImagePointerType ou,int x,int y,NeighborhoodType pNeighborhood);
  void Store8Neighbors(RealImagePointerType ou,int x,int y,NeighborhoodType pNeighborhood);
  void AddIsoLevel(PointPlaneType   tabPointsInShape, 
                   int              *pCurrentArea,
		   float            currentGrayLevel, 
		   NeighborhoodType *pNeighborhood, 
		   char             *p8Connected, 
		   char             *pIgnoreHoles);  
  void FindTerminalBranch(RealImagePointerType ou,int x,int y,
                          char b8Connected,
                          NeighborhoodType *pNeighborhood, 
			  ConnectionType *tabConnections);
  void Scan(RealImagePointerType ou,NeighborhoodType *pNeighborhood,
            ConnectionType *tabConnections);
  void CalculateArea(int Width, int Height);
#endif 
  void GenerateData(void);
 
private:
  Flst(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  int                   m_Width;
  int                   m_Height;
  int                   m_MinArea;
  int                   m_MaxArea;
  int                   m_AreaImage;
  int                   m_HalfAreaImage;
  int                   m_PerimeterImage;
  int                   m_AtBorder;
  int                   m_Exploration;
  IntImagePointerType   m_VisitedPixels;
  RealImagePointerType  m_PixelOutput;
  NeighborhoodType      m_Neighborhood;
  ConnectionType*       m_Connections;

  PointPlaneType m_PointsInShape;
        
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFlst.txx"
#endif

  
#endif
