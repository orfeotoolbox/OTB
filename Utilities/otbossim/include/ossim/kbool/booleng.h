/*! \file kbool/include/kbool/booleng.h
    \author Probably Klaas Holwerda

    Copyright: 2001-2004 (C) Probably Klaas Holwerda

    Licence: wxWidgets Licence

    RCS-ID: $Id: booleng.h 13215 2008-07-23 18:51:54Z dburken $
*/

#ifndef BOOLENG_H
#define BOOLENG_H

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface
#endif


#include <stdio.h>
#include <limits.h>
#include <ossim/kbool/bool_globals.h>
#include <ossim/kbool/_dl_itr.h>
class KBoolLink;
class GraphList;
class Graph;
class KBoolLink;
class Node;
template<class Type> class TDLI;

//! boolean engine to perform operation on two sets of polygons.
/*
	First the engine needs to be filled with polygons.
	The first operand in the operation is called group A polygons, the second group B.
	The boolean operation ( BOOL_OR, BOOL_AND, BOOL_EXOR, BOOL_A_SUB_B, BOOL_B_SUB_A )
	are based on the two sets of polygons in group A and B.
	The other operation on group A only.

    At the end of the operation the resulting polygons can be extracted.
*/
class A2DKBOOLDLLEXP Bool_Engine {

  public:

   //! constructor    
   Bool_Engine();
   Bool_Engine(const Bool_Engine& rhs);
   //! destructor   
   virtual ~Bool_Engine();

   const char* GetVersion() { return KBOOL_VERSION; }

	//! reports progress of algorithm.
   virtual void SetState( const char* = 0 );

	//! called at an internal error.
	virtual void error(const char *text, const char *title);

	//! called at an internal generated possible error.
   virtual void info(const char *text, const char *title);

   bool Do_Operation(BOOL_OP operation);


   //! distance within which points and lines will be snapped towards lines and other points
   /*
         The algorithm takes into account gaps and inaccuracies caused by rounding to integer coordinates
         in the original data.
         Imagine two rectangles one with a side ( 0,0 ) ( 2.0, 17.0 ) 
         and the other has a side ( 0,0 ) ( 1.0, 8.5 )
         If for some reason those coordinates where round to ( 0,0 ) ( 2, 17 ) ( 0,0 ) ( 1, 9 ),
         there will be clearly a gap or overlap that was not intended.
         Even without rounding this effect takes place since there is always a minimum significant bit
         also when using doubles.

         If the user used as minimum accuracy 0.001, you need to choose Marge > 0.001
         The boolean engine scales up the input data with GetDGrid() * GetGrid() and rounds the result to
         integer, So (assuming GRID = 100 DGRID = 1000)  a vertex of 123.001 in the user data will
         become 12300100 internal.
         At the end of the algorithm the internal vertexes are scaled down again with GetDGrid() * GetGrid(),
         so 12300103 becomes 123.00103 eventually.
         So indeed the minimum accuracy might increase, you are free to round again if needed.
   */
   void SetMarge(double marge);
   double GetMarge();

   //! input points are scaled up with GetDGrid() * GetGrid()
   /*
		Grid makes sure that the integer data used within the algorithm has room for extra intersections
		smaller than the smallest number within the input data.
		The input data scaled up with DGrid is related to the accuracy the user has in his input data.
        Another scaling with Grid is applied on top of it to create space in the integer number for 
		even smaller numbers. 
   */
   void SetGrid(B_INT grid);  
	
	//! See SetGrid
   B_INT GetGrid();

   //! input points are scaled up with GetDGrid() * GetGrid()
   /*
      The input data scaled up with DGrid is related to the accuracy the user has in his input data.
      User data with a minimum accuracy of 0.001, means set the DGrid to 1000.
      The input data may contain data with a minimum accuracy much smaller, but by setting the DGrid
      everything smaller than 1/DGrid is rounded.

      DGRID is only meant to make fractional parts of input data which can be
      doubles, part of the integers used in vertexes within the boolean algorithm.
      And therefore DGRID bigger than 1 is not usefull, you would only loose accuracy.
      Within the algorithm all input data is multiplied with DGRID, and the result
      is rounded to an integer. 
   */
   void SetDGrid(double dgrid);

	//! See SetDGrid
   double GetDGrid();

   //! When doing a correction operation ( also known as process offset )
	//! this defines the detail in the rounded corners.
	/*
		Depending on the round factor the corners of the polygon may be rounding within the correction
		algorithm. The detail within this rounded corner is set here.
		It defines the deviation the generated segments in arc like polygon may have towards the ideal
		rounded corner using a perfect arc.
    */
   void SetCorrectionAber(double aber);

