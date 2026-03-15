# Kỹ thuật Debugging trên Linux
**Khóa học**: Lập trình trên Linux — 60 giờ / 20 buổi  
**Tác giả**: Lưu An Phú — CEO Vinalinux  
**Thời lượng**: 3 giờ  
**Đối tượng**: Embedded MCU developers chuyển sang Linux

---

## Phần 1 — Pain point & Tư duy Debugging (15 phút)

### 1.1 Embedded developer debug kiểu gì?

Trên MCU, việc debug thường không quá khó:

- **IDE có sẵn hỗ trợ JTAG**: STM32CubeIDE, MPLAB X, Keil MDK — cắm J-Link hay ST-Link vào là debug được ngay, không cần setup thêm gì
- **Code đơn giản, một luồng**: toàn bộ trạng thái hệ thống nằm trong vài KB RAM, quan sát được hết — nên printf cũng đủ dùng trong nhiều trường hợp

Khi chuyển sang Linux, cả hai lợi thế này đều mất:

- **Không có IDE nào hỗ trợ sẵn** — muốn debug phải tự setup gdbserver, cấu hình launch.json, kết nối remote
- **Môi trường phức tạp hơn nhiều** — nhiều process, nhiều thread, kernel xen vào giữa — sinh ra các lớp lỗi mà printf không thể bắt được

Ví dụ điển hình: **deadlock**. Hai thread chờ nhau mãi, chương trình không crash, không có output gì — printf hoàn toàn bất lực. Không biết thread nào đang chờ cái gì.

---

### 1.2 Khi lên Linux, mọi thứ phức tạp hơn

Trên Linux, chương trình không còn chạy một mình:

- **Nhiều process, nhiều thread** chạy song song
- **Kernel quản lý tài nguyên** — memory, file, network đều qua syscall
- **Virtual memory**: địa chỉ trong code không phải địa chỉ vật lý
- **Shared library**: lỗi có thể nằm trong `.so` mà không có source

Printf vẫn dùng được, nhưng với một race condition hay memory corruption xảy ra ở thread khác — printf không giúp được gì.

---

### 1.3 Tư duy mới: "Hỏi hệ thống"

Linux cung cấp rất nhiều cơ chế để quan sát từ bên ngoài mà **không cần sửa code**:

| Muốn biết gì | Công cụ |
|---|---|
| Process đang gọi syscall nào? | `strace` |
| Có memory leak không? | `valgrind` |
| Race condition ở đâu? | `helgrind`, `tsan` |
| Crash ở dòng code nào? | `gdb` + debug symbol |
| Binary này được build thế nào? | `nm`, `objdump`, `file` |

> **Nguyên tắc cốt lõi**: Thay vì đoán lỗi rồi sửa code thử, hãy *đặt câu hỏi đúng cho hệ thống* — hệ thống sẽ trả lời chính xác.

Đây là sự thay đổi tư duy quan trọng nhất khi chuyển từ MCU sang Linux.

---

## Phần 2 — Debug Symbol & Build System (20 phút)

### 2.1 Debug symbol là gì?

Khi GCC biên dịch một file `.c`, output mặc định là **binary thuần** — chỉ chứa machine code. CPU chạy được, nhưng debugger không biết:

- Dòng code này tương ứng với file `.c` nào, dòng số mấy?
- Biến `count` nằm ở địa chỉ nào trong memory?
- Function `process_data()` bắt đầu ở offset nào?

**Debug symbol** là phần metadata bổ sung được nhúng vào binary, trả lời đúng các câu hỏi trên. Khi có symbol, debugger mới có thể hiện đúng tên biến, tên hàm, số dòng.

Analogy: binary không có symbol như một cuốn sách đã xé hết mục lục và đánh số trang — nội dung vẫn còn đó, nhưng không thể tra cứu được.

---

### 2.2 Compile flag: `-g` và trade-off với optimization

