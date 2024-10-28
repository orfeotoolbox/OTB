#
# Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

import os
from setuptools import setup, find_namespace_packages
import re

# align 
def request_gdal_version() -> str:
    try:
        r = subprocess.run(['gdal-config', '--version'], stdout=subprocess.PIPE )
        version = r.stdout.decode('utf-8').strip('\n')
        print("GDAL %s detected on the system, using 'gdal==%s'" % (version, version))
        return version
    except Exception:  # pylint: disable=broad-except
        return '3.9.0'

def normalize(name):
    return re.sub(r"[-_.]+", "-", name).lower()


BASEDIR = os.path.dirname(os.path.abspath(os.path.realpath(__file__)))
with open(os.path.join(BASEDIR, "README.md"), "r") as f:
    readme = f.read()


# all metadata are in the same file
metadata = {}
with open(os.path.join(BASEDIR, "__meta__.py"), "r") as f:
    exec(f.read(), metadata)


# All files named LICENSE* or LICENCE*, NOTICE*, all README* files
setup(
    name                          = normalize(metadata["__title__"]),
    version                       = metadata["__version__"],
    description                   = metadata["__description__"],
    long_description              = readme,
    long_description_content_type = "text/markdown",
    author                        = metadata["__author__"],
    author_email                  = metadata["__author_email__"],
    url                           = metadata["__url__"],
    license                       = metadata["__license__"],
    keywords                      = metadata["__keywords__"],

    # Liste les packages à insérer dans la distribution
    # plutôt que de le faire à la main, on utilise la fonction
    # find_packages() de setuptools qui va chercher tous les packages
    # python recursivement dans le dossier courant.
    # C'est pour cette raison que l'on a tout mis dans un seul dossier:
    # on peut ainsi utiliser cette fonction facilement
    packages=find_namespace_packages('lib', exclude=("*.tests", "*.tests.*", "tests.*", "tests", "cmake", "otb")),
    package_data={"": ["LICENSE", "NOTICE"]},
    include_package_data=True, # Take MANIFEST.in into account

    python_requires='>=3.10, <4',
    install_requires=[
        "click",
        "dask[distributed]>=2022.8.1",
        "eodag",
        "gdal=="+request_gdal_version(),
        "graphviz",
        "numpy",
        "objgraph", # leaks
        # "packaging", # version
        "pympler", # leaks
        "pyyaml>=5.1",
        # Any way to require OTB ?
    ],
    extras_require={
        "dev": [
            # "nose",
            # "tox",
            # "faker",
            # 'mock; python_version < "3.5" ',
            # "coverage",
            # "moto==1.3.6",
            # "twine",
            "wheel",
            "flake8",
            "mypy",
            "pre-commit",
            "pytest < 8.1",    # b/c pytest-bdd is old -> https://github.com/pytest-dev/pytest-bdd/issues/673
            "pytest-bdd < 6",  # Using "example table" feature, removed from v6
            #                    https://pytest-bdd.readthedocs.io/en/latest/#migration-from-5-x-x
            "pytest-check",
            "pylint",
        ],
        "docs": [
            "docutils<0.19.0", # reminder of sphinx_rtd_theme 1.3.0
            "jinja2",
            "m2r2",
            "natsort",
            "nbsphinx==0.9.3",
            "nbsphinx-link==1.3.0",
            "sphinx~=7.1",
            "sphinx_rtd_theme~=1.3.0",
            "sphinx-carousel",
        ],
    },

    classifiers=metadata["__classifiers__"],
    project_urls={
        "Bug Tracker": metadata["__bug_tracker__"],
        "Documentation": metadata["__documentation__"],
        "Source Code": metadata["__source_code__"],
        "Community": metadata["__community__"],
    },

    #scripts = ['s1tiling/S1Processor.py'],
)
