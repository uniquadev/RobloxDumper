# RobloxDumper x86

In response to the release of the new Roblox [Hyperion](https://devforum.roblox.com/t/welcoming-byfron-to-roblox/2018233) Antitamper system, we decided ([@plusgiant5](https://github.com/plusgiant5)) to comment and share the source code of our old windows Roblox client dumper.

> This project includes a simple DLL injector, memory libraries, dumps formatter and tons of usefull informations to study Roblox's internals.

Our aim is to help developers understand how to automate dumping processes and gain insights into the techniques employed by the old Roblox client to protect its codebase.

## Motivation
The motivation behind this project is to provide the community with valuable resources for studying Roblox's internals. By open-sourcing the code and sharing useful information, we hope to foster a better understanding of the platform's security mechanisms and inspire further research in this area.

## Building
This project is built using Visual Studio. You can download it [here](https://visualstudio.microsoft.com/downloads/).
To make development easier the dumping process can be started directly from the IDE trough the use of the custom injector.

### Debugging
The project is configured to automatically attach a debugger to the Roblox process waiting for the DLL to be loaded.

## License
This project is licensed under the MIT License, allowing for open use and distribution while maintaining the original copyright notice.

*Thank you for showing interest in our project. Happy studying and happy Roblox development! If you have any questions or need assistance, don't hesitate to reach out.*