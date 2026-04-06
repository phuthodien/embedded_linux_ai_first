# Yocto Project Contributor Guide

## Contributing Changes to a Component

Contributions to the Yocto Project and OpenEmbedded are very welcome.
Because the system is extremely configurable and flexible, we recognize
that developers will want to extend, configure or optimize it for their
specific uses.

## Contributing through mailing lists --- Why not using web-based workflows?

Both Yocto Project and OpenEmbedded have many key components that are
maintained by patches being submitted on mailing lists. We appreciate this
approach does look a little old fashioned when other workflows are available
through web technology such as GitHub, GitLab and others. Since we are often
asked this question, we’ve decided to document the reasons for using mailing
lists.

One significant factor is that we value peer review. When a change is proposed
to many of the core pieces of the project, it helps to have many eyes of review
go over them. Whilst there is ultimately one maintainer who needs to make the
final call on accepting or rejecting a patch, the review is made by many eyes
and the exact people reviewing it are likely unknown to the maintainer. It is
often the surprise reviewer that catches the most interesting issues!

This is in contrast to the "GitHub" style workflow where either just a
maintainer makes that review, or review is specifically requested from
nominated people. We believe there is significant value added to the codebase
by this peer review and that moving away from mailing lists would be to the
detriment of our code.

We also need to acknowledge that many of our developers are used to this
mailing list workflow and have worked with it for years, with tools and
processes built around it. Changing away from this would result in a loss
of key people from the project, which would again be to its detriment.

The projects are acutely aware that potential new contributors find the
mailing list approach off-putting and would prefer a web-based GUI.
Since we don’t believe that can work for us, the project is aiming to ensure
`patchwork <https://patchwork.yoctoproject.org/>`__ is available to help track
patch status and also looking at how tooling can provide more feedback to users
about patch status. We are looking at improving tools such as `patchtest` to
test user contributions before they hit the mailing lists and also at better
documenting how to use such workflows since we recognise that whilst this was
common knowledge a decade ago, it might not be as familiar now.

## Preparing Changes for Submission

### Set up Git

The first thing to do is to install Git packages. Here is an example
on Debian and Ubuntu::

   sudo apt install git-core git-email

Then, you need to set a name and e-mail address that Git will
use to identify your commits::

   git config --global user.name "Ada Lovelace"
   git config --global user.email "ada.lovelace@gmail.com"

By default, Git adds a signature line at the end of patches containing the Git
version. We suggest to remove it as it doesn't add useful information.

Remove it with the following command::

   git config --global format.signature ""

### Clone the Git repository for the component to modify

After identifying the component to modify as described in the
"/contributor-guide/identify-component" section, clone the
corresponding Git repository. Here is an example for OpenEmbedded-Core::

  git clone https://git.openembedded.org/openembedded-core
  cd openembedded-core

### Create a new branch

Then, create a new branch in your local Git repository
for your changes, starting from the reference branch in the upstream
repository (often called `master`)::

   $ git checkout <ref-branch>
   $ git checkout -b my-changes

If you have completely unrelated sets of changes to submit, you should even
create one branch for each set.

### Implement and commit changes

In each branch, you should group your changes into small, controlled and
isolated ones. Keeping changes small and isolated aids review, makes
merging/rebasing easier and keeps the change history clean should anyone need
to refer to it in future.

To this purpose, you should create *one Git commit per change*,
corresponding to each of the patches you will eventually submit.
See `further guidance <https://www.kernel.org/doc/html/latest/process/submitting-patches.html#separate-your-changes>`__
in the Linux kernel documentation if needed.

For example, when you intend to add multiple new recipes, each recipe
should be added in a separate commit. For upgrades to existing recipes,
the previous version should usually be deleted as part of the same commit
to add the upgraded version.

#. *Stage Your Changes:* Stage your changes by using the `git add`
   command on each file you modified. If you want to stage all the
   files you modified, you can even use the `git add -A` command.

