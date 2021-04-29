project(
  'coyotetm', 'cpp',
  version: '1.1',
  license: 'GPL3',
  default_options: ['c_std=c11', 'cpp_std=c++11', 'buildtype=debugoptimized'],
  meson_version: '>= 0.43.0'
)

coyotetm_name = meson.project_name()
coyotetm_version = meson.project_version()

coyotetm_prefix = get_option('prefix')
coyotetm_bindir = join_paths(coyotetm_prefix, get_option('bindir'))
coyotetm_libexecdir = join_paths(coyotetm_prefix, get_option('libexecdir'))
coyotetm_datadir = join_paths(coyotetm_prefix, get_option('datadir'))

cc = meson.get_compiler('cpp')

config_h = configuration_data()

application_id = 'io.github.igaldino.CoyoteTM'

# defines
set_defines = [
  # package
  ['PACKAGE', coyotetm_name],
  ['PACKAGE_BUGREPORT', 'https://github.org/igaldino/coyotetm/issues'],
  ['PACKAGE_NAME', coyotetm_name],
  ['PACKAGE_STRING', '@0@ @1@'.format(coyotetm_name, coyotetm_version)],
  ['PACKAGE_TARNAME', coyotetm_name],
  ['PACKAGE_URL', 'https://wiki.github.org/igaldino/coyotetm'],
  ['PACKAGE_VERSION', coyotetm_version],
  ['VERSION', coyotetm_version]
]

foreach define: set_defines
  config_h.set_quoted(define[0], define[1])
endforeach

# compiler flags
fltk_cflags = ''
r = run_command('fltk-config', '--cflags')
if r.returncode() == 0
  fltk_cflags = r.stdout().strip().split(' ')
endif

common_flags = [
  '-DHAVE_CONFIG_H',
  fltk_cflags
]

# linker flags
add_project_link_arguments(['-Wl,-z,relro,-z,now', '-Wl,--as-needed'], language: 'c')

coyotetm_deps = [
  cc.find_library('db_cxx'),
  cc.find_library('fltk'),
  cc.find_library('Xrender'),
  cc.find_library('Xcursor'),
  cc.find_library('Xfixes'),
  cc.find_library('Xext'),
  cc.find_library('Xft'),
  cc.find_library('fontconfig'),
  cc.find_library('Xinerama'),
  cc.find_library('pthread'),
  cc.find_library('dl'),
  cc.find_library('X11'),
  cc.find_library('m')
]

top_inc = include_directories('.')
xpm_inc = include_directories('data/xpm')

subdir('data')
subdir('src')

configure_file(
  output: 'config.h',
  configuration: config_h
)

meson.add_install_script(
  'build-aux/meson_post_install.py',
  application_id,
  coyotetm_prefix,
  coyotetm_bindir,
  coyotetm_datadir
)

output = '  Version: ' + coyotetm_version + '\n'
message(output)
