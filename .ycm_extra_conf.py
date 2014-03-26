flags = [
'-Wall',
'-Wextra',
'-Werror',
'-Wno-long-long',
'-Wno-variadic-macros',
'-fexceptions',
'-I/usr/include/',
'-std=c++11',
'-x',
'c++',
]

def FlagsForFile(filename, **kwargs):
    return {'flags': flags, 'do_cache': True}
