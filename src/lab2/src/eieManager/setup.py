#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright (C) 2015-2022 Hewlett Packard Enterprise Development LP
# Copyright (C) 2022 Universidad de Costa Rica (UCR)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

from pathlib import Path

from setuptools import setup, find_packages


def check_directory():
    """
    You must always change directory to the parent of this file before
    executing the setup.py script. setuptools will fail reading files,
    including and excluding files from the MANIFEST.in, defining the library
    path, etc, if not.
    """
    from os import chdir

    here = Path(__file__).parent.resolve()
    if Path.cwd().resolve() != here:
        print('Changing path to {}'.format(here))
        chdir(str(here))


check_directory()


def read(filename):
    """
    Read a file relative to setup.py location.
    """
    import os
    here = os.path.dirname(os.path.abspath(__file__))
    with open(os.path.join(here, filename)) as fd:
        return fd.read()


def find_version(filename):
    """
    Find package version in file.
    """
    import re
    content = read(filename)
    version_match = re.search(
        r"^__version__ = ['\"]([^'\"]*)['\"]", content, re.M
    )
    if version_match:
        return version_match.group(1)
    raise RuntimeError('Unable to find version string.')


def find_requirements(filename):
    """
    Find requirements in file.
    """
    import string
    content = read(filename)
    requirements = []
    for line in content.splitlines():
        line = line.strip()
        if line and line[:1] in string.ascii_letters:
            requirements.append(line)
    return requirements


setup(
    name='eieManager-zetavB',
    version=find_version('lib/eieManager/__init__.py'),
    package_dir={'': 'lib'},
    packages=find_packages('lib'),
    include_package_data = True,

    # Dependencies
    install_requires=find_requirements('requirements.txt'),

    # Metadata
    author='Jordi Louzao von Breymann',
    author_email='jordi.louzao@ucr.ac.cr',
    description=(
        'A python package for the '
        'IE0417 course @ EIE, UCR'
    ),
    long_description=read('README.md'),
    long_description_content_type='text/x-rst',
    url='http://ie0417-dev-b53993.rtfd.org/',

    classifiers=[
        'Intended Audience :: Developers',
        'License :: OSI Approved :: Apache Software License',
        'Natural Language :: English',
        'Operating System :: POSIX :: Linux',
        'Programming Language :: Python :: 3',
    ],

    # Entry points
    entry_points={
        'console_scripts': [
            'eieMgr=eieManager.main:main',
        ],
    }
)
