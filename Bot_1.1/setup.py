import os, sys

from distutils.core import setup, Extension
from distutils import sysconfig

#, '-mmacosx-version-min=10.7'
os.environ["CC"] = "g++-4.7"
os.environ["CXX"] = "g++-4.7"
cpp_args = ['-std=c++17', '-stdlib=libc++']

ext_modules = [
    Extension(
    'PlayerCoreBase',
        ['core.cpp', 'generator.cpp', 'wrap.cpp'],
        include_dirs=['pybind11/include', 'C:/Users/carlo/AppData/Local/Programs/Python/Python311/include', 'G:/Mi unidad/Up/TTC Crusher/Bot_1.0'],
    language='c++',
    extra_compile_args = cpp_args,
    ),
]

setup(
    name='PlayerCoreBase',
    version='0.0.1',
    author='your mom lol',
    author_email='your_mom69@hotmail.com',
    description='Random player maybe?',
    ext_modules=ext_modules,
)