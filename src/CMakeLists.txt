set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(fort STATIC ${CMAKE_CURRENT_SOURCE_DIR}/fort.c)

add_executable(
  ${CMAKE_PROJECT_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/CountSort.txt
  ${CMAKE_CURRENT_SOURCE_DIR}/BubbleSort.txt
  ${CMAKE_CURRENT_SOURCE_DIR}/InsertionSort.txt)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE project_options
                                                    project_warnings fort)
