aux_inc = include_directories('include')
aux_lib = library(
  'aux',
  'memory/libc.c',
  'memory/memory.c',
  'memory/region.c',
  'memory/system.c',
  'complain.c',
  'entry.c',
  include_directories: aux_inc,
)
aux_dep = declare_dependency(link_with: [aux_lib], include_directories: [aux_inc])
