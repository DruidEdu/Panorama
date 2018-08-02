# Panorama Plugin for Unreal Engine 4

This is a simple plugin that adds a new blueprint node called 'Render Panorama'.

The plugin takes photographs of the scene and sends them to the file location you specify in the format you specify (presently only bitmap supported)

There two different kinds of photographs the CubeMap and the Equirectangular. 

The cube Map takes a photograph down each of the 6 axes and composites them into the Unreal Engine 4 CubeMap format.

The Equirectangular takes a 360 degree photograph of the scene. (There is a bug in this feature at the moment)

Instructions

1) To use this plugin, copy it into the plugins folder in your project e.g. MySuperCoolGame/Plugins

2) right click on your project file e.g. MySuperCoolGame.uproject and select 'Generate Visual Studio Project Files'

3) After that is finished open your project and all going well you will have no errors.

4) To use this plugin make a blueprint or use an existing one (such as the level blueprint) right click and add a push key node. Then right click and add a 'Render Panorama' Node choose your settings.

5) Run your project and push the button your mapped and you should have a cubmap saved in the Official ue4 format as a bitmap at the location you saved.