Để nhúng debug symbol vào binary, thêm flag `-g` khi compile:

```bash
gcc -g -O0 -o my_app main.c
```

**Expected output khi kiểm tra:**
```bash
$ file my_app
my_app: ELF 64-bit LSB executable, not stripped
```

Trade-off cần biết:

- `-g`: nhúng debug symbol — binary to hơn, nhưng **tốc độ runtime không đổi**
- `-O0`: tắt optimization — code chạy chậm hơn, nhưng debugger hiển thị đúng thứ tự thực thi
- `-O2`: bật optimization — compiler có thể inline function, xóa biến, đảo thứ tự lệnh → debugger nhảy lung tung, biến hiện `<optimized out>`

> **Thực tế**: build debug dùng `-g -O0`, build release dùng `-O2`. Không bao giờ debug binary release trừ khi bất đắc dĩ.

---

### 2.3 Heisenbug — khi lỗi biến mất lúc debug

Đây là pitfall kinh điển: **một số lỗi chỉ xuất hiện ở release build, biến mất khi build debug**. Hiện tượng này có tên riêng — **Heisenbug** — lỗi thay đổi hành vi khi bị quan sát.

Nguyên nhân thường gặp:

**Memory layout thay đổi** — `-O0` làm stack frame lớn hơn, biến được cấp phát theo thứ tự khác. Một buffer overflow có thể ghi đè vùng nhớ "vô hại" ở debug build, nhưng ghi đè đúng return address ở release → crash.

**Timing thay đổi** — `-O0` chạy chậm hơn đáng kể. Race condition phụ thuộc vào timing giữa các thread — khi tốc độ thay đổi, window để race xảy ra cũng thay đổi, lỗi không tái hiện. Tương tự, khi attach GDB và dừng tại breakpoint, timing của toàn bộ hệ thống bị xáo trộn.

**Optimization che giấu lỗi logic** — biến shared giữa các thread không có `volatile` hay `atomic`: ở `-O0` compiler đọc lại từ memory mỗi lần nên chạy đúng, ở `-O2` compiler cache giá trị vào register → đọc giá trị cũ mãi.

**Heap allocator behavior** — debug build thường dùng malloc có guard/padding. Một số use-after-free hay heap corruption không trigger ngay ở debug vì padding che buffer, nhưng crash ở release.

> **Hệ quả thực tế**: Khi lỗi chỉ xảy ra ở release build, **không nên** cố debug bằng cách thêm printf hay rebuild với `-O0`. Thay vào đó dùng **ASan, Valgrind** — các tool này phân tích mà không thay đổi bản chất lỗi.

---

### 2.4 DWARF format

Debug symbol trên Linux được lưu theo chuẩn **DWARF** (Debugging With Attributed Record Formats), nhúng trực tiếp vào các section của ELF binary:

- `.debug_info` — kiểu dữ liệu, tên biến, tên hàm
- `.debug_line` — mapping giữa địa chỉ machine code ↔ dòng source
- `.debug_frame` — thông tin call stack

Không cần nhớ chi tiết format. Điều quan trọng: **DWARF nằm trong ELF, không phải file riêng** — nên khi `strip` binary thì mất luôn.

---

### 2.5 Kiểm tra binary bằng `nm`, `objdump`, `file`

Ba tool nhanh để kiểm tra tình trạng binary trước khi debug:

**`file`** — kiểm tra binary có bị strip chưa:
```bash
$ file my_app
my_app: ELF 64-bit LSB executable, not stripped
```

**`nm`** — liệt kê symbol table:
```bash
$ nm my_app | grep process_data
00000000004011a0 T process_data
```
Nếu binary bị strip: `nm: my_app: no symbols`

**`objdump`** — xem DWARF section có tồn tại không:
```bash
$ objdump --section-headers my_app | grep debug
  27 .debug_info   000123ab  ...
```
Có section `.debug_*` → symbol còn nguyên.

