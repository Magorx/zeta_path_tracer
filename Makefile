CUR_PROG = pather

ifndef VERBOSE
.SILENT:
endif

GENERAL_PREFIX = general
GC = $(GENERAL_PREFIX)/c
GCPP = $(GENERAL_PREFIX)/cpp

CC = gcc
CPP = g++

WARNINGS = -Wall -Wextra -Wno-multichar
STANDARD = -std=c++17
CFLAGS = $(STANDARD) $(WARNINGS) -lm -O3

all: $(CUR_PROG)

run: all
	./$(CUR_PROG) > image.ppm

view: run
	convert image.ppm image.png
	eog -f image.png

pather: main.cpp ray.o vec3d.o color.o camera.o hit_record.o collection_materials.o collection_hittables.o hittable.o path_tracer.o hittable_list.o
	$(CPP) $(CFLAGS) main.cpp vec3d.o ray.o color.o camera.o hit_record.o collection_materials.o collection_hittables.o hittable.o path_tracer.o hittable_list.o -o pather

ray.o: ray.cpp ray.h
	$(CPP) $(CFLAGS) ray.cpp -c

vec3d.o: vec3d.cpp vec3d.h
	$(CPP) $(CFLAGS) vec3d.cpp -c

color.o: color.cpp color.h
	$(CPP) $(CFLAGS) color.cpp -c

camera.o: camera.cpp camera.h
	$(CPP) $(CFLAGS) camera.cpp -c

hit_record.o: hit_record.cpp hit_record.h
	$(CPP) $(CFLAGS) hit_record.cpp -c

collection_materials.o: collection_materials.cpp collection_materials.h
	$(CPP) $(CFLAGS) collection_materials.cpp -c

collection_hittables.o: collection_hittables.cpp collection_hittables.h
	$(CPP) $(CFLAGS) collection_hittables.cpp -c

hittable.o: hittable.cpp hittable.h
	$(CPP) $(CFLAGS) hittable.cpp -c

path_tracer.o: path_tracer.cpp path_tracer.h
	$(CPP) $(CFLAGS) path_tracer.cpp -c

hittable_list.o: hittable_list.cpp hittable_list.h
	$(CPP) $(CFLAGS) hittable_list.cpp -c

clear:
	rm *.o
