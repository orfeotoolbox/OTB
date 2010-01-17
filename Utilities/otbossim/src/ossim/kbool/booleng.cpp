/*! \file kbool/src/booleng.cpp
    \author Probably Klaas Holwerda

    Copyright: 2001-2004 (C) Probably Klaas Holwerda

    Licence: wxWidgets Licence

    RCS-ID: $Id: booleng.cpp 13795 2008-10-28 19:32:44Z gpotts $
*/

#ifdef __GNUG__
#pragma implementation
#endif

#include <math.h>
#include <time.h>

#include <ossim/kbool/booleng.h>
#include <ossim/kbool/link.h>
#include <ossim/kbool/line.h>
#include <ossim/kbool/node.h>
#include <ossim/kbool/graph.h>
#include <ossim/kbool/graphlst.h>
#include <iostream>

B_INT bmin(B_INT value1, B_INT value2)
{
	return((value1 < value2) ? value1 : value2 );
}

B_INT bmax(B_INT value1, B_INT value2)
{
	return((value1 > value2) ? value1 : value2 );
}

B_INT babs(B_INT a)
{
   if (a < 0) a=-a;
   return a;
}

//-------------------------------------------------------------------/
//----------------- Bool_Engine_Error -------------------------------/
//-------------------------------------------------------------------/

Bool_Engine_Error::Bool_Engine_Error(const char* message,
                                     const char* header,
                                     int degree,
                                     int fatal)
{
   _message = message;
   _header  = header;
   _degree = degree;
   _fatal = fatal;

}

Bool_Engine_Error::Bool_Engine_Error(const Bool_Engine_Error& a)
{
   _message = a._message;
   _header  = a._header;
   _degree = a._degree;
   _fatal = a._fatal;

}

Bool_Engine::Bool_Engine(const Bool_Engine& rhs)
   :m_graphlist(0),
    m_MARGE(rhs.m_MARGE),
    m_GRID(rhs.m_GRID),
    m_DGRID(rhs.m_DGRID),
    m_CORRECTIONABER(rhs.m_CORRECTIONABER),
    m_CORRECTIONFACTOR(rhs.m_CORRECTIONFACTOR),
    m_SMOOTHABER(rhs.m_SMOOTHABER),
    m_MAXLINEMERGE(rhs.m_MAXLINEMERGE),
    m_WINDINGRULE(rhs.m_WINDINGRULE),
    m_ACCUR(rhs.m_ACCUR),
    m_ROUNDFACTOR(rhs.m_ROUNDFACTOR),
    m_orientationEntryMode(rhs.m_orientationEntryMode),
    m_doLinkHoles(rhs.m_doLinkHoles),
    m_GraphToAdd(0),
    m_lastNodeToAdd(0),
    m_firstNodeToAdd(0),
    m_groupType(rhs.m_groupType),
    m_getGraph(0),
    m_getLink(0),
    m_getNode(0),
    m_PolygonXPoint(rhs.m_PolygonXPoint),
    m_PolygonYPoint(rhs.m_PolygonYPoint),
    m_numPtsInPolygon(rhs.m_numPtsInPolygon),
    m_numNodesVisited(rhs.m_numNodesVisited),
    m_logfile(0),
    m_intersectionruns(rhs.m_intersectionruns)
{
   if(rhs.m_graphlist)
   {
      m_graphlist = new GraphList(rhs.m_graphlist);
   }
//    _linkiter=new TDLI<KBoolLink>();
#if KBOOL_LOG == 1
   SetLog( true );
#else
   SetLog( false );
#endif
}

Bool_Engine_Error::~Bool_Engine_Error()
{
}

const char* Bool_Engine_Error::GetErrorMessage()
{
   return _message.c_str();
}

const char* Bool_Engine_Error::GetHeaderMessage()
{
   return _header.c_str();
}

int Bool_Engine_Error::GetErrorDegree()
{
   return _degree;
}

int Bool_Engine_Error::GetFatal()
{
   return _fatal;
}

//-------------------------------------------------------------------/
//----------------- Bool_Engine -------------------------------------/
//-------------------------------------------------------------------/

Bool_Engine::Bool_Engine()
{
//    _linkiter=new TDLI<KBoolLink>();
   m_intersectionruns = 1;

   m_orientationEntryMode = false;
   m_doLinkHoles = true;
      
   m_graphlist = new GraphList(this);
   m_ACCUR = 1e-4;
   m_WINDINGRULE = true;
   m_GraphToAdd = NULL;
   m_firstNodeToAdd = NULL;
   m_lastNodeToAdd = NULL;
   
   m_logfile = NULL;
   
#if KBOOL_LOG == 1
   SetLog( true );
#else
   SetLog( false );
#endif
}

