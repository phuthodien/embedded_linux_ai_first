# Test Environment Manual

# The Yocto Project Test Environment Manual

## Welcome

Welcome to the Yocto Project Test Environment Manual! This manual is a
work in progress. The manual contains information about the testing
environment used by the Yocto Project to make sure each major and minor
release works as intended. All the project's testing infrastructure and
processes are publicly visible and available so that the community can
see what testing is being performed, how it's being done and the current
status of the tests and the project at any given time. It is intended
that other organizations can leverage off the process and testing
environment used by the Yocto Project to create their own automated,
production test environment, building upon the foundations from the
project core.

This manual is a work-in-progress and is being initially loaded with
information from the README files and notes from key engineers:

-  *yocto-autobuilder2:* This
   README.md 
   is the main README which details how to set up the Yocto Project
   Autobuilder. The `yocto-autobuilder2` repository represents the
   Yocto Project's console UI plugin to Buildbot and the configuration
   necessary to configure Buildbot to perform the testing the project
   requires.

-  *yocto-autobuilder-helper:* This README 
   and repository contains Yocto Project Autobuilder Helper scripts and
   configuration. The `yocto-autobuilder-helper` repository contains
   the "glue" logic that defines which tests to run and how to run them.
   As a result, it can be used by any Continuous Improvement (CI) system
   to run builds, support getting the correct code revisions, configure
   builds and layers, run builds, and collect results. The code is
   independent of any CI system, which means the code can work `Buildbot <https://docs.buildbot.net/current/>`__,
   Jenkins, or others. This repository has a branch per release of the
   project defining the tests to run on a per release basis.

## Yocto Project Autobuilder Overview

The Yocto Project Autobuilder collectively refers to the software,
tools, scripts, and procedures used by the Yocto Project to test
released software across supported hardware in an automated and regular
fashion. Basically, during the development of a Yocto Project release,
the Autobuilder tests if things work. The Autobuilder builds all test
targets and runs all the tests.

The Yocto Project uses standard upstream Buildbot to drive its integration and
testing. Buildbot has a plug-in interface that the Yocto Project customizes
using code from the yocto-autobuilder2 
repository, adding its own console UI plugin. The resulting UI plug-in allows
you to visualize builds in a way suited to the project's needs.

A `helper` layer provides configuration and job management through
scripts found in the `yocto-autobuilder-helper` repository. The
`helper` layer contains the bulk of the build configuration
information and is release-specific, which makes it highly customizable
on a per-project basis. The layer is CI system-agnostic and contains a
number of Helper scripts that can generate build configurations from
simple JSON files.

> **Note:**
> The project uses Buildbot for historical reasons but also because
> many of the project developers have knowledge of Python. It is
> possible to use the outer layers from another Continuous Integration
> (CI) system such as Jenkins 
> instead of Buildbot.
>

The following figure shows the Yocto Project Autobuilder stack with a
topology that includes a controller and a cluster of workers:

![ab-test-cluster.png](figures/ab-test-cluster.png)

## Yocto Project Tests --- Types of Testing Overview

The Autobuilder tests different elements of the project by using
the following types of tests:

-  *Build Testing:* Tests whether specific configurations build by
   varying **MACHINE**,
   **DISTRO**, other configuration
   options, and the specific target images being built (or `world`). This is
   used to trigger builds of all the different test configurations on the
   Autobuilder. Builds usually cover many different targets for
   different architectures, machines, and distributions, as well as
   different configurations, such as different init systems. The
   Autobuilder tests literally hundreds of configurations and targets.

   -  *Sanity Checks During the Build Process:* Tests initiated through the
      ref-classes-insane class. These checks ensure the output of the
      builds are correct. For example, does the ELF architecture in the
      generated binaries match the target system? ARM binaries would not work
      in a MIPS system!

-  *Build Performance Testing:* Tests whether or not commonly used steps
   during builds work efficiently and avoid regressions. Tests to time
   commonly used usage scenarios are run through `oe-build-perf-test`.
   These tests are run on isolated machines so that the time
   measurements of the tests are accurate and no other processes
   interfere with the timing results. The project currently tests
   performance on two different distributions, Fedora and Ubuntu, to
   ensure we have no single point of failure and can ensure the
   different distros work effectively.

-  *eSDK Testing:* Image tests initiated through the following command::

      $ bitbake image -c testsdkext

   The tests use the ref-classes-testsdk class and the
   `do_testsdkext` task.

-  *Feature Testing:* Various scenario-based tests are run through the
   OpenEmbedded Self test (oe-selftest) . We test oe-selftest on each of the main distributions
   we support.

-  *Image Testing:* Image tests initiated through the following command::

      $ bitbake image -c testimage

   The tests use the ref-classes-testimage
   class and the ref-tasks-testimage task. See the
   test-manual/runtime-testing:Performing Automated Runtime Testing
   section of the Yocto Project Test Environment Manual for more information.

-  *Layer Testing:* The Autobuilder has the possibility to test whether
   specific layers work with the test of the system. The layers tested
   may be selected by members of the project. Some key community layers
   are also tested periodically.

-  *Package Testing:* A Package Test (ptest) runs tests against packages
   built by the OpenEmbedded build system on the target machine. See the
   :ref:`Testing Packages With
   ptest <test-manual/ptest:Testing Packages With ptest>` section
   in the Yocto Project Development Tasks Manual and the
   "Ptest " Wiki page for more
   information on Ptest.

-  *SDK Testing:* Image tests initiated through the following command::

      $ bitbake image -c testsdk

   The tests use the ref-classes-testsdk class and
   the `do_testsdk` task.

-  *Unit Testing:* Unit tests on various components of the system run
   through bitbake-selftest  and
   oe-selftest .

-  *Automatic Upgrade Helper:* This target tests whether new versions of
   software are available and whether we can automatically upgrade to
   those new versions. If so, this target emails the maintainers with a
   patch to let them know this is possible.

## How Tests Map to Areas of Code

Tests map into the codebase as follows:

-  *bitbake-selftest:*

   These tests are self-contained and test BitBake as well as its APIs,
   which include the fetchers. The tests are located in
   `bitbake/lib/*/tests`.

   Some of these tests run the `bitbake` command, so `bitbake/bin`
   must be added to the `PATH` before running `bitbake-selftest`.
   From within the BitBake repository, run the following::

      $ export PATH=$PWD/bin:$PATH

   After that, you can run the selftest script::

      $ bitbake-selftest

   The default output is quiet and just prints a summary of what was
   run. To see more information, there is a verbose option::

      $ bitbake-selftest -v

   To skip tests that access the Internet, use the `BB_SKIP_NETTESTS`
   variable when running `bitbake-selftest` as follows::

      $ BB_SKIP_NETTESTS=yes bitbake-selftest

   Use this option when you wish to skip tests that access the network,
   which are mostly necessary to test the fetcher modules. To specify
   individual test modules to run, append the test module name to the
   `bitbake-selftest` command. For example, to specify the tests for
   `bb.tests.data.DataExpansions`, run::

      $ bitbake-selftest bb.tests.data.DataExpansions

   You can also specify individual tests by defining the full name and module
   plus the class path of the test, for example::

      $ bitbake-selftest bb.tests.data.DataExpansions.test_one_var

   The tests are based on
   `Python unittest <https://docs.python.org/3/library/unittest.html>`__.

-  *oe-selftest:*

   -  These tests use OE to test the workflows, which include testing
      specific features, behaviors of tasks, and API unit tests.

   -  The tests can take advantage of parallelism through the `-j`
      option, which can specify a number of threads to spread the tests
      across. Note that all tests from a given class of tests will run
      in the same thread. To parallelize large numbers of tests you can
      split the class into multiple units.

   -  The tests are based on
      `Python unittest <https://docs.python.org/3/library/unittest.html>`__.

   -  The code for the tests resides in
      `meta/lib/oeqa/selftest/cases/`.

   -  To run all the tests, enter the following command::

         $ oe-selftest -a

   -  To run a specific test, use the following command form where
      `testname` is the name of the specific test::

         $ oe-selftest -r <testname>

      For example, the following command would run the `tinfoil`
      `getVar` API test::

         $ oe-selftest -r tinfoil.TinfoilTests.test_getvar

      It is also possible to run a set
      of tests. For example the following command will run all of the
      `tinfoil` tests::

         $ oe-selftest -r tinfoil

