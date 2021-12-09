# SDL2 (Simple DirectMedia Layer)

![SDL2](/Resources/Branding/logo.png?raw=true"SDL")

Simple DirectMedia Layer is a cross-platform development library designed to provide low level access to audio, keyboard, mouse, joystick, and graphics hardware via OpenGL and Direct3D. It is used by video playback software, emulators, and popular games including Valve's award winning catalog and many Humble Bundle games.
SDL officially supports Windows, Mac OS X, Linux, iOS, and Android. Support for other platforms may be found in the source code.
SDL is written in C, works natively with C++, and there are bindings available for several other languages, including C# and Python.
SDL 2.0 is distributed under the zlib license. This license allows you to use SDL freely in any software.

# About This SDL2 Project Build For Visual Studio Code
I have included & fully set up a test project for SDL2. It has all the necessary files (check the Dependencies folder) to get started with SDL2.
In Dependencies folder, I have included a dll folder which have a bunch of DLL files for SDL2.
This test project setup will help to those peoples who are strugging with the SDL2 & SDL2_gfx project build & getting errors or to run any SDL2 programs. 
It has all the SDL2 header files (SDL2, SDL2_image, SDL2_mixer, SDL2_net, SDL2_ttf & SDL2_gfx).

# How to build this Test Project?
According to your Solution Configurations (Debug/Release),

If you set the project target platform to x86/x64, then go to project properties, then make sure you select for All Configurations & All Platforms.
After that, go to Linker & set the Additional Library Directories to ...\lib\x64 or ...\lib\x86 (according to your target platform). 
Then click on Apply & build the project. 

Now if you try to run the program, you'll get some DLL file missing errors. Don't get scared!
You have to then copy all the DLL files from the dll folder (according to your solution platform x86/x64) & paste it into your exectable binary(x86/x64) file's directory.
That's it! Clean the project, build the project again & run the program!
