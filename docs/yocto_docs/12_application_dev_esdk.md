# Application Development and the Extensible SDK (eSDK) Manual

# Introduction

## eSDK Introduction

Welcome to the Yocto Project Application Development and the Extensible
Software Development Kit (eSDK) manual. This manual
explains how to use both the Yocto Project extensible and standard
SDKs to develop applications and images.

All SDKs consist of the following:

-  *Cross-Development Toolchain*: This toolchain contains a compiler,
   debugger, and various associated tools.

-  *Libraries, Headers, and Symbols*: The libraries, headers, and
   symbols are specific to the image (i.e. they match the image
   against which the SDK was built).

-  *Environment Setup Script*: This `*.sh` file, once sourced, sets up the
   cross-development environment by defining variables and preparing for
   SDK use.

Additionally, an extensible SDK has tools that allow you to easily add
new applications and libraries to an image, modify the source of an
existing component, test changes on the target hardware, and easily
integrate an application into the **OpenEmbedded Build System**.

You can use an SDK to independently develop and test code that is
destined to run on some target machine. SDKs are completely
self-contained. The binaries are linked against their own copy of
`libc`, which results in no dependencies on the target system. To
achieve this, the pointer to the dynamic loader is configured at install
time since that path cannot be dynamically altered. This is the reason
for a wrapper around the `populate_sdk` and `populate_sdk_ext`
archives.

Another feature of the SDKs is that only one set of cross-compiler
toolchain binaries are produced for any given architecture. This feature
takes advantage of the fact that the target hardware can be passed to
`gcc` as a set of compiler options. Those options are set up by the
environment script and contained in variables such as
**CC** and
**LD**. This reduces the space needed
for the tools. Understand, however, that every target still needs its own
sysroot because those binaries are target-specific.

The SDK development environment consists of the following:

-  The self-contained SDK, which is an architecture-specific
   cross-toolchain and matching sysroots (target and native) all built
   by the OpenEmbedded build system (e.g. the SDK). The toolchain and
   sysroots are based on a **Metadata**
   configuration and extensions, which allows you to cross-develop on
   the host machine for the target hardware. Additionally, the
   extensible SDK contains the `devtool` functionality.

-  The Quick EMUlator (QEMU), which lets you simulate target hardware.
   QEMU is not literally part of the SDK. You must build and include
   this emulator separately. However, QEMU plays an important role in
   the development process that revolves around use of the SDK.

In summary, the extensible and standard SDK share many features.
However, the extensible SDK has powerful development tools to help you
more quickly develop applications. Here is a table that summarizes
the primary differences between the standard and extensible SDK types
when considering which to build:

+-----------------------+-----------------------+-----------------------+
| *Feature*             | *Standard SDK*        | *Extensible SDK*      |
+=======================+=======================+=======================+
| Toolchain             | Yes                   | Yes [1]_              |
+-----------------------+-----------------------+-----------------------+
| Debugger              | Yes                   | Yes [1]_              |
+-----------------------+-----------------------+-----------------------+
| Size                  | 100+ MBytes           | 1+ GBytes (or 300+    |
|                       |                       | MBytes for minimal    |
|                       |                       | w/toolchain)          |
+-----------------------+-----------------------+-----------------------+
| `devtool`           | No                    | Yes                   |
+-----------------------+-----------------------+-----------------------+
| Build Images          | No                    | Yes                   |
+-----------------------+-----------------------+-----------------------+
| Updateable            | No                    | Yes                   |
+-----------------------+-----------------------+-----------------------+
| Managed Sysroot [2]_  | No                    | Yes                   |
+-----------------------+-----------------------+-----------------------+
| Installed Packages    | No  [3]_              | Yes  [4]_             |
+-----------------------+-----------------------+-----------------------+
| Construction          | Packages              | Shared State          |
+-----------------------+-----------------------+-----------------------+

.. [1] Extensible SDK contains the toolchain and debugger if **SDK_EXT_TYPE**
       is "full" or **SDK_INCLUDE_TOOLCHAIN** is "1", which is the default.
.. [2] Sysroot is managed through the use of `devtool`. Thus, it is less
       likely that you will corrupt your SDK sysroot when you try to add
       additional libraries.
.. [3] You can add runtime package management to the standard SDK but it is not
       supported by default.
.. [4] You must build and make the shared state available to extensible SDK
       users for "packages" you want to enable users to install.

### The Cross-Development Toolchain

The **Cross-Development Toolchain** consists
of a cross-compiler, cross-linker, and cross-debugger that are used to
develop user-space applications for targeted hardware; in addition,
the extensible SDK comes with built-in `devtool`
functionality. This toolchain is created by running a SDK installer
script or through a **Build Directory** that is based on
your metadata configuration or extension for your targeted device. The
cross-toolchain works with a matching target sysroot.

### Sysroots

The native and target sysroots contain needed headers and libraries for
generating binaries that run on the target architecture. The target
sysroot is based on the target root filesystem image that is built by
the OpenEmbedded build system and uses the same metadata configuration
used to build the cross-toolchain.

### The QEMU Emulator

The QEMU emulator allows you to simulate your hardware while running
your application or image. QEMU is not part of the SDK but is
automatically installed and available if you have done any one of
the following:

-  Setup your **Build Host** as described in the /dev-manual/start
   section of the Yocto Project Development Tasks Manual.

-  installed the cross-toolchain tarball and
   sourced the toolchain's setup environment script.

## SDK Development Model

Fundamentally, the SDK fits into the development process as follows:

![sdk-environment.*](svg/sdk-environment.*)

The SDK is installed on any machine and can be used to develop applications,
images, and kernels. An SDK can even be used by a QA Engineer or Release
Engineer. The fundamental concept is that the machine that has the SDK
installed does not have to be associated with the machine that has the
Yocto Project installed. A developer can independently compile and test
an object on their machine and then, when the object is ready for
integration into an image, they can simply make it available to the
machine that has the Yocto Project. Once the object is available, the
image can be rebuilt using the Yocto Project to produce the modified
image.

You just need to follow these general steps:

#. *Install the SDK for your target hardware:* For information on how to
   install the SDK, see the "sdk-manual/using:installing the sdk"
   section.

#. *Download or Build the Target Image:* The Yocto Project supports
   several target architectures and has many pre-built kernel images and
   root filesystem images.

   If you are going to develop your application on hardware, go to the
   machines  download area and choose a
   target machine area from which to download the kernel image and root
   filesystem. This download area could have several files in it that
   support development using actual hardware. For example, the area
   might contain `.hddimg` files that combine the kernel image with
   the filesystem, boot loaders, and so forth. Be sure to get the files
   you need for your particular development process.

   If you are going to develop your application and then run and test it
   using the QEMU emulator, go to the
   machines/qemu  download area. From this
   area, go down into the directory for your target architecture (e.g.
   `qemux86_64` for an Intel-based 64-bit architecture). Download the
   kernel, root filesystem, and any other files you need for your
   process.

   .. note::

      To use the root filesystem in QEMU, you need to extract it. See the
      "sdk-manual/appendix-obtain:extracting the root filesystem"
      section for information on how to do this extraction.

#. *Develop and Test your Application:* At this point, you have the
   tools to develop your application. If you need to separately install
   and use the QEMU emulator, you can go to `QEMU Home
   Page <https://wiki.qemu.org/Main_Page>`__ to download and learn about
   the emulator. See the "/dev-manual/qemu" chapter in the
   Yocto Project Development Tasks Manual for information on using QEMU
   within the Yocto Project.

The remainder of this manual describes how to use the extensible and
standard SDKs. There is also information in appendix form describing
how you can build, install, and modify an SDK.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Using the Extensible SDK

This chapter describes the extensible SDK and how to install it.
Information covers the pieces of the SDK, how to install it, and
presents a look at using the `devtool` functionality. The extensible
SDK makes it easy to add new applications and libraries to an image,
modify the source for an existing component, test changes on the target
hardware, and ease integration into the rest of the
**OpenEmbedded Build System**.

