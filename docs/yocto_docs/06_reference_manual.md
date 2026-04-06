# Yocto Project Reference Manual

# System Requirements

Welcome to the Yocto Project Reference Manual. This manual provides
reference information for the current release of the Yocto Project, and
is most effectively used after you have an understanding of the basics
of the Yocto Project. The manual is neither meant to be read as a
starting point to the Yocto Project, nor read from start to finish.
Rather, use this manual to find variable definitions, class
descriptions, and so forth as needed during the course of using the
Yocto Project.

For introductory information on the Yocto Project, see the
`Yocto Project Website <>` and the
"overview-manual/development-environment:the yocto project development environment"
chapter in the Yocto Project Overview and Concepts Manual.

If you want to use the Yocto Project to quickly build an image without
having to understand concepts, work through the
/brief-yoctoprojectqs/index document. You can find "how-to"
information in the /dev-manual/index. You can find Yocto Project overview
and conceptual information in the /overview-manual/index.

> **Note:**
> For more information about the Yocto Project Documentation set, see
> the ref-manual/resources:links and related documentation section.
>

## Minimum Free Disk Space

To build an image such as `core-image-sato` for the `qemux86-64` machine,
you need a system with at least &MIN_DISK_SPACE; Gbytes of free disk space.
However, much more disk space will be necessary to build more complex images,
to run multiple builds and to cache build artifacts, improving build efficiency.

If you have a shortage of disk space, see the "/dev-manual/disk-space"
section of the Development Tasks Manual.

## Minimum System RAM

You will manage to build an image such as `core-image-sato` for the
`qemux86-64` machine with as little as &MIN_RAM; Gbytes of RAM on an old
system with 4 CPU cores, but your builds will be much faster on a system with
as much RAM and as many CPU cores as possible.

## Supported Linux Distributions

Currently, the &DISTRO; release ("&DISTRO_NAME;") of the Yocto Project is
supported on the following distributions:

   Can be generated with yocto-autobuilder-helper's scripts/yocto-supported-distros.
   yocto-supported-distros --release master --config yocto-autobuilder2/config.py --output-format docs

-  AlmaLinux 8
-  AlmaLinux 9
-  CentOS Stream 9
-  Debian GNU/Linux 11 (Bullseye)
-  Debian GNU/Linux 12 (Bookworm)
-  Debian GNU/Linux 13 (Trixie)
-  Fedora 39
-  Fedora 40
-  Fedora 42
-  Fedora 43
-  OpenSUSE Leap 15.5
-  OpenSUSE Leap 15.6
-  Rocky Linux 8
-  Rocky Linux 9
-  Ubuntu 22.04 (LTS)
-  Ubuntu 24.04 (LTS)
-  Ubuntu 25.04
-  Ubuntu 25.10

The following distribution versions are still tested, even though the
organizations publishing them no longer make updates publicly available:

   This list contains EOL distros that are still tested on the Autobuilder
   (meaning there are running workers).
   See https://endoflife.date for information of EOL releases.

-  Fedora 39
-  Fedora 40
-  OpenSUSE Leap 15.5
-  Ubuntu 25.04

Note that the Yocto Project doesn't have access to private updates
that some of these versions may have. Therefore, our testing has
limited value if you have access to such updates.  Also, **buildtools**
may have to be used on older releases in order to get current enough
tools, such as Python.

Finally, here are the distribution versions which were previously
tested on former revisions of "&DISTRO_NAME;", but no longer are:

   On the master branch it does not really make sense to fill this list. We
   should maintain the list below on stable branches once we drop support for
   distributions.
   The list can be generated with yocto-autobuilder-helper's scripts/yocto-supported-distros:
   yocto-supported-distros --release <release> --config yocto-autobuilder2/config.py --output-format docs --old-distros

*This list is currently empty*

> **Note:**
> -  While the Yocto Project Team attempts to ensure all Yocto Project
> releases are one hundred percent compatible with each officially
> supported Linux distribution, you may still encounter problems
> that happen only with a specific distribution.
>
> -  Yocto Project releases are tested against the stable Linux
> distributions in the above list. The Yocto Project should work
> on other distributions but validation is not performed against
> them.
>
> -  In particular, the Yocto Project does not support and currently
> has no plans to support rolling-releases or development
> distributions due to their constantly changing nature. We welcome
> patches and bug reports, but keep in mind that our priority is on
> the supported platforms listed above.
>
> -  If your Linux distribution is not in the above list, we recommend to
> get the **buildtools** or **buildtools-extended** tarballs
> containing the host tools required by your Yocto Project release,
> typically by running `scripts/install-buildtools` as explained in
> the "system-requirements-buildtools" section.
>
> -  You may use Windows Subsystem For Linux v2 to set up a build host
> using Windows 10 or later, or Windows Server 2019 or later, but validation
> is not performed against build hosts using WSL 2.
>
> See the
> dev-manual/start:setting up to use windows subsystem for linux (wsl 2)
> section in the Yocto Project Development Tasks Manual for more information.
>
> -  If you encounter problems, please go to :yocto_bugs:`Yocto Project
> Bugzilla <>` and submit a bug. We are
> interested in hearing about your experience. For information on
> how to submit a bug, see the Yocto Project
> Bugzilla wiki page 
> and the "/contributor-guide/report-defect"
> section in the Yocto Project and OpenEmbedded Contributor Guide.
>

## Required Packages for the Build Host

The list of packages you need on the host development system can be
large when covering all build scenarios using the Yocto Project. This
section describes required packages according to Linux distribution and
function.

### Ubuntu and Debian

Here are the packages needed to build Project documentation manuals:

.. literalinclude:: ../tools/host_packages_scripts/ubuntu_docs.sh
   :language: shell

.. literalinclude:: ../tools/host_packages_scripts/pip3_docs.sh
   :language: shell

In addition to the previous packages, here are the packages needed to build the
documentation in PDF format:

.. literalinclude:: ../tools/host_packages_scripts/ubuntu_docs_pdf.sh
   :language: shell

### Fedora Packages

Here are the packages needed to build an image on a headless system
with a supported Fedora Linux distribution:

.. literalinclude:: ../tools/host_packages_scripts/fedora_essential.sh
   :language: shell

Here are the packages needed to build Project documentation manuals:

.. literalinclude:: ../tools/host_packages_scripts/fedora_docs.sh
   :language: shell

.. literalinclude:: ../tools/host_packages_scripts/pip3_docs.sh
   :language: shell

In addition to the previous packages, here are the packages needed to build the
documentation in PDF format:

.. literalinclude:: ../tools/host_packages_scripts/fedora_docs_pdf.sh
   :language: shell

### openSUSE Packages

Here are the packages needed to build an image on a headless system
with a supported openSUSE distribution:

.. literalinclude:: ../tools/host_packages_scripts/opensuse_essential.sh
   :language: shell

Here are the packages needed to build Project documentation manuals:

.. literalinclude:: ../tools/host_packages_scripts/opensuse_docs.sh
   :language: shell

.. literalinclude:: ../tools/host_packages_scripts/pip3_docs.sh
   :language: shell

In addition to the previous packages, here are the packages needed to build the
documentation in PDF format:

.. literalinclude:: ../tools/host_packages_scripts/opensuse_docs_pdf.sh
   :language: shell

### AlmaLinux Packages

Here are the packages needed to build an image on a headless system
with a supported AlmaLinux distribution:

.. literalinclude:: ../tools/host_packages_scripts/almalinux_essential.sh
   :language: shell

> **Note:**
> -  Extra Packages for Enterprise Linux (i.e. `epel-release`) is
> a collection of packages from Fedora built on RHEL/CentOS for
> easy installation of packages not included in enterprise Linux
> by default. You need to install these packages separately.
>
> -  The `PowerTools/CRB` repo provides additional packages such as
> `rpcgen` and `texinfo`.
>
> -  The `makecache` command consumes additional Metadata from
> `epel-release`.
>

Here are the packages needed to build Project documentation manuals:

.. literalinclude:: ../tools/host_packages_scripts/almalinux_docs.sh
   :language: shell

.. literalinclude:: ../tools/host_packages_scripts/pip3_docs.sh
   :language: shell

In addition to the previous packages, the following TeX Live packages
are needed to build the documentation in PDF format and can be installed with
the `TeX Live package manager <https://tug.org/texlive/tlmgr.html>`__:

.. literalinclude:: ../tools/host_packages_scripts/tlmgr_docs_pdf.sh
   :language: shell

> **Warning:**
> The Tex Live installation above is only valid for `x86_64` hosts.
>

Before building the documentation PDF, setup the `PATH` to use the installed
packages (with `${textooldir}` pointing to the TeX Live installation from the
previous step):

```console
$ export PATH="${PATH}:${textooldir}/tl/bin/x86_64-linux"
```

### CentOS Stream Packages

Here are the packages needed to build an image on a headless system
with a supported CentOS Stream distribution:

.. literalinclude:: ../tools/host_packages_scripts/centosstream_essential.sh
   :language: shell

Here are the packages needed to build Project documentation manuals:

.. literalinclude:: ../tools/host_packages_scripts/centosstream_docs.sh
   :language: shell

.. literalinclude:: ../tools/host_packages_scripts/pip3_docs.sh
   :language: shell

In addition to the previous packages, the following TeX Live packages
are needed to build the documentation in PDF format and can be installed with
the `TeX Live package manager <https://tug.org/texlive/tlmgr.html>`__:

.. literalinclude:: ../tools/host_packages_scripts/tlmgr_docs_pdf.sh
   :language: shell

> **Warning:**
> The Tex Live installation above is only valid for `x86_64` hosts.
>

Before building the documentation PDF, setup the `PATH` to use the installed
packages (with `${textooldir}` pointing to the TeX Live installation from the
previous step):

```console
$ export PATH="${PATH}:${textooldir}/tl/bin/x86_64-linux"
```

### RockyLinux Packages

Here are the packages needed to build an image on a headless system
with a supported RockyLinux distribution:

.. literalinclude:: ../tools/host_packages_scripts/rockylinux_essential.sh
   :language: shell

> **Note:**
> -  Extra Packages for Enterprise Linux (i.e. `epel-release`) is
> a collection of packages from Fedora built on RHEL/CentOS for
> easy installation of packages not included in enterprise Linux
> by default. You need to install these packages separately.
>
> -  The `PowerTools/CRB` repo provides additional packages such as
> `rpcgen` and `texinfo`.
>
> -  The `makecache` command consumes additional Metadata from
> `epel-release`.
>

Here are the packages needed to build Project documentation manuals:

.. literalinclude:: ../tools/host_packages_scripts/rockylinux_docs.sh
   :language: shell

.. literalinclude:: ../tools/host_packages_scripts/pip3_docs.sh
   :language: shell

In addition to the previous packages, the following TeX Live packages
are needed to build the documentation in PDF format and can be installed with
the `TeX Live package manager <https://tug.org/texlive/tlmgr.html>`__:

.. literalinclude:: ../tools/host_packages_scripts/tlmgr_docs_pdf.sh
   :language: shell

> **Warning:**
> The Tex Live installation above is only valid for `x86_64` hosts.
>

Before building the documentation PDF, setup the `PATH` to use the installed
packages (with `${textooldir}` pointing to the TeX Live installation from the
previous step):

```console
$ export PATH="${PATH}:${textooldir}/tl/bin/x86_64-linux"
```

## Required Git, tar, Python, make and gcc Versions

In order to use the build system, your host development system must meet
the following version requirements for Git, tar, and Python:

-  Git &MIN_GIT_VERSION; or greater

-  tar &MIN_TAR_VERSION; or greater

-  Python &MIN_PYTHON_VERSION; or greater

-  GNU make &MIN_MAKE_VERSION; or greater

If your host development system does not meet all these requirements,
you can resolve this by installing a **buildtools** tarball that
contains these tools. You can either download a pre-built tarball or
use BitBake to build one.

In addition, your host development system must meet the following
version requirement for gcc:

-  gcc &MIN_GCC_VERSION; or greater

If your host development system does not meet this requirement, you can
resolve this by installing a **buildtools-extended** tarball that
contains additional tools, the equivalent of the Debian/Ubuntu `build-essential`
package.

For systems with a broken make version (e.g. make 4.2.1 without patches) but
where the rest of the host tools are usable, you can use the **buildtools-make**
tarball instead.

In the sections that follow, three different methods will be described for
installing the **buildtools**, **buildtools-extended** or **buildtools-make**
toolset.

### Installing a Pre-Built ``buildtools`` Tarball with ``install-buildtools`` script

The `install-buildtools` script is the easiest of the three methods by
which you can get these tools. It downloads a pre-built **buildtools**
installer and automatically installs the tools for you:

#. Execute the `install-buildtools` script. Here is an example::

      $ cd /path/to/bitbake-builds
      $ ./layers/openembedded-core/scripts/install-buildtools \
        --without-extended-buildtools \
        --base-url &YOCTO_DL_URL;/releases/yocto \
        --release yocto-&DISTRO; \
        --installer-version &DISTRO;

   During execution, the **buildtools** tarball will be downloaded, the
   checksum of the download will be verified, the installer will be run
   for you, and some basic checks will be run to make sure the
   installation is functional.

   To avoid the need of `sudo` privileges, the `install-buildtools`
   script will by default tell the installer to install in::

      /path/to/bitbake-builds/buildtools

   If your host development system needs the additional tools provided
   in the **buildtools-extended** tarball, you can instead execute the
   `install-buildtools` script with the default parameters::

      $ cd /path/to/bitbake-builds
      $ ./layers/openembedded-core/scripts/install-buildtools

   Alternatively if your host development system has a broken `make`
   version such that you only need a known good version of `make`,
   you can use the `--make-only` option::

      $ cd /path/to/bitbake-builds
      $ ./layers/openembedded-core/scripts/install-buildtools --make-only

#. Source the tools environment setup script by using a command like the
   following::

      $ source /path/to/bitbake-builds/buildtools/environment-setup-x86_64-pokysdk-linux

   After you have sourced the setup script, the tools are added to
   `PATH` and any other environment variables required to run the
   tools are initialized. The results are working versions versions of
   Git, tar, Python and `chrpath`. And in the case of the
   **buildtools-extended** tarball, additional working versions of tools
   including `gcc`, `make` and the other tools included in
   `packagegroup-core-buildessential`.

### Downloading a Pre-Built ``buildtools`` Tarball

If you would prefer not to use the `install-buildtools` script, you can instead
download and run a pre-built **buildtools** installer yourself with the following
steps:

#. Go to `/releases/yocto/&DISTRO_REL_LATEST_TAG;/buildtools/`, locate and
   download the `.sh` file corresponding to your host architecture
   and to **buildtools**, **buildtools-extended** or **buildtools-make**.

#. Execute the installation script. Here is an example for the
   traditional installer::

      $ sh ~/Downloads/x86_64-buildtools-nativesdk-standalone-&DISTRO;.sh

   Here is an example for the extended installer::

      $ sh ~/Downloads/x86_64-buildtools-extended-nativesdk-standalone-&DISTRO;.sh

   An example for the make-only installer::

      $ sh ~/Downloads/x86_64-buildtools-make-nativesdk-standalone-&DISTRO;.sh

   During execution, a prompt appears that allows you to choose the
   installation directory. For example, you could choose the following:
   `/home/your-username/buildtools`

#. As instructed by the installer script, you will have to source the tools
   environment setup script::

      $ source /home/your_username/buildtools/environment-setup-x86_64-pokysdk-linux

   After you have sourced the setup script, the tools are added to
   `PATH` and any other environment variables required to run the
   tools are initialized. The results are working versions versions of
   Git, tar, Python and `chrpath`. And in the case of the
   **buildtools-extended** tarball, additional working versions of tools
   including `gcc`, `make` and the other tools included in
   `packagegroup-core-buildessential`.

### Building Your Own ``buildtools`` Tarball

Building and running your own **buildtools** installer applies only when you
have a build host that can already run BitBake. In this case, you use
that machine to build the `.sh` file and then take steps to transfer
and run it on a machine that does not meet the minimal Git, tar, and
Python (or gcc) requirements.

Here are the steps to take to build and run your own **buildtools**
installer:

#. On the machine that is able to run BitBake, be sure you have set up
   your build environment with the setup script
   (structure-core-script).

#. Run the BitBake command to build the tarball::

      $ bitbake buildtools-tarball

   or to build the extended tarball::

      $ bitbake buildtools-extended-tarball

   or to build the make-only tarball::

      $ bitbake buildtools-make-tarball

   .. note::

      The **SDKMACHINE** variable in your `local.conf` file determines
      whether you build tools for a 32-bit or 64-bit system.

   Once the build completes, you can find the `.sh` file that installs
   the tools in the `tmp/deploy/sdk` subdirectory of the
   **Build Directory**. The installer file has the string
   "buildtools" or "buildtools-extended" in the name.

#. Transfer the `.sh` file from the build host to the machine that
   does not meet the Git, tar, or Python (or gcc) requirements.

#. On this machine, run the `.sh` file to install the tools. Here is an
   example for the traditional installer::

      $ sh ~/Downloads/x86_64-buildtools-nativesdk-standalone-&DISTRO;.sh

   For the extended installer::

      $ sh ~/Downloads/x86_64-buildtools-extended-nativesdk-standalone-&DISTRO;.sh

   And for the make-only installer::

      $ sh ~/Downloads/x86_64-buildtools-make-nativesdk-standalone-&DISTRO;.sh

   During execution, a prompt appears that allows you to choose the
   installation directory. For example, you could choose the following:
   `/home/your_username/buildtools`

#. Source the tools environment setup script by using a command like the
   following::

      $ source /home/your_username/buildtools/environment-setup-x86_64-poky-linux

   After you have sourced the setup script, the tools are added to
   `PATH` and any other environment variables required to run the
   tools are initialized. The results are working versions versions of
   Git, tar, Python and `chrpath`. And in the case of the
   **buildtools-extended** tarball, additional working versions of tools
   including `gcc`, `make` and the other tools included in
   `packagegroup-core-buildessential`.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Yocto Project Terms

Here is a list of terms and definitions users new to the Yocto Project
development environment might find helpful. While some of these terms are
universal, the list includes them just in case:

.. glossary::

   **Append Files**
      Files that append build information to a recipe file.  Append files are
      known as BitBake append files and `.bbappend` files. The OpenEmbedded
      build system expects every append file to have a corresponding recipe
      (`.bb`) file. Furthermore, the append file and corresponding recipe file
      must use the same root filename.  The filenames can differ only in the
      file type suffix used (e.g. `formfactor_0.0.bb` and
      `formfactor_0.0.bbappend`).

      Information in append files extends or overrides the information in the
      similarly-named recipe file. For an example of an append file in use, see
      the    "dev-manual/layers:appending other layers metadata with your layer"
      section in the Yocto Project Development Tasks Manual.

      When you name an append file, you can use the "`%`" wildcard character
      to allow for matching recipe names. For example, suppose you have an
      append file named as follows::

         busybox_1.21.%.bbappend

      That append file
      would match any `busybox_1.21.x.bb` version of the recipe. So,
      the append file would match any of the following recipe names:

      .. code-block:: shell

         busybox_1.21.1.bb
         busybox_1.21.2.bb
         busybox_1.21.3.bb
         busybox_1.21.10.bb
         busybox_1.21.25.bb

      .. note::

         The use of the "%" character is limited in that it only works
         directly in front of the .bbappend portion of the append file's
         name. You cannot use the wildcard character in any other location of
         the name.

   **BitBake**
      The task executor and scheduler used by the OpenEmbedded build system to
      build images. For more information on BitBake, see the :doc:`BitBake User
      Manual <bitbake:index>`.

   **Board Support Package (BSP)**
      A group of drivers, definitions, and other components that provide support
      for a specific hardware configuration. For more information on BSPs, see
      the /bsp-guide/index.

   **Build Directory**
      This term refers to the area used by the OpenEmbedded build system for
      builds. The area is created when you `source` the setup environment
      script that is found in the Source Directory
      (i.e. structure-core-script, or
      `init-build-env` when using :doc:`bitbake-setup
      <bitbake:bitbake-user-manual/bitbake-user-manual-environment-setup>`). The
      **TOPDIR** variable points to the **Build Directory**.

      When setting up the build manually (see /dev-manual/poky-manual-setup),
      you have a lot of flexibility when creating the **Build Directory**.
      Here are some examples that show how to create the directory.  The
      examples assume your **Source Directory** is named `bitbake-builds`:

      -  Create the **Build Directory** inside your Source Directory and let
         the name of the **Build Directory** default to `build`:

         .. code-block:: console

            $ cd bitbake-builds
            $ source layers/openembedded-core/oe-init-build-env

      -  Or provide a path to the **Build Directory** when sourcing
         `oe-init-build-env`. Any intermediate folders in the pathname must
         exist. This next example creates a **Build Directory** named
         `YP-&DISTRO;` within the existing directory `mybuilds`:

         .. code-block:: console

            $ source bitbake-builds/layers/openembedded-core/oe-init-build-env mybuilds/YP-&DISTRO;

         This path can also be an absolute path and be outside of the current
         working directory:

         .. code-block:: console

            $ source bitbake-builds/layers/openembedded-core/oe-init-build-env /opt/mybuilds/YP-&DISTRO;

      .. note::

         By default, the **Build Directory** contains **TMPDIR**, which is a
         temporary directory the build system uses for its work. **TMPDIR** cannot
         be under NFS. Thus, by default, the **Build Directory** cannot be under
         NFS. However, if you need the **Build Directory** to be under NFS, you can
         set this up by setting **TMPDIR** in your
         structure-build-conf-site.conf file to use a local drive. Doing
         so effectively separates **TMPDIR** from **TOPDIR**, which is
         the **Build Directory**.

   **Build Host**
      The system used to build images in a Yocto Project Development
      environment. The build system is sometimes referred to as the development
      host.

   **buildtools**
      Build tools in binary form, providing required versions of development
      tools (such as Git, GCC, Python and make), to run the OpenEmbedded build
      system on a development host without such minimum versions.

      See the "system-requirements-buildtools" paragraph in the
      Reference Manual for details about downloading or building an archive
      of such tools.

   **buildtools-extended**
      A set of **buildtools** binaries extended with additional development
      tools, such as a required version of the GCC compiler to run the
      OpenEmbedded build system.

      See the "system-requirements-buildtools" paragraph in the
      Reference Manual for details about downloading or building an archive
      of such tools.

   **buildtools-make**
      A variant of **buildtools**, just providing the required
      version of `make` to run the OpenEmbedded build system.

   **Built-in Fragment**
      A built-in fragment is a specific kind of **Configuration Fragment**
      that affects the value of a single variable globally. :term:`Built-in
      Fragments <Built-in Fragment>` do not require a separate configuration
      file, but like a standard **Configuration Fragment**, Built-in
      Fragments can be enabled or disabled using the :oe_git:`bitbake-config-build
      </bitbake/tree/bin/bitbake-config-build>` command-line utility.

      When declared, a built-in fragment follows the following naming
      convention::

         <fragment>:<variable name>

      Where:

      -  `<fragment>` is the name of the built-in fragment.
      -  `<variable name>` is the name of the variable to be modified by this
         fragment.

      For example::

         machine:MACHINE

      Will setup the `machine` Built-in Fragment for modifying the value of
      the **MACHINE** variable.

      Setting the **MACHINE** variable through this fragment must follow
      this syntax::

         machine/qemux86-64

      This sets the value of **MACHINE** to `qemux86-64`.

      In **OpenEmbedded-Core (OE-Core)**, the list of available
      **Built-in Fragments <Built-in Fragment>** can be obtained from the
      **OE_FRAGMENTS_BUILTIN** variable.

      For more details on fragments, see:

      -  The /ref-manual/fragments section of the Yocto Project Reference
         Manual for a list of fragments the **OpenEmbedded Build System**
         supports, and a quick reference guide on how to manage fragments.

      -  The /dev-manual/creating-fragments section of the Yocto Project
         Development Tasks Manual for details on how to create new fragments
         in your build.

   **Classes**
      Files that provide for logic encapsulation and inheritance so that
      commonly used patterns can be defined once and then easily used in
      multiple recipes. For reference information on the Yocto Project classes,
      see the "ref-manual/classes:Classes" chapter. Class files end with the
      `.bbclass` filename extension.

   **Configuration File**
      Files that hold global definitions of variables, user-defined variables,
      and hardware configuration information. These files tell the OpenEmbedded
      build system what to build and what to put into the image to support a
      particular platform.

      Configuration files end with a `.conf` filename extension. The
      `conf/local.conf` configuration file in the **Build Directory**
      contains user-defined variables that affect every build. The
      `meta-poky/conf/distro/poky.conf` configuration file defines Yocto
      "distro" configuration variables used only when building with this
      policy. Machine configuration files, which are located throughout the
      **Source Directory**, define variables for specific hardware and are
      only used when building for that target (e.g. the
      `machine/beaglebone.conf` configuration file defines variables for
      the Texas Instruments ARM Cortex-A8 development board).
      **Configuration Fragments <Configuration Fragment>** such as
      ref-fragments-core-yocto-sstate-mirror-cdn define snippets of
      configuration that can be enabled from the command-line.

   **Configuration Fragment**
      A **Configuration Fragment** (also called Standard :term:`Configuration
      Fragment`) is a **configuration file** that contains configuration
      statements such as variable assignments, affecting the build at a
      global-level when the fragment is enabled. By default, configuration
      fragments are located in the `conf/fragments/` directory of a
      **Layer**.

      .. note::

         Another form of fragment not to be confounded with Standard
         **Configuration Fragments <Configuration Fragment>** are
         **Built-in Fragments <Built-in Fragment>** which are used to assign
         a single variable value globally.

      A fragment **configuration file** must contain a summary
      (**BB_CONF_FRAGMENT_SUMMARY**) and a description
      (**BB_CONF_FRAGMENT_DESCRIPTION**) explaining the purpose of the
      fragment.

      In **OpenEmbedded-Core (OE-Core)**, the location of fragments and what
      variables are required in a fragment is specified in :oe_git:`bitbake.conf
      </openembedded-core/tree/meta/conf/bitbake.conf>` thanks to the
      addfragments <bitbake-user-manual/bitbake-user-manual-metadata:\\`addfragments\`\`
      directive>` directive and the **OE_FRAGMENTS**,
      **OE_FRAGMENTS_METADATA_VARS** and **OE_FRAGMENTS_BUILTIN**
      variables.

      Fragments can be listed, enabled and disabled with the
      bitbake-config-build 
      command-line utility.

      For more details on fragments, see:

      -  The /ref-manual/fragments section of the Yocto Project Reference
         Manual for a list of fragments the **OpenEmbedded Build System**
         supports, and a quick reference guide on how to manage fragments.

      -  The /dev-manual/creating-fragments section of the Yocto Project
         Development Tasks Manual for details on how to create new fragments
         in your build.

   **Container Layer**
      A flexible definition that typically refers to a single Git checkout
      which contains multiple (and typically related) sub-layers which can
      be included independently in your project's `bblayers.conf` file.

      In some cases, such as with OpenEmbedded's meta-openembedded 
      layer, the top level `meta-openembedded/` directory is not itself an actual layer,
      so you would never explicitly include it in a `bblayers.conf` file;
      rather, you would include any number of its layer subdirectories, such as
      meta-oe , :oe_git:`meta-python
      </meta-openembedded/tree/meta-python>` and so on.

      On the other hand, some container layers (such as
      meta-security )
      have a top-level directory that is itself an actual layer, as well as
      a variety of sub-layers, both of which could be included in your
      `bblayers.conf` file.

      In either case, the phrase "container layer" is simply used to describe
      a directory structure which contains multiple valid OpenEmbedded layers.

   **Cross-Development Toolchain**
      In general, a cross-development toolchain is a collection of software
      development tools and utilities that run on one architecture and allow you
      to develop software for a different, or targeted, architecture. These
      toolchains contain cross-compilers, linkers, and debuggers that are
      specific to the target architecture.

      The Yocto Project supports two different cross-development toolchains:

      - A toolchain only used by and within BitBake when building an image for a
        target architecture.

      - A relocatable toolchain used outside of BitBake by developers when
        developing applications that will run on a targeted device.

      Creation of these toolchains is simple and automated. For information on
      toolchain concepts as they apply to the Yocto Project, see the
      ":ref:`overview-manual/concepts:Cross-Development
      Toolchain Generation`" section in the Yocto Project Overview and Concepts
      Manual. You can also find more information on using the relocatable
      toolchain in the /sdk-manual/index manual.

   **Extensible Software Development Kit (eSDK)**
      A custom SDK for application developers. This eSDK allows developers to
      incorporate their library and programming changes back into the image to
      make their code available to other application developers.

      For information on the eSDK, see the /sdk-manual/index manual.

   **Image**
      An image is an artifact of the BitBake build process given a collection of
      recipes and related Metadata. Images are the binary output that run on
      specific hardware or QEMU and are used for specific use-cases. For a list
      of the supported image types that the Yocto Project provides, see the
      "ref-manual/images:Images" chapter.

   **Initramfs**
      An Initial RAM Filesystem (**Initramfs**) is an optionally compressed
      cpio  archive which is extracted
      by the Linux kernel into RAM in a special tmpfs 
      instance, used as the initial root filesystem.

      This is a replacement for the legacy init RAM disk ("initrd")
      technique, booting on an emulated block device in RAM, but being less
      efficient because of the overhead of going through a filesystem and
      having to duplicate accessed file contents in the file cache in RAM,
      as for any block device.

      .. note::

         As far as bootloaders are concerned, **Initramfs** and "initrd"
         images are still copied to RAM in the same way. That's why most
         most bootloaders refer to **Initramfs** images as "initrd"
         or "init RAM disk".

      This kind of mechanism is typically used for two reasons:

      -  For booting the same kernel binary on multiple systems requiring
         different device drivers. The **Initramfs** image is then customized
         for each type of system, to include the specific kernel modules
         necessary to access the final root filesystem. This technique
         is used on all GNU / Linux distributions for desktops and servers.

      -  For booting faster. As the root filesystem is extracted into RAM,
         accessing the first user-space applications is very fast, compared
         to having to initialize a block device, to access multiple blocks
         from it, and to go through a filesystem having its own overhead.
         For example, this allows to display a splashscreen very early,
         and to later take care of mounting the final root filesystem and
         loading less time-critical kernel drivers.

      This cpio archive can either be loaded to RAM by the bootloader,
      or be included in the kernel binary.

      For information on creating and using an **Initramfs**, see the
      "dev-manual/building:building an initial ram filesystem (Initramfs) image"
      section in the Yocto Project Development Tasks Manual.

   **Layer**
      A collection of related recipes. Layers allow you to consolidate related
      metadata to customize your build. Layers also isolate information used
      when building for multiple architectures.  Layers are hierarchical in
      their ability to override previous specifications. You can include any
      number of available layers from the Yocto Project and customize the build
      by adding your layers after them. You can search the Layer Index for
      layers used within Yocto Project.

      For introductory information on layers, see the
      ":ref:`overview-manual/yp-intro:The Yocto Project Layer
      Model`" section in the Yocto Project Overview and Concepts Manual. For
      more detailed information on layers, see the
      ":ref:`dev-manual/layers:Understanding and Creating
      Layers`" section in the Yocto Project Development Tasks Manual. For a
      discussion specifically on BSP Layers, see the ":ref:`bsp-guide/bsp:BSP
      Layers`" section in the Yocto Project Board Support Packages (BSP)
      Developer's Guide.

   **LTS**
      This term means "Long Term Support", and in the context of the Yocto
      Project, it corresponds to selected stable releases for which bug and
      security fixes are provided for at least four years. See
      the ref-long-term-support-releases section for details.

   **Metadata**
      A key element of the Yocto Project is the Metadata that
      is used to construct a Linux distribution and is contained in the
      files that the **OpenEmbedded Build System**
      parses when building an image. In general, Metadata includes recipes,
      configuration files, and other information that refers to the build
      instructions themselves, as well as the data used to control what
      things get built and the effects of the build. Metadata also includes
      commands and data used to indicate what versions of software are
      used, from where they are obtained, and changes or additions to the
      software itself (patches or auxiliary files) that are used to fix
      bugs or customize the software for use in a particular situation.
      OpenEmbedded-Core is an important set of validated metadata.

      In the context of the kernel ("kernel Metadata"), the term refers to
      the kernel config fragments and features contained in the
      yocto-kernel-cache 
      Git repository.

   **Mixin**
      A **Mixin** layer is a layer which can be created by the community to
      add a specific feature or support a new version of some package for an
      **LTS** release. See the ref-long-term-support-releases
      section for details.

   **OpenEmbedded Build System**
      The build system used by the Yocto Project, using **Bitbake** as the
      task executor. Throughout the Yocto Project documentation, the
      OpenEmbedded build system is sometimes referred to simply as "the build
      system". If other build systems, such as a host or target build system are
      referenced, the documentation clearly states the difference.

      .. note::

         For some historical information about Poky, see the **Poky** term.

   **OpenEmbedded-Core (OE-Core)**
      OE-Core is metadata comprised of
      foundational recipes, classes, and associated files that are meant to
      be common among many different OpenEmbedded-derived systems,
      including the Yocto Project. OE-Core is a curated subset of an
      original repository developed by the OpenEmbedded community that has
      been pared down into a smaller, core set of continuously validated
      recipes. The result is a tightly controlled and an quality-assured
      core set of recipes.

      You can browse the source of **OpenEmbedded-Core (OE-Core)** at
      openembedded-core .

   **Package**
      In the context of the Yocto Project, this term refers to a
      recipe's packaged output produced by BitBake (i.e. a "baked recipe").
      A package is generally the compiled binaries produced from the
      recipe's sources. You "bake" something by running it through BitBake.

      It is worth noting that the term "package" can, in general, have
      subtle meanings. For example, the packages referred to in the
      "ref-manual/system-requirements:required packages for the build host"
      section are compiled binaries that, when installed, add functionality to
      your Linux distribution.

      Another point worth noting is that historically within the Yocto
      Project, recipes were referred to as packages --- thus, the existence
      of several BitBake variables that are seemingly mis-named, (e.g.
      **PR**, **PV**, and
      **PE**).

   **Package Groups**
      Arbitrary groups of software Recipes. You use
      package groups to hold recipes that, when built, usually accomplish a
      single task. For example, a package group could contain the recipes
      for a company's proprietary or value-add software. Or, the package
      group could contain the recipes that enable graphics. A package group
      is really just another recipe. Because package group files are
      recipes, they end with the `.bb` filename extension.

   **Poky**
      Poky, which is pronounced *Pock*-ee, is a reference embedded
      distribution and a reference test configuration. Poky provides the
      following:

      -  A base-level functional distro used to illustrate how to customize
         a distribution.

      -  A means by which to test the Yocto Project components (i.e. Poky
         is used to validate the Yocto Project).

      Poky is not a product level distro. Rather, it is a good starting
      point for customization.

      .. note::

         Poky also used to be a repository containing the combination of
         **BitBake**, **OpenEmbedded-Core (OE-Core)**, the
         meta-yocto  repository and the Yocto Project
         documentation -- which were combined with the :yocto_git:`combo-layer
         </poky-config>` tool.

         This repository is still hosted at `/poky` and is still
         updated for maintained releases that are older than Whinlatter (5.3).
         Newer releases now use `bitbake-setup` to clone and setup the initial
         **OpenEmbedded Build System** environment. For more information on
         how to use `bitbake-setup`, see the /brief-yoctoprojectqs/index
         document. An alternative can be to setup the above repositories
         manually, by following the /dev-manual/poky-manual-setup
         section of the Yocto Project Development Tasks Manual.

      .. note::

         Poky began as an open-source project initially developed by
         OpenedHand. OpenedHand developed Poky from the existing
         OpenEmbedded build system to create a commercially supportable
         build system for embedded Linux. After Intel Corporation acquired
         OpenedHand, the poky project became the basis for the Yocto
         Project's build system.

   **Recipe**
      A set of instructions for building packages. A recipe
      describes where you get source code, which patches to apply, how to
      configure the source, how to compile it and so on. Recipes also
      describe dependencies for libraries or for other recipes. Recipes
      represent the logical unit of execution, the software to build, the
      images to build, and use the `.bb` file extension.

   **Reference Kit**
      A working example of a system, which includes a
      **BSP<Board Support Package (BSP)>** as well as a
      **build host<Build Host>** and other components, that can
      work on specific hardware.

   **SBOM**
      This term means *Software Bill of Materials*. When you distribute
      software, it offers a description of all the components you used,
      their corresponding licenses, their dependencies, the changes that were
      applied and the known vulnerabilities that were fixed.

      This can be used by the recipients of the software to assess
      their exposure to license compliance and security vulnerability issues.

      See the Software Supply Chain 
      article on Wikipedia for more details.

      The OpenEmbedded Build System can generate such documentation for your
      project, in **SPDX** format, based on all the metadata it used to
      build the software images. See the ":ref:`dev-manual/sbom:creating
      a software bill of materials`" section of the Development Tasks manual.

   **Source Directory**
     This term refers to the directory structure created as a result of setting
     up your environment to build images with the Yocto Project, which can be
     done in two ways:

     -  Using the `bitbake-setup` command-line utility (see :doc:`Setting Up
        The Environment With bitbake-setup
        <bitbake:bitbake-user-manual/bitbake-user-manual-environment-setup>`).

     -  Or manually setting up the **Layers <Layer>** (see
        /dev-manual/poky-manual-setup).

     In either case, the result will be a set of **Layers <Layer>** and a
     **Build Directory**: this is the Source Directory for your build. The
     documentation usually denotes this directory with the name "bitbake-builds".

     A typical layout for the **Source Directory** would be:

     .. code-block:: text

        bitbake-builds/
        ├── build/
        └── layers/

     Where the `layers/` directory corresponds to the directory containing
     **layers <Layer>** for your project, and the `build/` directory
     corresponds to the **Build Directory**.

     We present a **Source Directory** structure and its components in
     details in the /ref-manual/structure section of the Yocto Project
     Reference Manual.

     .. note::

        The OpenEmbedded build system does not support file or directory
        names that contain spaces. Be sure that the Source Directory you
        use does not contain these types of names.

     The **Source Directory** contains **BitBake**,
     **OpenEmbedded-Core (OE-Core)**, and other files that all support the
     Yocto Project.

     Consequently, you must have the **Source Directory** in place on your
     development system in order to do any development using the Yocto Project.

   **SPDX**
      This term means *Software Package Data Exchange*, and is used as an open
      standard for providing a *Software Bill of Materials* (**SBOM**).
      This standard is developed through a `Linux Foundation project
      <https://spdx.dev/>`__ and is used by the OpenEmbedded Build System to
      provide an **SBOM** associated to each software image.

      For details, see Wikipedia's SPDX page 
      and the "dev-manual/sbom:creating a software bill of materials"
      section of the Development Tasks manual.

   **Sysroot**
      When cross-compiling, the target file system may be differently laid
      out and contain different things compared to the host system. The concept
      of a *sysroot* is directory which looks like the target filesystem and
      can be used to cross-compile against.

      In the context of cross-compiling toolchains, a *sysroot*
      typically contains C library and kernel headers, plus the
      compiled binaries for the C library. A *multilib toolchain*
      can contain multiple variants of the C library binaries,
      each compiled for a target instruction set (such as `armv5`,
      `armv7` and `armv8`), and possibly optimized for a specific CPU core.

      In the more specific context of the OpenEmbedded build System and
      of the Yocto Project, each recipe has two sysroots:

      -  A *target sysroot* contains all the **target** libraries and headers
         needed to build the recipe.

      -  A *native sysroot* contains all the **host** files and executables
         needed to build the recipe.

      See the **SYSROOT_* <SYSROOT_DESTDIR>** variables controlling
      how sysroots are created and stored.

   **Task**
      A per-recipe unit of execution for BitBake (e.g.
      ref-tasks-compile,
      ref-tasks-fetch,
      ref-tasks-patch, and so forth).
      One of the major benefits of the build system is that, since each
      recipe will typically spawn the execution of numerous tasks,
      it is entirely possible that many tasks can execute in parallel,
      either tasks from separate recipes or independent tasks within
      the same recipe, potentially up to the parallelism of your
      build system.

   **Toaster**
      A web interface to the Yocto Project's **OpenEmbedded Build System**.
      The interface enables you to
      configure and run your builds. Information about builds is collected
      and stored in a database. For information on Toaster, see the
      /toaster-manual/index.

   **Upstream**
      A reference to source code or repositories that are not
      local to the development system but located in a remote area that is
      controlled by the maintainer of the source code. For example, in
      order for a developer to work on a particular piece of code, they
      need to first get a copy of it from an "upstream" source.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Yocto Project Releases and the Stable Release Process

The Yocto Project release process is predictable and consists of both
major and minor (point) releases. This brief chapter provides
information on how releases are named, their life cycle, and their
stability.

## Major and Minor Release Cadence

The Yocto Project delivers major releases (e.g. &DISTRO;) using a six
month cadence roughly timed each April and October of the year.
Here are examples of some major YP releases with their codenames
also shown. See the "ref-manual/release-process:major release codenames"
section for information on codenames used with major releases.

  - 4.1 ("Langdale")
  - 4.0 ("Kirkstone")
  - 3.4 ("Honister")

While the cadence is never perfect, this timescale facilitates
regular releases that have strong QA cycles while not overwhelming users
with too many new releases. The cadence is predictable and avoids many
major holidays in various geographies.

The Yocto project delivers minor (point) releases on an unscheduled
basis and are usually driven by the accumulation of enough significant
fixes or enhancements to the associated major release.
Some example past point releases are:

  - 4.1.3
  - 4.0.8
  - 3.4.4

The point release
indicates a point in the major release branch where a full QA cycle and
release process validates the content of the new branch.

> **Note:**
> Realize that there can be patches merged onto the stable release
> branches as and when they become available.
>

## Development Cycle

As explained in the previous :ref:`ref-manual/release-process:Major and Minor
Release Cadence` section, a new release comes out every six months.

During this six-months period of time, the Yocto Project releases four
"Milestone" releases which represent distinct points of time. The milestone
releases are tested through the :ref:`ref-manual/release-process:Testing and
Quality Assurance` process and helps spotting issues before the actual release
is out.

The time span between milestone releases can vary, but they are in general
evenly spaced out during this six-months period of time.

These milestone releases are tagged with a capital "M" after the future release
tag name. For example, the milestone tags "&DISTRO_RELEASE_SERIES;M1",
"&DISTRO_RELEASE_SERIES;M2", and "&DISTRO_RELEASE_SERIES;M3" are released before
the actual "&DISTRO_RELEASE_SERIES;" release.

> **Note:**
> The fourth milestone (M4) is not actually released and announced, but
> represents a point of time for the Quality Assurance team to start the
> ref-manual/release-process:Testing and Quality Assurance process
> before tagging and delivering the final release.
>

After the third milestone release (M3), the Yocto Project enters **Feature
Freeze**. This means that the maintainers of :term:`OpenEmbedded-Core
(OE-Core)`, **BitBake** and other core repositories stop accepting
significant changes on the "master" branch. Changes that may be accepted are
minor upgrades to core components and security/bug fixes.

During feature freeze, a new branch is created and maintained separately to
test new features and enhancements received from contributors, but these changes
will only make it to the master branch after the release is out.

## Major Release Codenames

Each major release receives a codename that identifies the release in
the overview-manual/development-environment:yocto project source repositories.
The concept is that branches of **Metadata** with the same
codename are likely to be compatible and thus work together.

> **Note:**
> Codenames are associated with major releases because a Yocto Project
> release number (e.g. &DISTRO;) could conflict with a given layer or
> company versioning scheme. Codenames are unique, interesting, and
> easily identifiable.
>

Releases are given a nominal release version as well but the codename is
used in repositories for this reason. You can find information on Yocto
Project releases and codenames in the :yocto_home:`Releases page
</development/releases/>`.

Our /migration-guides/index detail how to migrate from one release of
the Yocto Project to the next.

## Stable Release Process

Once released, the release enters the stable release process at which
time a person is assigned as the maintainer for that stable release.
This maintainer monitors activity for the release by investigating and
handling nominated patches and backport activity. Only fixes and
enhancements that have first been applied on the "master" branch (i.e.
the current, in-development branch) are considered for backporting to a
stable release.

> **Note:**
> The current Yocto Project policy regarding backporting is to consider
> bug fixes and security fixes only. Policy dictates that features are
> not backported to a stable release. This policy means generic recipe
> version upgrades are unlikely to be accepted for backporting. The
> exception to this policy occurs when there is a strong reason such as
> the fix happens to also be the preferred upstream approach.
>

## Long Term Support Releases

While stable releases are supported for a duration of seven months,
some specific ones are now supported for a longer period by the Yocto
Project, and are called Long Term Support (**LTS**) releases.

When significant issues are found, **LTS** releases allow to publish
fixes not only for the current stable release, but also to the
**LTS** releases that are still supported. Older stable releases which
have reached their End of Life (EOL) won't receive such updates.

This started with version 3.1 ("Dunfell"), released in April 2020, which
the project initially committed to supporting for two years, but this duration
was later extended to four years.

A new **LTS** release is made every two years and is supported for four
years. This offers more stability to project users and leaves more time to
upgrade to the following **LTS** release.

The currently supported **LTS** releases are:

-  Version 5.0 ("Scarthgap"), released in April 2024 and supported until April 2028.
-  Version 4.0 ("Kirkstone"), released in May 2022 and supported until May 2026.

See `/Stable_Release_and_LTS` for details about the management
of stable and **LTS** releases.

This documentation was built for the &DISTRO_NAME; release.

![releases.*](svg/releases.*)

> **Note:**
> In some circumstances, a layer can be created by the community in order to
> add a specific feature or support a new version of some package for an **LTS**
> release. This is called a **Mixin** layer. These are thin and specific
> purpose layers which can be stacked with an **LTS** release to "mix" a specific
> feature into that build. These are created on an as-needed basis and
> maintained by the people who need them.
>
> Policies on testing these layers depend on how widespread their usage is and
> determined on a case-by-case basis. You can find some **Mixin** layers in the
> meta-lts-mixins  repository. While the Yocto
> Project provides hosting for those repositories, it does not provides
> testing on them. Other **Mixin** layers may be released elsewhere by the wider
> community.
>

## Testing and Quality Assurance

Part of the Yocto Project development and release process is quality
assurance through the execution of test strategies. Test strategies
provide the Yocto Project team a way to ensure a release is validated.
Additionally, because the test strategies are visible to you as a
developer, you can validate your projects. This section overviews the
available test infrastructure used in the Yocto Project. For information
on how to run available tests on your projects, see the
"test-manual/runtime-testing:performing automated runtime testing"
section in the Yocto Project Test Environment Manual.

The QA/testing infrastructure is woven into the project to the point
where core developers take some of it for granted. The infrastructure
consists of the following pieces:

-  `bitbake-selftest`: A standalone command that runs unit tests on
   key pieces of BitBake and its fetchers.

-  ref-classes-sanity: This automatically
   included class checks the build environment for missing tools (e.g.
   `gcc`) or common misconfigurations such as
   **MACHINE** set incorrectly.

-  ref-classes-insane: This class checks the
   generated output from builds for sanity. For example, if building for
   an ARM target, did the build produce ARM binaries. If, for example,
   the build produced PPC binaries then there is a problem.

-  ref-classes-testimage: This class
   performs runtime testing of images after they are built. The tests
   are usually used with QEMU 
   to boot the images and check the combined runtime result boot
   operation and functions. However, the test can also use the IP
   address of a machine to test.

-  ptest :
   Runs tests against packages produced during the build for a given
   piece of software. The test allows the packages to be run within a
   target image.

-  `oe-selftest`: Tests combinations of BitBake invocations. These tests
   operate outside the OpenEmbedded build system itself. The
   `oe-selftest` can run all tests by default or can run selected
   tests or test suites.

Originally, much of this testing was done manually. However, significant
effort has been made to automate the tests so that more people can use
them and the Yocto Project development team can run them faster and more
efficiently.

The Yocto Project's main `Autobuilder <>` publicly tests each Yocto
Project release's code in the openembedded-core ,
meta-yocto  and bitbake  repositories. The
testing occurs for both the current state of the "master" branch and also for
submitted patches. Testing for submitted patches usually occurs
in the "master-next" branch in the :oe_git:`openembedded-core
</openembedded-core>`, meta-yocto  and
bitbake  repository.

> **Note:**
> You can find all these branches in the
> overview-manual/development-environment:yocto project source repositories.
>

Testing within these public branches ensures in a publicly visible way
that all of the main supposed architectures and recipes in OE-Core
successfully build and behave properly.

Various features such as `multilib`, sub architectures (e.g. `x32`,
`poky-tiny`, `musl`, `no-x11` and and so forth),
`bitbake-selftest`, and `oe-selftest` are tested as part of the QA
process of a release. Complete testing and validation for a release
takes the Autobuilder workers several hours.

> **Note:**
> The Autobuilder workers are non-homogeneous, which means regular
> testing across a variety of Linux distributions occurs. The
> Autobuilder is limited to only testing QEMU-based setups and not real
> hardware.
>

Finally, in addition to the Autobuilder's tests, the Yocto Project QA
team also performs testing on a variety of platforms, which includes
actual hardware, to ensure expected results.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Source Directory Structure

The **Source Directory** consists of numerous files,
directories and subdirectories; understanding their locations and
contents is key to using the Yocto Project effectively. This chapter
describes the Source Directory and gives information about those files
and directories.

> **Note:**
> The OpenEmbedded build system does not support file or directory
> names that contain spaces. Be sure that the Source Directory you use
> does not contain these types of names.
>

For the following sections, we assume the core components of the :term:`Source
Directory` are organized as follows:

```text
├── build/
│   └── ...
└── layers/
    ├── bitbake/
    ├── meta-yocto/
    ├── openembedded-core/
    └── yocto-docs/
```

## Top-Level Core Components

This section describes the top-level components of the **Source Directory**.

### ``bitbake/``

This directory includes a copy of BitBake for ease of use. The copy
usually matches the current stable BitBake release from the BitBake
project. BitBake, a **Metadata** interpreter, reads the
Yocto Project Metadata and runs the tasks defined by that data. Failures
are usually caused by errors in your Metadata and not from BitBake
itself.

When you run the `bitbake` command, the main BitBake executable (which
resides in the `bitbake/bin/` directory) starts. Sourcing the
environment setup script (i.e. structure-core-script) places
the `scripts/` and `bitbake/bin/` directories (in that order) into
the shell's `PATH` environment variable.

For more information on BitBake, see the :doc:`BitBake User Manual
<bitbake:index>`.

### ``build/``

This directory contains user configuration files and the output
generated by the OpenEmbedded build system in its standard configuration
where the source tree is combined with the output. The **Build Directory**
is created initially when you `source` the OpenEmbedded build environment
setup script (i.e. structure-core-script).

It is also possible to place output and configuration files in a
directory separate from the **Source Directory** by
providing a directory name when you `source` the setup script. For
information on separating output from your local Source Directory files
(commonly described as an "out of tree" build), see the
"structure-core-script" section.

See the "The Build Directory --- build/ " section for details
about the contents of the **Build Directory**.

### ``layers/yocto-docs/``

The Yocto documentation repository is hosted at `/yocto-docs`.

This directory holds the source for the Yocto Project documentation as
well as templates and tools that allow you to generate PDF and HTML
versions of the manuals. Each manual is contained in its own sub-folder;
for example, the files for this reference manual reside in the
`ref-manual/` directory.

This directory is not mandatory for building with **BitBake**.

### ``layers/openembedded-core/``

The **OpenEmbedded-Core (OE-Core)** repository is hosted at
`/openembedded-core`.

#### ``meta/``

This directory contains the minimal, underlying :term:`OpenEmbedded-Core
(OE-Core)` metadata. The directory holds recipes, common classes, and machine
configuration for strictly emulated targets (`qemux86`, `qemuarm`,
and so forth.)

#### ``meta-selftest/``

This directory adds additional recipes and append files used by the
OpenEmbedded selftests to verify the behavior of the build system. You
do not have to add this layer to your `bblayers.conf` file unless you
want to run the selftests.

#### ``meta-skeleton/``

This directory contains template recipes for BSP and kernel development.

#### ``scripts/``

This directory contains various integration scripts that implement extra
functionality in the Yocto Project environment (e.g. QEMU scripts). The
structure-core-script script prepends this directory to the
shell's `PATH` environment variable.

The `scripts` directory has useful scripts that assist in contributing
back to the Yocto Project, such as `create-pull-request` and
`send-pull-request`.

#### ``oe-init-build-env``

This script sets up the OpenEmbedded build environment. Running this
script with the `source` command in a shell makes changes to `PATH`
and sets other core BitBake variables based on the current working
directory. You need to run an environment setup script before running
BitBake commands. The script uses other scripts within the `scripts`
directory to do the bulk of the work.

When you run this script, your Yocto Project environment is set up, a
**Build Directory** is created, your working directory becomes the
**Build Directory**, and you are presented with some simple
suggestions as to what to do next, including a list of some possible
targets to build. Here is an example::

   $ source oe-init-build-env

   ### Shell environment set up for builds. ###

   You can now run 'bitbake <target>'

   Common targets are:
       core-image-minimal
       core-image-sato
       meta-toolchain
       meta-ide-support

   You can also run generated QEMU images with a command like 'runqemu qemux86-64'

> **Note:**
> When using bitbake-setup ,
> this script is indirectly sourced when sourcing the `init-build-env`
> located in the **bitbake:Setup**'s build directory. However, compared to
> `oe-init-build-env`, the bitbake-setup `init-build-env` does not take any
> argument, as it is already located in a **Build Directory**.
>
> See bitbake:bitbake-user-manual/bitbake-user-manual-environment-setup
> for more information.
>

The default output of the `oe-init-build-env` script is from the
`conf-summary.txt` and `conf-notes.txt` files, which are found in the
`meta/conf/templates/default` directory of :term:`OpenEmbedded-Core
(OE-Core)`. If you design a custom distribution, you can include your own
versions of these configuration files where you can provide a brief summary and
detailed usage notes, such as a list of the targets defined by your
distribution. See the
":ref:`dev-manual/custom-template-configuration-directory:creating a custom
template configuration directory`" section in the Yocto Project Development
Tasks Manual for more information.

By default, running this script without a **Build Directory** argument
creates the `build/` directory in your current working directory. If
you provide a **Build Directory** argument when you `source` the script,
you direct the OpenEmbedded build system to create a **Build Directory** of
your choice. For example, the following command creates a
**Build Directory** named `mybuilds/` that is outside of the
**Source Directory**::

   $ source oe-init-build-env ~/mybuilds

The OpenEmbedded build system uses the template configuration files, which
are found by default in the `meta/conf/templates/default` directory in
**OpenEmbedded-Core (OE-Core)**. See the
"dev-manual/custom-template-configuration-directory:creating a custom template configuration directory"
section in the Yocto Project Development Tasks Manual for more
information.

> **Note:**
> The OpenEmbedded build system does not support file or directory
> names that contain spaces. If you attempt to run the `oe-init-build-env`
> script from a **Source Directory** that contains spaces in either the
> filenames or directory names, the script returns an error indicating
> no such file or directory. Be sure to use a **Source Directory** free of
> names containing spaces.
>

### ``layers/meta-yocto/``

The `meta-yocto` repository is hosted at `/meta-yocto`.

#### ``meta-poky/``

Dependent on the **OpenEmbedded-Core (OE-Core)** metadata, this directory
adds just enough metadata to define the Poky reference distribution.

Namely, it contains the distro **configuration file** for Poky:
`conf/distro/poky.conf`.

#### ``meta-yocto-bsp/``

This directory contains the Yocto Project reference hardware Board
Support Packages (BSPs). For more information on BSPs, see the
/bsp-guide/index.

## The Build Directory --- ``build/``

The OpenEmbedded build system creates the **Build Directory** when you run
the build environment setup script structure-core-script. If you do not
give the **Build Directory** a specific name when you run the setup script,
the name defaults to `build/`.

For subsequent parsing and processing, the name of the Build directory
is available via the **TOPDIR** variable.

### ``build/buildhistory/``

The OpenEmbedded build system creates this directory when you enable
build history via the ref-classes-buildhistory class file. The directory
organizes build information into image, packages, and SDK
subdirectories. For information on the build history feature, see the
"dev-manual/build-quality:maintaining build output quality"
section in the Yocto Project Development Tasks Manual.

### ``build/cache/``

This directory contains several internal files used by the OpenEmbedded
build system. The path to this directory is defined by the
**PERSISTENT_DIR** variable.

It also contains `sanity_info`, a text file keeping track of important
build information such as the values of **TMPDIR**, **SSTATE_DIR**,
as well as the name and version of the host distribution.

This directory should not be shared between builds.

> **Note:**
> The default location of the overview-manual/concepts:Hash Equivalence
> database is this directory (when **BB_HASHSERVE** is set to `auto`). If
> you need to share the overview-manual/concepts:Hash Equivalence
> database, don't share this file and instead refer to the
> /dev-manual/hashequivserver document of the Yocto Project Development
> Tasks Manual to setup a Hash Equivalence server.
>

.. _structure-build-conf-local.conf:

### ``build/conf/local.conf``

This configuration file contains the local user configurations for the build
environment.

You could for example experiment with setting (or adding to)
**DISTRO_FEATURES** or **IMAGE_FEATURES**, or adjust build
configurations for specific recipes by setting the **PACKAGECONFIG**
variables for them. If you would like to publish and share changes made to this
file, it is recommended to put them into a distro **configuration file**, or
to create layer **configuration fragments <Configuration Fragment>** from
changes made here.

The **OpenEmbedded Build System** can create the `local.conf` file from a
`local.conf.sample` file when you `source` the top-level build environment
setup script structure-core-script.

The source `local.conf.sample` file used depends on the
**TEMPLATECONF** script variable, which defaults to `layers/meta-yocto/meta-poky/conf/templates/default`
when you are building from the Yocto Project development environment,
and to `layers/openembedded-core/meta/conf/templates/default` when you are building from the OpenEmbedded-Core
environment. Because the script variable points to the source of the
`local.conf.sample` file, this implies that you can configure your
build environment from any layer by setting the variable in the
top-level build environment setup script as follows::

   TEMPLATECONF=your_layer/conf/templates/your_template_name

Once the build process gets the sample
file, it uses `sed` to substitute final
`${`\ **OEROOT**\ `}` values for all
`##OEROOT##` values.

> **Note:**
> You can see how the **TEMPLATECONF** variable is used by looking at the
> `layers/openembedded-core/scripts/oe-setup-builddir` script in **OpenEmbedded-Core (OE-Core)**.
> You can find the **Poky** version of the `local.conf.sample` file in
> the `layers/meta-yocto/meta-poky/conf/templates/default` directory.
>

.. _structure-build-conf-bblayers.conf:

### ``build/conf/bblayers.conf``

This configuration file defines
layers ,
which are directory trees, traversed (or walked) by BitBake. The
`bblayers.conf` file uses the **BBLAYERS**
variable to list the layers BitBake tries to find.

The OpenEmbedded build system can create it from a `bblayers.conf.sample` file
when you `source` the top-level build environment setup script (i.e.
structure-core-script).

As with the structure-build-conf-local.conf file, the source `bblayers.conf.sample`
file used depends on the **TEMPLATECONF** script variable, which
defaults to `meta-poky/conf/templates/default` when you are building using the
**Poky** reference distro, and to `meta/conf/templates/default` when you are
building from the OpenEmbedded-Core environment (default). Because the script
variable points to the source of the `bblayers.conf.sample` file, this
implies that you can base your build from any layer by setting the
variable in the top-level build environment setup script as follows::

   TEMPLATECONF=your_layer/conf/templates/your_template_name

Once the build process gets the sample file, it uses `sed` to substitute final
`${`\ **OEROOT**\ `}` values for all `##OEROOT##` occurrences.

> **Note:**
> You can see how the **TEMPLATECONF** variable is defined by the `scripts/oe-setup-builddir`
> script in **OpenEmbedded-Core (OE-Core)**. You can find the **Poky**
> version of the `bblayers.conf.sample` file in the `layers/meta-yocto/meta-poky/conf/templates/default`
> directory.
>

.. _structure-build-conf-site.conf:

## ``build/conf/site.conf``

This configuration file contains the site specific configurations for your build
environment.

You could for example edit this file to limit the number of threads used by
**BitBake** (**BB_NUMBER_THREADS**) or set the location from which you
want to access downloaded files (**DL_DIR**).

This file can be shared for multiple build directories. For example,
bitbake-setup 
makes the structure-build-conf-site.conf file a symbolic link to a common
`site.conf` file::

   ├── poky-master-poky-distro_poky-machine_qemux86-64/
   │   └── build/
   │       └── conf/
   │           └── site.conf -> ../../../site.conf
   ├── poky-master-poky-with-sstate-distro_poky-machine_qemux86-64/
   │   └── build/
   │       └── conf/
   │           └── site.conf -> ../../../site.conf
   └── site.conf

This way, site-specific settings can be shared for multiple build
configurations.

> **Note:**
> For more information on how to use `bitbake-setup`, see the
> /brief-yoctoprojectqs/index document.
>

.. _structure-build-conf-bblock.conf:

### ``build/conf/bblock.conf``

This configuration file is generated by bblock  and
contains the signatures locked by `bblock`. By default, it does not exist
and will be created upon the first invocation of `bblock`.

.. _structure-build-conf-toolcfg.conf:

### ``build/conf/toolcfg.conf``

This file contains configuration variables that are automatically modified by
tools such as bitbake-config-build .
This file should not be modified manually.

.. _structure-build-conf-auto.conf:

### ``build/conf/auto.conf``

This file is intended for use by continuous integration environments, such as the
Yocto Project's Autobuilder . It
should not be created or modified when running local builds.

### ``build/downloads/``

This directory contains downloaded upstream source tarballs. You can
reuse the directory for multiple builds or move the directory to another
location. You can control the location of this directory through the
**DL_DIR** variable.

### ``build/sstate-cache/``

This directory contains the shared state cache. You can reuse the
directory for multiple builds or move the directory to another location.
You can control the location of this directory through the
**SSTATE_DIR** variable.

### ``build/tmp/``

The OpenEmbedded build system creates and uses this directory for all
the build system's output. The **TMPDIR** variable
points to this directory.

BitBake creates this directory if it does not exist. As a last resort,
to clean up a build and start it from scratch (other than the
downloads), you can remove everything in the `tmp` directory or get
rid of the directory completely. If you do, you should also completely
remove the `build/sstate-cache` directory.

#### ``build/tmp/buildstats/``

This directory stores the build statistics as generated by the
ref-classes-buildstats class.

#### ``build/tmp/cache/``

When BitBake parses the metadata (recipes and configuration files), it
caches the results in `build/tmp/cache/` to speed up future builds.
The results are stored on a per-machine basis.

During subsequent builds, BitBake checks each recipe (together with, for
example, any files included or appended to it) to see if they have been
modified. Changes can be detected, for example, through file
modification time (mtime) changes and hashing of file contents. If no
changes to the file are detected, then the parsed result stored in the
cache is reused. If the file has changed, it is reparsed.

#### ``build/tmp/deploy/``

This directory contains any "end result" output from the OpenEmbedded
build process. The **DEPLOY_DIR** variable points
to this directory. For more detail on the contents of the `deploy`
directory, see the
"overview-manual/concepts:images" and
"overview-manual/concepts:application development sdk" sections in the Yocto
Project Overview and Concepts Manual.

##### ``build/tmp/deploy/deb/``

This directory receives any `.deb` packages produced by the build
process. The packages are sorted into feeds for different architecture
types.

##### ``build/tmp/deploy/rpm/``

This directory receives any `.rpm` packages produced by the build
process. The packages are sorted into feeds for different architecture
types.

##### ``build/tmp/deploy/ipk/``

This directory receives `.ipk` packages produced by the build process.

##### ``build/tmp/deploy/licenses/``

This directory receives package licensing information. For example, the
directory contains sub-directories for `bash`, `busybox`, and
`glibc` (among others) that in turn contain appropriate `COPYING`
license files with other licensing information. For information on
licensing, see the
"dev-manual/licenses:maintaining open source license compliance during your product's lifecycle"
section in the Yocto Project Development Tasks Manual.

##### ``build/tmp/deploy/images/``

This directory is populated with the basic output objects of the build
(think of them as the "generated artifacts" of the build process),
including things like the boot loader image, kernel, root filesystem and
more. If you want to flash the resulting image from a build onto a
device, look here for the necessary components.

Be careful when deleting files in this directory. You can safely delete
old images from this directory (e.g. `core-image-*`). However, the
kernel (`*zImage*`, `*uImage*`, etc.), bootloader and other
supplementary files might be deployed here prior to building an image.
Because these files are not directly produced from the image, if you
delete them they will not be automatically re-created when you build the
image again.

If you do accidentally delete files here, you will need to force them to
be re-created. In order to do that, you will need to know the target
that produced them. For example, these commands rebuild and re-create
the kernel files::

   $ bitbake -c clean virtual/kernel
   $ bitbake virtual/kernel

##### ``build/tmp/deploy/sdk/``

The OpenEmbedded build system creates this directory to hold toolchain
installer scripts which, when executed, install the sysroot that matches
your target hardware. You can find out more about these installers in
the "sdk-manual/appendix-obtain:building an sdk installer"
section in the Yocto Project Application Development and the Extensible
Software Development Kit (eSDK) manual.

#### ``build/tmp/hosttools/``

The OpenEmbedded build system uses this directory to create symbolic links to
some of the host components that are allowed to be called within tasks. These
are basic components listed in the :ref:`ref-manual/system-requirements:required
packages for the build host` section. These components are also listed in the
**HOSTTOOLS** variable and are limited to this list to prevent host
contamination.

#### ``build/tmp/pkgdata/``

The OpenEmbedded build system uses this directory to store package metadata
generated during the ref-tasks-packagedata task. The files stored in this
directory contain information about each output package produced by the
OpenEmbedded build system, and are used in different ways by the build system
such as ":ref:`dev-manual/debugging:viewing package information with
`oe-pkgdata-util``".

#### ``build/tmp/sstate-control/``

The OpenEmbedded build system uses this directory for the shared state
manifest files. The shared state code uses these files to record the
files installed by each sstate task so that the files can be removed
when cleaning the recipe or when a newer version is about to be
installed. The build system also uses the manifests to detect and
produce a warning when files from one task are overwriting those from
another.

#### ``build/tmp/sysroots-components/``

This directory is the location of the sysroot contents that the task
ref-tasks-prepare_recipe_sysroot
links or copies into the recipe-specific sysroot for each recipe listed
in **DEPENDS**. Population of this directory is
handled through shared state, while the path is specified by the
**COMPONENTS_DIR** variable. Apart from a few
unusual circumstances, handling of the `sysroots-components` directory
should be automatic, and recipes should not directly reference
`build/tmp/sysroots-components`.

#### ``build/tmp/sysroots/``

Previous versions of the OpenEmbedded build system used to create a
global shared sysroot per machine along with a native sysroot. Since
the 2.3 version of the Yocto Project, there are sysroots in
recipe-specific **WORKDIR** directories. Thus, the
`build/tmp/sysroots/` directory is unused.

> **Note:**
> The `build/tmp/sysroots/` directory can still be populated using the
> `bitbake build-sysroots` command and can be used for compatibility in some
> cases. However, in general it is not recommended to populate this directory.
> Individual recipe-specific sysroots should be used.
>

#### ``build/tmp/stamps/``

This directory holds information that BitBake uses for accounting
purposes to track what tasks have run and when they have run. The
directory is sub-divided by architecture, package name, and version.
Here is an example::

      stamps/all-poky-linux/distcc-config/1.0-r0.do_build-2fdd....2do

Although the files in the directory are empty of data, BitBake uses the filenames
and timestamps for tracking purposes.

For information on how BitBake uses stamp files to determine if a task
should be rerun, see the
"overview-manual/concepts:stamp files and the rerunning of tasks"
section in the Yocto Project Overview and Concepts Manual.

#### ``build/tmp/log/``

This directory contains general logs that are not otherwise placed using
the package's **WORKDIR**. Examples of logs are the output from the
`do_check_pkg` or `do_distro_check` tasks. Running a build does not
necessarily mean this directory is created.

#### ``build/tmp/work/``

This directory contains architecture-specific work sub-directories for
packages built by BitBake. All tasks execute from the appropriate work
directory. For example, the source for a particular package is unpacked,
patched, configured and compiled all within its own work directory.
Within the work directory, organization is based on the package group
and version for which the source is being compiled as defined by the
**WORKDIR**.

It is worth considering the structure of a typical work directory. As an
example, consider `linux-yocto-kernel-3.0` on the machine `qemux86`
built within the Yocto Project. For this package, a work directory of
`tmp/work/qemux86-poky-linux/linux-yocto/3.0+git1+<.....>`, referred
to as the **WORKDIR**, is created. Within this directory, the source is
unpacked to `sources/linux-qemux86-standard-build` and then patched by Quilt.
(See the "dev-manual/quilt:using quilt in your workflow" section in
the Yocto Project Development Tasks Manual for more information.) Within
the `linux-qemux86-standard-build` directory, standard Quilt
directories `linux-3.0/patches` and `linux-3.0/.pc` are created, and
standard Quilt commands can be used.

There are other directories generated within **WORKDIR**. The most
important directory is `WORKDIR/temp/`, which has log files for each
task (`log.do_*.pid`) and contains the scripts BitBake runs for each
task (`run.do_*.pid`). The `WORKDIR/image/` directory is where "make
install" places its output that is then split into sub-packages within
`WORKDIR/packages-split/`.

##### ``build/tmp/work/tunearch/recipename/version/``

The recipe work directory --- `${WORKDIR}`.

As described earlier in the
"structure-build-tmp-sysroots" section,
beginning with the 2.3 release of the Yocto Project, the OpenEmbedded
build system builds each recipe in its own work directory (i.e.
**WORKDIR**). The path to the work directory is
constructed using the architecture of the given build (e.g.
**TUNE_PKGARCH**, **MACHINE_ARCH**, or "allarch"), the recipe
name, and the version of the recipe (i.e.
**PE**\ `:`\ **PV**\ `-`\ **PR**).

Here are key subdirectories within each recipe work directory:

-  `${WORKDIR}/temp`: Contains the log files of each task executed for
   this recipe, the "run" files for each executed task, which contain
   the code run, and a `log.task_order` file, which lists the order in
   which tasks were executed.

-  `${WORKDIR}/image`: Contains the output of the
   ref-tasks-install task, which corresponds to
   the `${`\ **D**\ `}` variable in that task.

-  `${WORKDIR}/pseudo`: Contains the pseudo database and log for any
   tasks executed under pseudo for the recipe.

-  `${WORKDIR}/sysroot-destdir`: Contains the output of the
   ref-tasks-populate_sysroot task.

-  `${WORKDIR}/package`: Contains the output of the
   ref-tasks-package task before the output is
   split into individual packages.

-  `${WORKDIR}/packages-split`: Contains the output of the
   ref-tasks-package task after the output has been split into individual
   packages. There are subdirectories for each individual package created by
   the recipe.

-  `${WORKDIR}/recipe-sysroot`: A directory populated with the target
   dependencies of the recipe. This directory looks like the target
   filesystem and contains libraries that the recipe might need to link
   against (e.g. the C library).

-  `${WORKDIR}/recipe-sysroot-native`: A directory populated with the
   native dependencies of the recipe. This directory contains the tools
   the recipe needs to build (e.g. the compiler, Autoconf, libtool, and
   so forth).

-  `${WORKDIR}/build`: This subdirectory applies only to recipes that
   support builds where the source is separate from the build artifacts.
   The OpenEmbedded build system uses this directory as a separate build
   directory (i.e. `${`\ **B**\ `}`).

#### ``build/tmp/work-shared/``

For efficiency, the OpenEmbedded build system creates and uses this
directory to hold recipes that share a work directory with other
recipes. This is for example used for `gcc` and its variants (e.g.
`gcc-cross`, `libgcc`, `gcc-runtime`, and so forth), or by the
ref-classes-kernel class to make the kernel source code and kernel build
artifacts available to out-of-tree kernel modules or other kernel-dependent
recipes.

In practice, only a few recipes make use of the `work-shared` directory. This
directory is especially useful for recipes that would induce a lot of storage
space if they were to be shared with the standard **Sysroot** mechanism.

## The OpenEmbedded-Core (OE-Core) Metadata --- ``meta/``

As mentioned previously, the **OpenEmbedded-Core (OE-Core)**
**metadata** is the core of the Yocto Project. This metadata has several
important subdivisions:

### ``meta/classes*/``

These directories contain the `*.bbclass` files. Class files are used to
abstract common code so it can be reused by multiple packages. Every
package inherits the ref-classes-base file. Examples of other important
classes are ref-classes-autotools, which in theory allows any
Autotool-enabled package to work with the Yocto Project with minimal
effort. Another example is ref-classes-kernel that contains common code
and functions for working with the Linux kernel. Functions like image
generation or packaging also have their specific class files such as
ref-classes-image, ref-classes-rootfs* and
package*.bbclass .

For reference information on classes, see the
"/ref-manual/classes" chapter.

### ``meta/conf/``

This directory contains the core set of configuration files that start
from `bitbake.conf` and from which all other configuration files are
included. See the include statements at the end of the `bitbake.conf`
file and you will note that even `local.conf` is loaded from there.
While `bitbake.conf` sets up the defaults, you can often override
these by using the (`local.conf`) file, machine file or the
distribution configuration file.

#### ``meta/conf/machine/``

This directory contains all the machine configuration files. If you set
`MACHINE = "qemux86"`, the OpenEmbedded build system looks for a
`qemux86.conf` file in this directory. The `include` directory
contains various data common to multiple machines. If you want to add
support for a new machine to the Yocto Project, look in this directory.

#### ``meta/conf/distro/``

The contents of this directory controls any distribution-specific
configurations. For the Yocto Project, the `defaultsetup.conf` is the
main file here. This directory includes the versions and the **SRCDATE**
definitions for applications that are configured here. An example of an
alternative configuration might be `poky-bleeding.conf`. Although this
file mainly inherits its configuration from Poky.

#### ``meta/conf/machine-sdk/``

The OpenEmbedded build system searches this directory for configuration
files that correspond to the value of
**SDKMACHINE**. By default, 32-bit and 64-bit x86
files ship with the Yocto Project that support some SDK hosts. However,
it is possible to extend that support to other SDK hosts by adding
additional configuration files in this subdirectory within another
layer.

### ``meta/files/``

This directory contains common license files and several text files used
by the build system. The text files contain minimal device information
and lists of files and directories with known permissions.

### ``meta/lib/``

This directory contains OpenEmbedded Python library code used during the
build process. It is enabled via the `addpylib` directive in
`meta/conf/local.conf`. For more information, see
bitbake-user-manual/bitbake-user-manual-metadata:extending python library code.

### ``meta/recipes-bsp/``

This directory contains anything linking to specific hardware or
hardware configuration information such as "u-boot" and "grub".

### ``meta/recipes-connectivity/``

This directory contains libraries and applications related to
communication with other devices.

### ``meta/recipes-core/``

This directory contains what is needed to build a basic working Linux
image including commonly used dependencies.

### ``meta/recipes-devtools/``

This directory contains tools that are primarily used by the build
system. The tools, however, can also be used on targets.

### ``meta/recipes-extended/``

This directory contains non-essential applications that add features
compared to the alternatives in core. You might need this directory for
full tool functionality.

### ``meta/recipes-gnome/``

This directory contains all things related to the GTK+ application
framework.

### ``meta/recipes-graphics/``

This directory contains X and other graphically related system
libraries.

### ``meta/recipes-kernel/``

This directory contains the kernel and generic applications and
libraries that have strong kernel dependencies.

### ``meta/recipes-multimedia/``

This directory contains codecs and support utilities for audio, images
and video.

### ``meta/recipes-rt/``

This directory contains package and image recipes for using and testing
the `PREEMPT_RT` kernel.

### ``meta/recipes-sato/``

This directory contains the Sato demo/reference UI/UX and its associated
applications and configuration data.

### ``meta/recipes-support/``

This directory contains recipes used by other recipes, but that are not
directly included in images (i.e. dependencies of other recipes).

### ``meta/site/``

This directory contains a list of cached results for various
architectures. Because certain "autoconf" test results cannot be
determined when cross-compiling due to the tests not able to run on a
live system, the information in this directory is passed to "autoconf"
for the various architectures.

### ``meta/recipes.txt``

This file is a description of the contents of `recipes-*`.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Classes

Class files are used to abstract common functionality and share it
amongst multiple recipe (`.bb`) files. To use a class file, you simply
make sure the recipe inherits the class. In most cases, when a recipe
inherits a class it is enough to enable its features. There are cases,
however, where in the recipe you might need to set variables or override
some default behavior. A class can also be inherited globally (in every recipe)
with the **INHERIT** variable.

Any **Metadata** usually found in a recipe can also be
placed in a class file. Class files are identified by the extension
`.bbclass` and are usually placed in one of the following subdirectories
of a **layer**:

  - `classes-recipe/` - classes intended to be inherited by recipes
    individually
  - `classes-global/` - classes intended to be inherited globally
  - `classes/` - classes whose usage context is not clearly defined

Class files can also be pointed to by
**BUILDDIR** (e.g. `build/`) in the same way as
`.conf` files in the `conf` directory. Class files are searched for
in **BBPATH** using the same method by which `.conf`
files are searched.

This chapter discusses only the most useful and important classes. Other
classes do exist within the `meta/classes*` directories of
**OpenEmbedded-Core (OE-Core)**. You can refer to the `.bbclass` files
directly for more information.

## ``allarch``

The ref-classes-allarch class is inherited by recipes that do not produce
architecture-specific output. The class disables functionality that is
normally needed for recipes that produce executable binaries (such as
building the cross-compiler and a C library as pre-requisites, and
splitting out of debug symbols during packaging).

> **Note:**
> Unlike some distro recipes (e.g. Debian), OpenEmbedded recipes that
> produce packages that depend on tunings through use of the
> **RDEPENDS** and
> **TUNE_PKGARCH** variables, should never be
> configured for all architectures using ref-classes-allarch. This is the case
> even if the recipes do not produce architecture-specific output.
>
> Configuring such recipes for all architectures causes the
> do_package_write_*  tasks to
> have different signatures for the machines with different tunings.
> Additionally, unnecessary rebuilds occur every time an image for a
> different **MACHINE** is built even when the recipe never changes.
>

By default, all recipes inherit the ref-classes-base and
ref-classes-package classes, which enable
functionality needed for recipes that produce executable output. If your
recipe, for example, only produces packages that contain configuration
files, media files, or scripts (e.g. Python and Perl), then it should
inherit the ref-classes-allarch class.

## ``archiver``

The ref-classes-archiver class supports releasing source code and other
materials with the binaries.

For more details on the source ref-classes-archiver, see the
"dev-manual/licenses:maintaining open source license compliance during your product's lifecycle"
section in the Yocto Project Development Tasks Manual. You can also see
the **ARCHIVER_MODE** variable for information
about the variable flags (varflags) that help control archive creation.

## ``autotools*``

The autotools*  classes support packages built with the
GNU Autotools .

The `autoconf`, `automake`, and `libtool` packages bring
standardization. This class defines a set of tasks (e.g. `configure`,
`compile` and so forth) that work for all Autotooled packages. It
should usually be enough to define a few standard variables and then
simply `inherit autotools`. These classes can also work with software
that emulates Autotools. For more information, see the
"dev-manual/new-recipe:building an autotooled package" section
in the Yocto Project Development Tasks Manual.

By default, the autotools*  classes use out-of-tree builds (i.e.
`autotools.bbclass` building with `B != S`).

If the software being built by a recipe does not support using
out-of-tree builds, you should have the recipe inherit the
autotools-brokensep  class. The autotools-brokensep  class behaves
the same as the ref-classes-autotools class but builds with **B**
== **S**. This method is useful when out-of-tree build
support is either not present or is broken.

> **Note:**
> It is recommended that out-of-tree support be fixed and used if at
> all possible.
>

It's useful to have some idea of how the tasks defined by the
autotools*  classes work and what they do behind the scenes.

-  ref-tasks-configure --- regenerates the
   configure script (using `autoreconf`) and then launches it with a
   standard set of arguments used during cross-compilation. You can pass
   additional parameters to `configure` through the **EXTRA_OECONF**
   or **PACKAGECONFIG_CONFARGS**
   variables.

-  ref-tasks-compile --- runs `make` with
   arguments that specify the compiler and linker. You can pass
   additional arguments through the **EXTRA_OEMAKE** variable.

-  ref-tasks-install --- runs `make install` and
   passes in `${`\ **D**\ `}` as `DESTDIR`.

## ``barebox``

The ref-classes-barebox class manages building the barebox bootloader.

If a file named `defconfig` is included in the **SRC_URI**, it will be
copied to `.config` in the build directory and used as the barebox
configuration.
Instead of providing a `defconfig` file, you can set **BAREBOX_CONFIG**
to a defconfig provided by the barebox source tree.
If neither `defconfig` nor **BAREBOX_CONFIG** is specified, the class
will raise an error.

The ref-classes-barebox class supports config fragments and internally
includes the ref-classes-cml1 class to provide `Kconfig
<https://docs.kernel.org/kbuild/kconfig-language.html>`__ support for
barebox, enabling tasks such as ref-tasks-menuconfig and
ref-tasks-diffconfig.

The generated barebox binaries are deployed to
**DEPLOY_DIR_IMAGE** as well as installed to `BAREBOX_INSTALL_PATH`
(`/boot` by default) making them part of the recipe’s base package.
This setup supports both using the barebox binaries as independent artifacts
and installing them into a rootfs.
**BAREBOX_BINARY** can be used to select a distinct binary to deploy and
install.
If `barebox` is set as the **EFI_PROVIDER**, the class will leverage
conf/image-uefi.conf 
to define the default installation paths and naming conventions.

The compiled-in barebox environment can be extended by adding environment files
to the `BAREBOX_ENV_DIR`.
The `BAREBOX_FIRMWARE_DIR` variable allows you to specify the firmware blob
search directory, enabling loading of additional firmware like TF-A or OP-TEE.

## ``base``

The ref-classes-base class is special in that every `.bb` file implicitly
inherits the class. This class contains definitions for standard basic
tasks such as fetching, unpacking, configuring (empty by default),
compiling (runs any `Makefile` present), installing (empty by default)
and packaging (empty by default). These tasks are often overridden or
extended by other classes such as the ref-classes-autotools class or the
ref-classes-package class.

The class also contains some commonly used functions such as
`oe_runmake`, which runs `make` with the arguments specified in
**EXTRA_OEMAKE** variable as well as the
arguments passed directly to `oe_runmake`.

## ``bash-completion``

Sets up packaging and dependencies appropriate for recipes that build
software that includes bash-completion data.

## ``bin_package``

The ref-classes-bin-package class is a helper class for recipes, that
disables the ref-tasks-configure and ref-tasks-compile tasks and
copies the content of the **S** directory into the **D** directory. This
is useful for installing binary packages (e.g. RPM packages) by passing the
package in the **SRC_URI** variable and inheriting this class.

For RPMs and other packages that do not contain a subdirectory, you should set
the **SRC_URI** option `subdir` to **BP** so that the contents are
extracted to the directory expected by the default value of **S**. For
example::

   SRC_URI = "https://example.com/downloads/somepackage.rpm;subdir=${BP}"

This class can also be used for tarballs. For example::

   SRC_URI = "file://somepackage.tar.xz;subdir=${BP}"

The ref-classes-bin-package class will copy the extracted content of the
tarball from **S** to **D**.

This class assumes that the content of the package as installed in **S**
mirrors the expected layout once installed on the target, which is generally the
case for binary packages. For example, an RPM package for a library would
usually contain the `usr/lib` directory, and should be extracted to
`${S}/usr/lib/<library>.so.<version>` to be installed in **D** correctly.

> **Note:**
> The extraction of the package passed in **SRC_URI** is not handled by the
> ref-classes-bin-package class, but rather by the appropriate
> fetcher 
> depending on the file extension.
>

## ``binconfig``

The ref-classes-binconfig class helps to correct paths in shell scripts.

Before `pkg-config` had become widespread, libraries shipped shell
scripts to give information about the libraries and include paths needed
to build software (usually named `LIBNAME-config`). This class assists
any recipe using such scripts.

During staging, the OpenEmbedded build system installs such scripts into
the `sysroots/` directory. Inheriting this class results in all paths
in these scripts being changed to point into the `sysroots/` directory
so that all builds that use the script use the correct directories for
the cross compiling layout. See the
**BINCONFIG_GLOB** variable for more
information.

## ``binconfig-disabled``

An alternative version of the ref-classes-binconfig
class, which disables binary configuration scripts by making them return
an error in favor of using `pkg-config` to query the information. The
scripts to be disabled should be specified using the **BINCONFIG**
variable within the recipe inheriting the class.

## ``buildhistory``

The ref-classes-buildhistory class records a history of build output metadata,
which can be used to detect possible regressions as well as used for
analysis of the build output. For more information on using Build
History, see the
"dev-manual/build-quality:maintaining build output quality"
section in the Yocto Project Development Tasks Manual.

## ``buildstats``

The ref-classes-buildstats class records performance statistics about each task
executed during the build (e.g. elapsed time, CPU usage, and I/O usage).

When you use this class, the output goes into the
**BUILDSTATS_BASE** directory, which defaults
to `${TMPDIR}/buildstats/`. You can analyze the elapsed time using
`scripts/pybootchartgui/pybootchartgui.py`, which produces a cascading
chart of the entire build process and can be useful for highlighting
bottlenecks.

Collecting build statistics is enabled by default through the
**USER_CLASSES** variable from your
`local.conf` file. Consequently, you do not have to do anything to
enable the class. However, if you want to disable the class, simply
remove "ref-classes-buildstats" from the **USER_CLASSES** list.

## ``buildstats-summary``

When inherited globally, prints statistics at the end of the build on
sstate re-use. In order to function, this class requires the
ref-classes-buildstats class be enabled.

## ``cargo``

The ref-classes-cargo class allows to compile Rust language programs
using `Cargo <https://doc.rust-lang.org/cargo/>`__. Cargo is Rust's package
manager, allowing to fetch package dependencies and build your program.

Using this class makes it very easy to build Rust programs. All you need
is to use the **SRC_URI** variable to point to a source repository
which can be built by Cargo, typically one that was created by the
`cargo new` command, containing a `Cargo.toml` file, a `Cargo.lock` file and a `src`
subdirectory.

If you want to build and package tests of the program, inherit the
ref-classes-ptest-cargo class instead of ref-classes-cargo.

You will find an example (that show also how to handle possible git source dependencies) in the
zvariant_3.12.0.bb 
recipe. Another example, with only crate dependencies, is the
uutils-coreutils 
recipe, which was generated by the `cargo-bitbake <https://crates.io/crates/cargo-bitbake>`__
tool.

This class inherits the ref-classes-cargo_common class.

## ``cargo_c``

The ref-classes-cargo_c class can be inherited by a recipe to generate
a Rust library that can be called by C/C++ code. The recipe which inherits this
class has to only replace `inherit cargo` by `inherit cargo_c`.

See the :oe_git:`rust-c-lib-example_git.bb
</openembedded-core/tree/meta-selftest/recipes-devtools/rust/rust-c-lib-example_git.bb>`
example recipe.

## ``cargo_common``

The ref-classes-cargo_common class is an internal class
that is not intended to be used directly.

An exception is the "rust" recipe, to build the Rust compiler and runtime
library, which is built by Cargo but cannot use the ref-classes-cargo
class. This is why this class was introduced.

## ``cargo-update-recipe-crates``

The ref-classes-cargo-update-recipe-crates class allows
recipe developers to update the list of Cargo crates in **SRC_URI**
by reading the `Cargo.lock` file in the source tree.

To do so, create a recipe for your program, for example using
devtool ,
make it inherit the ref-classes-cargo and
ref-classes-cargo-update-recipe-crates and run::

   bitbake -c update_crates recipe

This creates a `recipe-crates.inc` file that you can include in your
recipe::

   require ${BPN}-crates.inc

That's also something you can achieve by using the
`cargo-bitbake <https://crates.io/crates/cargo-bitbake>`__ tool.

## ``ccache``

The ref-classes-ccache class enables the C/C++ Compiler Cache for the build.
This class is used to give a minor performance boost during the build.

See https://ccache.samba.org/ for information on the C/C++ Compiler
Cache, and the ccache.bbclass 
file for details about how to enable this mechanism in your configuration
file, how to disable it for specific recipes, and how to share `ccache`
files between builds.

Recipes (including ref-classes-native ones) can make use of the host's
`ccache` binary (via **HOSTTOOLS**) if the following configuration
statements are provided in a **configuration file**::

   ASSUME_PROVIDED += "ccache-native"
   HOSTTOOLS += "ccache"

Recipes can also explicitly disable `Ccache` support even when the
ref-classes-ccache class is enabled, by setting the
**CCACHE_DISABLE** variable to "1".

Using the ref-classes-ccache class can lead to unexpected side-effects.
Using this class is not recommended.

## ``chrpath``

The ref-classes-chrpath class is a wrapper around the "chrpath" utility, which
is used during the build process for ref-classes-nativesdk, ref-classes-cross, and
ref-classes-cross-canadian recipes to change `RPATH` records within binaries
in order to make them relocatable.

## ``cmake``

The ref-classes-cmake class allows recipes to build software using the
`CMake <https://cmake.org/overview/>`__ build system. You can use the
**EXTRA_OECMAKE** variable to specify additional configuration options to
pass to the `cmake` command line.

By default, the ref-classes-cmake class uses
`Ninja <https://ninja-build.org/>`__ instead of GNU make for building, which
offers better build performance. If a recipe is broken with Ninja, then the
recipe can set the **OECMAKE_GENERATOR** variable to `Unix Makefiles` to
use GNU make instead.

If you need to install custom CMake toolchain files supplied by the application
being built, you should install them (during ref-tasks-install) to the
preferred CMake Module directory: `${D}${datadir}/cmake/modules/`.

## ``cmake-qemu``

The ref-classes-cmake-qemu class might be used instead of the
ref-classes-cmake class. In addition to the features provided by the
ref-classes-cmake class, the ref-classes-cmake-qemu class passes
the `CMAKE_CROSSCOMPILING_EMULATOR` setting to `cmake`. This allows to use
QEMU user-mode emulation for the execution of cross-compiled binaries on the
host machine.  For more information about `CMAKE_CROSSCOMPILING_EMULATOR`
please refer to the `related section of the CMake documentation
<https://cmake.org/cmake/help/latest/variable/CMAKE_CROSSCOMPILING_EMULATOR.html>`__.

Not all platforms are supported by QEMU. This class only works for machines with
`qemu-usermode` in the ref-features-machine. Using QEMU user-mode therefore
involves a certain risk, which is also the reason why this feature is not part of
the main ref-classes-cmake class by default.

One use case is the execution of cross-compiled unit tests with CTest on the build
machine. If `CMAKE_CROSSCOMPILING_EMULATOR` is configured::

   cmake --build --target test

works transparently with QEMU user-mode.

If the CMake project is developed with this use case in mind this works very nicely.
This also applies to an IDE configured to use `cmake-native` for cross-compiling.

## ``cml1``

The ref-classes-cml1 class provides basic support for the Linux kernel style
build configuration system. "cml" stands for "Configuration Menu Language", which
originates from the Linux kernel but is also used in other projects such as U-Boot
and BusyBox. It could have been called "kconfig" too.

## ``compress_doc``

Enables compression for manual and info pages. This class is intended
to be inherited globally. The default compression mechanism is gz (gzip)
but you can select an alternative mechanism by setting the
**DOC_COMPRESS** variable.

## ``copyleft_compliance``

The ref-classes-copyleft_compliance class preserves source code for the purposes
of license compliance. This class is an alternative to the ref-classes-archiver
class and is still used by some users even though it has been deprecated
in favor of the ref-classes-archiver class.

## ``copyleft_filter``

A class used by the ref-classes-archiver and
ref-classes-copyleft_compliance classes
for filtering licenses. The `copyleft_filter` class is an internal
class and is not intended to be used directly.

## ``core-image``

The ref-classes-core-image class provides common definitions for the
`core-image-*` image recipes, such as support for additional
**IMAGE_FEATURES**.

## ``cpan*``

The cpan*  classes support Perl modules.

Recipes for Perl modules are simple. These recipes usually only need to
point to the source's archive and then inherit the proper class file.
Building is split into two methods depending on which method the module
authors used.

-  Modules that use old `Makefile.PL`-based build system require
   `cpan.bbclass` in their recipes.

-  Modules that use `Build.PL`-based build system require using
   `cpan_build.bbclass` in their recipes.

Both build methods inherit the cpan-base  class for basic Perl
support.

## ``create-spdx``

The ref-classes-create-spdx class provides support for
automatically creating **SPDX** **SBOM** documents based upon image
and SDK contents.

This class is meant to be inherited globally from a configuration file::

   INHERIT += "create-spdx"

The toplevel **SPDX** output file is generated in JSON format as a
`IMAGE-MACHINE.spdx.json` file in `tmp/deploy/images/MACHINE/` inside the
**Build Directory**. There are other related files in the same directory,
as well as in `tmp/deploy/spdx`.

The exact behaviour of this class, and the amount of output can be controlled
by the **SPDX_PRETTY**, **SPDX_ARCHIVE_PACKAGED**,
**SPDX_ARCHIVE_SOURCES** and **SPDX_INCLUDE_SOURCES** variables.

See the description of these variables and the
"dev-manual/sbom:creating a software bill of materials"
section in the Yocto Project Development Manual for more details.

## ``cross``

The ref-classes-cross class provides support for the recipes that build the
cross-compilation tools.

## ``cross-canadian``

The ref-classes-cross-canadian class provides support for the recipes that build
the Canadian Cross-compilation tools for SDKs. See the
"overview-manual/concepts:cross-development toolchain generation"
section in the Yocto Project Overview and Concepts Manual for more
discussion on these cross-compilation tools.

## ``crosssdk``

The ref-classes-crosssdk class provides support for the recipes that build the
cross-compilation tools used for building SDKs. See the
"overview-manual/concepts:cross-development toolchain generation"
section in the Yocto Project Overview and Concepts Manual for more
discussion on these cross-compilation tools.

## ``cve-check``

The ref-classes-cve-check class looks for known CVEs (Common Vulnerabilities
and Exposures) while building with BitBake. This class is meant to be
inherited globally from a configuration file::

   INHERIT += "cve-check"

To filter out obsolete CVE database entries which are known not to impact
software from **OpenEmbedded-Core (OE-Core)**, add the following line to the
build configuration file::

   include cve-extra-exclusions.inc

You can also look for vulnerabilities in specific packages by passing
`-c cve_check` to BitBake.

After building the software with Bitbake, CVE check output reports are available in `tmp/deploy/cve`
and image specific summaries in `tmp/deploy/images/*.json` files.

When building, the CVE checker will emit build time warnings for any detected
issues which are in the state `Unpatched`, meaning that CVE issue seems to affect the software component
and version being compiled and no patches to address the issue are applied. Other states
for detected CVE issues are: `Patched` meaning that a patch to address the issue is already
applied, and `Ignored` meaning that the issue can be ignored.

The `Patched` state of a CVE issue is detected from patch files with the format
`CVE-ID.patch`, e.g. `CVE-2019-20633.patch`, in the **SRC_URI** and using
CVE metadata of format `CVE: CVE-ID` in the commit message of the patch file.

> **Note:**
> Commit message metadata (`CVE: CVE-ID` in a patch header) will not be scanned
> in any patches that are remote, i.e. that are anything other than local files
> referenced via `file://` in SRC_URI. However, a `CVE-ID` in a remote patch
> file name itself will be registered.
>

If the recipe adds `CVE-ID` as flag of the **CVE_STATUS** variable with status
mapped to `Ignored`, then the CVE state is reported as `Ignored`::

   CVE_STATUS[CVE-2020-15523] = "not-applicable-platform: Issue only applies on Windows"

If CVE check reports that a recipe contains false positives or false negatives, these may be
fixed in recipes by adjusting the CVE product name using **CVE_PRODUCT** and **CVE_VERSION** variables.
**CVE_PRODUCT** defaults to the plain recipe name **BPN** which can be adjusted to one or more CVE
database vendor and product pairs using the syntax::

   CVE_PRODUCT = "flex_project:flex"

where `flex_project` is the CVE database vendor name and `flex` is the product name. Similarly
if the default recipe version **PV** does not match the version numbers of the software component
in upstream releases or the CVE database, then the **CVE_VERSION** variable can be used to set the
CVE database compatible version number, for example::

   CVE_VERSION = "2.39"

Any bugs or missing or incomplete information in the CVE database entries should be fixed in the CVE database
via the `NVD feedback form <https://nvd.nist.gov/info/contact-form>`__.

Users should note that security is a process, not a product, and thus also CVE checking, analyzing results,
patching and updating the software should be done as a regular process. The data and assumptions
required for CVE checker to reliably detect issues are frequently broken in various ways.
These can only be detected by reviewing the details of the issues and iterating over the generated reports,
and following what happens in other Linux distributions and in the greater open source community.

You will find some more details in the
"security-manual/vulnerabilities:checking for vulnerabilities"
section in the Development Tasks Manual.

## ``cython``

The ref-classes-cython class can be used by Python recipes that require
`Cython <https://cython.org/>`__ as part of their build dependencies
(**DEPENDS**).

## ``debian``

The ref-classes-debian class renames output packages so that they follow the
Debian naming policy (i.e. `glibc` becomes `libc6` and
`glibc-devel` becomes `libc6-dev`.) Renaming includes the library
name and version as part of the package name.

If a recipe creates packages for multiple libraries (shared object files
of `.so` type), use the **LEAD_SONAME**
variable in the recipe to specify the library on which to apply the
naming scheme.

## ``deploy``

The ref-classes-deploy class handles deploying files to the
**DEPLOY_DIR_IMAGE** directory. The main
function of this class is to allow the deploy step to be accelerated by
shared state. Recipes that inherit this class should define their own
ref-tasks-deploy function to copy the files to be
deployed to **DEPLOYDIR**, and use `addtask` to
add the task at the appropriate place, which is usually after
ref-tasks-compile or
ref-tasks-install. The class then takes care of
staging the files from **DEPLOYDIR** to **DEPLOY_DIR_IMAGE**.

## ``devicetree``

The ref-classes-devicetree class allows to build a recipe that compiles
device tree source files that are not in the kernel tree.

The compilation of out-of-tree device tree sources is the same as the kernel
in-tree device tree compilation process. This includes the ability to include
sources from the kernel such as SoC `dtsi` files as well as C header files,
such as `gpio.h`.

The ref-tasks-compile task will compile two kinds of files:

- Regular device tree sources with a `.dts` extension.

- Device tree overlays, detected from the presence of the `/plugin/;`
  string in the file contents.

This class deploys the generated device tree binaries into
`${`\ **DEPLOY_DIR_IMAGE**\ `}/devicetree/`.  This is similar to
what the ref-classes-kernel-devicetree class does, with the added
`devicetree` subdirectory to avoid name clashes. Additionally, the device
trees are populated into the sysroot for access via the sysroot from within
other recipes.

By default, all device tree sources located in **DT_FILES_PATH** directory
are compiled. To select only particular sources, set **DT_FILES** to
a space-separated list of files (relative to **DT_FILES_PATH**). For
convenience, both `.dts` and `.dtb` extensions can be used.

An extra padding is appended to non-overlay device trees binaries. This
can typically be used as extra space for adding extra properties at boot time.
The padding size can be modified by setting **DT_PADDING_SIZE**
to the desired size, in bytes.

See :oe_git:`devicetree.bbclass sources
</openembedded-core/tree/meta/classes-recipe/devicetree.bbclass>`
for further variables controlling this class.

Here is an excerpt of an example `recipes-kernel/linux/devicetree-acme.bb`
recipe inheriting this class::

   inherit devicetree
   COMPATIBLE_MACHINE = "^mymachine$"
   SRC_URI:mymachine = "file://mymachine.dts"

## ``devshell``

The ref-classes-devshell class adds the ref-tasks-devshell task. Distribution
policy dictates whether to include this class. See the "dev-manual/development-shell:using a development shell"
section in the Yocto Project Development Tasks Manual for more
information about using ref-classes-devshell.

## ``devupstream``

The ref-classes-devupstream class uses
**BBCLASSEXTEND** to add a variant of the
recipe that fetches from an alternative URI (e.g. Git) instead of a
tarball. Here is an example::

   BBCLASSEXTEND = "devupstream:target"
   SRC_URI:class-devupstream = "git://git.example.com/example;branch=main"
   SRCREV:class-devupstream = "abcd1234"

Adding the above statements to your recipe creates a variant that has
**DEFAULT_PREFERENCE** set to "-1".
Consequently, you need to select the variant of the recipe to use it.
Any development-specific adjustments can be done by using the
`class-devupstream` override. Here is an example::

   DEPENDS:append:class-devupstream = " gperf-native"
   do_configure:prepend:class-devupstream() {
       touch ${S}/README
   }

The class
currently only supports creating a development variant of the target
recipe, not ref-classes-native or ref-classes-nativesdk variants.

The **BBCLASSEXTEND** syntax (i.e. `devupstream:target`) provides
support for ref-classes-native and ref-classes-nativesdk variants. Consequently, this
functionality can be added in a future release.

Support for other version control systems such as Subversion is limited
due to BitBake's automatic fetch dependencies (e.g.
`subversion-native`).

## ``externalsrc``

The ref-classes-externalsrc class supports building software from source code
that is external to the OpenEmbedded build system. Building software
from an external source tree means that the build system's normal fetch,
unpack, and patch process is not used.

By default, the OpenEmbedded build system uses the **S**
and **B** variables to locate unpacked recipe source code
and to build it, respectively. When your recipe inherits the
ref-classes-externalsrc class, you use the
**EXTERNALSRC** and **EXTERNALSRC_BUILD** variables to
ultimately define **S** and **B**.

By default, this class expects the source code to support recipe builds
that use the **B** variable to point to the directory in
which the OpenEmbedded build system places the generated objects built
from the recipes. By default, the **B** directory is set to the
following, which is separate from the source directory (**S**)::

   ${WORKDIR}/${BPN}-{PV}/

See these variables for more information:
**WORKDIR**, **BPN**, and
**PV**,

For more information on the ref-classes-externalsrc class, see the comments in
`meta/classes/externalsrc.bbclass` in **OpenEmbedded-Core (OE-Core)**.
For information on how to use the ref-classes-externalsrc class, see the
"dev-manual/building:building software from an external source"
section in the Yocto Project Development Tasks Manual.

## ``extrausers``

The ref-classes-extrausers class allows additional user and group configuration
to be applied at the image level. Inheriting this class either globally
or from an image recipe allows additional user and group operations to
be performed using the
**EXTRA_USERS_PARAMS** variable.

> **Note:**
> The user and group operations added using the ref-classes-extrausers
> class are not tied to a specific recipe outside of the recipe for the
> image. Thus, the operations can be performed across the image as a
> whole. Use the ref-classes-useradd class to add user and group
> configuration to a specific recipe.
>

Here is an example that uses this class in an image recipe::

   inherit extrausers
   EXTRA_USERS_PARAMS = "\
       useradd -p '' tester; \
       groupadd developers; \
       userdel nobody; \
       groupdel -g video; \
       groupmod -g 1020 developers; \
       usermod -s /bin/sh tester; \
       "

Here is an example that adds two users named "tester-jim" and "tester-sue" and assigns
passwords. First on host, create the (escaped) password hash::

   printf "%q" $(mkpasswd -m sha256crypt tester01)

The resulting hash is set to a variable and used in `useradd` command parameters::

   inherit extrausers
   PASSWD = "\$X\$ABC123\$A-Long-Hash"
   EXTRA_USERS_PARAMS = "\
       useradd -p '${PASSWD}' tester-jim; \
       useradd -p '${PASSWD}' tester-sue; \
       "

Finally, here is an example that sets the root password::

   inherit extrausers
   EXTRA_USERS_PARAMS = "\
       usermod -p '${PASSWD}' root; \
       "

> **Note:**
> From a security perspective, hardcoding a default password is not
> generally a good idea or even legal in some jurisdictions. It is
> recommended that you do not do this if you are building a production
> image.
>
>

## ``features_check``

The ref-classes-features_check class allows individual recipes to check
for required and conflicting **DISTRO_FEATURES**, **MACHINE_FEATURES**
or **COMBINED_FEATURES**.

This class provides support for the following variables:

- **REQUIRED_DISTRO_FEATURES**
- **CONFLICT_DISTRO_FEATURES**
- **ANY_OF_DISTRO_FEATURES**
- `REQUIRED_MACHINE_FEATURES`
- `CONFLICT_MACHINE_FEATURES`
- `ANY_OF_MACHINE_FEATURES`
- `REQUIRED_COMBINED_FEATURES`
- `CONFLICT_COMBINED_FEATURES`
- `ANY_OF_COMBINED_FEATURES`

If any conditions specified in the recipe using the above
variables are not met, the recipe will be skipped, and if the
build system attempts to build the recipe then an error will be
triggered.

## ``fontcache``

The ref-classes-fontcache class generates the proper post-install and
post-remove (postinst and postrm) scriptlets for font packages. These
scriptlets call `fc-cache` (part of `Fontconfig`) to add the fonts
to the font information cache. Since the cache files are
architecture-specific, `fc-cache` runs using QEMU if the postinst
scriptlets need to be run on the build host during image creation.

If the fonts being installed are in packages other than the main
package, set **FONT_PACKAGES** to specify the
packages containing the fonts.

## ``fs-uuid``

The ref-classes-fs-uuid class extracts UUID from
`${`\ **ROOTFS**\ `}`, which must have been built
by the time that this function gets called. The ref-classes-fs-uuid class only
works on `ext` file systems and depends on `tune2fs`.

## ``gconf``

The ref-classes-gconf class provides common functionality for recipes that need
to install GConf schemas. The schemas will be put into a separate
package (`${`\ **PN**\ `}-gconf`) that is created
automatically when this class is inherited. This package uses the
appropriate post-install and post-remove (postinst/postrm) scriptlets to
register and unregister the schemas in the target image.

## ``gettext``

The ref-classes-gettext class provides support for building
software that uses the GNU `gettext` internationalization and localization
system. All recipes building software that use `gettext` should inherit this
class.

This class will configure recipes to build translations *unless*:

-  the **USE_NLS** variable is set to `no`, or

-  the **INHIBIT_DEFAULT_DEPS** variable is set and the recipe inheriting
   the ref-classes-gettext class does not also inherit the
   ref-classes-cross-canadian class.

## ``github-releases``

For recipes that fetch release tarballs from github, the ref-classes-github-releases
class sets up a standard way for checking available upstream versions
(to support `devtool upgrade` and the Automated Upgrade Helper (AUH)).

To use it, add "ref-classes-github-releases" to the inherit line in the recipe,
and if the default value of **GITHUB_BASE_URI** is not suitable,
then set your own value in the recipe. You should then use `${GITHUB_BASE_URI}`
in the value you set for **SRC_URI** within the recipe.

## ``gnomebase``

The ref-classes-gnomebase class is the base class for recipes that build
software from the GNOME stack. This class sets
**SRC_URI** to download the source from the GNOME
mirrors as well as extending **FILES** with the typical
GNOME installation paths.

## ``go``

The ref-classes-go class supports building Go programs. The behavior of
this class is controlled by the mandatory **GO_IMPORT** variable, and
by the optional **GO_INSTALL** and **GO_INSTALL_FILTEROUT** ones.

To build a Go program with the Yocto Project, you can use the
go-helloworld_0.1.bb 
recipe as an example.

## ``go-mod``

The ref-classes-go-mod class allows to use Go modules, and inherits the
ref-classes-go class.

See the associated **GO_WORKDIR** variable.

## ``go-mod-update-modules``

The ref-classes-go-mod-update-modules class can be used in Go recipes and
defines a `do_update_modules` task that can be run manually to update two
files ("BPN" below corresponds to **BPN**):

-  `BPN-go-mods.inc`: list of Go modules the recipe depends on.
-  `BPN-licenses.inc`: list of licenses for each Go modules the recipe depends
   on.

These files can then updated automatically with the `do_update_modules` task.

## ``go-vendor``

The ref-classes-go-vendor class implements support for offline builds,
also known as Go vendoring. In such a scenario, the module dependencias are
downloaded during the ref-tasks-fetch task rather than when modules are
imported, thus being coherent with Yocto's concept of fetching every source
beforehand.

The dependencies are unpacked into the modules' `vendor` directory, where a
manifest file is generated.

## ``gobject-introspection``

Provides support for recipes building software that supports GObject
introspection. This functionality is only enabled if the
"gobject-introspection-data" feature is in
**DISTRO_FEATURES** as well as
"qemu-usermode" being in
**MACHINE_FEATURES**.

> **Note:**
> This functionality is backfilled  by default
> and, if not applicable, should be disabled through
> **DISTRO_FEATURES_BACKFILL_CONSIDERED** or
> **MACHINE_FEATURES_BACKFILL_CONSIDERED**, respectively.
>

## ``grub-efi``

The ref-classes-grub-efi class provides `grub-efi`-specific functions for
building bootable images.

This class supports several variables:

-  **INITRD**: Indicates list of filesystem images to
   concatenate and use as an initial RAM disk (initrd) (optional).
   Can be specified for each `LABEL`.

-  **ROOTFS**: Indicates a filesystem image to include
   as the root filesystem (optional).

-  **GRUB_GFXSERIAL**: Set this to "1" to have
   graphics and serial in the boot menu.

-  **LABELS**: A list of targets for the automatic
   configuration.

-  **APPEND**: An override list of append strings for
   each `LABEL`.

-  **GRUB_TITLE**: A custom title for each `LABEL`. If a label does not
   have a custom title, the label is used as title for the GRUB menu entry.

-  **GRUB_OPTS**: Additional options to add to the
   configuration (optional). Options are delimited using semi-colon
   characters (`;`).

-  **GRUB_TIMEOUT**: Timeout before executing
   the default `LABEL` (optional).

Each `LABEL` defined in the **LABELS** variable creates a GRUB boot
entry, and some variables can be defined individually per `LABEL`. The label
specific override names are defined as `grub_LABEL`.

For example, for a label `factory`, the override name would be
`grub_factory`. A custom GRUB menu entry titled "Factory Install" with the
additional parameter `factory=yes` can be achieved as follows::

   LABELS:append = " factory"
   APPEND:grub_factory = "factory=yes"
   GRUB_TITLE:grub_factory = "Factory Install"

## ``gsettings``

The ref-classes-gsettings class provides common functionality for recipes that
need to install GSettings (glib) schemas. The schemas are assumed to be
part of the main package. Appropriate post-install and post-remove
(postinst/postrm) scriptlets are added to register and unregister the
schemas in the target image.

## ``gtk-doc``

The ref-classes-gtk-doc class is a helper class to pull in the appropriate
`gtk-doc` dependencies and disable `gtk-doc`.

## ``gtk-icon-cache``

The ref-classes-gtk-icon-cache class generates the proper post-install and
post-remove (postinst/postrm) scriptlets for packages that use GTK+ and
install icons. These scriptlets call `gtk-update-icon-cache` to add
the fonts to GTK+'s icon cache. Since the cache files are
architecture-specific, `gtk-update-icon-cache` is run using QEMU if
the postinst scriptlets need to be run on the build host during image
creation.

## ``gtk-immodules-cache``

The ref-classes-gtk-immodules-cache class generates the proper post-install and
post-remove (postinst/postrm) scriptlets for packages that install GTK+
input method modules for virtual keyboards. These scriptlets call
`gtk-update-icon-cache` to add the input method modules to the cache.
Since the cache files are architecture-specific,
`gtk-update-icon-cache` is run using QEMU if the postinst scriptlets
need to be run on the build host during image creation.

If the input method modules being installed are in packages other than
the main package, set
**GTKIMMODULES_PACKAGES** to specify
the packages containing the modules.

## ``gzipnative``

The ref-classes-gzipnative class enables the use of different native versions of
`gzip` and `pigz` rather than the versions of these tools from the
build host.

## ``image``

The ref-classes-image class helps support creating images in different formats.
First, the root filesystem is created from packages using one of the
`rootfs*.bbclass` files (depending on the package format used) and
then one or more image files are created.

-  The **IMAGE_FSTYPES** variable controls the types of images to
   generate.

-  The **IMAGE_INSTALL** variable controls the list of packages to
   install into the image.

For information on customizing images, see the
"dev-manual/customizing-images:customizing images" section
in the Yocto Project Development Tasks Manual. For information on how
images are created, see the
"overview-manual/concepts:images" section in the
Yocto Project Overview and Concepts Manual.

## ``image-buildinfo``

The ref-classes-image-buildinfo class writes a plain text file containing
build information to the target filesystem at `${sysconfdir}/buildinfo`
by default (as specified by **IMAGE_BUILDINFO_FILE**).
This can be useful for manually determining the origin of any given
image. It writes out two sections:

#. `Build Configuration`: a list of variables and their values (specified
   by **IMAGE_BUILDINFO_VARS**, which defaults to **DISTRO** and
   **DISTRO_VERSION**)

#. `Layer Revisions`: the revisions of all of the layers used in the
   build.

Additionally, when building an SDK it will write the same contents
to `/buildinfo` by default (as specified by
**SDK_BUILDINFO_FILE**).

## ``image_types``

The ref-classes-image_types class defines all of the standard image output types
that you can enable through the
**IMAGE_FSTYPES** variable. You can use this
class as a reference on how to add support for custom image output
types.

By default, the ref-classes-image class automatically
enables the ref-classes-image_types class. The ref-classes-image class uses the
`IMGCLASSES` variable as follows::

   IMGCLASSES = "rootfs_${IMAGE_PKGTYPE} image_types ${IMAGE_CLASSES}"
   # Only Linux SDKs support populate_sdk_ext, fall back to populate_sdk_base
   # in the non-Linux SDK_OS case, such as mingw32
   inherit populate_sdk_base
   IMGCLASSES += "${@['', 'populate_sdk_ext']['linux' in d.getVar("SDK_OS")]}"
   IMGCLASSES += "${@bb.utils.contains_any('IMAGE_FSTYPES', 'live iso hddimg', 'image-live', '', d)}"
   IMGCLASSES += "${@bb.utils.contains('IMAGE_FSTYPES', 'container', 'image-container', '', d)}"
   IMGCLASSES += "image_types_wic"
   IMGCLASSES += "rootfs-postcommands"
   IMGCLASSES += "image-postinst-intercepts"
   IMGCLASSES += "overlayfs-etc"
   inherit_defer ${IMGCLASSES}

The ref-classes-image_types class also handles conversion and compression of images.

> **Note:**
> To build a VMware VMDK image, you need to add "wic.vmdk" to
> **IMAGE_FSTYPES**. This would also be similar for Virtual Box Virtual Disk
> Image ("vdi") and QEMU Copy On Write Version 2 ("qcow2") images.
>

## ``image-container``

The ref-classes-image-container class is automatically inherited in
image  recipes that have the `container` image type
in **IMAGE_FSTYPES**. It provides relevant settings to generate an image
ready for use with an OCI -compliant
container management tool, such as Podman  or
Docker .

> **Note:**
> This class neither builds nor installs container management tools on the
> target. Those tools are available in the :yocto_git:`meta-virtualization
> </meta-virtualization>` layer.
>

You should set the **PREFERRED_PROVIDER** for the Linux kernel to
`linux-dummy` in a **configuration file**::

   PREFERRED_PROVIDER_virtual/kernel = "linux-dummy"

Otherwise an error is triggered. If desired, the
**IMAGE_CONTAINER_NO_DUMMY** variable can be set to "1" to avoid triggering
this error.

The `linux-dummy` recipe acts as a Linux kernel recipe but builds nothing. It
is relevant to use as the preferred Linux kernel provider in this case as a
container image does not need to include a Linux kernel. Selecting it as the
preferred provider for the kernel will also decrease build time.

Using this class only deploys an additional `tar.bz2` archive to
**DEPLOY_DIR_IMAGE**. This archive can be used in a container file (a file
typically named `Dockerfile` or `Containerfile`). For example, to be used with
Podman  or Docker , the
`container file <https://docs.docker.com/reference/dockerfile/>`__ could contain
the following instructions:

```dockerfile
FROM scratch
ADD ./image-container-qemux86-64.rootfs.tar.bz2 /
ENTRYPOINT /bin/sh
```

This is suitable to build a container using our generated root filesystem image.

## ``image-live``

This class controls building "live" (i.e. HDDIMG and ISO) images. Live
images contain syslinux for legacy booting, as well as the bootloader
specified by **EFI_PROVIDER** if
**MACHINE_FEATURES** contains "efi".

Normally, you do not use this class directly. Instead, you add "live" to
**IMAGE_FSTYPES**.

## ``insane``

The ref-classes-insane class adds a step to the package generation process so
that output quality assurance checks are generated by the OpenEmbedded
build system. A range of checks are performed that check the build's
output for common problems that show up during runtime. Distribution
policy usually dictates whether to include this class.

You can configure the sanity checks so that specific test failures
either raise a warning or an error message. Typically, failures for new
tests generate a warning. Subsequent failures for the same test would
then generate an error message once the metadata is in a known and good
condition. See the "/ref-manual/qa-checks" Chapter for a list of all the warning
and error messages you might encounter using a default configuration.

Use the **WARN_QA** and
**ERROR_QA** variables to control the behavior of
these checks at the global level (i.e. in your custom distro
configuration). However, to skip one or more checks in recipes, you
should use **INSANE_SKIP**. For example, to skip
the check for symbolic link `.so` files in the main package of a
recipe, add the following to the recipe. You need to realize that the
package name override, in this example `${PN}`, must be used::

   INSANE_SKIP:${PN} += "dev-so"

Please keep in mind that the QA checks
are meant to detect real or potential problems in the packaged
output. So exercise caution when disabling these checks.

The tests you can list with the **WARN_QA** and
**ERROR_QA** variables are documented in the /ref-manual/qa-checks
document of the Yocto Project Reference Manual.

## ``kernel``

The ref-classes-kernel class handles building Linux kernels. The class contains
code to build all kernel trees. All needed headers are staged into the
**STAGING_KERNEL_DIR** directory to allow out-of-tree module builds
using the ref-classes-module class.

If a file named `defconfig` is listed in **SRC_URI**, then by default
ref-tasks-configure copies it as `.config` in the build directory,
so it is automatically used as the kernel configuration for the build. This
copy is not performed in case `.config` already exists there: this allows
recipes to produce a configuration by other means in
`do_configure:prepend`.

Each built kernel module is packaged separately and inter-module
dependencies are created by parsing the `modinfo` output.  If all modules
are required, then installing the `kernel-modules` package installs all
packages with modules and various other kernel packages such as
`kernel-vmlinux`.

The ref-classes-kernel class contains logic that allows you to embed an initial
RAM filesystem (**Initramfs**) image when you build the kernel image. For
information on how to build an **Initramfs**, see the
"dev-manual/building:building an initial ram filesystem (Initramfs) image" section in
the Yocto Project Development Tasks Manual.

Various other classes are used by the ref-classes-kernel and ref-classes-module classes
internally including the ref-classes-kernel-arch, ref-classes-module-base, and
ref-classes-linux-kernel-base classes.

## ``kernel-arch``

The ref-classes-kernel-arch class sets the `ARCH` environment variable for
Linux kernel compilation (including modules).

## ``kernel-devicetree``

The ref-classes-kernel-devicetree class, which is inherited by the
ref-classes-kernel class, supports device tree generation.

Its behavior is mainly controlled by the following variables:

-  **KERNEL_DEVICETREE_BUNDLE**: whether to bundle the kernel and device tree
-  **KERNEL_DTBDEST**: directory where to install DTB files
-  **KERNEL_DTBVENDORED**: whether to keep vendor subdirectories
-  **KERNEL_DTC_FLAGS**: flags for `dtc`, the Device Tree Compiler
-  **KERNEL_PACKAGE_NAME**: base name of the kernel packages

## ``kernel-fit-image``

The ref-classes-kernel-fit-image class provides support to pack a kernel image,
device trees, a U-boot script, an **Initramfs** and additional loadables into a
single FIT image.
In theory, a FIT image can support any number of kernels, U-boot scripts,
**Initramfs**, and device trees.
However, ref-classes-kernel-fit-image currently only supports
limited usecases: just one kernel image, an optional U-boot script,
an optional **Initramfs**, and any number of device trees and loadables.

The FIT image is created by a recipe which inherits the
ref-classes-kernel-fit-image class.
One such example is the `linux-yocto-fitimage` recipe which creates a FIT
image for the Linux Yocto kernel.
Additionally, it is required that **KERNEL_CLASSES** is set to include
ref-classes-kernel-fit-extra-artifacts.
The ref-classes-kernel-fit-extra-artifacts class exposes the required kernel
artifacts to the **DEPLOY_DIR_IMAGE** which are used by the
ref-classes-kernel-fit-image class to create the FIT image.

The simplest example for building a FIT image is to add::

   KERNEL_CLASSES += "kernel-fit-extra-artifacts"

to the machine **configuration file** and to execute::

   bitbake linux-yocto-fitimage

This results in a `fitImage` file deployed to the **DEPLOY_DIR_IMAGE**
directory and a `linux-yocto-fitimage` package which can be installed.

The same approach works for all variants of the `linux-yocto` kernel.
For example, if the `linux-yocto-rt` kernel should be used, add the following
lines to the machine configuration file::

   KERNEL_CLASSES += "kernel-fit-extra-artifacts"
   PREFERRED_PROVIDER_virtual/kernel = "linux-yocto-rt"

The FIT image, this time including the RT kernel, is built again by calling::

   bitbake linux-yocto-fitimage

For other kernels provided by other layers, the same approach would work.
However, it is usually more intuitive to add a custom FIT image recipe next to
the custom kernel recipe.
For example, if a layer provides a `linux-vanilla` recipe, a
`linux-vanilla-fitimage` recipe may be added as well.
The `linux-vanilla-fitimage` recipe can be created as a customized copy of
the `linux-yocto-fitimage` recipe.

Usually the kernel is built as a dependency of an image.
If the FIT image should be used as a replacement for the kernel image which
is installed in the root filesystem, then the following variables can be set
e.g. in the machine configuration file::

   # Create and deploy the vmlinux artifact which gets included into the FIT image
   KERNEL_CLASSES += "kernel-fit-extra-artifacts"

   # Do not install the kernel image package
   RRECOMMENDS:${KERNEL_PACKAGE_NAME}-base = ""
   # Install the FIT image package
   MACHINE_ESSENTIAL_EXTRA_RDEPENDS += "linux-yocto-fitimage"

   # Configure the image.bbclass to depend on the FIT image instead of only
   # the kernel to ensure the FIT image is built and deployed with the image
   KERNEL_DEPLOY_DEPEND = "linux-yocto-fitimage:do_deploy"

The ref-classes-kernel-fit-image class processes several variables that
allow configuration:

-  The options for the device tree compiler passed to `mkimage -D`
   when creating the FIT image are specified using the
   **UBOOT_MKIMAGE_DTCOPTS** variable.

-  Only a single kernel can be added to the FIT image created by
   ref-classes-kernel-fit-image and it is a mandatory component of the
   FIT image.
   The address where the kernel image is to be loaded by U-Boot is
   specified by **UBOOT_LOADADDRESS** and the entrypoint by
   **UBOOT_ENTRYPOINT**. Setting **FIT_ADDRESS_CELLS** to "2"
   is necessary if such addresses are 64 bit ones.

-  Multiple device trees can be added to the FIT image created by
   ref-classes-kernel-fit-image and the device tree is optional.
   The address where the device tree is to be loaded by U-Boot is
   specified by **UBOOT_DTBO_LOADADDRESS** for device tree overlays
   and by **UBOOT_DTB_LOADADDRESS** for device tree binaries.

-  Only a single **Initramfs** can be added to the FIT image created by
   ref-classes-kernel-fit-image. The **Initramfs** in FIT is optional.
   The address where the RAM disk image is to be loaded by U-Boot
   is specified by **UBOOT_RD_LOADADDRESS** and the entrypoint by
   **UBOOT_RD_ENTRYPOINT**. The **Initramfs** is added to the FIT image
   when **INITRAMFS_IMAGE** is specified.

-  It's recommended to add the **Initramfs** and the kernel image as
   independent image nodes to the FIT image.
   Bundling a RAM disk image with the kernel image and including the bundle
   (**INITRAMFS_IMAGE_BUNDLE** set to "1") in the FIT image is possible.
   However, this approach has the disadvantage that any change to the RAM
   disk image necessitates rebuilding the kernel image.
   This process requires the full kernel build directory, which is kind of
   incompatible with the **SSTATE_DIR** and, consequently, with SDKs.

-  Only a single U-Boot boot script can be added to the FIT image created by
   ref-classes-kernel-fit-image. The boot script is optional.
   The boot script is specified in the ITS file as a text file containing
   U-Boot commands. When using a boot script the recipe which inherits the
   ref-classes-kernel-fit-image class should add the script to
   **SRC_URI** and set the **FIT_UBOOT_ENV** variable to the name of the
   file like the following::

      FIT_UBOOT_ENV = "boot.txt"
      SRC_URI += "file://${FIT_UBOOT_ENV}"

   At run-time, U-boot's boot command can be configured to load the boot script
   from the FIT image and source it.

-  Multiple additional loadables (e.g.: firmwares for auxiliary processors) can
   be added to the FIT image created by ref-classes-kernel-fit-image.
   The addition of loadables is optional. The loadables are specified using the
   **FIT_LOADABLES** variable; each of them can then be configured through
   flags on the following variables: **FIT_LOADABLE_ARCH**,
   **FIT_LOADABLE_COMPRESSION**, **FIT_LOADABLE_DESCRIPTION**,
   **FIT_LOADABLE_ENTRYPOINT**, **FIT_LOADABLE_FILENAME**,
   **FIT_LOADABLE_LOADADDRESS**, **FIT_LOADABLE_OS** and
   **FIT_LOADABLE_TYPE**. All loadables specified in this way will be added
   to all configurations present in the FIT image.

-  The FIT image generated by the ref-classes-kernel-fit-image class is signed when the
   variables **UBOOT_SIGN_ENABLE**, **UBOOT_MKIMAGE_DTCOPTS**,
   **UBOOT_SIGN_KEYDIR** and **UBOOT_SIGN_KEYNAME** are set
   appropriately. The default values used for **FIT_HASH_ALG** and
   **FIT_SIGN_ALG** in ref-classes-kernel-fit-image are "sha256" and
   "rsa2048" respectively. The keys for signing the FIT image can be generated using
   the ref-classes-kernel-fit-image class when both **FIT_GENERATE_KEYS** and
   **UBOOT_SIGN_ENABLE** are set to "1".

## ``kernel-fit-extra-artifacts``

The ref-classes-kernel-fit-extra-artifacts class exposes the required
kernel artifacts to the **DEPLOY_DIR_IMAGE** directory.
These artifacts are used by the ref-classes-kernel-fit-image class to
create a FIT image that can include the kernel, device trees, an optional
U-Boot script, and an optional Initramfs.

This class is typically included by adding it to the **KERNEL_CLASSES**
variable in your kernel recipe or machine configuration when building FIT images.
It ensures that all necessary files are available for packaging into the FIT image,
such as the kernel binary, device tree blobs (DTBs), and other related files.

For example, to enable this class, set::

   KERNEL_CLASSES += "kernel-fit-extra-artifacts"

This is required when using the ref-classes-kernel-fit-image class to
generate FIT images for your kernel.

## ``kernel-grub``

The ref-classes-kernel-grub class updates the boot area and the boot menu with
the kernel as the priority boot mechanism while installing a RPM to
update the kernel on a deployed target.

## ``kernel-module-split``

The ref-classes-kernel-module-split class provides common functionality for
splitting Linux kernel modules into separate packages.

## ``kernel-uboot``

The ref-classes-kernel-uboot class provides support for building from
vmlinux-style kernel sources.

## ``kernel-uimage``

The ref-classes-kernel-uimage class provides support to pack uImage.

## ``kernel-yocto``

The ref-classes-kernel-yocto class provides common functionality for building
from linux-yocto style kernel source repositories.

## ``kernelsrc``

The ref-classes-kernelsrc class sets the Linux kernel source and version.

## ``lib_package``

The ref-classes-lib_package class supports recipes that build libraries and
produce executable binaries, where those binaries should not be
installed by default along with the library. Instead, the binaries are
added to a separate `${`\ **PN**\ `}-bin` package to
make their installation optional.

.. _ref-classes-libc*:

## ``libc*``

The ref-classes-libc* classes support recipes that build packages with `libc`:

-  The libc-common  class provides common support for building with
   `libc`.

-  The libc-package  class supports packaging up `glibc` and
   `eglibc`.

## ``license``

The ref-classes-license class provides license manifest creation and license
exclusion. This class is enabled by default using the default value for
the **INHERIT_DISTRO** variable.

## ``linux-kernel-base``

The ref-classes-linux-kernel-base class provides common functionality for
recipes that build out of the Linux kernel source tree. These builds
goes beyond the kernel itself. For example, the Perf recipe also
inherits this class.

## ``linuxloader``

Provides the function `linuxloader()`, which gives the value of the
dynamic loader/linker provided on the platform. This value is used by a
number of other classes.

## ``logging``

The ref-classes-logging class provides the standard shell functions used to log
messages for various BitBake severity levels (i.e. `bbplain`,
`bbnote`, `bbwarn`, `bberror`, `bbfatal`, and `bbdebug`).

This class is enabled by default since it is inherited by the ref-classes-base
class.

## ``meson``

The ref-classes-meson class allows to create recipes that build software
using the `Meson <https://mesonbuild.com/>`__ build system. You can use the
**MESON_BUILDTYPE**, **MESON_TARGET** and **EXTRA_OEMESON**
variables to specify additional configuration options to be passed using the
`meson` command line.

## ``metadata_scm``

The ref-classes-metadata_scm class provides functionality for querying the
branch and revision of a Source Code Manager (SCM) repository.

The ref-classes-base class uses this class to print the revisions of
each layer before starting every build. The ref-classes-metadata_scm
class is enabled by default because it is inherited by the
ref-classes-base class.

## ``mime``

The ref-classes-mime class generates the proper post-install and post-remove
(postinst/postrm) scriptlets for packages that install MIME type files.
These scriptlets call `update-mime-database` to add the MIME types to
the shared database.

## ``mime-xdg``

The ref-classes-mime-xdg class generates the proper
post-install and post-remove (postinst/postrm) scriptlets for packages
that install `.desktop` files containing `MimeType` entries.
These scriptlets call `update-desktop-database` to add the MIME types
to the database of MIME types handled by desktop files.

Thanks to this class, when users open a file through a file browser
on recently created images, they don't have to choose the application
to open the file from the pool of all known applications, even the ones
that cannot open the selected file.

If you have recipes installing their `.desktop` files as absolute
symbolic links, the detection of such files cannot be done by the current
implementation of this class. In this case, you have to add the corresponding
package names to the **MIME_XDG_PACKAGES** variable.

## ``mirrors``

The ref-classes-mirrors class sets up some standard
**MIRRORS** entries for source code mirrors. These
mirrors provide a fall-back path in case the upstream source specified
in **SRC_URI** within recipes is unavailable.

This class is enabled by default since it is inherited by the
ref-classes-base class.

## ``module``

The ref-classes-module class provides support for building out-of-tree Linux
kernel modules. The class inherits the ref-classes-module-base and
ref-classes-kernel-module-split classes, and implements the
ref-tasks-compile and ref-tasks-install tasks. The class provides
everything needed to build and package a kernel module.

For general information on out-of-tree Linux kernel modules, see the
"kernel-dev/common:incorporating out-of-tree modules"
section in the Yocto Project Linux Kernel Development Manual.

## ``module-base``

The ref-classes-module-base class provides the base functionality for
building Linux kernel modules. Typically, a recipe that builds software that
includes one or more kernel modules and has its own means of building the module
inherits this class as opposed to inheriting the ref-classes-module
class.

.. _ref-classes-multilib*:

## ``multilib*``

The ref-classes-multilib* classes provide support for building libraries with
different target optimizations or target architectures and installing
them side-by-side in the same image.

For more information on using the Multilib feature, see the
"dev-manual/libraries:combining multiple versions of library files into one image"
section in the Yocto Project Development Tasks Manual.

## ``native``

The ref-classes-native class provides common functionality for recipes that
build tools to run on the **Build Host** (i.e. tools that use the compiler
or other tools from the build host).

You can create a recipe that builds tools that run natively on the host
a couple different ways:

-  Create a `myrecipe-native.bb` recipe that inherits the ref-classes-native
   class. If you use this method, you must order the inherit statement
   in the recipe after all other inherit statements so that the
   ref-classes-native class is inherited last.

   .. note::

      When creating a recipe this way, the recipe name must follow this
      naming convention::

         myrecipe-native.bb

      Not using this naming convention can lead to subtle problems
      caused by existing code that depends on that naming convention.

-  Or, create a ref-classes-native variant of any target recipe (e.g.
   `myrecipe.bb`) by adding the following to the recipe::

      BBCLASSEXTEND = "native"

   Inside the
   recipe, use `:class-native` and `:class-target` overrides to
   specify any functionality specific to the respective native or target
   case.

Although applied differently, the ref-classes-native class is used with both
methods. The advantage of the second method is that you do not need to
have two separate recipes (assuming you need both) for native and
target. All common parts of the recipe are automatically shared.

## ``nativesdk``

The ref-classes-nativesdk class provides common functionality for recipes that
wish to build tools to run as part of an SDK (i.e. tools that run on
**SDKMACHINE**).

You can create a recipe that builds tools that run on the SDK machine a
couple different ways:

-  Create a `nativesdk-myrecipe.bb` recipe that inherits the
   ref-classes-nativesdk class. If you use this method, you must order the
   inherit statement in the recipe after all other inherit statements so
   that the ref-classes-nativesdk class is inherited last.

   .. note::

      When creating a recipe, you must follow this naming convention::

              nativesdk-myrecipe.bb

      Not doing so can lead to subtle problems because there is code that
      depends on the naming convention.

-  Or, create a ref-classes-nativesdk variant of any target recipe (e.g.
   `myrecipe.bb`) by adding the following to the recipe::

       BBCLASSEXTEND = "nativesdk"

   Inside the
   recipe, use `:class-nativesdk` and `:class-target` overrides to
   specify any functionality specific to the respective SDK machine or
   target case.

Although applied differently, the ref-classes-nativesdk class is used with both
methods. The advantage of the second method is that you do not need to
have two separate recipes (assuming you need both) for the SDK machine
and the target. All common parts of the recipe are automatically shared.

## ``nopackages``

Disables packaging tasks for those recipes and classes where packaging
is not needed.

## ``nospdx``

The ref-classes-nospdx allows a recipe to opt out of SPDX
generation provided by ref-classes-create-spdx.

## ``npm``

> **Warning:**
> The NPM fetcher is currently disabled due to security concerns. See
> `355cd226e0720a9ed7683bb01c8c0a58eee03664` for more information.
>

Provides support for building Node.js software fetched using the
node package manager (NPM) .

> **Note:**
> Currently, recipes inheriting this class must use the `npm://`
> fetcher to have dependencies fetched and packaged automatically.
>

For information on how to create NPM packages, see the
"dev-manual/packages:creating node package manager (npm) packages"
section in the Yocto Project Development Tasks Manual.

## ``overlayfs``

It's often desired in Embedded System design to have a read-only root filesystem.
But a lot of different applications might want to have read-write access to
some parts of a filesystem. It can be especially useful when your update mechanism
overwrites the whole root filesystem, but you may want your application data to be preserved
between updates. The ref-classes-overlayfs class provides a way
to achieve that by means of `overlayfs` and at the same time keeping the base
root filesystem read-only.

To use this class, set a mount point for a partition `overlayfs` is going to use as upper
layer in your machine configuration. The underlying file system can be anything that
is supported by `overlayfs`. This has to be done in your machine configuration::

  OVERLAYFS_MOUNT_POINT[data] = "/data"

> **Note:**

  * QA checks fail to catch file existence if you redefine this variable in your recipe!
  * Only the existence of the systemd mount unit file is checked, not its contents.
  * To get more details on `overlayfs`, its internals and supported operations, please refer
    to the official documentation of the `Linux kernel <https://www.kernel.org/doc/html/latest/filesystems/overlayfs.html>`__.

The class assumes you have a `data.mount` systemd unit defined elsewhere in your BSP
(e.g. in `systemd-machine-units` recipe) and it's installed into the image.

Then you can specify writable directories on a recipe basis (e.g. in my-application.bb)::

  OVERLAYFS_WRITABLE_PATHS[data] = "/usr/share/my-custom-application"

To support several mount points you can use a different variable flag. Assuming we
want to have a writable location on the file system, but do not need that the data
survives a reboot, then we could have a `mnt-overlay.mount` unit for a `tmpfs`
file system.

In your machine configuration::

  OVERLAYFS_MOUNT_POINT[mnt-overlay] = "/mnt/overlay"

and then in your recipe::

  OVERLAYFS_WRITABLE_PATHS[mnt-overlay] = "/usr/share/another-application"

On a practical note, your application recipe might require multiple
overlays to be mounted before running to avoid writing to the underlying
file system (which can be forbidden in case of read-only file system)
To achieve that ref-classes-overlayfs provides a `systemd`
helper service for mounting overlays. This helper service is named
`${PN}-overlays.service` and can be depended on in your application recipe
(named `application` in the following example) `systemd` unit by adding
to the unit the following::

  [Unit]
  After=application-overlays.service
  Requires=application-overlays.service

> **Note:**
> The class does not support the `/etc` directory itself, because `systemd` depends on it.
> In order to get `/etc` in overlayfs, see ref-classes-overlayfs-etc.
>

## ``overlayfs-etc``

In order to have the `/etc` directory in overlayfs a special handling at early
boot stage is required. The idea is to supply a custom init script that mounts
`/etc` before launching the actual init program, because the latter already
requires `/etc` to be mounted.

Example usage in image recipe::

   IMAGE_FEATURES += "overlayfs-etc"

> **Note:**
> This class must not be inherited directly. Use **IMAGE_FEATURES** or **EXTRA_IMAGE_FEATURES**
>

Your machine configuration should define at least the device, mount point, and file system type
you are going to use for `overlayfs`::

  OVERLAYFS_ETC_MOUNT_POINT = "/data"
  OVERLAYFS_ETC_DEVICE = "/dev/mmcblk0p2"
  OVERLAYFS_ETC_FSTYPE ?= "ext4"

To control more mount options you should consider setting mount options
(`defaults` is used by default)::

  OVERLAYFS_ETC_MOUNT_OPTIONS = "wsync"

The class provides two options for `/sbin/init` generation:

- The default option is to rename the original `/sbin/init` to `/sbin/init.orig`
  and place the generated init under original name, i.e. `/sbin/init`. It has an advantage
  that you won't need to change any kernel parameters in order to make it work,
  but it poses a restriction that package-management can't be used, because updating
  the init manager would remove the generated script.

- If you wish to keep original init as is, you can set::

   OVERLAYFS_ETC_USE_ORIG_INIT_NAME = "0"

  Then the generated init will be named `/sbin/preinit` and you would need to extend your
  kernel parameters manually in your bootloader configuration.

## ``own-mirrors``

The ref-classes-own-mirrors class makes it easier to set up your own
**PREMIRRORS** from which to first fetch source
before attempting to fetch it from the upstream specified in
**SRC_URI** within each recipe.

To use this class, inherit it globally and specify
**SOURCE_MIRROR_URL**. Here is an example::

   INHERIT += "own-mirrors"
   SOURCE_MIRROR_URL = "http://example.com/my-source-mirror"

You can specify only a single URL
in **SOURCE_MIRROR_URL**.

## ``package``

The ref-classes-package class supports generating packages from a build's
output. The core generic functionality is in `package.bbclass`. The
code specific to particular package types resides in these
package-specific classes: ref-classes-package_deb,
ref-classes-package_rpm, ref-classes-package_ipk.

You can control the list of resulting package formats by using the
**PACKAGE_CLASSES** variable defined in your `conf/local.conf`
configuration file, which is located in the **Build Directory**.
When defining the variable, you can specify one or more package types.
Since images are generated from packages, a packaging class is needed
to enable image generation. The first class listed in this variable is
used for image generation.

If you take the optional step to set up a repository (package feed) on
the development host that can be used by DNF, you can install packages
from the feed while you are running the image on the target (i.e.
runtime installation of packages). For more information, see the
"dev-manual/packages:using runtime package management"
section in the Yocto Project Development Tasks Manual.

The package-specific class you choose can affect build-time performance
and has space ramifications. In general, building a package with IPK
takes about thirty percent less time as compared to using RPM to build
the same or similar package. This comparison takes into account a
complete build of the package with all dependencies previously built.
The reason for this discrepancy is because the RPM package manager
creates and processes more **Metadata** than the IPK package
manager. Consequently, you might consider setting **PACKAGE_CLASSES** to
"ref-classes-package_ipk" if you are building smaller systems.

Before making your package manager decision, however, you should
consider some further things about using RPM:

-  RPM starts to provide more abilities than IPK due to the fact that it
   processes more Metadata. For example, this information includes
   individual file types, file checksum generation and evaluation on
   install, sparse file support, conflict detection and resolution for
   Multilib systems, ACID style upgrade, and repackaging abilities for
   rollbacks.

-  For smaller systems, the extra space used for the Berkeley Database
   and the amount of metadata when using RPM can affect your ability to
   perform on-device upgrades.

You can find additional information on the effects of the package class
at these two Yocto Project mailing list links:

-  `/pipermail/poky/2011-May/006362.html`

-  `/pipermail/poky/2011-May/006363.html`

## ``package_deb``

The ref-classes-package_deb class provides support for creating packages that
use the Debian (i.e. `.deb`) file format. The class ensures the
packages are written out in a `.deb` file format to the
`${`\ **DEPLOY_DIR_DEB**\ `}` directory.

This class inherits the ref-classes-package class and
is enabled through the **PACKAGE_CLASSES**
variable in the `local.conf` file.

## ``package_ipk``

The ref-classes-package_ipk class provides support for creating packages that
use the IPK (i.e. `.ipk`) file format. The class ensures the packages
are written out in a `.ipk` file format to the
`${`\ **DEPLOY_DIR_IPK**\ `}` directory.

This class inherits the ref-classes-package class and
is enabled through the **PACKAGE_CLASSES**
variable in the `local.conf` file.

## ``package_rpm``

The ref-classes-package_rpm class provides support for creating packages that
use the RPM (i.e. `.rpm`) file format. The class ensures the packages
are written out in a `.rpm` file format to the
`${`\ **DEPLOY_DIR_RPM**\ `}` directory.

This class inherits the ref-classes-package class and
is enabled through the **PACKAGE_CLASSES**
variable in the `local.conf` file.

## ``packagedata``

The ref-classes-packagedata class provides common functionality for reading
`pkgdata` files found in **PKGDATA_DIR**. These
files contain information about each output package produced by the
OpenEmbedded build system.

This class is enabled by default because it is inherited by the
ref-classes-package class.

## ``packagegroup``

The ref-classes-packagegroup class sets default values appropriate for package
group recipes (e.g. **PACKAGES**, **PACKAGE_ARCH**, **ALLOW_EMPTY**, and
so forth). It is highly recommended that all package group recipes
inherit this class.

For information on how to use this class, see the
"dev-manual/customizing-images:customizing images using custom package groups"
section in the Yocto Project Development Tasks Manual.

Previously, this class was called the `task` class.

## ``patch``

The ref-classes-patch class provides all functionality for applying patches
during the ref-tasks-patch task.

This class is enabled by default because it is inherited by the
ref-classes-base class.

## ``perlnative``

When inherited by a recipe, the ref-classes-perlnative class supports using the
native version of Perl built by the build system rather than using the
version provided by the build host.

## ``pypi``

The ref-classes-pypi class sets variables appropriately for recipes that build
Python modules from `PyPI <https://pypi.org/>`__, the Python Package Index.
By default it determines the PyPI package name based upon **BPN**
(stripping the "python-" or "python3-" prefix off if present), however in
some cases you may need to set it manually in the recipe by setting
**PYPI_PACKAGE**.

Variables set by the ref-classes-pypi class include **SRC_URI**, **SECTION**,
**HOMEPAGE**, **UPSTREAM_CHECK_URI**, **UPSTREAM_CHECK_REGEX**
and **CVE_PRODUCT**.

## ``python_flit_core``

The ref-classes-python_flit_core class enables building Python modules which declare
the  `PEP-517 <https://www.python.org/dev/peps/pep-0517/>`__ compliant
`flit_core.buildapi` `build-backend` in the `[build-system]`
section of `pyproject.toml` (See `PEP-518 <https://www.python.org/dev/peps/pep-0518/>`__).

Python modules built with `flit_core.buildapi` are pure Python (no
`C` or `Rust` extensions).

Internally this uses the ref-classes-python_pep517 class.

## ``python_maturin``

The ref-classes-python_maturin class provides support for python-maturin, a replacement
for setuptools_rust and another "backend" for building Python Wheels.

## ``python_mesonpy``

The ref-classes-python_mesonpy class enables building Python modules which use the
meson-python build system.

Internally this uses the ref-classes-python_pep517 class.

## ``python_pdm``

The ref-classes-python_pdm class adds support for building Python
packages with the `PDM <https://pdm-project.org/>`__ package and dependency manager.

This class adds  `python3-pdm-backend-native` to the recipe's build-time
dependencies.

Internally this uses the ref-classes-python_pep517 class.

## ``python_pep517``

The ref-classes-python_pep517 class builds and installs a Python `wheel` binary
archive (see `PEP-517 <https://peps.python.org/pep-0517/>`__).

Recipes wouldn't inherit this directly, instead typically another class will
inherit this and add the relevant native dependencies.

Examples of classes which do this are ref-classes-python_flit_core,
ref-classes-python_setuptools_build_meta, and
ref-classes-python_poetry_core.

## ``python_poetry_core``

The ref-classes-python_poetry_core class enables building Python modules which use the
`Poetry Core <https://python-poetry.org>`__ build system.

Internally this uses the ref-classes-python_pep517 class.

## ``python_pyo3``

The ref-classes-python_pyo3 class helps make sure that Python extensions
written in Rust and built with `PyO3 <https://pyo3.rs/>`__, properly set up the
environment for cross compilation.

This class is internal to the ref-classes-python-setuptools3_rust class
and is not meant to be used directly in recipes.

## ``python-setuptools3_rust``

The ref-classes-python-setuptools3_rust class enables building Python
extensions implemented in Rust with `PyO3 <https://pyo3.rs/>`__, which allows
to compile and distribute Python extensions written in Rust as easily
as if they were written in C.

This class inherits the ref-classes-setuptools3 and
ref-classes-python_pyo3 classes.

## ``pixbufcache``

The ref-classes-pixbufcache class generates the proper post-install and
post-remove (postinst/postrm) scriptlets for packages that install
pixbuf loaders, which are used with `gdk-pixbuf`. These scriptlets
call `update_pixbuf_cache` to add the pixbuf loaders to the cache.
Since the cache files are architecture-specific, `update_pixbuf_cache`
is run using QEMU if the postinst scriptlets need to be run on the build
host during image creation.

If the pixbuf loaders being installed are in packages other than the
recipe's main package, set
**PIXBUF_PACKAGES** to specify the packages
containing the loaders.

## ``pkgconfig``

The ref-classes-pkgconfig class provides a standard way to get header and
library information by using `pkg-config`. This class aims to smooth
integration of `pkg-config` into libraries that use it.

During staging, BitBake installs `pkg-config` data into the
`sysroots/` directory. By making use of sysroot functionality within
`pkg-config`, the ref-classes-pkgconfig class no longer has to manipulate the
files.

## ``populate_sdk``

The ref-classes-populate-sdk class provides support for SDK-only recipes. For
information on advantages gained when building a cross-development
toolchain using the ref-tasks-populate_sdk
task, see the "sdk-manual/appendix-obtain:building an sdk installer"
section in the Yocto Project Application Development and the Extensible
Software Development Kit (eSDK) manual.

.. _ref-classes-populate-sdk-*:

## ``populate_sdk_*``

The ref-classes-populate-sdk-* classes support SDK creation and consist of the
following classes:

-  populate_sdk_base : The base class supporting SDK creation under
   all package managers (i.e. DEB, RPM, and opkg).

-  populate_sdk_deb : Supports creation of the SDK given the Debian
   package manager.

-  populate_sdk_rpm : Supports creation of the SDK given the RPM
   package manager.

-  populate_sdk_ipk : Supports creation of the SDK given the opkg
   (IPK format) package manager.

-  populate_sdk_ext : Supports extensible SDK creation under all
   package managers.

The populate_sdk_base  class inherits the appropriate
`populate_sdk_*` (i.e. `deb`, `rpm`, and `ipk`) based on
**IMAGE_PKGTYPE**.

The base class ensures all source and destination directories are
established and then populates the SDK. After populating the SDK, the
populate_sdk_base  class constructs two sysroots:
`${`\ **SDK_ARCH**\ `}-nativesdk`, which
contains the cross-compiler and associated tooling, and the target,
which contains a target root filesystem that is configured for the SDK
usage. These two images reside in **SDK_OUTPUT**,
which consists of the following::

   ${SDK_OUTPUT}/${SDK_ARCH}-nativesdk-pkgs
   ${SDK_OUTPUT}/${SDKTARGETSYSROOT}/target-pkgs

Finally, the base populate SDK class creates the toolchain environment
setup script, the tarball of the SDK, and the installer.

The respective populate_sdk_deb , populate_sdk_rpm , and
populate_sdk_ipk  classes each support the specific type of SDK.
These classes are inherited by and used with the populate_sdk_base 
class.

For more information on the cross-development toolchain generation, see
the "overview-manual/concepts:cross-development toolchain generation"
section in the Yocto Project Overview and Concepts Manual. For
information on advantages gained when building a cross-development
toolchain using the ref-tasks-populate_sdk
task, see the
"sdk-manual/appendix-obtain:building an sdk installer"
section in the Yocto Project Application Development and the Extensible
Software Development Kit (eSDK) manual.

## ``prexport``

The ref-classes-prexport class provides functionality for exporting
**PR** values.

> **Note:**
> This class is not intended to be used directly. Rather, it is enabled
> when using "`bitbake-prserv-tool export`".
>

## ``primport``

The ref-classes-primport class provides functionality for importing
**PR** values.

> **Note:**
> This class is not intended to be used directly. Rather, it is enabled
> when using "`bitbake-prserv-tool import`".
>

## ``prserv``

The ref-classes-prserv class provides functionality for using a :ref:`PR
service <dev-manual/packages:working with a pr service>` in order to
automatically manage the incrementing of the **PR**
variable for each recipe.

This class is enabled by default because it is inherited by the
ref-classes-package class. However, the OpenEmbedded
build system will not enable the functionality of this class unless
**PRSERV_HOST** has been set.

## ``ptest``

The ref-classes-ptest class provides functionality for packaging and installing
runtime tests for recipes that build software that provides these tests.

This class is intended to be inherited by individual recipes. However,
the class' functionality is largely disabled unless "ptest" appears in
**DISTRO_FEATURES**. See the
"test-manual/ptest:testing packages with ptest"
section in the Yocto Project Development Tasks Manual for more information
on ptest.

## ``ptest-cargo``

The ref-classes-ptest-cargo class is a class which extends the
ref-classes-cargo class and adds `compile_ptest_cargo` and
`install_ptest_cargo` steps to respectively build and install
test suites defined in the `Cargo.toml` file, into a dedicated
`-ptest` package.

## ``ptest-gnome``

Enables package tests (ptests) specifically for GNOME packages, which
have tests intended to be executed with `gnome-desktop-testing`.

For information on setting up and running ptests, see the
"test-manual/ptest:testing packages with ptest"
section in the Yocto Project Development Tasks Manual.

## ``ptest-python-pytest``

The ref-classes-ptest-python-pytest class can be inherited in Python-based
recipes to automatically configure the ref-classes-ptest class for Python
packages leveraging the `pytest <https://docs.pytest.org>`__ unit test framework.

Within the recipe, the **PTEST_PYTEST_DIR** variable specifies the path to
the directory containing the tests that will be installed in **D** by the
ref-tasks-install_ptest_base task, as well as a specific `run-ptest`
script for this task.

## ``python3-dir``

The ref-classes-python3-dir class provides the base version, location, and site
package location for Python 3.

## ``python3native``

The ref-classes-python3native class supports using the native version of Python
3 built by the build system rather than support of the version provided
by the build host.

## ``python3targetconfig``

The ref-classes-python3targetconfig class supports using the native version of Python
3 built by the build system rather than support of the version provided
by the build host, except that the configuration for the target machine
is accessible (such as correct installation directories). This also adds a
dependency on target `python3`, so should only be used where appropriate
in order to avoid unnecessarily lengthening builds.

## ``qemu``

The ref-classes-qemu class provides functionality for recipes that either need
QEMU or test for the existence of QEMU. Typically, this class is used to
run programs for a target system on the build host using QEMU's
application emulation mode.

## ``recipe_sanity``

The ref-classes-recipe_sanity class checks for the presence of any host system
recipe prerequisites that might affect the build (e.g. variables that
are set or software that is present).

## ``relative_symlinks``

The ref-classes-relative_symlinks class walks the symbolic links in the
**D** directory and replaces links pointing to absolute paths to relative
paths. This is occasionally used in some recipes that create wrong symbolic
links when their ref-classes-native version is built, and/or would cause
breakage in the overview-manual/concepts:shared state cache.

For example, if the following symbolic link is found in **D**::

   /usr/bin/foo -> /sbin/bar

It is replaced by::

   /usr/bin/foo -> ../../sbin/bar

## ``relocatable``

The ref-classes-relocatable class enables relocation of binaries when they are
installed into the sysroot.

This class makes use of the ref-classes-chrpath class and is used by
both the ref-classes-cross and ref-classes-native classes.

## ``remove-libtool``

The ref-classes-remove-libtool class adds a post function to the
ref-tasks-install task to remove all `.la` files
installed by `libtool`. Removing these files results in them being
absent from both the sysroot and target packages.

If a recipe needs the `.la` files to be installed, then the recipe can
override the removal by setting `REMOVE_LIBTOOL_LA` to "0" as follows::

   REMOVE_LIBTOOL_LA = "0"

> **Note:**
> The ref-classes-remove-libtool class is not enabled by default.
>

## ``report-error``

The ref-classes-report-error class supports enabling the :ref:`error reporting
tool <dev-manual/error-reporting-tool:using the error reporting tool>`",
which allows you to submit build error information to a central database.

The class collects debug information for recipe, recipe version, task,
machine, distro, build system, target system, host distro, branch,
commit, and log. From the information, report files using a JSON format
are created and stored in
`${`\ **LOG_DIR**\ `}/error-report`.

## ``retain``

The ref-classes-retain class can be used to create a tarball of the work
directory for a recipe when one of its tasks fails, or any other nominated
directories. It is useful in cases where the environment in which builds are run
is ephemeral or otherwise inaccessible for examination during debugging.

To enable, add the following to your configuration::

   INHERIT += "retain"

The class can be disabled for specific recipes using the **RETAIN_ENABLED**
variable.

## ``rm_work``

The ref-classes-rm-work class supports deletion of temporary workspace, which
can ease your hard drive demands during builds.

The OpenEmbedded build system can use a substantial amount of disk space
during the build process. A portion of this space is the work files
under the `${TMPDIR}/work` directory for each recipe. Once the build
system generates the packages for a recipe, the work files for that
recipe are no longer needed. However, by default, the build system
preserves these files for inspection and possible debugging purposes. If
you would rather have these files deleted to save disk space as the build
progresses, you can enable ref-classes-rm-work by adding the following to
your `local.conf` file, which is found in the **Build Directory**::

   INHERIT += "rm_work"

If you are modifying and building source code out of the work directory for a
recipe, enabling ref-classes-rm-work will potentially result in your
changes to the source being lost. To exclude some recipes from having their work
directories deleted by ref-classes-rm-work, you can add the names of the
recipe or recipes you are working on to the **RM_WORK_EXCLUDE** variable,
which can also be set in your `local.conf` file. Here is an example::

   RM_WORK_EXCLUDE += "busybox glibc"

.. _ref-classes-rootfs*:

## ``rootfs*``

The ref-classes-rootfs* classes support creating the root filesystem for an
image and consist of the following classes:

-  The rootfs-postcommands  class, which defines filesystem
   post-processing functions for image recipes.

-  The rootfs_deb  class, which supports creation of root filesystems
   for images built using `.deb` packages.

-  The rootfs_rpm  class, which supports creation of root filesystems
   for images built using `.rpm` packages.

-  The rootfs_ipk  class, which supports creation of root filesystems
   for images built using `.ipk` packages.

-  The rootfsdebugfiles  class, which installs additional files found
   on the build host directly into the root filesystem.

The root filesystem is created from packages using one of the
ref-classes-rootfs* files as determined by the **PACKAGE_CLASSES**
variable.

For information on how root filesystem images are created, see the
"overview-manual/concepts:image generation"
section in the Yocto Project Overview and Concepts Manual.

## ``rust``

The ref-classes-rust class is an internal class which is just used
in the "rust" recipe, to build the Rust compiler and runtime
library. Except for this recipe, it is not intended to be used directly.

## ``rust-common``

The ref-classes-rust-common class is an internal class to the
ref-classes-cargo_common and ref-classes-rust classes and is not
intended to be used directly.

## ``rust-target-config``

The ref-classes-rust-target-config class is an internal class to the
ref-classes-cargo_common and ref-classes-rust classes and is not
intended to be used directly.

It is used to generate a JSON specification file from the features listed in
**TUNE_FEATURES**, which is used for cross-compiling. The logic is done in a
`do_rust_gen_targets` task.

## ``sanity``

The ref-classes-sanity class checks to see if prerequisite software is present
on the host system so that users can be notified of potential problems
that might affect their build. The class also performs basic user
configuration checks from the `local.conf` configuration file to
prevent common mistakes that cause build failures. Distribution policy
usually determines whether to include this class.

## ``scons``

The ref-classes-scons class supports recipes that need to build software
that uses the SCons build system. You can use the **EXTRA_OESCONS**
variable to specify additional configuration options you want to pass SCons
command line.

## ``sdl``

The ref-classes-sdl class supports recipes that need to build software that uses
the Simple DirectMedia Layer (SDL) library.

## ``python_setuptools_build_meta``

The ref-classes-python_setuptools_build_meta class enables building
Python modules which declare the
`PEP-517 <https://www.python.org/dev/peps/pep-0517/>`__ compliant
`setuptools.build_meta` `build-backend` in the `[build-system]`
section of `pyproject.toml` (See `PEP-518 <https://www.python.org/dev/peps/pep-0518/>`__).

Python modules built with `setuptools.build_meta` can be pure Python or
include `C` or `Rust` extensions).

Internally this uses the ref-classes-python_pep517 class.

## ``setuptools3``

The ref-classes-setuptools3 class supports Python version 3.x extensions
that use build systems based on `setuptools` (e.g. only have a `setup.py`
and have not migrated to the official `pyproject.toml` format). If your recipe
uses these build systems, the recipe needs to inherit the
ref-classes-setuptools3 class.

   .. note::

      The ref-classes-setuptools3 class ref-tasks-compile task now calls
      `setup.py bdist_wheel` to build the `wheel` binary archive format
      (See `PEP-427 <https://www.python.org/dev/peps/pep-0427/>`__).

      A consequence of this is that legacy software still using deprecated
      `distutils` from the Python standard library cannot be packaged as
      `wheels`. A common solution is the replace
      `from distutils.core import setup` with `from setuptools import setup`.

   .. note::

     The ref-classes-setuptools3 class ref-tasks-install task now
     installs the `wheel` binary archive. In current versions of
     `setuptools` the legacy `setup.py install` method is deprecated. If
     the `setup.py` cannot be used with wheels, for example it creates files
     outside of the Python module or standard entry points, then
     ref-classes-setuptools3_legacy should be used.

## ``setuptools3_legacy``

The ref-classes-setuptools3_legacy class supports
Python version 3.x extensions that use build systems based on `setuptools`
(e.g. only have a `setup.py` and have not migrated to the official
`pyproject.toml` format). Unlike ref-classes-setuptools3,
this uses the traditional `setup.py` `build` and `install` commands and
not wheels. This use of `setuptools` like this is
`deprecated <https://github.com/pypa/setuptools/blob/main/CHANGES.rst#v5830>`__
but still relatively common.

## ``setuptools3-base``

The ref-classes-setuptools3-base class provides a reusable base for
other classes that support building Python version 3.x extensions. If you need
functionality that is not provided by the ref-classes-setuptools3 class,
you may want to `inherit setuptools3-base`. Some recipes do not need the tasks
in the ref-classes-setuptools3 class and inherit this class instead.

## ``sign_rpm``

The ref-classes-sign_rpm class supports generating signed RPM packages.

## ``siteinfo``

The ref-classes-siteinfo class provides information about the targets
that might be needed by other classes or recipes.

As an example, consider Autotools, which can require tests that must
execute on the target hardware. Since this is not possible in general
when cross compiling, site information is used to provide cached test
results so these tests can be skipped over but still make the correct
values available. The `meta/site directory` contains test results
sorted into different categories such as architecture, endianness, and
the `libc` used. Site information provides a list of files containing
data relevant to the current build in the **CONFIG_SITE** variable that
Autotools automatically picks up.

The class also provides variables like **SITEINFO_ENDIANNESS** and
**SITEINFO_BITS** that can be used elsewhere in the metadata.

## ``sstate``

The ref-classes-sstate class provides support for Shared State (sstate).
By default, the class is enabled through the **INHERIT_DISTRO** variable's
default value.

For more information on sstate, see the
"overview-manual/concepts:shared state cache"
section in the Yocto Project Overview and Concepts Manual.

## ``staging``

The ref-classes-staging class installs files into individual recipe work
directories for sysroots. The class contains the following key tasks:

-  The ref-tasks-populate_sysroot task,
   which is responsible for handing the files that end up in the recipe
   sysroots.

-  The
   ref-tasks-prepare_recipe_sysroot
   task (a "partner" task to the `populate_sysroot` task), which
   installs the files into the individual recipe work directories (i.e.
   **WORKDIR**).

The code in the ref-classes-staging class is complex and basically works
in two stages:

-  *Stage One:* The first stage addresses recipes that have files they
   want to share with other recipes that have dependencies on the
   originating recipe. Normally these dependencies are installed through
   the ref-tasks-install task into
   `${`\ **D**\ `}`. The ref-tasks-populate_sysroot task
   copies a subset of these files into `${SYSROOT_DESTDIR}`. This
   subset of files is controlled by the
   **SYSROOT_DIRS**,
   **SYSROOT_DIRS_NATIVE**, and
   **SYSROOT_DIRS_IGNORE**
   variables.

   .. note::

      Additionally, a recipe can customize the files further by
      declaring a processing function in the **SYSROOT_PREPROCESS_FUNCS**
      variable.

   A shared state (sstate) object is built from these files and the
   files are placed into a subdirectory of
   structure-build-tmp-sysroots-components.
   The files are scanned for hardcoded paths to the original
   installation location. If the location is found in text files, the
   hardcoded locations are replaced by tokens and a list of the files
   needing such replacements is created. These adjustments are referred
   to as "FIXMEs". The list of files that are scanned for paths is
   controlled by the **SSTATE_SCAN_FILES**
   variable.

-  *Stage Two:* The second stage addresses recipes that want to use
   something from another recipe and declare a dependency on that recipe
   through the **DEPENDS** variable. The recipe will
   have a
   ref-tasks-prepare_recipe_sysroot
   task and when this task executes, it creates the `recipe-sysroot`
   and `recipe-sysroot-native` in the recipe work directory (i.e.
   **WORKDIR**). The OpenEmbedded build system
   creates hard links to copies of the relevant files from
   `sysroots-components` into the recipe work directory.

   .. note::

      If hard links are not possible, the build system uses actual
      copies.

   The build system then addresses any "FIXMEs" to paths as defined from
   the list created in the first stage.

   Finally, any files in `${bindir}` within the sysroot that have the
   prefix "`postinst-`" are executed.

   .. note::

      Although such sysroot post installation scripts are not
      recommended for general use, the files do allow some issues such
      as user creation and module indexes to be addressed.

   Because recipes can have other dependencies outside of **DEPENDS**
   (e.g. `do_unpack[depends] += "tar-native:do_populate_sysroot"`),
   the sysroot creation function `extend_recipe_sysroot` is also added
   as a pre-function for those tasks whose dependencies are not through
   **DEPENDS** but operate similarly.

   When installing dependencies into the sysroot, the code traverses the
   dependency graph and processes dependencies in exactly the same way
   as the dependencies would or would not be when installed from sstate.
   This processing means, for example, a native tool would have its
   native dependencies added but a target library would not have its
   dependencies traversed or installed. The same sstate dependency code
   is used so that builds should be identical regardless of whether
   sstate was used or not. For a closer look, see the
   `setscene_depvalid()` function in the ref-classes-sstate class.

   The build system is careful to maintain manifests of the files it
   installs so that any given dependency can be installed as needed. The
   sstate hash of the installed item is also stored so that if it
   changes, the build system can reinstall it.

## ``syslinux``

The ref-classes-syslinux class provides syslinux-specific functions for
building bootable images.

The class supports the following variables:

-  **INITRD**: Indicates list of filesystem images to
   concatenate and use as an initial RAM disk (initrd). This variable is
   optional.

-  **ROOTFS**: Indicates a filesystem image to include
   as the root filesystem. This variable is optional.

-  **AUTO_SYSLINUXMENU**: Enables creating
   an automatic menu when set to "1".

-  **LABELS**: Lists targets for automatic
   configuration.

-  **APPEND**: Lists append string overrides for each
   label.

-  **SYSLINUX_OPTS**: Lists additional options
   to add to the syslinux file. Semicolon characters separate multiple
   options.

-  **SYSLINUX_SPLASH**: Lists a background
   for the VGA boot menu when you are using the boot menu.

-  **SYSLINUX_DEFAULT_CONSOLE**: Set
   to "console=ttyX" to change kernel boot default console.

-  **SYSLINUX_SERIAL**: Sets an alternate
   serial port. Or, turns off serial when the variable is set with an
   empty string.

-  **SYSLINUX_SERIAL_TTY**: Sets an
   alternate "console=tty..." kernel boot argument.

## ``systemd``

The ref-classes-systemd class provides support for recipes that install
systemd unit files.

The functionality for this class is disabled unless you have "systemd"
in **DISTRO_FEATURES**.

Under this class, the recipe or Makefile (i.e. whatever the recipe is
calling during the ref-tasks-install task)
installs unit files into
`${`\ **D**\ `}${systemd_unitdir}/system`. If the unit
files being installed go into packages other than the main package, you
need to set **SYSTEMD_PACKAGES** in your
recipe to identify the packages in which the files will be installed.

You should set **SYSTEMD_SERVICE** to the
name of the service file. You should also use a package name override to
indicate the package to which the value applies. If the value applies to
the recipe's main package, use `${`\ **PN**\ `}`. Here
is an example from the connman recipe::

   SYSTEMD_SERVICE:${PN} = "connman.service"

Services are set up to start on boot automatically
unless you have set
**SYSTEMD_AUTO_ENABLE** to "disable".

For more information on ref-classes-systemd, see the
"dev-manual/init-manager:selecting an initialization manager"
section in the Yocto Project Development Tasks Manual.

## ``systemd-boot``

The ref-classes-systemd-boot class provides functions specific to the
systemd-boot bootloader for building bootable images. This is an
internal class and is not intended to be used directly.

> **Note:**
> The ref-classes-systemd-boot class is a result from merging the `gummiboot` class
> used in previous Yocto Project releases with the `systemd` project.
>

Set the **EFI_PROVIDER** variable to "ref-classes-systemd-boot" to
use this class. Doing so creates a standalone EFI bootloader that is not
dependent on systemd.

For information on more variables used and supported in this class, see
the **SYSTEMD_BOOT_CFG**,
**SYSTEMD_BOOT_ENTRIES**, and
**SYSTEMD_BOOT_TIMEOUT** variables.

You can also see the `Systemd-boot
documentation <https://www.freedesktop.org/wiki/Software/systemd/systemd-boot/>`__
for more information.

## ``terminal``

The ref-classes-terminal class provides support for starting a terminal
session.  The **OE_TERMINAL** variable controls which terminal emulator is
used for the session.

Other classes use the ref-classes-terminal class anywhere a separate
terminal session needs to be started. For example, the ref-classes-patch
class assuming **PATCHRESOLVE** is set to "user", the
ref-classes-cml1 class, and the ref-classes-devshell class all
use the ref-classes-terminal class.

## ``testexport``

Based on the ref-classes-testimage class, the
ref-classes-testexport class can be used to export the test environment
outside of the **OpenEmbedded Build System**. This will generate the
directory structure to execute the runtime tests using the
runexported.py 
Python script.

For more details on how to use ref-classes-testexport, see
the test-manual/runtime-testing:Exporting Tests section in the Yocto
Project Test Environment Manual.

## ``testimage``

The ref-classes-testimage class supports running automated tests against
images using QEMU and on actual hardware. The classes handle loading the
tests and starting the image. To use the classes, you need to perform
steps to set up the environment.

To enable this class, add the following to your configuration::

   IMAGE_CLASSES += "testimage"

The tests are commands that run on the target system over `ssh`. Each
test is written in Python and makes use of the `unittest` module.

The ref-classes-testimage class runs tests on an image when called using the
following::

   $ bitbake -c testimage image

Alternatively, if you wish to have tests automatically run for each image
after it is built, you can set **TESTIMAGE_AUTO**::

   TESTIMAGE_AUTO = "1"

For information on how to enable, run, and create new tests, see the
"test-manual/runtime-testing:performing automated runtime testing"
section in the Yocto Project Test Environment Manual.

## ``testsdk``

This class supports running automated tests against software development
kits (SDKs). The ref-classes-testsdk class runs tests on an SDK when called
using the following::

   $ bitbake -c testsdk image

The list of test modules that are run can be controlled with the
**TESTSDK_SUITES** variable.

> **Note:**
> Best practices include using **IMAGE_CLASSES** rather than
> **INHERIT** to inherit the ref-classes-testsdk class for automated SDK
> testing.
>

## ``texinfo``

This class should be inherited by recipes whose upstream packages invoke
the `texinfo` utilities at build-time. Native and cross recipes are
made to use the dummy scripts provided by `texinfo-dummy-native`, for
improved performance. Target architecture recipes use the genuine
Texinfo utilities. By default, they use the Texinfo utilities on the
host system.

> **Note:**
> If you want to use the Texinfo recipe shipped with the build system,
> you can remove "texinfo-native" from **ASSUME_PROVIDED** and makeinfo
> from **SANITY_REQUIRED_UTILITIES**.
>

## ``toaster``

The ref-classes-toaster class collects information about packages and images and
sends them as events that the BitBake user interface can receive. The
class is enabled when the Toaster user interface is running.

This class is not intended to be used directly.

## ``toolchain-scripts``

The ref-classes-toolchain-scripts class provides the scripts used for setting up
the environment for installed SDKs.

## ``toolchain/clang``

The ref-classes-toolchain-clang class defines commands for building
recipes with Clang/LLVM compiler and utilities.

This class is not meant to be inherited directly. Instead, you should either:

-  set the **PREFERRED_TOOLCHAIN_TARGET**, **PREFERRED_TOOLCHAIN_NATIVE**
   or **PREFERRED_TOOLCHAIN_SDK** variables to "clang" from a
   **Configuration File**. This will make the ref-classes-base class
   use the ref-classes-toolchain-clang accordingly. All recipes will
   be built with the Clang/LLVM toolchain, exception be made for recipes that
   override the value of **TOOLCHAIN** or **TOOLCHAIN_NATIVE** to
   another value.

-  set **TOOLCHAIN** or **TOOLCHAIN_NATIVE** to "clang" from a recipe
   when the recipe needs to override the default toolchain set by
   **PREFERRED_TOOLCHAIN_TARGET**, **PREFERRED_TOOLCHAIN_NATIVE** or
   **PREFERRED_TOOLCHAIN_SDK**.

## ``toolchain/clang-native``

The ref-classes-toolchain-clang-native class defines commands for
building ref-classes-native recipes with Clang/LLVM compiler and
utilities independently of the build context.

The ref-classes-toolchain-gcc-native class defines **BUILD_CC**,
**BUILD_CXX** and other such variables which are rarely used in recipes.
Exception be made for target recipes that need to use the compiler from the
build host at some point during the build.

This class should not be inherited directly. It is inherited by the
ref-classes-base class if **TOOLCHAIN_NATIVE** is set to "clang".

## ``toolchain/gcc``

The ref-classes-toolchain-gcc class defines commands for building
recipes with GCC/Binutils compiler and utilities.

This class is not meant to be inherited directly. Instead, you should either:

-  set the **PREFERRED_TOOLCHAIN_TARGET**, **PREFERRED_TOOLCHAIN_NATIVE**
   or **PREFERRED_TOOLCHAIN_SDK** variables to "gcc" from a
   **Configuration File**. This will make the ref-classes-base class
   use the ref-classes-toolchain-gcc accordingly. All recipes will
   be built with the GCC/Binutils toolchain, exception be made for recipes that
   override the value of **TOOLCHAIN** or **TOOLCHAIN_NATIVE** to
   another value.

-  set **TOOLCHAIN** or **TOOLCHAIN_NATIVE** to "gcc" from a recipe
   when the recipe needs to override the default toolchain set by
   **PREFERRED_TOOLCHAIN_TARGET**, **PREFERRED_TOOLCHAIN_NATIVE** or
   **PREFERRED_TOOLCHAIN_SDK**.

## ``toolchain/gcc-native``

The ref-classes-toolchain-gcc-native class defines commands for building
ref-classes-native recipes with GCC/Binutils compiler and utilities
independently of the build context.

The ref-classes-toolchain-gcc-native class defines **BUILD_CC**,
**BUILD_CXX** and other such variables which are rarely used in recipes.
Exception be made for target recipes that need to use the compiler from the build
host at some point during the build.

This class should not be inherited directly. It is inherited by the
ref-classes-base class if **TOOLCHAIN_NATIVE** is set to "gcc".

## ``typecheck``

The ref-classes-typecheck class provides support for validating the values of
variables set at the configuration level against their defined types.
The OpenEmbedded build system allows you to define the type of a
variable using the "type" varflag. Here is an example::

   IMAGE_FEATURES[type] = "list"

## ``uboot-config``

The ref-classes-uboot-config class provides support for configuring one
or more U-Boot build configurations.

There are three ways to configure the recipe for your machine:

-  Using the **UBOOT_MACHINE** variable (and its companion variable
   **UBOOT_BINARY**). For example::

      UBOOT_MACHINE = "config"
      UBOOT_BINARY = "u-boot.bin"

-  Using **UBOOT_CONFIG** variables. For example::

      UBOOT_CONFIG ??= "foo bar"
      UBOOT_CONFIG[foo] = "config"
      UBOOT_CONFIG[bar] = "config2"

      UBOOT_CONFIG_IMAGE_FSTYPES[bar] = "fstype"

      UBOOT_CONFIG_BINARY[foo] = "binary"

      UBOOT_CONFIG_MAKE_OPTS[foo] = "FOO=1"
      UBOOT_CONFIG_MAKE_OPTS[bar] = "BAR=1"

      UBOOT_CONFIG_FRAGMENTS[foo] = "foo.fragment"

   In this example, all possible configurations are selected (`foo` and
   `bar`), but it is also possible to build only `foo` or `bar` by
   changing the value of **UBOOT_CONFIG** to include either one or the
   other.  For exmaple::

      UBOOT_CONFIG = "foo"
      UBOOT_CONFIG[foo] = "config"
      UBOOT_CONFIG[bar] = "config2"

   Each build configuration is associated to a variable flag definition of
   **UBOOT_CONFIG**, and associated changes for each config as defined
   in the various UBOOT_CONFIG_* variables.

   -  `UBOOT_CONFIG[config]`: defconfig file selected for this build configuration.
      These files are found in the source tree's `configs` folder of U-Boot.

      *This option is mandatory.*

      See the documentation of **UBOOT_CONFIG** for more information.

   -  `UBOOT_CONFIG_IMAGE_FSTYPES[config]`: image types to append to the
      **IMAGE_FSTYPES** variable for image generation for this build
      configuration.

      This can allow building an extra image format that uses the binary
      generated by this build configuration.

      This option is not mandatory and can be left unspecified if not needed.

      See the documentation of **UBOOT_CONFIG_IMAGE_FSTYPES** for more information.

   -  `UBOOT_CONFIG_BINARY[config]`: binary to select as the one to deploy in
      **DEPLOY_DIR_IMAGE**. The output of a U-Boot build may be more than
      one binary, for example::

         u-boot.bin
         u-boot-with-spl.bin

      Setting the `binary` value to `u-boot-with-spl.bin` will make this
      binary the one deployed in **DEPLOY_DIR_IMAGE**. It is renamed to
      include the build configuration name in the process (`foo` or `bar` in
      the above example).

      This option defaults to **UBOOT_BINARY** if not specified.

      See the documentation of **UBOOT_CONFIG_BINARY** for more information.

   -  `UBOOT_CONFIG_MAKE_OPTS[config]`: additional options passed to
      `make` when configuring and compiling U-Boot for this configuration
      entry. The options in this entry are added before the options in
      **UBOOT_MAKE_OPTS**.

      This option is not mandatory and adds nothing if not specified.  If
      you do not have a make option for a given config, you can simply not
      set the flag for that config.

      See the documentation of **UBOOT_CONFIG_MAKE_OPTS** for more information.

   -  `UBOOT_CONFIG_FRAGMENTS[config]`: additional config fragment(s)
      from the source tree that is used during `do_configure()` to setup the
      build.  The options in this entry are added before the fragments in
      **UBOOT_FRAGMENTS**.

      This option is not mandatory and adds nothing if not specified.  If
      you do not have a fragment a given config, you can simply not set the
      flag for that config.

      See the documentation of **UBOOT_CONFIG_FRAGMENTS** for more information.

-  Or, a legacy method using the **UBOOT_CONFIG** variable by itself.
   *This method is being deprecated, see note below.*  For example::

      UBOOT_CONFIG ??= "foo bar"
      UBOOT_CONFIG[foo] = "config,images,binary"
      UBOOT_CONFIG[bar] = "config2,images2,binary2"

   In this example, all possible configurations are selected (`foo` and
   `bar`), but it is also possible to build only `foo` or `bar` by
   changing the value of **UBOOT_CONFIG** to include either one or the
   other.

   Each build configuration is associated to a variable flag definition of
   **UBOOT_CONFIG**, that can take up to three comma-separated options
   (`config,images,binary`):

   -  `config`: defconfig file selected for this build configuration.
      These files are found in the source tree's `configs` folder of U-Boot.

      *This option is mandatory.*

   -  `images`: image types to append to the **IMAGE_FSTYPES** variable
      for image generation for this build configuration.

      This can allow building an extra image format that uses the binary
      generated by this build configuration.

      This option is not mandatory and can be left empty.

   -  `binary`: binary to select as the one to deploy in
      **DEPLOY_DIR_IMAGE**. The output of a U-Boot build may be more than
      one binary, for example::

         u-boot.bin
         u-boot-with-spl.bin

      Setting the `binary` value to `u-boot-with-spl.bin` will make this
      binary the one deployed in **DEPLOY_DIR_IMAGE**. It is renamed to
      include the build configuration name in the process (`foo` or `bar` in
      the above example).

      This option defaults to **UBOOT_BINARY** if unset.

> **Note:**
> The legacy flow has been deprecated.  It is recommended to not use this
> legacy flow as any future extensions to the control knobs will be not added
> to this methodology.  This functionality will be removed after the wrynose
> LTS release.
>

Using **UBOOT_MACHINE** and **UBOOT_CONFIG** at the same time is not
possible.

See the **UBOOT_CONFIG** and **UBOOT_MACHINE** variables for additional
information.

## ``uboot-sign``

The ref-classes-uboot-sign class provides support for U-Boot verified boot.
It is intended to be inherited from U-Boot recipes.

The variables used by this class are:

-  **SPL_MKIMAGE_DTCOPTS**: DTC options for U-Boot `mkimage` when
   building the FIT image.
-  **SPL_SIGN_ENABLE**: enable signing the FIT image.
-  **SPL_SIGN_KEYDIR**: directory containing the signing keys.
-  **SPL_SIGN_KEYNAME**: base filename of the signing keys.
-  **SPL_DTB_BINARY**: Name of the SPL device tree binary. Can be set to an
   empty string to indicate that no SPL should be created and added to the FIT
   image.
-  **UBOOT_FIT_ADDRESS_CELLS**: `#address-cells` value for the FIT image.
-  **UBOOT_FIT_DESC**: description string encoded into the FIT image.
-  **UBOOT_FIT_GENERATE_KEYS**: generate the keys if they don't exist yet.
-  **UBOOT_FIT_HASH_ALG**: hash algorithm for the FIT image.
-  **UBOOT_FIT_KEY_GENRSA_ARGS**: `openssl genrsa` arguments.
-  **UBOOT_FIT_KEY_REQ_ARGS**: `openssl req` arguments.
-  **UBOOT_FIT_SIGN_ALG**: signature algorithm for the FIT image.
-  **UBOOT_FIT_SIGN_NUMBITS**: size of the private key for FIT image
   signing.
-  **UBOOT_FIT_KEY_SIGN_PKCS**: algorithm for the public key certificate
   for FIT image signing.
-  **UBOOT_FITIMAGE_ENABLE**: enable the generation of a U-Boot FIT image.
-  **UBOOT_MKIMAGE_DTCOPTS**: DTC options for U-Boot `mkimage` when
   rebuilding the FIT image containing the kernel.
-  **UBOOT_FIT_ARM_TRUSTED_FIRMWARE**: include the Trusted Firmware-A
   (TF-A) binary in the U-Boot FIT image.
-  **UBOOT_FIT_ARM_TRUSTED_FIRMWARE_IMAGE**: specifies the path to the
   Trusted Firmware-A (TF-A) binary.
-  **UBOOT_FIT_TEE**: include the Trusted Execution Environment (TEE)
   binary in the U-Boot FIT image.
-  **UBOOT_FIT_TEE_IMAGE**: specifies the path to the Trusted Execution
   Environment (TEE) binary.
-  **UBOOT_FIT_USER_SETTINGS**: adds a user-specific snippet to the U-Boot
   Image Tree Source (ITS). Users can include their custom U-Boot Image Tree
   Source (ITS) snippet in this variable.
-  **UBOOT_FIT_CONF_FIRMWARE**: adds one image to the `firmware` property
   of the configuration node.
-  **UBOOT_FIT_CONF_USER_LOADABLES**: adds one or more user-defined images
   to the `loadables` property of the configuration node.

See U-Boot's documentation for details about `verified boot
<https://docs.u-boot.org/en/latest/usage/fit/verified-boot.html>`__
and the `signature process
<https://docs.u-boot.org/en/latest/usage/fit/signature.html>`__.

See also the description of ref-classes-kernel-fit-image class, which this class
imitates.

## ``uki``

The ref-classes-uki class provides support for `Unified Kernel Image
(UKI) <https://uapi-group.org/specifications/specs/unified_kernel_image/>`__
format. UKIs combine kernel, **Initramfs**, signatures, metadata etc to a
single UEFI firmware compatible binary. The class is intended to be inherited
by rootfs image recipes. The build configuration should also use an
**Initramfs**, `systemd-boot` as boot menu provider and have UEFI support
on target hardware. Using `systemd` as init is recommended. Image builds
should create an ESP partition for UEFI firmware and copy `systemd-boot` and
UKI files there. Sample configuration for Wic images is provided in
:oe_git:`scripts/lib/wic/canned-wks/efi-uki-bootdisk.wks.in
</openembedded-core/tree/scripts/lib/wic/canned-wks/efi-uki-bootdisk.wks.in>`.
UKIs are generated using `systemd` reference implementation `ukify
<https://www.freedesktop.org/software/systemd/man/latest/ukify.html>`__.
This class uses a number of variables but tries to find sensible defaults for
them.

The variables used by this class are:

-  **EFI_ARCH**: architecture name within EFI standard, set in
   :oe_git:`meta/conf/image-uefi.conf
   </openembedded-core/tree/meta/conf/image-uefi.conf>`
-  **IMAGE_EFI_BOOT_FILES**: files to install to EFI boot partition
   created by the `bootimg_efi` Wic plugin
-  **INITRAMFS_IMAGE**: initramfs recipe name
-  **KERNEL_DEVICETREE**: optional devicetree files to embed into UKI
-  **UKIFY_CMD**: `ukify
   <https://www.freedesktop.org/software/systemd/man/latest/ukify.html>`__
   command to build the UKI image
-  **UKI_CMDLINE**: kernel command line to use with UKI
-  **UKI_CONFIG_FILE**: optional config file for `ukify
   <https://www.freedesktop.org/software/systemd/man/latest/ukify.html>`__
-  **UKI_FILENAME**: output file name for the UKI image
-  **UKI_KERNEL_FILENAME**: kernel image file name
-  **UKI_SB_CERT**: optional UEFI secureboot certificate matching the
   private key
-  **UKI_SB_KEY**: optional UEFI secureboot private key to sign UKI with

For examples on how to use this class see oeqa selftest
:oe_git:`meta/lib/oeqa/selftest/cases/uki.py
</openembedded-core/tree/meta/lib/oeqa/selftest/cases/uki.py>`.
Also an oeqa runtime test :oe_git:`meta/lib/oeqa/runtime/cases/uki.py
</openembedded-core/tree/meta/lib/oeqa/runtime/cases/uki.py>` is provided which
verifies that the target system booted the same UKI binary as was set at
buildtime via **UKI_FILENAME**.

## ``uninative``

The ref-classes-uninative class allows binaries to run on systems with
older or newer Glibc  versions. This means
ref-classes-native recipe :ref:`overview-manual/concepts:shared state
cache` can be shared among different host distributions of different versions,
i.e. the overview-manual/concepts:shared state cache is "universal".

To allow this to work, the dynamic loader is changed to our own :manpage:`ld.so
<ld.so.8>` when binaries are compiled using the
`--dynamic-linker` option. This means when the binary is executed, it finds
our own `ld.so <ld.so.8>` and that loader has a modified search path
which finds a newer Glibc version.

The linking of the binaries is not changed at link time since the
headers on the system wouldn't match the newer Glibc and this causes
obtuse failures. Changing the loader is effectively the same as if the
system had a Glibc upgrade after the binary was compiled, so it is a
mechanism supported by upstream.

One caveat to this approach is that the uninative Glibc binary must be
equal to or newer in version to the versions on all the systems using
the common overview-manual/concepts:shared state cache. This is why
ref-classes-uninative  is regularly changed on the development and stable
branches.

Another potential issue is static linking: static libraries created on
a system with a new Glibc version may have symbols not present in older
versions, which would then fail during linking on older systems. This
is one reason we don't use static linking for our ref-classes-native
binaries.

With this class enabled, a tarball containing a pre-built C library is
downloaded at the start of the build. In **OpenEmbedded-Core (OE-Core)** this is
enabled by default through :oe_git:`meta/conf/distro/include/yocto-uninative.inc
</openembedded-core/tree/meta/conf/distro/include/yocto-uninative.inc>`. Other distributions that do
not derive from **Poky** can also "`require conf/distro/include/yocto-uninative.inc`"
to use this. Alternatively if you prefer, you can build the uninative-tarball
recipe yourself, publish the resulting tarball (e.g. via HTTP) and set
**UNINATIVE_URL** and **UNINATIVE_CHECKSUM** appropriately. For an
example, see :oe_git:`meta/conf/distro/include/yocto-uninative.inc
</openembedded-core/tree/meta/conf/distro/include/yocto-uninative.inc>`.

The ref-classes-uninative class is also used unconditionally by the
extensible SDK . When building the extensible
SDK, `uninative-tarball` is built and the resulting tarball is included within
the SDK.

## ``update-alternatives``

The ref-classes-update-alternatives class helps the alternatives system when
multiple sources provide the same command. This situation occurs when
several programs that have the same or similar function are installed
with the same name. For example, the `ar` command is available from
the `busybox`, `binutils` and `elfutils` packages. The
ref-classes-update-alternatives class handles renaming the binaries so that
multiple packages can be installed without conflicts. The `ar` command
still works regardless of which packages are installed or subsequently
removed. The class renames the conflicting binary in each package and
symlinks the highest priority binary during installation or removal of
packages.

To use this class, you need to define a number of variables:

-  **ALTERNATIVE**

-  **ALTERNATIVE_LINK_NAME**

-  **ALTERNATIVE_TARGET**

-  **ALTERNATIVE_PRIORITY**

These variables list alternative commands needed by a package, provide
pathnames for links, default links for targets, and so forth. For
details on how to use this class, see the comments in the
update-alternatives.bbclass 
file.

> **Note:**
> You can use the `update-alternatives` command directly in your recipes.
> However, this class simplifies things in most cases.
>

.. _ref-classes-update-rc.d:

## ``update-rc.d``

The ref-classes-update-rc.d class uses `update-rc.d` to safely install an
initialization script on behalf of the package. The OpenEmbedded build
system takes care of details such as making sure the script is stopped
before a package is removed and started when the package is installed.

Three variables control this class: **INITSCRIPT_PACKAGES**,
**INITSCRIPT_NAME** and **INITSCRIPT_PARAMS**. See the variable links
for details.

## ``useradd*``

The useradd*  classes support the addition of users or groups for
usage by the package on the target. For example, if you have packages
that contain system services that should be run under their own user or
group, you can use these classes to enable creation of the user or
group. The :oe_git:`meta-skeleton/recipes-skeleton/useradd/useradd-example.bb
</openembedded-core/tree/meta-skeleton/recipes-skeleton/useradd/useradd-example.bb>`
recipe in **OpenEmbedded-Core (OE-Core)** provides a simple
example that shows how to add three users and groups to two packages.

The useradd_base  class provides basic functionality for user or
groups settings.

The useradd*  classes support the
**USERADD_PACKAGES**,
**USERADD_PARAM**,
**GROUPADD_PARAM**, and
**GROUPMEMS_PARAM** variables.

The useradd-staticids  class supports the addition of users or groups
that have static user identification (`uid`) and group identification
(`gid`) values.

The default behavior of the OpenEmbedded build system for assigning
`uid` and `gid` values when packages add users and groups during
package install time is to add them dynamically. This works fine for
programs that do not care what the values of the resulting users and
groups become. In these cases, the order of the installation determines
the final `uid` and `gid` values. However, if non-deterministic
`uid` and `gid` values are a problem, you can override the default,
dynamic application of these values by setting static values. When you
set static values, the OpenEmbedded build system looks in
**BBPATH** for `files/passwd` and `files/group`
files for the values.

To use static `uid` and `gid` values, you need to set some variables. See
the **USERADDEXTENSION**, **USERADD_UID_TABLES**,
**USERADD_GID_TABLES**, and **USERADD_ERROR_DYNAMIC** variables.
You can also see the ref-classes-useradd class for additional
information.

> **Note:**
> You do not use the useradd-staticids  class directly. You either enable
> or disable the class by setting the **USERADDEXTENSION** variable. If you
> enable or disable the class in a configured system, **TMPDIR** might
> contain incorrect `uid` and `gid` values. Deleting the **TMPDIR**
> directory will correct this condition.
>

## ``utility-tasks``

The ref-classes-utility-tasks class provides support for various
"utility" type tasks that are applicable to all recipes, such as
ref-tasks-clean and ref-tasks-listtasks.

This class is enabled by default because it is inherited by the
ref-classes-base class.

## ``utils``

The ref-classes-utils class provides some useful Python functions that are
typically used in inline Python expressions (e.g. `${@...}`). One
example use is for `bb.utils.contains()`.

This class is enabled by default because it is inherited by the
ref-classes-base class.

## ``vala``

The ref-classes-vala class supports recipes that need to build software written
using the Vala programming language.

## ``vex``

The ref-classes-vex class is used to generate metadata needed by external
tools to check for vulnerabilities, for example CVEs. It can be used as a
replacement for ref-classes-cve-check.

In order to use this class, inherit the class in the `local.conf` file and it
will add the `generate_vex` task for every recipe::

   INHERIT += "vex"

If an image is built it will generate a report in **DEPLOY_DIR_IMAGE** for
all the packages used, it will also generate a file for all recipes used in the
build.

Variables use the `CVE_CHECK` prefix to keep compatibility with the
ref-classes-cve-check class.

Example usage::

   bitbake -c generate_vex openssl

## ``waf``

The ref-classes-waf class supports recipes that need to build software that uses
the Waf build system. You can use the
**EXTRA_OECONF** or
**PACKAGECONFIG_CONFARGS** variables
to specify additional configuration options to be passed on the Waf
command line.

## ``yocto-check-layer``

The ref-classes-yocto-check-layer class is used by the
yocto-check-layer 
script to ensure that packages from Yocto Project Compatible layers don't skip
required QA checks listed in **CHECKLAYER_REQUIRED_TESTS** defined by the
ref-classes-insane class.

It adds an anonymous python function with extra processing to all recipes,
and globally inheriting this class with **INHERIT** is not advised. Instead
the `yocto-check-layer` script should be used as it handles usage of this
class.

For more information on the Yocto Project
Compatible layers, see the :ref:`dev-manual/layers:Making Sure Your Layer is
Compatible With Yocto Project` section of the Yocto Project Development Manual.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Tasks

Tasks are units of execution for BitBake. Recipes (`.bb` files) use
tasks to complete configuring, compiling, and packaging software. This
chapter provides a reference of the tasks defined in the OpenEmbedded
build system.

## Normal Recipe Build Tasks

The following sections describe normal tasks associated with building a
recipe. For more information on tasks and dependencies, see the
"bitbake-user-manual/bitbake-user-manual-metadata:tasks" and
"bitbake-user-manual/bitbake-user-manual-execution:dependencies" sections in the
BitBake User Manual.

### ``do_build``

The default task for all recipes. This task depends on all other normal
tasks required to build a recipe.

### ``do_compile``

Compiles the source code. This task runs with the current working
directory set to `${`\ **B**\ `}`.

The default behavior of this task is to run the `oe_runmake` function
if a makefile (`Makefile`, `makefile`, or `GNUmakefile`) is found.
If no such file is found, the ref-tasks-compile task does nothing.

### ``do_compile_ptest_base``

Compiles the runtime test suite included in the software being built.

### ``do_configure``

Configures the source by enabling and disabling any build-time and
configuration options for the software being built. The task runs with
the current working directory set to `${`\ **B**\ `}`.

The default behavior of this task is to run `oe_runmake clean` if a
makefile (`Makefile`, `makefile`, or `GNUmakefile`) is found and
**CLEANBROKEN** is not set to "1". If no such
file is found or the **CLEANBROKEN** variable is set to "1", the
ref-tasks-configure task does nothing.

### ``do_configure_ptest_base``

Configures the runtime test suite included in the software being built.

### ``do_deploy``

Writes output files that are to be deployed to
`${`\ **DEPLOY_DIR_IMAGE**\ `}`. The
task runs with the current working directory set to
`${`\ **B**\ `}`.

Recipes implementing this task should inherit the
ref-classes-deploy class and should write the output
to `${`\ **DEPLOYDIR**\ `}`, which is not to be
confused with `${DEPLOY_DIR}`. The ref-classes-deploy class sets up
ref-tasks-deploy as a shared state (sstate) task that can be accelerated
through sstate use. The sstate mechanism takes care of copying the
output from `${DEPLOYDIR}` to `${DEPLOY_DIR_IMAGE}`.

> **Note:**
> Do not write the output directly to `${DEPLOY_DIR_IMAGE}`, as this causes
> the sstate mechanism to malfunction.
>

The ref-tasks-deploy task is not added as a task by default and
consequently needs to be added manually. If you want the task to run
after ref-tasks-compile, you can add it by doing
the following::

      addtask deploy after do_compile

Adding ref-tasks-deploy after other tasks works the same way.

> **Note:**
> You do not need to add `before do_build` to the `addtask` command
> (though it is harmless), because the ref-classes-base class contains the following::
>
> do_build[recrdeptask] += "do_deploy"
>
>
> See the "bitbake-user-manual/bitbake-user-manual-execution:dependencies"
> section in the BitBake User Manual for more information.
>

If the ref-tasks-deploy task re-executes, any previous output is removed
(i.e. "cleaned").

### ``do_fetch``

Fetches the source code. This task uses the **SRC_URI** variable and the
argument's prefix to determine the correct
fetcher 
module.

### ``do_image``

Starts the image generation process. The ref-tasks-image task runs after
the OpenEmbedded build system has run the
ref-tasks-rootfs task during which packages are
identified for installation into the image and the root filesystem is
created, complete with post-processing.

The ref-tasks-image task performs pre-processing on the image through the
**IMAGE_PREPROCESS_COMMAND** and
dynamically generates supporting do_image_*  tasks as needed.

For more information on image creation, see the "overview-manual/concepts:image generation"
section in the Yocto Project Overview and Concepts Manual.

### ``do_image_complete``

Completes the image generation process. The do_image_complete  task
runs after the OpenEmbedded build system has run the
ref-tasks-image task during which image
pre-processing occurs and through dynamically generated do_image_* 
tasks the image is constructed.

The do_image_complete  task performs post-processing on the image
through the
**IMAGE_POSTPROCESS_COMMAND**.

For more information on image creation, see the
"overview-manual/concepts:image generation"
section in the Yocto Project Overview and Concepts Manual.

### ``do_install``

Copies files that are to be packaged into the holding area
`${`\ **D**\ `}`. This task runs with the current
working directory set to `${`\ **B**\ `}`, which is the
compilation directory. The ref-tasks-install task, as well as other tasks
that either directly or indirectly depend on the installed files (e.g.
ref-tasks-package, do_package_write_* , and
ref-tasks-rootfs), run under
fakeroot .

> **Note:**
> When installing files, be careful not to set the owner and group IDs
> of the installed files to unintended values. Some methods of copying
> files, notably when using the recursive `cp` command, can preserve
> the UID and/or GID of the original file, which is usually not what
> you want. The `host-user-contaminated` QA check checks for files
> that probably have the wrong ownership.
>
> Safe methods for installing files include the following:
>
> -  The `install` utility. This utility is the preferred method.
>
> -  The `cp` command with the `--no-preserve=ownership` option.
>
> -  The `tar` command with the `--no-same-owner` option. See the
> `bin_package.bbclass` file in the `meta/classes-recipe`
> subdirectory of **OpenEmbedded-Core (OE-Core)** for an example.
>

### ``do_install_ptest_base``

Copies the runtime test suite files from the compilation directory to a
holding area.

### ``do_package``

Analyzes the content of the holding area
`${`\ **D**\ `}` and splits the content into subsets
based on available packages and files. This task makes use of the
**PACKAGES** and **FILES**
variables.

The ref-tasks-package task, in conjunction with the
ref-tasks-packagedata task, also saves some
important package metadata. For additional information, see the
**PKGDESTWORK** variable and the
"overview-manual/concepts:automatically added runtime dependencies"
section in the Yocto Project Overview and Concepts Manual.

### ``do_package_qa``

Runs QA checks on packaged files. For more information on these checks,
see the /ref-manual/qa-checks document.

### ``do_package_write_deb``

Creates Debian packages (i.e. `*.deb` files) and places them in the
`${`\ **DEPLOY_DIR_DEB**\ `}` directory in
the package feeds area. For more information, see the
"overview-manual/concepts:package feeds" section in
the Yocto Project Overview and Concepts Manual.

### ``do_package_write_ipk``

Creates IPK packages (i.e. `*.ipk` files) and places them in the
`${`\ **DEPLOY_DIR_IPK**\ `}` directory in
the package feeds area. For more information, see the
"overview-manual/concepts:package feeds" section in
the Yocto Project Overview and Concepts Manual.

### ``do_package_write_rpm``

Creates RPM packages (i.e. `*.rpm` files) and places them in the
`${`\ **DEPLOY_DIR_RPM**\ `}` directory in
the package feeds area. For more information, see the
"overview-manual/concepts:package feeds" section in
the Yocto Project Overview and Concepts Manual.

### ``do_packagedata``

Saves package metadata generated by the
ref-tasks-package task in
**PKGDATA_DIR** to make it available globally.

### ``do_patch``

Locates patch files and applies them to the source code.

After fetching and unpacking source files, the build system uses the
recipe's **SRC_URI** statements
to locate and apply patch files to the source code.

> **Note:**
> The build system uses the **FILESPATH** variable to determine the
> default set of directories when searching for patches.
>

Patch files, by default, are `*.patch` and `*.diff` files created
and kept in a subdirectory of the directory holding the recipe file. For
example, consider the
bluez5 
recipe from the **OpenEmbedded-Core (OE-Core)** layer::

   meta/recipes-connectivity/bluez5

This recipe has two patch files located here::

   meta/recipes-connectivity/bluez5/bluez5

In the `bluez5` recipe, the **SRC_URI** statements point to the source
and patch files needed to build the package.

> **Note:**
> In the case for the `bluez5_5.48.bb` recipe, the **SRC_URI** statements
> are from an include file `bluez5.inc`.
>

As mentioned earlier, the build system treats files whose file types are
`.patch` and `.diff` as patch files. However, you can use the
"apply=yes" parameter with the **SRC_URI** statement to indicate any
file as a patch file::

   SRC_URI = " \
       git://path_to_repo/some_package \
       file://file;apply=yes \
       "

Conversely, if you have a file whose file type is `.patch` or `.diff`
and you want to exclude it so that the ref-tasks-patch task does not apply
it during the patch phase, you can use the "apply=no" parameter with the
**SRC_URI** statement::

   SRC_URI = " \
       git://path_to_repo/some_package \
       file://file1.patch \
       file://file2.patch;apply=no \
       "

In the previous example `file1.patch` would be applied as a patch by default
while `file2.patch` would not be applied.

You can find out more about the patching process in the
"overview-manual/concepts:patching" section in
the Yocto Project Overview and Concepts Manual and the
"dev-manual/new-recipe:patching code" section in the
Yocto Project Development Tasks Manual.

### ``do_populate_lic``

Writes license information for the recipe that is collected later when
the image is constructed.

### ``do_populate_sdk``

Creates the file and directory structure for an installable SDK. See the
"overview-manual/concepts:sdk generation"
section in the Yocto Project Overview and Concepts Manual for more
information.

### ``do_populate_sdk_ext``

Creates the file and directory structure for an installable extensible
SDK (eSDK). See the "overview-manual/concepts:sdk generation"
section in the Yocto Project Overview and Concepts Manual for more
information.

### ``do_populate_sysroot``

Stages (copies) a subset of the files installed by the
ref-tasks-install task into the appropriate
sysroot. For information on how to access these files from other
recipes, see the **STAGING_DIR* <STAGING_DIR_HOST>** variables.
Directories that would typically not be needed by other recipes at build
time (e.g. `/etc`) are not copied by default.

For information on what directories are copied by default, see the
**SYSROOT_DIRS* <SYSROOT_DIRS>** variables. You can change
these variables inside your recipe if you need to make additional (or
fewer) directories available to other recipes at build time.

The ref-tasks-populate_sysroot task is a shared state (sstate) task, which
means that the task can be accelerated through sstate use. Realize also
that if the task is re-executed, any previous output is removed (i.e.
"cleaned").

### ``do_prepare_recipe_sysroot``

Installs the files into the individual recipe specific sysroots (i.e.
`recipe-sysroot` and `recipe-sysroot-native` under
`${`\ **WORKDIR**\ `}` based upon the
dependencies specified by **DEPENDS**). See the
"ref-classes-staging" class for more information.

### ``do_recipe_qa``

Performs QA check on recipes that can operate entirely from recipe metadata and
do not need any of the build artefacts or source code.

The list of QA checks that this tasks defines are documented in
/ref-manual/qa-checks.

### ``do_rm_work``

Removes work files after the OpenEmbedded build system has finished with
them. You can learn more by looking at the
"ref-classes-rm-work" section.

### ``do_unpack``

Unpacks the source code into a working directory pointed to by
`${`\ **UNPACKDIR**\ `}`.
For more information on how source files are unpacked, see the
"overview-manual/concepts:source fetching"
section in the Yocto Project Overview and Concepts Manual.

## Manually Called Tasks

These tasks are typically manually triggered (e.g. by using the
`bitbake -c` command-line option):

### ``do_checkuri``

Validates the **SRC_URI** value.

### ``do_clean``

Removes all output files for a target from the
ref-tasks-unpack task forward (i.e. ref-tasks-unpack,
ref-tasks-configure,
ref-tasks-compile,
ref-tasks-install, and
ref-tasks-package).

You can run this task using BitBake as follows::

   $ bitbake -c clean recipe

Running this task does not remove the
sstate  cache files.
Consequently, if no changes have been made and the recipe is rebuilt
after cleaning, output files are simply restored from the sstate cache.
If you want to remove the sstate cache files for the recipe, you need to
use the ref-tasks-cleansstate task instead
(i.e. `bitbake -c cleansstate` recipe).

### ``do_cleanall``

Removes all output files, shared state
(sstate ) cache, and
downloaded source files for a target (i.e. the contents of
**DL_DIR**). Essentially, the ref-tasks-cleanall task is
identical to the ref-tasks-cleansstate task
with the added removal of downloaded source files.

You can run this task using BitBake as follows::

   $ bitbake -c cleanall recipe

You should never use the ref-tasks-cleanall task in a normal
scenario. If you want to start fresh with the ref-tasks-fetch task,
use instead::

  $ bitbake -f -c fetch recipe

> **Note:**
> The reason to prefer `bitbake -f -c fetch` is that the
> ref-tasks-cleanall task would break in some cases, such as::
>
> $ bitbake -c fetch    recipe
> $ bitbake -c cleanall recipe-native
> $ bitbake -c unpack   recipe
>
> because after step 1 there is a stamp file for the
> ref-tasks-fetch task of `recipe`, and it won't be removed at
> step 2 because step 2 uses a different work directory. So the unpack task
> at step 3 will try to extract the downloaded archive and fail as it has
> been deleted in step 2.
>
> Note that this also applies to BitBake from concurrent processes when a
> shared download directory (**DL_DIR**) is setup.
>

### ``do_cleansstate``

Removes all output files and shared state
(sstate ) cache for a
target. Essentially, the ref-tasks-cleansstate task is identical to the
ref-tasks-clean task with the added removal of
shared state (sstate )
cache.

You can run this task using BitBake as follows::

   $ bitbake -c cleansstate recipe

When you run the ref-tasks-cleansstate task, the OpenEmbedded build system
no longer uses any sstate. Consequently, building the recipe from
scratch is guaranteed.

> **Note:**
> Using ref-tasks-cleansstate with a shared **SSTATE_DIR** is
> not recommended because it could trigger an error during the build of a
> separate BitBake instance. This is because the builds check sstate "up
> front" but download the files later, so it if is deleted in the
> meantime, it will cause an error but not a total failure as it will
> rebuild it.
>
> The reliable and preferred way to force a new build is to use ``bitbake
> -f`` instead.
>

> **Note:**
> The ref-tasks-cleansstate task cannot remove sstate from a remote sstate
> mirror. If you need to build a target from scratch using remote mirrors, use
> the "-f" option as follows::
>
> $ bitbake -f -c do_cleansstate target
>
>

### ``do_pydevshell``

Starts a shell in which an interactive Python interpreter allows you to
interact with the BitBake build environment. From within this shell, you
can directly examine and set bits from the data store and execute
functions as if within the BitBake environment. See the "dev-manual/python-development-shell:using a Python development shell" section in
the Yocto Project Development Tasks Manual for more information about
using `pydevshell`.

### ``do_devshell``

Starts a shell whose environment is set up for development, debugging,
or both. See the "dev-manual/development-shell:using a development shell" section in the
Yocto Project Development Tasks Manual for more information about using
`devshell`.

### ``do_list_image_features``

Lists the available features for an image recipe. These features can be set
in the **IMAGE_FEATURES** variable.

### ``do_listtasks``

Lists all defined tasks for a target.

### ``do_package_index``

Creates or updates the index in the overview-manual/concepts:package feeds area.

> **Note:**
> This task is not triggered with the `bitbake -c` command-line option as
> are the other tasks in this section. Because this task is specifically for
> the `package-index` recipe, you run it using `bitbake package-index`.
>

## Image-Related Tasks

The following tasks are applicable to image recipes.

### ``do_bootimg``

Creates a bootable live image. See the
**IMAGE_FSTYPES** variable for additional
information on live image types.

### ``do_bundle_initramfs``

Combines an **Initramfs** image and kernel together to
form a single image.

### ``do_rootfs``

Creates the root filesystem (file and directory structure) for an image.
See the "overview-manual/concepts:image generation"
section in the Yocto Project Overview and Concepts Manual for more
information on how the root filesystem is created.

### ``do_testimage``

Boots an image and performs runtime tests within the image. For
information on automatically testing images, see the
"test-manual/runtime-testing:performing automated runtime testing"
section in the Yocto Project Test Environment Manual.

### ``do_testimage_auto``

Boots an image and performs runtime tests within the image immediately
after it has been built. This task is enabled when you set
**TESTIMAGE_AUTO** equal to "1".

For information on automatically testing images, see the
"test-manual/runtime-testing:performing automated runtime testing"
section in the Yocto Project Test Environment Manual.

## Kernel-Related Tasks

The following tasks are applicable to kernel recipes. Some of these
tasks (e.g. the ref-tasks-menuconfig task) are
also applicable to recipes that use Linux kernel style configuration
such as the BusyBox recipe.

### ``do_compile_kernelmodules``

Runs the step that builds the kernel modules (if needed). Building a
kernel consists of two steps: 1) the kernel (`vmlinux`) is built, and
2) the modules are built (i.e. `make modules`).

### ``do_diffconfig``

When invoked by the user, this task creates a file containing the
differences between the original config as produced by
ref-tasks-kernel_configme task and the
changes made by the user with other methods (i.e. using
(ref-tasks-kernel_menuconfig). Once the
file of differences is created, it can be used to create a config
fragment that only contains the differences. You can invoke this task
from the command line as follows::

   $ bitbake linux-yocto -c diffconfig

For more information, see the
"kernel-dev/common:creating configuration fragments"
section in the Yocto Project Linux Kernel Development Manual.

### ``do_kernel_checkout``

Converts the newly unpacked kernel source into a form with which the
OpenEmbedded build system can work. Because the kernel source can be
fetched in several different ways, the ref-tasks-kernel_checkout task makes
sure that subsequent tasks are given a clean working tree copy of the
kernel with the correct branches checked out.

### ``do_kernel_configcheck``

Validates the configuration produced by the
ref-tasks-kernel_menuconfig task. The
ref-tasks-kernel_configcheck task produces warnings when a requested
configuration does not appear in the final `.config` file or when you
override a policy configuration in a hardware configuration fragment.
You can run this task explicitly and view the output by using the
following command::

   $ bitbake linux-yocto -c kernel_configcheck -f

For more information, see the
"kernel-dev/common:validating configuration"
section in the Yocto Project Linux Kernel Development Manual.

### ``do_kernel_configme``

After the kernel is patched by the ref-tasks-patch
task, the ref-tasks-kernel_configme task assembles and merges all the
kernel config fragments into a merged configuration that can then be
passed to the kernel configuration phase proper. This is also the time
during which user-specified defconfigs are applied if present, and where
configuration modes such as `--allnoconfig` are applied.

### ``do_kernel_menuconfig``

Invoked by the user to manipulate the `.config` file used to build a
linux-yocto recipe. This task starts the Linux kernel configuration
tool, which you then use to modify the kernel configuration.

> **Note:**
> You can also invoke this tool from the command line as follows::
>
> $ bitbake linux-yocto -c menuconfig
>
>

See the "kernel-dev/common:using `menuconfig```"
section in the Yocto Project Linux Kernel Development Manual for more
information on this configuration tool.

### ``do_kernel_metadata``

Collects all the features required for a given kernel build, whether the
features come from **SRC_URI** or from Git
repositories. After collection, the ref-tasks-kernel_metadata task
processes the features into a series of config fragments and patches,
which can then be applied by subsequent tasks such as
ref-tasks-patch and
ref-tasks-kernel_configme.

### ``do_menuconfig``

Runs `make menuconfig` for the kernel. For information on
`menuconfig`, see the
"kernel-dev/common:using `menuconfig```"
section in the Yocto Project Linux Kernel Development Manual.

### ``do_savedefconfig``

When invoked by the user, creates a defconfig file that can be used
instead of the default defconfig. The saved defconfig contains the
differences between the default defconfig and the changes made by the
user using other methods (i.e. the
ref-tasks-kernel_menuconfig task. You
can invoke the task using the following command::

   $ bitbake linux-yocto -c savedefconfig

### ``do_shared_workdir``

After the kernel has been compiled but before the kernel modules have
been compiled, this task copies files required for module builds and
which are generated from the kernel build into the shared work
directory. With these copies successfully copied, the
ref-tasks-compile_kernelmodules task
can successfully build the kernel modules in the next step of the build.

### ``do_sizecheck``

After the kernel has been built, this task checks the size of the
stripped kernel image against
**KERNEL_IMAGE_MAXSIZE**. If that
variable was set and the size of the stripped kernel exceeds that size,
the kernel build produces a warning to that effect.

### ``do_strip``

If `KERNEL_IMAGE_STRIP_EXTRA_SECTIONS` is defined, this task strips
the sections named in that variable from `vmlinux`. This stripping is
typically used to remove nonessential sections such as `.comment`
sections from a size-sensitive configuration.

### ``do_validate_branches``

After the kernel is unpacked but before it is patched, this task makes
sure that the machine and metadata branches as specified by the
**SRCREV** variables actually exist on the specified
branches. Otherwise, if **AUTOREV** is not being used, the
ref-tasks-validate_branches task fails during the build.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Using Configuration Fragments

**Configuration Fragments <Configuration Fragment>** define top level build
configuration features that can be independently enabled and disabled using
standard tooling. Such features are made of one or several build configuration
statements that are either contained in a fragment file, or are set indirectly
using the **Built-in Fragment** mechanism.

This document provides a quick reference of the :oe_git:`bitbake-config-build
</bitbake/tree/bin/bitbake-config-build>` tool and lists the
**Configuration Fragments <Configuration Fragment>** and :term:`Built-in
Fragments <Built-in Fragment>` available in the :term:`OpenEmbedded Build
System` core repositories.

> **Note:**
> For details on how to define new fragments in your build, see the
> /dev-manual/creating-fragments section of the Yocto Project Development
> Tasks Manual.
>

## ``bitbake-config-build`` Quick Reference

**Configuration Fragments <Configuration Fragment>** are managed with the
bitbake-config-build 
command-line tool, which is available after :ref:`dev-manual/start:Initializing
the Build Environment`.

The `bitbake-config-build` command-line tool uses sub-commands to manage
fragments, which are detailed in the sections below. For each sub-command, the
`--help` flag can be passed to get more information on the sub-command.

### ``bitbake-config-build list-fragments``

The ref-bitbake-config-build-list-fragments command will list the :term:`Built-in
Fragments <Built-in Fragment>` and :term:`Configuration Fragments <Configuration
Fragment>` that are currently available, and will also print which fragments are
enabled or disabled.

### ``bitbake-config-build show-fragment``

The ref-bitbake-config-build-show-fragment command is used to show the
location and value of a fragment. For example, running ``bitbake-config-build
show-fragment core/yocto/sstate-mirror-cdn`` will show the content of the
ref-fragments-core-yocto-sstate-mirror-cdn fragment.

### ``bitbake-config-build enable-fragment``

The ref-bitbake-config-build-enable-fragment command is used to enable a
fragment. When a fragment is enabled, the configuration variables of this
fragment are parsed by **BitBake** and their values are available globally
in your build.

From the list obtained with the ref-bitbake-config-build-list-fragments
command, you can determine which fragments can be enabled for your build.

For example, the following command would enable the
ref-fragments-core-yocto-sstate-mirror-cdn fragment::

   bitbake-config-build enable-fragment core/yocto/sstate-mirror-cdn

> **Note:**
> Multiple fragments can be enabled at once with the same command::
>
> bitbake-config-build enable-fragment <fragment1> <fragment2> ...
>

**Built-in fragments <Built-in Fragment>** are enabled the same way, and
their values are defined from the command-line directly. For example, the
following command sets the `qemuarm64` **MACHINE** through the
ref-fragments-builtin-core-machine fragment::

   bitbake-config-build enable-fragment machine/qemuarm64

This fragment can be overridden from the command-line by setting it to another
value, for example::

   bitbake-config-build enable-fragment machine/qemux86-64

In the above example, the new value of **MACHINE** is now equal to
`qemux86-64`.

When a fragment is enabled with ref-bitbake-config-build-enable-fragment,
its name is automatically appended to the **OE_FRAGMENTS** variable in
structure-build-conf-toolcfg.conf.

> **Note:**
> It is also possible to manually remove or add fragments by modifying the
> **OE_FRAGMENTS** variable in a configuration file such as
> structure-build-conf-local.conf.
>

### ``bitbake-config-build disable-fragment``

Any fragment enabled with the ref-bitbake-config-build-enable-fragment
command can be disabled with the ref-bitbake-config-build-disable-fragment
command. The list of enabled fragments can be obtained with
ref-bitbake-config-build-list-fragments.

For example, the following command disables the
ref-fragments-core-yocto-sstate-mirror-cdn fragment::

   bitbake-config-build disable-fragment core/yocto/sstate-mirror-cdn

Likewise, **Built-in Fragments <Built-in Fragment>** are disabled the
same way. For example, this would disable the `machine/qemuarm64` fragment::

   bitbake-config-build disable-fragment machine/qemuarm64

> **Note:**
> Multiple fragments can be disabled at once with the same command::
>
> bitbake-config-build disable-fragment <fragment1> <fragment2>
>

### ``bitbake-config-build disable-all-fragments``

The ref-bitbake-config-build-disable-all-fragments command disables all of the
currently enabled fragments.  The list of enabled fragments can be obtained with
ref-bitbake-config-build-list-fragments.

This command is run without arguments::

   bitbake-config-build disable-all-fragments

## Core Fragments

### Core Built-in Fragments

**Built-in Fragments <Built-in Fragment>** are used to assign a single
variable globally. The **OpenEmbedded Build System** defines multiple
built-in fragments that are detailed in this section.

#### ``machine/``

The `machine/` **built-in fragment** can be used to assign the value of
the **MACHINE** variable globally.

#### ``distro/``

The `distro/` **built-in fragment** can be used to assign the value of
the **DISTRO** variable globally.

### Core Configuration Fragments

#### Yocto Project Fragments

This group defines fragments related to the Yocto Project infrastructure in
general.

##### ``core/yocto/sstate-mirror-cdn``

The `core/yocto/sstate-mirror-cdn` **configuration fragment** can be used
to set up **BB_HASHSERVE_UPSTREAM** and **SSTATE_MIRRORS** to use
pre-built :ref:`shared state cache <overview-manual/concepts:shared state
cache>` artifacts for standard Yocto build configurations.

This will mean the build will query the Yocto Project mirrors to check for
artifacts at the start of builds, which does slow it down initially but it will
then speed up the builds by not having to build things if they are present in
the cache. It assumes you can download something faster than you can build it
which will depend on your network configuration.

##### ``core/yocto/root-login-with-empty-password``

The `core/yocto/root-login-with-empty-password` **configuration fragment**
can be used to allow to login as the `root` user to login without a password
on the serial console and over SSH.

#### Yocto Project Autobuilder Fragments

This group defines fragment used for the Yocto Project Autobuilder. For details,
see the test-manual/intro:Yocto Project Autobuilder Overview section of
the Yocto Project Test Environment Manual.

##### ``core/yocto-autobuilder/autobuilder``

The `core/yocto-autobuilder/autobuilder` fragment defines common variables
used in builds started by the Yocto Project Autobuilder.

##### ``core/yocto-autobuilder/autobuilder-resource-constraints``

The `core/yocto-autobuilder/autobuilder` fragment defines variables for
limiting the resources used by the Yocto Project Autobuilder during builds. For
more details on how to limit resources, see the /dev-manual/limiting-resources
section of the Yocto Project Development Tasks Manual.

##### ``core/yocto-autobuilder/multilib-mips64-n32``

The `core/yocto-autobuilder/multilib-mips64-n32` fragment enables
tri-architecture :ref:`multilib <dev-manual/libraries:Combining Multiple
Versions of Library Files into One Image>` configurations for :wikipedia:`MIPS64
<MIPS_architecture>` machines, which includes `mips64-n32`, `mips64`, and
`mips32r2`.

##### ``core/yocto-autobuilder/multilib-x86-lib32``

The `core/yocto-autobuilder/multilib-x86-lib32` fragment enables
:ref:`multilib <dev-manual/libraries:Combining Multiple Versions of Library
Files into One Image>` configurations for supporting 32-bit libraries on 64-bit
X86  builds.

##### ``core/yocto-autobuilder/multilib-x86-lib64``

The `core/yocto-autobuilder/multilib-x86-lib64` fragment enables
:ref:`multilib <dev-manual/libraries:Combining Multiple Versions of Library
Files into One Image>` configurations for supporting 64-bit libraries on 32-bit
X86  builds.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# ``devtool`` Quick Reference

The `devtool` command-line tool provides a number of features that
help you build, test, and package software. This command is available
alongside the `bitbake` command. Additionally, the `devtool` command
is a key part of the extensible SDK.

This chapter provides a Quick Reference for the `devtool` command. For
more information on how to apply the command when using the extensible
SDK, see the "/sdk-manual/extensible" chapter in the Yocto
Project Application Development and the Extensible Software Development
Kit (eSDK) manual.

## Getting Help

The `devtool` command line is organized similarly to Git in that it
has a number of sub-commands for each function. You can run
`devtool --help` to see all the commands::

   $ devtool --help
   NOTE: Starting bitbake server...
   usage: devtool [--basepath BASEPATH] [--bbpath BBPATH] [-d] [-q] [--color COLOR] [-h] <subcommand> ...

   OpenEmbedded development tool

   options:
     --basepath BASEPATH   Base directory of SDK / build directory
     --bbpath BBPATH       Explicitly specify the BBPATH, rather than getting it from the metadata
     -d, --debug           Enable debug output
     -q, --quiet           Print only errors
     --color COLOR         Colorize output (where COLOR is auto, always, never)
     -h, --help            show this help message and exit

   subcommands:
     Beginning work on a recipe:
       add                   Add a new recipe
       modify                Modify the source for an existing recipe
       upgrade               Upgrade an existing recipe
     Getting information:
       status                Show workspace status
       latest-version        Report the latest version of an existing recipe
       check-upgrade-status  Report upgradability for multiple (or all) recipes
       search                Search available recipes
     Working on a recipe in the workspace:
       build                 Build a recipe
       ide-sdk               Setup the SDK and configure the IDE
       rename                Rename a recipe file in the workspace
       edit-recipe           Edit a recipe file
       find-recipe           Find a recipe file
       configure-help        Get help on configure script options
       update-recipe         Apply changes from external source tree to recipe
       reset                 Remove a recipe from your workspace
       finish                Finish working on a recipe in your workspace
     Testing changes on target:
       deploy-target         Deploy recipe output files to live target machine
       undeploy-target       Undeploy recipe output files in live target machine
       build-image           Build image including workspace recipe packages
     Advanced:
       create-workspace      Set up workspace in an alternative location
       import                Import exported tar archive into workspace
       export                Export workspace into a tar archive
       extract               Extract the source for an existing recipe
       sync                  Synchronize the source tree for an existing recipe
       menuconfig            Alter build-time configuration for a recipe
   Use devtool <subcommand> --help to get help on a specific command

As directed in the general help output, you can
get more syntax on a specific command by providing the command name and
using `--help`::

   $ devtool add --help
   NOTE: Starting bitbake server...
   usage: devtool add [-h] [--same-dir | --no-same-dir] [--fetch URI] [--npm-dev] [--no-pypi] [--version VERSION] [--no-git] [--srcrev SRCREV | --autorev]
                      [--srcbranch SRCBRANCH] [--binary] [--also-native] [--src-subdir SUBDIR] [--mirrors] [--provides PROVIDES]
                      [recipename] [srctree] [fetchuri]

   Adds a new recipe to the workspace to build a specified source tree. Can optionally fetch a remote URI and unpack it to create the source tree.

   arguments:
     recipename            Name for new recipe to add (just name - no version, path or extension). If not specified, will attempt to auto-detect it.
     srctree               Path to external source tree. If not specified, a subdirectory of /media/build1/poky/build/workspace/sources will be used.
     fetchuri              Fetch the specified URI and extract it to create the source tree

   options:
     -h, --help            show this help message and exit
     --same-dir, -s        Build in same directory as source
     --no-same-dir         Force build in a separate build directory
     --fetch URI, -f URI   Fetch the specified URI and extract it to create the source tree (deprecated - pass as positional argument instead)
     --npm-dev             For npm, also fetch devDependencies
     --no-pypi             Do not inherit pypi class
     --version VERSION, -V VERSION
                           Version to use within recipe (PV)
     --no-git, -g          If fetching source, do not set up source tree as a git repository
     --srcrev SRCREV, -S SRCREV
                           Source revision to fetch if fetching from an SCM such as git (default latest)
     --autorev, -a         When fetching from a git repository, set SRCREV in the recipe to a floating revision instead of fixed
     --srcbranch SRCBRANCH, -B SRCBRANCH
                           Branch in source repository if fetching from an SCM such as git (default master)
     --binary, -b          Treat the source tree as something that should be installed verbatim (no compilation, same directory structure). Useful with binary packages e.g. RPMs.
     --also-native         Also add native variant (i.e. support building recipe for the build host as well as the target machine)
     --src-subdir SUBDIR   Specify subdirectory within source tree to use
     --mirrors             Enable PREMIRRORS and MIRRORS for source tree fetching (disable by default).
     --provides PROVIDES, -p PROVIDES
                           Specify an alias for the item provided by the recipe. E.g. virtual/libgl

## The Workspace Layer Structure

`devtool` uses a "Workspace" layer in which to accomplish builds. This
layer is not specific to any single `devtool` command but is rather a
common working area used across the tool.

The following figure shows the workspace structure:

![build-workspace-directory.png](figures/build-workspace-directory.png)

```none
attic - A directory created if devtool believes it must preserve
        anything when you run "devtool reset".  For example, if you
        run "devtool add", make changes to the recipe, and then
        run "devtool reset", devtool takes notice that the file has
        been changed and moves it into the attic should you still
        want the recipe.

README - Provides information on what is in workspace layer and how to
         manage it.

.devtool_md5 - A checksum file used by devtool.

appends - A directory that contains *.bbappend files, which point to
          external source.

conf - A configuration directory that contains the layer.conf file.

recipes - A directory containing recipes.  This directory contains a
          folder for each directory added whose name matches that of the
          added recipe.  devtool places the recipe.bb file
          within that sub-directory.

sources - A directory containing a working copy of the source files used
          when building the recipe.  This is the default directory used
          as the location of the source tree when you do not provide a
          source tree path.  This directory contains a folder for each
          set of source files matched to a corresponding recipe.
```

## Adding a New Recipe to the Workspace Layer

Use the `devtool add` command to add a new recipe to the workspace
layer. The recipe you add should not exist --- `devtool` creates it for
you. The source files the recipe uses should exist in an external area.

The following example creates and adds a new recipe named `jackson` to
a workspace layer the tool creates. The source code built by the recipes
resides in `/home/user/sources/jackson`::

   $ devtool add jackson /home/user/sources/jackson

If you add a recipe and the workspace layer does not exist, the command
creates the layer and populates it as described in
"devtool-the-workspace-layer-structure" section.

Running `devtool add` when the workspace layer exists causes the tool
to add the recipe, append files, and source files into the existing
workspace layer. The `.bbappend` file is created to point to the
external source tree.

> **Note:**
> If your recipe has runtime dependencies defined, you must be sure
> that these packages exist on the target hardware before attempting to
> run your application. If dependent packages (e.g. libraries) do not
> exist on the target, your application, when run, will fail to find
> those functions. For more information, see the
> "ref-manual/devtool-reference:deploying your software on the target machine"
> section.
>

By default, `devtool add` uses the latest revision (i.e. master) when
unpacking files from a remote URI. In some cases, you might want to
specify a source revision by branch, tag, or commit hash. You can
specify these options when using the `devtool add` command:

-  To specify a source branch, use the `--srcbranch` option::

      $ devtool add --srcbranch &DISTRO_NAME_NO_CAP; jackson /home/user/sources/jackson

   In the previous example, you are checking out the &DISTRO_NAME_NO_CAP;
   branch.

-  To specify a specific tag or commit hash, use the `--srcrev`
   option::

      $ devtool add --srcrev &DISTRO_REL_TAG; jackson /home/user/sources/jackson
      $ devtool add --srcrev some_commit_hash /home/user/sources/jackson

   The previous examples check out the
   &DISTRO_REL_TAG; tag and the commit associated with the
   some_commit_hash hash.

> **Note:**
> If you prefer to use the latest revision every time the recipe is
> built, use the options `--autorev` or `-a`.
>

## Extracting the Source for an Existing Recipe

Use the `devtool extract` command to extract the source for an
existing recipe. When you use this command, you must supply the root
name of the recipe (i.e. no version, paths, or extensions), and you must
supply the directory to which you want the source extracted.

Additional command options let you control the name of a development
branch into which you can checkout the source and whether or not to keep
a temporary directory, which is useful for debugging.

## Synchronizing a Recipe's Extracted Source Tree

Use the `devtool sync` command to synchronize a previously extracted
source tree for an existing recipe. When you use this command, you must
supply the root name of the recipe (i.e. no version, paths, or
extensions), and you must supply the directory to which you want the
source extracted.

Additional command options let you control the name of a development
branch into which you can checkout the source and whether or not to keep
a temporary directory, which is useful for debugging.

## Modifying an Existing Recipe

Use the `devtool modify` command to begin modifying the source of an
existing recipe. This command is very similar to the
add  command
except that it does not physically create the recipe in the workspace
layer because the recipe already exists in an another layer.

The `devtool modify` command extracts the source for a recipe, sets it
up as a Git repository if the source had not already been fetched from
Git, checks out a branch for development, and applies any patches from
the recipe as commits on top. You can use the following command to
checkout the source files::

   $ devtool modify recipe

Using the above command form, `devtool` uses the existing recipe's
**SRC_URI** statement to locate the upstream source,
extracts the source into the default sources location in the workspace.
The default development branch used is "devtool".

## Edit an Existing Recipe

Use the `devtool edit-recipe` command to run the default editor, which
is identified using the `EDITOR` variable, on the specified recipe.

When you use the `devtool edit-recipe` command, you must supply the
root name of the recipe (i.e. no version, paths, or extensions). Also,
the recipe file itself must reside in the workspace as a result of the
`devtool add` or `devtool upgrade` commands.

## Updating a Recipe

Use the `devtool update-recipe` command to update your recipe with
patches that reflect changes you make to the source files. For example,
if you know you are going to work on some code, you could first use the
devtool modify  command to extract
the code and set up the workspace. After which, you could modify,
compile, and test the code.

When you are satisfied with the results and you have committed your
changes to the Git repository, you can then run the
`devtool update-recipe` to create the patches and update the recipe::

   $ devtool update-recipe recipe

If you run the `devtool update-recipe`
without committing your changes, the command ignores the changes.

Often, you might want to apply customizations made to your software in
your own layer rather than apply them to the original recipe. If so, you
can use the `-a` or `--append` option with the
`devtool update-recipe` command. These options allow you to specify
the layer into which to write an append file::

   $ devtool update-recipe recipe -a base-layer-directory

The `*.bbappend` file is created at the
appropriate path within the specified layer directory, which may or may
not be in your `bblayers.conf` file. If an append file already exists,
the command updates it appropriately.

## Checking on the Upgrade Status of a Recipe

Upstream recipes change over time. Consequently, you might find that you
need to determine if you can upgrade a recipe to a newer version.

To check on the upgrade status of a recipe, you can use the
`devtool latest-version recipe` command, which quickly shows the current
version and the latest version available upstream. To get a more global
picture, use the `devtool check-upgrade-status` command, which takes a
list of recipes as input, or no arguments, in which case it checks all
available recipes. This command will only print the recipes for which
a new upstream version is available. Each such recipe will have its current
version and latest upstream version, as well as the email of the maintainer
and any additional information such as the commit hash or reason for not
being able to upgrade it, displayed in a table.

This upgrade checking mechanism relies on the optional **UPSTREAM_CHECK_URI**,
**UPSTREAM_CHECK_REGEX**, **UPSTREAM_CHECK_GITTAGREGEX**,
**UPSTREAM_CHECK_COMMITS** and **UPSTREAM_VERSION_UNKNOWN**
variables in package recipes.

> **Note:**
> -  Most of the time, the above variables are unnecessary. They are only
> required when upstream does something unusual, and default
> mechanisms cannot find the new upstream versions.
>
> -  For the `oe-core` layer, recipe maintainers come from the
> maintainers.inc 
> file.
>
> -  If the recipe is using the bitbake-user-manual/bitbake-user-manual-fetching:git fetcher (\\`git://\`\`)`
> rather than a tarball, the commit hash points to the commit that matches
> the recipe's latest version tag, or in the absence of suitable tags,
> to the latest commit (when **UPSTREAM_CHECK_COMMITS** set to `1`
> in the recipe).
>

As with all `devtool` commands, you can get help on the individual
command::

   $ devtool check-upgrade-status -h
   NOTE: Starting bitbake server...
   usage: devtool check-upgrade-status [-h] [--all] [recipe [recipe ...]]

   Prints a table of recipes together with versions currently provided by recipes, and latest upstream versions, when there is a later version available

   arguments:
     recipe      Name of the recipe to report (omit to report upgrade info for all recipes)

   options:
     -h, --help  show this help message and exit
     --all, -a   Show all recipes, not just recipes needing upgrade

Unless you provide a specific recipe name on the command line, the
command checks all recipes in all configured layers.

Here is a partial example table that reports on all the recipes::

   $ devtool check-upgrade-status
   ...
   INFO: bind                      9.16.20         9.16.21         Armin Kuster <akuster808@gmail.com>
   INFO: inetutils                 2.1             2.2             Tom Rini <trini@konsulko.com>
   INFO: iproute2                  5.13.0          5.14.0          Changhyeok Bae <changhyeok.bae@gmail.com>
   INFO: openssl                   1.1.1l          3.0.0           Alexander Kanavin <alex.kanavin@gmail.com>
   INFO: base-passwd               3.5.29          3.5.51          Anuj Mittal <anuj.mittal@intel.com>  cannot be updated due to: Version 3.5.38 requires cdebconf for update-passwd utility
   ...

Notice the reported reason for not upgrading the `base-passwd` recipe.
In this example, while a new version is available upstream, you do not
want to use it because the dependency on `cdebconf` is not easily
satisfied. Maintainers can explicit the reason that is shown by adding
the **RECIPE_NO_UPDATE_REASON** variable to the corresponding recipe.
See the base-passwd_*.bb 
recipe for an example::

   RECIPE_NO_UPDATE_REASON = "Version 3.5.38 requires cdebconf for update-passwd utility"

Last but not least, you may set **UPSTREAM_VERSION_UNKNOWN** to `1`
in a recipe when there's currently no way to determine its latest upstream
version.

## Upgrading a Recipe

As software matures, upstream recipes are upgraded to newer versions. As
a developer, you need to keep your local recipes up-to-date with the
upstream version releases. There are several ways of upgrading recipes.
You can read about them in the "dev-manual/upgrading-recipes:upgrading recipes"
section of the Yocto Project Development Tasks Manual. This section
overviews the `devtool upgrade` command.

Before you upgrade a recipe, you can check on its upgrade status. See
the "devtool-checking-on-the-upgrade-status-of-a-recipe" section
for more information.

The `devtool upgrade` command upgrades an existing recipe to a more
recent version of the recipe upstream. The command puts the upgraded
recipe file along with any associated files into a "workspace" and, if
necessary, extracts the source tree to a specified location. During the
upgrade, patches associated with the recipe are rebased or added as
needed.

When you use the `devtool upgrade` command, you must supply the root
name of the recipe (i.e. no version, paths, or extensions), and you must
supply the directory to which you want the source extracted. Additional
command options let you control things such as the version number to
which you want to upgrade (i.e. the **PV**), the source
revision to which you want to upgrade (i.e. the
**SRCREV**), whether or not to apply patches, and so
forth.

You can read more on the `devtool upgrade` workflow in the
"dev-manual/devtool:use \\`devtool upgrade\`\` to create a version of the recipe that supports a newer version of the software`"
section in the Yocto Project Application Development and the Extensible
Software Development Kit (eSDK) manual. You can also see an example of
how to use `devtool upgrade` in the "dev-manual/upgrading-recipes:using `devtool upgrade```"
section in the Yocto Project Development Tasks Manual.

## Resetting a Recipe

Use the `devtool reset` command to remove a recipe and its
configuration (e.g. the corresponding `.bbappend` file) from the
workspace layer. Realize that this command deletes the recipe and the
append file. The command does not physically move them for you.
Consequently, you must be sure to physically relocate your updated
recipe and the append file outside of the workspace layer before running
the `devtool reset` command.

If the `devtool reset` command detects that the recipe or the append
files have been modified, the command preserves the modified files in a
separate "attic" subdirectory under the workspace layer.

Here is an example that resets the workspace directory that contains the
`mtr` recipe::

   $ devtool reset mtr
   NOTE: Cleaning sysroot for recipe mtr...
   NOTE: Leaving source tree /home/scottrif/bitbake-builds/build/workspace/sources/mtr as-is; if you no longer need it then please delete it manually
   $

## Finish Working on a Recipe

Use the `devtool finish` command to push any committed changes to the
specified recipe in the specified layer and remove it from your workspace.

This is roughly equivalent to the `devtool update-recipe` command followed by
the `devtool reset` command. The changes must have been committed to the git
repository created by `devtool`. Here is an example::

  $ devtool finish recipe /path/to/custom/layer

## Building Your Recipe

Use the `devtool build` command to build your recipe. The
`devtool build` command is equivalent to the
`bitbake -c populate_sysroot` command.

When you use the `devtool build` command, you must supply the root
name of the recipe (i.e. do not provide versions, paths, or extensions).
You can use either the `-s` or the `--disable-parallel-make` options to
disable parallel makes during the build. Here is an example::

   $ devtool build recipe

## Building Your Image

Use the `devtool build-image` command to build an image, extending it
to include packages from recipes in the workspace. Using this command is
useful when you want an image that ready for immediate deployment onto a
device for testing. For proper integration into a final image, you need
to edit your custom image recipe appropriately.

When you use the `devtool build-image` command, you must supply the
name of the image. This command has no command line options::

   $ devtool build-image image

## Deploying Your Software on the Target Machine

Use the `devtool deploy-target` command to deploy the recipe's build
output to the live target machine::

   $ devtool deploy-target recipe target

The target is the address of the target machine, which must be running
an SSH server (i.e. `user@hostname[:destdir]`).

This command deploys all files installed during the
ref-tasks-install task. Furthermore, you do not
need to have package management enabled within the target machine. If
you do, the package manager is bypassed.

> **Note:**
> The `deploy-target` functionality is for development only. You
> should never use it to update an image that will be used in
> production.
>

Some conditions could prevent a deployed application from
behaving as expected. When both of the following conditions are met, your
application has the potential to not behave correctly when run on the
target:

-  You are deploying a new application to the target and the recipe you
   used to build the application had correctly defined runtime
   dependencies.

-  The target does not physically have the packages on which the
   application depends installed.

If both of these conditions are met, your application will not behave as
expected. The reason for this misbehavior is because the
`devtool deploy-target` command does not deploy the packages (e.g.
libraries) on which your new application depends. The assumption is that
the packages are already on the target. Consequently, when a runtime
call is made in the application for a dependent function (e.g. a library
call), the function cannot be found.

> **Warning:**
> Runtime dependencies can be explicitly listed in the **RDEPENDS**
> variable, but may also be the result of a **DEPENDS** assignment in your
> application's recipe. This is usually the case when your application depends
> on libraries for compilation: these libraries are listed as build-time
> dependencies in the **DEPENDS** variable in your application's recipe.
> However these may also be runtime dependencies if they install shared objects
> on which your application will dynamically link to at runtime (e.g. shared
> libraries ending with `.so`).
>
> These runtime dependencies are automatically resolved by the
> **OpenEmbedded Build System** during the packaging phase. Since
> `devtool` ignores packaging dependencies, they will not be installed
> automatically with `devtool deploy-target`.
>
> For more information on how the **OpenEmbedded Build System** handles
> packaging, see the :ref:`overview-manual/concepts:Automatically Added Runtime
> Dependencies` section of the Yocto Project Overview and Concepts Manual.
>

To be sure you have all the dependencies local to the target, you need
to be sure that the packages are pre-deployed (installed) on the target
before attempting to run your application.

## Removing Your Software from the Target Machine

Use the `devtool undeploy-target` command to remove deployed build
output from the target machine. For the `devtool undeploy-target`
command to work, you must have previously used the
"devtool deploy-target "
command::

   $ devtool undeploy-target recipe target

The target is the
address of the target machine, which must be running an SSH server (i.e.
`user@hostname`).

## Creating the Workspace Layer in an Alternative Location

Use the `devtool create-workspace` command to create a new workspace
layer in your **Build Directory**. When you create a
new workspace layer, it is populated with the `README` file and the
`conf` directory only.

The following example creates a new workspace layer in your current
working and by default names the workspace layer "workspace"::

   $ devtool create-workspace

You can create a workspace layer anywhere by supplying a pathname with
the command. The following command creates a new workspace layer named
"new-workspace"::

   $ devtool create-workspace /home/scottrif/new-workspace

## Get the Status of the Recipes in Your Workspace

Use the `devtool status` command to list the recipes currently in your
workspace. Information includes the paths to their respective external
source trees.

The `devtool status` command has no command-line options::

   $ devtool status

Here is sample output after using
devtool add 
to create and add the `mtr_0.86.bb` recipe to the `workspace` directory::

   $ devtool status
   mtr:/home/scottrif/bitbake-builds/build/workspace/sources/mtr (/home/scottrif/bitbake-builds/build/workspace/recipes/mtr/mtr_0.86.bb)
   $

## Search for Available Target Recipes

Use the `devtool search` command to search for available target
recipes. The command matches the recipe name, package name, description,
and installed files. The command displays the recipe name as a result of
a match.

When you use the `devtool search` command, you must supply a keyword.
The command uses the keyword when searching for a match.

Alternatively, the `devtool find-recipe` command can be used to search for
recipe files instead of recipe names. Likewise, you must supply a keyword.

## Get Information on Recipe Configuration Scripts

Use the `devtool configure-help` command to get help on the configuration
script options for a given recipe. You must supply the recipe name to the
command. For example, it shows the output of `./configure --help` for
autotools -based recipes.

The `configure-help` command will also display the configuration options
currently in use, including the ones passed through the **EXTRA_OECONF**
variable.

## Generate an IDE Configuration for a Recipe

The `devtool ide-sdk` automatically creates an IDE configuration and SDK to
work on a given recipe. Depending on the `--mode` parameter, different types
of SDKs are generated:

-  `modified` mode: this creates an SDK and generates an IDE configuration in
   the workspace directory.

-  `shared` mode: this creates a cross-compiling toolchain and the
   corresponding shared sysroot directories of the supplied recipe(s).

The `--target` option can be used to specify a `username@hostname` string
and create a remote debugging configuration for the recipe. Similarly to
`devtool deploy-target`, it requires an SSH server running on the target.

For further details on the `devtool ide-sdk` command, see the
"/sdk-manual/extensible" chapter in the Yocto Project Application
Development and the Extensible Software Development Kit (eSDK) manual.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# OpenEmbedded Kickstart (``.wks``) Reference

## Introduction

The current Wic implementation supports only the basic kickstart
partitioning commands: `partition` (or `part` for short) and
`bootloader`.

> **Note:**
> Future updates will implement more commands and options. If you use
> anything that is not specifically supported, results can be
> unpredictable.
>

This chapter provides a reference on the available kickstart commands.
The information lists the commands, their syntax, and meanings.
Kickstart commands are based on the Fedora kickstart versions but with
modifications to reflect Wic capabilities. You can see the original
documentation for those commands at the following link:
https://pykickstart.readthedocs.io/en/latest/kickstart-docs.html

## Command: part or partition

Either of these commands creates a partition on the system and uses the
following syntax::

   part [mntpoint]
   partition [mntpoint]

If you do not
provide mntpoint, Wic creates a partition but does not mount it.

The `mntpoint` is where the partition is mounted and must be in one of
the following forms:

-  `/path`: For example, "/", "/usr", or "/home"

-  `swap`: The created partition is used as swap space

Specifying a mntpoint causes the partition to automatically be mounted.
Wic achieves this by adding entries to the filesystem table (fstab)
during image generation. In order for Wic to generate a valid fstab, you
must also provide one of the `--ondrive`, `--ondisk`, or
`--use-uuid` partition options as part of the command.

> **Note:**
> The mount program must understand the PARTUUID syntax you use with
> `--use-uuid` and non-root *mountpoint*, including swap. The default
> configuration of BusyBox in OpenEmbedded supports this, but this may
> be disabled in custom configurations.
>

Here is an example that uses "/" as the mountpoint. The command uses
`--ondisk` to force the partition onto the `sdb` disk::

      part / --source rootfs --ondisk sdb --fstype=ext3 --label platform --align 1024

Here is a list that describes other supported options you can use with
the `part` and `partition` commands:

-  `--size`: The minimum partition size. Specify as an integer value
   optionally followed by one of the units "k" / "K" for kibibyte,
   "M" for mebibyte and "G" for gibibyte. The default unit if none is
   given is "M". You do not need this option if you use `--source`.

-  `--fixed-size`: The exact partition size. Specify as an integer
   value optionally followed by one of the units "k" / "K" for kibibyte,
   "M" for mebibyte and "G" for gibibyte. The default unit if none is
   given is "M".  Cannot be specify together with `--size`. An error
   occurs when assembling the disk image if the partition data is larger
   than `--fixed-size`.

-  `--source`: This option is a Wic-specific option that names the
   source of the data that populates the partition. The most common
   value for this option is "rootfs", but you can use any value that
   maps to a valid source plugin. For information on the source plugins,
   see the "dev-manual/wic:using the wic plugin interface"
   section in the Yocto Project Development Tasks Manual.

   If you use `--source rootfs`, Wic creates a partition as large as
   needed and fills it with the contents of the root filesystem pointed
   to by the `-r` command-line option or the equivalent root filesystem derived
   from the `-e` command-line option. The filesystem type used to
   create the partition is driven by the value of the `--fstype`
   option specified for the partition. See the entry on `--fstype`
   that follows for more information.

   If you use `--source plugin-name`, Wic creates a partition as large
   as needed and fills it with the contents of the partition that is
   generated by the specified plugin name using the data pointed to by
   the `-r` command-line option or the equivalent root filesystem derived from
   the `-e` command-line option. Exactly what those contents are and
   filesystem type used are dependent on the given plugin
   implementation.

   If you do not use the `--source` option, the `wic` command
   creates an empty partition. Consequently, you must use the `--size`
   option to specify the size of the empty partition.

-  `--ondisk` or `--ondrive`: Forces the partition to be created
   on a particular disk.

-  `--fstype`: Sets the file system type for the partition. Valid
   values are:

   -  `btrfs`

   -  `erofs`

   -  `ext2`

   -  `ext3`

   -  `ext4`

   -  `squashfs`

   -  `swap`

   -  `vfat`

-  `--fsoptions`: Specifies a free-form string of options to be used
   when mounting the filesystem. This string is copied into the
   `/etc/fstab` file of the installed system and should be enclosed in
   quotes. If not specified, the default string is "defaults".

-  `--label label`: Specifies the label to give to the filesystem to
   be made on the partition. If the given label is already in use by
   another filesystem, a new label is created for the partition.

-  `--active`: Marks the partition as active.

-  `--align (in KBytes)`: This option is a Wic-specific option that
   says to start partitions on boundaries given x KBytes.

-  `--offset`: This option is a Wic-specific option that
   says to place a partition at exactly the specified offset. If the
   partition cannot be placed at the specified offset, the image build
   will fail. Specify as an integer value optionally followed by one of
   the units "s" / "S" for 512 byte sector, "k" / "K" for kibibyte, "M"
   for mebibyte and "G" for gibibyte. The default unit if none is given
   is "k".

-  `--no-table`: This option is a Wic-specific option. Using the
   option reserves space for the partition and causes it to become
   populated. However, the partition is not added to the partition
   table.

-  `--exclude-path`: This option is a Wic-specific option that
   excludes the given relative path from the resulting image. This
   option is only effective with the rootfs source plugin.

-  `--extra-filesystem-space`: This option is a Wic-specific option that adds
   extra space after the space filled by the content of the partition.
   The final size can exceed the size specified by the `--size`
   option. The default value is 10M. Specify as an integer value
   optionally followed by one of the units "k" / "K" for kibibyte, "M"
   for mebibyte and "G" for gibibyte. The default unit if none is given
   is "M".

-  `--extra-partition-space`: This option is a Wic-specific option that adds
   extra empty space after the space filled by the filesystem.
   With `--fixed-size` option, the extra partition space is removed from the
   filesystem size. Otherwise (with or without `--size` flag), the extra
   partition space is added to the final partition size.
   The default value is 0M. Specify as an integer value
   optionally followed by one of the units "k" / "K" for kibibyte, "M"
   for mebibyte and "G" for gibibyte. The default unit if none is given
   is "M".

-  `--overhead-factor`: This option is a Wic-specific option that
   multiplies the size of the partition by the option's value. You must
   supply a value greater than or equal to "1". The default value is
   "1.3".

-  `--part-name`: This option is a Wic-specific option that
   specifies a name for GPT partitions.

-  `--part-type`: This option is a Wic-specific option that
   specifies the partition type globally unique identifier (GUID) for
   GPT partitions. You can find the list of partition type GUIDs at
   `GUID_Partition_Table#Partition_type_GUIDs`.

-  `--use-uuid`: This option is a Wic-specific option that causes
   Wic to generate a random GUID for the partition. The generated
   identifier is used in the bootloader configuration to specify the
   root partition.

-  `--uuid`: This option is a Wic-specific option that specifies the
   partition UUID.

-  `--fsuuid`: This option is a Wic-specific option that specifies
   the filesystem UUID. You can generate or modify
   **WKS_FILE** with this option if a preconfigured
   filesystem UUID is added to the kernel command line in the bootloader
   configuration before you run Wic.

-  `--system-id`: This option is a Wic-specific option that
   specifies the partition system ID, which is a one byte long,
   hexadecimal parameter with or without the 0x prefix.

-  `--mkfs-extraopts`: This option specifies additional options to
   pass to the `mkfs` utility. Some default options for certain
   filesystems do not take effect. See Wic's help on kickstart (i.e.
   `wic help kickstart`).

## Command: bootloader

This command specifies how the bootloader should be configured and
supports the following options:

> **Note:**
> Bootloader functionality and boot partitions are implemented by the
> various source plugins that implement bootloader functionality. The bootloader
> command essentially provides a means of modifying bootloader
> configuration.
>

-  `--append`: Specifies kernel parameters. These parameters will be
   added to the syslinux **APPEND** or `grub` kernel command line.

-  `--configfile`: Specifies a user-defined configuration file for
   the bootloader. You can provide a full pathname for the file or a
   file located in the `canned-wks` folder. This option overrides
   all other bootloader options.

-  `--ptable`: Specifies the partition table format. Valid values are:

   -  `msdos`

   -  `gpt`

-  `--timeout`: Specifies the number of seconds before the
   bootloader times out and boots the default option.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# QA Error and Warning Messages

## Introduction

When building a recipe, the OpenEmbedded build system performs various
QA checks on the output to ensure that common issues are detected and
reported. Sometimes when you create a new recipe to build new software,
it will build with no problems. When this is not the case, or when you
have QA issues building any software, it could take a little time to
resolve them.

While it is tempting to ignore a QA message or even to disable QA
checks, it is best to try and resolve any reported QA issues. This
chapter provides a list of the QA messages and brief explanations of the
issues you could encounter so that you can properly resolve problems.

The next section provides a list of all QA error and warning messages
based on a default configuration. Each entry provides the message or
error form along with an explanation.

> **Note:**
> -  At the end of each message, the name of the associated QA test (as
> listed in the "ref-classes-insane"
> section) appears within square brackets.
>
> -  As mentioned, this list of error and warning messages is for QA
> checks only. The list does not cover all possible build errors or
> warnings you could encounter.
>
> -  Because some QA checks are disabled by default, this list does not
> include all possible QA check errors and warnings.
>

## Errors and Warnings

### ``already-stripped``

`File '<file>' from <recipename> was already stripped, this will prevent future debugging! [already-stripped]`

Produced binaries have already been stripped prior to the build
system extracting debug symbols. It is common for upstream software
projects to default to stripping debug symbols for output binaries.
In order for debugging to work on the target using `-dbg` packages,
this stripping must be disabled.

Depending on the build system used by the software being built,
disabling this stripping could be as easy as specifying an additional
configure option. If not, disabling stripping might involve patching
the build scripts. In the latter case, look for references to "strip"
or "STRIP", or the "-s" or "-S" command-line options being specified
on the linker command line (possibly through the compiler command
line if preceded with "-Wl,").

> **Note:**
> Disabling stripping here does not mean that the final packaged
> binaries will be unstripped. Once the OpenEmbedded build system
> splits out debug symbols to the `-dbg` package, it will then
> strip the symbols from the binaries.
>

### ``arch``

-  `Architecture did not match (<file_arch>, expected <machine_arch>) in <file> [arch]`

   By default, the OpenEmbedded build system checks the Executable and
   Linkable Format (ELF) type, bit size, and endianness of any binaries
   to ensure they match the target architecture. This test fails if any
   binaries do not match the type since there would be an
   incompatibility. The test could indicate that the wrong compiler or
   compiler options have been used. Sometimes software, like
   bootloaders, might need to bypass this check. If the file you receive
   the error for is firmware that is not intended to be executed within
   the target operating system or is intended to run on a separate
   processor within the device, you can add "arch" to
   **INSANE_SKIP** for the package. Another
   option is to check the ref-tasks-compile log
   and verify that the compiler options being used are correct.

-  `Bit size did not match (<file_bits>, expected <machine_bits>) in <file> [arch]`

   By default, the OpenEmbedded build system checks the Executable and
   Linkable Format (ELF) type, bit size, and endianness of any binaries
   to ensure they match the target architecture. This test fails if any
   binaries do not match the type since there would be an
   incompatibility. The test could indicate that the wrong compiler or
   compiler options have been used. Sometimes software, like
   bootloaders, might need to bypass this check. If the file you receive
   the error for is firmware that is not intended to be executed within
   the target operating system or is intended to run on a separate
   processor within the device, you can add "arch" to
   **INSANE_SKIP** for the package. Another
   option is to check the ref-tasks-compile log
   and verify that the compiler options being used are correct.

-  `Endianness did not match (<file_endianness>, expected <machine_endianness>) in <file> [arch]`

   By default, the OpenEmbedded build system checks the Executable and
   Linkable Format (ELF) type, bit size, and endianness of any binaries
   to ensure they match the target architecture. This test fails if any
   binaries do not match the type since there would be an
   incompatibility. The test could indicate that the wrong compiler or
   compiler options have been used. Sometimes software, like
   bootloaders, might need to bypass this check. If the file you receive
   the error for is firmware that is not intended to be executed within
   the target operating system or is intended to run on a separate
   processor within the device, you can add "arch" to
   **INSANE_SKIP** for the package. Another
   option is to check the ref-tasks-compile log
   and verify that the compiler options being used are correct.

### ``build-deps``

`<packagename1> rdepends on <packagename2>, but it isn't a build dependency? [build-deps]`

There is a runtime dependency between the two specified packages, but
there is nothing explicit within the recipe to enable the
OpenEmbedded build system to ensure that dependency is satisfied.
This condition is usually triggered by an
**RDEPENDS** value being added at the packaging
stage rather than up front, which is usually automatic based on the
contents of the package. In most cases, you should change the recipe
to add an explicit **RDEPENDS** for the dependency.

### ``buildpaths``

`File <filename> in package <packagename> contains reference to TMPDIR [buildpaths]`

This check ensures that build system paths (including **TMPDIR**) do not
appear in output files, which not only leaks build system configuration into
the target, but also hinders binary reproducibility as the output will change
if the build system configuration changes.

Typically these paths will enter the output through some mechanism in the
configuration or compilation of the software being built by the recipe. To
resolve this issue you will need to determine how the detected path is
entering the output. Sometimes it may require adjusting scripts or code to
use a relative path rather than an absolute one, or to pick up the path from
runtime configuration or environment variables.

### ``configure-gettext``

`AM_GNU_GETTEXT used but no inherit gettext [configure-gettext]`

If a recipe is building something that uses automake and the automake
files contain an `AM_GNU_GETTEXT` directive then this check will fail
if there is no `inherit gettext` statement in the recipe to ensure
that gettext is available during the build. Add `inherit gettext` to
remove the warning.

### ``configure-unsafe``

`This autoconf log indicates errors, it looked at host include and/or library paths while determining system capabilities. Rerun configure task after fixing this. [configure-unsafe]`

The log for the ref-tasks-configure task
indicates that paths on the host were searched for files, which is
not appropriate when cross-compiling. Look for "is unsafe for
cross-compilation" or "CROSS COMPILE Badness" in the specified log
file.

### ``debug-deps``

`<packagename> rdepends on <debug_packagename> [debug-deps]`

There is a dependency between the specified non-dbg package (i.e. a
package whose name does not end in `-dbg`) and a package that is a
`dbg` package. The `dbg` packages contain debug symbols and are
brought in using several different methods:

-  Using the `dbg-pkgs`
   **IMAGE_FEATURES** value.

-  Using **IMAGE_INSTALL**.

-  As a dependency of another `dbg` package that was brought in
   using one of the above methods.

The dependency might have been automatically added because the
`dbg` package erroneously contains files that it should not contain
(e.g. a non-symlink `.so` file) or it might have been added
manually (e.g. by adding to **RDEPENDS**).

### ``debug-files``

`non debug package contains .debug directory: <packagename> path <path> [debug-files]`

The specified package contains a `.debug` directory, which should
not appear in anything but the `-dbg` package. This situation might
occur if you add a path which contains a `.debug` directory and do
not explicitly add the `.debug` directory to the `-dbg` package.
If this is the case, add the `.debug` directory explicitly to
`FILES:${PN}-dbg`. See **FILES** for additional
information on **FILES**.

### ``dep-cmp``

`<var>:<packagename> is invalid: <comparison> (<value>)   only comparisons <, =, >, <=, and >= are allowed [dep-cmp]`

If you are adding a versioned dependency relationship to one of the
dependency variables (**RDEPENDS**,
**RRECOMMENDS**,
**RSUGGESTS**,
**RPROVIDES**,
**RREPLACES**, or
**RCONFLICTS**), you must only use the named
comparison operators. Change the versioned dependency values you are
adding to match those listed in the message.

### ``dev-deps``

`<packagename> rdepends on <dev_packagename> [dev-deps]`

There is a dependency between the specified non-dev package (a package
whose name does not end in `-dev`) and a package that is a `dev`
package. The `dev` packages contain development headers and are
usually brought in using several different methods:

-  Using the `dev-pkgs`
   **IMAGE_FEATURES** value.

-  Using **IMAGE_INSTALL**.

-  As a dependency of another `dev` package that was brought in
   using one of the above methods.

The dependency might have been automatically added (because the
`dev` package erroneously contains files that it should not have
(e.g. a non-symlink `.so` file) or it might have been added
manually (e.g. by adding to **RDEPENDS**).

### ``desktop``

`"Desktop file issue: ... [desktop]`

Runs the `desktop-file-validate` program against any
`.desktop` files to validate their contents against the
specification for `.desktop` files.

### ``dev-so``

`non -dev/-dbg/nativesdk- package contains symlink .so: <packagename> path '<path>' [dev-so]`

Symlink `.so` files are for development only, and should therefore
go into the `-dev` package. This situation might occur if you add
`*.so*` rather than `*.so.*` to a non-dev package. Change
**FILES** (and possibly
**PACKAGES**) such that the specified `.so`
file goes into an appropriate `-dev` package.

### ``empty-dirs``

`<packagename> installs files in <path>, but it is expected to be empty [empty-dirs]`

The specified package is installing files into a directory that is
normally expected to be empty (such as `/tmp`). These files may
be more appropriately installed to a different location, or
perhaps alternatively not installed at all, usually by updating the
ref-tasks-install task/function.

### ``file-rdeps``

`<packagename> requires <files>, but no providers in its RDEPENDS [file-rdeps]`

A file-level dependency has been identified from the specified
package on the specified files, but there is no explicit
corresponding entry in **RDEPENDS**. If
particular files are required at runtime then **RDEPENDS** should be
declared in the recipe to ensure the packages providing them are
built.

### ``files-invalid``

`FILES variable for package <packagename> contains '//' which is invalid. Attempting to fix this but you should correct the metadata. [files-invalid]`

The string "//" is invalid in a Unix path. Correct all occurrences
where this string appears in a **FILES** variable so
that there is only a single "/".

### ``host-user-contaminated``

`<package name>: <path> is owned by gid/uid <gid/uid>, which is the same as the user running bitbake. This may be due to host contamination [host-user-contaminated]`

Checks that no package produced by the
recipe contains any files outside of `/home` with a user or group
ID that matches the user running BitBake. A match usually indicates
that the files are being installed with an incorrect UID/GID, since
target IDs are independent from host IDs. For additional information,
see the section describing the
ref-tasks-install task.

### ``infodir``

`The /usr/share/info/dir file is not meant to be shipped in a particular package. [infodir]`

The `/usr/share/info/dir` should not be packaged. Add the following
line to your ref-tasks-install task or to your
`do_install:append` within the recipe as follows::

   rm ${D}${infodir}/dir

### ``installed-vs-shipped``

`<recipename>: Files/directories were installed but not shipped in any package [installed-vs-shipped]`

Files have been installed within the
ref-tasks-install task but have not been
included in any package by way of the **FILES**
variable. Files that do not appear in any package cannot be present
in an image later on in the build process. You need to do one of the
following:

-  Add the files to **FILES** for the package you want them to appear
   in (e.g. `FILES:${`\ **PN**\ `}` for the main
   package).

-  Delete the files at the end of the ref-tasks-install task if the
   files are not needed in any package.

### ``incompatible-license``

`Excluding <package> from packaging as it has incompatible license(s): <license> [incompatible-license]`

Report when packages are excluded from being created due to being marked with
a license that is in **INCOMPATIBLE_LICENSE**.

### ``invalid-chars``

`<variable> has non <envoding> characters [invalid-chars]`

Checks that the recipe metadata variables **DESCRIPTION**,
**SUMMARY**, **LICENSE**, and **SECTION** do not contain
non-UTF-8 characters. Some package managers do not support such characters.

### ``invalid-packageconfig``

`<package>: invalid PACKAGECONFIG(s): <configs> [invalid-packageconfig]`

Checks that no undefined features are being added to **PACKAGECONFIG**.
For example, any name "foo" for which the following form does not exist::

   PACKAGECONFIG[foo] = "..."

### ``la``

`<file> failed sanity test (workdir) in path <path> [la]`

The specified `.la` file contains **TMPDIR**
paths. Any `.la` file containing these paths is incorrect since
`libtool` adds the correct sysroot prefix when using the files
automatically itself.

### ``ldflags``

`File '<file>' in package '<package>' doesn't have GNU_HASH (didn't pass LDFLAGS?) [ldflags]`

This indicates that binaries produced when building the recipe have
not been linked with the **LDFLAGS** options
provided by the build system. Check to be sure that the **LDFLAGS**
variable is being passed to the linker command. A common workaround
for this situation is to pass in **LDFLAGS** using
**TARGET_CC_ARCH** within the recipe as
follows::

   TARGET_CC_ARCH += "${LDFLAGS}"

### ``libdir``

`<packagename>: found library in wrong location [libdir]`

The specified file may have been installed into an incorrect
(possibly hardcoded) installation path. For example, this test will
catch recipes that install `/lib/bar.so` when `${base_libdir}` is
"lib32". Another example is when recipes install
`/usr/lib64/foo.so` when `${libdir}` is "/usr/lib". False
positives occasionally exist. For these cases add "libdir" to
**INSANE_SKIP** for the package.

### ``libexec``

`<packagename>: <path> is using libexec please relocate to <libexecdir> [libexec]`

The specified package contains files in `/usr/libexec` when the
distro configuration uses a different path for `<libexecdir>` By
default, `<libexecdir>` is `$prefix/libexec`. However, this
default can be changed (e.g. `${libdir}`).

### ``mime``

`package contains mime types but does not inherit mime: <packagename> path '<file>' [mime]`

The specified package contains mime type files (`.xml` files in
`${datadir}/mime/packages`) and yet does not inherit the
ref-classes-mime class which will ensure that these get
properly installed. Either add `inherit mime` to the recipe or remove the
files at the ref-tasks-install step if they are not needed.

### ``mime-xdg``

`package contains desktop file with key 'MimeType' but does not inherit mime-xdg: <packagename> path '<file>' [mime-xdg]`

The specified package contains a .desktop file with a 'MimeType' key
present, but does not inherit the ref-classes-mime-xdg
class that is required in order for that to be activated. Either add
`inherit mime` to the recipe or remove the files at the
ref-tasks-install step if they are not needed.

### ``missing-update-alternatives``

`<recipename>: recipe defines ALTERNATIVE:<packagename> but doesn't inherit update-alternatives. This might fail during do_rootfs later! [missing-update-alternatives]`

This check ensures that if a recipe sets the **ALTERNATIVE** variable that the
recipe also inherits ref-classes-update-alternatives such
that the alternative will be correctly set up. If you are seeing this message, either
add `inherit update-alternatives` to your recipe or remove the reference to the variable
if it is not needed.

### ``packages-list``

`<packagename> is listed in PACKAGES multiple times, this leads to packaging errors. [packages-list]`

Package names must appear only once in the
**PACKAGES** variable. You might receive this
error if you are attempting to add a package to **PACKAGES** that is
already in the variable's value.

### ``patch-fuzz``

`Fuzz detected: <patch output> [patch-fuzz]`

This check looks for evidence of "fuzz" when applying patches within the ref-tasks-patch
task. Patch fuzz is a situation when the `patch` tool ignores some of the context
lines in order to apply the patch. Consider this example:

Patch to be applied::

   --- filename
   +++ filename
    context line 1
    context line 2
    context line 3
   +newly added line
    context line 4
    context line 5
    context line 6

Original source code::

   different context line 1
   different context line 2
   context line 3
   context line 4
   different context line 5
   different context line 6

Outcome (after applying patch with fuzz)::

   different context line 1
   different context line 2
   context line 3
   newly added line
   context line 4
   different context line 5
   different context line 6

Chances are, the newly added line was actually added in a completely
wrong location, or it was already in the original source and was added
for the second time. This is especially possible if the context line 3
and 4 are blank or have only generic things in them, such as `#endif` or `}`.
Depending on the patched code, it is entirely possible for an incorrectly
patched file to still compile without errors.

*How to eliminate patch fuzz warnings*

Use the `devtool` command as explained by the warning. First, unpack the
source into devtool workspace::

   devtool modify <recipe>

This will apply all of the patches, and create new commits out of them in
the workspace --- with the patch context updated.

Then, replace the patches in the recipe layer::

   devtool finish --force-patch-refresh <recipe> <layer_path>

The patch updates then need be reviewed (preferably with a side-by-side diff
tool) to ensure they are indeed doing the right thing i.e.:

#. they are applied in the correct location within the file;
#. they do not introduce duplicate lines, or otherwise do things that
   are no longer necessary.

To confirm these things, you can also review the patched source code in
devtool's workspace, typically in `<build_dir>/workspace/sources/<recipe>/`

Once the review is done, you can create and publish a layer commit with
the patch updates that modify the context. Devtool may also refresh
other things in the patches, those can be discarded.

### ``patch-status``

-  `Missing Upstream-Status in patch <patchfile> Please add according to <url> [patch-status]`

   The `Upstream-Status` value is missing in the specified patch file's header.
   This value is intended to track whether or not the patch has been sent
   upstream, whether or not it has been merged, etc.

   For more information, see the
   "contributor-guide/recipe-style-guide:patch upstream status"
   section in the Yocto Project and OpenEmbedded Contributor Guide.

-  `Malformed Upstream-Status in patch <patchfile> Please correct according to <url> [patch-status]`

   The `Upstream-Status` value in the specified patch file's header is invalid -
   it must be a specific format. See the "Missing Upstream-Status" entry above
   for more information.

### ``pep517-backend``

`inherits setuptools3 but has pyproject.toml with <build backend>, use the correct class [pep517-backend]`

Checks that a recipe inheriting ref-classes-setuptools3 has a
PEP517-compliant backend.

### ``perllocalpod``

`<packagename> contains perllocal.pod (<files>), should not be installed [perllocalpod]`

`perllocal.pod` is an index file of locally installed modules and so shouldn't be
installed by any distribution packages. The ref-classes-cpan class
already sets `NO_PERLLOCAL` to stop this file being generated by most Perl recipes,
but if a recipe is using `MakeMaker` directly then they might not be doing this
correctly. This check ensures that perllocal.pod is not in any package in order to
avoid multiple packages shipping this file and thus their packages conflicting
if installed together.

### ``perm-config``

`Fixup Perms: invalid config line <line> [perm-config]`

Reports lines in `fs-perms.txt` that have an invalid format.

### ``perm-line``

`Fixup perms: <config> invalid line: <line> [perm-line]`

Reports lines in `fs-perms.txt` that have an invalid format.

### ``perm-link``

`Fixup Perms: Unable to correct directory link, target already exists: <directory> -> <target> [perm-link]`

Reports lines in `fs-perms.txt` that specify 'link' where the specified
target already exists.

### ``perms``

Currently, this check is unused but reserved.

### ``pkgconfig``

`<file> failed sanity test (tmpdir) in path <path> [pkgconfig]`

The specified `.pc` file contains
**TMPDIR**\ `/`\ **WORKDIR**
paths. Any `.pc` file containing these paths is incorrect since
`pkg-config` itself adds the correct sysroot prefix when the files
are accessed.

### ``pkgname``

`<packagename> doesn't match the [a-z0-9.+-]+ regex [pkgname]`

The convention within the OpenEmbedded build system (sometimes
enforced by the package manager itself) is to require that package
names are all lower case and to allow a restricted set of characters.
If your recipe name does not match this, or you add packages to
**PACKAGES** that do not conform to the
convention, then you will receive this error. Rename your recipe. Or,
if you have added a non-conforming package name to **PACKAGES**,
change the package name appropriately.

### ``pkgvarcheck``

-  `<recipefile>: Variable <variable> is set as not being package specific, please fix this. [pkgvarcheck]`

   Certain variables (**RDEPENDS**,
   **RRECOMMENDS**,
   **RSUGGESTS**,
   **RCONFLICTS**,
   **RPROVIDES**,
   **RREPLACES**, **FILES**,
   `pkg_preinst`, `pkg_postinst`, `pkg_prerm`, `pkg_postrm`, and
   **ALLOW_EMPTY**) should always be set specific
   to a package (i.e. they should be set with a package name override
   such as `RDEPENDS:${PN} = "value"` rather than
   `RDEPENDS = "value"`). If you receive this error, correct any
   assignments to these variables within your recipe.

-  `recipe uses DEPENDS:${PN}, should use DEPENDS [pkgvarcheck]`

   This check looks for instances of setting `DEPENDS:${PN}`
   which is erroneous (**DEPENDS** is a recipe-wide variable and thus
   it is not correct to specify it for a particular package, nor will such
   an assignment actually work.) Set **DEPENDS** instead.

### ``pn-overrides``

`Recipe <recipefile> has PN of "<recipename>" which is in OVERRIDES, this can result in unexpected behavior. [pn-overrides]`

The specified recipe has a name (**PN**) value that
appears in **OVERRIDES**. If a recipe is named
such that its **PN** value matches something already in **OVERRIDES**
(e.g. **PN** happens to be the same as **MACHINE**
or **DISTRO**), it can have unexpected
consequences. For example, assignments such as
`FILES:${PN} = "xyz"` effectively turn into `FILES = "xyz"`.
Rename your recipe (or if **PN** is being set explicitly, change the
**PN** value) so that the conflict does not occur. See
**FILES** for additional information.

### ``shebang-size``

`<packagename>: <file> maximum shebang size exceeded, the maximum size is 128. [shebang-size]`

This check ensures that the shebang line (`#!` in the first line) for a script
is not longer than 128 characters, which can cause an error at runtime depending
on the operating system. If you are seeing this message then the specified script
may need to be patched to have a shorter in order to avoid runtime problems.

### Shlib provider

`<oldpackage>-<oldpkgversion> was registered as shlib provider for <library>, changing it to <newpackage>-<newpkgversion> because it was built later`

This message means that both `<oldpackage>` and `<newpackage>`
provide the specified shared library. You can expect this message
when a recipe has been renamed. However, if that is not the case, the
message might indicate that a private version of a library is being
erroneously picked up as the provider for a common library. If that
is the case, you should add the library's `.so` filename to
**PRIVATE_LIBS** in the recipe that provides
the private version of the library.

### ``space-around-equal``

`<filename>:<line number> has a lack of whitespace around the assignment: '<assignment>'`

This warning indicated that there is missing spaces around an assignment.

For example, the following assignments would print a warning::

   FOO="bar"
   FOO= "bar"
   FOO ="bar"

These should be replaced by::

   FOO = "bar"

### ``src-uri-bad``

-  `<recipename>: SRC_URI uses unstable GitHub archives [src-uri-bad]`

   GitHub provides "archive" tarballs, however these can be re-generated
   on the fly and thus the file's signature will not necessarily match that
   in the **SRC_URI** checksums in future leading to build failures. It is
   recommended that you use an official release tarball or switch to
   pulling the corresponding revision in the actual git repository instead.

-  `SRC_URI uses PN not BPN [src-uri-bad]`

   If some part of **SRC_URI** needs to reference the recipe name, it should do
   so using ${**BPN**} rather than ${**PN**} as the latter will change
   for different variants of the same recipe e.g. when **BBCLASSEXTEND**
   or multilib are being used. This check will fail if a reference to `${PN}`
   is found within the **SRC_URI** value --- change it to `${BPN}` instead.

### ``staticdev``

`non -staticdev package contains static .a library: <packagename> path '<path>' [staticdev]`

Static `.a` library files should go into a `-staticdev` package.
Change **FILES** (and possibly
**PACKAGES**) such that the specified `.a` file
goes into an appropriate `-staticdev` package.

### ``symlink-to-sysroot``

`Symlink <path> in <packagename> points to TMPDIR [symlink-to-sysroot]`

The specified symlink points into **TMPDIR** on the
host. Such symlinks will work on the host. However, they are clearly
invalid when running on the target. You should either correct the
symlink to use a relative path or remove the symlink.

### Recipe naming

`Recipe <recipe> appears native/nativesdk but is not, should inherit native/nativesdk`

Checks that the recipe name and recipe class match, so that `*-native`
recipes inherit ref-classes-native and `nativesdk-*` recipes
inherit ref-classes-nativesdk.

### ``rpaths``

`package <packagename> contains bad RPATH <rpath> in file <file> [rpaths]`

The specified binary produced by the recipe contains dynamic library
load paths (rpaths) that contain build system paths such as
**TMPDIR**, which are incorrect for the target and
could potentially be a security issue. Check for bad `-rpath`
options being passed to the linker in your
ref-tasks-compile log. Depending on the build
system used by the software being built, there might be a configure
option to disable rpath usage completely within the build of the
software.

### ``textrel``

`ELF binary '<file>' has relocations in .text [textrel]`

The specified ELF binary contains relocations in its `.text`
sections. This situation can result in a performance impact at
runtime.

Typically, the way to solve this performance issue is to add "-fPIC"
or "-fpic" to the compiler command-line options. For example, given
software that reads **CFLAGS** when you build it,
you could add the following to your recipe::

   CFLAGS:append = " -fPIC "

For more information on text relocations at runtime, see
https://www.akkadia.org/drepper/textrelocs.html.

### ``unhandled-features-check``

`<recipename>: recipe doesn't inherit features_check [unhandled-features-check]`

This check ensures that if one of the variables that the
ref-classes-features_check class supports (e.g.
**REQUIRED_DISTRO_FEATURES**) is used, then the recipe
inherits ref-classes-features_check in order for
the requirement to actually work. If you are seeing this message, either
add `inherit features_check` to your recipe or remove the reference to
the variable if it is not needed.

### ``unimplemented-ptest``

`<tool> tests detected [unimplemented-ptest]`

This check will detect if the source of the package contains some
upstream-provided tests and, if so, that ptests are implemented for this
recipe.  See the "test-manual/ptest:testing packages with ptest"
section in the Yocto Project Development Tasks Manual. See also the
"ref-classes-ptest" section.

### ``unknown-configure-option``

`<recipe>: configure was passed unrecognized options: <options> [unknown-configure-option]`

The configure script is reporting that the specified options are
unrecognized. This situation could be because the options were
previously valid but have been removed from the configure script. Or,
there was a mistake when the options were added and there is another
option that should be used instead. If you are unsure, consult the
upstream build documentation, the `./configure --help` output, and
the upstream change log or release notes. Once you have worked out
what the appropriate change is, you can update
**EXTRA_OECONF**,
**PACKAGECONFIG_CONFARGS**, or the
individual **PACKAGECONFIG** option values
accordingly.

### ``unlisted-pkg-lics``

`LICENSE:<packagename> includes licenses (<licenses>) that are not listed in LICENSE [unlisted-pkg-lics]`

The **LICENSE** of the recipe should be a superset
of all the licenses of all packages produced by this recipe. In other
words, any license in `LICENSE:*` should also appear in
**LICENSE**.

### ``useless-rpaths``

`<packagename>: <file> contains probably-redundant RPATH <rpath> [useless-rpaths]`

The specified binary produced by the recipe contains dynamic library
load paths (rpaths) that on a standard system are searched by default
by the linker (e.g. `/lib` and `/usr/lib`). While these paths
will not cause any breakage, they do waste space and are unnecessary.
Depending on the build system used by the software being built, there
might be a configure option to disable rpath usage completely within
the build of the software.

### ``usrmerge``

`<packagename> package is not obeying usrmerge distro feature. /<path> should be relocated to /usr. [usrmerge]`

If `usrmerge` is in **DISTRO_FEATURES**, this check will ensure that no package
installs files to root (`/bin`, `/sbin`, `/lib`, `/lib64`) directories. If you are seeing this
message, it indicates that the ref-tasks-install step (or perhaps the build process that
ref-tasks-install is calling into, e.g. `make install` is using hardcoded paths instead
of the variables set up for this (`bindir`, `sbindir`, etc.), and should be
changed so that it does.

### ``version-going-backwards``

`Package version for package <package> went backwards which would break package feeds (from <version2> to <version1>) [version-going-backwards]`

If the ref-classes-buildhistory class is enabled, reports when a
package being written out has a lower version than the previously written
package under the same name. If you are placing output packages into a feed
and upgrading packages on a target system using that feed, the version of a
package going backwards can result in the target system not correctly
upgrading to the "new" version of the package.

> **Note:**
> This is only relevant when you are using runtime package management
> on your target system.
>

### ``virtual-slash``

`<variable> is set to <value> but the substring 'virtual/' holds no meaning in this context. It only works for build time dependencies, not runtime ones. It is suggested to use 'VIRTUAL-RUNTIME_' variables instead. [virtual-slash]`

`virtual/` is a convention intended for use in the build context
(i.e. **PROVIDES** and **DEPENDS**) rather than the runtime
context (i.e. **RPROVIDES** and **RDEPENDS**). Use
**VIRTUAL-RUNTIME** variables instead for the latter.

### ``xorg-driver-abi``

`Package <packagename> contains Xorg driver (<driver>) but no xorg-abi- dependencies [xorg-driver-abi]`

The specified package contains an Xorg driver, but does not have a
corresponding ABI package dependency. The xserver-xorg recipe
provides driver ABI names. All drivers should depend on the ABI
versions that they have been built against. Driver recipes that
include `xorg-driver-input.inc` or `xorg-driver-video.inc` will
automatically get these versions. Consequently, you should only need
to explicitly add dependencies to binary driver recipes.

## Configuring and Disabling QA Checks

You can configure the QA checks globally so that specific check failures
either raise a warning or an error message, using the
**WARN_QA** and **ERROR_QA**
variables, respectively. You can also disable checks within a particular
recipe using **INSANE_SKIP**. For information on
how to work with the QA checks, see the
"ref-classes-insane" section.

> **Note:**
> Please keep in mind that the QA checks are meant to detect real
> or potential problems in the packaged output. So exercise caution
> when disabling these checks.
>
>

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Images

The OpenEmbedded build system provides several example images to satisfy
different needs. When you issue the `bitbake` command you provide a
"top-level" recipe that essentially begins the build for the type of
image you want.

> **Note:**
> Building an image without GNU General Public License Version 3
> (GPLv3), GNU Lesser General Public License Version 3 (LGPLv3), and
> the GNU Affero General Public License Version 3 (AGPL-3.0) components
> is only tested for core-image-minimal image. Furthermore, if you would like to
> build an image and verify that it does not include GPLv3 and similarly licensed
> components, you must make the following changes in the image recipe
> file before using the BitBake command to build the image:
>
> INCOMPATIBLE_LICENSE = "GPL-3.0* LGPL-3.0*"
>
> Alternatively, you can adjust `local.conf` file, repeating and adjusting the line
> for all images where the license restriction must apply:
>
> INCOMPATIBLE_LICENSE:pn-your-image-name = "GPL-3.0* LGPL-3.0*"
>

From within the **OpenEmbedded-Core (OE-Core)** Git repository, you can use the following
command to display the list of directories that contain image recipe files::

   $ ls meta*/recipes*/images/*.bb

Here is a list of supported recipes:

-  `build-appliance-image`: An example virtual machine that contains
   all the pieces required to run builds using the build system as well
   as the build system itself. You can boot and run the image using
   either the `VMware
   Player <https://www.vmware.com/products/player/overview.html>`__ or
   `VMware
   Workstation <https://www.vmware.com/products/workstation/overview.html>`__.
   For more information on this image, see the :yocto_home:`Build
   Appliance </software-item/build-appliance>` page
   on the Yocto Project website.

-  `core-image-base`: A console-only image that fully supports the
   target device hardware.

-  `core-image-full-cmdline`: A console-only image with more
   full-featured Linux system functionality installed.

-  `core-image-minimal`: A small image just capable of allowing a
   device to boot.

-  `core-image-minimal-dev`: A `core-image-minimal` image suitable
   for development work using the host. The image includes headers and
   libraries you can use in a host development environment.

-  `core-image-minimal-initramfs`: A `core-image-minimal` image that
   has the Minimal RAM-based Initial Root Filesystem (**Initramfs**) as part
   of the kernel, which allows the system to find the first "init"
   program more efficiently. See the
   **PACKAGE_INSTALL** variable for
   additional information helpful when working with **Initramfs** images.

-  `core-image-minimal-mtdutils`: A `core-image-minimal` image that
   has support for the Minimal MTD Utilities, which let the user
   interact with the MTD subsystem in the kernel to perform operations
   on flash devices.

-  `core-image-rt`: A `core-image-minimal` image plus a real-time
   test suite and tools appropriate for real-time use.

-  `core-image-rt-sdk`: A `core-image-rt` image that includes
   everything in the cross-toolchain. The image also includes
   development headers and libraries to form a complete stand-alone SDK
   and is suitable for development using the target.

-  `core-image-sato`: An image with Sato support, a mobile environment
   and visual style that works well with mobile devices. The image
   supports X11 with a Sato theme and applications such as a terminal,
   editor, file manager, media player, and so forth.

-  `core-image-sato-dev`: A `core-image-sato` image suitable for
   development using the host. The image includes libraries needed to
   build applications on the device itself, testing and profiling tools,
   and debug symbols. This image was formerly `core-image-sdk`.

-  `core-image-sato-sdk`: A `core-image-sato` image that includes
   everything in the cross-toolchain. The image also includes
   development headers and libraries to form a complete standalone SDK
   and is suitable for development using the target.

-  `core-image-testmaster`: A "controller" image designed to be used for
   automated runtime testing. Provides a "known good" image that is
   deployed to a separate partition so that you can boot into it and use
   it to deploy a second image to be tested. You can find more
   information about runtime testing in the
   "test-manual/runtime-testing:performing automated runtime testing"
   section in the Yocto Project Test Environment Manual.

-  `core-image-testmaster-initramfs`: A RAM-based Initial Root
   Filesystem (**Initramfs**) image tailored for use with the
   `core-image-testmaster` image.

-  `core-image-weston`: A very basic Wayland image with a terminal.
   This image provides the Wayland protocol libraries and the reference
   Weston compositor. For more information, see the
   "dev-manual/wayland:using wayland and weston"
   section in the Yocto Project Development Tasks Manual.

-  `core-image-x11`: A very basic X11 image with a terminal.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Features

This chapter provides a reference of shipped machine and distro features
you can include as part of your image, a reference on image features you
can select, and a reference on ref-features-backfill.

Features provide a mechanism for working out which packages should be
included in the generated images. Distributions can select which
features they want to support through the **DISTRO_FEATURES** variable,
which is set or appended to in a distribution's configuration file such
as `poky.conf`, `poky-tiny.conf`, `poky-altcfg.conf` and so forth.
Machine features are set in the **MACHINE_FEATURES** variable, which is
set in the machine configuration file and specifies the hardware
features for a given machine.

These two variables combine to work out which kernel modules, utilities,
and other packages to include. A given distribution can support a
selected subset of features so some machine features might not be
included if the distribution itself does not support them.

One method you can use to determine which recipes are checking to see if
a particular feature is contained or not is to `grep` through the
**Metadata** for the feature. Here is an example that
discovers the recipes whose build is potentially changed based on a
given feature::

   $ cd openembedded-core
   $ git grep 'contains.*MACHINE_FEATURES.*feature'

## Machine Features

The items below are features you can use with
**MACHINE_FEATURES**. Features do not have a
one-to-one correspondence to packages, and they can go beyond simply
controlling the installation of a package or packages. Sometimes a
feature can influence how certain recipes are built. For example, a
feature might determine whether a particular configure option is
specified within the ref-tasks-configure task
for a particular recipe.

This feature list only represents features as shipped with the Yocto
Project metadata:

-  *acpi:* Hardware has ACPI (x86/x86_64 only)

-  *alsa:* Hardware has ALSA audio drivers

-  *bluetooth:* Hardware has integrated BT

-  *coresight:* Support for the `Coresight
   <https://docs.kernel.org/trace/coresight/coresight.html>`__ Linux Kernel
   feature on Arm processors. This feature depends on the `opencsd` recipe
   which is present in the meta-arm  **layer**.

-  *efi:* Support for booting through EFI

-  *ext2:* Hardware HDD or Microdrive

-  *keyboard:* Hardware has a keyboard

-  *numa:* Hardware has non-uniform memory access

-  *pcbios:* Support for booting through BIOS

-  *pci:* Hardware has a PCI bus

-  *pcmcia:* Hardware has PCMCIA or CompactFlash sockets

-  *phone:* Mobile phone (voice) support

-  *qemu-usermode:* QEMU can support user-mode emulation for this machine

-  *qvga:* Machine has a QVGA (320x240) display

-  *rtc:* Machine has a Real-Time Clock

-  *screen:* Hardware has a screen

-  *serial:* Hardware has serial support (usually RS232)

-  *touchscreen:* Hardware has a touchscreen

-  *usbgadget:* Hardware is USB gadget device capable

-  *usbhost:* Hardware is USB Host capable

-  *vfat:* FAT file system support

-  *wifi:* Hardware has integrated WiFi

## Distro Features

The items below are features you can use with
**DISTRO_FEATURES** to enable features across
your distribution. Features do not have a one-to-one correspondence to
packages, and they can go beyond simply controlling the installation of
a package or packages. In most cases, the presence or absence of a
feature translates to the appropriate option supplied to the configure
script during the ref-tasks-configure task for
the recipes that optionally support the feature. Appropriate options
must be supplied, and enabling/disabling **PACKAGECONFIG** for the
concerned packages is one way of supplying such options.

Some distro features are also machine features. These select features
make sense to be controlled both at the machine and distribution
configuration level. See the
**COMBINED_FEATURES** variable for more
information.

> **Note:**
> **DISTRO_FEATURES** is normally independent of kernel configuration,
> so if a feature specified in **DISTRO_FEATURES** also relies on
> support in the kernel, you will also need to ensure that support is
> enabled in the kernel configuration.
>

This list only represents features as shipped with the Yocto Project
metadata, as extra layers can define their own:

-  *3g:* Include support for cellular data.

-  *acl:* Include Access Control List  support.

-  *alsa:* Include Advanced Linux Sound Architecture 
   support (OSS compatibility kernel modules installed if available).

-  *api-documentation:* Enables generation of API documentation during
   recipe builds. The resulting documentation is added to SDK tarballs
   when the `bitbake -c populate_sdk` command is used. See the
   "sdk-manual/appendix-customizing-standard:adding api documentation to the standard sdk"
   section in the Yocto Project Application Development and the
   Extensible Software Development Kit (eSDK) manual.

-  *bluetooth:* Include bluetooth support (integrated BT only).

-  *cramfs:* Include CramFS support.

-  *debuginfod:* Include support for getting ELF debugging information through
   a debuginfod 
   server.

-  *directfb:* Include DirectFB support.

-  *ext2:* Include tools for supporting devices with internal
   HDD/Microdrive for storing files (instead of Flash only devices).

-  *gobject-introspection-data:* Include data to support
   `GObject Introspection <https://gi.readthedocs.io/en/latest/>`__.

-  *ipsec:* Include IPSec support.

-  *ipv4:* Include IPv4 support.

-  *ipv6:* Include IPv6 support.

-  *keyboard:* Include keyboard support (e.g. keymaps will be loaded
   during boot).

-  *minidebuginfo:* Add minimal debug symbols (minidebuginfo)
   to binary files containing, allowing `coredumpctl` and `gdb` to show symbolicated stack traces.

-  *multiarch:* Enable building applications with multiple architecture
   support.

-  *ldconfig:* Include support for ldconfig and `ld.so.conf` on the
   target.

-  *lto:* Enable `Link-Time Optimisation <https://gcc.gnu.org/wiki/LinkTimeOptimization>`__.

-  *nfc:* Include support for
   `Near Field Communication <https://en.wikipedia.org/wiki/Near-field_communication>`__.

-  *nfs:* Include NFS client support (for mounting NFS exports on
   device).

-  *nls:* Include National Language Support (NLS).

-  *opencl:* Include support for the :wikipedia:`OpenCL (Open Computing
   Language) <OpenCL>` framework.

-  *opengl:* Include the Open Graphics Library, which is a
   cross-language, multi-platform application programming interface used
   for rendering two and three-dimensional graphics.

-  *overlayfs:* Include `OverlayFS <https://docs.kernel.org/filesystems/overlayfs.html>`__
   support.

-  *pam:* Include Pluggable Authentication Module (PAM) 
   support.

-  *pci:* Include PCI bus support.

-  *pcmcia:* Include PCMCIA/CompactFlash support.

-  *pni-names:* Enable generation of persistent network interface names, i.e.
   the system tries hard to have the same but unique names for the network
   interfaces even after a reinstall.

-  *polkit:* Include Polkit  support.

-  *ppp:* Include PPP dialup support.

-  *ptest:* Enables building the package tests where supported by
   individual recipes. For more information on package tests, see the
   "test-manual/ptest:testing packages with ptest" section
   in the Yocto Project Development Tasks Manual.

-  *pulseaudio:* Include support for
   `PulseAudio <https://www.freedesktop.org/wiki/Software/PulseAudio/>`__.

-  *selinux:* Include support for
   Security-Enhanced Linux (SELinux) 
   (requires `meta-selinux <https://layers.openembedded.org/layerindex/layer/meta-selinux/>`__).

-  *seccomp:* Enables building applications with
   seccomp  support, to
   allow them to strictly restrict the system calls that they are allowed
   to invoke.

-  *smbfs:* Include SMB networks client support (for mounting
   Samba/Microsoft Windows shares on device).

-  *systemd:* Include support for this `init` manager, which is a full
   replacement of for `init` with parallel starting of services,
   reduced shell overhead, and other features. This `init` manager is
   used by many distributions.

-  *systemd-resolved:* Include support and use `systemd-resolved` as the
   main DNS name resolver in `glibc` Name Service Switch. This is a DNS
   resolver daemon from `systemd`.

-  *usbgadget:* Include USB Gadget Device support (for USB
   networking/serial/storage).

-  *usbhost:* Include USB Host support (allows to connect external
   keyboard, mouse, storage, network etc).

-  *usrmerge:* Merges the `/bin`, `/sbin`, `/lib`, and `/lib64`
   directories into their respective counterparts in the `/usr`
   directory to provide better package and application compatibility.

-  *vfat:* Include FAT filesystem 
   support.

-  *vulkan:* Include support for the Vulkan API .

-  *wayland:* Include the Wayland display server protocol and the
   library that supports it.

-  *wifi:* Include WiFi support (integrated only).

-  *x11:* Include the X server and libraries.

-  *xattr:* Include support for
   extended file attributes .

-  *zeroconf:* Include support for
   `zero configuration networking <https://en.wikipedia.org/wiki/Zero-configuration_networking>`__.

## Image Features

The contents of images generated by the OpenEmbedded build system can be
controlled by the **IMAGE_FEATURES** and
**EXTRA_IMAGE_FEATURES** variables that
you typically configure in your image recipes. Through these variables,
you can add several different predefined packages such as development
utilities or packages with debug information needed to investigate
application problems or profile applications.

The image features available for all images are:

-  *allow-empty-password:* Allows Dropbear and OpenSSH to accept
   logins from accounts having an empty password string.

-  *allow-root-login:* Allows Dropbear and OpenSSH to accept root logins.

-  *dbg-pkgs:* Installs debug symbol packages for all packages installed
   in a given image.

-  *dev-pkgs:* Installs development packages (headers and extra library
   links) for all packages installed in a given image.

-  *doc-pkgs:* Installs documentation packages for all packages
   installed in a given image.

-  *empty-root-password:* This feature can be used if you want to allow root
   login with an empty password. If this feature is not present in
   **IMAGE_FEATURES**, a non-empty password is forced in `/etc/passwd` and
   `/etc/shadow` if such files exist.

   .. note::
       `empty-root-password` doesn't set an empty root password by itself.
       You get an initial empty root password thanks to the
       base-passwd 
       and shadow 
       recipes, and the presence of `empty-root-password` just disables the
       mechanism which forces an non-empty password for the root user.

-  *lic-pkgs:* Installs license packages for all packages installed in a
   given image.

-  *overlayfs-etc:* Configures the `/etc` directory to be in `overlayfs`.
   This allows to store device specific information elsewhere, especially
   if the root filesystem is configured to be read-only.

-  *package-management:* Installs package management tools and preserves
   the package manager database.

-  *post-install-logging:* Enables logging postinstall script runs to
   the `/var/log/postinstall.log` file on first boot of the image on
   the target system.

   .. note::

      To make the `/var/log` directory on the target persistent, remove the
      `files/fs-perms-volatile-log.txt` value from
      **FILESYSTEM_PERMS_TABLES**.

-  *ptest-pkgs:* Installs ptest packages for all ptest-enabled recipes.

-  *read-only-rootfs:* Creates an image whose root filesystem is
   read-only. See the
   "security-manual/read-only-rootfs:creating a read-only root filesystem"
   section in the Yocto Project Development Tasks Manual for more
   information.

-  *read-only-rootfs-delayed-postinsts:* when specified in conjunction
   with `read-only-rootfs`, specifies that post-install scripts are
   still permitted (this assumes that the root filesystem will be made
   writeable for the first boot; this feature does not do anything to
   ensure that - it just disables the check for post-install scripts.)

-  *serial-autologin-root:* when specified in conjunction with
   `empty-root-password` will automatically login as root on the
   serial console. This of course opens up a security hole if the
   serial console is potentially accessible to an attacker, so use
   with caution.

-  *splash:* Enables showing a splash screen during boot. By default,
   this screen is provided by `psplash`, which does allow
   customization. If you prefer to use an alternative splash screen
   package, you can do so by setting the **SPLASH** variable to a
   different package name (or names) within the image recipe or at the
   distro configuration level.

-  *stateless-rootfs:*: specifies that the image should be created as
   stateless - when using `systemd`, `systemctl-native` will not
   be run on the image, leaving the image for population at runtime by
   systemd.

-  *staticdev-pkgs:* Installs static development packages, which are
   static libraries (i.e. `*.a` files), for all packages installed in
   a given image.

Some image features are available only when you inherit the
ref-classes-core-image class. The current list of
these valid features is as follows:

-  *hwcodecs:* Installs hardware acceleration codecs.

-  *nfs-server:* Installs an NFS server.

-  *perf:* Installs profiling tools such as `perf`, `systemtap`, and
   `LTTng`. For general information on user-space tools, see the
   /sdk-manual/index manual.

-  *ssh-server-dropbear:* Installs the Dropbear minimal SSH server.

   .. note::

      As of the 4.1 release, the `ssh-server-dropbear` feature also
      recommends the `openssh-sftp-server` package, which by default
      will be pulled into the image. This is because recent versions of
      the OpenSSH `scp` client now use the SFTP protocol, and thus
      require an SFTP server to be present to connect to. However, if
      you wish to use the Dropbear ssh server `without` the SFTP server
      installed, you can either remove `ssh-server-dropbear` from
      `IMAGE_FEATURES` and add `dropbear` to **IMAGE_INSTALL**
      instead, or alternatively still use the feature but set
      **BAD_RECOMMENDATIONS** as follows::

         BAD_RECOMMENDATIONS += "openssh-sftp-server"

-  *ssh-server-openssh:* Installs the OpenSSH SSH server, which is more
   full-featured than Dropbear. Note that if both the OpenSSH SSH server
   and the Dropbear minimal SSH server are present in
   **IMAGE_FEATURES**, then OpenSSH will take precedence and Dropbear
   will not be installed.

-  *tools-debug:* Installs debugging tools such as `strace` and
   `gdb`. For information on GDB, see the
   "dev-manual/debugging:debugging with the gnu project debugger (gdb) remotely" section
   in the Yocto Project Development Tasks Manual. For information on
   tracing and profiling, see the /profile-manual/index.

-  *tools-sdk:* Installs a full SDK that runs on the device.

-  *tools-testapps:* Installs device testing tools (e.g. touchscreen
   debugging).

-  *weston:* Installs Weston (reference Wayland environment).

-  *x11:* Installs the X server.

-  *x11-base:* Installs the X server with a minimal environment.

-  *x11-sato:* Installs the OpenedHand Sato environment.

## Feature Backfilling

Sometimes it is necessary in the OpenEmbedded build system to
add new functionality to **MACHINE_FEATURES** or
**DISTRO_FEATURES**, but at the same time, allow existing
distributions or machine definitions to opt out of such new
features, to retain the same overall level of functionality.

To make this possible, the OpenEmbedded build system has a mechanism to
automatically "backfill" features into existing distro or machine
configurations. You can see the list of features for which this is done
by checking the **DISTRO_FEATURES_BACKFILL** and
**MACHINE_FEATURES_BACKFILL** variables in the
`meta/conf/bitbake.conf` file.

These two variables are paired with the
**DISTRO_FEATURES_BACKFILL_CONSIDERED**
and **MACHINE_FEATURES_BACKFILL_CONSIDERED** variables
which allow distro or machine configuration maintainers to `consider` any
added feature, and decide when they wish to keep or exclude such feature,
thus preventing the backfilling from happening.

Here are two examples to illustrate feature backfilling:

-  *The "pulseaudio" distro feature option*: Previously, PulseAudio support was
   enabled within the Qt and GStreamer frameworks. Because of this, the feature
   is now backfilled and thus enabled for all distros through the
   **DISTRO_FEATURES_BACKFILL** variable in the `meta/conf/bitbake.conf`
   file. However, if your distro needs to disable the feature, you can do so
   without affecting other existing distro configurations that need PulseAudio
   support. You do this by adding "pulseaudio" to
   **DISTRO_FEATURES_BACKFILL_CONSIDERED** in your distro's `.conf`
   file. So, adding the feature to this variable when it also exists in the
   **DISTRO_FEATURES_BACKFILL** variable prevents the build system from
   adding the feature to your configuration's **DISTRO_FEATURES**,
   effectively disabling the feature for that particular distro.

-  *The "rtc" machine feature option*: Previously, real time clock (RTC)
   support was enabled for all target devices. Because of this, the
   feature is backfilled and thus enabled for all machines through the
   **MACHINE_FEATURES_BACKFILL** variable in the `meta/conf/bitbake.conf`
   file. However, if your target device does not have this capability, you can
   disable RTC support for your device without affecting other machines
   that need RTC support. You do this by adding the "rtc" feature to the
   **MACHINE_FEATURES_BACKFILL_CONSIDERED** list in your machine's `.conf`
   file. So, adding the feature to this variable when it also exists in the
   **MACHINE_FEATURES_BACKFILL** variable prevents the build system from
   adding the feature to your configuration's **MACHINE_FEATURES**,
   effectively disabling RTC support for that particular machine.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Variables Glossary

This chapter lists common variables used in the OpenEmbedded build
system and gives an overview of their function and contents.

   check_glossary_begin

**A <ABIEXTENSION>** **B** **C <CACHE>**
**D** **E <EFI_ARCH>** **F <FAKEROOT>**
**G <GCCPIE>** **H <HGDIR>** **I <IMAGE_BASENAME>**
**K <KARCH>** **L <LABELS>** **M <MACHINE>**
**N <NATIVELSBSTRING>** **O <OBJCOPY>** **P**
**Q <QA_EMPTY_DIRS>** **R <RANLIB>** **S** **T**
**U <UBOOT_BINARY>** **V <VIRTUAL-RUNTIME>**
**W <WARN_QA>** **X <XSERVER>** **Z <ZSTD_COMPRESSION_LEVEL>**

   check_glossary_end

.. glossary::
   :sorted:

   **ABIEXTENSION**
      Extension to the Application Binary Interface (ABI) field of the GNU
      canonical architecture name (e.g. "eabi").

      ABI extensions are set in the machine include files. For example, the
      `meta/conf/machine/include/arm/arch-arm.inc` file sets the
      following extension::

         ABIEXTENSION = "eabi"

   **ALLOW_EMPTY**
      Specifies whether to produce an output package even if it is empty.
      By default, BitBake does not produce empty packages. This default
      behavior can cause issues when there is an
      **RDEPENDS** or some other hard runtime
      requirement on the existence of the package.

      Like all package-controlling variables, you must always use them in
      conjunction with a package name override, as in::

         ALLOW_EMPTY:${PN} = "1"
         ALLOW_EMPTY:${PN}-dev = "1"
         ALLOW_EMPTY:${PN}-staticdev = "1"

   **ALTERNATIVE**
      Lists commands in a package that need an alternative binary naming
      scheme. Sometimes the same command is provided in multiple packages.
      When this occurs, the OpenEmbedded build system needs to use the
      alternatives system to create a different binary naming scheme so the
      commands can co-exist.

      To use the variable, list out the package's commands that are also
      provided by another package. For example, if the `busybox` package
      has four such commands, you identify them as follows::

         ALTERNATIVE:busybox = "sh sed test bracket"

      For more information on the alternatives system, see the
      "ref-classes-update-alternatives"
      section.

   **ALTERNATIVE_LINK_NAME**
      Used by the alternatives system to map duplicated commands to actual
      locations. For example, if the `bracket` command provided by the
      `busybox` package is duplicated through another package, you must
      use the **ALTERNATIVE_LINK_NAME** variable to specify the actual
      location::

         ALTERNATIVE_LINK_NAME[bracket] = "/usr/bin/["

      In this example, the binary for the `bracket` command (i.e. `[`)
      from the `busybox` package resides in `/usr/bin/`.

      .. note::

         If **ALTERNATIVE_LINK_NAME** is not defined, it defaults to `${bindir}/name`.

      For more information on the alternatives system, see the
      "ref-classes-update-alternatives"
      section.

   **ALTERNATIVE_PRIORITY**
      Used by the alternatives system to create default priorities for
      duplicated commands. You can use the variable to create a single
      default regardless of the command name or package, a default for
      specific duplicated commands regardless of the package, or a default
      for specific commands tied to particular packages. Here are the
      available syntax forms::

         ALTERNATIVE_PRIORITY = "priority"
         ALTERNATIVE_PRIORITY[name] = "priority"
         ALTERNATIVE_PRIORITY_pkg[name] = "priority"

      For more information on the alternatives system, see the
      "ref-classes-update-alternatives"
      section.

   **ALTERNATIVE_TARGET**
      Used by the alternatives system to create default link locations for
      duplicated commands. You can use the variable to create a single
      default location for all duplicated commands regardless of the
      command name or package, a default for specific duplicated commands
      regardless of the package, or a default for specific commands tied to
      particular packages. Here are the available syntax forms::

         ALTERNATIVE_TARGET = "target"
         ALTERNATIVE_TARGET[name] = "target"
         ALTERNATIVE_TARGET_pkg[name] = "target"

      .. note::

         If **ALTERNATIVE_TARGET** is not defined, it inherits the value
         from the **ALTERNATIVE_LINK_NAME** variable.

         If **ALTERNATIVE_LINK_NAME** and **ALTERNATIVE_TARGET** are the
         same, the target for **ALTERNATIVE_TARGET** has "`.{BPN}`"
         appended to it.

         Finally, if the file referenced has not been renamed, the
         alternatives system will rename it to avoid the need to rename
         alternative files in the ref-tasks-install
         task while retaining support for the command if necessary.

      For more information on the alternatives system, see the
      "ref-classes-update-alternatives" section.

   **ANY_OF_DISTRO_FEATURES**
      When inheriting the ref-classes-features_check
      class, this variable identifies a list of distribution features where
      at least one must be enabled in the current configuration in order
      for the OpenEmbedded build system to build the recipe. In other words,
      if none of the features listed in **ANY_OF_DISTRO_FEATURES**
      appear in **DISTRO_FEATURES** within the current configuration, then
      the recipe will be skipped, and if the build system attempts to build
      the recipe then an error will be triggered.

   **APPEND**
      An override list of append strings for each target specified with
      **LABELS**.

      See the ref-classes-grub-efi class for more
      information on how this variable is used.

   **AR**
      The minimal command and arguments used to run `ar <ar(1)>`.

   **ARCHIVER_MODE**
      When used with the ref-classes-archiver class,
      determines the type of information used to create a released archive.
      You can use this variable to create archives of patched source,
      original source, configured source, and so forth by employing the
      following variable flags (varflags)::

         ARCHIVER_MODE[src] = "original"                   # Uses original (unpacked) source files.
         ARCHIVER_MODE[src] = "patched"                    # Uses patched source files. This is the default.
         ARCHIVER_MODE[src] = "configured"                 # Uses configured source files.
         ARCHIVER_MODE[diff] = "1"                         # Uses patches between do_unpack and do_patch.
         ARCHIVER_MODE[diff-exclude] ?= "file file ..."    # Lists files and directories to exclude from diff.
         ARCHIVER_MODE[dumpdata] = "1"                     # Uses environment data.
         ARCHIVER_MODE[recipe] = "1"                       # Uses recipe and include files.
         ARCHIVER_MODE[srpm] = "1"                         # Uses RPM package files.

      For information on how the variable works, see the
      `meta/classes/archiver.bbclass` file in :term:`OpenEmbedded-Core
      (OE-Core)`.

   **AS**
      Minimal command and arguments needed to run the :manpage:`assembler
      <as(1)>`.

   **ASSUME_PROVIDED**
      Lists recipe names (**PN** values) BitBake does not
      attempt to build. Instead, BitBake assumes these recipes have already
      been built.

      In OpenEmbedded-Core, **ASSUME_PROVIDED** mostly specifies native
      tools that should not be built. An example is `git-native`, which
      when specified, allows for the Git binary from the host to be used
      rather than building `git-native`.

   **ASSUME_SHLIBS**
      Provides additional `shlibs` provider mapping information, which
      adds to or overwrites the information provided automatically by the
      system. Separate multiple entries using spaces.

      As an example, use the following form to add an `shlib` provider of
      shlibname in packagename with the optional version::

         shlibname:packagename[_version]

      Here is an example that adds a shared library named `libEGL.so.1`
      as being provided by the `libegl-implementation` package::

         ASSUME_SHLIBS = "libEGL.so.1:libegl-implementation"

   **AUTO_LIBNAME_PKGS**
      When the ref-classes-debian class is inherited,
      which is the default behavior, **AUTO_LIBNAME_PKGS** specifies which
      packages should be checked for libraries and renamed according to
      Debian library package naming.

      The default value is "${PACKAGES}", which causes the
      ref-classes-debian class to act on all packages that are
      explicitly generated by the recipe.

   **AUTO_SYSLINUXMENU**
      Enables creating an automatic menu for the syslinux bootloader. You
      must set this variable in your recipe. The
      ref-classes-syslinux class checks this variable.

   **AUTOREV**
      When **SRCREV** is set to the value of this variable, it specifies to
      use the latest source revision in the repository. Here is an example::

         SRCREV = "${AUTOREV}"

      If you use the previous statement to retrieve the latest version of
      software, you need to make sure **PV** contains the `+` sign so
      **bitbake** includes source control information to **PKGV** when
      packaging the recipe. For example::

         PV = "6.10.y+git"

      For more information see the
      "dev-manual/packages:automatically incrementing a package version number"
      section in the Yocto Project Development Tasks Manual.

   **AUTOTOOLS_SCRIPT_PATH**
      When using the ref-classes-autotools class, the
      **AUTOTOOLS_SCRIPT_PATH** variable stores the location of the
      different scripts used by the Autotools build system. The default
      value for this variable is **S**.

   **AVAILTUNES**
      The list of defined CPU and Application Binary Interface (ABI)
      tunings (i.e. "tunes") available for use by the OpenEmbedded build
      system.

      The list simply presents the tunes that are available. Not all tunes
      may be compatible with a particular machine configuration, or with
      each other in a
      Multilib 
      configuration.

      To add a tune to the list, be sure to append it with spaces using the
      "+=" BitBake operator. Do not simply replace the list by using the
      "=" operator. See the
      "bitbake-user-manual/bitbake-user-manual-metadata:basic syntax" section in the BitBake
      User Manual for more information.

   **AZ_SAS**
      Azure Storage Shared Access Signature, when using the
      Azure Storage fetcher (az://) 
      This variable can be defined to be used by the fetcher to authenticate
      and gain access to non-public artifacts::

         AZ_SAS = ""se=2021-01-01&sp=r&sv=2018-11-09&sr=c&skoid=<skoid>&sig=<signature>""

      For more information see Microsoft's Azure Storage documentation at
      https://docs.microsoft.com/en-us/azure/storage/common/storage-sas-overview

   **B**
      The directory within the **Build Directory** in which the
      OpenEmbedded build system places generated objects during a recipe's
      build process. By default, this directory is the same as the
      **S** directory, which is defined as::

         S = "${UNPACKDIR}/${BP}"

      You can separate the (**S**) directory and the directory pointed to
      by the **B** variable. Most Autotools-based recipes support
      separating these directories. The build system defaults to using
      separate directories for `gcc` and some kernel recipes.

   **BAD_RECOMMENDATIONS**
      Lists "recommended-only" packages to not install. Recommended-only
      packages are packages installed only through the
      **RRECOMMENDS** variable. You can prevent any
      of these "recommended" packages from being installed by listing them
      with the **BAD_RECOMMENDATIONS** variable::

         BAD_RECOMMENDATIONS = "package_name package_name package_name ..."

      You can set this variable globally in your `local.conf` file or you
      can attach it to a specific image recipe by using the recipe name
      override::

         BAD_RECOMMENDATIONS:pn-target_image = "package_name"

      It is important to realize that if you choose to not install packages
      using this variable and some other packages are dependent on them
      (i.e. listed in a recipe's **RDEPENDS**
      variable), the OpenEmbedded build system ignores your request and
      will install the packages to avoid dependency errors.

      See the **NO_RECOMMENDATIONS** and the
      **PACKAGE_EXCLUDE** variables for related
      information.

   **BAREBOX_BINARY**
      When using the ref-classes-barebox class, this variable allows you
      to specify a particular binary that should be deployed and installed.

      The barebox build system can build multiple barebox binaries at once.
      By default, all built binaries will be deployed and installed under their
      original name.

      Here is an example usage of this variable::

         BAREBOX_BINARY = "barebox-boundarydevices-imx6dl-nitrogen6x-1g.img"

   **BAREBOX_CONFIG**
      When using the ref-classes-barebox class, this variable allows you
      to specify the name of the barebox defconfig to build.
      The name must be a defconfig file known to the barebox build environment.
      This variable is mainly useful for generic use cases where a dedicated
      configuration is not required.
      The ref-classes-barebox class itself already sets it for some QEMU
      machines::

         BAREBOX_CONFIG:qemuarm = "multi_v7_defconfig"
         BAREBOX_CONFIG:qemuarm64 = "multi_v8_defconfig"
         BAREBOX_CONFIG:qemux86-64 = "efi_defconfig"

      Except for these, the default value of **BAREBOX_CONFIG** is empty.
      For more information on how to provide a barebox configuration, see the
      ref-classes-barebox class.

   **BASE_LIB**
      The library directory name for the CPU or Application Binary
      Interface (ABI) tune. The **BASE_LIB** applies only in the Multilib
      context. See the "dev-manual/libraries:combining multiple versions of library files into one image"
      section in the Yocto Project Development Tasks Manual for information
      on Multilib.

      The **BASE_LIB** variable is defined in the machine include files in
      **OpenEmbedded-Core (OE-Core)**. If Multilib is not
      being used, the value defaults to "lib".

   **BASE_WORKDIR**
      Points to the base of the work directory for all recipes. The default
      value is "${TMPDIR}/work".

   **BB_ALLOWED_NETWORKS**
      Specifies a space-delimited list of hosts that the fetcher is allowed
      to use to obtain the required source code. Here are
      considerations surrounding this variable:

      -  This host list is only used if **BB_NO_NETWORK** is either not set
         or set to "0".

      -  There is limited support for wildcard matching against the beginning of
         host names. For example, the following setting matches
         `git.gnu.org`, `ftp.gnu.org`, and `foo.git.gnu.org`::

            BB_ALLOWED_NETWORKS = "*.gnu.org"

         .. note::

            The use of the "`*`" character only works at the beginning of
            a host name and it must be isolated from the remainder of the
            host name. You cannot use the wildcard character in any other
            location of the name or combined with the front part of the
            name.

            For example, `*.foo.bar` is supported, while `*aa.foo.bar`
            is not.

      -  Mirrors not in the host list are skipped and logged in debug.

      -  Attempts to access networks not in the host list cause a failure.

      Using **BB_ALLOWED_NETWORKS** in conjunction with
      **PREMIRRORS** is very useful. Adding the host
      you want to use to **PREMIRRORS** results in the source code being
      fetched from an allowed location and avoids raising an error when a
      host that is not allowed is in a **SRC_URI**
      statement. This is because the fetcher does not attempt to use the
      host listed in **SRC_URI** after a successful fetch from the
      **PREMIRRORS** occurs.

   **BB_BASEHASH_IGNORE_VARS**
      See **bitbake:BB_BASEHASH_IGNORE_VARS** in the BitBake manual.

   **BB_CACHEDIR**
      See **bitbake:BB_CACHEDIR** in the BitBake manual.

   **BB_CHECK_SSL_CERTS**
      See **bitbake:BB_CHECK_SSL_CERTS** in the BitBake manual.

   **BB_CONF_FRAGMENT_DESCRIPTION**
      The **BB_CONF_FRAGMENT_DESCRIPTION** variable defines the textual
      description of a **Configuration Fragment**. For details on how to use
      fragments, see the /ref-manual/fragments section of the Yocto
      Project Reference Manual.

   **BB_CONF_FRAGMENT_SUMMARY**
      The **BB_CONF_FRAGMENT_SUMMARY** variable defines the one-line textual
      summary of a **Configuration Fragment**. For details on how to use
      fragments, see the /ref-manual/fragments section of the Yocto
      Project Reference Manual.

   **BB_CONSOLELOG**
      See **bitbake:BB_CONSOLELOG** in the BitBake manual.

   **BB_CURRENT_MC**
      See **bitbake:BB_CURRENT_MC** in the BitBake manual.

   **BB_CURRENTTASK**
      See **bitbake:BB_CURRENTTASK** in the BitBake manual.

   **BB_DEFAULT_TASK**
      See **bitbake:BB_DEFAULT_TASK** in the BitBake manual.

   **BB_DEFAULT_UMASK**
      See **bitbake:BB_DEFAULT_UMASK** in the BitBake manual.

   **BB_DEFER_BBCLASSES**
      See **bitbake:BB_DEFER_BBCLASSES** in the BitBake manual.

   **BB_DISKMON_DIRS**
      Monitors disk space and available inodes during the build and allows
      you to control the build based on these parameters.

      Disk space monitoring is disabled by default. To enable monitoring,
      add the **BB_DISKMON_DIRS** variable to your `conf/local.conf` file
      found in the **Build Directory**. Use the
      following form:

      .. code-block:: none

         BB_DISKMON_DIRS = "action,dir,threshold [...]"

         where:

            action is:
               ABORT:     Immediately stop the build when
                          a threshold is broken.
               STOPTASKS: Stop the build after the currently
                          executing tasks have finished when
                          a threshold is broken.
               WARN:      Issue a warning but continue the
                          build when a threshold is broken.
                          Subsequent warnings are issued as
                          defined by the BB_DISKMON_WARNINTERVAL
                          variable, which must be defined in
                          the conf/local.conf file.

            dir is:
               Any directory you choose. You can specify one or
               more directories to monitor by separating the
               groupings with a space.  If two directories are
               on the same device, only the first directory
               is monitored.

            threshold is:
               Either the minimum available disk space,
               the minimum number of free inodes, or
               both.  You must specify at least one.  To
               omit one or the other, simply omit the value.
               Specify the threshold using G, M, K for Gbytes,
               Mbytes, and Kbytes, respectively. If you do
               not specify G, M, or K, Kbytes is assumed by
               default.  Do not use GB, MB, or KB.

      Here are some examples::

         BB_DISKMON_DIRS = "ABORT,${TMPDIR},1G,100K WARN,${SSTATE_DIR},1G,100K"
         BB_DISKMON_DIRS = "STOPTASKS,${TMPDIR},1G"
         BB_DISKMON_DIRS = "ABORT,${TMPDIR},,100K"

      The first example works only if you also provide the
      **BB_DISKMON_WARNINTERVAL**
      variable in the `conf/local.conf`. This example causes the build
      system to immediately stop when either the disk space in
      `${TMPDIR}` drops below 1 Gbyte or the available free inodes drops
      below 100 Kbytes. Because two directories are provided with the
      variable, the build system also issue a warning when the disk space
      in the `${SSTATE_DIR}` directory drops below 1 Gbyte or the number
      of free inodes drops below 100 Kbytes. Subsequent warnings are issued
      during intervals as defined by the **BB_DISKMON_WARNINTERVAL**
      variable.

      The second example stops the build after all currently executing
      tasks complete when the minimum disk space in the `${TMPDIR}`
      directory drops below 1 Gbyte. No disk monitoring occurs for the free
      inodes in this case.

      The final example immediately stops the build when the number of
      free inodes in the `${TMPDIR}` directory drops below 100 Kbytes. No
      disk space monitoring for the directory itself occurs in this case.

   **BB_DISKMON_WARNINTERVAL**
      Defines the disk space and free inode warning intervals. To set these
      intervals, define the variable in your `conf/local.conf` file in
      the **Build Directory**.

      If you are going to use the **BB_DISKMON_WARNINTERVAL** variable, you
      must also use the **BB_DISKMON_DIRS**
      variable and define its action as "WARN". During the build,
      subsequent warnings are issued each time disk space or number of free
      inodes further reduces by the respective interval.

      If you do not provide a **BB_DISKMON_WARNINTERVAL** variable and you
      do use **BB_DISKMON_DIRS** with the "WARN" action, the disk
      monitoring interval defaults to the following::

         BB_DISKMON_WARNINTERVAL = "50M,5K"

      When specifying the variable in your configuration file, use the
      following form:

      .. code-block:: none

         BB_DISKMON_WARNINTERVAL = "disk_space_interval,disk_inode_interval"

         where:

            disk_space_interval is:
               An interval of memory expressed in either
               G, M, or K for Gbytes, Mbytes, or Kbytes,
               respectively. You cannot use GB, MB, or KB.

            disk_inode_interval is:
               An interval of free inodes expressed in either
               G, M, or K for Gbytes, Mbytes, or Kbytes,
               respectively. You cannot use GB, MB, or KB.

      Here is an example::

         BB_DISKMON_DIRS = "WARN,${SSTATE_DIR},1G,100K"
         BB_DISKMON_WARNINTERVAL = "50M,5K"

      These variables cause the
      OpenEmbedded build system to issue subsequent warnings each time the
      available disk space further reduces by 50 Mbytes or the number of
      free inodes further reduces by 5 Kbytes in the `${SSTATE_DIR}`
      directory. Subsequent warnings based on the interval occur each time
      a respective interval is reached beyond the initial warning (i.e. 1
      Gbytes and 100 Kbytes).

   **BB_ENV_PASSTHROUGH**
      See **bitbake:BB_ENV_PASSTHROUGH** in the BitBake manual.

   **BB_ENV_PASSTHROUGH_ADDITIONS**
      See **bitbake:BB_ENV_PASSTHROUGH_ADDITIONS** in the BitBake manual.

   **BB_FETCH_PREMIRRORONLY**
      See **bitbake:BB_FETCH_PREMIRRORONLY** in the BitBake manual.

   **BB_FILENAME**
      See **bitbake:BB_FILENAME** in the BitBake manual.

   **BB_GENERATE_MIRROR_TARBALLS**
      Causes tarballs of the source control repositories (e.g. Git
      repositories), including metadata, to be placed in the
      **DL_DIR** directory.

      For performance reasons, creating and placing tarballs of these
      repositories is not the default action by the OpenEmbedded build
      system::

         BB_GENERATE_MIRROR_TARBALLS = "1"

      Set this variable in your
      `local.conf` file in the **Build Directory**.

      Once you have the tarballs containing your source files, you can
      clean up your **DL_DIR** directory by deleting any Git or other
      source control work directories.

   **BB_GENERATE_SHALLOW_TARBALLS**
      See **bitbake:BB_GENERATE_SHALLOW_TARBALLS** in the BitBake manual.

   **BB_GIT_DEFAULT_DESTSUFFIX**
      See **bitbake:BB_GIT_DEFAULT_DESTSUFFIX** in the BitBake manual.

      In **OpenEmbedded-Core (OE-Core)**, this variable is set to
      **BP** by default in :oe_git:`bitbake.conf
      </openembedded-core/tree/meta/conf/bitbake.conf>`.

   **BB_GIT_SHALLOW**
      See **bitbake:BB_GIT_SHALLOW** in the BitBake manual.

   **BB_GIT_SHALLOW_DEPTH**
      See **bitbake:BB_GIT_SHALLOW_DEPTH** in the BitBake manual.

   **BB_HASHCHECK_FUNCTION**
      See **bitbake:BB_HASHCHECK_FUNCTION** in the BitBake manual.

   **BB_HASHCONFIG_IGNORE_VARS**
      See **bitbake:BB_HASHCONFIG_IGNORE_VARS** in the BitBake manual.

   **BB_HASHSERVE**
      See **bitbake:BB_HASHSERVE** in the BitBake manual.

   **BB_HASHSERVE_UPSTREAM**
      See **bitbake:BB_HASHSERVE_UPSTREAM** in the BitBake manual.

   **BB_INVALIDCONF**
      See **bitbake:BB_INVALIDCONF** in the BitBake manual.

   **BB_LOADFACTOR_MAX**
      The system load threshold above which **BitBake** will stop running
      extra tasks.

      For more information on how to limit the resources used during builds, see
      the /dev-manual/limiting-resources section of the Yocto Project
      Development Tasks Manual.

   **BB_LOGCONFIG**
      See **bitbake:BB_LOGCONFIG** in the BitBake manual.

   **BB_LOGFMT**
      See **bitbake:BB_LOGFMT** in the BitBake manual.

   **BB_MULTI_PROVIDER_ALLOWED**
      See **bitbake:BB_MULTI_PROVIDER_ALLOWED** in the BitBake manual.

   **BB_NICE_LEVEL**
      See **bitbake:BB_NICE_LEVEL** in the BitBake manual.

   **BB_NO_NETWORK**
      See **bitbake:BB_NO_NETWORK** in the BitBake manual.

   **BB_NUMBER_PARSE_THREADS**
      See **bitbake:BB_NUMBER_PARSE_THREADS** in the BitBake manual.

   **BB_NUMBER_THREADS**
      The maximum number of tasks BitBake should run in parallel at any one
      time. The OpenEmbedded build system automatically configures this
      variable to be equal to the number of cores on the build system. For
      example, a system with a dual core processor that also uses
      hyper-threading causes the **BB_NUMBER_THREADS** variable to default
      to "4".

      For single socket systems (i.e. one CPU), you should not have to
      override this variable to gain optimal parallelism during builds.
      However, if you have very large systems that employ multiple physical
      CPUs, you might want to make sure the **BB_NUMBER_THREADS** variable
      is not set higher than "20".

      For more information on speeding up builds, see the
      "dev-manual/speeding-up-build:speeding up a build"
      section in the Yocto Project Development Tasks Manual.

      On the other hand, if your goal is to limit the amount of system
      resources consumed by BitBake tasks, setting **BB_NUMBER_THREADS**
      to a number lower than the number of CPU threads in your machine
      won't be sufficient. That's because each package will still be built
      and installed through a number of parallel jobs specified by the
      **PARALLEL_MAKE** variable, which is by default the number of CPU
      threads in your system, and is not impacted by the
      **BB_NUMBER_THREADS** value.

      So, if you set **BB_NUMBER_THREADS** to "1" but don't set
      **PARALLEL_MAKE**, most of your system resources will be consumed
      anyway.

      Therefore, if you intend to reduce the load of your build system by
      setting **BB_NUMBER_THREADS** to a relatively low value compared
      to the number of CPU threads on your system, you should also set
      **PARALLEL_MAKE** to a similarly low value.

      An alternative to using **BB_NUMBER_THREADS** to keep the usage
      of build system resources under control is to use the smarter
      **BB_PRESSURE_MAX_CPU**, **BB_PRESSURE_MAX_IO** or
      **BB_PRESSURE_MAX_MEMORY** controls. See the
      /dev-manual/limiting-resources section of the Yocto Project
      Development Tasks Manual.

   **BB_ORIGENV**
      See **bitbake:BB_ORIGENV** in the BitBake manual.

   **BB_PRESERVE_ENV**
      See **bitbake:BB_PRESERVE_ENV** in the BitBake manual.

   **BB_PRESSURE_MAX_CPU**
      See **bitbake:BB_PRESSURE_MAX_CPU** in the BitBake manual.

      For more information on how to limit the resources used during builds, see
      the /dev-manual/limiting-resources section of the Yocto Project
      Development Tasks Manual.

   **BB_PRESSURE_MAX_IO**
      See **bitbake:BB_PRESSURE_MAX_IO** in the BitBake manual.

      For more information on how to limit the resources used during builds, see
      the /dev-manual/limiting-resources section of the Yocto Project
      Development Tasks Manual.

   **BB_PRESSURE_MAX_MEMORY**
      See **bitbake:BB_PRESSURE_MAX_MEMORY** in the BitBake manual.

      For more information on how to limit the resources used during builds, see
      the /dev-manual/limiting-resources section of the Yocto Project
      Development Tasks Manual.

   **BB_RUNFMT**
      See **bitbake:BB_RUNFMT** in the BitBake manual.

   **BB_RUNTASK**
      See **bitbake:BB_RUNTASK** in the BitBake manual.

   **BB_SCHEDULER**
      See **bitbake:BB_SCHEDULER** in the BitBake manual.

   **BB_SCHEDULERS**
      See **bitbake:BB_SCHEDULERS** in the BitBake manual.

   **BB_SERVER_TIMEOUT**
      Specifies the time (in seconds) after which to unload the BitBake
      server due to inactivity. Set **BB_SERVER_TIMEOUT** to determine how
      long the BitBake server stays resident between invocations.

      For example, the following statement in your `local.conf` file
      instructs the server to be unloaded after 20 seconds of inactivity::

         BB_SERVER_TIMEOUT = "20"

      If you want the server to never be unloaded,
      set **BB_SERVER_TIMEOUT** to "-1".

   **BB_SETSCENE_DEPVALID**
      See **bitbake:BB_SETSCENE_DEPVALID** in the BitBake manual.

   **BB_SIGNATURE_EXCLUDE_FLAGS**
      See **bitbake:BB_SIGNATURE_EXCLUDE_FLAGS** in the BitBake manual.

   **BB_SIGNATURE_HANDLER**
      See **bitbake:BB_SIGNATURE_HANDLER** in the BitBake manual.

   **BB_SRCREV_POLICY**
      See **bitbake:BB_SRCREV_POLICY** in the BitBake manual.

   **BB_STRICT_CHECKSUM**
      See **bitbake:BB_STRICT_CHECKSUM** in the BitBake manual.

   **BB_TASK_IONICE_LEVEL**
      See **bitbake:BB_TASK_IONICE_LEVEL** in the BitBake manual.

   **BB_TASK_NICE_LEVEL**
      See **bitbake:BB_TASK_NICE_LEVEL** in the BitBake manual.

   **BB_TASKHASH**
      See **bitbake:BB_TASKHASH** in the BitBake manual.

   **BB_USE_HOME_NPMRC**
      See **bitbake:BB_USE_HOME_NPMRC** in the BitBake manual.

   **BB_VERBOSE_LOGS**
      See **bitbake:BB_VERBOSE_LOGS** in the BitBake manual.

   **BB_WORKERCONTEXT**
      See **bitbake:BB_WORKERCONTEXT** in the BitBake manual.

   **BBCLASSEXTEND**
      Allows you to extend a recipe so that it builds variants of the
      software. There are common variants for recipes as "natives" like
      `quilt-native`, which is a copy of Quilt built to run on the build
      system; "crosses" such as `gcc-cross`, which is a compiler built to
      run on the build machine but produces binaries that run on the target
      **MACHINE**; "ref-classes-nativesdk", which
      targets the SDK machine instead of **MACHINE**; and "mulitlibs" in
      the form "`multilib:`\ multilib_name".

      To build a different variant of the recipe with a minimal amount of
      code, it usually is as simple as adding the following to your recipe::

         BBCLASSEXTEND =+ "native nativesdk"
         BBCLASSEXTEND =+ "multilib:multilib_name"

      .. note::

         Internally, the **BBCLASSEXTEND** mechanism generates recipe
         variants by rewriting variable values and applying overrides such
         as `:class-native`. For example, to generate a native version of
         a recipe, a **DEPENDS** on "foo" is rewritten
         to a **DEPENDS** on "foo-native".

         Even when using **BBCLASSEXTEND**, the recipe is only parsed once.
         Parsing once adds some limitations. For example, it is not
         possible to include a different file depending on the variant,
         since `include` statements are processed when the recipe is
         parsed.

   **BBDEBUG**
      See **bitbake:BBDEBUG** in the BitBake manual.

   **BBFILE_COLLECTIONS**
      Lists the names of configured layers. These names are used to find
      the other `BBFILE_*` variables. Typically, each layer will append
      its name to this variable in its `conf/layer.conf` file.

   **BBFILE_PATTERN**
      Variable that expands to match files from
      **BBFILES** in a particular layer. This variable
      is used in the `conf/layer.conf` file and must be suffixed with the
      name of the specific layer (e.g. `BBFILE_PATTERN_emenlow`).

   **BBFILE_PRIORITY**
      Assigns the priority for recipe files in each layer.

      This variable is useful in situations where the same recipe appears
      in more than one layer. Setting this variable allows you to
      prioritize a layer against other layers that contain the same recipe
      --- effectively letting you control the precedence for the multiple
      layers. The precedence established through this variable stands
      regardless of a recipe's version (**PV** variable). For
      example, a layer that has a recipe with a higher **PV** value but for
      which the **BBFILE_PRIORITY** is set to have a lower precedence still
      has a lower precedence.

      A larger value for the **BBFILE_PRIORITY** variable results in a
      higher precedence. For example, the value 6 has a higher precedence
      than the value 5. If not specified, the **BBFILE_PRIORITY** variable
      is set based on layer dependencies (see the **LAYERDEPENDS** variable
      for more information. The default priority, if unspecified for a
      layer with no dependencies, is the lowest defined priority + 1 (or 1
      if no priorities are defined).

      .. tip::

         You can use the command `bitbake-layers show-layers`
         to list all configured layers along with their priorities.

   **BBFILES**
      A space-separated list of recipe files BitBake uses to build
      software.

      When specifying recipe files, you can pattern match using Python's
      `glob <https://docs.python.org/3/library/glob.html>`__ syntax.
      For details on the syntax, see the documentation by following the
      previous link.

   **BBFILES_DYNAMIC**
      Activates content when identified layers are present. You identify
      the layers by the collections that the layers define.

      Use the **BBFILES_DYNAMIC** variable to avoid `.bbappend` files
      whose corresponding `.bb` file is in a layer that attempts to
      modify other layers through `.bbappend` but does not want to
      introduce a hard dependency on those other layers.

      Use the following form for **BBFILES_DYNAMIC**:
      `collection_name:filename_pattern`.

      The following example identifies two collection names and two
      filename patterns::

         BBFILES_DYNAMIC += " \
            clang-layer:${LAYERDIR}/bbappends/meta-clang/*/*/*.bbappend \
            core:${LAYERDIR}/bbappends/openembedded-core/meta/*/*/*.bbappend \
            "

      This next example shows an error message that occurs because invalid
      entries are found, which cause parsing to fail:

      .. code-block:: none

         ERROR: BBFILES_DYNAMIC entries must be of the form <collection name>:<filename pattern>, not:
             /work/my-layer/bbappends/meta-security-isafw/*/*/*.bbappend
             /work/my-layer/bbappends/openembedded-core/meta/*/*/*.bbappend

   **BBINCLUDED**
      See **bitbake:BBINCLUDED** in the BitBake manual.

   **BBINCLUDELOGS**
      Variable that controls how BitBake displays logs on build failure.

   **BBINCLUDELOGS_LINES**
      If **BBINCLUDELOGS** is set, specifies the
      maximum number of lines from the task log file to print when
      reporting a failed task. If you do not set **BBINCLUDELOGS_LINES**,
      the entire log is printed.

   **BBLAYERS**
      Lists the layers to enable during the build. This variable is defined
      in the `bblayers.conf` configuration file in the **Build Directory**.
      Here is an example::

         BBLAYERS = " \
             /home/scottrif/bitbake-builds/layers/meta \
             /home/scottrif/bitbake-builds/layers/meta-poky \
             /home/scottrif/bitbake-builds/layers/meta-yocto-bsp \
             /home/scottrif/bitbake-builds/layers/meta-mykernel \
             "

      This example enables four layers, one of which is a custom,
      user-defined layer named `meta-mykernel`.

   **BBLAYERS_FETCH_DIR**
      See **bitbake:BBLAYERS_FETCH_DIR** in the BitBake manual.

   **BBMASK**
      Prevents BitBake from processing recipes and recipe append files.

      You can use the **BBMASK** variable to "hide" these `.bb` and
      `.bbappend` files. BitBake ignores any recipe or recipe append
      files that match any of the expressions. It is as if BitBake does not
      see them at all. Consequently, matching files are not parsed or
      otherwise used by BitBake.

      The values you provide are passed to Python's regular expression
      compiler. Consequently, the syntax follows Python's Regular
      Expression (re) syntax. The expressions are compared against the full
      paths to the files. For complete syntax information, see Python's
      documentation at https://docs.python.org/3/library/re.html#regular-expression-syntax.

      The following example uses a complete regular expression to tell
      BitBake to ignore all recipe and recipe append files in the
      `meta-ti/recipes-misc/` directory::

         BBMASK = "meta-ti/recipes-misc/"

      If you want to mask out multiple directories or recipes, you can
      specify multiple regular expression fragments. This next example
      masks out multiple directories and individual recipes::

         BBMASK += "/meta-ti/recipes-misc/ meta-ti/recipes-ti/packagegroup/"
         BBMASK += "/meta-oe/recipes-support/"
         BBMASK += "/meta-foo/.*/openldap"
         BBMASK += "opencv.*\.bbappend"
         BBMASK += "lzma"

      .. note::

         When specifying a directory name, use the trailing slash character
         to ensure you match just that directory name.

   **BBMULTICONFIG**
      Specifies each additional separate configuration when you are
      building targets with multiple configurations. Use this variable in
      your `conf/local.conf` configuration file. Specify a
      multiconfigname for each configuration file you are using. For
      example, the following line specifies three configuration files::

         BBMULTICONFIG = "configA configB configC"

      Each configuration file you use must reside in a `multiconfig`
      subdirectory of a configuration directory within a layer, or
      within the **Build Directory** (e.g.
      `build_directory/conf/multiconfig/configA.conf` or
      `mylayer/conf/multiconfig/configB.conf`).

      For information on how to use **BBMULTICONFIG** in an environment
      that supports building targets with multiple configurations, see the
      "dev-manual/building:building images for multiple targets using multiple configurations"
      section in the Yocto Project Development Tasks Manual.

   **BBPATH**
      See **bitbake:BBPATH** in the BitBake manual.

   **BBSERVER**
      If defined in the BitBake environment, **BBSERVER** points to the
      BitBake remote server.

      Use the following format to export the variable to the BitBake
      environment::

         export BBSERVER=localhost:$port

      By default, **BBSERVER** also appears in **BB_BASEHASH_IGNORE_VARS**.
      Consequently, **BBSERVER** is excluded from checksum and dependency
      data.

   **BBTARGETS**
      See **bitbake:BBTARGETS** in the BitBake manual.

   **BINCONFIG**
      When inheriting the ref-classes-binconfig-disabled class, this
      variable specifies binary configuration scripts to disable in favor of
      using `pkg-config` to query the information. The
      ref-classes-binconfig-disabled class will modify the specified
      scripts to return an error so that calls to them can be easily found
      and replaced.

      To add multiple scripts, separate them by spaces. Here is an example
      from the `libpng` recipe::

         BINCONFIG = "${bindir}/libpng-config ${bindir}/libpng16-config"

   **BINCONFIG_GLOB**
      When inheriting the ref-classes-binconfig class,
      this variable specifies a wildcard for configuration scripts that
      need editing. The scripts are edited to correct any paths that have
      been set up during compilation so that they are correct for use when
      installed into the sysroot and called by the build processes of other
      recipes.

      .. note::

         The **BINCONFIG_GLOB** variable uses
         `shell globbing <https://tldp.org/LDP/abs/html/globbingref.html>`__,
         which is recognition and expansion of wildcards during pattern
         matching. Shell globbing is very similar to
         `fnmatch <https://docs.python.org/3/library/fnmatch.html#module-fnmatch>`__
         and `glob <https://docs.python.org/3/library/glob.html>`__.

      For more information on how this variable works, see
      `meta/classes-recipe/binconfig.bbclass` in :term:`OpenEmbedded-Core
      (OE-Core)`. You can also find general
      information on the class in the
      "ref-classes-binconfig" section.

   **BITBAKE_UI**
      See **bitbake:BITBAKE_UI** in the BitBake manual.

   **BP**
      The base recipe name and version but without any special recipe name
      suffix (i.e. `-native`, `lib64-`, and so forth). **BP** is
      comprised of the following::

         ${BPN}-${PV}

   **BPN**
      This variable is a version of the **PN** variable with
      common prefixes and suffixes removed, such as `nativesdk-`,
      `-cross`, `-native`, and multilib's `lib64-` and `lib32-`.
      The exact lists of prefixes and suffixes removed are specified by the
      **MLPREFIX** and
      **SPECIAL_PKGSUFFIX** variables,
      respectively.

   **BUGTRACKER**
      Specifies a URL for an upstream bug tracking website for a recipe.
      The OpenEmbedded build system does not use this variable. Rather, the
      variable is a useful pointer in case a bug in the software being
      built needs to be manually reported.

   **BUILD_AR**
      Specifies the architecture-specific `archiver <ar(1)>` for the
      build host, and its default definition is derived in part from
      **BUILD_PREFIX**::

         BUILD_AR = "${BUILD_PREFIX}ar"

      When building a ref-classes-native recipe, **AR** is set to the
      value of this variable by default.

      The **BUILD_AR** variable should not be set manually, and is rarely
      used in recipes as **AR** contains the appropriate value depending on
      the context (native or target recipes). Exception be made for target
      recipes that need to use the `archiver <ar(1)>` from the build
      host at some point during the build.

   **BUILD_ARCH**
      Specifies the architecture of the build host (e.g. `i686`). The
      OpenEmbedded build system sets the value of **BUILD_ARCH** from the
      machine name reported by the `uname` command.

   **BUILD_AS**
      Specifies the architecture-specific `assembler <as(1)>` for the
      build host, and its default definition is derived in part from
      **BUILD_PREFIX**::

         BUILD_AS = "${BUILD_PREFIX}as ${BUILD_AS_ARCH}"

      When building a ref-classes-native recipe, **AS** is set to the
      value of this variable by default.

      The **BUILD_AS** variable should not be set manually, and is rarely
      used in recipes as **AS** contains the appropriate value depending on
      the context (native or target recipes). Exception be made for target
      recipes that need to use the `assembler <as(1)>` from the build
      host at some point during the build.

   **BUILD_AS_ARCH**
      Specifies the architecture-specific assembler flags for the build
      host. By default, the value of **BUILD_AS_ARCH** is empty.

   **BUILD_CC**
      Specifies the architecture-specific C compiler for the build host,
      and its default definition is derived in part from **BUILD_PREFIX**
      and **BUILD_CC_ARCH**::

         BUILD_CC = "${CCACHE}${BUILD_PREFIX}gcc ${BUILD_CC_ARCH}"

      When building a ref-classes-native recipe, **CC** is set to the
      value of this variable by default.

      The **BUILD_CC** variable should not be set manually, and is rarely
      used in recipes as **CC** contains the appropriate value depending on
      the context (native or target recipes). Exception be made for target
      recipes that need to use the compiler from the build host at some point
      during the build.

   **BUILD_CC_ARCH**
      Specifies the architecture-specific C compiler flags for the build
      host. By default, the value of **BUILD_CC_ARCH** is empty.

   **BUILD_CCLD**
      Specifies the `linker <ld(1)>` command to be used for the build
      host when the C compiler is being used as the linker, and its default
      definition is derived in part from **BUILD_PREFIX** and
      **BUILD_CC_ARCH**::

         BUILD_CCLD = "${BUILD_PREFIX}gcc ${BUILD_CC_ARCH}"

      When building a ref-classes-native recipe, **CCLD** is set to
      the value of this variable by default.

      The **BUILD_CCLD** variable should not be set manually, and is rarely
      used in recipes as **CCLD** contains the appropriate value depending on
      the context (native or target recipes). Exception be made for target
      recipes that need to use the `linker <ld(1)>` from the build host
      at some point during the build.

   **BUILD_CFLAGS**
      Specifies the flags to pass to the C compiler when building for the
      build host. When building a ref-classes-native recipe,
      **CFLAGS** is set to the value of this variable by
      default.

   **BUILD_CPP**
      Specifies the C preprocessor command (to both the C and the C++ compilers)
      when building for the build host, and its default definition is derived in
      part from **BUILD_PREFIX** and **BUILD_CC_ARCH**::

         BUILD_CPP = "${BUILD_PREFIX}gcc ${BUILD_CC_ARCH} -E"

      When building a ref-classes-native recipe, **CPP** is set to
      the value of this variable by default.

      The **BUILD_CPP** variable should not be set manually, and is rarely
      used in recipes as **CPP** contains the appropriate value depending on
      the context (native or target recipes). Exception be made for target
      recipes that need to use the preprocessor from the build host at some
      point during the build.

   **BUILD_CPPFLAGS**
      Specifies the flags to pass to the C preprocessor (i.e. to both the C
      and the C++ compilers) when building for the build host. When
      building in the `-native` context, **CPPFLAGS**
      is set to the value of this variable by default.

   **BUILD_CXX**
      Specifies the architecture-specific C++ compiler for the build host,
      and its default definition is derived in part from **BUILD_PREFIX**
      and **BUILD_CC_ARCH**::

         BUILD_CXX = "${CCACHE}${BUILD_PREFIX}g++ ${BUILD_CC_ARCH}"

      When building a ref-classes-native recipe, **CXX** is set to
      the value of this variable by default.

      The **BUILD_CXX** variable should not be set manually, and is rarely
      used in recipes as **CXX** contains the appropriate value depending on
      the context (native or target recipes). Exception be made for target
      recipes that need to use the C++ compiler from the build host at some
      point during the build.

   **BUILD_CXXFLAGS**
      Specifies the flags to pass to the C++ compiler when building for the
      build host. When building a ref-classes-native recipe,
      **CXXFLAGS** is set to the value of this variable
      by default.

   **BUILD_FC**
      Specifies the Fortran compiler command for the build host, and its default
      definition is derived in part from **BUILD_PREFIX** and
      **BUILD_CC_ARCH**::

         BUILD_FC = "${BUILD_PREFIX}gfortran ${BUILD_CC_ARCH}"

      When building a ref-classes-native recipe, **FC** is set to the
      value of this variable by default.

      The **BUILD_FC** variable should not be set manually, and is rarely
      used in recipes as **FC** contains the appropriate value depending on
      the context (native or target recipes). Exception be made for target
      recipes that need to use the Fortran compiler from the build host at some
      point during the build.

   **BUILD_LD**
      Specifies the linker command for the build host, and its default
      definition is derived in part from **BUILD_PREFIX** and
      **BUILD_LD_ARCH**::

         BUILD_LD = "${BUILD_PREFIX}ld ${BUILD_LD_ARCH}"

      When building a ref-classes-native recipe, **LD** is set to the
      value of this variable by default.

      The **BUILD_LD** variable should not be set manually, and is rarely
      used in recipes as **LD** contains the appropriate value depending on
      the context (native or target recipes). Exception be made for target
      recipes that need to use the linker from the build host at some point
      during the build.

   **BUILD_LD_ARCH**
      Specifies architecture-specific linker flags for the build host. By
      default, the value of **BUILD_LD_ARCH** is empty.

   **BUILD_LDFLAGS**
      Specifies the flags to pass to the linker when building for the build
      host. When building a ref-classes-native recipe,
      **LDFLAGS** is set to the value of this variable
      by default.

   **BUILD_NM**
      Specifies the architecture-specific utility to list symbols from object
      files for the build host, and its default definition is derived in part
      from **BUILD_PREFIX**::

         BUILD_NM = "${BUILD_PREFIX}nm"

      When building a ref-classes-native recipe, **NM** is set to the
      value of this variable by default.

      The **BUILD_NM** variable should not be set manually, and is rarely
      used in recipes as **NM** contains the appropriate value depending on
      the context (native or target recipes). Exception be made for target
      recipes that need to use the utility from the build host at some point
      during the build.

   **BUILD_OBJCOPY**
      Specifies the architecture-specific utility to copy object files for the
      build host, and its default definition is derived in part from
      **BUILD_PREFIX**::

         BUILD_OBJCOPY = "${BUILD_PREFIX}objcopy"

      When building a ref-classes-native recipe, **OBJCOPY** is set
      to the value of this variable by default.

      The **BUILD_OBJCOPY** variable should not be set manually, and is
      rarely used in recipes as **OBJCOPY** contains the appropriate value
      depending on the context (native or target recipes). Exception be made for
      target recipes that need to use the utility from the build host at some
      point during the build.

   **BUILD_OBJDUMP**
      Specifies the architecture-specific utility to display object files
      information for the build host, and its default definition is derived in
      part from **BUILD_PREFIX**::

         BUILD_OBJDUMP = "${BUILD_PREFIX}objdump"

      When building a ref-classes-native recipe, **OBJDUMP** is set
      to the value of this variable by default.

      The **BUILD_OBJDUMP** variable should not be set manually, and is
      rarely used in recipes as **OBJDUMP** contains the appropriate value
      depending on the context (native or target recipes). Exception be made for
      target recipes that need to use the utility from the build host at some
      point during the build.

   **BUILD_OPTIMIZATION**
      Specifies the optimization flags passed to the C compiler when
      building for the build host or the SDK. The flags are passed through
      the **BUILD_CFLAGS** and
      **BUILDSDK_CFLAGS** default values.

      The default value of the **BUILD_OPTIMIZATION** variable is "-O2
      -pipe".

   **BUILD_OS**
      Specifies the operating system in use on the build host (e.g.
      "linux"). The OpenEmbedded build system sets the value of
      **BUILD_OS** from the OS reported by the `uname` command --- the
      first word, converted to lower-case characters.

   **BUILD_PREFIX**
      The toolchain binary prefix used for native recipes. The OpenEmbedded
      build system uses the **BUILD_PREFIX** value to set the
      **TARGET_PREFIX** when building for ref-classes-native recipes.

   **BUILD_RANLIB**
      Specifies the architecture-specific utility to generate indexes for
      archives for the build host, and its default definition is derived in part
      from **BUILD_PREFIX**::

         BUILD_RANLIB = "${BUILD_PREFIX}ranlib -D"

      When building a ref-classes-native recipe, **RANLIB** is set to
      the value of this variable by default.

      The **BUILD_RANLIB** variable should not be set manually, and is
      rarely used in recipes as **RANLIB** contains the appropriate value
      depending on the context (native or target recipes). Exception be made for
      target recipes that need to use the utility from the build host at some
      point during the build.

   **BUILD_READELF**
      Specifies the architecture-specific utility to display information about
      ELF files for the build host, and its default definition is derived in
      part from **BUILD_PREFIX**::

         BUILD_READELF = "${BUILD_PREFIX}readelf"

      When building a ref-classes-native recipe, **READELF** is set
      to the value of this variable by default.

      The **BUILD_READELF** variable should not be set manually, and is
      rarely used in recipes as **READELF** contains the appropriate value
      depending on the context (native or target recipes). Exception be made for
      target recipes that need to use the utility from the build host at some
      point during the build.

   **BUILD_STRIP**
      Specifies the command to be used to strip debugging symbols from binaries
      produced for the build host, and its default definition is derived in part
      from **BUILD_PREFIX**::

         BUILD_STRIP = "${BUILD_PREFIX}strip"

      When building a ref-classes-native recipe, **STRIP** is set to
      the value of this variable by default.

      The **BUILD_STRIP** variable should not be set manually, and is
      rarely used in recipes as **STRIP** contains the appropriate value
      depending on the context (native or target recipes). Exception be made for
      target recipes that need to use the utility from the build host at some
      point during the build.

   **BUILD_SYS**
      Specifies the system, including the architecture and the operating
      system, to use when building for the build host (i.e. when building
      ref-classes-native recipes).

      The OpenEmbedded build system automatically sets this variable based
      on **BUILD_ARCH**,
      **BUILD_VENDOR**, and
      **BUILD_OS**. You do not need to set the
      **BUILD_SYS** variable yourself.

   **BUILD_VENDOR**
      Specifies the vendor name to use when building for the build host.
      The default value is an empty string ("").

   **BUILDDIR**
      Points to the location of the **Build Directory**. You can define
      this directory indirectly through the structure-core-script script
      by passing in a **Build Directory** path when you run the script. If
      you run the script and do not provide a **Build Directory** path, the
      **BUILDDIR** defaults to `build` in the current directory.

   **BUILDHISTORY_COMMIT**
      When inheriting the ref-classes-buildhistory class, this variable
      specifies whether or not to commit the build history output in a local
      Git repository. If set to "1", this local repository will be maintained
      automatically by the ref-classes-buildhistory class and a commit
      will be created on every build for changes to each top-level subdirectory
      of the build history output (images, packages, and sdk). If you want to
      track changes to build history over time, you should set this value to
      "1".

      By default, the ref-classes-buildhistory class
      enables committing the buildhistory output in a local Git repository::

         BUILDHISTORY_COMMIT ?= "1"

   **BUILDHISTORY_COMMIT_AUTHOR**
      When inheriting the ref-classes-buildhistory
      class, this variable specifies the author to use for each Git commit.
      In order for the **BUILDHISTORY_COMMIT_AUTHOR** variable to work, the
      **BUILDHISTORY_COMMIT** variable must
      be set to "1".

      Git requires that the value you provide for the
      **BUILDHISTORY_COMMIT_AUTHOR** variable takes the form of "name
      email@host". Providing an email address or host that is not valid
      does not produce an error.

      By default, the ref-classes-buildhistory class sets the variable
      as follows::

         BUILDHISTORY_COMMIT_AUTHOR ?= "buildhistory <buildhistory@${DISTRO}>"

   **BUILDHISTORY_DIR**
      When inheriting the ref-classes-buildhistory
      class, this variable specifies the directory in which build history
      information is kept. For more information on how the variable works,
      see the ref-classes-buildhistory class.

      By default, the ref-classes-buildhistory class sets the directory
      as follows::

         BUILDHISTORY_DIR ?= "${TOPDIR}/buildhistory"

   **BUILDHISTORY_FEATURES**
      When inheriting the ref-classes-buildhistory
      class, this variable specifies the build history features to be
      enabled. For more information on how build history works, see the
      "dev-manual/build-quality:maintaining build output quality"
      section in the Yocto Project Development Tasks Manual.

      You can specify these features in the form of a space-separated list:

      -  *image:* Analysis of the contents of images, which includes the
         list of installed packages among other things.

      -  *package:* Analysis of the contents of individual packages.

      -  *sdk:* Analysis of the contents of the software development kit
         (SDK).

      -  *task:* Save output file signatures for
         shared state 
         (sstate) tasks.
         This saves one file per task and lists the SHA-256 checksums for
         each file staged (i.e. the output of the task).

      By default, the ref-classes-buildhistory class enables the
      following features::

         BUILDHISTORY_FEATURES ?= "image package sdk"

   **BUILDHISTORY_IMAGE_FILES**
      When inheriting the ref-classes-buildhistory
      class, this variable specifies a list of paths to files copied from
      the image contents into the build history directory under an
      "image-files" directory in the directory for the image, so that you
      can track the contents of each file. The default is to copy
      `/etc/passwd` and `/etc/group`, which allows you to monitor for
      changes in user and group entries. You can modify the list to include
      any file. Specifying an invalid path does not produce an error.
      Consequently, you can include files that might not always be present.

      By default, the ref-classes-buildhistory class provides paths to
      the following files::

         BUILDHISTORY_IMAGE_FILES ?= "/etc/passwd /etc/group"

   **BUILDHISTORY_PATH_PREFIX_STRIP**
      When inheriting the ref-classes-buildhistory
      class, this variable specifies a common path prefix that should be
      stripped off the beginning of paths in the task signature list when the
      `task` feature is active in **BUILDHISTORY_FEATURES**. This can be
      useful when build history is populated from multiple sources that may not
      all use the same top level directory.

      By default, the ref-classes-buildhistory class sets the variable
      as follows::

         BUILDHISTORY_PATH_PREFIX_STRIP ?= ""

      In this case, no prefixes will be stripped.

   **BUILDHISTORY_PUSH_REPO**
      When inheriting the ref-classes-buildhistory class, this variable
      optionally specifies a remote repository to which build history pushes
      Git changes. In order for **BUILDHISTORY_PUSH_REPO** to work,
      **BUILDHISTORY_COMMIT** must be set to "1".

      The repository should correspond to a remote address that specifies a
      repository as understood by Git, or alternatively to a remote name
      that you have set up manually using `git remote` within the local
      repository.

      By default, the ref-classes-buildhistory class sets the variable
      as follows::

         BUILDHISTORY_PUSH_REPO ?= ""

   **BUILDNAME**
      See **bitbake:BUILDNAME** in the BitBake manual.

   **BUILDSDK_CFLAGS**
      Specifies the flags to pass to the C compiler when building for the
      SDK. When building in the `nativesdk-` context,
      **CFLAGS** is set to the value of this variable by
      default.

   **BUILDSDK_CPPFLAGS**
      Specifies the flags to pass to the C pre-processor (i.e. to both the
      C and the C++ compilers) when building for the SDK. When building in
      the `nativesdk-` context, **CPPFLAGS** is set
      to the value of this variable by default.

   **BUILDSDK_CXXFLAGS**
      Specifies the flags to pass to the C++ compiler when building for the
      SDK. When building in the `nativesdk-` context,
      **CXXFLAGS** is set to the value of this variable
      by default.

   **BUILDSDK_LDFLAGS**
      Specifies the flags to pass to the linker when building for the SDK.
      When building in the `nativesdk-` context,
      **LDFLAGS** is set to the value of this variable
      by default.

   **BUILDSTATS_BASE**
      Points to the location of the directory that holds build statistics
      when you use and enable the ref-classes-buildstats class. The
      **BUILDSTATS_BASE** directory defaults to
      `${`\ **TMPDIR**\ `}/buildstats/`.

   **BUSYBOX_SPLIT_SUID**
      For the BusyBox recipe, specifies whether to split the output
      executable file into two parts: one for features that require
      `setuid root`, and one for the remaining features (i.e. those that
      do not require `setuid root`).

      The **BUSYBOX_SPLIT_SUID** variable defaults to "1", which results in
      splitting the output executable file. Set the variable to "0" to get
      a single output executable file.

   **CACHE**
      Specifies the directory BitBake uses to store a cache of the
      **Metadata** so it does not need to be parsed every time
      BitBake is started.

   **CARGO_INSTALL_LIBRARIES**
      When inheriting the ref-classes-cargo class, the variable
      **CARGO_INSTALL_LIBRARIES** can be set to a non-empty value by
      individual recipes to enable the installation of the libraries the
      recipe has built in `${B}/target/${CARGO_TARGET_SUBDIR}` (files ending
      with `.so` or `.rlib`). By default this variable is not defined and
      libraries are not installed, to replicate the behavior of the ``cargo
      install`` command.

   **CC**
      The minimal command and arguments used to run the C compiler.

   **CCACHE_DISABLE**
      When inheriting the ref-classes-ccache class, the
      **CCACHE_DISABLE** variable can be set to "1" in a recipe to disable
      `Ccache` support. This is useful when the recipe is known to not support it.

   **CCACHE_TOP_DIR**
      When inheriting the ref-classes-ccache class, the
      **CCACHE_TOP_DIR** variable can be set to the location of where
      `Ccache` stores its cache files. This directory can be shared between
      builds.

   **CCLD**
      The minimal command and arguments used to run the linker when the C
      compiler is being used as the linker.

   **CFLAGS**
      Specifies the flags to pass to the C compiler. This variable is
      exported to an environment variable and thus made visible to the
      software being built during the compilation step.

      Default initialization for **CFLAGS** varies depending on what is
      being built:

      -  **TARGET_CFLAGS** when building for the
         target

      -  **BUILD_CFLAGS** when building for the
         build host (i.e. `-native`)

      -  **BUILDSDK_CFLAGS** when building for
         an SDK (i.e. `nativesdk-`)

   **CHECKLAYER_REQUIRED_TESTS**
      The **CHECKLAYER_REQUIRED_TESTS** variable lists the QA tests that are
      required to be enabled to pass the Yocto Project Compatible status for a
      layer. It is meant to be a read-only variable and any change to the
      variable may be done with the approval of the :oe_wiki:`Technical Steering
      Committee (TSC) </TSC>`.

      For more information on the Yocto Project Compatible status, see
      the :ref:`dev-manual/layers:Making Sure Your Layer is Compatible With
      Yocto Project` section of the Yocto Project Development Manual.

   **CLASSOVERRIDE**
      An internal variable specifying the special class override that
      should currently apply (e.g. "class-target", "class-native", and so
      forth). The classes that use this variable (e.g.
      ref-classes-native, ref-classes-nativesdk, and so forth)
      set the variable to appropriate values.

      .. note::

         **CLASSOVERRIDE** gets its default "class-target" value from the
         `bitbake.conf` file.

      As an example, the following override allows you to install extra
      files, but only when building for the target::

         do_install:append:class-target() {
             install my-extra-file ${D}${sysconfdir}
         }

      Here is an example where `FOO` is set to
      "native" when building for the build host, and to "other" when not
      building for the build host::

         FOO:class-native = "native"
         FOO = "other"

      The underlying mechanism behind **CLASSOVERRIDE** is simply
      that it is included in the default value of
      **OVERRIDES**.

   **CLEANBROKEN**
      If set to "1" within a recipe, **CLEANBROKEN** specifies that the
      `make clean` command does not work for the software being built.
      Consequently, the OpenEmbedded build system will not try to run
      `make clean` during the ref-tasks-configure
      task, which is the default behavior.

   **COMBINED_FEATURES**
      Provides a list of hardware features that are enabled in both
      **MACHINE_FEATURES** and
      **DISTRO_FEATURES**. This select list of
      features contains features that make sense to be controlled both at
      the machine and distribution configuration level. For example, the
      "bluetooth" feature requires hardware support but should also be
      optional at the distribution level, in case the hardware supports
      Bluetooth but you do not ever intend to use it.

   **COMMERCIAL_AUDIO_PLUGINS**
      This variable is specific to the :oe_git:`GStreamer recipes
      </openembedded-core/tree/meta/recipes-multimedia/gstreamer/>`.
      It allows to build the GStreamer `"ugly"
      <https://github.com/GStreamer/gst-plugins-ugly>`__ and
      `"bad" <https://github.com/GStreamer/gst-plugins-bad>`__ audio plugins.

      See the dev-manual/licenses:other variables related to commercial licenses
      section for usage details.

   **COMMERCIAL_VIDEO_PLUGINS**
      This variable is specific to the :oe_git:`GStreamer recipes
      </openembedded-core/tree/meta/recipes-multimedia/gstreamer/>`.
      It allows to build the GStreamer `"ugly"
      <https://github.com/GStreamer/gst-plugins-ugly>`__ and
      `"bad" <https://github.com/GStreamer/gst-plugins-bad>`__ video plugins.

      See the dev-manual/licenses:other variables related to commercial licenses
      section for usage details.

   **COMMON_LICENSE_DIR**
      Points to `meta/files/common-licenses` in
      **OpenEmbedded-Core (OE-Core)**, which is where generic license
      files reside.

   **COMPATIBLE_HOST**
      A regular expression that resolves to one or more hosts (when the
      recipe is native) or one or more targets (when the recipe is
      non-native) with which a recipe is compatible. The regular expression
      is matched against **HOST_SYS**. You can use the
      variable to stop recipes from being built for classes of systems with
      which the recipes are not compatible. Stopping these builds is
      particularly useful with kernels. The variable also helps to increase
      parsing speed since the build system skips parsing recipes not
      compatible with the current system.

   **COMPATIBLE_MACHINE**
      A regular expression that resolves to one or more target machines
      with which a recipe is compatible. The regular expression is matched
      against **MACHINEOVERRIDES**. You can use
      the variable to stop recipes from being built for machines with which
      the recipes are not compatible. Stopping these builds is particularly
      useful with kernels. The variable also helps to increase parsing
      speed since the build system skips parsing recipes not compatible
      with the current machine.

      If one wants to have a recipe only available for some architectures
      (here `aarch64` and `mips64`), the following can be used::

         COMPATIBLE_MACHINE = "^$"
         COMPATIBLE_MACHINE:arch64 = "^(aarch64)$"
         COMPATIBLE_MACHINE:mips64 = "^(mips64)$"

      The first line means "match all machines whose **MACHINEOVERRIDES**
      contains the empty string", which will always be none.

      The second is for matching all machines whose **MACHINEOVERRIDES**
      contains one override which is exactly `aarch64`.

      The third is for matching all machines whose **MACHINEOVERRIDES**
      contains one override which is exactly `mips64`.

      The same could be achieved with::

         COMPATIBLE_MACHINE = "^(aarch64|mips64)$"

      .. note::

         When **COMPATIBLE_MACHINE** is set in a recipe inherits from
         native, the recipe is always skipped. All native recipes must be
         entirely target independent and should not rely on **MACHINE**.

   **COMPLEMENTARY_GLOB**
      Defines wildcards to match when installing a list of complementary
      packages for all the packages explicitly (or implicitly) installed in
      an image.

      The **COMPLEMENTARY_GLOB** variable uses Unix filename pattern matching
      (`fnmatch <https://docs.python.org/3/library/fnmatch.html#module-fnmatch>`__),
      which is similar to the Unix style pathname pattern expansion
      (`glob <https://docs.python.org/3/library/glob.html>`__).

      The resulting list of complementary packages is associated with an
      item that can be added to
      **IMAGE_FEATURES**. An example usage of
      this is the "dev-pkgs" item that when added to **IMAGE_FEATURES**
      will install -dev packages (containing headers and other development
      files) for every package in the image.

      To add a new feature item pointing to a wildcard, use a variable flag
      to specify the feature item name and use the value to specify the
      wildcard. Here is an example::

         COMPLEMENTARY_GLOB[dev-pkgs] = '*-dev'

      .. note::

         When installing complementary packages, recommends relationships
         (set via **RRECOMMENDS**) are always ignored.

   **COMPONENTS_DIR**
      Stores sysroot components for each recipe. The OpenEmbedded build
      system uses **COMPONENTS_DIR** when constructing recipe-specific
      sysroots for other recipes.

      The default is
      "`${`\ **STAGING_DIR**\ `}-components`."
      (i.e.
      "`${`\ **TMPDIR**\ `}/sysroots-components`").

   **CONF_VERSION**
      Tracks the version of the local configuration file (i.e.
      `local.conf`). The value for **CONF_VERSION** increments each time
      `build/conf/` compatibility changes.

   **CONFFILES**
      Identifies editable or configurable files that are part of a package.
      If the Package Management System (PMS) is being used to update
      packages on the target system, it is possible that configuration
      files you have changed after the original installation and that you
      now want to remain unchanged are overwritten. In other words,
      editable files might exist in the package that you do not want reset
      as part of the package update process. You can use the **CONFFILES**
      variable to list the files in the package that you wish to prevent
      the PMS from overwriting during this update process.

      To use the **CONFFILES** variable, provide a package name override
      that identifies the resulting package. Then, provide a
      space-separated list of files. Here is an example::

         CONFFILES:${PN} += "${sysconfdir}/file1 \
             ${sysconfdir}/file2 ${sysconfdir}/file3"

      There is a relationship between the **CONFFILES** and **FILES**
      variables. The files listed within **CONFFILES** must be a subset of
      the files listed within **FILES**. Because the configuration files
      you provide with **CONFFILES** are simply being identified so that
      the PMS will not overwrite them, it makes sense that the files must
      already be included as part of the package through the **FILES**
      variable.

      .. note::

         When specifying paths as part of the **CONFFILES** variable, it is
         good practice to use appropriate path variables.
         For example, `${sysconfdir}` rather than `/etc` or `${bindir}`
         rather than `/usr/bin`. You can find a list of these variables at
         the top of the `meta/conf/bitbake.conf` file in
         **OpenEmbedded-Core (OE-Core)**.

   **CONFIG_INITRAMFS_SOURCE**
      Identifies the initial RAM filesystem (**Initramfs**) source files. The
      OpenEmbedded build system receives and uses this kernel Kconfig
      variable as an environment variable. By default, the variable is set
      to null ("").

      The **CONFIG_INITRAMFS_SOURCE** can be either a single cpio archive
      with a `.cpio` suffix or a space-separated list of directories and
      files for building the **Initramfs** image. A cpio archive should contain
      a filesystem archive to be used as an **Initramfs** image. Directories
      should contain a filesystem layout to be included in the **Initramfs**
      image. Files should contain entries according to the format described
      by the `usr/gen_init_cpio` program in the kernel tree.

      If you specify multiple directories and files, the **Initramfs** image
      will be the aggregate of all of them.

      For information on creating an **Initramfs**, see the
      "dev-manual/building:building an initial ram filesystem (Initramfs) image" section
      in the Yocto Project Development Tasks Manual.

   **CONFIG_SITE**
      A list of files that contains `autoconf` test results relevant to
      the current build. This variable is used by the Autotools utilities
      when running `configure`.

   **CONFIGURE_FLAGS**
      The minimal arguments for GNU configure.

   **CONFIGURE_SCRIPT**
      When using the ref-classes-autotools class, the
      **CONFIGURE_SCRIPT** variable stores the location of the `configure`
      script for the Autotools build system. The default definition for this
      variable is::

         CONFIGURE_SCRIPT ?= "${AUTOTOOLS_SCRIPT_PATH}/configure"

      Where **AUTOTOOLS_SCRIPT_PATH** is the location of the of the
      Autotools build system scripts, which defaults to **S**.

   **CONFLICT_DISTRO_FEATURES**
      When inheriting the ref-classes-features_check
      class, this variable identifies distribution features that would be
      in conflict should the recipe be built. In other words, if the
      **CONFLICT_DISTRO_FEATURES** variable lists a feature that also
      appears in **DISTRO_FEATURES** within the current configuration, then
      the recipe will be skipped, and if the build system attempts to build
      the recipe then an error will be triggered.

   **CONVERSION_CMD**
      This variable is used for storing image conversion commands.
      Image conversion can convert an image into different objects like:

      -   Compressed version of the image

      -   Checksums for the image

      An example of **CONVERSION_CMD** from ref-classes-image_types
      class is::

         CONVERSION_CMD:lzo = "lzop -9 ${IMAGE_NAME}${IMAGE_NAME_SUFFIX}.${type}"

   **COPY_LIC_DIRS**
      If set to "1" along with the
      **COPY_LIC_MANIFEST** variable, the
      OpenEmbedded build system copies into the image the license files,
      which are located in `/usr/share/common-licenses`, for each
      package. The license files are placed in directories within the image
      itself during build time.

      .. note::

         The **COPY_LIC_DIRS** does not offer a path for adding licenses for
         newly installed packages to an image, which might be most suitable for
         read-only filesystems that cannot be upgraded. See the
         **LICENSE_CREATE_PACKAGE** variable for additional information.
         You can also reference the "dev-manual/licenses:providing license text"
         section in the Yocto Project Development Tasks Manual for
         information on providing license text.

   **COPY_LIC_MANIFEST**
      If set to "1", the OpenEmbedded build system copies the license
      manifest for the image to
      `/usr/share/common-licenses/license.manifest` within the image
      itself during build time.

      .. note::

         The **COPY_LIC_MANIFEST** does not offer a path for adding licenses for
         newly installed packages to an image, which might be most suitable for
         read-only filesystems that cannot be upgraded. See the
         **LICENSE_CREATE_PACKAGE** variable for additional information.
         You can also reference the "dev-manual/licenses:providing license text"
         section in the Yocto Project Development Tasks Manual for
         information on providing license text.

   **COPYLEFT_LICENSE_EXCLUDE**
      A space-separated list of licenses to exclude from the source archived by
      the ref-classes-archiver class. In other words, if a license in a
      recipe's **LICENSE** value is in the value of
      **COPYLEFT_LICENSE_EXCLUDE**, then its source is not archived by the
      class.

      .. note::

         The **COPYLEFT_LICENSE_EXCLUDE** variable takes precedence over the
         **COPYLEFT_LICENSE_INCLUDE** variable.

      The default value, which is "CLOSED Proprietary", for
      **COPYLEFT_LICENSE_EXCLUDE** is set by the
      ref-classes-copyleft_filter class, which
      is inherited by the ref-classes-archiver class.

   **COPYLEFT_LICENSE_INCLUDE**
      A space-separated list of licenses to include in the source archived
      by the ref-classes-archiver class. In other
      words, if a license in a recipe's **LICENSE**
      value is in the value of **COPYLEFT_LICENSE_INCLUDE**, then its
      source is archived by the class.

      The default value is set by the ref-classes-copyleft_filter class,
      which is inherited by the ref-classes-archiver class. The default
      value includes "GPL*", "LGPL*", and "AGPL*".

   **COPYLEFT_PN_EXCLUDE**
      A list of recipes to exclude in the source archived by the
      ref-classes-archiver class. The **COPYLEFT_PN_EXCLUDE**
      variable overrides the license inclusion and exclusion caused through the
      **COPYLEFT_LICENSE_INCLUDE** and **COPYLEFT_LICENSE_EXCLUDE**
      variables, respectively.

      The default value, which is "" indicating to not explicitly exclude
      any recipes by name, for **COPYLEFT_PN_EXCLUDE** is set by the
      ref-classes-copyleft_filter class, which is inherited by the
      ref-classes-archiver class.

   **COPYLEFT_PN_INCLUDE**
      A list of recipes to include in the source archived by the
      ref-classes-archiver class. The **COPYLEFT_PN_INCLUDE**
      variable overrides the license inclusion and exclusion caused through the
      **COPYLEFT_LICENSE_INCLUDE** and **COPYLEFT_LICENSE_EXCLUDE**
      variables, respectively.

      The default value, which is "" indicating to not explicitly include
      any recipes by name, for **COPYLEFT_PN_INCLUDE** is set by the
      ref-classes-copyleft_filter class, which is inherited by the
      ref-classes-archiver class.

   **COPYLEFT_RECIPE_TYPES**
      A space-separated list of recipe types to include in the source
      archived by the archiver  class.
      Recipe types are `target`, ref-classes-native,
      ref-classes-nativesdk, ref-classes-cross,
      ref-classes-crosssdk, and ref-classes-cross-canadian.

      The default value, which is "target*", for **COPYLEFT_RECIPE_TYPES**
      is set by the ref-classes-copyleft_filter class, which is
      inherited by the ref-classes-archiver class.

   **CORE_IMAGE_EXTRA_INSTALL**
      Specifies the list of packages to be added to the image. You should
      only set this variable in the `local.conf` configuration file found
      in the **Build Directory**.

      This variable replaces `POKY_EXTRA_INSTALL`, which is no longer
      supported.

   **COREBASE**
      Specifies the parent directory of the **OpenEmbedded-Core (OE-Core)**
      metadata layer (i.e. `meta`).

   **COREBASE_FILES**
      Lists files from the **COREBASE** directory that
      should be copied other than the layers listed in the
      `bblayers.conf` file. The **COREBASE_FILES** variable allows
      to copy metadata from the OpenEmbedded build system
      into the extensible SDK.

      Explicitly listing files in **COREBASE** is needed because it
      typically contains build directories and other files that should not
      normally be copied into the extensible SDK. Consequently, the value
      of **COREBASE_FILES** is used in order to only copy the files that
      are actually needed.

   **CPP**
      The minimal command and arguments used to run the C preprocessor.

   **CPPFLAGS**
      Specifies the flags to pass to the C pre-processor (i.e. to both the
      C and the C++ compilers). This variable is exported to an environment
      variable and thus made visible to the software being built during the
      compilation step.

      Default initialization for **CPPFLAGS** varies depending on what is
      being built:

      -  **TARGET_CPPFLAGS** when building for
         the target

      -  **BUILD_CPPFLAGS** when building for the
         build host (i.e. `-native`)

      -  **BUILDSDK_CPPFLAGS** when building
         for an SDK (i.e. `nativesdk-`)

   **CROSS_COMPILE**
      The toolchain binary prefix for the target tools. The
      **CROSS_COMPILE** variable is the same as the
      **TARGET_PREFIX** variable.

      .. note::

         The OpenEmbedded build system sets the **CROSS_COMPILE**
         variable only in certain contexts (e.g. when building for kernel
         and kernel module recipes).

   **CVE_CHECK_CREATE_MANIFEST**
      Specifies whether to create a CVE manifest to place in the deploy
      directory. The default is "1".

   **CVE_CHECK_IGNORE**
      This variable is deprecated and should be replaced by **CVE_STATUS**.

   **CVE_CHECK_MANIFEST_JSON**
      Specifies the path to the CVE manifest in JSON format. See
      **CVE_CHECK_CREATE_MANIFEST**.

   **CVE_CHECK_MANIFEST_JSON_SUFFIX**
      Allows to modify the JSON manifest suffix. See
      **CVE_CHECK_MANIFEST_JSON**.

   **CVE_CHECK_REPORT_PATCHED**
      Specifies whether or not the ref-classes-cve-check
      class should report patched or ignored CVEs. The default is "1", but you
      may wish to set it to "0" if you do not need patched or ignored CVEs in
      the logs.

   **CVE_CHECK_SHOW_WARNINGS**
      Specifies whether or not the ref-classes-cve-check
      class should generate warning messages on the console when unpatched
      CVEs are found. The default is "1", but you may wish to set it to "0" if
      you are already examining/processing the logs after the build has
      completed and thus do not need the warning messages.

   **CVE_CHECK_SKIP_RECIPE**
      The list of package names (**PN**) for which
      CVEs (Common Vulnerabilities and Exposures) are ignored.

   **CVE_CHECK_STATUSMAP**
      Mapping variable for all possible reasons of **CVE_STATUS**:
      `Patched`, `Unpatched` and `Ignored`.
      See ref-classes-cve-check or `meta/conf/cve-check-map.conf` for more details::

         CVE_CHECK_STATUSMAP[cpe-incorrect] = "Ignored"

   **CVE_CHECK_VEX_JUSTIFICATION**
      Mapping variable for **CVE_STATUS** reasons
      (for example `not-applicable-platform`) to ref-classes-vex
      justifications. Should be set as follows::

         CVE_CHECK_VEX_JUSTIFICATION[not-applicable-config] = "vulnerableCodeNotPresent"

   **CVE_DB_INCR_UPDATE_AGE_THRES**
      Specifies the maximum age of the CVE database in seconds for an
      incremental update (instead of a full-download). Use "0" to force a
      full-download.

   **CVE_DB_UPDATE_INTERVAL**
      Specifies the CVE database update interval in seconds, as used by
      `cve-update-db-native`. The default value is "86400" i.e. once a day
      (24*60*60). If the value is set to "0" then the update will be forced
      every time. Alternatively, a negative value e.g. "-1" will disable
      updates entirely.

   **CVE_PRODUCT**
      In a recipe, defines the name used to match the recipe name
      against the name in the upstream `NIST CVE database <https://nvd.nist.gov/>`__.

      The default is ${**BPN**} (except for recipes that inherit the
      ref-classes-pypi class where it is set based upon
      **PYPI_PACKAGE**). If it does not match the name in the NIST CVE
      database or matches with multiple entries in the database, the default
      value needs to be changed.

      Here is an example from the `Berkeley DB recipe </layerindex/recipe/544>`::

         CVE_PRODUCT = "oracle_berkeley_db berkeley_db"

      Sometimes the product name is not specific enough, for example
      "tar" has been matching CVEs for the GNU `tar` package and also
      the `node-tar` node.js extension. To avoid this problem, use the
      vendor name as a prefix. The syntax for this is::

         CVE_PRODUCT = "vendor:package"

   **CVE_STATUS**
      The CVE ID which is patched or should be ignored. Here is
      an example from the `Python3 recipe</layerindex/recipe/23823>`::

         CVE_STATUS[CVE-2020-15523] = "not-applicable-platform: Issue only applies on Windows"

      It has the format "reason: description" and the description is optional.
      The Reason is mapped to the final CVE state by mapping via
      **CVE_CHECK_STATUSMAP**. See security-manual/vulnerabilities:fixing vulnerabilities in recipes
      for details.

   **CVE_STATUS_GROUPS**
      If there are many CVEs with the same status and reason, they can by simplified by using this
      variable instead of many similar lines with **CVE_STATUS**::

         CVE_STATUS_GROUPS = "CVE_STATUS_WIN CVE_STATUS_PATCHED"

         CVE_STATUS_WIN = "CVE-1234-0001 CVE-1234-0002"
         CVE_STATUS_WIN[status] = "not-applicable-platform: Issue only applies on Windows"
         CVE_STATUS_PATCHED = "CVE-1234-0003 CVE-1234-0004"
         CVE_STATUS_PATCHED[status] = "fixed-version: Fixed externally"

   **CVE_VERSION**
      In a recipe, defines the version used to match the recipe version
      against the version in the `NIST CVE database <https://nvd.nist.gov/>`__
      when usign ref-classes-cve-check.

      The default is ${**PV**} but if recipes use custom version numbers
      which do not map to upstream software component release versions and the versions
      used in the CVE database, then this variable can be used to set the
      version number for ref-classes-cve-check. Example::

          CVE_VERSION = "2.39"

   **CVSDIR**
      The directory in which files checked out under the CVS system are
      stored.

   **CXX**
      The minimal command and arguments used to run the C++ compiler.

   **CXXFLAGS**
      Specifies the flags to pass to the C++ compiler. This variable is
      exported to an environment variable and thus made visible to the
      software being built during the compilation step.

      Default initialization for **CXXFLAGS** varies depending on what is
      being built:

      -  **TARGET_CXXFLAGS** when building for
         the target

      -  **BUILD_CXXFLAGS** when building for the
         build host (i.e. `-native`)

      -  **BUILDSDK_CXXFLAGS** when building
         for an SDK (i.e. `nativesdk-`)

   **D**
      The destination directory. The location in the **Build Directory**
      where components are installed by the
      ref-tasks-install task. This location defaults
      to::

         ${WORKDIR}/image

      .. note::

         Tasks that read from or write to this directory should run under
         fakeroot .

   **DATE**
      The date the build was started. Dates appear using the year, month,
      and day (YMD) format (e.g. "20150209" for February 9th, 2015).

   **DATETIME**
      The date and time on which the current build started. The format is
      suitable for timestamps.

   **DEBIAN_NOAUTONAME**
      When the ref-classes-debian class is inherited,
      which is the default behavior, **DEBIAN_NOAUTONAME** specifies a
      particular package should not be renamed according to Debian library
      package naming. You must use the package name as an override when you
      set this variable. Here is an example from the `fontconfig` recipe::

         DEBIAN_NOAUTONAME:fontconfig-utils = "1"

   **DEBIANNAME**
      When the ref-classes-debian class is inherited,
      which is the default behavior, **DEBIANNAME** allows you to override
      the library name for an individual package. Overriding the library
      name in these cases is rare. You must use the package name as an
      override when you set this variable. Here is an example from the
      `dbus` recipe::

         DEBIANNAME:${PN} = "dbus-1"

   **DEBUG_BUILD**
      Specifies to build packages with debugging information. This
      influences the value of the **SELECTED_OPTIMIZATION** variable.

   **DEBUG_OPTIMIZATION**
      The options to pass in **TARGET_CFLAGS** and **CFLAGS** when
      compiling a system for debugging. This variable defaults to "-O
      -fno-omit-frame-pointer ${DEBUG_FLAGS} -pipe".

   **DEBUG_PREFIX_MAP**
      Allows to set C compiler options, such as `-fdebug-prefix-map`,
      `-fmacro-prefix-map`, and `-ffile-prefix-map`, which allow to
      replace build-time paths by install-time ones in the debugging sections
      of binaries.  This makes compiler output files location independent,
      at the cost of having to pass an extra command to tell the debugger
      where source files are.

      This is used by the Yocto Project to guarantee
      /test-manual/reproducible-builds even when the source code of
      a package uses the `__FILE__` or `assert()` macros. See the
      `reproducible-builds.org <https://reproducible-builds.org/docs/build-path/>`__
      website for details.

      This variable is set in the `meta/conf/bitbake.conf` file. It is
      not intended to be user-configurable.

   **DEFAULT_PREFERENCE**
      Specifies a weak bias for recipe selection priority.

      The most common usage of this is variable is to set it to "-1" within
      a recipe for a development version of a piece of software. Using the
      variable in this way causes the stable version of the recipe to build
      by default in the absence of **PREFERRED_VERSION** being used to
      build the development version.

      .. note::

         The bias provided by **DEFAULT_PREFERENCE** is weak and is overridden
         by **BBFILE_PRIORITY** if that variable is different between two
         layers that contain different versions of the same recipe.

   **DEFAULT_TIMEZONE**
      Specifies the time zone set in the image.

      This variable causes the `tzdata` package to configure
      `${sysconfdir}/localtime` accordingly. Valid values are all files
      found in `/usr/share/zoneinfo` like `CET` or `Asia/Baku`.

   **DEFAULTTUNE**
      The default CPU and Application Binary Interface (ABI) tunings (i.e.
      the "tune") used by the OpenEmbedded build system. The
      **DEFAULTTUNE** helps define
      **TUNE_FEATURES**.

      The default tune is either implicitly or explicitly set by the
      machine (**MACHINE**). However, you can override
      the setting using available tunes as defined with
      **AVAILTUNES**.

   **DEPENDS**
      Lists a recipe's build-time dependencies. These are dependencies on
      other recipes whose contents (e.g. headers and shared libraries) are
      needed by the recipe at build time.

      As an example, consider a recipe `foo` that contains the following
      assignment::

          DEPENDS = "bar"

      The practical effect of the previous assignment is that all files
      installed by bar will be available in the appropriate staging sysroot,
      given by the **STAGING_DIR* <STAGING_DIR>** variables, by the time
      the ref-tasks-configure task for `foo` runs. This mechanism is
      implemented by having ref-tasks-configure depend on the
      ref-tasks-populate_sysroot task of each recipe listed in
      **DEPENDS**, through a
      `[`\ deptask \ `]`
      declaration in the ref-classes-base class.

      .. note::

         It seldom is necessary to reference, for example, **STAGING_DIR_HOST**
         explicitly. The standard classes and build-related variables are
         configured to automatically use the appropriate staging sysroots.

      As another example, **DEPENDS** can also be used to add utilities
      that run on the build machine during the build. For example, a recipe
      that makes use of a code generator built by the recipe `codegen`
      might have the following::

         DEPENDS = "codegen-native"

      For more
      information, see the ref-classes-native class and
      the **EXTRANATIVEPATH** variable.

      .. note::

         -  **DEPENDS** is a list of recipe names. Or, to be more precise,
            it is a list of **PROVIDES** names, which
            usually match recipe names. Putting a package name such as
            "foo-dev" in **DEPENDS** does not make sense. Use "foo"
            instead, as this will put files from all the packages that make
            up `foo`, which includes those from `foo-dev`, into the
            sysroot.

         -  One recipe having another recipe in **DEPENDS** does not by
            itself add any runtime dependencies between the packages
            produced by the two recipes. However, as explained in the
            "overview-manual/concepts:automatically added runtime dependencies"
            section in the Yocto Project Overview and Concepts Manual,
            runtime dependencies will often be added automatically, meaning
            **DEPENDS** alone is sufficient for most recipes.

         -  Counterintuitively, **DEPENDS** is often necessary even for
            recipes that install precompiled components. For example, if
            `libfoo` is a precompiled library that links against
            `libbar`, then linking against `libfoo` requires both
            `libfoo` and `libbar` to be available in the sysroot.
            Without a **DEPENDS** from the recipe that installs `libfoo`
            to the recipe that installs `libbar`, other recipes might
            fail to link against `libfoo`.

      For information on runtime dependencies, see the **RDEPENDS**
      variable. You can also see the
      "bitbake-user-manual/bitbake-user-manual-metadata:tasks" and
      "bitbake-user-manual/bitbake-user-manual-execution:dependencies"
      sections in the BitBake User Manual for additional information on tasks
      and dependencies.

   **DEPLOY_DIR**
      Points to the general area that the OpenEmbedded build system uses to
      place images, packages, SDKs, and other output files that are ready
      to be used outside of the build system. By default, this directory
      resides within the **Build Directory** as `${TMPDIR}/deploy`.

      For more information on the structure of the Build Directory, see
      "ref-manual/structure:the build directory --- `build/```" section.
      For more detail on the contents of the `deploy` directory, see the
      "overview-manual/concepts:images",
      "overview-manual/concepts:package feeds", and
      "overview-manual/concepts:application development sdk" sections all in the
      Yocto Project Overview and Concepts Manual.

   **DEPLOY_DIR_DEB**
      Points to the area that the OpenEmbedded build system uses to place
      Debian packages that are ready to be used outside of the build
      system. This variable applies only when **PACKAGE_CLASSES** contains
      "ref-classes-package_deb".

      The BitBake configuration file initially defines the
      **DEPLOY_DIR_DEB** variable as a sub-folder of
      **DEPLOY_DIR**::

         DEPLOY_DIR_DEB = "${DEPLOY_DIR}/deb"

      The ref-classes-package_deb class uses the
      **DEPLOY_DIR_DEB** variable to make sure the
      ref-tasks-package_write_deb task
      writes Debian packages into the appropriate folder. For more
      information on how packaging works, see the
      "overview-manual/concepts:package feeds" section
      in the Yocto Project Overview and Concepts Manual.

   **DEPLOY_DIR_IMAGE**
      Points to the area that the OpenEmbedded build system uses to place
      images and other associated output files that are ready to be
      deployed onto the target machine. The directory is machine-specific
      as it contains the `${MACHINE}` name. By default, this directory
      resides within the **Build Directory** as
      `${DEPLOY_DIR}/images/${MACHINE}/`.

      It must not be used directly in recipes when deploying files. Instead,
      it's only useful when a recipe needs to "read" a file already deployed
      by a dependency. So, it should be filled with the contents of
      **DEPLOYDIR** by the ref-classes-deploy class or with the
      contents of **IMGDEPLOYDIR** by the ref-classes-image class.

      For more information on the structure of the **Build Directory**, see
      "ref-manual/structure:the build directory --- `build/```" section.
      For more detail on the contents of the `deploy` directory, see the
      "overview-manual/concepts:images" and
      "overview-manual/concepts:application development sdk" sections both in
      the Yocto Project Overview and Concepts Manual.

   **DEPLOY_DIR_IPK**
      Points to the area that the OpenEmbedded build system uses to place
      IPK packages that are ready to be used outside of the build system.
      This variable applies only when **PACKAGE_CLASSES** contains
      "ref-classes-package_ipk".

      The BitBake configuration file initially defines this variable as a
      sub-folder of **DEPLOY_DIR**::

         DEPLOY_DIR_IPK = "${DEPLOY_DIR}/ipk"

      The ref-classes-package_ipk class uses the **DEPLOY_DIR_IPK**
      variable to make sure the ref-tasks-package_write_ipk task
      writes IPK packages into the appropriate folder. For more information
      on how packaging works, see the
      "overview-manual/concepts:package feeds" section
      in the Yocto Project Overview and Concepts Manual.

   **DEPLOY_DIR_RPM**
      Points to the area that the OpenEmbedded build system uses to place
      RPM packages that are ready to be used outside of the build system.
      This variable applies only when **PACKAGE_CLASSES** contains
      "ref-classes-package_rpm".

      The BitBake configuration file initially defines this variable as a
      sub-folder of **DEPLOY_DIR**::

         DEPLOY_DIR_RPM = "${DEPLOY_DIR}/rpm"

      The ref-classes-package_rpm class uses the
      **DEPLOY_DIR_RPM** variable to make sure the
      ref-tasks-package_write_rpm task
      writes RPM packages into the appropriate folder. For more information
      on how packaging works, see the
      "overview-manual/concepts:package feeds" section
      in the Yocto Project Overview and Concepts Manual.

   **DEPLOYDIR**
      When inheriting the ref-classes-deploy class, the
      **DEPLOYDIR** points to a temporary work area for deployed files that
      is set in the ref-classes-deploy class as follows::

         DEPLOYDIR = "${WORKDIR}/deploy-${PN}"

      Recipes inheriting the ref-classes-deploy class should copy files to be
      deployed into **DEPLOYDIR**, and the class will take care of copying
      them into **DEPLOY_DIR_IMAGE**
      afterwards.

   **DESCRIPTION**
      The package description used by package managers. If not set,
      **DESCRIPTION** takes the value of the **SUMMARY**
      variable.

   **DEV_PKG_DEPENDENCY**
      Provides an easy way for recipes to disable or adjust the runtime recommendation
      (**RRECOMMENDS**) of the `${PN}-dev` package on the main
      (`${PN}`) package.

   **DISABLE_STATIC**
      Used in order to disable static linking by default (in order to save
      space, since static libraries are often unused in embedded systems.)
      The default value is " --disable-static", however it can be set to ""
      in order to enable static linking if desired. Certain recipes do this
      individually, and also there is a
      `meta/conf/distro/include/no-static-libs.inc` include file that
      disables static linking for a number of recipes. Some software
      packages or build tools (such as CMake) have explicit support for
      enabling / disabling static linking, and in those cases
      **DISABLE_STATIC** is not used.

   **DISTRO**
      The short name of the distribution. For information on the long name
      of the distribution, see the **DISTRO_NAME**
      variable.

      The **DISTRO** variable corresponds to a distribution configuration
      file whose root name is the same as the variable's argument and whose
      filename extension is `.conf`. For example, the distribution
      configuration file for the Poky distribution is named `poky.conf`
      and resides in the `meta-poky/conf/distro` directory of
      meta-poky .

      Within that `poky.conf` file, the **DISTRO** variable is set as
      follows::

         DISTRO = "poky"

      Distribution configuration files are located in a `conf/distro`
      directory within the **Metadata** that contains the
      distribution configuration. The value for **DISTRO** must not contain
      spaces, and is typically all lower-case.

      .. note::

         If the **DISTRO** variable is blank, a set of default configurations
         are used, which are specified within
         `meta/conf/distro/defaultsetup.conf` also in :term:`OpenEmbedded-Core
         (OE-Core)`.

   **DISTRO_CODENAME**
      Specifies a codename for the distribution being built.

   **DISTRO_EXTRA_RDEPENDS**
      Specifies a list of distro-specific packages to add to all images.
      This variable takes effect through `packagegroup-base` so the
      variable only really applies to the more full-featured images that
      include `packagegroup-base`. You can use this variable to keep
      distro policy out of generic images. As with all other distro
      variables, you set this variable in the distro `.conf` file.

   **DISTRO_EXTRA_RRECOMMENDS**
      Specifies a list of distro-specific packages to add to all images if
      the packages exist. The packages might not exist or be empty (e.g.
      kernel modules). The list of packages are automatically installed but
      you can remove them.

   **DISTRO_FEATURES**
      The software support you want in your distribution for various
      features. You define your distribution features in the distribution
      configuration file.

      In most cases, the presence or absence of a feature in
      **DISTRO_FEATURES** is translated to the appropriate option supplied
      to the configure script during the
      ref-tasks-configure task for recipes that
      optionally support the feature. For example, specifying "x11" in
      **DISTRO_FEATURES**, causes every piece of software built for the
      target that can optionally support X11 to have its X11 support
      enabled.

      .. note::

         Just enabling **DISTRO_FEATURES** alone doesn't
         enable feature support for packages. Mechanisms such as making
         **PACKAGECONFIG** track **DISTRO_FEATURES** are used
         to enable/disable package features.

      Two more examples are Bluetooth and NFS support. For a more complete
      list of features that ships with the Yocto Project and that you can
      provide with this variable, see the "ref-features-distro" section.

   **DISTRO_FEATURES_BACKFILL**
      A space-separated list of features to be added to **DISTRO_FEATURES**
      if not also present in **DISTRO_FEATURES_BACKFILL_CONSIDERED**.

      This variable is set in the `meta/conf/bitbake.conf` file. It is
      not intended to be user-configurable. It is best to just reference
      the variable to see which distro features are being
      backfilled  for all distro configurations.

   **DISTRO_FEATURES_BACKFILL_CONSIDERED**
      A space-separated list of features from **DISTRO_FEATURES_BACKFILL**
      that should not be backfilled  (i.e. added
      to **DISTRO_FEATURES**) during the build.

      This corresponds to an opt-out mechanism. When new default distro
      features are introduced, distribution maintainers can review (`consider`)
      them and decide to exclude them from the
      backfilled  features. Therefore, the
      combination of **DISTRO_FEATURES_BACKFILL** and
      **DISTRO_FEATURES_BACKFILL_CONSIDERED** makes it possible to
      add new default features without breaking existing distributions.

   **DISTRO_FEATURES_DEFAULT**
      A convenience variable that gives you the default list of distro
      features with the exception of any features specific to the C library
      (`libc`).

      When creating a custom distribution, you might find it useful to be
      able to reuse the default
      **DISTRO_FEATURES** options without the
      need to write out the full set. Here is an example that uses
      **DISTRO_FEATURES_DEFAULT** from a custom distro configuration file::

         DISTRO_FEATURES ?= "${DISTRO_FEATURES_DEFAULT} myfeature"

   **DISTRO_FEATURES_FILTER_NATIVE**
      Specifies a list of features that if present in the target
      **DISTRO_FEATURES** value should be
      included in **DISTRO_FEATURES** when building native recipes. This
      variable is used in addition to the features filtered using the
      **DISTRO_FEATURES_NATIVE**
      variable.

   **DISTRO_FEATURES_FILTER_NATIVESDK**
      Specifies a list of features that if present in the target
      **DISTRO_FEATURES** value should be included in
      **DISTRO_FEATURES** when building ref-classes-nativesdk
      recipes. This variable is used in addition to the features filtered using
      the **DISTRO_FEATURES_NATIVESDK** variable.

   **DISTRO_FEATURES_NATIVE**
      Specifies a list of features that should be included in
      **DISTRO_FEATURES** when building native
      recipes. This variable is used in addition to the features filtered
      using the
      **DISTRO_FEATURES_FILTER_NATIVE**
      variable.

   **DISTRO_FEATURES_NATIVESDK**
      Specifies a list of features that should be included in
      **DISTRO_FEATURES** when building
      ref-classes-nativesdk recipes. This variable is used
      in addition to the features filtered using the
      **DISTRO_FEATURES_FILTER_NATIVESDK** variable.

   **DISTRO_NAME**
      The long name of the distribution. For information on the short name
      of the distribution, see the **DISTRO** variable.

      The **DISTRO_NAME** variable corresponds to a distribution
      configuration file whose root name is the same as the variable's
      argument and whose filename extension is `.conf`. For example, the
      distribution configuration file for the Poky distribution is named
      `poky.conf` and resides in the `meta-poky/conf/distro` directory
      of meta-poky .

      Within that `poky.conf` file, the **DISTRO_NAME** variable is set
      as follows::

         DISTRO_NAME = "Poky (Yocto Project Reference Distro)"

      Distribution configuration files are located in a `conf/distro`
      directory within the **Metadata** that contains the
      distribution configuration.

      .. note::

         If the **DISTRO_NAME** variable is blank, a set of default
         configurations are used, which are specified within
         `meta/conf/distro/defaultsetup.conf` also in :term:`OpenEmbedded-Core
         (OE-Core)`.

   **DISTRO_VERSION**
      The version of the distribution.

   **DISTROOVERRIDES**
      A colon-separated list of overrides specific to the current
      distribution. By default, this list includes the value of
      **DISTRO**.

      You can extend **DISTROOVERRIDES** to add extra overrides that should
      apply to the distribution.

      The underlying mechanism behind **DISTROOVERRIDES** is simply that it
      is included in the default value of
      **OVERRIDES**.

      Here is an example from :yocto_git:`meta-poky/conf/distro/poky-tiny.conf
      </meta-yocto/tree/meta-poky/conf/distro/poky-tiny.conf>`::

         DISTROOVERRIDES = "poky:poky-tiny"

   **DL_DIR**
      The central download directory used by the build process to store
      downloads. By default, **DL_DIR** gets files suitable for mirroring
      for everything except Git repositories. If you want tarballs of Git
      repositories, use the
      **BB_GENERATE_MIRROR_TARBALLS**
      variable.

      You can set this directory by defining the **DL_DIR** variable in the
      `conf/local.conf` file. This directory is self-maintaining and you
      should not have to touch it. By default, the directory is
      `downloads` in the **Build Directory**::

         #DL_DIR ?= "${TOPDIR}/downloads"

      To specify a different download directory,
      simply remove the comment from the line and provide your directory.

      During a first build, the system downloads many different source code
      tarballs from various upstream projects. Downloading can take a
      while, particularly if your network connection is slow. Tarballs are
      all stored in the directory defined by **DL_DIR** and the build
      system looks there first to find source tarballs.

      .. note::

         When wiping and rebuilding, you can preserve this directory to
         speed up this part of subsequent builds.

      You can safely share this directory between multiple builds on the
      same development machine. For additional information on how the build
      process gets source files when working behind a firewall or proxy
      server, see this specific question in the "/ref-manual/faq"
      chapter. You can also refer to the
      "Working Behind a Network Proxy "
      Wiki page.

   **DOC_COMPRESS**
      When inheriting the ref-classes-compress_doc
      class, this variable sets the compression policy used when the
      OpenEmbedded build system compresses manual and info pages. By
      default, the compression method used is gz (gzip). Other policies
      available are xz and bz2.

      For information on policies and on how to use this variable, see the
      comments in the `meta/classes-recipe/compress_doc.bbclass` file.

   **DT_FILES**
      Space-separated list of device tree source files to compile using
      a recipe that inherits the ref-classes-devicetree class. These
      are relative to the **DT_FILES_PATH**.

      For convenience, both `.dts` and `.dtb` extensions can be used.

      Use an empty string (default) to build all device tree sources within
      the **DT_FILES_PATH** directory.

   **DT_FILES_PATH**
      When compiling out-of-tree device tree sources using a recipe that
      inherits the ref-classes-devicetree class, this variable specifies
      the path to the directory containing dts files to build.

      Defaults to the **S** directory.

   **DT_PADDING_SIZE**
      When inheriting the ref-classes-devicetree class, this variable
      specifies the size of padding appended to the device tree blob, used as
      extra space typically for additional properties during boot.

   **EFI_ARCH**
      The CPU architecture name within EFI standard. Set in
      :oe_git:`meta/conf/image-uefi.conf
      <openembedded-core/tree/meta/conf/image-uefi.conf>`.

   **EFI_PROVIDER**
      When building bootable images (i.e. where `hddimg`, `iso`, or
      `wic.vmdk` is in **IMAGE_FSTYPES**), the
      **EFI_PROVIDER** variable specifies the EFI bootloader to use. The
      default is "grub-efi", but "systemd-boot" can be used instead.

      See the ref-classes-systemd-boot and ref-classes-image-live
      classes for more information.

   **EFI_UKI_DIR**
      The primary place for the UKI image inside the EFI System Partition.

   **EFI_UKI_PATH**
      The path for the UKI image inside the root filesystem.

   **ENABLE_BINARY_LOCALE_GENERATION**
      Variable that controls which locales for `glibc` are generated
      during the build (useful if the target device has 64Mbytes of RAM or
      less).

   **ERR_REPORT_DIR**
      When used with the ref-classes-report-error class, specifies the
      path used for storing the debug files created by the :ref:`error reporting
      tool <dev-manual/error-reporting-tool:using the error reporting tool>`,
      which allows you to submit build errors you encounter to a central
      database. By default, the value of this variable is
      `${`\ **LOG_DIR**\ `}/error-report`.

      You can set **ERR_REPORT_DIR** to the path you want the error
      reporting tool to store the debug files as follows in your
      `local.conf` file::

         ERR_REPORT_DIR = "path"

   **ERROR_QA**
      Specifies the quality assurance checks whose failures are reported as
      errors by the OpenEmbedded build system. You set this variable in
      your distribution configuration file. For a list of the checks you
      can control with this variable, see the
      "ref-classes-insane" section.

   **ESDK_CLASS_INHERIT_DISABLE**
      A list of classes to remove from the **INHERIT**
      value globally within the extensible SDK configuration. The
      populate-sdk-ext  class sets the
      default value::

         ESDK_CLASS_INHERIT_DISABLE ?= "buildhistory"

      Some classes are not generally applicable within the extensible SDK
      context. You can use this variable to disable those classes.

      For additional information on how to customize the extensible SDK's
      configuration, see the
      "sdk-manual/appendix-customizing:configuring the extensible sdk"
      section in the Yocto Project Application Development and the
      Extensible Software Development Kit (eSDK) manual.

   **ESDK_LOCALCONF_ALLOW**
      A list of variables allowed through from the OpenEmbedded build
      system configuration into the extensible SDK configuration. By
      default, the list of variables is empty and is set in the
      populate-sdk-ext  class.

      This list overrides the variables specified using the
      **ESDK_LOCALCONF_REMOVE** variable as well as
      other variables automatically added due to the "/" character
      being found at the start of the
      value, which is usually indicative of being a path and thus might not
      be valid on the system where the SDK is installed.

      For additional information on how to customize the extensible SDK's
      configuration, see the
      "sdk-manual/appendix-customizing:configuring the extensible sdk"
      section in the Yocto Project Application Development and the
      Extensible Software Development Kit (eSDK) manual.

   **ESDK_LOCALCONF_REMOVE**
      A list of variables not allowed through from the OpenEmbedded build
      system configuration into the extensible SDK configuration. Usually,
      these are variables that are specific to the machine on which the
      build system is running and thus would be potentially problematic
      within the extensible SDK.

      By default, **ESDK_LOCALCONF_REMOVE** is set in the
      populate-sdk-ext  class and
      excludes the following variables:

      - **CONF_VERSION**
      - **BB_NUMBER_THREADS**
      - **BB_NUMBER_PARSE_THREADS**
      - **PARALLEL_MAKE**
      - **PRSERV_HOST**
      - **SSTATE_MIRRORS** **DL_DIR**
      - **SSTATE_DIR** **TMPDIR**
      - **BB_SERVER_TIMEOUT**

      For additional information on how to customize the extensible SDK's
      configuration, see the
      "sdk-manual/appendix-customizing:configuring the extensible sdk"
      section in the Yocto Project Application Development and the
      Extensible Software Development Kit (eSDK) manual.

   **EXCLUDE_FROM_SHLIBS**
      Triggers the OpenEmbedded build system's shared libraries resolver to
      exclude an entire package when scanning for shared libraries.

      .. note::

         The shared libraries resolver's functionality results in part from
         the internal function `package_do_shlibs`, which is part of the
         ref-tasks-package task. You should be aware that the shared
         libraries resolver might implicitly define some dependencies between
         packages.

      The **EXCLUDE_FROM_SHLIBS** variable is similar to the
      **PRIVATE_LIBS** variable, which excludes a
      package's particular libraries only and not the whole package.

      Use the **EXCLUDE_FROM_SHLIBS** variable by setting it to "1" for a
      particular package::

         EXCLUDE_FROM_SHLIBS = "1"

   **EXCLUDE_FROM_WORLD**
      Directs BitBake to exclude a recipe from world builds (i.e.
      `bitbake world`). During world builds, BitBake locates, parses and
      builds all recipes found in every layer exposed in the
      `bblayers.conf` configuration file.

      To exclude a recipe from a world build using this variable, set the
      variable to "1" in the recipe.

      .. note::

         Recipes added to **EXCLUDE_FROM_WORLD** may still be built during a
         world build in order to satisfy dependencies of other recipes. Adding
         a recipe to **EXCLUDE_FROM_WORLD** only ensures that the recipe is not
         explicitly added to the list of build targets in a world build.

   **EXTENDPE**
      Used with file and pathnames to create a prefix for a recipe's
      version based on the recipe's **PE** value. If **PE**
      is set and greater than zero for a recipe, **EXTENDPE** becomes that
      value (e.g if **PE** is equal to "1" then **EXTENDPE** becomes "1").
      If a recipe's **PE** is not set (the default) or is equal to zero,
      **EXTENDPE** becomes "".

      See the **STAMP** variable for an example.

   **EXTENDPKGV**
      The full package version specification as it appears on the final
      packages produced by a recipe. The variable's value is normally used
      to fix a runtime dependency to the exact same version of another
      package in the same recipe::

         RDEPENDS:${PN}-additional-module = "${PN} (= ${EXTENDPKGV})"

      The dependency relationships are intended to force the package
      manager to upgrade these types of packages in lock-step.

   **EXTERNAL_KERNEL_DEVICETREE**
      When inheriting ref-classes-kernel-fit-image and a
      **PREFERRED_PROVIDER** for `virtual/dtb` set to `devicetree`, the
      variable **EXTERNAL_KERNEL_DEVICETREE** can be used to specify a
      directory containing one or more compiled device tree or device tree
      overlays to use.

   **EXTERNAL_KERNEL_TOOLS**
      When set, the **EXTERNAL_KERNEL_TOOLS** variable indicates that these
      tools are not in the source tree.

      When kernel tools are available in the tree, they are preferred over
      any externally installed tools. Setting the **EXTERNAL_KERNEL_TOOLS**
      variable tells the OpenEmbedded build system to prefer the installed
      external tools. See the ref-classes-kernel-yocto class in
      `meta/classes-recipe` to see how the variable is used.

   **EXTERNAL_TOOLCHAIN**
      When you intend to use an
      external toolchain ,
      this variable allows to specify the directory where this toolchain was
      installed.

   **EXTERNALSRC**
      When inheriting the ref-classes-externalsrc
      class, this variable points to the source tree, which is outside of
      the OpenEmbedded build system. When set, this variable sets the
      **S** variable, which is what the OpenEmbedded build
      system uses to locate unpacked recipe source code.

      See the "ref-classes-externalsrc" section for details. You
      can also find information on how to use this variable in the
      "dev-manual/building:building software from an external source"
      section in the Yocto Project Development Tasks Manual.

   **EXTERNALSRC_BUILD**
      When inheriting the ref-classes-externalsrc
      class, this variable points to the directory in which the recipe's
      source code is built, which is outside of the OpenEmbedded build
      system. When set, this variable sets the **B** variable,
      which is what the OpenEmbedded build system uses to locate the
      **Build Directory**.

      See the "ref-classes-externalsrc" section for details. You
      can also find information on how to use this variable in the
      "dev-manual/building:building software from an external source"
      section in the Yocto Project Development Tasks Manual.

   **EXTRA_AUTORECONF**
      For recipes inheriting the ref-classes-autotools
      class, you can use **EXTRA_AUTORECONF** to specify extra options to
      pass to the `autoreconf` command that is executed during the
      ref-tasks-configure task.

      The default value is "--exclude=autopoint".

   **EXTRA_IMAGE_FEATURES**
      A list of additional features to include in an image. When listing
      more than one feature, separate them with a space.

      Typically, you configure this variable in your `local.conf` file,
      which is found in the **Build Directory**. Although you can use this
      variable from within a recipe, best practices dictate that you do not.

      .. note::

         To enable primary features from within the image recipe, use the
         **IMAGE_FEATURES** variable.

      Here are some examples of features you can add:

        - "dbg-pkgs" --- adds -dbg packages for all installed packages including
          symbol information for debugging and profiling.

        - "empty-root-password" --- This feature can be used if you want to
          allow root login with an empty password.
        - "allow-empty-password" --- Allows Dropbear and OpenSSH to accept
          logins from accounts having an empty password string.
        - "allow-root-login" --- Allows Dropbear and OpenSSH to accept root logins.
        - "post-install-logging" --- Enables logging postinstall script runs to
          the `/var/log/postinstall.log` file on first boot of the image on
          the target system.
        - "dev-pkgs" --- adds -dev packages for all installed packages. This is
          useful if you want to develop against the libraries in the image.
        - "read-only-rootfs" --- creates an image whose root filesystem is
          read-only. See the
          "security-manual/read-only-rootfs:creating a read-only root filesystem"
          section in the Yocto Project Development Tasks Manual for more
          information
        - "tools-debug" --- adds debugging tools such as gdb and strace.
        - "tools-sdk" --- adds development tools such as gcc, make,
          pkgconfig and so forth.
        - "tools-testapps" --- adds useful testing tools
          such as ts_print, aplay, arecord and so forth.

      For a complete list of image features that ships with the Yocto
      Project, see the "ref-features-image" section.

      For an example that shows how to customize your image by using this
      variable, see the "dev-manual/customizing-images:customizing images using custom \\`image_features\`\` and \`\`extra_image_features\`\``"
      section in the Yocto Project Development Tasks Manual.

   **EXTRA_IMAGECMD**
      Specifies additional options for the image creation command that has
      been specified in **IMAGE_CMD**. When setting
      this variable, use an override for the associated image type. Here is
      an example::

         EXTRA_IMAGECMD:ext3 ?= "-i 4096"

   **EXTRA_IMAGEDEPENDS**
      A list of recipes to build that do not provide packages for
      installing into the root filesystem.

      Sometimes a recipe is required to build the final image but is not
      needed in the root filesystem. You can use the **EXTRA_IMAGEDEPENDS**
      variable to list these recipes and thus specify the dependencies. A
      typical example is a required bootloader in a machine configuration.

      .. note::

         To add packages to the root filesystem, see the various
         **RDEPENDS** and **RRECOMMENDS** variables.

   **EXTRA_OECMAKE**
      Additional `CMake <https://cmake.org/overview/>`__ options. See the
      ref-classes-cmake class for additional information.

   **EXTRA_OECONF**
      Additional `configure` script options. See
      **PACKAGECONFIG_CONFARGS** for
      additional information on passing configure script options.

   **EXTRA_OEMAKE**
      Additional GNU `make` options.

      Because the **EXTRA_OEMAKE** defaults to "", you need to set the
      variable to specify any required GNU options.

      **PARALLEL_MAKE** and
      **PARALLEL_MAKEINST** also make use of
      **EXTRA_OEMAKE** to pass the required flags.

   **EXTRA_OEMESON**
      Additional `Meson <https://mesonbuild.com/>`__ options. See the
      ref-classes-meson class for additional information.

      In addition to standard Meson options, such options correspond to
      `Meson build options <https://mesonbuild.com/Build-options.html>`__
      defined in the `meson_options.txt` file in the sources to build.
      Here is an example::

         EXTRA_OEMESON = "-Dpython=disabled -Dvalgrind=disabled"

      Note that any custom value for the Meson `--buildtype` option
      should be set through the **MESON_BUILDTYPE** variable.

   **EXTRA_OESCONS**
      When inheriting the ref-classes-scons class, this
      variable specifies additional configuration options you want to pass
      to the `scons` command line.

   **EXTRA_USERS_PARAMS**
      When inheriting the ref-classes-extrausers
      class, this variable provides image level user and group operations.
      This is a more global method of providing user and group
      configuration as compared to using the
      ref-classes-useradd class, which ties user and
      group configurations to a specific recipe.

      The set list of commands you can configure using the
      **EXTRA_USERS_PARAMS** is shown in the
      ref-classes-extrausers class. These commands map to the normal
      Unix commands of the same names::

         # EXTRA_USERS_PARAMS = "\
         # useradd -p '' tester; \
         # groupadd developers; \
         # userdel nobody; \
         # groupdel -g video; \
         # groupmod -g 1020 developers; \
         # usermod -s /bin/sh tester; \
         # "

      Hardcoded passwords are supported via the `-p` parameters for
      `useradd` or `usermod`, but only hashed.

      Here is an example that adds two users named "tester-jim" and "tester-sue" and assigns
      passwords. First on host, create the (escaped) password hash::

         printf "%q" $(mkpasswd -m sha256crypt tester01)

      The resulting hash is set to a variable and used in `useradd` command parameters::

         inherit extrausers
         PASSWD = "\$X\$ABC123\$A-Long-Hash"
         EXTRA_USERS_PARAMS = "\
             useradd -p '${PASSWD}' tester-jim; \
             useradd -p '${PASSWD}' tester-sue; \
             "

      Finally, here is an example that sets the root password::

         inherit extrausers
         EXTRA_USERS_PARAMS = "\
             usermod -p '${PASSWD}' root; \
             "

      .. note::

         From a security perspective, hardcoding a default password is not
         generally a good idea or even legal in some jurisdictions. It is
         recommended that you do not do this if you are building a production
         image.

      Additionally there is a special `passwd-expire` command that will
      cause the password for a user to be expired and thus force changing it
      on first login, for example::

         EXTRA_USERS_PARAMS += " useradd myuser; passwd-expire myuser;"

      .. note::

         At present, `passwd-expire` may only work for remote logins when
         using OpenSSH and not dropbear as an SSH server.

   **EXTRANATIVEPATH**
      A list of subdirectories of
      `${`\ **STAGING_BINDIR_NATIVE**\ `}`
      added to the beginning of the environment variable `PATH`. As an
      example, the following prepends
      "${STAGING_BINDIR_NATIVE}/foo:${STAGING_BINDIR_NATIVE}/bar:" to
      `PATH`::

         EXTRANATIVEPATH = "foo bar"

   **FAKEROOT**
      See **bitbake:FAKEROOT** in the BitBake manual.

   **FAKEROOTBASEENV**
      See **bitbake:FAKEROOTBASEENV** in the BitBake manual.

   **FAKEROOTCMD**
      See **bitbake:FAKEROOTCMD** in the BitBake manual.

   **FAKEROOTDIRS**
      See **bitbake:FAKEROOTDIRS** in the BitBake manual.

   **FAKEROOTENV**
      See **bitbake:FAKEROOTENV** in the BitBake manual.

   **FAKEROOTNOENV**
      See **bitbake:FAKEROOTNOENV** in the BitBake manual.

   **FC**
      The minimal command and arguments used to run the Fortran compiler.

   **FEATURE_PACKAGES**
      Defines one or more packages to include in an image when a specific
      item is included in **IMAGE_FEATURES**.
      When setting the value, **FEATURE_PACKAGES** should have the name of
      the feature item as an override. Here is an example::

         FEATURE_PACKAGES_widget = "package1 package2"

      In this example, if "widget" were added to **IMAGE_FEATURES**,
      package1 and package2 would be included in the image.

      .. note::

         Packages installed by features defined through **FEATURE_PACKAGES**
         are often package groups. While similarly named, you should not
         confuse the **FEATURE_PACKAGES** variable with package groups, which
         are discussed elsewhere in the documentation.

   **FEED_DEPLOYDIR_BASE_URI**
      Points to the base URL of the server and location within the
      document-root that provides the metadata and packages required by
      OPKG to support runtime package management of IPK packages. You set
      this variable in your `local.conf` file.

      Consider the following example::

         FEED_DEPLOYDIR_BASE_URI = "http://192.168.7.1/BOARD-dir"

      This example assumes you are serving
      your packages over HTTP and your databases are located in a directory
      named `BOARD-dir`, which is underneath your HTTP server's
      document-root. In this case, the OpenEmbedded build system generates
      a set of configuration files for you in your target that work with
      the feed.

   **FETCHCMD**
      See **bitbake:FETCHCMD** in the BitBake manual.

   **FILE**
      See **bitbake:FILE** in the BitBake manual.

   **FILES**
      The list of files and directories that are placed in a package. The
      **PACKAGES** variable lists the packages
      generated by a recipe.

      To use the **FILES** variable, provide a package name override that
      identifies the resulting package. Then, provide a space-separated
      list of files or paths that identify the files you want included as
      part of the resulting package. Here is an example::

         FILES:${PN} += "${bindir}/mydir1 ${bindir}/mydir2/myfile"

      .. note::

         -  When specifying files or paths, you can pattern match using
            Python's
            `glob <https://docs.python.org/3/library/glob.html>`__
            syntax. For details on the syntax, see the documentation by
            following the previous link.

         -  When specifying paths as part of the **FILES** variable, it is
            good practice to use appropriate path variables. For example,
            use `${sysconfdir}` rather than `/etc`, or `${bindir}`
            rather than `/usr/bin`. You can find a list of these
            variables at the top of the `meta/conf/bitbake.conf` file in
            **OpenEmbedded-Core (OE-Core)**. You will also
            find the default values of the various `FILES:*` variables in
            this file.

      If some of the files you provide with the **FILES** variable are
      editable and you know they should not be overwritten during the
      package update process by the Package Management System (PMS), you
      can identify these files so that the PMS will not overwrite them. See
      the **CONFFILES** variable for information on
      how to identify these files to the PMS.

   **FILES_SOLIBSDEV**
      Defines the file specification to match
      **SOLIBSDEV**. In other words,
      **FILES_SOLIBSDEV** defines the full path name of the development
      symbolic link (symlink) for shared libraries on the target platform.

      The following statement from the `bitbake.conf` shows how it is
      set::

         FILES_SOLIBSDEV ?= "${base_libdir}/lib*${SOLIBSDEV} ${libdir}/lib*${SOLIBSDEV}"

   **FILESEXTRAPATHS**
      A colon-separated list to extend the search path the OpenEmbedded build
      system uses when looking for files and patches as it processes recipes
      and append files. The default directories BitBake uses when it processes
      recipes are initially defined by the **FILESPATH** variable. You can
      extend **FILESPATH** variable by using **FILESEXTRAPATHS**.

      Best practices dictate that you accomplish this by using
      **FILESEXTRAPATHS** from within a `.bbappend` file and that you
      prepend paths as follows::

         FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

      In the above example, the build system first
      looks for files in a directory that has the same name as the
      corresponding append file.

      .. note::

         When extending **FILESEXTRAPATHS**, be sure to use the immediate
         expansion (`:=`) operator. Immediate expansion makes sure that
         BitBake evaluates **THISDIR** at the time the
         directive is encountered rather than at some later time when
         expansion might result in a directory that does not contain the
         files you need.

         Also, include the trailing separating colon character if you are
         prepending. The trailing colon character is necessary because you
         are directing BitBake to extend the path by prepending directories
         to the search path.

      Here is another common use::

         FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

      In this example, the build system extends the
      **FILESPATH** variable to include a directory named `files` that is
      in the same directory as the corresponding append file.

      This next example specifically adds three paths::

         FILESEXTRAPATHS:prepend := "path_1:path_2:path_3:"

      A final example shows how you can extend the search path and include
      a **MACHINE**-specific override, which is useful
      in a BSP layer::

          FILESEXTRAPATHS:prepend:intel-x86-common := "${THISDIR}/${PN}:"

      The previous statement appears in the
      `linux-yocto-dev.bbappend` file, which is found in the
      overview-manual/development-environment:yocto project source repositories in
      `meta-intel/common/recipes-kernel/linux`. Here, the machine
      override is a special **PACKAGE_ARCH**
      definition for multiple `meta-intel` machines.

      .. note::

         For a layer that supports a single BSP, the override could just be
         the value of **MACHINE**.

      By prepending paths in `.bbappend` files, you allow multiple append
      files that reside in different layers but are used for the same
      recipe to correctly extend the path.

   **FILESOVERRIDES**
      A colon-separated list to specify a subset of **OVERRIDES** used by
      the OpenEmbedded build system for creating **FILESPATH**. The
      **FILESOVERRIDES** variable uses overrides to automatically extend
      the **FILESPATH** variable. For an example of how that works, see the
      **FILESPATH** variable description. Additionally, you find more
      information on how overrides are handled in the
      "bitbake-user-manual/bitbake-user-manual-metadata:conditional syntax (overrides)"
      section of the BitBake User Manual.

      By default, the **FILESOVERRIDES** variable is defined as::

         FILESOVERRIDES = "${TRANSLATED_TARGET_ARCH}:${MACHINEOVERRIDES}:${DISTROOVERRIDES}"

      .. note::

         Do not hand-edit the **FILESOVERRIDES** variable. The values match up
         with expected overrides and are used in an expected manner by the
         build system.

   **FILESPATH**
      A colon-separated list specifying the default set of directories the
      OpenEmbedded build system uses when searching for patches and files.

      During the build process, BitBake searches each directory in
      **FILESPATH** in the specified order when looking for files and
      patches specified by each `file://` URI in a recipe's
      **SRC_URI** statements.

      The default value for the **FILESPATH** variable is defined in the
      ref-classes-base class found in `meta/classes-global` in
      **OpenEmbedded-Core (OE-Core)**::

         FILESPATH = "${@base_set_filespath(["${FILE_DIRNAME}/${BP}", \
             "${FILE_DIRNAME}/${BPN}", "${FILE_DIRNAME}/files"], d)}"

      The
      **FILESPATH** variable is automatically extended using the overrides
      from the **FILESOVERRIDES** variable.

      .. note::

         -  Do not hand-edit the **FILESPATH** variable. If you want the
            build system to look in directories other than the defaults,
            extend the **FILESPATH** variable by using the
            **FILESEXTRAPATHS** variable.

         -  Be aware that the default **FILESPATH** directories do not map
            to directories in custom layers where append files
            (`.bbappend`) are used. If you want the build system to find
            patches or files that reside with your append files, you need
            to extend the **FILESPATH** variable by using the
            **FILESEXTRAPATHS** variable.

      You can take advantage of this searching behavior in useful ways. For
      example, consider a case where there is the following directory structure
      for general and machine-specific configurations::

         files/defconfig
         files/MACHINEA/defconfig
         files/MACHINEB/defconfig

      Also in the example, the **SRC_URI** statement contains
      "file://defconfig". Given this scenario, you can set
      **MACHINE** to "MACHINEA" and cause the build
      system to use files from `files/MACHINEA`. Set **MACHINE** to
      "MACHINEB" and the build system uses files from `files/MACHINEB`.
      Finally, for any machine other than "MACHINEA" and "MACHINEB", the
      build system uses files from `files/defconfig`.

      You can find out more about the patching process in the
      "overview-manual/concepts:patching" section
      in the Yocto Project Overview and Concepts Manual and the
      "dev-manual/new-recipe:patching code" section in
      the Yocto Project Development Tasks Manual. See the
      ref-tasks-patch task as well.

   **FILESYSTEM_PERMS_TABLES**
      Allows you to define your own file permissions settings tables as part
      of your configuration for the packaging process. For example, suppose
      you need a consistent set of custom permissions for a set of groups
      and users across an entire work project. It is best to do this in the
      packages themselves but this is not always possible.

      By default, the OpenEmbedded build system uses the `fs-perms.txt`,
      `fs-perms-volatile-log.txt` and `fs-perms-volatile-tmp.txt` which are
      located in the `meta/files` folder in **OpenEmbedded-Core (OE-Core)**. If
      you create your own permission setting table files, you should place
      those in your layer.

      You can override the value of **FILESYSTEM_PERMS_TABLES** variable
      in your distribution configuration file to point to your custom
      permission table files. You can specify one or more file permissions
      setting tables. The paths that you specify to these files must be defined
      within the **BBPATH** variable.

      In order to disable the volatile log, which is enabled by default, one
      can remove the `files/fs-perms-volatile-log.txt` value from
      `FILESYSTEM_PERMS_TABLES`. Similarly, in order to disable the volatile
      tmp, one can remove the `files/fs-perms-volatile-tmp.txt` value.

      For guidance on how to define your own file permissions settings
      tables, examine the existing `fs-perms.txt`,
      `fs-perms-volatile-log.txt` and `fs-perms-volatile-tmp.txt` files.

   **FIRMWARE_COMPRESSION**
      The **FIRMWARE_COMPRESSION** allows compressing the firmware provided
      by the `linux-firmware` recipe. The default value of this variable is an
      empty string (no compression), and the possible values it can take are
      `xz` and `zst`. This can allow significant disk space savings.

      For this to work, the Linux Kernel requires the
      `CONFIG_FW_LOADER_COMPRESS_XZ` or `CONFIG_FW_LOADER_COMPRESS_ZSTD`
      configuration options to be set.

   **FIT_ADDRESS_CELLS**
      Specifies the value of the `#address-cells` value for the
      description of the FIT image.

      The default value is set to "1" by the ref-classes-kernel-fit-image
      class, which corresponds to 32 bit addresses.

      For platforms that need to set 64 bit addresses, for example in
      **UBOOT_LOADADDRESS** and **UBOOT_ENTRYPOINT**, you need to
      set this value to "2", as two 32 bit values (cells) will be needed
      to represent such addresses.

      Here is an example setting "0x400000000" as a load address::

         FIT_ADDRESS_CELLS = "2"
         UBOOT_LOADADDRESS = "0x04 0x00000000"

      See `more details about #address-cells <https://elinux.org/Device_Tree_Usage#How_Addressing_Works>`__.

   **FIT_CONF_DEFAULT_DTB**
      Specifies the default device tree binary (dtb) file for a FIT image
      when multiple ones are provided.

      This variable is used in the ref-classes-kernel-fit-image class.

   **FIT_CONF_MAPPINGS**
      When inheriting the ref-classes-kernel-fit-image, the
      **FIT_CONF_MAPPINGS** variable allows specifying mappings to rename
      configuration nodes or add extra configuration nodes for existing device
      tree blobs (DTBs) in FIT images. This provides flexibility when a 1 to 1
      mapping between DTB names and configuration node names does not work,
      such as when the kernel defines a kernel specific name but the bootloader
      uses a different name. This might be the case when maintaining
      compatibility with existing boot scripts or bootloader configurations.
      But there are other use cases as well.

      The variable accepts a space-separated list of mapping commands:

      -  `dtb-conf:DTB_NAME:NEW_NAME`
         Renames the configuration node for a specific DTB. The `DTB_NAME` is
         the name of the DTB without its `.dtb` suffix.

      -  `dtb-extra-conf:DTB_NAME:EXTRA_NAME`
         Creates an additional configuration node for an existing DTB. This is
         useful when you need to provide alternative names for the same hardware
         configuration to maintain compatibility with existing boot scripts or
         bootloader configurations or just need an alias configuration name for
         some other reason.

      For example::

         FIT_CONF_MAPPINGS = "\
             dtb-extra-conf:am335x-bonegreen:bonegreen \
             dtb-conf:am335x-boneblack:bbblack"

      This generates three configuration nodes from two DTBs:

      -  `am335x-bonegreen`: the original configuration node for the
         `am335x-bonegreen` device tree
      -  `bonegreen`: an extra configuration node for the same
         `am335x-bonegreen` DTB
      -  `bbblack`: a renamed configuration node for the `am335x-boneblack` DTB

      The ref-classes-kernel-fit-image class validates all mappings
      and ensures they match existing DTBs.

   **FIT_CONF_PREFIX**
      When using the ref-classes-kernel-fit-image, this is the prefix
      used for creating FIT configuration nodes. Its default value is "conf-".

   **FIT_DESC**
      Specifies the description string encoded into a FIT image. The
      default value is set by the ref-classes-kernel-fit-image class as
      follows::

         FIT_DESC ?= "U-Boot fitImage for ${DISTRO_NAME}/${PV}/${MACHINE}"

   **FIT_GENERATE_KEYS**
      Decides whether to generate the keys for signing the FIT image if
      they don't already exist. The keys are created in
      **UBOOT_SIGN_KEYDIR**. The default value is set to "0"
      by the ref-classes-kernel-fit-image class.

   **FIT_HASH_ALG**
      Specifies the hash algorithm used in creating the FIT Image.
      This variable is set by default to "sha256" by the
      ref-classes-kernel-fit-image class.

   **FIT_KERNEL_COMP_ALG**
      The compression algorithm to use for the kernel image inside the FIT Image.
      At present, the only supported values are "gzip" (default), "lzo" or "none".
      If you set this variable to anything other than "none" you may also need
      to set **FIT_KERNEL_COMP_ALG_EXTENSION**.

      This variable is used in the ref-classes-kernel-uboot class.

   **FIT_KERNEL_COMP_ALG_EXTENSION**
      File extension corresponding to **FIT_KERNEL_COMP_ALG**. The default
      value is set ".gz" by the ref-classes-kernel-uboot class. If you
      set **FIT_KERNEL_COMP_ALG** to "lzo", you may want to set this
      variable to ".lzo".

   **FIT_KERNEL_SIGN_ENABLE**
      This variable is used by the ref-classes-kernel-fit-image class
      to enable or disable signing of the FIT image.
      The default value of **FIT_KERNEL_SIGN_ENABLE** is the value of
      **UBOOT_SIGN_ENABLE**, which means that when U-Boot FIT image signing
      is enabled, the FIT image will also be signed at build-time and U-Boot
      will verify the FIT image signature at run-time.

      If this variable is set to "1", the FIT image will be signed using the
      key specified by **FIT_KERNEL_SIGN_KEYNAME** from the directory
      **FIT_KERNEL_SIGN_KEYDIR**.

      If this variable is overridden, the **FIT_KERNEL_SIGN_KEYDIR** and
      **FIT_KERNEL_SIGN_KEYNAME** variables should also be set appropriately.

   **FIT_KERNEL_SIGN_KEYDIR**
      This variable is used by the ref-classes-kernel-fit-image class.
      The default value of **FIT_KERNEL_SIGN_KEYDIR** is the value of
      **UBOOT_SIGN_KEYDIR**, which means the kernel is signed at build-time
      with a private key found in **UBOOT_SIGN_KEYDIR** and U-Boot gets the
      public key from the same directory injected into its DTB for the
      on-target verification of the FIT image.

      If this variable is overridden, the **FIT_KERNEL_SIGN_ENABLE** and
      **FIT_KERNEL_SIGN_KEYNAME** variables should also be set appropriately.

   **FIT_KERNEL_SIGN_KEYNAME**
      This variable is used by the ref-classes-kernel-fit-image class.
      The default value of **FIT_KERNEL_SIGN_KEYNAME** is the value of
      **UBOOT_SIGN_KEYNAME**, which means the kernel is signed at
      build-time with a private key named according to
      **UBOOT_SIGN_KEYNAME** and U-Boot gets the public key with
      the same name injected into its DTB for on-target verification
      of the FIT image.

      If this variable is overridden, the **FIT_KERNEL_SIGN_ENABLE** and
      **FIT_KERNEL_SIGN_KEYDIR** variables should also be set appropriately.

   **FIT_KEY_GENRSA_ARGS**
      Arguments to `openssl genrsa` for generating a RSA private key for
      signing the FIT image. The default value is set to "-F4" by the
      ref-classes-kernel-fit-image class.

   **FIT_KEY_REQ_ARGS**
      Arguments to `openssl req` for generating a certificate for signing
      the FIT image. The default value is "-batch -new" by the
      ref-classes-kernel-fit-image class, "batch" for
      non interactive mode and "new" for generating new keys.

   **FIT_KEY_SIGN_PKCS**
      Format for the public key certificate used for signing the FIT image.
      The default value is set to "x509" by the
      ref-classes-kernel-fit-image class.

   **FIT_LOADABLE_ARCH**
      Architecture the loadables defined in **FIT_LOADABLES**; the value
      will be used for the `arch` property of the loadable.
      If no value is defined for a specific loadable, the kernel architecture
      will be used.

      This variable cannot be used directly, but only defining flags on it.

      Example::

         FIT_LOADABLES = "foo"
         FIT_LOADABLE_ARCH[foo] = "arm"

   **FIT_LOADABLE_COMPRESSION**
      Compression type for the loadables defined in **FIT_LOADABLES**; the
      value will be used for the `compression` property of the loadable.
      If no value is defined for a specific loadable, its `compression`
      property will be set to `none`.

      This variable cannot be used directly, but only defining flags on it.

      Example::

         FIT_LOADABLES = "foo"
         FIT_LOADABLE_COMPRESSION[foo] = "gzip"

      .. note::

         The binary should already be compressed, as no compression is
         performed by the ref-classes-kernel-fit-image class.

   **FIT_LOADABLE_DESCRIPTION**
      Description for the loadables defined in **FIT_LOADABLES**; the value
      will be used for the `description` property of the loadable.
      If no value is defined for a specific loadable, its description will be
      set to the loadable name followed by a space plus the string `loadable`.

      This variable cannot be used directly, but only defining flags on it.

      Example::

         FIT_LOADABLES = "foo"
         FIT_LOADABLE_DESCRIPTION[foo] = "Foo firmware binary"

   **FIT_LOADABLE_ENTRYPOINT**
      Entry point for the loadables defined in **FIT_LOADABLES**; the value
      will be used for the `entry` property of the loadable.
      If no value is defined for a specific loadable, the `entry` property
      will be omitted.

      This variable cannot be used directly, but only defining flags on it.

      Example::

         FIT_LOADABLES = "foo"
         FIT_LOADABLE_ENTRYPOINT[foo] = "0x80234000"

   **FIT_LOADABLE_FILENAME**
      Filename (or relative path) for the loadables defined in
      **FIT_LOADABLES**; this will be used to search for the binary to
      include and is therefore mandatory for each loadable. Binary files to be
      included need to be located in **DEPLOY_DIR_IMAGE**.

      This variable cannot be used directly, but only defining flags on it.

      Example::

         FIT_LOADABLES = "foo"
         FIT_LOADABLE_FILENAME[foo] = "foo-firmware.bin"

   **FIT_LOADABLE_LOADADDRESS**
      Load address for the loadables defined in **FIT_LOADABLES**; the
      value will be used for the `load` property of the loadable.
      This is mandatory for each loadable.

      This variable cannot be used directly, but only defining flags on it.

      Example::

         FIT_LOADABLES = "foo"
         FIT_LOADABLE_LOADADDRESS[foo] = "0x80230000"

   **FIT_LOADABLE_OS**
      Operating system for the loadables defined in **FIT_LOADABLES**; the
      value will be used for the `os` property of the loadable.
      If no value is defined for a specific loadable, the `os` property will
      be set to `linux`.

      This variable cannot be used directly, but only defining flags on it.

      Example::

         FIT_LOADABLES = "foo"
         FIT_LOADABLE_OS[foo] = "linux"

   **FIT_LOADABLE_TYPE**
      Type for the loadables defined in **FIT_LOADABLES**; the value will
      be used for the `type` property of the loadable.
      If no value is defined for a specific loadable, the `type` property
      will be set to `firmware`.

      This variable cannot be used directly, but only defining flags on it.

      Example::

         FIT_LOADABLES = "foo"
         FIT_LOADABLE_TYPE[foo] = "firmware"

   **FIT_LOADABLES**
      Space-separated list of loadables to add to a FIT image in addition to
      regular ones (kernel, initramfs, dtsb etc.).
      Values specified here will be used as node names inside the FIT image;
      all of them will be included in all configurations by using the
      `loadables` property.

      For each loadable specified in this variable, additional parameters can be
      defined using **FIT_LOADABLE_ARCH**, **FIT_LOADABLE_COMPRESSION**,
      **FIT_LOADABLE_DESCRIPTION**, **FIT_LOADABLE_ENTRYPOINT**,
      **FIT_LOADABLE_FILENAME**, **FIT_LOADABLE_LOADADDRESS**,
      **FIT_LOADABLE_OS** and **FIT_LOADABLE_TYPE**.

      This variable is used by the ref-classes-kernel-fit-image class and
      is empty by default.

      For example, the following configuration adds as loadables a TF-A BL31
      firmware and a (compressed) TEE firmware, to be loaded respectively at
      0x204E0000 and 0x96000000::

         FIT_LOADABLES = "atf tee"

         FIT_LOADABLE_FILENAME[atf] = "bl31.bin"
         FIT_LOADABLE_DESCRIPTION[atf] = "TF-A Firmware"
         FIT_LOADABLE_TYPE[atf] = "tfa-bl31"
         FIT_LOADABLE_ARCH[atf] = "arm64"
         FIT_LOADABLE_OS[atf] = "arm-trusted-firmware"
         FIT_LOADABLE_LOADADDRESS[atf] = "0x204E0000"
         FIT_LOADABLE_ENTRYPOINT[atf] = "0x204E0000"

         FIT_LOADABLE_FILENAME[tee] = "tee.bin.gz"
         FIT_LOADABLE_COMPRESSION[tee] = "gzip"
         FIT_LOADABLE_TYPE[tee] = "tee"
         FIT_LOADABLE_OS[tee] = "tee"
         FIT_LOADABLE_LOADADDRESS[tee] = "0x96000000"

      and will be converted to the following FIT source::

         images {
                 atf {
                         description = "TF-A Firmware";
                         type = "tfa-bl31";
                         compression = "none";
                         data = /incbin/("<DEPLOY_DIR_IMAGE>/bl31.bin");
                         arch = "arm64";
                         os = "arm-trusted-firmware";
                         load = <0x204E0000>;
                         entry = <0x204E0000>;
                 };
                 tee {
                         description = "tee loadable";
                         type = "tee";
                         compression = "gzip";
                         data = /incbin/("<DEPLOY_DIR_IMAGE>/tee.bin.gz");
                         arch = "arm64";
                         os = "tee";
                         load = <0x96000000>;
                 };
         };

         configurations {
                 default = "<my-board>.dtb";
                 <my-board>.dtb {
                         description = "1 Linux kernel, FDT blob, loadables";
                         kernel = "kernel-1";
                         fdt = "<my-board>.dtb";
                         loadables = "atf", "tee";
                 };
         };

   **FIT_MKIMAGE_EXTRA_OPTS**
      When inheriting the ref-classes-kernel-fit-image, the
      **FIT_MKIMAGE_EXTRA_OPTS** variable allows passing extra options to
      `mkimage` during FIT image generation, providing flexibility for platforms
      that require additional `mkimage` arguments beyond the defaults.

      For example::

         FIT_MKIMAGE_EXTRA_OPTS = "-B 8 -E"

      This results in the `mkimage` command being invoked as:

      .. parsed-literal::

         mkimage *-B 8 -E* -f fit-image.its fitImage

   **FIT_PAD_ALG**
      Specifies the padding algorithm used in creating the FIT Image.
      The default value is set to "pkcs-1.5" by the
      ref-classes-kernel-fit-image class.

   **FIT_SIGN_ALG**
      Specifies the signature algorithm used in creating the FIT Image.
      This variable is set by default to "rsa2048" by the
      ref-classes-kernel-fit-image class.

   **FIT_SIGN_INDIVIDUAL**
      If set to "1", the ref-classes-kernel-fit-image class signs each
      image node individually, including the kernel, DTB, RAM disk, and any
      other image types present in the FIT image, in addition to signing the
      configuration nodes.
      This can be useful if you need to verify signatures outside of the
      U-Boot boot process. By default, this variable is set to "0".

      If **UBOOT_SIGN_ENABLE** is set to "1" and
      **FIT_SIGN_INDIVIDUAL** remains at its default value of "0", only the
      configuration nodes are signed. Since configuration nodes include hashes
      of their referenced image nodes, the integrity of the entire FIT image is
      ensured as long as the image nodes are loaded via the configuration nodes
      and the hashes of the image nodes are checked. That's usually the case.

      Enabling **FIT_SIGN_INDIVIDUAL** typically increases complexity for
      little benefit. There might be exceptions such as image nodes that are
      not referenced by any configuration node or loaded directly for whatever
      reason.
      For most use cases, setting this variable to "0" provides sufficient
      security.

      For further details, refer to the official U-Boot documentation:
      `U-Boot fit signature <https://docs.u-boot.org/en/latest/usage/fit/signature.html>`__
      and more specifically at:
      `U-Boot signed configurations <https://docs.u-boot.org/en/latest/usage/fit/signature.html#signed-configurations>`__.

      Signing only the image nodes is intentionally not implemented by
      **OpenEmbedded-Core (OE-Core)**, as it is vulnerable to mix-and-match
      attacks.

   **FIT_SIGN_NUMBITS**
      Size of the private key used in the FIT image, in number of bits.
      The default value for this variable is set to "2048"
      by the ref-classes-kernel-fit-image class.

   **FIT_UBOOT_ENV**
      This variable allows to add a U-Boot script as a text file to the
      FIT image. Such a script can be sourced from the U-Boot shell.

      When inheriting the ref-classes-kernel-fit-image class a
      script file should be included in the **SRC_URI** of the Linux
      kernel recipe.

      Example:

      -  Add a script `boot.cmd` to the Linux kernel recipe::

            FIT_UBOOT_ENV = "boot.cmd"
            SRC_URI += "file://${FIT_UBOOT_ENV}"

      -  Use the script file from the U-Boot shell. The name of the script in
         FIT image is `bootscr-${FIT_UBOOT_ENV}`. This example loads the FIT
         image from a TFTP server::

            tftp $loadaddr $fit_tftp_path
            source $loadaddr#bootscr-boot.cmd

      More information can be found in the official U-Boot documentation:
      `U-Boot source command <https://docs.u-boot.org/en/latest/usage/cmd/source.html#fit-image.f>`__

   **FONT_EXTRA_RDEPENDS**
      When inheriting the ref-classes-fontcache class,
      this variable specifies the runtime dependencies for font packages.
      By default, the **FONT_EXTRA_RDEPENDS** is set to "fontconfig-utils".

   **FONT_PACKAGES**
      When inheriting the ref-classes-fontcache class, this variable
      identifies packages containing font files that need to be cached by
      Fontconfig. By default, the ref-classes-fontcache class assumes
      that fonts are in the recipe's main package (i.e.
      `${`\ **PN**\ `}`). Use this variable if fonts you
      need are in a package other than that main package.

   **FORCE_RO_REMOVE**
      Forces the removal of the packages listed in `ROOTFS_RO_UNNEEDED`
      during the generation of the root filesystem.

      Set the variable to "1" to force the removal of these packages.

   **FULL_OPTIMIZATION**
      The options to pass in **TARGET_CFLAGS** and **CFLAGS** when
      compiling an optimized system. This variable defaults to "-O2 -pipe
      ${DEBUG_FLAGS}".

   **GCCPIE**
      Enables Position Independent Executables (PIE) within the GNU C
      Compiler (GCC). Enabling PIE in the GCC makes Return Oriented
      Programming (ROP) attacks much more difficult to execute.

      By default the `security_flags.inc` file enables PIE by setting the
      variable as follows::

         GCCPIE ?= "--enable-default-pie"

   **GCCVERSION**
      Specifies the default version of the GNU C Compiler (GCC) used for
      compilation. By default, **GCCVERSION** is set to "8.x" in the
      `meta/conf/distro/include/tcmode-default.inc` include file::

         GCCVERSION ?= "8.%"

      You can override this value by setting it in a
      configuration file such as the `local.conf`.

   **GDB**
      The minimal command and arguments to run the GNU Debugger.

   **GIR_EXTRA_LIBS_PATH**
      Allows to specify an extra search path for `.so` files
      in GLib related recipes using GObject introspection,
      and which do not compile without this setting.
      See the "dev-manual/gobject-introspection:enabling gobject introspection support"
      section for details.

   **GITDIR**
      The directory in which a local copy of a Git repository is stored
      when it is cloned.

   **GITHUB_BASE_URI**
      When inheriting the ref-classes-github-releases
      class, specifies the base URL for fetching releases for the github
      project you wish to fetch sources from. The default value is as follows::

         GITHUB_BASE_URI ?= "https://github.com/${BPN}/${BPN}/releases/"

   **GLIBC_GENERATE_LOCALES**
      Specifies the list of GLIBC locales to generate should you not wish
      to generate all LIBC locals, which can be time consuming.

      .. note::

         If you specifically remove the locale `en_US.UTF-8`, you must set
         **IMAGE_LINGUAS** appropriately.

      You can set **GLIBC_GENERATE_LOCALES** in your `local.conf` file.
      By default, all locales are generated::

         GLIBC_GENERATE_LOCALES = "en_GB.UTF-8 en_US.UTF-8"

   **GO_IMPORT**
      When inheriting the ref-classes-go class, this mandatory variable
      sets the import path for the Go package that will be created for the code
      to build. If you have a `go.mod` file in the source directory, this
      typically matches the path in the `module` line in this file.

      Other Go programs importing this package will use this path.

      Here is an example setting from the
      go-helloworld_0.1.bb 
      recipe::

          GO_IMPORT = "golang.org/x/example"

   **GO_INSTALL**
      When inheriting the ref-classes-go class, this optional variable
      specifies which packages in the sources should be compiled and
      installed in the Go build space by the
      `go install <https://go.dev/ref/mod#go-install>`__ command.

      Here is an example setting from the
      crucible 
      recipe::

         GO_INSTALL = "\
             ${GO_IMPORT}/cmd/crucible \
             ${GO_IMPORT}/cmd/habtool \
         "

      By default, **GO_INSTALL** is defined as::

         GO_INSTALL ?= "${GO_IMPORT}/..."

      The `...` wildcard means that it will catch all
      packages found in the sources.

      See the **GO_INSTALL_FILTEROUT** variable for
      filtering out unwanted packages from the ones
      found from the **GO_INSTALL** value.

   **GO_INSTALL_FILTEROUT**
      When using the Go "vendor" mechanism to bring in dependencies for a Go
      package, the default **GO_INSTALL** setting, which uses the `...`
      wildcard, will include the vendored packages in the build, which produces
      incorrect results.

      There are also some Go packages that are structured poorly, so that the
      `...` wildcard results in building example or test code that should not
      be included in the build, or could fail to build.

      This optional variable allows for filtering out a subset of the sources.
      It defaults to excluding everything under the `vendor` subdirectory
      under package's main directory. This is the normal location for vendored
      packages, but it can be overridden by a recipe to filter out other
      subdirectories if needed.

   **GO_WORKDIR**
      When using Go Modules, the current working directory must be the directory
      containing the `go.mod` file, or one of its subdirectories. When the
      `go` tool is used, it will automatically look for the `go.mod` file
      in the Go working directory or in any parent directory, but not in
      subdirectories.

      When using the ref-classes-go-mod class to use Go modules,
      the optional **GO_WORKDIR** variable, defaulting to the value
      of **GO_IMPORT**, allows to specify a different Go working directory.

   **GROUPADD_PARAM**
      When inheriting the ref-classes-useradd class,
      this variable specifies for a package what parameters should be
      passed to the `groupadd` command if you wish to add a group to the
      system when the package is installed.

      Here is an example from the `dbus` recipe::

         GROUPADD_PARAM:${PN} = "-r netdev"

      More than one group can be added by separating each set of different
      groups' parameters with a semicolon.

      Here is an example adding multiple groups from the `useradd-example.bb`
      file in the `meta-skeleton` layer::

         GROUPADD_PARAM:${PN} = "-g 880 group1; -g 890 group2"

      For information on the standard Linux shell command
      `groupadd`, see https://linux.die.net/man/8/groupadd.

   **GROUPMEMS_PARAM**
      When inheriting the ref-classes-useradd class,
      this variable specifies for a package what parameters should be
      passed to the `groupmems` command if you wish to modify the members
      of a group when the package is installed.

      For information on the standard Linux shell command `groupmems`,
      see https://linux.die.net/man/8/groupmems.

   **GRUB_GFXSERIAL**
      Configures the GNU GRand Unified Bootloader (GRUB) to have graphics
      and serial in the boot menu. Set this variable to "1" in your
      `local.conf` or distribution configuration file to enable graphics
      and serial in the menu.

      See the ref-classes-grub-efi class for more
      information on how this variable is used.

   **GRUB_MKIMAGE_OPTS**
      This variable controls additional options passed to the `grub-mkimage`
      command in the GNU GRand Unified Bootloader (GRUB) recipe during the
      `do_mkimage` task.

   **GRUB_OPTS**
      Additional options to add to the GNU GRand Unified Bootloader (GRUB)
      configuration. Use a semi-colon character (`;`) to separate
      multiple options.

      The **GRUB_OPTS** variable is optional. See the
      ref-classes-grub-efi class for more information
      on how this variable is used.

   **GRUB_TIMEOUT**
      Specifies the timeout before executing the default `LABEL` in the
      GNU GRand Unified Bootloader (GRUB).

      The **GRUB_TIMEOUT** variable is optional. See the
      ref-classes-grub-efi class for more information
      on how this variable is used.

   **GRUB_TITLE**
      Specifies custom titles for GRUB labels defined in  **LABELS**. See
      the ref-classes-grub-efi class for more information on how this
      variable is used.

   **GTKIMMODULES_PACKAGES**
      When inheriting the ref-classes-gtk-immodules-cache class,
      this variable specifies the packages that contain the GTK+ input
      method modules being installed when the modules are in packages other
      than the main package.

   **HGDIR**
      See **bitbake:HGDIR** in the BitBake manual.

   **HOMEPAGE**
      Website where more information about the software the recipe is
      building can be found.

   **HOST_ARCH**
      The name of the target architecture, which is normally the same as
      **TARGET_ARCH**. The OpenEmbedded build system
      supports many architectures. Here is an example list of architectures
      supported. This list is by no means complete as the architecture is
      configurable:

      - arm
      - i586
      - x86_64
      - powerpc
      - powerpc64
      - mips
      - mipsel

   **HOST_AS_ARCH**
      Specifies architecture-specific assembler flags.

      Default initialization for **HOST_AS_ARCH** varies depending on what
      is being built:

      -  **TARGET_AS_ARCH** when building for the
         target

      -  **BUILD_AS_ARCH** when building for the build host (i.e.
         `-native`)

      -  **SDK_AS_ARCH** when building for an SDK (i.e. `nativesdk-`)

   **HOST_CC_ARCH**
      Specifies architecture-specific compiler flags that are passed to the
      C compiler.

      Default initialization for **HOST_CC_ARCH** varies depending on what
      is being built:

      -  **TARGET_CC_ARCH** when building for the
         target

      -  **BUILD_CC_ARCH** when building for the build host (i.e.
         `-native`)

      -  **SDK_CC_ARCH** when building for an SDK (i.e. `nativesdk-`)

   **HOST_LD_ARCH**
      Specifies architecture-specific linker flags.

      Default initialization for **HOST_LD_ARCH** varies depending on what
      is being built:

      -  **TARGET_LD_ARCH** when building for the target

      -  **BUILD_LD_ARCH** when building for the build host (i.e.
         `-native`)

      -  **SDK_LD_ARCH** when building for an SDK (i.e. `nativesdk-`)

   **HOST_OS**
      Specifies the name of the target operating system, which is normally
      the same as the **TARGET_OS**. The variable can
      be set to "linux" for `glibc`-based systems and to "linux-musl" for
      `musl`. For ARM/EABI targets, there are also "linux-gnueabi" and
      "linux-musleabi" values possible.

   **HOST_PREFIX**
      Specifies the prefix for the cross-compile toolchain. **HOST_PREFIX**
      is normally the same as **TARGET_PREFIX**.

   **HOST_SYS**
      Specifies the system, including the architecture and the operating
      system, for which the build is occurring in the context of the
      current recipe.

      The OpenEmbedded build system automatically sets this variable based
      on **HOST_ARCH**,
      **HOST_VENDOR**, and
      **HOST_OS** variables.

      .. note::

         You do not need to set the variable yourself.

      Consider these two examples:

      -  Given a native recipe on a 32-bit x86 machine running Linux, the
         value is "i686-linux".

      -  Given a recipe being built for a little-endian MIPS target running
         Linux, the value might be "mipsel-linux".

   **HOST_VENDOR**
      Specifies the name of the vendor. **HOST_VENDOR** is normally the
      same as **TARGET_VENDOR**.

   **HOSTTOOLS**
      A space-separated list (filter) of tools on the build host that
      should be allowed to be called from within build tasks. Using this
      filter helps reduce the possibility of host contamination. If a tool
      specified in the value of **HOSTTOOLS** is not found on the build
      host, the OpenEmbedded build system produces an error and the build
      is not started.

      For additional information, see
      **HOSTTOOLS_NONFATAL**.

   **HOSTTOOLS_NONFATAL**
      A space-separated list (filter) of tools on the build host that
      should be allowed to be called from within build tasks. Using this
      filter helps reduce the possibility of host contamination. Unlike
      **HOSTTOOLS**, the OpenEmbedded build system
      does not produce an error if a tool specified in the value of
      **HOSTTOOLS_NONFATAL** is not found on the build host. Thus, you can
      use **HOSTTOOLS_NONFATAL** to filter optional host tools.

   **IMAGE_BASENAME**
      The base name of image output files. This variable defaults to the
      recipe name (`${`\ **PN**\ `}`).

   **IMAGE_BOOT_FILES**
      A space-separated list of files installed into the boot partition
      when preparing an image using the Wic tool with the
      `bootimg_partition` source plugin. By default,
      the files are
      installed under the same name as the source files. To change the
      installed name, separate it from the original name with a semi-colon
      (;). Source files need to be located in
      **DEPLOY_DIR_IMAGE**. Here are two
      examples::

         IMAGE_BOOT_FILES = "u-boot.img uImage;kernel"
         IMAGE_BOOT_FILES = "u-boot.${UBOOT_SUFFIX} ${KERNEL_IMAGETYPE}"

      Alternatively, source files can be picked up using a glob pattern. In
      this case, the destination file must have the same name as the base
      name of the source file path. To install files into a directory
      within the target location, pass its name after a semi-colon (;).
      Here are two examples::

         IMAGE_BOOT_FILES = "bcm2835-bootfiles/*"
         IMAGE_BOOT_FILES = "bcm2835-bootfiles/*;boot/"

      The first example
      installs all files from `${DEPLOY_DIR_IMAGE}/bcm2835-bootfiles`
      into the root of the target partition. The second example installs
      the same files into a `boot` directory within the target partition.

      You can find information on how to use the Wic tool in the
      "dev-manual/wic:creating partitioned images using wic"
      section of the Yocto Project Development Tasks Manual. Reference
      material for Wic is located in the
      "/ref-manual/kickstart" chapter.

   **IMAGE_BUILDINFO_FILE**
      When using the ref-classes-image-buildinfo class,
      specifies the file in the image to write the build information into. The
      default value is "`${sysconfdir}/buildinfo`".

   **IMAGE_BUILDINFO_VARS**
      When using the ref-classes-image-buildinfo class,
      specifies the list of variables to include in the `Build Configuration`
      section of the output file (as a space-separated list). Defaults to
      "**DISTRO** **DISTRO_VERSION**".

   **IMAGE_CLASSES**
      A list of classes that all images should inherit. This is typically used
      to enable functionality across all image recipes.

      Classes specified in **IMAGE_CLASSES** must be located in the
      `classes-recipe/` or `classes/` subdirectories.

   **IMAGE_CMD**
      Specifies the command to create the image file for a specific image
      type, which corresponds to the value set in
      **IMAGE_FSTYPES**, (e.g. `ext3`,
      `btrfs`, and so forth). When setting this variable, you should use
      an override for the associated type. Here is an example::

         IMAGE_CMD:jffs2 = "mkfs.jffs2 --root=${IMAGE_ROOTFS} --faketime \
             --output=${IMGDEPLOYDIR}/${IMAGE_NAME}${IMAGE_NAME_SUFFIX}.jffs2 \
             ${EXTRA_IMAGECMD}"

      You typically do not need to set this variable unless you are adding
      support for a new image type. For more examples on how to set this
      variable, see the ref-classes-image_types
      class file, which is `meta/classes-recipe/image_types.bbclass`.

   **IMAGE_CONTAINER_NO_DUMMY**
      When an image recipe has the `container` image type in
      **IMAGE_FSTYPES**, it expects the **PREFERRED_PROVIDER** for
      the Linux kernel (`virtual/kernel`) to be set to `linux-dummy` from a
      **configuration file**. Otherwise, an error is triggered.

      When set to "1", the **IMAGE_CONTAINER_NO_DUMMY** variable allows the
      **PREFERRED_PROVIDER** variable to be set to another value, thus
      skipping the check and not triggering the build error. Any other value
      will keep the check.

      This variable should be set from the image recipe using the `container`
      image type.

      See the documentation of the ref-classes-image-container class for
      more information on why setting the **PREFERRED_PROVIDER** to
      `linux-dummy` is advised with this class.

   **IMAGE_DEVICE_TABLES**
      Specifies one or more files that contain custom device tables that
      are passed to the `makedevs` command as part of creating an image.
      These files list basic device nodes that should be created under
      `/dev` within the image. If **IMAGE_DEVICE_TABLES** is not set,
      `files/device_table-minimal.txt` is used, which is located by
      **BBPATH**. For details on how you should write
      device table files, see `meta/files/device_table-minimal.txt` as an
      example.

   **IMAGE_EFI_BOOT_FILES**
      A space-separated list of files installed into the boot partition
      when preparing an image using the Wic tool with the
      `bootimg_efi` source plugin. By default,
      the files are
      installed under the same name as the source files. To change the
      installed name, separate it from the original name with a semi-colon
      (;). Source files need to be located in
      **DEPLOY_DIR_IMAGE**. Here are two
      examples::

         IMAGE_EFI_BOOT_FILES = "${KERNEL_IMAGETYPE};bz2"
         IMAGE_EFI_BOOT_FILES = "${KERNEL_IMAGETYPE} microcode.cpio"

      Alternatively, source files can be picked up using a glob pattern. In
      this case, the destination file must have the same name as the base
      name of the source file path. To install files into a directory
      within the target location, pass its name after a semi-colon (;).
      Here are two examples::

         IMAGE_EFI_BOOT_FILES = "boot/loader/*"
         IMAGE_EFI_BOOT_FILES = "boot/loader/*;boot/"

      The first example
      installs all files from `${DEPLOY_DIR_IMAGE}/boot/loader/`
      into the root of the target partition. The second example installs
      the same files into a `boot` directory within the target partition.

      You can find information on how to use the Wic tool in the
      "dev-manual/wic:creating partitioned images using wic"
      section of the Yocto Project Development Tasks Manual. Reference
      material for Wic is located in the
      "/ref-manual/kickstart" chapter.

   **IMAGE_EXTRA_PARTITION_FILES**
      A space-separated list of files installed into the extra partition(s)
      when preparing an image using the Wic tool with the
      `extra_partition` source plugin. By default,
      the files are
      installed under the same name as the source files. To change the
      installed name, separate it from the original name with a semi-colon
      (;). Source files need to be located in
      **DEPLOY_DIR_IMAGE**. Here is an
      example::

         IMAGE_EXTRA_PARTITION_FILES = "foobar file.conf;config"

      In the above example, the file `foobar` is installed with its original name
      `foobar`, while the file `file.conf` is installed and renamed to `config`.

      Alternatively, source files can be picked up using a glob pattern.
      However, hidden files are ignored, and the pattern is non-recursive
      (subdirectories are ignored).
      The destination file will have the same name as the base
      name of the source file path. To install files into a renamed directory
      within the target location, pass its name after a semi-colon (;).
      Here are two examples::

         IMAGE_EXTRA_PARTITION_FILES = "foo/*"
         IMAGE_EXTRA_PARTITION_FILES = "foo/*;bar/"

      The first line in this example
      installs all files from `foo` directory
      into the root of the target partition. The second line in this example installs
      the same files into a `bar` directory within the target partition.
      The `bar/` directory is automatically created if it does not exist.

      You can also specify the target by label, UUID or partition name if multiple
      extra partitions coexist. Let's take the following example. This would be
      the WKS file for the image currently being built::

         part --source extra_partition --fstype=ext4 --label foo
         part --source extra_partition --fstype=ext4 --uuid e7d0824e-cda3-4bed-9f54-9ef5312d105d
         part --source extra_partition --fstype=ext4 --part-name config

      And the following configuration::

         IMAGE_EXTRA_PARTITION_FILES_label-foo = "foo/*"
         IMAGE_EXTRA_PARTITION_FILES_uuid-e7d0824e-cda3-4bed-9f54-9ef5312d105d = "foo/*;bar/"
         IMAGE_EXTRA_PARTITION_FILES_part-name-config = "config"

      Then:

      -  The partition labeled "foo" would get all files from the `foo`
         directory.

      -  The partition whose UUID is "e7d0824e-cda3-4bed-9f54-9ef5312d105d"
         would get all files from the `foo` directory, installed into a
         `bar` directory.

      -  The partition named "config" would get the file `config`.

      You can find information on how to use the Wic tool in the
      "dev-manual/wic:creating partitioned images using wic"
      section of the Yocto Project Development Tasks Manual. Reference
      material for Wic is located in the
      "/ref-manual/kickstart" chapter.

   **IMAGE_FEATURES**
      The primary list of features to include in an image. Typically, you
      configure this variable in an image recipe. Although you can use this
      variable from your `local.conf` file, which is found in the
      **Build Directory**, best practices dictate that you do
      not.

      .. note::

         To enable extra features from outside the image recipe, use the
         **EXTRA_IMAGE_FEATURES** variable.

      For a list of image features that ships with the Yocto Project, see
      the "ref-features-image" section.

      For an example that shows how to customize your image by using this
      variable, see the "dev-manual/customizing-images:customizing images using custom \\`image_features\`\` and \`\`extra_image_features\`\``"
      section in the Yocto Project Development Tasks Manual.

   **IMAGE_FSTYPES**
      Specifies the formats the OpenEmbedded build system uses during the
      build when creating the root filesystem. For example, setting
      **IMAGE_FSTYPES** as follows causes the build system to create root
      filesystems using two formats: `.ext3` and `.tar.bz2`::

         IMAGE_FSTYPES = "ext3 tar.bz2"

      For the complete list of supported image formats from which you can
      choose, see **IMAGE_TYPES**.

      .. note::

         -  If an image recipe uses the "inherit image" line and you are
            setting **IMAGE_FSTYPES** inside the recipe, you must set
            **IMAGE_FSTYPES** prior to using the "inherit image" line.

         -  Due to the way the OpenEmbedded build system processes this
            variable, you cannot update its contents by using `:append`
            or `:prepend`. You must use the `+=` operator to add one or
            more options to the **IMAGE_FSTYPES** variable.

   **IMAGE_INSTALL**
      Used by recipes to specify the packages to install into an image
      through the ref-classes-image class. Use the
      **IMAGE_INSTALL** variable with care to avoid ordering issues.

      Image recipes set **IMAGE_INSTALL** to specify the packages to
      install into an image through ref-classes-image. Additionally,
      there are "helper" classes such as the ref-classes-core-image
      class which can take lists used with **IMAGE_FEATURES** and turn
      them into auto-generated entries in **IMAGE_INSTALL** in addition
      to its default contents.

      When you use this variable, it is best to use it as follows::

         IMAGE_INSTALL:append = " package-name"

      Be sure to include the space
      between the quotation character and the start of the package name or
      names.

      .. note::

         -  When working with a
            core-image-minimal-initramfs 
            image, do not use the **IMAGE_INSTALL** variable to specify
            packages for installation. Instead, use the
            **PACKAGE_INSTALL** variable, which
            allows the initial RAM filesystem (**Initramfs**) recipe to use a
            fixed set of packages and not be affected by **IMAGE_INSTALL**.
            For information on creating an **Initramfs**, see the
            "dev-manual/building:building an initial ram filesystem (Initramfs) image"
            section in the Yocto Project Development Tasks Manual.

         -  Using **IMAGE_INSTALL** with the
            += 
            BitBake operator within the `/conf/local.conf` file or from
            within an image recipe is not recommended. Use of this operator in
            these ways can cause ordering issues. Since
            ref-classes-core-image sets **IMAGE_INSTALL** to a
            default value using the
            ?= 
            operator, using a `+=` operation against **IMAGE_INSTALL**
            results in unexpected behavior when used within
            `conf/local.conf`. Furthermore, the same operation from within an
            image recipe may or may not succeed depending on the specific
            situation. In both these cases, the behavior is contrary to how
            most users expect the `+=` operator to work.

   **IMAGE_LINGUAS**
      Specifies the list of locales to install into the image during the
      root filesystem construction process. The OpenEmbedded build system
      automatically splits locale files, which are used for localization,
      into separate packages. Setting the **IMAGE_LINGUAS** variable
      ensures that any locale packages that correspond to packages already
      selected for installation into the image are also installed. Here is
      an example::

         IMAGE_LINGUAS = "pt-br de-de"

      In this example, the build system ensures any Brazilian Portuguese
      and German locale files that correspond to packages in the image are
      installed (i.e. `*-locale-pt-br` and `*-locale-de-de` as well as
      `*-locale-pt` and `*-locale-de`, since some software packages
      only provide locale files by language and not by country-specific
      language).

      See the **GLIBC_GENERATE_LOCALES**
      variable for information on generating GLIBC locales.

   **IMAGE_LINK_NAME**
      The name of the output image symlink (which does not include
      the version part as **IMAGE_NAME** does). The default value
      is derived using the **IMAGE_BASENAME** and
      **IMAGE_MACHINE_SUFFIX** variables::

         IMAGE_LINK_NAME ?= "${IMAGE_BASENAME}${IMAGE_MACHINE_SUFFIX}"

      .. note::

         It is possible to set this to "" to disable symlink creation,
         however, you also need to set **IMAGE_NAME** to still have
         a reasonable value e.g.::

            IMAGE_LINK_NAME = ""
            IMAGE_NAME = "${IMAGE_BASENAME}${IMAGE_MACHINE_SUFFIX}${IMAGE_VERSION_SUFFIX}"

   **IMAGE_MACHINE_SUFFIX**
      Specifies the by default machine-specific suffix for image file names
      (before the extension). The default value is set as follows::

         IMAGE_MACHINE_SUFFIX ??= "-${MACHINE}"

      The default **DEPLOY_DIR_IMAGE** already has a **MACHINE**
      subdirectory, so you may find it unnecessary to also include this suffix
      in the name of every image file. If you prefer to remove the suffix you
      can set this variable to an empty string::

         IMAGE_MACHINE_SUFFIX = ""

      (Not to be confused with **IMAGE_NAME_SUFFIX**.)

   **IMAGE_MANIFEST**
      The manifest file for the image. This file lists all the installed
      packages that make up the image. The file contains package
      information on a line-per-package basis as follows::

          packagename packagearch version

      The rootfs-postcommands  class defines the manifest
      file as follows::

         IMAGE_MANIFEST = "${IMGDEPLOYDIR}/${IMAGE_NAME}${IMAGE_NAME_SUFFIX}.manifest"

      The location is
      derived using the **IMGDEPLOYDIR**
      and **IMAGE_NAME** variables. You can find
      information on how the image is created in the "overview-manual/concepts:image generation"
      section in the Yocto Project Overview and Concepts Manual.

   **IMAGE_NAME**
      The name of the output image files minus the extension. By default
      this variable is set using the **IMAGE_LINK_NAME**, and
      **IMAGE_VERSION_SUFFIX** variables::

         IMAGE_NAME ?= "${IMAGE_LINK_NAME}${IMAGE_VERSION_SUFFIX}"

   **IMAGE_NAME_SUFFIX**
      Suffix used for the image output filename --- defaults to `".rootfs"`
      to distinguish the image file from other files created during image
      building; however if this suffix is redundant or not desired you can
      clear the value of this variable (set the value to ""). For example,
      this is typically cleared in **Initramfs** image recipes.

   **IMAGE_OUTPUT_MANIFEST**
      When inheriting the ref-classes-image class directly or through the
      ref-classes-core-image class, the **IMAGE_OUTPUT_MANIFEST**
      points to a manifest `json` file that lists what images were created by
      various image creation tasks (as defined by the **IMAGE_FSTYPES**
      variable). It is set in the ref-classes-image class as follows::

          IMAGE_OUTPUT_MANIFEST = "${IMAGE_OUTPUT_MANIFEST_DIR}/manifest.json"

   **IMAGE_OUTPUT_MANIFEST_DIR**
      When inheriting the ref-classes-image class directly or through the
      ref-classes-core-image class, the **IMAGE_OUTPUT_MANIFEST_DIR** points to
      a directory that stores a manifest `json` file that lists what
      images were created by various image creation tasks (as defined by the
      **IMAGE_FSTYPES** variable). It is set in the ref-classes-image
      class as follows::

          IMAGE_OUTPUT_MANIFEST_DIR = "${WORKDIR}/deploy-image-output-manifest"

   **IMAGE_OVERHEAD_FACTOR**
      Defines a multiplier that the build system applies to the initial
      image size for cases when the multiplier times the returned disk
      usage value for the image is greater than the sum of
      **IMAGE_ROOTFS_SIZE** and **IMAGE_ROOTFS_EXTRA_SPACE**. The result of
      the multiplier applied to the initial image size creates free disk
      space in the image as overhead. By default, the build process uses a
      multiplier of 1.3 for this variable. This default value results in
      30% free disk space added to the image when this method is used to
      determine the final generated image size. You should be aware that
      post install scripts and the package management system uses disk
      space inside this overhead area. Consequently, the multiplier does
      not produce an image with all the theoretical free disk space. See
      **IMAGE_ROOTFS_SIZE** for information on how the build system
      determines the overall image size.

      The default 30% free disk space typically gives the image enough room
      to boot and allows for basic post installs while still leaving a
      small amount of free disk space. If 30% free space is inadequate, you
      can increase the default value. For example, the following setting
      gives you 50% free space added to the image::

         IMAGE_OVERHEAD_FACTOR = "1.5"

      Alternatively, you can ensure a specific amount of free disk space is
      added to the image by using the **IMAGE_ROOTFS_EXTRA_SPACE**
      variable.

      When using Wic tool, beware that a second overhead factor is also applied.
      This overhead value is defined by the `--overhead-factor` option, which
      defaults to "1.3" when omitted. See the
      ref-manual/kickstart:command: part or partition chapter in
      /ref-manual/kickstart for details.

   **IMAGE_PKGTYPE**
      Defines the package type (i.e. DEB, RPM or IPK) used by the
      OpenEmbedded build system. The variable is defined appropriately by
      one of the ref-classes-package_deb, ref-classes-package_rpm,
      or ref-classes-package_ipk classes.

      The ref-classes-populate-sdk-* and ref-classes-image
      classes use the **IMAGE_PKGTYPE** for packaging images and SDKs.

      You should not set the **IMAGE_PKGTYPE** manually. Rather, the
      variable is set indirectly through the appropriate
      package_*  class using the
      **PACKAGE_CLASSES** variable. The
      OpenEmbedded build system uses the first package type (e.g. DEB, RPM,
      or IPK) that appears with the variable

   **IMAGE_POSTPROCESS_COMMAND**
      Specifies a list of functions to call once the OpenEmbedded build
      system creates the final image output files. You can specify
      functions separated by spaces::

         IMAGE_POSTPROCESS_COMMAND += "function"

      If you need to pass the root filesystem path to a command within the
      function, you can use `${IMAGE_ROOTFS}`, which points to the
      directory that becomes the root filesystem image. See the
      **IMAGE_ROOTFS** variable for more
      information.

   **IMAGE_PREPROCESS_COMMAND**
      Specifies a list of functions to call before the OpenEmbedded build
      system creates the final image output files. You can specify
      functions separated by spaces::

         IMAGE_PREPROCESS_COMMAND += "function"

      If you need to pass the root filesystem path to a command within the
      function, you can use `${IMAGE_ROOTFS}`, which points to the
      directory that becomes the root filesystem image. See the
      **IMAGE_ROOTFS** variable for more
      information.

   **IMAGE_ROOTFS**
      The location of the root filesystem while it is under construction
      (i.e. during the ref-tasks-rootfs task). This
      variable is not configurable. Do not change it.

   **IMAGE_ROOTFS_ALIGNMENT**
      Specifies the alignment for the output image file in Kbytes. If the
      size of the image is not a multiple of this value, then the size is
      rounded up to the nearest multiple of the value. The default value is
      "1". See **IMAGE_ROOTFS_SIZE** for
      additional information.

   **IMAGE_ROOTFS_EXTRA_SPACE**
      Defines additional free disk space created in the image in Kbytes. By
      default, this variable is set to "0". This free disk space is added
      to the image after the build system determines the image size as
      described in **IMAGE_ROOTFS_SIZE**.

      This variable is particularly useful when you want to ensure that a
      specific amount of free disk space is available on a device after an
      image is installed and running. For example, to be sure 5 Gbytes of
      free disk space is available, set the variable as follows::

         IMAGE_ROOTFS_EXTRA_SPACE = "5242880"

      For example, the Yocto Project Build Appliance specifically requests
      40 Gbytes of extra space with the line::

         IMAGE_ROOTFS_EXTRA_SPACE = "41943040"

   **IMAGE_ROOTFS_MAXSIZE**
      Defines the maximum allowed size of the generated image in kilobytes.
      The build will fail if the generated image size exceeds this value.

      The generated image size undergoes several calculation steps before being
      compared to **IMAGE_ROOTFS_MAXSIZE**.
      In the first step, the size of the directory pointed to by **IMAGE_ROOTFS**
      is calculated.
      In the second step, the result from the first step is multiplied
      by **IMAGE_OVERHEAD_FACTOR**.
      In the third step, the result from the second step is compared with
      **IMAGE_ROOTFS_SIZE**. The larger value of these is added to
      **IMAGE_ROOTFS_EXTRA_SPACE**.
      In the fourth step, the result from the third step is checked for
      a decimal part. If it has one, it is rounded up to the next integer.
      If it does not, it is simply converted into an integer.
      In the fifth step, the **IMAGE_ROOTFS_ALIGNMENT** is added to the result
      from the fourth step and "1" is subtracted.
      In the sixth step, the remainder of the division between the result
      from the fifth step and **IMAGE_ROOTFS_ALIGNMENT** is subtracted from the
      result of the fifth step. In this way, the result from the fourth step is
      rounded up to the nearest multiple of **IMAGE_ROOTFS_ALIGNMENT**.

      Thus, if the **IMAGE_ROOTFS_MAXSIZE** is set, is compared with the result
      of the above calculations and is independent of the final image type.
      No default value is set for **IMAGE_ROOTFS_MAXSIZE**.

      It's a good idea to set this variable for images that need to fit on a limited
      space (e.g. SD card, a fixed-size partition, ...).

   **IMAGE_ROOTFS_SIZE**
      Defines the size in Kbytes for the generated image. The OpenEmbedded
      build system determines the final size for the generated image using
      an algorithm that takes into account the initial disk space used for
      the generated image, a requested size for the image, and requested
      additional free disk space to be added to the image. Programatically,
      the build system determines the final size of the generated image as
      follows::

         if (image-du * overhead) < rootfs-size:
             internal-rootfs-size = rootfs-size + xspace
         else:
             internal-rootfs-size = (image-du * overhead) + xspace
         where:
             image-du = Returned value of the du command on the image.
             overhead = IMAGE_OVERHEAD_FACTOR
             rootfs-size = IMAGE_ROOTFS_SIZE
             internal-rootfs-size = Initial root filesystem size before any modifications.
             xspace = IMAGE_ROOTFS_EXTRA_SPACE

      See the **IMAGE_OVERHEAD_FACTOR**
      and **IMAGE_ROOTFS_EXTRA_SPACE**
      variables for related information.

   **IMAGE_TYPEDEP**
      Specifies a dependency from one image type on another. Here is an
      example from the ref-classes-image-live class::

         IMAGE_TYPEDEP:live = "ext3"

      In the previous example, the variable ensures that when "live" is
      listed with the **IMAGE_FSTYPES** variable,
      the OpenEmbedded build system produces an `ext3` image first since
      one of the components of the live image is an `ext3` formatted
      partition containing the root filesystem.

   **IMAGE_TYPES**
      Specifies the complete list of supported image types by default:

      - btrfs
      - container
      - cpio
      - cpio.gz
      - cpio.lz4
      - cpio.lzma
      - cpio.xz
      - cramfs
      - erofs
      - erofs-lz4
      - erofs-lz4hc
      - ext2
      - ext2.bz2
      - ext2.gz
      - ext2.lzma
      - ext3
      - ext3.gz
      - ext4
      - ext4.gz
      - f2fs
      - hddimg
      - iso
      - jffs2
      - jffs2.sum
      - multiubi
      - squashfs
      - squashfs-lz4
      - squashfs-lzo
      - squashfs-xz
      - tar
      - tar.bz2
      - tar.gz
      - tar.lz4
      - tar.xz
      - tar.zst
      - ubi
      - ubifs
      - wic
      - wic.bz2
      - wic.gz
      - wic.lzma
      - wic.zst

      For more information about these types of images, see
      `meta/classes-recipe/image_types*.bbclass` in :term:`OpenEmbedded-Core
      (OE-Core)`.

   **IMAGE_VERSION_SUFFIX**
      Version suffix that is part of the default **IMAGE_NAME** and
      **KERNEL_ARTIFACT_NAME** values.
      Defaults to `"-${DATETIME}"`, however you could set this to a
      version string that comes from your external build environment if
      desired, and this suffix would then be used consistently across
      the build artifacts.

   **IMGDEPLOYDIR**
      When inheriting the ref-classes-image class directly or
      through the ref-classes-core-image class, the
      **IMGDEPLOYDIR** points to a temporary work area for deployed files
      that is set in the `image` class as follows::

         IMGDEPLOYDIR = "${WORKDIR}/deploy-${PN}-image-complete"

      Recipes inheriting the ref-classes-image class should copy
      files to be deployed into **IMGDEPLOYDIR**, and the class will take
      care of copying them into **DEPLOY_DIR_IMAGE** afterwards.

   **IMGMANIFESTDIR**
      When inheriting the ref-classes-image class directly or through the
      ref-classes-core-image class, the **IMGMANIFESTDIR** setting
      points to a temporary area that stores manifest `json` files, that list
      what images were created by various images creation tasks (as defined by
      the **IMAGE_FSTYPES** variable). It is set in the
      ref-classes-image class as follows::

          IMGMANIFESTDIR = "${WORKDIR}/image-task-manifest"

   **INCOMPATIBLE_LICENSE**
      Specifies a space-separated list of license names (as they would
      appear in **LICENSE**) that should be excluded
      from the build (if set globally), or from an image (if set locally
      in an image recipe).

      When the variable is set globally, recipes that provide no alternatives to listed
      incompatible licenses are not built. Packages that are individually
      licensed with the specified incompatible licenses will be deleted.
      Most of the time this does not allow a feasible build (because it becomes impossible
      to satisfy build time dependencies), so the recommended way to
      implement license restrictions is to set the variable in specific
      image recipes where the restrictions must apply. That way there
      are no build time restrictions, but the license check is still
      performed when the image's filesystem is assembled from packages.

      There is some support for wildcards in this variable's value,
      however it is restricted to specific licenses. Currently only
      these wildcards are allowed and expand as follows:

      - `AGPL-3.0*"`: `AGPL-3.0-only`, `AGPL-3.0-or-later`
      - `GPL-3.0*`: `GPL-3.0-only`, `GPL-3.0-or-later`
      - `LGPL-3.0*`: `LGPL-3.0-only`, `LGPL-3.0-or-later`

      .. note::

         This functionality is only regularly tested using the following
         setting::

                 INCOMPATIBLE_LICENSE = "GPL-3.0* LGPL-3.0* AGPL-3.0*"

         Although you can use other settings, you might be required to
         remove dependencies on (or provide alternatives to) components that
         are required to produce a functional system image.

   **INCOMPATIBLE_LICENSE_EXCEPTIONS**
      Specifies a space-separated list of package and license pairs that
      are allowed to be used even if the license is specified in
      **INCOMPATIBLE_LICENSE**. The package and license pairs are
      separated using a colon. Example::

         INCOMPATIBLE_LICENSE_EXCEPTIONS = "gdbserver:GPL-3.0-only gdbserver:LGPL-3.0-only"

   **INHERIT**
      Causes the named class or classes to be inherited globally. Anonymous
      functions in the class or classes are not executed for the base
      configuration and in each individual recipe. The OpenEmbedded build
      system ignores changes to **INHERIT** in individual recipes.
      Classes inherited using **INHERIT** must be located in the
      `classes-global/` or `classes/` subdirectories.

      For more information on **INHERIT**, see the
      bitbake-user-manual/bitbake-user-manual-metadata:\\`inherit\`\` configuration directive`"
      section in the BitBake User Manual.

   **INHERIT_DISTRO**
      Lists classes that will be inherited at the distribution level. It is
      unlikely that you want to edit this variable.

      Classes specified in **INHERIT_DISTRO** must be located in the
      `classes-global/` or `classes/` subdirectories.

      The default value of the variable is set as follows in the
      `meta/conf/distro/defaultsetup.conf` file::

         INHERIT_DISTRO ?= "debian devshell sstate license remove-libtool create-spdx"

   **INHIBIT_AUTOTOOLS_DEPS**
      Prevents the ref-classes-autotools class from automatically adding
      its default build-time dependencies.

      When a recipe inherits the ref-classes-autotools class, several
      native cross tools such as `autoconf-native`, `automake-native`,
      `libtool-native`, `libtool-cross` are added to **DEPENDS** to
      support the `autotools` build process.

      To prevent the build system from adding these dependencies automatically,
      set the **INHIBIT_AUTOTOOLS_DEPS** variable as follows::

         INHIBIT_AUTOTOOLS_DEPS = "1"

      By default, the value of **INHIBIT_AUTOTOOLS_DEPS** is empty. Setting
      it to "0" does not disable inhibition. Only the empty string will disable
      inhibition.

   **INHIBIT_DEFAULT_DEPS**
      Prevents the default dependencies, namely the C compiler and standard
      C library (libc), from being added to **DEPENDS**.
      This variable is usually used within recipes that do not require any
      compilation using the C compiler.

      Set the variable to "1" to prevent the default dependencies from
      being added.

   **INHIBIT_DEFAULT_RUST_DEPS**
      Prevents the ref-classes-rust class from automatically adding
      its default build-time dependencies.

      When a recipe inherits the ref-classes-rust class, several
      tools such as `rust-native` and `${RUSTLIB_DEP}` (only added when cross-compiling) are added
      to **DEPENDS** to support the `rust` build process.

      To prevent the build system from adding these dependencies automatically,
      set the **INHIBIT_DEFAULT_RUST_DEPS** variable as follows::

         INHIBIT_DEFAULT_RUST_DEPS = "1"

      By default, the value of **INHIBIT_DEFAULT_RUST_DEPS** is empty. Setting
      it to "0" does not disable inhibition. Only the empty string will disable
      inhibition.

   **INHIBIT_PACKAGE_DEBUG_SPLIT**
      Prevents the OpenEmbedded build system from splitting out debug
      information during packaging. By default, the build system splits out
      debugging information during the
      ref-tasks-package task. For more information on
      how debug information is split out, see the
      **PACKAGE_DEBUG_SPLIT_STYLE**
      variable.

      To prevent the build system from splitting out debug information
      during packaging, set the **INHIBIT_PACKAGE_DEBUG_SPLIT** variable as
      follows::

         INHIBIT_PACKAGE_DEBUG_SPLIT = "1"

   **INHIBIT_PACKAGE_STRIP**
      If set to "1", causes the build to not strip binaries in resulting
      packages and prevents the `-dbg` package from containing the source
      files.

      By default, the OpenEmbedded build system strips binaries and puts
      the debugging symbols into `${`\ **PN**\ `}-dbg`.
      Consequently, you should not set **INHIBIT_PACKAGE_STRIP** when you
      plan to debug in general.

   **INHIBIT_SYSROOT_STRIP**
      If set to "1", causes the build to not strip binaries in the
      resulting sysroot.

      By default, the OpenEmbedded build system strips binaries in the
      resulting sysroot. When you specifically set the
      **INHIBIT_SYSROOT_STRIP** variable to "1" in your recipe, you inhibit
      this stripping.

      If you want to use this variable, include the ref-classes-staging
      class. This class uses a `sys_strip()` function to test for the variable
      and acts accordingly.

      .. note::

         Use of the **INHIBIT_SYSROOT_STRIP** variable occurs in rare and
         special circumstances. For example, suppose you are building
         bare-metal firmware by using an external GCC toolchain. Furthermore,
         even if the toolchain's binaries are strippable, there are other files
         needed for the build that are not strippable.

   **INHIBIT_UPDATERCD_BBCLASS**
      Prevents the ref-classes-update-rc.d class from automatically
      installing and registering SysV init scripts for packages.

      When a recipe inherits the ref-classes-update-rc.d class, init
      scripts are typically installed and registered for the packages listed in
      **INITSCRIPT_PACKAGES**. This ensures that the relevant
      services are started and stopped at the appropriate runlevels using the
      traditional SysV init system.

      To prevent the build system from adding these scripts and configurations
      automatically, set the **INHIBIT_UPDATERCD_BBCLASS** variable as follows::

         INHIBIT_UPDATERCD_BBCLASS = "1"

      By default, the value of **INHIBIT_UPDATERCD_BBCLASS** is empty. Setting
      it to "0" does not disable inhibition. Only the empty string will disable
      inhibition.

   **INIT_MANAGER**
      Specifies the system init manager to use. Available options are:

      -  `sysvinit`
      -  `systemd`
      -  `mdev-busybox`

      With `sysvinit`, the init manager is set to
      SysVinit , the traditional UNIX init
      system. This is the default choice in the **Poky** distribution, together with
      the Udev device manager (see the "device-manager" section).

      With `systemd`, the init manager becomes systemd ,
      which comes with the udev  device manager.

      With `mdev-busybox`, the init manager becomes the much simpler BusyBox
      init, together with the BusyBox mdev device manager. This is the simplest
      and lightest solution, and probably the best choice for low-end systems
      with a rather slow CPU and a limited amount of RAM.

      More concretely, this is used to include
      `conf/distro/include/init-manager-${INIT_MANAGER}.inc` into the global
      configuration. You can have a look at the
      meta/conf/distro/include/init-manager-*.inc 
      files for more information, and also the "init-manager"
      section in the Yocto Project Development Tasks Manual.

   **INITRAMFS_DEPLOY_DIR_IMAGE**
      Indicates the deploy directory used by ref-tasks-bundle_initramfs
      where the **INITRAMFS_IMAGE** will be fetched from. This variable is
      set by default to `${DEPLOY_DIR_IMAGE}` in the
      ref-classes-kernel class and it's only meant to be changed when
      building an **Initramfs** image from a separate multiconfig via
      **INITRAMFS_MULTICONFIG**.

   **INITRAMFS_FSTYPES**
      Defines the format for the output image of an initial RAM filesystem
      (**Initramfs**), which is used during boot. Supported formats are the
      same as those supported by the
      **IMAGE_FSTYPES** variable.

      The default value of this variable, which is set in the
      `meta/conf/bitbake.conf` configuration file in
      **OpenEmbedded-Core (OE-Core)**, is "cpio.gz". The Linux kernel's
      **Initramfs** mechanism, as opposed to the initial RAM filesystem
      initrd  mechanism, expects
      an optionally compressed cpio archive.

   **INITRAMFS_IMAGE**
      Specifies the **PROVIDES** name of an image
      recipe that is used to build an initial RAM filesystem (**Initramfs**)
      image. In other words, the **INITRAMFS_IMAGE** variable causes an
      additional recipe to be built as a dependency to whatever root
      filesystem recipe you might be using (e.g. `core-image-sato`). The
      **Initramfs** image recipe you provide should set
      **IMAGE_FSTYPES** to
      **INITRAMFS_FSTYPES**.

      An **Initramfs** image provides a temporary root filesystem used for
      early system initialization (e.g. loading of modules needed to locate
      and mount the "real" root filesystem).

      .. note::

         See the `meta/recipes-core/images/core-image-minimal-initramfs.bb`
         recipe in **OpenEmbedded-Core (OE-Core)**
         for an example **Initramfs** recipe. To select this sample recipe as
         the one built to provide the **Initramfs** image, set **INITRAMFS_IMAGE**
         to "core-image-minimal-initramfs".

      You can also find more information by referencing the
      `conf/templates/default/local.conf.sample.extended`
      configuration file in meta-poky , the ref-classes-image
      class, and the ref-classes-kernel class to see how to use the
      **INITRAMFS_IMAGE** variable.

      If **INITRAMFS_IMAGE** is empty, which is the default, then no
      **Initramfs** image is built.

      For more information, you can also see the
      **INITRAMFS_IMAGE_BUNDLE**
      variable, which allows the generated image to be bundled inside the
      kernel image. Additionally, for information on creating an **Initramfs**
      image, see the "dev-manual/building:building an initial ram filesystem (Initramfs) image" section
      in the Yocto Project Development Tasks Manual.

   **INITRAMFS_IMAGE_BUNDLE**
      Controls whether or not the image recipe specified by
      **INITRAMFS_IMAGE** is run through an
      extra pass
      (ref-tasks-bundle_initramfs) during
      kernel compilation in order to build a single binary that contains
      both the kernel image and the initial RAM filesystem (**Initramfs**)
      image. This makes use of the
      **CONFIG_INITRAMFS_SOURCE** kernel
      feature.

      .. note::

         Bundling the **Initramfs** with the kernel conflates the code in the
         **Initramfs** with the GPLv2 licensed Linux kernel binary. Thus only GPLv2
         compatible software may be part of a bundled **Initramfs**.

      .. note::

         Using an extra compilation pass to bundle the **Initramfs** avoids a
         circular dependency between the kernel recipe and the **Initramfs**
         recipe should the **Initramfs** include kernel modules. Should that be
         the case, the **Initramfs** recipe depends on the kernel for the
         kernel modules, and the kernel depends on the **Initramfs** recipe
         since the **Initramfs** is bundled inside the kernel image.

      The combined binary is deposited into the `tmp/deploy` directory,
      which is part of the **Build Directory**.

      Setting the variable to "1" in a configuration file causes the
      OpenEmbedded build system to generate a kernel image with the
      **Initramfs** specified in **INITRAMFS_IMAGE** bundled within::

         INITRAMFS_IMAGE_BUNDLE = "1"

      By default, the ref-classes-kernel class sets this variable to a
      null string as follows::

         INITRAMFS_IMAGE_BUNDLE ?= ""

      .. note::

         You must set the **INITRAMFS_IMAGE_BUNDLE** variable in a
         configuration file. You cannot set the variable in a recipe file.

      See the
      local.conf.sample.extended 
      file for additional information. Also, for information on creating an
      **Initramfs**, see the "dev-manual/building:building an initial ram filesystem (Initramfs) image" section
      in the Yocto Project Development Tasks Manual.

   **INITRAMFS_IMAGE_NAME**

      This value needs to stay in sync with **IMAGE_LINK_NAME**, but with
      **INITRAMFS_IMAGE** instead of **IMAGE_BASENAME**. The default value
      is set as follows:

         INITRAMFS_IMAGE_NAME ?= "${@['${INITRAMFS_IMAGE}${IMAGE_MACHINE_SUFFIX}', ''][d.getVar('INITRAMFS_IMAGE') == '']}"

      That is, if **INITRAMFS_IMAGE** is set, the value of
      **INITRAMFS_IMAGE_NAME** will be set based upon
      **INITRAMFS_IMAGE** and **IMAGE_MACHINE_SUFFIX**.

   **INITRAMFS_LINK_NAME**
      The link name of the initial RAM filesystem image. This variable is
      set in the `meta/classes-recipe/kernel-artifact-names.bbclass` file as
      follows::

         INITRAMFS_LINK_NAME ?= "initramfs-${KERNEL_ARTIFACT_LINK_NAME}"

      The value of the
      `KERNEL_ARTIFACT_LINK_NAME` variable, which is set in the same
      file, has the following value::

         KERNEL_ARTIFACT_LINK_NAME ?= "${MACHINE}"

      See the **MACHINE** variable for additional
      information.

   **INITRAMFS_MAXSIZE**
      Defines the maximum allowed size of the **Initramfs** image in Kbytes.
      The build will fail if the **Initramfs** image size exceeds this value.

      The **Initramfs** image size undergoes several calculation steps before
      being compared to **INITRAMFS_MAXSIZE**.
      These steps are the same as those used for **IMAGE_ROOTFS_MAXSIZE**
      and are described in detail in that entry.

      Thus, **INITRAMFS_MAXSIZE** is compared with the result of the calculations
      and is independent of the final image type (e.g. compressed).
      A default value for **INITRAMFS_MAXSIZE** is set in
      meta/conf/bitbake.conf .

   **INITRAMFS_MULTICONFIG**
      Defines the multiconfig to create a multiconfig dependency to be used by
      the ref-classes-kernel class.

      This allows the kernel to bundle an **INITRAMFS_IMAGE** coming from
      a separate multiconfig, this is meant to be used in addition to **INITRAMFS_DEPLOY_DIR_IMAGE**.

      For more information on how to bundle an **Initramfs** image from a separate
      multiconfig see the "dev-manual/building:Bundling an Initramfs Image From a Separate Multiconfig"
      section in the Yocto Project Development Tasks Manual.

   **INITRAMFS_NAME**
      The base name of the initial RAM filesystem image. This variable is
      set in the `meta/classes-recipe/kernel-artifact-names.bbclass` file as
      follows::

         INITRAMFS_NAME ?= "initramfs-${KERNEL_ARTIFACT_NAME}"

      See **KERNEL_ARTIFACT_NAME** for additional information.

   **INITRD**
      Indicates list of filesystem images to concatenate and use as an
      initial RAM disk (`initrd`).

      The **INITRD** variable is an optional variable used with the
      ref-classes-image-live class.

   **INITRD_IMAGE**
      When building a "live" bootable image (i.e. when
      **IMAGE_FSTYPES** contains "live"),
      **INITRD_IMAGE** specifies the image recipe that should be built to
      provide the initial RAM disk image. The default value is
      "core-image-minimal-initramfs".

      See the ref-classes-image-live class for more information.

   **INITSCRIPT_NAME**
      The filename of the initialization script as installed to
      `${sysconfdir}/init.d`.

      This variable is used in recipes when using ref-classes-update-rc.d.
      The variable is mandatory.

   **INITSCRIPT_PACKAGES**
      A list of the packages that contain initscripts. If multiple packages
      are specified, you need to append the package name to the other
      `INITSCRIPT_*` as an override.

      This variable is used in recipes when using ref-classes-update-rc.d.
      The variable is optional and defaults to the **PN**
      variable.

   **INITSCRIPT_PARAMS**
      Specifies the options to pass to `update-rc.d`. Here is an example::

         INITSCRIPT_PARAMS = "start 99 5 2 . stop 20 0 1 6 ."

      In this example, the script has a runlevel of 99, starts the script
      in initlevels 2 and 5, and stops the script in levels 0, 1 and 6.

      The variable's default value is "defaults", which is set in the
      ref-classes-update-rc.d class.

      The value in **INITSCRIPT_PARAMS** is passed through to the
      `update-rc.d` command. For more information on valid parameters,
      please see the manual page: `update-rc.d <update-rc.d(8)>`.

   **INSANE_SKIP**
      Specifies the QA checks to skip for a specific package within a
      recipe. For example, to skip the check for symbolic link `.so`
      files in the main package of a recipe, add the following to the
      recipe. The package name override must be used, which in this example
      is `${PN}`::

         INSANE_SKIP:${PN} += "dev-so"

      See the "ref-classes-insane" section for a
      list of the valid QA checks you can specify using this variable.

   **INSTALL_TIMEZONE_FILE**
      By default, the `tzdata` recipe packages an `/etc/timezone` file.
      Set the **INSTALL_TIMEZONE_FILE** variable to "0" at the
      configuration level to disable this behavior.

   **IPK_FEED_URIS**
      When the IPK backend is in use and package management is enabled on
      the target, you can use this variable to set up `opkg` in the
      target image to point to package feeds on a nominated server. Once
      the feed is established, you can perform installations or upgrades
      using the package manager at runtime.

   **KARCH**
      Defines the kernel architecture used when assembling the
      configuration. Architectures supported for this release are:

      - powerpc
      - i386
      - x86_64
      - arm
      - qemu
      - mips

      You define the **KARCH** variable in the kernel-dev/advanced:bsp descriptions.

   **KBRANCH**
      A regular expression used by the build process to explicitly identify
      the kernel branch that is validated, patched, and configured during a
      build. You must set this variable to ensure the exact kernel branch
      you want is being used by the build process.

      Values for this variable are set in the kernel's recipe file and the
      kernel's append file. For example, if you are using the
      `linux-yocto_4.12` kernel, the kernel recipe file is the
      `meta/recipes-kernel/linux/linux-yocto_4.12.bb` file. **KBRANCH**
      is set as follows in that kernel recipe file::

         KBRANCH ?= "standard/base"

      This variable is also used from the kernel's append file to identify
      the kernel branch specific to a particular machine or target
      hardware. Continuing with the previous kernel example, the kernel's
      append file is located in the
      BSP layer for a given machine. For example, the append file for the
      Beaglebone and generic versions of both 32 and 64-bit IA
      machines (`meta-yocto-bsp`) is named
      `meta-yocto-bsp/recipes-kernel/linux/linux-yocto_6.1.bbappend`.
      Here are the related statements from that append file::

         KBRANCH:genericx86  = "v6.1/standard/base"
         KBRANCH:genericx86-64  = "v6.1/standard/base"
         KBRANCH:beaglebone-yocto = "v6.1/standard/beaglebone"

      The **KBRANCH** statements
      identify the kernel branch to use when building for each supported
      BSP.

   **KBUILD_DEFCONFIG**
      When used with the ref-classes-kernel-yocto
      class, specifies an "in-tree" kernel configuration file for use
      during a kernel build.

      Typically, when using a `defconfig` to configure a kernel during a
      build, you place the file in your layer in the same manner as you
      would place patch files and configuration fragment files (i.e.
      "out-of-tree"). However, if you want to use a `defconfig` file that
      is part of the kernel tree (i.e. "in-tree"), you can use the
      **KBUILD_DEFCONFIG** variable to point to the
      `defconfig` file.

      To use the variable, set it in the append file for your kernel recipe
      using the following form::

         KBUILD_DEFCONFIG:<machine> ?= "defconfig_file"

      Here is an example from a "raspberrypi2" **MACHINE** build that uses
      a `defconfig` file named "bcm2709_defconfig"::

         KBUILD_DEFCONFIG:raspberrypi2 = "bcm2709_defconfig"

      As an alternative, you can use the following within your append file::

         KBUILD_DEFCONFIG:pn-linux-yocto ?= "defconfig_file"

      For more
      information on how to use the **KBUILD_DEFCONFIG** variable, see the
      "kernel-dev/common:using an "in-tree" \\`defconfig\`\` file`"
      section in the Yocto Project Linux Kernel Development Manual.

   **KCONF_AUDIT_LEVEL**
      When inheriting the ref-classes-kernel-yocto class and when the
      **KMETA_AUDIT** variable is set to a non-empty string, the
      **KCONF_AUDIT_LEVEL** variable specifies whether to report Kernel
      configuration values that are different from the user-specified value. Its
      value is a positive integer (default: 1):

      -  0: no reporting is done.

      -  1: report the problems as warnings and trigger an error if
         **KMETA_AUDIT_WERROR** is set.

      -  2: if the ref-tasks-kernel_configme has failed to generate a
         `.config` file, print the content of the `merge_config_build.log`
         file containing the errors, instead of just providing the path to
         that file.

      For more details see the ref-classes-kernel-yocto class and the
      symbol_why.py 
      script in yocto-kernel-tools .

   **KCONF_BSP_AUDIT_LEVEL**
      When inheriting the ref-classes-kernel-yocto class and when the
      **KMETA_AUDIT** variable is set to a non-empty string, the
      **KCONF_BSP_AUDIT_LEVEL** variable can be set to report:

      #.  User-specified Kernel configurations that did not make it into the final
          Kernel configuration.

      #.  Configurations defined in multiple input files with differing values.

      Its value is a positive integer (default: 0):

      -  0: no reporting is done.

      -  1: reporting of configuration options that did not make it in the
         final configuration is done and is not limited to the current
         architecture (`ARCH`) in use.

      -  2: reporting of configuration options that did not make it in the
         final configuration is done and is strictly limited to the current
         architecture (`ARCH`) in use.

      -  3: report the problems found when this variable equals 2, and also
         report configurations options defined in multiple input files with
         differing values.

      For value 1, 2 and 3 an error is produced if **KMETA_AUDIT_WERROR**
      is set.

      For more details see the ref-classes-kernel-yocto class and the
      symbol_why.py 
      script in yocto-kernel-tools .

   **KCONFIG_MODE**
      When used with the ref-classes-kernel-yocto
      class, specifies the kernel configuration values to use for options
      not specified in the provided `defconfig` file. Valid options are::

         KCONFIG_MODE = "alldefconfig"
         KCONFIG_MODE = "allnoconfig"

      In `alldefconfig` mode the options not explicitly specified will be
      assigned their Kconfig default value. In `allnoconfig` mode the
      options not explicitly specified will be disabled in the kernel
      config.

      In case **KCONFIG_MODE** is not set the `defconfig` file
      will be handled in `allnoconfig` mode.

      A `defconfig` file compatible with `allnoconfig` mode can be
      generated by copying the `.config` file from a working Linux kernel
      build, renaming it to `defconfig` and placing it into the Linux
      kernel `${WORKDIR}` through your meta-layer. **KCONFIG_MODE** does
      not need to be explicitly set.

      A `defconfig` file compatible with `alldefconfig` mode can be
      generated using the
      ref-tasks-savedefconfig
      task and placed into the Linux kernel `${WORKDIR}` through your
      meta-layer. Explicitely set **KCONFIG_MODE**::

         KCONFIG_MODE = "alldefconfig"

   **KERNEL_ALT_IMAGETYPE**
      Specifies an alternate kernel image type for creation in addition to
      the kernel image type specified using the **KERNEL_IMAGETYPE** and
      **KERNEL_IMAGETYPES** variables.

   **KERNEL_ARTIFACT_NAME**
      Specifies the name of all of the build artifacts. You can change the
      name of the artifacts by changing the **KERNEL_ARTIFACT_NAME**
      variable.

      The value of **KERNEL_ARTIFACT_NAME**, which is set in the
      `meta/classes-recipe/kernel-artifact-names.bbclass` file, has the
      following default value::

         KERNEL_ARTIFACT_NAME ?= "${PKGE}-${PKGV}-${PKGR}${IMAGE_MACHINE_SUFFIX}${IMAGE_VERSION_SUFFIX}"

      See the **PKGE**, **PKGV**, **PKGR**, **IMAGE_MACHINE_SUFFIX**
      and **IMAGE_VERSION_SUFFIX** variables for additional information.

   **KERNEL_CLASSES**
      A list of classes defining kernel image types that the
      ref-classes-kernel class should inherit. You typically
      append this variable to enable extended image types.
      You can register custom kernel image types with the
      ref-classes-kernel class using this variable.

   **KERNEL_CONSOLE**
     The **KERNEL_CONSOLE** variable holds the value of the `console`
     parameter of the kernel command line and can be used in places such as a
     `wks` description file for :ref:`Wic images <dev-manual/wic:creating
     partitioned images using wic>`.

     The default value of this variable is extracted from the first console
     device and setting in **SERIAL_CONSOLES**. If nothing is found in
     **SERIAL_CONSOLES**, the default value is set to `ttyS0,115200`.

     For more information, see the `Kernel command-line documentation
     <https://www.kernel.org/doc/html/latest/admin-guide/kernel-parameters.html>`__.

   **KERNEL_DANGLING_FEATURES_WARN_ONLY**
      When kernel configuration fragments are missing for some
      **KERNEL_FEATURES** specified by layers or BSPs,
      building and configuring the kernel stops with an error.

      You can turn these errors into warnings by setting the
      following in `conf/local.conf`::

         KERNEL_DANGLING_FEATURES_WARN_ONLY = "1"

      You will still be warned that runtime issues may occur,
      but at least the kernel configuration and build process will
      be allowed to continue.

   **KERNEL_DEBUG_TIMESTAMPS**
      If set to "1", enables timestamping functionality during building
      the kernel. The default is "0" to disable this for reproducibility
      reasons.

   **KERNEL_DEPLOY_DEPEND**
      Provides a means of controlling the dependency of an image recipe
      on the kernel. The default value is "virtual/kernel:do_deploy",
      however for a small initramfs image or other images that do not
      need the kernel, this can be set to "" in the image recipe.

   **KERNEL_DEVICETREE**
      Specifies the name of the generated Linux kernel device tree (i.e.
      the `.dtb`) file.

      .. note::

         There is legacy support for specifying the full path to the device
         tree. However, providing just the `.dtb` file is preferred.

      In order to use this variable, the ref-classes-kernel-devicetree
      class must be inherited.

   **KERNEL_DEVICETREE_BUNDLE**
      When set to "1", this variable allows to bundle the Linux kernel
      and the Device Tree Binary together in a single file.

      This feature is currently only supported on the "arm" (32 bit)
      architecture.

      This variable is set to "0" by default by the
      ref-classes-kernel-devicetree class.

   **KERNEL_DTB_LINK_NAME**
      The link name of the kernel device tree binary (DTB). This variable
      is set in the `meta/classes-recipe/kernel-artifact-names.bbclass` file as
      follows::

         KERNEL_DTB_LINK_NAME ?= "${KERNEL_ARTIFACT_LINK_NAME}"

      The
      value of the `KERNEL_ARTIFACT_LINK_NAME` variable, which is set in
      the same file, has the following value::

         KERNEL_ARTIFACT_LINK_NAME ?= "${MACHINE}"

      See the **MACHINE** variable for additional
      information.

   **KERNEL_DTB_NAME**
      The base name of the kernel device tree binary (DTB). This variable
      is set in the `meta/classes-recipe/kernel-artifact-names.bbclass` file as
      follows::

         KERNEL_DTB_NAME ?= "${KERNEL_ARTIFACT_NAME}"

      See **KERNEL_ARTIFACT_NAME** for additional information.

   **KERNEL_DTBDEST**
      This variable, used by the ref-classes-kernel-devicetree
      class, allows to change the installation directory of the DTB
      (Device Tree Binary) files.

      It is set by default to "${KERNEL_IMAGEDEST}" by the
      ref-classes-kernel class.

   **KERNEL_DTBVENDORED**
      This variable, used by the ref-classes-kernel-devicetree,
      allows to ignore vendor subdirectories when installing DTB
      (Device Tree Binary) files, when it is set to "false".

      To keep vendor subdirectories, set this variable to "true".

      It is set by default to "false" by the ref-classes-kernel class.

   **KERNEL_DTC_FLAGS**
      Specifies the `dtc` flags that are passed to the Linux kernel build
      system when generating the device trees (via `DTC_FLAGS` environment
      variable).

      In order to use this variable, the ref-classes-kernel-devicetree
      class must be inherited.

   **KERNEL_EXTRA_ARGS**
      Specifies additional `make` command-line arguments the OpenEmbedded
      build system passes on when compiling the kernel.

   **KERNEL_FEATURES**
      Includes additional kernel metadata. In the OpenEmbedded build
      system, the default Board Support Packages (BSPs)
      **Metadata** is provided through the
      **KMACHINE** and **KBRANCH**
      variables. You can use the **KERNEL_FEATURES** variable from within
      the kernel recipe or kernel append file to further add metadata for
      all BSPs or specific BSPs.

      The metadata you add through this variable includes config fragments
      and features descriptions, which usually includes patches as well as
      config fragments. You typically override the **KERNEL_FEATURES**
      variable for a specific machine. In this way, you can provide
      validated, but optional, sets of kernel configurations and features.

      For example, the following example from the `linux-yocto-rt_4.12`
      kernel recipe adds "netfilter" and "taskstats" features to all BSPs
      as well as "virtio" configurations to all QEMU machines. The last two
      statements add specific configurations to targeted machine types::

         KERNEL_EXTRA_FEATURES ?= "features/netfilter/netfilter.scc features/taskstats/taskstats.scc"
         KERNEL_FEATURES:append = " ${KERNEL_EXTRA_FEATURES}"
         KERNEL_FEATURES:append:qemuall = " cfg/virtio.scc"
         KERNEL_FEATURES:append:qemux86 = "  cfg/sound.scc cfg/paravirt_kvm.scc"
         KERNEL_FEATURES:append:qemux86-64 = " cfg/sound.scc"

   **KERNEL_FIT_LINK_NAME**
      The link name of the kernel flattened image tree (FIT) image. This
      variable is set in the `meta/classes-recipe/kernel-artifact-names.bbclass`
      file as follows::

         KERNEL_FIT_LINK_NAME ?= "${KERNEL_ARTIFACT_LINK_NAME}"

      The value of the
      `KERNEL_ARTIFACT_LINK_NAME` variable, which is set in the same
      file, has the following value::

         KERNEL_ARTIFACT_LINK_NAME ?= "${MACHINE}"

      See the **MACHINE** variable for additional
      information.

   **KERNEL_FIT_NAME**
      The base name of the kernel flattened image tree (FIT) image. This
      variable is set in the `meta/classes-recipe/kernel-artifact-names.bbclass`
      file as follows::

         KERNEL_FIT_NAME ?= "${KERNEL_ARTIFACT_NAME}"

      See **KERNEL_ARTIFACT_NAME** for additional information.

   **KERNEL_IMAGE_LINK_NAME**
      The link name for the kernel image. This variable is set in the
      `meta/classes-recipe/kernel-artifact-names.bbclass` file as follows::

         KERNEL_IMAGE_LINK_NAME ?= "${KERNEL_ARTIFACT_LINK_NAME}"

      The value of
      the `KERNEL_ARTIFACT_LINK_NAME` variable, which is set in the same
      file, has the following value::

         KERNEL_ARTIFACT_LINK_NAME ?= "${MACHINE}"

      See the **MACHINE** variable for additional
      information.

   **KERNEL_IMAGE_MAXSIZE**
      Specifies the maximum size of the kernel image file in kilobytes. If
      **KERNEL_IMAGE_MAXSIZE** is set, the size of the kernel image file is
      checked against the set value during the
      ref-tasks-sizecheck task. The task fails if
      the kernel image file is larger than the setting.

      **KERNEL_IMAGE_MAXSIZE** is useful for target devices that have a
      limited amount of space in which the kernel image must be stored.

      By default, this variable is not set, which means the size of the
      kernel image is not checked.

   **KERNEL_IMAGE_NAME**
      The base name of the kernel image. This variable is set in the
      `meta/classes-recipe/kernel-artifact-names.bbclass` file as follows::

         KERNEL_IMAGE_NAME ?= "${KERNEL_ARTIFACT_NAME}"

      See **KERNEL_ARTIFACT_NAME** for additional information.

   **KERNEL_IMAGETYPE**
      The type of kernel to build for a device, usually set by the machine
      configuration files and defaults to "zImage". This variable is used
      when building the kernel and is passed to `make` as the target to
      build.

      To build additional kernel image types, use **KERNEL_IMAGETYPES**.

   **KERNEL_IMAGETYPES**
      Lists additional types of kernel images to build for a device in addition
      to image type specified in **KERNEL_IMAGETYPE**. Usually set by the
      machine configuration files.

   **KERNEL_LOCALVERSION**
      This variable allows to append a string to the version
      of the kernel image. This corresponds to the `CONFIG_LOCALVERSION`
      kernel configuration parameter.

      Using this variable is only useful when you are using a kernel recipe
      inheriting the ref-classes-kernel class, and which doesn't
      already set a local version. Therefore, setting this variable has no
      impact on `linux-yocto` kernels.

   **KERNEL_MODULE_AUTOLOAD**
      Lists kernel modules that need to be auto-loaded during boot.

      .. note::

         This variable replaces the deprecated **module_autoload**
         variable.

      You can use the **KERNEL_MODULE_AUTOLOAD** variable anywhere that it
      can be recognized by the kernel recipe or by an out-of-tree kernel
      module recipe (e.g. a machine configuration file, a distribution
      configuration file, an append file for the recipe, or the recipe
      itself).

      Specify it as follows::

         KERNEL_MODULE_AUTOLOAD += "module_name1 module_name2 module_name3"

      Including **KERNEL_MODULE_AUTOLOAD** causes the OpenEmbedded build
      system to populate the `/etc/modules-load.d/modname.conf` file with
      the list of modules to be auto-loaded on boot. The modules appear
      one-per-line in the file. Here is an example of the most common use
      case::

         KERNEL_MODULE_AUTOLOAD += "module_name"

      For information on how to populate the `modname.conf` file with
      `modprobe.d` syntax lines, see the **KERNEL_MODULE_PROBECONF** variable.

   **KERNEL_MODULE_PROBECONF**
      Provides a list of modules for which the OpenEmbedded build system
      expects to find `module_conf_`\ modname values that specify
      configuration for each of the modules. For information on how to
      provide those module configurations, see the
      **module_conf_* <module_conf>** variable.

   **KERNEL_PACKAGE_NAME**
      Specifies the base name of the kernel packages, such as "kernel"
      in the kernel packages such as "kernel-modules", "kernel-image" and
      "kernel-dbg".

      The default value for this variable is set to "kernel" by the
      ref-classes-kernel class.

   **KERNEL_PATH**
      The location of the kernel sources. This variable is set to the value
      of the **STAGING_KERNEL_DIR** within the ref-classes-module
      class. For information on how this variable is used, see the
      "kernel-dev/common:incorporating out-of-tree modules"
      section in the Yocto Project Linux Kernel Development Manual.

      To help maximize compatibility with out-of-tree drivers used to build
      modules, the OpenEmbedded build system also recognizes and uses the
      **KERNEL_SRC** variable, which is identical to
      the **KERNEL_PATH** variable. Both variables are common variables
      used by external Makefiles to point to the kernel source directory.

   **KERNEL_SPLIT_MODULES**
      When inheriting the ref-classes-kernel-module-split class, this
      variable controls whether kernel modules are split into separate packages
      or bundled into a single package.

      For some use cases, a monolithic kernel module package
      **KERNEL_PACKAGE_NAME** that contains all modules built from the
      kernel sources may be preferred to speed up the installation.

      By default, this variable is set to `1`, resulting in one package per
      module. Setting it to any other value will generate a single monolithic
      package containing all kernel modules.

      .. note::

         If **KERNEL_SPLIT_MODULES** is set to 0, it is still possible to
         install all kernel modules at once by adding `kernel-modules` (assuming
         **KERNEL_PACKAGE_NAME** is `kernel-modules`) to **IMAGE_INSTALL**.
         The way it works is that a placeholder "kernel-modules" package will be
         created and will depend on every other individual kernel module packages.

   **KERNEL_SRC**
      The location of the kernel sources. This variable is set to the value
      of the **STAGING_KERNEL_DIR** within the ref-classes-module
      class. For information on how this variable is used, see the
      "kernel-dev/common:incorporating out-of-tree modules"
      section in the Yocto Project Linux Kernel Development Manual.

      To help maximize compatibility with out-of-tree drivers used to build
      modules, the OpenEmbedded build system also recognizes and uses the
      **KERNEL_PATH** variable, which is identical
      to the **KERNEL_SRC** variable. Both variables are common variables
      used by external Makefiles to point to the kernel source directory.

   **KERNEL_STRIP**
      Allows to specific which `strip` command to use to strip the kernel
      binary, typically either GNU binutils `strip` or `llvm-strip`.

   **KERNEL_VERSION**
      Specifies the version of the kernel as extracted from `version.h`
      or `utsrelease.h` within the kernel sources. Effects of setting
      this variable do not take effect until the kernel has been
      configured. Consequently, attempting to refer to this variable in
      contexts prior to configuration will not work.

   **KERNELDEPMODDEPEND**
      Specifies whether the data referenced through
      **PKGDATA_DIR** is needed or not.
      **KERNELDEPMODDEPEND** does not control whether or not that data
      exists, but simply whether or not it is used. If you do not need to
      use the data, set the **KERNELDEPMODDEPEND** variable in your
      **Initramfs** recipe. Setting the variable there when the data is not
      needed avoids a potential dependency loop.

   **KFEATURE_DESCRIPTION**
      Provides a short description of a configuration fragment. You use
      this variable in the `.scc` file that describes a configuration
      fragment file. Here is the variable used in a file named `smp.scc`
      to describe SMP being enabled::

          define KFEATURE_DESCRIPTION "Enable SMP"

   **KMACHINE**
      The machine as known by the kernel. Sometimes the machine name used
      by the kernel does not match the machine name used by the
      OpenEmbedded build system. For example, the machine name that the
      OpenEmbedded build system understands as `core2-32-intel-common`
      goes by a different name in the Linux Yocto kernel. The kernel
      understands that machine as `intel-core2-32`. For cases like these,
      the **KMACHINE** variable maps the kernel machine name to the
      OpenEmbedded build system machine name.

      These mappings between different names occur in the Yocto Linux
      Kernel's `meta` branch. As an example take a look in the
      `common/recipes-kernel/linux/linux-yocto_3.19.bbappend` file::

         LINUX_VERSION:core2-32-intel-common = "3.19.0"
         COMPATIBLE_MACHINE:core2-32-intel-common = "${MACHINE}"
         SRCREV_meta:core2-32-intel-common = "8897ef68b30e7426bc1d39895e71fb155d694974"
         SRCREV_machine:core2-32-intel-common = "43b9eced9ba8a57add36af07736344dcc383f711"
         KMACHINE:core2-32-intel-common = "intel-core2-32"
         KBRANCH:core2-32-intel-common = "standard/base"
         KERNEL_FEATURES:append:core2-32-intel-common = " ${KERNEL_FEATURES_INTEL_COMMON}"

      The **KMACHINE** statement says
      that the kernel understands the machine name as "intel-core2-32".
      However, the OpenEmbedded build system understands the machine as
      "core2-32-intel-common".

   **KMETA_AUDIT**
      When inheriting the ref-classes-kernel-yocto class, the
      **KMETA_AUDIT** variable enables or disables auditing of Linux Kernel
      configuration. The default value for this variable is "yes", and auditing
      is enabled. Set this variable to an empty string to disable auditing.

   **KMETA_AUDIT_WERROR**
      When inheriting the ref-classes-kernel-yocto class and when the
      **KMETA_AUDIT** and the **KMETA_AUDIT_WERROR** variables are set
      to non-empty strings, produce an error if Linux kernel configuration
      problems were detected, instead of warnings only. For more details on the
      Linux Kernel configuration auditing feature, see **KCONF_AUDIT_LEVEL**
      and **KCONF_BSP_AUDIT_LEVEL**.

   **KMETA_CONFIG_FEATURES**
      The **KMETA_CONFIG_FEATURES** variable defines features enabled for the
      ref-classes-kernel-yocto class. The following list of features are
      supported:

      -  `prefer-modules`: prefer a kernel configuration to be set as `m`
         instead of the default value `y` if the kernel configuration was
         defined as follows::

            CONFIG_FOO=y # OVERRIDE:$MODULE_OR_Y

      The default value of this variable is an empty string.

   **KTYPE**
      Defines the kernel type to be used in assembling the configuration.
      The linux-yocto recipes define "standard", "tiny", and "preempt-rt"
      kernel types. See the "kernel-dev/advanced:kernel types"
      section in the
      Yocto Project Linux Kernel Development Manual for more information on
      kernel types.

      You define the **KTYPE** variable in the
      kernel-dev/advanced:bsp descriptions. The
      value you use must match the value used for the
      **LINUX_KERNEL_TYPE** value used by the
      kernel recipe.

   **LABELS**
      Provides a list of targets for automatic configuration.

      See the ref-classes-grub-efi class for more
      information on how this variable is used.

   **LAYERDEPENDS**
      Lists the layers, separated by spaces, on which this layer depends.
      Optionally, you can specify a specific layer version for a dependency
      by adding it to the end of the layer name. Here is an example::

         LAYERDEPENDS_mylayer = "anotherlayer (=3)"

      In this previous example,
      version 3 of "anotherlayer" is compared against
      **LAYERVERSION**\ `_anotherlayer`.

      An error is produced if any dependency is missing or the version
      numbers (if specified) do not match exactly. This variable is used in
      the `conf/layer.conf` file and must be suffixed with the name of
      the specific layer (e.g. `LAYERDEPENDS_mylayer`).

   **LAYERDIR**
      When used inside the `layer.conf` configuration file, this variable
      provides the path of the current layer. This variable is not
      available outside of `layer.conf` and references are expanded
      immediately when parsing of the file completes.

   **LAYERDIR_RE**
      See **bitbake:LAYERDIR_RE** in the BitBake manual.

   **LAYERRECOMMENDS**
      Lists the layers, separated by spaces, recommended for use with this
      layer.

      Optionally, you can specify a specific layer version for a
      recommendation by adding the version to the end of the layer name.
      Here is an example::

         LAYERRECOMMENDS_mylayer = "anotherlayer (=3)"

      In this previous example, version 3 of "anotherlayer" is compared
      against `LAYERVERSION_anotherlayer`.

      This variable is used in the `conf/layer.conf` file and must be
      suffixed with the name of the specific layer (e.g.
      `LAYERRECOMMENDS_mylayer`).

   **LAYERSERIES_COMPAT**
      See **bitbake:LAYERSERIES_COMPAT** in the BitBake manual.

   **LAYERVERSION**
      Optionally specifies the version of a layer as a single number. You
      can use this within **LAYERDEPENDS** for
      another layer in order to depend on a specific version of the layer.
      This variable is used in the `conf/layer.conf` file and must be
      suffixed with the name of the specific layer (e.g.
      `LAYERVERSION_mylayer`).

   **LD**
      The minimal command and arguments used to run the :manpage:`linker
      <ld(1)>`.

   **LDFLAGS**
      Specifies the flags to pass to the linker. This variable is exported
      to an environment variable and thus made visible to the software
      being built during the compilation step.

      Default initialization for **LDFLAGS** varies depending on what is
      being built:

      -  **TARGET_LDFLAGS** when building for the
         target

      -  **BUILD_LDFLAGS** when building for the
         build host (i.e. `-native`)

      -  **BUILDSDK_LDFLAGS** when building for
         an SDK (i.e. `nativesdk-`)

   **LEAD_SONAME**
      Specifies the lead (or primary) compiled library file (i.e. `.so`)
      that the ref-classes-debian class applies its
      naming policy to given a recipe that packages multiple libraries.

      This variable works in conjunction with the ref-classes-debian
      class.

   **LIC_FILES_CHKSUM**
      Checksums of the license text in the recipe source code.

      This variable tracks changes in license text of the source code
      files. If the license text is changed, it will trigger a build
      failure, which gives the developer an opportunity to review any
      license change.

      This variable must be defined for all recipes (unless
      **LICENSE** is set to "CLOSED").

      For more information, see the "dev-manual/licenses:tracking license changes"
      section in the Yocto Project Development Tasks Manual.

   **LICENSE**
      The list of source licenses for the recipe. Follow these rules:

      -  Do not use spaces within individual license names.

      -  Separate license names using \| (pipe) when there is a choice
         between licenses.

      -  Separate license names using & (ampersand) when there are
         multiple licenses for different parts of the source.

      -  You can use spaces between license names.

      -  For standard licenses, use the names of the files in
         `meta/files/common-licenses/` or the
         **SPDXLICENSEMAP** flag names defined in
         `meta/conf/licenses.conf`.

      Here are some examples::

         LICENSE = "LGPL-2.1-only | GPL-3.0-only"
         LICENSE = "MPL-1.0 & LGPL-2.1-only"
         LICENSE = "GPL-2.0-or-later"

      The first example is from the
      recipes for Qt, which the user may choose to distribute under either
      the LGPL version 2.1 or GPL version 3. The second example is from
      Cairo where two licenses cover different parts of the source code.
      The final example is from `sysstat`, which presents a single
      license.

      You can also specify licenses on a per-package basis to handle
      situations where components of the output have different licenses.
      For example, a piece of software whose code is licensed under GPLv2
      but has accompanying documentation licensed under the GNU Free
      Documentation License 1.2 could be specified as follows::

         LICENSE = "GFDL-1.2 & GPL-2.0-only"
         LICENSE:${PN} = "GPL-2.0.only"
         LICENSE:${PN}-doc = "GFDL-1.2"

   **LICENSE_CREATE_PACKAGE**
      Setting **LICENSE_CREATE_PACKAGE** to "1" causes the OpenEmbedded
      build system to create an extra package (i.e.
      `${`\ **PN**\ `}-lic`) for each recipe and to add
      those packages to the
      **RRECOMMENDS**\ `:${PN}`.

      The `${PN}-lic` package installs a directory in
      `/usr/share/licenses` named `${PN}`, which is the recipe's base
      name, and installs files in that directory that contain license and
      copyright information (i.e. copies of the appropriate license files
      from `meta/common-licenses` that match the licenses specified in
      the **LICENSE** variable of the recipe metadata
      and copies of files marked in
      **LIC_FILES_CHKSUM** as containing
      license text).

      For related information on providing license text, see the
      **COPY_LIC_DIRS** variable, the
      **COPY_LIC_MANIFEST** variable, and the
      "dev-manual/licenses:providing license text"
      section in the Yocto Project Development Tasks Manual.

   **LICENSE_FLAGS**
      Specifies additional flags for a recipe you must allow through
      **LICENSE_FLAGS_ACCEPTED** in
      order for the recipe to be built. When providing multiple flags,
      separate them with spaces.

      This value is independent of **LICENSE** and is
      typically used to mark recipes that might require additional licenses
      in order to be used in a commercial product. For more information,
      see the
      "dev-manual/licenses:enabling commercially licensed recipes"
      section in the Yocto Project Development Tasks Manual.

   **LICENSE_FLAGS_ACCEPTED**
      Lists license flags that when specified in
      **LICENSE_FLAGS** within a recipe should not
      prevent that recipe from being built.  For more information, see the
      "dev-manual/licenses:enabling commercially licensed recipes"
      section in the Yocto Project Development Tasks Manual.

   **LICENSE_FLAGS_DETAILS**
      Adds details about a flag in **LICENSE_FLAGS**. This way,
      if such a flag is not accepted through **LICENSE_FLAGS_ACCEPTED**,
      the error message will be more informative, containing the specified
      extra details.

      For example, a recipe with an EULA may set::

         LICENSE_FLAGS = "FooBar-EULA"
         LICENSE_FLAGS_DETAILS[FooBar-EULA] = "For further details, see https://example.com/eula."

      If `Foobar-EULA` isn't in **LICENSE_FLAGS_ACCEPTED**, the
      error message is more useful::

        Has a restricted license 'FooBar-EULA' which is not listed in your LICENSE_FLAGS_ACCEPTED.
        For further details, see https://example.com/eula.

   **LICENSE_PATH**
      Path to additional licenses used during the build. By default, the
      OpenEmbedded build system uses **COMMON_LICENSE_DIR** to define the
      directory that holds common license text used during the build. The
      **LICENSE_PATH** variable allows you to extend that location to other
      areas that have additional licenses::

         LICENSE_PATH += "path-to-additional-common-licenses"

   **LINUX_KERNEL_TYPE**
      Defines the kernel type to be used in assembling the configuration.
      The linux-yocto recipes define "standard", "tiny", and "preempt-rt"
      kernel types. See the "kernel-dev/advanced:kernel types"
      section in the
      Yocto Project Linux Kernel Development Manual for more information on
      kernel types.

      If you do not specify a **LINUX_KERNEL_TYPE**, it defaults to
      "standard". Together with **KMACHINE**, the
      **LINUX_KERNEL_TYPE** variable defines the search arguments used by
      the kernel tools to find the appropriate description within the
      kernel **Metadata** with which to build out the sources
      and configuration.

   **LINUX_VERSION**
      The Linux version from `kernel.org` on which the Linux kernel image
      being built using the OpenEmbedded build system is based. You define
      this variable in the kernel recipe. For example, the
      `linux-yocto-3.4.bb` kernel recipe found in
      `meta/recipes-kernel/linux` defines the variables as follows::

         LINUX_VERSION ?= "3.4.24"

      The **LINUX_VERSION** variable is used to define **PV**
      for the recipe::

         PV = "${LINUX_VERSION}+git"

   **LINUX_VERSION_EXTENSION**
      A string extension compiled into the version string of the Linux
      kernel built with the OpenEmbedded build system. You define this
      variable in the kernel recipe. For example, the linux-yocto kernel
      recipes all define the variable as follows::

         LINUX_VERSION_EXTENSION ?= "-yocto-${LINUX_KERNEL_TYPE}"

      Defining this variable essentially sets the Linux kernel
      configuration item `CONFIG_LOCALVERSION`, which is visible through
      the `uname` command. Here is an example that shows the extension
      assuming it was set as previously shown::

         $ uname -r
         3.7.0-rc8-custom

   **LOG_DIR**
      Specifies the directory to which the OpenEmbedded build system writes
      overall log files. The default directory is `${TMPDIR}/log`.

      For the directory containing logs specific to each task, see the
      **T** variable.

   **LTO**
      The **LTO** variable defines the flags specific to the `lto`
      **distro feature <DISTRO_FEATURES>**. The value of this variable is
      appended to the **TARGET_LDFLAGS** variable, adding `Link-Time
      Optimisation <https://wiki.gentoo.org/wiki/LTO>`__ flags to the linker
      in-use.

   **MACHINE**
      Specifies the target device for which the image is built. You define
      **MACHINE** in the `local.conf` file found in the
      **Build Directory**. By default, **MACHINE** is set to
      "qemux86", which is an x86-based architecture machine to be emulated
      using QEMU::

         MACHINE ?= "qemux86"

      The variable corresponds to a machine configuration file of the same
      name, through which machine-specific configurations are set. Thus,
      when **MACHINE** is set to "qemux86", the corresponding
      `qemux86.conf` machine configuration file can be found in
      **OpenEmbedded-Core (OE-Core)** in
      `meta/conf/machine`.

      The list of machines supported by the Yocto Project as shipped
      include the following::

         MACHINE ?= "qemuarm"
         MACHINE ?= "qemuarm64"
         MACHINE ?= "qemumips"
         MACHINE ?= "qemumips64"
         MACHINE ?= "qemuppc"
         MACHINE ?= "qemux86"
         MACHINE ?= "qemux86-64"
         MACHINE ?= "genericx86"
         MACHINE ?= "genericx86-64"
         MACHINE ?= "beaglebone"

      The last five are Yocto Project reference hardware
      boards, which are provided in the `meta-yocto-bsp` layer.

      .. note::

         Adding additional Board Support Package (BSP) layers to your
         configuration adds new possible settings for **MACHINE**.

   **MACHINE_ARCH**
      Specifies the name of the machine-specific architecture. This
      variable is set automatically from **MACHINE** or
      **TUNE_PKGARCH**. You should not hand-edit
      the **MACHINE_ARCH** variable.

   **MACHINE_ESSENTIAL_EXTRA_RDEPENDS**
      A list of required machine-specific packages to install as part of
      the image being built. The build process depends on these packages
      being present. Furthermore, because this is a "machine-essential"
      variable, the list of packages are essential for the machine to boot.
      The impact of this variable affects images based on
      `packagegroup-core-boot`, including the `core-image-minimal`
      image.

      This variable is similar to the
      **MACHINE_ESSENTIAL_EXTRA_RRECOMMENDS** variable with the exception
      that the image being built has a build dependency on the variable's
      list of packages. In other words, the image will not build if a file
      in this list is not found.

      As an example, suppose the machine for which you are building
      requires `example-init` to be run during boot to initialize the
      hardware. In this case, you would use the following in the machine's
      `.conf` configuration file::

         MACHINE_ESSENTIAL_EXTRA_RDEPENDS += "example-init"

   **MACHINE_ESSENTIAL_EXTRA_RRECOMMENDS**
      A list of recommended machine-specific packages to install as part of
      the image being built. The build process does not depend on these
      packages being present. However, because this is a
      "machine-essential" variable, the list of packages are essential for
      the machine to boot. The impact of this variable affects images based
      on `packagegroup-core-boot`, including the `core-image-minimal`
      image.

      This variable is similar to the **MACHINE_ESSENTIAL_EXTRA_RDEPENDS**
      variable with the exception that the image being built does not have
      a build dependency on the variable's list of packages. In other
      words, the image will still build if a package in this list is not
      found. Typically, this variable is used to handle essential kernel
      modules, whose functionality may be selected to be built into the
      kernel rather than as a module, in which case a package will not be
      produced.

      Consider an example where you have a custom kernel where a specific
      touchscreen driver is required for the machine to be usable. However,
      the driver can be built as a module or into the kernel depending on
      the kernel configuration. If the driver is built as a module, you
      want it to be installed. But, when the driver is built into the
      kernel, you still want the build to succeed. This variable sets up a
      "recommends" relationship so that in the latter case, the build will
      not fail due to the missing package. To accomplish this, assuming the
      package for the module was called `kernel-module-ab123`, you would
      use the following in the machine's `.conf` configuration file::

         MACHINE_ESSENTIAL_EXTRA_RRECOMMENDS += "kernel-module-ab123"

      .. note::

         In this example, the `kernel-module-ab123` recipe needs to
         explicitly set its **PACKAGES** variable to ensure that BitBake
         does not use the kernel recipe's **PACKAGES_DYNAMIC** variable to
         satisfy the dependency.

      Some examples of these machine essentials are flash, screen,
      keyboard, mouse, or touchscreen drivers (depending on the machine).

   **MACHINE_EXTRA_RDEPENDS**
      A list of machine-specific packages to install as part of the image
      being built that are not essential for the machine to boot. However,
      the build process for more fully-featured images depends on the
      packages being present.

      This variable affects all images based on `packagegroup-base`,
      which does not include the `core-image-minimal` or
      `core-image-full-cmdline` images.

      The variable is similar to the **MACHINE_EXTRA_RRECOMMENDS** variable
      with the exception that the image being built has a build dependency
      on the variable's list of packages. In other words, the image will
      not build if a file in this list is not found.

      An example is a machine that has WiFi capability but is not essential
      for the machine to boot the image. However, if you are building a
      more fully-featured image, you want to enable the WiFi. The package
      containing the firmware for the WiFi hardware is always expected to
      exist, so it is acceptable for the build process to depend upon
      finding the package. In this case, assuming the package for the
      firmware was called `wifidriver-firmware`, you would use the
      following in the `.conf` file for the machine::

         MACHINE_EXTRA_RDEPENDS += "wifidriver-firmware"

   **MACHINE_EXTRA_RRECOMMENDS**
      A list of machine-specific packages to install as part of the image
      being built that are not essential for booting the machine. The image
      being built has no build dependency on this list of packages.

      This variable affects only images based on `packagegroup-base`,
      which does not include the `core-image-minimal` or
      `core-image-full-cmdline` images.

      This variable is similar to the **MACHINE_EXTRA_RDEPENDS** variable
      with the exception that the image being built does not have a build
      dependency on the variable's list of packages. In other words, the
      image will build if a file in this list is not found.

      An example is a machine that has WiFi capability but is not essential
      For the machine to boot the image. However, if you are building a
      more fully-featured image, you want to enable WiFi. In this case, the
      package containing the WiFi kernel module will not be produced if the
      WiFi driver is built into the kernel, in which case you still want
      the build to succeed instead of failing as a result of the package
      not being found. To accomplish this, assuming the package for the
      module was called `kernel-module-examplewifi`, you would use the
      following in the `.conf` file for the machine::

         MACHINE_EXTRA_RRECOMMENDS += "kernel-module-examplewifi"

   **MACHINE_FEATURES**
      Specifies the list of hardware features the
      **MACHINE** is capable of supporting. For related
      information on enabling features, see the
      **DISTRO_FEATURES**,
      **COMBINED_FEATURES**, and
      **IMAGE_FEATURES** variables.

      For a list of hardware features supported by the Yocto Project as
      shipped, see the "ref-features-machine" section.

   **MACHINE_FEATURES_BACKFILL**
      A list of space-separated features to be added to
      **MACHINE_FEATURES** if not also present in
      **MACHINE_FEATURES_BACKFILL_CONSIDERED**.

      This variable is set in the `meta/conf/bitbake.conf` file. It is not
      intended to be user-configurable. It is best to just reference the
      variable to see which machine features are being
      backfilled  for all machine configurations.

   **MACHINE_FEATURES_BACKFILL_CONSIDERED**
      A list of space-separated features from **MACHINE_FEATURES_BACKFILL**
      that should not be backfilled  (i.e. added
      to **MACHINE_FEATURES**) during the build.

      This corresponds to an opt-out mechanism. When new default machine
      features are introduced, machine definition maintainers can review
      (`consider`) them and decide to exclude them from the
      backfilled  features. Therefore, the
      combination of **MACHINE_FEATURES_BACKFILL** and
      **MACHINE_FEATURES_BACKFILL_CONSIDERED** makes it possible to
      add new default features without breaking existing machine definitions.

   **MACHINEOVERRIDES**
      A colon-separated list of overrides that apply to the current
      machine. By default, this list includes the value of
      **MACHINE**.

      You can extend **MACHINEOVERRIDES** to add extra overrides that
      should apply to a machine. For example, all machines emulated in QEMU
      (e.g. `qemuarm`, `qemux86`, and so forth) include a file named
      `meta/conf/machine/include/qemu.inc` that prepends the following
      override to **MACHINEOVERRIDES**::

         MACHINEOVERRIDES =. "qemuall:"

      This
      override allows variables to be overridden for all machines emulated
      in QEMU, like in the following example from the `connman-conf`
      recipe::

         SRC_URI:append:qemuall = " file://wired.config \
             file://wired-setup \
             "

      The underlying mechanism behind
      **MACHINEOVERRIDES** is simply that it is included in the default
      value of **OVERRIDES**.

   **MAINTAINER**
      The email address of the distribution maintainer.

   **MESON_BUILDTYPE**
      Value of the Meson `--buildtype` argument used by the
      ref-classes-meson class. It defaults to `debug` if
      **DEBUG_BUILD** is set to "1", and `plain` otherwise.

      See `Meson build options <https://mesonbuild.com/Builtin-options.html>`__
      for the values you could set in a recipe. Values such as `plain`,
      `debug`, `debugoptimized`, `release` and `minsize` allow
      you to specify the inclusion of debugging symbols and the compiler
      optimizations (none, performance or size).

   **MESON_INSTALL_TAGS**
      A variable for the ref-classes-meson class, allowing to specify
      install tags (`--tags` argument of the `meson install` command).

   **MESON_TARGET**
      A variable for the ref-classes-meson class, allowing to choose
      a Meson target to build in ref-tasks-compile.  Otherwise, the
      default targets are built.

   **METADATA_BRANCH**
      The branch currently checked out for the OpenEmbedded-Core layer (path
      determined by **COREBASE**).

   **METADATA_REVISION**
      The revision currently checked out for the OpenEmbedded-Core layer (path
      determined by **COREBASE**).

   **MIME_XDG_PACKAGES**
      The current implementation of the ref-classes-mime-xdg
      class cannot detect `.desktop` files installed through absolute
      symbolic links. Use this setting to make the class create post-install
      and post-remove scripts for these packages anyway, to invoke the
      `update-destop-database` command.

   **MIRRORS**
      Specifies additional paths from which the OpenEmbedded build system
      gets source code. When the build system searches for source code, it
      first tries the local download directory. If that location fails, the
      build system tries locations defined by
      **PREMIRRORS**, the upstream source, and then
      locations specified by **MIRRORS** in that order.

      The default value for **MIRRORS** is defined in the
      `meta/classes-global/mirrors.bbclass` file in the core metadata layer.

   **MLPREFIX**
      Specifies a prefix has been added to **PN** to create a
      special version of a recipe or package (i.e. a Multilib version). The
      variable is used in places where the prefix needs to be added to or
      removed from a name (e.g. the **BPN** variable).
      **MLPREFIX** gets set when a prefix has been added to **PN**.

      .. note::

         The "ML" in **MLPREFIX** stands for "MultiLib". This representation
         is historical and comes from a time when "ref-classes-nativesdk"
         was a suffix rather than a prefix on the recipe name. When
         "ref-classes-nativesdk" was turned into a prefix, it made sense
         to set **MLPREFIX** for it as well.

      To help understand when **MLPREFIX** might be needed, consider when
      **BBCLASSEXTEND** is used to provide a ref-classes-nativesdk
      version of a recipe in addition to the target version. If that recipe
      declares build-time dependencies on tasks in other recipes by using
      **DEPENDS**, then a dependency on "foo" will automatically get
      rewritten to a dependency on "nativesdk-foo". However, dependencies like
      the following will not get rewritten automatically::

         do_foo[depends] += "recipe:do_foo"

      If you want such a dependency to also get transformed, you can do the
      following::

         do_foo[depends] += "${MLPREFIX}recipe:do_foo"

   **module_autoload**
      This variable has been replaced by the **KERNEL_MODULE_AUTOLOAD**
      variable. You should replace all occurrences of **module_autoload**
      with additions to **KERNEL_MODULE_AUTOLOAD**, for example::

         module_autoload_rfcomm = "rfcomm"

      should now be replaced with::

         KERNEL_MODULE_AUTOLOAD += "rfcomm"

      See the **KERNEL_MODULE_AUTOLOAD** variable for more information.

   **module_conf**
      Specifies `modprobe.d <https://linux.die.net/man/5/modprobe.d>`__
      syntax lines for inclusion in the `/etc/modprobe.d/modname.conf`
      file.

      You can use this variable anywhere that it can be recognized by the
      kernel recipe or out-of-tree kernel module recipe (e.g. a machine
      configuration file, a distribution configuration file, an append file
      for the recipe, or the recipe itself). If you use this variable, you
      must also be sure to list the module name in the
      **KERNEL_MODULE_PROBECONF**
      variable.

      Here is the general syntax::

         module_conf_module_name = "modprobe.d-syntax"

      You must use the kernel module name override.

      Run `man modprobe.d` in the shell to find out more information on
      the exact syntax you want to provide with **module_conf**.

      Including **module_conf** causes the OpenEmbedded build system to
      populate the `/etc/modprobe.d/modname.conf` file with
      `modprobe.d` syntax lines. Here is an example that adds the options
      `arg1` and `arg2` to a module named `mymodule`::

         module_conf_mymodule = "options mymodule arg1=val1 arg2=val2"

      For information on how to specify kernel modules to auto-load on
      boot, see the **KERNEL_MODULE_AUTOLOAD** variable.

   **MODULE_TARBALL_DEPLOY**
      Controls creation of the `modules-*.tgz` file. Set this variable to
      "0" to disable creation of this file, which contains all of the
      kernel modules resulting from a kernel build.

   **MODULE_TARBALL_LINK_NAME**
      The link name of the kernel module tarball. This variable is set in
      the `meta/classes-recipe/kernel-artifact-names.bbclass` file as follows::

         MODULE_TARBALL_LINK_NAME ?= "${KERNEL_ARTIFACT_LINK_NAME}"

      The value
      of the `KERNEL_ARTIFACT_LINK_NAME` variable, which is set in the
      same file, has the following value::

         KERNEL_ARTIFACT_LINK_NAME ?= "${MACHINE}"

      See the **MACHINE** variable for additional information.

   **MODULE_TARBALL_NAME**
      The base name of the kernel module tarball. This variable is set in
      the `meta/classes-recipe/kernel-artifact-names.bbclass` file as follows::

         MODULE_TARBALL_NAME ?= "${KERNEL_ARTIFACT_NAME}"

      See **KERNEL_ARTIFACT_NAME** for additional information.

   **MOUNT_BASE**
      On non-systemd systems (where `udev-extraconf` is being used),
      specifies the base directory for auto-mounting filesystems. The
      default value is "/run/media".

   **MOUNT_GROUP**
      On non-systemd systems (where `udev-extraconf` is being used),
      specifies the mount group for auto-mounting filesystems. The
      default value is "disk".

   **MULTIMACH_TARGET_SYS**
      Uniquely identifies the type of the target system for which packages
      are being built. This variable allows output for different types of
      target systems to be put into different subdirectories of the same
      output directory.

      The default value of this variable is::

         ${PACKAGE_ARCH}${TARGET_VENDOR}-${TARGET_OS}

      Some classes (e.g.  ref-classes-cross-canadian) modify the
      **MULTIMACH_TARGET_SYS** value.

      See the **STAMP** variable for an example. See the
      **STAGING_DIR_TARGET** variable for more information.

   **NATIVELSBSTRING**
      A string identifying the host distribution. Strings consist of the
      host distributor ID followed by the release, as reported by the
      `lsb_release` tool or as read from `/etc/lsb-release`. For
      example, when running a build on Ubuntu 12.10, the value is
      "Ubuntu-12.10". If this information is unable to be determined, the
      value resolves to "Unknown".

      This variable is used by default to isolate native shared state
      packages for different distributions (e.g. to avoid problems with
      `glibc` version incompatibilities). Additionally, the variable is
      checked against
      **SANITY_TESTED_DISTROS** if that
      variable is set.

   **NM**
      The minimal command and arguments to run `nm <nm(1)>`.

   **NO_GENERIC_LICENSE**
      Avoids QA errors when you use a non-common, non-CLOSED license in a
      recipe. There are packages, such as the linux-firmware package, with many
      licenses that are not in any way common. Also, new licenses are added
      occasionally to avoid introducing a lot of common license files,
      which are only applicable to a specific package.
      **NO_GENERIC_LICENSE** is used to allow copying a license that does
      not exist in common licenses.

      The following example shows how to add **NO_GENERIC_LICENSE** to a
      recipe::

         NO_GENERIC_LICENSE[license_name] = "license_file_in_fetched_source"

      Here is an example that
      uses the `LICENSE.Abilis.txt` file as the license from the fetched
      source::

         NO_GENERIC_LICENSE[Firmware-Abilis] = "LICENSE.Abilis.txt"

   **NO_RECOMMENDATIONS**
      Prevents installation of all "recommended-only" packages.
      Recommended-only packages are packages installed only through the
      **RRECOMMENDS** variable). Setting the
      **NO_RECOMMENDATIONS** variable to "1" turns this feature on::

         NO_RECOMMENDATIONS = "1"

      You can set this variable globally in your `local.conf` file or you
      can attach it to a specific image recipe by using the recipe name
      override::

         NO_RECOMMENDATIONS:pn-target_image = "1"

      It is important to realize that if you choose to not install packages
      using this variable and some other packages are dependent on them
      (i.e. listed in a recipe's **RDEPENDS**
      variable), the OpenEmbedded build system ignores your request and
      will install the packages to avoid dependency errors.

      .. note::

         Some recommended packages might be required for certain system
         functionality, such as kernel modules. It is up to you to add
         packages with the **IMAGE_INSTALL** variable.

      This variable is only supported when using the IPK and RPM
      packaging backends. DEB is not supported.

      See the **BAD_RECOMMENDATIONS** and
      the **PACKAGE_EXCLUDE** variables for
      related information.

   **NOAUTOPACKAGEDEBUG**
      Disables auto package from splitting `.debug` files. If a recipe
      requires `FILES:${PN}-dbg` to be set manually, the
      **NOAUTOPACKAGEDEBUG** can be defined allowing you to define the
      content of the debug package. For example::

         NOAUTOPACKAGEDEBUG = "1"
         FILES:${PN}-dev = "${includedir}/${QT_DIR_NAME}/Qt/*"
         FILES:${PN}-dbg = "/usr/src/debug/"
         FILES:${QT_BASE_NAME}-demos-doc = "${docdir}/${QT_DIR_NAME}/qch/qt.qch"

   **NON_MULTILIB_RECIPES**
      A list of recipes that should not be built for multilib. OE-Core's
      `multilib.conf` file defines a reasonable starting point for this
      list with::

         NON_MULTILIB_RECIPES = "grub grub-efi make-mod-scripts ovmf u-boot"

   **NVD_DB_VERSION**
      The **NVD_DB_VERSION** variable allows choosing the CVE feed when
      using the ref-classes-cve-check class. It can be one of:

      -  `FKIE` (default): the `FKIE-CAD <https://github.com/fkie-cad/nvd-json-data-feeds>`__
         feed reconstruction
      -  `NVD2`: the NVD feed with API version 2
      -  `NVD1`: the NVD JSON feed (deprecated)

      In case of a malformed feed name, the `NVD2` feed is selected and an
      error is printed.

   **NVDCVE_API_KEY**
      The NVD API key used to retrieve data from the CVE database when
      using ref-classes-cve-check.

      By default, no API key is used, which results in larger delays between API
      requests and limits the number of queries to the public rate limits posted
      at the `NVD developer's page <https://nvd.nist.gov/developers/start-here>`__.

      NVD API keys can be requested through the
      `Request an API Key <https://nvd.nist.gov/developers/request-an-api-key>`__
      page. You can set this variable to the NVD API key in your `local.conf` file.
      Example::

          NVDCVE_API_KEY = "fe753&7a2-1427-347d-23ff-b2e2b7ca5f3"

   **OBJCOPY**
      The minimal command and arguments to run `objcopy <objcopy(1)>`.

   **OBJDUMP**
      The minimal command and arguments to run `objdump <objdump(1)>`.

   **OE_BINCONFIG_EXTRA_MANGLE**
      When inheriting the ref-classes-binconfig class,
      this variable specifies additional arguments passed to the "sed"
      command. The sed command alters any paths in configuration scripts
      that have been set up during compilation. Inheriting this class
      results in all paths in these scripts being changed to point into the
      `sysroots/` directory so that all builds that use the script will
      use the correct directories for the cross compiling layout.

      See the `meta/classes-recipe/binconfig.bbclass` in
      **OpenEmbedded-Core (OE-Core)** for details on how this class
      applies these additional sed command arguments.

   **OE_FRAGMENTS**
      The **OE_FRAGMENTS** variable holds the list of :term:`Configuration
      Fragments <Configuration Fragment>` currently enabled for the build. For
      details on how to use fragments, see the /ref-manual/fragments
      section of the Yocto Project Reference Manual.

   **OE_FRAGMENTS_BUILTIN**
      The **OE_FRAGMENTS_BUILTIN** variable holds the list of
      **Built-in Fragments <Built-in Fragment>** available for being set with
      bitbake-config-build .
      For details on how to use fragments, see the /ref-manual/fragments
      section of the Yocto Project Reference Manual.

   **OE_FRAGMENTS_METADATA_VARS**
      The **OE_FRAGMENTS_METADATA_VARS** variable holds the list of
      variables that are required to set in a standard :term:`Configuration
      Fragment` file. In **OpenEmbedded-Core (OE-Core)**, these variables
      are **BB_CONF_FRAGMENT_SUMMARY** and
      **BB_CONF_FRAGMENT_DESCRIPTION**.

   **OE_FRAGMENTS_PREFIX**
      The **OE_FRAGMENTS_PREFIX** variable defines the prefix where
      **BitBake** tries to locate :term:`Configuration Fragments
      <Configuration Fragment>` in **layers <Layer>**. For details on how to
      use fragments, see the /ref-manual/fragments section of the Yocto
      Project Reference Manual.

   **OE_INIT_ENV_SCRIPT**
      The name of the build environment setup script for the purposes of
      setting up the environment within the extensible SDK. The default
      value is "oe-init-build-env".

      If you use a custom script to set up your build environment, set the
      **OE_INIT_ENV_SCRIPT** variable to its name.

   **OE_SHARED_UMASK**
      The **OE_SHARED_UMASK** variable contains the `umask`
      definition for shared files (for example files in **SSTATE_DIR** or
      **DL_DIR**).

   **OE_TERMINAL**
      Controls how the OpenEmbedded build system spawns interactive
      terminals on the host development system (e.g. using the BitBake
      command with the `-c devshell` command-line option). For more
      information, see the "dev-manual/development-shell:using a development shell" section in
      the Yocto Project Development Tasks Manual.

      You can use the following values for the **OE_TERMINAL** variable:

      - auto
      - gnome
      - xfce
      - rxvt
      - screen
      - konsole
      - none

   **OECMAKE_GENERATOR**
      A variable for the ref-classes-cmake class, allowing to choose
      which back-end will be generated by CMake to build an application.

      By default, this variable is set to `Ninja`, which is faster than GNU
      make, but if building is broken with Ninja, a recipe can use this
      variable to use GNU make instead::

         OECMAKE_GENERATOR = "Unix Makefiles"

   **OEQA_REPRODUCIBLE_TEST_LEAF_TARGETS**
      Set build target(s) for build reproducibility testing but activate
      shared state  build
      for most dependencies (i.e. the ones explicitly listed in DEPENDS, which
      may not be all dependencies, c.f. [depends] varflags, PACKAGE_DEPENDS and
      other implementations). See /test-manual/reproducible-builds.

   **OEQA_REPRODUCIBLE_TEST_PACKAGE**
      Set the package manager(s) for build reproducibility testing.
      See reproducible.py 
      and /test-manual/reproducible-builds.

   **OEQA_REPRODUCIBLE_TEST_SSTATE_TARGETS**
      Set build targets which can be rebuilt using shared state 
      when running build reproducibility tests. See /test-manual/reproducible-builds.

   **OEQA_REPRODUCIBLE_TEST_TARGET**
      Set build target for build reproducibility testing. By default
      all available recipes are compiled with "bitbake world", see also **EXCLUDE_FROM_WORLD**
      and /test-manual/reproducible-builds.

   **OEROOT**
      The directory from which the top-level build environment setup script
      is sourced. The Yocto Project provides a top-level build environment
      setup script: structure-core-script. When you run this
      script, the **OEROOT** variable resolves to the directory that
      contains the script.

      For additional information on how this variable is used, see the
      initialization script.

   **OLDEST_KERNEL**
      Declares the oldest version of the Linux kernel that the produced
      binaries must support. This variable is passed into the build of the
      Embedded GNU C Library (`glibc`).

      The default for this variable comes from the
      `meta/conf/bitbake.conf` configuration file. You can override this
      default by setting the variable in a custom distribution
      configuration file.

   **OPENSSH_HOST_KEY_DIR**
      Specifies the directory where OpenSSH host keys are stored. Default
      is `/etc/ssh`.

   **OPENSSH_HOST_KEY_DIR_READONLY_CONFIG**
      Specifies the directory where OpenSSH host keys are stored when the
      device uses a read-only filesystem. Default is `/var/run/ssh`.

   **OPKG_MAKE_INDEX_EXTRA_PARAMS**
      Specifies extra parameters for the `opkg-make-index` command.

   **OPKGBUILDCMD**
      The variable **OPKGBUILDCMD** specifies the command used to build opkg
      packages when using the ref-classes-package_ipk class. It is
      defined in ref-classes-package_ipk as::

          OPKGBUILDCMD ??= 'opkg-build -Z zstd -a "${ZSTD_DEFAULTS}"'

   **OVERLAYFS_ETC_DEVICE**
      When the ref-classes-overlayfs-etc class is
      inherited, specifies the device to be mounted for the read/write
      layer of `/etc`. There is no default, so you must set this if you
      wish to enable ref-classes-overlayfs-etc, for
      example, assuming `/dev/mmcblk0p2` was the desired device::

         OVERLAYFS_ETC_DEVICE = "/dev/mmcblk0p2"

   **OVERLAYFS_ETC_EXPOSE_LOWER**
      When the ref-classes-overlayfs-etc class is
      inherited, if set to "1" then a read-only access to the original
      `/etc` content will be provided as a `lower/` subdirectory of
      **OVERLAYFS_ETC_MOUNT_POINT**. The default value is "0".

   **OVERLAYFS_ETC_FSTYPE**
      When the ref-classes-overlayfs-etc class is
      inherited, specifies the file system type for the read/write
      layer of `/etc`. There is no default, so you must set this if you
      wish to enable ref-classes-overlayfs-etc,
      for example, assuming the file system is ext4::

         OVERLAYFS_ETC_FSTYPE = "ext4"

   **OVERLAYFS_ETC_MOUNT_OPTIONS**
      When the ref-classes-overlayfs-etc class is
      inherited, specifies the mount options for the read-write layer.
      The default value is "defaults".

   **OVERLAYFS_ETC_MOUNT_POINT**
      When the ref-classes-overlayfs-etc class is
      inherited, specifies the parent mount path for the filesystem layers.
      There is no default, so you must set this if you wish to enable
      ref-classes-overlayfs-etc, for example if the desired path is
      "/data"::

         OVERLAYFS_ETC_MOUNT_POINT = "/data"

   **OVERLAYFS_ETC_USE_ORIG_INIT_NAME**
      When the ref-classes-overlayfs-etc class is inherited, controls
      how the generated init will be named. For more information, see the
      ref-classes-overlayfs-etc class documentation. The default value
      is "1".

   **OVERLAYFS_MOUNT_POINT**
      When inheriting the ref-classes-overlayfs class,
      specifies mount point(s) to be used. For example::

         OVERLAYFS_MOUNT_POINT[data] = "/data"

      The assumes you have a `data.mount` systemd unit defined elsewhere in
      your BSP (e.g. in `systemd-machine-units` recipe) and it is installed
      into the image. For more information see ref-classes-overlayfs.

      .. note::

         Although the ref-classes-overlayfs class is
         inherited by individual recipes, **OVERLAYFS_MOUNT_POINT**
         should be set in your machine configuration.

   **OVERLAYFS_QA_SKIP**
      When inheriting the ref-classes-overlayfs class,
      provides the ability to disable QA checks for particular overlayfs
      mounts. For example::

         OVERLAYFS_QA_SKIP[data] = "mount-configured"

      .. note::

         Although the ref-classes-overlayfs class is
         inherited by individual recipes, **OVERLAYFS_QA_SKIP**
         should be set in your machine configuration.

   **OVERLAYFS_WRITABLE_PATHS**
      When inheriting the ref-classes-overlayfs class,
      specifies writable paths used at runtime for the recipe. For
      example::

         OVERLAYFS_WRITABLE_PATHS[data] = "/usr/share/my-custom-application"

   **OVERRIDES**
      A colon-separated list of overrides that currently apply. Overrides
      are a BitBake mechanism that allows variables to be selectively
      overridden at the end of parsing. The set of overrides in
      **OVERRIDES** represents the "state" during building, which includes
      the current recipe being built, the machine for which it is being
      built, and so forth.

      As an example, if the string "an-override" appears as an element in
      the colon-separated list in **OVERRIDES**, then the following
      assignment will override `FOO` with the value "overridden" at the
      end of parsing::

         FOO:an-override = "overridden"

      See the
      "bitbake-user-manual/bitbake-user-manual-metadata:conditional syntax (overrides)"
      section in the BitBake User Manual for more information on the
      overrides mechanism.

      The default value of **OVERRIDES** includes the values of the
      **CLASSOVERRIDE**,
      **MACHINEOVERRIDES**, and
      **DISTROOVERRIDES** variables. Another
      important override included by default is `pn-${PN}`. This override
      allows variables to be set for a single recipe within configuration
      (`.conf`) files. Here is an example::

         FOO:pn-myrecipe = "myrecipe-specific value"

      .. note::

         An easy way to see what overrides apply is to run the command
         `bitbake-getvar -r myrecipe OVERRIDES`. See the
         "dev-manual/debugging:viewing variable values" section in the Yocto
         Project Development Tasks Manual for more information.

   **P**
      The recipe name and version. **P** is comprised of the following::

         ${PN}-${PV}

   **P4DIR**
      See **bitbake:P4DIR** in the BitBake manual.

   **PACKAGE_ADD_METADATA**
      This variable defines additional metadata to add to packages.

      You may find you need to inject additional metadata into packages.
      This variable allows you to do that by setting the injected data as
      the value. Multiple fields can be added by splitting the content with
      the literal separator "\n".

      The suffixes '_IPK', '_DEB', or '_RPM' can be applied to the variable
      to do package type specific settings. It can also be made package
      specific by using the package name as a suffix.

      You can find out more about applying this variable in the
      "dev-manual/packages:adding custom metadata to packages"
      section in the Yocto Project Development Tasks Manual.

   **PACKAGE_ARCH**
      The architecture of the resulting package or packages.

      By default, the value of this variable is set to
      **TUNE_PKGARCH** when building for the
      target, **BUILD_ARCH** when building for the
      build host, and "${SDK_ARCH}-${SDKPKGSUFFIX}" when building for the
      SDK.

      .. note::

         See **SDK_ARCH** for more information.

      However, if your recipe's output packages are built specific to the
      target machine rather than generally for the architecture of the
      machine, you should set **PACKAGE_ARCH** to the value of
      **MACHINE_ARCH** in the recipe as follows::

         PACKAGE_ARCH = "${MACHINE_ARCH}"

   **PACKAGE_ARCHS**
      Specifies a list of architectures compatible with the target machine.
      This variable is set automatically and should not normally be
      hand-edited. Entries are separated using spaces and listed in order
      of priority. The default value for **PACKAGE_ARCHS** is "all any
      noarch ${PACKAGE_EXTRA_ARCHS} ${MACHINE_ARCH}".

   **PACKAGE_BEFORE_PN**
      Enables easily adding packages to **PACKAGES** before `${PN}` so
      that those added packages can pick up files that would normally be
      included in the default package.

   **PACKAGE_CLASSES**
      This variable, which is set in the `local.conf` configuration file
      found in the `conf` folder of the
      **Build Directory**, specifies the package manager the
      OpenEmbedded build system uses when packaging data.

      You can provide one or more of the following arguments for the
      variable::

         PACKAGE_CLASSES ?= "package_rpm package_deb package_ipk"

      The build system uses only the first argument in the list as the
      package manager when creating your image or SDK. However, packages
      will be created using any additional packaging classes you specify.
      For example, if you use the following in your `local.conf` file::

         PACKAGE_CLASSES ?= "package_ipk"

      The OpenEmbedded build system uses
      the IPK package manager to create your image or SDK.

      For information on packaging and build performance effects as a
      result of the package manager in use, see the
      "ref-classes-package" section.

   **PACKAGE_DEBUG_SPLIT_STYLE**
      Determines how to split up and package debug and source information
      when creating debugging packages to be used with the GNU Project
      Debugger (GDB). In general, based on the value of this variable,
      you can combine the source and debug info in a single package,
      you can break out the source into a separate package that can be
      installed independently, or you can choose to not have the source
      packaged at all.

      The possible values of **PACKAGE_DEBUG_SPLIT_STYLE** variable:

      -  "`.debug`": All debugging and source info is placed in a single
         `*-dbg` package; debug symbol files are placed next to the
         binary in a `.debug` directory so that, if a binary is installed
         into `/bin`, the corresponding debug symbol file is installed
         in `/bin/.debug`. Source files are installed in the same `*-dbg`
         package under `/usr/src/debug`.

      -  "`debug-file-directory`": As above, all debugging and source info
         is placed in a single `*-dbg` package; debug symbol files are
         placed entirely under the directory `/usr/lib/debug` and separated
         by the path from where the binary is installed, so that if a binary
         is installed in `/bin`, the corresponding debug symbols are installed
         in `/usr/lib/debug/bin`, and so on. As above, source is installed
         in the same package under `/usr/src/debug`.

      -  "`debug-with-srcpkg`": Debugging info is placed in the standard
         `*-dbg` package as with the `.debug` value, while source is
         placed in a separate `*-src` package, which can be installed
         independently.  This is the default setting for this variable,
         as defined in **OE-Core <OpenEmbedded-Core (OE-Core)>**'s `bitbake.conf` file.

      -  "`debug-without-src`": The same behavior as with the `.debug`
         setting, but no source is packaged at all.

      .. note::

         Much of the above package splitting can be overridden via
         use of the **INHIBIT_PACKAGE_DEBUG_SPLIT** variable.

      You can find out more about debugging using GDB by reading the
      "dev-manual/debugging:debugging with the gnu project debugger (gdb) remotely" section
      in the Yocto Project Development Tasks Manual.

   **PACKAGE_EXCLUDE**
      Lists packages that should not be installed into an image. For
      example::

         PACKAGE_EXCLUDE = "package_name package_name package_name ..."

      You can set this variable globally in your `local.conf` file or you
      can attach it to a specific image recipe by using the recipe name
      override::

         PACKAGE_EXCLUDE:pn-target_image = "package_name"

      If you choose to not install a package using this variable and some
      other package is dependent on it (i.e. listed in a recipe's
      **RDEPENDS** variable), the OpenEmbedded build
      system generates a fatal installation error. Because the build system
      halts the process with a fatal error, you can use the variable with
      an iterative development process to remove specific components from a
      system.

      This variable is supported only when using the IPK and RPM
      packaging backends. DEB is not supported.

      See the **NO_RECOMMENDATIONS** and the
      **BAD_RECOMMENDATIONS** variables for
      related information.

   **PACKAGE_EXCLUDE_COMPLEMENTARY**
      Prevents specific packages from being installed when you are
      installing complementary packages.

      You might find that you want to prevent installing certain packages
      when you are installing complementary packages. For example, if you
      are using **IMAGE_FEATURES** to install
      `dev-pkgs`, you might not want to install all packages from a
      particular multilib. If you find yourself in this situation, you can
      use the **PACKAGE_EXCLUDE_COMPLEMENTARY** variable to specify regular
      expressions to match the packages you want to exclude.

   **PACKAGE_EXTRA_ARCHS**
      Specifies the list of architectures compatible with the device CPU.
      This variable is useful when you build for several different devices
      that use miscellaneous processors such as XScale and ARM926-EJS.

   **PACKAGE_FEED_ARCHS**
      Optionally specifies the package architectures used as part of the
      package feed URIs during the build. When used, the
      **PACKAGE_FEED_ARCHS** variable is appended to the final package feed
      URI, which is constructed using the
      **PACKAGE_FEED_URIS** and
      **PACKAGE_FEED_BASE_PATHS**
      variables.

      .. note::

         You can use the **PACKAGE_FEED_ARCHS**
         variable to allow specific package architectures. If you do
         not need to allow specific architectures, which is a common
         case, you can omit this variable. Omitting the variable results in
         all available architectures for the current machine being included
         into remote package feeds.

      Consider the following example where the **PACKAGE_FEED_URIS**,
      **PACKAGE_FEED_BASE_PATHS**, and **PACKAGE_FEED_ARCHS** variables are
      defined in your `local.conf` file::

         PACKAGE_FEED_URIS = "https://example.com/packagerepos/release \
                              https://example.com/packagerepos/updates"
         PACKAGE_FEED_BASE_PATHS = "rpm rpm-dev"
         PACKAGE_FEED_ARCHS = "all core2-64"

      Given these settings, the resulting package feeds are as follows:

      .. code-block:: none

         https://example.com/packagerepos/release/rpm/all
         https://example.com/packagerepos/release/rpm/core2-64
         https://example.com/packagerepos/release/rpm-dev/all
         https://example.com/packagerepos/release/rpm-dev/core2-64
         https://example.com/packagerepos/updates/rpm/all
         https://example.com/packagerepos/updates/rpm/core2-64
         https://example.com/packagerepos/updates/rpm-dev/all
         https://example.com/packagerepos/updates/rpm-dev/core2-64

   **PACKAGE_FEED_BASE_PATHS**
      Specifies the base path used when constructing package feed URIs. The
      **PACKAGE_FEED_BASE_PATHS** variable makes up the middle portion of a
      package feed URI used by the OpenEmbedded build system. The base path
      lies between the **PACKAGE_FEED_URIS**
      and **PACKAGE_FEED_ARCHS** variables.

      Consider the following example where the **PACKAGE_FEED_URIS**,
      **PACKAGE_FEED_BASE_PATHS**, and **PACKAGE_FEED_ARCHS** variables are
      defined in your `local.conf` file::

         PACKAGE_FEED_URIS = "https://example.com/packagerepos/release \
                              https://example.com/packagerepos/updates"
         PACKAGE_FEED_BASE_PATHS = "rpm rpm-dev"
         PACKAGE_FEED_ARCHS = "all core2-64"

      Given these settings, the resulting package feeds are as follows:

      .. code-block:: none

         https://example.com/packagerepos/release/rpm/all
         https://example.com/packagerepos/release/rpm/core2-64
         https://example.com/packagerepos/release/rpm-dev/all
         https://example.com/packagerepos/release/rpm-dev/core2-64
         https://example.com/packagerepos/updates/rpm/all
         https://example.com/packagerepos/updates/rpm/core2-64
         https://example.com/packagerepos/updates/rpm-dev/all
         https://example.com/packagerepos/updates/rpm-dev/core2-64

   **PACKAGE_FEED_URIS**
      Specifies the front portion of the package feed URI used by the
      OpenEmbedded build system. Each final package feed URI is comprised
      of **PACKAGE_FEED_URIS**,
      **PACKAGE_FEED_BASE_PATHS**, and
      **PACKAGE_FEED_ARCHS** variables.

      Consider the following example where the **PACKAGE_FEED_URIS**,
      **PACKAGE_FEED_BASE_PATHS**, and **PACKAGE_FEED_ARCHS** variables are
      defined in your `local.conf` file::

         PACKAGE_FEED_URIS = "https://example.com/packagerepos/release \
                              https://example.com/packagerepos/updates"
         PACKAGE_FEED_BASE_PATHS = "rpm rpm-dev"
         PACKAGE_FEED_ARCHS = "all core2-64"

      Given these settings, the resulting package feeds are as follows:

      .. code-block:: none

         https://example.com/packagerepos/release/rpm/all
         https://example.com/packagerepos/release/rpm/core2-64
         https://example.com/packagerepos/release/rpm-dev/all
         https://example.com/packagerepos/release/rpm-dev/core2-64
         https://example.com/packagerepos/updates/rpm/all
         https://example.com/packagerepos/updates/rpm/core2-64
         https://example.com/packagerepos/updates/rpm-dev/all
         https://example.com/packagerepos/updates/rpm-dev/core2-64

   **PACKAGE_INSTALL**
      The final list of packages passed to the package manager for
      installation into the image.

      Because the package manager controls actual installation of all
      packages, the list of packages passed using **PACKAGE_INSTALL** is
      not the final list of packages that are actually installed. This
      variable is internal to the image construction code. Consequently, in
      general, you should use the
      **IMAGE_INSTALL** variable to specify
      packages for installation. The exception to this is when working with
      the core-image-minimal-initramfs 
      image. When working with an initial RAM filesystem (**Initramfs**) image,
      use the **PACKAGE_INSTALL** variable. For information on creating an
      **Initramfs**, see the "dev-manual/building:building an initial ram filesystem (Initramfs) image" section
      in the Yocto Project Development Tasks Manual.

   **PACKAGE_INSTALL_ATTEMPTONLY**
      Specifies a list of packages the OpenEmbedded build system attempts
      to install when creating an image. If a listed package fails to
      install, the build system does not generate an error. This variable
      is generally not user-defined.

   **PACKAGE_PREPROCESS_FUNCS**
      Specifies a list of functions run to pre-process the
      **PKGD** directory prior to splitting the files out
      to individual packages.

   **PACKAGE_WRITE_DEPS**
      Specifies a list of dependencies for post-installation and
      pre-installation scripts on native/cross tools. If your
      post-installation or pre-installation script can execute at root filesystem
      creation time rather than on the target but depends on a native tool
      in order to execute, you need to list the tools in
      **PACKAGE_WRITE_DEPS**.

      For information on running post-installation scripts, see the
      "dev-manual/new-recipe:post-installation scripts"
      section in the Yocto Project Development Tasks Manual.

   **PACKAGECONFIG**
      This variable provides a means of enabling or disabling features of a
      recipe on a per-recipe basis. **PACKAGECONFIG** blocks are defined in
      recipes when you specify features and then arguments that define
      feature behaviors. Here is the basic block structure (broken over
      multiple lines for readability)::

         PACKAGECONFIG ??= "f1 f2 f3 ..."
         PACKAGECONFIG[f1] = "\
             --with-f1, \
             --without-f1, \
             build-deps-for-f1, \
             runtime-deps-for-f1, \
             runtime-recommends-for-f1, \
             packageconfig-conflicts-for-f1"
         PACKAGECONFIG[f2] = "\
              ... and so on and so on ...

      The **PACKAGECONFIG** variable itself specifies a space-separated
      list of the features to enable. Following the features, you can
      determine the behavior of each feature by providing up to six
      order-dependent arguments, which are separated by commas. You can
      omit any argument you like but must retain the separating commas. The
      order is important and specifies the following:

      #. Extra arguments that should be added to **PACKAGECONFIG_CONFARGS**
         if the feature is enabled.

      #. Extra arguments that should be added to **PACKAGECONFIG_CONFARGS**
         if the feature is disabled.

      #. Additional build dependencies (**DEPENDS**)
         that should be added if the feature is enabled.

      #. Additional runtime dependencies (**RDEPENDS**)
         that should be added if the feature is enabled.

      #. Additional runtime recommendations
         (**RRECOMMENDS**) that should be added if
         the feature is enabled.

      #. Any conflicting (that is, mutually exclusive) **PACKAGECONFIG**
         settings for this feature.

      Consider the following **PACKAGECONFIG** block taken from the
      `librsvg` recipe. In this example the feature is `gtk`, which has
      three arguments that determine the feature's behavior::

         PACKAGECONFIG[gtk] = "--with-gtk3,--without-gtk3,gtk+3"

      The
      `--with-gtk3` and `gtk+3` arguments apply only if the feature is
      enabled. In this case, `--with-gtk3` is added to the configure
      script argument list and `gtk+3` is added to **DEPENDS**. On the
      other hand, if the feature is disabled say through a `.bbappend`
      file in another layer, then the second argument `--without-gtk3` is
      added to the configure script instead.

      The basic **PACKAGECONFIG** structure previously described holds true
      regardless of whether you are creating a block or changing a block.
      When creating a block, use the structure inside your recipe.

      If you want to change an existing **PACKAGECONFIG** block, you can do
      so one of two ways:

      -  *Append file:* Create an append file named
         `recipename.bbappend` in your layer and override the value of
         **PACKAGECONFIG**. You can either completely override the
         variable::

            PACKAGECONFIG = "f4 f5"

         Or, you can just append the variable::

            PACKAGECONFIG:append = " f4"

      -  *Configuration file:* This method is identical to changing the
         block through an append file except you edit your `local.conf`
         or `mydistro.conf` file. As with append files previously
         described, you can either completely override the variable::

            PACKAGECONFIG:pn-recipename = "f4 f5"

         Or, you can just amend the variable::

            PACKAGECONFIG:append:pn-recipename = " f4"

      Consider the following example of a ref-classes-cmake recipe with a systemd service
      in which **PACKAGECONFIG** is used to transform the systemd service
      into a feature that can be easily enabled or disabled via **PACKAGECONFIG**::

         example.c
         example.service
         CMakeLists.txt

      The `CMakeLists.txt` file contains::

         if(WITH_SYSTEMD)
            install(FILES ${PROJECT_SOURCE_DIR}/example.service DESTINATION /etc/systemd/systemd)
         endif(WITH_SYSTEMD)

      In order to enable the installation of `example.service` we need to
      ensure that `-DWITH_SYSTEMD=ON` is passed to the `cmake` command
      execution.  Recipes that have `CMakeLists.txt` generally inherit the
      ref-classes-cmake class, that runs `cmake` with
      **EXTRA_OECMAKE**, which **PACKAGECONFIG_CONFARGS** will be
      appended to.  Now, knowing that **PACKAGECONFIG_CONFARGS** is
      automatically filled with either the first or second element of
      **PACKAGECONFIG** flag value, the recipe would be like::

         inherit cmake
         PACKAGECONFIG = "systemd"
         PACKAGECONFIG[systemd] = "-DWITH_SYSTEMD=ON,-DWITH_SYSTEMD=OFF"

      A side note to this recipe is to check if `systemd` is in fact the used **INIT_MANAGER**
      or not::

         PACKAGECONFIG = "${@'systemd' if d.getVar('INIT_MANAGER') == 'systemd' else ''}"

   **PACKAGECONFIG_CONFARGS**
      A space-separated list of configuration options generated from the
      **PACKAGECONFIG** setting.

      Classes such as ref-classes-autotools and ref-classes-cmake
      use **PACKAGECONFIG_CONFARGS** to pass **PACKAGECONFIG** options
      to `configure` and `cmake`, respectively. If you are using
      **PACKAGECONFIG** but not a class that handles the
      ref-tasks-configure task, then you need to use
      **PACKAGECONFIG_CONFARGS** appropriately.

   **PACKAGEGROUP_DISABLE_COMPLEMENTARY**
      For recipes inheriting the ref-classes-packagegroup class, setting
      **PACKAGEGROUP_DISABLE_COMPLEMENTARY** to "1" specifies that the
      normal complementary packages (i.e. `-dev`, `-dbg`, and so forth)
      should not be automatically created by the `packagegroup` recipe,
      which is the default behavior.

   **PACKAGES**
      The list of packages the recipe creates. The default value is the
      following::

         ${PN}-src ${PN}-dbg ${PN}-staticdev ${PN}-dev ${PN}-doc ${PN}-locale ${PACKAGE_BEFORE_PN} ${PN}

      During packaging, the ref-tasks-package task
      goes through **PACKAGES** and uses the **FILES**
      variable corresponding to each package to assign files to the
      package. If a file matches the **FILES** variable for more than one
      package in **PACKAGES**, it will be assigned to the earliest
      (leftmost) package.

      Packages in the variable's list that are empty (i.e. where none of
      the patterns in `FILES:`\ pkg match any files installed by the
      ref-tasks-install task) are not generated,
      unless generation is forced through the
      **ALLOW_EMPTY** variable.

   **PACKAGES_DYNAMIC**
      A promise that your recipe satisfies runtime dependencies for
      optional modules that are found in other recipes.
      **PACKAGES_DYNAMIC** does not actually satisfy the dependencies, it
      only states that they should be satisfied. For example, if a hard,
      runtime dependency (**RDEPENDS**) of another
      package is satisfied at build time through the **PACKAGES_DYNAMIC**
      variable, but a package with the module name is never actually
      produced, then the other package will be broken. Thus, if you attempt
      to include that package in an image, you will get a dependency
      failure from the packaging system during the
      ref-tasks-rootfs task.

      Typically, if there is a chance that such a situation can occur and
      the package that is not created is valid without the dependency being
      satisfied, then you should use **RRECOMMENDS**
      (a soft runtime dependency) instead of **RDEPENDS**.

      For an example of how to use the **PACKAGES_DYNAMIC** variable when
      you are splitting packages, see the
      "dev-manual/packages:handling optional module packaging"
      section in the Yocto Project Development Tasks Manual.

   **PACKAGESPLITFUNCS**
      Specifies a list of functions run to perform additional splitting of
      files into individual packages. Recipes can either prepend to this
      variable or prepend to the `populate_packages` function in order to
      perform additional package splitting. In either case, the function
      should set **PACKAGES**,
      **FILES**, **RDEPENDS** and
      other packaging variables appropriately in order to perform the
      desired splitting.

   **PARALLEL_MAKE**

      Extra options passed to the build tool command (`make`,
      `ninja` or more specific build engines, like the Go language one)
      during the ref-tasks-compile task, to specify parallel compilation
      on the local build host. This variable is usually in the form "-j x",
      where x represents the maximum number of parallel threads such engines
      can run.

      .. note::

         For software compiled by `make`, in order for **PARALLEL_MAKE**
         to be effective, `make` must be called with
         `${`\ **EXTRA_OEMAKE**\ `}`. An easy
         way to ensure this is to use the `oe_runmake` function.

      By default, the OpenEmbedded build system automatically sets this
      variable to be equal to the number of cores the build system uses.

      .. note::

         If the software being built experiences dependency issues during
         the ref-tasks-compile task that result in race conditions, you can clear
         the **PARALLEL_MAKE** variable within the recipe as a workaround. For
         information on addressing race conditions, see the
         "dev-manual/debugging:debugging parallel make races"
         section in the Yocto Project Development Tasks Manual.

      For single socket systems (i.e. one CPU), you should not have to
      override this variable to gain optimal parallelism during builds.
      However, if you have very large systems that employ multiple physical
      CPUs, you might want to make sure the **PARALLEL_MAKE** variable is
      not set higher than "-j 20".

      For more information on speeding up builds, see the
      "dev-manual/speeding-up-build:speeding up a build"
      section in the Yocto Project Development Tasks Manual.

      For more information on how to limit the resources used during builds, see
      the /dev-manual/limiting-resources section of the Yocto Project
      Development Tasks Manual.

   **PARALLEL_MAKEINST**
      Extra options passed to the build tool install command
      (`make install`, `ninja install` or more specific ones)
      during the ref-tasks-install task in order to specify
      parallel installation. This variable defaults to the value of
      **PARALLEL_MAKE**.

      .. note::

         For software compiled by `make`, in order for **PARALLEL_MAKEINST**
         to be effective, `make` must be called with
         `${`\ **EXTRA_OEMAKE**\ `}`. An easy
         way to ensure this is to use the `oe_runmake` function.

         If the software being built experiences dependency issues during
         the ref-tasks-install task that result in race conditions, you can
         clear the **PARALLEL_MAKEINST** variable within the recipe as a
         workaround. For information on addressing race conditions, see the
         "dev-manual/debugging:debugging parallel make races"
         section in the Yocto Project Development Tasks Manual.

   **PATCHRESOLVE**
      Determines the action to take when a patch fails. You can set this
      variable to one of two values: "noop" and "user".

      The default value of "noop" causes the build to simply fail when the
      OpenEmbedded build system cannot successfully apply a patch. Setting
      the value to "user" causes the build system to launch a shell and
      places you in the right location so that you can manually resolve the
      conflicts.

      Set this variable in your `local.conf` file.

   **PATCHTOOL**
      Specifies the utility used to apply patches for a recipe during the
      ref-tasks-patch task. You can specify one of
      three utilities: "patch", "quilt", or "git". The default utility used
      is "quilt" except for the quilt-native recipe itself. Because the
      quilt tool is not available at the time quilt-native is being
      patched, it uses "patch".

      If you wish to use an alternative patching tool, set the variable in
      the recipe using one of the following::

         PATCHTOOL = "patch"
         PATCHTOOL = "quilt"
         PATCHTOOL = "git"

   **PE**
      The epoch of the recipe. By default, this variable is unset. The
      variable is used to make upgrades possible when the versioning scheme
      changes in some backwards incompatible way.

      **PE** is the default value of the **PKGE** variable.

   **PEP517_WHEEL_PATH**
      When used by recipes that inherit the ref-classes-python_pep517
      class, denotes the path to `dist/` (short for distribution) where the
      binary archive `wheel` is built.

   **PERSISTENT_DIR**
      See **bitbake:PERSISTENT_DIR** in the BitBake manual.

   **PF**
      Specifies the recipe or package name and includes all version and
      revision numbers (i.e. `glibc-2.13-r20+svnr15508/` and
      `bash-4.2-r1/`). This variable is comprised of the following:
      ${**PN**}-${**EXTENDPE**}${**PV**}-${**PR**}

   **PIXBUF_PACKAGES**
      When inheriting the ref-classes-pixbufcache
      class, this variable identifies packages that contain the pixbuf
      loaders used with `gdk-pixbuf`. By default, the
      ref-classes-pixbufcache class assumes that
      the loaders are in the recipe's main package (i.e.
      `${`\ **PN**\ `}`). Use this variable if the
      loaders you need are in a package other than that main package.

   **PKG**
      The name of the resulting package created by the OpenEmbedded build
      system.

      .. note::

         When using the **PKG** variable, you must use a package name override.

      For example, when the ref-classes-debian class renames the output
      package, it does so by setting `PKG:packagename`.

   **PKG_CONFIG_PATH**
      The path to `pkg-config` files for the current build context.
      `pkg-config` reads this variable from the environment.

   **PKGD**
      Points to the destination directory for files to be packaged before
      they are split into individual packages. This directory defaults to
      the following::

         ${WORKDIR}/package

      Do not change this default.

   **PKGDATA_DIR**
      Points to a shared, global-state directory that holds data generated
      during the packaging process. During the packaging process, the
      ref-tasks-packagedata task packages data
      for each recipe and installs it into this temporary, shared area.
      This directory defaults to the following, which you should not
      change::

         ${STAGING_DIR_HOST}/pkgdata

      For examples of how this data is used, see the
      "overview-manual/concepts:automatically added runtime dependencies"
      section in the Yocto Project Overview and Concepts Manual and the
      "dev-manual/debugging:viewing package information with `oe-pkgdata-util```"
      section in the Yocto Project Development Tasks Manual. For more
      information on the shared, global-state directory, see
      **STAGING_DIR_HOST**.

   **PKGDEST**
      Points to the parent directory for files to be packaged after they
      have been split into individual packages. This directory defaults to
      the following::

         ${WORKDIR}/packages-split

      Under this directory, the build system creates directories for each
      package specified in **PACKAGES**. Do not change
      this default.

   **PKGDESTWORK**
      Points to a temporary work area where the
      ref-tasks-package task saves package metadata.
      The **PKGDESTWORK** location defaults to the following::

         ${WORKDIR}/pkgdata

      Do not change this default.

      The ref-tasks-packagedata task copies the
      package metadata from **PKGDESTWORK** to
      **PKGDATA_DIR** to make it available globally.

   **PKGE**
      The epoch of the package(s) built by the recipe. By default, **PKGE**
      is set to **PE**.

   **PKGR**
      The revision of the package(s) built by the recipe. By default,
      **PKGR** is set to **PR**.

   **PKGV**
      The version of the package(s) built by the recipe. By default,
      **PKGV** is set to **PV**.

      If **PV** contains the `+` sign, source control information will be
      included in **PKGV** later in the packaging phase. For more
      information, see the /dev-manual/external-scm section of the Yocto
      Project Development Tasks Manual.

      .. warning::

         Since source control information is included in a late stage by the
         ref-classes-package class, it cannot be seen from the BitBake
         environment with `bitbake -e` or `bitbake-getvar`. Instead, after
         the package is built, the version information can be retrieved with
         `oe-pkgdata-util package-info <package name>`. See the
         :ref:`dev-manual/debugging:Viewing Package Information with
         `oe-pkgdata-util`` section of the Yocto Project Development Tasks
         Manual for more information on `oe-pkgdata-util`.

   **PN**
      This variable can have two separate functions depending on the
      context: a recipe name or a resulting package name.

      **PN** refers to a recipe name in the context of a file used by the
      OpenEmbedded build system as input to create a package. The name is
      normally extracted from the recipe file name. For example, if the
      recipe is named `expat_2.0.1.bb`, then the default value of **PN**
      will be "expat".

      The variable refers to a package name in the context of a file
      created or produced by the OpenEmbedded build system.

      If applicable, the **PN** variable also contains any special suffix
      or prefix. For example, using `bash` to build packages for the
      native machine, **PN** is `bash-native`. Using `bash` to build
      packages for the target and for Multilib, **PN** would be `bash`
      and `lib64-bash`, respectively.

   **POPULATE_SDK_POST_HOST_COMMAND**
      Specifies a list of functions to call once the OpenEmbedded build
      system has created the host part of the SDK. You can specify
      functions separated by spaces::

          POPULATE_SDK_POST_HOST_COMMAND += "function"

      If you need to pass the SDK path to a command within a function, you
      can use `${SDK_DIR}`, which points to the parent directory used by
      the OpenEmbedded build system when creating SDK output. See the
      **SDK_DIR** variable for more information.

   **POPULATE_SDK_POST_TARGET_COMMAND**
      Specifies a list of functions to call once the OpenEmbedded build
      system has created the target part of the SDK. You can specify
      functions separated by spaces::

         POPULATE_SDK_POST_TARGET_COMMAND += "function"

      If you need to pass the SDK path to a command within a function, you
      can use `${SDK_DIR}`, which points to the parent directory used by
      the OpenEmbedded build system when creating SDK output. See the
      **SDK_DIR** variable for more information.

   **PR**
      The revision of the recipe. The default value for this variable is
      "r0". Subsequent revisions of the recipe conventionally have the
      values "r1", "r2", and so forth. When **PV** increases,
      **PR** is conventionally reset to "r0".

      .. note::

         The OpenEmbedded build system does not need the aid of **PR**
         to know when to rebuild a recipe. The build system uses the task
         input checksums  along with the
         stamp  and
         overview-manual/concepts:shared state cache
         mechanisms.

      The **PR** variable primarily becomes significant when a package
      manager dynamically installs packages on an already built image. In
      this case, **PR**, which is the default value of
      **PKGR**, helps the package manager distinguish which
      package is the most recent one in cases where many packages have the
      same **PV** (i.e. **PKGV**). A component having many packages with
      the same **PV** usually means that the packages all install the same
      upstream version, but with later (**PR**) version packages including
      packaging fixes.

      .. note::

         **PR** does not need to be increased for changes that do not change the
         package contents or metadata.

      Because manually managing **PR** can be cumbersome and error-prone,
      an automated solution exists. See the
      "dev-manual/packages:working with a pr service" section
      in the Yocto Project Development Tasks Manual for more information.

   **PREFERRED_PROVIDER**
      If multiple recipes provide the same item, this variable determines
      which recipe is preferred and thus provides the item (i.e. the
      preferred provider). You should always suffix this variable with the
      name of the provided item. And, you should define the variable using
      the preferred recipe's name (**PN**). Here is a common
      example::

         PREFERRED_PROVIDER_virtual/kernel ?= "linux-yocto"

      In the previous example, multiple recipes are providing "virtual/kernel".
      The **PREFERRED_PROVIDER** variable is set with the name (**PN**) of
      the recipe you prefer to provide "virtual/kernel".

      Here are more examples::

         PREFERRED_PROVIDER_virtual/xserver = "xserver-xf86"
         PREFERRED_PROVIDER_virtual/libgl ?= "mesa"

      For more
      information, see the "dev-manual/new-recipe:using virtual providers"
      section in the Yocto Project Development Tasks Manual.

      .. note::

         If you use a `virtual/\*` item with **PREFERRED_PROVIDER**, then any
         recipe that **PROVIDES** that item but is not selected (defined)
         by **PREFERRED_PROVIDER** is prevented from building, which is usually
         desirable since this mechanism is designed to select between mutually
         exclusive alternative providers.

   **PREFERRED_PROVIDERS**
      See **bitbake:PREFERRED_PROVIDERS** in the BitBake manual.

   **PREFERRED_RPROVIDER**
      The **PREFERRED_RPROVIDER** variable works like the
      **PREFERRED_PROVIDER** variable, but it denotes packages that provide a
      *runtime* component. Runtime providers are declared in recipes that set
      the **RPROVIDES** variable for a specific package.

      For example::

         PREFERRED_RPROVIDER_virtual-x-terminal-emulator = "rxvt-unicode"

      This statement sets the runtime provider for the X terminal emulator to
      `rxvt-unicode`. The `rxvt-unicode` package is a runtime provider of
      this component because the `rxvt-unicode` recipe set the following
      **RPROVIDES** definition for the `rxvt-unicode` (`${PN}`)
      package::

         RPROVIDES:${PN} = "virtual-x-terminal-emulator"

      For more information on virtual providers, see the
      "dev-manual/new-recipe:using virtual providers" section in the
      Yocto Project Development Tasks Manual.

   **PREFERRED_TOOLCHAIN**
      The **PREFERRED_TOOLCHAIN** variable selects the toolchain to use for
      compiling recipes. This variable is not meant to be overridden globally.
      Instead, the values of **PREFERRED_TOOLCHAIN_TARGET**,
      **PREFERRED_TOOLCHAIN_NATIVE** and **PREFERRED_TOOLCHAIN_SDK**
      should be overridden.

   **PREFERRED_TOOLCHAIN_NATIVE**
      This variable controls the toolchain used for compiling
      ref-classes-native recipes.

      This variable should be set globally from a **configuration file**.

      See **PREFERRED_TOOLCHAIN_TARGET** for more details on the possible
      values for this variable.

      A recipe that does not support the toolchain specified by
      **PREFERRED_TOOLCHAIN_NATIVE** can override it locally with
      **TOOLCHAIN_NATIVE**.

   **PREFERRED_TOOLCHAIN_SDK**
      This variable controls the toolchain used for compiling
      ref-classes-nativesdk recipes.

      This variable should be set globally from a **configuration file**.

      See **PREFERRED_TOOLCHAIN_TARGET** for more details on the possible
      values for this variable.

   **PREFERRED_TOOLCHAIN_TARGET**
      This variable controls the toolchain used for compiling recipes in the
      architecture of the target **MACHINE**.

      There are two possible values for this variable at the moment:

      -  gcc  (default): the GCC/Binutils toolchain.
      -  clang : the Clang/LLVM toolchain.

      **PREFERRED_TOOLCHAIN_TARGET** will make the ref-classes-base
      class inherit one of the toolchain classes defined in
      :oe_git:`meta/classes/toolchain
      </openembedded-core/tree/meta/classes/toolchain>`. As a consequence, this
      variable should be set globally from a **configuration file**.

      These classes define commands used for cross-compiling such as **CC**,
      **CXX**, **LD** and so on.

      A recipe that does not support the toolchain specified by
      **PREFERRED_TOOLCHAIN_TARGET** can override it locally with
      **TOOLCHAIN**.

   **PREFERRED_VERSION**
      If there are multiple versions of a recipe available, this variable
      determines which version should be given preference. You must always
      suffix the variable with the **PN** you want to select (`python` in
      the first example below), and you should specify the **PV**
      accordingly (`3.4.0` in the example).

      The **PREFERRED_VERSION** variable supports limited wildcard use
      through the "`%`" character. You can use the character to match any
      number of characters, which can be useful when specifying versions
      that contain long revision numbers that potentially change. Here are
      two examples::

         PREFERRED_VERSION_python = "3.4.0"
         PREFERRED_VERSION_linux-yocto = "5.0%"

      .. note::

         The use of the "%" character is limited in that it only works at the end of the
         string. You cannot use the wildcard character in any other
         location of the string.

      The specified version is matched against **PV**, which does not
      necessarily match the version part of the recipe's filename.

      If you want to select a recipe named `foo_git.bb` which has **PV**
      set to `1.2.3+git`, you can do so by setting ``PREFERRED_VERSION_foo`
      to `1.2.3%` (i.e. simply setting `PREFERRED_VERSION_foo` to `git`
      will not work as the name of the recipe isn't used, but rather its
      **PV** definition).

      Sometimes the **PREFERRED_VERSION** variable can be set by
      configuration files in a way that is hard to change. You can use
      **OVERRIDES** to set a machine-specific
      override. Here is an example::

         PREFERRED_VERSION_linux-yocto:qemux86 = "5.0%"

      Although not recommended, worst case, you can also use the
      "forcevariable" override, which is the strongest override possible.
      Here is an example::

         PREFERRED_VERSION_linux-yocto:forcevariable = "5.0%"

      .. note::

         The `:forcevariable` override is not handled specially. This override
         only works because the default value of **OVERRIDES** includes "forcevariable".

      If a recipe with the specified version is not available, a warning
      message will be shown. See **REQUIRED_VERSION** if you want this
      to be an error instead.

   **PREMIRRORS**
      Specifies additional paths from which the OpenEmbedded build system
      gets source code. When the build system searches for source code, it
      first tries the local download directory. If that location fails, the
      build system tries locations defined by **PREMIRRORS**, the upstream
      source, and then locations specified by
      **MIRRORS** in that order.

      The default value for **PREMIRRORS** is defined in the
      `meta/classes-global/mirrors.bbclass` file in the core metadata layer.

      Typically, you could add a specific server for the build system to
      attempt before any others by adding something like the following to
      the `local.conf` configuration file in the
      **Build Directory**::

         PREMIRRORS:prepend = "\
             git://.*/.* &YOCTO_DL_URL;/mirror/sources/ \
             ftp://.*/.* &YOCTO_DL_URL;/mirror/sources/ \
             http://.*/.* &YOCTO_DL_URL;/mirror/sources/ \
             https://.*/.* &YOCTO_DL_URL;/mirror/sources/"

      These changes cause the
      build system to intercept Git, FTP, HTTP, and HTTPS requests and
      direct them to the `http://` sources mirror. You can use
      `file://` URLs to point to local directories or network shares as
      well.

   **PRIORITY**
      Indicates the importance of a package.

      **PRIORITY** is considered to be part of the distribution policy
      because the importance of any given recipe depends on the purpose for
      which the distribution is being produced. Thus, **PRIORITY** is not
      normally set within recipes.

      You can set **PRIORITY** to "required", "standard", "extra", and
      "optional", which is the default.

   **PRIVATE_LIBS**
      Specifies libraries installed within a recipe that should be ignored
      by the OpenEmbedded build system's shared library resolver. This
      variable is typically used when software being built by a recipe has
      its own private versions of a library normally provided by another
      recipe. In this case, you would not want the package containing the
      private libraries to be set as a dependency on other unrelated
      packages that should instead depend on the package providing the
      standard version of the library.

      Libraries specified in this variable should be specified by their
      file name. For example, from the Firefox recipe in meta-browser::

         PRIVATE_LIBS = "libmozjs.so \
                         libxpcom.so \
                         libnspr4.so \
                         libxul.so \
                         libmozalloc.so \
                         libplc4.so \
                         libplds4.so"

      For more information, see the
      "overview-manual/concepts:automatically added runtime dependencies"
      section in the Yocto Project Overview and Concepts Manual.

   **PROVIDES**
      A list of aliases by which a particular recipe can be known. By
      default, a recipe's own **PN** is implicitly already in its
      **PROVIDES** list and therefore does not need to mention that it
      provides itself. If a recipe uses **PROVIDES**, the additional
      aliases are synonyms for the recipe and can be useful for satisfying
      dependencies of other recipes during the build as specified by
      **DEPENDS**.

      Consider the following example **PROVIDES** statement from the recipe
      file `eudev_3.2.9.bb`::

         PROVIDES += "udev"

      The **PROVIDES** statement
      results in the "eudev" recipe also being available as simply "udev".

      .. note::

         A recipe's own recipe name (**PN**) is always implicitly prepended
         to **PROVIDES**, so while using "+=" in the above example may not be
         strictly necessary it is recommended to avoid confusion.

      In addition to providing recipes under alternate names, the
      **PROVIDES** mechanism is also used to implement virtual targets. A
      virtual target is a name that corresponds to some particular
      functionality (e.g. a Linux kernel). Recipes that provide the
      functionality in question list the virtual target in **PROVIDES**.
      Recipes that depend on the functionality in question can include the
      virtual target in **DEPENDS** to leave the choice of provider open.

      Conventionally, virtual targets have names on the form
      "virtual/function" (e.g. "virtual/kernel"). The slash is simply part
      of the name and has no syntactical significance.

      The **PREFERRED_PROVIDER** variable is
      used to select which particular recipe provides a virtual target.

      .. note::

         A corresponding mechanism for virtual runtime dependencies (packages)
         exists. However, the mechanism does not depend on any special
         functionality beyond ordinary variable assignments. For example,
         **VIRTUAL-RUNTIME_dev_manager <VIRTUAL-RUNTIME>** refers to the
         package of the component that manages the `/dev` directory.

         Setting the "preferred provider" for runtime dependencies is as
         simple as using the following assignment in a configuration file::

                 VIRTUAL-RUNTIME_dev_manager = "udev"

   **PRSERV_HOST**
      The network based **PR** service host and port.

      The `conf/templates/default/local.conf.sample.extended` configuration
      file in meta-poky  shows how the
      **PRSERV_HOST** variable is set::

         PRSERV_HOST = "localhost:0"

      You must
      set the variable if you want to automatically start a local :ref:`PR
      service <dev-manual/packages:working with a pr service>`. You can
      set **PRSERV_HOST** to other values to use a remote PR service.

   **PRSERV_UPSTREAM**
      This variable can be used to specify an upstream PR server for the local
      PR server to connect to, in the form of `host:port`.

      This makes it possible to implement local fixes to an upstream package.

   **PSEUDO_IGNORE_PATHS**
      A comma-separated (without spaces) list of path prefixes that should be ignored
      by pseudo when monitoring and recording file operations, in order to avoid
      problems with files being written to outside of the pseudo context and
      reduce pseudo's overhead. A path is ignored if it matches any prefix in the list
      and can include partial directory (or file) names. In case a path prefix is
      present in both **PSEUDO_IGNORE_PATHS** and in **PSEUDO_INCLUDE_PATHS**,
      **PSEUDO_INCLUDE_PATHS** takes precedence.

   **PSEUDO_INCLUDE_PATHS**
      A comma-separated (without spaces) list of path prefixes that should be included
      by pseudo when monitoring and recording file operations, in order to avoid
      problems with files being written to outside of the pseudo context and
      reduce pseudo 's overhead.
      A path is included if it matches any prefix in the list and can include
      partial directory (or file) names. In case a path prefix is present in both
      **PSEUDO_IGNORE_PATHS** and in **PSEUDO_INCLUDE_PATHS**,
      **PSEUDO_INCLUDE_PATHS** takes precedence.

   **PTEST_ENABLED**
      Specifies whether or not :ref:`Package
      Test <test-manual/ptest:testing packages with ptest>` (ptest)
      functionality is enabled when building a recipe. You should not set
      this variable directly. Enabling and disabling building Package Tests
      at build time should be done by adding "ptest" to (or removing it
      from) **DISTRO_FEATURES**.

   **PTEST_PYTEST_DIR**
      Within the ref-classes-ptest-python-pytest class, the
      **PTEST_PYTEST_DIR** variable represents the path within the source
      tree of a Python package holding the unit tests to be run with the
      `pytest <https://docs.pytest.org>`__ framework. The default value for this
      variable is `tests`.

      For more information, see
      the ref-classes-ptest-python-pytest class documentation.

   **PV**
      The version of the recipe. The version is normally extracted from the
      recipe filename. For example, if the recipe is named
      `expat_2.0.1.bb`, then the default value of **PV** will be "2.0.1".
      **PV** is generally not overridden within a recipe unless it is
      building an unstable (i.e. development) version from a source code
      repository (e.g. Git or Subversion).

      **PV** is the default value of the **PKGV** variable.

   **PYPI_PACKAGE**
      When inheriting the ref-classes-pypi class, specifies the
      `PyPI <https://pypi.org/>`__ package name to be built. The default value
      is set based upon **BPN** (stripping any "python-" or "python3-"
      prefix off if present), however for some packages it will need to be set
      explicitly if that will not match the package name (e.g. where the
      package name has a prefix, underscores, uppercase letters etc.)

   **PYPI_PACKAGE_EXT**
      When inheriting the ref-classes-pypi class, specifies the
      file extension to use when fetching a package from `PyPI
      <https://pypi.org/>`__. Default is `tar.gz`.

   **PYPI_SRC_URI**
      When inheriting the ref-classes-pypi class, specifies the
      full `pythonhosted <https://files.pythonhosted.org/>`__ URI for
      fetching the package to be built. The default value is constructed
      based upon **PYPI_PACKAGE**, **PYPI_PACKAGE_EXT**, and
      **PV**. Most recipes will not need to set this variable unless
      they are building an unstable (i.e. development) version.

   **PYTHON_ABI**
      When used by recipes that inherit the ref-classes-setuptools3
      class, denotes the Application Binary Interface (ABI) currently in use
      for Python. By default, the ABI is "m". You do not have to set this
      variable as the OpenEmbedded build system sets it for you.

      The OpenEmbedded build system uses the ABI to construct directory
      names used when installing the Python headers and libraries in
      sysroot (e.g. `.../python3.3m/...`).

   **QA_EMPTY_DIRS**
      Specifies a list of directories that are expected to be empty when
      packaging; if `empty-dirs` appears in **ERROR_QA** or
      **WARN_QA** these will be checked and an error or warning
      (respectively) will be produced.

      The default **QA_EMPTY_DIRS** value is set in
      insane.bbclass .

   **QA_EMPTY_DIRS_RECOMMENDATION**
      Specifies a recommendation for why a directory must be empty,
      which will be included in the error message if a specific directory
      is found to contain files. Must be overridden with the directory
      path to match on.

      If no recommendation is specified for a directory, then the default
      "but it is expected to be empty" will be used.

      An example message shows if files were present in '/dev'::

         QA_EMPTY_DIRS_RECOMMENDATION:/dev = "but all devices must be created at runtime"

   **QB_CMDLINE_IP_SLIRP**

      If **QB_NETWORK_DEVICE** adds more than one network interface to QEMU,
      usually the `ip=` Linux kernel command line argument needs to be changed
      accordingly. The **QB_CMDLINE_IP_SLIRP** variable allows controlling
      this value. See the Linux kernel documentation for more details:
      https://www.kernel.org/doc/Documentation/filesystems/nfs/nfsroot.txt.

   **QB_CMDLINE_IP_TAP**

      This variable is similar to the **QB_CMDLINE_IP_SLIRP** variable.

      Use as follows::

         QB_CMDLINE_IP_TAP = "ip=192.168.7.@CLIENT@::192.168.7.@GATEWAY@:255.255.255.0::eth0"

      Since the tap interface requires static IP configuration, `runqemu`
      replaces the `@CLIENT@` and `@GATEWAY@` place holders by the IP and
      the gateway address of the QEMU guest.

   **QB_DEFAULT_FSTYPE**

      The **QB_DEFAULT_FSTYPE** variable controls the default filesystem
      type to boot. It is represented as the file extension of one of the root
      filesystem image extension found in **DEPLOY_DIR_IMAGE**. For example:
      `ext4.zst`.

   **QB_DEFAULT_KERNEL**

      When using `runqemu`, the **QB_DEFAULT_KERNEL** variable controls
      the default Linux kernel image to boot, found in **DEPLOY_DIR_IMAGE**. For
      example: `bzImage`.

   **QB_DRIVE_TYPE**

      When using `runqemu`, the **QB_DRIVE_TYPE** variable specifies the
      type of drive to emulate when starting the emulated machine.
      Valid values are:

      -  `/dev/hd`: emulates an IDE drive.
      -  `/dev/mmcblk`: emulates an SD Card.
      -  `/dev/sd`: emulates an SCSI drive.
      -  `/dev/vd`: emulates a VirtIO drive.
      -  `/dev/vdb`: emulates a block VirtIO drive.

   **QB_GRAPHICS**

      When using `runqemu`, the **QB_GRAPHICS** variable controls the QEMU
      video card type to emulate. For example: `-vga std`.

      This value is appended to the argument list when running `qemu`.

   **QB_KERNEL_CMDLINE_APPEND**

      The **QB_KERNEL_CMDLINE_APPEND** variable controls the options passed
      to the Linux kernel's `-append` QEMU options, which controls the Linux kernel
      command-line.

      For example::

         QB_KERNEL_CMDLINE_APPEND = "console=ttyS0"

   **QB_MEM**

      The **QB_MEM** variable controls the amount of memory allocated to the
      emulated machine. Specify as follows::

         QB_MEM = "-m 512"

   **QB_NETWORK_DEVICE**

      When using `runqemu`, the **QB_NETWORK_DEVICE** variable controls
      the network device instantiated by QEMU. This value needs to be compatible
      with the **QB_TAP_OPT** variable.

      Example::

         QB_NETWORK_DEVICE = "-device virtio-net-pci,netdev=net0,mac=@MAC@"

      `runqemu` replaces `@MAC@` with a predefined mac address.

   **QB_NFSROOTFS_EXTRA_OPT**

      When using `runqemu`, the **QB_NFSROOTFS_EXTRA_OPT** variable
      controls extra options to be appended to the NFS rootfs options in the
      Linux kernel command-line.

      For example::

         QB_NFSROOTFS_EXTRA_OPT = "wsize=4096,rsize=4096"

   **QB_OPT_APPEND**

      When using `runqemu`, the **QB_OPT_APPEND** variable controls
      general options to append to QEMU when starting.

   **QB_RNG**

      When using `runqemu`, the **QB_RNG** variable controls
      pass-through for host random number generator, it can speedup boot
      in system mode, where system is experiencing entropy starvation.

      For example::

         QB_RNG = "-object rng-random,filename=/dev/urandom,id=rng0 -device virtio-rng-pci,rng=rng0"

   **QB_ROOTFS_EXTRA_OPT**

      When using `runqemu`, the **QB_ROOTFS_EXTRA_OPT** variable controls
      extra options to be appended to the rootfs device options.

   **QB_SERIAL_OPT**

      When using `runqemu`, the **QB_SERIAL_OPT** variable controls the
      serial port option.

      For example::

         QB_SERIAL_OPT = "-serial mon:stdio"

   **QB_SMP**

      When using `runqemu`, the **QB_SMP** variable controls
      amount of CPU cores made availalble inside the QEMU guest, each mapped to
      a thread on the host.

      For example::

         QB_SMP = "-smp 8".

   **QB_TAP_NAMESERVER**

      When using `runqemu`, the **QB_TAP_NAMESERVER** variable controls
      the default name server  used in the QEMU guest.

   **QB_TAP_OPT**

      When using `runqemu`, the **QB_TAP_OPT** variable controls
      the network option for "tap" mode.

      For example::

         QB_TAP_OPT = "-netdev tap,id=net0,ifname=@TAP@,script=no,downscript=no"

      Note that `runqemu` will replace `@TAP@` with the tap interface in
      use, such as `tap0`, `tap1`, etc.

   **RANLIB**
      The minimal command and arguments to run `ranlib <ranlib(1)>`.

   **RCONFLICTS**
      The list of packages that conflict with packages. Note that packages
      will not be installed if conflicting packages are not first removed.

      Like all package-controlling variables, you must always use them in
      conjunction with a package name override. Here is an example::

         RCONFLICTS:${PN} = "another_conflicting_package_name"

      BitBake, which the OpenEmbedded build system uses, supports
      specifying versioned dependencies. Although the syntax varies
      depending on the packaging format, BitBake hides these differences
      from you. Here is the general syntax to specify versions with the
      **RCONFLICTS** variable::

         RCONFLICTS:${PN} = "package (operator version)"

      For `operator`, you can specify the following:

      - =
      - <
      - >
      - <=
      - >=

      For example, the following sets up a dependency on version 1.2 or
      greater of the package `foo`::

         RCONFLICTS:${PN} = "foo (>= 1.2)"

   **RDEPENDS**
      Lists runtime dependencies of a package. These dependencies are other
      packages that must be installed in order for the package to function
      correctly. As an example, the following assignment declares that the
      package `foo` needs the packages `bar` and `baz` to be
      installed::

         RDEPENDS:foo = "bar baz"

      The most common types of package
      runtime dependencies are automatically detected and added. Therefore,
      most recipes do not need to set **RDEPENDS**. For more information,
      see the
      "overview-manual/concepts:automatically added runtime dependencies"
      section in the Yocto Project Overview and Concepts Manual.

      The practical effect of the above **RDEPENDS** assignment is that
      `bar` and `baz` will be declared as dependencies inside the
      package `foo` when it is written out by one of the
      do_package_write_*  tasks.
      Exactly how this is done depends on which package format is used,
      which is determined by
      **PACKAGE_CLASSES**. When the
      corresponding package manager installs the package, it will know to
      also install the packages on which it depends.

      To ensure that the packages `bar` and `baz` get built, the
      previous **RDEPENDS** assignment also causes a task dependency to be
      added. This dependency is from the recipe's
      ref-tasks-build (not to be confused with
      ref-tasks-compile) task to the
      do_package_write_*  task of the recipes that build `bar` and
      `baz`.

      The names of the packages you list within **RDEPENDS** must be the
      names of other packages --- they cannot be recipe names. Although
      package names and recipe names usually match, the important point
      here is that you are providing package names within the **RDEPENDS**
      variable. For an example of the default list of packages created from
      a recipe, see the **PACKAGES** variable.

      Because the **RDEPENDS** variable applies to packages being built,
      you should always use the variable in a form with an attached package
      name (remember that a single recipe can build multiple packages). For
      example, suppose you are building a development package that depends
      on the `perl` package. In this case, you would use the following
      **RDEPENDS** statement::

         RDEPENDS:${PN}-dev += "perl"

      In the example,
      the development package depends on the `perl` package. Thus, the
      **RDEPENDS** variable has the `${PN}-dev` package name as part of
      the variable.

      .. note::

         `RDEPENDS:${PN}-dev` includes `${`\ **PN**\ `}`
         by default. This default is set in the BitBake configuration file
         (`meta/conf/bitbake.conf`). Be careful not to accidentally remove
         `${PN}` when modifying `RDEPENDS:${PN}-dev`. Use the "+=" operator
         rather than the "=" operator.

      The package names you use with **RDEPENDS** must appear as they would
      in the **PACKAGES** variable. The **PKG** variable
      allows a different name to be used for the final package (e.g. the
      ref-classes-debian class uses this to rename
      packages), but this final package name cannot be used with
      **RDEPENDS**, which makes sense as **RDEPENDS** is meant to be
      independent of the package format used.

      BitBake, which the OpenEmbedded build system uses, supports
      specifying versioned dependencies. Although the syntax varies
      depending on the packaging format, BitBake hides these differences
      from you. Here is the general syntax to specify versions with the
      **RDEPENDS** variable::

         RDEPENDS:${PN} = "package (operator version)"

      For `operator`, you can specify the following:

      - =
      - <
      - >
      - <=
      - >=

      For version, provide the version number.

      .. note::

         You can use **EXTENDPKGV** to provide a full package version
         specification.

      For example, the following sets up a dependency on version 1.2 or
      greater of the package `foo`::

         RDEPENDS:${PN} = "foo (>= 1.2)"

      For information on build-time dependencies, see the **DEPENDS**
      variable. You can also see the
      "bitbake-user-manual/bitbake-user-manual-metadata:tasks" and
      "bitbake-user-manual/bitbake-user-manual-execution:dependencies" sections in the
      BitBake User Manual for additional information on tasks and dependencies.

   **READELF**
      The minimal command and arguments to run `readelf <readelf(1)>`.

   **RECIPE_MAINTAINER**
      This variable defines the name and e-mail address of the maintainer of a
      recipe. Such information can be used by human users submitted changes,
      and by automated tools to send notifications, for example about
      vulnerabilities or source updates.

      The variable can be defined in a global distribution :oe_git:`maintainers.inc
      </openembedded-core/tree/meta/conf/distro/include/maintainers.inc>` file::

          meta/conf/distro/include/maintainers.inc:RECIPE_MAINTAINER:pn-sysvinit = "Ross Burton <ross.burton@arm.com>"

      It can also be directly defined in a recipe,
      for example in the `libgpiod` one::

          RECIPE_MAINTAINER = "Bartosz Golaszewski <brgl@bgdev.pl>"

   **RECIPE_NO_UPDATE_REASON**
      If a recipe should not be replaced by a more recent upstream version,
      putting the reason why in this variable in a recipe allows
      `devtool check-upgrade-status` command to display it, as explained
      in the "ref-manual/devtool-reference:checking on the upgrade status of a recipe"
      section.

   **RECIPE_SYSROOT**
      This variable points to the directory that holds all files populated from
      recipes specified in **DEPENDS**. As the name indicates,
      think of this variable as a custom root (`/`) for the recipe that will be
      used by the compiler in order to find headers and other files needed to complete
      its job.

      This variable is related to **STAGING_DIR_HOST** or **STAGING_DIR_TARGET**
      according to the type of the recipe and the build target.

      To better understand this variable, consider the following examples:

      -  For `#include <header.h>`, `header.h` should be in `"${RECIPE_SYSROOT}/usr/include"`

      -  For `-lexample`, `libexample.so` should be in `"${RECIPE_SYSROOT}/lib"`
         or other library sysroot directories.

      The default value is `"${WORKDIR}/recipe-sysroot"`.
      Do not modify it.

   **RECIPE_SYSROOT_NATIVE**
      This is similar to **RECIPE_SYSROOT** but the populated files are from
      `-native` recipes. This allows a recipe built for the target machine to
      use `native` tools.

      This variable is related to **STAGING_DIR_NATIVE**.

      The default value is `"${WORKDIR}/recipe-sysroot-native"`.
      Do not modify it.

   **RECIPE_UPGRADE_EXTRA_TASKS**
      When upgrading a recipe with `devtool upgrade`, the variable
      **RECIPE_UPGRADE_EXTRA_TASKS** specifies a space-delimited list of
      tasks to run after the new sources have been unpacked.

      For some recipes, after the new source has been unpacked, additional tasks
      may need to be run during an upgrade. A good example of this is recipes
      which inherit ref-classes-cargo-update-recipe-crates, where the
      `do_update_crates` task needs to be run whenever Cargo.toml/Cargo.lock have
      changed in the source.

   **REPODIR**
      See **bitbake:REPODIR** in the BitBake manual.

   **REQUIRED_COMBINED_FEATURES**
      When inheriting the ref-classes-features_check class, this variable
      identifies combined features (the intersection of **MACHINE_FEATURES**
      and **DISTRO_FEATURES**) that must exist in the current configuration
      in order for the **OpenEmbedded Build System** to build the recipe. In
      other words, if the **REQUIRED_COMBINED_FEATURES** variable lists a
      feature that does not appear in **COMBINED_FEATURES** within the
      current configuration, then the recipe will be skipped, and if the build
      system attempts to build the recipe then an error will be triggered.

   **REQUIRED_DISTRO_FEATURES**
      When inheriting the ref-classes-features_check
      class, this variable identifies distribution features that must exist
      in the current configuration in order for the OpenEmbedded build
      system to build the recipe. In other words, if the
      **REQUIRED_DISTRO_FEATURES** variable lists a feature that does not
      appear in **DISTRO_FEATURES** within the current configuration, then
      the recipe will be skipped, and if the build system attempts to build
      the recipe then an error will be triggered.

   **REQUIRED_IMAGE_FEATURES**
      When inheriting the ref-classes-features_check class, this variable
      identifies image features that must exist in the current
      configuration in order for the **OpenEmbedded Build System** to build
      the recipe. In other words, if the **REQUIRED_IMAGE_FEATURES** variable
      lists a feature that does not appear in **IMAGE_FEATURES** within the
      current configuration, then the recipe will be skipped, and if the build
      system attempts to build the recipe then an error will be triggered.

      Compared to other `REQUIRED_*_FEATURES` variables, the
      **REQUIRED_IMAGE_FEATURES** varible only targets image recipes, as the
      **IMAGE_FEATURES** variable is handled by the ref-classes-core-image
      class). However, the **REQUIRED_IMAGE_FEATURES** varible can also be
      set from a **Configuration File**, such as a distro
      configuration file, if the list of required image features should apply to
      all images using this **DISTRO**.

   **REQUIRED_MACHINE_FEATURES**
      When inheriting the ref-classes-features_check class, this variable
      identifies **MACHINE_FEATURES** that must exist in the current
      configuration in order for the **OpenEmbedded Build System** to build
      the recipe. In other words, if the **REQUIRED_MACHINE_FEATURES** variable
      lists a feature that does not appear in **MACHINE_FEATURES** within the
      current configuration, then the recipe will be skipped, and if the build
      system attempts to build the recipe then an error will be triggered.

   **REQUIRED_TUNE_FEATURES**
      When inheriting the ref-classes-features_check class, this variable
      identifies tune features that must exist in the current configuration in
      order for the **OpenEmbedded Build System** to build the recipe. In
      other words, if the **REQUIRED_TUNE_FEATURES** variable lists a
      feature that does not appear in **TUNE_FEATURES** within the current
      configuration, then the recipe will be skipped, and if the build system
      attempts to build the recipe then an error will be triggered.

   **REQUIRED_VERSION**
      If there are multiple versions of a recipe available, this variable
      determines which version should be given preference.
      **REQUIRED_VERSION** works in exactly the same manner as
      **PREFERRED_VERSION**, except that if the specified version is not
      available then an error message is shown and the build fails
      immediately.

      If both **REQUIRED_VERSION** and **PREFERRED_VERSION** are set
      for the same recipe, the **REQUIRED_VERSION** value applies.

   **RETAIN_DIRS_ALWAYS**
      When inheriting the ref-classes-retain class, this variable holds
      space-separated recipe-specific directories to always save in a tarball
      whether the recipe build has failed or not.

   **RETAIN_DIRS_FAILURE**
      When inheriting the ref-classes-retain class, this variable holds
      space-separated recipe-specific directories to save in a tarball on
      failure of the recipe's build.

   **RETAIN_DIRS_GLOBAL_ALWAYS**
      When inheriting the ref-classes-retain class, this variable holds
      space-separated directories that are not specific to a recipe to save in a
      tarball whether the build has failed or not.

   **RETAIN_DIRS_GLOBAL_FAILURE**
      When inheriting the ref-classes-retain class, this variable holds
      space-separated directories that are not specific to a recipe to save in a
      tarball on build failure.

   **RETAIN_ENABLED**
      Disables the creation of a tarball of the work directory done by the
      ref-classes-retain class. Can be set to specific recipes to disable
      the class when the class was inherited globally with **INHERIT**.

   **RETAIN_OUTDIR**
      When inheriting the ref-classes-retain class, this variable
      specifies the directory where to save the tarball of the work directory.
      The default directory is `${TMPDIR}/retain`.

   **RETAIN_TARBALL_SUFFIX**
      When inheriting the ref-classes-retain class, this variable
      specifies the suffix of the tarball of the work directory. The default
      suffix is `${DATETIME}.tar.gz`.

   **RM_WORK_EXCLUDE**
      With ref-classes-rm-work enabled, this variable
      specifies a list of recipes whose work directories should not be removed.
      See the "ref-classes-rm-work" section for more details.

   **ROOT_HOME**
      Defines the root home directory. By default, this directory is set as
      follows in the BitBake configuration file::

         ROOT_HOME ??= "/home/root"

      .. note::

         This default value is likely used because some embedded solutions
         prefer to have a read-only root filesystem and prefer to keep
         writeable data in one place.

      When setting `INIT_MANAGER = systemd`, the default will be set to::

         ROOT_HOME ?= "/root"

      You can also override the default by setting the variable in your distro
      configuration or in the `local.conf` file.

   **ROOTFS**
      Indicates a filesystem image to include as the root filesystem.

      The **ROOTFS** variable is an optional variable used with the
      ref-classes-image-live class.

   **ROOTFS_POSTINSTALL_COMMAND**
      Specifies a list of functions to call after the OpenEmbedded build
      system has installed packages. You can specify functions separated by
      spaces::

         ROOTFS_POSTINSTALL_COMMAND += "function"

      If you need to pass the root filesystem path to a command within a
      function, you can use `${IMAGE_ROOTFS}`, which points to the
      directory that becomes the root filesystem image. See the
      **IMAGE_ROOTFS** variable for more
      information.

   **ROOTFS_POSTPROCESS_COMMAND**
      Specifies a list of functions to call once the OpenEmbedded build
      system has created the root filesystem. You can specify functions
      separated by spaces::

         ROOTFS_POSTPROCESS_COMMAND += "function"

      If you need to pass the root filesystem path to a command within a
      function, you can use `${IMAGE_ROOTFS}`, which points to the
      directory that becomes the root filesystem image. See the
      **IMAGE_ROOTFS** variable for more
      information.

   **ROOTFS_POSTUNINSTALL_COMMAND**
      Specifies a list of functions to call after the OpenEmbedded build
      system has removed unnecessary packages. When runtime package
      management is disabled in the image, several packages are removed
      including `base-passwd`, `shadow`, and `update-alternatives`.
      You can specify functions separated by spaces::

         ROOTFS_POSTUNINSTALL_COMMAND += "function"

      If you need to pass the root filesystem path to a command within a
      function, you can use `${IMAGE_ROOTFS}`, which points to the
      directory that becomes the root filesystem image. See the
      **IMAGE_ROOTFS** variable for more
      information.

   **ROOTFS_PREPROCESS_COMMAND**
      Specifies a list of functions to call before the OpenEmbedded build
      system has created the root filesystem. You can specify functions
      separated by spaces::

         ROOTFS_PREPROCESS_COMMAND += "function"

      If you need to pass the root filesystem path to a command within a
      function, you can use `${IMAGE_ROOTFS}`, which points to the
      directory that becomes the root filesystem image. See the
      **IMAGE_ROOTFS** variable for more
      information.

   **RPMBUILD_EXTRA_PARAMS**
      Specifies extra user-defined parameters for the `rpmbuild` command.

   **RPROVIDES**
      A list of package name aliases that a package also provides. These
      aliases are useful for satisfying runtime dependencies of other
      packages both during the build and on the target (as specified by
      **RDEPENDS**).

      .. note::

         A package's own name is implicitly already in its **RPROVIDES** list.

      As with all package-controlling variables, you must always use the
      variable in conjunction with a package name override. Here is an
      example::

         RPROVIDES:${PN} = "widget-abi-2"

   **RRECOMMENDS**
      A list of packages that extends the usability of a package being
      built. The package being built does not depend on this list of
      packages in order to successfully build, but rather uses them for
      extended usability. To specify runtime dependencies for packages, see
      the **RDEPENDS** variable.

      The package manager will automatically install the **RRECOMMENDS**
      list of packages when installing the built package. However, you can
      prevent listed packages from being installed by using the
      **BAD_RECOMMENDATIONS**,
      **NO_RECOMMENDATIONS**, and
      **PACKAGE_EXCLUDE** variables.

      Packages specified in **RRECOMMENDS** need not actually be produced.
      However, there must be a recipe providing each package, either
      through the **PACKAGES** or
      **PACKAGES_DYNAMIC** variables or the
      **RPROVIDES** variable, or an error will occur
      during the build. If such a recipe does exist and the package is not
      produced, the build continues without error.

      Because the **RRECOMMENDS** variable applies to packages being built,
      you should always attach an override to the variable to specify the
      particular package whose usability is being extended. For example,
      suppose you are building a development package that is extended to
      support wireless functionality. In this case, you would use the
      following::

         RRECOMMENDS:${PN}-dev += "wireless_package_name"

      In the
      example, the package name (`${PN}-dev`) must appear as it would in
      the **PACKAGES** namespace before any renaming of the output package
      by classes such as ref-classes-debian.

      BitBake, which the OpenEmbedded build system uses, supports
      specifying versioned recommends. Although the syntax varies depending
      on the packaging format, BitBake hides these differences from you.
      Here is the general syntax to specify versions with the
      **RRECOMMENDS** variable::

         RRECOMMENDS:${PN} = "package (operator version)"

      For `operator`, you can specify the following:

      - =
      - <
      - >
      - <=
      - >=

      For example, the following sets up a recommend on version 1.2 or
      greater of the package `foo`::

         RRECOMMENDS:${PN} = "foo (>= 1.2)"

   **RREPLACES**
      A list of packages replaced by a package. The package manager uses
      this variable to determine which package should be installed to
      replace other package(s) during an upgrade. In order to also have the
      other package(s) removed at the same time, you must add the name of
      the other package to the **RCONFLICTS** variable.

      As with all package-controlling variables, you must use this variable
      in conjunction with a package name override. Here is an example::

         RREPLACES:${PN} = "other_package_being_replaced"

      BitBake, which the OpenEmbedded build system uses, supports
      specifying versioned replacements. Although the syntax varies
      depending on the packaging format, BitBake hides these differences
      from you. Here is the general syntax to specify versions with the
      **RREPLACES** variable::

         RREPLACES:${PN} = "package (operator version)"

      For `operator`, you can specify the following:

      - =
      - <
      - >
      - <=
      - >=

      For example, the following sets up a replacement using version 1.2
      or greater of the package `foo`::

          RREPLACES:${PN} = "foo (>= 1.2)"

   **RSUGGESTS**
      A list of additional packages that you can suggest for installation
      by the package manager at the time a package is installed. Not all
      package managers support this functionality.

      As with all package-controlling variables, you must always use this
      variable in conjunction with a package name override. Here is an
      example::

         RSUGGESTS:${PN} = "useful_package another_package"

   **RUST_CHANNEL**
      Specifies which version of Rust to build - "stable", "beta" or "nightly".
      The default value is "stable". Set this at your own risk, as values other
      than "stable" are not guaranteed to work at a given time.

   **S**
      The location in the **Build Directory** where
      unpacked recipe source code resides. By default, this directory is
      `${`\ **UNPACKDIR**\ `}/${`\ **BPN**\ `}-${`\ **PV**\ `}`,
      where `${BPN}` is the base recipe name and `${PV}` is the recipe
      version. If the source tarball extracts the code to a directory named
      anything other than `${BPN}-${PV}`, or if the source code is
      fetched from an SCM such as Git or Subversion, then you must set
      **S** in the recipe so that the OpenEmbedded build system knows where
      to find the unpacked source.

      As an example, assume a **Source Directory**
      top-level folder named `bitbake-builds` and a default **Build Directory** at
      `bitbake-builds/build`. In this case, the work directory the build system
      uses to keep the unpacked recipe for `db` is the following::

         bitbake-builds/build/tmp/work/qemux86-poky-linux/db/5.1.19-r3/sources/db-5.1.19

      The unpacked source code resides in the `db-5.1.19` folder.

   **SANITY_REQUIRED_UTILITIES**
      Specifies a list of command-line utilities that should be checked for
      during the initial sanity checking process when running BitBake. If
      any of the utilities are not installed on the build host, then
      BitBake immediately exits with an error.

   **SANITY_TESTED_DISTROS**
      A list of the host distribution identifiers that the build system has
      been tested against. Identifiers consist of the host distributor ID
      followed by the release, as reported by the `lsb_release` tool or
      as read from `/etc/lsb-release`. Separate the list items with
      explicit newline characters (`\n`). If **SANITY_TESTED_DISTROS** is
      not empty and the current value of
      **NATIVELSBSTRING** does not appear in the
      list, then the build system reports a warning that indicates the
      current host distribution has not been tested as a build host.

   **SDK_ARCH**
      The target architecture for the SDK. Typically, you do not directly
      set this variable. Instead, use **SDKMACHINE**.

   **SDK_ARCHIVE_TYPE**
      Specifies the type of archive to create for the SDK. Valid values:

      - `tar.xz` (default)
      - `tar.zst`
      - `7zip`
      - `zip`

      Only one archive type can be specified.

   **SDK_AS_ARCH**
      Specifies architecture-specific assembler flags when building
      ref-classes-nativesdk recipes. By default, the value of
      **SDK_AS_ARCH** equals the one of **BUILD_AS_ARCH**.

   **SDK_BUILDINFO_FILE**
      When using the ref-classes-image-buildinfo class,
      specifies the file in the SDK to write the build information into. The
      default value is "`/buildinfo`".

   **SDK_CC_ARCH**
      Specifies the architecture-specific C compiler flags when building
      ref-classes-nativesdk recipes. By default, the value of
      **SDK_CC_ARCH** equals the one of **BUILD_CC_ARCH**.

   **SDK_CUSTOM_TEMPLATECONF**
      When building the extensible SDK, if **SDK_CUSTOM_TEMPLATECONF** is set to
      "1" and a `conf/templateconf.cfg` file exists in the **Build Directory**
      (**TOPDIR**) then this will be copied into the SDK.

   **SDK_DEPLOY**
      The directory set up and used by the
      populate_sdk_base  class to which the
      SDK is deployed. The populate_sdk_base 
      class defines **SDK_DEPLOY** as follows::

         SDK_DEPLOY = "${TMPDIR}/deploy/sdk"

   **SDK_DIR**
      The parent directory used by the OpenEmbedded build system when
      creating SDK output. The
      populate_sdk_base  class defines
      the variable as follows::

         SDK_DIR = "${WORKDIR}/sdk"

      .. note::

         The **SDK_DIR** directory is a temporary directory as it is part of
         **WORKDIR**. The final output directory is **SDK_DEPLOY**.

   **SDK_EXT_TYPE**
      Controls whether or not shared state artifacts are copied into the
      extensible SDK. The default value of "full" copies all of the
      required shared state artifacts into the extensible SDK. The value
      "minimal" leaves these artifacts out of the SDK.

      .. note::

         If you set the variable to "minimal", you need to ensure
         **SSTATE_MIRRORS** is set in the SDK's configuration to enable the
         artifacts to be fetched as needed.

   **SDK_HOST_MANIFEST**
      The manifest file for the host part of the SDK. This file lists all
      the installed packages that make up the host part of the SDK. The
      file contains package information on a line-per-package basis as
      follows::

         packagename packagearch version

      The populate_sdk_base  class
      defines the manifest file as follows::

         SDK_HOST_MANIFEST = "${SDK_DEPLOY}/${TOOLCHAIN_OUTPUTNAME}.host.manifest"

      The location is derived using the **SDK_DEPLOY** and
      **TOOLCHAIN_OUTPUTNAME** variables.

   **SDK_INCLUDE_PKGDATA**
      When set to "1", specifies to include the packagedata for all recipes
      in the "world" target in the extensible SDK. Including this data
      allows the `devtool search` command to find these recipes in search
      results, as well as allows the `devtool add` command to map
      dependencies more effectively.

      .. note::

         Enabling the **SDK_INCLUDE_PKGDATA**
         variable significantly increases build time because all of world
         needs to be built. Enabling the variable also slightly increases
         the size of the extensible SDK.

   **SDK_INCLUDE_TOOLCHAIN**
      When set to "1", specifies to include the toolchain in the extensible
      SDK. Including the toolchain is useful particularly when
      **SDK_EXT_TYPE** is set to "minimal" to keep
      the SDK reasonably small but you still want to provide a usable
      toolchain. For example, suppose you want to use the toolchain from an
      IDE or from other tools and you do not want to perform additional
      steps to install the toolchain.

      The **SDK_INCLUDE_TOOLCHAIN** variable defaults to "0" if
      **SDK_EXT_TYPE** is set to "minimal", and defaults to "1" if
      **SDK_EXT_TYPE** is set to "full".

   **SDK_LD_ARCH**
      Specifies architecture-specific linker flags when building
      ref-classes-nativesdk recipes. By default, the value of
      **SDK_LD_ARCH** equals the one of **BUILD_LD_ARCH**.

   **SDK_NAME**
      The base name for SDK output files. The default value (as set in
      `meta-poky/conf/distro/poky.conf`) is derived from the
      **DISTRO**,
      **TCLIBC**,
      **SDKMACHINE**,
      **IMAGE_BASENAME**,
      **TUNE_PKGARCH**, and
      **MACHINE** variables::

         SDK_NAME = "${DISTRO}-${TCLIBC}-${SDKMACHINE}-${IMAGE_BASENAME}-${TUNE_PKGARCH}-${MACHINE}"

   **SDK_OS**
      Specifies the operating system for which the SDK will be built. The
      default value is the value of **BUILD_OS**.

   **SDK_OUTPUT**
      The location used by the OpenEmbedded build system when creating SDK
      output. The populate_sdk_base 
      class defines the variable as follows::

         SDK_DIR = "${WORKDIR}/sdk"
         SDK_OUTPUT = "${SDK_DIR}/image"
         SDK_DEPLOY = "${DEPLOY_DIR}/sdk"

      .. note::

         The **SDK_OUTPUT** directory is a temporary directory as it is part of
         **WORKDIR** by way of **SDK_DIR**. The final output directory is
         **SDK_DEPLOY**.

   **SDK_PACKAGE_ARCHS**
      Specifies a list of architectures compatible with the SDK machine.
      This variable is set automatically and should not normally be
      hand-edited. Entries are separated using spaces and listed in order
      of priority. The default value for **SDK_PACKAGE_ARCHS** is "all any
      noarch ${SDK_ARCH}-${SDKPKGSUFFIX}".

   **SDK_POSTPROCESS_COMMAND**
      Specifies a list of functions to call once the OpenEmbedded build
      system creates the SDK. You can specify functions separated by
      spaces:

         SDK_POSTPROCESS_COMMAND += "function"

      If you need to pass an SDK path to a command within a function, you
      can use `${SDK_DIR}`, which points to the parent directory used by
      the OpenEmbedded build system when creating SDK output. See the
      **SDK_DIR** variable for more information.

   **SDK_PREFIX**
      The toolchain binary prefix used for
      ref-classes-nativesdk recipes. The
      OpenEmbedded build system uses the **SDK_PREFIX** value to set the
      **TARGET_PREFIX** when building
      `nativesdk` recipes. The default value is "${SDK_SYS}-".

   **SDK_RECRDEP_TASKS**
      A list of shared state tasks added to the extensible SDK. By default,
      the following tasks are added:

      - ref-tasks-populate_lic
      - ref-tasks-package_qa
      - ref-tasks-populate_sysroot
      - ref-tasks-deploy

      Despite the default value of "" for the
      **SDK_RECRDEP_TASKS** variable, the above four tasks are always added
      to the SDK. To specify tasks beyond these four, you need to use the
      **SDK_RECRDEP_TASKS** variable (e.g. you are defining additional
      tasks that are needed in order to build
      **SDK_TARGETS**).

   **SDK_SYS**
      Specifies the system, including the architecture and the operating
      system, for which the SDK will be built.

      The OpenEmbedded build system automatically sets this variable based
      on **SDK_ARCH**,
      **SDK_VENDOR**, and
      **SDK_OS**. You do not need to set the **SDK_SYS**
      variable yourself.

   **SDK_TARGET_MANIFEST**
      The manifest file for the target part of the SDK. This file lists all
      the installed packages that make up the target part of the SDK. The
      file contains package information on a line-per-package basis as
      follows::

         packagename packagearch version

      The populate_sdk_base  class
      defines the manifest file as follows::

         SDK_TARGET_MANIFEST = "${SDK_DEPLOY}/${TOOLCHAIN_OUTPUTNAME}.target.manifest"

      The location is derived using the **SDK_DEPLOY** and
      **TOOLCHAIN_OUTPUTNAME** variables.

   **SDK_TARGETS**
      A list of targets to install from shared state as part of the
      standard or extensible SDK installation. The default value is "${PN}"
      (i.e. the image from which the SDK is built).

      The **SDK_TARGETS** variable is an internal variable and typically
      would not be changed.

   **SDK_TITLE**
      The title to be printed when running the SDK installer. By default,
      this title is based on the **DISTRO_NAME** or
      **DISTRO** variable and is set in the
      populate_sdk_base  class as
      follows::

         SDK_TITLE ??= "${@d.getVar('DISTRO_NAME') or d.getVar('DISTRO')} SDK"

      For the **poky** reference distribution,
      **SDK_TITLE** is set to "Poky (Yocto Project Reference Distro)".

      For information on how to change this default title, see the
      "sdk-manual/appendix-customizing:changing the extensible sdk installer title"
      section in the Yocto Project Application Development and the
      Extensible Software Development Kit (eSDK) manual.

   **SDK_TOOLCHAIN_LANGS**
      Specifies programming languages to support in the SDK, as a
      space-separated list. Currently supported items are `rust` and `go`.

   **SDK_UPDATE_URL**
      An optional URL for an update server for the extensible SDK. If set,
      the value is used as the default update server when running
      `devtool sdk-update` within the extensible SDK.

   **SDK_VENDOR**
      Specifies the name of the SDK vendor.

   **SDK_VERSION**
      Specifies the version of the SDK. The Poky distribution configuration file
      (`/meta-poky/conf/distro/poky.conf`) sets the default
      **SDK_VERSION** as follows::

         SDK_VERSION = "${@d.getVar('DISTRO_VERSION').replace('snapshot-${METADATA_REVISION}', 'snapshot')}"

      For additional information, see the
      **DISTRO_VERSION** and
      **METADATA_REVISION** variables.

   **SDK_ZIP_OPTIONS**
      Specifies extra options to pass to the `zip` command when zipping the SDK
      (i.e. when **SDK_ARCHIVE_TYPE** is set to "zip"). The default value is
      "-y".

   **SDKEXTPATH**
      The default installation directory for the Extensible SDK. By
      default, this directory is based on the **DISTRO**
      variable and is set in the
      populate_sdk_base  class as
      follows::

         SDKEXTPATH ??= "~/${@d.getVar('DISTRO')}_sdk"

      For the
      **Poky** reference distro, the **SDKEXTPATH** is set to "poky_sdk".

      For information on how to change this default directory, see the
      "sdk-manual/appendix-customizing:changing the default sdk installation directory"
      section in the Yocto Project Application Development and the
      Extensible Software Development Kit (eSDK) manual.

   **SDKIMAGE_FEATURES**
      Equivalent to **IMAGE_FEATURES**. However, this variable applies to
      the SDK generated from an image using the following command::

         $ bitbake -c populate_sdk imagename

   **SDKMACHINE**
      The machine for which the SDK is built. In other words, the SDK is built
      such that it runs on the target you specify with the **SDKMACHINE**
      value. The value points to a corresponding `.conf` file under
      `conf/machine-sdk/` in the enabled layers, for example `aarch64`,
      `i586`, `i686`, `ppc64`, `ppc64le`, and `x86_64` are
      available in OpenEmbedded-Core .

      The variable defaults to **BUILD_ARCH** so that SDKs are built for the
      architecture of the build machine.

      .. note::

         You cannot set the **SDKMACHINE**
         variable in your distribution configuration file. If you do, the
         configuration will not take effect.

   **SDKPATH**
      Defines the path used to collect the SDK components and build the
      installer.

   **SDKPATHINSTALL**
      Defines the path offered to the user for installation of the SDK that
      is generated by the OpenEmbedded build system. The path appears as
      the default location for installing the SDK when you run the SDK's
      installation script. You can override the offered path when you run
      the script.

   **SDKTARGETSYSROOT**
      The full path to the sysroot used for cross-compilation within an SDK
      as it will be when installed into the default
      **SDKPATHINSTALL**.

   **SECTION**
      The section in which packages should be categorized. Package
      management utilities can make use of this variable.

   **SELECTED_OPTIMIZATION**
      Specifies the optimization flags passed to the C compiler when
      building for the target. The flags are passed through the default
      value of the **TARGET_CFLAGS** variable.

      The **SELECTED_OPTIMIZATION** variable takes the value of
      **FULL_OPTIMIZATION** unless **DEBUG_BUILD** = "1", in which
      case the value of **DEBUG_OPTIMIZATION** is used.

   **SERIAL_CONSOLES**
      Defines a serial console (TTY) to enable using
      getty . Provide a value that specifies the
      baud rate followed by the TTY device name separated by a semicolon.
      Use spaces to separate multiple devices::

         SERIAL_CONSOLES = "115200;ttyS0 115200;ttyS1"

   **SETUPTOOLS_BUILD_ARGS**
      When used by recipes that inherit the ref-classes-setuptools3
      class, this variable can be used to specify additional arguments to be
      passed to `setup.py build` in the `setuptools3_do_compile()` task.

   **SETUPTOOLS_SETUP_PATH**
      When used by recipes that inherit the ref-classes-setuptools3
      class, this variable should be used to specify the directory in which
      the `setup.py` file is located if it is not at the root of the source
      tree (as specified by **S**). For example, in a recipe where the
      sources are fetched from a Git repository and `setup.py` is in a
      `python/pythonmodule` subdirectory, you would have this::

         SETUPTOOLS_SETUP_PATH = "${S}/python/pythonmodule"

   **SIGGEN_EXCLUDE_SAFE_RECIPE_DEPS**
      A list of recipe dependencies that should not be used to determine
      signatures of tasks from one recipe when they depend on tasks from
      another recipe. For example::

         SIGGEN_EXCLUDE_SAFE_RECIPE_DEPS += "intone->mplayer2"

      In the previous example, `intone` depends on `mplayer2`.

      You can use the special token `"*"` on the left-hand side of the
      dependency to match all recipes except the one on the right-hand
      side. Here is an example::

         SIGGEN_EXCLUDE_SAFE_RECIPE_DEPS += "*->quilt-native"

      In the previous example, all recipes except `quilt-native` ignore
      task signatures from the `quilt-native` recipe when determining
      their task signatures.

      Use of this variable is one mechanism to remove dependencies that
      affect task signatures and thus force rebuilds when a recipe changes.

      .. note::

         If you add an inappropriate dependency for a recipe relationship,
         the software might break during runtime if the interface of the
         second recipe was changed after the first recipe had been built.

   **SIGGEN_EXCLUDERECIPES_ABISAFE**
      A list of recipes that are completely stable and will never change.
      The ABI for the recipes in the list are presented by output from the
      tasks run to build the recipe. Use of this variable is one way to
      remove dependencies from one recipe on another that affect task
      signatures and thus force rebuilds when the recipe changes.

      .. note::

         If you add an inappropriate variable to this list, the software
         might break at runtime if the interface of the recipe was changed
         after the other had been built.

   **SIGGEN_LOCKEDSIGS**
     The list of locked tasks, with the form::

       SIGGEN_LOCKEDSIGS += "<package>:<task>:<signature>"

     If `<signature>` exists for the specified `<task>` and `<package>`
     in the sstate cache, BitBake will use the cached output instead of
     rebuilding the `<task>`. If it does not exist, BitBake will build the
     `<task>` and the sstate cache will be used next time.

     Example::

       SIGGEN_LOCKEDSIGS += "bc:do_compile:09772aa4532512baf96d433484f27234d4b7c11dd9cda0d6f56fa1b7ce6f25f0"

     You can obtain the signature of all the tasks for the recipe `bc` using::

       bitbake -S none bc

     Then you can look at files in `build/tmp/stamps/<arch>/bc` and look for
     files like: `<PV>.do_compile.sigdata.09772aa4532512baf96d433484f27234d4b7c11dd9cda0d6f56fa1b7ce6f25f0`.

     Alternatively, you can also use bblock  to
     generate this line for you.

   **SIGGEN_LOCKEDSIGS_TASKSIG_CHECK**
     Specifies the debug level of task signature check. 3 levels are supported:

     * `info`: displays a "Note" message to remind the user that a task is locked
       and the current signature matches the locked one.
     * `warn`: displays a "Warning" message if a task is locked and the current
       signature does not match the locked one.
     * `error`: same as warn but displays an "Error" message and aborts.

   **SIGGEN_LOCKEDSIGS_TYPES**
     Allowed overrides for **SIGGEN_LOCKEDSIGS**. This is mainly used
     for architecture specific locks. A common value for
     **SIGGEN_LOCKEDSIGS_TYPES** is `${PACKAGE_ARCHS}`::

       SIGGEN_LOCKEDSIGS_TYPES += "${PACKAGE_ARCHS}"

       SIGGEN_LOCKEDSIGS_core2-64 += "bc:do_compile:09772aa4532512baf96d433484f27234d4b7c11dd9cda0d6f56fa1b7ce6f25f0"
       SIGGEN_LOCKEDSIGS_cortexa57 += "bc:do_compile:12178eb6d55ef602a8fe638e49862fd247e07b228f0f08967697b655bfe4bb61"

     Here, the `do_compile` task from `bc` will be locked only for
     `core2-64` and `cortexa57` but not for other architectures such as
     `mips32r2`.

   **SITEINFO_BITS**
      Specifies the number of bits for the target system CPU. The value
      should be either "32" or "64".

   **SITEINFO_ENDIANNESS**
      Specifies the endian byte order of the target system. The value
      should be either "le" for little-endian or "be" for big-endian.

   **SKIP_FILEDEPS**
      Enables removal of all files from the "Provides" section of an RPM
      package. Removal of these files is required for packages containing
      prebuilt binaries and libraries such as `libstdc++` and `glibc`.

      To enable file removal, set the variable to "1" in your
      `conf/local.conf` configuration file in your:
      **Build Directory**::

         SKIP_FILEDEPS = "1"

   **SKIP_RECIPE**
      Used to prevent the OpenEmbedded build system from building a given
      recipe. Specify the **PN** value as a variable flag (`varflag`)
      and provide a reason, which will be reported when attempting to
      build the recipe.

      To prevent a recipe from being built, use the **SKIP_RECIPE**
      variable in your `local.conf` file or distribution configuration.
      Here is an example which prevents `myrecipe` from being built::

         SKIP_RECIPE[myrecipe] = "Not supported by our organization."

   **SOC_FAMILY**
      A colon-separated list grouping together machines based upon the same
      family of SoC (System On Chip). You typically set this variable in a
      common `.inc` file that you include in the configuration files of all
      the machines.

      .. note::

         You must include `conf/machine/include/soc-family.inc` for this
         variable to appear in **MACHINEOVERRIDES**.

   **SOLIBS**
      Defines the suffix for shared libraries used on the target platform.
      By default, this suffix is ".so.*" for all Linux-based systems and is
      defined in the `meta/conf/bitbake.conf` configuration file.

      You will see this variable referenced in the default values of
      `FILES:${PN}`.

   **SOLIBSDEV**
      Defines the suffix for the development symbolic link (symlink) for
      shared libraries on the target platform. By default, this suffix is
      ".so" for Linux-based systems and is defined in the
      `meta/conf/bitbake.conf` configuration file.

      You will see this variable referenced in the default values of
      `FILES:${PN}-dev`.

   **SOURCE_DATE_EPOCH**
      This defines a date expressed in number of seconds since
      the UNIX EPOCH (01 Jan 1970 00:00:00 UTC), which is used by
      multiple build systems to force a timestamp in built binaries.
      Many upstream projects already support this variable.

      You will find more details in the `official specifications
      <https://reproducible-builds.org/specs/source-date-epoch/>`__.

      A value for each recipe is computed from the sources by
      meta/lib/oe/reproducible.py .

      If a recipe wishes to override the default behavior, it should set its
      own **SOURCE_DATE_EPOCH** value::

          SOURCE_DATE_EPOCH = "1613559011"

   **SOURCE_MIRROR_FETCH**
      When you are fetching files to create a mirror of sources (i.e.
      creating a source mirror), setting **SOURCE_MIRROR_FETCH** to "1" in
      your `local.conf` configuration file ensures the source for all
      recipes are fetched regardless of whether or not a recipe is
      compatible with the configuration. A recipe is considered
      incompatible with the currently configured machine when either or
      both the **COMPATIBLE_MACHINE**
      variable and **COMPATIBLE_HOST** variables
      specify compatibility with a machine other than that of the current
      machine or host.

      .. note::

         Do not set the **SOURCE_MIRROR_FETCH**
         variable unless you are creating a source mirror. In other words,
         do not set the variable during a normal build.

   **SOURCE_MIRROR_URL**
      Defines your own **PREMIRRORS** from which to
      first fetch source before attempting to fetch from the upstream
      specified in **SRC_URI**.

      To use this variable, you must globally inherit the
      ref-classes-own-mirrors class and then provide
      the URL to your mirrors. Here is the general syntax::

         INHERIT += "own-mirrors"
         SOURCE_MIRROR_URL = "http://example.com/my_source_mirror"

      .. note::

         You can specify only a single URL in **SOURCE_MIRROR_URL**.

      .. note::

         If the mirror is protected behind a username and password, the
         **build host** needs to be configured so the :term:`build system
         <OpenEmbedded Build System>` is able to fetch from the mirror.

         The recommended way to do that is by setting the following parameters
         in `$HOME/.netrc` (`$HOME` being the **build host** home
         directory)::

            machine example.com
            login <user>
            password <password>

         This file requires permissions set to `400` or `600` to prevent
         other users from reading the file::

            chmod 600 "$HOME/.netrc"

         Another method to configure the username and password is from the URL
         in **SOURCE_MIRROR_URL** directly, with the `user` and `pswd`
         parameters::

            SOURCE_MIRROR_URL = "http://example.com/my_source_mirror;user=<user>;pswd=<password>"

   **SPDX_ARCHIVE_PACKAGED**
      This option allows to add to **SPDX** output compressed archives
      of the files in the generated target packages.

      Such archives are available in
      `tmp/deploy/spdx/MACHINE/packages/packagename.tar.zst`
      under the **Build Directory**.

      Enable this option as follows::

         SPDX_ARCHIVE_PACKAGED = "1"

      According to our tests on release 4.1 "langdale", building
      `core-image-minimal` for the `qemux86-64` machine, enabling this
      option multiplied the size of the `tmp/deploy/spdx` directory by a
      factor of 13 (+1.6 GiB for this image), compared to just using the
      ref-classes-create-spdx class with no option.

      Note that this option doesn't increase the size of **SPDX**
      files in `tmp/deploy/images/MACHINE`.

   **SPDX_ARCHIVE_SOURCES**
      This option allows to add to **SPDX** output compressed archives
      of the sources for packages installed on the target. It currently
      only works when **SPDX_INCLUDE_SOURCES** is set.

      This is one way of fulfilling "source code access" license
      requirements.

      Such source archives are available in
      `tmp/deploy/spdx/MACHINE/recipes/recipe-packagename.tar.zst`
      under the **Build Directory**.

      Enable this option as follows::

         SPDX_INCLUDE_SOURCES = "1"
         SPDX_ARCHIVE_SOURCES = "1"

      According to our tests on release 4.1 "langdale", building
      `core-image-minimal` for the `qemux86-64` machine, enabling
      these options multiplied the size of the `tmp/deploy/spdx`
      directory by a factor of 11 (+1.4 GiB for this image),
      compared to just using the ref-classes-create-spdx
      class with no option.

      Note that using this option only marginally increases the size
      of the **SPDX** output in `tmp/deploy/images/MACHINE/`
      (+ 0.07\% with the tested image), compared to just enabling
      **SPDX_INCLUDE_SOURCES**.

   **SPDX_BUILD_HOST**
      The base variable name describing the build host on which the build is
      running. The value must name a key from `SPDX_IMPORTS`, allowing
      the generated SPDX to reference externally defined host identity data.

      Requires **SPDX_INCLUDE_BITBAKE_PARENT_BUILD** to be set to `"1"`.

      .. warning::

         Setting this variable will result in non-reproducible SPDX output,
         because the build host identity may vary across builds.

      See also **SPDX_INCLUDE_BITBAKE_PARENT_BUILD**,
      `SPDX_IMPORTS`, **SPDX_INVOKED_BY**,
      and **SPDX_ON_BEHALF_OF**.

   **SPDX_CONCLUDED_LICENSE**
      The **SPDX_CONCLUDED_LICENSE** variable allows overriding the
      `hasConcludedLicense` object to individual SBOM packages. This can be
      used when the license of a package was determined to be different than the
      original license string value, after analysis.

      This variable can be set in two ways:

      -  For the entire recipe::

            SPDX_CONCLUDED_LICENSE = "MIT & Apache-2.0"

      -  For an individual package produced by the recipe::

            SPDX_CONCLUDED_LICENSE:${PN} = "MIT & Apache-2.0"

   **SPDX_CUSTOM_ANNOTATION_VARS**
      This option allows to associate `SPDX annotations
      <https://spdx.github.io/spdx-spec/v2.3/annotations/>`__ to a recipe,
      using the values of variables in the recipe::

         ANNOTATION1 = "First annotation for recipe"
         ANNOTATION2 = "Second annotation for recipe"
         SPDX_CUSTOM_ANNOTATION_VARS = "ANNOTATION1 ANNOTATION2"

      This will add a new block to the recipe `.sdpx.json` output::

         "annotations": [
           {
             "annotationDate": "2023-04-18T08:32:12Z",
             "annotationType": "OTHER",
             "annotator": "Tool: oe-spdx-creator - 1.0",
             "comment": "ANNOTATION1=First annotation for recipe"
           },
           {
             "annotationDate": "2023-04-18T08:32:12Z",
             "annotationType": "OTHER",
             "annotator": "Tool: oe-spdx-creator - 1.0",
             "comment": "ANNOTATION2=Second annotation for recipe"
           }
         ],

   **SPDX_FILE_EXCLUDE_PATTERNS**
      A space-separated list of Python regular expressions used to exclude files
      from the SPDX output when **SPDX_INCLUDE_SOURCES** is enabled.
      Files whose paths match any of the patterns, via `re.search()`, are
      filtered out from the generated SBOM.

      By default this variable is empty, meaning no files are excluded.

      Example usage::

         SPDX_FILE_EXCLUDE_PATTERNS = "\\.patch$ \\.diff$ /test/ \\.pyc$ \\.o$"

      See also **SPDX_INCLUDE_SOURCES**.

   **SPDX_IMAGE_SUPPLIER**
      The name of an agent variable prefix describing the organization or
      person who supplies the image SBOM. When set, the supplier is attached
      to all root elements of the image SBOM using the `suppliedBy` property.

      The value of this variable is the base prefix used to look up the
      agent's details. The following sub-variables are read using that prefix:

      -  `<PREFIX>_name`: display name of the supplier (required)
      -  `<PREFIX>_type`: agent type: `organization`, `person`,
         `software`, or `agent` (optional, defaults to `agent`)
      -  `<PREFIX>_comment`: free-text comment (optional)
      -  `<PREFIX>_id_email`: contact e-mail address (optional)

      The simplest approach is to use the variable itself as its own prefix,
      so the sub-variable names follow directly from
      `SPDX_IMAGE_SUPPLIER`.

      Example (set in the image recipe or in a **configuration file**)::

         SPDX_IMAGE_SUPPLIER = "SPDX_IMAGE_SUPPLIER"
         SPDX_IMAGE_SUPPLIER_name = "Acme Corp"
         SPDX_IMAGE_SUPPLIER_type = "organization"

      Alternatively, you can use any other prefix name, which is useful for
      sharing an agent definition across multiple supplier variables::

         MY_COMPANY_name = "Acme Corp"
         MY_COMPANY_type = "organization"
         SPDX_IMAGE_SUPPLIER = "MY_COMPANY"
         SPDX_SDK_SUPPLIER = "MY_COMPANY"

      If not set, no supplier information is added to the image SBOM.

      See also **SPDX_PACKAGE_SUPPLIER** and **SPDX_SDK_SUPPLIER**.

   **SPDX_INCLUDE_BITBAKE_PARENT_BUILD**
      When set to `"1"`, the SPDX output will include a `Build` object
      representing the parent **BitBake** invocation. This allows consumers
      of the SBOM to trace which CI/CD job or orchestration system triggered
      the build.

      This variable is required for **SPDX_INVOKED_BY**,
      **SPDX_ON_BEHALF_OF**, and **SPDX_BUILD_HOST** to have any
      effect.

      .. warning::

         Enabling this variable will result in non-reproducible SPDX output,
         because the build invocation identity changes with every run.

      Enable as follows::

         SPDX_INCLUDE_BITBAKE_PARENT_BUILD = "1"

      See also **SPDX_BUILD_HOST**, **SPDX_INVOKED_BY**,
      and **SPDX_ON_BEHALF_OF**.

   **SPDX_INCLUDE_COMPILED_SOURCES**
      This option allows the same as **SPDX_INCLUDE_SOURCES** but including
      only the sources used to compile the host tools and the target packages.
      While **SPDX_INCLUDE_SOURCES** includes all files in the source
      directory as source file descriptions, **SPDX_INCLUDE_COMPILED_SOURCES**
      includes only the sources that are used to produce the binaries delivered
      as packages. The source files that are not used during compilation are not
      included in the SBOM. It uses debugsource information generated during
      `do_package` to filter out source files.

      This enables an external tool to use the SPDX information to disregard
      vulnerabilities that are not compiled in the packages.

      Enable this option as follows::

         SPDX_INCLUDE_COMPILED_SOURCES = "1"

      For SPDX 2.2 format (release 4.1 "langdale"), building
      `core-image-minimal` for the `qemux86-64` machine, this option
      reduced the size of the `tmp/deploy/spdx` directory from 2GB to
      1.6GB compared to **SPDX_INCLUDE_SOURCES**, as it includes only
      compiled objects without original source files.

      With SPDX 3.0.1 JSON format, enabling this option includes both
      compiled sources and original source files (same as
      `SPDX_INCLUDE_SOURCES = "1"`), which significantly increases
      the SBOM size. For example, with `core-image-minimal` on
      `qemux86-64`, the uncompressed SBOM file can grow from hundreds
      of megabytes to several gigabytes.

   **SPDX_INCLUDE_KERNEL_CONFIG**
      This option allows exporting the Linux kernel configuration
      (the contents of the `.config` file) into the recipe's SPDX
      document as a separate `build_Build` object. Each kernel
      configuration parameter (`CONFIG_*`) is recorded and linked to
      the main kernel object using an `ancestorOf` relationship.

      .. note::

         This variable only has effect when using the SPDX 3.0 output
         format (see ref-classes-create-spdx).

      Enable this option as follows::

         SPDX_INCLUDE_KERNEL_CONFIG = "1"

      When enabled, a separate SPDX object is created for the kernel
      configuration, improving reproducibility, compliance tracking,
      and auditing of build-time kernel features.

   **SPDX_INCLUDE_PACKAGECONFIG**
      This option allows exporting a recipe's **PACKAGECONFIG**
      features into the recipe's SPDX document. Each feature is
      recorded as a `DictionaryEntry` with key
      `PACKAGECONFIG:<feature>` and value `enabled` or
      `disabled`, depending on whether the feature is active in
      the current build.

      .. note::

         This variable only has effect when using the SPDX 3.0 output
         format (see ref-classes-create-spdx).

      Enable this option as follows::

         SPDX_INCLUDE_PACKAGECONFIG = "1"

      When enabled, the build-time configuration of each recipe is
      captured in the SPDX document, improving transparency,
      reproducibility, and security auditing. It allows consumers of
      the SPDX SBOM to determine which optional features were
      enabled or disabled in a given build.

   **SPDX_INCLUDE_SOURCES**
      This option allows to add a description of the source files used to build
      the host tools and the target packages, to the `spdx.json` files in
      `tmp/deploy/spdx/MACHINE/recipes/` under the **Build Directory**.
      As a consequence, the `spdx.json` files under the `by-namespace` and
      `packages` subdirectories in `tmp/deploy/spdx/MACHINE` are also
      modified to include references to such source file descriptions.

      Enable this option as follows::

         SPDX_INCLUDE_SOURCES = "1"

      For SPDX 2.2 format (release 4.1 "langdale"), building
      `core-image-minimal` for the `qemux86-64` machine, enabling
      this option multiplied the total size of the `tmp/deploy/spdx`
      directory by a factor of 3  (+291 MiB for this image),
      and the size of the `IMAGE-MACHINE.spdx.tar.zst` in
      `tmp/deploy/images/MACHINE` by a factor of 130 (+15 MiB for this
      image), compared to just using the ref-classes-create-spdx class
      with no option.

      With SPDX 3.0.1 JSON format, including source files significantly
      increases the SBOM size (potentially by several gigabytes for typical
      images).

   **SPDX_INVOKED_BY**
      The base variable name describing the agent that invoked the build.
      Each `Build` object in the SPDX output is linked to this agent with an
      `invokedBy` relationship. Requires
      **SPDX_INCLUDE_BITBAKE_PARENT_BUILD** to be set to `"1"`.

      The sub-variables follow the same agent prefix convention as
      **SPDX_IMAGE_SUPPLIER**:

      -  `SPDX_INVOKED_BY_name`: display name of the invoking agent
      -  `SPDX_INVOKED_BY_type`: agent type, such as `software` for a CI system

      Example (CI pipeline invoking the build)::

         SPDX_INCLUDE_BITBAKE_PARENT_BUILD = "1"
         SPDX_INVOKED_BY = "SPDX_INVOKED_BY"
         SPDX_INVOKED_BY_name = "GitLab CI"
         SPDX_INVOKED_BY_type = "software"

      .. warning::

         Setting this variable will likely result in non-reproducible SPDX
         output, because the invoking agent identity varies across builds.

      See also **SPDX_INCLUDE_BITBAKE_PARENT_BUILD**,
      **SPDX_ON_BEHALF_OF**, and **SPDX_BUILD_HOST**.

   **SPDX_LICENSES**
      Path to the JSON file containing SPDX license identifier mappings.
      This file maps common license names to official SPDX license
      identifiers used during SBOM generation.

      The default value points to a copy of the license mappings defined
      by SPDX (https://github.com/spdx/license-list-data) stored in
      **OpenEmbedded-Core (OE-Core)**.

      You can override this variable to use a custom license mapping file
      if your organization uses different license naming conventions.

   **SPDX_MULTILIB_SSTATE_ARCHS**
      The list of sstate architectures to consider when collecting SPDX
      dependencies. This includes multilib architectures when multilib is
      enabled.

      The default value is set to the value of `SSTATE_ARCHS`.

      This variable is used internally by the SPDX generation classes to
      ensure all relevant dependencies are included in the SBOM,
      regardless of whether multilib is enabled or not.

   **SPDX_NAMESPACE_PREFIX**
      This option could be used in order to change the prefix of `spdxDocument`
      and the prefix of `documentNamespace`. It is set by default to
      `http://spdx.org/spdxdoc`.

   **SPDX_ON_BEHALF_OF**
      The base variable name describing the agent on whose behalf the invoking
      agent (**SPDX_INVOKED_BY**) is running the build. Requires
      **SPDX_INCLUDE_BITBAKE_PARENT_BUILD** to be set to `"1"`.
      Has no effect if **SPDX_INVOKED_BY** is not also set.

      The sub-variables follow the same agent prefix convention as
      **SPDX_IMAGE_SUPPLIER**:

      -  `SPDX_ON_BEHALF_OF_name`: display name of the commissioning agent
      -  `SPDX_ON_BEHALF_OF_type`: agent type, such as `organization`

      Example (CI system building on behalf of a customer organization)::

         SPDX_INCLUDE_BITBAKE_PARENT_BUILD = "1"
         SPDX_INVOKED_BY = "SPDX_INVOKED_BY"
         SPDX_INVOKED_BY_name = "GitLab CI"
         SPDX_INVOKED_BY_type = "software"
         SPDX_ON_BEHALF_OF = "SPDX_ON_BEHALF_OF"
         SPDX_ON_BEHALF_OF_name = "Acme Corp"
         SPDX_ON_BEHALF_OF_type = "organization"

      .. warning::

         Setting this variable will likely result in non-reproducible SPDX
         output, because the agent identity varies across builds.

      See also **SPDX_INCLUDE_BITBAKE_PARENT_BUILD**,
      **SPDX_INVOKED_BY**, and **SPDX_BUILD_HOST**.

   **SPDX_PACKAGE_SUPPLIER**
      The base variable name describing the agent who supplies the artifacts
      produced by the build. Works identically to **SPDX_IMAGE_SUPPLIER**
      but applies to individual packages rather than the image SBOM.

      Typically set in a distro **configuration file** to apply globally
      to all packages, or in a specific software recipe (or a `.bbappend`)
      to apply only to packages of that recipe. Recipe-level overrides
      (`SPDX_PACKAGE_SUPPLIER:pn-<recipe>`) are also supported::

         SPDX_PACKAGE_SUPPLIER = "SPDX_PACKAGE_SUPPLIER"
         SPDX_PACKAGE_SUPPLIER_name = "Acme Corp"
         SPDX_PACKAGE_SUPPLIER_type = "organization"

      See also **SPDX_IMAGE_SUPPLIER** and **SPDX_SDK_SUPPLIER**.

   **SPDX_PACKAGE_URL**
      Provides a place for the SPDX data creator to record the package URL
      string (`software_packageUrl`), in accordance with the Package URL
      specification) for a software Package. The default value of this variable
      is an empty string.

   **SPDX_PACKAGE_VERSION**
      This variable controls the package version as seen in the SPDX 3.0 JSON
      output (`software_packageVersion`). The default value for this variable
      is **PV**.

   **SPDX_PRETTY**
      This option makes the SPDX output more human-readable, using
      identation and newlines, instead of the default output in a
      single line::

         SPDX_PRETTY = "1"

      The generated SPDX files are approximately 20% bigger, but
      this option is recommended if you want to inspect the SPDX
      output files with a text editor.

   **SPDX_SDK_SUPPLIER**
      The base variable name describing the agent who supplies the SDK SBOM.
      When set, the supplier is attached to all root elements of the SDK
      SBOM using the `suppliedBy` property.

      Works identically to **SPDX_IMAGE_SUPPLIER** but applies to SDK
      builds. This includes image-based SDKs produced by
      `bitbake <image> -c populate_sdk` as well as toolchain SDKs produced
      by `bitbake meta-toolchain`.

      Typically set in the image recipe or in a **configuration file**.

      If not set, no supplier information is added to the SDK SBOM.

      See also **SPDX_IMAGE_SUPPLIER** and **SPDX_PACKAGE_SUPPLIER**.

   **SPDX_UUID_NAMESPACE**
      The namespace used for generating UUIDs in SPDX documents. This
      should be a domain name or unique identifier for your organization
      to ensure globally unique SPDX IDs across different builds and
      organizations.

      The default value is set to a domain managed by the OpenEmbedded
      project. You can override this to use your organization's domain.

   **SPDXLICENSEMAP**
      Maps commonly used license names to their SPDX counterparts found in
      `meta/files/common-licenses/`. For the default **SPDXLICENSEMAP**
      mappings, see the `meta/conf/licenses.conf` file.

      For additional information, see the **LICENSE**
      variable.

   **SPECIAL_PKGSUFFIX**
      A list of prefixes for **PN** used by the OpenEmbedded
      build system to create variants of recipes or packages. The list
      specifies the prefixes to strip off during certain circumstances such
      as the generation of the **BPN** variable.

   **SPL_BINARY**
      The file type for the Secondary Program Loader (SPL). Some devices
      use an SPL from which to boot (e.g. the BeagleBone development
      board). For such cases, you can declare the file type of the SPL
      binary in the `u-boot.inc` include file, which is used in the
      U-Boot recipe.

      The SPL file type is set to "null" by default in the `u-boot.inc`
      file as follows::

         # Some versions of u-boot build an SPL (Second Program Loader) image that
         # should be packaged along with the u-boot binary as well as placed in the
         # deploy directory. For those versions they can set the following variables
         # to allow packaging the SPL.
         SPL_BINARY ?= ""
         SPL_BINARYNAME ?= "${@os.path.basename(d.getVar("SPL_BINARY"))}"
         SPL_IMAGE ?= "${SPL_BINARYNAME}-${MACHINE}-${PV}-${PR}"
         SPL_SYMLINK ?= "${SPL_BINARYNAME}-${MACHINE}"

      The **SPL_BINARY** variable helps form
      various `SPL_*` variables used by the OpenEmbedded build system.

      See the BeagleBone machine configuration example in the
      "dev-manual/layers:adding a layer using the \\`bitbake-layers\`\` script`"
      section in the Yocto Project Board Support Package Developer's Guide
      for additional information.

   **SPL_DTB_BINARY**
      When inheriting the ref-classes-uboot-sign class, the
      **SPL_DTB_BINARY** variable contains the name of the SPL binary to be
      compiled.

   **SPL_MKIMAGE_DTCOPTS**
      Options for the device tree compiler passed to `mkimage -D` feature
      while creating a FIT image with the ref-classes-uboot-sign
      class. If **SPL_MKIMAGE_DTCOPTS** is not set then the
      ref-classes-uboot-sign class will not pass the `-D` option
      to `mkimage`.

      The default value is set to "" by the ref-classes-uboot-config
      class.

   **SPL_SIGN_ENABLE**
      Enable signing of the U-Boot FIT image. The default value is "0".
      This variable is used by the ref-classes-uboot-sign class.

   **SPL_SIGN_KEYDIR**
      Location of the directory containing the RSA key and certificate used for
      signing the U-Boot FIT image, used by the ref-classes-uboot-sign
      class.

   **SPL_SIGN_KEYNAME**
      The name of keys used by the ref-classes-uboot-sign class
      for signing U-Boot FIT image stored in the **SPL_SIGN_KEYDIR**
      directory. If we have for example a `dev.key` key and a `dev.crt`
      certificate stored in the **SPL_SIGN_KEYDIR** directory, you will
      have to set **SPL_SIGN_KEYNAME** to `dev`.

   **SPLASH**
      This variable, used by the ref-classes-image class, allows
      to choose splashscreen applications. Set it to the names of packages
      for such applications to use. This variable is set by default to
      `psplash`.

   **SPLASH_IMAGES**
      This variable, used by the `psplash` recipe, allows to customize
      the default splashscreen image.

      Specified images in PNG format are converted to `.h` files by the recipe,
      and are included in the `psplash` binary, so you won't find them in
      the root filesystem.

      To make such a change, it is recommended to customize the
      `psplash` recipe in a custom layer. Here is an example structure for
      an `ACME` board::

          meta-acme/recipes-core/psplash
          ├── files
          │   └── logo-acme.png
          └── psplash_%.bbappend

      And here are the contents of the `psplash_%.bbappend` file in
      this example::

          SPLASH_IMAGES = "file://logo-acme.png;outsuffix=default"
          FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

      You could even add specific configuration options for `psplash`,
      for example::

          EXTRA_OECONF += "--disable-startup-msg --enable-img-fullscreen"

      For information on append files, see the
      "dev-manual/layers:appending other layers metadata with your layer"
      section.

   **SRC_URI**

      See the BitBake manual for the initial description for this variable:
      **bitbake:SRC_URI**.

      The following features are added by OpenEmbedded and the Yocto Project.

      There are standard and recipe-specific options. Here are standard ones:

      -  `apply` --- whether to apply the patch or not. The default
         action is to apply the patch.

      -  `striplevel` --- which striplevel to use when applying the
         patch. The default level is 1.

      -  `patchdir` --- specifies the directory in which the patch should
         be applied. The default is `${`\ **S**\ `}`.

      Here are options specific to recipes building code from a revision
      control system:

      -  `mindate` --- apply the patch only if
         **SRCDATE** is equal to or greater than
         `mindate`.

      -  `maxdate` --- apply the patch only if **SRCDATE** is not later
         than `maxdate`.

      -  `minrev` --- apply the patch only if **SRCREV** is equal to or
         greater than `minrev`.

      -  `maxrev` --- apply the patch only if **SRCREV** is not later
         than `maxrev`.

      -  `rev` --- apply the patch only if **SRCREV** is equal to
         `rev`.

      -  `notrev` --- apply the patch only if **SRCREV** is not equal to
         `rev`.

      .. note::

         If you want the build system to pick up files specified through
         a **SRC_URI** statement from your append file, you need to be
         sure to extend the **FILESPATH** variable by also using the
         **FILESEXTRAPATHS** variable from within your append file.

   **SRC_URI_OVERRIDES_PACKAGE_ARCH**
      By default, the OpenEmbedded build system automatically detects
      whether **SRC_URI** contains files that are machine-specific. If so,
      the build system automatically changes **PACKAGE_ARCH**. Setting this
      variable to "0" disables this behavior.

   **SRCDATE**
      The date of the source code used to build the package. This variable
      applies only if the source was fetched from a Source Code Manager
      (SCM).

   **SRCPV**
      The variable **SRCPV** is deprecated. It was previously used to
      include source control information in **PV** for **bitbake** to
      work correctly but this is no longer a requirement. Source control
      information will be automatically included by **bitbake** in the
      variable **PKGV** during packaging if the `+` sign is present in
      **PV**.

      .. note::

         The **SRCPV** variable used to be defined in the
         `meta/conf/bitbake.conf` configuration file in
         **OpenEmbedded-Core (OE-Core)** as follows::

            SRCPV = "${@bb.fetch2.get_srcrev(d)}"

         The `get_srcrev` function can still be used to include source control
         information in variables manually.

   **SRCREV**
      The revision of the source code used to build the package. This
      variable applies to Subversion, Git, and Mercurial only. Note
      that if you want to build a fixed revision and you want to avoid
      performing a query on the remote repository every time BitBake parses
      your recipe, you should specify a **SRCREV** that is a full revision
      identifier (e.g. the full SHA hash in git) and not just a tag.

      .. note::

         For information on limitations when inheriting the latest revision
         of software using **SRCREV**, see the **AUTOREV** variable
         description and the
         "dev-manual/packages:automatically incrementing a package version number"
         section, which is in the Yocto Project Development Tasks Manual.

   **SRCREV_FORMAT**
      See **bitbake:SRCREV_FORMAT** in the BitBake manual.

   **SRCTREECOVEREDTASKS**
      A list of tasks that are typically not relevant (and therefore skipped)
      when building using the ref-classes-externalsrc
      class. The default value as set in that class file is the set of tasks
      that are rarely needed when using external source::

         SRCTREECOVEREDTASKS ?= "do_patch do_unpack do_fetch"

      The notable exception is when processing external kernel source as
      defined in the ref-classes-kernel-yocto class file (formatted for
      aesthetics)::

         SRCTREECOVEREDTASKS += "\
           do_validate_branches \
           do_kernel_configcheck \
           do_kernel_checkout \
           do_fetch \
           do_unpack \
           do_patch \
         "

      See the associated **EXTERNALSRC** and **EXTERNALSRC_BUILD**
      variables for more information.

   **SSTATE_DIR**
      The directory for the shared state cache.

   **SSTATE_EXCLUDEDEPS_SYSROOT**
      This variable allows to specify indirect dependencies to exclude
      from sysroots, for example to avoid the situations when a dependency on
      any `-native` recipe will pull in all dependencies of that recipe
      in the recipe sysroot. This behaviour might not always be wanted,
      for example when that `-native` recipe depends on build tools
      that are not relevant for the current recipe.

      This way, irrelevant dependencies are ignored, which could have
      prevented the reuse of prebuilt artifacts stored in the Shared
      State Cache.

      **SSTATE_EXCLUDEDEPS_SYSROOT** is evaluated as two regular
      expressions of recipe and dependency to ignore. An example
      is the rule in meta/conf/layer.conf ::

         # Nothing needs to depend on libc-initial
         # base-passwd/shadow-sysroot don't need their dependencies
         SSTATE_EXCLUDEDEPS_SYSROOT += "\
             .*->.*-initial.* \
             .*(base-passwd|shadow-sysroot)->.* \
         "

      The `->` substring represents the dependency between
      the two regular expressions.

   **SSTATE_MIRROR_ALLOW_NETWORK**
      If set to "1", allows fetches from mirrors that are specified in
      **SSTATE_MIRRORS** to work even when
      fetching from the network is disabled by setting **BB_NO_NETWORK** to
      "1". Using the **SSTATE_MIRROR_ALLOW_NETWORK** variable is useful if
      you have set **SSTATE_MIRRORS** to point to an internal server for
      your shared state cache, but you want to disable any other fetching
      from the network.

   **SSTATE_MIRRORS**
      Configures the OpenEmbedded build system to search other mirror
      locations for prebuilt cache data objects before building out the
      data. This variable works like fetcher **MIRRORS**
      and **PREMIRRORS** and points to the cache
      locations to check for the shared state (sstate) objects.

      You can specify a filesystem directory or a remote URL such as HTTP
      or FTP. The locations you specify need to contain the shared state
      cache (sstate-cache) results from previous builds. The sstate-cache
      you point to can also be from builds on other machines.

      When pointing to sstate build artifacts on another machine that uses
      a different GCC version for native builds, you must configure
      **SSTATE_MIRRORS** with a regular expression that maps local search
      paths to server paths. The paths need to take into account
      **NATIVELSBSTRING** set by the ref-classes-uninative class.
      For example, the following maps the local search path `universal-4.9`
      to the server-provided path server_url_sstate_path::

         SSTATE_MIRRORS ?= "file://universal-4.9/(.*) https://server_url_sstate_path/universal-4.8/\1"

      If a mirror uses the same structure as
      **SSTATE_DIR**, you need to add "PATH" at the
      end as shown in the examples below. The build system substitutes the
      correct path within the directory structure::

         SSTATE_MIRRORS ?= "\
             file://.* https://someserver.tld/share/sstate/PATH;downloadfilename=PATH \
             file://.* file:///some-local-dir/sstate/PATH"

      .. note::

         If the mirror is protected behind a username and password, the
         **build host** needs to be configured so the :term:`build system
         <OpenEmbedded Build System>` is able to download the sstate cache using
         authentication.

         The recommended way to do that is by setting the following parameters
         in `$HOME/.netrc` (`$HOME` being the **build host** home
         directory)::

            machine someserver.tld
            login <user>
            password <password>

         This file requires permissions set to `400` or `600` to prevent
         other users from reading the file::

            chmod 600 "$HOME/.netrc"

         Another method to configure the username and password is from the
         URL in **SSTATE_MIRRORS** directly, with the `user` and `pswd`
         parameters::

            SSTATE_MIRRORS ?= "\
                file://.* https://someserver.tld/share/sstate/PATH;user=<user>;pswd=<password>;downloadfilename=PATH \
            "

      The Yocto Project actually shares the cache data objects built by its
      autobuilder::

         SSTATE_MIRRORS ?= "file://.* http://sstate.yoctoproject.org/all/PATH;downloadfilename=PATH"

      As such binary artifacts are built for the generic QEMU machines
      supported by the various Poky releases, they are less likely to be
      reusable in real projects building binaries optimized for a specific
      CPU family.

   **SSTATE_SCAN_FILES**
      Controls the list of files the OpenEmbedded build system scans for
      hardcoded installation paths. The variable uses a space-separated
      list of filenames (not paths) with standard wildcard characters
      allowed.

      During a build, the OpenEmbedded build system creates a shared state
      (sstate) object during the first stage of preparing the sysroots.
      That object is scanned for hardcoded paths for original installation
      locations. The list of files that are scanned for paths is controlled
      by the **SSTATE_SCAN_FILES** variable. Typically, recipes add files
      they want to be scanned to the value of **SSTATE_SCAN_FILES** rather
      than the variable being comprehensively set. The
      ref-classes-sstate class specifies the default list of files.

      For details on the process, see the ref-classes-staging class.

   **SSTATE_SKIP_CREATION**
      The **SSTATE_SKIP_CREATION** variable can be used to skip the
      creation of shared state 
      tarball files. It makes sense e.g. for image creation tasks as tarring images
      and keeping them in sstate would consume a lot of disk space.

      In general it is not recommended to use this variable as missing sstate
      artefacts adversely impact the build, particularly for entries in the
      middle of dependency chains. The case it can make sense is where the
      size and time costs of the artefact are similar to just running the
      tasks. This generally only applies to end artefact output like images.

      The syntax to disable it for one task is::

         SSTATE_SKIP_CREATION:task-image-complete = "1"

      The syntax to disable it for the whole recipe is::

         SSTATE_SKIP_CREATION = "1"

   **STAGING_BASE_LIBDIR_NATIVE**
      Specifies the path to the `/lib` subdirectory of the sysroot
      directory for the build host.

   **STAGING_BASELIBDIR**
      Specifies the path to the `/lib` subdirectory of the sysroot
      directory for the target for which the current recipe is being built
      (**STAGING_DIR_HOST**).

   **STAGING_BINDIR**
      Specifies the path to the `/usr/bin` subdirectory of the sysroot
      directory for the target for which the current recipe is being built
      (**STAGING_DIR_HOST**).

   **STAGING_BINDIR_CROSS**
      Specifies the path to the directory containing binary configuration
      scripts. These scripts provide configuration information for other
      software that wants to make use of libraries or include files
      provided by the software associated with the script.

      .. note::

         This style of build configuration has been largely replaced by
         `pkg-config`. Consequently, if `pkg-config` is supported by the
         library to which you are linking, it is recommended you use
         `pkg-config` instead of a provided configuration script.

   **STAGING_BINDIR_NATIVE**
      Specifies the path to the `/usr/bin` subdirectory of the sysroot
      directory for the build host.

   **STAGING_DATADIR**
      Specifies the path to the `/usr/share` subdirectory of the sysroot
      directory for the target for which the current recipe is being built
      (**STAGING_DIR_HOST**).

   **STAGING_DATADIR_NATIVE**
      Specifies the path to the `/usr/share` subdirectory of the sysroot
      directory for the build host.

   **STAGING_DIR**
      Helps construct the `recipe-sysroot*` directories, which are used
      during packaging.

      For information on how staging for recipe-specific sysroots occurs,
      see the ref-tasks-populate_sysroot
      task, the "dev-manual/devtool:sharing files between recipes"
      section in the Yocto Project Development Tasks Manual, the
      "overview-manual/concepts:configuration, compilation, and staging"
      section in the Yocto Project Overview and Concepts Manual, and the
      **SYSROOT_DIRS** variable.

      .. note::

         Recipes should never write files directly under the **STAGING_DIR**
         directory because the OpenEmbedded build system manages the
         directory automatically. Instead, files should be installed to
         `${`\ **D**\ `}` within your recipe's ref-tasks-install
         task and then the OpenEmbedded build system will stage a subset of
         those files into the sysroot.

   **STAGING_DIR_HOST**
      Specifies the path to the sysroot directory for the system on which
      the component is built to run (the system that hosts the component).
      For most recipes, this sysroot is the one in which that recipe's
      ref-tasks-populate_sysroot task copies
      files. Exceptions include `-native` recipes, where the
      ref-tasks-populate_sysroot task instead uses
      **STAGING_DIR_NATIVE**. Depending on
      the type of recipe and the build target, **STAGING_DIR_HOST** can
      have the following values:

      -  For recipes building for the target machine, the value is
         "${**STAGING_DIR**}/${**MACHINE**}".

      -  For native recipes building for the build host, the value is empty
         given the assumption that when building for the build host, the
         build host's own directories should be used.

         .. note::

            `-native` recipes are not installed into host paths like such
            as `/usr`. Rather, these recipes are installed into
            **STAGING_DIR_NATIVE**. When compiling `-native` recipes,
            standard build environment variables such as
            **CPPFLAGS** and
            **CFLAGS** are set up so that both host paths
            and **STAGING_DIR_NATIVE** are searched for libraries and
            headers using, for example, GCC's `-isystem` option.

            Thus, the emphasis is that the `STAGING_DIR*` variables
            should be viewed as input variables by tasks such as
            ref-tasks-configure,
            ref-tasks-compile, and
            ref-tasks-install. Having the real system
            root correspond to **STAGING_DIR_HOST** makes conceptual sense
            for `-native` recipes, as they make use of host headers and
            libraries.

      Check **RECIPE_SYSROOT** and **RECIPE_SYSROOT_NATIVE**.

   **STAGING_DIR_NATIVE**
      Specifies the path to the sysroot directory used when building
      components that run on the build host itself.

      The default value is `"${RECIPE_SYSROOT_NATIVE}"`,
      check **RECIPE_SYSROOT_NATIVE**.

   **STAGING_DIR_TARGET**
      Specifies the path to the sysroot used for the system for which the
      component generates code. For components that do not generate code,
      which is the majority, **STAGING_DIR_TARGET** is set to match
      **STAGING_DIR_HOST**.

      Some recipes build binaries that can run on the target system but those
      binaries in turn generate code for another different system (e.g.
      ref-classes-cross-canadian recipes). Using terminology from GNU,
      the primary system is referred to as the "HOST" and the secondary, or
      different, system is referred to as the "TARGET". Thus, the binaries
      run on the "HOST" system and generate binaries for the "TARGET"
      system. The **STAGING_DIR_HOST** variable points to the sysroot used
      for the "HOST" system, while **STAGING_DIR_TARGET** points to the
      sysroot used for the "TARGET" system.

   **STAGING_ETCDIR_NATIVE**
      Specifies the path to the `/etc` subdirectory of the sysroot
      directory for the build host.

   **STAGING_EXECPREFIXDIR**
      Specifies the path to the `/usr` subdirectory of the sysroot
      directory for the target for which the current recipe is being built
      (**STAGING_DIR_HOST**).

   **STAGING_INCDIR**
      Specifies the path to the `/usr/include` subdirectory of the
      sysroot directory for the target for which the current recipe being
      built (**STAGING_DIR_HOST**).

   **STAGING_INCDIR_NATIVE**
      Specifies the path to the `/usr/include` subdirectory of the
      sysroot directory for the build host.

   **STAGING_KERNEL_BUILDDIR**
      Points to the directory containing the kernel build artifacts.
      Recipes building software that needs to access kernel build artifacts
      (e.g. `systemtap-uprobes`) can look in the directory specified with
      the **STAGING_KERNEL_BUILDDIR** variable to find these artifacts
      after the kernel has been built.

   **STAGING_KERNEL_DIR**
      The directory with kernel headers that are required to build
      out-of-tree modules.

   **STAGING_LIBDIR**
      Specifies the path to the `/usr/lib` subdirectory of the sysroot
      directory for the target for which the current recipe is being built
      (**STAGING_DIR_HOST**).

   **STAGING_LIBDIR_NATIVE**
      Specifies the path to the `/usr/lib` subdirectory of the sysroot
      directory for the build host.

   **STAMP**
      Specifies the base path used to create recipe stamp files. The path
      to an actual stamp file is constructed by evaluating this string and
      then appending additional information. Currently, the default
      assignment for **STAMP** as set in the `meta/conf/bitbake.conf`
      file is::

         STAMP = "${STAMPS_DIR}/${MULTIMACH_TARGET_SYS}/${PN}/${EXTENDPE}${PV}-${PR}"

      For information on how BitBake uses stamp files to determine if a
      task should be rerun, see the
      "overview-manual/concepts:stamp files and the rerunning of tasks"
      section in the Yocto Project Overview and Concepts Manual.

      See **STAMPS_DIR**,
      **MULTIMACH_TARGET_SYS**,
      **PN**, **EXTENDPE**,
      **PV**, and **PR** for related variable
      information.

   **STAMPCLEAN**
      See **bitbake:STAMPCLEAN** in the BitBake manual.

   **STAMPS_DIR**
      Specifies the base directory in which the OpenEmbedded build system
      places stamps. The default directory is `${TMPDIR}/stamps`.

   **STRIP**
      The minimal command and arguments to run `strip <strip(1)>`,
      which is used to strip symbols.

   **SUMMARY**
      The short (72 characters or less) summary of the binary package for
      packaging systems such as `opkg`, `rpm`, or `dpkg`. By default,
      **SUMMARY** is used to define the
      **DESCRIPTION** variable if **DESCRIPTION** is
      not set in the recipe.

   **SVNDIR**
      The directory in which files checked out of a Subversion system are
      stored.

   **SYSLINUX_DEFAULT_CONSOLE**
      Specifies the kernel boot default console. If you want to use a
      console other than the default, set this variable in your recipe as
      follows where "X" is the console number you want to use::

         SYSLINUX_DEFAULT_CONSOLE = "console=ttyX"

      The ref-classes-syslinux class initially sets
      this variable to null but then checks for a value later.

   **SYSLINUX_OPTS**
      Lists additional options to add to the syslinux file. You need to set
      this variable in your recipe. If you want to list multiple options,
      separate the options with a semicolon character (`;`).

      The ref-classes-syslinux class uses this variable
      to create a set of options.

   **SYSLINUX_SERIAL**
      Specifies the alternate serial port or turns it off. To turn off
      serial, set this variable to an empty string in your recipe. The
      variable's default value is set in the
      ref-classes-syslinux class as follows::

         SYSLINUX_SERIAL ?= "0 115200"

      The class checks for and uses the variable as needed.

   **SYSLINUX_SERIAL_TTY**
      Specifies the alternate console=tty... kernel boot argument. The
      variable's default value is set in the ref-classes-syslinux
      class as follows::

         SYSLINUX_SERIAL_TTY ?= "console=ttyS0,115200"

      The class checks for and uses the variable as needed.

   **SYSLINUX_SPLASH**
      An `.LSS` file used as the background for the VGA boot menu when
      you use the boot menu. You need to set this variable in your recipe.

      The ref-classes-syslinux class checks for this
      variable and if found, the OpenEmbedded build system installs the
      splash screen.

   **SYSROOT_DESTDIR**
      Points to the temporary directory under the work directory (default
      "`${`\ **WORKDIR**\ `}/sysroot-destdir`")
      where the files populated into the sysroot are assembled during the
      ref-tasks-populate_sysroot task.

   **SYSROOT_DIRS**
      Directories that are staged into the sysroot by the
      ref-tasks-populate_sysroot task. By
      default, the following directories are staged::

         SYSROOT_DIRS = " \
             ${includedir} \
             ${libdir} \
             ${base_libdir} \
             ${nonarch_base_libdir} \
             ${datadir} \
             /sysroot-only \
             "

      Consider the following example in which you need to manipulate this variable.
      Assume you have a recipe `A` that provides a shared library `.so.*` that is
      installed into a custom folder other than "`${libdir}`"
      or "`${base_libdir}`", let's say "`/opt/lib`".

      .. note::

         This is not a recommended way to deal with shared libraries, but this
         is just to show the usefulness of setting **SYSROOT_DIRS**.

      When a recipe `B` **DEPENDS** on `A`, it means what is in
      **SYSROOT_DIRS** will be copied from **D** of the recipe `A`
      into `B`'s **SYSROOT_DESTDIR** that is "`${WORKDIR}/sysroot-destdir`".

      Now, since `/opt/lib` is not in **SYSROOT_DIRS**, it will never be copied to
      `A`'s **RECIPE_SYSROOT**, which is "`${WORKDIR}/recipe-sysroot`". So,
      the linking process will fail.

      To fix this, you need to add `/opt/lib` to **SYSROOT_DIRS**::

         SYSROOT_DIRS:append = " /opt/lib"

      .. note::
         Even after setting `/opt/lib` to **SYSROOT_DIRS**, the linking process will still fail
         because the linker does not know that location, since **TARGET_LDFLAGS**
         doesn't contain it (if your recipe is for the target). Therefore, so you should add::

            TARGET_LDFLAGS:append = " -L${RECIPE_SYSROOT}/opt/lib"

   **SYSROOT_DIRS_IGNORE**
      Directories that are not staged into the sysroot by the
      ref-tasks-populate_sysroot task. You
      can use this variable to exclude certain subdirectories of
      directories listed in **SYSROOT_DIRS** from
      staging. By default, the following directories are not staged::

         SYSROOT_DIRS_IGNORE = " \
             ${mandir} \
             ${docdir} \
             ${infodir} \
             ${datadir}/X11/locale \
             ${datadir}/applications \
             ${datadir}/bash-completion \
             ${datadir}/fonts \
             ${datadir}/gtk-doc/html \
             ${datadir}/installed-tests \
             ${datadir}/locale \
             ${datadir}/pixmaps \
             ${datadir}/terminfo \
             ${libdir}/${BPN}/ptest \
             "

   **SYSROOT_DIRS_NATIVE**
      Extra directories staged into the sysroot by the
      ref-tasks-populate_sysroot task for
      `-native` recipes, in addition to those specified in
      **SYSROOT_DIRS**. By default, the following
      extra directories are staged::

         SYSROOT_DIRS_NATIVE = " \
             ${bindir} \
             ${sbindir} \
             ${base_bindir} \
             ${base_sbindir} \
             ${libexecdir} \
             ${sysconfdir} \
             ${localstatedir} \
             "

      .. note::

         Programs built by `-native` recipes run directly from the sysroot
         (**STAGING_DIR_NATIVE**), which is why additional directories
         containing program executables and supporting files need to be staged.

   **SYSROOT_PREPROCESS_FUNCS**
      A list of functions to execute after files are staged into the
      sysroot. These functions are usually used to apply additional
      processing on the staged files, or to stage additional files.

   **SYSTEMD_AUTO_ENABLE**
      When inheriting the ref-classes-systemd class,
      this variable specifies whether the specified service in
      **SYSTEMD_SERVICE** should start
      automatically or not. By default, the service is enabled to
      automatically start at boot time. The default setting is in the
      ref-classes-systemd class as follows::

         SYSTEMD_AUTO_ENABLE ??= "enable"

      You can disable the service by setting the variable to "disable".

   **SYSTEMD_BOOT_CFG**
      When **EFI_PROVIDER** is set to
      "systemd-boot", the **SYSTEMD_BOOT_CFG** variable specifies the
      configuration file that should be used. By default, the
      ref-classes-systemd-boot class sets the
      **SYSTEMD_BOOT_CFG** as follows::

         SYSTEMD_BOOT_CFG ?= "${S}/loader.conf"

      For information on Systemd-boot, see the `Systemd-boot
      documentation <https://www.freedesktop.org/wiki/Software/systemd/systemd-boot/>`__.

   **SYSTEMD_BOOT_ENTRIES**
      When **EFI_PROVIDER** is set to
      "systemd-boot", the **SYSTEMD_BOOT_ENTRIES** variable specifies a
      list of entry files (`*.conf`) to install that contain one boot
      entry per file. By default, the ref-classes-systemd-boot class
      sets the **SYSTEMD_BOOT_ENTRIES** as follows::

          SYSTEMD_BOOT_ENTRIES ?= ""

      For information on Systemd-boot, see the `Systemd-boot
      documentation <https://www.freedesktop.org/wiki/Software/systemd/systemd-boot/>`__.

   **SYSTEMD_BOOT_TIMEOUT**
      When **EFI_PROVIDER** is set to
      "systemd-boot", the **SYSTEMD_BOOT_TIMEOUT** variable specifies the
      boot menu timeout in seconds. By default, the
      ref-classes-systemd-boot class sets the
      **SYSTEMD_BOOT_TIMEOUT** as follows::

         SYSTEMD_BOOT_TIMEOUT ?= "10"

      For information on Systemd-boot, see the `Systemd-boot
      documentation <https://www.freedesktop.org/wiki/Software/systemd/systemd-boot/>`__.

   **SYSTEMD_DEFAULT_TARGET**

      This variable allows to set the default unit that systemd starts at bootup.
      Usually, this is either `multi-user.target` or `graphical.target`.
      This works by creating a `default.target` symbolic link to the chosen systemd
      target file.

      See `systemd's documentation
      <https://www.freedesktop.org/software/systemd/man/systemd.special.html>`__
      for details.

      For example, this variable is used in the :oe_git:`core-image-minimal-xfce.bb
      </meta-openembedded/tree/meta-xfce/recipes-core/images/core-image-minimal-xfce.bb>`
      recipe::

          SYSTEMD_DEFAULT_TARGET = "graphical.target"

   **SYSTEMD_PACKAGES**
      When inheriting the ref-classes-systemd class,
      this variable locates the systemd unit files when they are not found
      in the main recipe's package. By default, the **SYSTEMD_PACKAGES**
      variable is set such that the systemd unit files are assumed to
      reside in the recipes main package::

         SYSTEMD_PACKAGES ?= "${PN}"

      If these unit files are not in this recipe's main package, you need
      to use **SYSTEMD_PACKAGES** to list the package or packages in which
      the build system can find the systemd unit files.

   **SYSTEMD_SERVICE**
      When inheriting the ref-classes-systemd class,
      this variable specifies the systemd service name for a package.

      Multiple services can be specified, each one separated by a space.

      When you specify this file in your recipe, use a package name
      override to indicate the package to which the value applies. Here is
      an example from the connman recipe::

         SYSTEMD_SERVICE:${PN} = "connman.service"

      The package overrides that can be specified are directly related to the value of
      **SYSTEMD_PACKAGES**. Overrides not included in **SYSTEMD_PACKAGES**
      will be silently ignored.

   **SYSVINIT_ENABLED_GETTYS**
      When using SysVinit ,
      specifies a space-separated list of the virtual terminals that should
      run a getty  (allowing login), assuming
      **USE_VT** is not set to "0".

      The default value for **SYSVINIT_ENABLED_GETTYS** is "1" (i.e. only
      run a getty on the first virtual terminal).

   **T**
      This variable points to a directory were BitBake places temporary
      files, which consist mostly of task logs and scripts, when building a
      particular recipe. The variable is typically set as follows::

         T = "${WORKDIR}/temp"

      The **WORKDIR** is the directory into which
      BitBake unpacks and builds the recipe. The default `bitbake.conf`
      file sets this variable.

      The **T** variable is not to be confused with the
      **TMPDIR** variable, which points to the root of
      the directory tree where BitBake places the output of an entire
      build.

   **TARGET_ARCH**
      The target machine's architecture. The OpenEmbedded build system
      supports many architectures. Here is an example list of architectures
      supported. This list is by no means complete as the architecture is
      configurable:

      - arm
      - i586
      - x86_64
      - powerpc
      - powerpc64
      - mips
      - mipsel

      For additional information on machine architectures, see the
      **TUNE_ARCH** variable.

   **TARGET_AS_ARCH**
      Specifies architecture-specific assembler flags for the target
      system. **TARGET_AS_ARCH** is initialized from
      **TUNE_ASARGS** by default in the BitBake
      configuration file (`meta/conf/bitbake.conf`)::

         TARGET_AS_ARCH = "${TUNE_ASARGS}"

   **TARGET_CC_ARCH**
      Specifies architecture-specific C compiler flags for the target
      system. **TARGET_CC_ARCH** is initialized from
      **TUNE_CCARGS** by default.

      .. note::

         It is a common workaround to append **LDFLAGS** to
         **TARGET_CC_ARCH** in recipes that build software for the target that
         would not otherwise respect the exported **LDFLAGS** variable.

   **TARGET_CC_KERNEL_ARCH**
      This is a specific kernel compiler flag for a CPU or Application
      Binary Interface (ABI) tune. The flag is used rarely and only for
      cases where a userspace **TUNE_CCARGS** is not
      compatible with the kernel compilation. The **TARGET_CC_KERNEL_ARCH**
      variable allows the kernel (and associated modules) to use a
      different configuration. See the
      `meta/conf/machine/include/arm/feature-arm-thumb.inc` file in
      **OpenEmbedded-Core (OE-Core)** for an example.

   **TARGET_CFLAGS**
      Specifies the flags to pass to the C compiler when building for the
      target. When building in the target context,
      **CFLAGS** is set to the value of this variable by
      default.

      Additionally, the SDK's environment setup script sets the **CFLAGS**
      variable in the environment to the **TARGET_CFLAGS** value so that
      executables built using the SDK also have the flags applied.

   **TARGET_CPPFLAGS**
      Specifies the flags to pass to the C pre-processor (i.e. to both the
      C and the C++ compilers) when building for the target. When building
      in the target context, **CPPFLAGS** is set to the
      value of this variable by default.

      Additionally, the SDK's environment setup script sets the
      **CPPFLAGS** variable in the environment to the **TARGET_CPPFLAGS**
      value so that executables built using the SDK also have the flags
      applied.

   **TARGET_CXXFLAGS**
      Specifies the flags to pass to the C++ compiler when building for the
      target. When building in the target context,
      **CXXFLAGS** is set to the value of this variable
      by default.

      Additionally, the SDK's environment setup script sets the
      **CXXFLAGS** variable in the environment to the **TARGET_CXXFLAGS**
      value so that executables built using the SDK also have the flags
      applied.

   **TARGET_DBGSRC_DIR**
      Specifies the target path to debug source files. The default is
      `/usr/src/debug/${PN}/${PV}`.

   **TARGET_FPU**
      Specifies the method for handling FPU code. For FPU-less targets,
      which include most ARM CPUs, the variable must be set to "soft". If
      not, the kernel emulation gets used, which results in a performance
      penalty.

   **TARGET_LD_ARCH**
      Specifies architecture-specific linker flags for the target system.
      **TARGET_LD_ARCH** is initialized from
      **TUNE_LDARGS** by default in the BitBake
      configuration file (`meta/conf/bitbake.conf`)::

         TARGET_LD_ARCH = "${TUNE_LDARGS}"

   **TARGET_LDFLAGS**
      Specifies the flags to pass to the linker when building for the
      target. When building in the target context,
      **LDFLAGS** is set to the value of this variable
      by default.

      Additionally, the SDK's environment setup script sets the
      **LDFLAGS** variable in the environment to the
      **TARGET_LDFLAGS** value so that executables built using the SDK also
      have the flags applied.

   **TARGET_OS**
      Specifies the target's operating system. The variable can be set to
      "linux" for glibc-based systems (GNU C Library) and to "linux-musl"
      for musl libc. For ARM/EABI targets, the possible values are
      "linux-gnueabi" and "linux-musleabi".

   **TARGET_PREFIX**
      Specifies the prefix used for the toolchain binary target tools.

      Depending on the type of recipe and the build target,
      **TARGET_PREFIX** is set as follows:

      -  For recipes building for the target machine, the value is
         "${**TARGET_SYS**}-".

      -  For native recipes, the build system sets the variable to the
         value of **BUILD_PREFIX**.

      -  For native SDK recipes (ref-classes-nativesdk),
         the build system sets the variable to the value of **SDK_PREFIX**.

   **TARGET_SYS**
      Specifies the system, including the architecture and the operating
      system, for which the build is occurring in the context of the
      current recipe.

      The OpenEmbedded build system automatically sets this variable based
      on **TARGET_ARCH**,
      **TARGET_VENDOR**, and
      **TARGET_OS** variables.

      .. note::

         You do not need to set the **TARGET_SYS** variable yourself.

      Consider these two examples:

      -  Given a native recipe on a 32-bit, x86 machine running Linux, the
         value is "i686-linux".

      -  Given a recipe being built for a little-endian, MIPS target
         running Linux, the value might be "mipsel-linux".

   **TARGET_VENDOR**
      Specifies the name of the target vendor.

   **TC_CXX_RUNTIME**
      Specifies the C/C++ STL and runtime variant to use during
      the build process. Default value is 'gnu'

      You can select "gnu", "llvm", or "android".

   **TCLIBC**
      Specifies the GNU standard C library (`libc`) variant to use during
      the build process.

      You can select "glibc", "musl", "newlib", or "baremetal".

   **TCMODE**
      Specifies the toolchain selector. **TCMODE** controls the
      characteristics of the generated packages and images by telling the
      OpenEmbedded build system which toolchain profile to use. By default,
      the OpenEmbedded build system builds its own internal toolchain. The
      variable's default value is "default", which uses that internal
      toolchain.

      .. note::

         If **TCMODE** is set to a value other than "default", then it is your
         responsibility to ensure that the toolchain is compatible with the
         default toolchain. Using older or newer versions of these
         components might cause build problems. See
         Release Information  for your
         version of the Yocto Project, to find the specific components with
         which the toolchain must be compatible.

      The **TCMODE** variable is similar to **TCLIBC**,
      which controls the variant of the GNU standard C library (`libc`)
      used during the build process: `glibc` or `musl`.

      With additional layers, it is possible to use a pre-compiled external
      toolchain. One example is the Sourcery G++ Toolchain. The support for
      this toolchain resides in the separate Mentor Graphics
      `meta-sourcery` layer at
      https://github.com/MentorEmbedded/meta-sourcery/.

      The layer's `README` file contains information on how to use the
      Sourcery G++ Toolchain as an external toolchain. You will have to
      add the layer to your `bblayers.conf` file and then set the
      **EXTERNAL_TOOLCHAIN** variable in your `local.conf` file to
      the location of the toolchain.

      The fundamentals used for this example apply to any external
      toolchain. You can use `meta-sourcery` as a template for adding
      support for other external toolchains.

      In addition to toolchain configuration, you will also need a
      corresponding toolchain recipe file. This recipe file needs to package
      up any pre-built objects in the toolchain such as `libgcc`,
      `libstdcc++`, any locales, and `libc`.

   **TEMPLATECONF**
      Specifies the directory used by the build system to find templates
      from which to build the `bblayers.conf` and `local.conf` files.
      Use this variable if you wish to customize such files, and the default
      BitBake targets shown when sourcing the `oe-init-build-env` script.

      For details, see the
      dev-manual/custom-template-configuration-directory:creating a custom template configuration directory
      section in the Yocto Project Development Tasks manual.

      .. note::

         You must set this variable in the external environment in order
         for it to work.

   **TEST_EXPORT_DIR**
      The location the OpenEmbedded build system uses to export tests when
      the **TEST_EXPORT_ONLY** variable is set
      to "1".

      The **TEST_EXPORT_DIR** variable defaults to
      `"${TMPDIR}/testimage/${PN}"`.

   **TEST_EXPORT_ONLY**
      Specifies to export the tests only. Set this variable to "1" if you
      do not want to run the tests but you want them to be exported in a
      manner that you to run them outside of the build system.

   **TEST_LOG_DIR**
      Holds the SSH log and the boot log for QEMU machines. The
      **TEST_LOG_DIR** variable defaults to `"${WORKDIR}/testimage"`.

      .. note::

         Actual test results reside in the task log (`log.do_testimage`),
         which is in the `${WORKDIR}/temp/` directory.

   **TEST_POWERCONTROL_CMD**
      For automated hardware testing, specifies the command to use to
      control the power of the target machine under test. Typically, this
      command would point to a script that performs the appropriate action
      (e.g. interacting with a web-enabled power strip). The specified
      command should expect to receive as the last argument "off", "on" or
      "cycle" specifying to power off, on, or cycle (power off and then
      power on) the device, respectively.

   **TEST_POWERCONTROL_EXTRA_ARGS**
      For automated hardware testing, specifies additional arguments to
      pass through to the command specified in
      **TEST_POWERCONTROL_CMD**. Setting
      **TEST_POWERCONTROL_EXTRA_ARGS** is optional. You can use it if you
      wish, for example, to separate the machine-specific and
      non-machine-specific parts of the arguments.

   **TEST_QEMUBOOT_TIMEOUT**
      The time in seconds allowed for an image to boot before automated
      runtime tests begin to run against an image. The default timeout
      period to allow the boot process to reach the login prompt is 500
      seconds. You can specify a different value in the `local.conf`
      file.

      For more information on testing images, see the
      "test-manual/runtime-testing:performing automated runtime testing"
      section in the Yocto Project Test Environment Manual.

   **TEST_SERIALCONTROL_CMD**
      For automated hardware testing, specifies the command to use to
      connect to the serial console of the target machine under test. This
      command simply needs to connect to the serial console and forward
      that connection to standard input and output as any normal terminal
      program does.

      For example, to use the Picocom terminal program on serial device
      `/dev/ttyUSB0` at 115200bps, you would set the variable as follows::

         TEST_SERIALCONTROL_CMD = "picocom /dev/ttyUSB0 -b 115200"

   **TEST_SERIALCONTROL_EXTRA_ARGS**
      For automated hardware testing, specifies additional arguments to
      pass through to the command specified in
      **TEST_SERIALCONTROL_CMD**. Setting
      **TEST_SERIALCONTROL_EXTRA_ARGS** is optional. You can use it if you
      wish, for example, to separate the machine-specific and
      non-machine-specific parts of the command.

   **TEST_SERVER_IP**
      The IP address of the build machine (host machine). This IP address
      is usually automatically detected. However, if detection fails, this
      variable needs to be set to the IP address of the build machine (i.e.
      where the build is taking place).

      .. note::

         The **TEST_SERVER_IP** variable is only used for a small number of
         tests such as the "dnf" test suite, which needs to download packages
         from `WORKDIR/oe-rootfs-repo`.

   **TEST_SUITES**
      An ordered list of tests (modules) to run against an image when
      performing automated runtime testing.

      The OpenEmbedded build system provides a core set of tests that can
      be used against images.

      .. note::

         Currently, there is only support for running these tests under
         QEMU.

      Tests include `ping`, `ssh`, `df` among others. You can add
      your own tests to the list of tests by appending **TEST_SUITES** as
      follows::

         TEST_SUITES:append = " mytest"

      Alternatively, you can
      provide the "auto" option to have all applicable tests run against
      the image::

         TEST_SUITES:append = " auto"

      Using this option causes the
      build system to automatically run tests that are applicable to the
      image. Tests that are not applicable are skipped.

      The order in which tests are run is important. Tests that depend on
      another test must appear later in the list than the test on which
      they depend. For example, if you append the list of tests with two
      tests (`test_A` and `test_B`) where `test_B` is dependent on
      `test_A`, then you must order the tests as follows::

         TEST_SUITES = "test_A test_B"

      For more information on testing images, see the
      "test-manual/runtime-testing:performing automated runtime testing"
      section in the Yocto Project Test Environment Manual.

   **TEST_TARGET**
      Specifies the target controller to use when running tests against a
      test image. The default controller to use is "qemu"::

         TEST_TARGET = "qemu"

      A target controller is a class that defines how an image gets
      deployed on a target and how a target is started. A layer can extend
      the controllers by adding a module in the layer's
      `/lib/oeqa/controllers` directory and by inheriting the
      `BaseTarget` class, which is an abstract class that cannot be used
      as a value of **TEST_TARGET**.

      You can provide the following arguments with **TEST_TARGET**:

      -  *"qemu":* Boots a QEMU image and runs the tests. See the
         "test-manual/runtime-testing:enabling runtime tests on qemu" section
         in the Yocto Project Test Environment Manual for more
         information.

      -  *"simpleremote":* Runs the tests on target hardware that is
         already up and running. The hardware can be on the network or it
         can be a device running an image on QEMU. You must also set
         **TEST_TARGET_IP** when you use
         "simpleremote".

         .. note::

            This argument is defined in
            `meta/lib/oeqa/controllers/simpleremote.py`.

      For information on running tests on hardware, see the
      "test-manual/runtime-testing:enabling runtime tests on hardware"
      section in the Yocto Project Test Environment Manual.

   **TEST_TARGET_IP**
      The IP address of your hardware under test. The **TEST_TARGET_IP**
      variable has no effect when **TEST_TARGET** is
      set to "qemu".

      When you specify the IP address, you can also include a port. Here is
      an example::

         TEST_TARGET_IP = "192.168.1.4:2201"

      Specifying a port is
      useful when SSH is started on a non-standard port or in cases when
      your hardware under test is behind a firewall or network that is not
      directly accessible from your host and you need to do port address
      translation.

   **TESTIMAGE_AUTO**
      Automatically runs the series of automated tests for images when an
      image is successfully built. Setting **TESTIMAGE_AUTO** to "1" causes
      any image that successfully builds to automatically boot under QEMU.
      Using the variable also adds in dependencies so that any SDK for
      which testing is requested is automatically built first.

      These tests are written in Python making use of the `unittest`
      module, and the majority of them run commands on the target system
      over `ssh`. You can set this variable to "1" in your `local.conf`
      file in the **Build Directory** to have the
      OpenEmbedded build system automatically run these tests after an
      image successfully builds:

         TESTIMAGE_AUTO = "1"

      For more information
      on enabling, running, and writing these tests, see the
      "test-manual/runtime-testing:performing automated runtime testing"
      section in the Yocto Project Test Environment Manual and the
      "ref-classes-testimage" section.

   **TESTIMAGE_FAILED_QA_ARTIFACTS**
      When using the ref-classes-testimage class, the variable
      **TESTIMAGE_FAILED_QA_ARTIFACTS**  lists space-separated paths on the
      target to retrieve onto the host.

   **TESTSDK_SUITES**
      When using the ref-classes-testsdk class, the
      **TESTSDK_SUITES** variable can be used to control the list of test
      modules to run. The default value for this variable is an empty string
      and all test modules that can be found are run.

   **THISDIR**
      The directory in which the file BitBake is currently parsing is
      located. Do not manually set this variable.

   **TIME**
      The time the build was started. Times appear using the hour, minute,
      and second (HMS) format (e.g. "140159" for one minute and fifty-nine
      seconds past 1400 hours).

   **TMPDIR**
      This variable is the base directory the OpenEmbedded build system
      uses for all build output and intermediate files (other than the
      shared state cache). By default, the **TMPDIR** variable points to
      `tmp` within the **Build Directory**.

      If you want to establish this directory in a location other than the
      default, you can set it to another value in your
      structure-build-conf-site.conf configuration file::

         TMPDIR = "/another/location"

      An example use for this scenario is to set **TMPDIR** to a local disk,
      which does not use NFS, while having the **Build Directory** use NFS.

      The filesystem used by **TMPDIR** must have standard filesystem
      semantics (i.e. mixed-case files are unique, POSIX file locking, and
      persistent inodes). Due to various issues with NFS and bugs in some
      implementations, NFS does not meet this minimum requirement.
      Consequently, **TMPDIR** cannot be on NFS.

   **TOOLCHAIN**
      The **TOOLCHAIN** variable can be used to override the toolchain used
      by a recipe.

      The default value for this variable is the value of
      **PREFERRED_TOOLCHAIN**. See the description of
      **PREFERRED_TOOLCHAIN** to know the list of possible values for
      **TOOLCHAIN**.

      It is possible to override the value of this variable from a recipe if
      this recipe is known to support only a specific toolchain. For example,
      the Pseudo 
      recipe overrides this variable to "gcc", because Pseudo uses GCC compiler
      built-ins options that the Clang/LLVM compiler does not provide.

   **TOOLCHAIN_HOST_TASK**
      This variable lists packages the OpenEmbedded build system uses when
      building an SDK, which contains a cross-development environment. The
      packages specified by this variable are part of the toolchain set
      that runs on the **SDKMACHINE**, and each
      package should usually have the prefix `nativesdk-`. For example,
      consider the following command when building an SDK::

         $ bitbake -c populate_sdk imagename

      In this case, a default list of packages is
      set in this variable, but you can add additional packages to the
      list. See the
      "sdk-manual/appendix-customizing-standard:adding individual packages to the standard sdk" section
      in the Yocto Project Application Development and the Extensible
      Software Development Kit (eSDK) manual for more information.

      For background information on cross-development toolchains in the
      Yocto Project development environment, see the
      "sdk-manual/intro:the cross-development toolchain"
      section in the Yocto Project Overview and Concepts Manual. For
      information on setting up a cross-development environment, see the
      /sdk-manual/index manual.

      Note that this variable applies to building an SDK, not an eSDK,
      in which case the **TOOLCHAIN_HOST_TASK_ESDK** setting should be
      used instead.

   **TOOLCHAIN_HOST_TASK_ESDK**
      This variable allows to extend what is installed in the host
      portion of an eSDK. This is similar to **TOOLCHAIN_HOST_TASK**
      applying to SDKs.

   **TOOLCHAIN_NATIVE**
      The **TOOLCHAIN_NATIVE** variable can be used to override the
      toolchain used by a ref-classes-native recipe.

      The default value for this variable is the value of
      **PREFERRED_TOOLCHAIN** (in ref-classes-native contexts). See
      the description of **PREFERRED_TOOLCHAIN** to know the list of
      possible values for **TOOLCHAIN_NATIVE**.

      It is possible to override the value of this variable from a recipe if
      this recipe is known to support only a specific toolchain.

   **TOOLCHAIN_OPTIONS**
      This variable holds extra options passed to the compiler and the linker
      for non `-native` recipes as they have to point to their custom
      `sysroot` folder pointed to by **RECIPE_SYSROOT**::

         TOOLCHAIN_OPTIONS = " --sysroot=${RECIPE_SYSROOT}"

      Native recipes don't need this variable to be set, as they are
      built for the host machine with the native compiler.

   **TOOLCHAIN_OUTPUTNAME**
      This variable defines the name used for the toolchain output. The
      populate_sdk_base  class sets
      the **TOOLCHAIN_OUTPUTNAME** variable as follows::

         TOOLCHAIN_OUTPUTNAME ?= "${SDK_NAME}-toolchain-${SDK_VERSION}"

      See
      the **SDK_NAME** and
      **SDK_VERSION** variables for additional
      information.

   **TOOLCHAIN_TARGET_TASK**
      This variable lists packages the OpenEmbedded build system uses when
      it creates the target part of an SDK (i.e. the part built for the
      target hardware), which includes libraries and headers. Use this
      variable to add individual packages to the part of the SDK that runs
      on the target. See the
      "sdk-manual/appendix-customizing-standard:adding individual packages to the standard sdk" section
      in the Yocto Project Application Development and the Extensible
      Software Development Kit (eSDK) manual for more information.

      For background information on cross-development toolchains in the
      Yocto Project development environment, see the
      "sdk-manual/intro:the cross-development toolchain"
      section in the Yocto Project Overview and Concepts Manual. For
      information on setting up a cross-development environment, see the
      /sdk-manual/index manual.

   **TOPDIR**
      See **bitbake:TOPDIR** in the BitBake manual.

   **TRANSLATED_TARGET_ARCH**
      A sanitized version of **TARGET_ARCH**. This
      variable is used where the architecture is needed in a value where
      underscores are not allowed, for example within package filenames. In
      this case, dash characters replace any underscore characters used in
      **TARGET_ARCH**.

      Do not edit this variable.

   **TUNE_ARCH**
      The GNU canonical architecture for a specific architecture (i.e.
      `arm`, `armeb`, `mips`, `mips64`, and so forth). BitBake uses
      this value to setup configuration.

      **TUNE_ARCH** definitions are specific to a given architecture. The
      definitions can be a single static definition, or can be dynamically
      adjusted. You can see details for a given CPU family by looking at
      the architecture's `README` file. For example, the
      `meta/conf/machine/include/mips/README` file in
      **OpenEmbedded-Core (OE-Core)** provides information for
      **TUNE_ARCH** specific to the `mips` architecture.

      **TUNE_ARCH** is tied closely to
      **TARGET_ARCH**, which defines the target
      machine's architecture. The BitBake configuration file
      (`meta/conf/bitbake.conf`) sets **TARGET_ARCH** as follows::

         TARGET_ARCH = "${TUNE_ARCH}"

      The following list, which is by no means complete since architectures
      are configurable, shows supported machine architectures:

      - arm
      - i586
      - x86_64
      - powerpc
      - powerpc64
      - mips
      - mipsel

   **TUNE_ASARGS**
      Specifies architecture-specific assembler flags for the target
      system. The set of flags is based on the selected tune features.
      **TUNE_ASARGS** is set using the tune include files, which are
      typically under `meta/conf/machine/include/` and are influenced
      through **TUNE_FEATURES**. For example, the
      `meta/conf/machine/include/x86/arch-x86.inc` file defines the flags
      for the x86 architecture as follows::

         TUNE_ASARGS += "${@bb.utils.contains("TUNE_FEATURES", "mx32", "-x32", "", d)}"

      .. note::

         Board Support Packages (BSPs) select the tune. The selected tune,
         in turn, affects the tune variables themselves (i.e. the tune can
         supply its own set of flags).

   **TUNE_CCARGS**
      Specifies architecture-specific C compiler flags for the target
      system. The set of flags is based on the selected tune features.
      **TUNE_CCARGS** is set using the tune include files, which are
      typically under `meta/conf/machine/include/` and are influenced
      through **TUNE_FEATURES**.

      .. note::

         Board Support Packages (BSPs) select the tune. The selected tune,
         in turn, affects the tune variables themselves (i.e. the tune can
         supply its own set of flags).

   **TUNE_FEATURES**
      Features used to "tune" a compiler for optimal use given a specific
      processor. The features are defined within the tune files and allow
      arguments (i.e. `TUNE_*ARGS`) to be dynamically generated based on
      the features.

      The OpenEmbedded build system verifies the features to be sure they
      are not conflicting and that they are supported.

      The BitBake configuration file (`meta/conf/bitbake.conf`) defines
      **TUNE_FEATURES** as follows::

         TUNE_FEATURES ??= "${TUNE_FEATURES:tune-${DEFAULTTUNE}}"

      See the **DEFAULTTUNE** variable for more information.

   **TUNE_LDARGS**
      Specifies architecture-specific linker flags for the target system.
      The set of flags is based on the selected tune features.
      **TUNE_LDARGS** is set using the tune include files, which are
      typically under `meta/conf/machine/include/` and are influenced
      through **TUNE_FEATURES**. For example, the
      `meta/conf/machine/include/x86/arch-x86.inc` file defines the flags
      for the x86 architecture as follows::

         TUNE_LDARGS += "${@bb.utils.contains("TUNE_FEATURES", "mx32", "-m elf32_x86_64", "", d)}"

      .. note::

         Board Support Packages (BSPs) select the tune. The selected tune,
         in turn, affects the tune variables themselves (i.e. the tune can
         supply its own set of flags).

   **TUNE_PKGARCH**
      The package architecture understood by the packaging system to define
      the architecture, ABI, and tuning of output packages. The specific
      tune is defined using the "_tune" override as follows::

         TUNE_PKGARCH:tune-tune = "tune"

      These tune-specific package architectures are defined in the machine
      include files. Here is an example of the "core2-32" tuning as used in
      the `meta/conf/machine/include/x86/tune-core2.inc` file::

         TUNE_PKGARCH:tune-core2-32 = "core2-32"

   **TUNECONFLICTS[feature]**
      Specifies CPU or Application Binary Interface (ABI) tuning features
      that conflict with feature.

      Known tuning conflicts are specified in the machine include files in
      **OpenEmbedded-Core (OE-Core)**. Here is an example from
      the `meta/conf/machine/include/mips/arch-mips.inc` include file
      that lists the "o32" and "n64" features as conflicting with the "n32"
      feature::

         TUNECONFLICTS[n32] = "o32 n64"

   **TUNEVALID[feature]**
      Specifies a valid CPU or Application Binary Interface (ABI) tuning
      feature. The specified feature is stored as a flag. Valid features
      are specified in the machine include files (e.g.
      `meta/conf/machine/include/arm/arch-arm.inc`). Here is an example
      from that file::

         TUNEVALID[bigendian] = "Enable big-endian mode."

      See the machine include files in **OpenEmbedded-Core (OE-Core)**
      for these features.

   **UBOOT_BINARY**
      Specifies the name of the binary build by U-Boot.

   **UBOOT_CONFIG**
      Configures one or more U-Boot configurations to build. Each
      configuration must define the **UBOOT_MACHINE** variable.
      Additional control variables are: **UBOOT_CONFIG_BINARY**,
      **UBOOT_CONFIG_FRAGMENTS**, **UBOOT_CONFIG_IMAGE_FSTYPES**, and
      **UBOOT_CONFIG_MAKE_OPTS**. 

      Here is an updated example from the `meta-freescale` layer. ::

         UBOOT_CONFIG ??= "sdcard-ifc-secure-boot sdcard-ifc sdcard-qspi lpuart qspi secure-boot nor"

         UBOOT_CONFIG[nor] = "ls1021atwr_nor_defconfig"
         UBOOT_CONFIG[sdcard-ifc] = "ls1021atwr_sdcard_ifc_defconfig"
         UBOOT_CONFIG[sdcard-qspi] = "ls1021atwr_sdcard_qspi_defconfig"
         UBOOT_CONFIG[lpuart] = "ls1021atwr_nor_lpuart_defconfig"
         UBOOT_CONFIG[qspi] = "ls1021atwr_qspi_defconfig"
         UBOOT_CONFIG[secure-boot] = "ls1021atwr_nor_SECURE_BOOT_defconfig"
         UBOOT_CONFIG[sdcard-ifc-secure-boot] = "ls1021atwr_sdcard_ifc_SECURE_BOOT_defconfig"

         UBOOT_CONFIG_BINARY[sdcard-ifc] = "u-boot-with-spl-pbl.bin"
         UBOOT_CONFIG_BINARY[sdcard-qspi] = "u-boot-with-spl-pbl.bin"
         UBOOT_CONFIG_BINARY[sdcard-ifc-secure-boot] = "u-boot-with-spl-pbl.bin"

      In this example, all possible seven configurations are selected. Each
      configuration specifies "..._defconfig" as **UBOOT_MACHINE**, and
      the "sd..." configurations define an individual name for
      **UBOOT_CONFIG_BINARY**.

      For more information on how the **UBOOT_CONFIG** variable is handled, see the
      ref-classes-uboot-config class.

   **UBOOT_CONFIG_BINARY**
      This variable cannot be set to a value in a config, it is a placeholder
      for configuring the **UBOOT_CONFIG** flow via flags::

         UBOOT_CONFIG_BINARY[foo] = "binary1"
         UBOOT_CONFIG_BINARY[bar] = "binary2"

      It specifies the binary to select as the one to deploy in **DEPLOY_DIR_IMAGE**.
      The output of a U-Boot build may be more than one binary, for example::

         u-boot.bin
         u-boot-with-spl.bin

      Setting the `binary` value to `u-boot-with-spl.bin` will make this
      binary the one deployed in **DEPLOY_DIR_IMAGE**. It is renamed to
      include the build configuration name in the process (`foo` or `bar` in
      the above example).

      This option defaults to **UBOOT_BINARY** if not specified.

      For more information on how the **UBOOT_CONFIG_BINARY** variable is
      handled, see the ref-classes-uboot-config class.

   **UBOOT_CONFIG_FRAGMENTS**
      This variable cannot be set to a value in a config, it is a placeholder
      for configuring the **UBOOT_CONFIG** flow via flags::

         UBOOT_CONFIG_FRAGMENTS[foo] = "frag1 frag2"
         UBOOT_CONFIG_FRAGMENTS[bar] = "frag3"

      It specifies a list of fragments from the source tree that should be combined
      with the defconfig from **UBOOT_CONFIG** that are used during `do_configure()`
      to configure the build.  These fragments are located in same
      `${S}/configs/` directory as the defconfig.

      This option is not required and you only need to specify it for
      configs that need them.

      For more information on how the **UBOOT_CONFIG_FRAGMENTS** variable is handled, see the
      ref-classes-uboot-config class.

   **UBOOT_CONFIG_IMAGE_FSTYPES**
      This variable cannot be set to a value in a config, it is a placeholder
      for configuring the **UBOOT_CONFIG** flow via flags::

         UBOOT_CONFIG_IMAGE_FSTYPES[foo] = "fstype1"
         UBOOT_CONFIG_IMAGE_FSTYPES[bar] = "fstype2"

      Append additional image types to the **IMAGE_FSTYPES** variable for
      image generation for this build configuration.  This can allow building an
      extra image format that uses the binary generated by this build configuration.

      This option is not required and you only need to specify flag settings for
      configs that need them.

      For more information on how the **UBOOT_CONFIG_IMAGE_FSTYPES**
      variable is handled, see the ref-classes-uboot-config class.

   **UBOOT_CONFIG_MAKE_OPTS**
      This variable cannot be set to a value in a config, it is a placeholder
      for configuring the **UBOOT_CONFIG** flow via flags::

         UBOOT_CONFIG_MAKE_OPTS[foo] = "OPT1=foo OPT2=2"
         UBOOT_CONFIG_MAKE_OPTS[bar] = "OPT1=bar"

      It specifies a list of make command line options that are passed to the `make` command
      during `do_compile()`.

      This option is not required and you only need to specify flag settings for
      configs that need them.

      For more information on how the **UBOOT_CONFIG_MAKE_OPTS** is handled, see the
      ref-classes-uboot-config class.

   **UBOOT_DTB_LOADADDRESS**
      Specifies the load address for the dtb image used by U-Boot. During FIT
      image creation, the **UBOOT_DTB_LOADADDRESS** variable is used in
      ref-classes-kernel-fit-image class to specify the load address to be
      used in creating the dtb sections of Image Tree Source for the FIT image.

   **UBOOT_DTBO_LOADADDRESS**
      Specifies the load address for the dtbo image used by U-Boot.  During FIT
      image creation, the **UBOOT_DTBO_LOADADDRESS** variable is used in
      ref-classes-kernel-fit-image class to specify the load address to be
      used in creating the dtbo sections of Image Tree Source for the FIT image.

   **UBOOT_ENTRYPOINT**
      Specifies the entry point for the U-Boot image. During U-Boot image
      creation, the **UBOOT_ENTRYPOINT** variable is passed as a
      command-line parameter to the `uboot-mkimage` utility.

      To pass a 64 bit address for FIT image creation, you will need to set:
      -  The **FIT_ADDRESS_CELLS** variable for FIT image creation.
      -  The **UBOOT_FIT_ADDRESS_CELLS** variable for U-Boot FIT image creation.

      This variable is used by the ref-classes-kernel-fit-image,
      ref-classes-kernel-uimage, ref-classes-kernel,
      ref-classes-uboot-config and ref-classes-uboot-sign
      classes.

   **UBOOT_ENV**
      This variable allows to add additional environment variables or a script
      to be installed together with U-Boot.
      This file, typically `uEnv.txt` or `boot.cmd`, is installed in
      `/boot` as well as copied to the **DEPLOYDIR** directory.

      For machine configurations needing one of these files a `.bbappend`
      file should include it in the **SRC_URI** of the U-Boot recipe.

      If the variable **UBOOT_ENV_SUFFIX** is set to `scr` the script is
      packaged as a uImage (`mkimage -T script..`) otherwise it gets
      installed verbatim.

      Some examples:

      -  Adding a script `boot.cmd` as a uImage to `/boot`::

            UBOOT_ENV = "boot"
            UBOOT_ENV_SUFFIX = "scr"
            SRC_URI += "file://${UBOOT_ENV_SRC}"

      -  Adding a script `uEnv.txt` as a plain text file to `/boot`::

            UBOOT_ENV = "uEnv"
            UBOOT_ENV_SUFFIX = "txt"
            SRC_URI += "file://${UBOOT_ENV_BINARY}"

   **UBOOT_ENV_SRC_SUFFIX**
      If **UBOOT_ENV_SUFFIX** is set to `scr` this is the suffix of the
      plain text script file as it is specified in the **SRC_URI** of the
      U-Boot recipe. It defaults to `cmd`.

   **UBOOT_ENV_SUFFIX**
      If this variable is set to `scr` the script referred to by
      **UBOOT_ENV** gets packaged as a uImage before it gets installed.
      The default is `txt` which means the script is installed as-is, with
      no modification.

   **UBOOT_FIT_ADDRESS_CELLS**
      Specifies the value of the `#address-cells` value for the
      description of the U-Boot FIT image.

      The default value is set to "1" by the ref-classes-uboot-sign
      class, which corresponds to 32 bit addresses.

      For platforms that need to set 64 bit addresses in
      **UBOOT_LOADADDRESS** and **UBOOT_ENTRYPOINT**, you need to
      set this value to "2", as two 32 bit values (cells) will be needed
      to represent such addresses.

      Here is an example setting "0x400000000" as a load address::

         UBOOT_FIT_ADDRESS_CELLS = "2"
         UBOOT_LOADADDRESS = "0x04 0x00000000"

      See `more details about #address-cells <https://elinux.org/Device_Tree_Usage#How_Addressing_Works>`__.

   **UBOOT_FIT_ARM_TRUSTED_FIRMWARE**
      `Trusted Firmware-A (TF-A) <https://www.trustedfirmware.org/projects/tf-a>`__
      is a reference implementation of secure world software for Arm A-Profile
      architectures (Armv8-A and Armv7-A), including an Exception Level 3 (EL3)
      Secure Monitor. This variable enables the generation of a U-Boot FIT
      image with a Trusted Firmware-A (TF-A) binary.

      Its default value is "0", so set it to "1" to enable this functionality::

         UBOOT_FIT_ARM_TRUSTED_FIRMWARE = "1"

   **UBOOT_FIT_ARM_TRUSTED_FIRMWARE_IMAGE**
      Specifies the path to the Trusted Firmware-A (TF-A) binary. Its default
      value is "bl31.bin"::

         UBOOT_FIT_ARM_TRUSTED_FIRMWARE_IMAGE ?= "bl31.bin"

      If a relative path is provided, the file is expected to be relative to
      U-Boot's **B** directory. An absolute path can be provided too,
      e.g.::

         UBOOT_FIT_ARM_TRUSTED_FIRMWARE_IMAGE ?= "${DEPLOY_DIR_IMAGE}/bl31.bin"

      If the Trusted Firmware-A (TF-A) binary is built in a separate recipe,
      you must add the necessary dependency in a U-Boot `.bbappend` file. The
      recipe name for Trusted Firmware-A (TF-A) binary is
      `trusted-firmware-a`, which comes from the
      meta-arm  layer::

         do_compile[depends] += "trusted-firmware-a:do_deploy"

   **UBOOT_FIT_CONF_FIRMWARE**
      Adds one image to the `firmware` property of the configuration node of
      the U-Boot Image Tree Source (ITS). Sets the `firmware` property to
      select the image to boot first::

         UBOOT_FIT_CONF_FIRMWARE = "fwa"

      If not set, the first entry in "loadables" is used to boot instead.

   **UBOOT_FIT_CONF_USER_LOADABLES**
      Adds one or more user-defined images to the `loadables` property of the
      configuration node of the U-Boot Image Tree Source (ITS). This variable
      is handled by the local shell in the recipe so appropriate escaping
      should be done, e.g. escaping quotes.::

         UBOOT_FIT_CONF_USER_LOADABLES = '\"fwa\", \"fwb\"'

   **UBOOT_FIT_DESC**
      Specifies the description string encoded into a U-Boot fitImage. The default
      value is set by the ref-classes-uboot-sign class as follows::

         UBOOT_FIT_DESC ?= "U-Boot fitImage for ${DISTRO_NAME}/${PV}/${MACHINE}"

   **UBOOT_FIT_GENERATE_KEYS**
      Decides whether to generate the keys for signing the U-Boot fitImage if
      they don't already exist. The keys are created in **SPL_SIGN_KEYDIR**.
      The default value is "0".

      Enable this as follows::

         UBOOT_FIT_GENERATE_KEYS = "1"

      This variable is used in the ref-classes-uboot-sign class.

   **UBOOT_FIT_HASH_ALG**
      Specifies the hash algorithm used in creating the U-Boot FIT Image.
      It is set by default to `sha256` by the ref-classes-uboot-sign
      class.

   **UBOOT_FIT_KEY_GENRSA_ARGS**
      Arguments to `openssl genrsa` for generating a RSA private key for
      signing the U-Boot FIT image. The default value of this variable
      is set to "-F4" by the ref-classes-uboot-sign class.

   **UBOOT_FIT_KEY_REQ_ARGS**
      Arguments to `openssl req` for generating a certificate for signing
      the U-Boot FIT image. The default value is "-batch -new" by the
      ref-classes-uboot-sign class, "batch" for
      non interactive mode and "new" for generating new keys.

   **UBOOT_FIT_KEY_SIGN_PKCS**
      Format for the public key certificate used for signing the U-Boot FIT
      image. The default value is set to "x509" by the
      ref-classes-uboot-sign class.

   **UBOOT_FIT_SIGN_ALG**
      Specifies the signature algorithm used in creating the U-Boot FIT Image.
      This variable is set by default to "rsa2048" by the
      ref-classes-uboot-sign class.

   **UBOOT_FIT_SIGN_NUMBITS**
      Size of the private key used in signing the U-Boot FIT image, in number
      of bits. The default value for this variable is set to "2048"
      by the ref-classes-uboot-sign class.

   **UBOOT_FIT_TEE**
      A Trusted Execution Environment (TEE) is a secure environment for
      executing code, ensuring high levels of trust in asset management within
      the surrounding system. This variable enables the generation of a U-Boot
      FIT image with a Trusted Execution Environment (TEE) binary.

      Its default value is "0", so set it to "1" to enable this functionality::

         UBOOT_FIT_TEE = "1"

   **UBOOT_FIT_TEE_IMAGE**
      Specifies the path to the Trusted Execution Environment (TEE) binary. Its
      default value is "tee-raw.bin"::

         UBOOT_FIT_TEE_IMAGE ?= "tee-raw.bin"

      If a relative path is provided, the file is expected to be relative to
      U-Boot's **B** directory. An absolute path can be provided too,
      e.g.::

         UBOOT_FIT_TEE_IMAGE ?= "${DEPLOY_DIR_IMAGE}/tee-raw.bin"

      If the Trusted Execution Environment (TEE) binary is built in a separate
      recipe, you must add the necessary dependency in a U-Boot `.bbappend`
      file. The recipe name for Trusted Execution Environment (TEE) binary is
      `optee-os`, which comes from the meta-arm 
      layer::

         do_compile[depends] += "optee-os:do_deploy"

   **UBOOT_FIT_USER_SETTINGS**
      Add a user-specific snippet to the U-Boot Image Tree Source (ITS). This
      variable allows the user to add one or more user-defined `/images` node
      to the U-Boot Image Tree Source (ITS). For more details, please refer to
      https://fitspec.osfw.foundation/\ .

      The original content of the U-Boot Image Tree Source (ITS) is as
      follows::

         images {
             uboot {
                 description = "U-Boot image";
                 data = /incbin/("u-boot-nodtb.bin");
                 type = "standalone";
                 os = "u-boot";
                 arch = "";
                 compression = "none";
                 load = <0x80000000>;
                 entry = <0x80000000>;
             };
         };

      Users can include their custom ITS snippet in this variable, e.g.::

         UBOOT_FIT_FWA_ITS = '\
             fwa {\n\
                 description = \"FW A\";\n\
                 data = /incbin/(\"fwa.bin\");\n\
                 type = \"firmware\";\n\
                 arch = \"\";\n\
                 os = \"\";\n\
                 load = <0xb2000000>;\n\
                 entry = <0xb2000000>;\n\
                 compression = \"none\";\n\
             };\n\
         '

         UBOOT_FIT_USER_SETTINGS = "${UBOOT_FIT_FWA_ITS}"

      This variable is handled by the local shell in the recipe so appropriate
      escaping should be done, e.g. escaping quotes and adding newlines with
      `\n`.

      The generated content of the U-Boot Image Tree Source (ITS) is as
      follows::

         images {
             uboot {
                 description = "U-Boot image";
                 data = /incbin/("u-boot-nodtb.bin");
                 type = "standalone";
                 os = "u-boot";
                 arch = "";
                 compression = "none";
                 load = <0x80000000>;
                 entry = <0x80000000>;
             };
             fwa {
                 description = "FW A";
                 data = /incbin/("fwa.bin");
                 type = "firmware";
                 arch = "";
                 os = "";
                 load = <0xb2000000>;
                 entry = <0xb2000000>;
                 compression = "none";
             };
         };

   **UBOOT_FITIMAGE_ENABLE**
      This variable allows to generate a FIT image for U-Boot, which is one
      of the ways to implement a verified boot process.

      Its default value is "0", so set it to "1" to enable this functionality::

         UBOOT_FITIMAGE_ENABLE = "1"

      See the ref-classes-uboot-sign class for details.

   **UBOOT_FRAGMENTS**
      The **UBOOT_FRAGMENTS** variable can be used to pass extra config
      fragments from the source tree to `make` when U-Boot is configured.
      These fragments are located in same `${S}/configs/` directory as the
      defconfig.

      For example::

          UBOOT_MACHINE = "am62x_evm_r5_defconfig"
          UBOOT_FRAGMENTS = "am62x_r5_usbdfu.config"

      See the ref-classes-uboot-config class for more information.

   **UBOOT_INITIAL_ENV_BINARY**
      This variable enables the generation of the U-Boot initial environment in
      binary format.

      Its default value is "0", set it to "1" to enable this functionality::

         UBOOT_INITIAL_ENV_BINARY = "1"

      If set to "1", you must also set the size of the environment with
      **UBOOT_INITIAL_ENV_BINARY_SIZE**.

      This variable is used in the ref-classes-uboot-config class.

      The resulting binary can be flashed using WIC  or
      any other flashing method at the environment offset, overriding any
      existing environment if one is present. Below is an example of a WKS file
      to flash the binary::

         part --source rawcopy --sourceparams="file=u-boot-initial-env-sd.bin" --ondisk sda --no-table --offset 4096k

      In this example, the U-Boot initial environment binary
      `u-boot-initial-env-sd.bin` is flashed at offset 4096 kibibyte.

   **UBOOT_INITIAL_ENV_BINARY_REDUND**
      If redundant environment support is enabled in U-boot's configuration,
      this variable should be set to properly generate the redundant environment
      in the output U-boot environment binary file.

      Its default value is "0", set it to "1" to enable this functionality::

         UBOOT_INITIAL_ENV_BINARY_REDUND = "1"

      The **UBOOT_INITIAL_ENV_BINARY** must also be set to "1" if
      **UBOOT_INITIAL_ENV_BINARY_REDUND** is enabled.

      This variable is used in the ref-classes-uboot-config class.

   **UBOOT_INITIAL_ENV_BINARY_SIZE**
      This variable sets the binary size (in bytes) of the U-Boot initial
      environment. Both hexadecimal and decimal values are supported and can be
      set as follows::

         UBOOT_INITIAL_ENV_BINARY_SIZE = "0x4000"

      This variable is used in the ref-classes-uboot-config class.

   **UBOOT_LOADADDRESS**
      Specifies the load address for the U-Boot image. During U-Boot image
      creation, the **UBOOT_LOADADDRESS** variable is passed as a
      command-line parameter to the `uboot-mkimage` utility.

      To pass a 64 bit address, you will also need to set:

      -  The **FIT_ADDRESS_CELLS** variable for FIT image creation.
      -  The **UBOOT_FIT_ADDRESS_CELLS** variable for U-Boot FIT image creation.

      This variable is used by the ref-classes-kernel-fit-image,
      ref-classes-kernel-uimage, ref-classes-kernel,
      ref-classes-uboot-config and ref-classes-uboot-sign
      classes.

   **UBOOT_LOCALVERSION**
      Appends a string to the name of the local version of the U-Boot
      image. For example, assuming the version of the U-Boot image built
      was "2013.10", the full version string reported by U-Boot would be
      "2013.10-yocto" given the following statement::

         UBOOT_LOCALVERSION = "-yocto"

   **UBOOT_MACHINE**
      Specifies the value passed on the `make` command line when building
      a U-Boot image. The value indicates the target platform
      configuration. You typically set this variable from the machine
      configuration file (i.e. `conf/machine/machine_name.conf`).

      Please see the "Selection of Processor Architecture and Board Type"
      section in the U-Boot README for valid values for this variable.

   **UBOOT_MAKE_OPTS**
      The **UBOOT_MAKE_OPTS** variable can be used to pass extra options to
      `make` when U-Boot is configured and compiled.

      See the ref-classes-uboot-config class for more information.

   **UBOOT_MAKE_TARGET**
      Specifies the target called in the `Makefile`. The default target
      is "all".

   **UBOOT_MKIMAGE**
      Specifies the name of the mkimage command as used by the
      ref-classes-kernel-fit-image class to assemble
      the FIT image. This can be used to substitute an alternative command, wrapper
      script or function if desired. The default is "uboot-mkimage".

   **UBOOT_MKIMAGE_DTCOPTS**
      Options for the device tree compiler passed to `mkimage -D` feature
      while creating a FIT image with the ref-classes-kernel-fit-image
      class. If **UBOOT_MKIMAGE_DTCOPTS** is not set then the
      ref-classes-kernel-fit-image class will not pass the `-D` option
      to `mkimage`.

      This variable is also used by the ref-classes-uboot-sign class.

   **UBOOT_MKIMAGE_KERNEL_TYPE**
      Specifies the type argument for the kernel as passed to `uboot-mkimage`.
      The default value is "kernel".

   **UBOOT_MKIMAGE_SIGN**
      Specifies the name of the mkimage command as used by the
      ref-classes-kernel-fit-image class to sign
      the FIT image after it has been assembled (if enabled). This can be used
      to substitute an alternative command, wrapper script or function if
      desired. The default is "${**UBOOT_MKIMAGE**}".

   **UBOOT_MKIMAGE_SIGN_ARGS**
      Optionally specifies additional arguments for the
      ref-classes-kernel-fit-image class to pass to the
      mkimage command when signing the FIT image.

   **UBOOT_RD_ENTRYPOINT**
      Specifies the entrypoint for the RAM disk image. During FIT image
      creation, the **UBOOT_RD_ENTRYPOINT** variable is used in
      ref-classes-kernel-fit-image class to specify the entrypoint to be
      used in creating the Image Tree Source for the FIT image.

   **UBOOT_RD_LOADADDRESS**
      Specifies the load address for the RAM disk image. During FIT image
      creation, the **UBOOT_RD_LOADADDRESS** variable is used in
      ref-classes-kernel-fit-image class to specify the load address to
      be used in creating the Image Tree Source for the FIT image.

   **UBOOT_SIGN_ENABLE**
      Enable signing of FIT image. The default value is "0".

      This variable is defined and used by ref-classes-uboot-config class.

      Additionally, it serves as the default value for the
      **FIT_KERNEL_SIGN_ENABLE** variable, which is
      used by the ref-classes-kernel-fit-image class.

      That means, if **UBOOT_SIGN_ENABLE** is set to "1", the
      ref-classes-kernel-fit-image class will sign the FIT image at
      build-time using the specified private key, and the
      ref-classes-uboot-sign class will inject the corresponding public
      key into U-Boot's device tree. This makes U-Boot verify the
      authenticity and integrity of the FIT image at boot time, providing a
      secure boot workflow that helps prevent unauthorized or tampered images
      from being loaded.

      See `<https://docs.u-boot.org/en/v2025.10/usage/fit/signature.html>`__ for
      more information on FIT signature verification in U-Boot.

   **UBOOT_SIGN_KEYDIR**
      Location of the directory containing the RSA key and certificate used for
      signing the FIT image, used by the ref-classes-kernel-fit-image
      (via the **FIT_KERNEL_SIGN_KEYDIR** variable)
      and ref-classes-uboot-sign classes.

   **UBOOT_SIGN_KEYNAME**
      The name of keys used by the ref-classes-kernel-fit-image class
      (via the **FIT_KERNEL_SIGN_KEYNAME** variable)
      for signing U-Boot FIT image stored in the **UBOOT_SIGN_KEYDIR**
      directory. If we have for example a `dev.key` key and a `dev.crt`
      certificate stored in the **UBOOT_SIGN_KEYDIR** directory, you will
      have to set **UBOOT_SIGN_KEYNAME** to `dev`.

   **UBOOT_SUFFIX**
      Points to the generated U-Boot extension. For example, `u-boot.sb`
      has a `.sb` extension.

      The default U-Boot extension is `.bin`

   **UBOOT_TARGET**
      Specifies the target used for building U-Boot. The target is passed
      directly as part of the "make" command (e.g. SPL and AIS). If you do
      not specifically set this variable, the OpenEmbedded build process
      passes and uses "all" for the target during the U-Boot building
      process.

   **UBOOT_VERSION**
      When naming the files that are installed and deployed, the package version
      (**PV**) and revision (**PR**) are part of the filename. The
      **UBOOT_VERSION** variable contains this information, but can be
      overridden if desired.

   **UKI_CMDLINE**
      When inheriting the ref-classes-uki class, the kernel command line
      to use when booting the `Unified Kernel Image (UKI)
      <https://uapi-group.org/specifications/specs/unified_kernel_image/>`__.
      Defaults to `rootwait root=LABEL=root console=${KERNEL_CONSOLE}`.

   **UKI_CONFIG_FILE**
      When inheriting the ref-classes-uki class, an optional config
      file for the `ukify
      <https://www.freedesktop.org/software/systemd/man/latest/ukify.html>`__
      command.

   **UKI_FILENAME**
      When inheriting the ref-classes-uki class, the output file name
      for the generated `Unified Kernel Image (UKI)
      <https://uapi-group.org/specifications/specs/unified_kernel_image/>`__.
      Defaults to `uki.efi`.

   **UKI_KERNEL_FILENAME**
      When inheriting the ref-classes-uki class, the kernel image file
      name to use as input. Defaults to **KERNEL_IMAGETYPE**.

   **UKI_SB_CERT**
      When inheriting the ref-classes-uki class, optional UEFI
      secureboot certificate matching the private key in **UKI_SB_KEY**.

   **UKI_SB_KEY**
      When inheriting the ref-classes-uki class, optional UEFI
      secureboot private key to sign the `Unified Kernel Image (UKI)
      <https://uapi-group.org/specifications/specs/unified_kernel_image/>`__.

   **UKIFY_CMD**
      When inheriting the ref-classes-uki class,
      `ukify <https://www.freedesktop.org/software/systemd/man/latest/ukify.html>`__ command to build
      `Unified Kernel Image (UKI) <https://uapi-group.org/specifications/specs/unified_kernel_image/>`__.
      Defaults to `ukify build`.

   **UNINATIVE_CHECKSUM**
      When inheriting the ref-classes-uninative class, the
      **UNINATIVE_CHECKSUM** variable flags contain the checksums of the
      uninative tarball as specified by the **UNINATIVE_URL** variable.
      There should be one checksum per tarballs published at
      **UNINATIVE_URL**, which match architectures. For example::

         UNINATIVE_CHECKSUM[aarch64] ?= "812045d826b7fda88944055e8526b95a5a9440bfef608d5b53fd52faab49bf85"
         UNINATIVE_CHECKSUM[i686] ?= "5cc28efd0c15a75de4bcb147c6cce65f1c1c9d442173a220f08427f40a3ffa09"
         UNINATIVE_CHECKSUM[x86_64] ?= "4c03d1ed2b7b4e823aca4a1a23d8f2e322f1770fc10e859adcede5777aff4f3a"

   **UNINATIVE_URL**
      When inheriting the ref-classes-uninative class, the
      **UNINATIVE_URL** variable contains the URL where the uninative
      tarballs are published.

   **UNKNOWN_CONFIGURE_OPT_IGNORE**
      Specifies a list of options that, if reported by the configure script
      as being invalid, should not generate a warning during the
      ref-tasks-configure task. Normally, invalid
      configure options are simply not passed to the configure script (e.g.
      should be removed from **EXTRA_OECONF** or
      **PACKAGECONFIG_CONFARGS**).
      However, there are common options that are passed to all
      configure scripts at a class level, but might not be valid for some
      configure scripts. Therefore warnings about these options are useless.
      For these cases, the options are added to **UNKNOWN_CONFIGURE_OPT_IGNORE**.

      The configure arguments check that uses
      **UNKNOWN_CONFIGURE_OPT_IGNORE** is part of the
      ref-classes-insane class and is only enabled if the
      recipe inherits the ref-classes-autotools class.

   **UNPACKDIR**
      This variable, used by the ref-classes-base class,
      specifies where fetched sources should be unpacked by the
      ref-tasks-unpack task.

   **UPDATERCPN**
      For recipes inheriting the
      ref-classes-update-rc.d class, **UPDATERCPN**
      specifies the package that contains the initscript that is enabled.

      The default value is "${PN}". Given that almost all recipes that
      install initscripts package them in the main package for the recipe,
      you rarely need to set this variable in individual recipes.

   **UPSTREAM_CHECK_COMMITS**
      You can perform a per-recipe check for what the latest upstream
      source code version is by calling `devtool latest-version recipe`. If
      the recipe source code is provided from Git repositories, but
      releases are not identified by Git tags, set **UPSTREAM_CHECK_COMMITS**
      to `1` in the recipe, and the OpenEmbedded build system
      will compare the latest commit with the one currently specified
      by the recipe (**SRCREV**)::

         UPSTREAM_CHECK_COMMITS = "1"

   **UPSTREAM_CHECK_GITTAGREGEX**
      You can perform a per-recipe check for what the latest upstream
      source code version is by calling `devtool latest-version recipe`. If
      the recipe source code is provided from Git repositories, the
      OpenEmbedded build system determines the latest upstream version by
      picking the latest tag from the list of all repository tags.

      You can use the **UPSTREAM_CHECK_GITTAGREGEX** variable to provide a
      regular expression to filter only the relevant tags should the
      default filter not work correctly::

         UPSTREAM_CHECK_GITTAGREGEX = "git_tag_regex"

   **UPSTREAM_CHECK_REGEX**
      Use the **UPSTREAM_CHECK_REGEX** variable to specify a different
      regular expression instead of the default one when the package
      checking system is parsing the page found using
      **UPSTREAM_CHECK_URI**::

         UPSTREAM_CHECK_REGEX = "package_regex"

   **UPSTREAM_CHECK_URI**
      You can perform a per-recipe check for what the latest upstream
      source code version is by calling `devtool latest-version recipe`. If
      the source code is provided from tarballs, the latest version is
      determined by fetching the directory listing where the tarball is and
      attempting to find a later tarball. When this approach does not work,
      you can use **UPSTREAM_CHECK_URI** to provide a different URI that
      contains the link to the latest tarball::

         UPSTREAM_CHECK_URI = "recipe_url"

   **UPSTREAM_VERSION_UNKNOWN**
      You can perform a per-recipe check for what the latest upstream
      source code version is by calling `devtool latest-version recipe`.
      If no combination of the **UPSTREAM_CHECK_URI**, **UPSTREAM_CHECK_REGEX**,
      **UPSTREAM_CHECK_GITTAGREGEX** and **UPSTREAM_CHECK_COMMITS** variables in
      the recipe allows to determine what the latest upstream version is,
      you can set **UPSTREAM_VERSION_UNKNOWN** to `1` in the recipe
      to acknowledge that the check cannot be performed::

         UPSTREAM_VERSION_UNKNOWN = "1"

   **USE_DEVFS**
      Determines if `devtmpfs` is used for `/dev` population. The
      default value used for **USE_DEVFS** is "1" when no value is
      specifically set. Typically, you would set **USE_DEVFS** to "0" for a
      statically populated `/dev` directory.

      See the "dev-manual/device-manager:selecting a device manager" section in
      the Yocto Project Development Tasks Manual for information on how to
      use this variable.

   **USE_NLS**
      Determine if language translations should be built for recipes that can
      build them. This variable can be equal to:

      -  `yes`: translations are enabled.
      -  `no`: translation are disabled.

      Recipes can use the value of this variable to enable language
      translations in their build. Classes such as ref-classes-gettext
      use the value of this variable to enable Gettext 
      support.

   **USE_VT**
      When using
      SysVinit ,
      determines whether or not to run a getty 
      on any virtual terminals in order to enable logging in through those
      terminals.

      The default value used for **USE_VT** is "1" when no default value is
      specifically set. Typically, you would set **USE_VT** to "0" in the
      machine configuration file for machines that do not have a graphical
      display attached and therefore do not need virtual terminal
      functionality.

   **USER_CLASSES**
      A list of classes to globally inherit. These classes are used by the
      OpenEmbedded build system to enable extra features.

      Classes inherited using **USER_CLASSES** must be located in the
      `classes-global/` or `classes/` subdirectories.

      The default list is set in your `local.conf` file::

         USER_CLASSES ?= "buildstats"

      For more information, see
      `conf/templates/default/local.conf.sample` in
      meta-poky .

   **USERADD_DEPENDS**
      Specifies a list of recipes that create users / groups (via
      **USERADD_PARAM** / **GROUPADD_PARAM**) which a recipe
      depends upon. This ensures that those users / groups are available
      when building a recipe.

   **USERADD_ERROR_DYNAMIC**
      If set to `error`, forces the OpenEmbedded build system to produce
      an error if the user identification (`uid`) and group
      identification (`gid`) values are not defined in any of the files
      listed in **USERADD_UID_TABLES** and
      **USERADD_GID_TABLES**. If set to
      `warn`, a warning will be issued instead.

      The default behavior for the build system is to dynamically apply
      `uid` and `gid` values. Consequently, the
      **USERADD_ERROR_DYNAMIC** variable is by default not set. If you plan
      on using statically assigned `gid` and `uid` values, you should
      set the **USERADD_ERROR_DYNAMIC** variable in your `local.conf`
      file as follows::

         USERADD_ERROR_DYNAMIC = "error"

      Overriding the
      default behavior implies you are going to also take steps to set
      static `uid` and `gid` values through use of the
      **USERADDEXTENSION**,
      **USERADD_UID_TABLES**, and
      **USERADD_GID_TABLES** variables.

      .. note::

         There is a difference in behavior between setting
         **USERADD_ERROR_DYNAMIC** to `error` and setting it to `warn`.
         When it is set to `warn`, the build system will report a warning for
         every undefined `uid` and `gid` in any recipe. But when it is set
         to `error`, it will only report errors for recipes that are actually
         built.
         This saves you from having to add static IDs for recipes that you
         know will never be built.

   **USERADD_GID_TABLES**
      Specifies a password file to use for obtaining static group
      identification (`gid`) values when the OpenEmbedded build system
      adds a group to the system during package installation.

      When applying static group identification (`gid`) values, the
      OpenEmbedded build system looks in **BBPATH** for a
      `files/group` file and then applies those `uid` values. Set the
      variable as follows in your `local.conf` file::

         USERADD_GID_TABLES = "files/group"

      .. note::

         Setting the **USERADDEXTENSION** variable to "useradd-staticids"
         causes the build system to use static `gid` values.

   **USERADD_PACKAGES**
      When inheriting the ref-classes-useradd class,
      this variable specifies the individual packages within the recipe
      that require users and/or groups to be added.

      You must set this variable if the recipe inherits the class. For
      example, the following enables adding a user for the main package in
      a recipe::

         USERADD_PACKAGES = "${PN}"

      .. note::

         It follows that if you are going to use the **USERADD_PACKAGES**
         variable, you need to set one or more of the **USERADD_PARAM**,
         **GROUPADD_PARAM**, or **GROUPMEMS_PARAM** variables.

   **USERADD_PARAM**
      When inheriting the ref-classes-useradd class,
      this variable specifies for a package what parameters should pass to
      the `useradd` command if you add a user to the system when the
      package is installed.

      Here is an example from the `dbus` recipe::

         USERADD_PARAM:${PN} = "--system --home ${localstatedir}/lib/dbus \
                                --no-create-home --shell /bin/false \
                                --user-group messagebus"

      For information on the
      standard Linux shell command `useradd`, see
      https://linux.die.net/man/8/useradd.

   **USERADD_UID_TABLES**
      Specifies a password file to use for obtaining static user
      identification (`uid`) values when the OpenEmbedded build system
      adds a user to the system during package installation.

      When applying static user identification (`uid`) values, the
      OpenEmbedded build system looks in **BBPATH** for a
      `files/passwd` file and then applies those `uid` values. Set the
      variable as follows in your `local.conf` file::

         USERADD_UID_TABLES = "files/passwd"

      .. note::

         Setting the **USERADDEXTENSION** variable to "useradd-staticids"
         causes the build system to use static `uid` values.

   **USERADDEXTENSION**
      When set to "useradd-staticids", causes the OpenEmbedded build system
      to base all user and group additions on a static `passwd` and
      `group` files found in **BBPATH**.

      To use static user identification (`uid`) and group identification
      (`gid`) values, set the variable as follows in your `local.conf`
      file: USERADDEXTENSION = "useradd-staticids"

      .. note::

         Setting this variable to use static `uid` and `gid`
         values causes the OpenEmbedded build system to employ the
         ref-classes-useradd class.

      If you use static `uid` and `gid` information, you must also
      specify the `files/passwd` and `files/group` files by setting the
      **USERADD_UID_TABLES** and
      **USERADD_GID_TABLES** variables.
      Additionally, you should also set the
      **USERADD_ERROR_DYNAMIC** variable.

   **VIRTUAL-RUNTIME**
      **VIRTUAL-RUNTIME** is a commonly used prefix for defining virtual
      packages for runtime usage, typically for use in **RDEPENDS**
      or in image definitions.

      An example is `VIRTUAL-RUNTIME_base-utils` that makes it possible
      to either use BusyBox based utilities::

         VIRTUAL-RUNTIME_base-utils = "busybox"

      or their full featured implementations from GNU Coreutils
      and other projects::

         VIRTUAL-RUNTIME_base-utils = "packagegroup-core-base-utils"

      Here are two examples using this virtual runtime package. The
      first one is in :oe_git:`initramfs-framework_1.0.bb
      </openembedded-core/tree/meta/recipes-core/initrdscripts/initramfs-framework_1.0.bb>`::

         RDEPENDS:${PN} += "${VIRTUAL-RUNTIME_base-utils}"

      The second example is in the :oe_git:`core-image-initramfs-boot
      </openembedded-core/tree/meta/recipes-core/images/core-image-initramfs-boot.bb>`
      image definition::

         PACKAGE_INSTALL = "${INITRAMFS_SCRIPTS} ${VIRTUAL-RUNTIME_base-utils} base-passwd"

   **WARN_QA**
      Specifies the quality assurance checks whose failures are reported as
      warnings by the OpenEmbedded build system. You set this variable in
      your distribution configuration file. For a list of the checks you
      can control with this variable, see the
      "ref-classes-insane" section.

   **WATCHDOG_RUNTIME_SEC**
      For the `systemd` recipe, this controls the value of the
      `RuntimeWatchdogSec` option in `/etc/systemd/system.conf`. The default
      value is an empty string.

   **WATCHDOG_TIMEOUT**
      Specifies the timeout in seconds used by the `watchdog-config` recipe
      and also by `systemd` during reboot. The default is 60 seconds.

   **WESTON_USER**
      The **WESTON_USER** variable defines the username for the `Weston`
      user.

   **WESTON_USER_HOME**
      The **WESTON_USER** variable defines the path to the home directory
      for the `Weston` user.

   **WIC_CREATE_EXTRA_ARGS**
      If the **IMAGE_FSTYPES** variable contains "wic", the build
      will generate a
      Wic image 
      automatically when BitBake builds an image recipe. As part of
      this process BitBake will invoke the "`wic create`" command. The
      **WIC_CREATE_EXTRA_ARGS** variable is placed at the end of this
      command which allows the user to supply additional arguments.

      One such useful purpose for this mechanism is to add the `-D` (or
      `--debug`) argument to the "`wic create`" command. This increases the
      amount of debugging information written out to the Wic log during the
      Wic creation process.

   **WIC_SECTOR_SIZE**
      The variable **WIC_SECTOR_SIZE** controls the sector size of Wic
      images. In the background, this controls the value of the
      `PARTED_SECTOR_SIZE` environment variable passed to the `parted`
      command-line utility, used to generated the images. The default value is
      `512`.

      For more information on how to create Wic images, see the
      "dev-manual/wic:creating partitioned images using wic" section in
      the Yocto Project Development Tasks Manual.

   **WIRELESS_DAEMON**
      For `connman` and `packagegroup-base`, specifies the wireless
      daemon to use. The default is "wpa-supplicant" (note that the value
      uses a dash and not an underscore).

   **WKS_FILE**
      Specifies the location of the Wic kickstart file that is used by the
      OpenEmbedded build system to create a partitioned image
      (`image.wic`). For information on how to create a partitioned
      image, see the
      "dev-manual/wic:creating partitioned images using wic"
      section in the Yocto Project Development Tasks Manual. For details on
      the kickstart file format, see the "/ref-manual/kickstart" Chapter.

   **WKS_FILE_DEPENDS**
      When placed in the recipe that builds your image, this variable lists
      build-time dependencies. The **WKS_FILE_DEPENDS** variable is only
      applicable when Wic images are active (i.e. when
      **IMAGE_FSTYPES** contains entries related
      to Wic). If your recipe does not create Wic images, the variable has
      no effect.

      The **WKS_FILE_DEPENDS** variable is similar to the
      **DEPENDS** variable. When you use the variable in
      your recipe that builds the Wic image, dependencies you list in the
      **WKS_FILE_DEPENDS** variable are added to the **DEPENDS** variable.

      With the **WKS_FILE_DEPENDS** variable, you have the possibility to
      specify a list of additional dependencies (e.g. native tools,
      bootloaders, and so forth), that are required to build Wic images.
      Here is an example::

         WKS_FILE_DEPENDS = "some-native-tool"

      In the
      previous example, some-native-tool would be replaced with an actual
      native tool on which the build would depend.

   **WKS_FILES**
      Specifies a list of candidate Wic kickstart files to be used by the
      OpenEmbedded build system to create a partitioned image. Only the
      first one that is found, from left to right, will be used.

      This is only useful when there are multiple `.wks` files that can be
      used to produce an image. A typical case is when multiple layers are
      used for different hardware platforms, each supplying a different
      `.wks` file. In this case, you specify all possible ones through
      **WKS_FILES**.

      If only one `.wks` file is used, set **WKS_FILE** instead.

   **WORKDIR**
      The pathname of the work directory in which the OpenEmbedded build
      system builds a recipe. This directory is located within the
      **TMPDIR** directory structure and is specific to
      the recipe being built and the system for which it is being built.

      The **WORKDIR** directory is defined as follows::

         ${TMPDIR}/work/${MULTIMACH_TARGET_SYS}/${PN}/${EXTENDPE}${PV}-${PR}

      The actual directory depends on several things:

      -  **TMPDIR**: The top-level build output directory
      -  **MULTIMACH_TARGET_SYS**: The target system identifier
      -  **PN**: The recipe name
      -  **EXTENDPE**: The epoch --- if **PE** is not specified, which
         is usually the case for most recipes, then **EXTENDPE** is blank.
      -  **PV**: The recipe version
      -  **PR**: The recipe revision

      As an example, assume a Source Directory top-level folder name
      `bitbake-builds`, a default **Build Directory** at `bitbake-builds/build`, and a
      `qemux86-poky-linux` machine target system. Furthermore, suppose
      your recipe is named `foo_1.3.0-r0.bb`. In this case, the work
      directory the build system uses to build the package would be as
      follows::

         bitbake-builds/build/tmp/work/qemux86-poky-linux/foo/1.3.0-r0

   **XSERVER**
      Specifies the packages that should be installed to provide an X
      server and drivers for the current machine, assuming your image
      directly includes `packagegroup-core-x11-xserver` or, perhaps
      indirectly, includes "x11-base" in
      **IMAGE_FEATURES**.

      The default value of **XSERVER**, if not specified in the machine
      configuration, is "xserver-xorg xf86-video-fbdev xf86-input-evdev".

   **XZ_MEMLIMIT**
      Specifies the maximum memory the xz compression should use as a percentage
      of system memory. If unconstrained the xz compressor can use large amounts of
      memory and become problematic with parallelism elsewhere in the build.
      "50%" has been found to be a good value.

   **XZ_THREADS**
      Specifies the number of parallel threads that should be used when
      using xz compression.

      By default this scales with core count, but is never set less than 2
      to ensure that multi-threaded mode is always used so that the output
      file contents are deterministic. Builds will work with a value of 1
      but the output will differ compared to the output from the compression
      generated when more than one thread is used.

      On systems where many tasks run in parallel, setting a limit to this
      can be helpful in controlling system resource usage.

   **ZSTD_COMPRESSION_LEVEL**
      Specifies the compression level to be used with ZStandard compression
      (from `1` to `19`, set to `3` by default, following upstream choice).
      Higher levels produce smaller files, but take longer to complete.

   **ZSTD_THREADS**
      Specifies the number of parallel threads that should be used when
      using ZStandard compression.

      By default this scales with core count, but is never set less than 2
      to ensure that multi-threaded mode is always used so that the output
      file contents are deterministic. Builds will work with a value of 1
      but the output will differ compared to the output from the compression
      generated when more than one thread is used.

      On systems where many tasks run in parallel, setting a limit to this
      can be helpful in controlling system resource usage.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Variable Context

While you can use most variables in almost any context such as
`.conf`, `.bbclass`, `.inc`, and `.bb` files, some variables are
often associated with a particular locality or context. This chapter
describes some common associations.

## Configuration

The following subsections provide lists of variables whose context is
configuration: distribution, machine, and local.

### Distribution (Distro)

This section lists variables whose configuration context is the
distribution, or distro.

-  **DISTRO**

-  **DISTRO_NAME**

-  **DISTRO_VERSION**

-  **MAINTAINER**

-  **PACKAGE_CLASSES**

-  **TARGET_OS**

-  **TARGET_FPU**

-  **TCMODE**

-  **TCLIBC**

### Machine

This section lists variables whose configuration context is the machine.

-  **TARGET_ARCH**

-  **SERIAL_CONSOLES**

-  **PACKAGE_EXTRA_ARCHS**

-  **IMAGE_FSTYPES**

-  **MACHINE_FEATURES**

-  **MACHINE_EXTRA_RDEPENDS**

-  **MACHINE_EXTRA_RRECOMMENDS**

-  **MACHINE_ESSENTIAL_EXTRA_RDEPENDS**

-  **MACHINE_ESSENTIAL_EXTRA_RRECOMMENDS**

### Local

This section lists variables whose configuration context is the local
configuration through the `local.conf` file.

-  **DISTRO**

-  **MACHINE**

-  **DL_DIR**

-  **BBFILES**

-  **EXTRA_IMAGE_FEATURES**

-  **PACKAGE_CLASSES**

-  **BB_NUMBER_THREADS**

-  **BBINCLUDELOGS**

-  **ENABLE_BINARY_LOCALE_GENERATION**

## Recipes

The following subsections provide lists of variables whose context is
recipes: required, dependencies, path, and extra build information.

### Required

This section lists variables that are required for recipes.

-  **LICENSE**

-  **LIC_FILES_CHKSUM**

-  **SRC_URI** --- used in recipes that fetch local or remote files.

### Dependencies

This section lists variables that define recipe dependencies.

-  **DEPENDS**

-  **RDEPENDS**

-  **RRECOMMENDS**

-  **RCONFLICTS**

-  **RREPLACES**

### Paths

This section lists variables that define recipe paths.

-  **WORKDIR**

-  **S**

-  **FILES**

### Extra Build Information

This section lists variables that define extra build information for
recipes.

-  **DEFAULT_PREFERENCE**

-  **EXTRA_OECMAKE**

-  **EXTRA_OECONF**

-  **EXTRA_OEMAKE**

-  **PACKAGECONFIG_CONFARGS**

-  **PACKAGES**

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# FAQ

## General questions

### How does Poky differ from OpenEmbedded?

The term **Poky** refers to the reference distribution that the Yocto
Project provides, and a reference test configuration.

The **Poky** distribution is built using the :term:`OpenEmbedded Build
System` and **BitBake**. The **metadata** involved to build the
**Poky** reference distribution is:

-  **OpenEmbedded-Core (OE-Core)**, which is hosted at
   `/openembedded-core`.

-  meta-poky , which defines the
   `poky` distro configuration file (**DISTRO**).

> **Note:**
> Poky also used to be a repository containing the combination of
> **BitBake**, **OpenEmbedded-Core (OE-Core)**, the
> meta-yocto  repository and the Yocto Project
> documentation -- which were combined with the :yocto_git:`combo-layer
> </poky-config>` tool.
>
> This repository is still hosted at `/poky` and is still
> updated for maintained releases that are older than Whinlatter (5.3).
> Newer releases now use `bitbake-setup` to clone and setup the initial
> **OpenEmbedded Build System** environment. For more information on
> how to use `bitbake-setup`, see the /brief-yoctoprojectqs/index
> document. An alternative can be to setup the above repositories
> manually, by following the /dev-manual/poky-manual-setup
> section of the Yocto Project Development Tasks Manual.
>

### How can you claim Poky / OpenEmbedded-Core is stable?

There are three areas that help with stability;

-  The Yocto Project team keeps **OpenEmbedded-Core (OE-Core)** small and
   focused, containing around a thousand recipes as opposed to the thousands
   available in other OpenEmbedded community layers. Keeping it small
   makes it easy to test and maintain.

-  The Yocto Project team runs manual and automated tests using a small,
   fixed set of reference hardware as well as emulated targets.

-  The Yocto Project uses an `autobuilder <>`, which provides
   continuous build and integration tests.

### Are there any products built using the OpenEmbedded build system?

See :yocto_wiki:`Products that use the Yocto Project
</Project_Users#Products_that_use_the_Yocto_Project>` in the Yocto Project
Wiki. Don't hesitate to contribute to this page if you know other such
products.

### Why isn't systemd the default init system for OpenEmbedded-Core/Yocto Project or in Poky?

`systemd <https://systemd.io/>`__ is a desktop Linux init system with a specific
focus that is not entirely aligned with a customisable "embedded" build
system/environment.

It understandably mandates certain layouts and configurations which may
or may not align with what the objectives and direction :term:`OpenEmbedded-Core
(OE-Core)` or Yocto Project want to take. It doesn't support all of our targets.
For example `musl <https://www.musl-libc.org/>`__ support in systemd is
problematic.

If it were our default, we would have to align with all their choices
and this doesn't make sense. It is therefore a configuration option and
available to anyone where the design goals align. But we are clear it
is not the only way to handle init.

Our automated testing includes it through the `poky-altcfg` **DISTRO** and
we don't really need it to be the default: it is tested, it works, and people
can choose to use it.

## Building environment

### Missing dependencies on the development system?

If your development system does not meet the required Git, tar, and
Python versions, you can get the required tools on your host development
system in different ways (i.e. building a tarball or downloading a
tarball). See the "ref-manual/system-requirements:required git, tar, python, make and gcc versions"
section for steps on how to update your build tools.

### How does OpenEmbedded fetch source code? Will it work through a firewall or proxy server?

The way the build system obtains source code is highly
configurable. You can setup the build system to get source code in most
environments if HTTP transport is available.

When the build system searches for source code, it first tries the local
download directory (**DL_DIR**). If that location fails, the :term:`OpenEmbedded Build
System` tries **PREMIRRORS**, the upstream source (specified in
**SRC_URI**), and then **MIRRORS** in that order.

Assuming your distribution is "poky", the OpenEmbedded build system uses
the Yocto Project source **PREMIRRORS** by default for SCM-based
sources, upstreams for normal tarballs, and then falls back to a number
of other mirrors including the Yocto Project source mirror if those
fail.

As an example, you could add a specific server for the build system to
attempt before any others by adding something like the following to the
`local.conf` configuration file::

   PREMIRRORS:prepend = "\
       git://.*/.* &YOCTO_DL_URL;/mirror/sources/ \
       ftp://.*/.* &YOCTO_DL_URL;/mirror/sources/ \
       http://.*/.* &YOCTO_DL_URL;/mirror/sources/ \
       https://.*/.* &YOCTO_DL_URL;/mirror/sources/"

These changes cause the build system to intercept Git, FTP, HTTP, and
HTTPS requests and direct them to the `http://` sources mirror. You
can use `file://` URLs to point to local directories or network shares
as well.

Another option is to set::

   BB_NO_NETWORK = "1"

This statement tells BitBake to issue an error
instead of trying to access the Internet. This technique is useful if
you want to ensure code builds only from local sources.

Here is another technique::

   BB_FETCH_PREMIRRORONLY = "1"

This statement limits the build system to pulling source from the
**PREMIRRORS** only.  Again, this technique is useful for reproducing
builds.

Here is yet another technique::

   BB_GENERATE_MIRROR_TARBALLS = "1"

This statement tells the build system to generate mirror tarballs. This
technique is useful if you want to create a mirror server. If not,
however, the technique can simply waste time during the build.

Finally, consider an example where you are behind an HTTP-only firewall.
You could make the following changes to the `local.conf` configuration
file as long as the **PREMIRRORS** server is current::

   PREMIRRORS:prepend = "\
       git://.*/.* &YOCTO_DL_URL;/mirror/sources/ \
       ftp://.*/.* &YOCTO_DL_URL;/mirror/sources/ \
       http://.*/.* &YOCTO_DL_URL;/mirror/sources/ \
       https://.*/.* &YOCTO_DL_URL;/mirror/sources/"
   BB_FETCH_PREMIRRORONLY = "1"

These changes would cause the build system to successfully fetch source
over HTTP and any network accesses to anything other than the
**PREMIRRORS** would fail.

Most source fetching by the OpenEmbedded build system is done by
`wget` and you therefore need to specify the proxy settings in a
`.wgetrc` file, which can be in your home directory if you are a
single user or can be in `/usr/local/etc/wgetrc` as a global user
file.

Here is the applicable code for setting various proxy types in the
`.wgetrc` file. By default, these settings are disabled with comments.
To use them, remove the comments::

   # You can set the default proxies for Wget to use for http, https, and ftp.
   # They will override the value in the environment.
   #https_proxy = http://proxy.yoyodyne.com:18023/
   #http_proxy = http://proxy.yoyodyne.com:18023/
   #ftp_proxy = http://proxy.yoyodyne.com:18023/

   # If you do not want to use proxy at all, set this to off.
   #use_proxy = on

The build system also accepts `http_proxy`, `ftp_proxy`, `https_proxy`,
and `all_proxy` set as to standard shell environment variables to redirect
requests through proxy servers.

The Yocto Project also includes a
`meta-poky/conf/templates/default/site.conf.sample` file that shows
how to configure a Git proxy server if needed.

> **Note:**
> You can find more information on the
> "Working Behind a Network Proxy "
> Wiki page.
>

## Using the OpenEmbedded Build system

### How do I use an external toolchain?

See the "dev-manual/external-toolchain:optionally using an external toolchain"
section in the Development Task manual.

### Why do I get chmod permission issues?

If you see the error
`chmod: XXXXX new permissions are r-xrwxrwx, not r-xr-xr-x`,
you are probably running the build on an NTFS filesystem. Instead,
run the build system on a partition with a modern Linux filesystem such as
`ext4`, `btrfs` or `xfs`.

### I see many 404 errors trying to download sources. Is anything wrong?

Nothing is wrong. The OpenEmbedded build system checks any
configured source mirrors before downloading from the upstream sources.
The build system does this searching for both source archives and
pre-checked out versions of SCM-managed software. These checks help in
large installations because it can reduce load on the SCM servers
themselves. This can also allow builds to continue to work if an
upstream source disappears.

### Why do I get random build failures?

If the same build is failing in totally different and random
ways, the most likely explanation is:

-  The hardware you are running the build on has some problem.

-  You are running the build under virtualization, in which case the
   virtualization probably has bugs.

The OpenEmbedded build system processes a massive amount of data that
causes lots of network, disk and CPU activity and is sensitive to even
single-bit failures in any of these areas. True random failures have
always been traced back to hardware or virtualization issues.

### Why does the build fail with ``iconv.h`` problems?

When you try to build a native recipe, you may get an error message that
indicates that GNU `libiconv` is not in use but `iconv.h` has been
included from `libiconv`::

   #error GNU libiconv not in use but included iconv.h is from libiconv

When this happens, you need to check whether you have a previously
installed version of the header file in `/usr/local/include/`.
If that's the case, you should either uninstall it or temporarily rename
it and try the build again.

This issue is just a single manifestation of "system leakage" issues
caused when the OpenEmbedded build system finds and uses previously
installed files during a native build. This type of issue might not be
limited to `iconv.h`. Make sure that leakage cannot occur from
`/usr/local/include` and `/opt` locations.

### Why don't other recipes find the files provided by my ``*-native`` recipe?

Files provided by your native recipe could be missing from the native
sysroot, your recipe could also be installing to the wrong place, or you
could be getting permission errors during the ref-tasks-install
task in your recipe.

This situation happens when the build system used by a package does not
recognize the environment variables supplied to it by **BitBake**. The
incident that prompted this FAQ entry involved a Makefile that used an
environment variable named `BINDIR` instead of the more standard
variable `bindir`. The makefile's hardcoded default value of
"/usr/bin" worked most of the time, but not for the recipe's `-native`
variant. For another example, permission errors might be caused by a
Makefile that ignores `DESTDIR` or uses a different name for that
environment variable. Check the build system of the package to see if
these kinds of issues exist.

### Can I get rid of build output so I can start over?

Yes --- you can easily do this. When you use BitBake to build an
image, all the build output goes into the directory created when you run
the build environment setup script (i.e.  structure-core-script).
By default, this **Build Directory** is named `build` but can be named
anything you want.

Within the **Build Directory**, is the `tmp` directory. To remove all the
build output yet preserve any source code or downloaded files from
previous builds, simply remove the `tmp` directory.

### Why isn't there a way to append bbclass files like bbappend for recipes?

The Yocto Project has consciously chosen not to implement such functionality.
Class code is designed to be shared and reused, and exposes some level of
configuration to its users. We want to encourage people to share these changes
so we can build the best classes.

If the `append` functionality was available for classes, our evidence and
experience suggest that people would create their custom changes in their
layer instead of sharing and discussing the issues and/or limitations they
encountered. This would lead to bizarre class interactions when new layers are
included. We therefore consciously choose to have a natural pressure to share
class code improvements or fixes.

There are also technical considerations like which recipes a class append would
apply to and how that would fit within the layer model. These are complications
we think we can live without!

### How do I integrate a third-party vulnerability scanning tool?

The **OpenEmbedded Build System** produces SPDX files that contain the
Software Bill Of Materials (SBOM) for any recipe that is built. For this it
uses the ref-classes-create-spdx class. This class also creates a final
SPDX document for an image recipe, found in the deployment directory
(**DEPLOY_DIR_IMAGE**). See the /dev-manual/sbom section of the
Yocto Project Development Tasks Manual for more information of SBOMs generated
by the **OpenEmbedded Build System**.

Some third-party vulnerability scanning tools have existing integration with the
SPDX documents generated by the **OpenEmbedded Build System**. One example
is `Blackduck`, which offers the `bd_scan_yocto_via_sbom
<https://github.com/blackducksoftware/bd_scan_yocto_via_sbom>`__ utility to scan
the vulnerabilities using the SPDX document as input.

These third-party tools have the responsibility of providing support for
integrating with the Yocto Project SBOMs.

Also see the /security-manual/vulnerabilities section of the Yocto Project
Development Tasks Manual for more information on dealing with vulnerabilities.

## Customizing generated images

### What does the OpenEmbedded build system produce as output?

Because you can use the same set of recipes to create output of
various formats, the output of an OpenEmbedded build depends on how you
start it. Usually, the output is a flashable image ready for the target
device.

### How do I make the Yocto Project support my board?

Support for an additional board is added by creating a Board
Support Package (BSP) layer for it. For more information on how to
create a BSP layer, see the
"dev-manual/layers:understanding and creating layers"
section in the Yocto Project Development Tasks Manual and the
/bsp-guide/index.

Usually, if the board is not completely exotic, adding support in the
Yocto Project is fairly straightforward.

### How do I make the Yocto Project support my package?

To add a package, you need to create a BitBake recipe. For
information on how to create a BitBake recipe, see the
"dev-manual/new-recipe:writing a new recipe"
section in the Yocto Project Development Tasks Manual.

### What do I need to ship for license compliance?

This is a difficult question and you need to consult your lawyer
for the answer for your specific case. It is worth bearing in mind that
for GPL compliance, there needs to be enough information shipped to
allow someone else to rebuild and produce the same end result you are
shipping. This means sharing the source code, any patches applied to it,
and also any configuration information about how that package was
configured and built.

You can find more information on licensing in the
"overview-manual/development-environment:licensing"
section in the Yocto Project Overview and Concepts Manual and also in the
"dev-manual/licenses:maintaining open source license compliance during your product's lifecycle"
section in the Yocto Project Development Tasks Manual.

### Do I have to make a full reflash after recompiling one package?

The OpenEmbedded build system can build packages in various
formats such as IPK for OPKG, Debian package (`.deb`), or RPM. You can
then upgrade only the modified packages using the package tools on the device,
much like on a desktop distribution such as Ubuntu or Fedora. However,
package management on the target is entirely optional.

### How to prevent my package from being marked as machine specific?

If you have machine-specific data in a package for one machine only
but the package is being marked as machine-specific in all cases,
you can set **SRC_URI_OVERRIDES_PACKAGE_ARCH** = "0" in the `.bb` file.
However, but make sure the package is manually marked as machine-specific for the
case that needs it. The code that handles **SRC_URI_OVERRIDES_PACKAGE_ARCH**
is in the `meta/classes-global/base.bbclass` file.

### What's the difference between ``target`` and ``target-native``?

The `*-native` targets are designed to run on the system being
used for the build. These are usually tools that are needed to assist
the build in some way such as `quilt-native`, which is used to apply
patches. The non-native version is the one that runs on the target
device.

### Why do ``${bindir}`` and ``${libdir}`` have strange values for ``-native`` recipes?

Executables and libraries might need to be used from a directory
other than the directory into which they were initially installed.
Complicating this situation is the fact that sometimes these executables
and libraries are compiled with the expectation of being run from that
initial installation target directory. If this is the case, moving them
causes problems.

This scenario is a fundamental problem for package maintainers of
mainstream Linux distributions as well as for the OpenEmbedded build
system. As such, a well-established solution exists. Makefiles,
Autotools configuration scripts, and other build systems are expected to
respect environment variables such as `bindir`, `libdir`, and
`sysconfdir` that indicate where executables, libraries, and data
reside when a program is actually run. They are also expected to respect
a `DESTDIR` environment variable, which is prepended to all the other
variables when the build system actually installs the files. It is
understood that the program does not actually run from within
`DESTDIR`.

When the OpenEmbedded build system uses a recipe to build a
target-architecture program (i.e. one that is intended for inclusion on
the image being built), that program eventually runs from the root file
system of that image. Thus, the build system provides a value of
"/usr/bin" for `bindir`, a value of "/usr/lib" for `libdir`, and so
forth.

Meanwhile, `DESTDIR` is a path within the **Build Directory**.
However, when the recipe builds a native program (i.e. one that is
intended to run on the build machine), that program is never installed
directly to the build machine's root file system. Consequently, the build
system uses paths within the Build Directory for `DESTDIR`, `bindir`
and related variables. To better understand this, consider the following
two paths (artificially broken across lines for readability) where the
first is relatively normal and the second is not::

   /home/maxtothemax/poky-bootchart2/build/tmp/work/i586-poky-linux/zlib/
      1.2.8-r0/sysroot-destdir/usr/bin

   /home/maxtothemax/poky-bootchart2/build/tmp/work/x86_64-linux/
      zlib-native/1.2.8-r0/sysroot-destdir/home/maxtothemax/poky-bootchart2/
      build/tmp/sysroots/x86_64-linux/usr/bin

Even if the paths look unusual, they both are correct --- the first for
a target and the second for a native recipe. These paths are a consequence
of the `DESTDIR` mechanism and while they appear strange, they are correct
and in practice very effective.

### How do I create images with more free space?

By default, the OpenEmbedded build system creates images that are
1.3 times the size of the populated root filesystem. To affect the image
size, you need to set various configurations:

-  *Image Size:* The OpenEmbedded build system uses the
   **IMAGE_ROOTFS_SIZE** variable to define
   the size of the image in Kbytes. The build system determines the size
   by taking into account the initial root filesystem size before any
   modifications such as requested size for the image and any requested
   additional free disk space to be added to the image.

-  *Overhead:* Use the
   **IMAGE_OVERHEAD_FACTOR** variable
   to define the multiplier that the build system applies to the initial
   image size, which is 1.3 by default.

-  *Additional Free Space:* Use the
   **IMAGE_ROOTFS_EXTRA_SPACE**
   variable to add additional free space to the image. The build system
   adds this space to the image after it determines its
   **IMAGE_ROOTFS_SIZE**.

### Why aren't spaces in path names supported?

The Yocto Project team has tried to do this before but too many
of the tools the OpenEmbedded build system depends on, such as
`autoconf`, break when they find spaces in pathnames. Until that
situation changes, the team will not support spaces in pathnames.

### I'm adding a binary in a recipe. Why is it different in the image?

The first most obvious change is the system stripping debug symbols from
it. Setting **INHIBIT_PACKAGE_STRIP** to stop debug symbols being
stripped and/or **INHIBIT_PACKAGE_DEBUG_SPLIT** to stop debug symbols
being split into a separate file will ensure the binary is unchanged.

## Issues on the running system

### How do I disable the cursor on my touchscreen device?

You need to create a form factor file as described in the
"bsp-guide/bsp:miscellaneous bsp-specific recipe files" section in
the Yocto Project Board Support Packages (BSP) Developer's Guide. Set
the `HAVE_TOUCHSCREEN` variable equal to one as follows::

   HAVE_TOUCHSCREEN=1

### How to always bring up connected network interfaces?

The default interfaces file provided by the netbase recipe does
not automatically bring up network interfaces. Therefore, you will need
to add a BSP-specific netbase that includes an interfaces file. See the
"bsp-guide/bsp:miscellaneous bsp-specific recipe files" section in
the Yocto Project Board Support Packages (BSP) Developer's Guide for
information on creating these types of miscellaneous recipe files.

For example, add the following files to your layer::

   meta-MACHINE/recipes-bsp/netbase/netbase/MACHINE/interfaces
   meta-MACHINE/recipes-bsp/netbase/netbase_5.0.bbappend

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Contributions and Additional Information

## Introduction

The Yocto Project team is happy for people to experiment with the Yocto
Project. There is a number of places where you can find help if you run into
difficulties or find bugs. This presents information about contributing
and participating in the Yocto Project.

## Contributions

The Yocto Project gladly accepts contributions. You can submit changes
to the project either by creating and sending pull requests, or by
submitting patches through email. For information on how to do both as
well as information on how to identify the maintainer for each area of
code, see the /contributor-guide/index.

## Yocto Project Bugzilla

The Yocto Project uses its own implementation of
`Bugzilla <>` to track defects (bugs).
Implementations of Bugzilla work well for group development because they
track bugs and code changes, can be used to communicate changes and
problems with developers, can be used to submit and review patches, and
can be used to manage quality assurance.

Sometimes it is helpful to submit, investigate, or track a bug against
the Yocto Project itself (e.g. when discovering an issue with some
component of the build system that acts contrary to the documentation or
your expectations).

For a general procedure and guidelines on how to use Bugzilla to submit a bug
against the Yocto Project, see the following:

-  The "/contributor-guide/report-defect"
   section in the Yocto Project and OpenEmbedded Contributor Guide.

-  The Yocto Project Bugzilla wiki page 

For information on Bugzilla in general, see https://www.bugzilla.org/about/.

## Mailing lists

There are multiple mailing lists maintained by the Yocto Project as well
as related OpenEmbedded mailing lists for discussion, patch submission
and announcements. To subscribe to one of the following mailing lists,
click on the appropriate URL in the following list and follow the
instructions:

-  `/g/yocto` --- general Yocto Project
   discussion mailing list.

-  `/g/yocto-patches` --- patch contribution mailing list for Yocto
   Project-related layers which do not have their own mailing list.

-  `/g/openembedded-core` --- discussion mailing
   list about OpenEmbedded-Core (the core metadata).

-  `/g/openembedded-devel` --- discussion
   mailing list about OpenEmbedded.

-  `/g/bitbake-devel` --- discussion mailing
   list about the **BitBake** build tool.

-  `/g/poky` --- discussion mailing list
   about **Poky**.

-  `/g/yocto-announce` --- mailing list to
   receive official Yocto Project release and milestone announcements.

-  `/g/docs` --- discussion mailing list about the Yocto Project
   documentation.

See also the description of all mailing lists .

## Internet Relay Chat (IRC)

Two IRC channels on `Libera Chat <https://libera.chat/>`__
are available for the Yocto Project and OpenEmbedded discussions:

-  `#yocto`

-  `#oe`

## Links and Related Documentation

Here is a list of resources you might find helpful:

-  `The Yocto Project Website <>`: The home site
   for the Yocto Project.

-  `The Yocto Project Main Wiki Page <>`: The main wiki page for
   the Yocto Project. This page contains information about project
   planning, release engineering, QA & automation, a reference site map,
   and other resources related to the Yocto Project.

-  `OpenEmbedded <>`: The build system used by the
   Yocto Project. This project is the upstream, generic, embedded
   distribution from which the Yocto Project derives its build system
   (Poky) and to which it contributes.

-  `BitBake </BitBake>`: The tool used to process metadata.

-  BitBake User Manual : A comprehensive
   guide to the BitBake tool. If you want information on BitBake, see
   this manual.

-  /brief-yoctoprojectqs/index: This
   short document lets you experience building an image using the Yocto
   Project without having to understand any concepts or details.

-  /overview-manual/index: This manual provides overview
   and conceptual information about the Yocto Project.

-  /dev-manual/index: This manual is a "how-to" guide
   that presents procedures useful to both application and system
   developers who use the Yocto Project.

-  /sdk-manual/index manual: This
   guide provides information that lets you get going with the standard
   or extensible SDK. An SDK, with its cross-development toolchains,
   allows you to develop projects inside or outside of the Yocto Project
   environment.

-  /bsp-guide/bsp: This guide defines the structure
   for BSP components. Having a commonly understood structure encourages
   standardization.

-  /kernel-dev/index: This manual describes
   how to work with Linux Yocto kernels as well as provides a bit of
   conceptual information on the construction of the Yocto Linux kernel
   tree.

-  /ref-manual/index: This
   manual provides reference material such as variable, task, and class
   descriptions.

-  `Yocto Project Mega-Manual </singleindex.html>`: This manual
   is simply a single HTML file comprised of the bulk of the Yocto
   Project manuals. It makes it easy to search for phrases and terms used
   in the Yocto Project documentation set.

-  /profile-manual/index: This manual presents a set of
   common and generally useful tracing and profiling schemes along with
   their applications (as appropriate) to each tool.

-  /toaster-manual/index: This manual
   introduces and describes how to set up and use Toaster. Toaster is an
   Application Programming Interface (API) and web-based interface to
   the **OpenEmbedded Build System**, which uses
   BitBake, that reports build information.

-  `Yocto Project BitBake extension for VSCode
   <https://marketplace.visualstudio.com/items?itemName=yocto-project.yocto-bitbake>`__:
   This extension provides a rich feature set when working with BitBake recipes
   within the Visual Studio Code IDE.

-  FAQ : A list of commonly asked
   questions and their answers.

-  Release Information :
   Migration guides, release notes, new features, updates and known issues
   for the current and past releases of the Yocto Project.

-  `Bugzilla <>`: The bug tracking application
   the Yocto Project uses. If you find problems with the Yocto Project,
   you should report them using this application.

-  :yocto_wiki:`Bugzilla Configuration and Bug Tracking Wiki Page
   </Bugzilla_Configuration_and_Bug_Tracking>`:
   Information on how to get set up and use the Yocto Project
   implementation of Bugzilla for logging and tracking Yocto Project
   defects.

-  Internet Relay Chat (IRC): Two IRC channels on
   `Libera Chat <https://libera.chat/>`__ are
   available for Yocto Project and OpenEmbeddded discussions: `#yocto` and
   `#oe`, respectively.

-  `Quick EMUlator (QEMU) <https://wiki.qemu.org/Index.html>`__: An
   open-source machine emulator and virtualizer.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Yocto Project Supported Architectures And Features

The Yocto Project is putting continuous efforts into testing the changes made to
the **OpenEmbedded-Core (OE-Core)** metadata and core tools. The details on
how this test environment functions is described in the
/test-manual/index.

These tests are also run for stable and **LTS** versions of the Yocto
Project. See the /ref-manual/release-process section of the Yocto Project
Reference Manual for more information on these types of releases.

The infrastructure behind the test environment is the
:ref:`Yocto Project Autobuilder <test-manual/intro:Yocto Project Autobuilder
Overview>`. The Autobuilder contains a set of Builders that are associated to an
architecture or a feature to test. For example, the `qemuarm64` builder
corresponds to testing the ARM 64-bit architecture.

Below is a comprehensive list of target architectures and features that are
supported, as well as their level of support. For each architecture or feature,
their corresponding builders are also listed.

## Primary Supported

The term "primary" means that dedicated builds for these architectures or
features are being run on a daily basis on the Yocto Project Autobuilder and
also tested with incoming changes before they merge. These changes are usually
on the "-next" Git branches of the **OpenEmbedded-Core (OE-Core)**
repositories.

Below is a list of primary tested features, their maintainer(s) and builder(s):

.. list-table::
   :widths: 20 20 20 40
   :header-rows: 1

   * - Feature
     - Description
     - Maintainer(s)
     - Builder(s)
   * - ARM 
     - ARM architecture testing
     - Collective effort
     - genericarm64,
       genericarm64-alt,
       musl-qemuarm64,
       qemuarm,
       qemuarm-alt,
       qemuarm-oecore,
       qemuarm-tc,
       qemuarm64,
       qemuarm64-alt,
       qemuarm64-armhost,
       qemuarm64-ltp,
       qemuarm64-ptest,
       qemuarm64-tc,
       qemuarmv5
   * - Beaglebone 
     - Beaglebone image and SDK build testing
     - Collective effort
     - beaglebone,
       beaglebone-alt
   * - Reproducible 
     - reproducibility testing
     - Collective effort
     - reproducible
   * - **Buildtools**
     - Buildtools generation
     - Collective effort
     - buildtools
   * - `meta-agl-core <https://gerrit.automotivelinux.org/gerrit/AGL/meta-agl>`__
     - meta-agl-core layer testing
     - TBD
     - meta-agl-core
   * - `meta-arm <https://git.yoctoproject.org/meta-arm>`__
     - meta-arm layer testing
     - meta-arm mailing list <meta-arm@lists.yoctoproject.org>
     - meta-arm
   * - `meta-aws <https://github.com/aws4embeddedlinux/meta-aws>`__
     - meta-aws layer testing
     - TBD
     - meta-aws
   * - `meta-intel <https://git.yoctoproject.org/meta-intel>`__
     - meta-intel layer testing
     - meta-intel mailing list <meta-intel@lists.yoctoproject.org>
     - meta-intel
   * - `meta-exein <https://github.com/exein-io/meta-exein>`__
     - meta-exein layer testing
     - TBD
     - meta-exein
   * - `meta-webosose <https://github.com/webosose/meta-webosose>`__
     - meta-webosose layer testing
     - TBD
     - meta-webosose
   * - Multilib 
     - Multilib feature testing
     - Collective effort
     - multilib
   * - **OpenEmbedded-Core selftest<OpenEmbedded-Core (OE-Core)>**
     - OpenEmbedded-Core layers selftests
     - Collective effort
     - oe-selftest-fedora,
       oe-selftest-debian,
       oe-selftest-armhost
   * - Package managers
     - Package managers (RPM, DEB and IPK formats) testing in the
       **OpenEmbedded Build System** (different from the
       `package-management` **image feature <IMAGE_FEATURES>**)
     - Collective effort
     - pkgman-non-rpm (other builders use RPM by default)
   * - Patchtest 
     - Patchtest tool selftests
     - Collective effort
     - patchtest-selftest
   * - RISC-V (64-bit) 
     - RISC-V architecture testing (64-bit)
     - Collective effort
     - qemuriscv64,
       qemuriscv64-ptest,
       qemuriscv64-tc
   * - systemd 
     - Systemd init manager testing
     - Collective effort
     - no-x11, qa-extras2
   * - **Toaster**
     - Toaster web interface testing
     - Collective effort
     - toaster
   * - Wic 
     - WIC image creation testing
     - Collective effort
     - wic
   * - X86 
     - X86 architecture testing
     - Collective effort
     - genericx86,
       genericx86-64,
       genericx86-64-alt,
       genericx86-alt,
       musl-qemux86,
       musl-qemux86-64,
       qemux86,
       qemux86-64,
       qemux86-64-alt,
       qemux86-64-ltp,
       qemux86-64-ptest,
       qemux86-64-tc,
       qemux86-64-x32,
       qemux86-alt,
       qemux86-tc,
       qemux86-world,
       qemux86-world-alt

## Secondary Supported

The term "secondary" means that in some cases there is code/feature/support
which is desired by people using the project and is in the project's interests
to support, however there isn't wide enough interest and support to justify
testing all incoming changes on it. There are however project member
organisations and maintainers willing to run tests and review fixes.

This category may be applicable as support/usage in an area develops and grows,
or as support/usage fades but we continue to have tests. It can also apply where
resourcing isn't available for full primary support but there is
member/maintainer support for running tests.

We therefore have the following criteria and policies for such items:

-  It can be clearly isolated and defined by specific configuration.

-  There is a clear documented group of maintainers agreeing to maintain it.

-  Those maintainers are active and responsive.

-  It is being actively and publicly tested (potentially using
   the Autobuilder 
   by agreement, or otherwise).

-  Testing would not be part of standard incoming change testing and regressions
   would not block incoming patches.

-  The SWAT  team would not handle
   any test builds on the Autobuilder.

-  Test results can be submitted as part of the release process if desired.

The Yocto Project Technical Steering Committee (TSC)  makes
decisions on features in this status and Autobuilder testing. Such support would
be dropped if the maintainers/testing were inactive.

If you are interested in providing resources for improving testing please
contact the Technical Steering Committee (TSC) .

Below is a list of secondary tested features, their maintainer(s) and
builder(s):

.. list-table::
   :widths: 20 20 20 40
   :header-rows: 1

   * - Feature
     - Description
     - Maintainer(s)
     - Builder(s)
   * - PowerPC (32-bit) 
     - PowerPC architecture testing (32-bit)
     - Peter Marko,
       Adrian Freihofer
     - qemuppc,
       qemuppc-tc
   * - meta-openembedded 
     - meta-openembedded layer testing
     - Collective effort / openembedded-devel mailing list <openebedded-devel@lists.openembedded.org>
     - meta-oe
   * - `meta-mingw <https://git.yoctoproject.org/meta-mingw>`__
     - mingw based SDKs testing
     - TBD
     - meta-mingw
   * - `meta-virtualization <https://git.yoctoproject.org/meta-virtualization/>`__
     - meta-virtualization layer testing
     - meta-virtualization mailing list <meta-virtualization@lists.yoctoproject.org>
     - meta-virt
   * - RISC-V (32-bit) 
     - RISC-V architecture testing (32-bit)
     - TBD
     - qemuriscv32,
       qemuriscv32,
       qemuriscv32-tc

## Untested

"Untested" means that whilst the configurations are present in the project, we
don't currently run the tests on any regular basis and new changes are not
tested against them. We may take patches in these areas if they make sense but
it is on a best effort only basis.

.. list-table::
   :widths: 20 20 20 40
   :header-rows: 1

   * - Feature
     - Description
     - Maintainer(s)
     - Builder(s)
   * - MIPS 
     - MIPS architecture testing
     - No maintainers
     - qemumips,
       qemumips64,
       qemumips-alt,
       qemumips-tc,
       qemumips64-tc
   * - PowerPC (32-bit)  Systemd
     - PowerPC architecture testing (32-bit) with systemd
     - No maintainers
     - qemuppc-alt
   * - PowerPC (64-bit) 
     - PowerPC architecture testing (64-bit)
     - No maintainers
     - qemuppc64,
       qemuppc64-tc
