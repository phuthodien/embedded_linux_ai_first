# Security Manual

# Introduction

Welcome to the Yocto Project Security Manual. This manual provides relevant
procedures necessary for dealing with security-related tasks supported in the
Yocto Project environment. This manual groups related procedures into
higher-level sections. Procedures can consist of high-level steps or low-level
steps depending on the topic.

This manual provides the following:

-  Procedures that help you securing an image with features supported by the
   Yocto Project; for example making a root filesystem read-only.

-  Procedures related to processes outside of the target images; for example how
   to deal with vulnerabilities.

This manual does not provide the following:

-  Procedures on security features implemented outside of
   **OpenEmbedded-Core (OE-Core)**.

-  Documentation on the security mechanisms themselves, which can often be found
   in the documentation of the feature itself. This manual focuses on how to
   integrate the security mechanism within the Yocto Project.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Making Images More Secure

When securing your image is of concern, there are steps, tools, and
variables that you can consider to help you reach the security goals you
need for your particular device. Not all situations are identical when
it comes to making an image secure. Consequently, this section provides
some guidance and suggestions for consideration when you want to make
your image more secure.

> **Note:**
> Because the security requirements and risks are different for every
> type of device, this section cannot provide a complete reference on
> securing your custom OS. It is strongly recommended that you also
> consult other sources of information on embedded Linux system
> hardening and on security.
>

## General Considerations

There are general considerations that help you create more secure images.
You should consider the following suggestions to make your device
more secure:

-  Scan additional code you are adding to the system (e.g. application
   code) by using static analysis tools. Look for buffer overflows and
   other potential security problems.

-  Pay particular attention to the security for any web-based
   administration interface.

   Web interfaces typically need to perform administrative functions and
   tend to need to run with elevated privileges. Thus, the consequences
   resulting from the interface's security becoming compromised can be
   serious. Look for common web vulnerabilities such as
   cross-site-scripting (XSS), unvalidated inputs, and so forth.

   As with system passwords, the default credentials for accessing a
   web-based interface should not be the same across all devices. This
   is particularly true if the interface is enabled by default as it can
   be assumed that many end-users will not change the credentials.

-  Ensure you can update the software on the device to mitigate
   vulnerabilities discovered in the future. This consideration
   especially applies when your device is network-enabled.

-  Regularly scan and apply fixes for CVE security issues affecting
   all software components in the product, see "security-manual/vulnerabilities:checking for vulnerabilities".

-  Regularly update your version of **BitBake** and your layers (e.g.
   **OpenEmbedded-Core (OE-Core)**, meta-yocto ,
   ...), e.g. to apply updates and security fixes from stable and **LTS**
   branches.

-  Ensure you remove or disable debugging functionality before producing
   the final image. For information on how to do this, see the
   "security-manual/securing-images:considerations specific to the openembedded build system"
   section.

-  Ensure you have no network services listening that are not needed.

-  Remove any software from the image that is not needed.

-  Enable hardware support for secure boot functionality when your
   device supports this functionality.

## Security Flags

The Yocto Project has security flags that you can enable that help make
your build output more secure. The security flags are in the
`meta/conf/distro/include/security_flags.inc` file in your
**OpenEmbedded-Core (OE-Core)**.

> **Note:**
> Depending on the recipe, certain security flags are enabled and
> disabled by default.
>

Use the following line in your `local.conf` file or in your custom
distribution configuration file to enable the security compiler and
linker flags for your build::

   require conf/distro/include/security_flags.inc

## Considerations Specific to the OpenEmbedded Build System

You can take some steps that are specific to the OpenEmbedded build
system to make your images more secure:

-  Ensure that "allow-empty-password", "allow-root-login", or
   "empty-root-password" are not one of your selected **IMAGE_FEATURES**.
   When creating a new project, the default is to provide you with an
   initial `local.conf` file that enables these features using the
   **EXTRA_IMAGE_FEATURES**
   variable with the line::

      EXTRA_IMAGE_FEATURES = "allow-empty-password empty-root-password allow-root-login"

   To disable these features, simply comment out that line in your
   `local.conf` file, or make sure **IMAGE_FEATURES** does not contain
   any of these features before producing your final image. Among other things,
   leaving this in place sets the root password as blank, which makes
   logging in for debugging or inspection easy during development but
   also means anyone can easily log in during production.