-  *testimage:*

   -  These tests build an image, boot it, and run tests against the
      image's content.

   -  The code for these tests resides in `meta/lib/oeqa/runtime/cases/`.

   -  You need to set the **IMAGE_CLASSES** variable as follows::

         IMAGE_CLASSES += "testimage"

   -  Run the tests using the following command form::

         $ bitbake image -c testimage

-  *testsdk:*

   -  These tests build an SDK, install it, and then run tests against
      that SDK.

   -  The code for these tests resides in `meta/lib/oeqa/sdk/cases/`.

   -  Run the test using the following command form::

         $ bitbake image -c testsdk

-  *testsdk_ext:*

   -  These tests build an extended SDK (eSDK), install that eSDK, and
      run tests against the eSDK.

   -  The code for these tests resides in `meta/lib/oeqa/sdkext/cases/`.

   -  To run the tests, use the following command form::

         $ bitbake image -c testsdkext

-  *oe-build-perf-test:*

   -  These tests run through commonly used usage scenarios and measure
      the performance times.

   -  The code for these tests resides in `meta/lib/oeqa/buildperf`.

   -  To run the tests, use the following command form::

         $ oe-build-perf-test <options>

      The command takes a number of options,
      such as where to place the test results. The Autobuilder Helper
      Scripts include the `build-perf-test-wrapper` script with
      examples of how to use the oe-build-perf-test from the command
      line.

      Use the `oe-git-archive` command to store test results into a
      Git repository.

      Use the `oe-build-perf-report` command to generate text reports
      and HTML reports with graphs of the performance data. See
      html 
      and
      txt 
      examples.

   -  The tests are contained in `meta/lib/oeqa/buildperf/test_basic.py`.

## Test Examples

This section provides example tests for each of the tests listed in the
test-manual/intro:How Tests Map to Areas of Code section.

-  `oe-selftest` testcases reside in the `meta/lib/oeqa/selftest/cases` directory.

-  `bitbake-selftest` testcases reside in the `bitbake/lib/bb/tests/` directory.

### ``bitbake-selftest``

A simple test example from `bitbake/lib/bb/tests/data.py` is::

   class DataExpansions(unittest.TestCase):
      def setUp(self):
            self.d = bb.data.init()
            self.d["foo"] = "value_of_foo"
            self.d["bar"] = "value_of_bar"
            self.d["value_of_foo"] = "value_of_'value_of_foo'"

      def test_one_var(self):
            val = self.d.expand("${foo}")
            self.assertEqual(str(val), "value_of_foo")

In this example, a `DataExpansions` class of tests is created, derived from
standard `Python unittest <https://docs.python.org/3/library/unittest.html>`__.
The class has a common `setUp` function which is shared by all the tests in
the class. A simple test is then added to test that when a variable is
expanded, the correct value is found.

BitBake selftests are straightforward
`Python unittest <https://docs.python.org/3/library/unittest.html>`__.
Refer to the `Python unittest documentation
<https://docs.python.org/3/library/unittest.html>`__ for additional information
on writing such tests.

### ``oe-selftest``

These tests are more complex due to the setup required behind the scenes
for full builds. Rather than directly using `Python unittest
<https://docs.python.org/3/library/unittest.html>`__, the code
wraps most of the standard objects. The tests can be simple, such as
testing a command from within the OE build environment using the
following example::

   class BitbakeLayers(OESelftestTestCase):
      def test_bitbakelayers_showcrossdepends(self):
            result = runCmd('bitbake-layers show-cross-depends')
            self.assertTrue('aspell' in result.output, msg = "No dependencies were shown. bitbake-layers show-cross-depends output: %s"% result.output)

This example, taken from `meta/lib/oeqa/selftest/cases/bblayers.py`,
creates a testcase from the `OESelftestTestCase` class, derived
from `unittest.TestCase`, which runs the `bitbake-layers` command
and checks the output to ensure it contains something we know should be
here.

The `oeqa.utils.commands` module contains Helpers which can assist
with common tasks, including:

-  *Obtaining the value of a bitbake variable:* Use
   `oeqa.utils.commands.get_bb_var()` or use
   `oeqa.utils.commands.get_bb_vars()` for more than one variable

-  *Running a bitbake invocation for a build:* Use
   `oeqa.utils.commands.bitbake()`

-  *Running a command:* Use `oeqa.utils.commands.runCmd()`

There is also a `oeqa.utils.commands.runqemu()` function for launching
the `runqemu` command for testing things within a running, virtualized
image.

You can run these tests in parallel. Parallelism works per test class,
so tests within a given test class should always run in the same build,
while tests in different classes or modules may be split into different
builds. There is no data store available for these tests since the tests
launch the `bitbake` command and exist outside of its context. As a
result, common BitBake library functions (`bb.\*`) are also unavailable.

### ``testimage``

These tests are run once an image is up and running, either on target
hardware or under QEMU. As a result, they are assumed to be running in a
target image environment, as opposed to in a host build environment. A
simple example from `meta/lib/oeqa/runtime/cases/python.py` contains
the following::

   class PythonTest(OERuntimeTestCase):
      @OETestDepends(['ssh.SSHTest.test_ssh'])
      @OEHasPackage(['python3-core'])
      def test_python3(self):
         cmd = "python3 -c \\"import codecs; print(codecs.encode('Uryyb, jbeyq', 'rot13'))\""
         status, output = self.target.run(cmd)
         msg = 'Exit status was not 0. Output: %s' % output
         self.assertEqual(status, 0, msg=msg)

In this example, the `OERuntimeTestCase` class wraps
`unittest.TestCase`. Within the test, `self.target` represents the
target system, where commands can be run using the `run()`
method.

To ensure certain tests or package dependencies are met, you can use the
`OETestDepends` and `OEHasPackage` decorators. For example, the test
in this example would only make sense if `python3-core` is installed in
the image.

### ``testsdk_ext``

These tests are run against built extensible SDKs (eSDKs). The tests can
assume that the eSDK environment has already been set up. An example from
`meta/lib/oeqa/sdk/cases/devtool.py` contains the following::

   class DevtoolTest(OESDKExtTestCase):
      @classmethod def setUpClass(cls):
         myapp_src = os.path.join(cls.tc.esdk_files_dir, "myapp")
         cls.myapp_dst = os.path.join(cls.tc.sdk_dir, "myapp")
         shutil.copytree(myapp_src, cls.myapp_dst)
         subprocess.check_output(['git', 'init', '.'], cwd=cls.myapp_dst)
         subprocess.check_output(['git', 'add', '.'], cwd=cls.myapp_dst)
         subprocess.check_output(['git', 'commit', '-m', "'test commit'"], cwd=cls.myapp_dst)

      @classmethod
      def tearDownClass(cls):
         shutil.rmtree(cls.myapp_dst)
      def _test_devtool_build(self, directory):
         self._run('devtool add myapp %s' % directory)
         try:
         self._run('devtool build myapp')
         finally:
         self._run('devtool reset myapp')
      def test_devtool_build_make(self):
         self._test_devtool_build(self.myapp_dst)

In this example, the `devtool`
command is tested to see whether a sample application can be built with
the `devtool build` command within the eSDK.

### ``testsdk``

These tests are run against built SDKs. The tests can assume that an SDK
has already been extracted and its environment file has been sourced. A
simple example from `meta/lib/oeqa/sdk/cases/python.py` contains the
following::

   class Python3Test(OESDKTestCase):
      def setUp(self):
            if not (self.tc.hasHostPackage("nativesdk-python3-core") or
                  self.tc.hasHostPackage("python3-core-native")):
               raise unittest.SkipTest("No python3 package in the SDK")

      def test_python3(self):
            cmd = "python3 -c \\"import codecs; print(codecs.encode('Uryyb, jbeyq', 'rot13'))\""
            output = self._run(cmd)
            self.assertEqual(output, "Hello, world\n")

In this example, if `nativesdk-python3-core` has been installed into the SDK,
the code runs the `python3` interpreter with a basic command to check it is
working correctly. The test would only run if Python3 is installed in the SDK.

### ``oe-build-perf-test``

