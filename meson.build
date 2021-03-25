project('ChimuAPI', 'c', 'cpp',
        default_options : ['c_std=c11', 'cpp_std=c++17', 'pic=false'],
        subproject_dir : 'third-party')

  
inc_dirs = include_directories([ 'src', 'include' ])

fmt_proj = subproject('fmt')
fmt_dep = fmt_proj.get_variable('fmt_dep')

json_proj = subproject('json')
json_dep = json_proj.get_variable('nlohmann_json_dep')

curl_dep = dependency('libcurl')

sources = [ 'src/lib.cc' ]

dependencies = [ curl_dep, fmt_dep, json_dep ]

chimu_inc_dirs = include_directories([ 'include' ])
chimu_lib = library('chimu', sources, include_directories : inc_dirs, dependencies : dependencies, install : true)
chimu_dep = declare_dependency(link_with : chimu_lib, include_directories : chimu_inc_dirs)

install_headers('include/chimu/api.hh', subdir : 'chimu')
install_headers('include/chimu/structs.hh', subdir : 'chimu')

subdir('tests')
