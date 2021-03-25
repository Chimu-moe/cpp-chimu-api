gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_main_dep')
gmock_dep = gtest_proj.get_variable('gmock_dep')

sources = [ 'src/get_set.cc', 'src/get_map.cc', 'src/search.cc', 'src/download.cc' ]

dependencies = [
    gtest_dep,
    gmock_dep,
    chimu_dep
]

e = executable('TestLibChimuAPI', sources, dependencies : dependencies)
test('ChimuAPI GTests', e)
