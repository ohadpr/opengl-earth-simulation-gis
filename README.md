Earth-Simulation, Ohad Eder Pressman, 2001

Original blog post: https://www.ohadpr.com/page/16/

After the WTC attacks, CNN started using their Weather-System Simulation Program (I think) to show Afghanistan and Tomahawk missiles flying :)
This looked very nice and moved by the fact that I really didn't know too much geography and had a day or two free (Sukkot Holiday), I went on and wrote my own Earth Simulation program.
The program is based on a large sphere, with a nice texture of the earth is mapped to. You can rotate the earth, zoom-in, etc, etc.
What's GIS you ask ? Well, GIS stands for 'Geographical Information System', which actually means adding any type of information layer to a raster or vector map of the world or a portion of it.

What do I have to do with GIS ? After rotating my small earth for a few minute I started to get bored again, so I went on a hunt to find information layers that I could add.

Where did the information come from ?
-------------------------------------

The information about the location of the Countries came from something called 'The CIA FactBook 2000'. It's basically an html page with links to about 200+ countries, each one has a page with facts (including Latitude/Longitude information). I downloaded all the pages using Flashget's 'Download All' (thank god for that option), and wrote a small program that fetches all the countries names and positions and generates a '.H' file with all this information.
Vector information on Coastlines, Political Boundaries, Rivers, Lakes, Islands and US-States was much harder to get.
I searched for a long time and eventually picked up the basic binary data from the Pospeschil Micro World Data Bank II. I couldn't manage to understand the format or find any explanation of it. I then used a trial-version of a program called Idrisi to convert the pairs of the '.vec' and 'dvc' files into '.vct' and '.vdc' (these are also binary files). I then used the same program to read in the '.vct' and '.vdc' files and write out '.vxp' files. Now, the '.vxp' files are an easily understandable text-representation of the vector data. I wrote something small to convert the '.vxp' ASCII files into a binary equivilant (Binary Vector Data - '.bvd') [this isn't a public format, just something I hacked for fast loading]. And eventually our program uses the '.bvd' files directly :)
The small conversion programs are included with the source-code provided below. I didn't include all the versions of the data because they take almost 10MB alltogether.


Now, What can we do with the program (function-wise) ?
------------------------------------------------------

You can enable/disable the big Earth-Texture.
You can enable/disable Wire-Frame mode.
You can play with the line-width used by the wire-frame mode and the vector-data.
You can enable/disable the display of Country-Names.
You can seperately enable/disable the display of all the vector-layers (Coast-Lines, Islands, Lakes, Rivers, Nation Boundaries and US-States).


Some Notes on the application itself :
--------------------------------------

Use the mouse and it's left-button to rotate earth. Hold down Ctrl before you hit the left mouse button to scale the earth. Right-Click the window to get the options menu. Note that options are controllable by keyboard too.

Country Names are shown based on Z, the more you zoom in the more country-names appear. Countries are shown using small red sine-scaling glutSolidCubes.


Some notes on the 3D :
----------------------

I used OpenGL for this one, it was simple and straightforward. I generate a large sphere for the earth, use glSolidCubes for the countries, and simple GL_LINE_STRIPs for the vector-data. There are no display-lists or any other optimizations, I just throw everything at the card :)
I used glut for wrapping up all of Windows's internal calls, mainly because the first app-skeleton I found used it and provided some minimal texture-loading calls. Apparently the skeleton is by Andreas Gustafsson, I can't remember where exactly I found it, sorry.
I used a font library called glTexFont by Nate Miller, it uses the Quake3 Font, cool :)
Some thanks to the people at #opengl (efnet I believe) for helping me correctly placing the country-names (especially f00Dave, thanks for all the help).

The source is provided for your enjoyment, free-use, modification, etc. Feel free to contact me if you find a bug, add a feature, or anything alike.


email : ohad@visual-i.com
web   : ohad.visual-i.com
