#!python.exe
# EASY-INSTALL-ENTRY-SCRIPT: 'nrfutil==2.3.0','console_scripts','nrfutil'
__requires__ = 'nrfutil==2.3.0'

import sys
from pkg_resources import load_entry_point

sys.exit(
   load_entry_point('nrfutil==2.3.0', 'console_scripts', 'nrfutil')()
)
