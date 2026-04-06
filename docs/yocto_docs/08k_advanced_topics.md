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