Bool_Engine::~Bool_Engine()
{
   if (m_logfile != NULL)
      fclose (m_logfile);
   
   if (m_logfile != NULL)
   {
      fclose (m_logfile);
   }
    if(m_graphlist)
    {
       delete m_graphlist;
       m_graphlist = 0;
    }
}

void Bool_Engine::SetLog( bool OnOff )
{
	m_doLog = OnOff;
   if ( m_doLog )
	{
		if ( m_logfile == NULL )
		{
		    // create a new logfile
		    m_logfile = fopen(KBOOL_LOGFILE, "w");
			if (m_logfile == NULL)
				fprintf(stderr,"Bool_Engine: Unable to write to Boolean Engine logfile\n");
			else
			{
            time_t timer;
	         struct tm * today;
	         timer = time(NULL);
	         today = localtime(&timer);

            fprintf(m_logfile, "Logfile created on:\t\t\t%s", ctime( &timer ) );
			}
		}
	}
	else 
	{
	   if (m_logfile != NULL)
      {      
		   fclose (m_logfile);
         m_logfile = NULL;
      }
	}
}

void Bool_Engine::SetState( const char* process )
{
   Write_Log(process);
}

void Bool_Engine::error(const char *text, const char *title)
{
   Write_Log("FATAL ERROR: ", title);
   Write_Log("FATAL ERROR: ", text);
   throw Bool_Engine_Error(" Fatal Error", "Fatal Error", 9, 1);
};

void Bool_Engine::info(const char *text, const char *title)
{
   Write_Log("FATAL ERROR: ", title);
   Write_Log("FATAL ERROR: ", text);
};

void Bool_Engine::SetMarge(double marge)
{
    m_MARGE = marge;
    Write_Log("Bool_Engine::m_MARGE = %f\n", m_MARGE);
}

double Bool_Engine::GetAccur()
{
    return m_ACCUR;
}

void Bool_Engine::SetRoundfactor(double roundfac)
{
    m_ROUNDFACTOR = roundfac;
    Write_Log("Bool_Engine::m_ROUNDFACTOR = %f\n", m_ROUNDFACTOR);
}

double Bool_Engine::GetRoundfactor()
{
    return m_ROUNDFACTOR;
}

double Bool_Engine::GetMarge()
{
    return m_MARGE;
}

void Bool_Engine::SetDGrid(double dgrid)
{
    m_DGRID = dgrid;
    Write_Log("Bool_Engine::m_DGRID = %f\n", m_DGRID);
}

double Bool_Engine::GetDGrid()
{
    return m_DGRID;
}

void Bool_Engine::SetGrid(B_INT grid)
{
    m_GRID = grid;
    Write_Log("Bool_Engine::m_GRID = %lld\n", m_GRID);
}

B_INT Bool_Engine::GetGrid()
{
    return m_GRID;
}

void Bool_Engine::SetCorrectionAber(double aber)
{
    m_CORRECTIONABER = aber;
    Write_Log("Bool_Engine::m_CORRECTIONABER = %f\n", m_CORRECTIONABER);
}

double Bool_Engine::GetCorrectionAber()
{
    return m_CORRECTIONABER;
}

void Bool_Engine::SetCorrectionFactor(double aber)
{
    m_CORRECTIONFACTOR = aber;
    Write_Log("Bool_Engine::m_CORRECTIONFACTOR = %f\n", m_CORRECTIONFACTOR );
}

double Bool_Engine::GetCorrectionFactor()
{
    return m_CORRECTIONFACTOR;
}

void Bool_Engine::SetSmoothAber(double aber)
{
    m_SMOOTHABER = aber;
    Write_Log("Bool_Engine::m_SMOOTHABER = %f\n",m_SMOOTHABER );
}

double Bool_Engine::GetSmoothAber()
{
    return m_SMOOTHABER;
}

void Bool_Engine::SetMaxlinemerge(double maxline)
{
    m_MAXLINEMERGE = maxline;
    Write_Log("Bool_Engine::m_MAXLINEMERGE = %f\n",m_MAXLINEMERGE );
}

double Bool_Engine::GetMaxlinemerge()
{
    return m_MAXLINEMERGE;
}

void Bool_Engine::SetWindingRule(bool rule)
{
    m_WINDINGRULE = rule;
}

bool Bool_Engine::GetWindingRule()
{
    return m_WINDINGRULE;
}


void Bool_Engine::SetInternalMarge( B_INT marge )
{
   m_MARGE = (double)marge/m_GRID/m_DGRID;
}

