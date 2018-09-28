GPXLab
======

![GPXLab](doc/gpxlab.png?raw=true)

What is GPXLab?
---------------
GPXLab is an application to display and manage GPS tracks previously recorded with a GPS tracker.

The idea is to give the user the possibility to combine several tracks into one single file and have statistic information about the summarized tracks or about a single track. Furthermore the user can see all tracks on a map and select there a track and/or a single track point. An altitude and a speed diagram and also a list of the track points offers visual information about the selected item.

![Demo](doc/demo.png?raw=true)

The view is divided into seven different parts:
- Statistic information about all tracks
- Calendar for track selection
- Track tree for track selection
- Statistic information about the selected track or track segment
- Map showing all tracks
- Diagram of the selected track or track segment
- List of the track points of the selected track or track segment

Except the map all other parts can be hid to maximize the map view.

Features & Functions
--------------------
- Support opening GPX (v1.0 or v1.1), NMEA and SpoQ files
- Combine several tracks into one single GPX file
- Rearrange the tracks (move/add/delete)
- Modify the meta data of the GPX file and of any tracks inside the file
- Get altitude data from the SRTM database
- Show statistic information about the summarized tracks and about a single track
- Show a map of all tracks
- Show an altitude and a speed diagram
- Show a list of the track points

Supported File Formats
----------------------
Currently GPX, NMEA and SpoQ files are supported. If you have a file in another format you may use GPSBabel (http://www.gpsbabel.org) to convert it into a GPX file.

License
-------
This software is free software and licensed under GPL version 3. 

Compiling
---------
The source code was developed under Qt 5.7.0. There is no guarantee that the code will compile with previous or later versions. 

The project is separated in two sub-projects GPXLab and QMapControl. QMapControl is a widget to display a map. Since I had to made some modifications to the original code I added the modified version here. QMapControl is compiled as a library and linked to the main sub-project GPXLab.

Both sub-projects are ready to build, although it was only tested under Windows 7 (64bit).
 
Credits
-------
- **QCustomPlot Qt Widget:** http://www.qcustomplot.com
- **QMapControl Qt Widget:** https://sourceforge.net/projects/qmapcontrol/
- **Nuvola Icons:** https://www.iconfinder.com/iconsets/nuvola2
- **FatCow Icons:** http://www.fatcow.com/free-icons
 
Release Notes
-------------

**[v0.4.2.1]**
- [fix] Increased gpx buffer size to 4096.
- [fix] Const in Settings::setValue(const QString &key, const QVariant &value)
 
**[v0.4.2]**
- [new] Show latitude and longitude in the status bar.
- [new] Show speed also in min/km.
- [new] "Follow item" and "show only selected item" are store in the settings.
- [fix] Following selected item failed.

**[v0.4.1]**
- [fix] Compilation with Linux should work now.
- [fix] Combination of atof() and locale.
 
**[v0.4.0]**
- [update] Updated to Qt 5.7.
- [update] Updated QMapControl to version 0.9.7.9.
- [update] Updated QCustomPlot to version 1.3.2.
- [new] The diagram's x axis now show elapsed time instead of absoulte time.
- [fix] Corrected timezone problem in NMEA and GPX parser.

**[v0.3.0]**
- [update] Updated QMapControl to version 0.9.7.8.
- [update] Updated QCustomPlot to version 1.3.0.
- [update] Updated copyright to "2014 - 2015".
- [new] Track on the map is highlighted when the mouse cursor is moved over it.
- [new] Show some point properties when the mouse cursor is over the point.
- [new] Added functionality to insert/delete track points.
- [new] Added functionality to set the start time of a track.
- [new] Added functionality to move a single track point using SHIFT + mouse click.
- [new] Added a calendar to select a track by date.
- [new] Diagram curves can be changed to show other properties.
- [new] Support SpoQ files (.act and .xml).
- [new] Support Garmin's TrackPointExtension heart rate extension.
- [fix] Ignore upper/lower case to detect file type.
- [fix] Draw tracks  with 0 or only 1 point correctly.
- [fix] Changed way of iteration in GPX_wrapper.
- [fix] Fixed bug in NMEA sentence parser.
 
**[v0.2.0 Beta]**
- [update] Updated QMapControl to version 0.9.7.6.
- [update] Changed some icons.
- [update] Some minor GUI changes.
- [new] Double clicking an item in the track selection opens the track properties dialogue.
- [new] Added undo/redo functionality.
- [new] Added functionality to split and combine a track.
- [new] Window state and position is stored on application closing and restored on restart.
- [new] Header state of point list is stored on application closing and restored on restart.
- [new] Added control to change the map zoom.
- [new] Added option to configure the map caching.
- [fix] Docks don't disappear any more when tabbed.

**[v0.1.0 Beta]**
- [new] Initial beta release.

Information
-----------
**Author:** Frédéric Bourgeois <bourgeoislab@gmail.com>

**Project homepage:** http://bourgeoislab.wordpress.com/gpxlab

**Latest release:** http://github.com/bourgeoislab/gpxlab/releases

**Latest sources:** http://github.com/bourgeoislab/gpxlab
