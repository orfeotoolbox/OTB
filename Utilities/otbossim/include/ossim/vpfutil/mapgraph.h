
/* MAPGRAPH.H */
#ifndef __MAPGRAPH_H__
#define __MAPGRAPH_H__ 1

#ifdef __cplusplus
extern "C" {
#endif

#define LONGITUDE(lon)  {\
   if (lon < -180.0) lon += 360.0; \
   if (lon > 180.0) lon -= 360.0; \
}

#define LATITUDE(lat)  {\
   if (lat < -90.0) lat += 180.0; \
   if (lat > 90.0) lat -= 180.0; \
}


/* Map Drawing Functions: */

#ifndef __MSDOS__
#define far
#endif

void mapinit( double xmn,
	      double ymn,
	      double xmx,
	      double ymx,
	      int width,
	      int height,
	      void far (*fwdproj)(),
	      void far (*invproj)() );

void screenxy( double xmap, double ymap,
	       int *xscreen, int *yscreen );

void mapxy( int xscreen, int yscreen, double *xmap, double *ymap );


void draw_graticule( double xstart,
		     double ystart,
		     double xstep,
		     double ystep,
		     int    ninterm_pts,
		     int    color,
		     int    style,
		     int    thickness );


#ifdef __cplusplus
}
#endif
   
#endif
