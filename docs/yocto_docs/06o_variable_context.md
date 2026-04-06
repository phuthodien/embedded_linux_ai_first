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