---

## Phần 3 — Kiến trúc Debugger: Frontend / Backend (25 phút)

### 3.1 Tại sao debugger cần tách Frontend / Backend?

Hầu hết developer nghĩ GDB là một khối duy nhất. Thực tế không phải vậy.

Một debugger cần làm hai việc rất khác nhau:

- **Quan sát và điều khiển process** — đặt breakpoint, đọc memory, dừng/tiếp tục execution. Việc này phụ thuộc vào OS và kiến trúc CPU.
- **Hiển thị thông tin cho người dùng** — show source code, tên biến, call stack. Việc này phụ thuộc vào giao diện (CLI, GUI, IDE).

Nếu gộp chung, mỗi IDE muốn hỗ trợ một kiến trúc mới phải viết lại từ đầu. Giải pháp: **tách làm hai tầng độc lập**.

```
[Frontend]  VS Code / CLion / GDB CLI
                    ↕ GDB MI protocol
[Backend]   GDB
                    ↕ ptrace syscall
[Target]    Process đang bị debug
```

---

### 3.2 ptrace — cơ chế kernel cho phép debugger hoạt động

`ptrace` là syscall của Linux kernel, cho phép một process (debugger) **quan sát và điều khiển** một process khác (tracee):

- Đọc/ghi memory của tracee
- Đọc/ghi register của tracee
- Nhận notification khi tracee nhận signal hoặc thực thi syscall
- Dừng và tiếp tục execution của tracee

Khi GDB attach vào một process, thực chất là GDB gọi `ptrace(PTRACE_ATTACH, pid, ...)` — kernel sẽ gửi `SIGSTOP` vào process đó và trao quyền kiểm soát cho GDB.

> **Lưu ý quan trọng**: `ptrace` chỉ hoạt động nếu debugger có đủ quyền. Trên nhiều hệ thống production, `ptrace` bị giới hạn bởi `/proc/sys/kernel/yama/ptrace_scope` — đây là lý do tại sao `gdb --pid` đôi khi bị từ chối dù chạy cùng user.

---

### 3.3 Breakpoint hoạt động thế nào?

Có hai loại breakpoint, cơ chế hoàn toàn khác nhau:

**Software breakpoint**

GDB ghi đè byte đầu tiên của instruction tại địa chỉ cần dừng bằng opcode `0xCC` (INT3 trên x86). Khi CPU thực thi đến đó, sinh ra exception `SIGTRAP`, kernel chuyển signal về GDB, GDB dừng process lại và restore byte gốc.

```
Trước breakpoint:    55 89 e5 83 ec 10  ...   (push rbp; mov rbp,rsp)
Sau khi GDB đặt:     CC 89 e5 83 ec 10  ...   (INT3; mov rbp,rsp)
```

**Hardware breakpoint**

CPU có các thanh ghi đặc biệt (DR0–DR3 trên x86) — ghi địa chỉ vào đó, CPU tự sinh exception khi execution đến địa chỉ đó mà không cần sửa memory. Số lượng giới hạn (thường 4 breakpoint). Dùng cho **watchpoint** — dừng khi một vùng memory bị đọc hoặc ghi.

> **Hệ quả**: Software breakpoint không dùng được trên ROM hay flash memory vì không thể ghi đè. Trên embedded target, hardware breakpoint là lựa chọn duy nhất cho những vùng này.

---

### 3.4 GDB MI Protocol — nền tảng để IDE kết nối GDB

GDB có hai chế độ giao tiếp:

- **GDB CLI**: giao diện text cho người dùng gõ lệnh trực tiếp
- **GDB MI** (Machine Interface): giao tiếp dạng structured text, thiết kế cho **program-to-program communication**

Khi VS Code debug một chương trình, thực chất VS Code spawn một process GDB ở background và giao tiếp qua GDB MI:

