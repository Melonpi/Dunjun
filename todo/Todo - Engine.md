# TODO - Engine

* [ ] - Not Done
* [~] - Semi Done
* [*] - External Tool at the moment
* [x] - Done
* [N] - Never Finished (Always worked upon)

## Graphics

* [x] Camera
* [~] Color
	* [x] rgb(a)
	* [ ] hsl
	* [ ] hsv
	* [ ] husl ?
* [x] Image (w/ loader)
* [~] Texture
	* [ ] 1D
	* [x] 2D
	* [ ] 3D
	* [ ] Cubemap
* [x] Material
* [x] Vertex
* [x] Mesh
* [x] ModelAsset
* [x] Transform Type (Alternative to Matrices)
	* [x] Handle Position, Orientation, and Scale Separately
	* [x] Operator Overloads
* [ ] Physically Based Rendering
	* [ ] PB Material System
		* [ ] Normal - Opaque textured
		* [ ] Transparent - Partly transparent
		* [ ] TransparentCutOut - Fully opaque with fully transparent areas, like fences
		* [ ] Self-Illuminated - Light emitting parts
		* [ ] Reflective -  Reflect an environment (Cubemap)
* [x] Lights
	* [x] Ambient
	* [x] Directional
	* [x] Point
	* [x] Spot
* [ ] Primitve Objects (Cube, Sphere, etc.)
* [ ] Fog
* [ ] Ambient Occlusion
* [ ] Real-Time Global Illumination
* [ ] Font System and Rendering
* [~] RTT Effects
* [ ] Post-Processing Effects
* [ ] SMAA

## World/Scene Architecture

* [x] SceneNode
* [N] SceneRenderer
* [x] NodeComponent
* [ ] Light as Nodes
* [ ] World - Manages Scene

## Physics

* [ ] Multithreaded
* [ ] Rigidbodies
* [ ] Colliders
* [ ] Joints
* [ ] Character Controllers

## Optimization

* [ ] Mutlithreaded
* [x] Cone Frustum Culling - To be replaced by below
* [ ] Frustum Culling
* [x] Deferred Rendering
* [*] Profiler
* [ ] Level of Detail

## UI

* [ ] Canvas
* [ ] Basic Layout
* [ ] Visual Components
* [ ] Interation Components
* [ ] Animation
* [ ] Rich Text / Markdown Support

## Audio

* [ ] Streaming
* [ ] Loading
* [ ] Mixing
* [ ] Hierachical Mixers
* [ ] Transitions

## System

* [x] typedef basic typesni
	* [x] u|s(8|16|32|64)
	* [x] f(16|32|64)
	* [x] b(8|32)
	* [ ] frac(8|16|32) - fractional type [0.0, 1.0]
* [x] Windows
* [~] OS X
* [ ] Linux
* [x] Time Type
* [x] Clock
* [x] Tick Counter
* [ ] Command/Task System
* [ ] State Machines
* [ ] Configuration Files
* [x] ResourceHolder
* [ ] Tag-Based Asset Loader/Manager
* [ ] Streaming Assets

## Settings (External/Config File)

* [ ] Graphical Settings
	* [ ] Antialiasing (SMAA)
	* [ ] Anisotropic Filtering
	* [ ] Post-Processing Effects
	* [ ] Fog
	* [ ] Number of Objects
	* [ ] Render Distance
	* [ ] Gamma
	* [ ] Brightness
	* [ ] Color Correction
	* [ ] Field of View
	* [ ] Vertical Sync
	* [ ] Model Detail
	* [ ] Texture Detail
	* [ ] Shadow Detail
	* [ ] Water Detail

* [ ] Audio Settings
	* [ ] Speaker Setup
	* [ ] Volume Levels
	* [ ] Quality

* [ ] Window Settings
	* [ ] Video Mode
	* [ ] Display Mode
		* [ ] Fullscreen
		* [ ] Window
		* [ ] Borderless Window

## Window

* [x] Event System
* [x] GLContext
* [x] Window
	* [x] Windowed
	* [x] Fullscreen
	* [ ] Toggle Fullscreen
* [x] VideoMode

## Input

* [x] Keyboard
* [x] Mouse
* [~] Controller
	* [x] Buttons
	* [x] Axes
	* [ ] Rumble
* [ ] Key mapping
* [x] Clipboard

## Animation

* [ ] Inverse Kinematics
* [ ] Key-Frame based animation

## Math
* [x] VectorN - N = {2, 3, 4}
	* [ ] VectorN<T> - T = {s32, u32, f32} (f32 be default)
		- Math::VectorN<T>
		- using VectorN = Math::VectorN<f32>
* [x] Vector functions
* [x] Matrix4
* [x] Quaternion
* [x] Constants
* [x] Random
* [x] Rectangle
* [x] Cube
* [x] Unit
* [x] Angle: Radian & Degree
* [x] Functions
	* [x] Common (Trig, Hypb, etc.)
	* [x] Interpolation (lerp, slerp, squad, etc.)
	* [x] Transformation (translate, scale, rotate)

## Other

* [ ] Path finding

## Scripting

* [ ] Scripting Language - Is it even needed?!
