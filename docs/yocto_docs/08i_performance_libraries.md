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
