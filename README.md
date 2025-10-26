# 2012-renderer  
An open-source place renderer for the version 0.69.0.646 Roblox Studio version (compatible with later versions, this was for December 2012 originally)  
You can get this client here: https://archive.org/details/roblox-studio_20250323  

## Usage  
Compile the project in **DEBUG** mode (Release mode could cause issues)  
Open the **MFCStudio** (RobloxApp clients are not compatible, no idea why)  
Run the **render** function (read functions below)

## Functions  
`render(filepath, x, y, isCharacter)`  
Example:  
`render("TestRender.png", 1280, 720, 1)`  
1 is ture, 0 is false, I dont care if your ass will yap why not true or false, figure it out yourself.
  
## Troubleshooting
1. Help! It's crashing!  
  - Make sure you compiled in **DEBUG** mode and that you're using **OPENGL**

2. I have no sky in my renders!
  - Make sure you are using **OPENGL**. **Direct3D** often makes the sky invisible.  
  
## FAQ
1. Can you hide the sky?
  - **Yes! You can.** Just change from OpenGL to Direct3D in studio settings, and also change **ViewBase::CreateView** to **Direct3D9** instead of **OpenGL**  
  
2. Why release this?  
   - I believe that a lot of revivals would benefit from this a lot, and I don't think that this should be gatekept.  