```
VS Code gửi:   -exec-continue
GDB trả về:    *stopped,reason="breakpoint-hit",bkptno="1",
               frame={addr="0x4011a0",func="main",line="10"}
```

Tầng trên GDB MI là **DAP — Debug Adapter Protocol** (do Microsoft định nghĩa): chuẩn hóa giao tiếp giữa IDE và bất kỳ debugger nào. VS Code không nói chuyện trực tiếp với GDB mà qua một **Debug Adapter** đóng vai trò phiên dịch.

```
VS Code ←→ DAP ←→ Debug Adapter ←→ GDB MI ←→ GDB ←→ ptrace ←→ Process
```

Đây là lý do VS Code có thể debug Python, Rust, Go, C++ với cùng một giao diện — mỗi ngôn ngữ chỉ cần viết một Debug Adapter riêng.

---

## Phần 4 — Runtime Analysis Tools (20 phút)

### 4.1 Tại sao cần tool phân tích runtime?

Có một nhóm lỗi mà GDB không giúp được nhiều: chương trình chạy **không crash ngay**, không có error message, nhưng hành vi sai hoặc memory cứ tăng dần theo thời gian. Ví dụ:

- Buffer overflow ghi đè vùng nhớ khác — chương trình chạy tiếp nhưng data bị corrupt
- Memory leak nhỏ — chạy vài giờ mới thấy RAM tăng bất thường
- Race condition — thỉnh thoảng mới xuất hiện, không tái hiện được

Nhóm tool phân tích runtime giải quyết đúng các lỗi này bằng cách **quan sát chương trình trong lúc chạy** — không cần biết lỗi ở đâu trước.

---

### 4.2 Instrumentation vs Simulation — hai cách tiếp cận

Trước khi đi vào từng tool, cần hiểu hai triết lý khác nhau:

**Instrumentation** — compiler chèn thêm code kiểm tra vào binary lúc build. Chương trình chạy với code gốc + code kiểm tra cùng lúc. Tốc độ chậm hơn một chút nhưng vẫn dùng được.

**Simulation** — tool tạo ra một môi trường ảo, chạy binary gốc bên trong đó và quan sát từng thao tác memory. Không cần recompile, nhưng chậm hơn rất nhiều vì mọi instruction đều phải đi qua lớp simulation.

```
Instrumentation (ASan, TSan):
  [Binary gốc + code kiểm tra] chạy trực tiếp trên CPU
  → Chậm hơn 2–5x

Simulation (Valgrind):
  [Binary gốc] chạy bên trong Valgrind virtual machine
  → Chậm hơn 10–50x
```

---

### 4.3 AddressSanitizer (ASan) — phát hiện memory error

ASan là công cụ **instrumentation** do Google phát triển, tích hợp sẵn trong GCC và Clang. Phát hiện:

- Buffer overflow (stack và heap)
- Use-after-free
- Use-after-return
- Double free

Cách dùng: chỉ cần thêm flag khi compile, không cần thay đổi source:

```bash
gcc -fsanitize=address -g -o main main.c
./main
```

ASan sẽ in ra báo lỗi chi tiết kèm stack trace ngay khi lỗi xảy ra — chỉ ra đúng dòng code gây ra vấn đề.

> **Giới hạn**: ASan cần recompile. Không dùng được với binary không có source.

---

### 4.4 Valgrind / Memcheck — memory leak và uninitialized memory

Valgrind là framework **simulation**. Tool mặc định bên trong Valgrind là **Memcheck**, phát hiện:

- Memory leak — malloc mà không free
- Đọc/ghi vùng nhớ chưa khởi tạo
- Đọc/ghi ngoài vùng nhớ được cấp phát
- Double free

Cách dùng: không cần recompile, chạy thẳng binary:

```bash
valgrind --leak-check=full ./main
```

> **Tại sao dùng Valgrind thay vì ASan cho memory leak?** Valgrind không cần source code hay recompile — dùng được với binary third-party. Đây là lý do Valgrind vẫn là công cụ **industry-standard** dù chậm hơn ASan.

