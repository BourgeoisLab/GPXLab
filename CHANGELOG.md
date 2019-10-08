# Changelog

## v0.7.0 (in development)
- [new] Use SRTM version 3.0 global 1 arc second data
- [new] SRTM files must now be located in the QStandardPaths::AppDataLocation directory
- [new] Settings to automatically reload the last opened file on startup
- [new] Support arbitrary map provider (thanks to sikmir). Tested with:
  - default: `http://tile.openstreetmap.org/%1/%2/%3.png`
  - with query: `http://a.tile.thunderforest.com/cycle/%1/%2/%3.png?apikey=<my-api-key>`
  - with https: `https://a.tile.opentopomap.org/%1/%2/%3.png`
  - with explicit port: `https://a.tile.opentopomap.org:443/%1/%2/%3.png`

## v0.6.0
- [new] Height files must now be placed in configuration direcotry
- [new] Increased OSM max zoom to 19 (thanks to sikmir)
- [new] Set precision of generated lat/lon to 6 and of elevation to 2 (thanks to sikmir)
- [new] Added waypoints parsing (thanks to sikmir)

## v0.5.0
- [new] Standard shortcuts and menu accelerators added (thanks to sikmir)
- [new] Finnish & Russian translations (thanks to sikmir)
- [fix] Parsing with Locale different than English
- [fix] Case-sensitivity while parsing removed

## v0.4.2.1
- [fix] Increased gpx buffer size to 4096.
- [fix] Const in Settings::setValue(const QString &key, const QVariant &value)
 
## v0.4.2
- [new] Show latitude and longitude in the status bar.
- [new] Show speed also in min/km.
- [new] "Follow item" and "show only selected item" are store in the settings.
- [fix] Following selected item failed.

## v0.4.1
- [fix] Compilation with Linux should work now.
- [fix] Combination of atof() and locale.
 
## v0.4.0
- [update] Updated to Qt 5.7.
- [update] Updated QMapControl to version 0.9.7.9.
- [update] Updated QCustomPlot to version 1.3.2.
- [new] The diagram's x axis now show elapsed time instead of absoulte time.
- [fix] Corrected timezone problem in NMEA and GPX parser.

## v0.3.0
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
 
## v0.2.0 Beta
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

## v0.1.0 Beta
- [new] Initial beta release.