B_INT Bool_Engine::GetInternalMarge()
{
   return (B_INT) ( m_MARGE*m_GRID*m_DGRID );
}

double Bool_Engine::GetInternalCorrectionAber()
{
   return  m_CORRECTIONABER*m_GRID*m_DGRID;
}

double Bool_Engine::GetInternalCorrectionFactor()
{
   return m_CORRECTIONFACTOR*m_GRID*m_DGRID;
}

double Bool_Engine::GetInternalSmoothAber()
{
   return m_SMOOTHABER*m_GRID*m_DGRID;
}

B_INT Bool_Engine::GetInternalMaxlinemerge()
{
   return (B_INT) ( m_MAXLINEMERGE*m_GRID*m_DGRID );
}

#define TRIALS 30  

bool Bool_Engine::Do_Operation(BOOL_OP operation)
{

#if KBOOL_DEBUG
   GraphList* saveme = new GraphList( m_graphlist );
#endif

   try 
   {
      switch (operation)
      {
         case (BOOL_OR):
         case (BOOL_AND):
         case (BOOL_EXOR):
         case (BOOL_A_SUB_B):
         case (BOOL_B_SUB_A):
            m_graphlist->Boolean(operation, m_intersectionruns);
            break;
         case (BOOL_CORRECTION):
            m_graphlist->Correction();
            break;
         case (BOOL_MAKERING):
            m_graphlist->MakeRings();
            break;
         case (BOOL_SMOOTHEN):
            m_graphlist->Smoothen( GetInternalSmoothAber() );
            break;
         default:
         {
	         error("Wrong operation","Command Error");
	         return false;
         }
      }
   }
	catch (Bool_Engine_Error& error)
	{

#if BOOL_DEBUG
      delete m_graphlist;
      m_graphlist = new GraphList( saveme );
      m_graphlist->WriteGraphsKEY(this);
#endif

	   if (m_logfile != NULL)
      {      
		   fclose (m_logfile);
         m_logfile = NULL;
      }

      info(error.GetErrorMessage(), "error");
		throw error;
	}
   catch(...)
   {

#if BOOL_DEBUG
      delete m_graphlist;
      m_graphlist = new GraphList( saveme );
      m_graphlist->WriteGraphsKEY(this);
#endif

	   if (m_logfile != NULL)
      {      
		   fclose (m_logfile);
         m_logfile = NULL;
      }

      info("Unknown exception", "error");
		throw ;
   }

#if BOOL_DEBUG
   delete saveme;
#endif

   return true;
}

bool Bool_Engine::StartPolygonAdd(GroupType A_or_B)
{
#if BOOL_DEBUG
    if (m_logfile != NULL)
   	fprintf(m_logfile, "-> StartPolygonAdd(%d)\n", A_or_B);
#endif
    if (m_GraphToAdd != NULL)
      return false;

    Graph *myGraph = new Graph(this);
    m_graphlist->insbegin(myGraph);
    m_GraphToAdd = myGraph;
    m_groupType = A_or_B;

    return true;
}

bool Bool_Engine::AddPoint(double x, double y)
{
   if (m_GraphToAdd == NULL){return false;}

   double scaledx = x * m_DGRID * m_GRID;
   double scaledy = y * m_DGRID * m_GRID;

   if ( scaledx > MAXB_INT  || scaledx < MINB_INT )
      error("X coordinate of vertex to big", "" );
   if ( scaledy > MAXB_INT || scaledy < MINB_INT )
      error("Y coordinate of vertex to big", "" );


   B_INT rintx = ((B_INT) (x * m_DGRID)) * m_GRID;
   B_INT rinty = ((B_INT) (y * m_DGRID)) * m_GRID;
   Node *myNode = new Node( rintx, rinty, this );

    // adding first point to graph
   if (m_firstNodeToAdd == NULL)
   {
#if BOOL_DEBUG
      if (m_logfile != NULL)
      {
		   fprintf(m_logfile, "-> AddPt() *FIRST* :");
		   fprintf(m_logfile, " input: x = %f, y = %f\n", x, y);
		   fprintf(m_logfile, " input: x = %I64d, y = %I64d\n", rintx, rinty) ;
      }
#endif

		m_firstNodeToAdd = (Node *) myNode;
		m_lastNodeToAdd  = (Node *) myNode;
   }
   else
   {
#if BOOL_DEBUG
      if (m_logfile != NULL)
      {
   		fprintf(m_logfile, "-> AddPt():");
	   	fprintf(m_logfile, " input: x = %f, y = %f\n", x, y);
		   fprintf(m_logfile, " input: x = %I64d, y = %I64d\n", rintx, rinty) ;
      }
#endif

		m_GraphToAdd->AddLink(m_lastNodeToAdd, myNode);
		m_lastNodeToAdd = (Node *) myNode;
   }

   return true;
}

