import liblas
import cx_Oracle
import cx_Oracle as oci

con = oci.Connection('lidar/lidar@ubuntu.hobu.biz/crrel.local')

cur = con.cursor()


def run (operations):
    import copy
    ops = copy.copy(operations)
    
    for op in ops:
        try:
            print op
            cur.execute(op)
        except cx_Oracle.DatabaseError, e:
            print e


def insert_lidar(filename):
    reader = liblas.file.File(filename)

    i= 0

    commit_frequency = 100000
    sql = """INSERT INTO LIDAR_DATA (ID, GEOMETRY) 
                VALUES (:id, 
                        MDSYS.SDO_GEOMETRY(3001, NULL, MDSYS.SDO_POINT_TYPE(:x,:y,:z), NULL, NULL))"""
    rows = []
    for p in reader:

        # sql = "INSERT INTO LIDAR_DATA (ID, GEOMETRY) VALUES (%d,SDO_UTIL.FROM_WKTGEOMETRY('POINT(%.6f %.6f %.6f)'))" % (i, p.x,p.y,p.z)
        # sql = "INSERT INTO LIDAR_DATA (ID, GEOMETRY) VALUES (%d, MDSYS.SDO_GEOMETRY(2001, NULL, MDSYS.SDO_POINT_TYPE(%.6f,%.6f,%.6f), NULL, NULL))" % (i,p.x,p.y,p.z)
    #    sql = "INSERT INTO LIDAR_DATA (ID, GEOMETRY) VALUES (%d, MDSYS.SDO_POINT_TYPE(6,8,10))"%i

        row = { 'x':p.x,
                'y':p.y,
                'z':p.z,
                'id':i}
        rows.append(row)
        if i %commit_frequency == 0:
            cur.executemany(sql, rows)
            con.commit()
            rows = []
            print 'commit'

        i+=1

    cur.executemany(sql,rows)
    con.commit()



cleanup = [  "DELETE FROM BASE",
                "DROP TABLE BASE",
                "DROP TABLE BLKTAB",
                "DROP TABLE INPTAB",
                "DROP TABLE LIDAR_DATA",
                "DROP TABLE RES",
                "DELETE FROM USER_SDO_GEOM_METADATA WHERE TABLE_NAME='BASE'",
                "DROP TABLE RESTST",
                "DELETE FROM HOBU",
                "DROP TABLE HOBU",
                "DROP TABLE hobures",
                "DROP TABLE FOO",
                "DELETE FROM USER_SDO_GEOM_METADATA WHERE TABLE_NAME='FOO'",
                "DROP INDEX hobu_cloud_idx"
                ]

run(cleanup)                    

creation = [  'CREATE TABLE LIDAR_DATA (ID NUMBER, GEOMETRY SDO_GEOMETRY)',
                    ]
run(creation)
 
filename = '/Users/hobu/svn/liblas/trunk/test/data/TO_core_last_zoom.las'
insert_lidar(filename)


inptab = ["""
CREATE TABLE inptab NOLOGGING AS
  SELECT TO_CHAR(a.id)          rid,
         a.geometry.sdo_point.x val_d1,
         a.geometry.sdo_point.y val_d2,
         a.geometry.sdo_point.z val_d3
  FROM lidar_data a
"""]
run(inptab)

base = ["""
create table base (pc sdo_pc)"""]
run(base)

blocktab = ["""
create table blktab as select * from mdsys.sdo_pc_blk_table"""]
run(blocktab)

res = ["""
create table res (ptn_id number, point_id number,
rid varchar2(24), val_d1 number, val_d2 number, val_d3 number)"""]
run(res)

truncates = ["""truncate table res""","truncate table blktab"]
run(truncates)


init = [
"""insert into base values (sdo_pc_pkg.init(
          'BASE', -- Table that has the SDO_POINT_CLOUD column defined
          'PC',   -- Column name of the SDO_POINT_CLOUD object 
          'BLKTAB', -- Table to store blocks of the point cloud
           'blk_capacity=50', -- max # of points per block 
           mdsys.sdo_geometry(2003, 8307, null,
              mdsys.sdo_elem_info_array(1,1003,3),
              mdsys.sdo_ordinate_array(-180, -90, 180, 90)),  -- Extent 
     0.5, -- Tolerance for point cloud
           3, -- Total number of dimensions
           null))
           """]
           