> **Note:**
> For a side-by-side comparison of main features supported for an
> extensible SDK as compared to a standard SDK, see the
> sdk-manual/intro:introduction section.
>

In addition to the functionality available through `devtool`, you can
alternatively make use of the toolchain directly, for example from
Makefile and Autotools. See the
"sdk-manual/working-projects:using the sdk toolchain directly" chapter
for more information.

## Why use the Extensible SDK and What is in It?

The extensible SDK provides a cross-development toolchain and libraries
tailored to the contents of a specific image. You would use the
Extensible SDK if you want a toolchain experience supplemented with the
powerful set of `devtool` commands tailored for the Yocto Project
environment.

The installed extensible SDK consists of several files and directories.
Basically, it contains an SDK environment setup script, some
configuration files, an internal build system, and the `devtool`
functionality.

## Installing the Extensible SDK

### Two ways to install the Extensible SDK

Extensible SDK can be installed in two different ways, and both have
their own pros and cons:

#. *Setting up the Extensible SDK environment directly in a Yocto build*. This
   avoids having to produce, test, distribute and maintain separate SDK
   installer archives, which can get very large. There is only one environment
   for the regular Yocto build and the SDK and less code paths where things can
   go not according to plan. It's easier to update the SDK: it simply means
   updating the Yocto layers with git fetch or layer management tooling. The
   SDK extensibility is better than in the second option: just run `bitbake`
   again to add more things to the sysroot, or add layers if even more things
   are required.

#. *Setting up the Extensible SDK from a standalone installer*. This has the
   benefit of having a single, self-contained archive that includes all the
   needed binary artifacts. So nothing needs to be rebuilt, and there is no
   need to provide a well-functioning binary artefact cache over the network
   for developers with underpowered laptops.

### Setting up the Extensible SDK environment directly in a Yocto build

#. Set up all the needed layers and a Yocto **Build Directory**, e.g. a regular Yocto
   build where `bitbake` can be executed.

#. Run:

```console
   $ bitbake meta-ide-support
   $ bitbake -c populate_sysroot gtk+3
   # or any other target or native item that the application developer would need
   $ bitbake build-sysroots -c build_native_sysroot && bitbake build-sysroots -c build_target_sysroot
```

### Setting up the Extensible SDK from a standalone installer

The first thing you need to do is install the SDK on your :term:`Build
Host` by running the `*.sh` installation script.

You can download a tarball installer, which includes the pre-built
toolchain, the `runqemu` script, the internal build system,
`devtool`, and support files from the appropriate
toolchain  directory within the Index of
Releases. Toolchains are available for several 32-bit and 64-bit
architectures with the `x86_64` directories, respectively. The
toolchains the Yocto Project provides are based off the
`core-image-sato` and `core-image-minimal` images and contain
libraries appropriate for developing against that image.

The names of the tarball installer scripts are such that a string
representing the host system appears first in the filename and then is
immediately followed by a string representing the target architecture.
An extensible SDK has the string "-ext" as part of the name. Following
is the general form:

.. parsed-literal::

   poky-glibc-*host_system*-*image_type*-*arch*-toolchain-ext-*release_version*.sh

Where:

-  *host_system* is a string representing your development system: `i686` or `x86_64`.

-  *image_type* is the image for which the SDK was built: `core-image-sato` or `core-image-minimal`.

-  *arch* is a string representing the tuned target architecture: `aarch64`, `armv5e`, `core2-64`, `i586`, `mips32r2`, `mips64`, `ppc7400`, or `cortexa8hf-neon`.

-  *release_version* is a string representing the release number of the Yocto Project: `&DISTRO;`, `&DISTRO;+snapshot`.

For example, the following SDK installer is for a 64-bit
development host system and a i586-tuned target architecture based off
the SDK for `core-image-sato` and using the current &DISTRO; snapshot:

```text
poky-glibc-x86_64-core-image-sato-i586-toolchain-ext-&DISTRO;.sh
```

> **Note:**
> As an alternative to downloading an SDK, you can build the SDK
> installer. For information on building the installer, see the
> sdk-manual/appendix-obtain:building an sdk installer
> section.
>

The SDK and toolchains are self-contained and by default are installed
into the `poky_sdk` folder in your home directory. You can choose to
install the extensible SDK in any location when you run the installer.
However, because files need to be written under that directory during
the normal course of operation, the location you choose for installation
must be writable for whichever users need to use the SDK.

The following command shows how to run the installer given a toolchain
tarball for a 64-bit x86 development host system and a 64-bit x86 target
architecture. The example assumes the SDK installer is located in
`~/Downloads/` and has execution rights:

```console
$ ./Downloads/poky-glibc-x86_64-core-image-minimal-core2-64-toolchain-ext-2.5.sh
Poky (Yocto Project Reference Distro) Extensible SDK installer version 2.5
==========================================================================
Enter target directory for SDK (default: poky_sdk):
You are about to install the SDK to "/home/scottrif/poky_sdk". Proceed [Y/n]? Y
Extracting SDK..............done
Setting it up...
Extracting buildtools...
Preparing build system...
Parsing recipes: 100% |##################################################################| Time: 0:00:52
Initialising tasks: 100% |###############################################################| Time: 0:00:00
Checking sstate mirror object availability: 100% |#######################################| Time: 0:00:00
Loading cache: 100% |####################################################################| Time: 0:00:00
Initialising tasks: 100% |###############################################################| Time: 0:00:00
done
SDK has been successfully set up and is ready to be used.
Each time you wish to use the SDK in a new shell session, you need to source the environment setup script e.g.
 $ . /home/scottrif/poky_sdk/environment-setup-core2-64-poky-linux
```

> **Note:**
> If you do not have write permissions for the directory into which you
> are installing the SDK, the installer notifies you and exits. For
> that case, set up the proper permissions in the directory and run the
> installer again.
>

## Running the Extensible SDK Environment Setup Script

Once you have the SDK installed, you must run the SDK environment setup
script before you can actually use the SDK.

When using an SDK directly in a Yocto build, you will find the script in
`tmp/deploy/images/qemux86-64/` in your **Build Directory**.

When using a standalone SDK installer, this setup script resides in
the directory you chose when you installed the SDK, which is either the
default `poky_sdk` directory or the directory you chose during
installation.

Before running the script, be sure it is the one that matches the
architecture for which you are developing. Environment setup scripts
begin with the string "`environment-setup`" and include as part of
their name the tuned target architecture. As an example, the following
commands set the working directory to where the SDK was installed and
then source the environment setup script. In this example, the setup
script is for an IA-based target machine using i586 tuning:

```console
$ cd /home/scottrif/poky_sdk
$ source environment-setup-core2-64-poky-linux
SDK environment now set up; additionally you may now run devtool to perform development tasks.
Run devtool --help for further details.
```

When using the environment script directly in a Yocto build, it can
be run similarly:

```console
$ source tmp/deploy/images/qemux86-64/environment-setup-core2-64-poky-linux
```

Running the setup script defines many environment variables needed in order to
use the SDK (e.g. `PATH`, **CC**, **LD**, and so forth). If you want
to see all the environment variables the script exports, examine the
installation file itself.

## Using ``devtool`` in Your SDK Workflow

The cornerstone of the extensible SDK is a command-line tool called
`devtool`. This tool provides a number of features that help you
build, test and package software within the extensible SDK, and
optionally integrate it into an image built by the OpenEmbedded build
system.

> **Note:**
> The use of `devtool` is not limited to the extensible SDK. You can use
> `devtool` to help you easily develop any project whose build output must be
> part of an image built using the build system.
>

The `devtool` command line is organized similarly to
overview-manual/development-environment:git in that it has a number of
sub-commands for each function. You can run `devtool --help` to see
all the commands.

> **Note:**
> See the "/ref-manual/devtool-reference"
> section in the Yocto Project Reference Manual.
>

`devtool` subcommands provide entry-points into development:

-  *devtool add*: Assists in adding new software to be built.

