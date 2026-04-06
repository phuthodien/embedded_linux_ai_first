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
