# Virtual Surround App

A virtual surround solution for any pair of headphones or speakers.

## Purpose

This project aims to allow users to experience immersive surround sound in any stereo setup without being dependant on proprietary software or existing "default" downmixing implementations. The idea is to provide a configurable experience with various downmixing methods (see [Downmixing Methods](#downmixing-methods) for details on current and planned methods for downmixing).

## Approach

## Downmixing Methods

- **Dolby Prologic II (DPLII)**
  - **Status**: Functional, in development.
  - **Details**: This is a modified version of the DPLII encoding matrix. Since we aren't concerned with making the signal decodable, we took the liberty to implement some enhancements to improve how natural it sounds
  - **Next steps**: Parameterize enhacements.

- **HRTF-Based Surround**
  - **Status**: Planned.
  - **Details**: [HRTF](https://en.wikipedia.org/wiki/Head-related_transfer_function#:~:text=A%20head%2Drelated%20transfer%20function,from%20a%20point%20in%20space.) (Head-related transfer function) is designed to mimic how our ears perceive sounds from different points in space. 
  - **Next steps**: TBD.

##

