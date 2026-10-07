# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "Benchmark_autogen"
  [[CMakeFiles\Benchmark_autogen.dir\AutogenUsed.txt]]
  [[CMakeFiles\Benchmark_autogen.dir\ParseCache.txt]]
  [[CMakeFiles\VectorForge_autogen.dir\AutogenUsed.txt]]
  [[CMakeFiles\VectorForge_autogen.dir\ParseCache.txt]]
  "VectorForge_autogen"
  )
endif()
