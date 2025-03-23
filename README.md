# 2012-renderer  
An open-source place renderer for the version 0.69.0.646 Roblox Studio version (compatible with later versions, this was for December 2012 originally)  
You can get this client here: https://archive.org/details/roblox-studio_20250323  

## Usage  
Compile the project in **DEBUG** mode (Release mode could cause issues)  
Open the **MFCStudio** (RobloxApp clients are not compatible, no idea why)  
Run the **render** function (read functions below)

## Functions  
`render(filepath, x, y, url)`  
Example:  
`render("TestRender.png", 1280, 720, "http://www.roblox.com")`  

The URL is where the image gets sent. If you don't want to send it, use a **random valid URL** instead.  
  
## Troubleshooting
1. Help! It's crashing!  
- Make sure you compiled in **DEBUG** mode and that you're using **OPENGL**

2. I have no sky in my renders!
   - Make sure you are using **OPENGL**. **Direct3D** often makes the sky invisible.  