The performance tests usually measure how long operations take and the
resource utilization as that happens. An example from
`meta/lib/oeqa/buildperf/test_basic.py` contains the following::

   class Test3(BuildPerfTestCase):
      def test3(self):
            """Bitbake parsing (bitbake -p)"""
            # Drop all caches and parse
            self.rm_cache()
            oe.path.remove(os.path.join(self.bb_vars['TMPDIR'], 'cache'), True)
            self.measure_cmd_resources(['bitbake', '-p'], 'parse_1',
                     'bitbake -p (no caches)')
            # Drop tmp/cache
            oe.path.remove(os.path.join(self.bb_vars['TMPDIR'], 'cache'), True)
            self.measure_cmd_resources(['bitbake', '-p'], 'parse_2',
                     'bitbake -p (no tmp/cache)')
            # Parse with fully cached data
            self.measure_cmd_resources(['bitbake', '-p'], 'parse_3',
                     'bitbake -p (cached)')

This example shows how three specific parsing timings are
measured, with and without various caches, to show how BitBake's parsing
performance trends over time.

## Considerations When Writing Tests

When writing good tests, there are several things to keep in mind. Since
things running on the Autobuilder are accessed concurrently by multiple
workers, consider the following:

**Running "cleanall" is not permitted.**

This can delete files from **DL_DIR** which would potentially break other
builds running in parallel. If this is required, **DL_DIR** must be set to
an isolated directory.

**Running "cleansstate" is not permitted.**

This can delete files from **SSTATE_DIR** which would potentially break
other builds running in parallel. If this is required, **SSTATE_DIR** must
be set to an isolated directory. Alternatively, you can use the `-f`
option with the `bitbake` command to "taint" tasks by changing the
sstate checksums to ensure sstate cache items will not be reused.

**Tests should not change the metadata.**

This is particularly true for oe-selftests since these can run in
parallel and changing metadata leads to changing checksums, which
confuses BitBake while running in parallel. If this is necessary, copy
layers to a temporary location and modify them. Some tests need to
change metadata, such as the devtool tests. To protect the metadata from
changes, set up temporary copies of that data first.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Project Testing and Release Process

## Day to Day Development

This section details how the project tests changes, through automation
on the Autobuilder or with the assistance of QA teams, through to making
releases.

The project aims to test changes against our test matrix before those
changes are merged into the master branch. As such, changes are queued
up in batches either in the `master-next` branch in the main trees, or
in user trees such as `ross/mut` in `poky-contrib` (Ross Burton
helps review and test patches and this is his testing tree).

We have two broad categories of test builds, including "full" and
"quick". On the Autobuilder, these can be seen as "a-quick" and
"a-full", simply for ease of sorting in the UI. Use our Autobuilder
console view  to see where we manage most
test-related items.

Builds are triggered manually when the test branches are ready. The
builds are monitored by the SWAT team. For additional information, see
`/Yocto_Build_Failure_Swat_Team`.
If successful, the changes would usually be merged to the `master`
branch. If not successful, someone would respond to the changes on the
mailing list explaining that there was a failure in testing. The choice
of quick or full would depend on the type of changes and the speed with
which the result was required.

The Autobuilder does build the `master` branch once daily for several
reasons, in particular, to ensure the current `master` branch does
build, but also to keep (yocto-testresults ),
(buildhistory ), and
our sstate up to date. On the weekend, there is a `master-next` build
instead to ensure the test results are updated for the less frequently
run targets.

Performance builds (`buildperf-\*` targets in the console) are triggered
separately every six hours and automatically push their results to the
buildstats  repository.

The "quick" targets have been selected to be the ones which catch the
most failures or give the most valuable data. We run "fast" ptests in
this case for example but not the ones which take a long time. The quick
target doesn't include `\*-lsb` builds for all architectures, some `world`
builds and doesn't trigger performance tests or `ltp` testing. The full
build includes all these things and is slower but more comprehensive.

## Release Builds

The project typically has two major releases a year with a six month
cadence in April and October. Between these there would be a number of
milestone releases (usually four) with the final one being stabilization
only along with point releases of our stable branches.

The build and release process for these project releases is similar to
that in test-manual/test-process:day to day development, in that the
a-full target of the Autobuilder is used but in addition the form is
configured to generate and publish artifacts and the milestone number,
version, release candidate number and other information is entered. The
box to "generate an email to QA" is also checked.

When the build completes, an email is sent out using the `send-qa-email`
script in the yocto-autobuilder-helper 
repository to the list of people configured for that release. Release builds
are placed into a directory in https://valkyrie.yocto.io/pub/releases on the
Autobuilder which is included in the email. The process from here is
more manual and control is effectively passed to release engineering.
The next steps include:

-  QA teams respond to the email saying which tests they plan to run and
   when the results will be available.

-  QA teams run their tests and share their results in the
   yocto-testresults-contrib 
   repository, along with a summary of their findings.

-  Release engineering prepare the release as per their process.

-  Test results from the QA teams are included into the release in
   separate directories and also uploaded to the
   yocto-testresults 
   repository alongside the other test results for the given revision.

-  The QA report in the final release is regenerated using resulttool to
   include the new test results and the test summaries from the teams
   (as headers to the generated report).

-  The release is checked against the release checklist and release
   readiness criteria.

-  A final decision on whether to release is made by the YP TSC who have
   final oversight on release readiness.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Understanding the Yocto Project Autobuilder

## Execution Flow within the Autobuilder

The "a-full" and "a-quick" targets are the usual entry points into the
Autobuilder and it makes sense to follow the process through the system
starting there. This is best visualized from the :yocto_ab:`Autobuilder
Console view </valkyrie/#/console>`.

Each item along the top of that view represents some "target build" and
these targets are all run in parallel. The 'full' build will trigger the
majority of them, the "quick" build will trigger some subset of them.
The Autobuilder effectively runs whichever configuration is defined for
each of those targets on a separate buildbot worker. To understand the
configuration, you need to look at the entry on `config.json` file
within the yocto-autobuilder-helper 
repository. The targets are defined in the `overrides` section, a quick
example could be `qemux86-64` which looks like::

   "qemux86-64" : {
         "MACHINE" : "qemux86-64",
         "TEMPLATE" : "arch-qemu",
         "step1" : {
               "extravars" : [
                     "IMAGE_FSTYPES:append = ' wic wic.bmap'"
                    ]
        }
   },

And to expand that, you need the `arch-qemu` entry from
the `templates` section, which looks like::

   "arch-qemu" : {
         "BUILDINFO" : true,
         "BUILDHISTORY" : true,
         "step1" : {
               "BBTARGETS" : "core-image-sato core-image-sato-dev core-image-sato-sdk core-image-minimal core-image-minimal-dev core-image-sato:do_populate_sdk",
         "SANITYTARGETS" : "core-image-minimal:do_testimage core-image-sato:do_testimage core-image-sato-sdk:do_testimage core-image-sato:do_testsdk"
         },
         "step2" : {
               "SDKMACHINE" : "x86_64",
               "BBTARGETS" : "core-image-sato:do_populate_sdk core-image-minimal:do_populate_sdk_ext core-image-sato:do_populate_sdk_ext",
               "SANITYTARGETS" : "core-image-sato:do_testsdk core-image-minimal:do_testsdkext core-image-sato:do_testsdkext"
         },
         "step3" : {
               "BUILDHISTORY" : false,
               "EXTRACMDS" : ["${SCRIPTSDIR}/checkvnc; DISPLAY=:1 oe-selftest ${HELPERSTMACHTARGS} -j 15"],
               "ADDLAYER" : ["${BUILDDIR}/../meta-selftest"]
         }
   },

Combining these two entries you can see that `qemux86-64` is a three step
build where `bitbake BBTARGETS` would be run, then `bitbake SANITYTARGETS`
for each step; all for `MACHINE="qemux86-64"` but with differing
**SDKMACHINE** settings. In step 1, an extra variable is added to the
`auto.conf` file to enable wic image generation.

While not every detail of this is covered here, you can see how the
template mechanism allows quite complex configurations to be built up
yet allows duplication and repetition to be kept to a minimum.

The different build targets are designed to allow for parallelization,
so different machines are usually built in parallel, operations using
the same machine and metadata are built sequentially, with the aim of
trying to optimize build efficiency as much as possible.

The `config.json` file is processed by the scripts in the Helper
repository in the `scripts` directory. The following section details
how this works.

## Autobuilder Target Execution Overview

For each given target in a build, the Autobuilder executes several
steps. These are configured in `yocto-autobuilder2/builders.py` and
roughly consist of:

#. *Run clobberdir*.

   This cleans out any previous build. Old builds are left around to
   allow easier debugging of failed builds. For additional information,
   see test-manual/understand-autobuilder:clobberdir.

