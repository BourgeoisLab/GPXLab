GPXLab
======

![GPXLab](doc/gpxlab.png?raw=true)

What is GPXLab?
---------------
GPXLab is an application to display and manage GPS tracks previously recorded with a GPS tracker.

The idea is to give the user the possibility to merge several tracks into one single file and have statistic information about the summarized tracks or about a single track. Furthermore the user can see all tracks on a map and select there a track and/or a single track point. An altitude and a speed diagram and also a list of the track points offers visual information about the selected item.

![Demo](doc/demo.png?raw=true)

The view is divided into 6 different parts:
- Statistic information about all tracks
- Track and track segment selection
- Statistic information about the selected track or track segment
- Map showing all tracks
- Altitude and speed diagram of the selected track or track segment
- List of the track points of the selected track or track segment

Except the map all other parts can be hid to maximize the map view.

Features & Functions
--------------------
- Support opening GPX (v1.0 or v1.1) and NMEA files
- Merge several tracks into one single GPX file
- Rearrange the tracks (move/add/delete)
- Modify the meta data of the GPX file and of any tracks inside the file
- Get altitude data from the SRTM database
- Show statistic information about the summarized tracks and about a single track
- Show a map of all tracks
- Show an altitude and a speed diagram
- Show a list of the track points

Supported File Formats
----------------------
Currently only GPX and NMEA files are supported. If you have a file in another format you may use GPSBabel (http://www.gpsbabel.org) to convert it into a GPX file.

License
-------
This software is free software and licensed under GPL version 3. 

Compiling
---------
The source code was developed under Qt 5.3.2. There is no guarantee that the code will compile with previous or even later versions. 

The project is separated in two sub-projects GPXLab and QMapControl. QMapControl is a widget to display a map. Since I had to made some modifications to the original code I added the modified version here. QMapControl is compiled as a library and linked to the main sub-project GPXLab.

Both sub-projects are ready to build although it was only tested under Windows.
 
Credits
-------
- **QCustomPlot:** http://www.qcustomplot.com
- **QMapControl:** http://www.medieninf.de/qmapcontrol
- **Nuvola:** http://www.icon-king.com/projects/nuvola
 
Release Notes
-------------
**[0.1]**
- Initial beta release

Information
-----------
**Author:** Frédéric Bourgeois <bourgeoislab@gmail.com>

**Project homepage:** http://bourgeoislab.wordpress.com/gpxlab

**Latest sources:** http://github.com/bourgeoislab/gpxlab
