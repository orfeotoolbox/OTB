#!/usr/bin/env python

from scipy import spatial
from numpy import array, vstack, empty, float

from liblas import file
from liblas import header
from liblas import point
from liblas import color
from liblas import srs
from liblas import guid


import sys
import os


class Translator(object):

    def construct_parser(self):
        from optparse import OptionParser, OptionGroup
        usage = "usage: %prog --leaf-size 1000 filename.las"
        parser = OptionParser(usage)
        g = OptionGroup(parser, "Base options", "Basic Translation Options")
        g.add_option("-i", "--input", dest="input",
                          help="Input LAS file", metavar="INPUT")
        g.add_option("-s", "--leaf-size", dest='leafsize',
                          help="Maximum Leaf size", metavar="LEAFSIZE")
        g.add_option("-o", "--output", dest='output',
                          help="Column name containing the point cloud object ", metavar="OUTPUT")

                          
        g.add_option("-w", "--overwrite",
                          action="store_true", dest="overwrite", 
                          help="overwrite the existing file")

                          
        g.add_option("-q", "--quiet",
                          action="store_false", dest="verbose", default=False,
                          help="Don't say what we're doing on stdout")
                          
        parser.add_option_group(g)

        if self.opts:
            g = OptionGroup(parser, "Special Options", "Special options")
            for o in self.opts:
                g.add_option(o)
            parser.add_option_group(g)
            
        parser.set_defaults(verbose=True, overwrite=False)

        self.parser = parser
        
    def __init__(self, arguments, options=None):
        self.connection = None
        self.output = None
        self.sql = None
        
        self.opts = options
        self.construct_parser()
        self.options, self.args = self.parser.parse_args(args=arguments)
        
        if self.args:
            self.options.input = self.args[0]
                
        if not self.options.leafsize:
            try:
                self.options.leafsize = int(self.args[2])
            except IndexError:
                self.options.leafsize = 1000           

        if self.options.input:
            self.options.input = os.path.abspath(self.options.input)
            if os.path.isdir(self.options.input):
                raise self.parser.error("Input '%s' is a directory, not a file " % self.options.input)
            
            exists = os.path.isfile(self.options.input)
            if not exists:
                raise self.parser.error("Input file '%s' does not exist" % self.options.input)
        else:
            raise self.parser.error("No input was specified")



        if self.options.output:
            self.options.output = os.path.abspath(self.options.output)
            if os.path.isdir(self.options.output):
                raise self.parser.error("Output '%s' is a directory, not a file " % self.options.output)
            
            if os.path.exists(self.options.output):
                if not self.options.overwrite:
                    raise self.parser.error("Output file '%s' exists, but you have not selected the --overwrite option" % self.options.output)
            self.options.output = open(self.args[1],'wb')
        else:
            self.options.output = open(self.options.input+'.kdx','wb')


    def get_leaves(self, node):
        leaves = []
        if isinstance(node, spatial.KDTree.leafnode):
            leaves.append(node)
        else:
            return self.get_leaves(node.less) + self.get_leaves(node.greater)
        return leaves            

    def get_bounds(self, ids, data):
        x, y = data[ids[0]]
    
        minx, miny, maxx, maxy = x, y, x, y
    
        for id in ids:
            x,y = data[id]
            minx = min(minx, x)
            maxx = max(maxx, x)
            miny = min(miny, y)
            maxy = max(maxy, y)
    
        return (minx, miny, maxx, maxy)
    
    def write(self, i, leaf, minx, miny, maxx, maxy):
        ids = ' '.join([str(i) for i in leaf.idx])
        output = '%d %d %.8f %.8f %.8f %.8f \n%s\n' % (i, len(leaf.idx), minx, miny, maxx, maxy, ids)
        
        self.options.output.write(output)
    def process(self):
        inp = file.File(self.options.input)

        data = array(empty( (inp.header.point_records_count, 2)),dtype=float)

        i=0
        for p in inp:
            data[i] = array([p.x, p.y])
            i+=1

        t = spatial.KDTree(data,leafsize=self.options.leafsize)

        l = self.get_leaves(t.tree)
        
        i = 0
        for leaf in l:
            minx, miny, maxx, maxy = self.get_bounds(leaf.idx, data)
            self.write(i, leaf, minx, miny, maxx, maxy)
            i+=1
        self.options.output.close()
def main():
    import optparse

    options = []

    d = Translator(sys.argv[1:], options=options)
    d.process()

if __name__=='__main__':
    main()
