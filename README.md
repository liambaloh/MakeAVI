# MakeAVI
A simple windows application to convert images into video.

## Credits
This application was retrieved from https://sourceforge.net/projects/makeavi/, where it was developed by Sourceforge user Redshoe8 (David) and shared under GLP2. 

The remainder of this readme was originally written by David, but has been modified so it doesn't sound like a contributor to this repository on Github wrote it, as the content originally said "I wrote...", but now says "David wrote..." or "they wrote..."

## Introduction
------------------------------------------------------------------------
This is a quick and dirty app that David wrote because they wanted to do some work with creating time-lapse movies. They set up a digital camera to take a picture every 10 seconds for several hours, then string the JPGs together into an AVI, and do any video work from there.

David took a quick look around, but didn't find any free implementations for Windows. So they looked around and found some code to base this on, and about 4 hours later, here it is.

The core of the AVI generation is the Microsoft/Windows AVI API. Wrapped around that is a thin wrapper, which David found at www.codeguru.com, but it may be from a Microsoft sample app.

The image loading end is handled by the FreeImage library, which is currently at http://www.6ixsoft.com/. It can handle many different image types, but not GIF. GIF is patent burdened and will not be supported, so don't bother asking.

## INSTALLATION
------------------------------------------------------------------------
Unzip all this stuff into one directory. Run MAKEAVI.EXE.

There, it's installed.  If you want to get cute, right click on MakeAVI, hit "Create shortcut" and drag the shortcut onto your desktop.

## BUILDING IT YOURSELF
------------------------------------------------------------------------
Grab the source files, and unzip them into a directory.  You'll also need the FreeImage files; the files FreeImage.h and FreeImage.lib will need to be in the build area; get these from the FreeImage home at www.6ixsoft.com Then, load the project into Visual C++ and build.

## Using MakeAVI
------------------------------------------------------------------------
You want to select all your images into the list on the left.  Use the "Add Files" button to choose your files.

Use the "Add Files", "Delete File", "Sort Files", and "Move file Up/Down" buttons to get the images in the order you want them.

Any image that is selected on the left will be previewed in the middle. If it isn't previewed properly, then the program is unable to read the image (can't access the file, or the file is corrupt, or something).

### CROPPING:
All the images have to be the same size to go into an AVI file If you have images of varying sizes, or you have junk at the edge that you want to eliminate from the final AVI, you can use the "Cropping" interface at the bottom.  Check the "Crop to" box, and select the size you want the final images to be cropped to in the boxes to the right.

NOTE that the original images are NOT cropped on disk, they are only cropped after they're loaded, and before they're added to the AVI.

NOTE that currently, images are cropped towards the center, you can't select where the cropping occurs.

As an aid to filling in these values, if you add new images to the list, the size of the last image added is put into those edit boxes.  Also, you can double click on any image in the list at any time and the size of that image will be copied in to the boxes.

### Ready to go...
When you've gotten the images all in order, input your frame rate in the box provided; when you play the final AVI file, that many images will be played per second.

Then, press the "Begin!" button.  The feedback box will be displayed, and you will be asked which video format you wish to save to. There  won't be talk about video formats here, except for a few observations:

If you will be continuing to work with the video, perhaps encoding it to other formats, then you may want to use "Uncompressed" - though that generates HUGE files.  In general time lapse stuff is not that big,so your file will probably only be a few gigabytes, so this is not a bad choice, but it should NOT be used for the final product; the  data rate is so high that most computers will not be able to play the video smoothly.

If you are going for a final AVI product, Indeo 4.5 or higher is a good choice, since everyone either has it or can get the codec automatically when they start to play the video under Windows media player.

## Possible future enhancements:
Cropping other than from the center
Resizing images
Fixing support for a few codecs that don't work for some reason
