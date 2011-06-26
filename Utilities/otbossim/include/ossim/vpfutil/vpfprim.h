#ifndef VPFPRIM_HEADER
#define VPFPRIM_HEADER
#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************/
/*                           VPFPRIM.H                              */
/********************************************************************/

/* Must include "vpftable.h" */


/* VPF edge record internal structure */
typedef struct {
   ossim_int32 id;
   ossim_int32 start;
   ossim_int32 end;
   ossim_int32 right;
   ossim_int32 left;
   ossim_int32 rightfwd;
   ossim_int32 leftfwd;
   char dir;
   ossim_int32 npts;
   coordinate_type *coord;
   /* New as of 2/18/92 for coordinate arrays too large for memory */
   FILE *fp;
   ossim_int32 startpos, pos, current_coordinate;
   char coord_type;
} edge_rec_type;


/* "static" part of the edge record (non-variable) */
typedef struct {
   ossim_int32 id;
   ossim_int32 start;
   ossim_int32 end;
   ossim_int32 right;
   ossim_int32 left;
   ossim_int32 rightfwd;
   ossim_int32 leftfwd;
   char dir;
   ossim_int32 npts;
} edge_static_type;


/* VPF face record structure */
typedef struct {
   ossim_int32 id;
   ossim_int32 ring;
} face_rec_type;


/* VPF ring record structure */
typedef struct {
   ossim_int32 id;
   ossim_int32 face;
   ossim_int32 edge;
} ring_rec_type;


/* VPF entity node record internal structure */
typedef struct {
   ossim_int32 id;
   ossim_int32 face;
   float x;
   float y;
} point_rec_type;


/* VPF annotation record internal structure */
typedef struct {
   long  int id;
   float height;
   char  *text;
   float x;
   float y;
} anno_rec_type;


/* Functions: */

edge_rec_type create_edge_rec( row_type row, vpf_table_type edge_table );

edge_rec_type read_edge( ossim_int32 id,
			 vpf_table_type edge_table );

edge_rec_type read_next_edge( vpf_table_type edge_table );

/* New as of 2/18/92 for coordinate arrays too large for memory */
coordinate_type first_edge_coordinate( edge_rec_type *edge_rec );
coordinate_type next_edge_coordinate( edge_rec_type *edge_rec );
coordinate_type get_edge_coordinate( ossim_int32 n,
				     edge_rec_type *edge_rec );
/***/

face_rec_type read_face( ossim_int32 id,
			 vpf_table_type face_table );

face_rec_type read_next_face( vpf_table_type face_table );

ring_rec_type read_ring( ossim_int32 id,
			 vpf_table_type ring_table );

ring_rec_type read_next_ring( vpf_table_type ring_table );

point_rec_type read_point( ossim_int32 id,
			   vpf_table_type point_table );

point_rec_type read_next_point( vpf_table_type point_table );

anno_rec_type read_anno( ossim_int32 id,
			 vpf_table_type anno_table );

anno_rec_type read_next_anno( vpf_table_type anno_table );


#ifdef __cplusplus
}
#endif

#endif
