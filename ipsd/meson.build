if ipsd_py_venv != '' and ipsd_py_venv_site == ''
  error('`ipsd_venv_site` cannot be undefined if `ipsd_venv` is set.')
endif

ipsd_src = ['support/python.c', 'articulate.c', 'dispatch.c', 'ipsd.c', 'seatbelt.c']

ipsd_deps = [aux_dep, libmicrohttpd_dep]

if ipsd_enable_python
  ipsd_deps += python_dep
endif

ipsd_c_args = [
  '-DIPSD_PORT=' + ipsd_port.to_string(),
  '-DIPSD_PY_VENV="' + ipsd_py_venv + '"',
  '-DIPSD_PY_VENV_ISOLATED=' + ipsd_py_venv_isolated.to_string(),
  '-DISPD_PY_VENV_SITE="' + ipsd_py_venv_site + '"',
]

executable(
  'ipsd',
  ipsd_src,
  dependencies: ipsd_deps,
  c_args: ipsd_c_args,
)