-  It is possible to set a root password for the image and also to set
   passwords for any extra users you might add (e.g. administrative or
   service type users). When you set up passwords for multiple images or
   users, you should not duplicate passwords.

   To set up passwords, use the ref-classes-extrausers class, which
   is the preferred method. For an example on how to set up both root and
   user passwords, see the "ref-classes-extrausers" section.

   .. note::

      When adding extra user accounts or setting a root password, be
      cautious about setting the same password on every device. If you
      do this, and the password you have set is exposed, then every
      device is now potentially compromised. If you need this access but
      want to ensure security, consider setting a different, random
      password for each device. Typically, you do this as a separate
      step after you deploy the image onto the device.

-  Consider enabling a Mandatory Access Control (MAC) framework such as
   SMACK or SELinux and tuning it appropriately for your device's usage.
   You can find more information in the
   meta-selinux  layer.

## Tools for Hardening Your Image

The Yocto Project provides tools for making your image more secure. You
can find these tools in the `meta-security` layer of the
`Yocto Project Source Repositories <>`.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Creating a Read-Only Root Filesystem

Suppose, for security reasons, you need to disable your target device's
root filesystem's write permissions (i.e. you need a read-only root
filesystem). Or, perhaps you are running the device's operating system
from a read-only storage device. For either case, you can customize your
image for that behavior.

> **Note:**
> Supporting a read-only root filesystem requires that the system and
> applications do not try to write to the root filesystem. You must
> configure all parts of the target system to write elsewhere, or to
> gracefully fail in the event of attempting to write to the root
> filesystem.
>

## Creating the Root Filesystem

To create the read-only root filesystem, simply add the
"read-only-rootfs" feature to your image, normally in one of two ways.
The first way is to add the "read-only-rootfs" image feature in the
image's recipe file via the **IMAGE_FEATURES** variable::

   IMAGE_FEATURES += "read-only-rootfs"

As an alternative, you can add the same feature
from within your **Build Directory**'s `local.conf` file with the
associated **EXTRA_IMAGE_FEATURES** variable, as in::

   EXTRA_IMAGE_FEATURES = "read-only-rootfs"

For more information on how to use these variables, see the
"dev-manual/customizing-images:Customizing Images Using Custom \\`IMAGE_FEATURES\`\` and \`\`EXTRA_IMAGE_FEATURES\`\``"
section. For information on the variables, see
**IMAGE_FEATURES** and
**EXTRA_IMAGE_FEATURES**.

## Post-Installation Scripts and Read-Only Root Filesystem

It is very important that you make sure all post-Installation
(`pkg_postinst`) scripts for packages that are installed into the
image can be run at the time when the root filesystem is created during
the build on the host system. These scripts cannot attempt to run during
the first boot on the target device. With the "read-only-rootfs" feature
enabled, the build system makes sure that all post-installation scripts
succeed at file system creation time. If any of these scripts
still need to be run after the root filesystem is created, the build
immediately fails. These build-time checks ensure that the build fails
rather than the target device fails later during its initial boot
operation.

Most of the common post-installation scripts generated by the build
system for the out-of-the-box Yocto Project are engineered so that they
can run during root filesystem creation (e.g. post-installation scripts
for caching fonts). However, if you create and add custom scripts, you
need to be sure they can be run during this file system creation.

Here are some common problems that prevent post-installation scripts
from running during root filesystem creation:

-  *Not using $D in front of absolute paths:* The build system defines
   `$`\ **D** when the root
   filesystem is created. Furthermore, `$D` is blank when the script
   is run on the target device. This implies two purposes for `$D`:
   ensuring paths are valid in both the host and target environments,
   and checking to determine which environment is being used as a method
   for taking appropriate actions.

-  *Attempting to run processes that are specific to or dependent on the
   target architecture:* You can work around these attempts by using
   native tools, which run on the host system, to accomplish the same
   tasks, or by alternatively running the processes under QEMU, which
   has the `qemu_run_binary` function. For more information, see the
   ref-classes-qemu class.

## Areas With Write Access

