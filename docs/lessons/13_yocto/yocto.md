# Bài giảng: Yocto Project — Build hệ thống Linux cho Embedded

**Giảng viên:** Lưu An Phú — FPT Software Corporate Training Center
**Khóa học:** Lập trình Embedded Linux
**Thời lượng:** 8 giờ (lý thuyết + thực hành)
**Target board:** BeagleBone Black (AM335x)

---

## MỤC LỤC

- [Phần 1 — Tại sao cần Yocto Project?](#phần-1--tại-sao-cần-yocto-project)
- [Phần 2 — Kiến trúc và các thành phần chính](#phần-2--kiến-trúc-và-các-thành-phần-chính)
- [Phần 3 — Workflow: Từ source code đến image](#phần-3--workflow-từ-source-code-đến-image)
- [Phần 4 — Thực hành: Build image cho BeagleBone Black](#phần-4--thực-hành-build-image-cho-beaglebone-black)
- [Phần 5 — Layer: Tổ chức metadata](#phần-5--layer-tổ-chức-metadata)
- [Phần 6 — Recipe: Đơn vị xây dựng phần mềm](#phần-6--recipe-đơn-vị-xây-dựng-phần-mềm)
- [Phần 7 — Thực hành: Tạo custom layer và recipe](#phần-7--thực-hành-tạo-custom-layer-và-recipe)
- [Phần 8 — Tùy chỉnh image](#phần-8--tùy-chỉnh-image)
- [Phần 9 — Tổng kết và bài tập](#phần-9--tổng-kết-và-bài-tập)

---

## Phần 1 — Tại sao cần Yocto Project?

### 1.1 Vấn đề: Tạo Linux cho embedded device không đơn giản

Khi phát triển embedded Linux, bạn cần một hệ thống Linux hoàn chỉnh chạy trên board: kernel, root filesystem, bootloader, toolchain. Câu hỏi là **lấy chúng từ đâu?**

Có ba cách tiếp cận phổ biến:

| Cách | Mô tả | Hạn chế |
|------|-------|---------|
| **Dùng distro có sẵn** | Debian, Ubuntu cho ARM | Image lớn, nhiều package thừa, khó tối ưu cho thiết bị nhỏ |
| **Build tay** | Tự tải source, cross-compile từng thành phần | Cực kỳ tốn thời gian, khó reproduce, khó maintain |
| **Build system** | Dùng công cụ tự động hóa (Yocto, Buildroot) | Cần học, nhưng giải quyết được các vấn đề trên |

Với embedded, bạn thường cần:
- **Image nhỏ** — thiết bị có flash/RAM giới hạn, không cần 90% package của Ubuntu
- **Reproducible** — build hôm nay và build tháng sau phải cho kết quả giống nhau
- **Customizable** — chỉ đưa vào đúng những gì cần thiết
- **Cross-compile** — build trên x86 host cho ARM target

Build tay thì không scale. Distro có sẵn thì quá nặng. Bạn cần một **build system**.

---

### 1.2 Yocto Project là gì — và KHÔNG phải là gì

Nhiều người mới hay nhầm lẫn, nên nói rõ:

**Yocto Project KHÔNG phải:**
- Một Linux distribution — nó dùng để **build** distribution
- Một SDK — nó dùng để **tạo** SDK
- Một image — nó dùng để **sinh** image

**Yocto Project LÀ:**
- Một **open-source collaboration project** cung cấp templates, tools, và methods để tạo ra custom Linux system cho embedded device
- Bất kể hardware architecture nào (ARM, x86, MIPS, RISC-V...)

Nói đơn giản: **Yocto là nhà máy sản xuất Linux**. Bạn đưa vào cấu hình (muốn build cho board nào, cần package gì), Yocto tự động tải source, cross-compile, đóng gói, và xuất ra image sẵn sàng flash.

---

### 1.3 Yocto vs Buildroot

Hai build system phổ biến nhất cho embedded Linux:

| | Yocto Project | Buildroot |
|---|---|---|
| **Độ phức tạp** | Cao — learning curve dốc | Thấp — dễ bắt đầu |
| **Tính mở rộng** | Rất cao — layer system linh hoạt | Trung bình |
| **Cộng đồng** | Rất lớn — hỗ trợ từ Intel, TI, NXP, AMD... | Nhỏ hơn |
| **Package management** | Có (rpm, deb, ipk) | Không |
| **Incremental build** | Có — shared state cache (sstate) | Hạn chế |
| **Thời gian build lần đầu** | Rất lâu (hàng giờ) | Nhanh hơn |
| **Phù hợp cho** | Sản phẩm lớn, dài hạn, nhiều board | Prototype, sản phẩm nhỏ |

Trong thực tế công nghiệp, **Yocto là tiêu chuẩn**. Hầu hết silicon vendor (TI, NXP, Intel, Qualcomm...) cung cấp BSP dưới dạng Yocto layer.

---

## Phần 2 — Kiến trúc và các thành phần chính

### 2.1 Ba thành phần cốt lõi

```
┌──────────────────────────────────────────────────┐
│                  Yocto Project                    │
│                                                   │
│  ┌────────────┐  ┌──────────────┐  ┌───────────┐ │
│  │  BitBake    │  │  OE-Core     │  │  Poky     │ │
│  │ (scheduler  │  │ (metadata    │  │ (reference│ │
│  │  & engine)  │  │  foundation) │  │  distro)  │ │
│  └────────────┘  └──────────────┘  └───────────┘ │
└──────────────────────────────────────────────────┘
```

**BitBake** — Task scheduler và execution engine
- Đọc recipe (file `.bb`), phân tích dependency, lên lịch build
- Tương tự `make` nhưng mạnh hơn nhiều: hỗ trợ cross-compile, parallel build, caching
- BitBake không biết gì về Linux hay embedded — nó chỉ biết chạy task theo thứ tự

**OpenEmbedded-Core (OE-Core)** — Metadata foundation
- Tập hợp các recipe, class, và configuration cơ bản
- Chứa recipe cho: Linux kernel, glibc, busybox, systemd, gcc...
- Đây là "nguyên liệu" mà BitBake dùng để build

**Poky** — Reference distribution
- Là **ví dụ hoàn chỉnh** về cách dùng BitBake + OE-Core để tạo một Linux distribution
- Bao gồm: BitBake + OE-Core + meta-poky (distro config) + meta-yocto-bsp (reference BSPs)
- Khi bắt đầu học Yocto, bạn clone Poky — nó chứa mọi thứ cần thiết

---

### 2.2 Khái niệm quan trọng: Recipe vs Package

Đây là điểm hay nhầm lẫn:

```
 Recipe (.bb file)                    Package (.rpm / .deb / .ipk)
 ┌─────────────────┐                 ┌─────────────┐
 │ Chỉ dẫn build:  │   BitBake      │ Binary đã   │
 │ - fetch source  │ ──────────▶    │ compile:     │
 │ - configure     │   (build)      │ - main pkg   │
 │ - compile       │                │ - dev pkg    │
 │ - install       │                │ - dbg pkg    │
 │ - package       │                │ - doc pkg    │
 └─────────────────┘                └─────────────┘
     (front-end)                       (output)
```

- **Recipe** = bản hướng dẫn (input) — nói cho BitBake biết: lấy source ở đâu, compile thế nào, install file gì
- **Package** = sản phẩm đã build (output) — file binary sẵn sàng đưa vào image

Một recipe có thể tạo ra **nhiều package**. Ví dụ recipe `bash` tạo ra: `bash`, `bash-dev`, `bash-dbg`, `bash-doc`.

---

### 2.3 Khái niệm: Layer

Layer là cách Yocto tổ chức metadata. Mỗi layer là một thư mục chứa recipe, configuration, và class liên quan đến một chức năng cụ thể.

```
layers/
├── meta/                    ← OE-Core (base recipes: kernel, glibc, busybox...)
├── meta-poky/               ← Poky distro configuration
├── meta-yocto-bsp/          ← Reference BSP (qemu, beaglebone, genericx86...)
├── meta-openembedded/       ← Community layers (networking, python, multimedia...)
│   ├── meta-oe/
│   ├── meta-python/
│   ├── meta-networking/
│   └── meta-multimedia/
├── meta-ti/                 ← TI BSP layer (cho AM335x, AM57xx...)
└── meta-custom/             ← Layer của bạn (custom recipe, config)
```

**Quy tắc vàng:**
- **KHÔNG BAO GIỜ** sửa trực tiếp layer gốc (meta, meta-poky, meta-ti...)
- Muốn thay đổi? Tạo layer riêng, dùng `.bbappend` để override
- Khi update Yocto lên version mới, layer gốc thay đổi — nếu bạn sửa trực tiếp, **mất hết**

Convention: tên layer bắt đầu bằng `meta-`.

---

## Phần 3 — Workflow: Từ source code đến image

### 3.1 Tám bước của Yocto build

Khi bạn chạy `bitbake core-image-minimal`, BitBake thực hiện toàn bộ quy trình sau cho **mỗi** recipe:

```
 ① Fetch          Tải source code (git, tarball, svn...)
      ▼
 ② Extract         Giải nén source vào thư mục làm việc
      ▼
 ③ Patch           Áp dụng patch (bug fix, customization)
      ▼
 ④ Configure       Chạy configure (autotools, cmake, meson...)
      ▼
 ⑤ Compile         Cross-compile source code
      ▼
 ⑥ Install         Copy file vào staging directory
      ▼
 ⑦ Package         Đóng gói thành .rpm / .deb / .ipk
      ▼
 ⑧ Image           Gộp tất cả package → tạo root filesystem image
```

Mỗi bước tương ứng với một **task** trong BitBake:

| Bước | Task | Mô tả |
|------|------|-------|
| Fetch | `do_fetch` | Tải source về `DL_DIR` |
| Extract | `do_unpack` | Giải nén vào `WORKDIR` |
| Patch | `do_patch` | Apply các file `.patch` |
| Configure | `do_configure` | Chạy `./configure` hoặc `cmake` |
| Compile | `do_compile` | Chạy `make` hoặc tương đương |
| Install | `do_install` | Copy vào `${D}` (destination dir) |
| Package | `do_package` | Chia file thành các package |
| Image | `do_image` | Tạo filesystem image cuối cùng |

Nếu build lỗi, biết step nào đang fail giúp bạn debug đúng hướng. Ví dụ: lỗi ở `do_fetch` → kiểm tra network/URL. Lỗi ở `do_compile` → kiểm tra cross-compile flags.

---

### 3.2 Shared State Cache (sstate) — Tại sao build lần 2 nhanh hơn nhiều

Build lần đầu rất lâu (có thể 2-4 giờ trên máy mạnh) vì Yocto build **mọi thứ từ source**: gcc, glibc, kernel, busybox... hàng trăm package.

Nhưng lần build sau nhanh hơn rất nhiều nhờ **sstate-cache**: sau mỗi task thành công, BitBake lưu output vào cache. Lần build sau, nếu input không đổi (source, config giữ nguyên), BitBake bỏ qua task đó và dùng lại kết quả cached.

```
Build lần 1:  gcc(build) → glibc(build) → kernel(build) → ...   [2-4 giờ]
Build lần 2:  gcc(cache) → glibc(cache) → kernel(cache) → ...   [vài phút]
                                           ↓
              Chỉ re-build package thực sự thay đổi
```

Đây là lợi thế lớn của Yocto so với Buildroot — và là lý do nó phù hợp cho dự án dài hạn.

---

## Phần 4 — Thực hành: Build image cho BeagleBone Black

### 4.1 Yêu cầu hệ thống

| Yêu cầu | Tối thiểu | Khuyến nghị |
|----------|-----------|-------------|
| Disk | 140 GB trống | 250 GB+ (SSD) |
| RAM | 8 GB | 16-32 GB |
| CPU | 4 cores | 8+ cores |
| OS | Ubuntu 20.04/22.04 | Ubuntu 22.04 LTS |

> **Lưu ý:** Build Yocto rất tốn tài nguyên. SSD tạo sự khác biệt lớn so với HDD.

---

### 4.2 Cài đặt package cần thiết

```bash
sudo apt-get update
sudo apt-get install gawk wget git diffstat unzip texinfo gcc build-essential \
    chrpath socat cpio python3 python3-pip python3-pexpect xz-utils \
    debianutils iputils-ping python3-git python3-jinja2 python3-subunit \
    zstd liblz4-tool file locales libacl1
sudo locale-gen en_US.UTF-8
```

---

### 4.3 Clone Poky và các layer cần thiết

Chọn branch **kirkstone** (Yocto 4.0 LTS — Long Term Support, được hỗ trợ đến 04/2026):

```bash
# Tạo thư mục làm việc
mkdir -p ~/yocto && cd ~/yocto

# Clone Poky (chứa BitBake + OE-Core + reference distro)
git clone git://git.yoctoproject.org/poky -b kirkstone

# Clone meta-openembedded (các recipe bổ sung: networking, python, multimedia...)
git clone git://git.openembedded.org/meta-openembedded -b kirkstone
```

Sau khi clone, cấu trúc thư mục:

```
~/yocto/
├── poky/
│   ├── bitbake/           ← BitBake engine
│   ├── meta/              ← OE-Core
│   ├── meta-poky/         ← Poky distro config
│   ├── meta-yocto-bsp/    ← Reference BSP (bao gồm beaglebone)
│   └── oe-init-build-env  ← Script khởi tạo build environment
└── meta-openembedded/
    ├── meta-oe/
    ├── meta-python/
    ├── meta-networking/
    └── meta-multimedia/
```

---

### 4.4 Khởi tạo build environment

```bash
# Source script khởi tạo — tự động tạo thư mục build
cd ~/yocto
source poky/oe-init-build-env build-bbb
```

Sau lệnh này:
- Thư mục `build-bbb/` được tạo
- Biến `BUILDDIR` trỏ đến `build-bbb/`
- Terminal chuyển vào `build-bbb/`
- Các lệnh BitBake đã có trong `PATH`

Bên trong `build-bbb/conf/`:

```
build-bbb/conf/
├── local.conf       ← Cấu hình local: MACHINE, parallel jobs, download dir...
└── bblayers.conf    ← Danh sách layer được sử dụng
```

---

### 4.5 Thêm layer vào build

```bash
bitbake-layers add-layer ../meta-openembedded/meta-oe
bitbake-layers add-layer ../meta-openembedded/meta-python
bitbake-layers add-layer ../meta-openembedded/meta-networking
bitbake-layers add-layer ../meta-openembedded/meta-multimedia
```

Kiểm tra danh sách layer:

```bash
bitbake-layers show-layers
```

Output sẽ hiện tất cả layer đang active, bao gồm đường dẫn và priority.

---

### 4.6 Cấu hình cho BeagleBone Black

Mở file `conf/local.conf` và cấu hình:

```bash
# ---- MACHINE ----
# BeagleBone Black dùng machine name "beaglebone-yocto" (có sẵn trong meta-yocto-bsp)
MACHINE ??= "beaglebone-yocto"

# ---- PARALLEL BUILD ----
# Số task chạy song song (= số CPU core)
BB_NUMBER_THREADS = "8"
# Số job cho make -j (= số CPU core)
PARALLEL_MAKE = "-j8"

# ---- OPTIONAL: Giữ lại thư mục download để build sau nhanh hơn ----
DL_DIR ?= "${TOPDIR}/../downloads"
SSTATE_DIR ?= "${TOPDIR}/../sstate-cache"
```

> **Giải thích `BB_NUMBER_THREADS` và `PARALLEL_MAKE`:**
> - `BB_NUMBER_THREADS` = số recipe build cùng lúc (BitBake level)
> - `PARALLEL_MAKE` = số job bên trong mỗi recipe (make level)
> - Trên máy 8 core: đặt cả hai = 8 là hợp lý
> - Nếu RAM < 16 GB, giảm xuống 4 để tránh hết RAM

---

### 4.7 Build image

```bash
# Build image tối thiểu (console only, boot nhanh nhất)
bitbake core-image-minimal
```

**Các image type phổ biến:**

| Image | Mô tả | Kích thước |
|-------|--------|------------|
| `core-image-minimal` | Boot + shell, không có gì thêm | ~8 MB |
| `core-image-base` | Console + full hardware support | ~30 MB |
| `core-image-full-cmdline` | Console + đầy đủ CLI tools | ~100 MB |
| `core-image-sato` | GUI (Sato mobile environment) | ~300 MB |

Build lần đầu **rất lâu** (1-4 giờ tùy máy). Đừng hoảng — đây là bình thường.

---

### 4.8 Kết quả build

Sau khi build xong, image nằm ở:

```
build-bbb/tmp/deploy/images/beaglebone-yocto/
├── core-image-minimal-beaglebone-yocto.wic.xz    ← Image sẵn sàng flash
├── MLO                                             ← First-stage bootloader
├── u-boot.img                                      ← U-Boot
├── zImage                                          ← Linux kernel
└── am335x-bone.dtb                                 ← Device Tree
```

### 4.9 Flash image lên SD card

```bash
# Giải nén image
xz -d core-image-minimal-beaglebone-yocto.wic.xz

# Flash lên SD card (thay sdX bằng device thực tế, ví dụ sdb)
# CẢNH BÁO: Chọn sai device sẽ xóa dữ liệu!
sudo dd if=core-image-minimal-beaglebone-yocto.wic of=/dev/sdX bs=4M status=progress
sudo sync
```

Cắm SD card vào BBB, giữ nút BOOT (S2), bật nguồn → board boot từ SD.

---

## Phần 5 — Layer: Tổ chức metadata

### 5.1 Cấu trúc một layer

Mỗi layer có cấu trúc chuẩn:

```
meta-mylayer/
├── conf/
│   └── layer.conf          ← Khai báo layer (bắt buộc)
├── recipes-example/
│   └── example/
│       └── example_0.1.bb  ← Recipe mẫu
├── COPYING.MIT              ← License file
└── README
```

File quan trọng nhất là `conf/layer.conf`:

```bitbake
# Nội dung layer.conf cơ bản
BBPATH .= ":${LAYERDIR}"

BBFILES += "${LAYERDIR}/recipes-*/*/*.bb \
            ${LAYERDIR}/recipes-*/*/*.bbappend"

BBFILE_COLLECTIONS += "mylayer"
BBFILE_PATTERN_mylayer = "^${LAYERDIR}/"
BBFILE_PRIORITY_mylayer = "6"

LAYERDEPENDS_mylayer = "core"
LAYERSERIES_COMPAT_mylayer = "kirkstone"
```

| Biến | Ý nghĩa |
|------|---------|
| `BBFILES` | Đường dẫn tới các recipe (`.bb`) và append (`.bbappend`) |
| `BBFILE_COLLECTIONS` | Tên định danh của layer |
| `BBFILE_PRIORITY` | Độ ưu tiên (số cao = ưu tiên hơn khi trùng recipe) |
| `LAYERDEPENDS` | Layer phụ thuộc (ví dụ: cần `core` = OE-Core) |
| `LAYERSERIES_COMPAT` | Tương thích với Yocto release nào |

---

### 5.2 Tại sao dùng layer?

**Không dùng layer** — Mọi thứ gộp chung:
- Update Yocto → xung đột
- Đổi board → phải sửa nhiều chỗ
- Chia sẻ code → khó tách

**Dùng layer** — Tách biệt chức năng:
- BSP layer: phần cứng (meta-ti, meta-raspberrypi)
- Distro layer: chính sách distribution (init system, package format)
- Application layer: phần mềm riêng (meta-myapp)
- Update Yocto → chỉ cần kiểm tra layer compatibility

Tư duy giống **separation of concerns** trong software engineering.

---

## Phần 6 — Recipe: Đơn vị xây dựng phần mềm

### 6.1 Anatomy của một recipe

Một recipe file (`.bb`) gồm các phần:

```bitbake
# Metadata
SUMMARY = "Simple helloworld application"
DESCRIPTION = "A hello world program for Yocto training"
HOMEPAGE = "https://example.com"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://COPYING;md5=<hash>"

# Source
SRC_URI = "file://helloworld.c"

# Build
S = "${WORKDIR}"

do_compile() {
    ${CC} ${CFLAGS} ${LDFLAGS} -o helloworld helloworld.c
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 helloworld ${D}${bindir}
}
```

Giải thích từng phần:

| Phần | Ý nghĩa |
|------|---------|
| `SUMMARY` | Mô tả ngắn (1 dòng) |
| `LICENSE` | License của source code |
| `LIC_FILES_CHKSUM` | Checksum file license — Yocto **bắt buộc** khai báo |
| `SRC_URI` | Nguồn source: URL, git repo, hoặc file local |
| `S` | Thư mục chứa source sau khi unpack |
| `do_compile()` | Task compile — dùng `${CC}` (cross-compiler do Yocto cung cấp) |
| `do_install()` | Task install — copy binary vào `${D}` (destination) |

---

### 6.2 Biến quan trọng trong recipe

BitBake cung cấp nhiều biến đã được set sẵn:

| Biến | Giá trị | Ý nghĩa |
|------|---------|---------|
| `${CC}` | `arm-poky-linux-gnueabi-gcc ...` | Cross-compiler |
| `${CFLAGS}` | `-O2 -pipe ...` | Compiler flags |
| `${LDFLAGS}` | `--sysroot=...` | Linker flags |
| `${D}` | `.../image/` | Destination directory cho install |
| `${bindir}` | `/usr/bin` | Đường dẫn chuẩn cho binary |
| `${libdir}` | `/usr/lib` | Đường dẫn chuẩn cho library |
| `${sysconfdir}` | `/etc` | Đường dẫn chuẩn cho config |
| `${WORKDIR}` | `.../work/<arch>/<recipe>/...` | Thư mục làm việc của recipe |

> **Quy tắc:** Luôn dùng `${CC}`, `${CFLAGS}` thay vì gọi trực tiếp `gcc`. BitBake đã setup cross-compile environment — dùng biến của nó đảm bảo build đúng cho target architecture.

---

### 6.3 Các class phổ biến

Nếu source code dùng build system chuẩn (autotools, cmake, meson), bạn không cần viết `do_compile()` tay. Chỉ cần **inherit** class tương ứng:

```bitbake
# Autotools project (./configure && make && make install)
inherit autotools

# CMake project
inherit cmake

# Meson project
inherit meson

# Makefile đơn giản (chỉ có make, không có configure)
inherit module    # cho kernel module
```

Class tự động cung cấp `do_configure()`, `do_compile()`, `do_install()` phù hợp. Bạn chỉ cần khai báo `SRC_URI` và metadata.

---

## Phần 7 — Thực hành: Tạo custom layer và recipe

### 7.1 Tạo layer

```bash
# Đảm bảo đang trong build environment
cd ~/yocto
source poky/oe-init-build-env build-bbb

# Tạo layer mới
bitbake-layers create-layer ../meta-training
```

BitBake tạo cấu trúc chuẩn:

```
meta-training/
├── conf/
│   └── layer.conf
├── recipes-example/
│   └── example/
│       └── example_0.1.bb
├── COPYING.MIT
└── README
```

Thêm layer vào build:

```bash
bitbake-layers add-layer ../meta-training

# Xác nhận
bitbake-layers show-layers
```

---

### 7.2 Tạo recipe helloworld

**Bước 1: Tạo cấu trúc thư mục**

```bash
mkdir -p ../meta-training/recipes-training/helloworld/files
```

**Bước 2: Viết source code**

Tạo file `../meta-training/recipes-training/helloworld/files/helloworld.c`:

```c
#include <stdio.h>

int main(void)
{
	printf("Hello from Yocto on BeagleBone Black!\n");
	return 0;
}
```

**Bước 3: Viết recipe**

Tạo file `../meta-training/recipes-training/helloworld/helloworld_1.0.bb`:

```bitbake
SUMMARY = "Hello World application for Yocto training"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://helloworld.c"

S = "${WORKDIR}"

do_compile() {
    ${CC} ${CFLAGS} ${LDFLAGS} -o helloworld helloworld.c
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 helloworld ${D}${bindir}
}
```

> **Giải thích tên file:** `helloworld_1.0.bb`
> - `helloworld` = tên recipe (`PN` — Package Name)
> - `1.0` = version (`PV` — Package Version)
> - Convention bắt buộc: `<name>_<version>.bb`

Cấu trúc sau khi tạo xong:

```
meta-training/
├── conf/
│   └── layer.conf
└── recipes-training/
    └── helloworld/
        ├── files/
        │   └── helloworld.c
        └── helloworld_1.0.bb
```

---

### 7.3 Build và test recipe

```bash
# Build chỉ recipe helloworld
bitbake helloworld

# Kiểm tra output
ls tmp/deploy/ipk/cortexa8hf-neon/helloworld*
```

Nếu build thành công, bạn sẽ thấy file `.ipk` — đó là package sẵn sàng cài vào image.

Để đưa helloworld vào image, thêm vào `conf/local.conf`:

```bitbake
IMAGE_INSTALL:append = " helloworld"
```

> **Lưu ý cú pháp:** Có **dấu cách** trước `helloworld` trong `:append`. Đây là yêu cầu bắt buộc — `:append` nối trực tiếp string, không tự thêm space.

Sau đó build lại image:

```bash
bitbake core-image-minimal
```

Flash và boot — chạy `helloworld` trên BBB sẽ thấy output.

---

## Phần 8 — Tùy chỉnh image

### 8.1 Thêm package vào image

Có nhiều cách:

**Cách 1: Thêm trong local.conf (nhanh, tạm thời)**

```bitbake
# Thêm SSH server + package manager
IMAGE_INSTALL:append = " openssh dropbear"

# Thêm các development tool
IMAGE_INSTALL:append = " strace gdb"
```

**Cách 2: Tạo image recipe riêng (khuyến nghị cho production)**

Tạo file `../meta-training/recipes-training/images/training-image.bb`:

```bitbake
SUMMARY = "Training image for BeagleBone Black"

# Kế thừa từ core-image-minimal
require recipes-core/images/core-image-minimal.bb

# Thêm package
IMAGE_INSTALL:append = " \
    helloworld \
    openssh-sftp-server \
    i2c-tools \
    devmem2 \
"

# Cho phép root login
IMAGE_FEATURES += "debug-tweaks"
```

Build: `bitbake training-image`

---

### 8.2 Các IMAGE_FEATURES hữu ích

```bitbake
# Cho phép root login không cần password (chỉ dùng khi develop)
IMAGE_FEATURES += "debug-tweaks"

# Thêm package manager vào image (cài thêm package runtime)
IMAGE_FEATURES += "package-management"

# Thêm SSH server
IMAGE_FEATURES += "ssh-server-dropbear"
```

---

### 8.3 Sử dụng .bbappend để sửa recipe có sẵn

Khi muốn thay đổi recipe của layer khác **mà không sửa trực tiếp** layer đó:

```
meta-training/
└── recipes-core/
    └── busybox/
        └── busybox_%.bbappend    ← Apply cho mọi version busybox
```

Ví dụ, thêm config fragment cho busybox:

```bitbake
# busybox_%.bbappend
FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI += "file://my-busybox.cfg"
```

Ký tự `%` = wildcard, match mọi version. Đây là cách chuẩn để customize mà vẫn giữ layer gốc sạch.

---

## Phần 9 — Tổng kết và bài tập

### 9.1 Tổng kết

| Khái niệm | Giải thích |
|------------|-----------|
| **Yocto Project** | Framework tạo custom Linux distribution cho embedded |
| **BitBake** | Build engine — đọc recipe, resolve dependency, chạy task |
| **Poky** | Reference distribution (BitBake + OE-Core + config) |
| **Layer** | Thư mục chứa recipe/config — tách biệt chức năng |
| **Recipe (.bb)** | File mô tả cách build một phần mềm |
| **Package** | Output đã build (.rpm/.deb/.ipk) |
| **Image** | Root filesystem hoàn chỉnh = tổng hợp các package |
| **sstate-cache** | Cache kết quả build — tăng tốc build lần sau |

### 9.2 Các lệnh cần nhớ

```bash
# Khởi tạo environment
source poky/oe-init-build-env <build-dir>

# Quản lý layer
bitbake-layers show-layers              # Liệt kê layer
bitbake-layers add-layer <path>         # Thêm layer
bitbake-layers create-layer <path>      # Tạo layer mới

# Build
bitbake <target>                        # Build image hoặc recipe
bitbake <recipe> -c <task>              # Chạy một task cụ thể
bitbake <recipe> -c clean               # Xóa build output
bitbake <recipe> -c devshell            # Mở shell trong build env

# Debug
bitbake -e <recipe> | grep ^VARIABLE=   # Xem giá trị biến
bitbake -g <target>                      # Sinh dependency graph
```

---

### 9.3 Bài tập

**Lab 1: Build image cho BBB**
1. Clone Poky (branch kirkstone)
2. Cấu hình `MACHINE = "beaglebone-yocto"`
3. Build `core-image-minimal`
4. Flash lên SD card và boot BBB

**Lab 2: Tạo custom layer + recipe**
1. Tạo layer `meta-training`
2. Viết recipe `helloworld_1.0.bb` build chương trình C đơn giản
3. `bitbake helloworld` — xác nhận build thành công
4. Thêm vào image, build lại, flash và chạy trên BBB

**Lab 3: Tùy chỉnh image**
1. Tạo image recipe `training-image.bb` kế thừa từ `core-image-minimal`
2. Thêm: `openssh-sftp-server`, `i2c-tools`, `helloworld`
3. Build và flash
4. SSH vào BBB, chạy `helloworld`, dùng `i2cdetect` scan bus

**Lab 4 (nâng cao): Thêm kernel module vào image**
1. Viết recipe build kernel module (từ bài 08) bằng Yocto
2. Dùng `inherit module` thay vì compile tay
3. Thêm vào image, boot BBB, `insmod` và kiểm tra `dmesg`

---

## Tài liệu tham khảo

Tài liệu Yocto chính thức đã có trong repo:

```
docs/yocto_docs/INDEX.md          ← Điểm bắt đầu, tra cứu theo chủ đề
docs/yocto_docs/04_overview_and_concepts.md   ← Khái niệm nền tảng
docs/yocto_docs/08d_writing_recipes.md        ← Chi tiết viết recipe
docs/yocto_docs/08b_creating_layers.md        ← Chi tiết tạo layer
docs/yocto_docs/06n_variables_glossary.md     ← Tra cứu biến (~12k dòng)
```
