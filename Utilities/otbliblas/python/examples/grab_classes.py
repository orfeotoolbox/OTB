from liblas import file
from liblas import point
import sys
import getopt
def extract_classifications(infile, outfile, classifications):
    inp = file.File(infile, mode='r')
    header = inp.header
    output = file.File(outfile, mode='w', header=header)
    cnt = 0
    for p in inp:
        cls = p.classification
        if int(cls) in classifications:
            output.write(p)
            cnt += 1 # keep a new count
    inp.close()
    output.close()
    del inp
    del output
    # overwrite our header with our point count
    header.point_records_count = cnt
    output = file.File(outfile, mode='w+', header=header)
    output.close()
    del output
    print 'wrote %d points' % cnt
def usage():
    print "grab_classes.py"
    print "\n"
    print "--input input.las - Input file name"
    print "--output output.las - Output file name"
    print "--classifications 0,1,2,11 - The classifications to keep "
    sys.exit(2)
def main():
    infile = None
    outfile = None
    classifications = None
    try:
        opts, args = getopt.getopt(sys.argv[1:],
                                    "i:o:c:",
                                    ["input=","output=","classifications="])
    except getopt.GetoptError, err:
        print str(err)
        usage()
        sys.exit(2)
    for o, a in opts:
        print o, a
        if o in ["-i", "--input"]:
            infile = a
        if o in ["-o", "--output"]:
            outfile = a
        if o in ["-c", "--classifications"]:
            try:
                classifications = [int(i) for i in a.split(',')]
            except:
                classifications = int(a)
    if not infile or not outfile:
        usage()
    print 'Input: ', infile
    print 'Output: ', outfile
    print 'Classifications to fetch: ', classifications
    extract_classifications(infile, outfile, classifications)
if __name__ == "__main__":
    main()