---

### 4.5 Helgrind / ThreadSanitizer (TSan) — race condition

Hai tool phát hiện lỗi liên quan đến multi-threading:

**Helgrind** (thuộc Valgrind): phát hiện race condition bằng cách theo dõi mọi thao tác đọc/ghi vào shared memory và kiểm tra lock discipline.

```bash
valgrind --tool=helgrind ./main
```

**ThreadSanitizer (TSan)**: tương tự Helgrind nhưng dùng instrumentation — nhanh hơn đáng kể. Tích hợp sẵn trong GCC/Clang:

```bash
gcc -fsanitize=thread -g -o main main.c
./main
```

> **Pitfall thực tế**: TSan chạy nhanh hơn Helgrind, nhưng đôi khi chính vì nhanh hơn mà **timing thay đổi** — race condition không tái hiện được. Khi TSan không bắt được, thử lại với Helgrind.

---

### 4.6 Khi nào dùng tool nào?

| Triệu chứng | Tool nên dùng |
|---|---|
| Crash, buffer overflow, use-after-free | ASan |
| Memory leak, uninitialized read | Valgrind / Memcheck |
| Race condition, deadlock | Helgrind hoặc TSan |
| Không có source code | Valgrind (không cần recompile) |
| Cần chạy nhanh, có source | ASan hoặc TSan |

---

## Phần 5 — Remote Debugging: gdbserver (25 phút)

### 5.1 Tại sao cần remote debug?

Trên MCU, JTAG debugger kết nối trực tiếp vào chip — hoàn toàn tự nhiên vì chip không có OS, không có network. Khi lên Linux embedded (BeagleBone, Raspberry Pi, custom board), tình huống thay đổi:

- **Target có thể không có màn hình, bàn phím** — không thể chạy GDB GUI trực tiếp trên board
- **Target có CPU yếu, RAM ít** — GDB đầy đủ tốn tài nguyên, không phù hợp chạy trên target
- **Developer làm việc trên host** — source code, IDE, công cụ đều trên máy host

Giải pháp: **tách GDB làm hai phần**, chạy trên hai máy khác nhau:

```
[Host - máy developer]          [Target - embedded board]
  GDB (full)              ←TCP→   gdbserver (nhỏ gọn)
  VS Code / GDB CLI               BeagleBone Black
  Source code, symbols            Binary cần debug
```

---

### 5.2 gdbserver là gì?

`gdbserver` là một binary **rất nhỏ** (vài trăm KB) chạy trên target. Nó:

- Nhận kết nối từ GDB trên host qua TCP hoặc serial
- Dùng `ptrace` để điều khiển process cần debug trên target
- Chuyển tiếp lệnh và data giữa GDB host ↔ process trên target

`gdbserver` **không cần debug symbol** — symbol nằm trên host, GDB host tự xử lý. Target chỉ cần chạy binary và gdbserver.

---

### 5.3 Quy trình kết nối

**Bước 1 — Trên target (BeagleBone Black):**
```bash
gdbserver :2345 ./my_app
```
Output:
```
Process ./my_app created; pid = 1234
Listening on port 2345
```

**Bước 2 — Trên host, khởi động GDB với binary có symbol:**
```bash
gdb-multiarch ./my_app
```

**Bước 3 — Trong GDB CLI, kết nối vào target:**
```bash
(gdb) target remote 192.168.1.100:2345
(gdb) break main
(gdb) continue
```

GDB host sẽ dừng tại `main()` trên target — có thể đặt breakpoint, xem biến, step từng dòng như debug local.

---

### 5.4 Cross-debug: host x86 debug binary ARM

Khi host là máy x86 và target là ARM (BeagleBone Black), cần dùng **cross GDB** — GDB được build để chạy trên x86 nhưng hiểu binary ARM:

```bash
# Trên Ubuntu host
sudo apt install gdb-multiarch

# Chạy với chỉ định kiến trúc target
gdb-multiarch ./my_app_arm
(gdb) set architecture arm
(gdb) target remote 192.168.1.100:2345
```

> **Lưu ý**: binary `my_app` trên target được cross-compile từ host — cùng source, cùng compiler flag `-g`. GDB host dùng binary này để đọc symbol, không phải để chạy.

---

### 5.5 Attach vào process đang chạy

Không nhất thiết phải khởi động process qua gdbserver. Có thể attach vào process đang chạy:

```bash
# Trên target — attach vào PID đang chạy
gdbserver :2345 --attach <pid>
```

Hữu ích khi: process đã chạy từ trước (daemon, service), lỗi xảy ra sau một thời gian dài chạy, không muốn restart từ đầu.

---

## Phần 6 — Remote Debugging với VS Code (20 phút)

### 6.1 Từ GDB CLI đến VS Code

Phần trước đã biết cách dùng GDB CLI kết nối vào gdbserver. VS Code không thay thế GDB — nó chỉ là **frontend** đẹp hơn, ngồi phía trên đúng stack đã học:

```
VS Code UI
    ↕ DAP (Debug Adapter Protocol)
cortex-debug / cppdbg extension
    ↕ GDB MI Protocol
gdb-multiarch (trên host)
    ↕ TCP
gdbserver (trên target)
    ↕ ptrace
Process đang debug
```

Mọi thứ bên dưới VS Code hoàn toàn giống Phần 5 — chỉ thêm hai tầng ở trên cùng.

---

### 6.2 Cài đặt extension

Trên VS Code host cần cài extension **C/C++** của Microsoft (ms-vscode.cpptools) — extension này đóng vai trò Debug Adapter, phiên dịch giữa DAP và GDB MI.

```
Extensions cần thiết:
- C/C++ (ms-vscode.cpptools)   ← bắt buộc
- cortex-debug                 ← nếu target là MCU có JTAG
```

---

### 6.3 Cấu hình launch.json

Toàn bộ cấu hình remote debug nằm trong file `.vscode/launch.json`:

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Remote Debug BeagleBone",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/my_app",
            "miDebuggerPath": "/usr/bin/gdb-multiarch",
            "miDebuggerServerAddress": "192.168.1.100:2345",
            "cwd": "${workspaceFolder}",
            "stopAtEntry": true
        }
    ]
}
```

Các field quan trọng:

- **`program`**: binary có debug symbol trên host — để GDB đọc symbol
- **`miDebuggerPath`**: đường dẫn đến gdb-multiarch trên host
- **`miDebuggerServerAddress`**: IP và port của gdbserver trên target
- **`stopAtEntry`**: dừng tại `main()` ngay khi kết nối

---

### 6.4 Quy trình debug hoàn chỉnh

**Bước 1 — Trên target, khởi động gdbserver:**
```bash
gdbserver :2345 ./my_app
```

**Bước 2 — Trên VS Code host**, nhấn `F5` hoặc vào Run → Start Debugging.

VS Code sẽ:
1. Spawn `gdb-multiarch` ở background
2. Load symbol từ binary trên host
3. Kết nối TCP vào gdbserver trên target
4. Dừng tại `main()` — hiển thị source code, call stack, biến local

**Từ đây có thể dùng đầy đủ:**
- **Breakpoint**: click vào lề trái của source file
- **Watch**: thêm biến vào Watch panel để theo dõi giá trị
- **Call stack**: xem toàn bộ call stack tại điểm dừng
- **Step over / Step into / Continue**: F10, F11, F5

---

### 6.5 So sánh GDB CLI vs VS Code

| | GDB CLI | VS Code |
|---|---|---|
| Cần cài thêm | Không | Extension cpptools |
| Xem source | Có (text) | Có (highlight) |
| Breakpoint | Lệnh `break` | Click chuột |
| Watch variable | Lệnh `print`, `display` | Panel trực quan |
| Phù hợp khi | SSH vào target, không có GUI | Máy host có GUI |

> **Thực tế**: GDB CLI vẫn cần biết — khi SSH vào server không có GUI, hay khi cần script automation. VS Code dùng cho workflow hàng ngày khi có máy host đầy đủ.

---

## Phần 7 — Lab thực hành (30 phút)

### Lab 1 — Inspect binary: stripped vs unstripped (10 phút)

**Mục tiêu**: quan sát sự khác biệt giữa binary có và không có debug symbol.

**Code:**
```c
// main.c
#include <stdio.h>