With the "read-only-rootfs" feature enabled, any attempt by the target
to write to the root filesystem at runtime fails. Consequently, you must
make sure that you configure processes and applications that attempt
these types of writes do so to directories with write access (e.g.
`/tmp` or `/var/run`).

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Checking for Vulnerabilities

## Vulnerabilities in OpenEmbedded-Core (OE-Core)

The Yocto Project has an infrastructure to track and address unfixed
known security vulnerabilities, as tracked by the public
Common Vulnerabilities and Exposures (CVE) 
database.

The Yocto Project maintains a `list of known vulnerabilities
<https://valkyrie.yocto.io/pub/non-release/patchmetrics/>`__
for packages in **OpenEmbedded-Core (OE-Core)**, tracking the evolution of the number of
unpatched CVEs and the status of patches. Such information is available for
the current development version and for each supported release.

Security is a process, not a product, and thus at any time, a number of security
issues may be impacting **OpenEmbedded-Core (OE-Core)**. It is up to the maintainers, users,
contributors and anyone interested in the issues to investigate and possibly fix them by
updating software components to newer versions or by applying patches to address them.
It is recommended to work with **OpenEmbedded-Core (OE-Core)** upstream maintainers and submit
patches to fix them, see "/contributor-guide/submit-changes" for details.

## Vulnerability check at build time

To enable a check for CVE security vulnerabilities using
ref-classes-cve-check in the specific image or target you are building,
add the following setting to your configuration::

   INHERIT += "cve-check"

The CVE database contains some old incomplete entries which have been
deemed not to impact **OpenEmbedded-Core (OE-Core)**. These CVE entries can be excluded from the
check using build configuration::

   include conf/distro/include/cve-extra-exclusions.inc

With this CVE check enabled, BitBake build will try to map each compiled software component
recipe name and version information to the CVE database and generate recipe and
image specific reports. These reports will contain:

-  metadata about the software component like names and versions

-  metadata about the CVE issue such as description and NVD link

-  for each software component, a list of CVEs which are possibly impacting this version

-  status of each CVE: `Patched`, `Unpatched` or `Ignored`

The status `Patched` means that a patch file to address the security issue has been
applied. `Unpatched` status means that no patches to address the issue have been
applied and that the issue needs to be investigated. `Ignored` means that after
analysis, it has been deemed to ignore the issue as it for example affects
the software component on a different operating system platform.

By default, no NVD API key is used to retrieve data from the CVE database, which
results in larger delays between NVD API requests. See the **NVDCVE_API_KEY**
documentation on how to request and set a NVD API key.

After a build with CVE check enabled, reports for each compiled source recipe will be
found in `build/tmp/deploy/cve`.

For example the CVE check report for the `flex-native` recipe looks like::

   $ cat ./tmp/deploy/cve/flex-native_cve.json
   {
     "version": "1",
     "package": [
       {
         "name": "flex-native",
         "layer": "meta",
         "version": "2.6.4",
         "products": [
           {
             "product": "flex",
             "cvesInRecord": "No"
           },
           {
             "product": "flex",
             "cvesInRecord": "Yes"
           }
         ],
         "issue": [
           {
             "id": "CVE-2006-0459",
             "status": "Patched",
             "link": "https://nvd.nist.gov/vuln/detail/CVE-2006-0459",
             "summary": "flex.skl in Will Estes and John Millaway Fast Lexical Analyzer Generator (flex) before 2.5.33 does not allocate enough memory for grammars containing (1) REJECT statements or (2) trailing context rules, which causes flex to generate code that contains a buffer overflow that might allow context-dependent attackers to execute arbitrary code.",
             "scorev2": "7.5",
             "scorev3": "0.0",
             "scorev4": "0.0",
             "modified": "2024-11-21T00:06Z",
             "vector": "NETWORK",
             "vectorString": "AV:N/AC:L/Au:N/C:P/I:P/A:P",
             "detail": "version-not-in-range"
           },
           {
             "id": "CVE-2016-6354",
             "status": "Patched",
             "link": "https://nvd.nist.gov/vuln/detail/CVE-2016-6354",
             "summary": "Heap-based buffer overflow in the yy_get_next_buffer function in Flex before 2.6.1 might allow context-dependent attackers to cause a denial of service or possibly execute arbitrary code via vectors involving num_to_read.",
             "scorev2": "7.5",
             "scorev3": "9.8",
             "scorev4": "0.0",
             "modified": "2024-11-21T02:55Z",
             "vector": "NETWORK",
             "vectorString": "AV:N/AC:L/Au:N/C:P/I:P/A:P",
             "detail": "version-not-in-range"
           },
           {
             "id": "CVE-2019-6293",
             "status": "Ignored",
             "link": "https://nvd.nist.gov/vuln/detail/CVE-2019-6293",
             "summary": "An issue was discovered in the function mark_beginning_as_normal in nfa.c in flex 2.6.4. There is a stack exhaustion problem caused by the mark_beginning_as_normal function making recursive calls to itself in certain scenarios involving lots of '*' characters. Remote attackers could leverage this vulnerability to cause a denial-of-service.",
             "scorev2": "4.3",
             "scorev3": "5.5",
             "scorev4": "0.0",
             "modified": "2024-11-21T04:46Z",
             "vector": "NETWORK",
             "vectorString": "AV:N/AC:M/Au:N/C:N/I:N/A:P",
             "detail": "upstream-wontfix",
             "description": "there is stack exhaustion but no bug and it is building the parser, not running it, effectively similar to a compiler ICE. Upstream no plans to address this."
           }
         ]
       }
     ]
   }

