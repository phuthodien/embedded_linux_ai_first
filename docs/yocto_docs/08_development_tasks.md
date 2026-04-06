# Yocto Project Development Tasks Manual

# The Yocto Project Development Tasks Manual

## Welcome

Welcome to the Yocto Project Development Tasks Manual. This manual
provides relevant procedures necessary for developing in the Yocto
Project environment (i.e. developing embedded Linux images and
user-space applications that run on targeted devices). This manual groups
related procedures into higher-level sections. Procedures can consist of
high-level steps or low-level steps depending on the topic.

This manual provides the following:

-  Procedures that help you get going with the Yocto Project; for
   example, procedures that show you how to set up a build host and work
   with the Yocto Project source repositories.

-  Procedures that show you how to submit changes to the Yocto Project.
   Changes can be improvements, new features, or bug fixes.

-  Procedures related to "everyday" tasks you perform while developing
   images and applications using the Yocto Project, such as
   creating a new layer, customizing an image, writing a new recipe,
   and so forth.

This manual does not provide the following:

-  Redundant step-by-step instructions: For example, the
   /sdk-manual/index manual contains detailed
   instructions on how to install an SDK, which is used to develop
   applications for target hardware.

-  Reference or conceptual material: This type of material resides in an
   appropriate reference manual. As an example, system variables are
   documented in the /ref-manual/index.

-  Detailed public information not specific to the Yocto Project: For
   example, exhaustive information on how to use the Git version
   control system is better covered with Internet searches and official Git
   documentation than through the Yocto Project documentation.

## Other Information

Because this manual presents information for many different topics,
supplemental information is recommended for full comprehension. For
introductory information on the Yocto Project, see the
`Yocto Project Website <>`. If you want to build an image with no
knowledge of Yocto Project as a way of quickly testing it out, see the
/brief-yoctoprojectqs/index document.

For a comprehensive list of links and other documentation, see the
"ref-manual/resources:links and related documentation"
section in the Yocto Project Reference Manual.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Setting Up to Use the Yocto Project

This chapter provides guidance on how to prepare to use the Yocto
Project. You can learn about creating a team environment to develop
using the Yocto Project, how to set up a :ref:`build
host <dev-manual/start:preparing the build host>`, how to locate
Yocto Project source repositories, and how to create local Git
repositories.

## Creating a Team Development Environment

It might not be immediately clear how you can use the Yocto Project in a
team development environment, or how to scale it for a large team of
developers. You can adapt the Yocto Project to many different use cases
and scenarios; however, this flexibility could cause difficulties if you
are trying to create a working setup that scales effectively.

To help you understand how to set up this type of environment, this
section presents a procedure that gives you information that can help
you get the results you want. The procedure is high-level and presents
some of the project's most successful experiences, practices, solutions,
and available technologies that have proved to work well in the past;
however, keep in mind, the procedure here is simply a starting point.
You can build off these steps and customize the procedure to fit any
particular working environment and set of practices.

#.  *Determine Who is Going to be Developing:* You first need to
    understand who is going to be doing anything related to the Yocto
    Project and determine their roles. Making this determination is
    essential to completing subsequent steps, which are to get your
    equipment together and set up your development environment's
    hardware topology.

    Possible roles are:

    -  *Application Developer:* This type of developer does application
       level work on top of an existing software stack.

    -  *Core System Developer:* This type of developer works on the
       contents of the operating system image itself.

    -  *Build Engineer:* This type of developer manages Autobuilders and
       releases. Depending on the specifics of the environment, not all
       situations might need a Build Engineer.

    -  *Test Engineer:* This type of developer creates and manages
       automated tests that are used to ensure all application and core
       system development meets desired quality standards.

#.  *Gather the Hardware:* Based on the size and make-up of the team,
    get the hardware together. Ideally, any development, build, or test
    engineer uses a system that runs a supported Linux distribution.
    These systems, in general, should be high performance (e.g. dual,
    six-core Xeons with 24 Gbytes of RAM and plenty of disk space). You
    can help ensure efficiency by having any machines used for testing
    or that run Autobuilders be as high performance as possible.

    .. note::

       Given sufficient processing power, you might also consider
       building Yocto Project development containers to be run under
       Docker, which is described later.

#.  *Understand the Hardware Topology of the Environment:* Once you
    understand the hardware involved and the make-up of the team, you
    can understand the hardware topology of the development environment.
    You can get a visual idea of the machines and their roles across the
    development environment.

#.  *Use Git as Your Source Control Manager (SCM):* Keeping your
    **Metadata** (i.e. recipes,
    configuration files, classes, and so forth) and any software you are
    developing under the control of an SCM system that is compatible
    with the **OpenEmbedded build system** is advisable. Of all of the SCMs
    supported by BitBake, the Yocto Project team strongly recommends using
    overview-manual/development-environment:git.
    Git is a distributed system
    that is easy to back up, allows you to work remotely, and then
    connects back to the infrastructure.

    .. note::

       For information about BitBake, see the
       bitbake:index.

    It is relatively easy to set up Git services and create infrastructure like
    `/`, which is based on server software called
    `Gitolite <https://gitolite.com>`__
    with `cgit <https://git.zx2c4.com/cgit/about/>`__ being used to
    generate the web interface that lets you view the repositories.
    `gitolite` identifies users using SSH keys and allows
    branch-based access controls to repositories that you can control as
    little or as much as necessary.

#.  *Set up the Application Development Machines:* As mentioned earlier,
    application developers are creating applications on top of existing
    software stacks. Here are some best practices for setting up
    machines used for application development:

    -  Use a pre-built toolchain that contains the software stack
       itself. Then, develop the application code on top of the stack.
       This method works well for small numbers of relatively isolated
       applications.

    -  Keep your cross-development toolchains updated. You can do this
       through provisioning either as new toolchain downloads or as
       updates through a package update mechanism to
       provide updates to an existing toolchain. The exact mechanics of
       how and when to do this depend on local policy.

    -  Use multiple toolchains installed locally into different
       locations to allow development across versions.

#.  *Set up the Core Development Machines:* As mentioned earlier, core
    developers work on the contents of the operating system itself.
    Here are some best practices for setting up machines used for
    developing images:

    -  Have the **OpenEmbedded Build System** available on
       the developer workstations so developers can run their own builds
       and directly rebuild the software stack.

    -  Keep the core system unchanged as much as possible and do your
       work in layers on top of the core system. Doing so gives you a
       greater level of portability when upgrading to new versions of
       the core system or Board Support Packages (BSPs).

    -  Share layers amongst the developers of a particular project and
       contain the policy configuration that defines the project.

#.  *Set up an Autobuilder:* Autobuilders are often the core of the
    development environment. It is here that changes from individual
    developers are brought together and centrally tested. Based on this
    automated build and test environment, subsequent decisions about
    releases can be made. Autobuilders also allow for "continuous
    integration" style testing of software components and regression
    identification and tracking.

    See "`Yocto Project Autobuilder <>`" for more
    information and links to buildbot. The Yocto Project team has found
    this implementation works well in this role. A public example of
    this is the Yocto Project Autobuilders, which the Yocto Project team
    uses to test the overall health of the project.

    The features of this system are:

    -  Highlights when commits break the build.

    -  Populates an :ref:`sstate
       cache <overview-manual/concepts:shared state cache>` from which
       developers can pull rather than requiring local builds.

    -  Allows commit hook triggers, which trigger builds when commits
       are made.

    -  Allows triggering of automated image booting and testing under
       the Quick EMUlator (QEMU).

    -  Supports incremental build testing and from-scratch builds.

    -  Shares output that allows developer testing and historical
       regression investigation.

    -  Creates output that can be used for releases.

    -  Allows scheduling of builds so that resources can be used
       efficiently.

#.  *Set up Test Machines:* Use a small number of shared, high
    performance systems for testing purposes. Developers can use these
    systems for wider, more extensive testing while they continue to
    develop locally using their primary development system.

#.  *Document Policies and Change Flow:* The Yocto Project uses a
    hierarchical structure and a pull model. There are scripts to create and
    send pull requests (i.e. `create-pull-request` and
    `send-pull-request`). This model is in line with other open source
    projects where maintainers are responsible for specific areas of the
    project and a single maintainer handles the final "top-of-tree"
    merges.

    .. note::

       You can also use a more collective push model. The `gitolite`
       software supports both the push and pull models quite easily.

    As with any development environment, it is important to document the
    policy used as well as any main project guidelines so they are
    understood by everyone. It is also a good idea to have
    well-structured commit messages, which are usually a part of a
    project's guidelines. Good commit messages are essential when
    looking back in time and trying to understand why changes were made.

    If you discover that changes are needed to the core layer of the
    project, it is worth sharing those with the community as soon as
    possible. Chances are if you have discovered the need for changes,
    someone else in the community needs them also.

#.  *Development Environment Summary:* Aside from the previous steps,
    here are best practices within the Yocto Project development
    environment:

    -  Use overview-manual/development-environment:git as the source control
       system.

    -  Maintain your Metadata in layers that make sense for your
       situation. See the "overview-manual/yp-intro:the yocto project layer model"
       section in the Yocto Project Overview and Concepts Manual and the
       "dev-manual/layers:understanding and creating layers"
       section for more information on layers.

    -  Separate the project's Metadata and code by using separate Git
       repositories. See the "overview-manual/development-environment:yocto project source repositories"
       section in the Yocto Project Overview and Concepts Manual for
       information on these repositories. See the
       "/dev-manual/poky-manual-setup"
       section for information on how to set up local Git repositories
       for related upstream Yocto Project Git repositories.

    -  Set up the directory for the shared state cache
       (**SSTATE_DIR**) where
       it makes sense. For example, set up the sstate cache on a system
       used by developers in the same organization and share the same
       source directories on their machines.

    -  Set up a local overview-manual/concepts:Hash Equivalence server.

       See the overview-manual/concepts:Hash Equivalence section of the
       Yocto Project Overview and Concepts Manual for more details on the hash
       equivalence feature.

       See the /dev-manual/hashequivserver section of the Yocto Project
       Development Tasks Manual for details on how to setup a
       overview-manual/concepts:Hash Equivalence server.

    -  Set up an Autobuilder and have it populate the sstate cache and
       source directories.

    -  The Yocto Project community encourages you to send patches to the
       project to fix bugs or add features. If you do submit patches,
       follow the project commit guidelines for writing good commit
       messages. See the "/contributor-guide/submit-changes"
       section in the Yocto Project and OpenEmbedded Contributor Guide.

    -  Send changes to the core sooner than later as others are likely
       to run into the same issues. For some guidance on mailing lists
       to use, see the lists in the
       "contributor-guide/submit-changes:finding a suitable mailing list"
       section. For a description
       of the available mailing lists, see the "resources-mailinglist" section in
       the Yocto Project Reference Manual.

## Preparing the Build Host

This section provides procedures to set up a system to be used as your
**Build Host** for
development using the Yocto Project. Your build host can be a native
Linux machine (recommended), it can be a machine (Linux, Mac, or
Windows) that uses `CROPS <https://github.com/crops/poky-container>`__,
which leverages `Docker Containers <https://www.docker.com/>`__ or it
can be a Windows machine capable of running version 2 of Windows Subsystem
For Linux (WSL 2).

> **Note:**
> The Yocto Project is not compatible with version 1 of
> Windows Subsystem for Linux .
> It is compatible but neither officially supported nor validated with
> WSL 2. If you still decide to use WSL please upgrade to
> `WSL 2 <https://learn.microsoft.com/en-us/windows/wsl/install>`__.
>

Once your build host is set up to use the Yocto Project, further steps
are necessary depending on what you want to accomplish. See the
following references for information on how to prepare for Board Support
Package (BSP) development and kernel development:

-  *BSP Development:* See the "bsp-guide/bsp:preparing your build host to work with bsp layers"
   section in the Yocto Project Board Support Package (BSP) Developer's
   Guide.

-  *Kernel Development:* See the "kernel-dev/common:preparing the build host to work on the kernel"
   section in the Yocto Project Linux Kernel Development Manual.

### Setting Up a Native Linux Host

Follow these steps to prepare a native Linux machine as your Yocto
Project Build Host:

#. *Use a Supported Linux Distribution:* You should have a reasonably
   current Linux-based host system. You will have the best results with
   a recent release of Fedora, openSUSE, Debian, Ubuntu, RHEL or CentOS
   as these releases are frequently tested against the Yocto Project and
   officially supported. For a list of the distributions under
   validation and their status, see the ":ref:`Supported Linux
   Distributions <system-requirements-supported-distros>`"
   section in the Yocto Project Reference Manual and the wiki page at
   Distribution Support .

#. *Have Enough Free Memory:* Your system should have at least 50 Gbytes
   of free disk space for building images.

#. *Meet Minimal Version Requirements:* The OpenEmbedded build system
   should be able to run on any modern distribution that has the
   following versions for Git, tar, Python, gcc and make.

   -  Git &MIN_GIT_VERSION; or greater

   -  tar &MIN_TAR_VERSION; or greater

   -  Python &MIN_PYTHON_VERSION; or greater.

   -  gcc &MIN_GCC_VERSION; or greater.

   -  GNU make &MIN_MAKE_VERSION; or greater

   If your build host does not satisfy all of these listed version
   requirements, you can take steps to prepare the system so that you
   can still use the Yocto Project. See the
   "ref-manual/system-requirements:required git, tar, python, make and gcc versions"
   section in the Yocto Project Reference Manual for information.

#. *Install Development Host Packages:* Required development host
   packages vary depending on your build host and what you want to do
   with the Yocto Project. Collectively, the number of required packages
   is large if you want to be able to cover all cases.

   For lists of required packages for all scenarios, see the
   "ref-manual/system-requirements:required packages for the build host"
   section in the Yocto Project Reference Manual.

Once you have completed the previous steps, you are ready to continue
using a given development path on your native Linux machine. If you are
going to use BitBake, see the
"dev-manual/start:preparing the poky reference distro"
section. If you are going
to use the Extensible SDK, see the "/sdk-manual/extensible" Chapter in the Yocto
Project Application Development and the Extensible Software Development
Kit (eSDK) manual. If you want to work on the kernel, see the /kernel-dev/index. If you are going to use
Toaster, see the "/toaster-manual/setup-and-use"
section in the Toaster User Manual. If you are a VSCode user, you can configure
the `Yocto Project BitBake
<https://marketplace.visualstudio.com/items?itemName=yocto-project.yocto-bitbake>`__
extension accordingly.

### Setting Up to Use CROss PlatformS (CROPS)

With `CROPS <https://github.com/crops/poky-container>`__, which
leverages `Docker Containers <https://www.docker.com/>`__, you can
create a Yocto Project development environment that is operating system
agnostic. You can set up a container in which you can develop using the
Yocto Project on a Windows, Mac, or Linux machine.

Follow these general steps to prepare a Windows, Mac, or Linux machine
as your Yocto Project build host:

#. *Determine What Your Build Host Needs:*
   `Docker <https://www.docker.com/what-docker>`__ is a software
   container platform that you need to install on the build host.
   Depending on your build host, you might have to install different
   software to support Docker containers. Go to the Docker installation
   page and read about the platform requirements in "`Supported
   Platforms <https://docs.docker.com/engine/install/#supported-platforms>`__"
   your build host needs to run containers.

#. *Choose What To Install:* Depending on whether or not your build host
   meets system requirements, you need to install "Docker CE Stable" or
   the "Docker Toolbox". Most situations call for Docker CE. However, if
   you have a build host that does not meet requirements (e.g.
   Pre-Windows 10 or Windows 10 "Home" version), you must install Docker
   Toolbox instead.

#. *Go to the Install Site for Your Platform:* Click the link for the
   Docker edition associated with your build host's native software. For
   example, if your build host is running Microsoft Windows Version 10
   and you want the Docker CE Stable edition, click that link under
   "Supported Platforms".

#. *Install the Software:* Once you have understood all the
   pre-requisites, you can download and install the appropriate
   software. Follow the instructions for your specific machine and the
   type of the software you need to install:

   -  Install `Docker Desktop on
      Windows <https://docs.docker.com/docker-for-windows/install/#install-docker-desktop-on-windows>`__
      for Windows build hosts that meet requirements.

   -  Install `Docker Desktop on
      MacOs <https://docs.docker.com/docker-for-mac/install/#install-and-run-docker-desktop-on-mac>`__
      for Mac build hosts that meet requirements.

   -  Install `Docker Engine on
      CentOS <https://docs.docker.com/engine/install/centos/>`__
      for Linux build hosts running the CentOS distribution.

   -  Install `Docker Engine on
      Debian <https://docs.docker.com/engine/install/debian/>`__
      for Linux build hosts running the Debian distribution.

   -  Install `Docker Engine for
      Fedora <https://docs.docker.com/engine/install/fedora/>`__
      for Linux build hosts running the Fedora distribution.

   -  Install `Docker Engine for
      Ubuntu <https://docs.docker.com/engine/install/ubuntu/>`__
      for Linux build hosts running the Ubuntu distribution.

#. *Optionally Orient Yourself With Docker:* If you are unfamiliar with
   Docker and the container concept, you can learn more here -
   https://docs.docker.com/get-started/.

#. *Launch Docker or Docker Toolbox:* You should be able to launch
   Docker or the Docker Toolbox and have a terminal shell on your
   development host.

#. *Set Up the Containers to Use the Yocto Project:* Go to
   https://github.com/crops/docker-win-mac-docs/wiki and follow
   the directions for your particular build host (i.e. Linux, Mac, or
   Windows).

   Once you complete the setup instructions for your machine, you have
   the Poky, Extensible SDK, and Toaster containers available. You can
   click those links from the page and learn more about using each of
   those containers.

Once you have a container set up, everything is in place to develop just
as if you were running on a native Linux machine. If you are going to
use the Poky container, see the
"dev-manual/start:preparing the Poky reference distro"
section. If you are going to use the Extensible SDK container, see the
"/sdk-manual/extensible" Chapter in the Yocto
Project Application Development and the Extensible Software Development
Kit (eSDK) manual. If you are going to use the Toaster container, see
the "/toaster-manual/setup-and-use"
section in the Toaster User Manual. If you are a VSCode user, you can configure
the `Yocto Project BitBake
<https://marketplace.visualstudio.com/items?itemName=yocto-project.yocto-bitbake>`__
extension accordingly.

### Setting Up to Use Windows Subsystem For Linux (WSL 2)

With `Windows Subsystem for Linux (WSL 2)
<https://learn.microsoft.com/en-us/windows/wsl/>`__,
you can create a Yocto Project development environment that allows you
to build on Windows. You can set up a Linux distribution inside Windows
in which you can develop using the Yocto Project.

Follow these general steps to prepare a Windows machine using WSL 2 as
your Yocto Project build host:

#. *Make sure your Windows machine is capable of running WSL 2:*

   While all Windows 11 and Windows Server 2022 builds support WSL 2,
   the first versions of Windows 10 and Windows Server 2019 didn't.
   Check the minimum build numbers for `Windows 10
   <https://learn.microsoft.com/en-us/windows/wsl/install-manual#step-2---check-requirements-for-running-wsl-2>`__
   and for `Windows Server 2019
   <https://learn.microsoft.com/en-us/windows/wsl/install-on-server>`__.

   To check which build version you are running, you may open a command
   prompt on Windows and execute the command "ver"::

      C:\Users\myuser> ver

      Microsoft Windows [Version 10.0.19041.153]

#. *Install the Linux distribution of your choice inside WSL 2:*
   Once you know your version of Windows supports WSL 2, you can
   install the distribution of your choice from the Microsoft Store.
   Open the Microsoft Store and search for Linux. While there are
   several Linux distributions available, the assumption is that your
   pick will be one of the distributions supported by the Yocto Project
   as stated on the instructions for using a native Linux host. After
   making your selection, simply click "Get" to download and install the
   distribution.

#. *Check which Linux distribution WSL 2 is using:* Open a Windows
   PowerShell and run::

      C:\WINDOWS\system32> wsl -l -v
      NAME    STATE   VERSION
      *Ubuntu Running 2

   Note that WSL 2 supports running as many different Linux distributions
   as you want to install.

#. *Optionally Get Familiar with WSL:* You can learn more on
   https://docs.microsoft.com/en-us/windows/wsl/wsl2-about.

#. *Launch your WSL Distibution:* From the Windows start menu simply
   launch your WSL distribution just like any other application.

#. *Optimize your WSL 2 storage often:* Due to the way storage is
   handled on WSL 2, the storage space used by the underlying Linux
   distribution is not reflected immediately, and since BitBake heavily
   uses storage, after several builds, you may be unaware you are
   running out of space. As WSL 2 uses a VHDX file for storage, this issue
   can be easily avoided by regularly optimizing this file in a manual way:

   1. *Find the location of your VHDX file:*

      First you need to find the distro app package directory, to achieve this
      open a Windows Powershell as Administrator and run::

         C:\WINDOWS\system32> Get-AppxPackage -Name "*Ubuntu*" | Select PackageFamilyName
### PackageFamilyName

         CanonicalGroupLimited.UbuntuonWindows_79abcdefgh

      You should now
      replace the PackageFamilyName and your user on the following path
      to find your VHDX file::

         ls C:\Users\myuser\AppData\Local\Packages\CanonicalGroupLimited.UbuntuonWindows_79abcdefgh\LocalState\
         Mode                 LastWriteTime         Length Name
         -a----         3/14/2020   9:52 PM    57418973184 ext4.vhdx

      Your VHDX file path is:
      `C:\Users\myuser\AppData\Local\Packages\CanonicalGroupLimited.UbuntuonWindows_79abcdefgh\LocalState\ext4.vhdx`

   2a. *Optimize your VHDX file using Windows Powershell:*

       To use the `optimize-vhd` cmdlet below, first install the Hyper-V
       option on Windows. Then, open a Windows Powershell as Administrator to
       optimize your VHDX file, shutting down WSL first::

         C:\WINDOWS\system32> wsl --shutdown
         C:\WINDOWS\system32> optimize-vhd -Path C:\Users\myuser\AppData\Local\Packages\CanonicalGroupLimited.UbuntuonWindows_79abcdefgh\LocalState\ext4.vhdx -Mode full

       A progress bar should be shown while optimizing the
       VHDX file, and storage should now be reflected correctly on the
       Windows Explorer.

   2b. *Optimize your VHDX file using DiskPart:*

       The `optimize-vhd` cmdlet noted in step 2a above is provided by
       Hyper-V. Not all SKUs of Windows can install Hyper-V. As an alternative,
       use the DiskPart tool. To start, open a Windows command prompt as
       Administrator to optimize your VHDX file, shutting down WSL first::

         C:\WINDOWS\system32> wsl --shutdown
         C:\WINDOWS\system32> diskpart

         DISKPART> select vdisk file="<path_to_VHDX_file>"
         DISKPART> attach vdisk readonly
         DISKPART> compact vdisk
         DISKPART> detach
         DISKPART> exit

> **Note:**
> The current implementation of WSL 2 does not have out-of-the-box
> access to external devices such as those connected through a USB
> port, but it automatically mounts your `C:` drive on `/mnt/c/`
> (and others), which you can use to share deploy artifacts to be later
> flashed on hardware through Windows, but your **Build Directory**
> should not reside inside this mountpoint.
>

Once you have WSL 2 set up, everything is in place to develop just as if
you were running on a native Linux machine. If you are going to use the
Extensible SDK container, see the "/sdk-manual/extensible" Chapter in the Yocto
Project Application Development and the Extensible Software Development
Kit (eSDK) manual. If you are going to use the Toaster container, see
the "/toaster-manual/setup-and-use"
section in the Toaster User Manual. If you are a VSCode user, you can configure
the `Yocto Project BitBake
<https://marketplace.visualstudio.com/items?itemName=yocto-project.yocto-bitbake>`__
extension accordingly.

### Preparing the Poky Reference Distro

Now that the **Build Host** has been prepared to build images, the next step
can be to build the **Poky** reference distro. There are two ways of setting
up the **Poky** reference distro:

-  Using the `bitbake-setup` command-line utility from **BitBake**.
   Follow:

   Setting Up The Environment With bitbake-setup 

-  Manually cloning the required repositories. Follow:

   /dev-manual/poky-manual-setup

> **Note:**
> The **Poky** distro is a reference distro. It can be used to try the
> Yocto Project but should not be used in a production environment. Instead, it
> is recommended to create your own distro, see the
> /dev-manual/custom-distribution section of the Yocto Project
> Development Tasks Manual.
>

### Initializing the Build Environment

Initializing the build environment is a common task, as it needs to be done for
each new instance of the shell where the **Poky** reference distro was set up:

-  If `bitbake-setup` was used to setup the **Poky** reference distro, you
   need to source the `init-build-env` file of the **bitbake:Setup**
   directory that was created after the bitbake:ref-bbsetup-command-init
   command execution:

   .. code-block:: console

      $ source <setup-dir>/build/init-build-env

-  If the **Poky** distro was setup using manual clones of the layers, you
   need to source the `oe-init-build-env` script present in the
   **OpenEmbedded-Core (OE-Core)** layer:

   .. code-block:: console

      $ source openembedded-core/oe-init-build-env

This will setup your current shell and change the current working directory to
the **Build Directory**.

### Where To Go From Here

-  Configure your build: follow overview-manual/concepts:User Configuration.

-  Learn how and what to build with **BitBake**: follow /dev-manual/building.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Understanding and Creating Layers

The OpenEmbedded build system supports organizing
**Metadata** into multiple layers.
Layers allow you to isolate different types of customizations from each
other. For introductory information on the Yocto Project Layer Model,
see the
"overview-manual/yp-intro:the yocto project layer model"
section in the Yocto Project Overview and Concepts Manual.

## Creating Your Own Layer

Follow these general steps to create your layer without using tools:

#. *Check Existing Layers:* Before creating a new layer, you should be
   sure someone has not already created a layer containing the Metadata
   you need. You can see the `OpenEmbedded Metadata Index <>`
   for a list of layers from the OpenEmbedded community that can be used in
   the Yocto Project. You could find a layer that is identical or close
   to what you need.

   .. note::

      For information on BSP layers, see the "bsp-guide/bsp:bsp layers"
      section in the Yocto Project Board Specific (BSP) Developer's Guide.

#. *Create a new Layer:* Create the directory for your layer. The
   `bitbake-layers` script with the `create-layer` subcommand simplifies
   creating a new general layer. Place it next to the other layers in your
   **Source Directory**.

   In its simplest form, you can use the following command to create a
   layer named "your_layer_name" in the current directory:

   .. code-block:: console

      $ bitbake-layers create-layer your_layer_name

   While not strictly required, prepend the name of the directory with
   the string "meta-". For example::

      meta-mylayer
      meta-GUI_xyz
      meta-mymachine

   With rare exceptions, a layer's name follows this form::

      meta-root_name

   Following this layer naming convention can save
   you trouble later when tools, components, or variables "assume" your
   layer name begins with "meta-". A notable example is in configuration
   files as shown in the following step where layer names without the
   "meta-" string are appended to several variables used in the
   configuration.

   As an example, the following command creates a layer named `meta-scottrif`
   in your home directory:

   .. code-block:: console

      $ bitbake-layers create-layer meta-scottrif
      NOTE: Starting bitbake server...
      Add your new layer with 'bitbake-layers add-layer meta-scottrif'

   In order to use a layer with the **OpenEmbedded Build System**, you
   need to add the layer to your `bblayers.conf` configuration
   file, as hinted by the previous command. See the
   "dev-manual/layers:adding a layer using the \\`bitbake-layers\`\`
   script`" section for more information.

   By default, the `bitbake-layers create-layer` command creates a layer
   with the following:

   -  A `conf/layer.conf` configuration file with default definitions.

      Here is an explanation of the typical content of the layer configuration file:

      -  **BBPATH**: Adds the layer's root directory to BitBake's search
         path. Through the use of the **BBPATH** variable, BitBake locates
         class files (`.bbclass`), configuration files, and files that are
         included with `include` and `require` statements. For these cases,
         BitBake uses the first file that matches the name found in
         **BBPATH**. This is similar to the way the `PATH` variable is
         used for binaries. It is recommended, therefore, that you use unique
         class and configuration filenames in your custom layer.

         See the :ref:`bitbake:bitbake-user-manual/bitbake-user-manual-metadata:Locating Include
         Files` section of the BitBake User Manual for more details on how
         files are included with **BitBake**.

      -  **BBFILES**: Defines the location for all recipes in the layer.

      -  **BBFILE_COLLECTIONS**: Establishes the current layer through a
         unique identifier that is used throughout the :term:`OpenEmbedded Build
         System` to refer to the layer. In this example, the identifier
         "yoctobsp" is the representation for the container layer named
         "meta-yocto-bsp".

         This name is used by other layers when specifying the layer
         dependencies via the **LAYERDEPENDS** variable.

      -  **BBFILE_PATTERN**: Expands immediately during parsing to provide
         the directory of the layer.

      -  **BBFILE_PRIORITY**: Establishes a priority to use for recipes in
         the layer when the **OpenEmbedded Build System** finds recipes of
         the same name in different layers.

      -  **LAYERVERSION**: Establishes a version number for the layer. You
         can use this version number to specify this exact version of the layer
         as a dependency when using the **LAYERDEPENDS** variable.

      -  **LAYERDEPENDS**: Lists all layers on which this layer depends (if
         any). It uses the layer names specified by the
         **BBFILE_COLLECTIONS** variable.

      -  **LAYERSERIES_COMPAT**: Lists the :yocto_home:`Yocto Project
         release </development/releases/>` codenames (in lowercase) this layer is
         compatible with. For example: "&DISTRO_NAME_NO_CAP;".

         This variable is a good way to indicate if a particular layer is
         current. 

   -  A `recipes-example` subdirectory that itself contains a
      subdirectory named `example`, which contains an `example.bb`
      recipe file.

   -  A `COPYING.MIT`, which is the license statement for the layer. The
      script assumes you want to use the MIT license, which is typical for
      most layers, for the contents of the layer itself.

   -  A `README` file, which is a file describing the contents of your
      new layer.

   If you want to set the priority of the layer to other than the default value
   of "6", you can either use the `--priority` option or you can edit the
   **BBFILE_PRIORITY** value in the `conf/layer.conf` after the script
   creates it. Furthermore, if you want to give the example recipe file some
   name other than the default, you can use the `--example-recipe-name`
   option.

   The easiest way to see how the `bitbake-layers create-layer` command
   works is to experiment with the script. You can also read the usage
   information by running `bitbake-layers create-layer --help`.

#. *Add Content:* Depending on the type of layer, add the content. If
   the layer adds support for a machine, add the machine configuration
   in a `conf/machine/` file within the layer. If the layer adds
   distro policy, add the distro configuration in a `conf/distro/`
   file within the layer. If the layer introduces new recipes, put the
   recipes you need in `recipes-*` subdirectories within the layer.

   .. note::

      For an explanation of layer hierarchy that is compliant with the
      Yocto Project, see the "bsp-guide/bsp:example filesystem layout"
      section in the Yocto Project Board Support Package (BSP) Developer's Guide.

#. *Optionally Test for Compatibility:* If you want permission to use
   the Yocto Project Compatibility logo with your layer or application
   that uses your layer, perform the steps to apply for compatibility.
   See the
   "dev-manual/layers:making sure your layer is compatible with yocto project"
   section for more information.

## Following Best Practices When Creating Layers

To create layers that are easier to maintain and that will not impact
builds for other machines, you should consider the information in the
following list:

-  *Avoid "Overlaying" Entire Recipes from Other Layers in Your
   Configuration:* In other words, do not copy an entire recipe into
   your layer and then modify it. Rather, use an append file
   (`.bbappend`) to override only those parts of the original recipe
   you need to modify.

-  *Avoid Duplicating Include Files:* Use append files (`.bbappend`)
   for each recipe that uses an include file. Or, if you are introducing
   a new recipe that requires the included file, use the path relative
   to the original layer directory to refer to the file. For example,
   use `require recipes-core/`\ `package`\ `/`\ `file`\ `.inc` instead
   of `require` `file`\ `.inc`. If you're finding you have to overlay
   the include file, it could indicate a deficiency in the include file
   in the layer to which it originally belongs. If this is the case, you
   should try to address that deficiency instead of overlaying the
   include file. For example, you could address this by getting the
   maintainer of the include file to add a variable or variables to make
   it easy to override the parts needing to be overridden.

-  *Structure Your Layers:* Proper use of overrides within append files
   and placement of machine-specific files within your layer can ensure
   that a build is not using the wrong Metadata and negatively impacting
   a build for a different machine. Here are some examples:

   -  *Modify Variables to Support a Different Machine:* Suppose you
      have a layer named `meta-one` that adds support for building
      machine "one". To do so, you use an append file named
      `base-files.bbappend` and create a dependency on "foo" by
      altering the **DEPENDS**
      variable::

         DEPENDS = "foo"

      The dependency is created during any
      build that includes the layer `meta-one`. However, you might not
      want this dependency for all machines. For example, suppose you
      are building for machine "two" but your `bblayers.conf` file has
      the `meta-one` layer included. During the build, the
      `base-files` for machine "two" will also have the dependency on
      `foo`.

      To make sure your changes apply only when building machine "one",
      use a machine override with the **DEPENDS** statement::

         DEPENDS:one = "foo"

      You should follow the same strategy when using `:append`
      and `:prepend` operations::

         DEPENDS:append:one = " foo"
         DEPENDS:prepend:one = "foo "

      As an actual example, here's a
      snippet from the generic kernel include file `linux-yocto.inc`,
      wherein the kernel compile and link options are adjusted in the
      case of a subset of the supported architectures::

         DEPENDS:append:aarch64 = " libgcc"
         KERNEL_CC:append:aarch64 = " ${TOOLCHAIN_OPTIONS}"
         KERNEL_LD:append:aarch64 = " ${TOOLCHAIN_OPTIONS}"

         DEPENDS:append:nios2 = " libgcc"
         KERNEL_CC:append:nios2 = " ${TOOLCHAIN_OPTIONS}"
         KERNEL_LD:append:nios2 = " ${TOOLCHAIN_OPTIONS}"

         DEPENDS:append:arc = " libgcc"
         KERNEL_CC:append:arc = " ${TOOLCHAIN_OPTIONS}"
         KERNEL_LD:append:arc = " ${TOOLCHAIN_OPTIONS}"

         KERNEL_FEATURES:append:qemuall=" features/debug/printk.scc"

   -  *Place Machine-Specific Files in Machine-Specific Locations:* When
      you have a base recipe, such as `base-files.bb`, that contains a
      **SRC_URI** statement to a
      file, you can use an append file to cause the build to use your
      own version of the file. For example, an append file in your layer
      at `meta-one/recipes-core/base-files/base-files.bbappend` could
      extend **FILESPATH** using **FILESEXTRAPATHS** as follows::

         FILESEXTRAPATHS:prepend := "${THISDIR}/${BPN}:"

      The build for machine "one" will pick up your machine-specific file as
      long as you have the file in
      `meta-one/recipes-core/base-files/base-files/`. However, if you
      are building for a different machine and the `bblayers.conf`
      file includes the `meta-one` layer and the location of your
      machine-specific file is the first location where that file is
      found according to **FILESPATH**, builds for all machines will
      also use that machine-specific file.

      You can make sure that a machine-specific file is used for a
      particular machine by putting the file in a subdirectory specific
      to the machine. For example, rather than placing the file in
      `meta-one/recipes-core/base-files/base-files/` as shown above,
      put it in `meta-one/recipes-core/base-files/base-files/one/`.
      Not only does this make sure the file is used only when building
      for machine "one", but the build process locates the file more
      quickly.

      In summary, you need to place all files referenced from
      **SRC_URI** in a machine-specific subdirectory within the layer in
      order to restrict those files to machine-specific builds.

-  *Perform Steps to Apply for Yocto Project Compatibility:* If you want
   permission to use the Yocto Project Compatibility logo with your
   layer or application that uses your layer, perform the steps to apply
   for compatibility. See the
   "dev-manual/layers:making sure your layer is compatible with yocto project"
   section for more information.

-  *Follow the Layer Naming Convention:* Store custom layers in a Git
   repository that use the `meta-layer_name` format.

-  *Group Your Layers Locally:* Clone your repository alongside other
   layers in your **Source Directory**.

## Making Sure Your Layer is Compatible With Yocto Project

When you create a layer used with the Yocto Project, it is advantageous
to make sure that the layer interacts well with existing Yocto Project
layers (i.e. the layer is compatible with the Yocto Project). Ensuring
compatibility makes the layer easy to be consumed by others in the Yocto
Project community and could allow you permission to use the Yocto
Project Compatible Logo.

> **Note:**
> Only Yocto Project member organizations are permitted to use the
> Yocto Project Compatible Logo. The logo is not available for general
> use. For information on how to become a Yocto Project member
> organization, see the `Yocto Project Website <>`.
>

The Yocto Project Compatibility Program consists of a layer application
process that requests permission to use the Yocto Project Compatibility
Logo for your layer and application. The process consists of two parts:

#. Successfully passing a script (`yocto-check-layer`) that, when run
   against your layer, tests it against constraints based on experiences
   of how layers have worked in the real world and where pitfalls have
   been found. Getting a "PASS" result from the script is required for
   successful compatibility registration.

#. Completion of an application acceptance form, which you can find at
   `/compatible-registration/`.

To be granted permission to use the logo, you need to satisfy the
following:

-  Be able to check the box indicating that you got a "PASS" when
   running the script against your layer.

-  Answer "Yes" to the questions on the form or have an acceptable
   explanation for any questions answered "No".

-  Be a Yocto Project Member Organization.

The remainder of this section presents information on the registration
form and on the `yocto-check-layer` script.

### Yocto Project Compatible Program Application

Use the form to apply for your layer's approval. Upon successful
application, you can use the Yocto Project Compatibility Logo with your
layer and the application that uses your layer.

To access the form, use this link:
`/compatible-registration`.
Follow the instructions on the form to complete your application.

The application consists of the following sections:

-  *Contact Information:* Provide your contact information as the fields
   require. Along with your information, provide the released versions
   of the Yocto Project for which your layer is compatible.

-  *Acceptance Criteria:* Provide "Yes" or "No" answers for each of the
   items in the checklist. There is space at the bottom of the form for
   any explanations for items for which you answered "No".

-  *Recommendations:* Provide answers for the questions regarding Linux
   kernel use and build success.

### ``yocto-check-layer`` Script

The `yocto-check-layer` script provides you a way to assess how
compatible your layer is with the Yocto Project. You should run this
script prior to using the form to apply for compatibility as described
in the previous section. You need to achieve a "PASS" result in order to
have your application form successfully processed.

The script divides tests into three areas: COMMON, BSP, and DISTRO. For
example, given a distribution layer (DISTRO), the layer must pass both
the COMMON and DISTRO related tests. Furthermore, if your layer is a BSP
layer, the layer must pass the COMMON and BSP set of tests.

To execute the script, enter the following commands from your build
directory::

   $ source oe-init-build-env
   $ yocto-check-layer your_layer_directory

Be sure to provide the actual directory for your
layer as part of the command.

You should pass the dependencies of your layer using the `--dependency`
argument::

   $ source oe-init-build-env
   $ yocto-check-layer --dependency <layer1_directory> <layer2_directory> ... -- your_layer_directory

For **BSP <Board Support Package (BSP)>** layers, the `--machines`
argument should be passed as part of the command::

   $ source oe-init-build-env
   $ yocto-check-layer --machines <machine1> <machine2> ... -- your_layer_directory

These machines are the ones present in your BSP layer, in the `conf/machine/`
directory.

Entering the command causes the script to determine the type of layer
and then to execute a set of specific tests against the layer.

The following list overviews the test:

-  `common.test_readme`: Tests if a `README` file exists in the
   layer and the file is not empty.

-  `common.test_security`: Tests that the layer has a `SECURITY.md`
   (or similar) file, either in the layer itself or at the top of the containing
   git repository.

-  `common.test_parse`: Tests to make sure that BitBake can parse the
   files without error (i.e. `bitbake -p`).

-  `common.test_show_environment`: Tests that the global or per-recipe
   environment is in order without errors (i.e. `bitbake -e`).

-  `common.test_world`: Verifies that `bitbake world` works.

-  `common.test_world_inherit_class`: Verifies that `bitbake world` works
   when the ref-classes-yocto-check-layer class is inherited.

-  `common.test_patches_upstream_status`: Verifies that all the patch files
   included in the layer contain a
   contributor-guide/recipe-style-guide:Patch Upstream Status.

-  `common.test_signatures`: Tests to be sure that BSP and DISTRO
   layers do not come with recipes that change signatures.

-  `common.test_layerseries_compat`: Verifies layer compatibility is
   set properly.

-  `bsp.test_bsp_defines_machines`: Tests if a BSP layer has machine
   configurations.

-  `bsp.test_bsp_no_set_machine`: Tests to ensure a BSP layer does not
   set the machine when the layer is added.

-  `bsp.test_machine_world`: Verifies that `bitbake world` works
   regardless of which machine is selected.

-  `bsp.test_machine_signatures`: Verifies that building for a
   particular machine affects only the signature of tasks specific to
   that machine.

-  `distro.test_distro_defines_distros`: Tests if a DISTRO layer has
   distro configurations.

-  `distro.test_distro_no_set_distros`: Tests to ensure a DISTRO layer
   does not set the distribution when the layer is added.

For a complete list of tests, see the :oe_git:`scripts/lib/checklayer/cases
</openembedded-core/tree/scripts/lib/checklayer/cases>` directory in
**OpenEmbedded-Core (OE-Core)**.

## Enabling Your Layer

Before the OpenEmbedded build system can use your new layer, you need to
enable it. To enable your layer, simply add your layer's path to the
**BBLAYERS** variable in your `conf/bblayers.conf` file, which is
found in the **Build Directory**. The following example shows how to
enable your new `meta-mylayer` layer (note how your new layer exists
outside of the official `poky` repository which you would have checked
out earlier)::

   # POKY_BBLAYERS_CONF_VERSION is increased each time build/conf/bblayers.conf
   # changes incompatibly
   POKY_BBLAYERS_CONF_VERSION = "2"
   BBPATH = "${TOPDIR}"
   BBFILES ?= ""
   BBLAYERS ?= " \
       /home/user/poky/meta \
       /home/user/poky/meta-poky \
       /home/user/poky/meta-yocto-bsp \
       /home/user/mystuff/meta-mylayer \
       "

BitBake parses each `conf/layer.conf` file from the top down as
specified in the **BBLAYERS** variable within the `conf/bblayers.conf`
file. During the processing of each `conf/layer.conf` file, BitBake
adds the recipes, classes and configurations contained within the
particular layer to the source directory.

## Appending Other Layers Metadata With Your Layer

A recipe that appends Metadata to another recipe is called a BitBake
append file. A BitBake append file uses the `.bbappend` file type
suffix, while the corresponding recipe to which Metadata is being
appended uses the `.bb` file type suffix.

You can use a `.bbappend` file in your layer to make additions or
changes to the content of another layer's recipe without having to copy
the other layer's recipe into your layer. Your `.bbappend` file
resides in your layer, while the main `.bb` recipe file to which you
are appending Metadata resides in a different layer.

Being able to append information to an existing recipe not only avoids
duplication, but also automatically applies recipe changes from a
different layer into your layer. If you were copying recipes, you would
have to manually merge changes as they occur.

When you create an append file, you must use the same root name as the
corresponding recipe file. For example, the append file
`someapp_3.1.bbappend` must apply to `someapp_3.1.bb`. This
means the original recipe and append filenames are version
number-specific. If the corresponding recipe is renamed to update to a
newer version, you must also rename and possibly update the
corresponding `.bbappend` as well.

During the build process, BitBake displays an error on startup if it detects a
`.bbappend` file that does not have a corresponding recipe with a matching
name. To handle these errors, the best practice is to rename the `.bbappend`
to match the original recipe version. This also gives you the opportunity to see
if the `.bbappend` is still relevant for the new version of the recipe.

Another method is to use the character `%` in the `.bbappend` filename. For
example, to append information to every `6.*` minor versions of the recipe
`someapp`, the `someapp_6.%.bbappend` file can be created. This way, an
error will only be triggered if the `someapp` recipe has a major version
update.

Finally, another method to deal with these errors is to use the variable
**BBMASK**, especially in cases where modifying the `.bbappend` is not
possible.

### Overlaying a File Using Your Layer

As an example, consider the main `formfactor` recipe and a corresponding
`formfactor` append file both from **OpenEmbedded-Core (OE-Core)**.
Here is the main
`formfactor` recipe, which is named `formfactor_0.0.bb` and located in
the "meta" layer at `meta/recipes-bsp/formfactor`::

   SUMMARY = "Device formfactor information"
   DESCRIPTION = "A formfactor configuration file provides information about the \
   target hardware for which the image is being built and information that the \
   build system cannot obtain from other sources such as the kernel."
   SECTION = "base"
   LICENSE = "MIT"
   LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

   SRC_URI = "file://config file://machconfig"
   S = "${UNPACKDIR}"

   PACKAGE_ARCH = "${MACHINE_ARCH}"
   INHIBIT_DEFAULT_DEPS = "1"

   do_install() {
           # Install file only if it has contents
           install -d ${D}${sysconfdir}/formfactor/
           install -m 0644 ${S}/config ${D}${sysconfdir}/formfactor/
           if [ -s "${S}/machconfig" ]; then
                   install -m 0644 ${S}/machconfig ${D}${sysconfdir}/formfactor/
           fi
   }

In the main recipe, note the **SRC_URI**
variable, which tells the OpenEmbedded build system where to find files
during the build.

Here is the append file, which is named `formfactor_0.0.bbappend`
and is from the Raspberry Pi BSP Layer named `meta-raspberrypi`. The
file is in the layer at `recipes-bsp/formfactor`::

   FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

By default, the build system uses the
**FILESPATH** variable to
locate files. This append file extends the locations by setting the
**FILESEXTRAPATHS**
variable. Setting this variable in the `.bbappend` file is the most
reliable and recommended method for adding directories to the search
path used by the build system to find files.

The statement in this example extends the directories to include
`${`\ **THISDIR**\ `}/${`\ **PN**\ `}`,
which resolves to a directory named `formfactor` in the same directory
in which the append file resides (i.e.
`meta-raspberrypi/recipes-bsp/formfactor`. This implies that you must
have the supporting directory structure set up that will contain any
files or patches you will be including from the layer.

Using the immediate expansion assignment operator `:=` is important
because of the reference to **THISDIR**. The trailing colon character is
important as it ensures that items in the list remain colon-separated.

> **Note:**
> BitBake automatically defines the **THISDIR** variable. You should
> never set this variable yourself. Using ":prepend" as part of the
> **FILESEXTRAPATHS** ensures your path will be searched prior to other
> paths in the final list.
>
> Also, not all append files add extra files. Many append files simply
> allow to add build options (e.g. `systemd`). For these cases, your
> append file would not even use the **FILESEXTRAPATHS** statement.
>

The end result of this `.bbappend` file is that on a Raspberry Pi, where
`rpi` will exist in the list of **OVERRIDES**, the file
`meta-raspberrypi/recipes-bsp/formfactor/formfactor/rpi/machconfig` will be
used during ref-tasks-fetch and the test for a non-zero file size in
ref-tasks-install will return true, and the file will be installed.

### Installing Additional Files Using Your Layer

As another example, consider the main `xserver-xf86-config` recipe and a
corresponding `xserver-xf86-config` append file both from
**OpenEmbedded-Core (OE-Core)**.  Here is the main `xserver-xf86-config` recipe, which is named
`xserver-xf86-config_0.1.bb` and located in the "meta" layer at
`meta/recipes-graphics/xorg-xserver`::

   SUMMARY = "X.Org X server configuration file"
   HOMEPAGE = "http://www.x.org"
   SECTION = "x11/base"
   LICENSE = "MIT"
   LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

   SRC_URI = "file://xorg.conf"

   S = "${UNPACKDIR}"

   CONFFILES:${PN} = "${sysconfdir}/X11/xorg.conf"

   PACKAGE_ARCH = "${MACHINE_ARCH}"
   ALLOW_EMPTY:${PN} = "1"

   do_install () {
        if test -s ${UNPACKDIR}/xorg.conf; then
                install -d ${D}/${sysconfdir}/X11
                install -m 0644 ${UNPACKDIR}/xorg.conf ${D}/${sysconfdir}/X11/
        fi
   }

Here is the append file, which is named `xserver-xf86-config_%.bbappend`
and is from the Raspberry Pi BSP Layer named `meta-raspberrypi`. The
file is in the layer at `recipes-graphics/xorg-xserver`::

   FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

   SRC_URI:append:rpi = " \
       file://xorg.conf.d/98-pitft.conf \
       file://xorg.conf.d/99-calibration.conf \
   "
   do_install:append:rpi () {
       PITFT = "${@bb.utils.contains("MACHINE_FEATURES", "pitft", "1", "0", d)}"
       if [ "${PITFT}" = "1" ]; then
           install -d ${D}/${sysconfdir}/X11/xorg.conf.d/
           install -m 0644 ${UNPACKDIR}/xorg.conf.d/98-pitft.conf ${D}/${sysconfdir}/X11/xorg.conf.d/
           install -m 0644 ${UNPACKDIR}/xorg.conf.d/99-calibration.conf ${D}/${sysconfdir}/X11/xorg.conf.d/
       fi
   }

   FILES:${PN}:append:rpi = " ${sysconfdir}/X11/xorg.conf.d/*"

Building off of the previous example, we once again are setting the
**FILESEXTRAPATHS** variable.  In this case we are also using
**SRC_URI** to list additional source files to use when `rpi` is found in
the list of **OVERRIDES**.  The ref-tasks-install task will then perform a
check for an additional **MACHINE_FEATURES** that if set will cause these
additional files to be installed.  These additional files are listed in
**FILES** so that they will be packaged.

## Prioritizing Your Layer

Each layer is assigned a priority value. Priority values control which
layer takes precedence if there are recipe files with the same name in
multiple layers. For these cases, the recipe file from the layer with a
higher priority number takes precedence. Priority values also affect the
order in which multiple `.bbappend` files for the same recipe are
applied. You can either specify the priority manually, or allow the
build system to calculate it based on the layer's dependencies.

To specify the layer's priority manually, use the
**BBFILE_PRIORITY**
variable and append the layer's root name::

   BBFILE_PRIORITY_mylayer = "1"

> **Note:**
> It is possible for a recipe with a lower version number
> **PV** in a layer that has a higher
> priority to take precedence.
>
> Also, the layer priority does not currently affect the precedence
> order of `.conf` or `.bbclass` files. Future versions of BitBake
> might address this.
>

### Providing Global-level Configurations With Your Layer

When creating a layer, you may need to define configurations that should take
effect globally in your build environment when the layer is part of the build.
The `layer.conf` file is a **configuration file** that affects the build
system globally, so it is a candidate for this use-case.

> **Warning:**
> Providing unconditional global level configuration from the `layer.conf`
> file is *not* a good practice, and should be avoided. For this reason, the
> section ref-conditional-layer-confs below shows how the `layer.conf`
> file can be used to provide configurations only if a certain condition is
> met.
>

For example, if your layer provides a Linux kernel recipe named
`linux-custom`, you may want to make :term:`PREFERRED_PROVIDER_virtual/kernel
<PREFERRED_PROVIDER>` point to `linux-custom`::

   PREFERRED_PROVIDER_virtual/kernel = "linux-custom"

This can be defined in the `layer.conf` file. If your layer is at the last
position in the **BBLAYERS** list, it will take precedence over previous
`PREFERRED_PROVIDER_virtual/kernel` assignments (unless one is set from a
**configuration file** that is parsed later, such as machine or distro
configuration files).

##### Conditionally Provide Global-level Configurations With Your Layer

In some cases, your layer may provide global configurations only if some
features it provides are enabled. Since the `layer.conf` file is parsed at an
earlier stage in the parsing process, the **DISTRO_FEATURES** and
**MACHINE_FEATURES** variables are not yet available to `layer.conf`, and
declaring conditional assignments based on these variables is not possible. The
following technique shows a way to bypass this limitation by using the
**USER_CLASSES** variable and a conditional `require` command.

In the following steps, let's assume our layer is named `meta-mylayer` and
that this layer defines a custom distro feature 
named `mylayer-kernel`. We will set the **PREFERRED_PROVIDER** variable
for the kernel only if our feature `mylayer-kernel` is part of the
**DISTRO_FEATURES**:

#. Create an include file in the directory
   `meta-mylayer/conf/distro/include/`, for example a file named
   `mylayer-kernel-provider.inc` that sets the kernel provider to
   `linux-custom`::

      PREFERRED_PROVIDER_virtual/kernel = "linux-custom"

#. Provide a path to this include file in your `layer.conf`::

      META_MYLAYER_KERNEL_PROVIDER_PATH = "${LAYERDIR}/conf/distro/include/mylayer-kernel-provider.inc"

#. Create a new class in `meta-mylayer/classes-global/`, for example a class
   `meta-mylayer-cfg.bbclass`. Make it conditionally require the file
   `mylayer-kernel-provider.inc` defined above, using the variable
   `META_MYLAYER_KERNEL_PROVIDER_PATH` defined in `layer.conf`::

      require ${@bb.utils.contains('DISTRO_FEATURES', 'mylayer-kernel', '${META_MYLAYER_KERNEL_PROVIDER_PATH}', '', d)}

   For details on the `bb.utils.contains` function, see its definition in
   `lib/bb/utils.py </tree/lib/bb/utils.py>`.

   .. note::

      The `require` command is designed to not fail if the function
      `bb.utils.contains` returns an empty string.

#. Back to your `layer.conf` file, add the class `meta-mylayer-cfg` class to
   the **USER_CLASSES** variable::

      USER_CLASSES:append = " meta-mylayer-cfg"

   This will add the class `meta-mylayer-cfg` to the list of classes to
   globally inherit. Since the `require` command is conditional in
   `meta-mylayer-cfg.bbclass`, even though inherited the class will have no
   effect unless the feature `mylayer-kernel` is enabled through
   **DISTRO_FEATURES**.

This technique can also be used for :ref:`Machine features
<ref-features-machine>` by following the same steps. Though not mandatory, it is
recommended to put include files for **DISTRO_FEATURES** in your layer's
`conf/distro/include` and the ones for **MACHINE_FEATURES** in your
layer's `conf/machine/include`.

## Managing Layers

You can use the BitBake layer management tool `bitbake-layers` to
provide a view into the structure of recipes across a multi-layer
project. Being able to generate output that reports on configured layers
with their paths and priorities and on `.bbappend` files and their
applicable recipes can help to reveal potential problems.

For help on the BitBake layer management tool, use the following
command::

   $ bitbake-layers --help

The following list describes the available commands:

-  `help:` Displays general help or help on a specified command.

-  `show-layers:` Shows the current configured layers.

-  `show-overlayed:` Lists overlayed recipes. A recipe is overlayed
   when a recipe with the same name exists in another layer that has a
   higher layer priority.

-  `show-recipes:` Lists available recipes and the layers that
   provide them.

-  `show-appends:` Lists `.bbappend` files and the recipe files to
   which they apply.

-  `show-cross-depends:` Lists dependency relationships between
   recipes that cross layer boundaries.

-  `add-layer:` Adds a layer to `bblayers.conf`.

-  `remove-layer:` Removes a layer from `bblayers.conf`

-  `flatten:` Flattens the layer configuration into a separate
   output directory. Flattening your layer configuration builds a
   "flattened" directory that contains the contents of all layers, with
   any overlayed recipes removed and any `.bbappend` files appended to
   the corresponding recipes. You might have to perform some manual
   cleanup of the flattened layer as follows:

   -  Non-recipe files (such as patches) are overwritten. The flatten
      command shows a warning for these files.

   -  Anything beyond the normal layer setup has been added to the
      `layer.conf` file. Only the lowest priority layer's
      `layer.conf` is used.

   -  Overridden and appended items from `.bbappend` files need to be
      cleaned up. The contents of each `.bbappend` end up in the
      flattened recipe. However, if there are appended or changed
      variable values, you need to tidy these up yourself. Consider the
      following example. Here, the `bitbake-layers` command adds the
      line `#### bbappended ...` so that you know where the following
      lines originate::

         ...
         DESCRIPTION = "A useful utility"
         ...
         EXTRA_OECONF = "--enable-something"
         ...

         #### bbappended from meta-anotherlayer ####

         DESCRIPTION = "Customized utility"
         EXTRA_OECONF += "--enable-somethingelse"

      Ideally, you would tidy up these utilities as follows::

         ...
         DESCRIPTION = "Customized utility"
         ...
         EXTRA_OECONF = "--enable-something --enable-somethingelse"
         ...

-  `layerindex-fetch`: Fetches a layer from a layer index, along
   with its dependent layers, and adds the layers to the
   `conf/bblayers.conf` file.

-  `layerindex-show-depends`: Finds layer dependencies from the
   layer index.

-  `save-build-conf`: Saves the currently active build configuration
   (`conf/local.conf`, `conf/bblayers.conf`) as a template into a layer.
   This template can later be used for setting up builds via **TEMPLATECONF**.
   For information about saving and using configuration templates, see
   "dev-manual/custom-template-configuration-directory:creating a custom template configuration directory".

-  `create-layer`: Creates a basic layer.

-  `create-layers-setup`: Writes out a configuration file and/or a script that
   can replicate the directory structure and revisions of the layers in a current build.
   For more information, see "dev-manual/layers:saving and restoring the layers setup".

-  `show-machines`: Lists the machines available in the currently configured layers.

## Adding a Layer Using the ``bitbake-layers`` Script

Once you create your general layer, you must add it to your
`bblayers.conf` file. Adding the layer to this configuration file
makes the OpenEmbedded build system aware of your layer so that it can
search it for metadata.

Add your layer by using the `bitbake-layers add-layer` command::

   $ bitbake-layers add-layer your_layer_name

Here is an example that adds a
layer named `meta-scottrif` to the configuration file. Following the
command that adds the layer is another `bitbake-layers` command that
shows the layers that are in your `bblayers.conf` file:

```console
$ bitbake-layers add-layer meta-scottrif
NOTE: Starting bitbake server...
Parsing recipes: 100% |##########################################################| Time: 0:00:49
Parsing of 1441 .bb files complete (0 cached, 1441 parsed). 2055 targets, 56 skipped, 0 masked, 0 errors.
$ bitbake-layers show-layers
NOTE: Starting bitbake server...
layer                 path                                                            priority
==============================================================================================
meta                  /home/scottrif/bitbake-builds/layers/openembedded-core/meta           5
meta-poky             /home/scottrif/bitbake-builds/layers/meta-yocto/meta-poky             5
meta-yocto-bsp        /home/scottrif/bitbake-builds/layers/meta-yocto/meta-yocto-bsp        5
workspace             /home/scottrif/bitbake-builds/build/workspace                         99
meta-scottrif         /home/scottrif/bitbake-builds/build/meta-scottrif                     6
```

Adding the layer to this file
enables the build system to locate the layer during the build.

> **Note:**
> During a build, the OpenEmbedded build system looks in the layers
> from the top of the list down to the bottom in that order.
>

## Saving and restoring the layers setup

Once you have a working build with the correct set of layers, it is beneficial
to capture the layer setup --- what they are, which repositories they come from
and which SCM revisions they're at --- into a configuration file, so that this
setup can be easily replicated later, perhaps on a different machine. Here's
how to do this::

   $ bitbake-layers create-layers-setup /srv/work/alex/meta-alex/
   NOTE: Starting bitbake server...
   NOTE: Created /srv/work/alex/meta-alex/setup-layers.json
   NOTE: Created /srv/work/alex/meta-alex/setup-layers

The tool needs a single argument which tells where to place the output, consisting
of a json formatted layer configuration, and a `setup-layers` script that can use that configuration
to restore the layers in a different location, or on a different host machine. The argument
can point to a custom layer (which is then deemed a "bootstrap" layer that needs to be
checked out first), or into a completely independent location.

The replication of the layers is performed by running the `setup-layers` script provided
above:

#. Clone the bootstrap layer or some other repository to obtain
   the json config and the setup script that can use it.

#. Run the script directly with no options::

      alex@Zen2:/srv/work/alex/my-build$ meta-alex/setup-layers
      Note: not checking out source meta-alex, use --force-bootstraplayer-checkout to override.

      Setting up source meta-intel, revision 15.0-hardknott-3.3-310-g0a96edae, branch master
      Running 'git init -q /srv/work/alex/my-build/meta-intel'
      Running 'git remote remove origin > /dev/null 2>&1; git remote add origin git://git.yoctoproject.org/meta-intel' in /srv/work/alex/my-build/meta-intel
      Running 'git fetch -q origin || true' in /srv/work/alex/my-build/meta-intel
      Running 'git checkout -q 0a96edae609a3f48befac36af82cf1eed6786b4a' in /srv/work/alex/my-build/meta-intel

      Setting up source poky, revision 4.1_M1-372-g55483d28f2, branch akanavin/setup-layers
      Running 'git init -q /srv/work/alex/my-build/poky'
      Running 'git remote remove origin > /dev/null 2>&1; git remote add origin git://git.yoctoproject.org/poky' in /srv/work/alex/my-build/poky
      Running 'git fetch -q origin || true' in /srv/work/alex/my-build/poky
      Running 'git remote remove poky-contrib > /dev/null 2>&1; git remote add poky-contrib ssh://git@push.yoctoproject.org/poky-contrib' in /srv/work/alex/my-build/poky
      Running 'git fetch -q poky-contrib || true' in /srv/work/alex/my-build/poky
      Running 'git checkout -q 11db0390b02acac1324e0f827beb0e2e3d0d1d63' in /srv/work/alex/my-build/poky

> **Note:**
> This will work to update an existing checkout as well.
>

> **Note:**
> The script is self-sufficient and requires only python3
> and git on the build machine.
>

> **Note:**
> Both the `create-layers-setup` and the `setup-layers` provided several additional options
> that customize their behavior - you are welcome to study them via `--help` command line parameter.
>
>
>

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Customizing Images

You can customize images to satisfy particular requirements. This
section describes several methods and provides guidelines for each.

## Customizing Images Using ``local.conf``

Probably the easiest way to customize an image is to add a package by
way of the `local.conf` configuration file. Because it is limited to
local use, this method generally only allows you to add packages and is
not as flexible as creating your own customized image. When you add
packages using local variables this way, you need to realize that these
variable changes are in effect for every build and consequently affect
all images, which might not be what you require.

To add a package to your image using the local configuration file, use
the **IMAGE_INSTALL** variable with the `:append` operator::

   IMAGE_INSTALL:append = " strace"

Use of the syntax is important; specifically, the leading space
after the opening quote and before the package name, which is
`strace` in this example. This space is required since the `:append`
operator does not add the space.

Furthermore, you must use `:append` instead of the `+=` operator if
you want to avoid ordering issues. The reason for this is because doing
so unconditionally appends to the variable and avoids ordering problems
due to the variable being set in image recipes and `.bbclass` files
with operators like `?=`. Using `:append` ensures the operation
takes effect.

As shown in its simplest use, `IMAGE_INSTALL:append` affects all
images. It is possible to extend the syntax so that the variable applies
to a specific image only. Here is an example::

   IMAGE_INSTALL:append:pn-core-image-minimal = " strace"

This example adds `strace` to the `core-image-minimal` image only.

You can add packages using a similar approach through the
**CORE_IMAGE_EXTRA_INSTALL** variable. If you use this variable, only
`core-image-*` images are affected.

## Customizing Images Using Custom ``IMAGE_FEATURES`` and ``EXTRA_IMAGE_FEATURES``

Another method for customizing your image is to enable or disable
high-level image features by using the
**IMAGE_FEATURES** and
**EXTRA_IMAGE_FEATURES**
variables. Although the functions for both variables are nearly
equivalent, best practices dictate using **IMAGE_FEATURES** from within
a recipe and using **EXTRA_IMAGE_FEATURES** from within your
`local.conf` file, which is found in the **Build Directory**.

To understand how these features work, the best reference is
meta/classes-recipe/image.bbclass .
This class lists out the available
**IMAGE_FEATURES** of which most map to package groups while some, such
as `read-only-rootfs`, resolve as general configuration settings.

In summary, the file looks at the contents of the **IMAGE_FEATURES**
variable and then maps or configures the feature accordingly. Based on
this information, the build system automatically adds the appropriate
packages or configurations to the
**IMAGE_INSTALL** variable.
Effectively, you are enabling extra features by extending the class or
creating a custom class for use with specialized image `.bb` files.

Use the **EXTRA_IMAGE_FEATURES** variable from within your local
configuration file. Using a separate area from which to enable features
with this variable helps you avoid overwriting the features in the image
recipe that are enabled with **IMAGE_FEATURES**. The value of
**EXTRA_IMAGE_FEATURES** is added to **IMAGE_FEATURES** within
`meta/conf/bitbake.conf`.

To illustrate how you can use these variables to modify your image, consider an
example that selects the SSH server. The Yocto Project ships with two SSH
servers you can use with your images: Dropbear and OpenSSH. Dropbear is a
minimal SSH server appropriate for resource-constrained environments, while
OpenSSH is a well-known standard SSH server implementation. By default, the
`core-image-sato` image is configured to use Dropbear. The
`core-image-full-cmdline` image includes OpenSSH. The `core-image-minimal`
image does not contain an SSH server.

You can customize your image and change these defaults. Edit the
**IMAGE_FEATURES** variable in your recipe or use the
**EXTRA_IMAGE_FEATURES** in your `local.conf` file so that it
configures the image you are working with to include
`ssh-server-dropbear` or `ssh-server-openssh`.

> **Note:**
> See the "ref-manual/features:image features" section in the Yocto
> Project Reference Manual for a complete list of image features that ship
> with the Yocto Project.
>

## Customizing Images Using Custom .bb Files

You can also customize an image by creating a custom recipe that defines
additional software as part of the image. The following example shows
the form for the two lines you need::

   IMAGE_INSTALL = "packagegroup-core-x11-base package1 package2"
   inherit core-image

Defining the software using a custom recipe gives you total control over
the contents of the image. It is important to use the correct names of
packages in the **IMAGE_INSTALL** variable. You must use the
OpenEmbedded notation and not the Debian notation for the names (e.g.
`glibc-dev` instead of `libc6-dev`).

The other method for creating a custom image is to base it on an
existing image. For example, if you want to create an image based on
`core-image-sato` but add the additional package `strace` to the
image, copy the `meta/recipes-sato/images/core-image-sato.bb` to a new
`.bb` and add the following line to the end of the copy::

   IMAGE_INSTALL += "strace"

## Customizing Images Using Custom Package Groups

For complex custom images, the best approach for customizing an image is
to create a custom package group recipe that is used to build the image
or images. A good example of a package group recipe is
`meta/recipes-core/packagegroups/packagegroup-base.bb`.

If you examine that recipe, you see that the **PACKAGES** variable lists
the package group packages to produce. The `inherit packagegroup`
statement sets appropriate default values and automatically adds
`-dev`, `-dbg`, and `-ptest` complementary packages for each
package specified in the **PACKAGES** statement.

> **Note:**
> The `inherit packagegroup` line should be located near the top of the
> recipe, certainly before the **PACKAGES** statement.
>

For each package you specify in **PACKAGES**, you can use **RDEPENDS**
and **RRECOMMENDS** entries to provide a list of packages the parent
task package should contain. You can see examples of these further down
in the `packagegroup-base.bb` recipe.

Here is a short, fabricated example showing the same basic pieces for a
hypothetical packagegroup defined in `packagegroup-custom.bb`, where
the variable **PN** is the standard way to abbreviate the reference to
the full packagegroup name `packagegroup-custom`::

   DESCRIPTION = "My Custom Package Groups"

   inherit packagegroup

   PACKAGES = "\
       ${PN}-apps \
       ${PN}-tools \
       "

   RDEPENDS:${PN}-apps = "\
       dropbear \
       portmap \
       psplash"

   RDEPENDS:${PN}-tools = "\
       oprofile \
       oprofileui-server \
       lttng-tools"

   RRECOMMENDS:${PN}-tools = "\
       kernel-module-oprofile"

In the previous example, two package group packages are created with
their dependencies and their recommended package dependencies listed:
`packagegroup-custom-apps`, and `packagegroup-custom-tools`. To
build an image using these package group packages, you need to add
`packagegroup-custom-apps` and/or `packagegroup-custom-tools` to
**IMAGE_INSTALL**. For other forms of image dependencies see the other
areas of this section.

## Customizing an Image Hostname

By default, the configured hostname (i.e. `/etc/hostname`) in an image
is the same as the machine name. For example, if
**MACHINE** equals "qemux86", the
configured hostname written to `/etc/hostname` is "qemux86".

You can customize this name by altering the value of the "hostname"
variable in the `base-files` recipe using either an append file or a
configuration file. Use the following in an append file::

   hostname = "myhostname"

Use the following in a configuration file::

   hostname:pn-base-files = "myhostname"

Changing the default value of the variable "hostname" can be useful in
certain situations. For example, suppose you need to do extensive
testing on an image and you would like to easily identify the image
under test from existing images with typical default hostnames. In this
situation, you could change the default hostname to "testme", which
results in all the images using the name "testme". Once testing is
complete and you do not need to rebuild the image for test any longer,
you can easily reset the default hostname.

Another point of interest is that if you unset the variable, the image
will have no default hostname in the filesystem. Here is an example that
unsets the variable in a configuration file::

  hostname:pn-base-files = ""

Having no default hostname in the filesystem is suitable for
environments that use dynamic hostnames such as virtual machines.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Writing a New Recipe

Recipes (`.bb` files) are fundamental components in the Yocto Project
environment. Each software component built by the OpenEmbedded build
system requires a recipe to define the component. This section describes
how to create, write, and test a new recipe.

> **Note:**
> For information on variables that are useful for recipes and for
> information about recipe naming issues, see the
> "ref-manual/varlocality:recipes" section of the Yocto Project
> Reference Manual.
>

## Overview

The following figure shows the basic process for creating a new recipe.
The remainder of the section provides details for the steps.

![recipe-workflow.png](figures/recipe-workflow.png)

## Locate or Automatically Create a Base Recipe

You can always write a recipe from scratch. However, there are three choices
that can help you quickly get started with a new recipe:

-  `devtool add`: A command that assists in creating a recipe and an
   environment conducive to development.

-  `recipetool create`: A command provided by the Yocto Project that
   automates creation of a base recipe based on the source files.

-  *Existing Recipes:* Location and modification of an existing recipe
   that is similar in function to the recipe you need.

> **Note:**
> For information on recipe syntax, see the
> "dev-manual/new-recipe:recipe syntax" section.
>

### Creating the Base Recipe Using ``devtool add``

The `devtool add` command uses the same logic for auto-creating the
recipe as `recipetool create`, which is listed below. Additionally,
however, `devtool add` sets up an environment that makes it easy for
you to patch the source and to make changes to the recipe as is often
necessary when adding a recipe to build a new piece of software to be
included in a build.

You can find a complete description of the `devtool add` command in
the "dev-manual/devtool:a closer look at `devtool add```" section
in the Yocto Project Application Development and the Extensible Software
Development Kit (eSDK) manual.

### Creating the Base Recipe Using ``recipetool create``

`recipetool create` automates creation of a base recipe given a set of
source code files. As long as you can extract or point to the source
files, the tool will construct a recipe and automatically configure all
pre-build information into the recipe. For example, suppose you have an
application that builds using Autotools. Creating the base recipe using
`recipetool` results in a recipe that has the pre-build dependencies,
license requirements, and checksums configured.

To run the tool, you just need to be in your **Build Directory** and
have sourced the build environment setup script (i.e.
structure-core-script). To get help on the tool, use the following
command::

   $ recipetool -h
   NOTE: Starting bitbake server...
   usage: recipetool [-d] [-q] [--color COLOR] [-h] <subcommand> ...

   OpenEmbedded recipe tool

   options:
     -d, --debug       Enable debug output
     -q, --quiet       Print only errors
     --color COLOR     Colorize output (where COLOR is auto, always, never)
     -h, --help        show this help message and exit

   subcommands:
     newappend         Create a bbappend for the specified target in the specified layer
     create            Create a new recipe
     setvar            Set a variable within a recipe
     appendfile        Create/update a bbappend to replace a target file
     appendsrcfiles    Create/update a bbappend to add or replace source files
     appendsrcfile     Create/update a bbappend to add or replace a source file
     edit              Edit the recipe and appends for the specified target. This obeys $VISUAL if set,
                       otherwise $EDITOR, otherwise vi.
   Use recipetool <subcommand> --help to get help on a specific command

Running `recipetool create -o OUTFILE` creates the base recipe and
locates it properly in the layer that contains your source files.
Here are some syntax examples:

 - Use this syntax to generate a recipe based on source. Once generated,
   the recipe resides in the existing source code layer::

      recipetool create -o OUTFILE source

 - Use this syntax to generate a recipe using code that
   you extract from source. The extracted code is placed in its own layer
   defined by **EXTERNALSRC**::

      recipetool create -o OUTFILE -x EXTERNALSRC source

 - Use this syntax to generate a recipe based on source. The options
   direct `recipetool` to generate debugging information. Once generated,
   the recipe resides in the existing source code layer::

      recipetool create -d -o OUTFILE source

### Locating and Using a Similar Recipe

Before writing a recipe from scratch, it is often useful to discover
whether someone else has already written one that meets (or comes close
to meeting) your needs. The Yocto Project and OpenEmbedded communities
maintain many recipes that might be candidates for what you are doing.
You can find a good central index of these recipes in the
`OpenEmbedded Layer Index <>`.

Working from an existing recipe or a skeleton recipe is the best way to
get started. Here are some points on both methods:

-  *Locate and modify a recipe that is close to what you want to do:*
   This method works when you are familiar with the current recipe
   space. The method does not work so well for those new to the Yocto
   Project or writing recipes.

   Some risks associated with this method are using a recipe that has
   areas totally unrelated to what you are trying to accomplish with
   your recipe, not recognizing areas of the recipe that you might have
   to add from scratch, and so forth. All these risks stem from
   unfamiliarity with the existing recipe space.

-  *Use and modify the following skeleton recipe:* If for some reason
   you do not want to use `recipetool` and you cannot find an existing
   recipe that is close to meeting your needs, you can use the following
   structure to provide the fundamental areas of a new recipe::

      DESCRIPTION = ""
      HOMEPAGE = ""
      LICENSE = ""
      SECTION = ""
      DEPENDS = ""
      LIC_FILES_CHKSUM = ""

      SRC_URI = ""

## Storing and Naming the Recipe

Once you have your base recipe, you should put it in your own layer and
name it appropriately. Locating it correctly ensures that the
OpenEmbedded build system can find it when you use BitBake to process
the recipe.

-  *Storing Your Recipe:* The OpenEmbedded build system locates your
   recipe through the layer's `conf/layer.conf` file and the
   **BBFILES** variable. This
   variable sets up a path from which the build system can locate
   recipes. Here is the typical use::

      BBFILES += "${LAYERDIR}/recipes-*/*/*.bb \
                  ${LAYERDIR}/recipes-*/*/*.bbappend"

   Consequently, you need to be sure you locate your new recipe inside
   your layer such that it can be found.

   You can find more information on how layers are structured in the
   "dev-manual/layers:understanding and creating layers" section.

-  *Naming Your Recipe:* When you name your recipe, you need to follow
   this naming convention::

      basename_version.bb

   Use lower-cased characters and do not include the reserved suffixes
   `-native`, `-cross`, `-initial`, or `-dev` casually (i.e. do not use
   them as part of your recipe name unless the string applies). Here are some
   examples (which includes the use of the string "git" as a special case of a
   version identifier):

   .. code-block:: none

      cups_2.4.12.bb
      gawk_5.3.2.bb
      psplash_git.bb

## Running a Build on the Recipe

Creating a new recipe is usually an iterative process that requires
using BitBake to process the recipe multiple times in order to
progressively discover and add information to the recipe file.

Assuming you have sourced the build environment setup script (i.e.
structure-core-script) and you are in the **Build Directory**, use
BitBake to process your recipe. All you need to provide is the
`basename` of the recipe as described in the previous section::

   $ bitbake basename

During the build, the OpenEmbedded build system creates a temporary work
directory for each recipe
(`${`\ **WORKDIR**\ `}`)
where it keeps extracted source files, log files, intermediate
compilation and packaging files, and so forth.

The path to the per-recipe temporary work directory depends on the
context in which it is being built. The quickest way to find this path
is to use the `bitbake-getvar` utility::

   $ bitbake-getvar -r basename WORKDIR

As an example, assume a **Source Directory**
top-level folder named `bitbake-builds`, a default **Build Directory** at
`bitbake-builds/build`, a `cortexa57` **PACKAGE_ARCH**, `poky`
**DISTRO** and `linux` as **TARGET_OS**.

Furthermore, suppose your recipe is named `foo_1.3.0.bb`. In this
case, the work directory the build system uses to build the package
would be as follows::

   bitbake-builds/build/tmp/work/cortexa57-poky-linux/foo/1.3.0

> **Note:**
> This covers the case when the **PACKAGE_ARCH** variable is set to its
> default value, **TUNE_PKGARCH**. This variable is sometimes set to
> **MACHINE_ARCH**. See the documentation of the **PACKAGE_ARCH**
> variable for more information.
>

Inside this directory you can find sub-directories such as `image`,
`packages-split`, and `temp`. After the build, you can examine these
to determine how well the build went.

> **Note:**
> You can find log files for each task in the recipe's `temp`
> directory (e.g. `bitbake-builds/build/tmp/work/cortexa57-poky-linux/foo/1.3.0/temp`).
> Log files are named `log.taskname` (e.g. `log.do_configure`,
> `log.do_fetch`, and `log.do_compile`).
>

You can find more information about the build process in
"/overview-manual/development-environment"
chapter of the Yocto Project Overview and Concepts Manual.

## Fetching Code

The first thing your recipe must do is specify how to fetch the source
files. Fetching is controlled mainly through the
**SRC_URI** variable. Your recipe
must have a **SRC_URI** variable that points to where the source is
located. For a graphical representation of source locations, see the
"overview-manual/concepts:sources" section in
the Yocto Project Overview and Concepts Manual.

The ref-tasks-fetch task uses the prefix of each entry in the
**SRC_URI** variable value to determine which
fetcher 
to use to get your source files. It is the **SRC_URI** variable that triggers
the fetcher. The ref-tasks-patch task uses the variable after source is
fetched to apply patches. The OpenEmbedded build system uses
**FILESOVERRIDES** for scanning directory locations for local files in
**SRC_URI**.

The **SRC_URI** variable in your recipe must define each unique location
for your source files. It is good practice to not hard-code version
numbers in a URL used in **SRC_URI**. Rather than hard-code these
values, use `${`\ **PV**\ `}`,
which causes the fetch process to use the version specified in the
recipe filename. Specifying the version in this manner means that
upgrading the recipe to a future version is as simple as renaming the
recipe to match the new version.

Here is a simple example from the
strace recipe 
where the source comes from a single tarball. Notice the use of the
**PV** variable::

   SRC_URI = "${GITHUB_BASE_URI}/download/v${PV}/strace-${PV}.tar.xz \

Files mentioned in **SRC_URI** whose names end in a typical archive
extension (e.g. `.tar`, `.tar.gz`, `.tar.bz2`, `.zip`, and so
forth), are automatically extracted during the
ref-tasks-unpack task. For
another example that specifies these types of files, see the
"dev-manual/new-recipe:building an autotooled package" section.

Another way of specifying source is from an SCM. For Git repositories,
you must specify **SRCREV** and you should specify **PV** to include
a `+` sign in its definition. Here is an example from the recipe
l3afpad_git.bb ::

   SRC_URI = "git://github.com/stevenhoneyman/l3afpad.git;branch=master;protocol=https"

   PV = "0.8.18.1.11+git"
   SRCREV = "3cdccdc9505643e50f8208171d9eee5de11a42ff"

If your **SRC_URI** statement includes URLs pointing to individual files
fetched from a remote server other than a version control system,
BitBake attempts to verify the files against checksums defined in your
recipe to ensure they have not been tampered with or otherwise modified
since the recipe was written. Multiple checksums are supported:
`SRC_URI[md5sum]`, `SRC_URI[sha1sum]`, `SRC_URI[sha256sum]`.
`SRC_URI[sha384sum]` and `SRC_URI[sha512sum]`, but only
`SRC_URI[sha256sum]` is commonly used.

> **Note:**
> `SRC_URI[md5sum]` used to also be commonly used, but it is deprecated
> and should be replaced by `SRC_URI[sha256sum]` when updating existing
> recipes.
>

If your **SRC_URI** variable points to more than a single URL (excluding
SCM URLs), you need to provide the `sha256` checksum for each URL. For these
cases, you provide a name for each URL as part of the **SRC_URI** and then
reference that name in the subsequent checksum statements. Here is an example
combining lines from the files `git.inc` and `git_2.24.1.bb`::

   SRC_URI = "${KERNELORG_MIRROR}/software/scm/git/git-${PV}.tar.gz;name=tarball \
              ${KERNELORG_MIRROR}/software/scm/git/git-manpages-${PV}.tar.gz;name=manpages"

   SRC_URI[tarball.sha256sum] = "ad5334956301c86841eb1e5b1bb20884a6bad89a10a6762c958220c7cf64da02"
   SRC_URI[manpages.sha256sum] = "9a7ae3a093bea39770eb96ca3e5b40bff7af0b9f6123f089d7821d0e5b8e1230"

The proper value for the `sha256` checksum might be available together
with other signatures on the download page for the upstream source (e.g.
`md5`, `sha1`, `sha256`, `GPG`, and so forth). Because the
OpenEmbedded build system typically only deals with `sha256sum`,
you should verify all the signatures you find by hand.

If no **SRC_URI** checksums are specified when you attempt to build the
recipe, or you provide an incorrect checksum, the build will produce an
error for each missing or incorrect checksum. As part of the error
message, the build system provides the checksum string corresponding to
the fetched file. Once you have the correct checksums, you can copy and
paste them into your recipe and then run the build again to continue.

> **Note:**
> As mentioned, if the upstream source provides signatures for
> verifying the downloaded source code, you should verify those
> manually before setting the checksum values in the recipe and
> continuing with the build.
>

This final example is a bit more complicated and is from the
rxvt-unicode 
recipe. The example's **SRC_URI** statement identifies multiple files as the source
files for the recipe: a tarball, a patch file, a desktop file, and an icon::

   SRC_URI = "http://dist.schmorp.de/rxvt-unicode/Attic/rxvt-unicode-${PV}.tar.bz2 \
              file://xwc.patch \
              file://rxvt.desktop \
              file://rxvt.png"

When you specify local files using the `file://` URI protocol, the
build system fetches files from the local machine. The path is relative
to the **FILESPATH** variable
and searches specific directories in a certain order:
`${`\ **BP**\ `}`,
`${`\ **BPN**\ `}`, and
`files`. The directories are assumed to be subdirectories of the
directory in which the recipe or append file resides. For another
example that specifies these types of files, see the
"`building a single .c file package`_" section.

The previous example also specifies a patch file. Patch files are files
whose names usually end in `.patch` or `.diff` but can end with
compressed suffixes such as `diff.gz` and `patch.bz2`, for example.
The build system automatically applies patches as described in the
"dev-manual/new-recipe:patching code" section.

### Fetching Code Through Firewalls

Some users are behind firewalls and need to fetch code through a proxy.
See the "/ref-manual/faq" chapter for advice.

### Limiting the Number of Parallel Connections

Some users are behind firewalls or use servers where the number of parallel
connections is limited. In such cases, you can limit the number of fetch
tasks being run in parallel by adding the following to your `local.conf`
file::

   do_fetch[number_threads] = "4"

## Unpacking Code

During the build, the
ref-tasks-unpack task unpacks
the source with `${`\ **S**\ `}`
pointing to where it is unpacked.

If you are fetching your source files from an upstream source archived
tarball and the tarball's internal structure matches the common
convention of a top-level subdirectory named
`${`\ **BPN**\ `}-${`\ **PV**\ `}`,
then you do not need to set **S**. However, if **SRC_URI** specifies to
fetch source from an archive that does not use this convention, or from
an SCM like Git or Subversion, your recipe needs to define **S**.

If processing your recipe using BitBake successfully unpacks the source
files, you need to be sure that the directory pointed to by `${S}`
matches the structure of the source.

## Patching Code

Sometimes it is necessary to patch code after it has been fetched. Any
files mentioned in **SRC_URI** whose names end in `.patch` or
`.diff` or compressed versions of these suffixes (e.g. `diff.gz`,
`patch.bz2`, etc.) are treated as patches. The
ref-tasks-patch task
automatically applies these patches.

The build system should be able to apply patches with the "-p1" option
(i.e. one directory level in the path will be stripped off). If your
patch needs to have more directory levels stripped off, specify the
number of levels using the "striplevel" option in the **SRC_URI** entry
for the patch. Alternatively, if your patch needs to be applied in a
specific subdirectory that is not specified in the patch file, use the
"patchdir" option in the entry.

As with all local files referenced in
**SRC_URI** using `file://`,
you should place patch files in a directory next to the recipe either
named the same as the base name of the recipe
(**BP** and
**BPN**) or "files".

## Licensing

Your recipe needs to define variables related to the license
under which the software is distributed. See the
contributor-guide/recipe-style-guide:recipe license fields
section in the Contributor Guide for details.

## Dependencies

Most software packages have a short list of other packages that they
require, which are called dependencies. These dependencies fall into two
main categories: build-time dependencies, which are required when the
software is built; and runtime dependencies, which are required to be
installed on the target in order for the software to run.

Within a recipe, you specify build-time dependencies using the
**DEPENDS** variable. Although there are nuances,
items specified in **DEPENDS** should be names of other
recipes. It is important that you specify all build-time dependencies
explicitly.

Another consideration is that configure scripts might automatically
check for optional dependencies and enable corresponding functionality
if those dependencies are found. If you wish to make a recipe that is
more generally useful (e.g. publish the recipe in a layer for others to
use), instead of hard-disabling the functionality, you can use the
**PACKAGECONFIG** variable to allow functionality and the
corresponding dependencies to be enabled and disabled easily by other
users of the recipe.

Similar to build-time dependencies, you specify runtime dependencies
through a variable -
**RDEPENDS**, which is
package-specific. All variables that are package-specific need to have
the name of the package added to the end as an override. Since the main
package for a recipe has the same name as the recipe, and the recipe's
name can be found through the
`${`\ **PN**\ `}` variable, then
you specify the dependencies for the main package by setting
`RDEPENDS:${PN}`. If the package were named `${PN}-tools`, then you
would set `RDEPENDS:${PN}-tools`, and so forth.

Some runtime dependencies will be set automatically at packaging time.
These dependencies include any shared library dependencies (i.e. if a
package "example" contains "libexample" and another package "mypackage"
contains a binary that links to "libexample" then the OpenEmbedded build
system will automatically add a runtime dependency to "mypackage" on
"example"). See the
"overview-manual/concepts:automatically added runtime dependencies"
section in the Yocto Project Overview and Concepts Manual for further
details.

## Configuring the Recipe

Most software provides some means of setting build-time configuration
options before compilation. Typically, setting these options is
accomplished by running a configure script with options, or by modifying
a build configuration file.

> **Note:**
> As of Yocto Project Release 1.7, some of the core recipes that
> package binary configuration scripts now disable the scripts due to
> the scripts previously requiring error-prone path substitution. The
> OpenEmbedded build system uses `pkg-config` now, which is much more
> robust. You can find a list of the `*-config` scripts that are disabled
> in the "migration-1.7-binary-configuration-scripts-disabled" section
> in the Yocto Project Reference Manual.
>

A major part of build-time configuration is about checking for
build-time dependencies and possibly enabling optional functionality as
a result. You need to specify any build-time dependencies for the
software you are building in your recipe's
**DEPENDS** value, in terms of
other recipes that satisfy those dependencies. You can often find
build-time or runtime dependencies described in the software's
documentation.

The following list provides configuration items of note based on how
your software is built:

-  *Autotools:* If your source files have a `configure.ac` file, then
   your software is built using Autotools. If this is the case, you just
   need to modify the configuration.

   When using Autotools, your recipe needs to inherit the
   ref-classes-autotools class and it does not have to
   contain a ref-tasks-configure task. However, you might still want to
   make some adjustments. For example, you can set **EXTRA_OECONF** or
   **PACKAGECONFIG_CONFARGS** to pass any needed configure options that
   are specific to the recipe.

-  *CMake:* If your source files have a `CMakeLists.txt` file, then
   your software is built using CMake. If this is the case, you just
   need to modify the configuration.

   When you use CMake, your recipe needs to inherit the
   ref-classes-cmake class and it does not have to contain a
   ref-tasks-configure task. You can make some adjustments by setting
   **EXTRA_OECMAKE** to pass any needed configure options that are
   specific to the recipe.

   .. note::

      If you need to install one or more custom CMake toolchain files
      that are supplied by the application you are building, install the
      files to `${D}${datadir}/cmake/Modules` during ref-tasks-install.

-  *Other:* If your source files do not have a `configure.ac` or
   `CMakeLists.txt` file, then your software is built using some
   method other than Autotools or CMake. If this is the case, you
   normally need to provide a
   ref-tasks-configure task
   in your recipe unless, of course, there is nothing to configure.

   Even if your software is not being built by Autotools or CMake, you
   still might not need to deal with any configuration issues. You need
   to determine if configuration is even a required step. You might need
   to modify a Makefile or some configuration file used for the build to
   specify necessary build options. Or, perhaps you might need to run a
   provided, custom configure script with the appropriate options.

   For the case involving a custom configure script, you would run
   `./configure --help` and look for the options you need to set.

Once configuration succeeds, it is always good practice to look at the
`log.do_configure` file to ensure that the appropriate options have
been enabled and no additional build-time dependencies need to be added
to **DEPENDS**. For example, if the configure script reports that it
found something not mentioned in **DEPENDS**, or that it did not find
something that it needed for some desired optional functionality, then
you would need to add those to **DEPENDS**. Looking at the log might
also reveal items being checked for, enabled, or both that you do not
want, or items not being found that are in **DEPENDS**, in which case
you would need to look at passing extra options to the configure script
as needed. For reference information on configure options specific to
the software you are building, you can consult the output of the
`./configure --help` command within `${S}` or consult the software's
upstream documentation.

## Using Headers to Interface with Devices

If your recipe builds an application that needs to communicate with some
device or needs an API into a custom kernel, you will need to provide
appropriate header files. Under no circumstances should you ever modify
the existing
`meta/recipes-kernel/linux-libc-headers/linux-libc-headers.inc` file.
These headers are used to build `libc` and must not be compromised
with custom or machine-specific header information. If you customize
`libc` through modified headers all other applications that use
`libc` thus become affected.

> **Note:**
> Never copy and customize the `libc` header file (i.e.
> `meta/recipes-kernel/linux-libc-headers/linux-libc-headers.inc`).
>

The correct way to interface to a device or custom kernel is to use a
separate package that provides the additional headers for the driver or
other unique interfaces. When doing so, your application also becomes
responsible for creating a dependency on that specific provider.

Consider the following:

-  Never modify `linux-libc-headers.inc`. Consider that file to be
   part of the `libc` system, and not something you use to access the
   kernel directly. You should access `libc` through specific `libc`
   calls.

-  Applications that must talk directly to devices should either provide
   necessary headers themselves, or establish a dependency on a special
   headers package that is specific to that driver.

For example, suppose you want to modify an existing header that adds I/O
control or network support. If the modifications are used by a small
number programs, providing a unique version of a header is easy and has
little impact. When doing so, bear in mind the guidelines in the
previous list.

> **Note:**
> If for some reason your changes need to modify the behavior of the `libc`,
> and subsequently all other applications on the system, use a `.bbappend`
> to modify the `linux-kernel-headers.inc` file. However, take care to not
> make the changes machine specific.
>

Consider a case where your kernel is older and you need an older
`libc` ABI. The headers installed by your recipe should still be a
standard mainline kernel, not your own custom one.

When you use custom kernel headers you need to get them from
**STAGING_KERNEL_DIR**,
which is the directory with kernel headers that are required to build
out-of-tree modules. Your recipe will also need the following::

   do_configure[depends] += "virtual/kernel:do_shared_workdir"

## Compilation

During a build, the ref-tasks-compile task happens after source is fetched,
unpacked, and configured. If the recipe passes through ref-tasks-compile
successfully, nothing needs to be done.

However, if the compile step fails, you need to diagnose the failure.
Here are some common issues that cause failures.

> **Note:**
> For cases where improper paths are detected for configuration files
> or for when libraries/headers cannot be found, be sure you are using
> the more robust `pkg-config`. See the note in section
> "dev-manual/new-recipe:Configuring the Recipe" for additional information.
>

-  *Parallel build failures:* These failures manifest themselves as
   intermittent errors, or errors reporting that a file or directory
   that should be created by some other part of the build process could
   not be found. This type of failure can occur even if, upon
   inspection, the file or directory does exist after the build has
   failed, because that part of the build process happened in the wrong
   order.

   To fix the problem, you need to either satisfy the missing dependency
   in the Makefile or whatever script produced the Makefile, or (as a
   workaround) set **PARALLEL_MAKE** to an empty string::

      PARALLEL_MAKE = ""

   For information on parallel Makefile issues, see the
   "dev-manual/debugging:debugging parallel make races" section.

-  *Improper host path usage:* This failure applies to recipes building
   for the target or "ref-classes-nativesdk" only. The
   failure occurs when the compilation process uses improper headers,
   libraries, or other files from the host system when cross-compiling for
   the target.

   To fix the problem, examine the `log.do_compile` file to identify
   the host paths being used (e.g. `/usr/include`, `/usr/lib`, and
   so forth) and then either add configure options, apply a patch, or do
   both.

-  *Failure to find required libraries/headers:* If a build-time
   dependency is missing because it has not been declared in
   **DEPENDS**, or because the
   dependency exists but the path used by the build process to find the
   file is incorrect and the configure step did not detect it, the
   compilation process could fail. For either of these failures, the
   compilation process notes that files could not be found. In these
   cases, you need to go back and add additional options to the
   configure script as well as possibly add additional build-time
   dependencies to **DEPENDS**.

   Occasionally, it is necessary to apply a patch to the source to
   ensure the correct paths are used. If you need to specify paths to
   find files staged into the sysroot from other recipes, use the
   variables that the OpenEmbedded build system provides (e.g.
   **STAGING_BINDIR**, **STAGING_INCDIR**, **STAGING_DATADIR**, and so
   forth).

## Installing

During ref-tasks-install, the task copies the built files along with their
hierarchy to locations that would mirror their locations on the target
device. The installation process copies files from the
`${`\ **S**\ `}`,
`${`\ **B**\ `}`, and
`${`\ **UNPACKDIR**\ `}`
directories to the `${`\ **D**\ `}`
directory to create the structure as it should appear on the target
system.

How your software is built affects what you must do to be sure your
software is installed correctly. The following list describes what you
must do for installation depending on the type of build system used by
the software being built:

-  *Autotools and CMake:* If the software your recipe is building uses
   Autotools or CMake, the OpenEmbedded build system understands how to
   install the software. Consequently, you do not have to have a
   ref-tasks-install task as part of your recipe. You just need to make
   sure the install portion of the build completes with no issues.
   However, if you wish to install additional files not already being
   installed by `make install`, you should do this using a
   `do_install:append` function using the install command as described
   in the "Manual" bulleted item later in this list.

-  *Other (using* `make install`\ *)*: You need to define a ref-tasks-install
   function in your recipe. The function should call
   `oe_runmake install` and will likely need to pass in the
   destination directory as well. How you pass that path is dependent on
   how the `Makefile` being run is written (e.g. `DESTDIR=${D}`,
   `PREFIX=${D}`, `INSTALLROOT=${D}`, and so forth).

   For an example recipe using `make install`, see the
   "dev-manual/new-recipe:building a makefile-based package" section.

-  *Manual:* You need to define a ref-tasks-install function in your
   recipe. The function must first use `install -d` to create the
   directories under
   `${`\ **D**\ `}`. Once the
   directories exist, your function can use `install` to manually
   install the built software into the directories.

   You can find more information on `install` at
   https://www.gnu.org/software/coreutils/manual/html_node/install-invocation.html.

For the scenarios that do not use Autotools or CMake, you need to track
the installation and diagnose and fix any issues until everything
installs correctly. You need to look in the default location of
`${D}`, which is `${WORKDIR}/image`, to be sure your files have been
installed correctly.

> **Note:**
> -  During the installation process, you might need to modify some of
> the installed files to suit the target layout. For example, you
> might need to replace hard-coded paths in an initscript with
> values of variables provided by the build system, such as
> replacing `/usr/bin/` with `${bindir}`. If you do perform such
> modifications during ref-tasks-install, be sure to modify the
> destination file after copying rather than before copying.
> Modifying after copying ensures that the build system can
> re-execute ref-tasks-install if needed.
>
> -  `oe_runmake install`, which can be run directly or can be run
> indirectly by the ref-classes-autotools and
> ref-classes-cmake classes, runs `make install` in parallel.
> Sometimes, a Makefile can have missing dependencies between targets that
> can result in race conditions. If you experience intermittent failures
> during ref-tasks-install, you might be able to work around them by
> disabling parallel Makefile installs by adding the following to the
> recipe::
>
> PARALLEL_MAKEINST = ""
>
> See **PARALLEL_MAKEINST** for additional information.
>
> -  If you need to install one or more custom CMake toolchain files
> that are supplied by the application you are building, install the
> files to `${D}${datadir}/cmake/Modules` during
> ref-tasks-install.
>

## Enabling System Services

If you want to install a service, which is a process that usually starts
on boot and runs in the background, then you must include some
additional definitions in your recipe.

If you are adding services and the service initialization script or the
service file itself is not installed, you must provide for that
installation in your recipe using a `do_install:append` function. If
your recipe already has a ref-tasks-install function, update the function
near its end rather than adding an additional `do_install:append`
function.

When you create the installation for your services, you need to
accomplish what is normally done by `make install`. In other words,
make sure your installation arranges the output similar to how it is
arranged on the target system.

The OpenEmbedded build system provides support for starting services two
different ways:

-  *SysVinit:* SysVinit is a system and service manager that manages the
   init system used to control the very basic functions of your system.
   The init program is the first program started by the Linux kernel
   when the system boots. Init then controls the startup, running and
   shutdown of all other programs.

   To enable a service using SysVinit, your recipe needs to inherit the
   ref-classes-update-rc.d class. The class helps
   facilitate safely installing the package on the target.

   You will need to set the
   **INITSCRIPT_PACKAGES**,
   **INITSCRIPT_NAME**,
   and
   **INITSCRIPT_PARAMS**
   variables within your recipe.

-  *systemd:* System Management Daemon (systemd) was designed to replace
   SysVinit and to provide enhanced management of services. For more
   information on systemd, see the systemd homepage at
   https://freedesktop.org/wiki/Software/systemd/.

   To enable a service using systemd, your recipe needs to inherit the
   ref-classes-systemd class. See the `systemd.bbclass` file
   located in **OpenEmbedded-Core (OE-Core)** section for more information.

## Packaging

Successful packaging is a combination of automated processes performed
by the OpenEmbedded build system and some specific steps you need to
take. The following list describes the process:

-  *Splitting Files*: The ref-tasks-package task splits the files produced
   by the recipe into logical components. Even software that produces a
   single binary might still have debug symbols, documentation, and
   other logical components that should be split out. The ref-tasks-package
   task ensures that files are split up and packaged correctly.

-  *Running QA Checks*: The ref-classes-insane class adds a
   step to the package generation process so that output quality
   assurance checks are generated by the OpenEmbedded build system. This
   step performs a range of checks to be sure the build's output is free
   of common problems that show up during runtime. For information on
   these checks, see the ref-classes-insane class and
   the "ref-manual/qa-checks:qa error and warning messages"
   chapter in the Yocto Project Reference Manual.

-  *Hand-Checking Your Packages*: After you build your software, you
   need to be sure your packages are correct. Examine the
   `${`\ **WORKDIR**\ `}/packages-split`
   directory and make sure files are where you expect them to be. If you
   discover problems, you can set
   **PACKAGES**,
   **FILES**,
   `do_install(:append)`, and so forth as needed.

-  *Splitting an Application into Multiple Packages*: If you need to
   split an application into several packages, see the
   "dev-manual/new-recipe:splitting an application into multiple packages"
   section for an example.

-  *Installing a Post-Installation Script*: For an example showing how
   to install a post-installation script, see the
   "dev-manual/new-recipe:post-installation scripts" section.

-  *Marking Package Architecture*: Depending on what your recipe is
   building and how it is configured, it might be important to mark the
   packages produced as being specific to a particular machine, or to
   mark them as not being specific to a particular machine or
   architecture at all.

   By default, packages apply to any machine with the same architecture
   as the target machine. When a recipe produces packages that are
   machine-specific (e.g. the
   **MACHINE** value is passed
   into the configure script or a patch is applied only for a particular
   machine), you should mark them as such by adding the following to the
   recipe::

      PACKAGE_ARCH = "${MACHINE_ARCH}"

   On the other hand, if the recipe produces packages that do not
   contain anything specific to the target machine or architecture at
   all (e.g. recipes that simply package script files or configuration
   files), you should use the ref-classes-allarch class to
   do this for you by adding this to your recipe::

      inherit allarch

   Ensuring that the package architecture is correct is not critical
   while you are doing the first few builds of your recipe. However, it
   is important in order to ensure that your recipe rebuilds (or does
   not rebuild) appropriately in response to changes in configuration,
   and to ensure that you get the appropriate packages installed on the
   target machine, particularly if you run separate builds for more than
   one target machine.

## Sharing Files Between Recipes

Recipes often need to use files provided by other recipes on the build
host. For example, an application linking to a common library needs
access to the library itself and its associated headers. The way this
access is accomplished is by populating a sysroot with files. Each
recipe has two sysroots in its work directory, one for target files
(`recipe-sysroot`) and one for files that are native to the build host
(`recipe-sysroot-native`).

> **Note:**
> You could find the term "staging" used within the Yocto project
> regarding files populating sysroots (e.g. the **STAGING_DIR**
> variable).
>

Recipes should never populate the sysroot directly (i.e. write files
into sysroot). Instead, files should be installed into standard
locations during the
ref-tasks-install task within
the `${`\ **D**\ `}` directory. The
reason for this limitation is that almost all files that populate the
sysroot are cataloged in manifests in order to ensure the files can be
removed later when a recipe is either modified or removed. Thus, the
sysroot is able to remain free from stale files.

A subset of the files installed by the ref-tasks-install task are
used by the ref-tasks-populate_sysroot task as defined by the
**SYSROOT_DIRS** variable to automatically populate the sysroot. It
is possible to modify the list of directories that populate the sysroot.
The following example shows how you could add the `/opt` directory to
the list of directories within a recipe::

   SYSROOT_DIRS += "/opt"

> **Note:**
> The `/sysroot-only` is to be used by recipes that generate artifacts
> that are not included in the target filesystem, allowing them to share
> these artifacts without needing to use the **DEPLOY_DIR**.
>

For a more complete description of the ref-tasks-populate_sysroot
task and its associated functions, see the
staging  class.

## Using Virtual Providers

Prior to a build, if you know that several different recipes provide the
same functionality, you can use a virtual provider (i.e. `virtual/*`)
as a placeholder for the actual provider. The actual provider is
determined at build-time.

A common scenario where a virtual provider is used would be for the kernel
recipe. Suppose you have three kernel recipes whose **PN** values map to
`kernel-big`, `kernel-mid`, and `kernel-small`. Furthermore, each of
these recipes in some way uses a **PROVIDES** statement that essentially
identifies itself as being able to provide `virtual/kernel`. Here is one way
through the ref-classes-kernel class::

   PROVIDES += "virtual/kernel"

Any recipe that inherits the ref-classes-kernel class is
going to utilize a **PROVIDES** statement that identifies that recipe as
being able to provide the `virtual/kernel` item.

Now comes the time to actually build an image and you need a kernel
recipe, but which one? You can configure your build to call out the
kernel recipe you want by using the **PREFERRED_PROVIDER** variable. As
an example, consider the :oe_git:`x86-base.inc
</openembedded-core/tree/meta/conf/machine/include/x86/x86-base.inc>` include file, which is a
machine (i.e. **MACHINE**) configuration file. This include file is the
reason all x86-based machines use the `linux-yocto` kernel. Here are the
relevant lines from the include file::

   PREFERRED_PROVIDER_virtual/kernel ??= "linux-yocto"
   PREFERRED_VERSION_linux-yocto ??= "4.15%"

When you use a virtual provider, you do not have to "hard code" a recipe
name as a build dependency. You can use the
**DEPENDS** variable to state the
build is dependent on `virtual/kernel` for example::

   DEPENDS = "virtual/kernel"

During the build, the OpenEmbedded build system picks
the correct recipe needed for the `virtual/kernel` dependency based on
the **PREFERRED_PROVIDER** variable. If you want to use the small kernel
mentioned at the beginning of this section, configure your build as
follows::

   PREFERRED_PROVIDER_virtual/kernel ??= "kernel-small"

> **Note:**
> Any recipe that **PROVIDES** a `virtual/*` item that is ultimately not
> selected through **PREFERRED_PROVIDER** does not get built. Preventing these
> recipes from building is usually the desired behavior since this mechanism's
> purpose is to select between mutually exclusive alternative providers.
>

The following lists specific examples of virtual providers:

-  `virtual/kernel`: Provides the name of the kernel recipe to use
   when building a kernel image.

-  `virtual/bootloader`: Provides the name of the bootloader to use
   when building an image.

-  `virtual/libgbm`: Provides `gbm.pc`.

-  `virtual/egl`: Provides `egl.pc` and possibly `wayland-egl.pc`.

-  `virtual/libgl`: Provides `gl.pc` (i.e. libGL).

-  `virtual/libgles1`: Provides `glesv1_cm.pc` (i.e. libGLESv1_CM).

-  `virtual/libgles2`: Provides `glesv2.pc` (i.e. libGLESv2).

> **Note:**
> Virtual providers only apply to build time dependencies specified with
> **PROVIDES** and **DEPENDS**. They do not apply to runtime
> dependencies specified with **RPROVIDES** and **RDEPENDS**.
>

## Properly Versioning Pre-Release Recipes

Sometimes the name of a recipe can lead to versioning problems when the
recipe is upgraded to a final release. For example, consider the
`irssi_0.8.16-rc1.bb` recipe file in the list of example recipes in
the "dev-manual/new-recipe:storing and naming the recipe" section.
This recipe is at a release candidate stage (i.e. "rc1"). When the recipe is
released, the recipe filename becomes `irssi_0.8.16.bb`. The version
change from `0.8.16-rc1` to `0.8.16` is seen as a decrease by the
build system and package managers, so the resulting packages will not
correctly trigger an upgrade.

In order to ensure the versions compare properly, the recommended
convention is to use a tilde (`~`) character as follows::

  PV = 0.8.16~rc1

This way `0.8.16~rc1` sorts before `0.8.16`. See the
"contributor-guide/recipe-style-guide:version policy" section in the
Yocto Project and OpenEmbedded Contributor Guide for more details about
versioning code corresponding to a pre-release or to a specific Git commit.

## Post-Installation Scripts

Post-installation scripts run immediately after installing a package on
the target or during image creation when a package is included in an
image. To add a post-installation script to a package, add a
`pkg_postinst:`\ `PACKAGENAME`\ `()` function to the recipe file
(`.bb`) and replace `PACKAGENAME` with the name of the package you want
to attach to the `postinst` script. To apply the post-installation
script to the main package for the recipe, which is usually what is
required, specify
`${`\ **PN**\ `}` in place of
PACKAGENAME.

A post-installation function has the following structure::

   pkg_postinst:PACKAGENAME() {
       # Commands to carry out
   }

The script defined in the post-installation function is called when the
root filesystem is created. If the script succeeds, the package is
marked as installed.

> **Note:**
> Any RPM post-installation script that runs on the target should
> return a 0 exit code. RPM does not allow non-zero exit codes for
> these scripts, and the RPM package manager will cause the package to
> fail installation on the target.
>

Sometimes it is necessary for the execution of a post-installation
script to be delayed until the first boot. For example, the script might
need to be executed on the device itself. To delay script execution
until boot time, you must explicitly mark post installs to defer to the
target. You can use `pkg_postinst_ontarget()` or call
`postinst_intercept delay_to_first_boot` from `pkg_postinst()`. Any
failure of a `pkg_postinst()` script (including exit 1) triggers an
error during the
ref-tasks-rootfs task.

If you have recipes that use `pkg_postinst` function and they require
the use of non-standard native tools that have dependencies during
root filesystem construction, you need to use the
**PACKAGE_WRITE_DEPS**
variable in your recipe to list these tools. If you do not use this
variable, the tools might be missing and execution of the
post-installation script is deferred until first boot. Deferring the
script to the first boot is undesirable and impossible for read-only
root filesystems.

> **Note:**
> There is equivalent support for pre-install, pre-uninstall, and post-uninstall
> scripts by way of `pkg_preinst`, `pkg_prerm`, and `pkg_postrm`,
> respectively. These scrips work in exactly the same way as does
> `pkg_postinst` with the exception that they run at different times. Also,
> because of when they run, they are not applicable to being run at image
> creation time like `pkg_postinst`.
>

## Testing

The final step for completing your recipe is to be sure that the
software you built runs correctly. To accomplish runtime testing, add
the build's output packages to your image and test them on the target.

For information on how to customize your image by adding specific
packages, see "dev-manual/customizing-images:customizing images" section.

## Examples

To help summarize how to write a recipe, this section provides some
recipe examples given various scenarios:

-  `Building a single .c file package`_

-  `Building a Makefile-based package`_

-  `Building an Autotooled package`_

-  `Building a Meson package`_

-  `Splitting an application into multiple packages`_

-  `Packaging externally produced binaries`_

### Building a Single .c File Package

Building an application from a single file that is stored locally (e.g. under
`files`) requires a recipe that has the file listed in the **SRC_URI**
variable. Additionally, you need to manually write the ref-tasks-compile
and ref-tasks-install tasks. The **S** variable defines the
directory containing the source code, which is set to **UNPACKDIR** in this
case --- the directory BitBake uses for the build::

   SUMMARY = "Simple helloworld application"
   SECTION = "examples"
   LICENSE = "MIT"
   LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

   SRC_URI = "file://helloworld.c"

   S = "${UNPACKDIR}"

   do_compile() {
       ${CC} ${LDFLAGS} helloworld.c -o helloworld
   }

   do_install() {
       install -d ${D}${bindir}
       install -m 0755 helloworld ${D}${bindir}
   }

By default, the `helloworld`, `helloworld-dbg`, and `helloworld-dev` packages
are built. For information on how to customize the packaging process, see the
"dev-manual/new-recipe:splitting an application into multiple packages"
section.

### Building a Makefile-Based Package

Applications built with GNU `make` require a recipe that has the source archive
listed in **SRC_URI**. You do not need to add a ref-tasks-compile
step since by default BitBake starts the `make` command to compile the
application. If you need additional `make` options, you should store them in
the **EXTRA_OEMAKE** or **PACKAGECONFIG_CONFARGS** variables. BitBake
passes these options into the GNU `make` invocation. Note that a
ref-tasks-install task is still required. Otherwise, BitBake runs an
empty ref-tasks-install task by default.

Some applications might require extra parameters to be passed to the
compiler. For example, the application might need an additional header
path. You can accomplish this by adding to the **CFLAGS** variable. The
following example shows this::

   CFLAGS:prepend = "-I ${S}/include "

In the following example, `lz4` is a makefile-based package::

   SUMMARY = "Extremely Fast Compression algorithm"
   DESCRIPTION = "LZ4 is a very fast lossless compression algorithm, providing compression speed at 400 MB/s per core, scalable with multi-cores CPU. It also features an extremely fast decoder, with speed in multiple GB/s per core, typically reaching RAM speed limits on multi-core systems."
   HOMEPAGE = "https://github.com/lz4/lz4"

   LICENSE = "BSD-2-Clause | GPL-2.0-only"
   LIC_FILES_CHKSUM = "file://lib/LICENSE;md5=ebc2ea4814a64de7708f1571904b32cc \
                       file://programs/COPYING;md5=b234ee4d69f5fce4486a80fdaf4a4263 \
                       file://LICENSE;md5=d57c0d21cb917fb4e0af2454aa48b956 \
                       "

   PE = "1"

   SRCREV = "d44371841a2f1728a3f36839fd4b7e872d0927d3"

   SRC_URI = "git://github.com/lz4/lz4.git;branch=release;protocol=https \
              file://CVE-2021-3520.patch \
              "
   UPSTREAM_CHECK_GITTAGREGEX = "v(?P<pver>.*)"

   CVE_STATUS[CVE-2014-4715] = "fixed-version: Fixed in r118, which is larger than the current version"

   EXTRA_OEMAKE = "PREFIX=${prefix} CC='${CC}' CFLAGS='${CFLAGS}' DESTDIR=${D} LIBDIR=${libdir} INCLUDEDIR=${includedir} BUILD_STATIC=no"

   do_install() {
           oe_runmake install
   }

   BBCLASSEXTEND = "native nativesdk"

### Building an Autotooled Package

Applications built with the Autotools such as `autoconf` and `automake`
require a recipe that has a source archive listed in **SRC_URI** and also
inherit the ref-classes-autotools class, which contains the definitions
of all the steps needed to build an Autotool-based application. The result of
the build is automatically packaged. And, if the application uses NLS for
localization, packages with local information are generated (one package per
language). Here is one example: (`hello_2.3.bb`)::

   SUMMARY = "GNU Helloworld application"
   SECTION = "examples"
   LICENSE = "GPL-2.0-or-later"
   LIC_FILES_CHKSUM = "file://COPYING;md5=751419260aa954499f7abaabaa882bbe"

   SRC_URI = "${GNU_MIRROR}/hello/hello-${PV}.tar.gz"

   inherit autotools gettext

The variable **LIC_FILES_CHKSUM** is used to track source license changes
as described in the "dev-manual/licenses:tracking license changes"
section in the Yocto Project Overview and Concepts Manual. You can quickly
create Autotool-based recipes in a manner similar to the previous example.

### Building a Meson Package

Applications built with the `Meson build system <https://mesonbuild.com/>`__
just need a recipe that has sources described in **SRC_URI** and inherits
the ref-classes-meson class.

The ipcalc recipe 
is a simple example of an application without dependencies::

   SUMMARY = "Tool to assist in network address calculations for IPv4 and IPv6."
   HOMEPAGE = "https://gitlab.com/ipcalc/ipcalc"

   SECTION = "net"

   LICENSE = "GPL-2.0-only"
   LIC_FILES_CHKSUM = "file://COPYING;md5=b234ee4d69f5fce4486a80fdaf4a4263"

   SRC_URI = "git://gitlab.com/ipcalc/ipcalc.git;protocol=https;branch=master"
   SRCREV = "4c4261a47f355946ee74013d4f5d0494487cc2d6"

   inherit meson

Applications with dependencies are likely to inherit the
ref-classes-pkgconfig class, as `pkg-config` is the default method
used by Meson to find dependencies and compile applications against them.

### Splitting an Application into Multiple Packages

You can use the variables **PACKAGES** and **FILES** to split an
application into multiple packages.

Here is an example that uses the `libxpm` recipe. By default,
this recipe generates a single package that contains the library along
with a few binaries. You can modify the recipe to split the binaries
into separate packages::

   require xorg-lib-common.inc

   SUMMARY = "Xpm: X Pixmap extension library"
   LICENSE = "MIT"
   LIC_FILES_CHKSUM = "file://COPYING;md5=51f4270b012ecd4ab1a164f5f4ed6cf7"
   DEPENDS += "libxext libsm libxt"
   PE = "1"

   XORG_PN = "libXpm"

   PACKAGES =+ "sxpm cxpm"
   FILES:cxpm = "${bindir}/cxpm"
   FILES:sxpm = "${bindir}/sxpm"

In the previous example, we want to ship the `sxpm` and `cxpm`
binaries in separate packages. Since `bindir` would be packaged into
the main **PN** package by default, we prepend the **PACKAGES** variable
so additional package names are added to the start of list. This results
in the extra `FILES:*` variables then containing information that
define which files and directories go into which packages. Files
included by earlier packages are skipped by latter packages. Thus, the
main **PN** package does not include the above listed files.

### Packaging Externally Produced Binaries

Sometimes, you need to add pre-compiled binaries to an image. For
example, suppose that there are binaries for proprietary code,
created by a particular division of a company. Your part of the company
needs to use those binaries as part of an image that you are building
using the OpenEmbedded build system. Since you only have the binaries
and not the source code, you cannot use a typical recipe that expects to
fetch the source specified in
**SRC_URI** and then compile it.

One method is to package the binaries and then install them as part of
the image. Generally, it is not a good idea to package binaries since,
among other things, it can hinder the ability to reproduce builds and
could lead to compatibility problems with ABI in the future. However,
sometimes you have no choice.

The easiest solution is to create a recipe that uses the
ref-classes-bin-package class and to be sure that you are using default
locations for build artifacts.  In most cases, the
ref-classes-bin-package class handles "skipping" the configure and
compile steps as well as sets things up to grab packages from the appropriate
area. In particular, this class sets `noexec` on both the
ref-tasks-configure and ref-tasks-compile tasks, sets
`FILES:${PN}` to "/" so that it picks up all files, and sets up a
ref-tasks-install task, which effectively copies all files from `${S}`
to `${D}`. The ref-classes-bin-package class works well when the files
extracted into `${S}` are already laid out in the way they should be laid out
on the target. For more information on these variables, see the **FILES**,
**PN**, **S**, and **D** variables in the Yocto Project Reference
Manual's variable glossary.

> **Note:**
> -  Using **DEPENDS** is a good
> idea even for components distributed in binary form, and is often
> necessary for shared libraries. For a shared library, listing the
> library dependencies in **DEPENDS** makes sure that the libraries
> are available in the staging sysroot when other recipes link
> against the library, which might be necessary for successful
> linking.
>
> -  Using **DEPENDS** also allows runtime dependencies between
> packages to be added automatically. See the
> "overview-manual/concepts:automatically added runtime dependencies"
> section in the Yocto Project Overview and Concepts Manual for more
> information.
>

If you cannot use the ref-classes-bin-package class, you need to be sure you are
doing the following:

-  Create a recipe where the
   ref-tasks-configure and
   ref-tasks-compile tasks do
   nothing: It is usually sufficient to just not define these tasks in
   the recipe, because the default implementations do nothing unless a
   Makefile is found in
   `${`\ **S**\ `}`.

   If `${S}` might contain a Makefile, or if you inherit some class
   that replaces ref-tasks-configure and ref-tasks-compile with custom
   versions, then you can use the
   `[`\ noexec \ `]`
   flag to turn the tasks into no-ops, as follows::

      do_configure[noexec] = "1"
      do_compile[noexec] = "1"

   Unlike bitbake-user-manual/bitbake-user-manual-metadata:deleting a task,
   using the flag preserves the dependency chain from the ref-tasks-fetch,
   ref-tasks-unpack, and ref-tasks-patch tasks to the
   ref-tasks-install task.

-  Make sure your ref-tasks-install task installs the binaries
   appropriately.

-  Ensure that you set up **FILES**
   (usually
   `FILES:${`\ **PN**\ `}`) to
   point to the files you have installed, which of course depends on
   where you have installed them and whether those files are in
   different locations than the defaults.

As a basic example of a ref-classes-bin-package-style recipe, consider
this snippet from the
wireless-regdb 
recipe file, which fetches a single tarball of binary content and manually
installs with no need for any configuration or compilation::

   SRC_URI = "https://www.kernel.org/pub/software/network/${BPN}/${BP}.tar.xz"
   SRC_URI[sha256sum] = "57f8e7721cf5a880c13ae0c202edbb21092a060d45f9e9c59bcd2a8272bfa456"

   inherit bin_package allarch

   do_install() {
       install -d -m0755 ${D}${nonarch_libdir}/crda
       install -d -m0755 ${D}${sysconfdir}/wireless-regdb/pubkeys
       install -m 0644 regulatory.bin ${D}${nonarch_libdir}/crda/regulatory.bin
       install -m 0644 wens.key.pub.pem ${D}${sysconfdir}/wireless-regdb/pubkeys/wens.key.pub.pem
       install -m 0644 -D regulatory.db ${D}${nonarch_base_libdir}/firmware/regulatory.db
       install -m 0644 regulatory.db.p7s ${D}${nonarch_base_libdir}/firmware/regulatory.db.p7s
   }

## Following Recipe Style Guidelines

When writing recipes, it is good to conform to existing style guidelines.
See the "/contributor-guide/recipe-style-guide" in the Yocto Project
and OpenEmbedded Contributor Guide for reference.

It is common for existing recipes to deviate a bit from this style.
However, aiming for at least a consistent style is a good idea. Some
practices, such as omitting spaces around `=` operators in assignments
or ordering recipe components in an erratic way, are widely seen as poor
style.

## Recipe Syntax

Understanding recipe file syntax is important for writing recipes. The
following list overviews the basic items that make up a BitBake recipe
file. For more complete BitBake syntax descriptions, see the
"bitbake:bitbake-user-manual/bitbake-user-manual-metadata"
chapter of the BitBake User Manual.

-  *Variable Assignments and Manipulations:* Variable assignments allow
   a value to be assigned to a variable. The assignment can be static
   text or might include the contents of other variables. In addition to
   the assignment, appending and prepending operations are also
   supported.

   The following example shows some of the ways you can use variables in
   recipes::

      S = "${UNPACKDIR}/postfix-${PV}"
      CFLAGS += "-DNO_ASM"
      CFLAGS:append = " --enable-important-feature"

-  *Functions:* Functions provide a series of actions to be performed.
   You usually use functions to override the default implementation of a
   task function or to complement a default function (i.e. append or
   prepend to an existing function). Standard functions use `sh` shell
   syntax, although access to OpenEmbedded variables and internal
   methods are also available.

   Here is an example function from the `sed` recipe::

      do_install () {
          autotools_do_install
          install -d ${D}${base_bindir}
          if [ ! ${D}${bindir} -ef ${D}${base_bindir} ]; then
              mv ${D}${bindir}/sed ${D}${base_bindir}/sed
              rmdir ${D}${bindir}/
          fi
      }

   It is
   also possible to implement new functions that are called between
   existing tasks as long as the new functions are not replacing or
   complementing the default functions. You can implement functions in
   Python instead of shell. Both of these options are not seen in the
   majority of recipes.

-  *Keywords:* BitBake recipes use only a few keywords. You use keywords
   to include common functions (`inherit`), load parts of a recipe
   from other files (`include` and `require`) and export variables
   to the environment (`export`).

   The following example shows the use of some of these keywords::

      export POSTCONF = "${STAGING_BINDIR}/postconf"
      inherit autoconf
      require otherfile.inc

-  *Comments (#):* Any lines that begin with the hash character (`#`)
   are treated as comment lines and are ignored::

      # This is a comment

This next list summarizes the most important and most commonly used
parts of the recipe syntax. For more information on these parts of the
syntax, you can reference the
"bitbake:bitbake-user-manual/bitbake-user-manual-metadata" chapter
in the BitBake User Manual.

-  *Line Continuation (\\):* Use the backward slash (`\`) character to
   split a statement over multiple lines. Place the slash character at
   the end of the line that is to be continued on the next line::

       VAR = "A really long \
              line"

   .. note::

      You cannot have any characters including spaces or tabs after the
      slash character.

-  *Using Variables (${VARNAME}):* Use the `${VARNAME}` syntax to
   access the contents of a variable::

      SRC_URI = "${SOURCEFORGE_MIRROR}/libpng/zlib-${PV}.tar.gz"

   .. note::

      It is important to understand that the value of a variable
      expressed in this form does not get substituted automatically. The
      expansion of these expressions happens on-demand later (e.g.
      usually when a function that makes reference to the variable
      executes). This behavior ensures that the values are most
      appropriate for the context in which they are finally used. On the
      rare occasion that you do need the variable expression to be
      expanded immediately, you can use the
      :=
      operator instead of
      =
      when you make the assignment, but this is not generally needed.

-  *Quote All Assignments ("value"):* Use double quotes around values in
   all variable assignments (e.g. `"value"`). Here is an example::

      VAR1 = "${OTHERVAR}"
      VAR2 = "The version is ${PV}"

-  *Conditional Assignment (?=):* Conditional assignment is used to
   assign a value to a variable, but only when the variable is currently
   unset. Use the question mark followed by the equal sign (`?=`) to
   make a "soft" assignment used for conditional assignment. Typically,
   "soft" assignments are used in the `local.conf` file for variables
   that are allowed to come through from the external environment.

   Here is an example where `VAR1` is set to "New value" if it is
   currently empty. However, if `VAR1` has already been set, it
   remains unchanged::

      VAR1 ?= "New value"

   In this next example, `VAR1` is left with the value "Original value"::

      VAR1 = "Original value"
      VAR1 ?= "New value"

-  *Appending (+=):* Use the plus character followed by the equals sign
   (`+=`) to append values to existing variables.

   .. note::

      This operator adds a space between the existing content of the
      variable and the new content.

   Here is an example::

      SRC_URI += "file://fix-makefile.patch"

-  *Prepending (=+):* Use the equals sign followed by the plus character
   (`=+`) to prepend values to existing variables.

   .. note::

      This operator adds a space between the new content and the
      existing content of the variable.

   Here is an example::

      VAR =+ "Starts"

-  *Appending (:append):* Use the `:append` operator to append values
   to existing variables. This operator does not add any additional
   space. Also, the operator is applied after all the `+=`, and `=+`
   operators have been applied and after all `=` assignments have
   occurred. This means that if `:append` is used in a recipe, it can
   only be overridden by another layer using the special `:remove`
   operator, which in turn will prevent further layers from adding it back.

   The following example shows the space being explicitly added to the
   start to ensure the appended value is not merged with the existing
   value::

      CFLAGS:append = " --enable-important-feature"

   You can also use
   the `:append` operator with overrides, which results in the actions
   only being performed for the specified target or machine::

      CFLAGS:append:sh4 = " --enable-important-sh4-specific-feature"

-  *Prepending (:prepend):* Use the `:prepend` operator to prepend
   values to existing variables. This operator does not add any
   additional space. Also, the operator is applied after all the `+=`,
   and `=+` operators have been applied and after all `=`
   assignments have occurred.

   The following example shows the space being explicitly added to the
   end to ensure the prepended value is not merged with the existing
   value::

      CFLAGS:prepend = "-I${S}/myincludes "

   You can also use the
   `:prepend` operator with overrides, which results in the actions
   only being performed for the specified target or machine::

      CFLAGS:prepend:sh4 = "-I${S}/myincludes "

-  *Overrides:* You can use overrides to set a value conditionally,
   typically based on how the recipe is being built. For example, to set
   the **KBRANCH** variable's
   value to "standard/base" for any target
   **MACHINE**, except for
   qemuarm where it should be set to "standard/arm-versatile-926ejs",
   you would do the following::

      KBRANCH = "standard/base"
      KBRANCH:qemuarm = "standard/arm-versatile-926ejs"

   Overrides are also used to separate
   alternate values of a variable in other situations. For example, when
   setting variables such as
   **FILES** and
   **RDEPENDS** that are
   specific to individual packages produced by a recipe, you should
   always use an override that specifies the name of the package.

-  *Indentation:* Use spaces for indentation rather than tabs. For
   shell functions, both currently work. However, it is a policy
   decision of the Yocto Project to use tabs in shell functions. Realize
   that some layers have a policy to use spaces for all indentation.

-  *Using Python for Complex Operations:* For more advanced processing,
   it is possible to use Python code during variable assignments (e.g.
   search and replacement on a variable).

   You indicate Python code using the `${@python_code}` syntax for the
   variable assignment::

      SRC_URI = "ftp://ftp.info-zip.org/pub/infozip/src/zip${@d.getVar('PV',1).replace('.', '')}.tgz

-  *Shell Function Syntax:* Write shell functions as if you were writing
   a shell script when you describe a list of actions to take. You
   should ensure that your script works with a generic `sh` and that
   it does not require any `bash` or other shell-specific
   functionality. The same considerations apply to various system
   utilities (e.g. `sed`, `grep`, `awk`, and so forth) that you
   might wish to use. If in doubt, you should check with multiple
   implementations --- including those from BusyBox.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Adding a New Machine

Adding a new machine to the Yocto Project is a straightforward process.
This section describes how to add machines that are similar to those
that the Yocto Project already supports.

> **Note:**
> Although well within the capabilities of the Yocto Project, adding a
> totally new architecture might require changes to `gcc`/`glibc`
> and to the site information, which is beyond the scope of this
> manual.
>

For a complete example that shows how to add a new machine, see the
"bsp-guide/bsp:creating a new bsp layer using the \\`bitbake-layers\`\` script`"
section in the Yocto Project Board Support Package (BSP) Developer's
Guide.

## Adding the Machine Configuration File

To add a new machine, you need to add a new machine configuration file
to the layer's `conf/machine` directory. This configuration file
provides details about the device you are adding.

The OpenEmbedded build system uses the root name of the machine
configuration file to reference the new machine. For example, given a
machine configuration file named `crownbay.conf`, the build system
recognizes the machine as "crownbay".

The most important variables you must set in your machine configuration
file or include from a lower-level configuration file are as follows:

-  **TARGET_ARCH** (e.g. "arm")

-  `PREFERRED_PROVIDER_virtual/kernel`

-  **MACHINE_FEATURES** (e.g. "screen wifi")

You might also need these variables:

-  **SERIAL_CONSOLES** (e.g. "115200;ttyS0 115200;ttyS1")

-  **KERNEL_IMAGETYPE** (e.g. "zImage")

-  **IMAGE_FSTYPES** (e.g. "tar.gz jffs2")

You can find full details on these variables in the reference section.
You can leverage existing machine `.conf` files from
`meta-yocto-bsp/conf/machine/`.

## Adding a Kernel for the Machine

The OpenEmbedded build system needs to be able to build a kernel for the
machine. You need to either create a new kernel recipe for this machine,
or extend an existing kernel recipe. You can find several kernel recipe
examples in the Source Directory at `meta/recipes-kernel/linux` that
you can use as references.

If you are creating a new kernel recipe, normal recipe-writing rules
apply for setting up a **SRC_URI**. Thus, you need to specify any
necessary patches and set **S** to point at the source code. You need to
create a ref-tasks-configure task that configures the unpacked kernel with
a `defconfig` file. You can do this by using a `make defconfig`
command or, more commonly, by copying in a suitable `defconfig` file
and then running `make oldconfig`. By making use of `inherit kernel`
and potentially some of the `linux-*.inc` files, most other
functionality is centralized and the defaults of the class normally work
well.

If you are extending an existing kernel recipe, it is usually a matter
of adding a suitable `defconfig` file. The file needs to be added into
a location similar to `defconfig` files used for other machines in a
given kernel recipe. A possible way to do this is by listing the file in
the **SRC_URI** and adding the machine to the expression in
**COMPATIBLE_MACHINE**::

   COMPATIBLE_MACHINE = '(qemux86|qemumips)'

For more information on `defconfig` files, see the
"kernel-dev/common:changing the configuration"
section in the Yocto Project Linux Kernel Development Manual.

## Adding a Formfactor Configuration File

A formfactor configuration file provides information about the target
hardware for which the image is being built and information that the
build system cannot obtain from other sources such as the kernel. Some
examples of information contained in a formfactor configuration file
include framebuffer orientation, whether or not the system has a
keyboard, the positioning of the keyboard in relation to the screen, and
the screen resolution.

The build system uses reasonable defaults in most cases. However, if
customization is necessary, you need to create a `machconfig` file in
the `meta/recipes-bsp/formfactor/files` directory. This directory
contains directories for specific machines such as `qemuarm` and
`qemux86`. For information about the settings available and the
defaults, see the `meta/recipes-bsp/formfactor/files/config` file
found in the same area.

Here is an example for "qemuarm" machine::

   HAVE_TOUCHSCREEN=1
   HAVE_KEYBOARD=1
   DISPLAY_CAN_ROTATE=0
   DISPLAY_ORIENTATION=0
   #DISPLAY_WIDTH_PIXELS=640
   #DISPLAY_HEIGHT_PIXELS=480
   #DISPLAY_BPP=16
   DISPLAY_DPI=150
   DISPLAY_SUBPIXEL_ORDER=vrgb

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Upgrading Recipes

Over time, upstream developers publish new versions for software built
by layer recipes. It is recommended to keep recipes up-to-date with
upstream version releases.

While there are several methods to upgrade a recipe, you might
consider checking on the upgrade status of a recipe first. You can do so
using the `devtool check-upgrade-status` command. See the
"devtool-checking-on-the-upgrade-status-of-a-recipe"
section in the Yocto Project Reference Manual for more information.

The remainder of this section describes three ways you can upgrade a
recipe. You can use the Automated Upgrade Helper (AUH) to set up
automatic version upgrades. Alternatively, you can use
`devtool upgrade` to set up semi-automatic version upgrades. Finally,
you can manually upgrade a recipe by editing the recipe itself.

## Using the Auto Upgrade Helper (AUH)

The AUH utility works in conjunction with the OpenEmbedded build system
in order to automatically generate upgrades for recipes based on new
versions being published upstream. Use AUH when you want to create a
service that performs the upgrades automatically and optionally sends
you an email with the results.

AUH allows you to update several recipes with a single use. You can also
optionally perform build and integration tests using images with the
results saved to your hard drive and emails of results optionally sent
to recipe maintainers. Finally, AUH creates Git commits with appropriate
commit messages in the layer's tree for the changes made to recipes.

> **Note:**
> In some conditions, you should not use AUH to upgrade recipes
> and should instead use either `devtool upgrade` or upgrade your
> recipes manually:
>
> -  When AUH cannot complete the upgrade sequence. This situation
> usually results because custom patches carried by the recipe
> cannot be automatically rebased to the new version. In this case,
> `devtool upgrade` allows you to manually resolve conflicts.
>
> -  When for any reason you want fuller control over the upgrade
> process. For example, when you want special arrangements for
> testing.
>

The following steps describe how to set up the AUH utility:

#. *Be Sure the Development Host is Set Up:* You need to be sure that
   your development host is set up to use the Yocto Project. For
   information on how to set up your host, see the
   "dev-manual/start:Preparing the Build Host" section.

#. *Make Sure Git is Configured:* The AUH utility requires Git to be
   configured because AUH uses Git to save upgrades. Thus, you must have
   Git user and email configured. The following command shows your
   configurations::

      $ git config --list

   If you do not have the user and
   email configured, you can use the following commands to do so::

      $ git config --global user.name some_name
      $ git config --global user.email username@domain.com

#. *Clone the AUH Repository:* To use AUH, you must clone the repository
   onto your development host. The following command uses Git to create
   a local copy of the repository on your system::

      $ git clone git://git.yoctoproject.org/auto-upgrade-helper
      Cloning into 'auto-upgrade-helper'... remote: Counting objects: 768, done.
      remote: Compressing objects: 100% (300/300), done.
      remote: Total 768 (delta 499), reused 703 (delta 434)
      Receiving objects: 100% (768/768), 191.47 KiB | 98.00 KiB/s, done.
      Resolving deltas: 100% (499/499), done.
      Checking connectivity... done.

   AUH is not part of the **OpenEmbedded-Core (OE-Core)** or
   **Poky** repositories.

#. *Create a Dedicated Build Directory:* Create a new **bitbake:Setup** with
   `bitbake-setup` or manually run the structure-core-script
   script to create a fresh **Build Directory** that you use exclusively
   for running the AUH utility::

      $ source layers/openembedded-core/oe-init-build-env your_AUH_build_directory

   Re-using an existing **Build Directory** and its configurations is not
   recommended as existing settings could cause AUH to fail or behave
   undesirably.

#. *Make Configurations in Your Local Configuration File:* Several
   settings are needed in the `local.conf` file in the build
   directory you just created for AUH. Make these following
   configurations:

   -  If you want to enable :ref:`Build
      History <dev-manual/build-quality:maintaining build output quality>`,
      which is optional, you need the following lines in the
      `conf/local.conf` file::

         INHERIT =+ "buildhistory"
         BUILDHISTORY_COMMIT = "1"

      With this configuration and a successful
      upgrade, a build history "diff" file appears in the
      `upgrade-helper/work/recipe/buildhistory-diff.txt` file found in
      your **Build Directory**.

   -  If you want to enable testing through the ref-classes-testimage
      class, which is optional, you need to have the following set in
      your `conf/local.conf` file::

         IMAGE_CLASSES += "testimage"

      .. note::

         If your distro does not enable by default ptest, which **Poky**
         does, you need the following in your `local.conf` file::

                 DISTRO_FEATURES:append = " ptest"

#. *Optionally Start a vncserver:* If you are running in a server
   without an X11 session, you need to start a vncserver::

      $ vncserver :1
      $ export DISPLAY=:1

#. *Create and Edit an AUH Configuration File:* You need to have the
   `upgrade-helper/upgrade-helper.conf` configuration file in your
   **Build Directory**. You can find a sample configuration file in the
   AUH source repository .

   Read through the sample file and make configurations as needed. For
   example, if you enabled build history in your `local.conf` as
   described earlier, you must enable it in `upgrade-helper.conf`.

   Also, if you are using the default `maintainers.inc` file supplied
   in **OpenEmbedded-Core (OE-Core)** and you do not set a
   "maintainers_whitelist" or "global_maintainer_override" in the
   `upgrade-helper.conf` configuration, and you specify "-e all" on
   the AUH command-line, the utility automatically sends out emails to
   all the default maintainers. Please avoid this.

This next set of examples describes how to use the AUH:

-  *Upgrading a Specific Recipe:* To upgrade a specific recipe, use the
   following form::

      $ upgrade-helper.py recipe_name

   For example, this command upgrades the `xmodmap` recipe::

      $ upgrade-helper.py xmodmap

-  *Upgrading a Specific Recipe to a Particular Version:* To upgrade a
   specific recipe to a particular version, use the following form::

      $ upgrade-helper.py recipe_name -t version

   For example, this command upgrades the `xmodmap` recipe to version 1.2.3::

      $ upgrade-helper.py xmodmap -t 1.2.3

-  *Upgrading all Recipes to the Latest Versions and Suppressing Email
   Notifications:* To upgrade all recipes to their most recent versions
   and suppress the email notifications, use the following command::

      $ upgrade-helper.py all

-  *Upgrading all Recipes to the Latest Versions and Send Email
   Notifications:* To upgrade all recipes to their most recent versions
   and send email messages to maintainers for each attempted recipe as
   well as a status email, use the following command::

      $ upgrade-helper.py -e all

Once you have run the AUH utility, you can find the results in the AUH
**Build Directory**::

   ${BUILDDIR}/upgrade-helper/timestamp

The AUH utility
also creates recipe update commits from successful upgrade attempts in
the layer tree.

You can easily set up to run the AUH utility on a regular basis by using
a cron job. See the
weeklyjob.sh 
file distributed with the utility for an example.

## Using ``devtool upgrade``

As mentioned earlier, an alternative method for upgrading recipes to
newer versions is to use
devtool upgrade .
You can read about `devtool upgrade` in general in the
"dev-manual/devtool:use \\`devtool upgrade\`\` to create a version of the recipe that supports a newer version of the software`"
section in the Yocto Project Application Development and the Extensible
Software Development Kit (eSDK) Manual.

To see all the command-line options available with `devtool upgrade`,
use the following help command::

   $ devtool upgrade -h

If you want to find out what version a recipe is currently at upstream
without any attempt to upgrade your local version of the recipe, you can
use the following command::

   $ devtool latest-version recipe_name

As mentioned in the previous section describing AUH, `devtool upgrade`
works in a less-automated manner than AUH. Specifically,
`devtool upgrade` only works on a single recipe that you name on the
command line, cannot perform build and integration testing using images,
and does not automatically generate commits for changes in the source
tree. Despite all these "limitations", `devtool upgrade` updates the
recipe file to the new upstream version and attempts to rebase custom
patches contained by the recipe as needed.

> **Note:**
> AUH uses much of `devtool upgrade` behind the scenes making AUH somewhat
> of a "wrapper" application for `devtool upgrade`.
>

A typical scenario involves having used Git to clone an upstream
repository that you use during build operations. Because you have built the
recipe in the past, the layer is likely added to your
configuration already. If for some reason, the layer is not added, you
could add it easily using the
"bitbake-layers "
script. For example, suppose you use the `nano.bb` recipe from the
`meta-oe` layer in the `meta-openembedded` repository. For this
example, assume that the layer has been cloned into following area::

   /home/scottrif/bitbake-builds/layers/meta-openembedded

The following command from your **Build Directory** adds the layer to
your build configuration (i.e. `${BUILDDIR}/conf/bblayers.conf`)::

   $ bitbake-layers add-layer /home/scottrif/bitbake-builds/layers/meta-openembedded/meta-oe
   NOTE: Starting bitbake server...
   Parsing recipes: 100% |##########################################| Time: 0:00:55
   Parsing of 1431 .bb files complete (0 cached, 1431 parsed). 2040 targets, 56 skipped, 0 masked, 0 errors.
   Removing 12 recipes from the x86_64 sysroot: 100% |##############| Time: 0:00:00
   Removing 1 recipes from the x86_64_i586 sysroot: 100% |##########| Time: 0:00:00
   Removing 5 recipes from the i586 sysroot: 100% |#################| Time: 0:00:00
   Removing 5 recipes from the qemux86 sysroot: 100% |##############| Time: 0:00:00

For this example, assume that the `nano.bb` recipe that
is upstream has a 2.9.3 version number. However, the version in the
local repository is 2.7.4. The following command from your build
directory automatically upgrades the recipe for you::

   $ devtool upgrade nano -V 2.9.3
   NOTE: Starting bitbake server...
   NOTE: Creating workspace layer in /home/scottrif/bitbake-builds/build/workspace
   Parsing recipes: 100% |##########################################| Time: 0:00:46
   Parsing of 1431 .bb files complete (0 cached, 1431 parsed). 2040 targets, 56 skipped, 0 masked, 0 errors.
   NOTE: Extracting current version source...
   NOTE: Resolving any missing task queue dependencies
          .
          .
          .
   NOTE: Executing SetScene Tasks
   NOTE: Executing RunQueue Tasks
   NOTE: Tasks Summary: Attempted 74 tasks of which 72 didn't need to be rerun and all succeeded.
   Adding changed files: 100% |#####################################| Time: 0:00:00
   NOTE: Upgraded source extracted to /home/scottrif/bitbake-builds/build/workspace/sources/nano
   NOTE: New recipe is /home/scottrif/bitbake-builds/build/workspace/recipes/nano/nano_2.9.3.bb

> **Note:**
> Using the `-V` option is not necessary. Omitting the version number causes
> `devtool upgrade` to upgrade the recipe to the most recent version.
>

Continuing with this example, you can use `devtool build` to build the
newly upgraded recipe::

   $ devtool build nano
   NOTE: Starting bitbake server...
   Loading cache: 100% |################################################################################################| Time: 0:00:01
   Loaded 2040 entries from dependency cache.
   Parsing recipes: 100% |##############################################################################################| Time: 0:00:00
   Parsing of 1432 .bb files complete (1431 cached, 1 parsed). 2041 targets, 56 skipped, 0 masked, 0 errors.
   NOTE: Resolving any missing task queue dependencies
          .
          .
          .
   NOTE: Executing SetScene Tasks
   NOTE: Executing RunQueue Tasks
   NOTE: nano: compiling from external source tree /home/scottrif/bitbake-builds/build/workspace/sources/nano
   NOTE: Tasks Summary: Attempted 520 tasks of which 304 didn't need to be rerun and all succeeded.

Within the `devtool upgrade` workflow, you can
deploy and test your rebuilt software. For this example,
however, running `devtool finish` cleans up the workspace once the
source in your workspace is clean. This usually means using Git to stage
and submit commits for the changes generated by the upgrade process.

Once the tree is clean, you can clean things up in this example with the
following command from the `${BUILDDIR}/workspace/sources/nano`
directory::

   $ devtool finish nano meta-oe
   NOTE: Starting bitbake server...
   Loading cache: 100% |################################################################################################| Time: 0:00:00
   Loaded 2040 entries from dependency cache.
   Parsing recipes: 100% |##############################################################################################| Time: 0:00:01
   Parsing of 1432 .bb files complete (1431 cached, 1 parsed). 2041 targets, 56 skipped, 0 masked, 0 errors.
   NOTE: Adding new patch 0001-nano.bb-Stuff-I-changed-when-upgrading-nano.bb.patch
   NOTE: Updating recipe nano_2.9.3.bb
   NOTE: Removing file /home/scottrif/bitbake-builds/layers/meta-openembedded/meta-oe/recipes-support/nano/nano_2.7.4.bb
   NOTE: Moving recipe file to /home/scottrif/bitbake-builds/layers/meta-openembedded/meta-oe/recipes-support/nano
   NOTE: Leaving source tree /home/scottrif/bitbake-builds/build/workspace/sources/nano as-is; if you no longer need it then please delete it manually

Using the `devtool finish` command cleans up the workspace and creates a patch
file based on your commits. The tool puts all patch files back into the
source directory in a sub-directory named `nano` in this case.

## Manually Upgrading a Recipe

If for some reason you choose not to upgrade recipes using
dev-manual/upgrading-recipes:Using the Auto Upgrade Helper (AUH) or
by dev-manual/upgrading-recipes:Using `devtool upgrade```,
you can manually edit the recipe files to upgrade the versions.

> **Note:**
> Manually updating multiple recipes scales poorly and involves many
> steps. The recommendation to upgrade recipe versions is through AUH
> or `devtool upgrade`, both of which automate some steps and provide
> guidance for others needed for the manual process.
>

To manually upgrade recipe versions, follow these general steps:

#. *Change the Version:* Rename the recipe such that the version (i.e.
   the **PV** part of the recipe name)
   changes appropriately. If the version is not part of the recipe name,
   change the value as it is set for **PV** within the recipe itself.

#. *Update* **SRCREV** *if Needed*: If the source code your recipe builds
   is fetched from Git or some other version control system, update
   **SRCREV** to point to the
   commit hash that matches the new version.

#. *Build the Software:* Try to build the recipe using BitBake. Typical
   build failures include the following:

   -  License statements were updated for the new version. For this
      case, you need to review any changes to the license and update the
      values of **LICENSE** and
      **LIC_FILES_CHKSUM**
      as needed.

      .. note::

         License changes are often inconsequential. For example, the
         license text's copyright year might have changed.

   -  Custom patches carried by the older version of the recipe might
      fail to apply to the new version. For these cases, you need to
      review the failures. Patches might not be necessary for the new
      version of the software if the upgraded version has fixed those
      issues. If a patch is necessary and failing, you need to rebase it
      into the new version.

#. *Optionally Attempt to Build for Several Architectures:* Once you
   successfully build the new software for a given architecture, you
   could test the build for other architectures by changing the
   **MACHINE** variable and
   rebuilding the software. This optional step is especially important
   if the recipe is to be released publicly.

#. *Check the Upstream Change Log or Release Notes:* Checking both these
   reveals if there are new features that could break
   backwards-compatibility. If so, you need to take steps to mitigate or
   eliminate that situation.

#. *Optionally Create a Bootable Image and Test:* If you want, you can
   test the new software by booting it onto actual hardware.

#. *Create a Commit with the Change in the Layer Repository:* After all
   builds work and any testing is successful, you can create commits for
   any changes in the layer holding your upgraded recipe.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Finding Temporary Source Code

You might find it helpful during development to modify the temporary
source code used by recipes to build packages. For example, suppose you
are developing a patch and you need to experiment a bit to figure out
your solution. After you have initially built the package, you can
iteratively tweak the source code, which is located in the
**Build Directory**, and then you can force a re-compile and quickly
test your altered code. Once you settle on a solution, you can then preserve
your changes in the form of patches.

During a build, the unpacked temporary source code used by recipes to
build packages is available in the **Build Directory** as defined by the
**S** variable. Below is the default value for the **S** variable as
defined in the `meta/conf/bitbake.conf` configuration file in
**OpenEmbedded-Core (OE-Core)**::

   S = "${UNPACKDIR}/${BP}"

You should be aware that many recipes override the
**S** variable when the default isn't accurate.

> **Note:**
> The **BP** represents the base recipe name, which consists of the name
> and version::
>
> BP = "${BPN}-${PV}"
>
> This matches the location that the git fetcher unpacks to, and usually
> matches unpacked content of release tarballs (e.g. they contain a single
> directory which matches value of ${BP} exactly).
>

The path to the unpack directory for the recipe
(**UNPACKDIR**) is defined as follows::

   ${WORKDIR}/sources

In turn, the path to the work directory for the recipe
(**WORKDIR**) is defined as
follows::

   ${BASE_WORKDIR}/${MULTIMACH_TARGET_SYS}/${PN}/${PV}

The actual directory depends on several things:

-  **BASE_WORKDIR**: The top-level build
   output directory.

-  **MULTIMACH_TARGET_SYS**:
   The target system identifier.

-  **PN**: The recipe name.

-  **PV**: The recipe version.

As an example, assume a Source Directory top-level folder named
`project`, a default **Build Directory** at `project/build`, and a
`qemux86-poky-linux` machine target system. Furthermore, suppose your
recipe is named `foo_1.3.0.bb`. In this case, the work directory the
build system uses to build the package would be as follows::

   project/build/tmp/work/qemux86-poky-linux/foo/1.3.0

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Using a Development Shell

When debugging certain commands or even when just editing packages,
`devshell` can be a useful tool. When you invoke `devshell`, all
tasks up to and including
ref-tasks-patch are run for the
specified target. Then, a new terminal is opened and you are placed in
`${`\ **S**\ `}`, the source
directory. In the new terminal, all the OpenEmbedded build-related
environment variables are still defined so you can use commands such as
`configure` and `make`. The commands execute just as if the
OpenEmbedded build system were executing them. Consequently, working
this way can be helpful when debugging a build or preparing software to
be used with the OpenEmbedded build system.

Here is an example that uses `devshell` on a target named
`matchbox-desktop`::

  $ bitbake matchbox-desktop -c devshell

This command spawns a terminal with a shell prompt within the
OpenEmbedded build environment. The
**OE_TERMINAL** variable
controls what type of shell is opened.

For spawned terminals, the following occurs:

-  The `PATH` variable includes the cross-toolchain.

-  The `pkgconfig` variables find the correct `.pc` files.

-  The `configure` command finds the Yocto Project site files as well
   as any other necessary files.

Within this environment, you can run configure or compile commands as if
they were being run by the OpenEmbedded build system itself. As noted
earlier, the working directory also automatically changes to the Source
Directory (**S**).

To manually run a specific task using `devshell`, run the
corresponding `run.*` script in the
`${`\ **WORKDIR**\ `}/temp`
directory (e.g., `run.do_configure.`\ `pid`). If a task's script does
not exist, which would be the case if the task was skipped by way of the
sstate cache, you can create the task by first running it outside of the
`devshell`::

   $ bitbake -c task

> **Note:**
> -  Execution of a task's `run.*` script and BitBake's execution of
> a task are identical. In other words, running the script re-runs
> the task just as it would be run using the `bitbake -c` command.
>
> -  Any `run.*` file that does not have a `.pid` extension is a
> symbolic link (symlink) to the most recent version of that file.
>

Remember, that the `devshell` is a mechanism that allows you to get
into the BitBake task execution environment. And as such, all commands
must be called just as BitBake would call them. That means you need to
provide the appropriate options for cross-compilation and so forth as
applicable.

When you are finished using `devshell`, exit the shell or close the
terminal window.

> **Note:**
> -  It is worth remembering that when using `devshell` you need to
> use the full compiler name such as `arm-poky-linux-gnueabi-gcc`
> instead of just using `gcc`. The same applies to other
> applications such as `binutils`, `libtool` and so forth.
> BitBake sets up environment variables such as **CC** to assist
> applications, such as `make` to find the correct tools.
>
> -  It is also worth noting that `devshell` still works over X11
> forwarding and similar situations.
>
>
>

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Using a Python Development Shell

Similar to working within a development shell as described in the
previous section, you can also spawn and work within an interactive
Python development shell. When debugging certain commands or even when
just editing packages, `pydevshell` can be a useful tool. When you
invoke the `pydevshell` task, all tasks up to and including
ref-tasks-patch are run for the
specified target. Then a new terminal is opened. Additionally, key
Python objects and code are available in the same way they are to
BitBake tasks, in particular, the data store 'd'. So, commands such as
the following are useful when exploring the data store and running
functions::

   pydevshell> d.getVar("STAGING_DIR")
   '/media/build1/bitbake-builds/build/tmp/sysroots'
   pydevshell> d.getVar("STAGING_DIR", False)
   '${TMPDIR}/sysroots'
   pydevshell> d.setVar("FOO", "bar")
   pydevshell> d.getVar("FOO")
   'bar'
   pydevshell> d.delVar("FOO")
   pydevshell> d.getVar("FOO")
   pydevshell> bb.build.exec_func("do_unpack", d)
   pydevshell>

See the "bitbake-user-manual/bitbake-user-manual-metadata:functions you can call from within python"
section in the BitBake User Manual for details about available functions.

The commands execute just as if the OpenEmbedded build
system were executing them. Consequently, working this way can be
helpful when debugging a build or preparing software to be used with the
OpenEmbedded build system.

Here is an example that uses `pydevshell` on a target named
`matchbox-desktop`::

   $ bitbake matchbox-desktop -c pydevshell

This command spawns a terminal and places you in an interactive Python
interpreter within the OpenEmbedded build environment. The
**OE_TERMINAL** variable
controls what type of shell is opened.

When you are finished using `pydevshell`, you can exit the shell
either by using Ctrl+d or closing the terminal window.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Building

This section describes various build procedures, such as the steps
needed for a simple build, building a target for multiple configurations,
generating an image for more than one machine, and so forth.

## Building a Simple Image

In the development environment, you need to build an image whenever you
change hardware support, add or change system libraries, or add or
change services that have dependencies. There are several methods that allow
you to build an image within the Yocto Project. This section presents
the basic steps you need to build a simple image using BitBake from a
build host running Linux.

> **Note:**
> -  For information on how to build an image using
> **Toaster**, see the
> /toaster-manual/index.
>
> -  For information on how to use `devtool` to build images, see the
> "sdk-manual/extensible:using \\`devtool\`\` in your sdk workflow`"
> section in the Yocto Project Application Development and the
> Extensible Software Development Kit (eSDK) manual.
>
> -  For a quick example on how to build an image using the
> OpenEmbedded build system, see the
> /brief-yoctoprojectqs/index document.
>
> -  You can also use the `Yocto Project BitBake
> <https://marketplace.visualstudio.com/items?itemName=yocto-project.yocto-bitbake>`__
> extension for Visual Studio Code to build images.
>

The build process creates an entire Linux distribution from source and
places it in your **Build Directory** under `tmp/deploy/images`. For
detailed information on the build process using BitBake, see the
"overview-manual/concepts:images" section in the Yocto Project Overview
and Concepts Manual.

The following figure and list overviews the build process:

![bitbake-build-flow.png](figures/bitbake-build-flow.png)

#. *Set up Your Host Development System to Support Development Using the
   Yocto Project*: See the "/dev-manual/start" section for options on how to get a
   build host ready to use the Yocto Project.

#. *Make Sure Your Configuration is Correct*: Use bitbake-config-build  to
   define the **MACHINE** or **DISTRO**, and open your
   structure-build-conf-site.conf file to set site-specific settings.

#. *Build the Image:* Build the image using the `bitbake` command::

      $ bitbake target

   .. note::

      For information on BitBake, see the bitbake:index.

   The target is the name of the recipe you want to build. Common
   targets are the images in `meta/recipes-core/images`,
   `meta/recipes-sato/images`, and so forth all found in
   **OpenEmbedded-Core (OE-Core)**. Alternatively, the target
   can be the name of a recipe for a specific piece of software such as
   BusyBox. For more details about the images the OpenEmbedded build
   system supports, see the
   "ref-manual/images:Images" chapter in the Yocto
   Project Reference Manual.

   As an example, the following command builds the
   `core-image-minimal` image::

      $ bitbake core-image-minimal

   Once an
   image has been built, it often needs to be installed. The images and
   kernels built by the OpenEmbedded build system are placed in the
   **Build Directory** in `tmp/deploy/images`. For information on how to
   run pre-built images such as `qemux86` and `qemuarm`, see the
   /sdk-manual/index manual. For
   information about how to install these images, see the documentation
   for your particular board or machine.

## Building Images for Multiple Targets Using Multiple Configurations

See the /dev-manual/multiconfig section of the Yocto Project Development Tasks
Manual.

## Building an Initial RAM Filesystem (Initramfs) Image

An initial RAM filesystem (**Initramfs**) image provides a temporary root
filesystem used for early system initialization, typically providing tools and
loading modules needed to locate and mount the final root filesystem.

Follow these steps to create an **Initramfs** image:

#. *Create the Initramfs Image Recipe:* You can reference the
   `core-image-minimal-initramfs.bb` recipe found in the
   `meta/recipes-core` directory in **OpenEmbedded-Core (OE-Core)**
   as an example from which to work. The `core-image-minimal-initramfs` recipe
   is based on the :ref:`initramfs-framework <dev-manual/building:Customizing an
   Initramfs using \`\`initramfs-framework\`\`>` recipe described below.

#. *Decide if You Need to Bundle the Initramfs Image Into the Kernel
   Image:* If you want the **Initramfs** image that is built to be bundled
   in with the kernel image, set the **INITRAMFS_IMAGE_BUNDLE**
   variable to `"1"` in your `local.conf` configuration file and set the
   **INITRAMFS_IMAGE** variable in the recipe that builds the kernel image.

   Setting the **INITRAMFS_IMAGE_BUNDLE** flag causes the **Initramfs**
   image to be unpacked into the `${B}/usr/` directory. The unpacked
   **Initramfs** image is then passed to the kernel's `Makefile` using the
   **CONFIG_INITRAMFS_SOURCE** variable, allowing the **Initramfs**
   image to be built into the kernel normally.

#. *Optionally Add Items to the Initramfs Image Through the Initramfs
   Image Recipe:* If you add items to the **Initramfs** image by way of its
   recipe, you should use **PACKAGE_INSTALL** rather than
   **IMAGE_INSTALL**. **PACKAGE_INSTALL** gives more direct control of
   what is added to the image as compared to the defaults you might not
   necessarily want that are set by the ref-classes-image
   or ref-classes-core-image classes.

#. *Build the Kernel Image and the Initramfs Image:* Build your kernel
   image using BitBake. Because the **Initramfs** image recipe is a
   dependency of the kernel image, the **Initramfs** image is built as well
   and bundled with the kernel image if you used the
   **INITRAMFS_IMAGE_BUNDLE** variable described earlier.

### Customizing an Initramfs using ``initramfs-framework``

The `core-image-minimal-initramfs.bb` recipe found in
:oe_git:`meta/recipes-core/images
</openembedded-core/tree/meta/recipes-core/images>` uses the
:oe_git:`initramfs-framework_1.0.bb
</openembedded-core/tree/meta/recipes-core/initrdscripts/initramfs-framework_1.0.bb>`
recipe as its base component. The goal of the `initramfs-framework` recipe is
to provide the building blocks to build a customized **Initramfs**.

The `initramfs-framework` recipe relies on shell initialization scripts
defined in :oe_git:`meta/recipes-core/initrdscripts/initramfs-framework
</openembedded-core/tree/meta/recipes-core/initrdscripts/initramfs-framework>`. Since some of
these scripts do not apply for all use cases, the `initramfs-framework` recipe
defines different packages:

-  `initramfs-framework-base`: this package installs the basic components of
   an **Initramfs**, such as the `init` script or the `/dev/console`
   character special file. As this package is a runtime dependency of all
   modules listed below, it is automatically pulled in when one of the modules
   is installed in the image.
-  `initramfs-module-exec`: support for execution of applications.
-  `initramfs-module-mdev`: support for `mdev
   <https://wiki.gentoo.org/wiki/Mdev>`__.
-  `initramfs-module-udev`: support for Udev .
-  `initramfs-module-e2fs`: support for :wikipedia:`ext4/ext3/ext2
   <Extended_file_system>` filesystems.
-  `initramfs-module-nfsrootfs`: support for locating and mounting the root
   partition via NFS .
-  `initramfs-module-rootfs`: support for locating and mounting the root
   partition.
-  `initramfs-module-debug`: dynamic debug support.
-  `initramfs-module-lvm`: LVM  rootfs support.
-  `initramfs-module-overlayroot`: support for mounting a read-write overlay
   on top of a read-only root filesystem.

In addition to the packages defined by the `initramfs-framework` recipe
itself, the following packages are defined by the recipes present in
meta/recipes-core/initrdscripts :

-  `initramfs-module-install`: module to create and install a partition layout
   on a selected block device.
-  `initramfs-module-install-efi`: module to create and install an EFI
   partition layout on a selected block device.
-  `initramfs-module-setup-live`: module to start a shell in the
   **Initramfs** if `root=/dev/ram0` in passed in the `Kernel command-line
   <https://www.kernel.org/doc/html/latest/admin-guide/kernel-parameters.html>`__
   or the `root=` parameter was not passed.

To customize the **Initramfs**, you can add or remove packages listed
earlier from the **PACKAGE_INSTALL** variable with a :ref:`bbappend
<dev-manual/layers:Appending Other Layers Metadata With Your Layer>` on the
`core-image-minimal-initramfs` recipe, or create a custom recipe for the
**Initramfs** taking `core-image-minimal-initramfs` as example.

Custom scripts can be added to the **Initramfs** by writing your own
recipes. The recipes are conventionally named `initramfs-module-<module name>`
where `<module name>` is the name of the module. The recipe should set its
**RDEPENDS** package-specific variables to include
`initramfs-framework-base` and the other packages on which the module depends
at runtime.

The recipe must install shell initialization scripts in **${D} <D>**\
`/init.d` and must follow the `<number>-<script name>` naming scheme where:

-  `<number>` is a *two-digit* number that affects the execution order of the
   script compared to others. For example, the script `80-setup-live` would be
   executed after `01-udev` because 80 is greater than 01.

   This number being two-digits is important here as the scripts are executed
   alphabetically. For example, the script `10-script` would be executed
   before the script `8-script`, because `1` is inferior to `8`.
   Therefore, the script should be named `08-script`.

-  `<script name>` is the script name which you can choose freely.

   If two script use the same `<number>`, they are sorted alphabetically based
   on `<script name>`.

### Bundling an Initramfs Image From a Separate Multiconfig

There may be a case where we want to build an **Initramfs** image which does not
inherit the same distro policy as our main image, for example, we may want
our main image to use `TCLIBC = "glibc"`, but to use `TCLIBC = "musl"` in our **Initramfs**
image to keep a smaller footprint. However, by performing the steps mentioned
above the **Initramfs** image will inherit `TCLIBC = "glibc"` without allowing us
to override it.

To achieve this, you need to perform some additional steps:

#. *Create a multiconfig for your Initramfs image:* You can perform the steps
   on "dev-manual/building:building images for multiple targets using multiple configurations" to create a separate multiconfig.
   For the sake of simplicity let's assume such multiconfig is called: `initramfscfg.conf` and
   contains the variables::

      TMPDIR = "${TOPDIR}/tmp-initramfscfg"
      TCLIBC = "musl"

#. *Set additional Initramfs variables on your main configuration:*
   Additionally, on your main configuration (`local.conf`) you need to set the
   variables::

     INITRAMFS_MULTICONFIG = "initramfscfg"
     INITRAMFS_DEPLOY_DIR_IMAGE = "${TOPDIR}/tmp-initramfscfg/deploy/images/${MACHINE}"

   The variables **INITRAMFS_MULTICONFIG** and **INITRAMFS_DEPLOY_DIR_IMAGE**
   are used to create a multiconfig dependency from the kernel to the **INITRAMFS_IMAGE**
   to be built coming from the `initramfscfg` multiconfig, and to let the
   buildsystem know where the **INITRAMFS_IMAGE** will be located.

   Building a system with such configuration will build the kernel using the
   main configuration but the ref-tasks-bundle_initramfs task will grab the
   selected **INITRAMFS_IMAGE** from **INITRAMFS_DEPLOY_DIR_IMAGE**
   instead, resulting in a musl based **Initramfs** image bundled in the kernel
   but a glibc based main image.

   The same is applicable to avoid inheriting **DISTRO_FEATURES** on **INITRAMFS_IMAGE**
   or to build a different **DISTRO** for it such as `poky-tiny`.

## Building a Tiny System

Very small distributions have some significant advantages such as
requiring less on-die or in-package memory (cheaper), better performance
through efficient cache usage, lower power requirements due to less
memory, faster boot times, and reduced development overhead. Some
real-world examples where a very small distribution gives you distinct
advantages are digital cameras, medical devices, and small headless
systems.

This section presents information that shows you how you can trim your
distribution to even smaller sizes than the `poky-tiny` distribution,
which is around 5 Mbytes, that can be built out-of-the-box using the
Yocto Project.

### Tiny System Overview

The following list presents the overall steps you need to consider and
perform to create distributions with smaller root filesystems, achieve
faster boot times, maintain your critical functionality, and avoid
initial RAM disks:

-  :ref:`Determine your goals and guiding principles
   <dev-manual/building:goals and guiding principles>`

-  dev-manual/building:understand what contributes to your image size

-  :ref:`Reduce the size of the root filesystem
   <dev-manual/building:trim the root filesystem>`

-  Reduce the size of the kernel 

-  dev-manual/building:remove package management requirements

-  dev-manual/building:look for other ways to minimize size

-  dev-manual/building:iterate on the process

### Goals and Guiding Principles

Before you can reach your destination, you need to know where you are
going. Here is an example list that you can use as a guide when creating
very small distributions:

-  Determine how much space you need (e.g. a kernel that is 1 Mbyte or
   less and a root filesystem that is 3 Mbytes or less).

-  Find the areas that are currently taking 90% of the space and
   concentrate on reducing those areas.

-  Do not create any difficult "hacks" to achieve your goals.

-  Leverage the device-specific options.

-  Work in a separate layer so that you keep changes isolated. For
   information on how to create layers, see the
   "dev-manual/layers:understanding and creating layers" section.

### Understand What Contributes to Your Image Size

It is easiest to have something to start with when creating your own
distribution. You can use the Yocto Project out-of-the-box to create the
`poky-tiny` distribution. Ultimately, you will want to make changes in
your own distribution that are likely modeled after `poky-tiny`.

> **Note:**
> To use `poky-tiny` in your build, set the **DISTRO** variable in your
> `local.conf` file to "poky-tiny" as described in the
> "dev-manual/custom-distribution:creating your own distribution"
> section.
>

Understanding some memory concepts will help you reduce the system size.
Memory consists of static, dynamic, and temporary memory. Static memory
is the TEXT (code), DATA (initialized data in the code), and BSS
(uninitialized data) sections. Dynamic memory represents memory that is
allocated at runtime: stacks, hash tables, and so forth. Temporary
memory is recovered after the boot process. This memory consists of
memory used for decompressing the kernel and for the `__init__`
functions.

To help you see where you currently are with kernel and root filesystem
sizes, you can use two tools found in **OpenEmbedded-Core (OE-Core)**
in the `scripts/tiny/` directory:

-  `ksize.py`: Reports component sizes for the kernel build objects.

-  `dirsize.py`: Reports component sizes for the root filesystem.

This next tool and command help you organize configuration fragments and
view file dependencies in a human-readable form:

-  `merge_config.sh`: Helps you manage configuration files and
   fragments within the kernel. With this tool, you can merge individual
   configuration fragments together. The tool allows you to make
   overrides and warns you of any missing configuration options. The
   tool is ideal for allowing you to iterate on configurations, create
   minimal configurations, and create configuration files for different
   machines without having to duplicate your process.

   The `merge_config.sh` script is part of the Linux Yocto kernel Git
   repositories (i.e. `linux-yocto-3.14`, `linux-yocto-3.10`,
   `linux-yocto-3.8`, and so forth) in the `scripts/kconfig`
   directory.

   For more information on configuration fragments, see the
   "kernel-dev/common:creating configuration fragments"
   section in the Yocto Project Linux Kernel Development Manual.

-  `bitbake -u taskexp -g bitbake_target`: Using the BitBake command
   with these options brings up a Dependency Explorer from which you can
   view file dependencies. Understanding these dependencies allows you
   to make informed decisions when cutting out various pieces of the
   kernel and root filesystem.

### Trim the Root Filesystem

The root filesystem is made up of packages for booting, libraries, and
applications. To change things, you can configure how the packaging
happens, which changes the way you build them. You can also modify the
filesystem itself or select a different filesystem.

First, find out what is hogging your root filesystem by running the
`dirsize.py` script from your root directory::

   $ cd root-directory-of-image
   $ dirsize.py 100000 > dirsize-100k.log
   $ cat dirsize-100k.log

You can apply a filter to the script to ignore files
under a certain size. The previous example filters out any files below
100 Kbytes. The sizes reported by the tool are uncompressed, and thus
will be smaller by a relatively constant factor in a compressed root
filesystem. When you examine your log file, you can focus on areas of
the root filesystem that take up large amounts of memory.

You need to be sure that what you eliminate does not cripple the
functionality you need. One way to see how packages relate to each other
is by using the Dependency Explorer UI with the BitBake command::

   $ cd image-directory
   $ bitbake -u taskexp -g image

Use the interface to
select potential packages you wish to eliminate and see their dependency
relationships.

When deciding how to reduce the size, get rid of packages that result in
minimal impact on the feature set. For example, you might not need a VGA
display. Or, you might be able to get by with `devtmpfs` and `mdev`
instead of `udev`.

Use your `local.conf` file to make changes. For example, to eliminate
`udev` and `glib`, set the following in the local configuration
file::

   VIRTUAL-RUNTIME_dev_manager = ""

Finally, you should consider exactly the type of root filesystem you
need to meet your needs while also reducing its size. For example,
consider `cramfs`, `squashfs`, `ubifs`, `ext2`, or an
**Initramfs** using `initramfs`. Be aware that `ext3` requires a 1
Mbyte journal. If you are okay with running read-only, you do not need
this journal.

> **Note:**
> After each round of elimination, you need to rebuild your system and
> then use the tools to see the effects of your reductions.
>

### Trim the Kernel

The kernel is built by including policies for hardware-independent
aspects. What subsystems do you enable? For what architecture are you
building? Which drivers do you build by default?

> **Note:**
> You can modify the kernel source if you want to help with boot time.
>

Run the `ksize.py` script from the top-level Linux build directory to
get an idea of what is making up the kernel::

   $ cd top-level-linux-build-directory
   $ ksize.py > ksize.log
   $ cat ksize.log

When you examine the log, you will see how much space is taken up with
the built-in `.o` files for drivers, networking, core kernel files,
filesystem, sound, and so forth. The sizes reported by the tool are
uncompressed, and thus will be smaller by a relatively constant factor
in a compressed kernel image. Look to reduce the areas that are large
and taking up around the "90% rule."

To examine, or drill down, into any particular area, use the `-d`
option with the script::

   $ ksize.py -d > ksize.log

Using this option
breaks out the individual file information for each area of the kernel
(e.g. drivers, networking, and so forth).

Use your log file to see what you can eliminate from the kernel based on
features you can let go. For example, if you are not going to need
sound, you do not need any drivers that support sound.

After figuring out what to eliminate, you need to reconfigure the kernel
to reflect those changes during the next build. You could run
`menuconfig` and make all your changes at once. However, that makes it
difficult to see the effects of your individual eliminations and also
makes it difficult to replicate the changes for perhaps another target
device. A better method is to start with no configurations using
`allnoconfig`, create configuration fragments for individual changes,
and then manage the fragments into a single configuration file using
`merge_config.sh`. The tool makes it easy for you to iterate using the
configuration change and build cycle.

Each time you make configuration changes, you need to rebuild the kernel
and check to see what impact your changes had on the overall size.

### Remove Package Management Requirements

Packaging requirements add size to the image. One way to reduce the size
of the image is to remove all the packaging requirements from the image.
This reduction includes both removing the package manager and its unique
dependencies as well as removing the package management data itself.

To eliminate all the packaging requirements for an image, be sure that
"package-management" is not part of your
**IMAGE_FEATURES**
statement for the image. When you remove this feature, you are removing
the package manager as well as its dependencies from the root
filesystem.

### Look for Other Ways to Minimize Size

Depending on your particular circumstances, other areas that you can
trim likely exist. The key to finding these areas is through tools and
methods described here combined with experimentation and iteration. Here
are a couple of areas to experiment with:

-  `glibc`: In general, follow this process:

   #. Remove `glibc` features from
      **DISTRO_FEATURES**
      that you think you do not need.

   #. Build your distribution.

   #. If the build fails due to missing symbols in a package, determine
      if you can reconfigure the package to not need those features. For
      example, change the configuration to not support wide character
      support as is done for `ncurses`. Or, if support for those
      characters is needed, determine what `glibc` features provide
      the support and restore the configuration.

   4. Rebuild and repeat the process.

-  `busybox`: For BusyBox, use a process similar as described for
   `glibc`. A difference is you will need to boot the resulting system
   to see if you are able to do everything you expect from the running
   system. You need to be sure to integrate configuration fragments into
   Busybox because BusyBox handles its own core features and then allows
   you to add configuration fragments on top.

### Iterate on the Process

If you have not reached your goals on system size, you need to iterate
on the process. The process is the same. Use the tools and see just what
is taking up 90% of the root filesystem and the kernel. Decide what you
can eliminate without limiting your device beyond what you need.

Depending on your system, a good place to look might be Busybox, which
provides a stripped down version of Unix tools in a single, executable
file. You might be able to drop virtual terminal services or perhaps
ipv6.

## Building Images for More than One Machine

A common scenario developers face is creating images for several
different machines that use the same software environment. In this
situation, it is tempting to set the tunings and optimization flags for
each build specifically for the targeted hardware (i.e. "maxing out" the
tunings). Doing so can considerably add to build times and package feed
maintenance collectively for the machines. For example, selecting tunes
that are extremely specific to a CPU core used in a system might enable
some micro optimizations in GCC for that particular system but would
otherwise not gain you much of a performance difference across the other
systems as compared to using a more general tuning across all the builds
(e.g. setting **DEFAULTTUNE**
specifically for each machine's build). Rather than "max out" each
build's tunings, you can take steps that cause the OpenEmbedded build
system to reuse software across the various machines where it makes
sense.

If build speed and package feed maintenance are considerations, you
should consider the points in this section that can help you optimize
your tunings to best consider build times and package feed maintenance.

-  *Share the* **Build Directory** *:* If at all possible, share the
   **TMPDIR** across builds. The Yocto Project supports switching between
   different **MACHINE** values in the same **TMPDIR**. This practice
   is well supported and regularly used by developers when building for
   multiple machines. When you use the same **TMPDIR** for multiple
   machine builds, the OpenEmbedded build system can reuse the existing native
   and often cross-recipes for multiple machines. Thus, build time decreases.

   .. note::

      If **DISTRO** settings change or fundamental configuration settings
      such as the filesystem layout, you need to work with a clean **TMPDIR**.
      Sharing **TMPDIR** under these circumstances might work but since it is
      not guaranteed, you should use a clean **TMPDIR**.

-  *Enable the Appropriate Package Architecture:* By default, the
   OpenEmbedded build system enables three levels of package
   architectures: "all", "tune" or "package", and "machine". Any given
   recipe usually selects one of these package architectures (types) for
   its output. Depending for what a given recipe creates packages,
   making sure you enable the appropriate package architecture can
   directly impact the build time.

   A recipe that just generates scripts can enable "all" architecture
   because there are no binaries to build. To specifically enable "all"
   architecture, be sure your recipe inherits the
   ref-classes-allarch class.
   This class is useful for "all" architectures because it configures
   many variables so packages can be used across multiple architectures.

   If your recipe needs to generate packages that are machine-specific
   or when one of the build or runtime dependencies is already
   machine-architecture dependent, which makes your recipe also
   machine-architecture dependent, make sure your recipe enables the
   "machine" package architecture through the
   **MACHINE_ARCH**
   variable::

      PACKAGE_ARCH = "${MACHINE_ARCH}"

   When you do not
   specifically enable a package architecture through the
   **PACKAGE_ARCH**, The
   OpenEmbedded build system defaults to the
   **TUNE_PKGARCH** setting::

      PACKAGE_ARCH = "${TUNE_PKGARCH}"

-  *Choose a Generic Tuning File if Possible:* Some tunes are more
   generic and can run on multiple targets (e.g. an `armv5` set of
   packages could run on `armv6` and `armv7` processors in most
   cases). Similarly, `i486` binaries could work on `i586` and
   higher processors. You should realize, however, that advances on
   newer processor versions would not be used.

   If you select the same tune for several different machines, the
   OpenEmbedded build system reuses software previously built, thus
   speeding up the overall build time. Realize that even though a new
   sysroot for each machine is generated, the software is not recompiled
   and only one package feed exists.

-  *Manage Granular Level Packaging:* Sometimes there are cases where
   injecting another level of package architecture beyond the three
   higher levels noted earlier can be useful. For example, consider how
   NXP (formerly Freescale) allows for the easy reuse of binary packages
   in their layer
   meta-freescale .
   In this example, the
   fsl-dynamic-packagearch 
   class shares GPU packages for i.MX53 boards because all boards share
   the AMD GPU. The i.MX6-based boards can do the same because all
   boards share the Vivante GPU. This class inspects the BitBake
   datastore to identify if the package provides or depends on one of
   the sub-architecture values. If so, the class sets the
   **PACKAGE_ARCH** value
   based on the `MACHINE_SUBARCH` value. If the package does not
   provide or depend on one of the sub-architecture values but it
   matches a value in the machine-specific filter, it sets
   **MACHINE_ARCH**. This
   behavior reduces the number of packages built and saves build time by
   reusing binaries.

-  *Use Tools to Debug Issues:* Sometimes you can run into situations
   where software is being rebuilt when you think it should not be. For
   example, the OpenEmbedded build system might not be using shared
   state between machines when you think it should be. These types of
   situations are usually due to references to machine-specific
   variables such as **MACHINE**,
   **SERIAL_CONSOLES**,
   **XSERVER**,
   **MACHINE_FEATURES**,
   and so forth in code that is supposed to only be tune-specific or
   when the recipe depends
   (**DEPENDS**,
   **RDEPENDS**,
   **RRECOMMENDS**,
   **RSUGGESTS**, and so forth)
   on some other recipe that already has
   **PACKAGE_ARCH** defined
   as "${MACHINE_ARCH}".

   .. note::

      Patches to fix any issues identified are most welcome as these
      issues occasionally do occur.

   For such cases, you can use some tools to help you sort out the
   situation:

   -  `state-diff-machines.sh`*:* You can find this tool in the
      `scripts` directory of the Source Repositories. See the comments
      in the script for information on how to use the tool.

   -  *BitBake's "-S printdiff" Option:* Using this option causes
      BitBake to try to establish the most recent signature match
      (e.g. in the shared state cache) and then compare matched signatures
      to determine the stamps and delta where these two stamp trees diverge.

## Building Software from an External Source

By default, the OpenEmbedded build system uses the **Build Directory**
when building source code. The build process involves fetching the source
files, unpacking them, and then patching them if necessary before the build
takes place.

There are situations where you might want to build software from source
files that are external to and thus outside of the OpenEmbedded build
system. For example, suppose you have a project that includes a new BSP
with a heavily customized kernel. And, you want to minimize exposing the
build system to the development team so that they can focus on their
project and maintain everyone's workflow as much as possible. In this
case, you want a kernel source directory on the development machine
where the development occurs. You want the recipe's
**SRC_URI** variable to point to
the external directory and use it as is, not copy it.

To build from software that comes from an external source, all you need to do
is inherit the ref-classes-externalsrc class and then set
the **EXTERNALSRC** variable to point to your external source code. Here
are the statements to put in your `local.conf` file::

   INHERIT += "externalsrc"
   EXTERNALSRC:pn-myrecipe = "path-to-your-source-tree"

This next example shows how to accomplish the same thing by setting
**EXTERNALSRC** in the recipe itself or in the recipe's append file::

   EXTERNALSRC = "path"
   EXTERNALSRC_BUILD = "path"

> **Note:**
> In order for these settings to take effect, you must globally or
> locally inherit the ref-classes-externalsrc class.
>

By default, ref-classes-externalsrc builds the source code in a
directory separate from the external source directory as specified by
**EXTERNALSRC**. If you need
to have the source built in the same directory in which it resides, or
some other nominated directory, you can set
**EXTERNALSRC_BUILD**
to point to that directory::

   EXTERNALSRC_BUILD:pn-myrecipe = "path-to-your-source-tree"

> **Note:**
> The values of **EXTERNALSRC** and **EXTERNALSRC_BUILD**
> must be absolute paths.
>

## Replicating a Build Offline

It can be useful to take a "snapshot" of upstream sources used in a
build and then use that "snapshot" later to replicate the build offline.
To do so, you need to first prepare and populate your downloads
directory your "snapshot" of files. Once your downloads directory is
ready, you can use it at any time and from any machine to replicate your
build.

Follow these steps to populate your Downloads directory:

#. *Create a Clean Downloads Directory:* Start with an empty downloads
   directory (**DL_DIR**). You
   start with an empty downloads directory by either removing the files
   in the existing directory or by setting **DL_DIR** to point to either
   an empty location or one that does not yet exist.

#. *Generate Tarballs of the Source Git Repositories:* Edit your
   `local.conf` configuration file as follows::

      DL_DIR = "/home/your-download-dir/"
      BB_GENERATE_MIRROR_TARBALLS = "1"

   During
   the fetch process in the next step, BitBake gathers the source files
   and creates tarballs in the directory pointed to by **DL_DIR**. See
   the
   **BB_GENERATE_MIRROR_TARBALLS**
   variable for more information.

#. *Populate Your Downloads Directory Without Building:* Use BitBake to
   fetch your sources but inhibit the build::

      $ bitbake target --runonly=fetch

   The downloads directory (i.e. `${DL_DIR}`) now has
   a "snapshot" of the source files in the form of tarballs, which can
   be used for the build.

#. *Optionally Remove Any Git or other SCM Subdirectories From the
   Downloads Directory:* If you want, you can clean up your downloads
   directory by removing any Git or other Source Control Management
   (SCM) subdirectories such as `${DL_DIR}/git2/*`. The tarballs
   already contain these subdirectories.

Once your downloads directory has everything it needs regarding source
files, you can create your "own-mirror" and build your target.
Understand that you can use the files to build the target offline from
any machine and at any time.

Follow these steps to build your target using the files in the downloads
directory:

#. *Using Local Files Only:* Inside your `local.conf` file, add the
   **SOURCE_MIRROR_URL** variable, inherit the
   ref-classes-own-mirrors class, and add the
   **BB_NO_NETWORK** variable to your `local.conf`::

      SOURCE_MIRROR_URL ?= "file:///home/your-download-dir/"
      INHERIT += "own-mirrors"
      BB_NO_NETWORK = "1"

   The **SOURCE_MIRROR_URL** and ref-classes-own-mirrors
   class set up the system to use the downloads directory as your "own
   mirror". Using the **BB_NO_NETWORK** variable makes sure that
   BitBake's fetching process in step 3 stays local, which means files
   from your "own-mirror" are used.

#. *Start With a Clean Build:* You can start with a clean build by
   removing the `${`\ **TMPDIR**\ `}` directory or using a new
   **Build Directory**.

#. *Build Your Target:* Use BitBake to build your target::

      $ bitbake target

   The build completes using the known local "snapshot" of source
   files from your mirror. The resulting tarballs for your "snapshot" of
   source files are in the downloads directory.

   .. note::

      The offline build does not work if recipes attempt to find the
      latest version of software by setting
      **SRCREV** to
      `${`\ **AUTOREV**\ `}`::

         SRCREV = "${AUTOREV}"

      When a recipe sets **SRCREV** to
      `${`\ **AUTOREV**\ `}`, the build system accesses the network in an
      attempt to determine the latest version of software from the SCM.
      Typically, recipes that use **AUTOREV** are custom or modified
      recipes. Recipes that reside in public repositories usually do not
      use **AUTOREV**.

      If you do have recipes that use **AUTOREV**, you can take steps to
      still use the recipes in an offline build. Do the following:

      #. Use a configuration generated by enabling :ref:`build
         history <dev-manual/build-quality:maintaining build output quality>`.

      #. Use the `buildhistory-collect-srcrevs` command to collect the
         stored **SRCREV** values from the build's history. For more
         information on collecting these values, see the
         "dev-manual/build-quality:build history package information"
         section.

      #. Once you have the correct source revisions, you can modify
         those recipes to set **SRCREV** to specific versions of the
         software.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Creating Partitioned Images Using Wic

Creating an image for a particular hardware target using the
OpenEmbedded build system does not necessarily mean you can boot that
image as is on your device. Physical devices accept and boot images in
various ways depending on the specifics of the device. Usually,
information about the hardware can tell you what image format the device
requires. Should your device require multiple partitions on an SD card,
flash, or an HDD, you can use the OpenEmbedded Image Creator, Wic, to
create the properly partitioned image.

The `wic` command generates partitioned images from existing
OpenEmbedded build artifacts. Image generation is driven by partitioning
commands contained in an OpenEmbedded kickstart file (`.wks`)
specified either directly on the command line or as one of a selection
of canned kickstart files as shown with the `wic list images` command
in the
"dev-manual/wic:generate an image using an existing kickstart file"
section. When you apply the command to a given set of build artifacts, the
result is an image or set of images that can be directly written onto media and
used on a particular system.

> **Note:**
> For a kickstart file reference, see the
> "ref-manual/kickstart:openembedded kickstart (\\`.wks\`\`) reference`"
> Chapter in the Yocto Project Reference Manual.
>

The `wic` command and the infrastructure it is based on is by
definition incomplete. The purpose of the command is to allow the
generation of customized images, and as such, was designed to be
completely extensible through a plugin interface. See the
"dev-manual/wic:using the wic plugin interface" section
for information on these plugins.

This section provides some background information on Wic, describes what
you need to have in place to run the tool, provides instruction on how
to use the Wic utility, provides information on using the Wic plugins
interface, and provides several examples that show how to use Wic.

## Background

This section provides some background on the Wic utility. While none of
this information is required to use Wic, you might find it interesting.

-  The name "Wic" is derived from OpenEmbedded Image Creator (oeic). The
   "oe" diphthong in "oeic" was promoted to the letter "w", because
   "oeic" is both difficult to remember and to pronounce.

-  Wic is loosely based on the Meego Image Creator (`mic`) framework.
   The Wic implementation has been heavily modified to make direct use
   of OpenEmbedded build artifacts instead of package installation and
   configuration, which are already incorporated within the OpenEmbedded
   artifacts.

-  Wic is a completely independent standalone utility that initially
   provides easier-to-use and more flexible replacements for an existing
   functionality in OE-Core's ref-classes-image-live
   class. The difference between Wic and those examples is that with Wic
   the functionality of those scripts is implemented by a
   general-purpose partitioning language, which is based on Redhat
   kickstart syntax.

## Requirements

In order to use the Wic utility with the OpenEmbedded Build system, your
system needs to meet the following requirements:

-  The Linux distribution on your development host must support the
   Yocto Project. See the "system-requirements-supported-distros"
   section in the Yocto Project Reference Manual for the list of
   distributions that support the Yocto Project.

-  The standard system utilities, such as `cp`, must be installed on
   your development host system.

-  You must have sourced the build environment setup script (i.e.
   structure-core-script) found in the **Build Directory**.

-  You need to have the build artifacts already available, which
   typically means that you must have already created an image using the
   OpenEmbedded build system (e.g. `core-image-minimal`). While it
   might seem redundant to generate an image in order to create an image
   using Wic, the current version of Wic requires the artifacts in the
   form generated by the OpenEmbedded build system.

-  You must build several native tools, which are built to run on the
   build system::

      $ bitbake wic-tools

-  Include "wic" as part of the
   **IMAGE_FSTYPES**
   variable.

-  Include the name of the wic kickstart file 
   as part of the **WKS_FILE** variable. If multiple candidate files can
   be provided by different layers, specify all the possible names through the
   **WKS_FILES** variable instead.

## Getting Help

You can get general help for the `wic` command by entering the `wic`
command by itself or by entering the command with a help argument as
follows::

   $ wic -h
   $ wic --help
   $ wic help

Currently, Wic supports seven commands: `cp`, `create`, `help`,
`list`, `ls`, `rm`, and `write`. You can get help for all these
commands except "help" by using the following form::

   $ wic help command

For example, the following command returns help for the `write`
command::

   $ wic help write

Wic supports help for three topics: `overview`, `plugins`, and
`kickstart`. You can get help for any topic using the following form::

   $ wic help topic

For example, the following returns overview help for Wic::

   $ wic help overview

There is one additional level of help for Wic. You can get help on
individual images through the `list` command. You can use the `list`
command to return the available Wic images as follows::

   $ wic list images
     genericx86                                 Create an EFI disk image for genericx86*
     beaglebone-yocto                           Create SD card image for Beaglebone
     qemuriscv                                  Create qcow2 image for RISC-V QEMU machines
     mkefidisk                                  Create an EFI disk image
     qemuloongarch                              Create qcow2 image for LoongArch QEMU machines
     directdisk-multi-rootfs                    Create multi rootfs image using rootfs plugin
     directdisk                                 Create a 'pcbios' direct disk image
     efi-bootdisk
     mkhybridiso                                Create a hybrid ISO image
     directdisk-gpt                             Create a 'pcbios' direct disk image
     systemd-bootdisk                           Create an EFI disk image with systemd-boot
     sdimage-bootpart                           Create SD card image with a boot partition
     qemux86-directdisk                         Create a qemu machine 'pcbios' direct disk image
     directdisk-bootloader-config               Create a 'pcbios' direct disk image with custom bootloader config

Once you know the list of available
Wic images, you can use `help` with the command to get help on a
particular image. For example, the following command returns help on the
"beaglebone-yocto" image::

   $ wic list beaglebone-yocto help

   Creates a partitioned SD card image for Beaglebone.
   Boot files are located in the first vfat partition.

## Operational Modes

You can use Wic in two different modes, depending on how much control
you need for specifying the OpenEmbedded build artifacts that are used
for creating the image: Raw and Cooked:

-  *Raw Mode:* You explicitly specify build artifacts through Wic
   command-line arguments.

-  *Cooked Mode:* The current
   **MACHINE** setting and image
   name are used to automatically locate and provide the build
   artifacts. You just supply a kickstart file and the name of the image
   from which to use artifacts.

Regardless of the mode you use, you need to have the build artifacts
ready and available.

### Raw Mode

Running Wic in raw mode allows you to specify all the partitions through
the `wic` command line. The primary use for raw mode is if you have
built your kernel outside of the Yocto Project **Build Directory**.
In other words, you can point to arbitrary kernel, root filesystem locations,
and so forth. Contrast this behavior with cooked mode where Wic looks in the
**Build Directory** (e.g. `tmp/deploy/images/`\ machine).

The general form of the `wic` command in raw mode is::

   $ wic create wks_file options ...

     Where:

        wks_file:
           An OpenEmbedded kickstart file.  You can provide
           your own custom file or use a file from a set of
           existing files as described by further options.

        optional arguments:
          -h, --help            show this help message and exit
          -o OUTDIR, --outdir OUTDIR
                                name of directory to create image in
          -e IMAGE_NAME, --image-name IMAGE_NAME
                                name of the image to use the artifacts from e.g. core-
                                image-sato
          -r ROOTFS_DIR, --rootfs-dir ROOTFS_DIR
                                path to the /rootfs dir to use as the .wks rootfs
                                source
          -b BOOTIMG_DIR, --bootimg-dir BOOTIMG_DIR
                                path to the dir containing the boot artifacts (e.g.
                                /EFI or /syslinux dirs) to use as the .wks bootimg
                                source
          -k KERNEL_DIR, --kernel-dir KERNEL_DIR
                                path to the dir containing the kernel to use in the
                                .wks bootimg
          -n NATIVE_SYSROOT, --native-sysroot NATIVE_SYSROOT
                                path to the native sysroot containing the tools to use
                                to build the image
          -s, --skip-build-check
                                skip the build check
          -f, --build-rootfs    build rootfs
          -c {gzip,bzip2,xz}, --compress-with {gzip,bzip2,xz}
                                compress image with specified compressor
          -m, --bmap            generate .bmap
          --no-fstab-update     Do not change fstab file.
          -v VARS_DIR, --vars VARS_DIR
                                directory with <image>.env files that store bitbake
                                variables
          -D, --debug           output debug information

> **Note:**
> You do not need root privileges to run Wic. In fact, you should not
> run as root when using the utility.
>

### Cooked Mode

Running Wic in cooked mode leverages off artifacts in the
**Build Directory**. In other words, you do not have to specify kernel or
root filesystem locations as part of the command. All you need to provide is
a kickstart file and the name of the image from which to use artifacts
by using the "-e" option. Wic looks in the **Build Directory** (e.g.
`tmp/deploy/images/`\ machine) for artifacts.

The general form of the `wic` command using Cooked Mode is as follows::

   $ wic create wks_file -e IMAGE_NAME

     Where:

        wks_file:
           An OpenEmbedded kickstart file.  You can provide
           your own custom file or use a file from a set of
           existing files provided with the Yocto Project
           release.

        required argument:
           -e IMAGE_NAME, --image-name IMAGE_NAME
                                name of the image to use the artifacts from e.g. core-
                                image-sato

## Using an Existing Kickstart File

If you do not want to create your own kickstart file, you can use an
existing file provided by the Wic installation. As shipped, kickstart
files can be found in the overview-manual/development-environment:yocto project source repositories in the
following two locations::

   meta-yocto/meta-yocto-bsp/wic
   openembedded-core/scripts/lib/wic/canned-wks

Use the following command to list the available kickstart files::

   $ wic list images
     genericx86                                 Create an EFI disk image for genericx86*
     beaglebone-yocto                           Create SD card image for Beaglebone
     qemuriscv                                  Create qcow2 image for RISC-V QEMU machines
     mkefidisk                                  Create an EFI disk image
     qemuloongarch                              Create qcow2 image for LoongArch QEMU machines
     directdisk-multi-rootfs                    Create multi rootfs image using rootfs plugin
     directdisk                                 Create a 'pcbios' direct disk image
     efi-bootdisk
     mkhybridiso                                Create a hybrid ISO image
     directdisk-gpt                             Create a 'pcbios' direct disk image
     systemd-bootdisk                           Create an EFI disk image with systemd-boot
     sdimage-bootpart                           Create SD card image with a boot partition
     qemux86-directdisk                         Create a qemu machine 'pcbios' direct disk image
     directdisk-bootloader-config               Create a 'pcbios' direct disk image with custom bootloader config

When you use an existing file, you
do not have to use the `.wks` extension. Here is an example in Raw
Mode that uses the `directdisk` file::

   $ wic create directdisk -r rootfs_dir -b bootimg_dir \
         -k kernel_dir -n native_sysroot

Here are the actual partition language commands used in the
`genericx86.wks` file to generate an image::

   # short-description: Create an EFI disk image for genericx86*
   # long-description: Creates a partitioned EFI disk image for genericx86* machines
   part /boot --source bootimg_efi --sourceparams="loader=grub-efi" --ondisk sda --label msdos --active --align 1024
   part / --source rootfs --ondisk sda --fstype=ext4 --label platform --align 1024 --use-uuid
   part swap --ondisk sda --size 44 --label swap1 --fstype=swap

   bootloader --ptable gpt --timeout=5 --append="rootfstype=ext4 console=ttyS0,115200 console=tty0"

## Using the Wic Plugin Interface

You can extend and specialize Wic functionality by using Wic plugins.
This section explains the Wic plugin interface.

> **Note:**
> Wic plugins consist of "source" and "imager" plugins. Imager plugins
> are beyond the scope of this section.
>

Source plugins provide a mechanism to customize partition content during
the Wic image generation process. You can use source plugins to map
values that you specify using `--source` commands in kickstart files
(i.e. `*.wks`) to a plugin implementation used to populate a given
partition.

> **Note:**
> If you use plugins that have build-time dependencies (e.g. native
> tools, bootloaders, and so forth) when building a Wic image, you need
> to specify those dependencies using the **WKS_FILE_DEPENDS**
> variable.
>

Source plugins are subclasses defined in plugin files. As shipped, the
Yocto Project provides several plugin files. You can see the source
plugin files that ship with the Yocto Project
here .
Each of these plugin files contains source plugins that are designed to
populate a specific Wic image partition.

Source plugins are subclasses of the `SourcePlugin` class, which is
defined in the `openembedded-core/scripts/lib/wic/pluginbase.py` file. For example,
the `BootimgEFIPlugin` source plugin found in the `bootimg_efi.py`
file is a subclass of the `SourcePlugin` class, which is found in the
`pluginbase.py` file.

You can also implement source plugins in a layer outside of the Source
Repositories (external layer). To do so, be sure that your plugin files
are located in a directory whose path is
`scripts/lib/wic/plugins/source/` within your external layer. When the
plugin files are located there, the source plugins they contain are made
available to Wic.

When the Wic implementation needs to invoke a partition-specific
implementation, it looks for the plugin with the same name as the
`--source` parameter used in the kickstart file given to that
partition. For example, if the partition is set up using the following
command in a kickstart file::

   part /boot --source bootimg_pcbios --ondisk sda --label boot --active --align 1024

The methods defined as class
members of the matching source plugin (i.e. `bootimg_pcbios`) in the
`bootimg_pcbios.py` plugin file are used.

To be more concrete, here is the corresponding plugin definition from
the `bootimg_pcbios.py` file for the previous command along with an
example method called by the Wic implementation when it needs to prepare
a partition using an implementation-specific function::

                .
                .
                .
   class BootimgPcbiosPlugin(SourcePlugin):
       """
       Create MBR boot partition and install syslinux on it.
       """

      name = 'bootimg_pcbios'
                .
                .
                .
       @classmethod
       def do_prepare_partition(cls, part, source_params, creator, cr_workdir,
                                oe_builddir, bootimg_dir, kernel_dir,
                                rootfs_dir, native_sysroot):
           """
           Called to do the actual content population for a partition i.e. it
           'prepares' the partition to be incorporated into the image.
           In this case, prepare content for legacy bios boot partition.
           """
                .
                .
                .

If a
subclass (plugin) itself does not implement a particular function, Wic
locates and uses the default version in the superclass. It is for this
reason that all source plugins are derived from the `SourcePlugin`
class.

The `SourcePlugin` class defined in the `pluginbase.py` file defines
a set of methods that source plugins can implement or override. Any
plugins (subclass of `SourcePlugin`) that do not implement a
particular method inherit the implementation of the method from the
`SourcePlugin` class. For more information, see the `SourcePlugin`
class in the `pluginbase.py` file for details:

The following list describes the methods implemented in the
`SourcePlugin` class:

-  `do_prepare_partition()`: Called to populate a partition with
   actual content. In other words, the method prepares the final
   partition image that is incorporated into the disk image.

-  `do_configure_partition()`: Called before
   `do_prepare_partition()` to create custom configuration files for a
   partition (e.g. syslinux or grub configuration files).

-  `do_install_disk()`: Called after all partitions have been
   prepared and assembled into a disk image. This method provides a hook
   to allow finalization of a disk image (e.g. writing an MBR).

-  `do_stage_partition()`: Special content-staging hook called
   before `do_prepare_partition()`. This method is normally empty.

   Typically, a partition just uses the passed-in parameters (e.g. the
   unmodified value of `bootimg_dir`). However, in some cases, things
   might need to be more tailored. As an example, certain files might
   additionally need to be taken from `bootimg_dir + /boot`. This hook
   allows those files to be staged in a customized fashion.

   .. note::

      `get_bitbake_var()` allows you to access non-standard variables that
      you might want to use for this behavior.

You can extend the source plugin mechanism. To add more hooks, create
more source plugin methods within `SourcePlugin` and the corresponding
derived subclasses. The code that calls the plugin methods uses the
`plugin.get_source_plugin_methods()` function to find the method or
methods needed by the call. Retrieval of those methods is accomplished
by filling up a dict with keys that contain the method names of
interest. On success, these will be filled in with the actual methods.
See the Wic implementation for examples and details.

## Wic Examples

This section provides several examples that show how to use the Wic
utility. All the examples assume the list of requirements in the
"dev-manual/wic:requirements" section have been met. The
examples assume the previously generated image is
`core-image-minimal`.

### Generate an Image using an Existing Kickstart File

This example runs in Cooked Mode and uses the `mkefidisk` kickstart
file::

   $ wic create mkefidisk -e core-image-minimal
   INFO: Building wic-tools...
             .
             .
             .
   INFO: The new image(s) can be found here:
     ./mkefidisk-201804191017-sda.direct

   The following build artifacts were used to create the image(s):
     ROOTFS_DIR:                   /home/stephano/yocto/build/tmp-glibc/work/qemux86-oe-linux/core-image-minimal/1.0-r0/rootfs
     BOOTIMG_DIR:                  /home/stephano/yocto/build/tmp-glibc/work/qemux86-oe-linux/core-image-minimal/1.0-r0/recipe-sysroot/usr/share
     KERNEL_DIR:                   /home/stephano/yocto/build/tmp-glibc/deploy/images/qemux86
     NATIVE_SYSROOT:               /home/stephano/yocto/build/tmp-glibc/work/i586-oe-linux/wic-tools/1.0-r0/recipe-sysroot-native

   INFO: The image(s) were created using OE kickstart file:
     /home/stephano/yocto/openembedded-core/scripts/lib/wic/canned-wks/mkefidisk.wks

The previous example shows the easiest way to create an image by running
in cooked mode and supplying a kickstart file and the "-e" option to
point to the existing build artifacts. Your `local.conf` file needs to
have the **MACHINE** variable set
to the machine you are using, which is "qemux86" in this example.

Once the image builds, the output provides image location, artifact use,
and kickstart file information.

> **Note:**
> You should always verify the details provided in the output to make
> sure that the image was indeed created exactly as expected.
>

Continuing with the example, you can now write the image from the
**Build Directory** onto a USB stick, or whatever media for which you
built your image, and boot from the media. You can write the image by using
`bmaptool` or `dd`::

   $ oe-run-native bmaptool-native bmaptool copy mkefidisk-201804191017-sda.direct /dev/sdX

or ::

   $ sudo dd if=mkefidisk-201804191017-sda.direct of=/dev/sdX

> **Note:**
> For more information on how to use the `bmaptool`
> to flash a device with an image, see the
> "dev-manual/bmaptool:flashing images using \bmaptool\``"
> section.
>

### Using a Modified Kickstart File

Because partitioned image creation is driven by the kickstart file, it
is easy to affect image creation by changing the parameters in the file.
This next example demonstrates that through modification of the
`directdisk-gpt` kickstart file.

As mentioned earlier, you can use the command `wic list images` to
show the list of existing kickstart files. The directory in which the
`directdisk-gpt.wks` file resides is
`scripts/lib/image/canned-wks/`, which is located in
**OpenEmbedded-Core (OE-Core)**.
Because available files reside in this directory, you can create and add
your own custom files to the directory. Subsequent use of the
`wic list images` command would then include your kickstart files.

In this example, the existing `directdisk-gpt` file already does most
of what is needed. However, for the hardware in this example, the image
will need to boot from `sdb` instead of `sda`, which is what the
`directdisk-gpt` kickstart file uses.

The example begins by making a copy of the `directdisk-gpt.wks` file
in the `scripts/lib/image/canned-wks` directory and then by changing
the lines that specify the target disk from which to boot::

   $ cp /home/stephano/yocto/openembedded-core/scripts/lib/wic/canned-wks/directdisk-gpt.wks \
        /home/stephano/yocto/openembedded-core/scripts/lib/wic/canned-wks/directdisksdb-gpt.wks

Next, the example modifies the `directdisksdb-gpt.wks` file and
changes all instances of "`--ondisk sda`" to "`--ondisk sdb`". The
example changes the following two lines and leaves the remaining lines
untouched::

   part /boot --source bootimg_pcbios --ondisk sdb --label boot --active --align 1024
   part / --source rootfs --ondisk sdb --fstype=ext4 --label platform --align 1024 --use-uuid

Once the lines are changed, the
example generates the `directdisksdb-gpt` image. The command points
the process at the `core-image-minimal` artifacts for the Next Unit of
Computing (nuc) **MACHINE** the
`local.conf`::

   $ wic create directdisksdb-gpt -e core-image-minimal
   INFO: Building wic-tools...
              .
              .
              .
   Initialising tasks: 100% |#######################################| Time: 0:00:01
   NOTE: Executing SetScene Tasks
   NOTE: Executing RunQueue Tasks
   NOTE: Tasks Summary: Attempted 1161 tasks of which 1157 didn't need to be rerun and all succeeded.
   INFO: Creating image(s)...

   INFO: The new image(s) can be found here:
     ./directdisksdb-gpt-201710090938-sdb.direct

   The following build artifacts were used to create the image(s):
     ROOTFS_DIR:                   /home/stephano/yocto/build/tmp-glibc/work/qemux86-oe-linux/core-image-minimal/1.0-r0/rootfs
     BOOTIMG_DIR:                  /home/stephano/yocto/build/tmp-glibc/work/qemux86-oe-linux/core-image-minimal/1.0-r0/recipe-sysroot/usr/share
     KERNEL_DIR:                   /home/stephano/yocto/build/tmp-glibc/deploy/images/qemux86
     NATIVE_SYSROOT:               /home/stephano/yocto/build/tmp-glibc/work/i586-oe-linux/wic-tools/1.0-r0/recipe-sysroot-native

   INFO: The image(s) were created using OE kickstart file:
     /home/stephano/yocto/openembedded-core/scripts/lib/wic/canned-wks/directdisksdb-gpt.wks

Continuing with the example, you can now directly `dd` the image to a
USB stick, or whatever media for which you built your image, and boot
the resulting media::

   $ sudo dd if=directdisksdb-gpt-201710090938-sdb.direct of=/dev/sdb
   140966+0 records in
   140966+0 records out
   72174592 bytes (72 MB, 69 MiB) copied, 78.0282 s, 925 kB/s
   $ sudo eject /dev/sdb

### Using a Modified Kickstart File and Running in Raw Mode

This next example manually specifies each build artifact (runs in Raw
Mode) and uses a modified kickstart file. The example also uses the
`-o` option to cause Wic to create the output somewhere other than the
default output directory, which is the current directory::

   $ wic create test.wks -o /home/stephano/testwic \
        --rootfs-dir /home/stephano/yocto/build/tmp/work/qemux86-poky-linux/core-image-minimal/1.0-r0/rootfs \
        --bootimg-dir /home/stephano/yocto/build/tmp/work/qemux86-poky-linux/core-image-minimal/1.0-r0/recipe-sysroot/usr/share \
        --kernel-dir /home/stephano/yocto/build/tmp/deploy/images/qemux86 \
        --native-sysroot /home/stephano/yocto/build/tmp/work/i586-poky-linux/wic-tools/1.0-r0/recipe-sysroot-native

   INFO: Creating image(s)...

   INFO: The new image(s) can be found here:
     /home/stephano/testwic/test-201710091445-sdb.direct

   The following build artifacts were used to create the image(s):
     ROOTFS_DIR:                   /home/stephano/yocto/build/tmp-glibc/work/qemux86-oe-linux/core-image-minimal/1.0-r0/rootfs
     BOOTIMG_DIR:                  /home/stephano/yocto/build/tmp-glibc/work/qemux86-oe-linux/core-image-minimal/1.0-r0/recipe-sysroot/usr/share
     KERNEL_DIR:                   /home/stephano/yocto/build/tmp-glibc/deploy/images/qemux86
     NATIVE_SYSROOT:               /home/stephano/yocto/build/tmp-glibc/work/i586-oe-linux/wic-tools/1.0-r0/recipe-sysroot-native

   INFO: The image(s) were created using OE kickstart file:
     test.wks

For this example,
**MACHINE** did not have to be
specified in the `local.conf` file since the artifact is manually
specified.

### Using Wic to Manipulate an Image

Wic image manipulation allows you to shorten turnaround time during
image development. For example, you can use Wic to delete the kernel
partition of a Wic image and then insert a newly built kernel. This
saves you time from having to rebuild the entire image each time you
modify the kernel.

> **Note:**
> In order to use Wic to manipulate a Wic image as in this example,
> your development machine must have the `mtools` package installed.
>

The following example examines the contents of the Wic image, deletes
the existing kernel, and then inserts a new kernel:

#. *List the Partitions:* Use the `wic ls` command to list all the
   partitions in the Wic image::

      $ wic ls tmp/deploy/images/qemux86/core-image-minimal-qemux86.wic
      Num     Start        End          Size      Fstype
       1       1048576     25041919     23993344  fat16
       2      25165824     72157183     46991360  ext4

   The previous output shows two partitions in the
   `core-image-minimal-qemux86.wic` image.

#. *Examine a Particular Partition:* Use the `wic ls` command again
   but in a different form to examine a particular partition.

   .. note::

      You can get command usage on any Wic command using the following
      form::

              $ wic help command

      For example, the following command shows you the various ways to
      use the
      wic ls
      command::

              $ wic help ls

   The following command shows what is in partition one::

        $ wic ls tmp/deploy/images/qemux86/core-image-minimal-qemux86.wic:1
        Volume in drive : is boot
         Volume Serial Number is E894-1809
        Directory for ::/

        libcom32 c32    186500 2017-10-09  16:06
        libutil  c32     24148 2017-10-09  16:06
        syslinux cfg       220 2017-10-09  16:06
        vesamenu c32     27104 2017-10-09  16:06
        vmlinuz        6904608 2017-10-09  16:06
                5 files           7 142 580 bytes
                                 16 582 656 bytes free

   The previous output shows five files, with the
   `vmlinuz` being the kernel.

   .. note::

      If you see the following error, you need to update or create a
      `~/.mtoolsrc` file and be sure to have the line "mtools_skip_check=1"
      in the file. Then, run the Wic command again::

              ERROR: _exec_cmd: /usr/bin/mdir -i /tmp/wic-parttfokuwra ::/ returned '1' instead of 0
               output: Total number of sectors (47824) not a multiple of sectors per track (32)!
               Add mtools_skip_check=1 to your .mtoolsrc file to skip this test

#. *Remove the Old Kernel:* Use the `wic rm` command to remove the
   `vmlinuz` file (kernel)::

      $ wic rm tmp/deploy/images/qemux86/core-image-minimal-qemux86.wic:1/vmlinuz

#. *Add In the New Kernel:* Use the `wic cp` command to add the
   updated kernel to the Wic image. Depending on how you built your
   kernel, it could be in different places. If you used `devtool` and
   an SDK to build your kernel, it resides in the `tmp/work` directory
   of the extensible SDK. If you used `make` to build the kernel, the
   kernel will be in the `workspace/sources` area.

   The following example assumes `devtool` was used to build the
   kernel::

      $ wic cp poky_sdk/tmp/work/qemux86-poky-linux/linux-yocto/4.12.12+git999-r0/linux-yocto-4.12.12+git999/arch/x86/boot/bzImage \
               build/tmp/deploy/images/qemux86/core-image-minimal-qemux86.wic:1/vmlinuz

   Once the new kernel is added back into the image, you can use the
   `dd` command or :ref:`bmaptool
   <dev-manual/bmaptool:flashing images using \`bmaptool\`>` commands
   to flash your wic image onto an SD card or USB stick and test your
   target.

   .. note::

      Using `bmaptool` is generally 10 to 20 times faster than using `dd`.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Flashing Images Using `bmaptool`

A fast and easy way to flash an image to a bootable device is to use
`bmaptool`, which is integrated into the OpenEmbedded build system.
`bmaptool` is a generic tool that creates a file's block map (bmap) and
then uses that map to copy the file. As compared to traditional tools
such as `dd` or `cp`, `bmaptool` can copy (or flash) large files like raw
system image files much faster.

> **Note:**
> -  If you are using Ubuntu or Debian distributions, you can install
> the `bmap-tools` package using the following command and then
> use the tool without specifying `PATH` even from the root
> account::
>
> $ sudo apt install bmap-tools
>
> -  If you are unable to install the `bmap-tools` package, you will
> need to build `bmaptool` before using it. Use the following command::
>
> $ bitbake bmaptool-native -caddto_recipe_sysroot
>

Following, is an example that shows how to flash a Wic image. Realize
that while this example uses a Wic image, you can use `bmaptool` to flash
any type of image. Use these steps to flash an image using `bmaptool`:

#. *Update your local.conf File:* You need to have the following set
   in your `local.conf` file before building your image::

      IMAGE_FSTYPES += "wic wic.bmap"

#. *Get Your Image:* Either have your image ready (pre-built with the
   **IMAGE_FSTYPES**
   setting previously mentioned) or take the step to build the image::

      $ bitbake image

#. *Flash the Device:* Flash the device with the image by using `bmaptool`
   depending on your particular setup. The following commands assume the
   image resides in the **Build Directory**'s `deploy/images/` area:

   -  If you installed the package for `bmaptool`, you can directly run::

         $ sudo bmaptool copy build-directory/tmp/deploy/images/machine/image.wic /dev/sdX

   -  Otherwise, if you built `bmaptool` with BitBake, run::

         $ sudo chmod a+w /dev/sdX       # get write access to the media, needed only once after booting
         $ oe-run-native bmaptool-native bmaptool copy build-directory/tmp/deploy/images/machine/image.wic /dev/sdX

For help on the `bmaptool` command, use the following command::

   $ bmaptool --help

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Speeding Up a Build

Build time can be an issue. By default, the build system uses simple
controls to try and maximize build efficiency. In general, the default
settings for all the following variables result in the most efficient
build times when dealing with single socket systems (i.e. a single CPU).
If you have multiple CPUs, you might try increasing the default values
to gain more speed. See the descriptions in the glossary for each
variable for more information:

-  **BB_NUMBER_THREADS**:
   The maximum number of threads BitBake simultaneously executes.

-  **BB_NUMBER_PARSE_THREADS**:
   The number of threads BitBake uses during parsing.

-  **PARALLEL_MAKE**: Extra
   options passed to the `make` command during the
   ref-tasks-compile task in
   order to specify parallel compilation on the local build host.

-  **PARALLEL_MAKEINST**:
   Extra options passed to the `make` command during the
   ref-tasks-install task in
   order to specify parallel installation on the local build host.

As mentioned, these variables all scale to the number of processor cores
available on the build system. For single socket systems, this
auto-scaling ensures that the build system fundamentally takes advantage
of potential parallel operations during the build based on the build
machine's capabilities.

Additional factors that can affect build speed are:

-  File system type: The file system type that the build is being
   performed on can also influence performance. Using `ext4` is
   recommended as compared to `ext2` and `ext3` due to `ext4`
   improved features such as extents.

-  Disabling the updating of access time using `noatime`: The
   `noatime` mount option prevents the build system from updating file
   and directory access times.

-  Setting a longer commit: Using the "commit=" mount option increases
   the interval in seconds between disk cache writes. Changing this
   interval from the five second default to something longer increases
   the risk of data loss but decreases the need to write to the disk,
   thus increasing the build performance.

-  Choosing the packaging backend: Of the available packaging backends,
   IPK is the fastest. Additionally, selecting a singular packaging
   backend also helps.

-  Using `tmpfs` for **TMPDIR**
   as a temporary file system: While this can help speed up the build,
   the benefits are limited due to the compiler using `-pipe`. The
   build system goes to some lengths to avoid `sync()` calls into the
   file system on the principle that if there was a significant failure,
   the **Build Directory** contents could easily be rebuilt.

-  Inheriting the ref-classes-rm-work class:
   Inheriting this class has shown to speed up builds due to
   significantly lower amounts of data stored in the data cache as well
   as on disk. Inheriting this class also makes cleanup of
   **TMPDIR** faster, at the
   expense of being easily able to dive into the source code. File
   system maintainers have recommended that the fastest way to clean up
   large numbers of files is to reformat partitions rather than delete
   files due to the linear nature of partitions. This, of course,
   assumes you structure the disk partitions and file systems in a way
   that this is practical.

Aside from the previous list, you should keep some trade offs in mind
that can help you speed up the build:

-  Remove items from
   **DISTRO_FEATURES**
   that you might not need.

-  Exclude debug symbols and other debug information: If you do not need
   these symbols and other debug information, disabling the `*-dbg`
   package generation can speed up the build. You can disable this
   generation by setting the
   **INHIBIT_PACKAGE_DEBUG_SPLIT**
   variable to "1".

-  Disable static library generation for recipes derived from
   `autoconf` or `libtool`: Here is an example showing how to
   disable static libraries and still provide an override to handle
   exceptions::

      STATICLIBCONF = "--disable-static"
      STATICLIBCONF:sqlite3-native = ""
      EXTRA_OECONF += "${STATICLIBCONF}"

   .. note::

      -  Some recipes need static libraries in order to work correctly
         (e.g. `pseudo-native` needs `sqlite3-native`). Overrides,
         as in the previous example, account for these kinds of
         exceptions.

      -  Some packages have packaging code that assumes the presence of
         the static libraries. If so, you might need to exclude them as
         well.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Conserving Disk Space

## Conserving Disk Space During Builds

To help conserve disk space during builds, you can add the following
statement to your project's `local.conf` configuration file found in
the **Build Directory**::

   INHERIT += "rm_work"

Adding this statement deletes the work directory used for
building a recipe once the recipe is built. For more information on
"rm_work", see the ref-classes-rm-work class in the
Yocto Project Reference Manual.

When you inherit this class and build a `core-image-sato` image for a
`qemux86-64` machine from an Ubuntu 22.04 x86-64 system, you end up with a
final disk usage of 22 Gbytes instead of &MIN_DISK_SPACE; Gbytes. However,
&MIN_DISK_SPACE_RM_WORK; Gbytes of initial free disk space are still needed to
create temporary files before they can be deleted.

## Purging Obsolete Shared State Cache Files

After multiple build iterations, the Shared State (sstate) cache can contain
multiple cache files for a given package, consuming a substantial amount of
disk space. However, only the most recent ones are likely to be reused.

The following command is a quick way to purge all the cache files which
haven't been used for a least a specified number of days::

   find build/sstate-cache -type f -mtime +$DAYS -delete

The above command relies on the fact that BitBake touches the sstate cache
files as it accesses them, when it has write access to the cache.

You could use `-atime` instead of `-mtime` if the partition isn't mounted
with the `noatime` option for a read only cache.

For more advanced needs, OpenEmbedded-Core also offers a more elaborate
command. It has the ability to purge all but the newest cache files on each
architecture, and also to remove files that it considers unreachable by
exploring a set of build configurations. However, this command
requires a full build environment to be available and doesn't work well
covering multiple releases. It won't work either on limited environments
such as BSD based NAS::

   sstate-cache-management.py --remove-duplicated --cache-dir=sstate-cache

This command will ask you to confirm the deletions it identifies.
Run `sstate-cache-management.py` for more details about this script.

> **Note:**
> As this command is much more cautious and selective, removing only cache files,
> it will execute much slower than the simple `find` command described above.
> Therefore, it may not be your best option to trim huge cache directories.
>
>

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Working With Libraries

Libraries are an integral part of your system. This section describes
some common practices you might find helpful when working with libraries
to build your system:

-  :ref:`How to include static library files
   <dev-manual/libraries:including static library files>`

-  :ref:`How to use the Multilib feature to combine multiple versions of
   library files into a single image
   <dev-manual/libraries:combining multiple versions of library files into one image>`

-  :ref:`How to install multiple versions of the same library in parallel on
   the same system
   <dev-manual/libraries:installing multiple versions of the same library>`

## Including Static Library Files

If you are building a library and the library offers static linking, you
can control which static library files (`*.a` files) get included in
the built library.

The **PACKAGES** and
**FILES:* <FILES>** variables in the
`meta/conf/bitbake.conf` configuration file define how files installed
by the ref-tasks-install task are packaged. By default, the **PACKAGES**
variable includes `${PN}-staticdev`, which represents all static
library files.

> **Note:**
> Some previously released versions of the Yocto Project defined the
> static library files through `${PN}-dev`.
>

Here is a small part of the BitBake configuration file, where you can see
how the static library files are defined::

   PACKAGES = "${PN}-src ${PN}-dbg ${PN}-staticdev ${PN}-dev ${PN}-doc ${PN}-locale ${PACKAGE_BEFORE_PN} ${PN}"

   FILES:${PN}-staticdev = "${libdir}/*.a ${base_libdir}/*.a ${libdir}/${BPN}/*.a"
   SECTION:${PN}-staticdev = "devel"
   RDEPENDS:${PN}-staticdev = "${PN}-dev (= ${EXTENDPKGV})"

## Combining Multiple Versions of Library Files into One Image

The build system offers the ability to build libraries with different
target optimizations or architecture formats and combine these together
into one system image. You can link different binaries in the image
against the different libraries as needed for specific use cases. This
feature is called "Multilib".

An example would be where you have most of a system compiled in 32-bit
mode using 32-bit libraries, but you have something large, like a
database engine, that needs to be a 64-bit application and uses 64-bit
libraries. Multilib allows you to get the best of both 32-bit and 64-bit
libraries.

While the Multilib feature is most commonly used for 32 and 64-bit
differences, the approach the build system uses facilitates different
target optimizations. You could compile some binaries to use one set of
libraries and other binaries to use a different set of libraries. The
libraries could differ in architecture, compiler options, or other
optimizations.

There are several examples in the `meta-skeleton` layer found in
**OpenEmbedded-Core (OE-Core)**:

-  conf/multilib-example.conf 
   configuration file.

-  conf/multilib-example2.conf 
   configuration file.

-  recipes-multilib/images/core-image-multilib-example.bb 
   recipe

### Preparing to Use Multilib

User-specific requirements drive the Multilib feature. Consequently,
there is no one "out-of-the-box" configuration that would
meet your needs.

In order to enable Multilib, you first need to ensure your recipe is
extended to support multiple libraries. Many standard recipes are
already extended and support multiple libraries. You can check in the
`meta/conf/multilib.conf` configuration file in
**OpenEmbedded-Core (OE-Core)** to see how this is
done using the
**BBCLASSEXTEND** variable.
Eventually, all recipes will be covered and this list will not be
needed.

For the most part, the Multilib 
class extension works automatically to
extend the package name from `${PN}` to `${MLPREFIX}${PN}`, where
**MLPREFIX** is the particular multilib (e.g. "lib32-" or "lib64-").
Standard variables such as
**DEPENDS**,
**RDEPENDS**,
**RPROVIDES**,
**RRECOMMENDS**,
**PACKAGES**, and
**PACKAGES_DYNAMIC** are
automatically extended by the system. If you are extending any manual
code in the recipe, you can use the `${MLPREFIX}` variable to ensure
those names are extended correctly.

### Using Multilib

After you have set up the recipes, you need to define the actual
combination of multiple libraries you want to build. You accomplish this
through your `local.conf` configuration file in the
**Build Directory**. An example configuration would be as follows::

   MACHINE = "qemux86-64"
   require conf/multilib.conf
   MULTILIBS = "multilib:lib32"
   DEFAULTTUNE:virtclass-multilib-lib32 = "x86"
   IMAGE_INSTALL:append = " lib32-glib-2.0"

This example enables an additional library named
`lib32` alongside the normal target packages. When combining these
"lib32" alternatives, the example uses "x86" for tuning. For information
on this particular tuning, see
`meta/conf/machine/include/ia32/arch-ia32.inc`.

The example then includes `lib32-glib-2.0` in all the images, which
illustrates one method of including a multiple library dependency. You
can use a normal image build to include this dependency, for example::

   $ bitbake core-image-sato

You can also build Multilib packages
specifically with a command like this::

   $ bitbake lib32-glib-2.0

### Additional Implementation Details

There are generic implementation details as well as details that are specific to
package management systems. Here are implementation details
that exist regardless of the package management system:

-  The typical convention used for the class extension code as used by
   Multilib assumes that all package names specified in
   **PACKAGES** that contain
   `${PN}` have `${PN}` at the start of the name. When that
   convention is not followed and `${PN}` appears at the middle or the
   end of a name, problems occur.

-  The **TARGET_VENDOR**
   value under Multilib will be extended to "-vendormlmultilib" (e.g.
   "-pokymllib32" for a "lib32" Multilib with Poky). The reason for this
   slightly unwieldy contraction is that any "-" characters in the
   vendor string presently break Autoconf's `config.sub`, and other
   separators are problematic for different reasons.

Here are the implementation details for the RPM Package Management System:

-  A unique architecture is defined for the Multilib packages, along
   with creating a unique deploy folder under `tmp/deploy/rpm` in the
   **Build Directory**. For example, consider `lib32` in a
   `qemux86-64` image. The possible architectures in the system are "all",
   "qemux86_64", "lib32:qemux86_64", and "lib32:x86".

-  The `${MLPREFIX}` variable is stripped from `${PN}` during RPM
   packaging. The naming for a normal RPM package and a Multilib RPM
   package in a `qemux86-64` system resolves to something similar to
   `bash-4.1-r2.x86_64.rpm` and `bash-4.1.r2.lib32_x86.rpm`,
   respectively.

-  When installing a Multilib image, the RPM backend first installs the
   base image and then installs the Multilib libraries.

-  The build system relies on RPM to resolve the identical files in the
   two (or more) Multilib packages.

Here are the implementation details for the IPK Package Management System:

-  The `${MLPREFIX}` is not stripped from `${PN}` during IPK
   packaging. The naming for a normal RPM package and a Multilib IPK
   package in a `qemux86-64` system resolves to something like
   `bash_4.1-r2.x86_64.ipk` and `lib32-bash_4.1-rw:x86.ipk`,
   respectively.

-  The IPK deploy folder is not modified with `${MLPREFIX}` because
   packages with and without the Multilib feature can exist in the same
   folder due to the `${PN}` differences.

-  IPK defines a sanity check for Multilib installation using certain
   rules for file comparison, overridden, etc.

## Installing Multiple Versions of the Same Library

There are be situations where you need to install and use multiple versions
of the same library on the same system at the same time. This
almost always happens when a library API changes and you have
multiple pieces of software that depend on the separate versions of the
library. To accommodate these situations, you can install multiple
versions of the same library in parallel on the same system.

The process is straightforward as long as the libraries use proper
versioning. With properly versioned libraries, all you need to do to
individually specify the libraries is create separate, appropriately
named recipes where the **PN** part of
the name includes a portion that differentiates each library version
(e.g. the major part of the version number). Thus, instead of having a
single recipe that loads one version of a library (e.g. `clutter`),
you provide multiple recipes that result in different versions of the
libraries you want. As an example, the following two recipes would allow
the two separate versions of the `clutter` library to co-exist on the
same system:

```none
clutter-1.6_1.6.20.bb
clutter-1.8_1.8.4.bb
```

Additionally, if
you have other recipes that depend on a given library, you need to use
the **DEPENDS** variable to
create the dependency. Continuing with the same example, if you want to
have a recipe depend on the 1.8 version of the `clutter` library, use
the following in your recipe::

   DEPENDS = "clutter-1.8"

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Working with Pre-Built Libraries

## Introduction

Some library vendors do not release source code for their software but do
release pre-built binaries. When shared libraries are built, they should
be versioned (see `this article
<https://tldp.org/HOWTO/Program-Library-HOWTO/shared-libraries.html>`__
for some background), but sometimes this is not done.

To summarize, a versioned library must meet two conditions:

#.    The filename must have the version appended, for example: `libfoo.so.1.2.3`.
#.    The library must have the ELF tag `SONAME` set to the major version
      of the library, for example: `libfoo.so.1`. You can check this by
      running `readelf -d filename | grep SONAME`.

This section shows how to deal with both versioned and unversioned
pre-built libraries.

## Versioned Libraries

In this example we work with pre-built libraries for the FT4222H USB I/O chip.
Libraries are built for several target architecture variants and packaged in
an archive as follows::

   ├── build-arm-hisiv300
   │   └── libft4222.so.1.4.4.44
   ├── build-arm-v5-sf
   │   └── libft4222.so.1.4.4.44
   ├── build-arm-v6-hf
   │   └── libft4222.so.1.4.4.44
   ├── build-arm-v7-hf
   │   └── libft4222.so.1.4.4.44
   ├── build-arm-v8
   │   └── libft4222.so.1.4.4.44
   ├── build-i386
   │   └── libft4222.so.1.4.4.44
   ├── build-i486
   │   └── libft4222.so.1.4.4.44
   ├── build-mips-eglibc-hf
   │   └── libft4222.so.1.4.4.44
   ├── build-pentium
   │   └── libft4222.so.1.4.4.44
   ├── build-x86_64
   │   └── libft4222.so.1.4.4.44
   ├── examples
   │   ├── get-version.c
   │   ├── i2cm.c
   │   ├── spim.c
   │   └── spis.c
   ├── ftd2xx.h
   ├── install4222.sh
   ├── libft4222.h
   ├── ReadMe.txt
   └── WinTypes.h

To write a recipe to use such a library in your system:

-  The vendor will probably have a proprietary licence, so set
   **LICENSE_FLAGS** in your recipe.
-  The vendor provides a tarball containing libraries so set **SRC_URI**
   appropriately.
-  Set **COMPATIBLE_HOST** so that the recipe cannot be used with an
   unsupported architecture. In the following example, we only support the 32
   and 64 bit variants of the `x86` architecture.
-  As the vendor provides versioned libraries, we can use `oe_soinstall`
   from ref-classes-utils to install the shared library and create
   symbolic links. If the vendor does not do this, we need to follow the
   non-versioned library guidelines in the next section.
-  As the vendor likely used **LDFLAGS** different from those in your Yocto
   Project build, disable the corresponding checks by adding `ldflags`
   to **INSANE_SKIP**.
-  The vendor will typically ship release builds without debugging symbols.
   Avoid errors by preventing the packaging task from stripping out the symbols
   and adding them to a separate debug package. This is done by setting the
   `INHIBIT_` flags shown below.

The complete recipe would look like this::

   SUMMARY = "FTDI FT4222H Library"
   SECTION = "libs"
   LICENSE_FLAGS = "ftdi"
   LICENSE = "CLOSED"

   COMPATIBLE_HOST = "(i.86|x86_64).*-linux"

   # Sources available in a .tgz file in .zip archive
   # at https://ftdichip.com/wp-content/uploads/2021/01/libft4222-linux-1.4.4.44.zip
   # Found on https://ftdichip.com/software-examples/ft4222h-software-examples/
   # Since dealing with this particular type of archive is out of topic here,
   # we use a local link.
   SRC_URI = "file://libft4222-linux-${PV}.tgz"

   S = "${UNPACKDIR}"

   ARCH_DIR:x86-64 = "build-x86_64"
   ARCH_DIR:i586 = "build-i386"
   ARCH_DIR:i686 = "build-i386"

   INSANE_SKIP:${PN} = "ldflags"
   INHIBIT_PACKAGE_STRIP = "1"
   INHIBIT_SYSROOT_STRIP = "1"
   INHIBIT_PACKAGE_DEBUG_SPLIT = "1"

   do_install () {
           install -m 0755 -d ${D}${libdir}
           oe_soinstall ${S}/${ARCH_DIR}/libft4222.so.${PV} ${D}${libdir}
           install -d ${D}${includedir}
           install -m 0755 ${S}/*.h ${D}${includedir}
   }

If the precompiled binaries are not statically linked and have dependencies on
other libraries, then by adding those libraries to **DEPENDS**, the linking
can be examined and the appropriate **RDEPENDS** automatically added.

## Non-Versioned Libraries

### Some Background

Libraries in Linux systems are generally versioned so that it is possible
to have multiple versions of the same library installed, which eases upgrades
and support for older software. For example, suppose that in a versioned
library, an actual library is called `libfoo.so.1.2`, a symbolic link named
`libfoo.so.1` points to `libfoo.so.1.2`, and a symbolic link named
`libfoo.so` points to `libfoo.so.1.2`. Given these conditions, when you
link a binary against a library, you typically provide the unversioned file
name (i.e. `-lfoo` to the linker). However, the linker follows the symbolic
link and actually links against the versioned filename. The unversioned symbolic
link is only used at development time. Consequently, the library is packaged
along with the headers in the development package `${PN}-dev` along with the
actual library and versioned symbolic links in `${PN}`. Because versioned
libraries are far more common than unversioned libraries, the default packaging
rules assume versioned libraries.

### Yocto Library Packaging Overview

It follows that packaging an unversioned library requires a bit of work in the
recipe. By default, `libfoo.so` gets packaged into `${PN}-dev`, which
triggers a QA warning that a non-symlink library is in a `-dev` package,
and binaries in the same recipe link to the library in `${PN}-dev`,
which triggers more QA warnings. To solve this problem, you need to package the
unversioned library into `${PN}` where it belongs. The abridged
default **FILES** variables in `bitbake.conf` are::

   SOLIBS = ".so.*"
   SOLIBSDEV = ".so"
   FILES:${PN} = "... ${libdir}/lib*${SOLIBS} ..."
   FILES_SOLIBSDEV ?= "... ${libdir}/lib*${SOLIBSDEV} ..."
   FILES:${PN}-dev = "... ${FILES_SOLIBSDEV} ..."

**SOLIBS** defines a pattern that matches real shared object libraries.
**SOLIBSDEV** matches the development form (unversioned symlink). These two
variables are then used in `FILES:${PN}` and `FILES:${PN}-dev`, which puts
the real libraries into `${PN}` and the unversioned symbolic link into `${PN}-dev`.
To package unversioned libraries, you need to modify the variables in the recipe
as follows::

   SOLIBS = ".so"
   FILES_SOLIBSDEV = ""

The modifications cause the `.so` file to be the real library
and unset **FILES_SOLIBSDEV** so that no libraries get packaged into
`${PN}-dev`. The changes are required because unless **PACKAGES** is changed,
`${PN}-dev` collects files before `${PN}`. `${PN}-dev` must not collect any of
the files you want in `${PN}`.

Finally, loadable modules, essentially unversioned libraries that are linked
at runtime using `dlopen()` instead of at build time, should generally be
installed in a private directory. However, if they are installed in `${libdir}`,
then the modules can be treated as unversioned libraries.

### Example

The example below installs an unversioned x86-64 pre-built library named
`libfoo.so`. The **COMPATIBLE_HOST** variable limits recipes to the
x86-64 architecture while the **INSANE_SKIP**, **INHIBIT_PACKAGE_STRIP**
and **INHIBIT_SYSROOT_STRIP** variables are all set as in the above
versioned library example. The "magic" is setting the **SOLIBS** and
**FILES_SOLIBSDEV** variables as explained above::

   SUMMARY = "libfoo sample recipe"
   SECTION = "libs"
   LICENSE = "CLOSED"

   SRC_URI = "file://libfoo.so"

   COMPATIBLE_HOST = "x86_64.*-linux"

   INSANE_SKIP:${PN} = "ldflags"
   INHIBIT_PACKAGE_STRIP = "1"
   INHIBIT_SYSROOT_STRIP = "1"
   SOLIBS = ".so"
   FILES_SOLIBSDEV = ""

   do_install () {
           install -d ${D}${libdir}
           install -m 0755 ${UNPACKDIR}/libfoo.so ${D}${libdir}
   }

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Using the ``devtool`` command-line tool

The `devtool` command-line tool provides a number of features that
help you build, test, and package software. This command is available
alongside the `bitbake` command. Additionally, the `devtool` command
is a key part of the **Extensible Software Development Kit (eSDK)**.

## Use ``devtool add`` to Add an Application

The `devtool add` command generates a new recipe based on existing
source code. This command takes advantage of the
devtool-the-workspace-layer-structure
layer that many `devtool` commands use. The command is flexible enough
to allow you to extract source code into both the workspace or a
separate local Git repository and to use existing code that does not
need to be extracted.

Depending on your particular scenario, the arguments and options you use
with `devtool add` form different combinations. The following diagram
shows common development flows you would use with the `devtool add`
command:

![devtool-add-flow.png](figures/devtool-add-flow.png)

#. *Generating the New Recipe*: The top part of the flow shows three
   scenarios by which you could use `devtool add` to generate a recipe
   based on existing source code.

   In a shared development environment, it is typical for other
   developers to be responsible for various areas of source code. As a
   developer, you are probably interested in using that source code as
   part of your development within the Yocto Project. All you need is
   access to the code, a recipe, and a controlled area in which to do
   your work.

   Within the diagram, three possible scenarios feed into the
   `devtool add` workflow:

   -  *Left*: The left scenario in the figure represents a common
      situation where the source code does not exist locally and needs
      to be extracted. In this situation, the source code is extracted
      to the default workspace --- you do not want the files in some
      specific location outside of the workspace. Thus, everything you
      need will be located in the workspace::

         $ devtool add recipe fetchuri

      With this command, `devtool` extracts the upstream
      source files into a local Git repository within the `sources`
      folder. The command then creates a recipe named recipe and a
      corresponding append file in the workspace. If you do not provide
      recipe, the command makes an attempt to determine the recipe name.

   -  *Middle*: The middle scenario in the figure also represents a
      situation where the source code does not exist locally. In this
      case, the code is again upstream and needs to be extracted to some
      local area --- this time outside of the default workspace.

      .. note::

         If required, `devtool` always creates a Git repository locally
         during the extraction.

      Furthermore, the first positional argument `srctree` in this case
      identifies where the `devtool add` command will locate the
      extracted code outside of the workspace. You need to specify an
      empty directory::

         $ devtool add recipe srctree fetchuri

      In summary, the source code is pulled from fetchuri and extracted into the
      location defined by `srctree` as a local Git repository.

      Within workspace, `devtool` creates a recipe named recipe along
      with an associated append file.

   -  *Right*: The right scenario in the figure represents a situation
      where the `srctree` has been previously prepared outside of the
      `devtool` workspace.

      The following command provides a new recipe name and identifies
      the existing source tree location::

         $ devtool add recipe srctree

      The command examines the source code and creates a recipe named
      recipe for the code and places the recipe into the workspace.

      Because the extracted source code already exists, `devtool` does
      not try to relocate the source code into the workspace --- only the
      new recipe is placed in the workspace.

      Aside from a recipe folder, the command also creates an associated
      append folder and places an initial `*.bbappend` file within.

#. *Edit the Recipe*: You can use `devtool edit-recipe` to open up the
   editor as defined by the `$EDITOR` environment variable and modify
   the file::

      $ devtool edit-recipe recipe

   From within the editor, you can make modifications to the recipe that
   take effect when you build it later.

#. *Build the Recipe or Rebuild the Image*: The next step you take
   depends on what you are going to do with the new code.

   If you need to eventually move the build output to the target
   hardware, use the following `devtool` command::

      $ devtool build recipe

   On the other hand, if you want an image to contain the recipe's
   packages from the workspace for immediate deployment onto a device
   (e.g. for testing purposes), you can use the `devtool build-image`
   command::

      $ devtool build-image image

#. *Deploy the Build Output*: When you use the `devtool build` command
   to build out your recipe, you probably want to see if the resulting
   build output works as expected on the target hardware.

   .. note::

      This step assumes you have a previously built image that is
      already either running in QEMU or is running on actual hardware.
      Also, it is assumed that for deployment of the image to the
      target, SSH is installed in the image and, if the image is running
      on real hardware, you have network access to and from your
      development machine.

   You can deploy your build output to that target hardware by using the
   `devtool deploy-target` command::

      $ devtool deploy-target recipe target

   The target is a live target machine running as an SSH server.

   You can, of course, also deploy the image you build to actual
   hardware by using the `devtool build-image` command. However,
   `devtool` does not provide a specific command that allows you to
   deploy the image to actual hardware.

#. *Finish Your Work With the Recipe*: The `devtool finish` command
   creates any patches corresponding to commits in the local Git
   repository, moves the new recipe to a more permanent layer, and then
   resets the recipe so that the recipe is built normally rather than
   from the workspace::

      $ devtool finish recipe layer

   .. note::

      Any changes you want to turn into patches must be committed to the
      Git repository in the source tree.

   As mentioned, the `devtool finish` command moves the final recipe
   to its permanent layer.

   As a final process of the `devtool finish` command, the state of
   the standard layers and the upstream source is restored so that you
   can build the recipe from those areas rather than the workspace.

   .. note::

      You can use the `devtool reset` command to put things back should you
      decide you do not want to proceed with your work. If you do use this
      command, realize that the source tree is preserved.

## Use ``devtool modify`` to Modify the Source of an Existing Component

The `devtool modify` command prepares the way to work on existing code
that already has a local recipe in place that is used to build the
software. The command is flexible enough to allow you to extract code
from an upstream source, specify the existing recipe, and keep track of
and gather any patch files from other developers that are associated
with the code.

Depending on your particular scenario, the arguments and options you use
with `devtool modify` form different combinations. The following
diagram shows common development flows for the `devtool modify`
command:

![devtool-modify-flow.png](figures/devtool-modify-flow.png)

#. *Preparing to Modify the Code*: The top part of the flow shows three
   scenarios by which you could use `devtool modify` to prepare to
   work on source files. Each scenario assumes the following:

   -  The recipe exists locally in a layer external to the `devtool`
      workspace.

   -  The source files exist either upstream in an un-extracted state or
      locally in a previously extracted state.

   The typical situation is where another developer has created a layer
   for use with the Yocto Project and their recipe already resides in
   that layer. Furthermore, their source code is readily available
   either upstream or locally.

   -  *Left*: The left scenario in the figure represents a common
      situation where the source code does not exist locally and it
      needs to be extracted from an upstream source. In this situation,
      the source is extracted into the default `devtool` workspace
      location. The recipe, in this scenario, is in its own layer
      outside the workspace (i.e. `meta-`\ layername).

      The following command identifies the recipe and, by default,
      extracts the source files::

         $ devtool modify recipe

      Once `devtool` locates the recipe, `devtool` uses the recipe's
      **SRC_URI** statements to locate the source code and any local
      patch files from other developers.

      With this scenario, there is no `srctree` argument. Consequently, the
      default behavior of the `devtool modify` command is to extract
      the source files pointed to by the **SRC_URI** statements into a
      local Git structure. Furthermore, the location for the extracted
      source is the default area within the `devtool` workspace. The
      result is that the command sets up both the source code and an
      append file within the workspace while the recipe remains in its
      original location.

      Additionally, if you have any non-patch local files (i.e. files
      referred to with `file://` entries in **SRC_URI** statement
      excluding `*.patch/` or `*.diff`), these files are copied to
      an `oe-local-files` folder under the newly created source tree.
      Copying the files here gives you a convenient area from which you
      can modify the files. Any changes or additions you make to those
      files are incorporated into the build the next time you build the
      software just as are other changes you might have made to the
      source.

   -  *Middle*: The middle scenario in the figure represents a situation
      where the source code also does not exist locally. In this case,
      the code is again upstream and needs to be extracted to some local
      area as a Git repository. The recipe, in this scenario, is again
      local and in its own layer outside the workspace.

      The following command tells `devtool` the recipe with which to
      work and, in this case, identifies a local area for the extracted
      source files that exists outside of the default `devtool`
      workspace::

         $ devtool modify recipe srctree

      .. note::

         You cannot provide a URL for `srctree` using the `devtool` command.

      As with all extractions, the command uses the recipe's **SRC_URI**
      statements to locate the source files and any associated patch
      files. Non-patch files are copied to an `oe-local-files` folder
      under the newly created source tree.

      Once the files are located, the command by default extracts them
      into `srctree`.

      Within workspace, `devtool` creates an append file for the
      recipe. The recipe remains in its original location but the source
      files are extracted to the location you provide with `srctree`.

   -  *Right*: The right scenario in the figure represents a situation
      where the source tree (`srctree`) already exists locally as a
      previously extracted Git structure outside of the `devtool`
      workspace. In this example, the recipe also exists elsewhere
      locally in its own layer.

      The following command tells `devtool` the recipe with which to
      work, uses the "-n" option to indicate source does not need to be
      extracted, and uses `srctree` to point to the previously extracted
      source files::

         $ devtool modify -n recipe srctree

      If an `oe-local-files` subdirectory happens to exist and it
      contains non-patch files, the files are used. However, if the
      subdirectory does not exist and you run the `devtool finish`
      command, any non-patch files that might exist next to the recipe
      are removed because it appears to `devtool` that you have
      deleted those files.

      Once the `devtool modify` command finishes, it creates only an
      append file for the recipe in the `devtool` workspace. The
      recipe and the source code remain in their original locations.

#. *Edit the Source*: Once you have used the `devtool modify` command,
   you are free to make changes to the source files. You can use any
   editor you like to make and save your source code modifications.

#. *Build the Recipe or Rebuild the Image*: The next step you take
   depends on what you are going to do with the new code.

   If you need to eventually move the build output to the target
   hardware, use the following `devtool` command::

      $ devtool build recipe

   On the other hand, if you want an image to contain the recipe's
   packages from the workspace for immediate deployment onto a device
   (e.g. for testing purposes), you can use the `devtool build-image`
   command::

      $ devtool build-image image

#. *Deploy the Build Output*: When you use the `devtool build` command
   to build out your recipe, you probably want to see if the resulting
   build output works as expected on target hardware.

   .. note::

      This step assumes you have a previously built image that is
      already either running in QEMU or running on actual hardware.
      Also, it is assumed that for deployment of the image to the
      target, SSH is installed in the image and if the image is running
      on real hardware that you have network access to and from your
      development machine.

   You can deploy your build output to that target hardware by using the
   `devtool deploy-target` command::

      $ devtool deploy-target recipe target

   The target is a live target machine running as an SSH server.

   You can, of course, use other methods to deploy the image you built
   using the `devtool build-image` command to actual hardware.
   `devtool` does not provide a specific command to deploy the image
   to actual hardware.

#. *Finish Your Work With the Recipe*: The `devtool finish` command
   creates any patches corresponding to commits in the local Git
   repository, updates the recipe to point to them (or creates a
   `.bbappend` file to do so, depending on the specified destination
   layer), and then resets the recipe so that the recipe is built
   normally rather than from the workspace::

      $ devtool finish recipe layer

   .. note::

      Any changes you want to turn into patches must be staged and
      committed within the local Git repository before you use the
      `devtool finish` command.

   Because there is no need to move the recipe, `devtool finish`
   either updates the original recipe in the original layer or the
   command creates a `.bbappend` file in a different layer as provided
   by layer. Any work you did in the `oe-local-files` directory is
   preserved in the original files next to the recipe during the
   `devtool finish` command.

   As a final process of the `devtool finish` command, the state of
   the standard layers and the upstream source is restored so that you
   can build the recipe from those areas rather than from the workspace.

   .. note::

      You can use the `devtool reset` command to put things back should you
      decide you do not want to proceed with your work. If you do use this
      command, realize that the source tree is preserved.

## ``devtool ide-sdk`` configures IDEs and bootstraps SDKs

The `devtool ide-sdk` command can provide an IDE configuration for IDEs when
working on the source code of one or more recipes.
Depending on the programming language, and the build system used by the recipe,
the tools required for cross-development and remote debugging are different.
For example:

-  A C/C++ project usually uses CMake or Meson.

-  A Python project uses setuptools or one of its successors.

-  A Rust project uses Cargo.

Also, the IDE plugins needed for the integration of a build system with the
IDE and the corresponding settings are usually specific to these build-systems.
To hide all these details from the user, `devtool ide-sdk` does two things:

-  It generates any kind of SDK needed for cross-development and remote
   debugging of the specified recipes.

-  It generates the configuration for the IDE (and the IDE plugins) for using
   the cross-toolchain and remote debugging tools provided by the SDK directly
   from the IDE.

For supported build systems the configurations generated by `devtool ide-sdk`
combine the advantages of the `devtool modify` based workflow
(see using_devtool) with the advantages of the simple Environment Setup
script based workflow (see running_the_ext_sdk_env) provided by Yocto's
SDK or eSDK:

-  The source code of the recipe is in the workspace created by
   `devtool modify` or `devtool add`.
   Using `devtool build`, `devtool build-image`,
   `devtool deploy-target` or `bitbake` is possible.
   Also `devtool ide-sdk` can be used to update the SDK and the IDE
   configuration at any time.

-  `devtool ide-sdk` aims to support multiple programming languages and
   multiple IDEs natively. "Natively" means that the IDE is configured to call
   the build tool (e.g. `cmake` or `meson`) directly. This has several
   advantages.
   First of all, it is usually much faster to call for example `cmake` than
   `devtool build`.
   It also allows to benefit from the very good integration that IDEs like
   VSCode offer for tools like CMake or GDB.

   However, supporting many programming languages and multiple
   IDEs is quite an elaborate and constantly evolving thing. Support for IDEs
   is therefore implemented as plugins. Plugins can also be provided by
   optional layers.

So much about the introduction to the default mode of `devtool sdk-ide` which
is called the "modified" mode because it uses the workspace created by
`devtool modify` and the per recipe **Sysroots <Sysroot>** of BitBake.

For some recipes and use cases, this default behavior of `devtool ide-sdk`
with full `devtool` and `bitbake` integration might not be suitable.
To offer full feature parity with the SDK and the eSDK, `devtool ide-sdk` has
a second mode called "shared" mode.
If `devtool ide-sdk` is called with the `--mode=shared` option, it
bootstraps an SDK directly from the BitBake environment, which offers the same
Environment Setup script as described in running_the_ext_sdk_env.
In addition to the (e)SDK installer-based setup, the IDE gets configured
to use the shared **Sysroots <Sysroot>** and the tools from the SDK.
`devtool ide-sdk --mode=shared` is basically a wrapper for the setup of the
extensible SDK as described in setting_up_ext_sdk_in_build.

The use of `devtool ide-sdk` is an alternative to using one of the SDK
installers.
`devtool ide-sdk` allows the creation of SDKs that offer all the
functionality of the SDK and the eSDK installers. Compared to the installers,
however, the SDK created with `devtool ide-sdk` is much more flexible.
For example, it is very easy to change the **MACHINE** in the
`local.conf` file, update the layer meta data and then regenerate the SDK.

Let's take a look at an example of how to use `devtool ide-sdk` in each of
the two modes:

#. *Modified mode*:

   In order to use the `devtool ide-sdk`, a few settings are needed. As a
   starting example, the following lines of code can be added to the
   `local.conf` file::

      # Build the companion debug file system
      IMAGE_GEN_DEBUGFS = "1"
      # Optimize build time: with devtool ide-sdk the dbg tar is not needed
      IMAGE_FSTYPES_DEBUGFS = ""
      # Without copying the binaries into roofs-dbg, GDB does not find all source files.
      IMAGE_CLASSES += "image-combined-dbg"

      # SSH is mandatory, no password simplifies the usage
      EXTRA_IMAGE_FEATURES += "\
         ssh-server-openssh \
         allow-empty-password \
         allow-root-login \
         empty-root-password \
      "

      # Remote debugging needs gdbserver on the target device
      IMAGE_INSTALL:append = " gdbserver"

      # Add the recipes which should be modified to the image
      # Otherwise some dependencies might be missing.
      IMAGE_INSTALL:append = " my-recipe"

   Assuming the BitBake environment is set up correctly and a workspace has
   been created for the recipe using `devtool modify my-recipe` or probably
   even better by using `devtool modify my-recipe --debug-build`, the
   following command can create the SDK and the configuration for VSCode in
   the recipe workspace::

      $ devtool ide-sdk my-recipe core-image-minimal --target root@192.168.7.2

   The command requires an image recipe (`core-image-minimal` for this
   example) that is used to create the SDK.
   This firmware image should also be installed on the target device.
   It is possible to pass multiple package recipes::

      $ devtool ide-sdk my-recipe-1 my-recipe-2 core-image-minimal --target root@192.168.7.2

   `devtool ide-sdk` tries to create an IDE configuration for all package
   recipes.

   What this command does exactly depends on the recipe, more precisely on the
   build tool used by the recipe. The basic idea is to configure the IDE so
   that it calls the build tool exactly as `bitbake` does.

   For example, a CMake preset is created for a recipe that inherits
   ref-classes-cmake. In the case of VSCode, CMake presets are supported
   by the CMake Tools plugin. This is an example of how the build configuration
   used by `bitbake` is exported to an IDE configuration that gives exactly
   the same build results.

   Support for remote debugging with seamless integration into the IDE is
   important for a cross-SDK. `devtool ide-sdk` automatically generates the
   necessary helper scripts for deploying the compiled artifacts to the target
   device as well as the necessary configuration for the debugger and the IDE.

   .. note::

      To ensure that the debug symbols on the build machine match the binaries
      running on the target device, it is essential that the image built by
      `devtool ide-sdk` is running on the target device.

   The default IDE is VSCode. Some hints about using VSCode:

   -  VSCode can be used to work on the BitBake recipes or the application
      source code.
      Usually there is one instance of VSCode running in the folder where the
      BitBake recipes are. This instance has the
      `Yocto Project BitBake plugin <https://marketplace.visualstudio.com/items?itemName=yocto-project.yocto-bitbake>`_
      running.

      .. warning::

         Some VSCode plugins (Python, BitBake and others) need a reasonable
         configuration to work as expected. Otherwise, some plugins try to
         index the build directory of BitBake, which keeps your system quite
         busy until an out of memory exception stops this nonsense.
         Other plugins, such as the BitBake plugin, do not behave as expected.

         To work around such issues, the `oe-init-build-env` script creates
         an initial `.vscode/settings.json` file if `code` can be found
         and the `.vscode` folder does not yet exist.
         It is best to run `oe-init-build-env` once before starting VSCode.
         An alternative approach is to use a build folder outside the layers,
         e.g. `oe-init-build-env ../build`.

      The BitBake plugin also offers to create devtool workspaces and run
      `devtool ide-sdk` with a few mouse clicks.
      Of course, issuing commands in the terminal works as well.

   -  To work on the source code of a recipe another instance of VSCode is
      started in the recipe's workspace. Example::

         code build/workspace/sources/my-recipe

      This instance of VSCode uses plugins that are useful for the development
      of the application. `devtool ide-sdk` generates the necessary
      `extensions.json`, `settings.json`, `tasks.json` and `launch.json`
      configuration files for all the involved plugins.

      When the source code folder present in the workspace folder is opened in
      VSCode for the first time, a pop-up message recommends installing the
      required plugins.
      After accepting the installation of the plugins, working with the source
      code or some debugging tasks should work as usual with VSCode.

      Starting the VSCode instances in the recipe workspace folders can also be
      done by a mouse click on the recipe workspaces in the first VSCode
      instance.

   -  To work with CMake press `Ctrl + Shift + p`, type `cmake`. This will
      show some possible commands like selecting a CMake preset, compiling or
      running CTest.

      For recipes inheriting ref-classes-cmake-qemu rather than
      ref-classes-cmake, executing cross-compiled unit tests on the host
      can be supported transparently with QEMU user-mode.

   -  To work with Meson press `Ctrl + Shift + p`, type `meson`. This will
      show some possible commands like compiling or executing the unit tests.

      A note on running cross-compiled unit tests on the host: Meson enables
      support for QEMU user mode by default. It is expected that the execution
      of the unit tests from the IDE will work without any additional steps,
      given that the code is suitable for the execution on the host machine.

   -  For the deployment to the target device, just press `Ctrl + Shift + p`,
      type `task`.  Select `install && deploy-target`.

   -  For remote debugging, switch to the debugging view by pressing the "play"
      button with the `bug icon` on the left side. This will provide a green
      play button with a drop-down list where a debug configuration can be
      selected.  After selecting one of the generated configurations, press the
      "play" button.

      Starting a remote debugging session automatically initiates the
      deployment to the target device. If this is not desired, the
      `"dependsOn": ["install && deploy-target...]` parameter of the tasks
      with `"label": "gdbserver start...` can be removed from the
      `tasks.json` file.

      VSCode supports GDB with many different setups and configurations for
      many different use cases.  However, most of these setups have some
      limitations when it comes to cross-development, support only a few target
      architectures or require a high performance target device. Therefore
      `devtool ide-sdk` supports the classic, generic setup with GDB on the
      development host and gdbserver on the target device.

      Roughly summarized, this means:

      -  The binaries are copied via SSH to the remote target device by a
         script referred by `tasks.json`.

      -  gdbserver is started on the remote target device via SSH by a script
         referred by `tasks.json`.

         Changing the parameters that are passed to the debugging executable
         requires modifying the generated script. The script is located at
         `oe-scripts/gdbserver_*`. Defining the parameters in the `args`
         field in the `launch.json` file does not work.

      -  VSCode connects to gdbserver as documented in
         `Remote debugging or debugging with a local debugger server
         <https://code.visualstudio.com/docs/cpp/launch-json-reference#_remote-debugging-or-debugging-with-a-local-debugger-server>`__.

   Additionally `--ide=none` is supported. With the `none` IDE parameter,
   some generic configuration files like `gdbinit` files and some helper
   scripts starting gdbserver remotely on the target device as well as the GDB
   client on the host are generated.

   Here is a usage example for the `cmake-example` recipe from the
   `meta-selftest` layer which inherits ref-classes-cmake-qemu:

   .. code-block:: sh

      # Create the SDK
      devtool modify cmake-example --debug-build
      devtool ide-sdk cmake-example core-image-minimal -c --ide=none

      # Install the firmware on a target device or start QEMU
      runqemu

      # From exploring the workspace of cmake-example
      cd build/workspace/sources/cmake-example

      # Find cmake-native and save the path into a variable
      # Note: using just cmake instead of $CMAKE_NATIVE would work in many cases
      CMAKE_NATIVE="$(jq -r '.configurePresets[0] | "\(.cmakeExecutable)"' CMakeUserPresets.json)"

      # List available CMake presets
      "$CMAKE_NATIVE" --list-presets
      Available configure presets:

        "cmake-example-cortexa57" - cmake-example: cortexa57

      # Re-compile the already compiled sources
      "$CMAKE_NATIVE" --build --preset cmake-example-cortexa57
      ninja: no work to do.
      # Do a clean re-build
      "$CMAKE_NATIVE" --build --preset cmake-example-cortexa57 --target clean
      [1/1] Cleaning all built files...
      Cleaning... 8 files.
      "$CMAKE_NATIVE" --build --preset cmake-example-cortexa57 --target all
      [7/7] Linking CXX executable cmake-example

      # Run the cross-compiled unit tests with QEMU user-mode
      "$CMAKE_NATIVE" --build --preset cmake-example-cortexa57 --target test
      [0/1] Running tests...
      Test project .../build/tmp/work/cortexa57-poky-linux/cmake-example/1.0/cmake-example-1.0
          Start 1: test-cmake-example
      1/1 Test #1: test-cmake-example ...............   Passed    0.03 sec

      100% tests passed, 0 tests failed out of 1

      Total Test time (real) =   0.03 sec

      # Using CTest directly is possible as well
      CTEST_NATIVE="$(dirname "$CMAKE_NATIVE")/ctest"

      # List available CMake presets
      "$CTEST_NATIVE" --list-presets
      Available test presets:

        "cmake-example-cortexa57" - cmake-example: cortexa57

      # Run the cross-compiled unit tests with QEMU user-mode
      "$CTEST_NATIVE" --preset "cmake-example-cortexa57"
      Test project ...build/tmp/work/cortexa57-poky-linux/cmake-example/1.0/cmake-example-1.0
          Start 1: test-cmake-example
      1/1 Test #1: test-cmake-example ...............   Passed    0.03 sec

      100% tests passed, 0 tests failed out of 1

      Total Test time (real) =   0.03 sec

      # Deploying the new build to the target device (default is QEUM at 192.168.7.2)
      oe-scripts/install_and_deploy_cmake-example-cortexa57

      # Start a remote debugging session with gdbserver on the target and GDB on the host
      oe-scripts/gdbserver_1234_usr-bin-cmake-example_m
      oe-scripts/gdb_1234_usr-bin-cmake-example
      break main
      run
      step
      stepi
      continue
      quit

      # Stop gdbserver on the target device
      oe-scripts/gdbserver_1234_usr-bin-cmake-example_m stop

#. *Shared sysroots mode*

   Creating an SDK with shared **Sysroots <Sysroot>** that contains all the
   dependencies needed to work with `my-recipe` is possible with the following
   example command::

      $ devtool ide-sdk --mode=shared my-recipe

   For VSCode the cross-toolchain is exposed as a CMake kit. CMake kits are
   defined in `~/.local/share/CMakeTools/cmake-tools-kits.json`.
   The following example shows how the cross-toolchain can be selected in
   VSCode. First of all we need a folder containing a CMake project.
   For this example, let's create a CMake project and start VSCode::

      mkdir kit-test
      echo "project(foo VERSION 1.0)" > kit-test/CMakeLists.txt
      code kit-test

   If there is a CMake project in the workspace, cross-compilation is
   supported:

   - Press `Ctrl + Shift + P`, type `CMake: Scan for Kits`
   - Press `Ctrl + Shift + P`, type `CMake: Select a Kit`

   Finally most of the features provided by CMake and the IDE should be
   available.

   Other IDEs than VSCode are supported as well. However,
   `devtool ide-sdk --mode=shared --ide=none my-recipe` is currently
   just a simple wrapper for the setup of the extensible SDK, as described in
   setting_up_ext_sdk_in_build.

## Use ``devtool upgrade`` to Create a Version of the Recipe that Supports a Newer Version of the Software

The `devtool upgrade` command upgrades an existing recipe to that of a
more up-to-date version found upstream. Throughout the life of software,
recipes continually undergo version upgrades by their upstream
publishers. You can use the `devtool upgrade` workflow to make sure
your recipes you are using for builds are up-to-date with their upstream
counterparts.

> **Note:**
> Several methods exist by which you can upgrade recipes ---
> `devtool upgrade` happens to be one. You can read about all the methods by
> which you can upgrade recipes in the
> dev-manual/upgrading-recipes:upgrading recipes section of the Yocto
> Project Development Tasks Manual.
>

The `devtool upgrade` command is flexible enough to allow you to specify
source code revision and versioning schemes, extract code into or out of the
`devtool` devtool-the-workspace-layer-structure, and work with any
source file forms that the
bitbake-user-manual/bitbake-user-manual-fetching:fetchers support.

The following diagram shows the common development flow used with the
`devtool upgrade` command:

![devtool-upgrade-flow.png](figures/devtool-upgrade-flow.png)

#. *Initiate the Upgrade*: The top part of the flow shows the typical
   scenario by which you use the `devtool upgrade` command. The
   following conditions exist:

   -  The recipe exists in a local layer external to the `devtool`
      workspace.

   -  The source files for the new release exist in the same location
      pointed to by **SRC_URI**
      in the recipe (e.g. a tarball with the new version number in the
      name, or as a different revision in the upstream Git repository).

   A common situation is where third-party software has undergone a
   revision so that it has been upgraded. The recipe you have access to
   is likely in your own layer. Thus, you need to upgrade the recipe to
   use the newer version of the software::

      $ devtool upgrade -V version recipe

   By default, the `devtool upgrade` command extracts source
   code into the `sources` directory in the
   devtool-the-workspace-layer-structure.
   If you want the code extracted to any other location, you need to
   provide the `srctree` positional argument with the command as follows::

      $ devtool upgrade -V version recipe srctree

   .. note::

      In this example, the "-V" option specifies the new version. If you
      don't use "-V", the command upgrades the recipe to the latest
      version.

   If the source files pointed to by the **SRC_URI** statement in the
   recipe are in a Git repository, you must provide the "-S" option and
   specify a revision for the software.

   Once `devtool` locates the recipe, it uses the **SRC_URI** variable
   to locate the source code and any local patch files from other
   developers. The result is that the command sets up the source code,
   the new version of the recipe, and an append file all within the
   workspace.

   Additionally, if you have any non-patch local files (i.e. files
   referred to with `file://` entries in **SRC_URI** statement
   excluding `*.patch/` or `*.diff`), these files are copied to an
   `oe-local-files` folder under the newly created source tree.
   Copying the files here gives you a convenient area from which you can
   modify the files. Any changes or additions you make to those files
   are incorporated into the build the next time you build the software
   just as are other changes you might have made to the source.

#. *Resolve any Conflicts created by the Upgrade*: Conflicts could happen
   after upgrading the software to a new version. Conflicts occur
   if your recipe specifies some patch files in **SRC_URI** that
   conflict with changes made in the new version of the software. For
   such cases, you need to resolve the conflicts by editing the source
   and following the normal `git rebase` conflict resolution process.

   Before moving onto the next step, be sure to resolve any such
   conflicts created through use of a newer or different version of the
   software.

#. *Build the Recipe or Rebuild the Image*: The next step you take
   depends on what you are going to do with the new code.

   If you need to eventually move the build output to the target
   hardware, use the following `devtool` command::

      $ devtool build recipe

   On the other hand, if you want an image to contain the recipe's
   packages from the workspace for immediate deployment onto a device
   (e.g. for testing purposes), you can use the `devtool build-image`
   command::

      $ devtool build-image image

#. *Deploy the Build Output*: When you use the `devtool build` command
   or `bitbake` to build your recipe, you probably want to see if the
   resulting build output works as expected on target hardware.

   .. note::

      This step assumes you have a previously built image that is
      already either running in QEMU or running on actual hardware.
      Also, it is assumed that for deployment of the image to the
      target, SSH is installed in the image and if the image is running
      on real hardware that you have network access to and from your
      development machine.

   You can deploy your build output to that target hardware by using the
   `devtool deploy-target` command::

      $ devtool deploy-target recipe target

   The target is a live target machine running as an SSH server.

   You can, of course, also deploy the image you build using the
   `devtool build-image` command to actual hardware. However,
   `devtool` does not provide a specific command that allows you to do
   this.

#. *Finish Your Work With the Recipe*: The `devtool finish` command
   creates any patches corresponding to commits in the local Git
   repository, moves the new recipe to a more permanent layer, and then
   resets the recipe so that the recipe is built normally rather than
   from the workspace.

   Any work you did in the `oe-local-files` directory is preserved in
   the original files next to the recipe during the `devtool finish`
   command.

   If you specify a destination layer that is the same as the original
   source, then the old version of the recipe and associated files are
   removed prior to adding the new version::

      $ devtool finish recipe layer

   .. note::

      Any changes you want to turn into patches must be committed to the
      Git repository in the source tree.

   As a final process of the `devtool finish` command, the state of
   the standard layers and the upstream source is restored so that you
   can build the recipe from those areas rather than the workspace.

   .. note::

      You can use the `devtool reset` command to put things back should you
      decide you do not want to proceed with your work. If you do use this
      command, realize that the source tree is preserved.

## A Closer Look at ``devtool add``

The `devtool add` command automatically creates a recipe based on the
source tree you provide with the command. Currently, the command has
support for the following:

-  Autotools (`autoconf` and `automake`)

-  CMake

-  Scons

-  `qmake`

-  Plain `Makefile`

-  Out-of-tree kernel module

-  Binary package (i.e. "-b" option)

-  Node.js module

-  Python modules that use `setuptools` or `distutils`

Apart from binary packages, the determination of how a source tree
should be treated is automatic based on the files present within that
source tree. For example, if a `CMakeLists.txt` file is found, then
the source tree is assumed to be using CMake and is treated accordingly.

> **Note:**
> In most cases, you need to edit the automatically generated recipe in
> order to make it build properly. Typically, you would go through
> several edit and build cycles until the recipe successfully builds.
> Once the recipe builds, you could use possible further iterations to
> test the recipe on the target device.
>

The remainder of this section covers specifics regarding how parts of
the recipe are generated.

### Name and Version

If you do not specify a name and version on the command line,
`devtool add` uses various metadata within the source tree in an
attempt to determine the name and version of the software being built.
Based on what the tool determines, `devtool` sets the name of the
created recipe file accordingly.

If `devtool` cannot determine the name and version, the command prints
an error. For such cases, you must re-run the command and provide the
name and version, just the name, or just the version as part of the
command line.

Sometimes the name or version determined from the source tree might be
incorrect. For such a case, you must reset the recipe::

   $ devtool reset -n recipename

After running the `devtool reset` command, you need to
run `devtool add` again and provide the name or the version.

### Dependency Detection and Mapping

The `devtool add` command attempts to detect build-time dependencies and map
them to other recipes in the system. During this mapping, the command fills in
the names of those recipes as part of the **DEPENDS** variable within the
recipe. If a dependency cannot be mapped, `devtool` places a comment
in the recipe indicating such. The inability to map a dependency can
result from naming not being recognized or because the dependency simply
is not available. For cases where the dependency is not available, you
must use the `devtool add` command to add an additional recipe that
satisfies the dependency. Once you add that recipe, you need to update
the **DEPENDS** variable in the original recipe to include the new
recipe.

If you need to add runtime dependencies, you can do so by adding the
following to your recipe::

   RDEPENDS:${PN} += "dependency1 dependency2 ..."

> **Note:**
> The `devtool add` command often cannot distinguish between mandatory and
> optional dependencies. Consequently, some of the detected dependencies might
> in fact be optional. When in doubt, consult the documentation or the
> configure script for the software the recipe is building for further
> details. In some cases, you might find you can substitute the
> dependency with an option that disables the associated functionality
> passed to the configure script.
>

### License Detection

The `devtool add` command attempts to determine if the software you are
adding is able to be distributed under a common, open-source license. If
so, the command sets the **LICENSE** value accordingly.
You should double-check the value added by the command against the
documentation or source files for the software you are building and, if
necessary, update that **LICENSE** value.

The `devtool add` command also sets the **LIC_FILES_CHKSUM**
value to point to all files that appear to be license-related. Realize
that license statements often appear in comments at the top of source
files or within the documentation. In such cases, the command does not
recognize those license statements. Consequently, you might need to
amend the **LIC_FILES_CHKSUM** variable to point to one or more of those
comments if present. Setting **LIC_FILES_CHKSUM** is particularly
important for third-party software. The mechanism attempts to ensure
correct licensing should you upgrade the recipe to a newer upstream
version in future. Any change in licensing is detected and you receive
an error prompting you to check the license text again.

If the `devtool add` command cannot determine licensing information,
`devtool` sets the **LICENSE** value to "CLOSED" and leaves the
**LIC_FILES_CHKSUM** value unset. This behavior allows you to continue
with development even though the settings are unlikely to be correct in
all cases. You should check the documentation or source files for the
software you are building to determine the actual license.

### Adding Makefile-Only Software

The use of Make by itself is very common in both proprietary and
open-source software. Unfortunately, Makefiles are often not written
with cross-compilation in mind. Thus, `devtool add` often cannot do
very much to ensure that these Makefiles build correctly. It is very
common, for example, to explicitly call `gcc` instead of using the
**CC** variable. Usually, in a
cross-compilation environment, `gcc` is the compiler for the build
host and the cross-compiler is named something similar to
`arm-poky-linux-gnueabi-gcc` and might require arguments (e.g. to
point to the associated sysroot for the target machine).

When writing a recipe for Makefile-only software, keep the following in
mind:

-  You probably need to patch the Makefile to use variables instead of
   hardcoding tools within the toolchain such as `gcc` and `g++`.

-  The environment in which Make runs is set up with various standard
   variables for compilation (e.g. **CC**, **CXX**, and so forth) in a
   similar manner to the environment set up by an :ref:`SDK
   <overview-manual/concepts:Application Development SDK>`'s environment
   setup script. One easy way to see these variables is to run the
   `devtool build` command on the recipe and then look in
   `oe-logs/run.do_compile`. Towards the top of this file, there is
   a list of environment variables that are set. You can take
   advantage of these variables within the Makefile.

-  If the Makefile sets a default for a variable using "=", that default
   overrides the value set in the environment, which is usually not
   desirable. For this case, you can either patch the Makefile so it
   sets the default using the "?=" operator, or you can alternatively
   force the value on the `make` command line. To force the value on
   the command line, add the variable setting to
   **EXTRA_OEMAKE** or
   **PACKAGECONFIG_CONFARGS**
   within the recipe. Here is an example using **EXTRA_OEMAKE**::

      EXTRA_OEMAKE += "'CC=${CC}' 'CXX=${CXX}'"

   In the above example,
   single quotes are used around the variable settings as the values are
   likely to contain spaces because required default options are passed
   to the compiler.

-  Hardcoding paths inside Makefiles is often problematic in a
   cross-compilation environment. This is particularly true because
   those hardcoded paths often point to locations on the build host and
   thus will either be read-only or will introduce contamination into
   the cross-compilation because they are specific to the build host
   rather than the target. Patching the Makefile to use prefix variables
   or other path variables is usually the way to handle this situation.

-  Sometimes a Makefile runs target-specific commands such as
   `ldconfig`. For such cases, you might be able to apply patches that
   remove these commands from the Makefile.

### Adding Native Tools

Often, you need to build additional tools that run on the **Build Host**
as opposed to the target. You should indicate this requirement by using one of
the following methods when you run `devtool add`:

-  Specify the name of the recipe such that it ends with "-native".
   Specifying the name like this produces a recipe that only builds for
   the build host.

-  Specify the "--also-native" option with the `devtool add`
   command. Specifying this option creates a recipe file that still
   builds for the target but also creates a variant with a "-native"
   suffix that builds for the build host.

> **Note:**
> If you need to add a tool that is shipped as part of a source tree
> that builds code for the target, you can typically accomplish this by
> building the native and target parts separately rather than within
> the same compilation process. Realize though that with the
> "--also-native" option, you can add the tool using just one
> recipe file.
>

### Adding Node.js Modules

> **Warning:**
> The NPM fetcher is currently disabled due to security concerns. See
> `355cd226e0720a9ed7683bb01c8c0a58eee03664` for more information.
>

You can use the `devtool add` command two different ways to add
Node.js modules: through `npm` or from a repository or local source.

Use the following form to add Node.js modules through `npm`::

   $ devtool add "npm://registry.npmjs.org;name=forever;version=0.15.1"

The name and
version parameters are mandatory. Lockdown and shrinkwrap files are
generated and pointed to by the recipe in order to freeze the version
that is fetched for the dependencies according to the first time. This
also saves checksums that are verified on future fetches. Together,
these behaviors ensure the reproducibility and integrity of the build.

> **Note:**
> -  You must use quotes around the URL. `devtool add` does not
> require the quotes, but the shell considers ";" as a splitter
> between multiple commands. Thus, without the quotes,
> `devtool add` does not receive the other parts, which results in
> several "command not found" errors.
>

As mentioned earlier, you can also add Node.js modules directly from a
repository or local source tree. To add modules this way, use
`devtool add` in the following form::

   $ devtool add https://github.com/diversario/node-ssdp

In this example, `devtool` fetches the specified Git repository, detects the
code as Node.js code, fetches dependencies using `npm`, and sets
**SRC_URI** accordingly.

## Working With Recipes

When building a recipe using the `devtool build` command, the typical
build progresses as follows:

#. Fetch the source

#. Unpack the source

#. Configure the source

#. Compile the source

#. Install the build output

#. Package the installed output

For recipes in the workspace, fetching and unpacking is disabled as the
source tree has already been prepared and is persistent. Each of these
build steps is defined as a function (task), usually with a "do\_" prefix
(e.g. ref-tasks-fetch,
ref-tasks-unpack, and so
forth). These functions are typically shell scripts but can instead be
written in Python.

If you look at the contents of a recipe, you will see that the recipe
does not include complete instructions for building the software.
Instead, common functionality is encapsulated in classes inherited with
the `inherit` directive. This technique leaves the recipe to describe
just the things that are specific to the software being built. There is
a ref-classes-base class that is implicitly inherited by all recipes
and provides the functionality that most recipes typically need.

The remainder of this section presents information useful when working
with recipes.

### Finding Logs and Work Files

After the first run of the `devtool build` command, recipes that were
previously created using the `devtool add` command or whose sources
were modified using the `devtool modify` command contain symbolic
links created within the source tree:

-  `oe-logs`: This link points to the directory in which log files and
   run scripts for each build step are created.

-  `oe-workdir`: This link points to the temporary work area for the
   recipe. The following locations under `oe-workdir` are particularly
   useful:

   -  `image/`: Contains all of the files installed during the
      ref-tasks-install stage.
      Within a recipe, this directory is referred to by the expression
      `${`\ **D**\ `}`.

   -  `sysroot-destdir/`: Contains a subset of files installed within
      ref-tasks-install that have been put into the shared sysroot. For
      more information, see the
      "dev-manual/new-recipe:sharing files between recipes" section.

   -  `packages-split/`: Contains subdirectories for each package
      produced by the recipe. For more information, see the
      "dev-manual/devtool:packaging" section.

You can use these links to get more information on what is happening at
each build step.

### Setting Configure Arguments

If the software your recipe is building uses GNU autoconf, then a fixed
set of arguments is passed to it to enable cross-compilation plus any
extras specified by **EXTRA_OECONF** or **PACKAGECONFIG_CONFARGS**
set within the recipe. If you wish to pass additional options, add them
to **EXTRA_OECONF** or **PACKAGECONFIG_CONFARGS**. Other supported build
tools have similar variables (e.g.  **EXTRA_OECMAKE** for CMake,
**EXTRA_OESCONS** for Scons, and so forth). If you need to pass anything on
the `make` command line, you can use **EXTRA_OEMAKE** or the
**PACKAGECONFIG_CONFARGS** variables to do so.

You can use the `devtool configure-help` command to help you set the
arguments listed in the previous paragraph. The command determines the
exact options being passed, and shows them to you along with any custom
arguments specified through **EXTRA_OECONF** or
**PACKAGECONFIG_CONFARGS**. If applicable, the command also shows you
the output of the configure script's "--help" option as a
reference.

### Sharing Files Between Recipes

Recipes often need to use files provided by other recipes on the
**Build Host**. For example,
an application linking to a common library needs access to the library
itself and its associated headers. The way this access is accomplished
is through the **Sysroot**. There is a sysroot for the target machine, and a
sysroot for the build host.

Recipes should never write files directly into the sysroot. Instead,
files should be installed into standard locations during the
ref-tasks-install task within the `${`\ **D**\ `}` directory. A
subset of these files automatically goes into the sysroot. The reason
for this limitation is that almost all files that go into the sysroot
are cataloged in manifests in order to ensure they can be removed later
when a recipe is modified or removed. Thus, the sysroot is able to
remain free from stale files.

### Packaging

If you examine how build output gets into the final image on
the target device, it is important to understand packaging because the
contents of the image are expressed in terms of packages and not
recipes.

During the ref-tasks-package task, files installed during the
ref-tasks-install task are split into one main package, which is almost
always named the same as the recipe, and into several other packages. This
separation exists because not all of those installed files are useful in every
image. For example, you probably do not need any of the documentation installed
in a production image. Consequently, for each recipe the documentation
files are separated into a `-doc` package. Recipes that package
software containing optional modules or plugins might undergo additional
package splitting as well.

After building a recipe, you can see where files have gone by looking in
the `oe-workdir/packages-split` directory, which contains a
subdirectory for each package. Apart from some advanced cases, the
**PACKAGES** and **FILES** variables controls
splitting. The **PACKAGES** variable lists all of the packages to be
produced, while the **FILES** variable specifies which files to include
in each package by using an override to specify the package. For
example, `FILES:${PN}` specifies the files to go into the main package
(i.e. the main package has the same name as the recipe and
`${`\ **PN**\ `}` evaluates to the
recipe name). The order of the **PACKAGES** value is significant. For
each installed file, the first package whose **FILES** value matches the
file is the package into which the file goes. Both the **PACKAGES** and
**FILES** variables have default values. Consequently, you might find
you do not even need to set these variables in your recipe unless the
software the recipe is building installs files into non-standard
locations.

## Restoring the Target Device to its Original State

If you use the `devtool deploy-target` command to write a recipe's
build output to the target, and you are working on an existing component
of the system, then you might find yourself in a situation where you
need to restore the original files that existed prior to running the
`devtool deploy-target` command. Because the `devtool deploy-target`
command backs up any files it overwrites, you can use the
`devtool undeploy-target` command to restore those files and remove
any other files the recipe deployed. Consider the following example::

   $ devtool undeploy-target lighttpd root@192.168.7.2

If you have deployed
multiple applications, you can remove them all using the "-a" option
thus restoring the target device to its original state::

   $ devtool undeploy-target -a root@192.168.7.2

Information about files deployed to
the target as well as any backed up files are stored on the target
itself. This storage, of course, requires some additional space on the
target machine.

> **Note:**
> The `devtool deploy-target` and `devtool undeploy-target` commands do
> not currently interact with any package management system on the target
> device (e.g. RPM or OPKG). Consequently, you should not intermingle
> `devtool deploy-target` and package manager operations on the target
> device. Doing so could result in a conflicting set of files.
>
>

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Building Images for Multiple Targets With Multiconfig

You can use a single `bitbake` command to build multiple images or
packages for different targets where each image or package requires a
different configuration (multiple configuration builds). The builds, in
this scenario, are sometimes referred to as "multiconfigs", and this
section uses that term throughout.

This section describes how to set up for multiple configuration builds
and how to account for cross-build dependencies between the
multiconfigs.

## Setting Up and Running a Multiple Configuration Build

To accomplish a multiple configuration build, you must define each
target's configuration separately using a parallel **configuration file** in
the **Build Directory** or configuration directory within a layer, and you
must follow a required file hierarchy. Additionally, you must enable the
multiple configuration builds in your `local.conf` file.

Follow these steps to set up and execute multiple configuration builds:

-  *Create Separate Configuration Files*: You need to create a single
   **Configuration File** for each build target (each multiconfig).
   The configuration definitions are implementation dependent but often
   each configuration file will define the **MACHINE** and the
   temporary directory (**TMPDIR**) BitBake uses for the build.

   .. note::

      Whether the same temporary directory (**TMPDIR**) can be shared will
      depend on what is similar and what is different between the
      configurations. Multiple **MACHINE** targets can share the same
      **TMPDIR** as long as the rest of the configuration is the same,
      multiple **DISTRO** settings would need separate **TMPDIR**
      directories.

      For example, consider a scenario with two different multiconfigs for the same
      **MACHINE**: "qemux86" built for two distributions such as "poky" and
      "poky-lsb". In this case, you would need to use two different **TMPDIR**.

      In the general case, using separate **TMPDIR** for the different
      multiconfigs is strongly recommended.

   The location for these multiconfig configuration files is specific.
   They must reside in the current **Build Directory** in a sub-directory of
   `conf` named `multiconfig` or within a **Layer**'s `conf` directory
   under a directory named `multiconfig`. Here is an example that defines
   two configuration files for the "x86" and "arm" multiconfigs:

   .. image:: figures/multiconfig_files.png
      :align: center
      :width: 50%

   The usual **BBPATH** search path is used to locate multiconfig files in
   a similar way to other configuration files.

   Here is an example showing the minimal statements needed in a
   **configuration file** named `qemux86.conf` for a `qemux86` target
   whose temporary build directory is `tmp-qemux86`::

      MACHINE = "qemux86"
      TMPDIR .= "-${BB_CURRENT_MC}"

   BitBake will expand the **BB_CURRENT_MC** variable to the value of the
   current multiconfig in use. We append this value to **TMPDIR** so that
   any change on the definition of **TMPDIR** will automatically affect the
   value of **TMPDIR** for each multiconfig.

-  *Add the BitBake Multi-configuration Variable to the Local
   Configuration File*: Use the
   **BBMULTICONFIG**
   variable in your `conf/local.conf` configuration file to specify
   each multiconfig. Continuing with the example from the previous
   figure, the **BBMULTICONFIG** variable needs to enable two
   multiconfigs: "x86" and "arm" by specifying each configuration file::

      BBMULTICONFIG = "x86 arm"

   .. note::

      A "default" configuration already exists by definition. This
      configuration is named: "" (i.e. empty string) and is defined by
      the variables coming from your `local.conf`
      file. Consequently, the previous example actually adds two
      additional configurations to your build: "arm" and "x86" along
      with "".

-  *Launch BitBake*: Use the following BitBake command form to launch
   the multiple configuration build::

      $ bitbake [mc:multiconfigname:]target [[[mc:multiconfigname:]target] ... ]

   For the example in this section, the following command applies::

      $ bitbake mc:x86:core-image-minimal mc:arm:core-image-sato mc::core-image-base

   The previous BitBake command builds several components:

   -  A `core-image-minimal` image that is configured through the `x86.conf`
      configuration file

   -  A `core-image-sato` image that is configured through the `arm.conf`
      configuration file

   -  A `core-image-base` that is configured through your `local.conf`
      configuration file

> **Note:**
> Support for multiple configuration builds in the Yocto Project &DISTRO;
> (&DISTRO_NAME;) Release does not include Shared State (sstate)
> optimizations. Consequently, if a build uses the same object twice
> in, for example, two different **TMPDIR**
> directories, the build either loads from an existing sstate cache for
> that build at the start or builds the object fresh.
>

## Enabling Multiple Configuration Build Dependencies

Sometimes dependencies can exist between targets (multiconfigs) in a
multiple configuration build. For example, suppose that in order to
build a `core-image-sato` image for an "x86" multiconfig, the root
filesystem of an "arm" multiconfig must exist. This dependency is
essentially that the
ref-tasks-image task in the
`core-image-sato` recipe depends on the completion of the
ref-tasks-rootfs task of the
`core-image-minimal` recipe.

To enable dependencies in a multiple configuration build, you must
declare the dependencies in the recipe using the following statement
form::

   task_or_package[mcdepends] = "mc:from_multiconfig:to_multiconfig:recipe_name:task_on_which_to_depend"

To better show how to use this statement, consider the example scenario
from the first paragraph of this section. The following statement needs
to be added to the recipe that builds the `core-image-sato` image::

   do_image[mcdepends] = "mc:x86:arm:core-image-minimal:do_rootfs"

In this example, the `from_multiconfig` is "x86". The `to_multiconfig` is "arm". The
task on which the ref-tasks-image task in the recipe depends is the
ref-tasks-rootfs task from the `core-image-minimal` recipe associated
with the "arm" multiconfig.

Once you set up this dependency, you can build the "x86" multiconfig
using a BitBake command as follows::

   $ bitbake mc:x86:core-image-sato

This command executes all the tasks needed to create the
`core-image-sato` image for the "x86" multiconfig. Because of the
dependency, BitBake also executes through the ref-tasks-rootfs task for the
"arm" multiconfig build.

Having a recipe depend on the root filesystem of another build might not
seem that useful. Consider this change to the statement in the
`core-image-sato` recipe::

   do_image[mcdepends] = "mc:x86:arm:core-image-minimal:do_image"

In this case, BitBake must
create the `core-image-minimal` image for the "arm" build since the
"x86" build depends on it.

Because "x86" and "arm" are enabled for multiple configuration builds
and have separate configuration files, BitBake places the artifacts for
each build in the respective temporary build directories (i.e.
**TMPDIR**).

## Suggested best practices

-  **TMPDIR** (other than the default set in bitbake.conf) is only set in
   `local.conf` by the user. This means that we should **not** manipulate
   **TMPDIR** in any way within the Machine or Distro :term:`configuration
   file`.

-  A multiconfig should specify a **TMPDIR**, and should specify it by
   appending the multiconfig name with **BB_CURRENT_MC**.

-  Recipes that are used to transfer the output from a multiconfig build to
   another should use `do_task[mcdepends]` to trigger the build of the
   component, and then transfer the item to the current configuration in
   ref-tasks-install and ref-tasks-deploy, assuming the value of
   the deployed item based on **TMPDIR**.

   The ref-tasks-install and ref-tasks-deploy tasks should look
   like this::

      do_install() {
          install -m 0644 ${TMPDIR}-<multiconfig>/tmp/deploy/images/<machine>/somefile ${D}/some/path
      }

      do_deploy() {
          install -m 0644 ${TMPDIR}-<multiconfig>/tmp/deploy/images/<machine>/somefile ${DEPLOYDIR}/somefile
      }

   In the example above:

   -  `<multiconfig>` is the multiconfig name as set by the multiconfig
      **configuration file** (see the :ref:`dev-manual/multiconfig:Setting
      Up and Running a Multiple Configuration Build` section above).

   -  `<machine>` must be the **MACHINE** for which `somefile` was built
      and deployed. This value may differ from the current **MACHINE** if
      the multiconfig **configuration file** overrides it.

-  Firmware recipes can set the **INHIBIT_DEFAULT_DEPS** variable to `1`
   if they don't rely on default dependencies such as the standard C library.

## Common use case: building baremetal firmware alongside a Linux build

A common use case for multiconfig is to use the default configuration as the
regular Linux build, while one or more multiconfigs can be used to build special
components, such as baremetal firmware. It would also apply to a scenario where
a microcontroller, for example, is companion to a main processor where Linux is
running. This section details how one can achieve these kinds of scenarios with
a multiconfig build.

### Adding a multiconfig configuration file and recipe for a baremetal firmware

As described in :ref:`dev-manual/multiconfig:Setting Up and Running a Multiple
Configuration Build`, each multiconfig will require a separate
**Configuration File**. In addition, we will define a separate
**TMPDIR** for our baremetal firmware build configuration.

For example, we will define a new `conf/multiconfig/baremetal-firmware.conf`
as follows::

   TMPDIR .= "-${BB_CURRENT_MC}"
   TCLIBC = "newlib"

The `baremetal-firmware.conf` file configures a separate **TMPDIR** for
holding binaries compiled with the `newlib <https://sourceware.org/newlib/>`__
toolchain (see **TCLIBC**).

> **Note:**
> Here, the default **MACHINE** is not overridden by the multiconfig
> configuration file. As a consequence, the architecture of the built baremetal
> binaries will be the same. In other cases, where the firmware runs on a
> completely different architecture, the **MACHINE** must be overridden.
>

We then create a recipe `my-firmware.bb` that defines how the baremetal
firmware is built. The recipe should contain enough information for the
**OpenEmbedded build system** to properly compile the firmware with our
toolchain. The building tasks may vary depending on the nature of the firmware.
However, the recipe should define a ref-classes-deploy task that deploys
the output into the **DEPLOYDIR** directory. We will consider in the
following that the file is named `my-firmware.elf`.

### Building the firmware

The firmware can be built with BitBake with the following command::

   $ bitbake mc:baremetal-firmware:my-firmware

However, we would prefer for `my-firmware` to be automatically built when
triggering a normal Linux build.

Using a `mcdepend`, a recipe belonging to the Linux build can trigger the
build of `my-firmware`. For example, let's consider that our Linux build needs
to assemble a "special" firmware that uses the output of our `my-firmware`
recipe - let's call it `my-parent-firmware.bb`. Then, we should specify this
dependency in `my-parent-firmware.bb` with::

   do_compile[mcdepends] = "mc::baremetal-firmware:my-firmware:do_deploy"

The above will ensure that when the ref-tasks-compile task of
`my-parent-firmware` is triggered, the ref-tasks-deploy task of
`my-firmware` will already have run successfully.

### Using the output of ``my-firmware``

After `my-firmware` recipe has deployed `my-firmware.elf`, we need to use
the output in some way. We can make a series of assumptions, based on the
default Yocto Project variables in order to get the binary for packaging.

First, we can set the following in `my-parent-firmware.bb`::

   FIRMWARE_FILE ??= "${TMPDIR}-baremetal-firmware/deploy/images/<machine>/my-firmware.elf"
   FIRMWARE_FILE[vardepsexclude] += "TMPDIR"

The first assignment stores the value of the path to the firmware built and
deployed by the `my-firmware.bb` recipe. The second assignment excludes the
**TMPDIR** variable from being part of `FIRMWARE_FILE`'s dependencies ---
meaning that changing the value of **TMPDIR** (for example, changing the
host on which the firmware is built) will not invalidate the :ref:`shared state
cache <overview-manual/concepts:shared state cache>`.

Additionally, `<machine>` should be replaced by the **MACHINE** for which
we are building in the baremetal-firmware context.

We can then add a ref-tasks-install task to `my-parent-firmware`::

   do_install() {
       install -Dm 0644 ${FIRMWARE_FILE} ${D}/lib/firmware/my-firmware.elf
   }

Doing the above will allow the firmware binary to be transferred and packaged
into the Linux context and rootfs.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Using an External SCM

If you're working on a recipe that pulls from an external Source Code
Manager (SCM), it is possible to have the OpenEmbedded build system
notice new recipe changes added to the SCM and then build the resulting
packages that depend on the new recipes by using the latest versions.
This only works for SCMs from which it is possible to get a sensible
revision number for changes. Currently, you can do this with Apache
Subversion (SVN) and Git.

To enable this behavior, the **PV** of
the recipe needs to include a `+` sign in its assignment.
Here is an example::

   PV = "1.2.3+git"

**Bitbake** later includes the source control information in **PKGV**
during the packaging phase.

Then, you can add the following to your
`local.conf`::

   SRCREV:pn-PN = "${AUTOREV}"

**PN** is the name of the recipe for
which you want to enable automatic source revision updating.

If you do not want to update your local configuration file, you can add
the following directly to the recipe to finish enabling the feature::

   SRCREV = "${AUTOREV}"

The Yocto Project provides a distribution named `poky-bleeding`, whose
configuration file contains the line::

   require conf/distro/include/poky-floating-revisions.inc

This line pulls in the
listed include file that contains numerous lines of exactly that form::

   #SRCREV:pn-opkg-native ?= "${AUTOREV}"
   #SRCREV:pn-opkg-sdk ?= "${AUTOREV}"
   #SRCREV:pn-opkg ?= "${AUTOREV}"
   #SRCREV:pn-opkg-utils-native ?= "${AUTOREV}"
   #SRCREV:pn-opkg-utils ?= "${AUTOREV}"
   SRCREV:pn-gconf-dbus ?= "${AUTOREV}"
   SRCREV:pn-matchbox-common ?= "${AUTOREV}"
   SRCREV:pn-matchbox-config-gtk ?= "${AUTOREV}"
   SRCREV:pn-matchbox-desktop ?= "${AUTOREV}"
   SRCREV:pn-matchbox-keyboard ?= "${AUTOREV}"
   SRCREV:pn-matchbox-panel-2 ?= "${AUTOREV}"
   SRCREV:pn-matchbox-themes-extra ?= "${AUTOREV}"
   SRCREV:pn-matchbox-terminal ?= "${AUTOREV}"
   SRCREV:pn-matchbox-wm ?= "${AUTOREV}"
   SRCREV:pn-settings-daemon ?= "${AUTOREV}"
   SRCREV:pn-screenshot ?= "${AUTOREV}"
   . . .

These lines allow you to
experiment with building a distribution that tracks the latest
development source for numerous packages.

> **Note:**
> The `poky-bleeding` distribution is not tested on a regular basis. Keep
> this in mind if you use it.
>
>
>

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Optionally Using an External Toolchain

You might want to use an external toolchain as part of your development.
If this is the case, the fundamental steps you need to accomplish are as
follows:

-  Understand where the installed toolchain resides. For cases where you
   need to build the external toolchain, you would need to take separate
   steps to build and install the toolchain.

-  Make sure you add the layer that contains the toolchain to your
   `bblayers.conf` file through the
   **BBLAYERS** variable.

-  Set the **EXTERNAL_TOOLCHAIN** variable in your `local.conf` file
   to the location in which you installed the toolchain.

The toolchain configuration is very flexible and customizable. It
is primarily controlled with the **TCMODE** variable. This variable
controls which `tcmode-*.inc` file to include from the
`meta/conf/distro/include` directory within **OpenEmbedded-Core (OE-Core)**.

The default value of **TCMODE** is "default", which tells the
OpenEmbedded build system to use its internally built toolchain (i.e.
`tcmode-default.inc`). However, other patterns are accepted. In
particular, "external-\*" refers to external toolchains. One example is
the Mentor Graphics Sourcery G++ Toolchain. Support for this toolchain resides
in the separate `meta-sourcery` layer at
https://github.com/MentorEmbedded/meta-sourcery/.
See its `README` file for details about how to use this layer.

Another example of external toolchain layer is
meta-arm-toolchain 
supporting GNU toolchains released by ARM.

You can find further information by reading about the **TCMODE** variable
in the Yocto Project Reference Manual's variable glossary.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Selecting an Initialization Manager

By default, the Yocto Project uses SysVinit  as
the initialization manager. There is also support for BusyBox init, a simpler
implementation, as well as support for systemd , which
is a full replacement for init with parallel starting of services, reduced
shell overhead, increased security and resource limits for services, and other
features that are used by many distributions.

Within the system, SysVinit and BusyBox init treat system components as
services. These services are maintained as shell scripts stored in the
`/etc/init.d/` directory.

SysVinit is more elaborate than BusyBox init and organizes services in
different run levels. This organization is maintained by putting links
to the services in the `/etc/rcN.d/` directories, where `N/` is one
of the following options: "S", "0", "1", "2", "3", "4", "5", or "6".

> **Note:**
> Each runlevel has a dependency on the previous runlevel. This
> dependency allows the services to work properly.
>

Both SysVinit and BusyBox init are configured through the `/etc/inittab`
file, with a very similar syntax, though of course BusyBox init features
are more limited.

In comparison, systemd treats components as units. Using units is a
broader concept as compared to using a service. A unit includes several
different types of entities. `Service` is one of the types of entities.
The runlevel concept in SysVinit corresponds to the concept of a target
in systemd, where target is also a type of supported unit.

In systems with SysVinit or BusyBox init, services load sequentially (i.e. one
by one) during init and parallelization is not supported. With systemd, services
start in parallel. This method can have an impact on the startup performance
of a given service, though systemd will also provide more services by default,
therefore increasing the total system boot time. systemd also substantially
increases system size because of its multiple components and the extra
dependencies it pulls.

By contrast, BusyBox init is the simplest and the lightest solution and
also comes with BusyBox mdev as device manager, a lighter replacement to
udev , which SysVinit and systemd both use.

The "device-manager" chapter has more details about device managers.

## Using SysVinit with udev

SysVinit with the udev device manager corresponds to the
default setting in **Poky**. This corresponds to setting::

   INIT_MANAGER = "sysvinit"

## Using BusyBox init with BusyBox mdev

BusyBox init with BusyBox mdev is the simplest and lightest solution
for small root filesystems. All you need is BusyBox, which most systems
have anyway::

   INIT_MANAGER = "mdev-busybox"

## Using systemd

The last option is to use systemd together with the udev device
manager. This is the most powerful and versatile solution, especially
for more complex systems::

   INIT_MANAGER = "systemd"

This will enable systemd and remove sysvinit components from the image.
See :oe_git:`meta/conf/distro/include/init-manager-systemd.inc
</openembedded-core/tree/meta/conf/distro/include/init-manager-systemd.inc>` for exact
details on what this does.

### Controling systemd from the target command line

Here is a quick reference for controling systemd from the command line on the
target. Instead of opening and sometimes modifying files, most interaction
happens through the `systemctl` and `journalctl` commands:

-  `systemctl status`: show the status of all services
-  `systemctl status <service>`: show the status of one service
-  `systemctl [start|stop] <service>`: start or stop a service
-  `systemctl [enable|disable] <service>`: enable or disable a service at boot time
-  `systemctl list-units`: list all available units
-  `journalctl -a`: show all logs for all services
-  `journalctl -f`: show only the last log entries, and keep printing updates as they arrive
-  `journalctl -u`: show only logs from a particular service

### Using systemd-journald without a traditional syslog daemon

Counter-intuitively, `systemd-journald` is not a syslog runtime or provider,
and the proper way to use `systemd-journald` as your sole logging mechanism is to
effectively disable syslog entirely by setting these variables in your distribution
configuration file::

   VIRTUAL-RUNTIME_syslog = ""
   VIRTUAL-RUNTIME_base-utils-syslog = ""

Doing so will prevent `rsyslog` / `busybox-syslog` from being pulled in by
default, leaving only `systemd-journald`.

### Summary

The Yocto Project supports three different initialization managers, offering
increasing levels of complexity and functionality:

.. list-table::
   :widths: 40 20 20 20
   :header-rows: 1

   * -
     - BusyBox init
     - SysVinit
     - systemd
   * - Size
     - Small
     - Small
     - Big [#footnote-systemd-size]_
   * - Complexity
     - Small
     - Medium
     - High
   * - Support for boot profiles
     - No
     - Yes ("runlevels")
     - Yes ("targets")
   * - Services defined as
     - Shell scripts
     - Shell scripts
     - Description files
   * - Starting services in parallel
     - No
     - No
     - Yes
   * - Setting service resource limits
     - No
     - No
     - Yes
   * - Support service isolation
     - No
     - No
     - Yes
   * - Integrated logging
     - No
     - No
     - Yes

.. [#footnote-systemd-size] Using systemd increases the `core-image-minimal`
   image size by 160\% for `qemux86-64` on Mickledore (4.2), compared to SysVinit.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Selecting a Device Manager

The Yocto Project provides multiple ways to manage the device manager
(`/dev`):

-  Persistent and Pre-Populated `/dev`: For this case, the `/dev`
   directory is persistent and the required device nodes are created
   during the build.

-  Use `devtmpfs` with a Device Manager: For this case, the `/dev`
   directory is provided by the kernel as an in-memory file system and
   is automatically populated by the kernel at runtime. Additional
   configuration of device nodes is done in user space by a device
   manager like `udev` or `busybox-mdev`.

## Using Persistent and Pre-Populated ``/dev``

To use the static method for device population, you need to set the
**USE_DEVFS** variable to "0"
as follows::

   USE_DEVFS = "0"

The content of the resulting `/dev` directory is defined in a Device
Table file. The
**IMAGE_DEVICE_TABLES**
variable defines the Device Table to use and should be set in the
machine or distro configuration file. Alternatively, you can set this
variable in your `local.conf` configuration file.

If you do not define the **IMAGE_DEVICE_TABLES** variable, the default
`device_table-minimal.txt` is used::

   IMAGE_DEVICE_TABLES = "device_table-mymachine.txt"

The population is handled by the `makedevs` utility during image
creation:

## Using ``devtmpfs`` and a Device Manager

To use the dynamic method for device population, you need to use (or be
sure to set) the **USE_DEVFS**
variable to "1", which is the default::

   USE_DEVFS = "1"

With this
setting, the resulting `/dev` directory is populated by the kernel
using `devtmpfs`. Make sure the corresponding kernel configuration
variable `CONFIG_DEVTMPFS` is set when building you build a Linux
kernel.

All devices created by `devtmpfs` will be owned by `root` and have
permissions `0600`.

To have more control over the device nodes, you can use a device manager like
`udev` or `busybox-mdev`. You choose the device manager by defining the
**VIRTUAL-RUNTIME_dev_manager <VIRTUAL-RUNTIME>** variable in your machine
or distro configuration file. Alternatively, you can set this variable in
your `local.conf` configuration file::

   VIRTUAL-RUNTIME_dev_manager = "udev"

   # Some alternative values
   # VIRTUAL-RUNTIME_dev_manager = "busybox-mdev"
   # VIRTUAL-RUNTIME_dev_manager = "systemd"

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Creating Your Own Distribution

When you build an image using the Yocto Project and follow the default setup
showcased in /dev-manual/start, you are using the **Poky** reference
distribution.

Poky is explicitly a *reference* distribution for testing and
development purposes. It enables most hardware and software features
so that they can be tested, but this also means that from a security
point of view the attack surface is very large. Additionally, at some
point it is likely that you will want to gain more control over package
alternative selections, compile-time options, and other low-level
configurations. For both of these reasons, if you are using the Yocto
Project for production use then you are strongly encouraged to create
your own distribution.

To create your own distribution, the basic steps consist of creating
your own distribution layer, creating your own distribution
configuration file, and then adding any needed code and Metadata to the
layer. The following steps provide some more detail:

-  *Create a layer for your new distro:* Create your distribution layer
   so that you can keep your Metadata and code for the distribution
   separate. It is strongly recommended that you create and use your own
   layer for configuration and code. Using your own layer as compared to
   just placing configurations in a `local.conf` configuration file
   makes it easier to reproduce the same build configuration when using
   multiple build machines. See the
   "dev-manual/layers:Creating Your Own Layer"
   section for information on how to quickly set up a layer.

-  *Create the distribution configuration file:* The distribution
   configuration file needs to be created in the `conf/distro`
   directory of your layer. You need to name it using your distribution
   name (e.g. `mydistro.conf`).

   .. note::

      The **DISTRO** variable in your `local.conf` file determines the
      name of your distribution.

   You can split out parts of your configuration file into include files
   and then "require" them from within your distribution configuration
   file. Be sure to place the include files in the
   `conf/distro/include` directory of your layer. A common example
   usage of include files would be to separate out the selection of
   desired version and revisions for individual recipes.

   Your configuration file needs to set the following required
   variables:

   - **DISTRO_NAME**

   - **DISTRO_VERSION**

   These following variables are optional and you typically set them
   from the distribution configuration file:

   - **DISTRO_FEATURES**

   - **DISTRO_EXTRA_RDEPENDS**

   - **DISTRO_EXTRA_RRECOMMENDS**

   - **TCLIBC**

   .. tip::

      If you want to base your distribution configuration file on the
      very basic configuration from OE-Core, you can use
      `conf/distro/defaultsetup.conf` as a reference and just include
      variables that differ as compared to `defaultsetup.conf`.
      Alternatively, you can create a distribution configuration file
      from scratch using the `defaultsetup.conf` file or configuration files
      from another distribution such as Poky as a reference.

-  *Provide miscellaneous variables:* Be sure to define any other
   variables for which you want to create a default or enforce as part
   of the distribution configuration. You can include nearly any
   variable from the `local.conf` file. The variables you use are not
   limited to the list in the previous bulleted item.

-  *Point to Your distribution configuration file:* In your `local.conf`
   file in the **Build Directory**, set your **DISTRO** variable to
   point to your distribution's configuration file. For example, if your
   distribution's configuration file is named `mydistro.conf`, then
   you point to it as follows::

      DISTRO = "mydistro"

-  *Add more to the layer if necessary:* Use your layer to hold other
   information needed for the distribution:

   -  Add recipes for installing distro-specific configuration files
      that are not already installed by another recipe. If you have
      distro-specific configuration files that are included by an
      existing recipe, you should add an append file (`.bbappend`) for
      those. For general information and recommendations on how to add
      recipes to your layer, see the
      "dev-manual/layers:creating your own layer" and
      "dev-manual/layers:following best practices when creating layers"
      sections.

   -  Add any image recipes that are specific to your distribution.

   -  Add a `psplash` append file for a branded splash screen, using
      the **SPLASH_IMAGES** variable.

   -  Add any other append files to make custom changes that are
      specific to individual recipes.

   For information on append files, see the
   "dev-manual/layers:appending other layers metadata with your layer"
   section.

## Copying and modifying the Poky distribution

Instead of creating a custom distribution from scratch as per above, you may
wish to start your custom distribution configuration by copying the Poky
distribution provided within the `meta-poky` layer and then modifying it.
This is fine, however if you do this you should keep the following in mind:

-  Every reference to Poky needs to be updated in your copy so that it
   will still apply. This includes override usage within files (e.g. `:poky`)
   and in directory names. This is a good opportunity to evaluate each one of
   these customizations to see if they are needed for your use case.

-  Unless you also intend to use them, the `poky-tiny`, `poky-altcfg` and
   `poky-bleeding` variants and any references to them can be removed.

-  More generally, the Poky distribution configuration enables a lot more
   than you likely need for your production use case. You should evaluate *every*
   configuration choice made in your copy to determine if it is needed.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Using Wayland and Weston

Wayland 
is a computer display server protocol that provides a method for
compositing window managers to communicate directly with applications
and video hardware and expects them to communicate with input hardware
using other libraries. Using Wayland with supporting targets can result
in better control over graphics frame rendering than an application
might otherwise achieve.

The Yocto Project provides the Wayland protocol libraries and the
reference Weston 
compositor as part of its release. You can find the integrated packages
in the `meta` layer of **OpenEmbedded-Core (OE-Core)**.
Specifically, you
can find the recipes that build both Wayland and Weston at
`meta/recipes-graphics/wayland`.

You can build both the Wayland and Weston packages for use only with targets
that accept the :wikipedia:`Mesa 3D and Direct Rendering Infrastructure
<Mesa_(computer_graphics)>`, which is also known as Mesa DRI. This implies that
you cannot build and use the packages if your target uses, for example, the
Intel Embedded Media and Graphics Driver (Intel EMGD) that overrides Mesa DRI.

> **Note:**
> Due to lack of EGL support, Weston 1.0.3 will not run directly on the
> emulated QEMU hardware. However, this version of Weston will run
> under X emulation without issues.
>

This section describes what you need to do to implement Wayland and use
the Weston compositor when building an image for a supporting target.

## Enabling Wayland in an Image

To enable Wayland, you need to enable it to be built and enable it to be
included (installed) in the image.

### Building Wayland

To cause Mesa to build the `wayland-egl` platform and Weston to build
Wayland with Kernel Mode Setting
(`KMS <https://wiki.archlinux.org/index.php/Kernel_Mode_Setting>`__)
support, include the "wayland" flag in the
**DISTRO_FEATURES**
statement in your `local.conf` file::

   DISTRO_FEATURES:append = " wayland"

> **Note:**
> If X11 has been enabled elsewhere, Weston will build Wayland with X11
> support
>

### Installing Wayland and Weston

To install the Wayland feature into an image, you must include the
following
**CORE_IMAGE_EXTRA_INSTALL**
statement in your `local.conf` file::

   CORE_IMAGE_EXTRA_INSTALL += "wayland weston"

## Running Weston

To run Weston inside X11, enabling it as described earlier and building
a Sato image is sufficient. If you are running your image under Sato, a
Weston Launcher appears in the "Utility" category.

Alternatively, you can run Weston through the command-line interpretor
(CLI), which is better suited for development work. To run Weston under
the CLI, you need to do the following after your image is built:

#. Run these commands to export `XDG_RUNTIME_DIR`::

      mkdir -p /tmp/$USER-weston
      chmod 0700 /tmp/$USER-weston
      export XDG_RUNTIME_DIR=/tmp/$USER-weston

#. Launch Weston in the shell::

      weston

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Enabling GObject Introspection Support

`GObject introspection <https://gi.readthedocs.io/en/latest/>`__
is the standard mechanism for accessing GObject-based software from
runtime environments. GObject is a feature of the GLib library that
provides an object framework for the GNOME desktop and related software.
GObject Introspection adds information to GObject that allows objects
created within it to be represented across different programming
languages. If you want to construct GStreamer pipelines using Python, or
control UPnP infrastructure using Javascript and GUPnP, GObject
introspection is the only way to do it.

This section describes the Yocto Project support for generating and
packaging GObject introspection data. GObject introspection data is a
description of the API provided by libraries built on top of the GLib
framework, and, in particular, that framework's GObject mechanism.
GObject Introspection Repository (GIR) files go to `-dev` packages,
`typelib` files go to main packages as they are packaged together with
libraries that are introspected.

The data is generated when building such a library, by linking the
library with a small executable binary that asks the library to describe
itself, and then executing the binary and processing its output.

Generating this data in a cross-compilation environment is difficult
because the library is produced for the target architecture, but its
code needs to be executed on the build host. This problem is solved with
the OpenEmbedded build system by running the code through QEMU, which
allows precisely that. Unfortunately, QEMU does not always work
perfectly as mentioned in the "dev-manual/gobject-introspection:known issues"
section.

## Enabling the Generation of Introspection Data

Enabling the generation of introspection data (GIR files) in your
library package involves the following:

#. Inherit the ref-classes-gobject-introspection class.

#. Make sure introspection is not disabled anywhere in the recipe or
   from anything the recipe includes. Also, make sure that
   "gobject-introspection-data" is not in
   **DISTRO_FEATURES_BACKFILL_CONSIDERED**
   and that "qemu-usermode" is not in
   **MACHINE_FEATURES_BACKFILL_CONSIDERED**.
   In either of these conditions, nothing will happen.

#. Try to build the recipe. If you encounter build errors that look like
   something is unable to find `.so` libraries, check where these
   libraries are located in the source tree and add the following to the
   recipe::

      GIR_EXTRA_LIBS_PATH = "${B}/something/.libs"

   .. note::

      See recipes in the `oe-core` repository that use that
      **GIR_EXTRA_LIBS_PATH** variable as an example.

#. Look for any other errors, which probably mean that introspection
   support in a package is not entirely standard, and thus breaks down
   in a cross-compilation environment. For such cases, custom-made fixes
   are needed. A good place to ask and receive help in these cases is
   the :ref:`Yocto Project mailing
   lists <resources-mailinglist>`.

> **Note:**
> Using a library that no longer builds against the latest Yocto
> Project release and prints introspection related errors is a good
> candidate for the previous procedure.
>

## Disabling the Generation of Introspection Data

You might find that you do not want to generate introspection data. Or,
perhaps QEMU does not work on your build host and target architecture
combination. If so, you can use either of the following methods to
disable GIR file generations:

-  Add the following to your distro configuration::

      DISTRO_FEATURES_BACKFILL_CONSIDERED = "gobject-introspection-data"

   Adding this statement disables generating introspection data using
   QEMU but will still enable building introspection tools and libraries
   (i.e. building them does not require the use of QEMU).

-  Add the following to your machine configuration::

      MACHINE_FEATURES_BACKFILL_CONSIDERED = "qemu-usermode"

   Adding this statement disables the use of QEMU when building packages for your
   machine. Currently, this feature is used only by introspection
   recipes and has the same effect as the previously described option.

   .. note::

      Future releases of the Yocto Project might have other features
      affected by this option.

If you disable introspection data, you can still obtain it through other
means such as copying the data from a suitable sysroot, or by generating
it on the target hardware. The OpenEmbedded build system does not
currently provide specific support for these techniques.

## Testing that Introspection Works in an Image

Use the following procedure to test if generating introspection data is
working in an image:

#. Make sure that "gobject-introspection-data" is not in
   **DISTRO_FEATURES_BACKFILL_CONSIDERED**
   and that "qemu-usermode" is not in
   **MACHINE_FEATURES_BACKFILL_CONSIDERED**.

#. Build `core-image-sato`.

#. Launch a Terminal and then start Python in the terminal.

#. Enter the following in the terminal::

      >>> from gi.repository import GLib
      >>> GLib.get_host_name()

#. For something a little more advanced, enter the following see:
   https://python-gtk-3-tutorial.readthedocs.io/en/latest/introduction.html

## Known Issues

Here are know issues in GObject Introspection Support:

-  `qemu-ppc64` immediately crashes. Consequently, you cannot build
   introspection data on that architecture.

-  x32 is not supported by QEMU. Consequently, introspection data is
   disabled.

-  musl causes transient GLib binaries to crash on assertion failures.
   Consequently, generating introspection data is disabled.

-  Because QEMU is not able to run the binaries correctly, introspection
   is disabled for some specific packages under specific architectures
   (e.g. `gcr`, `libsecret`, and `webkit`).

-  QEMU usermode might not work properly when running 64-bit binaries
   under 32-bit host machines. In particular, "qemumips64" is known to
   not work under i686.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Using x32 psABI

x32 processor-specific Application Binary Interface (`x32
psABI <https://software.intel.com/en-us/node/628948>`__) is a native
32-bit processor-specific ABI for Intel 64 (x86-64) architectures. An
ABI defines the calling conventions between functions in a processing
environment. The interface determines what registers are used and what
the sizes are for various C data types.

Some processing environments prefer using 32-bit applications even when
running on Intel 64-bit platforms. Consider the i386 psABI, which is a
very old 32-bit ABI for Intel 64-bit platforms. The i386 psABI does not
provide efficient use and access of the Intel 64-bit processor
resources, leaving the system underutilized. Now consider the x86_64
psABI. This ABI is newer and uses 64-bits for data sizes and program
pointers. The extra bits increase the footprint size of the programs,
libraries, and also increases the memory and file system size
requirements. Executing under the x32 psABI enables user programs to
utilize CPU and system resources more efficiently while keeping the
memory footprint of the applications low. Extra bits are used for
registers but not for addressing mechanisms.

The Yocto Project supports the final specifications of x32 psABI as
follows:

-  You can create packages and images in x32 psABI format on x86_64
   architecture targets.

-  You can successfully build recipes with the x32 toolchain.

-  You can create and boot `core-image-minimal` and
   `core-image-sato` images.

-  There is RPM Package Manager (RPM) support for x32 binaries.

-  There is support for large images.

To use the x32 psABI, you need to edit your `conf/local.conf`
configuration file as follows::

   MACHINE = "qemux86-64"
   DEFAULTTUNE = "x86-64-x32"
   baselib = "${@d.getVar('BASE_LIB:tune-' + (d.getVar('DEFAULTTUNE') \
       or 'INVALID')) or 'lib'}"

Once you have set
up your configuration file, use BitBake to build an image that supports
the x32 psABI. Here is an example::

   $ bitbake core-image-sato

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Locking and Unlocking Recipes Using ``bblock``

By design, the OpenEmbedded build system builds everything from scratch
unless BitBake determines that specific tasks do not require rebuilding.
At startup, it computes a signature for all tasks, based on the task's input.
Then, it compares these signatures with the ones from the sstate cache (if they
exist). Any changes cause the task to rerun.

During development, changes might trigger BitBake to rebuild certain
recipes, even when we know they do not require rebuilding at that stage.
For example, modifying a recipe can lead to rebuilding its native
counterpart, which might prove unnecessary. Editing the `python3` recipe,
for instance, can prompt BitBake to rebuild `python3-native` along with any
recipes that depend on it.

To prevent this, use `bblock` to lock specific tasks or recipes to
specific signatures, forcing BitBake to use the sstate cache for them.

> **Warning:**
> Use `bblock` only during the development phase.
>
> Forcing BitBake to use the sstate cache, regardless of input changes, means
> the recipe metadata no longer directly reflect the output. Use this feature
> with caution. If you do not understand why signatures change, see the section
> on understanding what changed .
>
>

### Locking tasks and recipes

To lock a recipe, use::

   $ bblock recipe

You can also use a space-separated list of recipes to lock multiple recipes::

   $ bblock recipe1 recipe2

Locking a recipe means locking all tasks of the recipe. If you need to
lock only particular tasks, use the `-t` option with a comma-separated
list of tasks::

  $ bblock -t task1,task2 recipe

### Unlocking tasks and recipes

To unlock a recipe, use the `-r` option::

   $ bblock -r recipe

You can also use a space-separated list of recipes to unlock multiple recipes::

   $ bblock -r recipe1 recipe2

Unlocking a recipe means unlocking all tasks of the recipe. If you need to
unlock only particular tasks use the `-t` option with a comma-separated
list of tasks::

  $ bblock -r -t task1,task2 recipe

To unlock all recipes, do not specify any recipe::

  $ bblock -r

### Configuration file

`bblock` will dump the signatures in the `build/conf/bblock.conf` file,
included by default in meta/conf/bitbake.conf .

To dump the file, use the `-d` option::

  $ bblock -d

### Locking mechanism

`bblock` computes the signature(s) of the task(s) and sets the 3 following
variables: **SIGGEN_LOCKEDSIGS**, **SIGGEN_LOCKEDSIGS_TYPES**
and **SIGGEN_LOCKEDSIGS_TASKSIG_CHECK**.

In particular, `bblock` sets::

  SIGGEN_LOCKEDSIGS_TASKSIG_CHECK = "info"
  SIGGEN_LOCKEDSIGS_TYPES += "${PACKAGE_ARCHS}"

  SIGGEN_LOCKEDSIGS_<package_arch> += "<recipe>:<task>:<signature>"

This produces architecture specific locks and reminds user that some tasks
have locked signatures.

### Example

When working on the `python3` recipe, we can lock `python3-native` with
the following::

  $ bblock python3-native
  $ bblock -d
  # Generated by bblock
  SIGGEN_LOCKEDSIGS_TASKSIG_CHECK = "info"
  SIGGEN_LOCKEDSIGS_TYPES += "${PACKAGE_ARCHS}"

  SIGGEN_LOCKEDSIGS_x86_64 += "python3-native:do_patch:865859c27e603ba42025b7bb766c3cd4c0f477e4962cfd39128c0619d695fce7"
  SIGGEN_LOCKEDSIGS_x86_64 += "python3-native:do_populate_sysroot:f8fa5d3194cef638416000252b959e86d0a19f6b7898e1f56b643c588cdd8605"
  SIGGEN_LOCKEDSIGS_x86_64 += "python3-native:do_prepare_recipe_sysroot:fe295ac505d9d1143313424b201c6f3f2a0a90da40a13a905b86b874705f226a"
  SIGGEN_LOCKEDSIGS_x86_64 += "python3-native:do_fetch:1b6e4728fee631bc7a8a7006855c5b8182a8224579e32e3d0a2db77c26459f25"
  SIGGEN_LOCKEDSIGS_x86_64 += "python3-native:do_unpack:2ad74d6f865ef75c35c0e6bbe3f9a90923a6b2c62c18a3ddef514ea31fbc588f"
  SIGGEN_LOCKEDSIGS_x86_64 += "python3-native:do_deploy_source_date_epoch:15f89b8483c1ad7507480f337619bb98c26e231227785eb3543db163593e7b42"
  SIGGEN_LOCKEDSIGS_x86_64 += "python3-native:do_configure:7960c13d23270fdb12b3a7c426ce1da0d2f5c7cf5e5d3f5bdce5fa330eb7d482"
  SIGGEN_LOCKEDSIGS_x86_64 += "python3-native:do_compile:012e1d4a63f1a78fc2143bd90d704dbcf5865c5257d6272aa7540ec1cd3063d9"
  SIGGEN_LOCKEDSIGS_x86_64 += "python3-native:do_install:d3401cc2afa4c996beb154beaad3e45fa0272b9c56fb86e9db14ec3544c68f9d"
  SIGGEN_LOCKEDSIGS_x86_64 += "python3-native:do_build:fa88bb7afb9046c0417c24a3fa98a058653805a8b00eda2c2d7fea68fc42f882"
  SIGGEN_LOCKEDSIGS_x86_64 += "python3-native:do_collect_spdx_deps:cc9c53ba7c495567e9a38ec4801830c425c0d1f895aa2fc66930a2edd510d9b4"
  SIGGEN_LOCKEDSIGS_x86_64 += "python3-native:do_create_spdx:766a1d09368438b7b5a1a8e2a8f823b2b731db44b57e67d8b3196de91966f9c5"
  SIGGEN_LOCKEDSIGS_x86_64 += "python3-native:do_create_package_spdx:46f80faeab25575e9977ba3bf14c819489c3d489432ae5145255635108c21020"
  SIGGEN_LOCKEDSIGS_x86_64 += "python3-native:do_recipe_qa:cb960cdb074e7944e894958db58f3dc2a0436ecf87c247feb3e095e214fec0e4"
  SIGGEN_LOCKEDSIGS_x86_64 += "python3-native:do_populate_lic:15657441621ee83f15c2e650e7edbb036870b56f55e72e046c6142da3c5783fd"
  SIGGEN_LOCKEDSIGS_x86_64 += "python3-native:do_create_manifest:24f0abbec221d27bbb2909b6e846288b12cab419f1faf9f5006ed80423d37e28"
  SIGGEN_LOCKEDSIGS_x86_64 += "python3-native:do_addto_recipe_sysroot:bcb6a1905f113128de3f88d702b706befd6a786267c045ee82532759a7c214d7"

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Creating New Configuration Fragments In Your Build

**Configuration Fragments <Configuration Fragment>** define top level build
configuration features that can be independently enabled and disabled using
standard tooling. Such features are made of one or several build configuration
statements that are either contained in a fragment file, or are set indirectly
using the **Built-in Fragment** mechanism.

This section will describe how to create new fragments for your builds.

There are two kinds of configuration fragments:

-  Standard **Configuration Fragments <Configuration Fragment>** which a
   stored in a file. These fragments include a summary and a description,
   following by configuration statements.

-  **Built-in Fragments <Built-in Fragment>** which can be used to assign a
   value to a single variable and do not require a separate definition file.
   They are especially useful when a list of possible values is very long (or
   infinite).

## Creating A Standard Configuration Fragment

By default, all configuration fragments are located within the
`conf/fragments` directory of a **layer**. This location is defined by the
**OE_FRAGMENTS_PREFIX** variable which, in turn, is used as a parameter in an
addfragments <bitbake-user-manual/bitbake-user-manual-metadata:\\`addfragments\`\`
directive>` directive in bitbake.conf .

You can create one or more **configuration fragment** files in your
**layer** in this directory. Let's take the following example, where
`custom-fragment.conf` is our custom fragment file::

   meta-custom
   ├── conf
   │   ├── fragments
   │   │   └── custom-fragment.conf
   │   └── layer.conf
   ...

For our `custom-fragment.conf` file, the following variables **must** be set
for our fragment to be considered a valid fragment by the :term:`OpenEmbedded
Build System`:

-  **BB_CONF_FRAGMENT_SUMMARY**: a one-line summary of this fragment.

-  **BB_CONF_FRAGMENT_DESCRIPTION**: a description of this fragment.

> **Note:**
> The **BB_CONF_FRAGMENT_SUMMARY** and **BB_CONF_FRAGMENT_DESCRIPTION**
> variables are also passed as parameters in an :ref:`addfragments
> <bitbake-user-manual/bitbake-user-manual-metadata:\`\`addfragments\`\`
> directive>` directive in :oe_git:`bitbake.conf
> </openembedded-core/tree/meta/conf/bitbake.conf>`.
>

After creating these variables, our custom fragment should look like the
following:

```
:caption: custom-fragment.conf

BB_CONF_FRAGMENT_SUMMARY = "This fragment sets a limit of 4 bitbake threads and 4 parsing threads"
BB_CONF_FRAGMENT_DESCRIPTION = "This fragment is useful to constrain resource consumption when the Yocto default \
is causing an overload of host machine's memory and CPU resources."
```

For now, our fragment does not have any additional configuration statement.
Let's add the following assignments to our fragment:

```
:caption: custom-fragment.conf (continued)

BB_NUMBER_THREADS = "4"
BB_NUMBER_PARSE_THREADS = "4"
```

This means that our fragment can be enabled to set a limit on the number of
threads **BitBake** will use with the **BB_NUMBER_THREADS** and
**BB_NUMBER_PARSE_THREADS** variables.

For now, our fragment exists and is listed by the
ref-bitbake-config-build-list-fragments command, but is not enabled. To
enable this fragment, use the ref-bitbake-config-build-enable-fragment
command::

   bitbake-config-build enable-fragment meta-custom/custom-fragment

> **Note:**
> The `meta-custom` prefix in the above command depends on the name of your
> layer. This name is defined by the **BBFILE_COLLECTIONS** variable in
> the `conf/layer.conf` file of your layer.
>

Standard Configuration fragments can be organized in a more complex way. For
example, it's possible to create sub-directories to organize your fragments::

   meta-custom
   ├── conf
   │   ├── fragments
   │   │   ├── networking
   │   │   │   └── mirrors.conf
   │   │   └── resources
   │   │       └── numberthreads.conf
   │   └── layer.conf
   ...

In the above example, the `meta-custom/networking/mirrors` and
`meta-custom/resources/numberthreads` fragments will be available in your
build.

## Creating A Built-in Fragment

Within the **OpenEmbedded Build System**, Built-in Fragments are defined
with the **OE_FRAGMENTS_BUILTIN** variable, which is passed as a
parameter in an addfragments <bitbake-user-manual/bitbake-user-manual-metadata:\\`addfragments\`\`
directive>` directive in bitbake.conf .

Adding new **Built-in Fragments <Built-in Fragment>** can be done by
appending the **OE_FRAGMENTS_BUILTIN** variable from your **layer**
configuration file:

```
:caption: layer.conf

OE_FRAGMENTS_BUILTIN:append = " custom-builtin-fragment:CUSTOM_VARIABLE"
```

> **Warning:**
> Make sure to use the `:append` override in the above assignment, as using
> `+=` can lead to unexpected behavior.
>

> **Warning:**
> Due to the way **BitBake** parses files, it is not possible to modify
> **OE_FRAGMENTS_BUILTIN** from any kind of **configuration file**.
> Setting it from the **layer** configuration file (`conf/layer.conf`) is
> the retained solution to create new built-in fragments.
>

You can then use the ref-bitbake-config-build-enable-fragment command to
set a value to the `CUSTOM_VARIABLE` variable::

   bitbake-config-build enable-fragment custom-builtin-fragment/somevalue

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Efficiently Fetching Source Files During a Build

The OpenEmbedded build system works with source files located through
the **SRC_URI** variable. When
you build something using BitBake, a big part of the operation is
locating and downloading all of the source code. For images,
downloading all the source for various packages can take a significant
amount of time.

This section shows you how you can use mirrors to speed up fetching
source files and how you can pre-fetch files all of which leads to more
efficient use of resources and time.

## Setting up Effective Mirrors

A good deal that goes into a Yocto Project build is simply downloading
source code. Maybe you have been working with another
build system for which you have built up a
sizable directory of source tarballs. Or, perhaps someone else has such
a directory for which you have read access. If so, you can save time by
adding statements to your configuration file so that the build process
checks local directories first for existing tarballs before checking the
Internet.

Here is an efficient way to set it up in your `local.conf` file::

   SOURCE_MIRROR_URL ?= "file:///home/you/your-download-dir/"
   INHERIT += "own-mirrors"
   BB_GENERATE_MIRROR_TARBALLS = "1"
   # BB_NO_NETWORK = "1"

In the previous example, the
**BB_GENERATE_MIRROR_TARBALLS**
variable causes the OpenEmbedded build system to generate tarballs of
the Git repositories and store them in the
**DL_DIR** directory. Due to
performance reasons, generating and storing these tarballs is not the
build system's default behavior.

You can also use the
**PREMIRRORS** variable. For
an example, see the variable's glossary entry in the Yocto Project
Reference Manual.

## Getting Source Files and Suppressing the Build

Another technique you can use to ready yourself for a successive string
of build operations, is to pre-fetch all the source files without
actually starting a build. This technique lets you work through any
download issues and ultimately gathers all the source files into your
download directory structure-build-downloads,
which is located with **DL_DIR**.

Use the following BitBake command form to fetch all the necessary
sources without starting the build::

   $ bitbake target --runall=fetch

This
variation of the BitBake command guarantees that you have all the
sources for that BitBake target should you disconnect from the Internet
and want to do the build later offline.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Limiting the Host Resources Usage

While you sometimes need to :doc:`speed up a build
</dev-manual/speeding-up-build>`, you may also need to limit the resources used
by the **OpenEmbedded Build System**, especially on shared infrastructures
where multiple users start heavy-load builds, or when building on low-power
machines.

This document aims at giving the different configuration variables available to
limit the resources used by the build system. These variables should be set from
a **configuration file** and thus take effect over the entire build environment.
For each variable, also see the variable description in the glossary for more
details.

-  **BB_NUMBER_THREADS**:

   This sets a hard limit on the number of threads **BitBake** can run at the
   same time. Lowering this value will set a limit to the number of
   **BitBake** threads, but will not prevent a single task from starting more
   compilation threads (see **PARALLEL_MAKE**).

-  **BB_NUMBER_PARSE_THREADS**:

   Like **BB_NUMBER_THREADS**, but this variable sets a limit on the number
   of threads during the parsing of the environment (before executing tasks).

-  **PARALLEL_MAKE**:

   This variable should be set in the form of `-jN`, where `N` is a positive
   integer. This integer controls the number of threads used when starting
   `make`. Note that this variable is not limited to the usage of `make`,
   but extends to the compilation (ref-tasks-compile task) commands
   defined by the ref-classes-meson, ref-classes-cmake and such
   classes.

   If you want to have a different limit from the rest of the build for a
   recipe, it is also possible to achieve with the following line added to your
   `local.conf` **configuration file**::

      PARALLEL_MAKE:pn-linux-yocto = "-j4"

   The above example will limit the number of threads used by `make` for the
   `linux-yocto` recipe to 4.

-  **PARALLEL_MAKEINST**:

   Like **PARALLEL_MAKE**, but this variable controls the number of threads
   used during the ref-tasks-install task.

   The default value of **PARALLEL_MAKEINST** is the value of
   **PARALLEL_MAKE**.

> **Note:**
> While most of the variables in this document help to limit the CPU load, it
> is also possible that the host system runs out of physical RAM when running
> builds. This can trigger the out-of-memory killer and stop the related
> processes abruptly. This can create strange looking failures in the output
> log of the tasks in question. The out-of-memory killer only logs in the
> kernel dmesg logs, so it is advised to monitor it closely with the `dmesg`
> command when encountering unexpected failures during builds.
>
> In these situations, lowering the value of **PARALLEL_MAKE** and
> **BB_NUMBER_THREADS** is recommended.
>

-  **BB_PRESSURE_MAX_CPU**, **BB_PRESSURE_MAX_IO** and
   **BB_PRESSURE_MAX_MEMORY**:

   These variables control the limit of pressure (PSI as defined by
   https://docs.kernel.org/accounting/psi.html) on the system, and will
   limit the number of **BitBake** threads dynamically depending on the
   current pressure of the system. This also means that your host must support
   the PSI kernel feature (otherwise see **BB_LOADFACTOR_MAX** below).

   These variables take a positive integer between 1 (extremely low limit) and
   1000000 (value unlikely ever reached). Setting an extremely low value, such
   as 2, is not desirable as it will result in **BitBake** limiting the
   number of threads to 1 most of the time.

   To determine a reasonable value to set for your host, follow the steps below:

   #. In a Bash shell, start the following script, which will provide an
      estimate of the current pressure on your host:

      .. code-block:: bash

         pressure="0"
         while true; do
            prev_pressure="$pressure"
            pressure=$(head -1 /proc/pressure/cpu  | cut -d' ' -f5 | cut -d'=' -f2)
            echo $(( $pressure - $prev_pressure ))
            sleep 1
         done

      .. note::

         Change `/proc/pressure/cpu` to `/proc/pressure/io` or
         `/proc/pressure/memory` to change the pressure type to monitor.

      This script can be stopped by pressing Control + C.

   #.  Then, start a heavy-load build, for example::

          bitbake virtual/kernel -c compile -f

       You can stop the build at anytime with Control + C.

   #.  Monitor the values printed on the console. These should indicate how the
       pressure evolves during the build. You can take a value below the maximum
       printed value as a starting point.

   After setting initial values, you can monitor when the pressure exceeds of the
   limit set by the above variables by running **BitBake** with the
   `--verbose` and `--log-domains` flag:

   .. code-block:: console

      $ bitbake virtual/kernel -c compile -f --verbose --log-domains BitBake.RunQueue.PSI

   In parallel, you can monitor the pressure information by tracking the output
   log file:

   .. code-block:: console

      $ tail -F tmp/log/cooker/<machine>/console-latest.log | grep "Pressure status changed"
      NOTE: Pressure status changed to CPU: False, IO: None, Mem: None (CPU: 1105.9/2.0, IO: 0.0/2.0, Mem: 0.0/2.0) - using 1/64 bitbake threads
      ...

   Take a look at the value between parenthesis: ``CPU: 1105.9/2.0, IO: 0.0/2.0,
   Mem: 0.0/2.0``. They correspond to the current pressure value for the CPU, IO
   and memory respectively. If **BitBake** prints these messages a lot, it
   is likely that your pressure limit is too low, and thus can be raised to a
   higher value.

-  **BB_LOADFACTOR_MAX**:

   This variable will limit the number of threads **BitBake** will start
   by monitoring the current CPU load of the host system. **BitBake** will
   print the following when the limit set by **BB_LOADFACTOR_MAX** is
   reached::

      Load average limiting set to True as load average: 0.7188262939453125 - using 37/64 bitbake threads

   This variable has no effect when any of **BB_PRESSURE_MAX_CPU**,
   **BB_PRESSURE_MAX_IO** or **BB_PRESSURE_MAX_MEMORY** is set, as it
   was designed for systems that do not have pressure information available.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Using the Quick EMUlator (QEMU)

The Yocto Project uses an implementation of the Quick EMUlator (QEMU)
Open Source project as part of the Yocto Project development "tool set".
This chapter provides both procedures that show you how to use the Quick
EMUlator (QEMU) and other QEMU information helpful for development
purposes.

## Overview

Within the context of the Yocto Project, QEMU is an emulator and
virtualization machine that allows you to run a complete image you have
built using the Yocto Project as just another task on your build system.
QEMU is useful for running and testing images and applications on
supported Yocto Project architectures without having actual hardware.
Among other things, the Yocto Project uses QEMU to run automated Quality
Assurance (QA) tests on final images shipped with each release.

> **Note:**
> This implementation is not the same as QEMU in general.
>

This section provides a brief reference for the Yocto Project
implementation of QEMU.

For official information and documentation on QEMU in general, see the
following references:

-  `QEMU Website <https://wiki.qemu.org/Main_Page>`__\ *:* The official
   website for the QEMU Open Source project.

-  `Documentation <https://wiki.qemu.org/Manual>`__\ *:* The QEMU user
   manual.

## Running QEMU

To use QEMU, you need to have QEMU installed and initialized as well as
have the proper artifacts (i.e. image files and root filesystems)
available. Follow these general steps to run QEMU:

#. *Install QEMU:* QEMU is made available with the Yocto Project a
   number of ways. One method is to install a Software Development Kit
   (SDK). See "sdk-manual/intro:the qemu emulator" section in the
   Yocto Project Application Development and the Extensible Software
   Development Kit (eSDK) manual for information on how to install QEMU.

#. *Setting Up the Environment:* How you set up the QEMU environment
   depends on how you installed QEMU:

   -  If you followed /dev-manual/start, either use `bitbake-setup` or source
      the environment setup script manually:

      .. code-block:: console

         $ cd bitbake-builds
         $ source layers/openembedded-core/oe-init-build-env

   -  If you installed a cross-toolchain, you can run the script that
      initializes the toolchain. For example, the following commands run
      the initialization script from the default `poky_sdk` directory::

         . poky_sdk/environment-setup-core2-64-poky-linux

#. *Ensure the Artifacts are in Place:* You need to be sure you have a
   pre-built kernel that will boot in QEMU. You also need the target
   root filesystem for your target machine's architecture:

   -  If you have previously built an image for QEMU (e.g. `qemux86`,
      `qemuarm`, and so forth), then the artifacts are in place in
      your **Build Directory**.

   -  If you have not built an image, you can go to the
      machines/qemu  area and download a
      pre-built image that matches your architecture and can be run on
      QEMU.

   See the "sdk-manual/appendix-obtain:extracting the root filesystem"
   section in the Yocto Project Application Development and the
   Extensible Software Development Kit (eSDK) manual for information on
   how to extract a root filesystem.

#. *Run QEMU:* The basic `runqemu` command syntax is as follows::

      $ runqemu [option ] [...]

   Based on what you provide on the command
   line, `runqemu` does a good job of figuring out what you are trying
   to do. For example, by default, QEMU looks for the most recently
   built image according to the timestamp when it needs to look for an
   image. Minimally, through the use of options, you must provide either
   a machine name, a virtual machine image (`*wic.vmdk`), or a kernel
   image (`*.bin`).

   Here are some additional examples to help illustrate further QEMU:

   -  This example starts QEMU with MACHINE set to "qemux86-64".
      Assuming a standard **Build Directory**, `runqemu`
      automatically finds the `bzImage-qemux86-64.bin` image file and
      the `core-image-minimal-qemux86-64-20200218002850.rootfs.ext4`
      (assuming the current build created a `core-image-minimal`
      image)::

        $ runqemu qemux86-64

      .. note::

         When more than one image with the same name exists, QEMU finds
         and uses the most recently built image according to the
         timestamp.

   -  This example produces the exact same results as the previous
      example. This command, however, specifically provides the image
      and root filesystem type::

         $ runqemu qemux86-64 core-image-minimal ext4

   -  This example starts QEMU in a temporary root filesystem ("snapshot") with
      no graphical environment ("nographic"), meaning QEMU is started in the
      current shell:

      .. code-block:: console

         $ runqemu qemux86-64 snapshot nographic

      Running in "snapshot" mode means that changes in root filesystem will not
      remain after quitting QEMU.

   -  This example specifies to boot an **Initramfs** image and to
      enable audio in QEMU. For this case, `runqemu` sets the internal
      variable `FSTYPE` to `cpio.gz`. Also, for audio to be enabled,
      an appropriate driver must be installed (see the `audio` option
      in dev-manual/qemu:\\`runqemu\`\` command-line options`
      for more information)::

         $ runqemu qemux86-64 ramfs audio

   -  This example does not provide enough information for QEMU to
      launch. While the command does provide a root filesystem type, it
      must also minimally provide a `MACHINE`, `KERNEL`, or `VM` option::

         $ runqemu ext4

   -  This example specifies to boot a virtual machine image
      (`.wic.vmdk` file). From the `.wic.vmdk`, `runqemu`
      determines the QEMU architecture (`MACHINE`) to be "qemux86-64" and
      the root filesystem type to be "vmdk"::

         $ runqemu /home/scott-lenovo/vm/core-image-minimal-qemux86-64.wic.vmdk

## Switching Between Consoles

When booting or running QEMU, you can switch between supported consoles
by using Ctrl+Alt+number. For example, Ctrl+Alt+3 switches you to the
serial console as long as that console is enabled. Being able to switch
consoles is helpful, for example, if the main QEMU console breaks for
some reason.

> **Note:**
> Usually, "2" gets you to the main console and "3" gets you to the
> serial console.
>

## Removing the Splash Screen

You can remove the splash screen when QEMU is booting by using Alt+left.
Removing the splash screen allows you to see what is happening in the
background.

## Disabling the Cursor Grab

The default QEMU integration captures the cursor within the main window.
It does this since standard mouse devices only provide relative input
and not absolute coordinates. You then have to break out of the grab
using the "Ctrl+Alt" key combination. However, the Yocto Project's
integration of QEMU enables the wacom USB touch pad driver by default to
allow input of absolute coordinates. This default means that the mouse
can enter and leave the main window without the grab taking effect
leading to a better user experience.

## Running Under a Network File System (NFS) Server

One method for running QEMU is to run it on an NFS server. This is
useful when you need to access the same file system from both the build
and the emulated system at the same time. It is also worth noting that
the system does not need root privileges to run. It uses a user space
NFS server to avoid that. Follow these steps to set up for running QEMU
using an NFS server.

#. *Extract a Root Filesystem:* Once you are able to run QEMU in your
   environment, you can use the `runqemu-extract-sdk` script, which is
   located in the `scripts` directory along with the `runqemu`
   script.

   The `runqemu-extract-sdk` takes a root filesystem tarball and
   extracts it into a location that you specify. Here is an example that
   takes a file system and extracts it to a directory named
   `test-nfs`:

   .. code-block:: none

      runqemu-extract-sdk ./tmp/deploy/images/qemux86-64/core-image-sato-qemux86-64.tar.bz2 test-nfs

#. *Start QEMU:* Once you have extracted the file system, you can run
   `runqemu` normally with the additional location of the file system.
   You can then also make changes to the files within `./test-nfs` and
   see those changes appear in the image in real time. Here is an
   example using the `qemux86` image:

   .. code-block:: none

      runqemu qemux86-64 ./test-nfs

> **Note:**
> Should you need to start, stop, or restart the NFS share, you can use
> the following commands:
>
> -  To start the NFS share::
>
> runqemu-export-rootfs start file-system-location
>
> -  To stop the NFS share::
>
> runqemu-export-rootfs stop file-system-location
>
> -  To restart the NFS share::
>
> runqemu-export-rootfs restart file-system-location
>

## QEMU CPU Compatibility Under KVM

By default, the QEMU build compiles for and targets 64-bit and x86 Intel
Core2 Duo processors and 32-bit x86 Intel Pentium II processors. QEMU
builds for and targets these CPU types because they display a broad
range of CPU feature compatibility with many commonly used CPUs.

Despite this broad range of compatibility, the CPUs could support a
feature that your host CPU does not support. Although this situation is
not a problem when QEMU uses software emulation of the feature, it can
be a problem when QEMU is running with KVM enabled. Specifically,
software compiled with a certain CPU feature crashes when run on a CPU
under KVM that does not support that feature. To work around this
problem, you can override QEMU's runtime CPU setting by changing the
`QB_CPU_KVM` variable in `qemuboot.conf` in the **Build Directory**
`deploy/image` directory. This setting specifies a `-cpu` option passed
into QEMU in the `runqemu` script. Running `qemu -cpu help` returns a
list of available supported CPU types.

## QEMU Performance

Using QEMU to emulate your hardware can result in speed issues depending
on the target and host architecture mix. For example, using the
`qemux86` image in the emulator on an Intel-based 32-bit (x86) host
machine is fast because the target and host architectures match. On the
other hand, using the `qemuarm` image on the same Intel-based host can
be slower. But, you still achieve faithful emulation of ARM-specific
issues.

To speed things up, the QEMU images support using `distcc` to call a
cross-compiler outside the emulated system. If you used `runqemu` to
start QEMU, and the `distccd` application is present on the host
system, any BitBake cross-compiling toolchain available from the build
system is automatically used from within QEMU simply by calling
`distcc`. You can accomplish this by defining the cross-compiler
variable (e.g. `export CC="distcc"`). Alternatively, if you are using
a suitable SDK image or the appropriate stand-alone toolchain is
present, the toolchain is also automatically used.

> **Note:**
> There are several mechanisms to connect to the system running
> on the QEMU emulator:
>
> -  QEMU provides a framebuffer interface that makes standard consoles
> available.
>
> -  Generally, headless embedded devices have a serial port. If so,
> you can configure the operating system of the running image to use
> that port to run a console. The connection uses standard IP
> networking.
>
> -  SSH servers are available in some QEMU images. The `core-image-sato`
> QEMU image has a Dropbear secure shell (SSH) server that runs with the
> root password disabled. The `core-image-full-cmdline` QEMU image has
> OpenSSH instead of Dropbear. Including these SSH servers allow you to use
> standard `ssh` and `scp` commands. The `core-image-minimal` QEMU
> image, however, contains no SSH server.
>
> -  You can use a provided, user-space NFS server to boot the QEMU
> session using a local copy of the root filesystem on the host. In
> order to make this connection, you must extract a root filesystem
> tarball by using the `runqemu-extract-sdk` command. After
> running the command, you must then point the `runqemu` script to
> the extracted directory instead of a root filesystem image file.
> See the
> "dev-manual/qemu:running under a network file system (nfs) server"
> section for more information.
>

## QEMU Command-Line Syntax

The basic `runqemu` command syntax is as follows::

   $ runqemu [option ] [...]

Based on what you provide on the command line, `runqemu` does a
good job of figuring out what you are trying to do. For example, by
default, QEMU looks for the most recently built image according to the
timestamp when it needs to look for an image. Minimally, through the use
of options, you must provide either a machine name, a virtual machine
image (`*wic.vmdk`), or a kernel image (`*.bin`).

Here is the command-line help output for the `runqemu` command::

   $ runqemu --help

   Usage: you can run this script with any valid combination
   of the following environment variables (in any order):
     KERNEL - the kernel image file to use
     ROOTFS - the rootfs image file or nfsroot directory to use
     MACHINE - the machine name (optional, autodetected from KERNEL filename if unspecified)
     Simplified QEMU command-line options can be passed with:
       nographic - disable video console
       serial - enable a serial console on /dev/ttyS0
       slirp - enable user networking, no root privileges required
       kvm - enable KVM when running x86/x86_64 (VT-capable CPU required)
       kvm-vhost - enable KVM with vhost when running x86/x86_64 (VT-capable CPU required)
       publicvnc - enable a VNC server open to all hosts
       audio - enable audio
       [*/]ovmf* - OVMF firmware file or base name for booting with UEFI
     tcpserial=<port> - specify tcp serial port number
     biosdir=<dir> - specify custom bios dir
     biosfilename=<filename> - specify bios filename
     qemuparams=<xyz> - specify custom parameters to QEMU
     bootparams=<xyz> - specify custom kernel parameters during boot
     help, -h, --help: print this text

   Examples:
     runqemu
     runqemu qemuarm
     runqemu tmp/deploy/images/qemuarm
     runqemu tmp/deploy/images/qemux86/<qemuboot.conf>
     runqemu qemux86-64 core-image-sato ext4
     runqemu qemux86-64 wic-image-minimal wic
     runqemu path/to/bzImage-qemux86.bin path/to/nfsrootdir/ serial
     runqemu qemux86 iso/hddimg/wic.vmdk/wic.qcow2/wic.vdi/ramfs/cpio.gz...
     runqemu qemux86 qemuparams="-m 256"
     runqemu qemux86 bootparams="psplash=false"
     runqemu path/to/<image>-<machine>.wic
     runqemu path/to/<image>-<machine>.wic.vmdk

## ``runqemu`` Command-Line Options

Here is a description of `runqemu` options you can provide on the
command line:

> **Note:**
> If you do provide some "illegal" option combination or perhaps you do
> not provide enough in the way of options, `runqemu`
> provides appropriate error messaging to help you correct the problem.
>

-  `QEMUARCH`: The QEMU machine architecture, which must be "qemuarm",
   "qemuarm64", "qemumips", "qemumips64", "qemuppc", "qemux86", or
   "qemux86-64".

-  `VM`: The virtual machine image, which must be a `.wic.vmdk`
   file. Use this option when you want to boot a `.wic.vmdk` image.

-  `ROOTFS`: A root filesystem that has one of the following filetype
   extensions: "ext2", "ext3", "ext4", "jffs2", "nfs", or "btrfs". If
   the filename you provide for this option uses "nfs", it must provide
   an explicit root filesystem path.

-  `KERNEL`: A kernel image, which is a `.bin` file. When you provide a
   `.bin` file, `runqemu` detects it and assumes the file is a
   kernel image.

-  `MACHINE`: The architecture of the QEMU machine, which must be one of
   the following: "qemux86", "qemux86-64", "qemuarm", "qemuarm64",
   "qemumips", "qemumips64", or "qemuppc". The MACHINE and QEMUARCH
   options are basically identical. If you do not provide a MACHINE
   option, `runqemu` tries to determine it based on other options.

-  `ramfs`: Indicates you are booting an **Initramfs**
   image, which means the `FSTYPE` is `cpio.gz`.

-  `iso`: Indicates you are booting an ISO image, which means the
   `FSTYPE` is `.iso`.

-  `nographic`: Disables the video console, which sets the console to
   "ttys0". This option is useful when you have logged into a server and
   you do not want to disable forwarding from the X Window System (X11)
   to your workstation or laptop.

-  `serial`: Enables a serial console on `/dev/ttyS0`.

-  `biosdir`: Establishes a custom directory for BIOS, VGA BIOS and
   keymaps.

-  `biosfilename`: Establishes a custom BIOS name.

-  `qemuparams=\"xyz\"`: Specifies custom QEMU parameters. Use this
   option to pass options other than the simple "kvm" and "serial"
   options.

-  `bootparams=\"xyz\"`: Specifies custom boot parameters for the
   kernel.

-  `audio`: Enables audio in QEMU. The MACHINE option must be either
   "qemux86" or "qemux86-64" in order for audio to be enabled.
   Additionally, the `snd_intel8x0` or `snd_ens1370` driver must be
   installed in linux guest.

-  `slirp`: Enables "slirp" networking, which is a different way of
   networking that does not need root access but also is not as easy to
   use or comprehensive as the default.

   Using `slirp` by default will forward the guest machine's
   22 and 23 TCP ports to host machine's 2222 and 2323 ports
   (or the next free ports). Specific forwarding rules can be configured
   by setting `QB_SLIRP_OPT` as environment variable or in `qemuboot.conf`
   in the **Build Directory** `deploy/image` directory.
   Examples::

      QB_SLIRP_OPT = "-netdev user,id=net0,hostfwd=tcp::8080-:80"

      QB_SLIRP_OPT = "-netdev user,id=net0,hostfwd=tcp::8080-:80,hostfwd=tcp::2222-:22"

   The first example forwards TCP port 80 from the emulated system to
   port 8080 (or the next free port) on the host system,
   allowing access to an http server running in QEMU from
   `http://<host ip>:8080/`.

   The second example does the same, but also forwards TCP port 22 on the
   guest system to 2222 (or the next free port) on the host system,
   allowing ssh access to the emulated system using
   `ssh -P 2222 <user>@<host ip>`.

   Keep in mind that proper configuration of firewall software is required.

-  `kvm`: Enables KVM when running "qemux86" or "qemux86-64" QEMU
   architectures. For KVM to work, all the following conditions must be
   met:

   -  Your MACHINE must be either qemux86" or "qemux86-64".

   -  Your build host has to have the KVM modules installed, which are
      `/dev/kvm`.

   -  The build host `/dev/kvm` directory has to be both writable and
      readable.

-  `kvm-vhost`: Enables KVM with VHOST support when running "qemux86"
   or "qemux86-64" QEMU architectures. For KVM with VHOST to work, the
   following conditions must be met:

   -  `kvm` option conditions defined above must be met.

   -  Your build host has to have virtio net device, which are
      `/dev/vhost-net`.

   -  The build host `/dev/vhost-net` directory has to be either
      readable or writable and "slirp-enabled".

-  `publicvnc`: Enables a VNC server open to all hosts.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Using Quilt in Your Workflow

`Quilt <https://savannah.nongnu.org/projects/quilt>`__ is a powerful tool
that allows you to capture source code changes without having a clean
source tree. This section outlines the typical workflow you can use to
modify source code, test changes, and then preserve the changes in the
form of a patch all using Quilt.

> **Note:**
> With regard to preserving changes to source files, if you clean a
> recipe or have ref-classes-rm-work enabled, the
> devtool workflow 
> as described in the Yocto Project Application Development and the
> Extensible Software Development Kit (eSDK) manual is a safer
> development flow than the flow that uses Quilt.
>

Follow these general steps:

#. *Find the Source Code:* Temporary source code used by the
   OpenEmbedded build system is kept in the **Build Directory**. See the
   "dev-manual/temporary-source-code:finding temporary source code" section to
   learn how to locate the directory that has the temporary source code for a
   particular package.

#. *Change Your Working Directory:* You need to be in the directory that
   has the temporary source code. That directory is defined by the
   **S** variable.

#. *Create a New Patch:* Before modifying source code, you need to
   create a new patch. To create a new patch file, use `quilt new` as
   below::

      $ quilt new my_changes.patch

#. *Notify Quilt and Add Files:* After creating the patch, you need to
   notify Quilt about the files you plan to edit. You notify Quilt by
   adding the files to the patch you just created::

      $ quilt add file1.c file2.c file3.c

#. *Edit the Files:* Make your changes in the source code to the files
   you added to the patch.

#. *Test Your Changes:* Once you have modified the source code, the
   easiest way to test your changes is by calling the ref-tasks-compile
   task as shown in the following example::

      $ bitbake -c compile -f package

   The `-f` or `--force` option forces the specified task to
   execute. If you find problems with your code, you can just keep
   editing and re-testing iteratively until things work as expected.

   .. note::

      All the modifications you make to the temporary source code disappear
      once you run the ref-tasks-clean or ref-tasks-cleanall
      tasks using BitBake (i.e. `bitbake -c clean package` and
      `bitbake -c cleanall package`). Modifications will also disappear if
      you use the ref-classes-rm-work feature as described in
      the "dev-manual/disk-space:conserving disk space during builds"
      section.

#. *Generate the Patch:* Once your changes work as expected, you need to
   use Quilt to generate the final patch that contains all your
   modifications::

      $ quilt refresh

   At this point, the
   `my_changes.patch` file has all your edits made to the `file1.c`,
   `file2.c`, and `file3.c` files.

   You can find the resulting patch file in the `patches/`
   subdirectory of the source (**S**) directory.

#. *Copy the Patch File:* For simplicity, copy the patch file into a
   directory named `files`, which you can create in the same directory
   that holds the recipe (`.bb`) file or the append (`.bbappend`)
   file. Placing the patch here guarantees that the OpenEmbedded build
   system will find the patch. Next, add the patch into the **SRC_URI**
   of the recipe. Here is an example::

      SRC_URI += "file://my_changes.patch"

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Maintaining Build Output Quality

Many factors can influence the quality of a build. For example, if you
upgrade a recipe to use a new version of an upstream software package or
you experiment with some new configuration options, subtle changes can
occur that you might not detect until later. Consider the case where
your recipe is using a newer version of an upstream package. In this
case, a new version of a piece of software might introduce an optional
dependency on another library, which is auto-detected. If that library
has already been built when the software is building, the software will
link to the built library and that library will be pulled into your
image along with the new software even if you did not want the library.

The ref-classes-buildhistory class helps you maintain the quality of
your build output. You can use the class to highlight unexpected and possibly
unwanted changes in the build output. When you enable build history, it records
information about the contents of each package and image and then commits that
information to a local Git repository where you can examine the information.

The remainder of this section describes the following:

-  How you can enable and disable build history 

-  How to understand what the build history contains 

-  How to limit the information used for build history 

-  How to examine the build history from both a command-line and web interface 

## Enabling and Disabling Build History

Build history is disabled by default. To enable it, add the following
**INHERIT** statement and set the **BUILDHISTORY_COMMIT** variable to
"1" at the end of your `conf/local.conf` file found in the
**Build Directory**::

   INHERIT += "buildhistory"
   BUILDHISTORY_COMMIT = "1"

Enabling build history as
previously described causes the OpenEmbedded build system to collect
build output information and commit it as a single commit to a local
overview-manual/development-environment:git repository.

> **Note:**
> Enabling build history increases your build times slightly,
> particularly for images, and increases the amount of disk space used
> during the build.
>

You can disable build history by removing the previous statements from
your `conf/local.conf` file.

## Understanding What the Build History Contains

Build history information is kept in `${`\ **TOPDIR**\ `}/buildhistory`
in the **Build Directory** as defined by the **BUILDHISTORY_DIR**
variable. Here is an example abbreviated listing:

![buildhistory.png](figures/buildhistory.png)

At the top level, there is a `metadata-revs` file that lists the
revisions of the repositories for the enabled layers when the build was
produced. The rest of the data splits into separate `packages`,
`images` and `sdk` directories, the contents of which are described
as follows.

### Build History Package Information

The history for each package contains a text file that has name-value
pairs with information about the package. For example,
`buildhistory/packages/i586-poky-linux/busybox/busybox/latest`
contains the following:

```none
PV = 1.22.1
PR = r32
RPROVIDES =
RDEPENDS = glibc (>= 2.20) update-alternatives-opkg
RRECOMMENDS = busybox-syslog busybox-udhcpc update-rc.d
PKGSIZE = 540168
FILES = /usr/bin/* /usr/sbin/* /usr/lib/busybox/* /usr/lib/lib*.so.* \
   /etc /com /var /bin/* /sbin/* /lib/*.so.* /lib/udev/rules.d \
   /usr/lib/udev/rules.d /usr/share/busybox /usr/lib/busybox/* \
   /usr/share/pixmaps /usr/share/applications /usr/share/idl \
   /usr/share/omf /usr/share/sounds /usr/lib/bonobo/servers
FILELIST = /bin/busybox /bin/busybox.nosuid /bin/busybox.suid /bin/sh \
   /etc/busybox.links.nosuid /etc/busybox.links.suid
```

Most of these
name-value pairs correspond to variables used to produce the package.
The exceptions are `FILELIST`, which is the actual list of files in
the package, and `PKGSIZE`, which is the total size of files in the
package in bytes.

There is also a file that corresponds to the recipe from which the package
came (e.g. `buildhistory/packages/i586-poky-linux/busybox/latest`):

```none
PV = 1.22.1
PR = r32
DEPENDS = initscripts kern-tools-native update-rc.d-native \
   virtual/i586-poky-linux-compilerlibs virtual/i586-poky-linux-gcc \
   virtual/libc virtual/update-alternatives
PACKAGES = busybox-ptest busybox-httpd busybox-udhcpd busybox-udhcpc \
   busybox-syslog busybox-mdev busybox-hwclock busybox-dbg \
   busybox-staticdev busybox-dev busybox-doc busybox-locale busybox
```

Finally, for those recipes fetched from a version control system (e.g.,
Git), there is a file that lists source revisions that are specified in
the recipe and the actual revisions used during the build. Listed
and actual revisions might differ when
**SRCREV** is set to
${**AUTOREV**}. Here is an
example assuming
`buildhistory/packages/qemux86-poky-linux/linux-yocto/latest_srcrev`)::

   # SRCREV_machine = "38cd560d5022ed2dbd1ab0dca9642e47c98a0aa1"
   SRCREV_machine = "38cd560d5022ed2dbd1ab0dca9642e47c98a0aa1"
   # SRCREV_meta = "a227f20eff056e511d504b2e490f3774ab260d6f"
   SRCREV_meta ="a227f20eff056e511d504b2e490f3774ab260d6f"

You can use the
`buildhistory-collect-srcrevs` command with the `-a` option to
collect the stored **SRCREV** values from build history and report them
in a format suitable for use in global configuration (e.g.,
`local.conf` or a distro include file) to override floating
**AUTOREV** values to a fixed set of revisions. Here is some example
output from this command::

   $ buildhistory-collect-srcrevs -a
   # all-poky-linux
   SRCREV:pn-ca-certificates = "07de54fdcc5806bde549e1edf60738c6bccf50e8"
   SRCREV:pn-update-rc.d = "8636cf478d426b568c1be11dbd9346f67e03adac"
   # core2-64-poky-linux
   SRCREV:pn-binutils = "87d4632d36323091e731eb07b8aa65f90293da66"
   SRCREV:pn-btrfs-tools = "8ad326b2f28c044cb6ed9016d7c3285e23b673c8"
   SRCREV_bzip2-tests:pn-bzip2 = "f9061c030a25de5b6829e1abf373057309c734c0"
   SRCREV:pn-e2fsprogs = "02540dedd3ddc52c6ae8aaa8a95ce75c3f8be1c0"
   SRCREV:pn-file = "504206e53a89fd6eed71aeaf878aa3512418eab1"
   SRCREV_glibc:pn-glibc = "24962427071fa532c3c48c918e9d64d719cc8a6c"
   SRCREV:pn-gnome-desktop-testing = "e346cd4ed2e2102c9b195b614f3c642d23f5f6e7"
   SRCREV:pn-init-system-helpers = "dbd9197569c0935029acd5c9b02b84c68fd937ee"
   SRCREV:pn-kmod = "b6ecfc916a17eab8f93be5b09f4e4f845aabd3d1"
   SRCREV:pn-libnsl2 = "82245c0c58add79a8e34ab0917358217a70e5100"
   SRCREV:pn-libseccomp = "57357d2741a3b3d3e8425889a6b79a130e0fa2f3"
   SRCREV:pn-libxcrypt = "50cf2b6dd4fdf04309445f2eec8de7051d953abf"
   SRCREV:pn-ncurses = "51d0fd9cc3edb975f04224f29f777f8f448e8ced"
   SRCREV:pn-procps = "19a508ea121c0c4ac6d0224575a036de745eaaf8"
   SRCREV:pn-psmisc = "5fab6b7ab385080f1db725d6803136ec1841a15f"
   SRCREV:pn-ptest-runner = "bcb82804daa8f725b6add259dcef2067e61a75aa"
   SRCREV:pn-shared-mime-info = "18e558fa1c8b90b86757ade09a4ba4d6a6cf8f70"
   SRCREV:pn-zstd = "e47e674cd09583ff0503f0f6defd6d23d8b718d3"
   # qemux86_64-poky-linux
   SRCREV_machine:pn-linux-yocto = "20301aeb1a64164b72bc72af58802b315e025c9c"
   SRCREV_meta:pn-linux-yocto = "2d38a472b21ae343707c8bd64ac68a9eaca066a0"
   # x86_64-linux
   SRCREV:pn-binutils-cross-x86_64 = "87d4632d36323091e731eb07b8aa65f90293da66"
   SRCREV_glibc:pn-cross-localedef-native = "24962427071fa532c3c48c918e9d64d719cc8a6c"
   SRCREV_localedef:pn-cross-localedef-native = "794da69788cbf9bf57b59a852f9f11307663fa87"
   SRCREV:pn-debianutils-native = "de14223e5bffe15e374a441302c528ffc1cbed57"
   SRCREV:pn-libmodulemd-native = "ee80309bc766d781a144e6879419b29f444d94eb"
   SRCREV:pn-virglrenderer-native = "363915595e05fb252e70d6514be2f0c0b5ca312b"
   SRCREV:pn-zstd-native = "e47e674cd09583ff0503f0f6defd6d23d8b718d3"

> **Note:**
> Here are some notes on using the `buildhistory-collect-srcrevs` command:
>
> -  By default, only values where the **SRCREV** was not hardcoded
> (usually when **AUTOREV** is used) are reported. Use the `-a`
> option to see all **SRCREV** values.
>
> -  The output statements might not have any effect if overrides are
> applied elsewhere in the build system configuration. Use the
> `-f` option to add the `forcevariable` override to each output
> line if you need to work around this restriction.
>
> -  The script does apply special handling when building for multiple
> machines. However, the script does place a comment before each set
> of values that specifies which triplet to which they belong as
> previously shown (e.g., `i586-poky-linux`).
>

### Build History Image Information

The files produced for each image are as follows:

-  `image-files:` A directory containing selected files from the root
   filesystem. The files are defined by
   **BUILDHISTORY_IMAGE_FILES**.

-  `build-id.txt:` Human-readable information about the build
   configuration and metadata source revisions. This file contains the
   full build header as printed by BitBake.

-  `*.dot:` Dependency graphs for the image that are compatible with
   `graphviz`.

-  `files-in-image.txt:` A list of files in the image with
   permissions, owner, group, size, and symlink information.

-  `image-info.txt:` A text file containing name-value pairs with
   information about the image. See the following listing example for
   more information.

-  `installed-package-names.txt:` A list of installed packages by name
   only.

-  `installed-package-sizes.txt:` A list of installed packages ordered
   by size.

-  `installed-packages.txt:` A list of installed packages with full
   package filenames.

> **Note:**
> Installed package information is able to be gathered and produced
> even if package management is disabled for the final image.
>

Here is an example of `image-info.txt`:

```none
DISTRO = poky
DISTRO_VERSION = 3.4+snapshot-a0245d7be08f3d24ea1875e9f8872aa6bbff93be
USER_CLASSES = buildstats
IMAGE_CLASSES = qemuboot qemuboot license_image
IMAGE_FEATURES = allow-empty-password empty-root-password allow-root-login post-install-logging
IMAGE_LINGUAS =
IMAGE_INSTALL = packagegroup-core-boot speex speexdsp
BAD_RECOMMENDATIONS =
NO_RECOMMENDATIONS =
PACKAGE_EXCLUDE =
ROOTFS_POSTPROCESS_COMMAND = write_package_manifest; license_create_manifest; cve_check_write_rootfs_manifest;   ssh_allow_empty_password;  ssh_allow_root_login;  postinst_enable_logging;  rootfs_update_timestamp;   write_image_test_data;   empty_var_volatile;   sort_passwd; rootfs_reproducible;
IMAGE_POSTPROCESS_COMMAND =  buildhistory_get_imageinfo ;
IMAGESIZE = 9265
```

Other than `IMAGESIZE`,
which is the total size of the files in the image in Kbytes, the
name-value pairs are variables that may have influenced the content of
the image. This information is often useful when you are trying to
determine why a change in the package or file listings has occurred.

### Using Build History to Gather Image Information Only

As you can see, build history produces image information, including
dependency graphs, so you can see why something was pulled into the
image. If you are just interested in this information and not interested
in collecting specific package or SDK information, you can enable
writing only image information without any history by adding the
following to your `conf/local.conf` file found in the
**Build Directory**::

   INHERIT += "buildhistory"
   BUILDHISTORY_COMMIT = "0"
   BUILDHISTORY_FEATURES = "image"

Here, you set the
**BUILDHISTORY_FEATURES**
variable to use the image feature only.

### Build History SDK Information

Build history collects similar information on the contents of SDKs (e.g.
`bitbake -c populate_sdk imagename`) as compared to information it
collects for images. Furthermore, this information differs depending on
whether an extensible or standard SDK is being produced.

The following list shows the files produced for SDKs:

-  `files-in-sdk.txt:` A list of files in the SDK with permissions,
   owner, group, size, and symlink information. This list includes both
   the host and target parts of the SDK.

-  `sdk-info.txt:` A text file containing name-value pairs with
   information about the SDK. See the following listing example for more
   information.

-  `sstate-task-sizes.txt:` A text file containing name-value pairs
   with information about task group sizes (e.g. ref-tasks-populate_sysroot
   tasks have a total size). The `sstate-task-sizes.txt` file exists
   only when an extensible SDK is created.

-  `sstate-package-sizes.txt:` A text file containing name-value pairs
   with information for the shared-state packages and sizes in the SDK.
   The `sstate-package-sizes.txt` file exists only when an extensible
   SDK is created.

-  `sdk-files:` A folder that contains copies of the files mentioned
   in `BUILDHISTORY_SDK_FILES` if the files are present in the output.
   Additionally, the default value of `BUILDHISTORY_SDK_FILES` is
   specific to the extensible SDK although you can set it differently if
   you would like to pull in specific files from the standard SDK.

   The default files are `conf/local.conf`, `conf/bblayers.conf`,
   `conf/auto.conf`, `conf/locked-sigs.inc`, and
   `conf/devtool.conf`. Thus, for an extensible SDK, these files get
   copied into the `sdk-files` directory.

-  The following information appears under each of the `host` and
   `target` directories for the portions of the SDK that run on the
   host and on the target, respectively:

   .. note::

      The following files for the most part are empty when producing an
      extensible SDK because this type of SDK is not constructed from
      packages as is the standard SDK.

   -  `depends.dot:` Dependency graph for the SDK that is compatible
      with `graphviz`.

   -  `installed-package-names.txt:` A list of installed packages by
      name only.

   -  `installed-package-sizes.txt:` A list of installed packages
      ordered by size.

   -  `installed-packages.txt:` A list of installed packages with full
      package filenames.

Here is an example of `sdk-info.txt`:

```none
DISTRO = poky
DISTRO_VERSION = 1.3+snapshot-20130327
SDK_NAME = poky-glibc-i686-arm
SDK_VERSION = 1.3+snapshot
SDKMACHINE =
SDKIMAGE_FEATURES = dev-pkgs dbg-pkgs
BAD_RECOMMENDATIONS =
SDKSIZE = 352712
```

Other than `SDKSIZE`, which is
the total size of the files in the SDK in Kbytes, the name-value pairs
are variables that might have influenced the content of the SDK. This
information is often useful when you are trying to determine why a
change in the package or file listings has occurred.

### Examining Build History Information

You can examine build history output from the command line or from a web
interface.

To see any changes that have occurred (assuming you have
**BUILDHISTORY_COMMIT** = "1"),
you can simply use any Git command that allows you to view the history
of a repository. Here is one method::

   $ git log -p

You need to realize,
however, that this method does show changes that are not significant
(e.g. a package's size changing by a few bytes).

There is a command-line tool called `buildhistory-diff`, though,
that queries the Git repository and prints just the differences that
might be significant in human-readable form. Here is an example::

   $ bitbake-builds/layers/openembedded-core/scripts/buildhistory-diff . HEAD^
   Changes to images/qemux86_64/glibc/core-image-minimal (files-in-image.txt):
      /etc/anotherpkg.conf was added
      /sbin/anotherpkg was added
      * (installed-package-names.txt):
      *   anotherpkg was added
   Changes to images/qemux86_64/glibc/core-image-minimal (installed-package-names.txt):
      anotherpkg was added
   packages/qemux86_64-poky-linux/v86d: PACKAGES: added "v86d-extras"
      * PR changed from "r0" to "r1"
      * PV changed from "0.1.10" to "0.1.12"
   packages/qemux86_64-poky-linux/v86d/v86d: PKGSIZE changed from 110579 to 144381 (+30%)
      * PR changed from "r0" to "r1"
      * PV changed from "0.1.10" to "0.1.12"

> **Note:**
> The `buildhistory-diff` tool requires the `GitPython`
> package. Be sure to install it using Pip3 as follows::
>
> $ pip3 install GitPython --user
>
>
> Alternatively, you can install `python3-git` using the appropriate
> distribution package manager (e.g. `apt`, `dnf`, or `zipper`).
>

To see changes to the build history using a web interface, follow the
instruction in the `README` file
here .

Here is a sample screenshot of the interface:

![buildhistory-web.png](figures/buildhistory-web.png)

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Working with Packages

This section describes a few tasks that involve packages:

-  dev-manual/packages:excluding packages from an image

-  dev-manual/packages:incrementing a package version

-  dev-manual/packages:handling optional module packaging

-  dev-manual/packages:using runtime package management

-  dev-manual/packages:generating and using signed packages

-  :ref:`Setting up and running package test
   (ptest) <test-manual/ptest:testing packages with ptest>`

-  (**disabled**) dev-manual/packages:creating node package manager (npm) packages

-  dev-manual/packages:adding custom metadata to packages

## Excluding Packages from an Image

You might find it necessary to prevent specific packages from being
installed into an image. If so, you can use several variables to direct
the build system to essentially ignore installing recommended packages
or to not install a package at all.

The following list introduces variables you can use to prevent packages
from being installed into your image. Each of these variables only works
with IPK and RPM package types, not for Debian packages.
Also, you can use these variables from your `local.conf` file
or attach them to a specific image recipe by using a recipe name
override. For more detail on the variables, see the descriptions in the
Yocto Project Reference Manual's glossary chapter.

-  **BAD_RECOMMENDATIONS**:
   Use this variable to specify "recommended-only" packages that you do
   not want installed.

-  **NO_RECOMMENDATIONS**:
   Use this variable to prevent all "recommended-only" packages from
   being installed.

-  **PACKAGE_EXCLUDE**:
   Use this variable to prevent specific packages from being installed
   regardless of whether they are "recommended-only" or not. You need to
   realize that the build process could fail with an error when you
   prevent the installation of a package whose presence is required by
   an installed package.

## Incrementing a Package Version

This section provides some background on how binary package versioning
is accomplished and presents some of the services, variables, and
terminology involved.

In order to understand binary package versioning, you need to consider
the following:

-  Binary Package: The binary package that is eventually built and
   installed into an image.

-  Binary Package Version: The binary package version is composed of two
   components --- a version and a revision.

   .. note::

      Technically, a third component, the "epoch" (i.e. **PE**) is involved
      but this discussion for the most part ignores **PE**.

   The version and revision are taken from the
   **PV** and
   **PR** variables, respectively.

-  **PV**: The recipe version. **PV** represents the version of the
   software being packaged. Do not confuse **PV** with the binary
   package version.

-  **PR**: The recipe revision.

-  PR Service : A
   network-based service that helps automate keeping package feeds
   compatible with existing package manager applications such as RPM,
   APT, and OPKG.

Whenever the binary package content changes, the binary package version
must change. Changing the binary package version is accomplished by
changing or "bumping" the **PR** and/or **PV** values. Increasing these
values occurs one of two ways:

-  Automatically using a Package Revision Service (PR Service).

-  Manually incrementing the **PR** and/or **PV** variables.

Given a primary challenge of any build system and its users is how to
maintain a package feed that is compatible with existing package manager
applications such as RPM, APT, and OPKG, using an automated system is
much preferred over a manual system. In either system, the main
requirement is that binary package version numbering increases in a
linear fashion and that there is a number of version components that
support that linear progression. For information on how to ensure
package revisioning remains linear, see the
"dev-manual/packages:automatically incrementing a package version number"
section.

The following three sections provide related information on the PR
Service, the manual method for "bumping" **PR** and/or **PV**, and on
how to ensure binary package revisioning remains linear.

### Working With a PR Service

As mentioned, attempting to maintain revision numbers in the
**Metadata** is error prone, inaccurate,
and causes problems for people submitting recipes. Conversely, the PR
Service automatically generates increasing numbers, particularly the
revision field, which removes the human element.

> **Note:**
> For additional information on using a PR Service, you can see the
> PR Service  wiki page.
>

The Yocto Project uses variables in order of decreasing priority to
facilitate revision numbering (i.e.
**PE**,
**PV**, and
**PR** for epoch, version, and
revision, respectively). The values are highly dependent on the policies
and procedures of a given distribution and package feed.

Because the OpenEmbedded build system uses
"signatures ", which are
unique to a given build, the build system knows when to rebuild
packages. All the inputs into a given task are represented by a
signature, which can trigger a rebuild when different. Thus, the build
system itself does not rely on the **PR**, **PV**, and **PE** numbers to
trigger a rebuild. The signatures, however, can be used to generate
these values.

The PR Service works with both `OEBasic` and `OEBasicHash`
generators. The value of **PR** bumps when the checksum changes and the
different generator mechanisms change signatures under different
circumstances.

As implemented, the build system includes values from the PR Service
into the **PR** field as an addition using the form "`.x`" so `r0`
becomes `r0.1`, `r0.2` and so forth. This scheme allows existing
**PR** values to be used for whatever reasons, which include manual
**PR** bumps, should it be necessary.

By default, the PR Service is not enabled or running. Thus, the packages
generated are just "self consistent". The build system adds and removes
packages and there are no guarantees about upgrade paths but images will
be consistent and correct with the latest changes.

The simplest form for a PR Service is for a single host development system
that builds the package feed (building system). For this scenario, you can
enable a local PR Service by setting **PRSERV_HOST** in your
`local.conf` file in the **Build Directory**::

   PRSERV_HOST = "localhost:0"

Once the service is started, packages will automatically
get increasing **PR** values and BitBake takes care of starting and
stopping the server.

If you have a more complex setup where multiple host development systems
work against a common, shared package feed, you have a single PR Service
running and it is connected to each building system. For this scenario,
you need to start the PR Service using the `bitbake-prserv` command::

   bitbake-prserv --host ip --port port --start

In addition to
hand-starting the service, you need to update the `local.conf` file of
each building system as described earlier so each system points to the
server and port.

It is also recommended you use build history, which adds some sanity
checks to binary package versions, in conjunction with the server that
is running the PR Service. To enable build history, add the following to
each building system's `local.conf` file::

   # It is recommended to activate "buildhistory" for testing the PR service
   INHERIT += "buildhistory"
   BUILDHISTORY_COMMIT = "1"

For information on build
history, see the
"dev-manual/build-quality:maintaining build output quality" section.

> **Note:**
> The OpenEmbedded build system does not maintain **PR** information as
> part of the shared state (sstate) packages. If you maintain an sstate
> feed, it's expected that either all your building systems that
> contribute to the sstate feed use a shared PR service, or you do not
> run a PR service on any of your building systems.
>
> That's because if you had multiple machines sharing a PR service but
> not their sstate feed, you could end up with "diverging" hashes for
> the same output artefacts. When presented to the share PR service,
> each would be considered as new and would increase the revision
> number, causing many unnecessary package upgrades.
>
> For more information on shared state, see the
> "overview-manual/concepts:shared state cache"
> section in the Yocto Project Overview and Concepts Manual.
>

### Manually Bumping PR

The alternative to setting up a PR Service is to manually "bump" the
**PR** variable.

If a committed change results in changing the package output, then the
value of the **PR** variable needs to be increased (or "bumped") as part of
that commit. For new recipes you should add the **PR** variable and set
its initial value equal to "r0", which is the default. Even though the
default value is "r0", the practice of adding it to a new recipe makes
it harder to forget to bump the variable when you make changes to the
recipe in future.

Usually, version increases occur only to binary packages. However, if
for some reason **PV** changes but does not increase, you can increase
the **PE** variable (Package Epoch). The **PE** variable defaults to
"0".

Binary package version numbering strives to follow the `Debian Version
Field Policy
Guidelines <https://www.debian.org/doc/debian-policy/ch-controlfields.html>`__.
These guidelines define how versions are compared and what "increasing"
a version means.

### Automatically Incrementing a Package Version Number

When fetching a repository, BitBake uses the
**SRCREV** variable to determine
the specific source code revision from which to build. You set the
**SRCREV** variable to
**AUTOREV** to cause the
OpenEmbedded build system to automatically use the latest revision of
the software::

   SRCREV = "${AUTOREV}"

Furthermore, you need to include a `+` sign in **PV** in order to
automatically update the version whenever the revision of the source
code changes. Here is an example::

   PV = "1.0+git"

The OpenEmbedded build system will automatically add the source control
information to the end of the variable **PKGV**, in this format::

   AUTOINC+source_code_revision

The build system replaces the `AUTOINC`
with a number. The number used depends on the state of the PR Service:

-  If PR Service is enabled, the build system increments the number,
   which is similar to the behavior of
   **PR**. This behavior results in
   linearly increasing package versions, which is desirable. Here is an
   example:

   .. code-block:: none

      hello-world-git_1.0+git0+b6558dd387-r0.0_armv7a-neon.ipk
      hello-world-git_1.0+git1+dd2f5c3565-r0.1_armv7a-neon.ipk

   Two numbers got incremented here:

   -  `gitX` changed from `git0` to `git1`. This is because there was a
      change in the source code (`SRCREV`).

   -  `r0.X` changed from `r0.0` to `r0.1`. This is because the hash of
      the ref-tasks-package task changed.

      The reason for this change can be many. To understand why the hash of the
      ref-tasks-package task changed, you can run the following command:

      .. code-block:: console

         $ bitbake-diffsigs -t hello-world package

-  If PR Service is not enabled, the build system replaces the
   `AUTOINC` placeholder with zero (i.e. "0"). This results in
   changing the package version since the source revision is included.
   However, package versions are not increased linearly. Here is an
   example:

   .. code-block:: none

      hello-world-git_1.0+git0+b6558dd387-r0_armv7a-neon.ipk
      hello-world-git_1.0+git0+dd2f5c3565-r0_armv7a-neon.ipk

In summary, the OpenEmbedded build system does not track the history of
binary package versions for this purpose. `AUTOINC`, in this case, is
comparable to **PR**. If PR server is not enabled, `AUTOINC` in the
package version is simply replaced by "0". If PR server is enabled, the
build system keeps track of the package versions and bumps the number
when the package revision changes.

## Handling Optional Module Packaging

Many pieces of software split functionality into optional modules (or
plugins) and the plugins that are built might depend on configuration
options. To avoid having to duplicate the logic that determines what
modules are available in your recipe or to avoid having to package each
module by hand, the OpenEmbedded build system provides functionality to
handle module packaging dynamically.

To handle optional module packaging, you need to do two things:

-  Ensure the module packaging is actually done.

-  Ensure that any dependencies on optional modules from other recipes
   are satisfied by your recipe.

### Making Sure the Packaging is Done

To ensure the module packaging actually gets done, you use the
`do_split_packages` function within the `populate_packages` Python
function in your recipe. The `do_split_packages` function searches for
a pattern of files or directories under a specified path and creates a
package for each one it finds by appending to the
**PACKAGES** variable and
setting the appropriate values for `FILES:packagename`,
`RDEPENDS:packagename`, `DESCRIPTION:packagename`, and so forth.
Here is an example from the `lighttpd` recipe::

   python populate_packages:prepend () {
       lighttpd_libdir = d.expand('${libdir}')
       do_split_packages(d, lighttpd_libdir, '^mod_(.*).so$',
                        'lighttpd-module-%s', 'Lighttpd module for %s',
                         extra_depends='')
   }

The previous example specifies a number of things in the call to
`do_split_packages`.

-  A directory within the files installed by your recipe through
   ref-tasks-install in which to search.

-  A regular expression used to match module files in that directory. In
   the example, note the parentheses () that mark the part of the
   expression from which the module name should be derived.

-  A pattern to use for the package names.

-  A description for each package.

-  An empty string for `extra_depends`, which disables the default
   dependency on the main `lighttpd` package. Thus, if a file in
   `${libdir}` called `mod_alias.so` is found, a package called
   `lighttpd-module-alias` is created for it and the
   **DESCRIPTION** is set to
   "Lighttpd module for alias".

Often, packaging modules is as simple as the previous example. However,
there are more advanced options that you can use within
`do_split_packages` to modify its behavior. And, if you need to, you
can add more logic by specifying a hook function that is called for each
package. It is also perfectly acceptable to call `do_split_packages`
multiple times if you have more than one set of modules to package.

For more examples that show how to use `do_split_packages`, see the
`connman.inc` file in the :oe_git:`meta/recipes-connectivity/connman/
</openembedded-core/tree/meta/recipes-connectivity/connman/>`
directory of **OpenEmbedded-Core (OE-Core)**. You can
also find examples in `meta/classes-recipe/kernel.bbclass`.

Here is a reference that shows `do_split_packages` mandatory and
optional arguments::

   Mandatory arguments

   root
      The path in which to search
   file_regex
      Regular expression to match searched files.
      Use parentheses () to mark the part of this
      expression that should be used to derive the
      module name (to be substituted where %s is
      used in other function arguments as noted below)
   output_pattern
      Pattern to use for the package names. Must
      include %s.
   description
      Description to set for each package. Must
      include %s.

   Optional arguments

   postinst
      Postinstall script to use for all packages
      (as a string)
   recursive
      True to perform a recursive search --- default
      False
   hook
      A hook function to be called for every match.
      The function will be called with the following
      arguments (in the order listed):

      f
         Full path to the file/directory match
      pkg
         The package name
      file_regex
         As above
      output_pattern
         As above
      modulename
         The module name derived using file_regex
   extra_depends
      Extra runtime dependencies (RDEPENDS) to be
      set for all packages. The default value of None
      causes a dependency on the main package
      (${PN}) --- if you do not want this, pass empty
      string '' for this parameter.
   aux_files_pattern
      Extra item(s) to be added to FILES for each
      package. Can be a single string item or a list
      of strings for multiple items. Must include %s.
   postrm
      postrm script to use for all packages (as a
      string)
   allow_dirs
      True to allow directories to be matched -
      default False
   prepend
      If True, prepend created packages to PACKAGES
      instead of the default False which appends them
   match_path
      match file_regex on the whole relative path to
      the root rather than just the filename
   aux_files_pattern_verbatim
      Extra item(s) to be added to FILES for each
      package, using the actual derived module name
      rather than converting it to something legal
      for a package name. Can be a single string item
      or a list of strings for multiple items. Must
      include %s.
   allow_links
      True to allow symlinks to be matched --- default
      False
   summary
      Summary to set for each package. Must include %s;
      defaults to description if not set.

### Satisfying Dependencies

The second part for handling optional module packaging is to ensure that
any dependencies on optional modules from other recipes are satisfied by
your recipe. You can be sure these dependencies are satisfied by using
the **PACKAGES_DYNAMIC**
variable. Here is an example that continues with the `lighttpd` recipe
shown earlier::

   PACKAGES_DYNAMIC = "lighttpd-module-.*"

The name
specified in the regular expression can of course be anything. In this
example, it is `lighttpd-module-` and is specified as the prefix to
ensure that any **RDEPENDS** and
**RRECOMMENDS** on a package
name starting with the prefix are satisfied during build time. If you
are using `do_split_packages` as described in the previous section,
the value you put in **PACKAGES_DYNAMIC** should correspond to the name
pattern specified in the call to `do_split_packages`.

## Using Runtime Package Management

During a build, BitBake always transforms a recipe into one or more
packages. For example, BitBake takes the `bash` recipe and produces a
number of packages (e.g. `bash`, `bash-bashbug`,
`bash-completion`, `bash-completion-dbg`, `bash-completion-dev`,
`bash-completion-extra`, `bash-dbg`, and so forth). Not all
generated packages are included in an image.

In several situations, you might need to update, add, remove, or query
the packages on a target device at runtime (i.e. without having to
generate a new image). Examples of such situations include:

-  You want to provide in-the-field updates to deployed devices (e.g.
   security updates).

-  You want to have a fast turn-around development cycle for one or more
   applications that run on your device.

-  You want to temporarily install the "debug" packages of various
   applications on your device so that debugging can be greatly improved
   by allowing access to symbols and source debugging.

-  You want to deploy a more minimal package selection of your device
   but allow in-the-field updates to add a larger selection for
   customization.

In all these situations, you have something similar to a more
traditional Linux distribution in that in-field devices are able to
receive pre-compiled packages from a server for installation or update.
Being able to install these packages on a running, in-field device is
what is termed "runtime package management".

In order to use runtime package management, you need a host or server
machine that serves up the pre-compiled packages plus the required
metadata. You also need package manipulation tools on the target. The
build machine is a likely candidate to act as the server. However, that
machine does not necessarily have to be the package server. The build
machine could push its artifacts to another machine that acts as the
server (e.g. Internet-facing). In fact, doing so is advantageous for a
production environment as getting the packages away from the development
system's **Build Directory** prevents accidental overwrites.

A simple build that targets just one device produces more than one
package database. In other words, the packages produced by a build are
separated out into a couple of different package groupings based on
criteria such as the target's CPU architecture, the target board, or the
C library used on the target. For example, a build targeting the
`qemux86` device produces the following three package databases:
`noarch`, `i586`, and `qemux86`. If you wanted your `qemux86`
device to be aware of all the packages that were available to it, you
would need to point it to each of these databases individually. In a
similar way, a traditional Linux distribution usually is configured to
be aware of a number of software repositories from which it retrieves
packages.

Using runtime package management is completely optional and not required
for a successful build or deployment in any way. But if you want to make
use of runtime package management, you need to do a couple things above
and beyond the basics. The remainder of this section describes what you
need to do.

### Build Considerations

This section describes build considerations of which you need to be
aware in order to provide support for runtime package management.

When BitBake generates packages, it needs to know what format or formats
to use. In your configuration, you use the
**PACKAGE_CLASSES**
variable to specify the format:

#. Open the `local.conf` file inside your **Build Directory** (e.g.
   `bitbake-builds/build/conf/local.conf`).

#. Select the desired package format as follows::

      PACKAGE_CLASSES ?= "package_packageformat"

   where packageformat can be "ipk", "rpm", or
   "deb" which are the supported package formats.

   .. note::

      Because the Yocto Project supports three different package formats,
      you can set the variable with more than one argument. However, the
      OpenEmbedded build system only uses the first argument when
      creating an image or Software Development Kit (SDK).

If you would like your image to start off with a basic package database
containing the packages in your current build as well as to have the
relevant tools available on the target for runtime package management,
you can include "package-management" in the
**IMAGE_FEATURES**
variable. Including "package-management" in this configuration variable
ensures that when the image is assembled for your target, the image
includes the currently-known package databases as well as the
target-specific tools required for runtime package management to be
performed on the target. However, this is not strictly necessary. You
could start your image off without any databases but only include the
required on-target package tool(s). As an example, you could include
"opkg" in your
**IMAGE_INSTALL** variable
if you are using the IPK package format. You can then initialize your
target's package database(s) later once your image is up and running.

Whenever you perform any sort of build step that can potentially
generate a package or modify existing package, it is always a good idea
to re-generate the package index after the build by using the following
command::

   $ bitbake package-index

It might be tempting to build the
package and the package index at the same time with a command such as
the following::

   $ bitbake some-package package-index

Do not do this as
BitBake does not schedule the package index for after the completion of
the package you are building. Consequently, you cannot be sure of the
package index including information for the package you just built.
Thus, be sure to run the package update step separately after building
any packages.

You can use the
**PACKAGE_FEED_ARCHS**,
**PACKAGE_FEED_BASE_PATHS**,
and
**PACKAGE_FEED_URIS**
variables to pre-configure target images to use a package feed. If you
do not define these variables, then manual steps as described in the
subsequent sections are necessary to configure the target. You should
set these variables before building the image in order to produce a
correctly configured image.

> **Note:**
> Your image will need enough free storage space to run package upgrades,
> especially if many of them need to be downloaded at the same time.
> You should make sure images are created with enough free space
> by setting the **IMAGE_ROOTFS_EXTRA_SPACE** variable.
>

When your build is complete, your packages reside in the
`${TMPDIR}/deploy/packageformat` directory. For example, if
`${`\ **TMPDIR**\ `}` is
`tmp` and your selected package type is RPM, then your RPM packages
are available in `tmp/deploy/rpm`.

### Host or Server Machine Setup

Although other protocols are possible, a server using HTTP typically
serves packages. If you want to use HTTP, then set up and configure a
web server such as Apache 2, lighttpd, or Python web server on the
machine serving the packages.

To keep things simple, this section describes how to set up a
Python web server to share package feeds from the developer's
machine. Although this server might not be the best for a production
environment, the setup is simple and straight forward. Should you want
to use a different server more suited for production (e.g. Apache 2,
Lighttpd, or Nginx), take the appropriate steps to do so.

From within the **Build Directory** where you have built an image based on
your packaging choice (i.e. the **PACKAGE_CLASSES** setting), simply start
the server. The following example assumes a **Build Directory** of `bitbake-builds/build`
and a **PACKAGE_CLASSES** setting of "ref-classes-package_rpm"::

   $ cd bitbake-builds/build/tmp/deploy/rpm
   $ python3 -m http.server

### Target Setup

Setting up the target differs depending on the package management
system. This section provides information for RPM, IPK, and DEB.

#### Using RPM

The Dandified Packaging  (DNF) performs
runtime package management of RPM packages. In order to use DNF for
runtime package management, you must perform an initial setup on the
target machine for cases where the `PACKAGE_FEED_*` variables were not
set as part of the image that is running on the target. This means if
you built your image and did not use these variables as part of the
build and your image is now running on the target, you need to perform
the steps in this section if you want to use runtime package management.

> **Note:**
> For information on the `PACKAGE_FEED_*` variables, see
> **PACKAGE_FEED_ARCHS**, **PACKAGE_FEED_BASE_PATHS**, and
> **PACKAGE_FEED_URIS** in the Yocto Project Reference Manual variables
> glossary.
>

On the target, you must inform DNF that package databases are available.
You do this by creating a file named
`/etc/yum.repos.d/oe-packages.repo` and defining the `oe-packages`.

As an example, assume the target is able to use the following package
databases: `all`, `i586`, and `qemux86` from a server named
`my.server`. The specifics for setting up the web server are up to
you. The critical requirement is that the URIs in the target repository
configuration point to the correct remote location for the feeds.

> **Note:**
> For development purposes, you can point the web server to the build
> system's `deploy` directory. However, for production use, it is better to
> copy the package directories to a location outside of the build area and use
> that location. Doing so avoids situations where the build system
> overwrites or changes the `deploy` directory.
>

When telling DNF where to look for the package databases, you must
declare individual locations per architecture or a single location used
for all architectures. You cannot do both:

-  *Create an Explicit List of Architectures:* Define individual base
   URLs to identify where each package database is located:

   .. code-block:: none

      [oe-packages]
      baseurl=http://my.server/rpm/i586  http://my.server/rpm/qemux86 http://my.server/rpm/all

   This example
   informs DNF about individual package databases for all three
   architectures.

-  *Create a Single (Full) Package Index:* Define a single base URL that
   identifies where a full package database is located::

      [oe-packages]
      baseurl=http://my.server/rpm

   This example informs DNF about a single
   package database that contains all the package index information for
   all supported architectures.

Once you have informed DNF where to find the package databases, you need
to fetch them:

```none
# dnf makecache
```

DNF is now able to find, install, and
upgrade packages from the specified repository or repositories.

> **Note:**
> See the `DNF documentation <https://dnf.readthedocs.io/en/latest/>`__ for
> additional information.
>

#### Using IPK

The `opkg` application performs runtime package management of IPK
packages. You must perform an initial setup for `opkg` on the target
machine if the
**PACKAGE_FEED_ARCHS**,
**PACKAGE_FEED_BASE_PATHS**,
and
**PACKAGE_FEED_URIS**
variables have not been set or the target image was built before the
variables were set.

The `opkg` application uses configuration files to find available
package databases. Thus, you need to create a configuration file inside
the `/etc/opkg/` directory, which informs `opkg` of any repository
you want to use.

As an example, suppose you are serving packages from a `ipk/`
directory containing the `i586`, `all`, and `qemux86` databases
through an HTTP server named `my.server`. On the target, create a
configuration file (e.g. `my_repo.conf`) inside the `/etc/opkg/`
directory containing the following:

```none
src/gz all http://my.server/ipk/all
src/gz i586 http://my.server/ipk/i586
src/gz qemux86 http://my.server/ipk/qemux86
```

Next, instruct `opkg` to fetch the
repository information:

```none
# opkg update
```

The `opkg` application is now able to find, install, and upgrade packages
from the specified repository.

#### Using DEB

The `apt` application performs runtime package management of DEB
packages. This application uses a source list file to find available
package databases. You must perform an initial setup for `apt` on the
target machine if the
**PACKAGE_FEED_ARCHS**,
**PACKAGE_FEED_BASE_PATHS**,
and
**PACKAGE_FEED_URIS**
variables have not been set or the target image was built before the
variables were set.

To inform `apt` of the repository you want to use, you might create a
list file (e.g. `my_repo.list`) inside the
`/etc/apt/sources.list.d/` directory. As an example, suppose you are
serving packages from a `deb/` directory containing the `i586`,
`all`, and `qemux86` databases through an HTTP server named
`my.server`. The list file should contain:

```none
deb http://my.server/deb/all ./
deb http://my.server/deb/i586 ./
deb http://my.server/deb/qemux86 ./
```

Next, instruct the `apt` application
to fetch the repository information:

```none
```

  $ sudo apt update

After this step,
`apt` is able to find, install, and upgrade packages from the
specified repository.

## Generating and Using Signed Packages

In order to add security to RPM packages used during a build, you can
take steps to securely sign them. Once a signature is verified, the
OpenEmbedded build system can use the package in the build. If security
fails for a signed package, the build system stops the build.

This section describes how to sign RPM packages during a build and how
to use signed package feeds (repositories) when doing a build.

### Signing RPM Packages

To enable signing RPM packages, you must modify the `rpm`
recipe configuration to include support for OpenPGP signing.
That may be done either in a `.bbappend` for the `rpm` recipe::

   PACKAGECONFIG:append = " sequoia"

or in a **Configuration File**::

   PACKAGECONFIG:append:pn-rpm-native = " sequoia"
   PACKAGECONFIG:append:pn-rpm = " sequoia"

You must also set up the following settings in a
**Configuration File**::

   # Inherit sign_rpm.bbclass to enable signing functionality
   INHERIT += " sign_rpm"
   # Define the GPG key that will be used for signing.
   RPM_GPG_NAME = "key_name"
   # Provide passphrase for the key
   RPM_GPG_PASSPHRASE = "passphrase"

> **Note:**
> Be sure to supply appropriate values for both `key_name` and
> `passphrase`.
>

Aside from the `RPM_GPG_NAME` and `RPM_GPG_PASSPHRASE` variables in
the previous example, two optional variables related to signing are available:

-  *GPG_BIN:* Specifies a `gpg` binary/wrapper that is executed
   when the package is signed.

-  *GPG_PATH:* Specifies the `gpg` home directory used when the
   package is signed.

### Processing Package Feeds

In addition to being able to sign RPM packages, you can also enable
signed package feeds for IPK and RPM packages.

The steps you need to take to enable signed package feed use are similar
to the steps used to sign RPM packages. You must define the following in
your `local.config` or `distro.config` file::

   INHERIT += "sign_package_feed"
   PACKAGE_FEED_GPG_NAME = "key_name"
   PACKAGE_FEED_GPG_PASSPHRASE_FILE = "path_to_file_containing_passphrase"

For signed package feeds, the passphrase must be specified in a separate file,
which is pointed to by the `PACKAGE_FEED_GPG_PASSPHRASE_FILE`
variable. Regarding security, keeping a plain text passphrase out of the
configuration is more secure.

Aside from the `PACKAGE_FEED_GPG_NAME` and
`PACKAGE_FEED_GPG_PASSPHRASE_FILE` variables, three optional variables
related to signed package feeds are available:

-  *GPG_BIN* Specifies a `gpg` binary/wrapper that is executed
   when the package is signed.

-  *GPG_PATH:* Specifies the `gpg` home directory used when the
   package is signed.

-  *PACKAGE_FEED_GPG_SIGNATURE_TYPE:* Specifies the type of `gpg`
   signature. This variable applies only to RPM and IPK package feeds.
   Allowable values for the `PACKAGE_FEED_GPG_SIGNATURE_TYPE` are
   "ASC", which is the default and specifies ascii armored, and "BIN",
   which specifies binary.

## Testing Packages With ptest

See the test-manual/ptest:Testing Packages With ptest section of the
Yocto Project Test Environment Manual.

## Creating Node Package Manager (NPM) Packages

> **Warning:**
> The NPM fetcher is currently disabled due to security concerns. See
> `355cd226e0720a9ed7683bb01c8c0a58eee03664` for more information.
>
> This section is left there if it is re-enabled in the future, but is
> currently obsolete.
>

NPM  is a package manager for the JavaScript
programming language. The Yocto Project supports the NPM
fetcher .
You can use this fetcher in combination with
devtool  to create recipes that produce
NPM packages.

There are two workflows that allow you to create NPM packages using
`devtool`: the NPM registry modules method and the NPM project code
method.

> **Note:**
> While it is possible to create NPM recipes manually, using
> `devtool` is far simpler.
>

Additionally, some requirements and caveats exist.

### Requirements and Caveats

You need to be aware of the following before using `devtool` to create
NPM packages:

-  Of the two methods that you can use `devtool` to create NPM
   packages, the registry approach is slightly simpler. However, you
   might consider the project approach because you do not have to
   publish your module in the `NPM registry <https://docs.npmjs.com/misc/registry>`__,
   which is NPM's public registry.

-  Be familiar with
   devtool .

-  The NPM host tools need the native `nodejs-npm` package, which is
   part of the OpenEmbedded environment. You need to get the package by
   cloning the meta-openembedded 
   repository. Be sure to add the path to your local copy
   to your `bblayers.conf` file.

-  `devtool` cannot detect native libraries in module dependencies.
   Consequently, you must manually add packages to your recipe.

-  While deploying NPM packages, `devtool` cannot determine which
   dependent packages are missing on the target (e.g. the node runtime
   `nodejs`). Consequently, you need to find out what files are
   missing and be sure they are on the target.

-  Although you might not need NPM to run your node package, it is
   useful to have NPM on your target. The NPM package name is
   `nodejs-npm`.

### Using the Registry Modules Method

This section presents an example that uses the `cute-files` module,
which is a file browser web application.

> **Note:**
> You must know the `cute-files` module version.
>

The first thing you need to do is use `devtool` and the NPM fetcher to
create the recipe::

   $ devtool add "npm://registry.npmjs.org;package=cute-files;version=1.0.2"

The
`devtool add` command runs `recipetool create` and uses the same
fetch URI to download each dependency and capture license details where
possible. The result is a generated recipe.

After running for quite a long time, in particular building the
`nodejs-native` package, the command should end as follows::

   INFO: Recipe /home/.../build/workspace/recipes/cute-files/cute-files_1.0.2.bb has been automatically created; further editing may be required to make it fully functional

The recipe file is fairly simple and contains every license that
`recipetool` finds and includes the licenses in the recipe's
**LIC_FILES_CHKSUM**
variables. You need to examine the variables and look for those with
"unknown" in the **LICENSE**
field. You need to track down the license information for "unknown"
modules and manually add the information to the recipe.

`recipetool` creates a "shrinkwrap" file for your recipe. Shrinkwrap
files capture the version of all dependent modules. Many packages do not
provide shrinkwrap files but `recipetool` will create a shrinkwrap file as it
runs.

> **Note:**
> A package is created for each sub-module. This policy is the only
> practical way to have the licenses for all of the dependencies
> represented in the license manifest of the image.
>

The `devtool edit-recipe` command lets you take a look at the recipe::

   $ devtool edit-recipe cute-files
   # Recipe created by recipetool
   # This is the basis of a recipe and may need further editing in order to be fully functional.
   # (Feel free to remove these comments when editing.)

   SUMMARY = "Turn any folder on your computer into a cute file browser, available on the local network."
   # WARNING: the following LICENSE and LIC_FILES_CHKSUM values are best guesses - it is
   # your responsibility to verify that the values are complete and correct.
   #
   # NOTE: multiple licenses have been detected; they have been separated with &
   # in the LICENSE value for now since it is a reasonable assumption that all
   # of the licenses apply. If instead there is a choice between the multiple
   # licenses then you should change the value to separate the licenses with |
   # instead of &. If there is any doubt, check the accompanying documentation
   # to determine which situation is applicable.

   SUMMARY = "Turn any folder on your computer into a cute file browser, available on the local network."
   LICENSE = "BSD-3-Clause & ISC & MIT"
   LIC_FILES_CHKSUM = "file://LICENSE;md5=71d98c0a1db42956787b1909c74a86ca \
                       file://node_modules/accepts/LICENSE;md5=bf1f9ad1e2e1d507aef4883fff7103de \
                       file://node_modules/array-flatten/LICENSE;md5=44088ba57cb871a58add36ce51b8de08 \
   ...
                       file://node_modules/cookie-signature/Readme.md;md5=57ae8b42de3dd0c1f22d5f4cf191e15a"

   SRC_URI = " \
       npm://registry.npmjs.org/;package=cute-files;version=${PV} \
       npmsw://${THISDIR}/${BPN}/npm-shrinkwrap.json \
       "

   S = "${UNPACKDIR}/npm"

   inherit npm

   LICENSE:${PN} = "MIT"
   LICENSE:${PN}-accepts = "MIT"
   LICENSE:${PN}-array-flatten = "MIT"
   ...
   LICENSE:${PN}-vary = "MIT"

Three key points in the previous example are:

-  **SRC_URI** uses the NPM
   scheme so that the NPM fetcher is used.

-  `recipetool` collects all the license information. If a
   sub-module's license is unavailable, the sub-module's name appears in
   the comments.

-  The `inherit npm` statement causes the ref-classes-npm class to
   package up all the modules.

You can run the following command to build the `cute-files` package::

   $ devtool build cute-files

Remember that `nodejs` must be installed on
the target before your package.

Assuming 192.168.7.2 for the target's IP address, use the following
command to deploy your package::

   $ devtool deploy-target -s cute-files root@192.168.7.2

Once the package is installed on the target, you can
test the application to show the contents of any directory::

  $ cd /usr/lib/node_modules/cute-files
  $ cute-files

On a browser,
go to `http://192.168.7.2:3000` and you see the following:

![cute-files-npm-example.png](figures/cute-files-npm-example.png)

You can find the recipe in `workspace/recipes/cute-files`. You can use
the recipe in any layer you choose.

### Using the NPM Projects Code Method

Although it is useful to package modules already in the NPM registry,
adding `node.js` projects under development is a more common developer
use case.

This section covers the NPM projects code method, which is very similar
to the "registry" approach described in the previous section. In the NPM
projects method, you provide `devtool` with an URL that points to the
source files.

Replicating the same example, (i.e. `cute-files`) use the following
command::

   $ devtool add https://github.com/martinaglv/cute-files.git

The recipe this command generates is very similar to the recipe created in
the previous section. However, the **SRC_URI** looks like the following::

   SRC_URI = " \
       git://github.com/martinaglv/cute-files.git;protocol=https;branch=master \
       npmsw://${THISDIR}/${BPN}/npm-shrinkwrap.json \
       "

In this example,
the main module is taken from the Git repository and dependencies are
taken from the NPM registry. Other than those differences, the recipe is
basically the same between the two methods. You can build and deploy the
package exactly as described in the previous section that uses the
registry modules method.

## Adding custom metadata to packages

The variable
**PACKAGE_ADD_METADATA**
can be used to add additional metadata to packages. This is reflected in
the package control/spec file. To take the ipk format for example, the
CONTROL file stored inside would contain the additional metadata as
additional lines.

The variable can be used in multiple ways, including using suffixes to
set it for a specific package type and/or package. Note that the order
of precedence is the same as this list:

-  `PACKAGE_ADD_METADATA_<PKGTYPE>:<PN>`

-  `PACKAGE_ADD_METADATA_<PKGTYPE>`

-  `PACKAGE_ADD_METADATA:<PN>`

-  **PACKAGE_ADD_METADATA**

`<PKGTYPE>` is a parameter and expected to be a distinct name of specific
package type:

-  IPK for .ipk packages

-  DEB for .deb packages

-  RPM for .rpm packages

`<PN>` is a parameter and expected to be a package name.

The variable can contain multiple [one-line] metadata fields separated
by the literal sequence '\\n'. The separator can be redefined using the
variable flag `separator`.

Here is an example that adds two custom fields for ipk
packages::

   PACKAGE_ADD_METADATA_IPK = "Vendor: CustomIpk\nGroup:Applications/Spreadsheets"

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Creating a Software Bill of Materials

Once you are able to build an image for your project, once the licenses for
each software component are all identified (see
"dev-manual/licenses:working with licenses") and once vulnerability
fixes are applied (see ":ref:`security-manual/vulnerabilities:checking
for vulnerabilities`"), the OpenEmbedded build system can generate
a description of all the components you used, their licenses, their dependencies,
their sources, the changes that were applied to them and the known
vulnerabilities that were fixed.

This description is generated in the form of a *Software Bill of Materials*
(**SBOM**), using the **SPDX** standard.

When you release software, this is the most standard way to provide information
about the Software Supply Chain of your software image and SDK. The
**SBOM** tooling is often used to ensure open source license compliance by
providing the license texts used in the product which legal departments and end
users can read in standardized format.

**SBOM** information is also critical to performing vulnerability exposure
assessments, as all the components used in the Software Supply Chain are listed.

The OpenEmbedded build system generates such information by default (by
inheriting the ref-classes-create-spdx class in **INHERIT_DISTRO**).

If needed, it can be disabled from a **configuration file**::

   INHERIT_DISTRO:remove = "create-spdx"

Upon building an image, you will then get:

-  **SPDX** output in JSON format as an `IMAGE-MACHINE.spdx.json` file in
   `tmp/deploy/images/MACHINE/` inside the **Build Directory**.

-  This toplevel file is accompanied by an `IMAGE-MACHINE.spdx.index.json`
   containing an index of JSON **SPDX** files for individual recipes.

-  The compressed archive `IMAGE-MACHINE.spdx.tar.zst` contains the index
   and the files for the single recipes.

The ref-classes-create-spdx class offers options to include
more information in the output **SPDX** data:

-  Make the json files more human readable by setting (**SPDX_PRETTY**).

-  Add compressed archives of the files in the generated target packages by
   setting (**SPDX_ARCHIVE_PACKAGED**).

-  Add a description of the source files used to generate host tools and target
   packages (**SPDX_INCLUDE_SOURCES**)

-  Add a description of the **compiled** source files used to generate host tools
   and target packages (**SPDX_INCLUDE_COMPILED_SOURCES**)

-  Export the Linux kernel configuration (`CONFIG_*` parameters) into the
   SPDX document (**SPDX_INCLUDE_KERNEL_CONFIG**).

-  Export the recipe's `PACKAGECONFIG` features (enabled/disabled) into the
   SPDX document (**SPDX_INCLUDE_PACKAGECONFIG**).

-  Add archives of these source files themselves (**SPDX_ARCHIVE_SOURCES**).

-  Exclude specific files from the SPDX output using Python regular expressions
   (**SPDX_FILE_EXCLUDE_PATTERNS**).

-  Attach supplier information to the image SBOM, SDK SBOM, or individual
   packages (**SPDX_IMAGE_SUPPLIER**, **SPDX_SDK_SUPPLIER**,
   **SPDX_PACKAGE_SUPPLIER**).

-  Enrich source downloads with ecosystem-specific Package URLs (PURLs), using
   the ref-classes-cargo_common, ref-classes-go-mod,
   ref-classes-pypi, ref-classes-npm, and
   ref-classes-cpan classes to automatically populate PURL identifiers
   for the corresponding language ecosystems.

-  Record which agent invoked the build and on whose behalf it ran, enabling
   CI/CD traceability in the SBOM
   (**SPDX_INCLUDE_BITBAKE_PARENT_BUILD**, **SPDX_INVOKED_BY**,
   **SPDX_ON_BEHALF_OF**).

Though the toplevel **SPDX** output is available in
`tmp/deploy/images/MACHINE/` inside the **Build Directory**, ancillary
generated files are available in `tmp/deploy/spdx/MACHINE` too, such as:

-  The individual **SPDX** JSON files in the `IMAGE-MACHINE.spdx.tar.zst`
   archive.

-  Compressed archives of the files in the generated target packages,
   in `packages/packagename.tar.zst` (when **SPDX_ARCHIVE_PACKAGED**
   is set).

-  Compressed archives of the source files used to build the host tools
   and the target packages in `recipes/recipe-packagename.tar.zst`
   (when **SPDX_ARCHIVE_SOURCES** is set). Those are needed to fulfill
   "source code access" license requirements.

See also the **SPDX_CUSTOM_ANNOTATION_VARS** variable which allows
to associate custom notes to a recipe.
See the `tools page <https://spdx.dev/resources/tools/>`__ on the **SPDX**
project website for a list of tools to consume and transform the **SPDX**
data generated by the OpenEmbedded build system.

See also Joshua Watt's presentations
`Automated SBoM generation with OpenEmbedded and the Yocto Project <https://youtu.be/Q5UQUM6zxVU>`__
at FOSDEM 2023 and
`SPDX in the Yocto Project <https://fosdem.org/2024/schedule/event/fosdem-2024-3318-spdx-in-the-yocto-project/>`__
at FOSDEM 2024.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Setting up a Hash Equivalence Server

A overview-manual/concepts:Hash Equivalence server can help reduce build
times by using the mechanism described in the overview-manual/concepts:Hash Equivalence
section of the Yocto Project Overview and Concepts Manual.

This document will guide you through the steps required to set up the reference
Hash Equivalence server provided by the :oe_git:`bitbake-hashserv
</bitbake/tree/bin/bitbake-hashserv>` script in **BitBake**.

This guide will explain how to setup a local Hash Equivalence server and does
not explain how to setup the surrounding infrastructure to secure this server.

## Hash Equivalence Server Setup

From this point onwards, the commands displayed below are assumed to be run from
the **BitBake** repository, which can be found here .

To start a basic Hash Equivalence server, one could simply run::

   ./bin/bitbake-hashserv

This will take all of the default options of the script, which are already
sufficient to start a local server.

Run `./bin/bitbake-hashserv --help` to see what options the script can take.
Some of the important ones are:

-  `--database` controls the location of the hash server database (default:
   `./hashserver.db`).

-  `--bind` controls the bind address of the server (default:
   `unix://./hashserver.sock`).

   You can specify three types of addresses:

   -  `unix://PATH`: will bind to a Unix socket at `PATH`.

   -  `wss://ADDRESS:PORT`: will bind to a Websocket at `ADDRESS:PORT`.

   -  `ADDRESS:PORT`: will bind to a raw TCP socket at `ADDRESS:PORT`.

-  `--log` can be used to control the logging level of the server (e.g.
   `INFO` will print information about clients connection to the server).

-  `--upstream` can be used to specify an upstream server to pull hashes from.
   This has no default value, meaning no upstream server is used.

-  `--db-username` and `--db-password` can be used to control the access to
   the database.

-  `--read-only` will disable hash reports from clients.

These variables can also be set from the environment from where it is being run.
Run `./bin/bitbake-hashserv --help` to get the variable names that you can
export.

> **Warning:**
> The shared Hash Equivalence server needs to be maintained together with the
> Shared State  cache. Otherwise,
> the server could report Shared State hashes that only exist on specific
> clients.
>
> We therefore recommend that one Hash Equivalence server be set up to
> correspond with a given Shared State cache, and to start this server
> in *read-only mode* (`--read-only` option), so that it doesn't store
> equivalences for Shared State caches that are local to clients.
>
> If there is no pre-existing Shared State Cache, the server should allow
> hashes to be reported (no `--read-only` option) to create the initial
> Hash Equivalence database.
>

## Yocto Project Build Setup

To use the server started in the previous section, set the following variables
in a **configuration file**::

   BB_HASHSERVE = "<bind address>"
   BB_SIGNATURE_HANDLER = "OEEquivHash"

The `<bind address>` value should be replaced to point to the server started
in the previous section.

See the documentation of **BB_SIGNATURE_HANDLER** for more details on this
variable.

You can optionally specify an upstream server with **BB_HASHSERVE_UPSTREAM**
variable. For example::

   BB_HASHSERVE_UPSTREAM = "wss://hashserv.yoctoproject.org/ws"

This will make the local server pull hashes from the upstream server. The
**BB_HASHSERVE_UPSTREAM** only works when a server is running
(**BB_HASHSERVE** is set).

To output debugging information on what is happening with Hash Equivalence when
builds are started, you can configure **BitBake** logging as follows from a
**configuration file**::

   BB_LOGCONFIG = "hashequiv.json"

With `hashequiv.json` containing the following content:

```json
{
   "version": 1,
   "loggers": {
      "BitBake.SigGen.HashEquiv": {
         "level": "VERBOSE",
         "handlers": ["BitBake.verbconsole"]
      },
      "BitBake.RunQueue.HashEquiv": {
         "level": "VERBOSE",
         "handlers": ["BitBake.verbconsole"]
      }
   }
}
```

This will make Hash Equivalence related changes be printed on the console, such
as::

   NOTE: Task <recipe.bb>:do_<task> unihash changed to dc0da29c62a2d78d8d853fbb9c173778fe7d6fa4a68c67494b17afffe8ca1894

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Creating a Custom Template Configuration Directory

If you are producing your own customized version of the build system for
use by other users, you might want to provide a custom build configuration
that includes all the necessary settings and layers (i.e. `local.conf` and
`bblayers.conf` that are created in a new **Build Directory**) and a custom
message that is shown when setting up the build. This can be done by
creating one or more template configuration directories in your
custom distribution layer.

This can be done by using `bitbake-layers save-build-conf`::

   $ bitbake-layers save-build-conf ../../meta-alex/ test-1
   NOTE: Starting bitbake server...
   NOTE: Configuration template placed into /srv/bitbake-builds/layers/meta-alex/conf/templates/test-1
   Please review the files in there, and particularly provide a configuration description in /srv/bitbake-builds/meta-alex/conf/templates/test-1/conf-notes.txt
   You can try out the configuration with
   TEMPLATECONF=/srv/bitbake-builds/layers/meta-alex/conf/templates/test-1 . /srv/bitbake-builds/layers/openembedded-core/oe-init-build-env build-try-test-1

The above command takes the config files from the currently active **Build Directory** under `conf`,
replaces site-specific paths in `bblayers.conf` with `##OECORE##`-relative paths, and copies
the config files into a specified layer under a specified template name.

To use those saved templates as a starting point for a build, users should point
to one of them with **TEMPLATECONF** environment variable::

   TEMPLATECONF=/srv/bitbake-builds/layers/meta-alex/conf/templates/test-1 . /srv/bitbake-builds/layers/openembedded-core/oe-init-build-env build-try-test-1

The OpenEmbedded build system uses the environment variable
**TEMPLATECONF** to locate the directory from which it gathers
configuration information that ultimately ends up in the
**Build Directory** `conf` directory.

If **TEMPLATECONF** is not set, the default value is obtained
from `.templateconf` file that is read from the same directory as
`oe-init-build-env` script. For the Poky reference distribution this
would be::

   TEMPLATECONF=${TEMPLATECONF:-meta-poky/conf/templates/default}

If you look at a configuration template directory, you will
see the `bblayers.conf.sample`, `local.conf.sample`, `conf-summary.txt` and
`conf-notes.txt` files. The build system uses these files to form the
respective `bblayers.conf` file, `local.conf` file, and show
users usage information about the build they're setting up
when running the `oe-init-build-env` setup script. These can be
edited further if needed to improve or change the build configurations
available to the users, and provide useful summaries and detailed usage notes.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Debugging Tools and Techniques

The exact method for debugging build failures depends on the nature of
the problem and on the system's area from which the bug originates.
Standard debugging practices such as comparison against the last known
working version with examination of the changes and the re-application
of steps to identify the one causing the problem are valid for the Yocto
Project just as they are for any other system. Even though it is
impossible to detail every possible potential failure, this section
provides some general tips to aid in debugging given a variety of
situations.

> **Note:**
> A useful feature for debugging is the error reporting tool.
> Configuring the Yocto Project to use this tool causes the
> OpenEmbedded build system to produce error reporting commands as part
> of the console output. You can enter the commands after the build
> completes to log error information into a common database, that can
> help you figure out what might be going wrong. For information on how
> to enable and use this feature, see the
> "dev-manual/error-reporting-tool:using the error reporting tool"
> section.
>

The following list shows the debugging topics in the remainder of this
section:

-  "dev-manual/debugging:viewing logs from failed tasks" describes
   how to find and view logs from tasks that failed during the build
   process.

-  "dev-manual/debugging:viewing variable values" describes how to
   use the BitBake `-e` option to examine variable values after a
   recipe has been parsed.

-  "dev-manual/debugging:viewing package information with `oe-pkgdata-util```"
   describes how to use the `oe-pkgdata-util` utility to query
   **PKGDATA_DIR** and
   display package-related information for built packages.

-  "dev-manual/debugging:viewing dependencies between recipes and tasks"
   describes how to use the BitBake `-g` option to display recipe
   dependency information used during the build.

-  "dev-manual/debugging:viewing task variable dependencies" describes
   how to use the `bitbake-dumpsig` command in conjunction with key
   subdirectories in the **Build Directory** to determine variable
   dependencies.

-  "dev-manual/debugging:running specific tasks" describes
   how to use several BitBake options (e.g. `-c`, `-C`, and `-f`)
   to run specific tasks in the build chain. It can be useful to run
   tasks "out-of-order" when trying isolate build issues.

-  "dev-manual/debugging:general BitBake problems" describes how
   to use BitBake's `-D` debug output option to reveal more about what
   BitBake is doing during the build.

-  "dev-manual/debugging:building with no dependencies"
   describes how to use the BitBake `-b` option to build a recipe
   while ignoring dependencies.

-  "dev-manual/debugging:recipe logging mechanisms"
   describes how to use the many recipe logging functions to produce
   debugging output and report errors and warnings.

-  "dev-manual/debugging:debugging parallel make races"
   describes how to debug situations where the build consists of several
   parts that are run simultaneously and when the output or result of
   one part is not ready for use with a different part of the build that
   depends on that output.

-  "dev-manual/debugging:debugging with the gnu project debugger (gdb) remotely"
   describes how to use GDB to allow you to examine running programs, which can
   help you fix problems.

-  "dev-manual/debugging:debugging with the gnu project debugger (gdb) on the target"
   describes how to use GDB directly on target hardware for debugging.

-  "dev-manual/debugging:other debugging tips" describes
   miscellaneous debugging tips that can be useful.

## Viewing Logs from Failed Tasks

You can find the log for a task in the file
`${`\ **WORKDIR**\ `}/temp/log.do_`\ `taskname`.
For example, the log for the
ref-tasks-compile task of the
QEMU minimal image for the x86 machine (`qemux86`) might be in
`tmp/work/qemux86-poky-linux/core-image-minimal/1.0-r0/temp/log.do_compile`.
To see the commands **BitBake** ran
to generate a log, look at the corresponding `run.do_`\ `taskname` file
in the same directory.

`log.do_`\ `taskname` and `run.do_`\ `taskname` are actually symbolic
links to `log.do_`\ `taskname`\ `.`\ `pid` and
`log.run_`\ `taskname`\ `.`\ `pid`, where `pid` is the PID the task had
when it ran. The symlinks always point to the files corresponding to the
most recent run.

## Viewing Variable Values

Sometimes you need to know the value of a variable as a result of
BitBake's parsing step. This could be because some unexpected behavior
occurred in your project. Perhaps an attempt to :ref:`modify a variable
<bitbake-user-manual/bitbake-user-manual-metadata:modifying existing
variables>` did not work out as expected.

BitBake's `bitbake-getvar` command is used to display variable values after
parsing. The following command displays the variable value for **OVERRIDES**
after the configuration files (i.e. `local.conf`, `bblayers.conf`,
`bitbake.conf` and so forth) have been parsed::

   $ bitbake-getvar OVERRIDES

The following command displays the value of **PV** after a specific recipe
has been parsed::

   $ bitbake-getvar -r recipename PV

> **Note:**
> Each recipe has its own private set of variables (datastore).
> Internally, after parsing the configuration, a copy of the resulting
> datastore is made prior to parsing each recipe. This copying implies
> that variables set in one recipe will not be visible to other
> recipes.
>
> Likewise, each task within a recipe gets a private datastore based on
> the recipe datastore, which means that variables set within one task
> will not be visible to other tasks.
>

In the output of `bitbake-getvar`, the line containing the value of the
variable is preceded by a description of how the variable got its value,
including temporary values that were later overridden. This description also
includes variable flags (varflags) set on the variable. The output can be very
helpful during debugging.

Variables that are exported to the environment are preceded by
`export` in the output of `bitbake-getvar`. See the following example::

   export CC="i586-poky-linux-gcc -m32 -march=i586 --sysroot=/home/ulf/poky/build/tmp/sysroots/qemux86"

Shell functions and tasks can also be inspected with the same mechanism::

   $ bitbake-getvar -r recipename do_install

For Python functions and tasks, `bitbake -e recipename` can be used instead.

Moreover, the output of the `bitbake -e` and `bitbake -e` recipe commands
includes the following information:

-  The output starts with a tree listing all configuration files and
   classes included globally, recursively listing the files they include
   or inherit in turn. Much of the behavior of the OpenEmbedded build
   system (including the behavior of the ref-manual/tasks:normal recipe build tasks) is
   implemented in the ref-classes-base class and the
   classes it inherits, rather than being built into BitBake itself.

-  After the variable values, all functions appear in the output. For
   shell functions, variables referenced within the function body are
   expanded. If a function has been modified using overrides or using
   override-style operators like `:append` and `:prepend`, then the
   final assembled function body appears in the output.

## Viewing Package Information with ``oe-pkgdata-util``

You can use the `oe-pkgdata-util` command-line utility to query
**PKGDATA_DIR** and display
various package-related information. When you use the utility, you must
use it to view information on packages that have already been built.

Here are a few of the available `oe-pkgdata-util` subcommands.

> **Note:**
> You can use the standard \* and ? globbing wildcards as part of
> package names and paths.
>

-  `oe-pkgdata-util list-pkgs [pattern]`: Lists all packages
   that have been built, optionally limiting the match to packages that
   match pattern.

-  `oe-pkgdata-util list-pkg-files package ...`: Lists the
   files and directories contained in the given packages.

   .. note::

      A different way to view the contents of a package is to look at
      the
      `${`\ **WORKDIR**\ `}/packages-split`
      directory of the recipe that generates the package. This directory
      is created by the
      ref-tasks-package task
      and has one subdirectory for each package the recipe generates,
      which contains the files stored in that package.

      If you want to inspect the `${WORKDIR}/packages-split`
      directory, make sure that ref-classes-rm-work is not
      enabled when you build the recipe.

-  `oe-pkgdata-util find-path path ...`: Lists the names of
   the packages that contain the given paths. For example, the following
   tells us that `/usr/share/man/man1/make.1` is contained in the
   `make-doc` package::

      $ oe-pkgdata-util find-path /usr/share/man/man1/make.1
      make-doc: /usr/share/man/man1/make.1

-  `oe-pkgdata-util lookup-recipe package ...`: Lists the name
   of the recipes that produce the given packages.

For more information on the `oe-pkgdata-util` command, use the help
facility::

   $ oe-pkgdata-util --help
   $ oe-pkgdata-util subcommand --help

## Viewing Dependencies Between Recipes and Tasks

Sometimes it can be hard to see why BitBake wants to build other recipes
before the one you have specified. Dependency information can help you
understand why a recipe is built.

To generate dependency information for a recipe, run the following
command::

   $ bitbake -g recipename

This command writes the following files in the current directory:

-  `pn-buildlist`: A list of recipes/targets involved in building
   `recipename`. "Involved" here means that at least one task from the
   recipe needs to run when building `recipename` from scratch. Targets
   that are in
   **ASSUME_PROVIDED**
   are not listed.

-  `task-depends.dot`: A graph showing dependencies between tasks.

The graphs are in DOT 
format and can be converted to images (e.g. using the `dot` tool from
`Graphviz <https://www.graphviz.org/>`__).

> **Note:**
> -  DOT files use a plain text format. The graphs generated using the
> `bitbake -g` command are often so large as to be difficult to
> read without special pruning (e.g. with BitBake's `-I` option)
> and processing. Despite the form and size of the graphs, the
> corresponding `.dot` files can still be possible to read and
> provide useful information.
>
> As an example, the `task-depends.dot` file contains lines such
> as the following::
>
> "libxslt.do_configure" -> "libxml2.do_populate_sysroot"
>
> The above example line reveals that the
> ref-tasks-configure
> task in `libxslt` depends on the
> ref-tasks-populate_sysroot
> task in `libxml2`, which is a normal
> **DEPENDS** dependency
> between the two recipes.
>
> -  For an example of how `.dot` files can be processed, see the
> `scripts/contrib/graph-tool` Python script, which finds and
> displays paths between graph nodes.
>

You can use a different method to view dependency information by using
either::

   $ bitbake -g -u taskexp recipename

or::

   $ bitbake -g -u taskexp_ncurses recipename

The `-u taskdep` option GUI window from which you can view build-time and
runtime dependencies for the recipes involved in building recipename. The
`-u taskexp_ncurses` option uses ncurses instead of GTK to render the UI.

## Viewing Task Variable Dependencies

As mentioned in the
"bitbake-user-manual/bitbake-user-manual-execution:checksums (signatures)"
section of the BitBake User Manual, BitBake tries to automatically determine
what variables a task depends on so that it can rerun the task if any values of
the variables change. This determination is usually reliable. However, if you
do things like construct variable names at runtime, then you might have to
manually declare dependencies on those variables using `vardeps` as described
in the "bitbake-user-manual/bitbake-user-manual-metadata:variable flags"
section of the BitBake User Manual.

If you are unsure whether a variable dependency is being picked up
automatically for a given task, you can list the variable dependencies
BitBake has determined by doing the following:

#. Build the recipe containing the task::

   $ bitbake recipename

#. Inside the **STAMPS_DIR**
   directory, find the signature data (`sigdata`) file that
   corresponds to the task. The `sigdata` files contain a pickled
   Python database of all the metadata that went into creating the input
   checksum for the task. As an example, for the
   ref-tasks-fetch task of the
   `db` recipe, the `sigdata` file might be found in the following
   location::

      ${BUILDDIR}/tmp/stamps/i586-poky-linux/db/6.0.30-r1.do_fetch.sigdata.7c048c18222b16ff0bcee2000ef648b1

   For tasks that are accelerated through the shared state
   (sstate ) cache, an
   additional `siginfo` file is written into
   **SSTATE_DIR** along with
   the cached task output. The `siginfo` files contain exactly the
   same information as `sigdata` files.

#. Run `bitbake-dumpsig` on the `sigdata` or `siginfo` file. Here
   is an example::

      $ bitbake-dumpsig ${BUILDDIR}/tmp/stamps/i586-poky-linux/db/6.0.30-r1.do_fetch.sigdata.7c048c18222b16ff0bcee2000ef648b1

   In the output of the above command, you will find a line like the
   following, which lists all the (inferred) variable dependencies for
   the task. This list also includes indirect dependencies from
   variables depending on other variables, recursively::

      Task dependencies: ['PV', 'SRCREV', 'SRC_URI', 'SRC_URI[sha256sum]', 'base_do_fetch']

   .. note::

      Functions (e.g. `base_do_fetch`) also count as variable dependencies.
      These functions in turn depend on the variables they reference.

   The output of `bitbake-dumpsig` also includes the value each
   variable had, a list of dependencies for each variable, and
   **BB_BASEHASH_IGNORE_VARS**
   information.

## Debugging signature construction and unexpected task executions

There is a `bitbake-diffsigs` command for comparing two
`siginfo` or `sigdata` files. This command can be helpful when
trying to figure out what changed between two versions of a task. If you
call `bitbake-diffsigs` with just one file, the command behaves like
`bitbake-dumpsig`.

You can also use BitBake to dump out the signature construction
information without executing tasks by using either of the following
BitBake command-line options::

   ‐‐dump-signatures=SIGNATURE_HANDLER
   -S SIGNATURE_HANDLER

> **Note:**
> Two common values for `SIGNATURE_HANDLER` are "none" and "printdiff", which
> dump only the signature or compare the dumped signature with the most recent one,
> respectively. "printdiff" will try to establish the most recent
> signature match (e.g. in the sstate cache) and then
> compare the matched signatures to determine the stamps and delta
> where these two stamp trees diverge. This can be used to determine why
> tasks need to be re-run in situations where that is not expected.
>

Using BitBake with either of these options causes BitBake to dump out
`sigdata` files in the `stamps` directory for every task it would
have executed instead of building the specified target package.

## Viewing Metadata Used to Create the Input Signature of a Shared State Task

Seeing what metadata went into creating the input signature of a shared
state (sstate) task can be a useful debugging aid. This information is
available in signature information (`siginfo`) files in
**SSTATE_DIR**. For
information on how to view and interpret information in `siginfo`
files, see the
"dev-manual/debugging:viewing task variable dependencies" section.

For conceptual information on shared state, see the
"overview-manual/concepts:shared state"
section in the Yocto Project Overview and Concepts Manual.

## Invalidating Shared State to Force a Task to Run

The OpenEmbedded build system uses
checksums  and
overview-manual/concepts:shared state cache to avoid unnecessarily
rebuilding tasks. Collectively, this scheme is known as "shared state
code".

As with all schemes, this one has some drawbacks. It is possible that
you could make implicit changes to your code that the checksum
calculations do not take into account. These implicit changes affect a
task's output but do not trigger the shared state code into rebuilding a
recipe. Consider an example during which a tool changes its output.
Assume that the output of `rpmdeps` changes. The result of the change
should be that all the `package` and `package_write_rpm` shared
state cache items become invalid. However, because the change to the
output is external to the code and therefore implicit, the associated
shared state cache items do not become invalidated. In this case, the
build process uses the cached items rather than running the task again.
Obviously, these types of implicit changes can cause problems.

To avoid these problems during the build, you need to understand the
effects of any changes you make. Realize that changes you make directly
to a function are automatically factored into the checksum calculation.
Thus, these explicit changes invalidate the associated area of shared
state cache. However, you need to be aware of any implicit changes that
are not obvious changes to the code and could affect the output of a
given task.

When you identify an implicit change, you can easily take steps to
invalidate the cache and force the tasks to run. The steps you can take
are as simple as changing a function's comments in the source code. For
example, to invalidate package shared state files, change the comment
statements of
ref-tasks-package or the
comments of one of the functions it calls. Even though the change is
purely cosmetic, it causes the checksum to be recalculated and forces
the build system to run the task again.

> **Note:**
> For an example of a commit that makes a cosmetic change to invalidate
> shared state, see this
> commit .
>

## Running Specific Tasks

Any given recipe consists of a set of tasks. The standard BitBake
behavior in most cases is: ref-tasks-fetch, ref-tasks-unpack, ref-tasks-patch,
ref-tasks-configure, ref-tasks-compile, ref-tasks-install, ref-tasks-package,
do_package_write_* , and ref-tasks-build. The default task is
ref-tasks-build and any tasks on which it depends build first. Some tasks,
such as ref-tasks-devshell, are not part of the default build chain. If you
wish to run a task that is not part of the default build chain, you can
use the `-c` option in BitBake. Here is an example::

   $ bitbake matchbox-desktop -c devshell

The `-c` option respects task dependencies, which means that all other
tasks (including tasks from other recipes) that the specified task
depends on will be run before the task. Even when you manually specify a
task to run with `-c`, BitBake will only run the task if it considers
it "out of date". See the
"overview-manual/concepts:stamp files and the rerunning of tasks"
section in the Yocto Project Overview and Concepts Manual for how
BitBake determines whether a task is "out of date".

If you want to force an up-to-date task to be rerun (e.g. because you
made manual modifications to the recipe's
**WORKDIR** that you want to try
out), then you can use the `-f` option.

> **Note:**
> The reason `-f` is never required when running the
> ref-tasks-devshell task is because the
> [\ nostamp \ ]
> variable flag is already set for the task.
>

The following example shows one way you can use the `-f` option::

   $ bitbake matchbox-desktop
             .
             .
   make some changes to the source code in the work directory
             .
             .
   $ bitbake matchbox-desktop -c compile -f
   $ bitbake matchbox-desktop

This sequence first builds and then recompiles `matchbox-desktop`. The
last command reruns all tasks (basically the packaging tasks) after the
compile. BitBake recognizes that the ref-tasks-compile task was rerun and
therefore understands that the other tasks also need to be run again.

Another, shorter way to rerun a task and all
ref-manual/tasks:normal recipe build tasks
that depend on it is to use the `-C` option.

> **Note:**
> This option is upper-cased and is separate from the `-c`
> option, which is lower-cased.
>

Using this option invalidates the given task and then runs the
ref-tasks-build task, which is
the default task if no task is given, and the tasks on which it depends.
You could replace the final two commands in the previous example with
the following single command::

   $ bitbake matchbox-desktop -C compile

Internally, the `-f` and `-C` options work by tainting (modifying)
the input checksum of the specified task. This tainting indirectly
causes the task and its dependent tasks to be rerun through the normal
task dependency mechanisms.

> **Note:**
> BitBake explicitly keeps track of which tasks have been tainted in
> this fashion, and will print warnings such as the following for
> builds involving such tasks:
>
> .. code-block:: none
>
> WARNING: /home/ulf/openembedded-core/meta/recipes-sato/matchbox-desktop/matchbox-desktop_2.1.bb.do_compile is tainted from a forced run
>
>
> The purpose of the warning is to let you know that the work directory
> and build output might not be in the clean state they would be in for
> a "normal" build, depending on what actions you took. To get rid of
> such warnings, you can remove the work directory and rebuild the
> recipe, as follows::
>
> $ bitbake matchbox-desktop -c clean
> $ bitbake matchbox-desktop
>
>

You can view a list of tasks in a given package by running the
ref-tasks-listtasks task as follows::

   $ bitbake matchbox-desktop -c listtasks

The results appear as output to the console and are also in
the file `${WORKDIR}/temp/log.do_listtasks`.

## General BitBake Problems

You can see debug output from BitBake by using the `-D` option. The
debug output gives more information about what BitBake is doing and the
reason behind it. Each `-D` option you use increases the logging
level. The most common usage is `-DDD`.

The output from `bitbake -DDD -v targetname` can reveal why BitBake
chose a certain version of a package or why BitBake picked a certain
provider. This command could also help you in a situation where you
think BitBake did something unexpected.

## Building with No Dependencies

To build a specific recipe (`.bb` file), you can use the following
command form::

   $ bitbake -b somepath/somerecipe.bb

This command form does
not check for dependencies. Consequently, you should use it only when
you know existing dependencies have been met.

> **Note:**
> You can also specify fragments of the filename. In this case, BitBake
> checks for a unique match.
>

## Recipe Logging Mechanisms

The Yocto Project provides several logging functions for producing
debugging output and reporting errors and warnings. For Python
functions, the following logging functions are available. All of these functions
log to `${T}/log.do_`\ `task`, and can also log to standard output
(stdout) with the right settings:

-  `bb.plain(msg)`: Writes msg as is to the log while also
   logging to stdout.

-  `bb.note(msg)`: Writes "NOTE: msg" to the log. Also logs to
   stdout if BitBake is called with "-v".

-  `bb.debug(level, msg)`: Writes "DEBUG: msg" to the log. Also logs to
   stdout if the log level is greater than or equal to level. See the
   "bitbake-user-manual/bitbake-user-manual-intro:usage and syntax"
   option in the BitBake User Manual for more information.

-  `bb.warn(msg)`: Writes "WARNING: msg" to the log while also
   logging to stdout.

-  `bb.error(msg)`: Writes "ERROR: msg" to the log while also
   logging to standard out (stdout).

   .. note::

      Calling this function does not cause the task to fail.

-  `bb.fatal(msg)`: This logging function is similar to
   `bb.error(msg)` but also causes the calling task to fail.

   .. note::

      `bb.fatal()` raises an exception, which means you do not need to put a
      "return" statement after the function.

The same logging functions are also available in shell functions, under
the names `bbplain`, `bbnote`, `bbdebug`, `bbwarn`, `bberror`,
and `bbfatal`. The ref-classes-logging class
implements these functions. See that class in the `meta/classes`
folder of **OpenEmbedded-Core (OE-Core)** for information.

### Logging With Python

When creating recipes using Python and inserting code that handles build
logs, keep in mind the goal is to have informative logs while keeping
the console as "silent" as possible. Also, if you want status messages
in the log, use the "debug" loglevel.

Here is an example written in Python. The code handles logging for
a function that determines the number of tasks needed to be run. See the
"ref-tasks-listtasks"
section for additional information::

   python do_listtasks() {
       bb.debug(2, "Starting to figure out the task list")
       if noteworthy_condition:
           bb.note("There are 47 tasks to run")
       bb.debug(2, "Got to point xyz")
       if warning_trigger:
           bb.warn("Detected warning_trigger, this might be a problem later.")
       if recoverable_error:
           bb.error("Hit recoverable_error, you really need to fix this!")
       if fatal_error:
           bb.fatal("fatal_error detected, unable to print the task list")
       bb.plain("The tasks present are abc")
       bb.debug(2, "Finished figuring out the tasklist")
   }

### Logging With Bash

When creating recipes using Bash and inserting code that handles build
logs, you have the same goals --- informative with minimal console output.
The syntax you use for recipes written in Bash is similar to that of
recipes written in Python described in the previous section.

Here is an example written in Bash. The code logs the progress of
the `do_my_function` function::

   do_my_function() {
       bbdebug 2 "Running do_my_function"
       if [ exceptional_condition ]; then
           bbnote "Hit exceptional_condition"
       fi
       bbdebug 2  "Got to point xyz"
       if [ warning_trigger ]; then
           bbwarn "Detected warning_trigger, this might cause a problem later."
       fi
       if [ recoverable_error ]; then
           bberror "Hit recoverable_error, correcting"
       fi
       if [ fatal_error ]; then
           bbfatal "fatal_error detected"
       fi
       bbdebug 2 "Completed do_my_function"
   }

## Debugging Parallel Make Races

A parallel `make` race occurs when the build consists of several parts
that are run simultaneously and a situation occurs when the output or
result of one part is not ready for use with a different part of the
build that depends on that output. Parallel make races are annoying and
can sometimes be difficult to reproduce and fix. However, there are some simple
tips and tricks that can help you debug and fix them. This section
presents a real-world example of an error encountered on the Yocto
Project autobuilder and the process used to fix it.

> **Note:**
> If you cannot properly fix a `make` race condition, you can work around it
> by clearing either the **PARALLEL_MAKE** or **PARALLEL_MAKEINST**
> variables.
>

### The Failure

For this example, assume that you are building an image that depends on
the "neard" package. And, during the build, BitBake runs into problems
and creates the following output.

> **Note:**
> This example log file has longer lines artificially broken to make
> the listing easier to read.
>

If you examine the output or the log file, you see the failure during
`make`:

```none
| DEBUG: SITE files ['endian-little', 'bit-32', 'ix86-common', 'common-linux', 'common-glibc', 'i586-linux', 'common']
| DEBUG: Executing shell function do_compile
| NOTE: make -j 16
| make --no-print-directory all-am
| /bin/mkdir -p include/near
| /bin/mkdir -p include/near
| /bin/mkdir -p include/near
| ln -s /home/pokybuild/yocto-autobuilder/nightly-x86/build/build/tmp/work/i586-poky-linux/neard/
  0.14-r0/neard-0.14/include/types.h include/near/types.h
| ln -s /home/pokybuild/yocto-autobuilder/nightly-x86/build/build/tmp/work/i586-poky-linux/neard/
  0.14-r0/neard-0.14/include/log.h include/near/log.h
| ln -s /home/pokybuild/yocto-autobuilder/nightly-x86/build/build/tmp/work/i586-poky-linux/neard/
  0.14-r0/neard-0.14/include/plugin.h include/near/plugin.h
| /bin/mkdir -p include/near
| /bin/mkdir -p include/near
| /bin/mkdir -p include/near
| ln -s /home/pokybuild/yocto-autobuilder/nightly-x86/build/build/tmp/work/i586-poky-linux/neard/
  0.14-r0/neard-0.14/include/tag.h include/near/tag.h
| /bin/mkdir -p include/near
| ln -s /home/pokybuild/yocto-autobuilder/nightly-x86/build/build/tmp/work/i586-poky-linux/neard/
  0.14-r0/neard-0.14/include/adapter.h include/near/adapter.h
| /bin/mkdir -p include/near
| ln -s /home/pokybuild/yocto-autobuilder/nightly-x86/build/build/tmp/work/i586-poky-linux/neard/
  0.14-r0/neard-0.14/include/ndef.h include/near/ndef.h
| ln -s /home/pokybuild/yocto-autobuilder/nightly-x86/build/build/tmp/work/i586-poky-linux/neard/
  0.14-r0/neard-0.14/include/tlv.h include/near/tlv.h
| /bin/mkdir -p include/near
| /bin/mkdir -p include/near
| ln -s /home/pokybuild/yocto-autobuilder/nightly-x86/build/build/tmp/work/i586-poky-linux/neard/
  0.14-r0/neard-0.14/include/setting.h include/near/setting.h
| /bin/mkdir -p include/near
| /bin/mkdir -p include/near
| /bin/mkdir -p include/near
| ln -s /home/pokybuild/yocto-autobuilder/nightly-x86/build/build/tmp/work/i586-poky-linux/neard/
  0.14-r0/neard-0.14/include/device.h include/near/device.h
| ln -s /home/pokybuild/yocto-autobuilder/nightly-x86/build/build/tmp/work/i586-poky-linux/neard/
  0.14-r0/neard-0.14/include/nfc_copy.h include/near/nfc_copy.h
| ln -s /home/pokybuild/yocto-autobuilder/nightly-x86/build/build/tmp/work/i586-poky-linux/neard/
  0.14-r0/neard-0.14/include/snep.h include/near/snep.h
| ln -s /home/pokybuild/yocto-autobuilder/nightly-x86/build/build/tmp/work/i586-poky-linux/neard/
  0.14-r0/neard-0.14/include/version.h include/near/version.h
| ln -s /home/pokybuild/yocto-autobuilder/nightly-x86/build/build/tmp/work/i586-poky-linux/neard/
  0.14-r0/neard-0.14/include/dbus.h include/near/dbus.h
| ./src/genbuiltin nfctype1 nfctype2 nfctype3 nfctype4 p2p > src/builtin.h
| i586-poky-linux-gcc  -m32 -march=i586 --sysroot=/home/pokybuild/yocto-autobuilder/nightly-x86/
  build/build/tmp/sysroots/qemux86 -DHAVE_CONFIG_H -I. -I./include -I./src -I./gdbus  -I/home/pokybuild/
  yocto-autobuilder/nightly-x86/build/build/tmp/sysroots/qemux86/usr/include/glib-2.0
  -I/home/pokybuild/yocto-autobuilder/nightly-x86/build/build/tmp/sysroots/qemux86/usr/
  lib/glib-2.0/include  -I/home/pokybuild/yocto-autobuilder/nightly-x86/build/build/
  tmp/sysroots/qemux86/usr/include/dbus-1.0 -I/home/pokybuild/yocto-autobuilder/
  nightly-x86/build/build/tmp/sysroots/qemux86/usr/lib/dbus-1.0/include  -I/home/pokybuild/yocto-autobuilder/
  nightly-x86/build/build/tmp/sysroots/qemux86/usr/include/libnl3
  -DNEAR_PLUGIN_BUILTIN -DPLUGINDIR=\""/usr/lib/near/plugins"\"
  -DCONFIGDIR=\""/etc/neard\"" -O2 -pipe -g -feliminate-unused-debug-types -c
  -o tools/snep-send.o tools/snep-send.c
| In file included from tools/snep-send.c:16:0:
| tools/../src/near.h:41:23: fatal error: near/dbus.h: No such file or directory
|  #include <near/dbus.h>
|                        ^
| compilation terminated.
| make[1]: *** [tools/snep-send.o] Error 1
| make[1]: *** Waiting for unfinished jobs....
| make: *** [all] Error 2
| ERROR: oe_runmake failed
```

### Reproducing the Error

Because race conditions are intermittent, they do not manifest
themselves every time you do the build. In fact, most times the build
will complete without problems even though the potential race condition
exists. Thus, once the error surfaces, you need a way to reproduce it.

In this example, compiling the "neard" package is causing the problem.
So the first thing to do is build "neard" locally. Before you start the
build, set the
**PARALLEL_MAKE** variable
in your `local.conf` file to a high number (e.g. "-j 20"). Using a
high value for **PARALLEL_MAKE** increases the chances of the race
condition showing up::

   $ bitbake neard

Once the local build for "neard" completes, start a `devshell` build::

   $ bitbake neard -c devshell

For information on how to use a `devshell`, see the
"dev-manual/development-shell:using a development shell" section.

In the `devshell`, do the following::

   $ make clean
   $ make tools/snep-send.o

The `devshell` commands cause the failure to clearly
be visible. In this case, there is a missing dependency for the `neard`
Makefile target. Here is some abbreviated, sample output with the
missing dependency clearly visible at the end::

   i586-poky-linux-gcc  -m32 -march=i586 --sysroot=/home/scott-lenovo/......
      .
      .
      .
   tools/snep-send.c
   In file included from tools/snep-send.c:16:0:
   tools/../src/near.h:41:23: fatal error: near/dbus.h: No such file or directory
    #include <near/dbus.h>
                     ^
   compilation terminated.
   make: *** [tools/snep-send.o] Error 1
   $

### Creating a Patch for the Fix

Because there is a missing dependency for the Makefile target, you need
to patch the `Makefile.am` file, which is generated from
`Makefile.in`. You can use Quilt to create the patch::

   $ quilt new parallelmake.patch
   Patch patches/parallelmake.patch is now on top
   $ quilt add Makefile.am
   File Makefile.am added to patch patches/parallelmake.patch

For more information on using Quilt, see the
"dev-manual/quilt:using quilt in your workflow" section.

At this point you need to make the edits to `Makefile.am` to add the
missing dependency. For our example, you have to add the following line
to the file::

   tools/snep-send.$(OBJEXT): include/near/dbus.h

Once you have edited the file, use the `refresh` command to create the
patch::

   $ quilt refresh
   Refreshed patch patches/parallelmake.patch

Once the patch file is created, you need to add it back to the originating
recipe folder. Here is an example assuming **OpenEmbedded-Core (OE-Core)**
is named `openembedded-core`::

   $ cp patches/parallelmake.patch openembedded-core/meta/recipes-connectivity/neard/neard

The final thing you need to do to implement the fix in the build is to
update the "neard" recipe (i.e. `neard-0.14.bb`) so that the
**SRC_URI** statement includes
the patch file. The recipe file is in the folder above the patch. Here
is what the edited **SRC_URI** statement would look like::

   SRC_URI = "${KERNELORG_MIRROR}/linux/network/nfc/${BPN}-${PV}.tar.xz \
              file://neard.in \
              file://neard.service.in \
              file://parallelmake.patch \
             "

With the patch complete and moved to the correct folder and the
**SRC_URI** statement updated, you can exit the `devshell`::

   $ exit

### Testing the Build

With everything in place, you can get back to trying the build again
locally::

   $ bitbake neard

This build should succeed.

Now you can open up a `devshell` again and repeat the clean and make
operations as follows::

   $ bitbake neard -c devshell
   $ make clean
   $ make tools/snep-send.o

The build should work without issue.

As with all solved problems, if they originated upstream, you need to
submit the fix for the recipe in OE-Core and upstream so that the
problem is taken care of at its source. See the
"/contributor-guide/submit-changes" section for more information.

## Debugging With the GNU Project Debugger (GDB) Remotely

GDB allows you to examine running programs, which in turn helps you to
understand and fix problems. It also allows you to perform post-mortem
style analysis of program crashes. GDB is available as a package within
the Yocto Project and is installed in SDK images by default. See the
"ref-manual/images:Images" chapter in the Yocto
Project Reference Manual for a description of these images. You can find
information on GDB at https://sourceware.org/gdb/.

> **Note:**
> For best results, install debug (`-dbg`) packages for the applications you
> are going to debug. Doing so makes extra debug symbols available that give
> you more meaningful output.
>

Sometimes, due to memory or disk space constraints, it is not possible
to use GDB directly on the remote target to debug applications. These
constraints arise because GDB needs to load the debugging information
and the binaries of the process being debugged. Additionally, GDB needs
to perform many computations to locate information such as function
names, variable names and values, stack traces and so forth --- even
before starting the debugging process. These extra computations place
more load on the target system and can alter the characteristics of the
program being debugged.

To help get past the previously mentioned constraints, there are two
methods you can use: running a debuginfod server and using gdbserver.

### Using the debuginfod server method

`debuginfod` from `elfutils` is a way to distribute `debuginfo` files.
Running a `debuginfod` server makes debug symbols readily available,
which means you don't need to download debugging information
and the binaries of the process being debugged. You can just fetch
debug symbols from the server.

To run a `debuginfod` server, you need to do the following:

-  Ensure that `debuginfod` is present in **DISTRO_FEATURES**
   (it already is in **OpenEmbedded-Core (OE-Core)** defaults and
   **Poky** reference distribution).

   If not, set in your distro config file or in `local.conf`::

      DISTRO_FEATURES:append = " debuginfod"

   This distro feature enables the server and client library in `elfutils`,
   and enables `debuginfod` support in clients (at the moment, `gdb` and `binutils`).

-  Run the following commands to launch the `debuginfod` server on the host::

      $ oe-debuginfod

-  To use `debuginfod` on the target, you need to know the ip:port where
   `debuginfod` is listening on the host (port defaults to 8002), and export
   that into the shell environment, for example in `qemu`::

      root@qemux86-64:~# export DEBUGINFOD_URLS="http://192.168.7.1:8002/"

-  Then debug info fetching should simply work when running the target `gdb`,
   `readelf` or `objdump`, for example::

      root@qemux86-64:~# gdb /bin/cat
      ...
      Reading symbols from /bin/cat...
      Downloading separate debug info for /bin/cat...
      Reading symbols from /home/root/.cache/debuginfod_client/923dc4780cfbc545850c616bffa884b6b5eaf322/debuginfo...

-  It's also possible to use `debuginfod-find` to just query the server::

      root@qemux86-64:~# debuginfod-find debuginfo /bin/ls
      /home/root/.cache/debuginfod_client/356edc585f7f82d46f94fcb87a86a3fe2d2e60bd/debuginfo

### Using the gdbserver method

gdbserver, which runs on the remote target and does not load any
debugging information from the debugged process. Instead, a GDB instance
processes the debugging information that is run on a remote computer -
the host GDB. The host GDB then sends control commands to gdbserver to
make it stop or start the debugged program, as well as read or write
memory regions of that debugged program. All the debugging information
loaded and processed as well as all the heavy debugging is done by the
host GDB. Offloading these processes gives the gdbserver running on the
target a chance to remain small and fast.

Because the host GDB is responsible for loading the debugging
information and for doing the necessary processing to make actual
debugging happen, you have to make sure the host can access the
unstripped binaries complete with their debugging information and also
be sure the target is compiled with no optimizations. The host GDB must
also have local access to all the libraries used by the debugged
program. Because gdbserver does not need any local debugging
information, the binaries on the remote target can remain stripped.
However, the binaries must also be compiled without optimization so they
match the host's binaries.

To remain consistent with GDB documentation and terminology, the binary
being debugged on the remote target machine is referred to as the
"inferior" binary. For documentation on GDB see the `GDB
site <https://sourceware.org/gdb/documentation/>`__.

The following steps show you how to debug using the GNU project
debugger.

#. *Configure your build system to construct the companion debug
   filesystem:*

   In your `local.conf` file, set the following::

      IMAGE_GEN_DEBUGFS = "1"
      IMAGE_FSTYPES_DEBUGFS = "tar.bz2"

   These options cause the
   OpenEmbedded build system to generate a special companion filesystem
   fragment, which contains the matching source and debug symbols to
   your deployable filesystem. The build system does this by looking at
   what is in the deployed filesystem, and pulling the corresponding
   `-dbg` packages.

   The companion debug filesystem is not a complete filesystem, but only
   contains the debug fragments. This filesystem must be combined with
   the full filesystem for debugging. Subsequent steps in this procedure
   show how to combine the partial filesystem with the full filesystem.

#. *Configure the system to include gdbserver in the target filesystem:*

   Make the following addition in your `local.conf` file::

      EXTRA_IMAGE_FEATURES:append = " tools-debug"

   The change makes
   sure the `gdbserver` package is included.

#. *Build the environment:*

   Use the following command to construct the image and the companion
   Debug Filesystem::

      $ bitbake image

   Build the cross GDB component and
   make it available for debugging. Build the SDK that matches the
   image. Building the SDK is best for a production build that can be
   used later for debugging, especially during long term maintenance::

      $ bitbake -c populate_sdk image

   Alternatively, you can build the minimal toolchain components that
   match the target. Doing so creates a smaller than typical SDK and
   only contains a minimal set of components with which to build simple
   test applications, as well as run the debugger::

      $ bitbake meta-toolchain

   A final method is to build Gdb itself within the build system::

      $ bitbake gdb-cross-<architecture>

   Doing so produces a temporary copy of
   `cross-gdb` you can use for debugging during development. While
   this is the quickest approach, the two previous methods in this step
   are better when considering long-term maintenance strategies.

   .. note::

      If you run `bitbake gdb-cross`, the OpenEmbedded build system suggests
      the actual image (e.g. `gdb-cross-i586`). The suggestion is usually the
      actual name you want to use.

#. *Set up the* `debugfs`\ *:*

   Run the following commands to set up the `debugfs`::

      $ mkdir debugfs
      $ cd debugfs
      $ tar xvfj build-dir/tmp/deploy/images/machine/image.rootfs.tar.bz2
      $ tar xvfj build-dir/tmp/deploy/images/machine/image-dbg.rootfs.tar.bz2

#. *Set up GDB:*

   Install the SDK (if you built one) and then source the correct
   environment file. Sourcing the environment file puts the SDK in your
   `PATH` environment variable and sets `$GDB` to the SDK's debugger.

   If you are using the build system, Gdb is located in
   `build-dir`\ `/tmp/sysroots/`\ `host`\ `/usr/bin/`\ `architecture`\ `/`\ `architecture`\ `-gdb`

#. *Boot the target:*

   For information on how to run QEMU, see the `QEMU
   Documentation <https://wiki.qemu.org/Documentation/GettingStartedDevelopers>`__.

   .. note::

      Be sure to verify that your host can access the target via TCP.

#. *Debug a program:*

   Debugging a program involves running gdbserver on the target and then
   running Gdb on the host. The example in this step debugs `gzip`:

   .. code-block:: shell

      root@qemux86:~# gdbserver localhost:1234 /bin/gzip —help

   For
   additional gdbserver options, see the `GDB Server
   Documentation <https://www.gnu.org/software/gdb/documentation/>`__.

   After running gdbserver on the target, you need to run Gdb on the
   host and configure it and connect to the target. Use these commands::

      $ cd directory-holding-the-debugfs-directory
      $ arch-gdb
      (gdb) set sysroot debugfs
      (gdb) set substitute-path /usr/src/debug debugfs/usr/src/debug
      (gdb) target remote IP-of-target:1234

   At this
   point, everything should automatically load (i.e. matching binaries,
   symbols and headers).

   .. note::

      The Gdb `set` commands in the previous example can be placed into the
      users `~/.gdbinit` file. Upon starting, Gdb automatically runs whatever
      commands are in that file.

#. *Deploying without a full image rebuild:*

   In many cases, during development you want a quick method to deploy a
   new binary to the target and debug it, without waiting for a full
   image build.

   One approach to solving this situation is to just build the component
   you want to debug. Once you have built the component, copy the
   executable directly to both the target and the host `debugfs`.

   If the binary is processed through the debug splitting in
   OpenEmbedded, you should also copy the debug items (i.e. `.debug`
   contents and corresponding `/usr/src/debug` files) from the work
   directory. Here is an example::

      $ bitbake bash
      $ bitbake -c devshell bash
      $ cd ..
      $ scp packages-split/bash/bin/bash target:/bin/bash
      $ cp -a packages-split/bash-dbg/\* path/debugfs

## Debugging with the GNU Project Debugger (GDB) on the Target

The previous section addressed using GDB remotely for debugging
purposes, which is the most usual case due to the inherent hardware
limitations on many embedded devices. However, debugging in the target
hardware itself is also possible with more powerful devices. This
section describes what you need to do in order to support using GDB to
debug on the target hardware.

To support this kind of debugging, you need do the following:

-  Ensure that GDB is on the target. You can do this by making
   the following addition to your `local.conf` file::

      EXTRA_IMAGE_FEATURES:append = " tools-debug"

-  Ensure that debug symbols are present. You can do so by adding the
   corresponding `-dbg` package to **IMAGE_INSTALL**::

      IMAGE_INSTALL:append = " packagename-dbg"

   Alternatively, you can add the following to `local.conf` to include
   all the debug symbols::

      EXTRA_IMAGE_FEATURES:append = " dbg-pkgs"

> **Note:**
> To improve the debug information accuracy, you can reduce the level
> of optimization used by the compiler. For example, when adding the
> following line to your `local.conf` file, you will reduce optimization
> from **FULL_OPTIMIZATION** of "-O2" to **DEBUG_OPTIMIZATION**
> of "-O -fno-omit-frame-pointer"::
>
> DEBUG_BUILD = "1"
>
> Consider that this will reduce the application's performance and is
> recommended only for debugging purposes.
>

## Enabling Minidebuginfo

Enabling the **DISTRO_FEATURES** minidebuginfo adds a compressed ELF section `.gnu_debugdata`
to all binary files, containing only function names, and thus increasing the size of the
binaries only by 5 to 10%. For comparison, full debug symbols can be 10 times as big as
a stripped binary, and it is thus not always possible to deploy full debug symbols.
Minidebuginfo data allows, on the one side, to retrieve a call-stack using
GDB (command backtrace) without deploying full debug symbols to the target. It also
allows to retrieve a symbolicated call-stack when using `systemd-coredump` to manage
coredumps (commands `coredumpctl list` and `coredumpctl info`).

This feature was created by Fedora, see https://fedoraproject.org/wiki/Features/MiniDebugInfo for
more details.

## Other Debugging Tips

Here are some other tips that you might find useful:

-  When adding new packages, it is worth watching for undesirable items
   making their way into compiler command lines. For example, you do not
   want references to local system files like `/usr/lib/` or
   `/usr/include/`.

-  If you want to remove the `psplash` boot splashscreen, add
   `psplash=false` to the kernel command line. Doing so prevents
   `psplash` from loading and thus allows you to see the console. It
   is also possible to switch out of the splashscreen by switching the
   virtual console (e.g. Fn+Left or Fn+Right on a Zaurus).

-  Removing **TMPDIR** (usually `tmp/`, within the
   **Build Directory**) can often fix temporary build issues. Removing
   **TMPDIR** is usually a relatively cheap operation, because task output
   will be cached in **SSTATE_DIR** (usually `sstate-cache/`, which is
   also in the **Build Directory**).

   .. note::

      Removing **TMPDIR** might be a workaround rather than a fix.
      Consequently, trying to determine the underlying cause of an issue before
      removing the directory is a good idea.

-  Understanding how a feature is used in practice within existing
   recipes can be very helpful. It is recommended that you configure
   some method that allows you to quickly search through files.

   Using GNU Grep, you can use the following shell function to
   recursively search through common recipe-related files, skipping
   binary files, `.git` directories, and the **Build Directory**
   (assuming its name starts with "build")::

      g() {
          grep -Ir \
               --exclude-dir=.git \
               --exclude-dir='build*' \
               --include='*.bb*' \
               --include='*.inc*' \
               --include='*.conf*' \
               --include='*.py*' \
               "$@"
      }

   Here are some usage examples::

      $ g FOO # Search recursively for "FOO"
      $ g -i foo # Search recursively for "foo", ignoring case
      $ g -w FOO # Search recursively for "FOO" as a word, ignoring e.g. "FOOBAR"

   If figuring
   out how some feature works requires a lot of searching, it might
   indicate that the documentation should be extended or improved. In
   such cases, consider filing a documentation bug using the Yocto
   Project implementation of
   `Bugzilla <>`. For information on
   how to submit a bug against the Yocto Project, see the Yocto Project
   Bugzilla wiki page 
   and the "/contributor-guide/report-defect" section.

   .. note::

      The manuals might not be the right place to document variables
      that are purely internal and have a limited scope (e.g. internal
      variables used to implement a single `.bbclass` file).

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Using the Error Reporting Tool

The error reporting tool allows you to submit errors encountered during
builds to a central database. Outside of the build environment, you can
use a web interface to browse errors, view statistics, and query for
errors. The tool works using a client-server system where the client
portion is integrated in **OpenEmbedded-Core (OE-Core)**.
The server receives the information collected and saves it in a
database.

There is a live instance of the error reporting server at
https://errors.yoctoproject.org.
When you want to get help with build failures, you can submit all of the
information on the failure easily and then point to the URL in your bug
report or send an email to the mailing list.

> **Note:**
> If you send error reports to this server, the reports become publicly
> visible.
>

## Enabling and Using the Tool

By default, the error reporting tool is disabled. You can enable it by
inheriting the ref-classes-report-error class by adding the
following statement to the end of your `local.conf` file in your
**Build Directory**::

   INHERIT += "report-error"

By default, the error reporting feature stores information in
`${`\ **LOG_DIR**\ `}/error-report`.
However, you can specify a directory to use by adding the following to
your `local.conf` file::

   ERR_REPORT_DIR = "path"

Enabling error
reporting causes the build process to collect the errors and store them
in a file as previously described. When the build system encounters an
error, it includes a command as part of the console output. You can run
the command to send the error file to the server. For example, the
following command sends the errors to an upstream server::

   $ send-error-report /home/brandusa/project/poky/build/tmp/log/error-report/error_report_201403141617.txt

In the previous example, the errors are sent to a public database
available at https://errors.yoctoproject.org, which is used by the
entire community. If you specify a particular server, you can send the
errors to a different database. Use the following command for more
information on available options::

   $ send-error-report --help

When sending the error file, you are prompted to review the data being
sent as well as to provide a name and optional email address. Once you
satisfy these prompts, the command returns a link from the server that
corresponds to your entry in the database. For example, here is a
typical link: https://errors.yoctoproject.org/Errors/Details/9522/

Following the link takes you to a web interface where you can browse,
query the errors, and view statistics.

## Disabling the Tool

To disable the error reporting feature, simply remove or comment out the
following statement from the end of your `local.conf` file in your
**Build Directory**::

   INHERIT += "report-error"

## Setting Up Your Own Error Reporting Server

If you want to set up your own error reporting server, you can obtain
the code from the Git repository at `/error-report-web/`.
Instructions on how to set it up are in the README document.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Working With Licenses

As mentioned in the "overview-manual/development-environment:licensing"
section in the Yocto Project Overview and Concepts Manual, open source
projects are open to the public and they consequently have different
licensing structures in place. This section describes the mechanism by
which the **OpenEmbedded Build System**
tracks changes to
licensing text and covers how to maintain open source license compliance
during your project's lifecycle. The section also describes how to
enable commercially licensed recipes, which by default are disabled.

## Tracking License Changes

The license of an upstream project might change in the future. In order
to prevent these changes going unnoticed, the
**LIC_FILES_CHKSUM**
variable tracks changes to the license text. The checksums are validated
at the end of the configure step, and if the checksums do not match, the
build will fail.

### Specifying the ``LIC_FILES_CHKSUM`` Variable

The **LIC_FILES_CHKSUM** variable contains checksums of the license text
in the source code for the recipe. Here is an example of how to
specify **LIC_FILES_CHKSUM**::

   LIC_FILES_CHKSUM = "file://COPYING;md5=xxxx \
                       file://licfile1.txt;beginline=5;endline=29;md5=yyyy \
                       file://licfile2.txt;endline=50;md5=zzzz \
                       ..."

> **Note:**
> -  When using "beginline" and "endline", realize that line numbering
> begins with one and not zero. Also, the included lines are
> inclusive (i.e. lines five through and including 29 in the
> previous example for `licfile1.txt`).
>
> -  When a license check fails, the selected license text is included
> as part of the QA message. Using this output, you can determine
> the exact start and finish for the needed license text.
>

The build system uses the **S**
variable as the default directory when searching files listed in
**LIC_FILES_CHKSUM**. The previous example employs the default
directory.

Consider this next example::

   LIC_FILES_CHKSUM = "file://src/ls.c;beginline=5;endline=16;\
                                       md5=bb14ed3c4cda583abc85401304b5cd4e"
   LIC_FILES_CHKSUM = "file://${UNPACKDIR}/license.html;md5=5c94767cedb5d6987c902ac850ded2c6"

The first line locates a file in `${S}/src/ls.c` and isolates lines
five through 16 as license text. The second line refers to a file in
**UNPACKDIR**.

Note that **LIC_FILES_CHKSUM** variable is mandatory for all recipes,
unless the **LICENSE** variable is set to "CLOSED".

### Explanation of Syntax

As mentioned in the previous section, the **LIC_FILES_CHKSUM** variable
lists all the important files that contain the license text for the
source code. It is possible to specify a checksum for an entire file, or
a specific section of a file (specified by beginning and ending line
numbers with the "beginline" and "endline" parameters, respectively).
The latter is useful for source files with a license notice header,
README documents, and so forth. If you do not use the "beginline"
parameter, then it is assumed that the text begins on the first line of
the file. Similarly, if you do not use the "endline" parameter, it is
assumed that the license text ends with the last line of the file.

The "md5" parameter stores the md5 checksum of the license text. If the
license text changes in any way as compared to this parameter then a
mismatch occurs. This mismatch triggers a build failure and notifies the
developer. Notification allows the developer to review and address the
license text changes. Also note that if a mismatch occurs during the
build, the correct md5 checksum is placed in the build log and can be
easily copied to the recipe.

There is no limit to how many files you can specify using the
**LIC_FILES_CHKSUM** variable. Generally, however, every project
requires a few specifications for license tracking. Many projects have a
"COPYING" file that stores the license information for all the source
code files. This practice allows you to just track the "COPYING" file as
long as it is kept up to date.

> **Note:**
> -  If you specify an empty or invalid "md5" parameter,
> **BitBake** returns an md5
> mis-match error and displays the correct "md5" parameter value
> during the build. The correct parameter is also captured in the
> build log.
>
> -  If the whole file contains only license text, you do not need to
> use the "beginline" and "endline" parameters.
>

## Enabling Commercially Licensed Recipes

By default, the OpenEmbedded build system disables components that have
commercial or other special licensing requirements. Such requirements
are defined on a recipe-by-recipe basis through the
**LICENSE_FLAGS** variable
definition in the affected recipe. For instance, the
`meta/recipes-multimedia/gstreamer/gstreamer1.0-plugins-ugly` recipe of
**OpenEmbedded-Core (OE-Core)** contains the following statement::

   LICENSE_FLAGS = "commercial"

Here is a
slightly more complicated example that contains both an explicit recipe
name and version (after variable expansion)::

   LICENSE_FLAGS = "license_${PN}_${PV}"

It is possible to give more details about a specific license
using flags on the **LICENSE_FLAGS_DETAILS** variable::

   LICENSE_FLAGS_DETAILS[my-eula-license] = "For further details, see https://example.com/eula."

If set, this will be displayed to the user if the license hasn't been accepted.

In order for a component restricted by a
**LICENSE_FLAGS** definition to be enabled and included in an image, it
needs to have a matching entry in the global
**LICENSE_FLAGS_ACCEPTED**
variable, which is a variable typically defined in your `local.conf`
file. For example, to enable the
`meta/recipes-multimedia/gstreamer/gstreamer1.0-plugins-ugly` package of
**OpenEmbedded-Core (OE-Core)**, you
could add either the string "commercial_gst-plugins-ugly" or the more
general string "commercial" to **LICENSE_FLAGS_ACCEPTED**. See the
"dev-manual/licenses:license flag matching" section for a full
explanation of how **LICENSE_FLAGS** matching works. Here is the
example::

   LICENSE_FLAGS_ACCEPTED = "commercial_gst-plugins-ugly"

Likewise, to additionally enable the package built from the recipe
containing `LICENSE_FLAGS = "license_${PN}_${PV}"`, and assuming that
the actual recipe name was `emgd_1.10.bb`, the following string would
enable that package as well as the original `gst-plugins-ugly`
package::

   LICENSE_FLAGS_ACCEPTED = "commercial_gst-plugins-ugly license_emgd_1.10"

As a convenience, you do not need to specify the
complete license string for every package. You can use
an abbreviated form, which consists of just the first portion or
portions of the license string before the initial underscore character
or characters. A partial string will match any license that contains the
given string as the first portion of its license. For example, the
following value will also match both of the packages
previously mentioned as well as any other packages that have licenses
starting with "commercial" or "license"::

   LICENSE_FLAGS_ACCEPTED = "commercial license"

### License Flag Matching

License flag matching allows you to control what recipes the
OpenEmbedded build system includes in the build. Fundamentally, the
build system attempts to match **LICENSE_FLAGS** strings found in
recipes against strings found in **LICENSE_FLAGS_ACCEPTED**.
A match causes the build system to include a recipe in the
build, while failure to find a match causes the build system to exclude
a recipe.

In general, license flag matching is simple. However, understanding some
concepts will help you correctly and effectively use matching.

Before a flag defined by a particular recipe is tested against the
entries of **LICENSE_FLAGS_ACCEPTED**, the expanded
string `_${PN}` is appended to the flag. This expansion makes each
**LICENSE_FLAGS** value recipe-specific. After expansion, the
string is then matched against the entries. Thus, specifying
`LICENSE_FLAGS = "commercial"` in recipe "foo", for example, results
in the string `"commercial_foo"`. And, to create a match, that string
must appear among the entries of **LICENSE_FLAGS_ACCEPTED**.

Judicious use of the **LICENSE_FLAGS** strings and the contents of the
**LICENSE_FLAGS_ACCEPTED** variable allows you a lot of flexibility for
including or excluding recipes based on licensing. For example, you can
broaden the matching capabilities by using license flags string subsets
in **LICENSE_FLAGS_ACCEPTED**.

> **Note:**
> When using a string subset, be sure to use the part of the expanded
> string that precedes the appended underscore character (e.g.
> `usethispart_1.3`, `usethispart_1.4`, and so forth).
>

For example, simply specifying the string "commercial" in the
**LICENSE_FLAGS_ACCEPTED** variable matches any expanded
**LICENSE_FLAGS** definition that starts with the string
"commercial" such as "commercial_foo" and "commercial_bar", which
are the strings the build system automatically generates for
hypothetical recipes named "foo" and "bar" assuming those recipes simply
specify the following::

   LICENSE_FLAGS = "commercial"

Thus, you can choose to exhaustively enumerate each license flag in the
list and allow only specific recipes into the image, or you can use a
string subset that causes a broader range of matches to allow a range of
recipes into the image.

This scheme works even if the **LICENSE_FLAGS** string already has
`_${PN}` appended. For example, the build system turns the license
flag "commercial_1.2_foo" into "commercial_1.2_foo_foo" and would match
both the general "commercial" and the specific "commercial_1.2_foo"
strings found in the **LICENSE_FLAGS_ACCEPTED** variable, as expected.

Here are some other scenarios:

-  You can specify a versioned string in the recipe such as
   "commercial_foo_1.2" in a "foo" recipe. The build system expands this
   string to "commercial_foo_1.2_foo". Combine this license flag with a
   **LICENSE_FLAGS_ACCEPTED** variable that has the string
   "commercial" and you match the flag along with any other flag that
   starts with the string "commercial".

-  Under the same circumstances, you can add "commercial_foo" in the
   **LICENSE_FLAGS_ACCEPTED** variable and the build system not only
   matches "commercial_foo_1.2" but also matches any license flag with
   the string "commercial_foo", regardless of the version.

-  You can be very specific and use both the package and version parts
   in the **LICENSE_FLAGS_ACCEPTED** list (e.g.
   "commercial_foo_1.2") to specifically match a versioned recipe.

### Other Variables Related to Commercial Licenses

There are other helpful variables related to commercial license handling,
defined in the
`meta/conf/distro/include/default-distrovars.inc` file in **OpenEmbedded-Core (OE-Core)**::

   COMMERCIAL_AUDIO_PLUGINS ?= ""
   COMMERCIAL_VIDEO_PLUGINS ?= ""

If you want to enable these components, you can do so by making sure you have
statements similar to the following in your `local.conf` configuration file::

   COMMERCIAL_AUDIO_PLUGINS = "gst-plugins-ugly-mad \
       gst-plugins-ugly-mpegaudioparse"
   COMMERCIAL_VIDEO_PLUGINS = "gst-plugins-ugly-mpeg2dec \
       gst-plugins-ugly-mpegstream gst-plugins-bad-mpegvideoparse"
   LICENSE_FLAGS_ACCEPTED = "commercial_gst-plugins-ugly commercial_gst-plugins-bad commercial_qmmp"

Of course, you could also create a matching list for those components using the
more general "commercial" string in the **LICENSE_FLAGS_ACCEPTED** variable,
but that would also enable all the other packages with **LICENSE_FLAGS**
containing "commercial", which you may or may not want::

   LICENSE_FLAGS_ACCEPTED = "commercial"

Specifying audio and video plugins as part of the
**COMMERCIAL_AUDIO_PLUGINS** and **COMMERCIAL_VIDEO_PLUGINS** statements
(along with **LICENSE_FLAGS_ACCEPTED**) includes the plugins or
components into built images, thus adding support for media formats or
components.

> **Note:**
> GStreamer "ugly" and "bad" plugins are actually available through
> open source licenses. However, the "ugly" ones can be subject to software
> patents in some countries, making it necessary to pay licensing fees
> to distribute them. The "bad" ones are just deemed unreliable by the
> GStreamer community and should therefore be used with care.
>

## Maintaining Open Source License Compliance During Your Product's Lifecycle

One of the concerns for a development organization using open source
software is how to maintain compliance with various open source
licensing during the lifecycle of the product. While this section does
not provide legal advice or comprehensively cover all scenarios, it does
present methods that you can use to assist you in meeting the compliance
requirements during a software release.

With hundreds of different open source licenses that the Yocto Project
tracks, it is difficult to know the requirements of each and every
license. However, the requirements of the major FLOSS licenses can begin
to be covered by assuming that there are three main areas of concern:

-  Source code must be provided.

-  License text for the software must be provided.

-  Compilation scripts and modifications to the source code must be
   provided.

There are other requirements beyond the scope of these three and the
methods described in this section (e.g. the mechanism through which
source code is distributed).

As different organizations have different ways of releasing software,
there can be multiple ways of meeting license obligations. At
least, we describe here two methods for achieving compliance:

-  The first method is to use OpenEmbedded's ability to provide
   the source code, provide a list of licenses, as well as
   compilation scripts and source code modifications.

   The remainder of this section describes supported methods to meet
   the previously mentioned three requirements.

-  The second method is to generate a *Software Bill of Materials*
   (**SBoM**), as described in the "/dev-manual/sbom" section.
   Not only do you generate **SPDX** output which can be used meet
   license compliance requirements (except for sharing the build system
   and layers sources for the time being), but this output also includes
   component version and patch information which can be used
   for vulnerability assessment.

Whatever method you choose, prior to releasing images, sources,
and the build system, you should audit all artifacts to ensure
completeness.

> **Note:**
> The Yocto Project generates a license manifest during image creation
> that is located in
> `${DEPLOY_DIR}/licenses/${SSTATE_PKGARCH}/<image-name>-<machine>.rootfs-<datestamp>/`
> to assist with any audits.
>

### Providing the Source Code

Compliance activities should begin before you generate the final image.
The first thing you should look at is the requirement that tops the list
for most compliance groups --- providing the source. The Yocto Project has
a few ways of meeting this requirement.

One of the easiest ways to meet this requirement is to provide the
entire **DL_DIR** used by the
build. This method, however, has a few issues. The most obvious is the
size of the directory since it includes all sources used in the build
and not just the source used in the released image. It will include
toolchain source, and other artifacts, which you would not generally
release. However, the more serious issue for most companies is
accidental release of proprietary software. The Yocto Project provides
an ref-classes-archiver class to help avoid some of these concerns.

Before you employ **DL_DIR** or the ref-classes-archiver class, you
need to decide how you choose to provide source. The source
ref-classes-archiver class can generate tarballs and SRPMs and can
create them with various levels of compliance in mind.

One way of doing this (but certainly not the only way) is to release
just the source as a tarball. You can do this by adding the following to
the `local.conf` file found in the **Build Directory**::

   INHERIT += "archiver"
   ARCHIVER_MODE[src] = "original"

During the creation of your
image, the source from all recipes that deploy packages to the image is
placed within subdirectories of `DEPLOY_DIR/sources` based on the
**LICENSE** for each recipe.
Releasing the entire directory enables you to comply with requirements
concerning providing the unmodified source. It is important to note that
the size of the directory can get large.

A way to help mitigate the size issue is to only release tarballs for
licenses that require the release of source. Let us assume you are only
concerned with GPL code as identified by running the following script:

```shell
# Script to archive a subset of packages matching specific license(s)
# Source and license files are copied into sub folders of package folder
# Must be run from build folder
#!/bin/bash
src_release_dir="source-release"
mkdir -p $src_release_dir
for a in tmp/deploy/sources/*; do
   for d in $a/*; do
      # Get package name from path
      p=`basename $d`
      p=${p%-*}
      p=${p%-*}
      # Only archive GPL packages (update *GPL* regex for your license check)
      numfiles=`ls tmp/deploy/licenses/$p/*GPL* 2> /dev/null | wc -l`
      if [ $numfiles -ge 1 ]; then
         echo Archiving $p
         mkdir -p $src_release_dir/$p/source
         cp $d/* $src_release_dir/$p/source 2> /dev/null
         mkdir -p $src_release_dir/$p/license
         cp tmp/deploy/licenses/$p/* $src_release_dir/$p/license 2> /dev/null
      fi
   done
done
```

At this point, you
could create a tarball from the `gpl_source_release` directory and
provide that to the end user. This method would be a step toward
achieving compliance with section 3a of GPLv2 and with section 6 of
GPLv3.

### Providing License Text

One requirement that is often overlooked is inclusion of license text.
This requirement also needs to be dealt with prior to generating the
final image. Some licenses require the license text to accompany the
binary. You can achieve this by adding the following to your
`local.conf` file::

   COPY_LIC_MANIFEST = "1"
   COPY_LIC_DIRS = "1"
   LICENSE_CREATE_PACKAGE = "1"

Adding these statements to the
configuration file ensures that the licenses collected during package
generation are included on your image.

> **Note:**
> Setting all three variables to "1" results in the image having two
> copies of the same license file. One copy resides in
> `/usr/share/common-licenses` and the other resides in
> `/usr/share/license`.
>
> The reason for this behavior is because
> **COPY_LIC_DIRS** and
> **COPY_LIC_MANIFEST**
> add a copy of the license when the image is built but do not offer a
> path for adding licenses for newly installed packages to an image.
> **LICENSE_CREATE_PACKAGE**
> adds a separate package and an upgrade path for adding licenses to an
> image.
>

As the source ref-classes-archiver class has already archived the
original unmodified source that contains the license files, you would have
already met the requirements for inclusion of the license information
with source as defined by the GPL and other open source licenses.

### Providing Compilation Scripts and Source Code Modifications

At this point, we have addressed all we need prior to generating the
image. The next two requirements are addressed during the final
packaging of the release.

By releasing the version of the OpenEmbedded build system and the layers
used during the build, you will be providing both compilation scripts
and the source code modifications in one step.

If the deployment team has a overview-manual/concepts:bsp layer
and a distro layer, and those
layers are used to patch, compile, package, or modify (in any way)
any open source software included in your released images, you might be
required to release those layers under section 3 of GPLv2 or section 1
of GPLv3. One way of doing that is with a clean checkout of the version
of the Yocto Project and layers used during your build. Archiving a Git
repository can be done with the `git-archive <git-archive(1)>` command.

One thing a development organization might want to consider for end-user
convenience is to modify
`meta-poky/conf/templates/default/bblayers.conf.sample` to ensure that when
the end user utilizes the released build system to build an image, the
development organization's layers are included in the `bblayers.conf` file
automatically::

   # POKY_BBLAYERS_CONF_VERSION is increased each time build/conf/bblayers.conf
   # changes incompatibly
   POKY_BBLAYERS_CONF_VERSION = "2"

   BBPATH = "${TOPDIR}"
   BBFILES ?= ""

   BBLAYERS ?= " \
     ##OEROOT##/meta \
     ##OEROOT##/../meta-yocto/meta-poky \
     ##OEROOT##/../meta-yocto/meta-yocto-bsp \
     ##OEROOT##/../meta-mylayer \
     "

Creating and
providing an archive of the **Metadata**
layers (recipes, configuration files, and so forth) enables you to meet
your requirements to include the scripts to control compilation as well
as any modifications to the original source.

### Compliance Limitations with Executables Built from Static Libraries

When package A is added to an image via the **RDEPENDS** or **RRECOMMENDS**
mechanisms as well as explicitly included in the image recipe with
**IMAGE_INSTALL**, and depends on a static linked library recipe B
(`DEPENDS += "B"`), package B will neither appear in the generated license
manifest nor in the generated source tarballs.  This occurs as the
ref-classes-license and ref-classes-archiver classes assume that
only packages included via **RDEPENDS** or **RRECOMMENDS**
end up in the image.

As a result, potential obligations regarding license compliance for package B
may not be met.

The Yocto Project doesn't enable static libraries by default, in part because
of this issue. Before a solution to this limitation is found, you need to
keep in mind that if your root filesystem is built from static libraries,
you will need to manually ensure that your deliveries are compliant
with the licenses of these libraries.

## Copying Non Standard Licenses

Some packages, such as the linux-firmware package, have many licenses
that are not in any way common. You can avoid adding a lot of these
types of common license files, which are only applicable to a specific
package, by using the
**NO_GENERIC_LICENSE**
variable. Using this variable also avoids QA errors when you use a
non-common, non-CLOSED license in a recipe.

Here is an example that uses the `LICENSE.Abilis.txt` file as
the license from the fetched source::

   NO_GENERIC_LICENSE[Firmware-Abilis] = "LICENSE.Abilis.txt"
