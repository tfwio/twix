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

> tfw's wdl/iplug extras --- a few tiny classes and iplug controls supporting a particular midi effect/arpeggiator up here and whatever else may pop up over time.

see github.com/olilarkin/wdl-ol

Its been tested/used in VisualStudio Express 2013 of yet.

# Why?

The only use-case/example is thus far in what is currently up here, called [arpeggio](https://github.com/tfwio/arp).  There have been a few changes made to IPlug (windows) that have yet to be noted.  I've documented what I could think of that you might need to change in order to compile the arp project and lice test-app. [well, at least that's part of the reason as to why ;)]

Its probably worth pointing out that you're probably not going to want to navigate through the code here except through its use-case as not everything here is used or perhaps working.

# Usage

- Note that if you would like to compile arpeggio 20170422 (github.com/tfwio/arp), the only 'animation' used renders a little (useless) timing information, so a call to OnTimer in the `class Arpeggio` can be omitted if you don't want to hack iplug as such.
- Clone this project somewhere to its default name: `twix`
- Move the cloned directory parallel to `./WDL` in your wdl-ol fork which happens to be parallel to `./VST_SDK`, `VST3_SDK`, etc...
- Go to Property-Page editor within your respective WDL-OL/IPLUG project.  It's called 'Property Manager' within the IDE.
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

- changes are best described by the patches in the ['./patch' directory](patch)  
  against github.com/olilarkin/wdl-ol [commit](https://github.com/olilarkin/wdl-ol/commit/c16a383314612020b83d900c5ada1b8e82938816)/[tree](https://github.com/olilarkin/wdl-ol/tree/c16a383314612020b83d900c5ada1b8e82938816) c16a383
- [wdl-ol_win.patch](patch/wdl-ol_win.patch) adds the possibility for custom animations per the respective frame-rate passed into IplugBase's timer cycles, adding a virtual call to `void OnTimer()` and there might be a few other less mindful things in iplugstructs.h-IRECT and such...
- [lice-test_vs2013.patch](patch/lice-test_vs2013.patch) helps building the lice/test app