#. *Commit Your Changes:* This is when you can create separate commits. For
   each commit to create, use the `git commit -s` command with the files
   or directories you want to include in the commit::

      $ git commit -s file1 file2 dir1 dir2 ...

   To include all staged files::

      $ git commit -sa

   #.  The `-s` option of `git commit` adds a "Signed-off-by:" line
       to your commit message. There is the same requirement for contributing
       to the Linux kernel. Adding such a line signifies that you, the
       submitter, have agreed to the `Developer's Certificate of Origin 1.1
       <https://www.kernel.org/doc/html/latest/process/submitting-patches.html#sign-your-work-the-developer-s-certificate-of-origin>`__
       as follows:

       .. code-block:: none

          Developer's Certificate of Origin 1.1

          By making a contribution to this project, I certify that:

          (a) The contribution was created in whole or in part by me and I
              have the right to submit it under the open source license
              indicated in the file; or

          (b) The contribution is based upon previous work that, to the best
              of my knowledge, is covered under an appropriate open source
              license and I have the right under that license to submit that
              work with modifications, whether created in whole or in part
              by me, under the same open source license (unless I am
              permitted to submit under a different license), as indicated
              in the file; or

          (c) The contribution was provided directly to me by some other
              person who certified (a), (b) or (c) and I have not modified
              it.

          (d) I understand and agree that this project and the contribution
              are public and that a record of the contribution (including all
              personal information I submit with it, including my sign-off) is
              maintained indefinitely and may be redistributed consistent with
              this project or the open source license(s) involved.

   #.  Provide a single-line summary of the change and, if more
       explanation is needed, provide more detail in the description of the
       commit. This summary is typically viewable in the "shortlist" of
       changes. Thus, providing something short and descriptive that
       gives the reader a summary of the change is useful when viewing a
       list of many commits. You should prefix this short description
       with the recipe name (if changing a recipe), or else with the
       short form path to the file being changed.

       .. note::

          To find a suitable prefix for the commit summary, a good idea
          is to look for prefixes used in previous commits touching the
          same files or directories::

             git log --oneline <paths>

   #.  For the commit description, provide detailed information
       that describes what you changed, why you made the change, and the
       approach you used. It might also be helpful if you mention how you
       tested the change. Provide as much detail as you can in the commit
       description.

       .. note::

          If the single line summary is enough to describe a simple
          change, the commit description can be left empty.

   #.  If the change addresses a specific bug or issue that is associated
       with a bug-tracking ID, include a reference to that ID in the body of the
       commit message. For example, the Yocto Project uses a
       specific convention for bug references --- any commit that addresses
       a specific bug should use the following form for the body of the commit
       message. Be sure to use the actual bug-tracking ID from
       Bugzilla for bug-id::

          single-line summary of change

          Fixes [YOCTO #bug-id]

          detailed description of change

   #. If other people participated in this patch, add some tags to the commit
      description to credit other contributors to the change:

      -  `Reported-by`: name and email of a person reporting a bug
         that your commit is trying to fix. This is a good practice
         to encourage people to go on reporting bugs and let them
         know that their reports are taken into account.

      -  `Suggested-by`: name and email of a person to credit for the
         idea of making the change.

      -  `Tested-by`, `Reviewed-by`: name and email for people having
         tested your changes or reviewed their code. These fields are
         usually added by the maintainer accepting a patch, or by
         yourself if you submitted your patches to early reviewers,
         or are submitting an unmodified patch again as part of a
         new iteration of your patch series.

      -  `Cc`: name and email of people you want to send a copy
         of your changes to. This field will be used by `git send-email`.

      See `more guidance about using such tags
      <https://www.kernel.org/doc/html/latest/process/submitting-patches.html#using-reported-by-tested-by-reviewed-by-suggested-by-and-fixes>`__
      in the Linux kernel documentation.

      .. note::

         One can amend an existing git commit message to add missing tags for
         contributors with the `git commit --amend` command.

### Test your changes

For each contributions you make, you should test your changes as well.
For this the Yocto Project offers several types of tests. Those tests cover
different areas and it depends on your changes which are feasible. For example run:

   -  For changes that affect the build environment:

      -  `bitbake-selftest`: for changes within BitBake

      -  `oe-selftest`: to test combinations of BitBake runs

      -  `oe-build-perf-test`: to test the performance of common build scenarios

   -  For changes in a recipe:

      - `ptest`: run package specific tests, if they exist

      - `testimage`: build an image, boot it and run testcases on it

      - If applicable, ensure also the `native` and `nativesdk` variants builds

   -  For changes relating to the SDK:

      - `testsdk`: to build, install and run tests against a SDK

      - `testsdk_ext`: to build, install and run tests against an extended SDK

Note that this list just gives suggestions and is not exhaustive. More details can
be found here: test-manual/intro:Yocto Project Tests --- Types of Testing Overview.

## Creating Patches

Here is the general procedure on how to create patches to be sent through email:

#. *Describe the Changes in your Branch:* If you have more than one commit
   in your branch, it's recommended to provide a cover letter describing
   the series of patches you are about to send.

   For this purpose, a good solution is to store the cover letter contents
   in the branch itself::

      git branch --edit-description

   This will open a text editor to fill in the description for your
   changes. This description can be updated when necessary and will
   be used by Git to create the cover letter together with the patches.

   It is recommended to start this description with a title line which
   will serve a the subject line for the cover letter.

#. *Generate Patches for your Branch:* The `git format-patch` command will
   generate patch files for each of the commits in your branch. You need
   to pass the reference branch your branch starts from.

   If you branch didn't need a description in the previous step::

      $ git format-patch <ref-branch>

   If you filled a description for your branch, you will want to generate
   a cover letter too::

      $ git format-patch --cover-letter --cover-from-description=auto <ref-branch>

   After the command is run, the current directory contains numbered
   `.patch` files for the commits in your branch. If you have a cover
   letter, it will be in the `0000-cover-letter.patch`.

   .. note::

      The `--cover-from-description=auto` option makes `git format-patch`
      use the first paragraph of the branch description as the cover
      letter title. Another possibility, which is easier to remember, is to pass
      only the `--cover-letter` option, but you will have to edit the
      subject line manually every time you generate the patches.

      See the `git format-patch manual page <https://git-scm.com/docs/git-format-patch>`__
      for details.

#. *Review each of the Patch Files:* This final review of the patches
   before sending them often allows to view your changes from a different
   perspective and discover defects such as typos, spacing issues or lines
   or even files that you didn't intend to modify. This review should
   include the cover letter patch too.

   If necessary, rework your commits as described in
   "contributor-guide/submit-changes:taking patch review into account".

## Validating Patches with Patchtest

`patchtest` is available in `openembedded-core` as a tool for making
sure that your patches are well-formatted and contain important info for
maintenance purposes, such as the `Signed-off-by` presence. Note that no
functional testing of the changes will be performed by `patchtest`. Currently,
it only supports testing patches for `openembedded-core` branches. To setup,
perform the following::

    pip install -r meta/lib/patchtest/requirements.txt
    source oe-init-build-env
    bitbake-layers add-layer ../meta-selftest

Once these steps are complete and you have generated your patch files,
you can run `patchtest` like so::

    patchtest --patch <patch_name>

Alternatively, if you want `patchtest` to iterate over and test
multiple patches stored in a directory, you can use::

    patchtest --directory <directory_name>

By default, `patchtest` uses its own modules' file paths to determine what
repository and test suite to check patches against. If you wish to test
patches against a repository other than `openembedded-core` and/or use
a different set of tests, you can use the `--repodir` and `--testdir`
flags::

    patchtest --patch <patch_name> --repodir <path/to/repo> --testdir <path/to/testdir>

Finally, note that `patchtest` is designed to test patches in a standalone
way, so if your patches are meant to apply on top of changes made by
previous patches in a series, it is possible that `patchtest` will report
false failures regarding the "merge on head" test.

Using `patchtest` in this manner provides a final check for the overall
quality of your changes before they are submitted for review by the
maintainers.

## Sending the Patches via Email

### Using Git to Send Patches

To submit patches through email, it is very important that you send them
without any whitespace or HTML formatting that either you or your mailer
introduces. The maintainer that receives your patches needs to be able
to save and apply them directly from your emails, using the `git am`
command.

Using the `git send-email` command is the only error-proof way of sending
your patches using email since there is no risk of compromising whitespace
in the body of the message, which can occur when you use your own mail
client. It will also properly include your patches as *inline attachments*,
which is not easy to do with standard e-mail clients without breaking lines.
If you used your regular e-mail client and shared your patches as regular
attachments, reviewers wouldn't be able to quote specific sections of your
changes and make comments about them.

### Setting up Git to Send Email

The `git send-email` command can send email by using a local or remote
Mail Transport Agent (MTA) such as `msmtp`, `sendmail`, or
through a direct SMTP configuration in your Git `~/.gitconfig` file.

Here are the settings for letting `git send-email` send e-mail through your
regular STMP server, using a Google Mail account as an example::

   git config --global sendemail.smtpserver smtp.gmail.com
   git config --global sendemail.smtpserverport 587
   git config --global sendemail.smtpencryption tls
   git config --global sendemail.smtpuser ada.lovelace@gmail.com
   git config --global sendemail.smtppass = XXXXXXXX

These settings will appear in the `.gitconfig` file in your home directory.

If you neither can use a local MTA nor SMTP,  make sure you use an email client
that does not touch the message (turning spaces in tabs, wrapping lines, etc.).
A good mail client to do so is Pine (or Alpine) or Mutt. For more
information about suitable clients, see `Email clients info for Linux
<https://www.kernel.org/doc/html/latest/process/email-clients.html>`__
in the Linux kernel sources.

If you use such clients, just include the patch in the body of your email.

### Finding a Suitable Mailing List

You should send patches to the appropriate mailing list so that they can be
reviewed by the right contributors and merged by the appropriate maintainer.
The specific mailing list you need to use depends on the location of the code
you are changing.

If people have concerns with any of the patches, they will usually voice
their concern over the mailing list. If patches do not receive any negative
reviews, the maintainer of the affected layer typically takes them, tests them,
and then based on successful testing, merges them.

In general, each component (e.g. layer) should have a `README` file
that indicates where to send the changes and which process to follow.

If you intend to submit a new recipe that neither fits in
**OpenEmbedded-Core (OE-Core)**,
nor into meta-openembedded , you should
look for a suitable layer in https://layers.openembedded.org. If similar
recipes can be expected, you may consider dev-manual/layers:creating your own layer.

If in doubt, please ask on the yocto  general mailing list
or on the `openembedded-devel </g/openembedded-devel>` mailing list.

### Subscribing to the Mailing List

After identifying the right mailing list to use, you will have to subscribe to
it if you haven't done it yet.

If you attempt to send patches to a list you haven't subscribed to, your email
will be returned as undelivered.

However, if you don't want to be receive all the messages sent to a mailing list,
you can set your subscription to "no email". You will still be a subscriber able
to send messages, but you won't receive any e-mail. If people reply to your message,
their e-mail clients will default to including your email address in the
conversation anyway.

Anyway, you'll also be able to access the new messages on mailing list archives,
either through a web browser, or for the lists archived on https://lore.kernel.org,
through an individual newsgroup feed or a git repository.

### Sending Patches via Email

At this stage, you are ready to send your patches via email. Here's the
typical usage of `git send-email`::

   git send-email --to <mailing-list-address> *.patch

Then, review each subject line and list of recipients carefully, and then
allow the command to send each message.

You will see that `git send-email` will automatically copy the people listed
in any commit tags such as `Signed-off-by` or `Reported-by`.

In case you are sending patches for meta-openembedded 
or any layer other than openembedded-core ,
please add the appropriate prefix so that it is clear which layer the patch is intended
to be applied to::

   git format-patch --subject-prefix="meta-oe][PATCH" ...

> **Note:**
> It is actually possible to send patches without generating them
> first. However, make sure you have reviewed your changes carefully
> because `git send-email` will just show you the title lines of
> each patch.
>
> Here's a command you can use if you just have one patch in your
> branch::
>
> git send-email --to <mailing-list-address> -1
>
> If you have multiple patches and a cover letter, you can send
> patches for all the commits between the reference branch
> and the tip of your branch::
>
> git send-email --cover-letter --cover-from-description=auto --to <mailing-list-address> -M <ref-branch>
>

See the `git send-email manual page <https://git-scm.com/docs/git-send-email>`__
for details.

### Troubleshooting Email Issues

#### Fixing your From identity

We have a frequent issue with contributors whose patches are received through
a `From` field which doesn't match the `Signed-off-by` information. Here is
a typical example for people sending from a domain name with `DMARC`::

   From: "Linus Torvalds via lists.openembedded.org <linus.torvalds=kernel.org@lists.openembedded.org>"

This `From` field is used by `git am` to recreate commits with the right
author name. The following will ensure that your e-mails have an additional
`From` field at the beginning of the Email body, and therefore that
maintainers accepting your patches don't have to fix commit author information
manually::

   git config --global sendemail.from "linus.torvalds@kernel.org"

The `sendemail.from` should match your `user.email` setting,
which appears in the `Signed-off-by` line of your commits.

### Streamlining git send-email usage

If you want to save time and not be forced to remember the right options to use
with `git send-email`, you can use Git configuration settings.

-  To set the right mailing list address for a given repository::

      git config --local sendemail.to openembedded-devel@lists.openembedded.org

-  If the mailing list requires a subject prefix for the layer
   (this only works when the repository only contains one layer)::

      git config --local format.subjectprefix "meta-something][PATCH"

## Using Scripts to Push a Change Upstream and Request a Pull

For larger patch series it is preferable to send a pull request which not
only includes the patch but also a pointer to a branch that can be pulled
from. This involves making a local branch for your changes, pushing this
branch to an accessible repository and then using the `create-pull-request`
and `send-pull-request` scripts from openembedded-core to create and send a
patch series with a link to the branch for review.

Follow this procedure to push a change to an upstream "contrib" Git
repository once the steps in
"contributor-guide/submit-changes:preparing changes for submission"
have been followed:

> **Note:**
> You can find general Git information on how to push a change upstream
> in the
> `Git Community Book <https://git-scm.com/book/en/v2/Distributed-Git-Distributed-Workflows>`__.
>

#. *Request Push Access to an "Upstream" Contrib Repository:* Send an email to
   `helpdesk@yoctoproject.org`:

    -  Attach your SSH public key which usually named `id_rsa.pub.`.
       If you don't have one generate it by running `ssh-keygen -t rsa -b 4096 -C "your_email@example.com"`.

    -  List the repositories you're planning to contribute to.

    -  Include your preferred branch prefix for `-contrib` repositories.

#. *Push Your Commits to the "Contrib" Upstream:* Push your
   changes to that repository::

      $ git push upstream_remote_repo local_branch_name

   For example, suppose you have permissions to push
   into the upstream `meta-intel-contrib` repository and you are
   working in a local branch named `your_name`\ `/README`. The following
   command pushes your local commits to the `meta-intel-contrib`
   upstream repository and puts the commit in a branch named
   `your_name`\ `/README`::

      $ git push meta-intel-contrib your_name/README

#. *Determine Who to Notify:* Determine the maintainer or the mailing
   list that you need to notify for the change.

   Before submitting any change, you need to be sure who the maintainer
   is or what mailing list that you need to notify. Use either these
   methods to find out:

   -  *Maintenance File:* Examine the `maintainers.inc` file, which is
      located in **OpenEmbedded-Core (OE-Core)** at
      `meta/conf/distro/include`, to see who is responsible for code.

   -  *Search by File:* Using overview-manual/development-environment:git, you can
      enter the following command to bring up a short list of all
      commits against a specific file::

         git shortlog -- filename

      Just provide the name of the file for which you are interested. The
      information returned is not ordered by history but does include a
      list of everyone who has committed grouped by name. From the list,
      you can see who is responsible for the bulk of the changes against
      the file.

   -  *Find the Mailing List to Use:* See the
      "contributor-guide/submit-changes:finding a suitable mailing list"
      section above.

#. *Make a Pull Request:* Notify the maintainer or the mailing list that
   you have pushed a change by making a pull request.

   The Yocto Project provides two scripts that conveniently let you
   generate and send pull requests to the Yocto Project. These scripts
   are `create-pull-request` and `send-pull-request`. You can find
   these scripts in the `scripts` directory within
   **OpenEmbedded-Core (OE-Core)**.

   Using these scripts correctly formats the requests without
   introducing any whitespace or HTML formatting. The maintainer that
   receives your patches either directly or through the mailing list
   needs to be able to save and apply them directly from your emails.
   Using these scripts is the preferred method for sending patches.

   First, create the pull request. For example, the following command
   runs the script, specifies the upstream repository in the contrib
   directory into which you pushed the change, and provides a subject
   line in the created patch files::

      $ ./scripts/create-pull-request -u meta-intel-contrib -s "Updated Manual Section Reference in README"

   Running this script forms `*.patch` files in a folder named
   `pull-`\ `PID` in the current directory. One of the patch files is a
   cover letter.

   Before running the `send-pull-request` script, you must edit the
   cover letter patch to insert information about your change. After
   editing the cover letter, send the pull request. For example, the
   following command runs the script and specifies the patch directory
   and email address. In this example, the email address is a mailing
   list::

      $ ./scripts/send-pull-request -p ~/meta-intel/pull-10565 -t meta-intel@lists.yoctoproject.org

   You need to follow the prompts as the script is interactive.

   .. note::

      For help on using these scripts, simply provide the `-h`
      argument as follows::

              $ ./scripts/create-pull-request -h
              $ ./scripts/send-pull-request -h

## Submitting Changes to Stable Release Branches

The process for proposing changes to a Yocto Project stable branch differs
from the steps described above. Changes to a stable branch must address
identified bugs or CVEs and should be made carefully in order to avoid the
risk of introducing new bugs or breaking backwards compatibility. Typically
bug fixes must already be accepted into the master branch before they can be
backported to a stable branch unless the bug in question does not affect the
master branch or the fix on the master branch is unsuitable for backporting.

The list of stable branches along with the status and maintainer for each
branch can be obtained from the Releases 
page.

> **Note:**
> Changes will not typically be accepted for branches which are marked as
> End-Of-Life (EOL).
>

With this in mind, the steps to submit a change for a stable branch are as
follows:

#. *Identify the bug or CVE to be fixed:* This information should be
   collected so that it can be included in your submission.

   See security-manual/vulnerabilities:checking for vulnerabilities
   for details about CVE tracking.

#. *Check if the fix is already present in the master branch:* This will
   result in the most straightforward path into the stable branch for the
   fix.

   #. *If the fix is present in the master branch --- submit a backport request
      by email:* You should send an email to the relevant stable branch
      maintainer and the mailing list with details of the bug or CVE to be
      fixed, the commit hash on the master branch that fixes the issue and
      the stable branches which you would like this fix to be backported to.

   #. *If the fix is not present in the master branch --- submit the fix to the
      master branch first:* This will ensure that the fix passes through the
      project's usual patch review and test processes before being accepted.
      It will also ensure that bugs are not left unresolved in the master
      branch itself. Once the fix is accepted in the master branch a backport
      request can be submitted as above.

   #. *If the fix is unsuitable for the master branch --- submit a patch
      directly for the stable branch:* This method should be considered as a
      last resort. It is typically necessary when the master branch is using
      a newer version of the software which includes an upstream fix for the
      issue or when the issue has been fixed on the master branch in a way
      that introduces backwards incompatible changes. In this case follow the
      steps in "contributor-guide/submit-changes:preparing changes for submission"
      and in the following sections but modify the subject header of your patch
      email to include the name of the stable branch which you are
      targetting. This can be done using the `--subject-prefix` argument to
      `git format-patch`, for example to submit a patch to the
      "&DISTRO_NAME_NO_CAP_MINUS_ONE;" branch use::

         git format-patch --subject-prefix='&DISTRO_NAME_NO_CAP_MINUS_ONE;][PATCH' ...

## Taking Patch Review into Account

You may get feedback on your submitted patches from other community members
or from the automated patchtest service. If issues are identified in your
patches then it is usually necessary to address these before the patches are
accepted into the project. In this case you should revise your commits according
to the feedback and submit an updated version to the relevant mailing list.

In any case, never fix reported issues by fixing them in new commits
on the tip of your branch. Always come up with a new series of commits
without the reported issues.

> **Note:**
> It is a good idea to send a copy to the reviewers who provided feedback
> to the previous version of the patch. You can make sure this happens
> by adding a `CC` tag to the commit description::
>
> CC: William Shakespeare <bill@yoctoproject.org>
>

A single patch can be amended using `git commit --amend`, and multiple
patches can be easily reworked and reordered through an interactive Git rebase::

   git rebase -i <ref-branch>

See `this tutorial <https://hackernoon.com/beginners-guide-to-interactive-rebasing-346a3f9c3a6d>`__
for practical guidance about using Git interactive rebasing.

You should also modify the `[PATCH]` tag in the email subject line when
sending the revised patch to mark the new iteration as `[PATCH v2]`,
`[PATCH v3]`, etc as appropriate. This can be done by passing the `-v`
argument to `git format-patch` with a version number::

   git format-patch -v2 <ref-branch>

After generating updated patches (v2, v3, and so on) via ``git
format-patch``, ideally developers will add a patch version changelog
to each patch that describes what has changed between each revision of
the patch. Add patch version changelogs after the `---` marker in the
patch, indicating that this information is part of this patch, but is not
suitable for inclusion in the commit message (i.e. the git history) itself.
Providing a patch version changelog makes it easier for maintainers and
reviewers to succinctly understand what changed in all versions of the
patch, without having to consult alternate sources of information, such as
searching through messages on a mailing list. For example::

   <patch title>

   <commit message>

   <Signed-off-by/other trailers>
   ---
   changes in v4:
   - provide a clearer commit message
   - fix spelling mistakes

   changes in v3:
   - replace func() to use other_func() instead

   changes in v2:
   - this patch was added in v2
   ---
   <diffstat output>

   <unified diff>

Lastly please ensure that you also test your revised changes. In particular
please don't just edit the patch file written out by `git format-patch` and
resend it.

## Tracking the Status of Patches

The Yocto Project uses a `Patchwork instance <https://patchwork.yoctoproject.org/>`__
to track the status of patches submitted to the various mailing lists and to
support automated patch testing. Each submitted patch is checked for common
mistakes and deviations from the expected patch format and submitters are
notified by `patchtest` if such mistakes are found. This process helps to
reduce the burden of patch review on maintainers.

> **Note:**
> This system is imperfect and changes can sometimes get lost in the flow.
> Asking about the status of a patch or change is reasonable if the change
> has been idle for a while with no feedback.
>

If your patches have not had any feedback in a few days, they may have already
been merged. You can run `git pull`  branch to check this. Note that many if
not most layer maintainers do not send out acknowledgement emails when they
accept patches. Alternatively, if there is no response or merge after a few days
the patch may have been missed or the appropriate reviewers may not currently be
around. It is then perfectly fine to reply to it yourself with a reminder asking
for feedback.

> **Note:**
> Patch reviews for feature and recipe upgrade patches are likely be delayed
> during a feature freeze because these types of patches aren't merged during
> at that time --- you may have to wait until after the freeze is lifted.
>

Maintainers also commonly use `-next` branches to test submissions prior to
merging patches. Thus, you can get an idea of the status of a patch based on
whether the patch has been merged into one of these branches. The commonly
used testing branches for OpenEmbedded-Core are as follows:

-  *openembedded-core "master-next" branch:* This branch is part of the
   openembedded-core  repository and contains
   proposed changes to the core metadata.

-  *bitbake "master-next" branch:* This branch is part of the :oe_git:`bitbake
   </bitbake/>` repository and contains changes to **BitBake**.

-  *meta-yocto "master-next" branch:* This branch is part of the
   meta-yocto  repository and contains proposed
   changes to meta-yocto.

Similarly, stable branches maintained by the project may have corresponding
`-next` branches which collect proposed changes. For example,
`&DISTRO_NAME_NO_CAP;-next` and `&DISTRO_NAME_NO_CAP_MINUS_ONE;-next`
branches in both the "openembedded-core" and "meta-yocto" repositories.

Other layers may have similar testing branches but there is no formal
requirement or standard for these so please check the documentation for the
layers you are contributing to.

## Acceptance of AI Generated Code

The Yocto Project and OpenEmbedded follow the guidance of the Linux Foundation
in regards to the use of generative AI tools. See:
https://www.linuxfoundation.org/legal/generative-ai.

All of the existing guidelines in this document are expected to be followed,
including in the /contributor-guide/recipe-style-guide, and contributing the changes with
additional requirements to the items in section
contributor-guide/submit-changes:Implement and commit changes.

All AI Generated Code must be labeled as such in the commit message,
prior to your `Signed-off-by` line. It is also strongly recommended,
that any patches or code within the commit also have a comment or other
indication that this code was AI generated.

For example, here is a properly formatted commit message::

   component: Add the ability to ...

   AI-Generated: Uses GitHub Copilot

   Signed-off-by: Your Name <your.name@domain>

The `Signed-off-by` line must be written by you, and not the AI helper.
As a reminder, when contributing a change, your `Signed-off-by` line is
required and the stipulations in the `Developer's Statement of Origin
1.1 <https://developercertificate.org/>`__ still apply.

Additionally, you must stipulate AI contributions conform to the Linux
Foundation policy, specifically:

#. Contributors should ensure that the terms and conditions of the generative AI
   tool do not place any contractual restrictions on how the tool's output can
   be used that are inconsistent with the project's open source software
   license, the project's intellectual property policies, or the Open Source
   Definition.

#. If any pre-existing copyrighted materials (including pre-existing open
   source code) authored or owned by third parties are included in the AI tool's
   output, prior to contributing such output to the project, the Contributor
   should confirm that they have permission from the third party
   owners -- such as the form of an open source license or public domain
   declaration that complies with the project's licensing policies -- to use and
   modify such pre-existing materials and contribute them to the project.
   Additionally, the contributor should provide notice and attribution of such
   third party rights, along with information about the applicable license
   terms, with their contribution.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Reporting a Defect Against the Yocto Project and OpenEmbedded

