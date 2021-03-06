function(packages_finder packages include_dirs libraries library_dirs)
    set(_include_dirs)
    set(_libraries)
    set(_library_dirs)
    foreach(package ${packages})
        set(package_components ${${package}_components})
        if(package_components)
            find_package(${package} REQUIRED COMPONENTS ${package_components})
        else()
            find_package(${package} REQUIRED)
        endif()

        set(_include_dirs ${_include_dirs} ${${package}_INCLUDE_DIRS})
        set(_libraries ${_libraries} ${${package}_LIBRARIES})
        set(_library_dirs ${_library_dirs} ${${package}_LIBRARY_DIRS})
    endforeach()

    set(${include_dirs} ${_include_dirs} PARENT_SCOPE)
    set(${libraries} ${_libraries} PARENT_SCOPE)
    set(${library_dirs} ${_library_dirs} PARENT_SCOPE)
endfunction()