void process_data(int x) {
    printf("Processing: %d\n", x * 2);
}

int main() {
    process_data(42);
    return 0;
}
```

**Thực hành:**
```bash
# Build với debug symbol
gcc -g -o main_debug main.c

# Build stripped
gcc -o main_release main.c
strip main_release

# So sánh
file main_debug
file main_release

nm main_debug | grep process_data
nm main_release | grep process_data

objdump --section-headers main_debug | grep debug
objdump --section-headers main_release | grep debug
```

**Expected output:**
```
main_debug:   ELF 64-bit LSB executable, not stripped
main_release: ELF 64-bit LSB executable, stripped

000000000040116e T process_data   ← có symbol
nm: main_release: no symbols      ← stripped, mất hết

27 .debug_info  000123ab ...      ← section còn trong debug
                                  ← không có dòng nào trong release
```

**Câu hỏi thảo luận**: Nếu ship binary release lên target mà không giữ lại bản debug, khi có bug ngoài thực tế thì làm thế nào?

---

### Lab 2 — Remote debug BeagleBone Black bằng VS Code (10 phút)

**Mục tiêu**: thiết lập toàn bộ pipeline remote debug từ VS Code host đến BeagleBone Black.

**Chuẩn bị trên host:**
```bash
# Cross-compile binary với debug symbol
arm-linux-gnueabihf-gcc -g -o my_app main.c

# Copy binary lên target
scp my_app debian@192.168.1.100:~/
```

**Trên target (BeagleBone Black):**
```bash
gdbserver :2345 ./my_app
```

**Trên host — tạo `.vscode/launch.json`:**
```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Remote Debug BeagleBone",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/my_app",
            "miDebuggerPath": "/usr/bin/gdb-multiarch",
            "miDebuggerServerAddress": "192.168.1.100:2345",
            "cwd": "${workspaceFolder}",
            "stopAtEntry": true
        }
    ]
}
```

**Nhấn F5** — VS Code dừng tại `main()` trên BeagleBone.

**Học viên tự thực hành:**
- Đặt breakpoint tại `process_data()`
- Xem giá trị biến `x` trong Watch panel
- Step từng dòng qua F10

---

### Lab 3 — Phát hiện bug bằng ASan và Valgrind (10 phút)

**Mục tiêu**: chạy ASan và Valgrind trên code có bug cố ý, đọc output và xác định lỗi.

**Code có bug:**
```c
// buggy.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bug_overflow() {
    char buf[8];
    strcpy(buf, "hello_overflow_bug");  // ghi 18 bytes vào buffer 8 bytes
    printf("%s\n", buf);
}

void bug_leak() {
    char *p = malloc(64);
    strcpy(p, "this memory is never freed");
    printf("%s\n", p);
    // quên free(p)
}

int main() {
    bug_overflow();
    bug_leak();
    return 0;
}
```

**Chạy với ASan:**
```bash
gcc -fsanitize=address -g -o buggy buggy.c
./buggy
```

**Expected output ASan:**
```
==1234==ERROR: AddressSanitizer: stack-buffer-overflow
WRITE of size 18 at 0x... thread T0
    #0 0x... in bug_overflow buggy.c:7
    #1 0x... in main buggy.c:16
