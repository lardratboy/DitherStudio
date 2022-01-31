DitherStudio (aka DitherBuddy) is a bit of a mess - the initial premise behind this tool was to provide a host around a Photoshop plugin I wrote for RGB555/RGB565 dithering. Over time it turned more into a viewer of colorspaces than a dithering tool so its name is not really indicative of the use. So what is it good for if it doesn't dither well? Good question, I mainly find it useful for viewing streams of bytes in 3D and for zooming in on images in 2D. The tool has a primitive document model, it displays a bitmap that comes from the clipboard or a file and allows you to pan/zoom around. Output is either a PNG file or what you copy into your clipboard, from there you can paste it into another tool for further processing.

Here is a video of the tool in action https://youtu.be/U4zH0mZ9cD4 should give you an idea if you want to download it and try it out.

The Project uses vcpkg and has the following dependencies.

glm
boost
angle

--BPT
