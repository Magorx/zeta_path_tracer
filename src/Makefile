CUR_PROG = pather

ifndef VERBOSE
.SILENT:
endif

GENERAL_PREFIX = general
GC = $(GENERAL_PREFIX)/c
GCPP = $(GENERAL_PREFIX)/cpp

CC = gcc
CPP = g++

WARNINGS = -Wall -Wextra -Wno-multichar -Wno-unused-result
STANDARD = -std=c++17
CFLAGS = $(STANDARD) $(WARNINGS) -lm -O3 -D DEBUG="if(0)"

all: $(CUR_PROG)

run: all
	./$(CUR_PROG) -k 4

convert:
	convert image.ppm image.png

_view:
	eog -f image.png

view: run convert _view

pather: main.cpp ray.o vec3d.o color.o camera.o hit_record.o collection_instances.o collection_textures.o collection_materials.o collection_hittables.o collection_lights.o hittable.o path_tracer.o hittable_list.o aabb.o bvh.o material.o light.o render_task.o
	$(CPP) $(CFLAGS) main.cpp vec3d.o ray.o color.o camera.o collection_instances.o collection_textures.o hit_record.o collection_materials.o collection_hittables.o collection_lights.o hittable.o path_tracer.o hittable_list.o aabb.o bvh.o material.o light.o render_task.o -pthread -o pather

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

collection_lights.o: collection_lights.cpp collection_lights.h
	$(CPP) $(CFLAGS) collection_lights.cpp -c

collection_hittables.o: collection_hittables.cpp collection_hittables.h
	$(CPP) $(CFLAGS) collection_hittables.cpp -c

collection_instances.o: collection_instances.cpp collection_instances.h
	$(CPP) $(CFLAGS) collection_instances.cpp -c

hittable.o: hittable.cpp hittable.h
	$(CPP) $(CFLAGS) hittable.cpp -c

material.o: material.cpp material.h
	$(CPP) $(CFLAGS) material.cpp -c

light.o: light.cpp light.h
	$(CPP) $(CFLAGS) light.cpp -c

path_tracer.o: path_tracer.cpp path_tracer.h
	$(CPP) $(CFLAGS) path_tracer.cpp -c

hittable_list.o: hittable_list.cpp hittable_list.h
	$(CPP) $(CFLAGS) hittable_list.cpp -c

collection_textures.o: collection_textures.cpp collection_textures.h
	$(CPP) $(CFLAGS) collection_textures.cpp -c

aabb.o: aabb.cpp aabb.h
	$(CPP) $(CFLAGS) aabb.cpp -c

bvh.o: bvh.cpp bvh.h
	$(CPP) $(CFLAGS) bvh.cpp -c

render_task.o: render_task.cpp render_task.h
	$(CPP) $(CFLAGS) render_task.cpp -c

clear:
	rm *.o
