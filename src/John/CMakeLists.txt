cmake_minimum_required(VERSION 2.8)
project(covid19sarov)
set(SOURCE_LIB
				Person.h
				Person.cpp
				PersonShedule.h
				PersonShedule.cpp
				Location.h
				Location.cpp
				LocationShedule.h
				LocationShedule.cpp
				Generator.h
				Generator.cpp
				Model.h
				Model.cpp
				SEIR_model.h
				data_types.h
				lib_migration.h
				lib_migration.cpp
				translate_data.h
				translate_data.cpp
				)

add_library(covid19sarov_lib STATIC ${SOURCE_LIB})

set(SOURCE_EXE main.cpp)
set(MIGRATION_EXE main_migration_example.cpp)

add_executable(covid19sarov ${SOURCE_EXE})
#add_executable(migration_main ${MIGRATION_EXE})

target_link_libraries(covid19sarov covid19sarov_lib)
#target_link_libraries(migration_main covid19sarov_lib)
