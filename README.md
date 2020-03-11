GPXLab
======
[![Packaging status](https://repology.org/badge/tiny-repos/gpxlab.svg)](https://repology.org/metapackage/gpxlab/versions)

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

Development
-----------
The project is separated in two sub-projects GPXLab and QMapControl. QMapControl is a widget to display a map. Since I had to made some modifications to the original code I added the modified version here. QMapControl is compiled as a library and linked to the main sub-project GPXLab.

Get latest source from master branch using:

```bash
$ git clone https://github.com/BourgeoisLab/GPXLab/
```

Compile source using:

```bash
$ cd GPXLab
$ lrelease GPXLab/GPXLab.pro
$ qmake
$ make
```

Credits
-------
- **QCustomPlot Qt Widget:** http://www.qcustomplot.com
- **QMapControl Qt Widget:** https://sourceforge.net/projects/qmapcontrol/
- **Nuvola Icons:** https://www.iconfinder.com/iconsets/nuvola2
- **FatCow Icons:** http://www.fatcow.com/free-icons
 
Release Notes
-------------

See [Changelog](CHANGELOG.md).

Information
-----------
**Author:** Frédéric Bourgeois <bourgeoislab@gmail.com>

**Latest release:** http://github.com/bourgeoislab/gpxlab/releases

**Latest sources:** http://github.com/bourgeoislab/gpxlab
