/* PROJECTN.H */

#ifndef __PROJECTN_H__

#define __PROJECTN_H__
#ifdef __cplusplus
extern "C" {
#endif

#define PLATE_CARREE        0
#define TRANSVERSE_MERCATOR 1
#define LAMBERT_EQUAL_AREA  2
#define GNOMONIC            3
#define NO_PROJECTION       4

void set_projection_parameters( double cm, double origin, double r,
				int projection );

double central_meridian( double xmin, double xmax );

void set_plate_carree_parameters( double cm, double origin, double r );
void pcarree_xy( double *x, double *y );
void pcarree_latlon( double *x, double *y );

void set_tm_parameters( double cm, double origin, double r );
void tm_xy( double *x, double *y );
void tm_latlon( double *x, double *y );

void set_lambert_parameters( double cm, double origin, double r );
void lambert_xy( double *x, double *y );
void lambert_latlon( double *x, double *y );

void set_gnomonic_parameters( double cm, double origin, double r );
void gnomonic_xy( double *x, double *y );
void gnomonic_latlon( double *x, double *y );

#ifdef __cplusplus
}
#endif
   
#endif
