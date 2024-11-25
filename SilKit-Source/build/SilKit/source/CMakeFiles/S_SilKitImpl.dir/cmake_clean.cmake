file(REMOVE_RECURSE
  "libS_SilKitImpl.a"
  "libS_SilKitImpl.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/S_SilKitImpl.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