#. *Obtain yocto-autobuilder-helper*

   This step clones the yocto-autobuilder-helper 
   git repository. This is necessary to avoid the requirement to maintain all
   the release or project-specific code within Buildbot. The branch chosen
   matches the release being built so we can support older releases and
   still make changes in newer ones.

#. *Write layerinfo.json*

   This transfers data in the Buildbot UI when the build was configured
   to the Helper.

#. *Call scripts/shared-repo-unpack*

   This is a call into the Helper scripts to set up a checkout of all
   the pieces this build might need. It might clone the **BitBake**
   repository, the **OpenEmbedded-Core (OE-Core)** repository as well as
   additional layers. It will use the data
   from the `layerinfo.json` file to help understand the
   configuration. It will also use a local cache of repositories to
   speed up the clone checkouts. For additional information, see
   test-manual/understand-autobuilder:Autobuilder Clone Cache.

   This step has two possible modes of operation. If the build is part
   of a parent build, it's possible that all the repositories needed may
   already be available, ready in a pre-prepared directory. An "a-quick"
   or "a-full" build would prepare this before starting the other
   sub-target builds. This is done for two reasons:

   -  the upstream may change during a build, for example, from a forced
      push and this ensures we have matching content for the whole build

   -  if 15 Workers all tried to pull the same data from the same repos,
      we can hit resource limits on upstream servers as they can think
      they are under some kind of network attack

   This pre-prepared directory is shared among the Workers over NFS. If
   the build is an individual build and there is no "shared" directory
   available, it would clone from the cache and the upstreams as
   necessary. This is considered the fallback mode.

#. *Call scripts/run-config*

   This is another call into the Helper scripts where it's expected that
   the main functionality of this target will be executed.

## Autobuilder Technology

The Autobuilder has Yocto Project-specific functionality to allow builds
to operate with increased efficiency and speed.

### clobberdir

When deleting files, the Autobuilder uses `clobberdir`, which is a
special script that moves files to a special location, rather than
deleting them. Files in this location are deleted by an `rm` command,
which is run under `ionice -c 3`. For example, the deletion only
happens when there is idle IO capacity on the Worker. The Autobuilder
Worker Janitor runs this deletion. See test-manual/understand-autobuilder:Autobuilder Worker Janitor.

### Autobuilder Clone Cache

Cloning repositories from scratch each time they are required was slow
on the Autobuilder. We therefore have a stash of commonly used
repositories pre-cloned on the Workers. Data is fetched from these
during clones first, then "topped up" with later revisions from any
upstream when necessary. The cache is maintained by the Autobuilder
Worker Janitor. See test-manual/understand-autobuilder:Autobuilder Worker Janitor.

### Autobuilder Worker Janitor

This is a process running on each Worker that performs two basic
operations, including background file deletion at IO idle (see
"Run clobberdir" in test-manual/understand-autobuilder:Autobuilder Target Execution Overview)
and maintenance of a cache of cloned repositories to improve the speed
the system can checkout repositories.

### Shared DL_DIR

The Workers are all connected over NFS which allows **DL_DIR** to be shared
between them. This reduces network accesses from the system and allows
the build to be sped up. The usage of the directory within the build system
is designed to be able to be shared over NFS.

### Shared SSTATE_DIR

The Workers are all connected over NFS which allows the `sstate`
directory to be shared between them. This means once a Worker has built
an artifact, all the others can benefit from it. The usage of the directory
within the build system is designed for sharing over NFS.

### Shared Hash Equivalence Server

The Workers all use the same Hash Equivalence server, through a common
definition for **BB_HASHSERVE**.

### Resulttool

All of the different tests run as part of the build generate output into
`testresults.json` files. This allows us to determine which tests ran
in a given build and their status. Additional information, such as
failure logs or the time taken to run the tests, may also be included.

Resulttool is part of OpenEmbedded-Core and is used to manipulate these
JSON results files. It has the ability to merge files together, display
reports of the test results and compare different result files.

For details, see `/Resulttool`.

## run-config Target Execution

The `scripts/run-config` execution is where most of the work within
the Autobuilder happens. It runs through a number of steps; the first
are general setup steps that are run once and include:

#. Set up any **buildtools** tarball if configured.

#. Call `buildhistory-init` if ref-classes-buildhistory is configured.

For each step that is configured in `config.json`, it will perform the
following:

#. Add any layers that are specified using the
   `bitbake-layers add-layer` command (logging as stepXa)

#. Call the `scripts/setup-config` script to generate the necessary
   `auto.conf` configuration file for the build

#. Run the `bitbake BBTARGETS` command (logging as stepXb)

#. Run the `bitbake SANITYTARGETS` command (logging as stepXc)

#. Run the `EXTRACMDS` command, which are run within the BitBake build
   environment (logging as stepXd)

#. Run the `EXTRAPLAINCMDS` command(s), which are run outside the
   BitBake build environment (logging as stepXd)

#. Remove any layers added in step
   1 using the `bitbake-layers remove-layer` command (logging as stepXa)

Once the execution steps above complete, `run-config` executes a set
of post-build steps, including:

#. Call `scripts/publish-artifacts` to collect any output which is to
   be saved from the build.

#. Call `scripts/collect-results` to collect any test results to be
   saved from the build.

#. Call `scripts/upload-error-reports` to send any error reports
   generated to the remote server.

#. Cleanup the **Build Directory** using
   test-manual/understand-autobuilder:clobberdir if the build was successful,
   else rename it to "build-renamed" for potential future debugging.

## Deploying Yocto Autobuilder

The most up to date information about how to setup and deploy your own
Autobuilder can be found in README.md 
in the yocto-autobuilder2  repository.

We hope that people can use the yocto-autobuilder2 
code directly but it is inevitable that users will end up needing to heavily
customize the yocto-autobuilder-helper 
repository, particularly the `config.json` file as they will want to define
their own test matrix.

The Autobuilder supports two customization options:

-  variable substitution

-  overlaying configuration files

The standard `config.json` minimally attempts to allow substitution of
the paths. The Helper script repository includes a
`local-example.json` file to show how you could override these from a
separate configuration file. Pass the following into the environment of
the Autobuilder::

   $ ABHELPER_JSON="config.json local-example.json"

As another example, you could also pass the following into the
environment::

   $ ABHELPER_JSON="config.json /some/location/local.json"

One issue users often run into is validation of the `config.json` files. A
tip for minimizing issues from invalid JSON files is to use a Git
`pre-commit-hook.sh` script to verify the JSON file before committing
it. Create a symbolic link as follows::

   $ ln -s ../../scripts/pre-commit-hook.sh .git/hooks/pre-commit

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Testing Packages With ptest

A Package Test (ptest) runs tests against packages built by the
OpenEmbedded build system on the target machine. A ptest contains at
least two items: the actual test, and a shell script (`run-ptest`)
that starts the test. The shell script that starts the test must not
contain the actual test --- the script only starts the test. On the other
hand, the test can be anything from a simple shell script that runs a
binary and checks the output to an elaborate system of test binaries and
data files.

The test generates output in the format used by Automake::

   result: testname

where the result can be `PASS`, `FAIL`, or `SKIP`, and
the testname can be any identifying string.

For a list of Yocto Project recipes that are already enabled with ptest,
see the Ptest  wiki page.

> **Note:**
> A recipe is "ptest-enabled" if it inherits the ref-classes-ptest
> class.
>

## Adding ptest to Your Build

To add package testing to your build, add the **DISTRO_FEATURES** and
**EXTRA_IMAGE_FEATURES** variables to your `local.conf` file, which
is found in the **Build Directory**::

   DISTRO_FEATURES:append = " ptest"
   EXTRA_IMAGE_FEATURES += "ptest-pkgs"

Once your build is complete, the ptest files are installed into the
`/usr/lib/package/ptest` directory within the image, where `package`
is the name of the package.

## Running ptest

The `ptest-runner` package installs a shell script that loops through
all installed ptest test suites and runs them in sequence.

During the execution `ptest-runner` keeps count of total and failed
`ptests`. At end the execution summary is written to the console.
If any of the `run-ptest` fails, `ptest-runner` returns '1'.

Consequently, you might want to add `ptest-runner` to your image.

## Getting Your Package Ready

In order to enable a recipe to run installed `ptests` on target hardware,
you need to prepare the recipes that build the packages you want to
test. Here is what you have to do for each recipe:

