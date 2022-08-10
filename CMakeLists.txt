cmake_minimum_required(VERSION 3.15)
MACRO(ADD_PRECOMPILED_HEADER PrecompiledHeader PrecompiledSource SourcesVar)
    GET_FILENAME_COMPONENT(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
    SET(PrecompiledBinary "${CMAKE_CURRENT_BINARY_DIR}/${PrecompiledBasename}.pch")
    SET(Sources ${${SourcesVar}})

    SET_SOURCE_FILES_PROPERTIES(${PrecompiledSource}
            PROPERTIES COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
            OBJECT_OUTPUTS "${PrecompiledBinary}")
    SET_SOURCE_FILES_PROPERTIES(${Sources}
            PROPERTIES COMPILE_FLAGS "/Yu\"${PrecompiledHeader}\" /FI\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
            OBJECT_DEPENDS "${PrecompiledBinary}")
    # Add precompiled header to SourcesVar
    LIST(APPEND ${SourcesVar} ${PrecompiledSource})
ENDMACRO(ADD_PRECOMPILED_HEADER)


project(HZDR_Data_Acquisition)
#find_package(OpenMP REQUIRED)

link_directories("${PROJECT_SOURCE_DIR}/Libraries/LTT_API")
include_directories("${PROJECT_SOURCE_DIR}/Libraries/LTT_API")
add_subdirectory(Libraries/LTT_API)

#if (OPENMP_FOUND)
#    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} ")
#    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
#endif()



add_custom_target(LTT
        # todo: check if debug and release folder exist
        # debug version
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/Libraries/LTT_API/ltt2api30.dll ${PROJECT_SOURCE_DIR}/cmake-build-debug
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/Libraries/LTT_API/lttosal30.dll ${PROJECT_SOURCE_DIR}/cmake-build-debug)
set(CMAKE_CXX_STANDARD 20)
#ADD_PRECOMPILED_HEADER("Pch/pch.h" "Pch/pch.cpp" PRECOMPILED_HEADER)
#ADD_LIBRARY(MyLibrary PRECOMPILED_HEADER)

add_executable(HZDR_Data_Acquisition main.cpp IOs/LTT24/ltt24_whisperer.h DataStructures/Config.h
        Engine/DataEngine.h Engine/LTTEngine.h Tools/custom_algorithms.h DataStructures/Events.h Engine/CommandInterface.h
        IOs/Files/Logger.h Engine/Tasks.h Tools/Filter_factory.h IOs/TCP_IP/TCPServer.h Examples/Tomography_Controlled_Continuous_Steel_Casting.h)
set_target_properties(HZDR_Data_Acquisition PROPERTIES COMPILE_FLAGS "-m32 -O2 " LINK_FLAGS "-m32")
target_link_libraries(HZDR_Data_Acquisition wsock32 ws2_32)
add_dependencies(HZDR_Data_Acquisition LTT)
target_link_libraries(HZDR_Data_Acquisition lttosal30)
target_link_libraries(HZDR_Data_Acquisition ltt2api30)
