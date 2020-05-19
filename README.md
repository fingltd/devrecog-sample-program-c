<p align="center">
  <h1 align="center">Fing</h1>
  <a href="https://example.com/">
    <img src="https://is3-ssl.mzstatic.com/image/thumb/Purple123/v4/eb/71/e7/eb71e77e-4171-e679-81eb-e41835eb4361/AppIcon-0-1x_U007emarketing-0-0-85-220-0-7.png/246x0w.jpg" 
    alt="Fing" width=72 height=72>
  </a>
</p>

Table of contents
-----------------

- [Description](#Description)
- [Dependencies](#Dependencies)
- [Build](#Build)
- [Authors](#Authors)
- [License](#License) 

Description
------------
This folder contains the source code of a sample demo program exploiting Fing SDK 
for network scanning and device recognition.

The __Fing SDK for Device Recognition__ (<u>Fing Kit</u>) is a software designed 
to provide device scanning and device recognition technology.

It consists of two components:

1. The local scanner, which scans the network for devices
2. The device recognition API, providing the recognition technology
   
The Fing SDK is delivered as a library for Windows, Linux, MAC OSx,
OpenWRT and other Unix-like platforms. 
 
#### Fing Kit SDK

The Fing SDK is a lightweight development kit containing a portable C Header, the libraries and some working example to simplify users' job.
The available release can be found [here](https://app.fing.com/internet/business/devrecog/documentation).

__Fing SDK requires a license key to work. [Create a trial license](https://app.fing.com/internet/business/devrecog/trial) 
or [contact us](mailto:sales@fing.com) to get a valid key.__ 

The current documentation is available [here](https://get.fing.com/fing-business/devrecog/documentation/Fing_Desktop_Embedded_SDK.pdf).

```text
.
├── example/
│   ├── bin
│   │   └── FingKitDemo.bin
│   ├── fingkitdemo
│   └── src
│       ├── FingKitDemo.c
│       └── FingKitDemoCpp.cpp
├── include/
│   └── fingkit.h
└── lib/
    ├── ...
    └── ...
```

The ```lib/``` folder contains all the dependencies in different format: 
   * ```.dll``` for Windows
   * ```.so``` for Linux / Unix / OpenWRT
   * ```.dylib``` for MAC OSx
   
The ```include/``` folder hold the library interface that should be included
by the integrator.

The ```example/``` folder contains the demo programs with the source code.
On Windows the executable ```FingKitDemo.bin``` is called 
``` FingKitDemo``` and the script ```fingkitdemo``` is called 
```runFingKitDemo.bat```

Dependencies
-----

This sample project comes with all the dependencies:

- [boost](https://www.boost.org/users/history/version_1_58_0.html) (v1.58.0)
- [protobuf](https://github.com/protocolbuffers/protobuf/releases/tag/v2.6.1) (v2.6.1)
- [libpcap](https://www.tcpdump.org/release/) (v1.6.2)
- [netsnmp](http://www.net-snmp.org/download.html) (v5.7.3)
- [openssl](https://www.openssl.org/source/) (v1.0.x)
- [zlib](https://www.zlib.net/) (v1.2.x) 

Build
-----

All the package are built using [CMake](https://cmake.org/) as build, 
test and packaging tools.

The ```build/``` directory contains some utilities to create packages 
for the variety of supported operating systems.

#### Windows

From a PC mounting a Windows 7 or greater from the folder run the script:

```
build\windows-cmake-build-mscv9-solution.bat
```

It creates a Visual Studio project. The documentation of Visual Studio 9 2008 is available [here](https://docs.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2008) 

#### MAC OSX / Linux / Unix

```bash
cd build
cmake ..
make
```

Authors
--------

**Project Owner**

- Carlo Medas (carlo at fing.com)

**Contributors**

- Giuseppe Badoni (giuseppe at fing.com)
- Tommaso Latini (tommaso at fing.com)

License
--------

Code released under the [MIT License](https://reponame/blob/master/LICENSE).