-  *Be sure the recipe inherits the* ref-classes-ptest *class:*
   Include the following line in each recipe::

      inherit ptest

   .. note::

      Classes for common frameworks already exist in :term:`OpenEmbedded-Core
      (OE-Core)`, such as:

      -  go-ptest 
      -  ref-classes-ptest-cargo
      -  ref-classes-ptest-gnome
      -  ptest-perl 
      -  ptest-python-pytest 

      Inheriting these classes with the `inherit` keyword in your recipe will
      make the next steps automatic.

-  *Create run-ptest:* This script starts your test. Locate the
   script where you will refer to it using
   **SRC_URI**. Be sure `run-ptest` exits with 0 to mark it
   as successfully executed otherwise will be marked as fail.
   Here is an example that starts a test for `dbus`::

      #!/bin/sh
      cd test
      make -k runtest-TESTS

-  *Return an appropriate exit code*: The `run-ptest` script must return 0 on
   success, 1 on failure. This is needed by `ptest-runner` to keep track of
   the successful and failed tests.

-  *Make sure the test prints at least one test result*: The execution of the
   `run-ptest` script must result in at least one test result output on the
   console, with the following format::

      result: testname

   Where `result` can be one of `PASS`, `SKIP`, or `FAIL`. `testname`
   can be any name.

   There can be as many test results as desired.

   This information is read by the ref-classes-testimage class and
   logparser 
   module.

-  *Ensure dependencies are met:* If the test adds build or runtime
   dependencies that normally do not exist for the package (such as
   requiring "make" to run the test suite), use the
   **DEPENDS** and
   **RDEPENDS** variables in
   your recipe in order for the package to meet the dependencies. Here
   is an example where the package has a runtime dependency on "make"::

      RDEPENDS:${PN}-ptest += "make"

-  *Add a function to build the test suite:* Not many packages support
   cross-compilation of their test suites. Consequently, you usually
   need to add a cross-compilation function to the package.

   Many packages based on Automake compile and run the test suite by
   using a single command such as `make check`. However, the host
   `make check` builds and runs on the same computer, while
   cross-compiling requires that the package is built on the host but
   executed for the target architecture (though often, as in the case
   for ptest, the execution occurs on the host). The built version of
   Automake that ships with the Yocto Project includes a patch that
   separates building and execution. Consequently, packages that use the
   unaltered, patched version of `make check` automatically
   cross-compiles.

   Regardless, you still must add a `do_compile_ptest` function to
   build the test suite. Add a function similar to the following to your
   recipe::

      do_compile_ptest() {
          oe_runmake buildtest-TESTS
      }

-  *Ensure special configurations are set:* If the package requires
   special configurations prior to compiling the test code, you must
   insert a `do_configure_ptest` function into the recipe.

-  *Install the test suite:* The ref-classes-ptest class
   automatically copies the file `run-ptest` to the target and then runs make
   `install-ptest` to run the tests. If this is not enough, you need
   to create a `do_install_ptest` function and make sure it gets
   called after the "make install-ptest" completes.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Performing Automated Runtime Testing

The OpenEmbedded build system makes available a series of automated
tests for images to verify runtime functionality. You can run these
tests on either QEMU or actual target hardware. Tests are written in
Python making use of the `unittest` module, and the majority of them
run commands on the target system over SSH. This section describes how
you set up the environment to use these tests, run available tests, and
write and add your own tests.

For information on the test and QA infrastructure available within the
Yocto Project, see the "ref-manual/release-process:testing and quality assurance"
section in the Yocto Project Reference Manual.

## Enabling Tests

Depending on whether you are planning to run tests using QEMU or on the
hardware, you have to take different steps to enable the tests. See the
following subsections for information on how to enable both types of
tests.

### Enabling Runtime Tests on QEMU

In order to run tests, you need to do the following:

-  *Set up to avoid interaction with sudo for networking:* To
   accomplish this, you must do one of the following:

   -  Add `NOPASSWD` for your user in `/etc/sudoers` either for all
      commands or just for `runqemu-ifup`. You must provide the full
      path as that can change if you are using multiple clones of the
      source repository.

      .. note::

         On some distributions, you also need to comment out "Defaults
         requiretty" in `/etc/sudoers`.

   -  Manually configure a tap interface for your system.

   -  Run as root the script in `scripts/runqemu-gen-tapdevs`, which
      should generate a list of tap devices. This is the option
      typically chosen for Autobuilder-type environments.

      .. note::

         -  Be sure to use an absolute path when calling this script
            with sudo.

         -  Ensure that your host has the package `iptables` installed.

         -  The package recipe `qemu-helper-native` is required to run
            this script. Build the package using the following command::

               $ bitbake qemu-helper-native

-  *Set the DISPLAY variable:* You need to set this variable so that
   you have an X server available (e.g. start `vncserver` for a
   headless machine).

-  *Be sure your host's firewall accepts incoming connections from
   192.168.7.0/24:* Some of the tests (in particular DNF tests) start an
   HTTP server on a random high number port, which is used to serve
   files to the target. The DNF module serves
   `${WORKDIR}/oe-rootfs-repo` so it can run DNF channel commands.
   That means your host's firewall must accept incoming connections from
   192.168.7.0/24, which is the default IP range used for tap devices by
   `runqemu`.

-  *Be sure your host has the correct packages installed:* Depending
   your host's distribution, you need to have the following packages
   installed:

   -  Ubuntu and Debian: `sysstat` and `iproute2`

   -  openSUSE: `sysstat` and `iproute2`

   -  Fedora: `sysstat` and `iproute`

   -  CentOS: `sysstat` and `iproute`

Once you start running the tests, the following happens:

#. A copy of the root filesystem is written to `${WORKDIR}/testimage`.

#. The image is booted under QEMU using the standard `runqemu` script.

#. A default timeout of 500 seconds occurs to allow for the boot process
   to reach the login prompt. You can change the timeout period by
   setting
   **TEST_QEMUBOOT_TIMEOUT**
   in the `local.conf` file.

#. Once the boot process is reached and the login prompt appears, the
   tests run. The full boot log is written to
   `${WORKDIR}/testimage/qemu_boot_log`.

#. Each test module loads in the order found in **TEST_SUITES**. You can
   find the full output of the commands run over SSH in
   `${WORKDIR}/testimgage/ssh_target_log`.

#. If no failures occur, the task running the tests ends successfully.
   You can find the output from the `unittest` in the task log at
   `${WORKDIR}/temp/log.do_testimage`.

### Enabling Runtime Tests on Hardware

The OpenEmbedded build system can run tests on real hardware, and for
certain devices it can also deploy the image to be tested onto the
device beforehand.

For automated deployment, a "controller image" is installed onto the
hardware once as part of setup. Then, each time tests are to be run, the
following occurs:

#. The controller image is booted into and used to write the image to be
   tested to a second partition.

#. The device is then rebooted using an external script that you need to
   provide.

#. The device boots into the image to be tested.

When running tests (independent of whether the image has been deployed
automatically or not), the device is expected to be connected to a
network on a pre-determined IP address. You can either use static IP
addresses written into the image, or set the image to use DHCP and have
your DHCP server on the test network assign a known IP address based on
the MAC address of the device.

In order to run tests on hardware, you need to set **TEST_TARGET** to an
appropriate value. For QEMU, you do not have to change anything, the
default value is "qemu". For running tests on hardware, the following
options are available:

-  *"simpleremote":* Choose "simpleremote" if you are going to run tests
   on a target system that is already running the image to be tested and
   is available on the network. You can use "simpleremote" in
   conjunction with either real hardware or an image running within a
   separately started QEMU or any other virtual machine manager.

-  *"SystemdbootTarget":* Choose "SystemdbootTarget" if your hardware is
   an EFI-based machine with `systemd-boot` as bootloader and
   `core-image-testmaster` (or something similar) is installed. Also,
   your hardware under test must be in a DHCP-enabled network that gives
   it the same IP address for each reboot.

   If you choose "SystemdbootTarget", there are additional requirements
   and considerations. See the
   "test-manual/runtime-testing:selecting systemdboottarget" section, which
   follows, for more information.

-  *"BeagleBoneTarget":* Choose "BeagleBoneTarget" if you are deploying
   images and running tests on the BeagleBone "Black" or original
   "White" hardware. For information on how to use these tests, see the
   comments at the top of the BeagleBoneTarget
   `meta-yocto-bsp/lib/oeqa/controllers/beaglebonetarget.py` file.

