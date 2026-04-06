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
>
