-------------------------------------------------------------------------------
glTexFont Library Release 6 -- 6/26/99
Contact            : Nate 'm|d' Miller (vandals1@home.com), ICQ: 8347914
Web Site           : http://members.home.com/vandals1/
glTexFont Web Site : http://members.home.com/vandals1/gltexfont
-------------------------------------------------------------------------------

ABOUT
-----
  This is a library to draw text in OpenGL.  

INSTALLING
----------
First extract the gltexfontlib.zip to a temporary folder. Once you have done this 
move "glTexFont.lib" to your compilers library directory. For MSVC it is "lib", for
 you compiler it may be different, just make sure "glTexFont.lib" is with all the 
other libraries. Then move "glTexFont.h" to your compilers include directory. For 
MSVC the directory is named "include", yours may be different. 

HELP AND USAGE 
--------------
Visit http://members.home.com/vandals1/gltexfont/gltexfontdoc.html, its better than anything 
I could do in here.  Also get the samples from the webpage which show off the features.  

MISC NOTES
----------
  This library was made to be used with OpenGL.  Make sure that you are linking to the
OpenGL libraries when you use glTexFont.  In my sample app for GLUT I link too the 
following libraries: msvcrt.lib opengl32.lib glu32.lib glut32.lib glTexFont.lib

LEGAL
-----
Notice: Usage of any code in this archive is subject to the rules 
described in the LICENSE.TXT file included in this directory.
Reading, compiling, or otherwise using the code constitutes
automatic acceptance of the rules in said text file.