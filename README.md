# Virtual Surround App

A virtual surround solution for any pair of headphones or speakers.

## Purpose

This project aims to allow users to experience immersive surround sound in any stereo setup without being dependant on proprietary software or existing "default" downmixing implementations. The idea is to provide a configurable experience with various downmixing methods (see [Downmixing Methods](#downmixing-methods) for details on current and planned methods for downmixing).

## Approach

(TODO)

**Next steps**:
- Clean up console UX/UI.
- Add safeguards to device configuration.
- Add 7.1 surround downmixing capabilities (only able to do 5.1 for now).

## Downmixing Methods

- **Dolby Prologic II (DPLII)**
  - **Status**: Functional, in development.
  - **Details**: This is a modified version of the DPLII encoding matrix. Since we aren't concerned with making the signal decodable, we took the liberty to implement some enhancements to improve how natural it sounds
  - **Next steps**: Parameterize enhacements.

- **HRTF-Based Surround**
  - **Status**: Planned.
  - **Details**: [HRTF](https://en.wikipedia.org/wiki/Head-related_transfer_function#:~:text=A%20head%2Drelated%20transfer%20function,from%20a%20point%20in%20space.) (Head-related transfer function) is designed to mimic how our ears perceive sounds from different points in space. 
  - **Next steps**: TBD.

## Usage

The current iteration of the app uses a simple console UI to configure the input and output devices, with plans to add the ability to enable and disable certain features depending on what virtual surround driver is being used.

### Pre-use checks
To use the app, make sure:
- You have at least one playback device configured to 5.1 (7.1 support is still in development).
- You have at least one playback device configured for stereo output.

These settings can be viewed and configured in the `mmsys.cpl` dialog (Win + R, type `mmsys.cpl` and click "OK"). 

Because the application uses loopback capture, we recommend [VB-Cable](https://vb-audio.com/Cable/) as the nominal input device. By itself, it doesn't produce any output, so you won't have any unwanted audio coming from your surround device. However, any surround device can be used as an input, provided you find a way to not hear it (e.g. using your default system drivers as the input and using an audio interface as the output, connecting your headphones or speakers to the interface).

### Using the app

All you need to do is run the executable and follow the prompts given by the application, bearing in mind the configuration you set in the [Pre-use checks](pre-use-checks).
