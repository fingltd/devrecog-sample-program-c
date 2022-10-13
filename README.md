<div align="center">
    <a href="https://example.com/">
    <img src="https://is3-ssl.mzstatic.com/image/thumb/Purple123/v4/eb/71/e7/eb71e77e-4171-e679-81eb-e41835eb4361/AppIcon-0-1x_U007emarketing-0-0-85-220-0-7.png/246x0w.jpg" 
         alt="Fing" width=72 height=72>
  </a>
</div>

Table of contents
-----------------

- [Description](#Description)
- [Dependencies](#Dependencies)
- [Build](#Build)
   1. [Windows](#Windows)
   2. [Linux](#Linux)
   3. [MAC OSx](#MACOSX)
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
OpenWRT and other Unix-like platforms and can be downloaded from here:

| Name | Description | Link |
| ---- | ----------- | ---- |
| Windows SDK | Fing Kit for Windows Operating System | [Latest](https://get.fing.com/fing-business/devrecog/releases/sdk/windows/fingkit.zip)
| MAC OSx SDK | Fing Kit for Mac OSx | [Latest](https://get.fing.com/fing-business/devrecog/releases/sdk/macosx/fingkit.zip)

__M1 processor (aarch64) are not supported yet. Please open a ticket if you want to try the SDK for that architecture!__

Below the link for Linux-based operating system:

| Architecture | Description | Link |
| ------------ | ----------- | ---- |
| AMD64 | Fing Kit for Linux x86 64 | [Latest](https://get.fing.com/fing-business/devrecog/releases/sdk/linux/fingkit_amd64.zip)
| i686 | Fing Kit for Linux i686 | [Latest](https://get.fing.com/fing-business/devrecog/releases/sdk/linux/fingkit_i686.zip)
| ARM | Fing Kit for Linux ARM | [Latest](https://get.fing.com/fing-business/devrecog/releases/sdk/linux/fingkit_arm.zip)
| ARMHF | Fing Kit for Linux ARM Hard Float | [Latest](https://get.fing.com/fing-business/devrecog/releases/sdk/linux/fingkit_armhf.zip)
| ARM64 | Fing Kit for Linux ARM 64 | [Latest](https://get.fing.com/fing-business/devrecog/releases/sdk/linux/fingkit_arm64.zip)
| MIPS | Fing Kit for Linux MIPS | [Latest](https://get.fing.com/fing-business/devrecog/releases/sdk/linux/fingkit_mips.zip)
| MIPS64 | Fing Kit for Linux MIPS 64| [Latest](https://get.fing.com/fing-business/devrecog/releases/sdk/linux/fingkit_mips64.zip)
| MIPSEL | Fing Kit for Linux MIPSEL | [Latest](https://get.fing.com/fing-business/devrecog/releases/sdk/linux/fingkit_mipsel.zip)
| MIPS64EL | Fing Kit for Linux MIPSEL 64 | [Latest](https://get.fing.com/fing-business/devrecog/releases/sdk/linux/fingkit_mips64el.zip)

__Fing SDK requires a license key to work. [Create a trial license](https://app.fing.com/internet/business/devrecog/trial) 
or [contact us](mailto:sales@fing.com) to get a valid key.__ 

The documentation can download from this link:

[Fing Desktop/Embedded SDK](https://get.fing.com/fing-business/devrecog/documentation/Fing_Desktop_Embedded_SDK.pdf)

The Fing SDK contains a portable C Header and the libraries along with some working example to simplify users' job.

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

All the dependencies are available in the Fing SDK:

- [boost](https://www.boost.org/users/history/version_1_58_0.html) (v1.58.0)
- [protobuf](https://github.com/protocolbuffers/protobuf/releases/tag/v2.6.1) (v2.6.1)
- [libpcap](https://www.tcpdump.org/release/) (v1.6.2)
- [netsnmp](http://www.net-snmp.org/download.html) (v5.7.3)
- [openssl](https://www.openssl.org/source/) (v1.0.x)
- [zlib](https://www.zlib.net/) (v1.2.x) 

Build
-----

All the package are built using [CMake](https://cmake.org/) as build, 
test and packaging tools. Below the list of CMake variable that can be set:

| Variable        | Description
| --------------- | -----------
| C_EXECUTABLE    | Name of C Executable
| CPP_EXECUTABLE  | Name of C++ Executable
| CS_EXECUTABLE   | Name of C# Executable

Before bulding the project on any platform it's mandatory to __copy the content 
of the ```lib/``` folder of Fing SDK into the ```lib/``` folder of the current 
project__.

The ```build/``` directory contains some utilities to create packages 
for the variety of supported operating systems.

#### Windows

From a PC mounting a Windows 7 or greater from the folder run the script:

```
build\windows-cmake-build-mscv9-solution.bat
```

It creates a Visual Studio project. The documentation of Visual Studio 9 2008 is available [here](https://docs.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2008) 

#### Linux

```bash
cd build
cmake ..
make
DYLD_LIBRARY_PATH=../lib ./devrecog-c
```

#### MACOSX 

```bash
cd build
cmake ..
make
LD_LIBRARY_PATH=../lib ./devrecog-c
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

Code released under the [MIT License](https://github.com/fingltd/devrecog-sample-program-c/blob/master/LICENSE).



