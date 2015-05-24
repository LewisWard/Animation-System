# README #

C++ Animation System

Copyright (C) 2015 Lewis Ward, All rights reserved. (lewis.ward@outlook.com)

For 3rd party libraries (i.e. Glew) see either their README or header file for copyright and notices.

The project uses two different custom Maya Plugins for exporting objects, an file exported that ends in '.amesh' is from my plugin. Files exported ending in '.meshes' is not from my plugin, this is a 3rd party plugin as a result this plugin and its source code is not provided (the exported meshes have been provided).

Modification of any source code, which is mine and not third party, is not permitted. 

I've done my best to resolve all bugs I've found but if you download any part of this project and do find any, I ACCEPT NO RESPONSIBILITY IF ANYTHING GOES WRONG! Use at your own risk!

### What is this repository for? ###

* Summary : A C++/OpenGL 3D Animation System that allows the user to import Rigid Skinned objects from Maya. There are two main parts to this animation system, the first being the Maya plugin that allows animated objects to be export from a Maya scene and the second is the C++/OpenGL program at loads the exported objects into a game scene.
* Version : 1.0.0

### How do I get set up? ###

* If do you download this project, all the header file you need "should" be provided. I wrote the project in Visual Studio, so including headers/source files should be easy. I used SDL 2.0 library, the headers have been included but not the .lib files so you would need to download SDL yourself and do the linking. 
* The Maya Plugin code does require Maya (tested on 2014 and 2015 version) to be included/linked. 
* Maya builds the plugin in x64, although the C++ program will work in both x86 and x64 its recommended to build both in x64!

### Contribution guidelines ###

* I'm a very busy person but if you encounter any bugs please do send me a message with details about it, I'll see what I can do.
* If you have an constructive feedback, good or bad, also please message me. I would like to know what I've done well and area to be improved on. Although I'm not looking for feature requests due to my lack of free time.