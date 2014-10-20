# Overview
Community Core Vision (CCV for short) is an open-source/cross-platform solution for computer vision and machine sensing, specifically blob tracking.

View videos and learn more at: http://ccv.nuigroup.com


It takes an video input stream and outputs tracking data (e.g. coordinates and blob size) and events (e.g. finger down, moved and released) that are used in building NUI-aware applications. 
CCV can interface with various web cameras and video devices as well as connect to various TUIO/OSC/XML enabled applications and supports many multi-touch lighting techniques including:
FTIR, DI, DSI, and LLP with expansion planned for the future vision applications (custom modules/filters) in CCV 2.0.

# Compiling instructions

This page aims to provide more information about the compiling procedures for the Community Core Vision.

## Windows - Microsoft Visual Studio 2008

### Compiling Instructions
1. Open the VS 2008
2. File -> Open -> Project/Solution
3. Then , go to : "apps\addonsExamples\VS2008"
4. You may choose to open the project : "Community Core Vision.vcproj" or the "Community Core Vision.sln"
5. Press F7 or click Build-> Build Solution or Build -> Build Community Core Vision
6. You may press the green arrow besides "Release/Debug" to Run , or press F5

### Known Issues
Error Message : fatal error C1900: Il mismatch between 'P1' version '20080116' and 'P2' version '20070207'
LINK : fatal error LNK1257: code generation failed
**Solution**: *Install the  Service Pack 1 for Visual Studio 2008

## Ubuntu (32-bit) - Codeblocks

### Compiling Instructions
* In the scripts folder, run the "install_codeblocks.sh"
* Run the "install_dependencies.sh" script
* Open Codeblocks, and you can find the project in "Linux/apps/addonsExamples/Codeblocks_8_linux/"
* Named as "Community Core Vision.cbp"
* Then Build and run :)

### Known Issues

libfmodex.so Error
**Solution**: Rename libfmodex-4.22.00.so to libfmodex.so, or make a symlink between both: ln -s libfmodex-4.22.00.so libfmodex.so

## Ubuntu (64-bit) - Codeblocks

### Known Issues
libfmodex.so Error
**Solution**: *enter the libs directory and make the following symlink: ln -s libfmodex64.so.4.10.03 libfmodex.so*
