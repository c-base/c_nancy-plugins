# uccnc_msgflo_plugin
A msgflo plugin for UCCNC which publishes c_nancy activity to c-base c-flo MQTT network

### Description ###
- This UCCNC plugin is **NOT** written in .NET but C++
- The .NET assembly which is loaded by UCCNC only acts as a gateway between C++ dll and .NET plugin interface
- The assembly loads the C++ dll after it got loaded by UCCNC

### How to use ###
- Open **msgflo.sln** and compile using Visual Studio 2017
- copy **msgflo_gateway.dll** to **Plugins** directory
- copy **msgflo.dll** to **Plugins/cpp** directory

### Features ###
- Use modern C++ without any feature limitations
- Debug using printf
  - UCCNC has a copyprotection which prevents you attaching the visual studio debugger to UCCNC. This makes it hard to debug your plugin. When compiling the C++ dll in debug configuration a console is attached to UCCNC so you can debug using printf.

### Reasons for using C++ ###
- The author has very few knowledge about C# / .NET but strong knowledge in C++
- The author was too lazy to learn C# just to write a plugin
- UCCNC is written in .NET 2.0. Therefore any plugin (the assembly) has also to be written in .NET 2.0. Assemblies written in higher .NET versions cannot be used. Unfortunately .NET 2.0 is so old that many libraries (like MQTT) are not available for it.