-  *"GrubTarget":* Choose "GrubTarget" if you are deploying images and running
   tests on any generic PC that boots using GRUB. For information on how
   to use these tests, see the comments at the top of the GrubTarget
   `meta-yocto-bsp/lib/oeqa/controllers/grubtarget.py` file.

-  *"your-target":* Create your own custom target if you want to run
   tests when you are deploying images and running tests on a custom
   machine within your BSP layer. To do this, you need to add a Python
   unit that defines the target class under `lib/oeqa/controllers/`
   within your layer. You must also provide an empty `__init__.py`.
   For examples, see files in `meta-yocto-bsp/lib/oeqa/controllers/`.

### Selecting SystemdbootTarget

If you did not set **TEST_TARGET** to "SystemdbootTarget", then you do
not need any information in this section. You can skip down to the
"test-manual/runtime-testing:running tests" section.

If you did set **TEST_TARGET** to "SystemdbootTarget", you also need to
perform a one-time setup of your controller image by doing the following:

#. *Set EFI_PROVIDER:* Be sure that **EFI_PROVIDER** is as follows::

      EFI_PROVIDER = "systemd-boot"

#. *Build the controller image:* Build the `core-image-testmaster` image.
   The `core-image-testmaster` recipe is provided as an example for a
   "controller" image and you can customize the image recipe as you would
   any other recipe.

   Image recipe requirements are:

   -  Inherits `core-image` so that kernel modules are installed.

   -  Installs normal linux utilities not BusyBox ones (e.g. `bash`,
      `coreutils`, `tar`, `gzip`, and `kmod`).

   -  Uses a custom **Initramfs** image with a custom
      installer. A normal image that you can install usually creates a
      single root filesystem partition. This image uses another installer that
      creates a specific partition layout. Not all Board Support
      Packages (BSPs) can use an installer. For such cases, you need to
      manually create the following partition layout on the target:

      -  First partition mounted under `/boot`, labeled "boot".

      -  The main root filesystem partition where this image gets installed,
         which is mounted under `/`.

      -  Another partition labeled "testrootfs" where test images get
         deployed.

#. *Install image:* Install the image that you just built on the target
   system.

The final thing you need to do when setting **TEST_TARGET** to
"SystemdbootTarget" is to set up the test image:

#. *Set up your local.conf file:* Make sure you have the following
   statements in your `local.conf` file::

      IMAGE_FSTYPES += "tar.gz"
      IMAGE_CLASSES += "testimage"
      TEST_TARGET = "SystemdbootTarget"
      TEST_TARGET_IP = "192.168.2.3"

#. *Build your test image:* Use BitBake to build the image::

      $ bitbake core-image-sato

### Power Control

For most hardware targets other than "simpleremote", you can control
power:

-  You can use **TEST_POWERCONTROL_CMD** together with
   **TEST_POWERCONTROL_EXTRA_ARGS** as a command that runs on the host
   and does power cycling. The test code passes one argument to that
   command: off, on or cycle (off then on). Here is an example that
   could appear in your `local.conf` file::

      TEST_POWERCONTROL_CMD = "powercontrol.exp test 10.11.12.1 nuc1"

   In this example, the expect
   script does the following:

   .. code-block:: shell

      ssh test@10.11.12.1 "pyctl nuc1 arg"

   It then runs a Python script that controls power for a label called
   `nuc1`.

   .. note::

      You need to customize **TEST_POWERCONTROL_CMD** and
      **TEST_POWERCONTROL_EXTRA_ARGS** for your own setup. The one requirement
      is that it accepts "on", "off", and "cycle" as the last argument.

-  When no command is defined, it connects to the device over SSH and
   uses the classic reboot command to reboot the device. Classic reboot
   is fine as long as the machine actually reboots (i.e. the SSH test
   has not failed). It is useful for scenarios where you have a simple
   setup, typically with a single board, and where some manual
   interaction is okay from time to time.

If you have no hardware to automatically perform power control but still
wish to experiment with automated hardware testing, you can use the
`dialog-power-control` script that shows a dialog prompting you to perform
the required power action. This script requires either KDialog or Zenity
to be installed. To use this script, set the
**TEST_POWERCONTROL_CMD**
variable as follows::

   TEST_POWERCONTROL_CMD = "${COREBASE}/scripts/contrib/dialog-power-control"

### Serial Console Connection

For test target classes requiring a serial console to interact with the
bootloader (e.g. BeagleBoneTarget and GrubTarget),
you need to specify a command to use to connect to the serial console of
the target machine by using the
**TEST_SERIALCONTROL_CMD**
variable and optionally the
**TEST_SERIALCONTROL_EXTRA_ARGS**
variable.

These cases could be a serial terminal program if the machine is
connected to a local serial port, or a `telnet` or `ssh` command
connecting to a remote console server. Regardless of the case, the
command simply needs to connect to the serial console and forward that
connection to standard input and output as any normal terminal program
does. For example, to use the picocom terminal program on serial device
`/dev/ttyUSB0` at 115200bps, you would set the variable as follows::

   TEST_SERIALCONTROL_CMD = "picocom /dev/ttyUSB0 -b 115200"

For local
devices where the serial port device disappears when the device reboots,
an additional "serdevtry" wrapper script is provided. To use this
wrapper, simply prefix the terminal command with
`${COREBASE}/scripts/contrib/serdevtry`::

   TEST_SERIALCONTROL_CMD = "${COREBASE}/scripts/contrib/serdevtry picocom -b 115200 /dev/ttyUSB0"

## Running Tests

You can start the tests automatically or manually:

-  *Automatically running tests:* To run the tests automatically after the
   OpenEmbedded build system successfully creates an image, first set the
   **TESTIMAGE_AUTO** variable to "1" in your `local.conf` file in the
   **Build Directory**::

      TESTIMAGE_AUTO = "1"

   Next, build your image. If the image successfully builds, the
   tests run::

      bitbake core-image-sato

-  *Manually running tests:* To manually run the tests, first globally
   inherit the ref-classes-testimage class by editing your
   `local.conf` file::

      IMAGE_CLASSES += "testimage"

   Next, use BitBake to run the tests::

      bitbake -c testimage image

All test files reside in `meta/lib/oeqa/runtime/cases` in
**OpenEmbedded-Core (OE-Core)**. A test name maps
directly to a Python module. Each test module may contain a number of
individual tests. Tests are usually grouped together by the area tested
(e.g tests for systemd reside in `meta/lib/oeqa/runtime/cases/systemd.py`).

You can add tests to any layer provided you place them in the proper
area and you extend **BBPATH** in
the `local.conf` file as normal. Be sure that tests reside in
`layer/lib/oeqa/runtime/cases`.

> **Note:**
> Be sure that module names do not collide with module names used in
> the default set of test modules in `meta/lib/oeqa/runtime/cases`.
>

You can change the set of tests run by appending or overriding
**TEST_SUITES** variable in
`local.conf`. Each name in **TEST_SUITES** represents a required test
for the image. Test modules named within **TEST_SUITES** cannot be
skipped even if a test is not suitable for an image (e.g. running the
RPM tests on an image without `rpm`). Appending "auto" to
**TEST_SUITES** causes the build system to try to run all tests that are
suitable for the image (i.e. each test module may elect to skip itself).

The order you list tests in **TEST_SUITES** is important and influences
test dependencies. Consequently, tests that depend on other tests should
be added after the test on which they depend. For example, since the
`ssh` test depends on the `ping` test, "ssh" needs to come after
"ping" in the list. The test class provides no re-ordering or dependency
handling.

> **Note:**
> Each module can have multiple classes with multiple test methods.
> And, Python `unittest` rules apply.
>

Here are some things to keep in mind when running tests:

-  The default tests for the image are defined as::

      DEFAULT_TEST_SUITES:pn-image = "ping ssh df connman syslog xorg scp vnc date rpm dnf dmesg"

-  Add your own test to the list of the by using the following::

      TEST_SUITES:append = " mytest"

-  Run a specific list of tests as follows::

     TEST_SUITES = "test1 test2 test3"

   Remember, order is important. Be sure to place a test that is
   dependent on another test later in the order.

## Exporting Tests

You can export tests so that they can run independently of the build
system. Exporting tests is required if you want to be able to hand the
test execution off to a scheduler. You can only export tests that are
defined in **TEST_SUITES**.