```

**Chạy với Valgrind:**
```bash
gcc -g -o buggy buggy.c
valgrind --leak-check=full ./buggy
```

**Expected output Valgrind:**
```
LEAK SUMMARY:
   definitely lost: 64 bytes in 1 blocks
      at 0x...: malloc (vg_replace_malloc.c)
      by 0x...: bug_leak (buggy.c:12)
      by 0x...: main (buggy.c:17)
```

**Câu hỏi thảo luận**: ASan báo lỗi overflow nhưng không báo leak — tại sao? *(gợi ý: chương trình crash trước khi đến bug_leak)*

---

## Phần 8 — Tổng kết (15 phút)

### 8.1 Bức tranh toàn cảnh

Nhìn lại toàn bộ buổi học, tất cả các tool và kỹ thuật đều xoay quanh một câu hỏi duy nhất: **"Làm thế nào để quan sát chương trình đang chạy mà không phá vỡ nó?"**

Mỗi tầng trong hệ thống cung cấp một góc nhìn khác nhau:

```
[Source code]         ← Debug symbol (DWARF) kết nối tầng này
[Binary / ELF]        ← nm, objdump, file đọc tầng này
[Process runtime]     ← GDB / ptrace điều khiển tầng này
[Memory]              ← ASan, Valgrind quan sát tầng này
[Threads]             ← Helgrind, TSan phát hiện lỗi tầng này
[Syscall boundary]    ← strace quan sát tầng này (advanced)
```

---

### 8.2 Decision tree — tình huống nào dùng tool nào?

```
Chương trình có vấn đề
│
├─ Crash / hành vi sai rõ ràng
│   ├─ Có source code → GDB + debug symbol
│   └─ Không có source → core dump + gdb-multiarch
│
├─ Memory tăng dần, nghi memory leak
│   ├─ Có source → Valgrind --leak-check=full
│   └─ Không có source → Valgrind (không cần recompile)
│
├─ Crash kiểu buffer overflow, use-after-free
│   └─ Có source → ASan (-fsanitize=address)
│
├─ Race condition, deadlock nghi ngờ
│   ├─ Thử nhanh → TSan (-fsanitize=thread)
│   └─ TSan không bắt được → Helgrind
│
└─ Debug trên embedded target
    ├─ Có GUI trên host → VS Code + gdbserver
    └─ Chỉ có terminal → GDB CLI + gdbserver
```

---

### 8.3 Nguyên tắc cốt lõi cần nhớ

**1. Debug symbol không ảnh hưởng tốc độ runtime** — `-g` chỉ làm binary to hơn, không làm chương trình chậm hơn. Luôn build với `-g` trong môi trường development.

**2. Heisenbug là có thật** — khi lỗi chỉ xuất hiện ở release build, đừng cố rebuild với `-O0`. Dùng ASan hoặc Valgrind để tìm lỗi mà không thay đổi bản chất của nó.

**3. gdbserver là cầu nối thiết yếu** — workflow remote debug (gdbserver + GDB host) là chuẩn công nghiệp cho embedded Linux. VS Code chỉ là frontend, bên dưới vẫn là GDB.

**4. Chọn tool theo triệu chứng, không theo thói quen** — printf vẫn hữu ích, nhưng deadlock, race condition, memory corruption cần đúng tool mới giải quyết được.

---

### 8.4 Câu hỏi ôn tập

1. Tại sao binary bị `strip` vẫn chạy được nhưng không debug được?
2. Breakpoint hoạt động thế nào ở mức hardware? Tại sao software breakpoint không dùng được trên ROM?
3. Heisenbug là gì? Kể một tình huống có thể gặp trong project thực tế.
4. Khi target là BeagleBone Black và host là laptop x86, tại sao cần `gdb-multiarch` thay vì `gdb` thông thường?
5. ASan và Valgrind đều phát hiện memory error — khi nào chọn Valgrind thay vì ASan?
