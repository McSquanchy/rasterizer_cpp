# Path Tracer - Documentation

By Kevin Buman

## Setup

1) Rename `CMakeListsCopy.txt` to `CMakeLists.txt`. 
2) Replace all `<path_to>` in `CMakeLists.txt`.
3) Generate your project files.
4) In order to run the program you have to set the working directory (i.e. where the program is run from) to the root directory.

Developed with Mingw on Windows.

Libraries used:

* GLM
* SDL2

## Configuration Options

```c++
// main.cc
BasicRenderer<sRGB, CustomResolution> renderer({480,480}); // Basic Renderer (single intersection) with diffuse colors only
MonteCarloRenderer<sRGB, CustomResolution> renderer({480,480}); // Renderer in sRGB Mode with a custom resolution of 480x480
MonteCarloRenderer<sRGB, HD> renderer; // Renderer in sRGB Mode with a resolution of 1280x720
renderer.SetSampling(1); 			// Set sampling quality (1 = render every pixel, 2 = render 2x2 grids, etc.)
renderer.SetRenderQuality(4096); 	// Set rays per pixel
renderer.SetAASigma(0.5f);			// Set sigma of the normal distribution, default 0.5f
renderer.SetAA(true);				// enable anti aliasing
```

## Lab 01

* Two m_diffuse_color modes, which are applied at the end of the render process. Before that, I only work on floating point rgb

```c++
void RGB::Process(glm::vec3 &base_color) const {
  base_color[0] = glm::clamp(base_color[0], 0.0f, 1.0f) * 255;
  base_color[1] = glm::clamp(base_color[1], 0.0f, 1.0f) * 255;
  base_color[2] = glm::clamp(base_color[2], 0.0f, 1.0f) * 255;
}

void sRGB::Process(glm::vec3 &base_color) const {
  base_color[0] = glm::pow(glm::clamp(base_color[0], 0.0f, 1.0f), (1 / m_gamma)) * 255;
  base_color[1] = glm::pow(glm::clamp(base_color[1], 0.0f, 1.0f), (1 / m_gamma)) * 255;
  base_color[2] = glm::pow(glm::clamp(base_color[2], 0.0f, 1.0f), (1 / m_gamma)) * 255;
}
```

## Screenshots

## Lab 02

![mc_1rpp_1sampling_1635020069](renders\mc_1rpp_1sampling_1635020069.bmp)

* Basic path tracer with diffuse and emmissive colors

## Lab 03

![mc_32rpp_1sampling_1635014522](renders\mc_32rpp_1sampling_1635014522.bmp)

* Reflections with 32 rays per pixels

  

  ![mc_8192rpp_1sampling_noAA_1635018925](renders\mc_8192rpp_1sampling_noAA_1635018925.bmp)

* 8192 rays per pixel

![mc_4096rpp_1sampling_1635023428](E:\Github\pathtracer_cpp\documentation\renders\mc_4096rpp_1sampling_1635023428.bmp)

* Additional Custom Scene with 3 Emissive spheres(two on the side are also specular in the same m_diffuse_color as the middle one) - 4096 rays per pixel

## Lab 04

![mc_1024rpp_1sampling_custom_1_AA_05_1635014778](E:\Github\pathtracer_cpp\documentation\renders\mc_1024rpp_1sampling_custom_1_AA_05_1635014778.bmp)

* Custom scene with 1024 rays per pixel



![mc_4096rpp_1sampling_noAA_1635019307](E:\Github\pathtracer_cpp\documentation\renders\mc_4096rpp_1sampling_noAA_1635019307.bmp)

* Custom Scene with 4096 rays per pixel and bitmap textures

## Lab 05

![mc_8192rpp_1sampling_AA_05_1635018084](E:\Github\pathtracer_cpp\documentation\renders\mc_8192rpp_1sampling_AA_05_1635018084.bmp)

* Cornell Box with 8192 rays per pixel and gaussian anti aliasing (sigma = 0.5f)

![mc_4096rpp_1sampling_AA_05_1635016573](E:\Github\pathtracer_cpp\documentation\renders\mc_4096rpp_1sampling_AA_05_1635016573.bmp)

* Custom Scene 1 with 4096 rays per pixel and anti aliasing turned on(sigma = 0.5f)

![mc_4096rpp_1sampling_AA_05_1635016890](E:\Github\pathtracer_cpp\documentation\renders\mc_4096rpp_1sampling_AA_05_1635016890.bmp)

* Custom Scene 2 with 4096 rays per pixel and anti aliasing turned on (sigma = 0.5f)

### Additional Screenshots

* See `renders/` 