If your image is already built, make sure the following are set in your
`local.conf` file::

   IMAGE_CLASSES += "testexport"
   TEST_TARGET_IP = "IP-address-for-the-test-target"
   TEST_SERVER_IP = "IP-address-for-the-test-server"

You can then export the tests with the
following BitBake command form::

   $ bitbake image -c testexport

Exporting the tests places them in the **Build Directory** in
`tmp/testimage/`\ image, which is controlled by the **TEST_EXPORT_DIR**
variable.

You can now run the tests outside of the build environment::

   $ cd tmp/testimage/image
   $ ./oe-test runtime

> **Note:**
> You might need to run the image under QEMU or deploy it to your
> hardware before you can run the tests.
>

Here is a complete example that shows IP addresses and uses the
`core-image-sato` image::

   IMAGE_CLASSES += "testexport"
   TEST_TARGET_IP = "192.168.7.2"
   TEST_SERVER_IP = "192.168.7.1"

Use BitBake to export the tests::

   $ bitbake core-image-sato -c testexport

Run the tests outside of
the build environment using the following::

   $ cd tmp/testimage/core-image-sato
   $ ./oe-test runtime

## Writing New Tests

As mentioned previously, all new test files need to be in the proper
place for the build system to find them. New tests for additional
functionality outside of the core should be added to the layer that adds
the functionality, in `layer/lib/oeqa/runtime/cases` (as long as
**BBPATH** is extended in the
layer's `layer.conf` file as normal). Just remember the following:

-  Filenames need to map directly to test (module) names.

-  Do not use module names that collide with existing core tests.

-  Minimally, an empty `__init__.py` file must be present in the runtime
   directory.

To create a new test, start by copying an existing module (e.g.
`oe_syslog.py` or `gcc.py` are good ones to use). Test modules can use
code from `meta/lib/oeqa/utils`, which are helper classes.

> **Note:**
> Structure shell commands such that you rely on them and they return a
> single code for success. Be aware that sometimes you will need to
> parse the output. See the `df.py` and `date.py` modules for examples.
>

You will notice that all test classes inherit `oeRuntimeTest`, which
is found in `meta/lib/oetest.py`. This base class offers some helper
attributes, which are described in the following sections:

### Class Methods

Class methods are as follows:

-  *hasPackage(pkg):* Returns "True" if `pkg` is in the installed
   package list of the image, which is based on the manifest file that
   is generated during the ref-tasks-rootfs task.

-  *hasFeature(feature):* Returns "True" if the feature is in
   **IMAGE_FEATURES** or
   **DISTRO_FEATURES**.

### Class Attributes

Class attributes are as follows:

-  *pscmd:* Equals "ps -ef" if `procps` is installed in the image.
   Otherwise, `pscmd` equals "ps" (busybox).

-  *tc:* The called test context, which gives access to the
   following attributes:

   -  *d:* The BitBake datastore, which allows you to use stuff such
      as `oeRuntimeTest.tc.d.getVar("VIRTUAL-RUNTIME_init_manager")`.

   -  *testslist and testsrequired:* Used internally. The tests
      do not need these.

   -  *filesdir:* The absolute path to
      `meta/lib/oeqa/runtime/files`, which contains helper files for
      tests meant for copying on the target such as small files written
      in C for compilation.

   -  *target:* The target controller object used to deploy and
      start an image on a particular target (e.g. Qemu, SimpleRemote,
      and SystemdbootTarget). Tests usually use the following:

      -  *ip:* The target's IP address.

      -  *server_ip:* The host's IP address, which is usually used
         by the DNF test suite.

      -  *run(cmd, timeout=None):* The single, most used method.
         This command is a wrapper for: `ssh root@host "cmd"`. The
         command returns a tuple: (status, output), which are what their
         names imply - the return code of "cmd" and whatever output it
         produces. The optional timeout argument represents the number
         of seconds the test should wait for "cmd" to return. If the
         argument is "None", the test uses the default instance's
         timeout period, which is 300 seconds. If the argument is "0",
         the test runs until the command returns.

      -  *copy_to(localpath, remotepath):*
         `scp localpath root@ip:remotepath`.

      -  *copy_from(remotepath, localpath):*
         `scp root@host:remotepath localpath`.

### Instance Attributes

There is a single instance attribute, which is `target`. The `target`
instance attribute is identical to the class attribute of the same name,
which is described in the previous section. This attribute exists as
both an instance and class attribute so tests can use
`self.target.run(cmd)` in instance methods instead of
`oeRuntimeTest.tc.target.run(cmd)`.

## Installing Packages in the DUT Without the Package Manager

When a test requires a package built by BitBake, it is possible to
install that package. Installing the package does not require a package
manager be installed in the device under test (DUT). It does, however,
require an SSH connection and the target must be using the
`sshcontrol` class.

> **Note:**
> This method uses `scp` to copy files from the host to the target, which
> causes permissions and special attributes to be lost.
>

A JSON file is used to define the packages needed by a test. This file
must be in the same path as the file used to define the tests.
Furthermore, the filename must map directly to the test module name with
a `.json` extension.

The JSON file must include an object with the test name as keys of an
object or an array. This object (or array of objects) uses the following
data:

-  "pkg" --- a mandatory string that is the name of the package to be
   installed.

-  "rm" --- an optional boolean, which defaults to "false", that specifies
   to remove the package after the test.

-  "extract" --- an optional boolean, which defaults to "false", that
   specifies if the package must be extracted from the package format.
   When set to "true", the package is not automatically installed into
   the DUT.

Here is an example JSON file that handles test "foo" installing
package "bar" and test "foobar" installing packages "foo" and "bar".
Once the test is complete, the packages are removed from the DUT::

     {
         "foo": {
             "pkg": "bar"
         },
         "foobar": [
             {
                 "pkg": "foo",
                 "rm": true
             },
             {
                 "pkg": "bar",
                 "rm": true
             }
         ]
     }

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Reproducible Builds

# How we define it

The Yocto Project defines reproducibility as where a given input build
configuration will give the same binary output regardless of when it is built
(now or in 5 years time), regardless of the path on the filesystem the build is
run in, and regardless of the distro and tools on the underlying host system the
build is running on.

# Why it matters

The project aligns with the `Reproducible Builds project
<https://reproducible-builds.org/>`__, which shares information about why
reproducibility matters. The primary focus of the project is the ability to
detect security issues being introduced. However, from a Yocto Project
perspective, it is also hugely important that our builds are deterministic. When
you build a given input set of metadata, we expect you to get consistent output.
This has always been a key focus but, :ref:`since release 3.1 ("dunfell")
<migration-guides/migration-3.1:reproducible builds now enabled by default>`,
it is now true down to the binary level including timestamps.

For example, at some point in the future life of a product, you find that you
need to rebuild to add a security fix. If this happens, only the components that
have been modified should change at the binary level. This would lead to much
easier and clearer bounds on where validation is needed.

This also gives an additional benefit to the project builds themselves, our
overview-manual/concepts:Hash Equivalence for
overview-manual/concepts:Shared State object reuse works much more
effectively when the binary output remains the same.

> **Note:**
> We strongly advise you to make sure your project builds reproducibly
> before finalizing your production images. It would be too late if you
> only address this issue when the first updates are required.
>

# How we implement it

There are many different aspects to build reproducibility, but some particular
things we do within the build system to ensure reproducibility include:

-  Adding mappings to the compiler options to ensure debug filepaths are mapped
   to consistent target compatible paths. This is done through the
   **DEBUG_PREFIX_MAP** variable which sets the `-fmacro-prefix-map` and
   `-fdebug-prefix-map` compiler options correctly to map to target paths.
-  Being explicit about recipe dependencies and their configuration (no floating
   configure options or host dependencies creeping in). In particular this means
   making sure **PACKAGECONFIG** coverage covers configure options which may
   otherwise try and auto-detect host dependencies.
-  Using recipe specific sysroots to isolate recipes so they only see their
   dependencies. These are visible as `recipe-sysroot` and
   `recipe-sysroot-native` directories within the **WORKDIR** of a given
   recipe and are populated only with the dependencies a recipe has.
-  Build images from a reduced package set: only packages from recipes the image
   depends upon.
-  Filtering the tools available from the host's `PATH` to only a specific set
   of tools, set using the **HOSTTOOLS** variable.

# Can we prove the project is reproducible?

Yes, we can prove it and we regularly test this on the Autobuilder. At the
time of writing (release 3.3, "hardknott"), **OpenEmbedded-Core (OE-Core)**
is 100% reproducible for all its recipes (i.e. world builds) apart from the Go
language and Ruby documentation packages. Unfortunately, the current
implementation of the Go language has fundamental reproducibility problems as
it always depends upon the paths it is built in.

> **Note:**
> Only BitBake and **OpenEmbedded-Core (OE-Core)** guarantee complete
> reproducibility. The moment you add another layer, this warranty is voided,
> because of additional configuration files, `bbappend` files, overridden
> classes, etc.
>

To run our automated selftest, as we use in our CI on the Autobuilder, you can
run::

   oe-selftest -r reproducible.ReproducibleTests.test_reproducible_builds

This defaults to including a `world` build so, if other layers are added, it
would also run the tests for recipes in the additional layers. Different build
targets can be defined using the **OEQA_REPRODUCIBLE_TEST_TARGET** variable
in `local.conf`. For example, running reproducibility tests for only the
`python3-numpy` recipe can be done by setting::

   OEQA_REPRODUCIBLE_TEST_TARGET = "python3-numpy"

in local.conf before running the `oe-selftest` command shown above.

Reproducibility builds the target list twice. The first build will be run using
Shared State  if available, the
second build explicitly disables :ref:`Shared State
<overview-manual/concepts:Shared State>` except for recipes defined in the
**OEQA_REPRODUCIBLE_TEST_SSTATE_TARGETS** variable, and builds on the
specific host the build is running on. This means we can test reproducibility
builds between different host distributions over time on the Autobuilder.

If `OEQA_DEBUGGING_SAVED_OUTPUT` is set, any differing packages will be saved
here. The test is also able to run the `diffoscope` command on the output to
generate HTML files showing the differences between the packages, to aid
debugging. On the Autobuilder, these appear under
https://valkyrie.yocto.io/pub/repro-fail/ in the form ``oe-reproducible +
<date> + <random ID>`, e.g. `oe-reproducible-20200202-1lm8o1th``.

The project's current reproducibility status can be seen at
`/reproducible-build-results/`

You can also check the reproducibility status on the Autobuilder:
`/valkyrie/#/builders/reproducible`.

# How can I test my layer or recipes?

#### With world build

Once again, you can run a `world` test using the
oe-selftest 
command provided above. This functionality is implemented
in :oe_git:`meta/lib/oeqa/selftest/cases/reproducible.py
</openembedded-core/tree/meta/lib/oeqa/selftest/cases/reproducible.py>`.

#### Subclassing the test

You could subclass the test and change `targets` to a different target.

You may also change `sstate_targets` which would allow you to "pre-cache" some
set of recipes before the test, meaning they are excluded from reproducibility
testing. As a practical example, you could set `sstate_targets` to
`core-image-sato`, then setting `targets` to `core-image-sato-sdk` would
run reproducibility tests only on the targets belonging only to `core-image-sato-sdk`.

#### Using :term:`OEQA_REPRODUCIBLE_TEST_* <OEQA_REPRODUCIBLE_TEST_LEAF_TARGETS>` variables

If you want to test the reproducibility of a set of recipes, you can define
**OEQA_REPRODUCIBLE_TEST_LEAF_TARGETS**, in your local.conf::

   OEQA_REPRODUCIBLE_TEST_LEAF_TARGETS = "my-recipe"

This will test the reproducibility of `my-recipe` but will use the
Shared State  for most its
dependencies (i.e. the ones explicitly listed in DEPENDS, which may not be all
dependencies, c.f. [depends] varflags, PACKAGE_DEPENDS and other
implementations).

You can have finer control on the test with:

- **OEQA_REPRODUCIBLE_TEST_TARGET**: lists recipes to be built,
- **OEQA_REPRODUCIBLE_TEST_SSTATE_TARGETS**: lists recipes that will
  be built using Shared State .

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

# Yocto Project Compatible

# Introduction

After the introduction of layers to OpenEmbedded, it quickly became clear
that while some layers were popular and worked well, others developed a
reputation for being "problematic". Those were layers which didn't
interoperate well with others and tended to assume they controlled all
the aspects of the final output.  This usually isn't intentional but happens
because such layers are often created by developers with a particular focus
(e.g. a company's **BSP<Board Support Package (BSP)>**) whilst the end
users have a different one (e.g. integrating that
**BSP<Board Support Package (BSP)>** into a product).

As a result of noticing such patterns and friction between layers, the project
developed the "Yocto Project Compatible" badge program, allowing layers
following the best known practises to be marked as being widely compatible
with other ones. This takes the form of a set of "yes/no" binary answer
questions where layers can declare if they meet the appropriate criteria.
In the second version of the program, a script was added to make validation
easier and clearer, the script is called  `yocto-check-layer` and is
available in **OpenEmbedded-Core (OE-Core)**.

See dev-manual/layers:making sure your layer is compatible with yocto project
for details.

# Benefits

overview-manual/yp-intro:the yocto project layer model is powerful
and flexible: it gives users the ultimate power to change pretty much any
aspect of the system but as with most things, power comes with responsibility.
The Yocto Project would like to see people able to mix and match BSPs with
distro configs or software stacks and be able to merge successfully.
Over time, the project identified characteristics in layers that allow them
to operate well together. "anti-patterns" were also found, preventing layers
from working well together.

The intent of the compatibility program is simple: if the layer passes the
compatibility tests, it is considered "well behaved" and should operate
and cooperate well with other compatible layers.

The benefits of compatibility can be seen from multiple different user and
member perspectives. From a hardware perspective
(a overview-manual/concepts:bsp layer), compatibility means the
hardware can be used in many different products and use cases without
impacting the software stacks being run with it. For a company developing
a product, compatibility gives you a specification / standard you can
require in a contract and then know it will have certain desired
characteristics for interoperability. It also puts constraints on how invasive
the code bases are into the rest of the system, meaning that multiple
different separate hardware support layers can coexist (e.g. for multiple
product lines from different hardware manufacturers). This can also make it
easier for one or more parties to upgrade those system components for security
purposes during the lifecycle of a product.

# Validating a layer

The badges are available to members of the Yocto Project (as member benefit)
and to open source projects run on a non-commercial basis. However, anyone can
answer the questions and run the script.

The project encourages all layer maintainers to review the questions and the
output from the script against their layer, as the way some layers are
constructed often has unintended consequences. The questions and the script
are designed to highlight known issues which are often easy to solve. This
makes layers easier to use and therefore more popular.

It is intended that over time, the tests will evolve as new best known
practices are identified, and as new interoperability issues are found,
unnecessarily restricting layer interoperability. If anyone becomes aware of
either type, please let the project know through the
technical calls ,
the mailing lists 
or through the `Technical Steering Committee (TSC) </TSC>`.
The TSC is responsible for the technical criteria used by the program.

Layers are divided into three types:

-  "BSP" or "hardware support"
   layers contain support for particular pieces of hardware. This includes
   kernel and boot loader configuration, and any recipes for firmware or
   kernel modules needed for the hardware. Such layers usually correspond
   to a **MACHINE** setting.

-  "distro" layers defined
   as layers providing configuration options and settings such as the
   choice of init system, compiler and optimisation options, and
   configuration and choices of software components. This would usually
   correspond to a **DISTRO** setting.

-  "software" layers are usually recipes. A layer might target a
   particular graphical UI or software stack component.

Here are key best practices the program tries to encourage:

-  A layer should clearly show who maintains it, and who change
   submissions and bug reports should be sent to.

-  Where multiple types of functionality are present, the layer should
   be internally divided into sublayers to separate these components.
   That's because some users may only need one of them and separability
   is a key best practice.

-  Adding a layer to a build should not modify that build, unless the
   user changes a configuration setting to activate the layer, by selecting
   a **MACHINE**, a **DISTRO** or a **DISTRO_FEATURES** setting.

-  Layers should be documenting where they don’t support normal "core"
   functionality such as where debug symbols are disabled or missing, where
   development headers and on-target library usage may not work or where
   functionality like the SDK/eSDK would not be expected to work.

The project does test the compatibility status of the core project layers on
its Autobuilder .

The official form to submit compatibility requests with is at
`/ecosystem/branding/compatible-registration/`.
Applicants can display the badge they get when their application is successful.
