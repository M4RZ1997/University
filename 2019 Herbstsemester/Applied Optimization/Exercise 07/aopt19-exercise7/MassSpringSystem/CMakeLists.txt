
add_library (MassSpringSystem
        MassSpringSystem
        MassSpringSystem
        )

add_library (AOPT::MassSpringSystem ALIAS MassSpringSystem)

target_link_libraries(MassSpringSystem AOPT::AOPT)

set_target_properties(MassSpringSystem PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES
        )

target_include_directories(MassSpringSystem
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        )