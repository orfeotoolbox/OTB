#!/usr/bin/env python

from liblas import file as lasfile
from liblas import header
from liblas import point
from liblas import srs

import glob
import struct

import os, sys


import cx_Oracle as oci

# big-endian DOUBLE, DOUBLE, DOUBLE, LONG, LONG
format = '>dddll'
ptsize = struct.calcsize(format)



class Translator(object):

    def construct_parser(self):
        from optparse import OptionParser, OptionGroup
        usage = "usage: %prog [options] arg"
        parser = OptionParser(usage)
        g = OptionGroup(parser, "Base options", "Basic Translation Options")
        g.add_option("-c", "--connection", dest="connection",
                          help="OCI connection string", metavar="CONNECTION")
        g.add_option("-o", "--output", dest='output',
                          help="LAS file to write", metavar="OUTPUT")
        g.add_option("-s", "--sql", dest='sql',
                          help="SQL to select the point cloud ", metavar="SQL")
        g.add_option("-l", "--column", dest='column',
                          help="Column name containing the point cloud object ", metavar="COLUMN")

        g.add_option("-p", "--precision", dest='precision',
                          help="Numeric precision (# of digits) to maintain for the output file ", metavar="PRECISION")

        g.add_option("-r", "--srs", dest='srs',
                          help="Coordinate system override", metavar="SRS")
                          
        g.add_option("-w", "--overwrite",
                          action="store_true", dest="overwrite", 
                          help="overwrite the existing file")

        g.add_option("-m", "--min-offset",
                          action="store_true", dest="offset", default=False,
                          help="Use the minimum values as base for offset")
                          
        g.add_option("-q", "--quiet",
                          action="store_false", dest="verbose", default=False,
                          help="Don't say what we're doing on stdout")
                          
        parser.add_option_group(g)

        if self.opts:
            g = OptionGroup(parser, "Special Options", "Special options")
            for o in self.opts:
                g.add_option(o)
            parser.add_option_group(g)
            
        parser.set_defaults(verbose=True, precision = 6)

        self.parser = parser
        
    def __init__(self, arguments, options=None):
        self.connection = None
        self.output = None
        self.sql = None
        
        self.opts = options
        self.construct_parser()
        self.options, self.args = self.parser.parse_args(args=arguments)
        
        if self.args:
            self.options.connection = self.args[0]
            
        if not self.options.output:
            try:
                self.options.output = self.args[1]
            except IndexError:
                self.options.output = 'output.las'
                
        if not self.options.sql:
            try:
                self.options.sql = self.args[2]
            except IndexError:
                raise self.parser.error("No SQL was provided to select the point cloud!")           

        if self.options.output:
            self.options.output = os.path.abspath(self.options.output)
            if os.path.isdir(self.options.output):
                raise self.parser.error("Output '%s' is a directory, not a file " % self.options.output)
            
            if os.path.exists(self.options.output):
                if not self.options.overwrite:
                    raise self.parser.error("Output file '%s' exists, but you have not selected the --overwrite option" % self.options.output)
        else:
            raise self.parser.error("No output was specified")

        try:
            self.options.precision = int(self.options.precision)
            if not self.options.precision:
                raise self.parser.error("Precision cannot be 0")
        except:
            raise self.parser.error("Precision was not an number")
            
        self.min = point.Point()
        self.max = point.Point()
        self.count = 0
        self.first_point = True
        self.cloud_column = True
        
        if self.options.srs:
            self.srs = srs.SRS()
            self.srs.set_userinput(self.options.srs)
            print 'setting srs to %s' %self.srs.proj4
        else:
            self.srs = None
            
    def print_options(self):
        print self.options
        
    def connect(self):
        self.con = oci.Connection(self.options.connection)

    def is_block_table(self, cursor_description):
        output = True
        names = [   'OBJ_ID','BLK_ID','BLK_EXTENT','BLK_DOMAIN',
                    'PCBLK_MIN_RES','PCBLK_MAX_RES','NUM_POINTS',
                    'NUM_UNSORTED_POINTS','PT_SORT_DIM','POINTS']
        for name in cursor_description:
            if name.upper() not in names:
                return False

            
    def get_points(self, num_points, blob):
        points = []

        for i in xrange(num_points):
            rng = ptsize*i,ptsize*(i+1)
            d = struct.unpack(format,blob[ptsize*i:ptsize*(i+1)])
            x, y, z, blk_id, pt_id = d
            p = point.Point()
            p.x = x; p.y = y; p.z = z

            if self.first_point:
                self.min.x = p.x
                self.min.y = p.y
                self.max.x = p.x
                self.max.y = p.y
                self.min.z = p.z
                self.max.z = p.z
                self.first_point = False

            # cumulate min/max for the header
            self.min.x = min(self.min.x, p.x)
            self.max.x = max(self.max.x, p.x)
        
            self.min.y = min(self.min.y, p.y)
            self.max.y = max(self.max.y, p.y)
        
            self.min.z = min(self.min.z, p.z)
            self.max.z = max(self.max.z, p.z)
        
            self.count += 1
            
            points.append(p)
        return points
    
    def summarize_files(self):
        pass
    
    def open_output(self):
        h = header.Header()
        
        prec = 10**-(self.options.precision-1)
        h.scale = [prec, prec, prec]
        
        if self.options.offset:
            h.offset = [self.min.x, self.min.y, self.min.z]
            if self.options.verbose:
                print 'using minimum offsets', h.offset
        
        if self.srs:
            h.srs = self.srs
            
        output = lasfile.File(self.options.output,mode='w',header=h)
        return output
    
    def write_points(self, points):
        
        for p in points:
            self.output.write(p)
    
    def rewrite_header(self):
        self.output.close()
        self.output = lasfile.File(self.options.output)
        h = self.output.header
        self.output.close()
        h.min = [self.min.x, self.min.y, self.min.z]
        h.max = [self.max.x, self.max.y, self.max.z]

        rc = h.point_return_count
        rc[0] = self.count
        h.point_return_count = rc
        
        self.output = lasfile.File(self.options.output, mode='w+', header=h)
        self.output.close()
    
    def get_srid(self, srid):
        cur = self.con.cursor()
        if not srid: return ''
        cur.execute('SELECT WKTEXT,WKTEXT3D from MDSYS.CS_SRS where srid=%d'%(int(srid)))
        res = cur.fetchall()
        for wkt in res:
            text = wkt[0]
            text3d = wkt[1]
            s = srs.SRS()
            if text3d:
                s.wkt = text3d
            else:
                s.wkt = text
            return s
        
    def process(self):
        self.print_options()
        self.connect()

        self.cur = self.con.cursor()
