<!-- Author:tfw
Author-meta:tfw
Title: twix
Subtitle: 
Date:
Encoding:utf8
version:tfwio.wordpress.com
mainfont:Montserrat
monofont:FreeMono
monoscale:0.8
dh:8in
dw:5in
top:0.75in
bottom:0.75in
lr:0.35in -->

see github.com/olilarkin/wdl-ol

tom's wdl/iplug extras --- a few iplug controls supporting a particular midi effect/arpeggiator up here.

Its been tested/used in VisualStudio Express 2013 of yet.

This happens to be a first-draft of this doc, so I'm crossing my fingers that this covers everything.

# Why?

The only use-case/example is thus far in what is currently up here, called [arpeggio](https://github.com/tfwio/arp).  There have been a few changes made to IPlugVST (windows) that have yet to be noted.  I've documented what I could think of that you might need to change in order to compile the arp project and lice test-app. [well, at least that's part of the reason as to why ;)]

Its probably worth pointing out that you're probably not going to want to navigate through the code here except through its use-case as not everything here is used or perhaps working.  Its WIP after all.

# Usage

- Clone this project somewhere to its default name: `twix`
- Move the cloned directory parallel to `./WDL` in your wdl-ol fork which happens to be parallel to `./VST_SDK`, `VST3_SDK`, etc...
- Got to Property-Page editor within your respective WDL-OL/IPLUG project.  It's called 'Property Manager' within the IDE.
    - If the "Property Manager" tab isn't visible within your IDE, then check `View->Other Windows->Property Manager` to make it visible.  If you don't see such options, then you probably have to change some `Tools->Options`-like setting for 'Expert Mode' or some-such.
- Expand any configuration folder icon (it doesn't matter which one)
- Right-click your project-name in the configuration...  
  If your project's name is `MyIplugin` then you will see a property page with that name in a given configuration/platform 'directory'-icon when its expanded*—resulting in the property-sheet having been added to each configuration*.
- Select 'Add Existing ...' and select the property-page in the `./twix` root directory.
- Like I had noticed, you might have just slightly broken your configuration.  
  To resolve this, you simply need to append `.` to your Plugin Property-Page configuration in the C++ incude-directories and make sure it is before all other include directories.
- Now add IControls.cpp to your source project.
- See the hacks below for implementing `OnTimer`

# IPlug Hacks

I made a few simple little changes to some IPlug files to test out doing some custom animation stuffs on something other than the audio thread.  As I'd only done this in windows, I'd like to eventually see how this might be handled on mac.  Point being, if I wrote any plugins dependant on this library that are up on github, they'll likely use the simple modification below somehow to enable animations one way or another via a call to `OnTimer` as mentioned...

**IPlugBase.h** (within the class)

```cpp
virtual void OnTimer(){}
```

**IGraphicsWin.h**

The call to OnTimer was added within the following context.

```cpp
// static
LRESULT CALLBACK IGraphicsWin::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  if (msg == WM_CREATE)
  {
    LPCREATESTRUCT lpcs = (LPCREATESTRUCT) lParam;
    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LPARAM) (lpcs->lpCreateParams));
    int mSec = int(1000.0 / sFPS);
    SetTimer(hWnd, IPLUG_TIMER_ID, mSec, NULL);
    SetFocus(hWnd); // gets scroll wheel working straight away
    return 0;
  }

  IGraphicsWin* pGraphics = (IGraphicsWin*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

  if (msg == WM_TIMER && wParam == IPLUG_TIMER_ID)
  {
    pGraphics->mPlug->OnTimer();
  }
// ...
```

**IPlugStandalone.cpp**

Added a `double mFramesCounted` member variable for a default (fallback) test display to render on the above timing method.

```cpp
void IPlugStandalone::LockMutexAndProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
  IMutexLock lock(this);
  ProcessDoubleReplacing(inputs, outputs, nFrames);
  mFramesCounted = fmod(mFramesCounted + nFrames, 60*44100*24);
}
```

**IPlugStructs.h**

in IRECT, I still hadn't gotten this out of there.  There is probably an equivelant somewhere that makes this pointless, though if there is a dependency on this and I didn't mention it may be rude.

```
static IRECT Create(int x, int y, int w, int h) { return IRECT(x,y,x+w, y+h); }
```

## LICE test-app?


In `WDL/lice/test/test.rc`, change the following if you want to compile the lice test-app for Visual Studio Express 2013.

```cpp
// replace with windows.h
//#include "afxres.h"
#include <windows.h>
```

Also to compile the lice test-proj `test.vcxproj` you need to add `PNG_WRITE_SUPPORTED` to the preprocessor definitions...

There are (of course) a few ways you can do this either building from the command-line, manually adding this to the vcxproj file or from the IDE's property-manager or editor.

I might also have included something like the following somewhere

File-name=`[wdl-ol]/WDL/lice/test/LiceInc.props`

```xml
<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ImportGroup Label="PropertySheets" />
  <PropertyGroup Label="UserMacros" />
  <PropertyGroup />
  <ItemDefinitionGroup>
    <ClCompile>
      <AdditionalIncludeDirectories>..;%(AdditionalIncludeDirs)</AdditionalIncludeDirectories>
    </ClCompile>
    <Link>
      <AdditionalDependencies>../build-win/$(Platform)/$(Configuration)/lice.lib;%(AdditionalDependencies)</AdditionalDependencies>
    </Link>
  </ItemDefinitionGroup>
  <ItemGroup />
</Project>
```