-  *devtool modify*: Sets up an environment to enable you to modify
   the source of an existing component.

-  *devtool ide-sdk*: Generates a configuration for an IDE.

-  *devtool upgrade*: Updates an existing recipe so that you can
   build it for an updated set of source files.

As with the build system, "recipes" represent software packages within
`devtool`. When you use `devtool add`, a recipe is automatically
created. When you use `devtool modify`, the specified existing recipe
is used in order to determine where to get the source code and how to
patch it. In both cases, an environment is set up so that when you build
the recipe a source tree that is under your control is used in order to
allow you to make changes to the source as desired. By default, new
recipes and the source go into a "workspace" directory under the SDK.

To learn how to use `devtool` to add, modify, upgrade recipes and more, see
the dev-manual/devtool:Using the \\`devtool\`\` command-line tool`
section of the Yocto Project Development Tasks Manual.

## Installing Additional Items Into the Extensible SDK

Out of the box the extensible SDK typically only comes with a small
number of tools and libraries. A minimal SDK starts mostly empty and is
populated on-demand. Sometimes you must explicitly install extra items
into the SDK. If you need these extra items, you can first search for
the items using the `devtool search` command. For example, suppose you
need to link to libGL but you are not sure which recipe provides libGL.
You can use the following command to find out:

```console
$ devtool search libGL mesa
A free implementation of the OpenGL API
```

Once you know the recipe
(i.e. `mesa` in this example), you can install it.

### When using the extensible SDK directly in a Yocto build

In this scenario, the Yocto build tooling, e.g. `bitbake`
is directly accessible to build additional items, and it
can simply be executed directly:

```console
$ bitbake curl-native
# Add newly built native items to native sysroot
$ bitbake build-sysroots -c build_native_sysroot
$ bitbake mesa
# Add newly built target items to target sysroot
$ bitbake build-sysroots -c build_target_sysroot
```

### When using a standalone installer for the Extensible SDK

```console
$ devtool sdk-install mesa
```

By default, the `devtool sdk-install` command assumes
the item is available in pre-built form from your SDK provider. If the
item is not available and it is acceptable to build the item from
source, you can add the "-s" option as follows:

```console
$ devtool sdk-install -s mesa
```

It is important to remember that building the item from source
takes significantly longer than installing the pre-built artifact. Also,
if there is no recipe for the item you want to add to the SDK, you must
instead add the item using the `devtool add` command.

## Applying Updates to an Installed Extensible SDK

If you are working with an installed extensible SDK that gets
occasionally updated (e.g. a third-party SDK), then you will need to
manually "pull down" the updates into the installed SDK.

To update your installed SDK, use `devtool` as follows:

```console
$ devtool sdk-update
```

The previous command assumes your SDK provider has set the default update URL
for you through the **SDK_UPDATE_URL** variable as described in the
"sdk-manual/appendix-customizing:Providing Updates to the Extensible SDK After Installation"
section. If the SDK provider has not set that default URL, you need to
specify it yourself in the command as follows:

```console
$ devtool sdk-update path_to_update_directory
```

> **Note:**
> The URL needs to point specifically to a published SDK and not to an
> SDK installer that you would download and install.
>

## Creating a Derivative SDK With Additional Components

You might need to produce an SDK that contains your own custom
libraries. A good example would be if you were a vendor with customers
that use your SDK to build their own platform-specific software and
those customers need an SDK that has custom libraries. In such a case,
you can produce a derivative SDK based on the currently installed SDK
fairly easily by following these steps:

#. If necessary, install an extensible SDK that you want to use as a
   base for your derivative SDK.

#. Source the environment script for the SDK.

#. Add the extra libraries or other components you want by using the
   `devtool add` command.

#. Run the `devtool build-sdk` command.

The previous steps take the recipes added to the workspace and construct
a new SDK installer that contains those recipes and the resulting binary
artifacts. The recipes go into their own separate layer in the
constructed derivative SDK, which leaves the workspace clean and ready
for users to add their own recipes.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Using the Standard SDK

This chapter describes the standard SDK and how to install it.
Information includes unique installation and setup aspects for the
standard SDK.

> **Note:**
> For a side-by-side comparison of main features supported for a
> standard SDK as compared to an extensible SDK, see the
> "sdk-manual/intro:introduction" section.
>

You can use a standard SDK to work on Makefile and Autotools-based
projects. See the
"sdk-manual/working-projects:using the sdk toolchain directly" chapter
for more information.

## Why use the Standard SDK and What is in It?

The Standard SDK provides a cross-development toolchain and libraries
tailored to the contents of a specific image. You would use the Standard
SDK if you want a more traditional toolchain experience as compared to
the extensible SDK, which provides an internal build system and the
`devtool` functionality.

The installed Standard SDK consists of several files and directories.
Basically, it contains an SDK environment setup script, some
configuration files, and host and target root filesystems to support
usage. You can see the directory structure in the
"sdk-manual/appendix-obtain:installed standard sdk directory structure"
section.

## Installing the SDK

The first thing you need to do is install the SDK on your :term:`Build
Host` by running the `*.sh` installation script.

You can download a tarball installer, which includes the pre-built
toolchain, the `runqemu` script, and support files from the
appropriate toolchain  directory within
the Index of Releases. Toolchains are available for several 32-bit and
64-bit architectures with the `x86_64` directories, respectively. The
toolchains the Yocto Project provides are based off the
`core-image-sato` and `core-image-minimal` images and contain
libraries appropriate for developing against the corresponding image.

The names of the tarball installer scripts are such that a string
representing the host system appears first in the filename and then is
immediately followed by a string representing the target architecture:

.. parsed-literal::

   poky-glibc-*host_system*-*image_type*-*arch*-toolchain-*release_version*.sh

Where:

-  *host_system* is a string representing your development system: `i686` or `x86_64`.

-  *image_type* is the image for which the SDK was built: `core-image-minimal` or `core-image-sato`.

-  *arch* is a string representing the tuned target architecture: `aarch64`, `armv5e`, `core2-64`, `i586`, `mips32r2`, `mips64`, `ppc7400`, or `cortexa8hf-neon`.

-  *release_version* is a string representing the release number of the Yocto Project: `&DISTRO;`, `&DISTRO;+snapshot`.

For example, the following SDK installer is for a 64-bit
development host system and a i586-tuned target architecture based off
the SDK for `core-image-sato` and the `&DISTRO;` release:

```text
poky-glibc-x86_64-core-image-sato-i586-toolchain-&DISTRO;.sh
```

> **Note:**
> As an alternative to downloading an SDK, you can build the SDK
> installer. For information on building the installer, see the
> "sdk-manual/appendix-obtain:building an sdk installer"
> section.
>

The SDK and toolchains are self-contained and by default are installed
into the `poky_sdk` folder in your home directory. You can choose to
install the extensible SDK in any location when you run the installer.
However, because files need to be written under that directory during
the normal course of operation, the location you choose for installation
must be writable for whichever users need to use the SDK.

The following command shows how to run the installer given a toolchain
tarball for a 64-bit x86 development host system and a 64-bit x86 target
architecture. The example assumes the SDK installer is located in
`~/Downloads/` and has execution rights:

```console
$ ./Downloads/poky-glibc-x86_64-core-image-sato-i586-toolchain-&DISTRO;.sh
Poky (Yocto Project Reference Distro) SDK installer version &DISTRO;
===============================================================
Enter target directory for SDK (default: /opt/poky/&DISTRO;):
You are about to install the SDK to "/opt/poky/&DISTRO;". Proceed [Y/n]? Y
Extracting SDK........................................ ..............................done
Setting it up...done
SDK has been successfully set up and is ready to be used.
Each time you wish to use the SDK in a new shell session, you need to source the environment setup script e.g.
 $ . /opt/poky/&DISTRO;/environment-setup-i586-poky-linux
```

> **Note:**
> If you do not have write permissions for the directory into which you
> are installing the SDK, the installer notifies you and exits. For
> that case, set up the proper permissions in the directory and run the
> installer again.
>

