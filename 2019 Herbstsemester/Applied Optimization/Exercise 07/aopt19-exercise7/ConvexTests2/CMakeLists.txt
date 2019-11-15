get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_executable(${PROJECT_NAME}
        main.cc
        )

target_link_libraries(${PROJECT_NAME}
        AOPT::AOPT
        AOPT::MassSpringSystem
        )

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES
        )