code ="""
declare
  pc sdo_pc;
begin
  -- Initialize the Point Cloud object. 
  pc := sdo_pc_pkg.init(
          'BASE', -- Table that has the SDO_POINT_CLOUD column defined
          'PC',   -- Column name of the SDO_POINT_CLOUD object 
          'BLKTAB', -- Table to store blocks of the point cloud
           'blk_capacity=20000', -- max # of points per block 
           mdsys.sdo_geometry(2003, 8307, null,
              mdsys.sdo_elem_info_array(1,1003,3),
              mdsys.sdo_ordinate_array(630250.00,4834500.00,630500.00,4834750.00)),  -- Extent 
     0.5, -- Tolerance for point cloud
           3, -- Total number of dimensions
           null);

  -- Insert the Point Cloud object  into the "base" table.
  insert into base values (pc);

  -- Create the blocks for the point cloud.
  sdo_pc_pkg.create_pc(
    pc,       -- Initialized PointCloud object
    'INPTAB', -- Name of input table to ingest into the pointcloud
    'RES'     -- Name of output table that stores the points (with ptn_id,pt_id)
  );
end;"""
run([code])

con.commit()


code = """
create table restst as select * from mdsys.sdo_pc_blk_table


"""
run([code])

code ="""

declare
  inp  sdo_pc;
begin
  select pc INTO inp  from base where rownum=1;
  insert into restst
  select * from
    table(sdo_pc_pkg.clip_pc
	  (
            inp,  -- Input Point Cloud object
            sdo_geometry(2003, 8307, null,
              mdsys.sdo_elem_info_array(1, 1003, 3),
              mdsys.sdo_ordinate_array(630355, 4834609,630395,  4834641)), -- Query 
	    null, null, null, null));
end;
"""
# -- 630355 4834609 630395 4834641
#               mdsys.sdo_ordinate_array(630355, 630395, 4834609,4834641)), -- Query 
# 
# --              mdsys.sdo_ordinate_array(-74.1, -73.9, 39.99999,40.00001)), -- Query 

run([code])

sql = "select num_points, blk_id from restst order by blk_id"
cur.execute(sql)
for i in cur.fetchall():
    print i

sql = "select NUM_POINTS, POINTS from blktab where blk_id=10"
cur.execute(sql)

num, pts = cur.fetchone()

data = pts.read()

import struct

# big-endian DOUBLE, DOUBLE, DOUBLE, LONG, LONG
format = '>dddll'
size = struct.calcsize(format)

for i in xrange(num):
    rng = size*i,size*(i+1)
  #  print rng
    d = struct.unpack(format,data[size*i:size*(i+1)])

con.commit()


code = ["create table foo as select * from blktab",
        "create table hobures as select * from mdsys.sdo_pc_blk_table"]
run(code)


base = ["""
create table HOBU (CLOUD sdo_pc)"""]
run(base)

code = """
INSERT INTO user_sdo_geom_metadata VALUES (
    'foo',
    'blk_extent',
    MDSYS.SDO_DIM_ARRAY(
MDSYS.SDO_DIM_ELEMENT('X', 630250.000000, 630500.000000, 0.05),
MDSYS.SDO_DIM_ELEMENT('Y', 4834500, 4834750, 0.05)),
    8307)
"""
#MDSYS.SDO_DIM_ELEMENT('Z', 46.830000, 170.650000, 0.05)),

run([code])

index=["""CREATE INDEX hobu_cloud_idx ON foo (blk_extent)
INDEXTYPE IS MDSYS.SPATIAL_INDEX"""]
run(index)

code ="""
declare
  pc sdo_pc;
begin
  -- Initialize the Point Cloud object. 
  pc := sdo_pc_pkg.init(
          'HOBU', -- Table that has the SDO_POINT_CLOUD column defined
          'CLOUD',   -- Column name of the SDO_POINT_CLOUD object 
          'FOO', -- Table to store blocks of the point cloud
           'blk_capacity=20000', -- max # of points per block 
           mdsys.sdo_geometry(2003, 8307, null,
              mdsys.sdo_elem_info_array(1,1003,3),
              mdsys.sdo_ordinate_array(630250.00,4834500.00,630500.00,4834750.00)),  -- Extent 
     0.5, -- Tolerance for point cloud
           3, -- Total number of dimensions
           null);

  -- Insert the Point Cloud object  into the "base" table.
  insert into hobu values (pc);


end;"""
run([code])
con.commit()


code ="""

declare
  inp  sdo_pc;
begin
  select cloud INTO inp  from HOBU where rownum=1;
  insert into hobures
  select * from
    table(sdo_pc_pkg.clip_pc
	  (
            inp,  -- Input Point Cloud object
            sdo_geometry(2003, 8307, null,
              mdsys.sdo_elem_info_array(1, 1003, 3),
              mdsys.sdo_ordinate_array(630355, 4834609,630395,  4834641)), -- Query 
	    null, null, null, null));
end;
"""
# -- 630355 4834609 630395 4834641
#               mdsys.sdo_ordinate_array(630355, 630395, 4834609,4834641)), -- Query 
# 
# --              mdsys.sdo_ordinate_array(-74.1, -73.9, 39.99999,40.00001)), -- Query 

run([code])

sql = "select num_points, blk_id from hobures order by blk_id"
cur.execute(sql)
for i in cur.fetchall():
    print i
