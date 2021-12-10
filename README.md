# About SDL2

![SDL2](/Resources/Branding/logo.png?raw=true"SDL")

Simple DirectMedia Layer is a cross-platform development library designed to provide low level access to audio, keyboard, mouse, joystick, and graphics hardware via OpenGL and Direct3D. It is used by video playback software, emulators, and popular games including Valve's award winning catalog and many Humble Bundle games.
SDL officially supports Windows, Mac OS X, Linux, iOS, and Android. Support for other platforms may be found in the source code.
SDL is written in C, works natively with C++, and there are bindings available for several other languages, including C# and Python.
SDL 2.0 is distributed under the zlib license. This license allows you to use SDL freely in any software.

# About This Repository
I have included & fully set up a test project of SDL2 for Visual Studio Code. It has all the necessary files (check the Dependencies folder) to get started with SDL2.
In the Dependencies folder, I have included a DLL folder that has a bunch of DLL files for SDL2.
This test project setup will help those people who are struggling with the SDL2 & SDL2_gfx project build & getting errors or running any SDL2 programs. 
It has all the SDL2 header files (SDL2, SDL2_image, SDL2_mixer, SDL2_net, SDL2_ttf & SDL2_gfx).

# How to build this Test Project?
According to your Solution Configurations (Debug/Release),

If you set the project target platform to x86/x64, then go to project properties, then make sure you select for All Configurations & All Platforms.
After that, go to Linker & set the Additional Library Directories to ...\lib\x64 or ...\lib\x86 (according to your target platform). 
Then click on Apply & build the project. 

Now if you try to run the program, you'll get some DLL file missing errors. Don't get scared!
You have to then copy all the DLL files from the DLL folder (according to your solution platform x86/x64) & paste them into your executable binary (x86/x64) file's directory.
That's it! just clean the project, build the project again & run the program!

# Note
If you find any build toolset kind of errors, make sure you have selected Windows SDK Version 10.0.

# License
Simple DirectMedia Layer
  Copyright (C) 1997-2021 Sam Lantinga <slouken@libsdl.org>
  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

SDL2_gfx
  Copyright (C) 2012-2014  Andreas Schiffler
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

Andreas Schiffler -- aschiffler at ferzkopp dot net
