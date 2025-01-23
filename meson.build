project('ips', 'c', meson_version: '>1.1', version: '0.1.0')

cc = meson.get_compiler('c')

ipsd_port = get_option('ipsd_port')
ipsd_enable_python = get_option('ipsd_enable_python')

ipsd_py_venv = get_option('ipsd_py_venv')
ipsd_py_venv_isolated = get_option('ipsd_py_venv_isolated')
ipsd_py_venv_site = get_option('ipsd_py_venv_site')

if ipsd_enable_python
  python = import('python').find_installation('python3.12')
  python_dep = python.dependency(embed: true)
endif

libmicrohttpd_proj = subproject('libmicrohttpd')
libmicrohttpd_dep = libmicrohttpd_proj.get_variable('libmicrohttpd_dep')

subdir('aux')
subdir('ipsd')
