# COMP371_A1
Olaf Snowman 3D Environment

![alt text]("https://github.com/RemineralizedWater/COMP371_A1/blob/master/COMP371A1 screenshot1.jpg"?raw=true)
![alt text](https://github.com/RemineralizedWater/COMP371_A1/blob/master/COMP371A1 screenshot2.jpg?raw=true)
![alt text](https://github.com/RemineralizedWater/COMP371_A1/blob/master/COMP371A1 screenshot3.jpg?raw=true)
![alt text](https://github.com/RemineralizedWater/COMP371_A1/blob/master/COMP371A1 screenshot4.jpg?raw=true)

README for the COMP371: Computer Graphics Assignment 1
======================================================
By RemineralizedWater
=====================

Important Files and Folders:
============================

readme.txt: 			Contains the details of this program
VS2017/COMP371_A1.vcxproj	Opens the project in Visual Studio, to enable compiling and running
Source/COMP371_a1.cpp:		The source code for this project
Assets/Shaders			Contains all relevant vertex shaders and fragment shaders
Assets/Textures			Contains all relevant textures (For Fair Use Educational Purposes)
Source/stb_image.h		Required third-party libraries
ThirdParty			Required third-party libraries

Features:
=========

This OpenGL program offers a number of features. To enable visualization of the scale and dimensions of our virtual 'world' environment, a 100 x 100 unit transparent square grid is rendered, as well as coordinate axes (in math they are often called 'unit vectors', aka i, j & k, although ours are 5 units not 1). Our coordinate vectors specify the three-dimensional axes of our world (the x-, y- and z-planes). To view and navigate this world, our program features a robust camera, which allows for pan (yaw), tilt (pitch) and zoom (movement). Additionally, the world can be rotated about all 3 axes to any orientation, and reset to the original. Finally, the key feature of our program is the Olaf snowman, which is a fully textured snowman complete with snow skin, wood arms & fingers, coal eyes, a carrot nose, a corncob mouth, and 'real' hair. He can be viewed with these textures, by the lines connecting his vertices, or as the points of his vertices. He can move forward, back, left and right, as well as turn left and right. He is able to scale to massive or miniscule size. If desired, he can be teleported to a random location on the grid.

Functionality:
==============

The specific functionality of this program will now be described. The grid is rendered using a two vertex array, implemented through two for loops. The three coordinate axes and every part of Olaf use a second 36 vertex cube array, which undergoes the necessary transformations. Additionally, the coordinate axes receive custom vertex color information from a variable to enable each axis to have a different color. Olaf receives custom imported textures which are rendered using the stb_image library when in triangle mode. When in point or lines mode, each line/vertex becomes the texture color at that position. The transformations done on Olaf's body parts are non-hierarchical in nature, and instead are implemented as affine transformations. As such, they are unsuited to massive modifications to the quantity and quality of Olaf model component parts. The rotations done to Olaf, the camera, and world orientation are all done using standard three-dimensional matrices, so are unsuited to the prevention of gimbal locking (this would require quaternion matrices). To avoid this issue, the vertical camera angle has been capped to a maximum movement of 89 degrees in either direction. The two vertex shaders and two fragment shaders have been stored in GLSL files, and have then been loaded, compiled, linked to shader programs, and then the compiled vertex & fragment shader copies are safely destroyed. For the world orientation transformations, this is implemented by using a second world matrix, which stores all world transformations easily without distorting the other objects in the world. This second world matrix is returned to identity easily by pressing the Home button. Camera movement is done using Euler angles theta and phi to track the horizontal and vertical camera angles and what the camera is looking at. Finally, Olaf teleportation is handled using the rand function inherent to C++.

Keyboard / Mouse Shortcuts:
===========================

Move Olaf to random location					                        Space
Scale-up Olaf							                                      u
Scale-down Olaf							                                      j
Move Olaf left							                                a + hold Shift
Move Olaf right							                                d + hold Shift
Move Olaf up						                                  	w + hold Shift
Move Olaf down							                                     s + hold Shift
Rotate Olaf counterclockwise 5 degrees about Y-axis		            a
Rotate Olaf clockwise 5 degrees about Y-axis		              	d
Rotate world counterclockwise 5 degrees about X-axis	      	Up Arrow
Rotate world clockwise 5 degrees about X-axis		        	      Down Arrow
Rotate world counterclockwise 5 degrees about Y-axis		      Left Arrow
Rotate world clockwise 5 degrees about Y-axis		          	Right Arrow
Rotate world counterclockwise 5 degrees about Z-axis		       Page Up
Rotate world clockwise 5 degrees about Z-axis			          Page Down
Reset world orientation						                            Home
Render Olaf with textured triangles				                        t
Render Olaf with lines						                                l
Render Olaf with points						                              p
Pan camera							                           hold Right Mouse Button + X-axis Mouse Movement
Tilt camera							                           hold Middle Mouse Button + Y-axis Mouse Movement
Zoom camera							                           hold Left Mouse Button + X-axis Mouse Movement
