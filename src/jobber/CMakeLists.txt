aux_source_directory(. JOBBER_SOURCES)
aux_source_directory(./splitter JOBBER_SOURCES)
aux_source_directory(./storage JOBBER_SOURCES)
aux_source_directory(./extern JOBBER_SOURCES)

add_library(jobber ${JOBBER_SOURCES})

target_include_directories(jobber PUBLIC ..)
