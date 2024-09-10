# RobloxDumper x86
 
* uniq
* ([@plusgiant5](https://github.com/plusgiant5))

### Roblox offsets dumper
This project includes a simple DLL injector, memory libraries, dumps formatter and usefull informations to study Roblox's internals.

### Example dump
[04-27-2023.json](Examples/04-27-2023.json)

<img src="https://github.com/uniquadev/RobloxDumper/blob/main/Examples/19-04-23.png?raw=true" width="450" height="450" />

## Motivation
Our aim is to help developers understand how to automate dumping processes and gain insights into the techniques employed by the old Roblox client to protect its codebase.

## Building
This project is built using Visual Studio. You can download it [here](https://visualstudio.microsoft.com/downloads/).

To make development easier the dumping process can be started directly from the IDE trough the use of the custom injector.
The project is configured to automatically attach a debugger to the Roblox process waiting for the DLL to be loaded.

## License
This project is licensed under the MIT License, allowing for open use and distribution while maintaining the original copyright notice.
