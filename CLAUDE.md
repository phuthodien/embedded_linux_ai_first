# Embedded Linux Training - AI-First

Repo chứa học liệu chương trình đào tạo Embedded Linux theo hướng AI-first.
Giảng viên: Lưu An Phú | FPT Software - Corporate Training Center.

## Cấu trúc thư mục

```
.
├── conversations/                  # Log các cuộc hội thoại với Claude Code
├── docs/
│   ├── lessons/<NN>_<topic>/       # Học liệu từng bài (markdown, tạo bằng AI)
│   ├── linux_programming_interface_book/  # Sách TLPI tham khảo
│   ├── linux_device_drivers_3rd_book/     # Sách LDD3 (Corbet, Rubini, Kroah-Hartman)
│   ├── essential_linux_device_drivers_book/ # Sách ELDD (Venkateswaran)
│   └── beaglebone_black_docs/
│       └── hw-docs/                # Tài liệu phần cứng BBB (xem index.md bên trong)
├── examples/
│   ├── lessons/<NN>_<topic>/       # Example code & bài thực hành
│   └── projects/                   # Sample projects hoàn chỉnh (source + docs)
│       └── mini_edr/               # Mini EDR agent (kprobe + netlink)
└── CLAUDE.md
```

Mỗi bài học gồm: file markdown học liệu trong `docs/lessons/` + example code trong `examples/lessons/`.

## Conversation logs

- Mọi cuộc hội thoại với Claude Code được lưu trong `conversations/`
- Cuối mỗi cuộc hội thoại, tạo file log tóm tắt nội dung trao đổi và kết quả đạt được
- Đặt tên file: `YYYY-MM-DD_<chủ-đề-ngắn>.md`

## Target board

- Board: **BeagleBone Black**
- IP: `192.168.137.2`
- SSH/SCP: `ssh debian@192.168.137.2` / `scp <file> debian@192.168.137.2:/home/debian/`
- Thư mục làm việc trên target: `/home/debian`

## Coding convention

Source code C tuân theo **Linux kernel coding style**:
- Indent bằng tab, tab width = 8
- Mở ngoặc nhọn `{` cùng dòng với statement (trừ function definition)
- Tên biến, hàm: `snake_case`, viết thường
- Tên macro, constant: `UPPER_CASE`
- Dòng không quá 80 ký tự (linh hoạt đến 100 nếu cần)
- Không dùng `typedef` che giấu struct/pointer trừ khi có lý do rõ ràng
- Comment dùng `/* */` cho block, `//` cho inline ngắn

Tham khảo: https://www.kernel.org/doc/html/latest/process/coding-style.html

## Build

- **"compile"** = dùng `gcc` của host (chạy trên máy host)
- **"cross compile"** = dùng toolchain BBB (chạy trên board)

BBB toolchain: `/home/phula/work/bbb/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf`

```bash
# Compile cho host
gcc -g -O0 -Wall -Wextra -std=c99 -o <output> <source>.c

# Cross compile cho BBB
/home/phula/work/bbb/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-gcc -g -O0 -Wall -Wextra -std=c99 -o <output> <source>.c

# Compile với Address Sanitizer (host)
gcc -g -O1 -fsanitize=address -fno-omit-frame-pointer -o <output> <source>.c

# Kiểm tra memory leak (host)
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./<program>
```

## BBB Kernel Source

- Source code: `/home/phula/work/bbb/kernelbuildscripts`
- Build script: `/home/phula/work/bbb/kernelbuildscripts/tools/rebuild.sh`

## U-Boot Source

- Source code: `/home/phula/work/bbb/u-boot`
- Build script: `/home/phula/work/bbb/u-boot/build-am335x.sh`
- Defconfig cho BBB: `am335x_evm_defconfig`
- Output: `MLO` (SPL) và `u-boot.img`