For images, a summary of all recipes included in the image and their CVEs is also
generated in the JSON format. These `.json` reports can be found
in the `tmp/deploy/images` directory for each compiled image.

At build time CVE check will also throw warnings about `Unpatched` CVEs::

   WARNING: qemu-native-9.2.0-r0 do_cve_check: Found unpatched CVE (CVE-2023-1386)

It is also possible to check the CVE status of individual packages as follows::

   bitbake -c cve_check flex libarchive

## Fixing CVE product name and version mappings

By default, ref-classes-cve-check uses the recipe name **BPN** as CVE
product name when querying the CVE database. If this mapping contains false positives, e.g.
some reported CVEs are not for the software component in question, or false negatives like
some CVEs are not found to impact the recipe when they should, then the problems can be
in the recipe name to CVE product mapping. These mapping issues can be fixed by setting
the **CVE_PRODUCT** variable inside the recipe. This defines the name of the software component in the
upstream `NIST CVE database <https://nvd.nist.gov/>`__.

The variable supports using vendor and product names like this::

   CVE_PRODUCT = "flex_project:flex westes:flex"

In this example we have two possible vendors names,  `flex_project` and `westes`,
with the product name `flex`. With this setting the `flex` recipe only maps to this specific
product and not products from other vendors with same name `flex`.

Similarly, when the recipe version **PV** is not compatible with software versions used by
the upstream software component releases and the CVE database, these can be fixed using
the **CVE_VERSION** variable.

Note that if the CVE entries in the NVD database contain bugs or have missing or incomplete
information, it is recommended to fix the information there directly instead of working
around the issues possibly for a long time in **OpenEmbedded-Core (OE-Core)** side recipes. Feedback to
NVD about CVE entries can be provided through the `NVD contact form <https://nvd.nist.gov/info/contact-form>`__.

## Fixing vulnerabilities in recipes

Suppose a CVE security issue impacts a software component. In that case, it can
be fixed by updating to a newer version, by applying a patch, or by marking it
as patched via **CVE_STATUS** variable flag. For OE-Core master
branches, updating to a more recent software component release with fixes is
the best option, but patches can be applied if releases are not yet available.

For stable branches, we want to avoid API (Application Programming Interface)
or ABI (Application Binary Interface) breakages. When submitting an update,
a minor version update of a component is preferred if the version is
backward-compatible. Many software components have backward-compatible stable
versions, with a notable example of the Linux kernel. However, if the new
version does or likely might introduce incompatibilities, extracting and
backporting patches is preferred.

Here is an example of fixing CVE security issues with patch files,
an example from the `ffmpeg recipe for dunfell </layerindex/recipe/122174>`::

   SRC_URI = "https://www.ffmpeg.org/releases/${BP}.tar.xz \
              file://mips64_cpu_detection.patch \
              file://CVE-2020-12284.patch \
              file://0001-libavutil-include-assembly-with-full-path-from-sourc.patch \
              file://CVE-2021-3566.patch \
              file://CVE-2021-38291.patch \
              file://CVE-2022-1475.patch \
              file://CVE-2022-3109.patch \
              file://CVE-2022-3341.patch \
              file://CVE-2022-48434.patch \
          "