You can use the Yocto Project instance of
`Bugzilla <https://www.bugzilla.org/about/>`__ to submit a defect (bug)
against BitBake, OpenEmbedded-Core, against any other Yocto Project component
or for tool issues. For additional information on this implementation of
Bugzilla see the "Yocto Project Bugzilla " section
in the Yocto Project Reference Manual. For more detail on any of the following
steps, see the Yocto Project
Bugzilla wiki page .

Use the following general steps to submit a bug:

#.  Open the Yocto Project implementation of `Bugzilla <>`.

#.  Click "File a Bug" to enter a new bug.

#.  Choose the appropriate "Classification", "Product", and "Component"
    for which the bug was found. Bugs for the Yocto Project fall into
    one of several classifications, which in turn break down into
    several products and components. For example, for a bug against the
    `meta-intel` layer, you would choose "Build System, Metadata &
    Runtime", "BSPs", and "bsps-meta-intel", respectively.

#.  Choose the "Version" of the Yocto Project for which you found the
    bug (e.g. &DISTRO;).

#.  Determine and select the "Severity" of the bug. The severity
    indicates how the bug impacted your work.

#.  Choose the "Hardware" that the bug impacts.

#.  Choose the "Architecture" that the bug impacts.

#.  Choose a "Documentation change" item for the bug. Fixing a bug might
    or might not affect the Yocto Project documentation. If you are
    unsure of the impact to the documentation, select "Don't Know".

