#!/usr/bin/env python3

import glob
import os
import shutil
import subprocess
import sys

if not os.environ.get('DESTDIR'):
  application_id = sys.argv[1]
  prefix = sys.argv[2]
  bindir = sys.argv[3]
  datadir = sys.argv[4]

  icondir = os.path.join(datadir, 'icons', 'hicolor')
  print('Update icon cache...')
  subprocess.call(['gtk-update-icon-cache', '-f', '-t', icondir])

  desktop_file = os.path.join(datadir, 'applications', application_id + '.desktop')
  print('Validate desktop file...')
  subprocess.call(['desktop-file-validate', desktop_file])

  print('Remove unneeded files...')
  for fl in glob.glob(bindir + '/fl*'):
    os.remove(fl)
  for fl in glob.glob(bindir + '/db*'):
    os.remove(fl)
  try:
    shutil.rmtree(prefix + '/docs')
  except:
    pass
  try:
    shutil.rmtree(prefix + '/include')
  except:
    pass
  try:
    shutil.rmtree(datadir + '/doc')
  except:
    pass
  try:
    shutil.rmtree(datadir + '/man')
  except:
    pass