The recipe has both generic and security-related fixes. The CVE patch files are named
according to the CVE they fix.

When preparing the patch file, take the original patch from the upstream repository.
Do not use patches from different distributions, except if it is the only available source.

Modify the patch adding OE-related metadata. We will follow the example of the
`CVE-2022-3341.patch`.

The original `commit message <https://github.com/FFmpeg/FFmpeg/commit/9cf652cef49d74afe3d454f27d49eb1a1394951e.patch/>`__
is::

   From 9cf652cef49d74afe3d454f27d49eb1a1394951e Mon Sep 17 00:00:00 2001
   From: Jiasheng Jiang <jiasheng@iscas.ac.cn>
   Date: Wed, 23 Feb 2022 10:31:59 +0800
   Subject: [PATCH] avformat/nutdec: Add check for avformat_new_stream

   Check for failure of avformat_new_stream() and propagate
   the error code.

   Signed-off-by: Michael Niedermayer <michael@niedermayer.cc>
   ---
    libavformat/nutdec.c | 16 ++++++++++++----
    1 file changed, 12 insertions(+), 4 deletions(-)

For the correct operations of the `cve-check`, it requires the CVE
identification in a `CVE:` tag of the patch file commit message using
the format::

   CVE: CVE-2022-3341

It is also recommended to add the `Upstream-Status:` tag with a link
to the original patch and sign-off by people working on the backport.
If there are any modifications to the original patch, note them in
the `Comments:` tag.