Again, reference the
"sdk-manual/appendix-obtain:installed standard sdk directory structure"
section for more details on the resulting directory structure of the installed
SDK.

## Running the SDK Environment Setup Script

Once you have the SDK installed, you must run the SDK environment setup
script before you can actually use the SDK. This setup script resides in
the directory you chose when you installed the SDK, which is either the
default `/opt/poky/&DISTRO;` directory or the directory you chose during
installation.

Before running the script, be sure it is the one that matches the
architecture for which you are developing. Environment setup scripts
begin with the string "`environment-setup`" and include as part of
their name the tuned target architecture. As an example, the following
commands set the working directory to where the SDK was installed and
then source the environment setup script. In this example, the setup
script is for an IA-based target machine using i586 tuning:

```console
$ source /opt/poky/&DISTRO;/environment-setup-i586-poky-linux
```

When you run the
setup script, the same environment variables are defined as are when you
run the setup script for an extensible SDK. See the
"sdk-manual/appendix-obtain:installed extensible sdk directory structure"
section for more information.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Using the SDK Toolchain Directly

You can use the SDK toolchain directly with Makefile and Autotools-based
projects.

## Autotools-Based Projects

Once you have a suitable sdk-manual/intro:the cross-development toolchain
installed, it is very easy to develop a project using the :wikipedia:`GNU
Autotools-based <GNU_Build_System>` workflow, which is outside of the
**OpenEmbedded Build System**.

The following figure presents a simple Autotools workflow.

![sdk-autotools-flow.png](figures/sdk-autotools-flow.png)

Follow these steps to create a simple Autotools-based "Hello World"
project:

> **Note:**
> For more information on the GNU Autotools workflow, see the same
> example on the
> GNOME Developer
> site.
>

#. *Create a Working Directory and Populate It:* Create a clean
   directory for your project and then make that directory your working
   location:

   .. code-block:: console

      $ mkdir $HOME/helloworld
      $ cd $HOME/helloworld

   After setting up the directory, populate it with files needed for the flow.
   You need a project source file, a file to help with configuration,
   and a file to help create the Makefile, and a README file:
   `hello.c`, `configure.ac`, `Makefile.am`, and `README`,
   respectively.

   Use the following command to create an empty README file, which is
   required by GNU Coding Standards:

   .. code-block:: console

      $ touch README

   Create the remaining
   three files as follows:

   -  `hello.c`:

      .. code-block:: c

         #include <stdio.h>

         int main()
             {
                 printf("Hello World!\n");
                 return 0;
             }

   -  `configure.ac`:

      .. code-block:: none

         AC_INIT(hello,0.1)
         AM_INIT_AUTOMAKE([foreign])
         AC_PROG_CC
         AC_CONFIG_FILES(Makefile)
         AC_OUTPUT

   -  `Makefile.am`:

      .. code-block:: none

         bin_PROGRAMS = hello
         hello_SOURCES = hello.c

#. *Source the Cross-Toolchain Environment Setup File:* As described
   earlier in the manual, installing the cross-toolchain creates a
   cross-toolchain environment setup script in the directory that the
   SDK was installed. Before you can use the tools to develop your
   project, you must source this setup script. The script begins with
   the string "environment-setup" and contains the machine architecture,
   which is followed by the string "poky-linux". For this example, the
   command sources a script from the default SDK installation directory
   that uses the 32-bit Intel x86 Architecture and the &DISTRO; Yocto
   Project release:

   .. code-block:: console

      $ source /opt/poky/&DISTRO;/environment-setup-i586-poky-linux

   Another example is sourcing the environment setup directly in a Yocto
   build:

   .. code-block:: console

      $ source tmp/deploy/images/qemux86-64/environment-setup-core2-64-poky-linux

#. *Create the configure Script:* Use the `autoreconf` command to
   generate the `configure` script:

   .. code-block:: console

      $ autoreconf

   The `autoreconf`
   tool takes care of running the other Autotools such as `aclocal`,
   `autoconf`, and `automake`.

   .. note::

      If you get errors from `configure.ac`, which `autoreconf`
      runs, that indicate missing files, you can use the "-i" option,
      which ensures missing auxiliary files are copied to the build
      host.

#. *Cross-Compile the Project:* This command compiles the project using
   the cross-compiler. The
   **CONFIGURE_FLAGS**
   environment variable provides the minimal arguments for GNU
   configure:

   .. code-block:: console

      $ ./configure ${CONFIGURE_FLAGS}

   For an Autotools-based
   project, you can use the cross-toolchain by just passing the
   appropriate host option to `configure.sh`. The host option you use
   is derived from the name of the environment setup script found in the
   directory in which you installed the cross-toolchain. For example,
   the host option for an ARM-based target that uses the GNU EABI is
   `armv5te-poky-linux-gnueabi`. You will notice that the name of the
   script is `environment-setup-armv5te-poky-linux-gnueabi`. Thus, the
   following command works to update your project and rebuild it using
   the appropriate cross-toolchain tools:

   .. code-block:: console

     $ ./configure --host=armv5te-poky-linux-gnueabi --with-libtool-sysroot=sysroot_dir

#. *Make and Install the Project:* These two commands generate and
   install the project into the destination directory:

   .. code-block:: console

      $ make
      $ make install DESTDIR=./tmp

   .. note::

      To learn about environment variables established when you run the
      cross-toolchain environment setup script and how they are used or
      overridden by the Makefile, see the
      sdk-manual/working-projects:makefile-based projects section.

   This next command is a simple way to verify the installation of your
   project. Running the command prints the architecture on which the
   binary file can run. This architecture should be the same
   architecture that the installed cross-toolchain supports:

   .. code-block:: console

      $ file ./tmp/usr/local/bin/hello

#. *Execute Your Project:* To execute the project, you would need to run
   it on your target hardware. If your target hardware happens to be
   your build host, you could run the project as follows:

   .. code-block:: console

      $ ./tmp/usr/local/bin/hello

   As expected, the project displays the "Hello World!" message.

## Makefile-Based Projects

Simple Makefile-based projects use and interact with the cross-toolchain
environment variables established when you run the cross-toolchain
environment setup script. The environment variables are subject to
general `make` rules.

This section presents a simple Makefile development flow and provides an
example that lets you see how you can use cross-toolchain environment
variables and Makefile variables during development.

![sdk-makefile-flow.png](figures/sdk-makefile-flow.png)

The main point of this section is to explain the following three cases
regarding variable behavior:

-  *Case 1 --- No Variables Set in the Makefile Map to Equivalent
   Environment Variables Set in the SDK Setup Script:* Because matching
   variables are not specifically set in the `Makefile`, the variables
   retain their values based on the environment setup script.

-  *Case 2 --- Variables Are Set in the Makefile that Map to Equivalent
   Environment Variables from the SDK Setup Script:* Specifically
   setting matching variables in the `Makefile` during the build
   results in the environment settings of the variables being
   overwritten. In this case, the variables you set in the `Makefile`
   are used.

-  *Case 3 --- Variables Are Set Using the Command Line that Map to
   Equivalent Environment Variables from the SDK Setup Script:*
   Executing the `Makefile` from the command line results in the
   environment variables being overwritten. In this case, the
   command-line content is used.

> **Note:**
> Regardless of how you set your variables, if you use the "-e" option
> with `make`, the variables from the SDK setup script take precedence:
>
> .. code-block:: console
>
> $ make -e target
>
>

The remainder of this section presents a simple Makefile example that
demonstrates these variable behaviors.

In a new shell environment variables are not established for the SDK
until you run the setup script. For example, the following commands show
a null value for the compiler variable (i.e.
**CC**):

```console
$ echo ${CC}

$
```

Running the
SDK setup script for a 64-bit build host and an i586-tuned target
architecture for a `core-image-sato` image using the current &DISTRO;
Yocto Project release and then echoing that variable shows the value
established through the script:

