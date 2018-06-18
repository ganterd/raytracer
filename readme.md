# Simple BVH Raytracer

Pretty simple raytracer using C++ on CPU (no GPU acceleration). Made so I could better understand BVH construction and traversal. Not massively optimised at all. Possible optimisations outlined below.

## Building
Should just be a simple cmake generate and build process. Libraries used:
* glm (Bundled as git submodule)
* Assimp (Bundled as git submodule)
* SFML (Cmake should pull this)

Should build correctly as so:
```
mkdir build
cd build
cmake ../
make
./rt_app_minimal_sfml/rt_app_minimal_sfml [path_to_scene_file]
```

## Parameters
The following parameters are optional:
``` 
Buffer Size:
   -bsx [buffer_x_dimensions]
   -bsy [buffer_y_dimensions]

Render Regions (Buffer Subdivision):
   -rsx [region_x_dimensions]
   -rsy [region_y_dimensions]

Draw box around current render region:
   -ol OR --outline

Threads:
   -t [num_render_threads]

Indirect Lighting:
   -b [num_ray_bounces]
   -bs [num_tertiary_rays]
   
Quit after rendering frame:
   -q OR --quit
```

Example: To render Sponza to a 1920x1080 buffer, with 16 render threads, visualising what region each thread is working on.

``` 
./rt_app_minimal_sfml/rt_app_minimal_sfml ../scenes/test_sponza.blend -bsx 1920 -bsy 1080 -t 16 -ol
```

Example: To redner Suzanne heads with 2 ray bounces with 64 tertiary rays per bounce.
```
./rt_app_minimal_sfml/rt_app_minimal_sfml ../scenes/test.blend -b 2 -bs 64
```

## Possible Optimisations
* Re-arranging BVH Ray-AABB intersection tests to test 4 or 8 AABBs at a time using SSE/AVX. Example of this can be seen in the Blender source code.
* Using tight-wrapping bounding boxes rather than AABBs.
