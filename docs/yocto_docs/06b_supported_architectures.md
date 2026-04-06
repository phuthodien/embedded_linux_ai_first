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
