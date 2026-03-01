# Embedded Linux Training - AI-First

Repo chứa học liệu chương trình đào tạo Embedded Linux theo hướng AI-first.
Giảng viên: Lưu An Phú | FPT Software - Corporate Training Center.

## Cấu trúc thư mục

```
.
├── conversations/         # Log các cuộc hội thoại với Claude Code
├── docs/<NN>_<topic>/     # Học liệu từng bài (markdown, tạo bằng AI)
├── examples/<NN>_<topic>/ # Example code & bài thực hành
└── CLAUDE.md
```

Mỗi bài học gồm: file markdown học liệu trong docs/ + example code trong examples/.

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

## Ngôn ngữ

- Commit message: ưu tiên **tiếng Việt**
- Học liệu markdown: tiếng Việt xen tiếng Anh (thuật ngữ kỹ thuật giữ nguyên tiếng Anh)
- Code comment: tiếng Anh
