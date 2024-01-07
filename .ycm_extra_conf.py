
def FlagsForFile(filename, **kwargs):
    return {
        'flags': [
            '-Wall',
            '-D_PLATFORM_DESKTOP',
            '-lraylib', '-lm'
        ],
    }
