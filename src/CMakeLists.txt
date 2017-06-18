cmake_minimum_required (VERSION 2.6)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} -DBSLS_ASSERT_LEVEL_ASSERT_SAFE")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_RELEASE
    "${CMAKE_CXX_FLAGS_RELEASE} -DBSLS_ASSERT_LEVEL_ASSERT_OPT")

include_directories(".")
include_directories("ext/bde/groups/bsl/bsls")
include_directories("ext/bde/groups/bdl/bdlb")
include_directories("ext/bde/groups/bdl/bdld")
include_directories("ext/bde/groups/bdl/bdlma")
include_directories("ext/bde/groups/bdl/bdls")
include_directories("ext/bde/groups/bdl/bdldfp")
include_directories("ext/bde/groups/bdl/bdlt")
include_directories("ext/bde/groups/bdl/bdlscm")
include_directories("ext/bde/groups/bsl/bslscm")
include_directories("ext/bde/groups/bsl/bslmf")
include_directories("ext/bde/groups/bsl/bsltf")
include_directories("ext/bde/groups/bsl/bsl+bslhdrs")
include_directories("ext/bde/groups/bsl/bslstl")
include_directories("ext/bde/groups/bsl/bslalg")
include_directories("ext/bde/groups/bsl/bslh")
include_directories("ext/bde/groups/bsl/bslma")
include_directories("ext/bde/thirdparty")
include_directories("ext/bde/thirdparty/inteldfp/LIBRARY/src")

add_subdirectory(apps)
add_subdirectory(ext)
add_subdirectory(groups)
