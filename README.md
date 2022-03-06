# About Repository
![DemoWindow](/Resources/Branding/DemoWindow.png?raw=true"DemoWindow")

I have fully set up a test project of SDL2 for Visual Studio Code 2019/2022 users. It has all the necessary dependency files to get started with SDL2.
In the Dependencies folder, I have included a dll folder that has a bunch of DLL files for dynamically linking of your project.
The main reason I have made this repository to help those people who are struggling with SDL2 & SDL2_gfx success project build & those who are getting errors when they try to run any SDL2 projects. It has all the SDL2 header files & also it has SDL2_image, SDL2_mixer, SDL2_net, SDL2_ttf, SDL2_gfx. Additionally, It also has GLFW, GLEW & ImGui support for your SDL2 projects.

# How to build this project?
According to your Solution Configurations (Debug/Release),

If you set the project target platform to x86/x64, then go to project properties, then make sure you select for All Configurations & All Platforms.
After that, go to Linker & set the Additional Library Directories to ...\lib\x64 or ...\lib\x86 (according to your target platform). 
Then click on Apply & build the project. 

Now if you try to run the program, you'll get some DLL file missing ERRORS. DON'T get scared!
You have to then copy all the DLL files from the dll folder (according to your solution platform x86/x64) & paste them into your executable binary (x86/x64) file's directory.
That's it! then just run the program!

# NOTE
If you find any build toolset kind of errors, make sure you have selected Windows SDK Version 10.0.
