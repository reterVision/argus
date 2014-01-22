from distutils.core import setup, Extension

argus = [Extension(
    'argus',
    sources=['argus.c'],
    depends=['argus.h'])]

setup(name='argus',
      version='0.1',
      description='A monitoring system.',
      ext_modules=argus)