#.  Provide a brief "Summary" of the bug. Try to limit your summary to
    just a line or two and be sure to capture the essence of the bug.

#.  Provide a detailed "Description" of the bug. You should provide as
    much detail as you can about the context, behavior, output, and so
    forth that surrounds the bug. You can even attach supporting files
    for output from logs by using the "Add an attachment" button.

#.  Click the "Submit Bug" button submit the bug. A new Bugzilla number
    is assigned to the bug and the defect is logged in the bug tracking
    system.

Once you file a bug, the bug is processed by the Yocto Project Bug
Triage Team and further details concerning the bug are assigned (e.g.
priority and owner). You are the "Submitter" of the bug and any further
categorization, progress, or comments on the bug result in Bugzilla
sending you an automated email concerning the particular change or
progress to the bug.

There are no guarantees about if or when a bug might be worked on since an
open-source project has no dedicated engineering resources. However, the
project does have a good track record of resolving common issues over the
medium and long term. We do encourage people to file bugs so issues are
at least known about. It helps other users when they find somebody having
the same issue as they do, and an issue that is unknown is much less likely
to ever be fixed!

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Identify the component

The Yocto Project and OpenEmbedded ecosystem is built of **layers <Layer>**
so the first step is to identify the component where the issue likely lies.
For example, if you have a hardware issue, it is likely related to the BSP
you are using and the best place to seek advice would be from the BSP provider
or **layer**. If the issue is a build/configuration one and a distro is in
use, they would likely be the first place to ask questions. If the issue is a
generic one and/or in the core classes or metadata, the core layer or BitBake
might be the appropriate component.