Toolchain để build U-Boot:
- Path: `/home/phula/work/bbb/gcc-11.3.0-nolibc/arm-linux-gnueabi/bin/`
- Prefix: `arm-linux-gnueabi-`

```bash
cd /home/phula/work/bbb/u-boot
export CROSS_COMPILE=/home/phula/work/bbb/gcc-11.3.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
make ARCH=arm am335x_evm_defconfig
make ARCH=arm -j$(nproc)
# Output: MLO (104KB) và u-boot.img (968KB)
```

## Tài liệu tham khảo

> **Quy tắc ưu tiên**: Khi cần thông tin về BeagleBone Black, SoC AM335x, hoặc Linux programming,
> **luôn tìm trong repo trước** (hw-docs, reference manual, TLPI book). Tài liệu trong repo đã được
> kiểm chứng và chuẩn hơn nguồn trên mạng. Chỉ dùng kiến thức bên ngoài khi repo không cover.

### Hardware docs (BeagleBone Black)

Khi cần tra cứu về **phần cứng BBB** (register, pin mux, GPIO, timer, clock, boot process, schematic, device tree), đọc:

```
docs/beaglebone_black_docs/hw-docs/index.md
```

File index mô tả mục đích từng tài liệu và workflow tra cứu. Đọc index trước, sau đó đọc file cụ thể.

### Linux Programming Interface

Khi người dùng hỏi về lý thuyết hệ điều hành ở tầng user-space (system calls, file I/O, processes, threads, signals, IPC, sockets, memory management, scheduling, ...), **ưu tiên tra cứu** trong:

```
docs/linux_programming_interface_book/chapters/
```

Đây là sách *The Linux Programming Interface* (Michael Kerrisk) đã split thành 64 chapter.
- File index: `docs/linux_programming_interface_book/chapters/INDEX.md`
- Đọc INDEX.md để xác định chapter phù hợp, sau đó đọc chapter đó để trả lời.
- Chỉ dùng kiến thức bên ngoài khi sách không cover chủ đề được hỏi.

### Linux Device Drivers, 3rd Edition (LDD3)

Khi người dùng hỏi về **Linux kernel module, device driver cơ bản** (char driver, block driver, network driver, PCI, USB, memory mapping, DMA, concurrency, interrupt handling, device model, debugging), **ưu tiên tra cứu** trong:

```
docs/linux_device_drivers_3rd_book/chapters/
```

Đây là sách *Linux Device Drivers, 3rd Edition* (Corbet, Rubini, Kroah-Hartman) đã split thành 18 chapter.
- File index: `docs/linux_device_drivers_3rd_book/chapters/INDEX.md`
- Đọc INDEX.md để xác định chapter phù hợp, sau đó đọc chapter đó để trả lời.
- Chỉ dùng kiến thức bên ngoài khi sách không cover chủ đề được hỏi.

### Essential Linux Device Drivers

Khi người dùng hỏi về **device driver cho các bus/thiết bị cụ thể** (serial/UART, input subsystem, I2C, SPI, PCMCIA, PCI, USB, video/framebuffer, MTD/flash, embedded Linux, user-space drivers, driver debugging), **ưu tiên tra cứu** trong:

```
docs/essential_linux_device_drivers_book/chapters/
```

Đây là sách *Essential Linux Device Drivers* (Sreekrishnan Venkateswaran) đã split thành 19 chapter (ch01-12, ch17-23).
- File index: `docs/essential_linux_device_drivers_book/chapters/INDEX.md`
- Đọc INDEX.md để xác định chapter phù hợp, sau đó đọc chapter đó để trả lời.
- Chỉ dùng kiến thức bên ngoài khi sách không cover chủ đề được hỏi.

## Ngôn ngữ

- Commit message: ưu tiên **tiếng Việt**
- Học liệu markdown: tiếng Việt xen tiếng Anh (thuật ngữ kỹ thuật giữ nguyên tiếng Anh)
- Code comment: tiếng Anh