```console
$ source /opt/poky/&DISTRO;/environment-setup-i586-poky-linux
$ echo ${CC}
i586-poky-linux-gcc -m32 -march=i586 --sysroot=/opt/poky/&DISTRO;/sysroots/i586-poky-linux
```

To illustrate variable use, work through this simple "Hello World!"
example:

#. *Create a Working Directory and Populate It:* Create a clean
   directory for your project and then make that directory your working
   location:

   .. code-block:: console

      $ mkdir $HOME/helloworld
      $ cd $HOME/helloworld

   After
   setting up the directory, populate it with files needed for the flow.
   You need a `main.c` file from which you call your function, a
   `module.h` file to contain headers, and a `module.c` that defines
   your function.

   Create the three files as follows:

   -  `main.c`:

      .. code-block:: c

         #include "module.h"
         void sample_func();
         int main()
         {
             sample_func();
             return 0;
         }

   -  `module.h`:

      .. code-block:: c

         #include <stdio.h>
         void sample_func();

   -  `module.c`:

      .. code-block:: c

         #include "module.h"
         void sample_func()
         {
             printf("Hello World!");
             printf("\n");
         }

#. *Source the Cross-Toolchain Environment Setup File:* As described
   earlier in the manual, installing the cross-toolchain creates a
   cross-toolchain environment setup script in the directory that the
   SDK was installed. Before you can use the tools to develop your
   project, you must source this setup script. The script begins with
   the string "environment-setup" and contains the machine architecture,
   which is followed by the string "poky-linux". For this example, the
   command sources a script from the default SDK installation directory
   that uses the 32-bit Intel x86 Architecture and the &DISTRO_NAME; Yocto
   Project release:

   .. code-block:: console

      $ source /opt/poky/&DISTRO;/environment-setup-i586-poky-linux

   Another example is sourcing the environment setup directly in a Yocto
   build:

   .. code-block:: console

      $ source tmp/deploy/images/qemux86-64/environment-setup-core2-64-poky-linux

#. *Create the Makefile:* For this example, the Makefile contains
   two lines that can be used to set the **CC** variable. One line is
   identical to the value that is set when you run the SDK environment
   setup script, and the other line sets **CC** to "gcc", the default
   GNU compiler on the build host:

   .. code-block:: Makefile

      # CC=i586-poky-linux-gcc -m32 -march=i586 --sysroot=/opt/poky/2.5/sysroots/i586-poky-linux
      # CC="gcc"
      all: main.o module.o
        ${CC} main.o module.o -o target_bin
      main.o: main.c module.h
        ${CC} -I . -c main.c
      module.o: module.c module.h
        ${CC} -I . -c module.c
      clean:
        rm -rf *.o
        rm target_bin

#. *Make the Project:* Use the `make` command to create the binary
   output file. Because variables are commented out in the Makefile, the
   value used for **CC** is the value set when the SDK environment setup
   file was run:

   .. code-block:: console

      $ make
      i586-poky-linux-gcc -m32 -march=i586 --sysroot=/opt/poky/2.5/sysroots/i586-poky-linux -I . -c main.c
      i586-poky-linux-gcc -m32 -march=i586 --sysroot=/opt/poky/2.5/sysroots/i586-poky-linux -I . -c module.c
      i586-poky-linux-gcc -m32 -march=i586 --sysroot=/opt/poky/2.5/sysroots/i586-poky-linux main.o module.o -o target_bin

   From the results of the previous command, you can see that
   the compiler used was the compiler established through the **CC**
   variable defined in the setup script.

   You can override the **CC** environment variable with the same
   variable as set from the Makefile by uncommenting the line in the
   Makefile and running `make` again:

   .. code-block:: console

      $ make clean
      rm -rf *.o
      rm target_bin
# # Edit the Makefile by uncommenting the line that sets CC to "gcc"

      $ make
      gcc -I . -c main.c
      gcc -I . -c module.c
      gcc main.o module.o -o target_bin

   As shown in the previous example, the
   cross-toolchain compiler is not used. Rather, the default compiler is
   used.

   This next case shows how to override a variable by providing the
   variable as part of the command line. Go into the Makefile and
   re-insert the comment character so that running `make` uses the
   established SDK compiler. However, when you run `make`, use a
   command-line argument to set **CC** to "gcc":

   .. code-block:: console

      $ make clean
      rm -rf *.o
      rm target_bin
# # Edit the Makefile to comment out the line setting CC to "gcc"

      $ make
      i586-poky-linux-gcc  -m32 -march=i586 --sysroot=/opt/poky/2.5/sysroots/i586-poky-linux -I . -c main.c
      i586-poky-linux-gcc  -m32 -march=i586 --sysroot=/opt/poky/2.5/sysroots/i586-poky-linux -I . -c module.c
      i586-poky-linux-gcc  -m32 -march=i586 --sysroot=/opt/poky/2.5/sysroots/i586-poky-linux main.o module.o -o target_bin
      $ make clean
      rm -rf *.o
      rm target_bin
      $ make CC="gcc"
      gcc -I . -c main.c
      gcc -I . -c module.c
      gcc main.o module.o -o target_bin

   In the previous case, the command-line argument overrides the SDK
   environment variable.

   In this last case, edit Makefile again to use the "gcc" compiler but
   then use the "-e" option on the `make` command line:

   .. code-block:: console

      $ make clean
      rm -rf *.o
      rm target_bin
# # Edit the Makefile to use "gcc"

      $ make
      gcc -I . -c main.c
      gcc -I . -c module.c
      gcc main.o module.o -o target_bin
      $ make clean
      rm -rf *.o
      rm target_bin
      $ make -e
      i586-poky-linux-gcc  -m32 -march=i586 --sysroot=/opt/poky/2.5/sysroots/i586-poky-linux -I . -c main.c
      i586-poky-linux-gcc  -m32 -march=i586 --sysroot=/opt/poky/2.5/sysroots/i586-poky-linux -I . -c module.c
      i586-poky-linux-gcc  -m32 -march=i586 --sysroot=/opt/poky/2.5/sysroots/i586-poky-linux main.o module.o -o target_bin

   In the previous case, the "-e" option forces `make` to
   use the SDK environment variables regardless of the values in the
   Makefile.

#. *Execute Your Project:* To execute the project (i.e. `target_bin`),
   use the following command:

   .. code-block:: console

      $ ./target_bin
      Hello World!

   .. note::

      If you used the cross-toolchain compiler to build
      target_bin
      and your build host differs in architecture from that of the
      target machine, you need to run your project on the target device.

   As expected, the project displays the "Hello World!" message.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Obtaining the SDK

## Working with the SDK components directly in a Yocto build

Please refer to section
"sdk-manual/extensible:Setting up the Extensible SDK environment directly in a Yocto build"

Note that to use this feature effectively either a powerful build
machine, or a well-functioning sstate cache infrastructure is required:
otherwise significant time could be spent waiting for components to be built
by BitBake from source code.

## Working with standalone SDK Installers

### Locating Pre-Built SDK Installers

You can use existing, pre-built toolchains by locating and running an
SDK installer script that ships with the Yocto Project. Using this
method, you select and download an architecture-specific SDK installer
and then run the script to hand-install the toolchain.

Follow these steps to locate and hand-install the toolchain:

#. *Go to the Installers Directory:* Go to
   `/releases/yocto/&DISTRO_REL_LATEST_TAG;/toolchain/`

#. *Open the Folder for Your Build Host:* Open the folder that matches
   your **Build Host** (i.e.
   `i686` for 32-bit machines or `x86_64` for 64-bit machines).

