/******************************************************************************
 *
 *N  Module POLYGRF.C
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Coordinate transformations of points.  Can create and manipulate
 *    sets of x,y coords
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    N/A
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Thomas Wood  October, 1990, Kurnigan/Ritchie C
 *    Updated August 1991, ANSI C
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions:
 *F
 *    ISegment MakeISegment( IPoint from, IPoint to );
 *    ITransform MakeITransform( IBox from, IBox to );
 *    IPoint TranslateIPoint( ITransform transform, IPoint a, int flag );
 *    IBox AndIBox( IBox a, IBox b );
 *E
 *****************************************************************************/

#include <ossim/vpfutil/xmemory.h>
#include <ossim/vpfutil/polygrf.h>

/******************************************************************************
 *
 *N  MakeISegment
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Called with two sets of xy coordinates, from and to.  Returns from and
 *    the delta of from and to.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    from         <input>  == (IPoint) originating xy coordinates.
 *    to           <input>  == (IPoint) ending xy coordinates.
 *    MakeISegment <output> == (ISegement) origin and delta
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Thomas Wood October 1990 Original Kurnigan/Ritchie C
 *    Updated August 1991, ANSI C
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *****************************************************************************/
ISegment MakeISegment( IPoint from,
		       IPoint to )
{
  ISegment current;

  current.origin = from;
  current.delta.x = to.x - from.x;
  current.delta.y = to.y - from.y;

  return current;
}


/*****************************************************************************
 *
 *N  MakeITransform
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Inputs two sets of origin coordinates plus their delta coordinates.
 *    Calculates and returns offset of the origins and if the deltas are
 *    different, the new scale. (if deltas are the same, scale is 1.0).
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    from        <input>  == (IBox) originating coordinates & delta
 *    to          <input>  == (IBOX) ending coordinates and delta
 *    MakeITransform <output> == (ITransform) offset and scale
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Thomas Wood, October 1990, Original version Kurnigan/Ritchie C
 *    Updated August 1991, ANSI C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 ****************************************************************************/
ITransform  MakeITransform( IBox from,
                            IBox to )
{
  ITransform current;
  register float temp;

  current.scale.x = current.scale.y = 1.0;

  if ((to.delta.x == 0.0)||
      (from.delta.x == 0.0))
    to.delta.x = from.delta.x = 1;

  if ((to.delta.y == 0.0)||
      (from.delta.y == 0.0))
    to.delta.y = from.delta.y = 1;

  if (to.delta.x == from.delta.x)
      current.offset.x = to.origin.x - from.origin.x;
  else
    {
      current.scale.x =
	    (float)to.delta.x / (float)from.delta.x;
      temp = (float)(to.origin.x * from.delta.x);
      temp = temp / to.delta.x;
      temp += (float)((temp < 0)? -.05: 0.5);
      current.offset.x = (int)(temp - from.origin.x);
    }

  if (to.delta.y == from.delta.y)
      current.offset.y = to.origin.y - from.origin.y;
  else
    {
      current.scale.y =
	    (float)to.delta.y / (float)from.delta.y;
      temp = (float)(to.origin.y * from.delta.y);
      temp = temp / to.delta.y;
      temp += (float)((temp < 0)? -.05: 0.5);
      current.offset.y = (int)(temp - from.origin.y);
    }

  return current;
}

/*****************************************************************************
 *
 *N  TranslateIPoint
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Rescales 'a' based on flag and transform.  If flag is 1, scales down,
 *    else up.  Scaling factor based on transform.scale and transform.offset
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    transform       <input>  == (ITransform) tranformation index/ratio
 *    a               <input>  == (IPoint) coordinates to be transformed
 *    flag            <input>  == (int) scale index (up=0,down=1)
 *    TranslateIPoint <output> == (IPoint) scaled coordinates
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Thomas Wood, October 1990, Original Version Kurnigan/Ritchie C
 *    Updated August 1991, ANSI C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 ****************************************************************************/
IPoint TranslateIPoint( ITransform transform,
                        IPoint 	   a,
			int        flag )
{
  IPoint output;
  register float temp;

  output = a;
  if ((transform.scale.x == 1.0)||(transform.scale.x == 0.0))
    output.x += ((flag)? -1: 1) * transform.offset.x;
  else
    {
      if (flag)
	{
	  temp = (float)output.x;
	  temp /= transform.scale.x;
	  temp += (float)((temp < 0)?  -.05: 0.5);
	  temp -= transform.offset.x;
	  output.x = (int)temp;
	}
      else
	{
	  temp = (float)(output.x + transform.offset.x);
	  temp *= transform.scale.x;
	  temp += (float)((temp < 0)?  -.05: 0.5);
	  output.x = (int)temp;
	}
    }

  if ((transform.scale.y == 1)||(transform.scale.y == 0.0))
    output.y += ((flag)? -1: 1) * transform.offset.y;
  else
    {
      if (flag)
	{
	  temp = (float)output.y;
	  temp /= transform.scale.y;
	  temp += (float)((temp < 0)?  -.05: 0.5);
	  temp -= transform.offset.y;
	  output.y = (int)temp;
	}
      else
	{
	  temp = (float)(output.y + transform.offset.y);
	  temp *= transform.scale.y;
	  temp += (float)((temp < 0)?  -.05: 0.5);
	  output.y = (int)temp;
	}
    }
  return output;
}


/*****************************************************************************
 *
 *N AndIBox
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Inputs two sets of coordinates and deltas, checks if coords are proper.
 *    If not, returns IBox=-1.  Calculates origin values plus delta.  IF  'a'
 *    origin values < 'b' origin values, returns -1.  Finally, the higher origin
 *    values and lower delta values are calculated and returned
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    a       <input>  == (IBox) set of coords plus deltas
 *    b       <input>  == (IBox) set of coords plus deltas
 *    AndIBox <output> == (IBox) set of highest values of above
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Thomas Wood  October, 1990, Kurnigan/Ritchie C
 *    Updated August 1991, ANSI C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    isBox
 *E
 ****************************************************************************/
IBox AndIBox( IBox a,
              IBox b )
{
  IBox output;
  IPoint acorner,bcorner;

  output.origin.x = output.delta.x = 
    output.origin.y = output.delta.y = -1; 
  
  if (!isBox(a)) return output;
  if (!isBox(b)) return output;

  acorner.x = a.origin.x + a.delta.x;
  acorner.y = a.origin.y + a.delta.y;

  bcorner.x = b.origin.x + b.delta.x;
  bcorner.y = b.origin.y + b.delta.y;

  if (acorner.x < b.origin.x) return output;
  if (acorner.y < b.origin.y) return output;
  if (bcorner.x < a.origin.x) return output;
  if (bcorner.y < a.origin.y) return output;

  if (a.origin.x < b.origin.x)
    output.origin.x = b.origin.x;
  else
    output.origin.x = a.origin.x;

  if (a.origin.y < b.origin.y)
    output.origin.y = b.origin.y;
  else
    output.origin.y = a.origin.y;

  if (acorner.x < bcorner.x)
    output.delta.x = acorner.x - output.origin.x;
  else
    output.delta.x = bcorner.x - output.origin.x;

  if (acorner.y < bcorner.y)
    output.delta.y = acorner.y - output.origin.y;
  else
    output.delta.y = bcorner.y - output.origin.y;

  return output;
}
