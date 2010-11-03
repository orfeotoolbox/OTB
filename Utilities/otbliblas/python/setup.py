"""
$Id$

"""
try:
    from setuptools import setup, Extension
except ImportError:
    from distutils.core import setup, Extension
    
from sys import version_info

try:
    readme = file('../README','rb').read()
except:
    readme = ''

install_requires = ['setuptools']

import os

version = '1.6.0b1'

if os.name == 'nt':
    # Windows NT library
    lib_name = 'liblas1.dll' 
    data_files=[('DLLs', ['DLLs/%s'% lib_name, 'c:\\osgeo4w\\bin\\geotiff.dll','c:\\osgeo4w\\bin\\libtiff.dll','c:\\osgeo4w\\bin\\zlib_osgeo.dll','c:\\osgeo4w\\bin\\jpeg_osgeo.dll']),]
else:
    data_files = None

scripts =['scripts/oci2las.py','scripts/laskdtree.py','scripts/colorize.py']
        
setup(name          = 'libLAS',
      version       = version,
      description   = 'LAS 1.0/1.1/1.2 LiDAR data format translation',
      license       = 'BSD',
      keywords      = 'DEM elevation LIDAR',
      author        = 'Howard Butler',
      author_email  = 'hobu.inc@gmail.com',
      maintainer    = 'Howard Butler',
      maintainer_email  = 'hobu.inc@gmail.com',
      url   = 'http://liblas.org',
      data_files = data_files,
      long_description = readme,
      packages      = ['liblas'],
      install_requires = install_requires,
      test_suite = 'tests.test_suite',
      classifiers   = [
        'Development Status :: 5 - Production/Stable',
        'Intended Audience :: Developers',
        'Intended Audience :: Science/Research',
        'License :: OSI Approved :: BSD License',
        'Operating System :: OS Independent',
        'Programming Language :: Python',
        'Topic :: Scientific/Engineering :: GIS',
        ],
      scripts = scripts
)