#. *Locate and Download the SDK Installer:* You need to find and
   download the installer appropriate for your build host, target
   hardware, and image type.

   The installer files (`*.sh`) follow this naming convention:

   .. parsed-literal::

      poky-glibc-*host_system*-core-image-*type*-*arch*-toolchain[-ext]-*release*.sh

   With:

   -  *host_system*: string representing your development system: `i686` or `x86_64`

   -  *type*: string representing the image: `sato` or `minimal`

   -  *arch*: string representing the target architecture such as `cortexa57-qemuarm64`

   -  *release*: version of the Yocto Project.

   .. note::
      The standard SDK installer does not have the `-ext` string as
      part of the filename.

   The toolchains provided by the Yocto
   Project are based off of the `core-image-sato` and
   `core-image-minimal` images and contain libraries appropriate for
   developing against those images.

   For example, if your build host is a 64-bit x86 system and you need
   an extended SDK for a 64-bit core2 QEMU target, go into the `x86_64`
   folder and download the following installer:

   .. code-block:: text

      poky-glibc-x86_64-core-image-sato-core2-64-qemux86-64-toolchain-&DISTRO;.sh

#. *Run the Installer:* Be sure you have execution privileges and run
   the installer. Here is an example from the `Downloads`
   directory:

   .. code-block:: console

      $ ~/Downloads/poky-glibc-x86_64-core-image-sato-core2-64-qemux86-64-toolchain-&DISTRO;.sh

   During execution of the script, you choose the root location for the
   toolchain. See the
   "sdk-manual/appendix-obtain:installed standard sdk directory structure"
   section and the
   "sdk-manual/appendix-obtain:installed extensible sdk directory structure"
   section for more information.

### Building an SDK Installer

As an alternative to locating and downloading an SDK installer, you can
build the SDK installer. Follow these steps:

#. *Set Up the Build Environment:* Be sure you are set up to use BitBake
   in a shell. See the "/dev-manual/start" section
   in the Yocto Project Development Tasks Manual for information on how
   to get a build host ready that is either a native Linux machine or a
   machine that uses CROPS.

#. *Make Sure You Are Building an Installer for the Correct Machine:*
   Check to be sure that your **MACHINE** variable in the `local.conf`
   file in your **Build Directory** matches the architecture
   for which you are building.

#. *Make Sure Your SDK Machine is Correctly Set:* If you are building a
   toolchain designed to run on an architecture that differs from your
   current development host machine (i.e. the build host), be sure that
   the **SDKMACHINE** variable in the `local.conf` file in your
   **Build Directory** is correctly set.

   .. note::

      If you are building an SDK installer for the Extensible SDK, the
      **SDKMACHINE** value must be set for the architecture of the
      machine you are using to build the installer. If **SDKMACHINE**
      is not set appropriately, the build fails and provides an error
      message similar to the following:

      .. code-block:: text

         The extensible SDK can currently only be built for the same
         architecture as the machine being built on - SDK_ARCH
         is set to i686 (likely via setting SDKMACHINE) which is
         different from the architecture of the build machine (x86_64).
         Unable to continue.

#. *Build the SDK Installer:* To build the SDK installer for a standard
   SDK and populate the SDK image, use the following command form. Be
   sure to replace `image` with an image (e.g. "core-image-sato"):

   .. code-block:: console

      $ bitbake image -c populate_sdk

   You can do the same for the extensible SDK using this command form:

   .. code-block:: console

      $ bitbake image -c populate_sdk_ext

   These commands produce an SDK installer that contains the sysroot
   that matches your target root filesystem.

   When the `bitbake` command completes, the SDK installer will be in
   `tmp/deploy/sdk` in the **Build Directory**.

   .. note::

      -  By default, the previous BitBake command does not build static
         binaries. If you want to use the toolchain to build these types
         of libraries, you need to be sure your SDK has the appropriate
         static development libraries. Use the
         **TOOLCHAIN_TARGET_TASK**
         variable inside your `local.conf` file before building the
         SDK installer. Doing so ensures that the eventual SDK
         installation process installs the appropriate library packages
         as part of the SDK. Here is an example using `libc`
         static development libraries: TOOLCHAIN_TARGET_TASK:append = "
         libc-staticdev"

#. *Run the Installer:* You can now run the SDK installer from
   `tmp/deploy/sdk` in the **Build Directory**. Here is an example:

   .. code-block:: console

      $ cd bitbake-builds/build/tmp/deploy/sdk
      $ ./poky-glibc-x86_64-core-image-sato-core2-64-toolchain-ext-&DISTRO;.sh

   During execution of the script, you choose the root location for the
   toolchain. See the
   "sdk-manual/appendix-obtain:installed standard sdk directory structure"
   section and the
   "sdk-manual/appendix-obtain:installed extensible sdk directory structure"
   section for more information.

## Extracting the Root Filesystem

After installing the toolchain, for some use cases you might need to
separately extract a root filesystem:

-  You want to boot the image using NFS.

-  You want to use the root filesystem as the target sysroot.

-  You want to develop your target application using the root filesystem
   as the target sysroot.

Follow these steps to extract the root filesystem:

#. *Locate and Download the Tarball for the Pre-Built Root Filesystem
   Image File:* You need to find and download the root filesystem image
   file that is appropriate for your target system. These files are kept
   in machine-specific folders in the
   Index of Releases 
   in the "machines" directory.

   The machine-specific folders of the "machines" directory contain
   tarballs (`*.tar.bz2`) for supported machines. These directories
   also contain flattened root filesystem image files (`*.ext4`),
   which you can use with QEMU directly.

   The pre-built root filesystem image files follow this naming convention:

   .. parsed-literal::

      core-image-*profile*-*machine*.tar.bz2

   With:

   -  *profile*: filesystem image's profile, such as `minimal`,
      `minimal-dev` or `sato`. For information on these types of image
      profiles, see the "Images" chapter in the Yocto Project Reference Manual.

   -  *machine*:  same string as the name of the parent download directory.

   The root filesystems
   provided by the Yocto Project are based off of the
   `core-image-sato` and `core-image-minimal` images.

   For example, if you plan on using a BeagleBone device as your target
   hardware and your image is a `core-image-sato-sdk` image, you can
   download the following file:

   .. code-block:: text

      core-image-sato-sdk-beaglebone-yocto.tar.bz2

#. *Initialize the Cross-Development Environment:* You must `source`
   the cross-development environment setup script to establish necessary
   environment variables.

   This script is located in the top-level directory in which you
   installed the toolchain (e.g. `poky_sdk`).

   Here is an example based on the toolchain installed in the
   "sdk-manual/appendix-obtain:locating pre-built sdk installers" section:

   .. code-block:: console

      $ source poky_sdk/environment-setup-core2-64-poky-linux

#. *Extract the Root Filesystem:* Use the `runqemu-extract-sdk`
   command and provide the root filesystem image.

   Here is an example command that extracts the root filesystem
   from a previously built root filesystem image that was downloaded
   from the Index of Releases .
   This command extracts the root filesystem into the `core2-64-sato`
   directory:

   .. code-block:: console

      $ runqemu-extract-sdk ~/Downloads/core-image-sato-sdk-beaglebone-yocto.tar.bz2 ~/beaglebone-sato

   You could now point to the target sysroot at `beaglebone-sato`.

## Installed Standard SDK Directory Structure

After you install the Standard SDK by running the `*.sh` SDK installation
script, the following directory structure should be observed:

.. parsed-literal::

   *install_dir*/*version*/
   ├── buildinfo
   ├── environment-setup-*target*-poky-linux
   ├── site-config-*target*-poky-linux
   ├── sysroots/
   │   ├── *target*-poky-linux/
   │   │   ├── bin/
   │   │   ├── boot/
   │   │   ├── etc/
   │   │   ├── home/
   │   │   ├── lib/
   │   │   ├── media/
   │   │   ├── mnt/
   │   │   ├── proc/
   │   │   ├── run/
   │   │   ├── sbin/
   │   │   ├── sys/
   │   │   ├── tmp/
   │   │   ├── usr/
   │   │   └── var/
   │   └── *host*-pokysdk-linux/
   │       ├── bin/
   │       ├── environment-setup.d/
   │       ├── etc/
   │       ├── lib/
   │       ├── sbin/
   │       ├── usr/
   │       └── var/
   └── version-*target*-poky-linux