Each metadata layer being used should contain a `README` file and that should
explain where to report issues, where to send changes and how to contact the
maintainers.

If the issue is in the core metadata layer (OpenEmbedded-Core) or in BitBake,
issues can be reported in the `Yocto Project Bugzilla <>`. The
yocto  mailing list is a general “catch-all” location
where questions can be sent if you can’t work out where something should go.

.. SPDX-License-Identifier: CC-BY-SA-2.0-UK

## Recipe Style Guide

## Recipe Naming Conventions

In general, most recipes should follow the naming convention
`recipes-category/recipename/recipename_version.bb`. Recipes for related
projects may share the same recipe directory. `recipename` and `category`
may contain hyphens, but hyphens are not allowed in `version`.

If the recipe is tracking a Git revision that does not correspond to a released
version of the software, `version` may be `git` (e.g. `recipename_git.bb`)
and the recipe would set **PV**.

## Version Policy

Our versions follow the form `<epoch>:<version>-<revision>`
or in BitBake variable terms ${**PE**}:${**PV**}-${**PR**}. We
generally follow the `Debian <https://www.debian.org/doc/debian-policy/ch-controlfields.html#version>`__
version policy which defines these terms.

In most cases the version **PV** will be set automatically from the recipe
file name. It is recommended to use released versions of software as these are
revisions that upstream are expecting people to use.