bool Bool_Engine::EndPolygonAdd()
{
   if (m_GraphToAdd == NULL) {return false;}

   m_GraphToAdd->AddLink(m_lastNodeToAdd, m_firstNodeToAdd);
   m_GraphToAdd->SetGroup(m_groupType);
   m_GraphToAdd = NULL;
   m_lastNodeToAdd  = NULL;
   m_firstNodeToAdd = NULL;

   return true;
}

bool Bool_Engine::StartPolygonGet()
{
//    if(m_graphiter.isAttached())
//    {
      m_graphiter.Detach();
//    }
   if(m_graphlist)
   {
      m_graphiter.Attach((DL_List<GraphList*>*)m_graphlist);
      m_graphiter.toroot();
      return true;
   }
   return false;
}

bool Bool_Engine::PolygonHasMorePoints()
{
    // see if first point
    if (m_numNodesVisited == 0)
    {
        // don't need to touch the m_getNode
        m_numNodesVisited++;
        return true;
    }

    if (m_numNodesVisited < m_numPtsInPolygon)
    {
        // traverse to the next node
        m_getNode = m_getLink->GetOther(m_getNode);
        m_getLink = m_getLink->Forth(m_getNode);

        m_numNodesVisited++;
        return true;
    }
    else
    {
        return false;
    }
}
bool Bool_Engine::nextPolygon()
{
   if(!m_graphiter.empty())
   {
      ++m_graphiter;
      if(m_graphiter.hitroot())
      {
         m_graphiter.Detach();
         return false;
      }
      m_getGraph = (Graph*) m_graphiter.item();
      if(m_getGraph)
      {
         m_getLink  = m_getGraph->GetFirstLink();
      }
      else
      {
         m_getLink = 0;
      }
      if(m_getLink)
      {
         m_getNode  = m_getLink->GetBeginNode();
      }
      else
      {
         m_getNode = 0;
      }
      if(m_getGraph)
      {
         m_numPtsInPolygon = m_getGraph->GetNumberOfLinks();
      }
      else
      {
         m_numPtsInPolygon = 0; 
      }
      m_numNodesVisited = 0;
      
      return (m_getGraph&&m_getLink);
   } 
   
   return false;
}

void Bool_Engine::EndPolygonGet()
{
   m_getGraph = 0;
   m_getGraph = 0;
   m_getLink  = 0;
   m_getNode  = 0;
//     m_graphlist->removehead();
//     delete m_getGraph;
}

double Bool_Engine::GetPolygonXPoint()
{
   if(!m_getNode) return 0;
    return m_getNode->GetX()/m_GRID/m_DGRID;
}

double Bool_Engine::GetPolygonYPoint()
{
   if(!m_getNode) return 0;
    return m_getNode->GetY()/m_GRID/m_DGRID;
}

bool Bool_Engine::GetHoleSegment()
{
   if(!m_getLink) return false;
     if (m_getLink->GetHole())
		 return true;
	 return false;
}

bool Bool_Engine::GetHoleConnectionSegment()
{
   if(!m_getLink) return false;
   if (m_getLink->GetHoleLink())
      return true;
   return false;
}

kbEdgeType Bool_Engine::GetPolygonPointEdgeType()
{
   if(!m_getLink) return KB_FALSE_EDGE;
   
   // see if the point is the beginning of a false edge
   if ( m_getLink->GetHoleLink() )
      return  KB_FALSE_EDGE;
   else
      // it's either an outside or inside edge
      if ( m_getLink->GetHole() )
         return  KB_INSIDE_EDGE;
      else
         return  KB_OUTSIDE_EDGE;
}


void Bool_Engine::Write_Log(const char *msg1)
{
   if (m_logfile == NULL)
       return;

   fprintf(m_logfile,"%s \n",msg1);
}

void Bool_Engine::Write_Log(const char *msg1, const char* msg2)
{
   if (m_logfile == NULL)
       return;

   fprintf(m_logfile,"%s %s\n",msg1, msg2);
}

void Bool_Engine::Write_Log(const char *fmt, double dval)
{
   if (m_logfile == NULL)
       return;

   fprintf(m_logfile,fmt,dval);
}

void Bool_Engine::Write_Log(const char *fmt, B_INT bval)
{
   if (m_logfile == NULL)
       return;

   fprintf(m_logfile,fmt,bval);
}
