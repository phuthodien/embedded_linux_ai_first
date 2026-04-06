# Yocto Project Documentation Index

**Source**: Yocto Project Documentation (master branch, yocto-docs git repo)
**Format**: Converted from RST source to Markdown (clean, no OCR artifacts)
**Total**: 38 markdown files

---

## Getting Started

| File | Topic | Description |
|------|-------|-------------|
| [01_quick_build.md](01_quick_build.md) | Quick Build | Step-by-step first image build, system requirements, host setup |
| [02_what_i_wish_i_had_known.md](02_what_i_wish_i_had_known.md) | Best Practices | Common pitfalls, layer organization, BKMs |
| [03_transitioning_custom_environment.md](03_transitioning_custom_environment.md) | Custom Environment | Migrating from existing build systems to Yocto |
| [04_overview_and_concepts.md](04_overview_and_concepts.md) | Overview & Concepts | Layer model, components, tools, terminology, workflow |

## Contributor Guide

| File | Topic | Description |
|------|-------|-------------|
| [05_contributor_guide.md](05_contributor_guide.md) | Contributing | Submitting patches, reporting defects, recipe style guide |

## Reference Manual (Chapter 06)

Main merged file: [06_reference_manual.md](06_reference_manual.md) — complete reference (~22k lines)

Sub-chapters for targeted lookup:

| File | Topic | Description |
|------|-------|-------------|
| [06a_system_requirements.md](06a_system_requirements.md) | System Requirements | Build host requirements, supported Linux distributions |
| [06b_supported_architectures.md](06b_supported_architectures.md) | Supported Features | CPU architectures, platform support levels |
| [06c_terms_glossary.md](06c_terms_glossary.md) | Terms & Glossary | Yocto Project terminology definitions |
| [06d_releases_process.md](06d_releases_process.md) | Release Process | Versioning, release lifecycle |
| [06e_source_directory_structure.md](06e_source_directory_structure.md) | Directory Structure | Source tree organization, layer structure |
| [06f_classes.md](06f_classes.md) | Classes | BitBake .bbclass file reference (~3.2k lines) |
| [06g_tasks.md](06g_tasks.md) | Tasks | BitBake task descriptions and execution flow |
| [06h_configuration_fragments.md](06h_configuration_fragments.md) | Configuration Fragments | Config file formats and options |
| [06i_devtool_reference.md](06i_devtool_reference.md) | Devtool Reference | devtool command reference |
| [06j_wks_reference.md](06j_wks_reference.md) | Kickstart (WKS) | Image definition syntax (wic/kickstart) |
| [06k_qa_messages.md](06k_qa_messages.md) | QA Checks | Build warnings and error messages explained |
| [06l_images.md](06l_images.md) | Images | Available image types and their purposes |
| [06m_features.md](06m_features.md) | Features | DISTRO_FEATURES, MACHINE_FEATURES, IMAGE_FEATURES |
| [06n_variables_glossary.md](06n_variables_glossary.md) | Variables Glossary | **Complete** variable reference (~12k lines) |
| [06o_variable_context.md](06o_variable_context.md) | Variable Context | Variable precedence and locality rules |
| [06p_faq.md](06p_faq.md) | FAQ | Common questions and answers |
| [06q_contributions.md](06q_contributions.md) | Resources | Links, mailing lists, community resources |

## BSP Developer Guide (Chapter 07)

| File | Topic | Description |
|------|-------|-------------|
| [07_bsp_developer_guide.md](07_bsp_developer_guide.md) | BSP Guide | BSP layer creation, machine configuration |

## Development Tasks (Chapter 08)

Main merged file: [08_development_tasks.md](08_development_tasks.md) — complete dev manual (~13k lines)

Sub-chapters for targeted lookup:

| File | Topic | Description |
|------|-------|-------------|
| [08a_setting_up.md](08a_setting_up.md) | Setting Up | Build host setup, initial configuration |
| [08b_creating_layers.md](08b_creating_layers.md) | Creating Layers | Layer structure, creating custom layers |
| [08c_customizing_images.md](08c_customizing_images.md) | Customizing Images | Adding packages, modifying distro config |
| [08d_writing_recipes.md](08d_writing_recipes.md) | Writing Recipes | Recipe syntax, creating .bb files |
| [08e_adding_new_machine.md](08e_adding_new_machine.md) | New Machine | BSP creation, machine configuration |
| [08f_upgrading_recipes.md](08f_upgrading_recipes.md) | Upgrading Recipes | Version upgrades, recipe maintenance |
| [08g_source_config_shells.md](08g_source_config_shells.md) | Source & Shells | Temporary source, dev shell, Python shell |
| [08h_building_images.md](08h_building_images.md) | Building Images | Build process, wic, bmaptool, optimization |
| [08i_performance_libraries.md](08i_performance_libraries.md) | Libraries | Shared/static libraries, prebuilt libraries |
| [08j_using_devtool.md](08j_using_devtool.md) | Using devtool | devtool workflow for recipe development |
| [08k_advanced_topics.md](08k_advanced_topics.md) | Advanced Topics | Multiconfig, QEMU, Wayland, packages, SBOM, etc. |
| [08l_debugging_tools.md](08l_debugging_tools.md) | Debugging | Build debugging, error reporting tool |
| [08m_licenses_tools.md](08m_licenses_tools.md) | Licenses | License compliance, manifest generation |

## Specialized Topics

| File | Topic | Description |
|------|-------|-------------|
| [09_kernel_development.md](09_kernel_development.md) | Kernel Development | Kernel config, metadata, patching, customization |
| [10_security_manual.md](10_security_manual.md) | Security | Securing images, read-only rootfs, vulnerabilities |
| [11_profiling_and_tracing.md](11_profiling_and_tracing.md) | Profiling & Tracing | perf, ftrace, systemtap, LTTng |
| [12_application_dev_esdk.md](12_application_dev_esdk.md) | App Dev & eSDK | Extensible SDK, application development |
| [13_toaster_user_manual.md](13_toaster_user_manual.md) | Toaster | Web-based build management interface |
| [14_test_environment.md](14_test_environment.md) | Testing | Test framework, ptest, runtime testing |
| [15_bitbake_documentation.md](15_bitbake_documentation.md) | BitBake | Pointer to BitBake documentation |
| [16_security_reference.md](16_security_reference.md) | Security Reference | Security team, vulnerability reporting |
| [17_release_information.md](17_release_information.md) | Migration Guides | Migration notes for versions 4.0-5.1 |

## Quick Reference by Topic

| Subject | Primary Files | Secondary |
|---------|---------------|-----------|
| **Getting Started** | 01, 04, 02 | 08a, 06a |
| **Building Images** | 08h, 08c | 06l, 08d |
| **Recipe Development** | 08d, 08b | 06f, 06g, 06n |
| **Layer Management** | 08b, 02 | 06e, 08a |
| **Kernel Development** | 09 | 06_reference |
| **Machine/BSP** | 08e, 07 | 06b |
| **Debugging** | 08l | 08k, 11 |
| **Variables** | 06n | 06o, 06p |
| **Security** | 10, 16 | 08m |
| **Testing** | 14 | 08l |
| **devtool** | 08j | 06i |