Recipe versions should always compare and sort correctly so that upgrades work
as expected. With conventional versions such as `1.4` upgrading `to 1.5`
this happens naturally, but some versions don't sort. For example,
`1.5 Release Candidate 2` could be written as `1.5rc2` but this sorts after
`1.5`, so upgrades from feeds won't happen correctly.

Instead the tilde (`~`) operator can be used, which sorts before the empty
string so `1.5~rc2` comes before `1.5`. There is a historical syntax which
may be found where **PV** is set as a combination of the prior version
`+` the pre-release version, for example `PV=1.4+1.5rc2`. This is a valid
syntax but the tilde form is preferred.

For version comparisons, the `opkg-compare-versions` program from
`opkg-utils` can be useful when attempting to determine how two version
numbers compare to each other. Our definitive version comparison algorithm is
the one within bitbake which aims to match those of the package managers and
Debian policy closely.

When a recipe references a git revision that does not correspond to a released
version of software (e.g. is not a tagged version), the **PV** variable
should include the sign `+`, so **bitbake** automatically includes package
version information during the packaging phase::

    PV = "<version>+git"

In this case, `<version>` should be the most recently released version of the
software from the current source revision (`git describe` can be useful for
determining this). Whilst not recommended for published layers, this format is
also useful when using **AUTOREV** to set the recipe to increment source
control revisions automatically, which can be useful during local development.

## Version Number Changes

