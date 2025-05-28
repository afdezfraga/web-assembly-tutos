# SDL2 ShaderToy Project

This project demonstrates how to create a simple SDL2 application that runs a GLSL shader, inspired by ShaderToy. The shader creates a visually appealing animation using a palette function and various mathematical operations.

## Project Structure

```
006.SDL2ShaderToy
├── CMakeLists.txt        # CMake configuration file
├── src                   # Source files
│   ├── main.cpp          # Entry point of the application
│   ├── shader.frag       # GLSL shader code
├── build                 # Build directory for CMake
└── README.md             # Project documentation
```

## Dependencies

- CMake
- SDL2
- OpenGL (for shader compilation and rendering)
- GLEW (for OpenGL function loading)
- (Optional) Emscripten (for web deployment)

## Building the Project

1. **Install Dependencies**: Make sure you have CMake and SDL2 installed on your system.

2. **Create Build Directory**: Navigate to the project root and create a build directory:
   ```
   mkdir build
   cd build
   ```

3. **Run CMake**: Generate the build files using CMake:
   ```
   cmake ..
   ```

4. **Build the Project**: Compile the project:
   ```
   cmake --build .
   ```

## Building for Web (Optional)

1. **Set Up Emscripten Environment**: Activate the Emscripten environment:
   ```
   source /path/to/emsdk_env.sh
   ```

2. **Run CMake with Emscripten**: Generate the build files for web deployment:
   ```
   emcmake cmake ..
   ```

3. **Build the Project**: Compile the project for web:
   ```
   cmake --build .
   ```

4. **Run the Application in a Browser**: Use a local web server to serve the generated `.html` file. For example:
   ```
   emrun ./006.SDL2ShaderToy.html
   ```

## Running the Desktop Application

After building the project, you can run the application from the build directory:
```
./006.SDL2ShaderToy
```

## Shader Explanation

The shader code in `shader.frag` creates an animated visual effect using a palette function and various mathematical transformations. The `mainImage` function computes the final color for each pixel based on the fragment coordinates and time.

## License

This project is open-source and available for modification and distribution.