Vector SIL Kit Release Version: 4.0.50
---------------------------------------------

The layout of this release bundle is as follows:

+ SilKit/
  - the binary Vector SIL Kit release including the object files,
    the C++ interface headers and CMake exported targets.

+ SilKit-Demos/
  - SIL Kit Demos in source form. The demos are preconfigured to use
    the pre-built binary distribution of the SilKit/ directory.

+ SilKit-Documentation/
  - Documentation in HTML and text format.

+ SilKit-Source/
  - The complete source tree of the SIL Kit with a limited set of
    third-party components.

+/ GatewayApplication/
  - A Gateway Application Demo that Receive CAN Frames from CanWriter 
    and send it into Ethernet Frame to an EthernetReader

+/ GatewayApplication/
  - A Gateway Applecation Demo that Receive Ethernet Frames from EthernetWriter 
    and send it into CAN Frame to a CanReader