The **PR** variable is used to indicate different revisions of a recipe
that reference the same upstream source version. It can be used to force a
new version of a recipe to be installed onto a device from a package feed.
These once had to be set manually but in most cases these can now be set and
incremented automatically by a PR Server connected with a package feed.

When **PV** increases, any existing **PR** value can and should be
removed.

If **PV** changes in such a way that it does not increase with respect to
the previous value, you need to increase **PE** to ensure package managers
will upgrade it correctly. If unset you should set **PE** to "1" since
the default of empty is easily confused with "0" depending on the package
manager. **PE** can only have an integer value.

## Recipe formatting

### Variable Formatting

-  Variable assignment should a space around each side of the operator, e.g.
   `FOO = "bar"`, not `FOO="bar"`.

-  Double quotes should be used on the right-hand side of the assignment,
   e.g. `FOO = "bar"` not `FOO = 'bar'`

-  Spaces should be used for indenting variables, with 4 spaces per tab

-  Long variables should be split over multiple lines when possible by using
   the continuation character (`\`)

-  When splitting a long variable over multiple lines, all continuation lines
   should be indented (with spaces) to align with the start of the quote on the
   first line::

       FOO = "this line is \
              long \
              "

   Instead of::

       FOO = "this line is \
       long \
       "

### Python Function formatting

-  Spaces must be used for indenting Python code, with 4 spaces per tab

### Shell Function formatting

-  The formatting of shell functions should be consistent within layers.
   Some use tabs, some use spaces.

## Recipe metadata

### Required Variables

The following variables should be included in all recipes:

-  **SUMMARY**: a one line description of the upstream project

-  **DESCRIPTION**: an extended description of the upstream project,
   possibly with multiple lines. If no reasonable description can be written,
   this may be omitted as it defaults to **SUMMARY**.

-  **HOMEPAGE**: the URL to the upstream projects homepage.

-  **BUGTRACKER**: the URL upstream projects bug tracking website,
   if applicable.

### Recipe Ordering

When a variable is defined in recipes and classes, variables should follow the
general order when possible:

-  **SUMMARY**
-  **DESCRIPTION**
-  **HOMEPAGE**
-  **BUGTRACKER**
-  **SECTION**
-  **LICENSE**
-  **LIC_FILES_CHKSUM**
-  **DEPENDS**
-  **PROVIDES**
-  **PV**
-  **SRC_URI**
-  **SRCREV**
-  **S**
-  `inherit ...`
-  **PACKAGECONFIG**
-  Build class specific variables such as **EXTRA_OECONF**
-  Tasks such as ref-tasks-configure
-  **PACKAGE_ARCH**
-  **PACKAGES**
-  **FILES**
-  **RDEPENDS**
-  **RRECOMMENDS**
-  **RSUGGESTS**
-  **RPROVIDES**
-  **RCONFLICTS**
-  **BBCLASSEXTEND**

There are some cases where ordering is important and these cases would override
this default order. Examples include:

-  **PACKAGE_ARCH** needing to be set before `inherit packagegroup`

Tasks should be ordered based on the order they generally execute. For commonly
used tasks this would be:

-  ref-tasks-fetch
-  ref-tasks-unpack
-  ref-tasks-patch
-  ref-tasks-prepare_recipe_sysroot
-  ref-tasks-configure
-  ref-tasks-compile
-  ref-tasks-install
-  ref-tasks-populate_sysroot
-  ref-tasks-package

Custom tasks should be sorted similarly.

Package specific variables are typically grouped together, e.g.::

    RDEPENDS:${PN} = “foo”
    RDEPENDS:${PN}-libs = “bar”

    RRECOMMENDS:${PN} = “one”
    RRECOMMENDS:${PN}-libs = “two”

### Recipe License Fields

Recipes need to define both the **LICENSE** and
**LIC_FILES_CHKSUM** variables:

-  **LICENSE**: This variable specifies the license for the software.
   If you do not know the license under which the software you are
   building is distributed, you should go to the source code and look
   for that information. Typical files containing this information
   include `COPYING`, **LICENSE**, and `README` files. You could
   also find the information near the top of a source file. For example,
   given a piece of software licensed under the GNU General Public
   License version 2, you would set **LICENSE** as follows::

      LICENSE = "GPL-2.0-only"

   The licenses you specify within **LICENSE** can have any name as long
   as you do not use spaces, since spaces are used as separators between
   license names. For standard licenses, use the names of the files in
   `meta/files/common-licenses/` or the **SPDXLICENSEMAP** flag names
   defined in `meta/conf/licenses.conf`.

   .. note::

      Setting a **LICENSE** in a recipe applies to the software to be built
      by this recipe, not to the recipe file itself. The license of recipes,
      configuration files and scripts should also be clearly specified, for
      example via comments or via a license found in the **layer** that
      holds these files. These license files are usually found at the root of
      the layer. Exceptions should be clearly stated in the layer README or
      LICENSE file.

      For example, the **OpenEmbedded-Core (OE-Core)** layer provides both
      the GPL-2.0-only and MIT license files, and a "LICENSE" file to explain
      how these two licenses are attributed to files found in the layer.

-  **LIC_FILES_CHKSUM**: The OpenEmbedded build system uses this
   variable to make sure the license text has not changed. If it has,
   the build produces an error and it affords you the chance to figure
   it out and correct the problem.

   You need to specify all applicable licensing files for the software.
   At the end of the configuration step, the build process will compare
   the checksums of the files to be sure the text has not changed. Any
   differences result in an error with the message containing the
   current checksum. For more explanation and examples of how to set the
   **LIC_FILES_CHKSUM** variable, see the
   "dev-manual/licenses:tracking license changes" section.

   To determine the correct checksum string, you can list the
   appropriate files in the **LIC_FILES_CHKSUM** variable with incorrect
   md5 strings, attempt to build the software, and then note the
   resulting error messages that will report the correct md5 strings.
   See the "dev-manual/new-recipe:fetching code" section for
   additional information.

   Here is an example that assumes the software has a `COPYING` file::

      LIC_FILES_CHKSUM = "file://COPYING;md5=xxx"

   When you try to build the
   software, the build system will produce an error and give you the
   correct string that you can substitute into the recipe file for a
   subsequent build.

#### License Updates

When you change the **LICENSE** or **LIC_FILES_CHKSUM** in the recipe
you need to briefly explain the reason for the change via a `License-Update:`
tag.  Often it's quite trivial, such as::

    License-Update: copyright years refreshed

Less often, the actual licensing terms themselves will have changed.  If so, do
try to link to upstream making/justifying that decision.

### Tips and Guidelines for Writing Recipes

-  Use **BBCLASSEXTEND** instead of creating separate recipes such as `-native`
   and `-nativesdk` ones, whenever possible. This avoids having to maintain multiple
   recipe files at the same time.

-  Recipes should have tasks which are idempotent, i.e. that executing a given task
   multiple times shouldn't change the end result. The build environment is built upon
   this assumption and breaking it can cause obscure build failures.

-  For idempotence when modifying files in tasks, it is usually best to:

   - copy a file `X` to `X.orig` (only if it doesn't exist already)
   - then, copy `X.orig` back to `X`,
   - and, finally, modify `X`.

   This ensures if rerun the task always has the same end result and the
   original file can be preserved to reuse. It also guards against an
   interrupted build corrupting the file.

## Patch Upstream Status

In order to keep track of patches applied by recipes and ultimately reduce the
number of patches that need maintaining, the OpenEmbedded build system
requires information about the upstream status of each patch.

In its description, each patch should provide detailed information about the
bug that it addresses, such as the URL in a bug tracking system and links
to relevant mailing list archives.

Then, you should also add an `Upstream-Status:` tag containing one of the
following status strings:

`Pending`
   No determination has been made yet, or patch has not yet been submitted to
   upstream.

   Keep in mind that every patch submitted upstream reduces the maintainance
   burden in OpenEmbedded and Yocto Project in the long run, so this patch
   status should only be used in exceptional cases if there are genuine
   obstacles to submitting a patch upstream; the reason for that should be
   included in the patch.

`Submitted [where]`
   Submitted to upstream, waiting for approval. Optionally include where
   it was submitted, such as the author, mailing list, etc.

`Backport [version]`
   Accepted upstream and included in the next release, or backported from newer
   upstream version, because we are at a fixed version.
   Include upstream version info (e.g. commit ID or next expected version).

`Denied`
   Not accepted by upstream, include reason in patch.

`Inactive-Upstream [lastcommit: when (and/or) lastrelease: when]`
   The upstream is no longer available. This typically means a defunct project
   where no activity has happened for a long time --- measured in years. To make
   that judgement, it is recommended to look at not only when the last release
   happened, but also when the last commit happened, and whether newly made bug
   reports and merge requests since that time receive no reaction. It is also
   recommended to add to the patch description any relevant links where the
   inactivity can be clearly seen.

`Inappropriate [reason]`
   The patch is not appropriate for upstream, include a brief reason on the
   same line enclosed with `[]`. In the past, there were several different
   reasons not to submit patches upstream, but we have to consider that every
   non-upstreamed patch means a maintainance burden for recipe maintainers.
   Currently, the only reasons to mark patches as inappropriate for upstream
   submission are:

   -  `oe specific`: the issue is specific to how OpenEmbedded performs builds
      or sets things up at runtime, and can be resolved only with a patch that
      is not however relevant or appropriate for general upstream submission.
   -  `upstream ticket <link>`: the issue is not specific to Open-Embedded
      and should be fixed upstream, but the patch in its current form is not
      suitable for merging upstream, and the author lacks sufficient expertise
      to develop a proper patch. Instead the issue is handled via a bug report
      (include link).

Of course, if another person later takes care of submitting this patch upstream,
the status should be changed to `Submitted [where]`, and an additional
`Signed-off-by:` line should be added to the patch by the person claiming
responsibility for upstreaming.

### Examples

Here's an example of a patch that has been submitted upstream::

   rpm: Adjusted the foo setting in bar

   [RPM Ticket #65] -- http://rpm5.org/cvs/tktview?tn=65,5

   The foo setting in bar was decreased from X to X-50% in order to
   ensure we don't exhaust all system memory with foobar threads.

   Upstream-Status: Submitted [rpm5-devel@rpm5.org]

   Signed-off-by: Joe Developer <joe.developer@example.com>

A future update can change the value to `Backport` or `Denied` as
appropriate.

Another example of a patch that is specific to OpenEmbedded::

   Do not treat warnings as errors

   There are additional warnings found with musl which are
   treated as errors and fails the build, we have more combinations
   than upstream supports to handle.

   Upstream-Status: Inappropriate [oe specific]

Here's a patch that has been backported from an upstream commit::

   include missing sys/file.h for LOCK_EX

   Upstream-Status: Backport [https://github.com/systemd/systemd/commit/ac8db36cbc26694ee94beecc8dca208ec4b5fd45]

## CVE patches

In order to have a better control of vulnerabilities, patches that fix CVEs must
contain a `CVE:` tag. This tag list all CVEs fixed by the patch. If more than
one CVE is fixed, separate them using spaces.

### CVE Examples

This should be the header of patch that fixes `2015-8370` in GRUB2::

   grub2: Fix CVE-2015-8370

   [No upstream tracking] -- https://bugzilla.redhat.com/show_bug.cgi?id=1286966

   Back to 28; Grub2 Authentication

   Two functions suffer from integer underflow fault; the grub_username_get() and grub_password_get()located in
   grub-core/normal/auth.c and lib/crypto.c respectively. This can be exploited to obtain a Grub rescue shell.

   Upstream-Status: Backport [http://git.savannah.gnu.org/cgit/grub.git/commit/?id=451d80e52d851432e109771bb8febafca7a5f1f2]
   CVE: CVE-2015-8370
   Signed-off-by: Joe Developer <joe.developer@example.com>

## Patch format

By default, patches created with `git format-patch` have a `Git` version signature at the end.
To avoid having a `Git` signature at the end of generated or updated patches,
you can use `Git` configuration settings::

   git config --global format.signature ""

> **Note:**
> Patches generated or updated by `devtool` are created with no signature.
>
>
>