	//! see SetCorrectionAber
   double GetCorrectionAber();

   //! When doing a correction operation ( also known as process offset )
	//! this defines the amount of correction.
   /*
      The correction algorithm can apply positive and negative offset to polygons.
      It takes into account closed in areas within a polygon, caused by overlapping/selfintersecting
      polygons. So holes form that way are corrected proberly, but the overlapping parts itself
      are left alone. An often used trick to present polygons with holes by linking to the outside
      boundary, is therefore also handled properly.
      The algoritm first does a boolean OR operation on the polygon, and seperates holes and
      outside contours.
      After this it creates a ring shapes on the above holes and outside contours.
      This ring shape is added or subtracted from the holes and outside contours.
      The result is the corrected polygon.
      If the correction factor is > 0, the outside contours will become larger, while the hole contours
      will become smaller.
   */
   void SetCorrectionFactor(double aber);

	//! see SetCorrectionFactor
   double GetCorrectionFactor();

	//! used within the smooth algorithm to define how much the smoothed curve may deviate
	//! from the original.
   void SetSmoothAber(double aber);

	//! see SetSmoothAber
   double GetSmoothAber();

	//! segments of this size will be left alone in the smooth algorithm.
   void SetMaxlinemerge(double maxline);

	//! see SetMaxlinemerge
   double GetMaxlinemerge();

	//! Polygon may be filled in different ways (alternate and winding rule).
	//! This here defines which method will be assumed within the algorithm.
   void SetWindingRule(bool rule);

	//! see SetWindingRule
   bool GetWindingRule();

	//! the smallest accuracy used within the algorithm for comparing two real numbers.
   double GetAccur();

    //! Used with in correction/offset algorithm.
	/*
		When the polygon contains sharp angles ( < 90 ), after a positive correction the
		extended parrallel constructed offset lines may leed to extreme offsets on the angles.
		The length of the crossing generated by the parrallel constructed offset lines
		towards the original point in the polygon is compared to the offset which needs to be applied.
		The Roundfactor then decides if this corner will be rounded.
		A Roundfactor of 1 means that the resulting offset will not be bigger then the correction factor
		set in the algorithm. Meaning even straight 90 degrees corners will be rounded.
		A Roundfactor of > sqrt(2) is where 90 corners will be left alone, and smaller corners will be rounded.
	*/
	void SetRoundfactor(double roundfac);

	//! see SetRoundfactor
   double GetRoundfactor();

// the following are only be used within the algorithm,
// since they are scaled with m_DGRID

   //! only used internal.
   void SetInternalMarge( B_INT marge );
   //! only used internal.
   B_INT GetInternalMarge();

   //! only used internal.
   double GetInternalCorrectionAber();

   //! only used internal.
   double GetInternalCorrectionFactor();

   //! only used internal.
   double GetInternalSmoothAber();

   //! only used internal.
   B_INT GetInternalMaxlinemerge();

   //! in this mode polygons add clockwise, or contours,
   /*!
       and polygons added counter clockwise or holes.
   */
   void SetOrientationEntryMode( bool orientationEntryMode ) { m_orientationEntryMode = orientationEntryMode; }

	//! see SetOrientationEntryMode()
   bool GetOrientationEntryMode() { return m_orientationEntryMode; }

   //! if set true holes are linked into outer contours by double overlapping segments.
   /*!
       This mode is needed when the software using the boolean algorithm does 
       not understand hole polygons. In that case a contour and its holes form one
       polygon. In cases where software understands the concept of holes, contours
       are clockwise oriented, while holes are anticlockwise oriented.
       The output of the boolean operations, is following those rules also.
       But even if extracting the polygons from the engine, each segment is marked such
       that holes and non holes and linksegments to holes can be recognized.
   */
   void SetLinkHoles( bool doLinkHoles ) { m_doLinkHoles = doLinkHoles; }

	//! see SetLinkHoles()
   bool GetLinkHoles() { return m_doLinkHoles; } 

   //!lof file will be created when set True
   void SetLog( bool OnOff ); 

   //! used to write to log file
   void Write_Log(const char *);
   //! used to write to log file
   void Write_Log(const char *, const char *);
   //! used to write to log file
   void Write_Log(const char *, double);
   //! used to write to log file
   void Write_Log(const char *, B_INT);

   FILE* GetLogFile() { return m_logfile; }

    // methods used to add polygons to the eng using points