With the additional information, the header of the patch file in OE-core becomes::

   From 9cf652cef49d74afe3d454f27d49eb1a1394951e Mon Sep 17 00:00:00 2001
   From: Jiasheng Jiang <jiasheng@iscas.ac.cn>
   Date: Wed, 23 Feb 2022 10:31:59 +0800
   Subject: [PATCH] avformat/nutdec: Add check for avformat_new_stream

   Check for failure of avformat_new_stream() and propagate
   the error code.

   Signed-off-by: Michael Niedermayer <michael@niedermayer.cc>

   CVE: CVE-2022-3341

   Upstream-Status: Backport [https://github.com/FFmpeg/FFmpeg/commit/9cf652cef49d74afe3d454f27d49eb1a1394951e]

   Comments: Refreshed Hunk
   Signed-off-by: Narpat Mali <narpat.mali@windriver.com>
   Signed-off-by: Bhabu Bindu <bhabu.bindu@kpit.com>
   ---
    libavformat/nutdec.c | 16 ++++++++++++----
    1 file changed, 12 insertions(+), 4 deletions(-)

A good practice is to include the CVE identifier in the patch file name, the patch file
commit message and optionally in the recipe commit message.

CVE checker will then capture this information and change the CVE status to `Patched`
in the generated reports.

If analysis shows that the CVE issue does not impact the recipe due to configuration, platform,
version or other reasons, the CVE can be marked as `Ignored` by using
the **CVE_STATUS** variable flag with appropriate reason which is mapped to `Ignored`.
The entry should have the format like::

   CVE_STATUS[CVE-2016-10642] = "cpe-incorrect: This is specific to the npm package that installs cmake, so isn't relevant to OpenEmbedded"

As mentioned previously, if data in the CVE database is wrong, it is recommended
to fix those issues in the CVE database (NVD in the case of
**OpenEmbedded-Core (OE-Core)**) directly.

Note that if there are many CVEs with the same status and reason, those can be
shared by using the **CVE_STATUS_GROUPS** variable.

Recipes can be completely skipped by CVE check by including the recipe name in
the **CVE_CHECK_SKIP_RECIPE** variable.

## Implementation details

Here's what the ref-classes-cve-check class does to find unpatched CVE IDs.

First the code goes through each patch file provided by a recipe. If a valid CVE ID
is found in the name of the file, the corresponding CVE is considered as patched.
Don't forget that if multiple CVE IDs are found in the filename, only the last
one is considered. Then, the code looks for `CVE: CVE-ID` lines in the patch
file. The found CVE IDs are also considered as patched.
Additionally `CVE_STATUS` variable flags are parsed for reasons mapped to `Patched`
and these are also considered as patched.

Then, the code looks up all the CVE IDs in the NIST database for all the
products defined in **CVE_PRODUCT**. Then, for each found CVE:

-  If the package name (**PN**) is part of
   **CVE_CHECK_SKIP_RECIPE**, it is considered as `Patched`.

-  If the CVE ID has status `CVE_STATUS[<CVE ID>] = "ignored"` or if it's set to
   any reason which is mapped to status `Ignored` via `CVE_CHECK_STATUSMAP`,
   it is  set as `Ignored`.

-  If the CVE ID is part of the patched CVE for the recipe, it is
   already considered as `Patched`.

-  Otherwise, the code checks whether the recipe version (**PV**)
   is within the range of versions impacted by the CVE. If so, the CVE
   is considered as `Unpatched`.

The CVE database is stored in **DL_DIR** and can be inspected using
`sqlite3` command as follows::

   sqlite3 downloads/CVE_CHECK2/nvd*.db .dump | grep CVE-2021-37462

When analyzing CVEs, it is recommended to:

-  study the latest information in `CVE database <https://nvd.nist.gov/vuln/search>`__.

-  check how upstream developers of the software component addressed the issue, e.g.
   what patch was applied, which upstream release contains the fix.

-  check what other Linux distributions like `Debian <https://security-tracker.debian.org/tracker/>`__
   did to analyze and address the issue.

-  follow security notices from other Linux distributions.

-  follow public `open source security mailing lists <https://oss-security.openwall.org/wiki/mailing-lists>`__ for
   discussions and advance notifications of CVE bugs and software releases with fixes.

## Linux kernel vulnerabilities

Since the Linux kernel became a CVE Numbering Authority (CNA), the number of
associated CVEs has increased dramatically. Security teams must address these
CVEs to meet regulatory and customer requirements. Automation on identifying
issues helps to reduce their workload.

**OpenEmbedded-Core (OE-Core)** has two scripts that help to characterize
and filter CVEs affecting the Linux kernel:

-  `openembedded-core/meta/recipes-kernel/linux/generate-cve-exclusions.py`
-  `openembedded-core/scripts/contrib/improve_kernel_cve_report.py`

### ``generate-cve-exclusions.py``

When updating a kernel recipe, a helper script needs to be run manually to
update the **CVE_STATUS** for the kernel recipe. The script can be used
for custom kernels.

First we need to get an updated version of the CVE information from the
`CVE Project`. Run it as follows:

```shell
$ git clone https://github.com/CVEProject/cvelistV5 ~/cvelistV5
```

Or if you have already cloned it, you need to pull the latest data:

```shell
$ git -C ~/cvelistV5 pull
```

Then, generate the **CVE_STATUS** information for the desired version
of the kernel:

```shell
$ ./generate-cve-exclusions.py ~/cvelistV5 <version> > cve-exclusion_<kernel_version>.inc
```

Example:

```shell
$ git clone https://github.com/CVEProject/cvelistV5 ~/cvelistV5
$ cd openembedded-core/meta/recipes-kernel/linux/
$ ./generate-cve-exclusions.py ~/cvelistV5 6.12.27 > ~/meta-custom/recipes-kernel/linux/cve-exclusion_6.12.inc
```

Don't forget to update your kernel recipe with::

   include cve-exclusion_6.12.inc

Then the CVE information will automatically be added in the
ref-classes-cve-check or ref-classes-vex report.

### ``improve_kernel_cve_report.py``

The `openembedded-core/scripts/contrib/improve_kernel_cve_report.py` script
leverages CVE kernel metadata and the **SPDX_INCLUDE_COMPILED_SOURCES**
variable to update a `cve-summary.json` file. It reduces CVE false
positives by 70%-80% and provide detailed responses for all kernel-related
CVEs by analyzing the files used to build the kernel. The script is decoupled from
the build and can be run outside of the **BitBake** environment.

The script uses the output from the ref-classes-vex or
ref-classes-cve-check class as input, together with CVE information from
the Linux kernel CNA to enrich the `cve-summary.json` file with updated CVE
information.

The file name can be specified as argument. Optionally, it can also use the
list of compiled files from the kernel **SPDX** to ignore CVEs that are
not affected because the files are not compiled.

For this, BitBake uses the debug information to extract the sources used to
build a binary. Therefore, it needs to be configured in the kernel to extract
the kernel compiled files.

If you are using the `linux-yocto` recipe, enable it by adding the following
in a **configuration file** or in a `.bbappend`::

   KERNEL_EXTRA_FEATURES:append = " features/debug/debug-kernel.scc"

Or by editing your kernel configuration to include `DWARF4` debug information.

See the kernel-dev/common:Changing the Configuration section of the Yocto
Project Linux Kernel Development Manual for more information.

For the following example, we will consider that the kernel recipe used is
`linux-yocto`. Instructions also apply to other kernel recipes named
differently.

The sources for the kernel are stored under
`tmp/pkgdata/<MACHINE>/debugsources/linux-yocto-debugsources.json.zstd`. In
order to include the information into the **SPDX** file to filter out
source files that are not used to compile the kernel, add the following in a
**configuration file**::

   SPDX_INCLUDE_COMPILED_SOURCES:pn-linux-yocto = "1"

Finally, store either the `recipe-linux-yocto.spdx.json` or the
`linux-yocto-debugsources.json.zstd` outside the **build directory**.

The **SPDX** file is under
`tmp/deploy/spdx/<spdx_version>/<MACHINE>/recipes/recipe-linux-yocto.spdx.json`

Once you have the input data, first you need to clone or fetch the latest CVE
information from https://git.kernel.org:

```shell
$ git clone https://git.kernel.org/pub/scm/linux/security/vulns.git ~/vulns
```

Or if already checked out:

```shell
$ git -C ~/vulns pull
```

Finally, run the script by using one of the examples below. The most exact are
the first two examples, using the old cve-summary.json.

-  Example using `--old-cve-report` as input:

   .. code-block:: shell

      $ python3 openembedded-core/scripts/contrib/improve_kernel_cve_report.py \
         --spdx tmp/deploy/spdx/3.0.1/qemux86_64/recipes/recipe-linux-yocto.spdx.json \
         --datadir ~/vulns \
         --old-cve-report build/tmp/log/cve/cve-summary.json

-  Example using `--debug-sources` file instead of SPDX kernel file:

   .. code-block:: shell

      $ python3 openembedded-core/scripts/contrib/improve_kernel_cve_report.py \
         --debug-sources tmp/pkgdata/qemux86_64/debugsources/linux-yocto-debugsources.json.zstd \
         --datadir ~/vulns \
         --old-cve-report build/tmp/log/cve/cve-summary.json

-  Example using the `--kernel-version`:

   .. code-block:: shell

      $ python3 openembedded-core/scripts/contrib/improve_kernel_cve_report.py \
         --spdx tmp/deploy/spdx/3.0.1/qemux86_64/recipes/recipe-linux-yocto.spdx.json \
         --kernel-version 6.12.27 \
         --datadir ~/vulns

Example output for a CVE for which the status was changed to "Ignored" because
the source files associated to the CVE were not compiled:

```json
{
   "id": "CVE-2025-38384",
   "status": "Ignored",
   "detail": "not-applicable-config",
   "summary": "In the Linux kernel, the following vulnerability has been resolved (...)",
   "description": "Source code not compiled by config. {'drivers/mtd/nand/spi/core.c'}"
}
```

Example of output for a CVE not in range:

```json
{
   "id": "CVE-2025-40017",
   "status": "Patched",
   "detail": "fixed-version",
   "summary": "In the Linux kernel, the following vulnerability has been resolved (...)",
   "description": "only affects 6.15 onwards"
}
```

Example of output for a CVE that is vulnerable:

```json
{
   "id": "CVE-2024-58093",
   "status": "Unpatched",
   "detail": "version-in-range",
   "summary": "In the Linux kernel, the following vulnerability has been resolved (...)",
   "description": "Needs backporting (fixed from 6.15)"
}
```

Example of output for a CVE rejected by the Linux CNA:

```json
{
   "id": "CVE-2025-38380",
   "status": "Ignored",
   "detail": "rejected",
   "summary": "In the Linux kernel, the following vulnerability has been resolved (...)",
   "description": "Rejected by CNA"
}
```
