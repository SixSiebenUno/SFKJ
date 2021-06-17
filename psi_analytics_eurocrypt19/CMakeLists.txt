find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)

find_package(Boost COMPONENTS program_options REQUIRED)

if (NOT Boost_FOUND)
    message(FATAL_ERROR "Boost library not found.  Rerun cmake with -DCMAKE_PREFIX_PATH=\"<path to lib1>;<path to lib2>\"")
endif ()

find_library(NTL_LIB ntl)
if (NOT NTL_LIB)
    message(FATAL_ERROR "ntl library not found.  Rerun cmake with -DCMAKE_PREFIX_PATH=\"<path to lib1>;<path to lib2>\"")
endif ()

find_library(GMP_LIB gmp)
if(NOT GMP_LIB)
  message(FATAL_ERROR "gmp library not found.")
endif()

find_library(GMPXX_LIB gmpxx)
if(NOT GMP_LIB)
  message(FATAL_ERROR "gmpxx library not found.")
endif()

add_library(psi_analytics_eurocrypt19
        common/psi_analytics.cpp
        common/helpers.cpp
        polynomials/Mersenne.cpp
        polynomials/Poly.cpp
        ots/ots.cpp
        PRNG/PRNG.cpp
        PermNet/PermutationNetwork.cpp
        PermNet/WaksmanPermutation.cpp
        OEP/OEP.cpp
        join/join.cpp
        join/twoserverjoin.cpp
        utils/multicom.cpp
        Purify/PurificationCircuit.cpp
        sort/sort.cpp
        )
        
set_target_properties(psi_analytics_eurocrypt19
        PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        )

target_compile_options(psi_analytics_eurocrypt19
        PRIVATE
        -march=native
        -ffunction-sections -mpclmul -mbmi2 -maes
        -mavx -msse2 -msse3 -msse4.1
        -Wall -Wno-strict-overflow -Wno-ignored-attributes -Wno-parentheses)

target_link_libraries(psi_analytics_eurocrypt19 INTERFACE
        ABY::aby
        ENCRYPTO_utils::encrypto_utils
        ${GLIB_LDFLAGS}
        ${NTL_LIB}
        ${GMP_LIB}
        ${GMPXX_LIB}
        HashingTables
        libOTe
        Boost::program_options
        Threads::Threads
        # relic is a transitive dependency of libOTe right now
        # change relic to another library (if needed) if this changes
        relic_s
        )

target_include_directories(psi_analytics_eurocrypt19 PUBLIC
        ${PSI_ANALYTICS_SOURCE_ROOT}/psi_analytics_eurocrypt19
        ${PSI_ANALYTICS_SOURCE_ROOT}/extern
        ${PSI_ANALYTICS_SOURCE_ROOT}/extern/libOTe/cryptoTools
        ${PSI_ANALYTICS_SOURCE_ROOT}/extern/libOTe
        ${PSI_ANALYTICS_SOURCE_ROOT}/extern/ABY/src
        ${PSI_ANALYTICS_SOURCE_ROOT}/extern/ABY/extern/ENCRYPTO_utils/src
        ${PSI_ANALYTICS_SOURCE_ROOT}/extern/HashingTables
        ${GLIB_INCLUDE_DIRS}
        )

if (DEMO)
    add_executable(demo demo.cpp)
    target_link_libraries(demo PUBLIC
            psi_analytics_eurocrypt19
            )
    set_target_properties(demo
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
    add_executable(q5 tpchq5demo.cpp)
    target_link_libraries(q5 PUBLIC
            psi_analytics_eurocrypt19
            )
    set_target_properties(q5
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
    add_executable(linear lineardemo.cpp)
    target_link_libraries(linear PUBLIC
            psi_analytics_eurocrypt19
            )
    set_target_properties(linear
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif (DEMO)

if (MULDEMO)
    add_executable(muldemo multidemo.cpp)
    target_link_libraries(muldemo PUBLIC
            psi_analytics_eurocrypt19
            )
    set_target_properties(muldemo
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif (MULDEMO)

if (PSI_FUNCTION)
    add_executable(psi psi.cpp)

    target_link_libraries(psi PUBLIC
            psi_analytics_eurocrypt19
            )
    set_target_properties(psi
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif (PSI_FUNCTION)

if (PSI_ANALYTICS_BUILD_EXAMPLE)
    add_executable(psi_analytics_eurocrypt19_example psi_analytics_example.cpp)

    target_link_libraries(psi_analytics_eurocrypt19_example PUBLIC
            psi_analytics_eurocrypt19
            )
    set_target_properties(psi_analytics_eurocrypt19_example
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif (PSI_ANALYTICS_BUILD_EXAMPLE)

if (OEP_FUNCTION)
    add_executable(oep testpermnet.cpp)
    target_link_libraries(oep PUBLIC psi_analytics_eurocrypt19)
    set_target_properties(oep
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif (OEP_FUNCTION)