   //! Start adding a polygon to the engine
   /*
      The boolean operation work on two groups of polygons ( group A or B ),
      other algorithms are only using group A.

      You add polygons like this to the engine.

      // foreach point in a polygon ...
      if (booleng->StartPolygonAdd(GROUP_A))
      {
	      booleng->AddPoint(100,100);
	      booleng->AddPoint(-100,100); 
	      booleng->AddPoint(-100,-100); 
	      booleng->AddPoint(100,-100); 
      }
      booleng->EndPolygonAdd(); 

      \param A_or_B defines if the new polygon will be of group A or B

      Holes or added by adding an inside polygons with opposite orientation compared
      to another polygon added.
      So the contour polygon ClockWise, then add counterclockwise polygons for holes, and visa versa.
      BUT only if m_orientationEntryMode is set true, else all polygons are redirected, and become
      individual areas without holes. 
      Holes in such a case must be linked into the contour using two extra segments.
   */
   bool StartPolygonAdd( GroupType A_or_B );

   //! see StartPolygonAdd
   bool AddPoint(double x, double y);

   //! see StartPolygonAdd
   bool EndPolygonAdd();

    // methods used to extract polygons from the eng by getting its points

   //! Use after StartPolygonGet()
   int GetNumPointsInPolygon() { return m_numPtsInPolygon ; }

	//! get resulting polygons at end of an operation
	/*!
		// foreach resultant polygon in the booleng ...
		while ( booleng->StartPolygonGet() )
		{
			// foreach point in the polygon
			while ( booleng->PolygonHasMorePoints() )
			{
				fprintf(stdout,"x = %f\t", booleng->GetPolygonXPoint());
				fprintf(stdout,"y = %f\n", booleng->GetPolygonYPoint());
			}
			booleng->EndPolygonGet();
		}
   */
   
   bool StartPolygonGet();
   bool nextPolygon();
   //! see StartPolygonGet
   /*!
      This iterates through the first graph in the graphlist.
      Setting the current Node properly by following the links in the graph
      through its nodes. 
   */
   bool PolygonHasMorePoints(); 

   //! see StartPolygonGet
   double GetPolygonXPoint(); 

   //! see StartPolygonGet
   double GetPolygonYPoint(); 

	//! in the resulting polygons this tells if the current polygon segment is one
	//! used to link holes into the outer contour of the surrounding polygon
	bool GetHoleConnectionSegment();

	//! in the resulting polygons this tells if the current polygon segment is part
	//! of a hole within a polygon.
	bool GetHoleSegment();

   //! an other way to get the type of segment.
   kbEdgeType GetPolygonPointEdgeType();

	//! see StartPolygonGet()
   /*!
      Removes a graph from the graphlist.
      Called after an extraction of an output polygon was done.
   */
	void EndPolygonGet(); 

        GraphList* graphlist()
        {
           return m_graphlist;
        }
        const GraphList* graphlist()const
        {
           return m_graphlist;
        }
  private:

   bool m_doLog;

   //! contains polygons in graph form
   GraphList* m_graphlist;

   double m_MARGE;
   B_INT  m_GRID;
   double m_DGRID;
   double m_CORRECTIONABER;
   double m_CORRECTIONFACTOR;
   double m_SMOOTHABER;
   double m_MAXLINEMERGE;
   bool   m_WINDINGRULE;
   double m_ACCUR;
   double m_ROUNDFACTOR;

   bool m_orientationEntryMode;

   bool m_doLinkHoles;

   //! used in the StartPolygonAdd, AddPt, EndPolygonAdd sequence
   Graph*    m_GraphToAdd;
   //! used in the StartPolygonAdd, AddPt, EndPolygonAdd sequence
   Node*     m_lastNodeToAdd;
   //! used in the StartPolygonAdd, AddPt, EndPolygonAdd sequence
   Node*     m_firstNodeToAdd;

   //! the current group type ( group A or B )
   GroupType m_groupType;

   //! used in extracting the points from the resultant polygons
   Graph* m_getGraph;
   //! used in extracting the points from the resultant polygons
   KBoolLink* m_getLink;
   //! used in extracting the points from the resultant polygons
   Node* m_getNode;
   //! used in extracting the points from the resultant polygons
   double m_PolygonXPoint;
   //! used in extracting the points from the resultant polygons
   double m_PolygonYPoint;
   //! used in extracting the points from the resultant polygons
   int m_numPtsInPolygon;
   //! used in extracting the points from the resultant polygons
   int m_numNodesVisited;

   FILE* m_logfile;

   DL_Iter<GraphList*> m_graphiter;
public:

   //! use in Node to iterate links. 
/*    TDLI<KBoolLink>* 	_linkiter; */

   //! how many time run intersections fase.
   unsigned int m_intersectionruns;

};

#endif