#        self.cur.execute(self.options.sql)
#        clouds = []
#
#        res = self.cur.fetchall()

#        for row in res:
#            for column in row:
#                try:
#                    column.BASE_TABLE_COL
#                    clouds.append(column)
#                except AttributeError:
#                    # This column isn't a cloud
#                    pass

#        points = []
        
        # write an actual cloud column
#        for cloud in clouds:
#            cur2 = self.con.cursor()
#
#            cur2.execute('SELECT NUM_POINTS, POINTS, BLK_EXTENT FROM %s'% cloud.BLK_TABLE)
            
            
#            for num_points, blob, extent in cur2:
#                # set the SRS from the first geometry
#                if not self.srs:
#                    self.srs = self.get_srid(extent.SDO_SRID)
                    
#                b = blob.read()
#                points.append(self.get_points(num_points,b))
        
#        num_pts_index, blob_index = self.get_block_indexes(self.cur)
        
        # if we don't have a cloud object, we'll assume that NUM_POINTS and 
        # the POINTS blob exist in our already queried cursor
        clouds = None
        points = []
        print 'have srs?: %s' % bool(self.srs)
        if not clouds:
            cur = self.cur.execute(self.options.sql)
            num_pts_index, blob_index = self.get_block_indexes(cur)
            for row in cur:
                num_points = row[num_pts_index]
                blob = row[blob_index].read()
                points.append(self.get_points(num_points, blob))
                # try to set the SRS
                if not self.srs:
                    for col in row:
                        try:
                            col.SDO_SRID
                            self.srs = self.get_srid(col.SDO_SRID)
                            break
                        except AttributeError:
                            continue
                    
                    # if we still haven't been able to set an SRS, don't try 
                    # to anymore
                    if not self.srs:
                        self.srs = srs.SRS()

        self.output = self.open_output()
        for pts in points:
            self.write_points(pts)
        
        self.rewrite_header()

    def get_block_indexes(self, cursor):
        
        num_pts_index = None
        blob_index = None
        i = 0
        for name in cursor.description:
            name = name[0]
            if name.upper() == 'POINTS':
                blob_index = i
            if name.upper() == 'NUM_POINTS':
                num_pts_index = i
            i+=1
        return (num_pts_index, blob_index)

def main():
    import optparse

    options = []
#     o = optparse.make_option("-r", "--remainder", dest="remainder",
#                          type="choice",default='end', 
#                           help="""what to do with the remainder -- place it at the beginning, 
# place it at the end, or evenly distribute it across the segment""",
#                           choices=['end','begin','uniform'])
#     options.append(o)
    
    d = Translator(sys.argv[1:], options=options)
    d.process()

if __name__=='__main__':
    main()