The installed SDK consists of an environment setup script for the SDK, a
configuration file for the target, a version file for the target, and
the root filesystem (`sysroots`) needed to develop objects for the
target system.

In the layout above, italicized text is used to indicate replaceable
portions of the file or directory name. For example, *install_dir*/*version*
is the directory where the SDK is installed. By default, *install_dir*
is `/opt/poky/`. And, *version* represents the specific snapshot of the
SDK (e.g. &DISTRO;). Furthermore, *target* represents the target architecture
(e.g. `i586`) and *host* represents the development system's
architecture (e.g. `x86_64`). Thus, the complete names of the two
directories within the `sysroots` could be `i586-poky-linux` and
`x86_64-pokysdk-linux` for the target and host, respectively.

## Installed Extensible SDK Directory Structure

After you install the Extensible SDK by running the `*.sh` SDK installation
script, the following directory structure should be observed:

.. parsed-literal::

   *install_dir*/
   ├── bitbake-cookerdaemon.log
   ├── buildinfo
   ├── buildtools/
   ├── cache/
   ├── conf/
   ├── .devtoolbase
   ├── downloads/
   ├── environment-setup-*target*-poky-linux
   ├── layers/
   ├── oe-time-dd-test.dat
   ├── preparing_system_build.log
   ├── site-config-*target*-poky-linux
   ├── sstate-cache/
   ├── sysroots/
   ├── tmp/
   ├── version-*target*-poky-linux
   └── workspace/

The installed directory structure for the extensible SDK is quite
different than the installed structure for the standard SDK. The
extensible SDK does not separate host and target parts in the same
manner as does the standard SDK. The extensible SDK uses an embedded
copy of the OpenEmbedded build system, which has its own sysroots.

Of note in the directory structure are an environment setup script for
the SDK, a configuration file for the target, a version file for the
target, and log files for the OpenEmbedded build system preparation
script run by the installer and BitBake.

In the layout above, italicized text is used to indicate replaceable
portions of the file or directory name. For example, *install_dir* is the
directory where the SDK is installed, which is by default `poky_sdk` in your
home directory and *target* represents the target architecture (e.g. `i586`).

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Customizing the Extensible SDK standalone installer

This appendix describes customizations you can apply to the extensible
SDK when using in the standalone installer version.

> **Note:**
> It is also possible to use the Extensible SDK functionality directly in a
> Yocto build, avoiding separate installer artefacts. Please refer to
> "sdk-manual/extensible:Installing the Extensible SDK"
>

## Configuring the Extensible SDK

The extensible SDK primarily consists of a pre-configured copy of the
OpenEmbedded build system from which it was produced. Thus, the SDK's
configuration is derived using that build system and the filters shown
in the following list. When these filters are present, the OpenEmbedded
build system applies them against `local.conf` and `auto.conf`:

-  Variables whose values start with "/" are excluded since the
   assumption is that those values are paths that are likely to be
   specific to the **Build Host**.

-  Variables listed in
   **ESDK_LOCALCONF_REMOVE**
   are excluded. These variables are not allowed through from the
   OpenEmbedded build system configuration into the extensible SDK
   configuration. Typically, these variables are specific to the machine
   on which the build system is running and could be problematic as part
   of the extensible SDK configuration.

   For a list of the variables excluded by default, see the
   **ESDK_LOCALCONF_REMOVE**
   in the glossary of the Yocto Project Reference Manual.

-  Variables listed in
   **ESDK_LOCALCONF_ALLOW**
   are included. Including a variable in the value of
   **ESDK_LOCALCONF_ALLOW** overrides either of the previous two
   filters. The default value is blank.

-  Classes inherited globally with **INHERIT** that are listed in
   **ESDK_CLASS_INHERIT_DISABLE** are disabled. Using
   **ESDK_CLASS_INHERIT_DISABLE** to disable these classes is the typical
   method to disable classes that are problematic or unnecessary in the SDK
   context. The default value disables the ref-classes-buildhistory
   class.

Additionally, the contents of `conf/sdk-extra.conf`, when present, are
appended to the end of `conf/local.conf` within the produced SDK,
without any filtering. The `sdk-extra.conf` file is particularly
useful if you want to set a variable value just for the SDK and not the
OpenEmbedded build system used to create the SDK.

## Adjusting the Extensible SDK to Suit Your Build Host's Setup

In most cases, the extensible SDK defaults should work with your :term:`Build
Host`'s setup. However, there are cases when you might consider making
adjustments:

-  If your SDK configuration inherits additional classes using the
   **INHERIT** variable and you
   do not need or want those classes enabled in the SDK, you can
   disable them by adding them to the **ESDK_CLASS_INHERIT_DISABLE**
   variable as described in the previous section.

   .. note::

      The default value of **ESDK_CLASS_INHERIT_DISABLE**
      is set using the "?=" operator. Consequently, you will need to
      either define the entire list by using the "=" operator, or you
      will need to append a value using either ":append" or the "+="
      operator. You can learn more about these operators in the
      "bitbake-user-manual/bitbake-user-manual-metadata:basic syntax"
      section of the BitBake User Manual.

-  If you have classes or recipes that add additional tasks to the
   standard build flow (i.e. the tasks execute as the recipe builds as
   opposed to being called explicitly), then you need to do one of the
   following:

   -  After ensuring the tasks are :ref:`shared
      state <overview-manual/concepts:shared state cache>` tasks (i.e. the
      output of the task is saved to and can be restored from the shared
      state cache) or ensuring the tasks are able to be produced quickly
      from a task that is a shared state task, add the task name to the
      value of
      **SDK_RECRDEP_TASKS**.

   -  Disable the tasks if they are added by a class and you do not need
      the functionality the class provides in the extensible SDK. To
      disable the tasks, add the class to the **ESDK_CLASS_INHERIT_DISABLE**
      variable as described in the previous section.

-  Generally, you want to have a shared state mirror set up so users of
   the SDK can add additional items to the SDK after installation
   without needing to build the items from source. See the
   "sdk-manual/appendix-customizing:providing additional installable extensible sdk content"
   section for information.

-  If you want users of the SDK to be able to easily update the SDK, you
   need to set the
   **SDK_UPDATE_URL**
   variable. For more information, see the
   "sdk-manual/appendix-customizing:providing updates to the extensible sdk after installation"
   section.

-  If you have adjusted the list of files and directories that appear in
   **COREBASE** (other than
   layers that are enabled through `bblayers.conf`), then you must
   list these files in
   **COREBASE_FILES** so
   that the files are copied into the SDK.

-  If your OpenEmbedded build system setup uses a different environment
   setup script other than
   structure-core-script, then you must
   set
   **OE_INIT_ENV_SCRIPT**
   to point to the environment setup script you use.

   .. note::

      You must also reflect this change in the value used for the
      **COREBASE_FILES** variable as previously described.

## Changing the Extensible SDK Installer Title

You can change the displayed title for the SDK installer by setting the
**SDK_TITLE** variable and then
rebuilding the SDK installer. For information on how to build an SDK
installer, see the "sdk-manual/appendix-obtain:building an sdk installer"
section.

By default, this title is derived from
**DISTRO_NAME** when it is
set. If the **DISTRO_NAME** variable is not set, the title is derived
from the **DISTRO** variable.

The
populate_sdk_base 
class defines the default value of the **SDK_TITLE** variable as
follows:

```none
SDK_TITLE ??= "${@d.getVar('DISTRO_NAME') or d.getVar('DISTRO')} SDK"
```

While there are several ways of changing this variable, an efficient method is
to set the variable in your distribution's configuration file. Doing so
creates an SDK installer title that applies across your distribution. As
an example, assume you have your own layer for your distribution named
"meta-mydistro" and you are using the same type of file hierarchy as
does the default "poky" distribution. If so, you could update the
**SDK_TITLE** variable in the
`~/meta-mydistro/conf/distro/mydistro.conf` file using the following
form:

```none
SDK_TITLE = "your_title"
```

## Providing Updates to the Extensible SDK After Installation

