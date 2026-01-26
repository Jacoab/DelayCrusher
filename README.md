# Delay Crusher

<center>
    <img src="images/UI.png" alt="drawing" width="600"/>
</center>


Delay Crusher is a VST3 audio effect plugin that provides bit crushing and delay effects for music production. The plugin combines digital distortion with a delay to create lo-fi, retro, and experimental sounds.

## Features

- **Bit Crusher**: Reduce the bit depth and sample rate of audio
- **Noise Generator**: Add noise using Box-Muller Gaussian noise generation
- **Delay**: Time-based delay effect with dry/wet mixing

## Requirements

- CMake 3.24 or later
- Xcode Command Line Tools (MacOS)
- JUCE framework (included as a submodule)

## Building

Start by cloning the repository and initializing the JUCE submodule:

```
git clone --recurse-submodules https://github.com/Jacoab/DelayCrusher.git
```

To build Delay Crusher:

```
cmake -B build -S . -G Xcode
cmake --build ./build --config Release
```

This will generate an Xcode project in the `build` directory and compile the VST3 plugin. The built plugin will be located in `build/DelayCrusher_artefacts/Release/Delay Crusher.vst3`.

## Testing

To run unit tests:

```
ctest --test-dir build -C Release -V
```

## Installing

### MacOS VST3 Installation

The CMake build should automatically put the plugin into the correct directory.  If the plugin does not appear then verify that it has been installed to the correct location.

```
ls ~/Library/Audio/Plug-Ins/VST3/DelayCrusher.vst3/Contents/MacOS
```

If the directory is not found then you can install Delay Crusher by copying the built plugin to the standard VST3 location:

```
cp -r build/DelayCrusher_artefacts/Release/VST3/DelayCrusher.vst3 ~/Library/Audio/Plug-Ins/VST3/
```

After installation, the plugin will be available in any VST3-compatible DAW. You may need to re-scan the plugin directory in your DAW.
