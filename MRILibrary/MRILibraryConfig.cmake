add_library(MRILibrary STATIC IMPORTED)
find_library(MRILibrary_LIBRARY_PATH MRILibrary HINTS "${CMAKE_CURRENT_LIST_DIR}/../../")
set_target_properties(MRILibrary PROPERTIES IMPORTED_LOCATION "${MRILibrary_LIBRARY_PATH}")