When you make changes to your configuration or to the metadata and if
you want those changes to be reflected in installed SDKs, you need to
perform additional steps. These steps make it possible for anyone using
the installed SDKs to update the installed SDKs by using the
`devtool sdk-update` command:

#. Create a directory that can be shared over HTTP or HTTPS. You can do
   this by setting up a web server such as an :wikipedia:`Apache HTTP Server
   <Apache_HTTP_Server>` or Nginx  server in the cloud
   to host the directory. This directory must contain the published SDK.

#. Set the
   **SDK_UPDATE_URL**
   variable to point to the corresponding HTTP or HTTPS URL. Setting
   this variable causes any SDK built to default to that URL and thus,
   the user does not have to pass the URL to the `devtool sdk-update`
   command as described in the
   "sdk-manual/extensible:applying updates to an installed extensible sdk"
   section.

#. Build the extensible SDK normally (i.e., use the
   `bitbake -c populate_sdk_ext` imagename command).

#. Publish the SDK using the following command:

   .. code-block:: console

      $ oe-publish-sdk some_path/sdk-installer.sh path_to_shared_http_directory

   You must
   repeat this step each time you rebuild the SDK with changes that you
   want to make available through the update mechanism.

Completing the above steps allows users of the existing installed SDKs
to simply run `devtool sdk-update` to retrieve and apply the latest
updates. See the
"sdk-manual/extensible:applying updates to an installed extensible sdk"
section for further information.

## Changing the Default SDK Installation Directory

When you build the installer for the Extensible SDK, the default
installation directory for the SDK is based on the
**DISTRO** and
**SDKEXTPATH** variables from
within the
populate_sdk_base 
class as follows:

```none
SDKEXTPATH ??= "~/${@d.getVar('DISTRO')}_sdk"
```

You can
change this default installation directory by specifically setting the
**SDKEXTPATH** variable.

While there are several ways of setting this variable,
the method that makes the most sense is to set the variable in your
distribution's configuration file. Doing so creates an SDK installer
default directory that applies across your distribution. As an example,
assume you have your own layer for your distribution named
"meta-mydistro" and you are using the same type of file hierarchy as
does the default "poky" distribution. If so, you could update the
**SDKEXTPATH** variable in the
`~/meta-mydistro/conf/distro/mydistro.conf` file using the following
form:

```none
SDKEXTPATH = "some_path_for_your_installed_sdk"
```

After building your installer, running it prompts the user for
acceptance of the some_path_for_your_installed_sdk directory as the
default location to install the Extensible SDK.

## Providing Additional Installable Extensible SDK Content

If you want the users of an extensible SDK you build to be able to add
items to the SDK without requiring the users to build the items from
source, you need to do a number of things:

#. Ensure the additional items you want the user to be able to install
   are already built:

   -  Build the items explicitly. You could use one or more "meta"
      recipes that depend on lists of other recipes.

   -  Build the "world" target and set
      `EXCLUDE_FROM_WORLD:pn-`\ recipename for the recipes you do not
      want built. See the
      **EXCLUDE_FROM_WORLD**
      variable for additional information.

#. Expose the `sstate-cache` directory produced by the build.
   Typically, you expose this directory by making it available through
   an Apache HTTP Server  or
   Nginx  server.

#. Set the appropriate configuration so that the produced SDK knows how
   to find the configuration. The variable you need to set is
   **SSTATE_MIRRORS**:

   .. code-block:: none

      SSTATE_MIRRORS = "file://.* https://example.com/some_path/sstate-cache/PATH"

   You can set the **SSTATE_MIRRORS** variable in two different places:

   -  If the mirror value you are setting is appropriate to be set for
      both the OpenEmbedded build system that is actually building the
      SDK and the SDK itself (i.e. the mirror is accessible in both
      places or it will fail quickly on the OpenEmbedded build system
      side, and its contents will not interfere with the build), then
      you can set the variable in your `local.conf` or custom distro
      configuration file. You can then pass the variable to the SDK by
      adding the following:

      .. code-block:: none

         ESDK_LOCALCONF_ALLOW = "SSTATE_MIRRORS"

   -  Alternatively, if you just want to set the **SSTATE_MIRRORS**
      variable's value for the SDK alone, create a `conf/sdk-extra.conf`
      file either in your **Build Directory** or within any
      layer and put your **SSTATE_MIRRORS** setting within that file.

      .. note::

         This second option is the safest option should you have any
         doubts as to which method to use when setting
         **SSTATE_MIRRORS**

## Minimizing the Size of the Extensible SDK Installer Download

By default, the extensible SDK bundles the shared state artifacts for
everything needed to reconstruct the image for which the SDK was built.
This bundling can lead to an SDK installer file that is a Gigabyte or
more in size. If the size of this file causes a problem, you can build
an SDK that has just enough in it to install and provide access to the
`devtool command` by setting the following in your configuration:

```none
SDK_EXT_TYPE = "minimal"
```

Setting
**SDK_EXT_TYPE** to
"minimal" produces an SDK installer that is around 35 Mbytes in size,
which downloads and installs quickly. You need to realize, though, that
the minimal installer does not install any libraries or tools out of the
box. These libraries and tools must be installed either "on the fly" or
through actions you perform using `devtool` or explicitly with the
`devtool sdk-install` command.

In most cases, when building a minimal SDK you need to also enable
bringing in the information on a wider range of packages produced by the
system. Requiring this wider range of information is particularly true
so that `devtool add` is able to effectively map dependencies it
discovers in a source tree to the appropriate recipes. Additionally, the
information enables the `devtool search` command to return useful
results.

To facilitate this wider range of information, you would need to set the
following:

```none
SDK_INCLUDE_PKGDATA = "1"
```

See the **SDK_INCLUDE_PKGDATA** variable for additional information.

Setting the **SDK_INCLUDE_PKGDATA** variable as shown causes the "world"
target to be built so that information for all of the recipes included
within it are available. Having these recipes available increases build
time significantly and increases the size of the SDK installer by 30-80
Mbytes depending on how many recipes are included in your configuration.

You can use `EXCLUDE_FROM_WORLD:pn-`\ recipename for recipes you want
to exclude. However, it is assumed that you would need to be building
the "world" target if you want to provide additional items to the SDK.
Consequently, building for "world" should not represent undue overhead
in most cases.

> **Note:**
> If you set
> SDK_EXT_TYPE
> to "minimal", then providing a shared state mirror is mandatory so
> that items can be installed as needed. See the
> sdk-manual/appendix-customizing:providing additional installable extensible sdk content
> section for more information.
>

You can explicitly control whether or not to include the toolchain when
you build an SDK by setting the
**SDK_INCLUDE_TOOLCHAIN**
variable to "1". In particular, it is useful to include the toolchain
when you have set **SDK_EXT_TYPE** to "minimal", which by default,
excludes the toolchain. Also, it is helpful if you are building a small
SDK for use with an IDE or some other tool where you do not want to take
extra steps to install a toolchain.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Customizing the Standard SDK

This appendix presents customizations you can apply to the standard SDK.

## Adding Individual Packages to the Standard SDK

When you build a standard SDK using the `bitbake -c populate_sdk`, a
default set of packages is included in the resulting SDK. The
**TOOLCHAIN_HOST_TASK**
and
**TOOLCHAIN_TARGET_TASK**
variables control the set of packages adding to the SDK.

If you want to add individual packages to the toolchain that runs on the
host, simply add those packages to the **TOOLCHAIN_HOST_TASK** variable.
Similarly, if you want to add packages to the default set that is part
of the toolchain that runs on the target, add the packages to the
**TOOLCHAIN_TARGET_TASK** variable.

## Adding API Documentation to the Standard SDK

You can include API documentation as well as any other documentation
provided by recipes with the standard SDK by adding "api-documentation"
to the
**DISTRO_FEATURES**
variable: DISTRO_FEATURES:append = " api-documentation" Setting this
variable as shown here causes the OpenEmbedded build system to build the
documentation and then include it in the standard SDK.
