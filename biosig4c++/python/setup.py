try:
    from setuptools import setup
    from setuptools import Extension
except ImportError:
    from distutils.core import setup
    from distutils.extension import Extension

import numpy.distutils.misc_util as mu
try:
    import pkgconfig
    PKG=pkgconfig.parse('libbiosig')
    CPATH=PKG['include_dirs']
    LIBS=PKG['libraries']
    LIBDIR=PKG['library_dirs']
except ValueError:
    print('cannot load pkgconfig(libbiosig) - use default location')
    CPATH='/usr/local/include'
    LIBS='-lbiosig'
    LIBDIR='/usr/local/lib'

module1 = Extension('biosig',
        define_macros = [('MAJOR_VERSION', '1'), ('MINOR_VERSION', '9')],
        include_dirs = [CPATH, mu.get_numpy_include_dirs()[0]],
        libraries    = LIBS,
        library_dirs = LIBDIR,
        sources      = ['biosigmodule.c'])

setup (name = 'Biosig',
       version = '1.9',
       description = 'This is a biosig package',
       author = 'Alois Schloegl',
       author_email = 'alois.schloegl@ist.ac.at',
       license = 'GPL',
       url = 'http://biosig.sf.net',
       long_description = '''This is the biosig demo package.''',
       keywords = 'EEG ECG EKG EMG EOG Polysomnography ECoG biomedical signals SCP EDF GDF HEKA CFS ABF',
       ext_modules = [module1])

