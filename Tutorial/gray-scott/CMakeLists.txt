cmake_minimum_required(VERSION 3.10)

include(FetchContent)
FetchContent_Declare(
  gRPC
  GIT_REPOSITORY https://github.com/grpc/grpc
  GIT_TAG        v1.28.1
)
set(FETCHCONTENT_QUIET OFF)
FetchContent_MakeAvailable(gRPC)

set(CMAKE_CXX_STANDARD 11)
# ENABLE ADIOS2_ROOT
if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

project(gray-scott C CXX)

find_package(MPI REQUIRED)
find_package(ADIOS2 REQUIRED)

option(USE_TIMERS "Use profiling timers")
if(USE_TIMERS)
  message(STATUS "Enabling profiling timers")
  add_definitions(-DENABLE_TIMERS)
endif()

# We are not using the C++ API of MPI, this will stop the compiler look for it
add_definitions(-DOMPI_SKIP_MPICXX -DMPICH_SKIP_MPICXX)

add_executable(gray-scott
  simulation/main.cpp
  simulation/gray-scott.cpp
  simulation/settings.cpp
  simulation/writer.cpp
  common/collector.pb.cc
  common/collector.grpc.pb.cc
)
target_link_libraries(gray-scott adios2::adios2 MPI::MPI_C grpc++)

add_executable(pdf_calc analysis/pdf_calc.cpp)
target_link_libraries(pdf_calc adios2::adios2 MPI::MPI_C grpc++)

option(VTK "Build VTK apps")
if (VTK_ROOT)
  set(VTK ON)
endif(VTK_ROOT)

if (VTK)
  message(STATUS "Configuring VTK apps")

  find_package(VTK COMPONENTS
    vtkFiltersCore
    vtkIOImage
    vtkIOXML
  )

  if(VTK_FOUND)
    add_executable(isosurface
      analysis/isosurface.cpp
      common/collector.pb.cc
      common/collector.grpc.pb.cc
    )
    target_link_libraries(isosurface adios2::adios2 ${VTK_LIBRARIES}
      MPI::MPI_C grpc++)
  endif(VTK_FOUND)

  find_package(VTK COMPONENTS
    vtkFiltersCore
    vtkFiltersGeometry
  )

  if(VTK_FOUND)
    add_executable(find_blobs
      analysis/find_blobs.cpp
      common/collector.pb.cc
      common/collector.grpc.pb.cc
    )
    target_link_libraries(find_blobs adios2::adios2 ${VTK_LIBRARIES}
      MPI::MPI_C grpc++)
  endif(VTK_FOUND)

  find_package(VTK COMPONENTS
    vtkFiltersGeneral
  )

  if(VTK_FOUND)
    add_executable(compute_curvature
      analysis/curvature.cpp
      common/collector.pb.cc
      common/collector.grpc.pb.cc
    )
    target_link_libraries(compute_curvature adios2::adios2 ${VTK_LIBRARIES}
      MPI::MPI_C grpc++)
  endif(VTK_FOUND)


  find_package(VTK COMPONENTS
    vtkRenderingOpenGL2
    vtkViewsInfovis
  )

  if(VTK_FOUND)
    add_executable(render_isosurface plot/render_isosurface.cpp)
    target_link_libraries(render_isosurface adios2::adios2 ${VTK_LIBRARIES}
      MPI::MPI_C)
  endif(VTK_FOUND)
endif(VTK)
