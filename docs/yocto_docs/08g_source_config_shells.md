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
