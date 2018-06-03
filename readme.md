## Makina ##
Makina is a ”not-game engine (NGE)” which provides a subset of common game engine features such as audio, input, physics, rendering, scripting but in contrary to most game engines, is easily extendable with prototypical computer graphics, scientific visualization and virtual reality research. It essentially is a collection of high-quality (or ambitious) libraries weaved together into a cohesive framework.

## Intent ##
- As a computer graphics scientist I want to load a standard test scene (e.g. Sponza) and apply/extend raytracing/rendering methods on it.
  - I load the test scene as a model.
  - I append the model to the default scene. 
  - It is rendered with the default raytracing pipeline.
  - I can then extend the rendering pipeline with custom render tasks or even completely replace it to my liking. [insert_research_here]
  - Majority of research is done in render tasks.

- As a visualization scientist I want to load various custom combustion/medical/... datasets and visualize them as line segments/meshes/point clouds/volumes/...
  - I load the datasets as line segments/meshes/point clouds/volumes/...
    - If necessary, I implement the appropriate loaders, or even complete new resources and their related components here.
  	- I then filter the datasets as necessary - e.g. run particle tracing on a vector field to generate line segments, sample spherical harmonic coefficients to generate meshes, [insert_research_here].
  - I create an entity with a line_render/mesh_render/point_render/volume_render/... component which points to the dataset.
  - I add the entity to the default scene.
  - It is rendered with the default raytracing pipeline, which is often good enough for me.
  - Majority of research is done in components or externally - e.g. in a particle tracer.

- As a virtual reality scientist I want to load custom scenes, including animated humanoids, and navigate/interact within/with them.
  - I load the custom scene and the humanoids as models.
    - If necessary, I implement the appropriate resources/loaders for importing animated meshes from a custom virtual human library.
  - I append the models to the default scene.
  - If necessary, I replace the default VR controller interaction. [insert_research_here]
  - If necessary, I customize the UI as I like or even add new UI approaches. [insert research here]
  - If necessary, I add custom components for assigning user study tasks and collecting feedback from the user. [insert research here]
  - It is rendered with the default PBR shading pipeline, including appropriate physics, which is often good enough for me.
  - Majority of research is done in components.

- As a simulation scientist / applied mathematician, I want to visualize iterative, stochastic algorithms in-situ (e.g. Markov Chain Monte Carlo, Variational Inference).
  - I fire up an external library to crunch numbers and obtain the output stream. [insert research here]
  - I create an entity and attach a behavior to it which gives me access to immediate mode rendering, i.e. rendering without any shaders as in OpenGL 1/2.
    - I convert the stream of numbers into geometry in a lambda passed to the behavior, which is run every frame.
  - I add the entity to the default scene.
  - It is rendered with the default immediate mode pipeline, which is often good enough for me.
  - Majority of research is done externally. The library is just for visualization / results.

## Libraries and Tech ##
- Build
  - [x] CMake
  - [x] Conan
- Core
  - [x] acdemiralp/ec
  - [x] Boost
  - [x] foonathan/memory
  - [x] gabime/spdlog
  - [x] rttrorg/rttr
- Display and Input
  - [x] acdemiralp/di (OpenVR + SDL2)
- Assets
  - [x] acdemiralp/fi (FreeImage)
  - [x] acdemiralp/ra
  - [x] Assimp
  - [x] bgrimstad/splinter
  - [x] Eigen
  - [x] FreeType
  - [x] GLM
  - [ ] LibVLC
  - [x] nlohmann/json
- Compute
  - [x] 01org/tbb
  - [x] Cuda
- Rendering
  - [x] acdemiralp/fg
  - [x] acdemiralp/gl (GLEW + OpenGL)
  - [x] bkaradzic/bgfx (DX9-12 + Metal + OpenGL + Vulkan)
  - [ ] DX12
  - [x] OptiX
  - [x] ospray/ospray
  - [x] VkHLF (Vulkan)
- Audio
  - [x] FMOD
- Physics
  - [x] Bullet
- Networking
  - [ ] RakNet
  - [x] ZeroMQ
- Scripting
  - [ ] Mono
  - [ ] V8
- UI
  - [x] ocornut/imgui
- Profiling
  - [x] milostosic/MTuner
- Benchmarking
  - [x] acdemiralp/bm
- Testing
  - [x] philsquared/catch2

## Layers (In-Progress) ##
- Components: `animator, audio_listener, audio_source, camera, collider, cs_script, image_renderer, js_script, light, mesh_renderer, nurbs_renderer, rigidbody, scalar/vector/tensor_field_renderer, text_renderer, transform, ui_canvas, ui_element...`
- Systems: `audio_system, interpreters (cs_interpreter, js_interpreter), networking_system, physics_system, renderers (dx12_renderer, gl_renderer, optix_renderer, ospray_renderer, vk_renderer), ui_system`
- Serialization: 
  - Assets: `audio_clip, font, image, model (animation_clip, curve, keyframe, material, mesh, transform), nurbs, physics_material, script, shader, video_clip`
  - Natives: Components, prefabs (partial ec-tables), scenes (complete ec-tables), systems.
- Rendering:
  - JoeyDeVries/LearnOpenGL (/pbr)
  - nvpro-samples/gl_dynamic_lod
  - nvpro-samples/gl_occlusion_culling
  - nvpro-samples/gl_optix_composite
  - nvpro-samples/gl_ssao