import os, sys

from distutils.core import setup, Extension
from distutils import sysconfig

#, '-mmacosx-version-min=10.7'
cpp_args = ['-std=c++11', '-stdlib=libc++']

ext_modules = [
    Extension(
    'Player',
        ['player_test.cpp', 'wrapper.cpp'],
        include_dirs=['pybind11/include', 'C:/Users/carlo/AppData/Local/Programs/Python/Python311/include'],
    language='c++',
    extra_compile_args = cpp_args,
    ),
]

setup(
    name='player',
    version='0.0.1',
    author='your mom',
    author_email='your_mom69@hotmail.com',
    description='test player module',
    ext_modules=ext_modules,
)