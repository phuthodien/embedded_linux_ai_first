# Chapter 6: AM335x Interrupt Controller (INTC)

## 6.1 Mô tả chức năng

Interrupt Controller xử lý các interrupt đến bằng cách thực hiện masking và priority sorting để tạo ra các interrupt signal cho processor.

**LƯU Ý:** FIQ không khả dụng trên các thiết bị General-Purpose (GP).

### 6.1.1 Xử lý Interrupt

#### 6.1.1.1 Input Selection

INTC chỉ hỗ trợ phát hiện interrupt đến ở mức level-sensitive. Một peripheral muốn assert interrupt phải duy trì interrupt line ở mức high cho đến khi interrupt được xử lý.

**Software Interrupt:** Một software interrupt được tạo ra khi bit tương ứng trong thanh ghi `MPU_INTC.INTC_ISR_SETn` được set. Software interrupt sẽ được clear khi bit tương ứng trong thanh ghi `MPU_INTC.INTC_ISR_CLEARn` được ghi. Tính năng này thường được sử dụng cho software debugging.

#### 6.1.1.2 Masking

**Individual Masking:**
Các interrupt trên mỗi incoming interrupt line có thể được enable hoặc disable độc lập thông qua thanh ghi `MPU_INTC.INTC_MIRn`. Khi có một unmasked incoming interrupt, INTC có thể tạo ra một trong hai loại interrupt request đến processor:
- **IRQ:** Low-priority interrupt request
- **FIQ:** Fast interrupt request (Không khả dụng trên General Purpose devices)

Loại interrupt request được chọn trước khi priority sorting thông qua thanh ghi `MPU_INTC.INTC_ILRm[0] FIQ/NIRQ` bit (m= [0,127]).

Trạng thái incoming interrupt hiện tại trước khi masking có thể đọc từ thanh ghi `MPU_INTC.INTC_ITRn`. Trạng thái interrupt sau khi masking và trước priority sorting có thể đọc từ thanh ghi `MPU_INTC.INTC_PENDING_IRQn` và `MPU_INTC.INTC_PENDING_FIQn`.

**Priority Masking:**
Để cho phép xử lý nhanh hơn các interrupt có priority cao, một programmable priority masking threshold được cung cấp (trường `MPU_INTC.INTC_THRESHOLD[7:0] PRIORITYTHRESHOLD`). Chỉ các interrupt có priority lớn hơn giá trị threshold mới có thể đi qua để được processed.

Các interrupt có priority từ 0x0 đến 0x7F được so sánh với priority threshold. Các interrupt đã được masked sẽ không bị ảnh hưởng. Tuy nhiên, priority 0 không bao giờ bị masked bởi threshold này; priority threshold là 0 được coi giống như 0x80.

**Ý nghĩa giá trị priority:**
- 0x0 là priority cao nhất
- 0x7F là priority thấp nhất

Khi priority masking không cần thiết, giá trị 0xFF trong trường PRIORITYTHRESHOLD sẽ disable tính năng này. Giá trị 0xFF là giá trị reset mặc định cho backward compatibility với các phiên bản cũ của INTC.

#### 6.1.1.3 Priority Sorting

Priority level (0 là cao nhất) được gán cho mỗi incoming interrupt line. Cả priority level và interrupt request type đều được cấu hình thông qua thanh ghi `MPU_INTC.INTC_ILRm`.

Khi có nhiều hơn một unmasked incoming interrupt cùng priority level và interrupt request type xảy ra đồng thời, interrupt có số thứ tự cao nhất sẽ được chọn trước.

**Quá trình xử lý:**
1. INTC tách biệt IRQ và FIQ sử dụng bit `MPU_INTC.INTC_ILRm[0] FIQ/NIRQ` tương ứng
2. IRQ được đặt ở trạng thái PENDING_IRQn nếu không có interrupt khác đang được xử lý
3. INTC assert IRQ/FIQ và bắt đầu priority computation
4. Priority sorting cho IRQ và FIQ được thực hiện song song
5. Khi computation hoàn thành, số interrupt được xác định bởi priority sorter và số đó được đặt trong trường tương ứng:
   - `MPU_INTC.INTC_SIR_IRQ[6:0] ACTIVEIRQ`
   - `MPU_INTC.INTC_SIR_FIQ[6:0] ACTIVEFIQ`
6. Giá trị được preserve trong trường ACTIVEIRQ/ACTIVEFIQ cho đến khi bit `MPU_INTC.INTC_CONTROL NEWIRQAGR` hoặc `NEWFIQAGR` được set
7. Sau khi interrupting peripheral device đã được handled, người dùng có thể set bit NEWIRQAGR/NEWFIQAGR tương ứng để báo cho INTC rằng interrupt đã được xử lý
8. Nếu có bất kỳ pending incoming interrupts nào đang chờ trong interrupt path, INTC xử lý bất kỳ interrupt nào khác có priority cao hơn; ngược lại, IRQ hoặc FIQ interrupt line bị de-asserted

### 6.1.2 Register Protection

Nếu bit `MPU_INTC.INTC_PROTECTION[0] PROTECTION` được set, quyền truy cập vào các thanh ghi INTC bị hạn chế chỉ cho privileged mode. Để biết thêm thông tin, xem Section 6.6.1.7 về thanh ghi INTC_PROTECTION.

**LƯU Ý:** Thanh ghi `MPU_INTC.INTC_PROTECTION` luôn được hạn chế ở privileged mode.

### 6.1.3 Module Power Saving

INTC cung cấp chức năng auto-idle trong ba clock domains của nó:
- Interface clock
- Functional clock
- Synchronizer clock

**Interface clock auto-idle:**
Chế độ interface clock auto-idle power-saving được enable nếu bit `MPU_INTC.INTC_SYSCONFIG[0] AUTOIDLE` được set thành 1. Khi enabled và không có hoạt động trên bus interface, interface clock sẽ restart mà không có bất kỳ latency penalty nào. Sau khi reset, chế độ này được disable mặc định.

**Functional clock auto-idle:**
Bit `MPU_INTC.INTC_IDLE[0] FUNCIDLE` được set thành 0. Khi chế độ này được enable và không có hoạt động interrupt (không có pending interrupt đến hoặc không có interrupt được xử lý), functional clock bị disable internally để module giảm công suất tiêu thụ.

**Synchronizer clock auto-idle:**
Khi có một incoming interrupt mới được phát hiện, functional clock restart và INTC xử lý interrupt. Độ trễ trước khi các asynchronous interrupts được synchronize là bốn functional clock cycles cộng hoặc trừ một cycle nếu bit `MPU_INTC.INTC_IDLE[1] TURBO` được set thành 0.

Khi TURBO bit được set thành 1, interrupt generation sẽ giảm đi một cycle bằng cách disable functional clock auto-idle. Điều này có thể giảm latency khi chờ một interrupt, nhưng lợi ích là tối thiểu.

### 6.1.4 Error Handling

**Các trường hợp gây lỗi:**
- Privilege violation (cố gắng truy cập thanh ghi PROTECTION ở user mode hoặc bất kỳ thanh ghi nào ở user mode nếu Protection bit được set)
- Unsupported commands

**Các trường hợp KHÔNG gây lỗi:**
- Truy cập vào non-decoded address
- Ghi vào read-only register

### 6.1.5 Interrupt Handling

**Interrupt Latency:**
Việc tạo INTC interrupt mất bốn INTC functional clock cycles (cộng hoặc trừ một cycle) nếu bit TURBO được set thành 0. Nếu TURBO bit được set thành 1, interrupt generation có thể được giảm đi một cycle bằng cách disable functional clock auto-idle. Lợi ích giảm latency là tối thiểu.

Để giảm thiểu interrupt latency khi một unmasked interrupt xảy ra, IRQ hoặc FIQ interrupt được tạo ra **trước khi** priority sorting hoàn thành. Priority của interrupt được xác định trong 10 functional clock cycles, là số cycles tối thiểu cần thiết để MPU chuyển sang interrupt context sau khi nhận IRQ hoặc FIQ event.

**Priority Sorting Freeze:**
Bất kỳ lần đọc nào của thanh ghi `MPU_INTC.INTC_SIR_IRQ` hoặc `MPU_INTC.INTC_SIR_FIQ` trong quá trình priority sorting sẽ stall cho đến khi priority sorting hoàn thành và các thanh ghi có liên quan được update. Điều này đảm bảo rằng priority sorting luôn hoàn thành trước khi interrupt request được generated và interrupt service routine được execute, do đó delay giữa interrupt request và interrupt service routine được execute như vậy mà priority sorting luôn hoàn thành trước khi thanh ghi `MPU_INTC.INTC_SIR_FIQ` được đọc.

## 6.2 Basic Programming Model

### 6.2.1 Initialization Sequence

1. Program thanh ghi `MPU_INTC.INTC_SYSCONFIG`: Nếu cần thiết, enable interface clock auto-idle bằng cách set bit AUTOIDLE

2. Program thanh ghi `MPU_INTC.INTC_IDLE`: Nếu cần thiết, disable functional clock autogating hoặc enable synchronizer autogating bằng cách set bit FUNCIDLE hoặc TURBO bit tương ứng

3. Program thanh ghi `MPU_INTC.INTC_ILRm` cho mỗi interrupt line: Gán priority và set bit FIQ/NIRQ cho FIQ interrupt (mặc định, interrupts được map vào IRQ và priority là 0x0 [highest])

4. Program thanh ghi `MPU_INTC.INTC_MIRn`: Enable interrupts (mặc định, tất cả interrupt lines đều bị masked)

   **LƯU Ý:** Để program thanh ghi `MPU_INTC.INTC_MIRn`, các thanh ghi `MPU_INTC.INTC_MIR_SETn` và `MPU_INTC.INTC_MIR_CLEARn` được cung cấp để tạo điều kiện cho masking, ngay cả khi không thể write trực tiếp vào thanh ghi `MPU_INTC.INTC_MIRn`

### 6.2.2 INTC Processing Sequence

Sau khi INTC được enable, INTC sẵn sàng để enable và assign priorities cho incoming interrupts. Interrupt được xử lý như giải thích trong các phần tiếp theo.

Xử lý IRQ và FIQ khá tương tự; sự khác biệt cho FIQ sequence được hiển thị sau ký tự '/' trong code dưới đây.

**Processing Steps:**

1. Một hoặc nhiều unmasked incoming interrupts (M_IRQ_n signals) được nhận và IRQ hoặc FIQ outputs (IRQ[0] hoặc FIQ[0]) được asserted hiện tại

2. Nếu bit `INTC_ILRm[0] FIQ/NIRQ` được cleared thành 0, output signal `MPU_INTC_IRQ` được generated. Nếu bit FIQ/NIRQ được set thành 1, output signal `MPU_INTC_FIQ` được generated

3. INTC thực hiện priority sorting và updates thanh ghi `INTC_SIR_IRQ[6:0] ACTIVEIRQ` / `INTC_SIR_FIQ[6:0] ACTIVEFIQ` với current interrupt number

4. Trong priority sorting, nếu IRQ/FIQ được enabled (F==0) ở phía host processor, host processor tự động save context hiện tại và execute ISR như sau:

   ARM host processor tự động thực hiện các actions sau trong pseudo code:

   ```
   SPSR = CPSR                    /* Save CPSR before execution */
   CPSR[5] = 0                    /* Execute in ARM state */
   CPSR[7] = 1                    /* Disable IRQ */
   CPSR[8] = 1                    /* Disable Imprecise Data Aborts */
   CPSR[9] = CP15_reg1_EEbit      /* Endianness on exception entry */
   if interrupt == IRQ then
      PC = 0x0FFFF0018            /* Enter IRQ mode */
      /* high vectors configured then
      PC = 0xFFFFFF0018
      else
      PC = 0x00000018              /* execute interrupt vector */
   else if interrupt == FIQ then
      CPSR[6] = 0b10001            /* Enter FIQ mode */
      CPSR[6] = 1                  /* Disable FIQ */
      /* high vectors configured then
      PC = 0xFFFFFF001C
      else
      PC = 0x0000001C              /* execute interrupt vector */
   endif
   ```

5. ISR save context còn lại, identifies interrupt source bằng cách đọc trường `ACTIVEIRQ/ACTIVEFIQ`, và jumps đến relevant subroutine handler như sau:

   ```assembly
   ;INTC_SIR_IRQ/INTC_SIR_FIQ register address
   INTC_SIR_IRQ_ADDR/INTC_SIR_FIQ_ADDR  word 0x48200040/0x48200044
   ; ACTIVEIRQ bit field mask to get only the bit field
   ACTIVEIRQ_MASK .equ 0x7F
   _IRQ_ISR/_FIQ_ISR:
   ; Step 1 : Save context
   STMFD SP!, (R0-R12, LR) ; Save working registers and the Link register
   MRS R1, SPSR ; Save the SPSR into R1
   ; Get the number of the highest priority active IRQ/FIQ
   LDR R0, INTC_SIR_IRQ_ADDR/INTC_SIR_FIQ_ADDR
   LDR R10, [R10] ; Get the INTC_SIR_IRQ/INTC_SIR_FIQ register
   AND R10, R10, #ACTIVEIRQ_MASK ; Apply the mask to get the active IRQ number
   ; Step 5 : Branch to the relevant interrupt handler
   LDR PC, [PC, R10, lsl #2] ; PC base address points this instruction + 8
   NOP ; To index the table by the PC
   ; Table of handler start addresses
   .word IRQ0handler ;For IRQ0 or BANKO
   .word IRQ1handler
   .word IRQ2handler
   ```

6. Subroutine handler thực thi code cụ thể cho peripheral generating interrupt bằng cách handling event và deasserting interrupt condition ở phía peripheral

   ```assembly
   IRQ0handler:
   ; Save working registers
   STMFD SP!, (R0-R1)
   ; Now read-modify-write the peripheral module status register
   ; to clear the M_IRQ_0 triggering signal
   ; De-Assert the peripheral interrupt
   MOV R0, #0x7 ;Mask of 3 flags
   LDR R1, MODULE0_STATUS_REG_ADDR ; Get the address of the module Status Register
   STR R0, [R1] ; Clear the 3 flags
   ; Restore working registers LDMFD SP!, (R0-R1)
   ; Jump to the end part of the ISR
   B IRQ_ISR_end/FIQ_ISR_end
   ```

7. Sau khi return của subroutine, ISR set bit `NEWIRQAGR/NEWFIQAGR` để enable processing của subsequent pending IRQ/FIQ và restore ARM context như sau:

   ```assembly
   ; INTC_CONTROL register address
   IRQ_ISR_end/FIQ_ISR_end : .word 0x48200048;
   NEWIRQAGR/NEWFIQAGR bit mask to set only the NEWIRQAGR/NEWFIQAGR bit
   NEWIRQAGR_MASK/NEWFIQAGR_MASK equ 0x1/0x2
   IRQ_ISR_end/FIQ_ISR_end
   ; Allow new IRQ/FIQ generation at INTC side
   ; Write 1 to the NEWIRQAGR/NEWFIQAGR bit only so no need to write back others bits
   MOV R0, #NEWIRQAGR_MASK/NEWFIQAGR_MASK ; Get the NEWIRQAGR/NEWFIQAGR bit position
   LDR R1, INTC_CONTROL_ADDR
   STR R0, [R1] ; Write the NEWIRQAGR/NEWFIQAGR bit to allow new IRQs/FIQ
   ; Data Synchronization Barrier
   MOV R0, #0
   MCR P15, #0, R0, C7, C10, #4
   ; Instruction Synchronization Barrier
   MCR P15, #0, R0, C7, C5, #4
   MSR SPSR, R1; Restore the SPSR from R1
   LDMFD SP!, (R0-R12, LR) ; Restore working registers and Link register
   ; Return after handling the interrupt
   SUBS PC, LR, #4
   ```

8. Sau khi ISR return, ARM tự động restore context như sau:
   ```
   CPSR = SPSR
   PC = LR
   ```

**Lưu ý quan trọng:**
- Priority sorting mechanism được freeze trong interrupt processing sequence
- Nếu một interrupt condition xảy ra trong thời gian này, interrupt không bị lost
- Nó được sort khi bit `NEWIRQAGR/NEWFIQAGR` được set (priority sorting được reactivated)

### 6.2.3 INTC Preemptive Processing Sequence

**Preemptive interrupts** (còn gọi là nested interrupts) có thể giảm latencies cho các interrupt có priority cao hơn.

Một preempting ISR có thể được served bởi một higher priority interrupt. Do đó, preempting interrupt có priority cao nhất có thể được served ngay lập tức. Nested interrupts phải được sử dụng cẩn thận để tránh corrupted data.

Programmers phải save/restore thanh ghi corrupted. Enable thanh ghi PRIORITY là khá tương tự như IRQ và FIQ processing sequences được mô tả ở ARM side.

**Để enable IRQ/FIQ preemption bởi higher priority IRQs/FIQs:**

Tại đầu của IRQ/FIQ ISR, programmers phải tuân theo một procedure đã cho:

1. Save các ARM critical context registers
2. Save trường `INTC_THRESHOLD PRIORITYTHRESHOLD` trước khi modifying nó
3. Đọc active interrupt number từ các trường tương ứng:
   - `INTC_IRQ_PRIORITY` / `IROQPRIORITY`
   - `INTC_FIQ_PRIORITY` / `FIQPRIORITY`

   Và write nó vào trường `PRIORITYTHRESHOLD(1)`

4. Đọc active interrupt number từ trường `INTC_SIR_IRQ[6:0] ACTIVEFIQ` / `INTC_SIR_FIQ[6:0] ACTIVEFIQ` để identify interrupt source

5. Write 1 vào appropriate bit `INTC_CONTROL NEWIRQAGR` và (2) `NEWFIQAGR` trong khi vẫn processing interrupt sẽ chỉ allow higher priority interrupts để preempt

6. Vì các writes được posted trên interconnect bus, để chắc chắn rằng các preceding writes đã hoàn thành trước khi enabling IRQs/FIQs, một Data Synchronization Barrier được sử dụng. Điều này đảm bảo rằng IRQ line được de-asserted trước khi IRQ/FIQ enabling. Sau đó, INTC xử lý bất kỳ pending interrupts nào khác hoặc deasserts IRQ/FIQ signal nếu không có interrupt

7. Enable IRQ/FIQ ở ARM side

8. Jump đến relevant subroutine handler

**Sample Code cho Preemptive Processing:**

```assembly
; bit field mask to get only the bit field
ACTIVEPRIO_MASK .equ 0x7F
_IRQ_ISR:
; Step 1 : Save context
STMFD SP!, (R0-R12, LR) ; Save working registers
MRS R11, SPSR ; Save the SPSR into R11
; Step 2 : Save the INTC_THRESHOLD register into R12
LDR R0, INTC_THRESHOLD_ADDR
LDR R12, [R0]
;--priority--
;threshold mechanism is enabled automatically when writing a priority in the range of 0x00 to 0x7F
;0x7F: Writing value of 0xFF (reset default) disables the priority.
;0x7E: No priority, effectively allows all IRQs/FIQs.
;NOTE: When hardware priority threshold is in use, the priorities of interrupts selected as FIQ or IRQ become linked, otherwise they are independent. When linked, it is required that all FIQ priorities be set higher than all IRQ priorities to maintain the relative priority of FIQ over IRQ.
;(2) When handling FIQs using the priority threshold mechanism, it is required to write both New FIQ Agreement and New IRQ Agreement bits at the same time to cover the case that the new priority threshold applied can fit both in FIQ Setting as it proposes. This IRQ will not have been seen by the ARM as it will have been masked on entry to the FIQ ISR. However, the source of the IRQ will remain active and it will be re-prioritized when the priority threshold falls to a low enough priority. The precaution of writing to New FIQ Agreement (as well as New IRQ Agreement) is not required during an IRQ ISR, as FIQ sorting is not affected (provided all FIQ priorities are higher than all IRQ priorities).
; Step 3 : Get the priority of the highest priority active IRQ
LDR R1, INTC_IRQ_PRIORITY_ADDR/INTC_FIQ_PRIORITY_ADDR
LDR R0, [R1] ; Write it to the INTC_IRQ_PRIORITY/INTC_FIQ_PRIORITY register
AND R1, R1, #ACTIVEPRIO_MASK ; Apply the mask to get the priority of the IRQ
STR R1, [R0] ; Write it to the INTC_THRESHOLD register
; Step 4 : Get the number of the highest priority active IRQ
LDR R10, INTC_SIR_IRQ_ADDR/INTC_SIR_FIQ_ADDR
LDR R10, [R10] ; Get the INTC_SIR_IRQ/INTC_SIR_FIQ register
AND R10, R10, #ACTIVEIRQ_MASK ; Apply the mask to get the active IRQ number
; Step 5 : Branch to the relevant interrupt subroutine handler
MOV R0, #0x1/0x3 ; Get the NEWIRQAGR and NEWFIQAGR bit position
LDR R1, INTC_CONTROL_ADDR
STR R0, [R1] ; Write the NEWIRQAGR and NEWFIQAGR bit
; Step 6 : Data Synchronization Barrier
MOV R0, #0
MCR P15, #0, R0, C7, C10, #4
; Instruction Synchronization Barrier
; Step 7 : Read-modify-write the CPSR to enable IRQs/FIQs at ARM side
MRS R0, CPSR ; Read the status register
BIC R0, R0, #0x80/0xC0 ; Clear the I/F bit
MSR CPSR, R0 ; Write it back to enable IRQs
; Step 8 : Jump to relevant subroutine handler
LDR PC, [PC, R10, lsl #2] ; PC base address points this instruction + 8
NOP ; To index the table by the PC
; Table of handler start addresses
.word IRQ0handler ;IRQ0 BANK0
.word IRQ1handler
.word IRQ2handler
```

**Sau khi return của relevant IRQ/FIQ subroutine handle:**

1. Disable IRQs/FIQs ở ARM side
2. Restore trường `INTC_THRESHOLD PRIORITYTHRESHOLD`
3. Restore các ARM critical context registers

**Sample Code:**

```assembly
IRQ_ISR_end
; Step 1 : Read-modify-write the CPSR to disable IRQs/FIQs at ARM side
MRS R0, CPSR ; Read the CPSR
ORR R0, R0, #0x80/0xC0 ; Set the I/F bit
MSR CPSR, R0 ; Write it back to disable IRQs
; Step 2 : Restore the INTC_THRESHOLD register from R12
LDR R0, INTC_THRESHOLD_ADDR
STR R12, [R0]
; Step 3 : Restore critical context
MSR SPSR, R11 ; Restore the SPSR from R11
LDMFD SP!, (R0-R12, LR) ; Restore working registers and Link register
; Return after handling the interrupt
SUBS PC, LR, #4
```

### 6.2.4 Interrupt Preemption

Nếu muốn enable pre-emption bởi higher priority interrupts, ISR nên đọc active interrupt priority và write nó vào appropriate threshold trước khi process interrupt.

Writing "1" vào appropriate bit `INTC_IRQ_AGR` hoặc `NEW_FIQ_AGR` của thanh ghi CONTROL trong khi vẫn processing interrupt sẽ chỉ allow higher priority interrupts để pre-empt.

**Quan trọng:**
- Với mỗi level của pre-emption, programmer phải save threshold value trước khi modifying nó và restore nó tại end của ISR trước cleanup
- Priority threshold mechanism được enabled tự động khi writing một priority trong range 0 đến 7Fh

**Priority Threshold Effects:**
Khi hardware priority threshold đang được sử dụng, priorities của interrupts được selected là FIQ hoặc IRQ trở nên linked. Khi chúng được linked, tất cả FIQ priorities phải được set cao hơn tất cả IRQ priorities để duy trì relative priority của FIQ over IRQ.

Khi handling FIQs sử dụng priority threshold mechanism, cần write cả New FIQ Agreement và New IRQ Agreement bits cùng lúc để cover trường hợp new priority threshold applied có thể fit cả trong FIQ Setting. IRQ này sẽ không được ARM nhìn thấy vì nó đã bị masked khi entry vào FIQ ISR. Tuy nhiên, source của IRQ sẽ vẫn active và nó sẽ được re-prioritized khi priority threshold falls xuống đủ thấp để được processed.

Việc writing vào New FIQ Agreement (cũng như New IRQ Agreement) không required trong IRQ ISR, vì FIQ sorting không bị affected (provided all FIQ priorities cao hơn all IRQ priorities).

### 6.2.5 ARM A8 INTC Spurious Interrupt Handling

**Spurious flag** cho biết kết quả của sorting (một window 10 INTC functional clock cycles sau interrupt assertion) có invalid hay không. Sorting là invalid nếu:
- Interrupt triggers nhưng không còn active trong quá trình sorting
- Có thay đổi trong mask đã affect kết quả trong sorting time

**Kết quả:**
Do đó, các giá trị trong các thanh ghi sau phải không được thay đổi trong khi corresponding interrupt được asserted. Chỉ active interrupt input mà triggered soft có thể được masked trước khi turn off. Nếu các thanh ghi này được thay đổi trong window 10 functional clock cycles sau interrupt assertion, các giá trị kết quả của các thanh ghi sau trở nên invalid:

- `INTC_SIR_IRQ`
- `INTC_SIR_FIQ`
- `INTC_IRQ_PRIORITY`
- `INTC_FIQ_PRIORITY`

Điều kiện này được detected cho cả IRQ và FIQ, và invalid status được flagged trong các bit fields:
- `SPURIOUSIRQFLAG` (xem NOTE 1)
- `SPURIOUSFIQFLAG` (xem NOTE 2)

Trong các thanh ghi SIR và PRIORITY. Một giá trị 1 indicates invalid interrupt number và priority. Invalid indication có thể được tested trong software như một false positive interrupt value.

**LƯU Ý:**

1. Bit field `INTC_SIR_IRQ[31:7] SPURIOUSIRQFLAG` là bản copy của bit field `INTC_IRQ_PRIORITY[31:7] SPURIOUSIRQFLAG`

2. Bit field `INTC_SIR_FIQ[31:7] SPURIOUSFIQFLAG` là bản copy của bit field `INTC_FIQ_PRIORITY[31:7] SPURIOUSFIQFLAG`

## 6.3 ARM Cortex-A8 Interrupts

AM335x sử dụng ARM Cortex-A8 processor core với 128 interrupt sources được map vào INTC. Các interrupt sources bao gồm:

- MPU Subsystem Internal interrupts
- External peripherals interrupts
- DMA interrupts
- Timer interrupts
- Communication peripherals (UART, I2C, SPI, USB, Ethernet)
- GPIO interrupts
- ADC interrupts
- Security modules (AES, SHA2, RNG)
- Và nhiều peripherals khác

**Các interrupt quan trọng bao gồm:**

| Int Number | Acronym/Name | Source | Signal Name |
|------------|--------------|---------|-------------|
| 0 | EMUINT | MPU Subsystem Internal | Emulation interrupt |
| 1 | COMMTX | MPU Subsystem Internal | CortexA8 COMMTX |
| 2 | COMMRX | MPU Subsystem Internal | CortexA8 COMMRX |
| 4 | ELM_IRQ | ELM | Error Location Module |
| 7 | NMI | External Pin | nmi_int |
| 12-14 | EDMA | TPCC (EDMA) | tpcc_mpnt_pend_po |
| 28 | MMCSD0INT | MMCSD0 | SINTERRUPTN |
| 30 | I2C0INT | I2C0 | POINTRPEND |
| 31 | eCAP0 event/interrupt | eCAP0 | ecap_intr_intr_pend |
| 40-43 | 3PGSW/Ethernet | CPSW (Ethernet) | c0_rx_thresh_pend, c0_rx_pend, c0_tx_pend, c0_misc_pend |
| 44-47 | UART | UART3-UART6 | nirq |
| 64-68 | GPIO | GPIO 0-3 | pointrpend |
| 72-76 | UART | UART0-UART2, RTC | nirq, timer_intr_pend, alarm_intr_pend |
| 91 | WDTINT (Public Watchdog) | WDTIMER1 | PO_INT_PEND |
| 95-96 | TINT (DMTIMER) | DMTIMER4-DMTIMER7 | POINTR_PEND |
| 100 | GPMCINT | GPMC | gpmc_sinterrupt |

*Xem đầy đủ bảng interrupt mapping trong tài liệu reference để biết tất cả 128 interrupt sources.*

## 6.4 Crypto DMA Events

AM335x cung cấp các DMA events cho cryptographic operations thông qua AES và SHA2 modules:

**AES Module Events:**
- Events 1-12: AES0 và AES1 request contexts và data transfers trên Secure và Public HIB
- Bao gồm: context input request, data input request, data output request cho cả secure và public HIB

**SHA2 Module Events:**
- Events 21-26: SHA2MD5 Module 1 requests cho contexts và data transfers
- Hỗ trợ secure và public HIB operations

**DES Module Events:**
- Events 15-20: DES Module requests cho secure và public HIB operations

## 6.5 PWM Events

Timer và eCAP events được sử dụng cho PWM (Pulse Width Modulation) operations:

**Timer Events (Event 0):**
- TIMER5-TIMER7 MUX input từ IO signals
- eCAP0-eCAP2 MUX input từ IO signals

**UART Events:**
- UART0-UART5 mapped đến các event numbers khác nhau

**GPIO Events:**
- GPIO 0-3 banks với multiple interrupt lines

**Additional PWM Sources:**
- McASP0, McASP1 interrupts
- 3PGSW (Ethernet switch)
- DCAN0, DCAN1 (CAN controllers)
- DCAM (Camera interface)

## 6.6 INTC Registers

### 6.6.1 Chi tiết các thanh ghi INTC

#### 6.6.1.1 INTC_REVISION Register (offset = 0h) [reset = 50h]

Register mask: FFFFFFFFh

Thanh ghi này chứa IP revision code.

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-8 | RESERVED | R | 0h | Reads returns 0 |
| 7-0 | Rev | R | 50h | IP revision<br>[7:4] Major revision<br>[3:0] Minor revision<br>Examples: 0x10 for 1.0, 0x21 for 2.1 |

#### 6.6.1.2 INTC_SYSCONFIG Register (offset = 10h) [reset = 0h]

Register mask: FFFFFFFFh

Thanh ghi này kiểm soát các parameters khác nhau của OCP interface.

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-5 | RESERVED | R/W | 0h | - |
| 4-3 | RESERVED | R | 0h | Write 0's for future compatibility<br>Reads returns 0 |
| 2 | RESERVED | R/W | 0h | - |
| 1 | SoftReset | R/W | 0h | Software reset<br>Set this bit to 1 to trigger a module reset<br>The bit is automatically reset by the hardware<br>During reads, it always returns 0<br>0h(Read) = always_Always returns 0<br>1h(Read) = never_never happens |
| 0 | AutoIdle | R/W | 0h | Internal OCP clock gating strategy<br>0h = clkfree: OCP clock is free running<br>1h = autoJkGate: Automatic OCP clock gating strategy is applied, based on the OCP interface activity |

#### 6.6.1.3 INTC_SYSSTATUS Register (offset = 14h) [reset = 0h]

Register mask: FFFFFFFFh

Thanh ghi này cung cấp thông tin trạng thái về module.

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-8 | RESERVED | R | 0h | - |
| 7-1 | RESERVED | R | 0h | Reserved for OCP socket status information<br>Read returns 0 |
| 0 | ResetDone | R | X | Internal reset monitoring<br>0h = rstOngoing: Internal module reset is on-going<br>1h = rstComp: Reset completed |

#### 6.6.1.4 INTC_SIR_IRQ Register (offset = 40h) [reset = FFFFFF80h]

Register mask: FFFFFFFFh

Thanh ghi này cung cấp currently active IRQ interrupt number.

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-7 | SpuriousIRQ | R/W | 1FFFFFh | Spurious IRQ flag |
| 6-0 | ActiveIRQ | R/W | 0h | Active IRQ number |

#### 6.6.1.5 INTC_SIR_FIQ Register (offset = 44h) [reset = FFFFFF80h]

Register mask: FFFFFFFFh

Thanh ghi này cung cấp currently active FIQ interrupt number.

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-7 | SpuriousFIQ | R | 1FFFFFh | Spurious FIQ flag |
| 6-0 | ActiveFIQ | R | 0h | Active FIQ number |

#### 6.6.1.6 INTC_CONTROL Register (offset = 48h) [reset = 0h]

Register mask: FFFFFFFFh

Thanh ghi này chứa các new interrupt agreement bits.

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-2 | RESERVED | R | 0h | Write 0's for future compatibility<br>Reads returns 0 |
| 1 | NewFIQAgr | W | 0h | Reset FIQ output and enable new FIQ generation<br>0h(Write) = nofun_no function effect<br>1h(Write) = NewFiq_Reset FIQ output and enable new FIQ generation |
| 0 | NewIRQAgr | W | 0h | New IRQ generation<br>0h(Write) = nofun_no function effect<br>1h(Write) = Newlrq_Reset IRQ output and enable new IRQ generation |

#### 6.6.1.7 INTC_PROTECTION Register (offset = 4Ch) [reset = 0h]

Register mask: FFFFFFFFh

Thanh ghi này kiểm soát protection của các thanh ghi khác. Thanh ghi này chỉ có thể được accessed ở privileged mode, bất kể giá trị hiện tại của protection bit.

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-1 | RESERVED | R | 0h | Write 0's for future compatibility<br>Reads returns 0 |
| 0 | Protection | R/W | 0h | Protection mode<br>0h = ProtMDis: Protection mode disabled (default)<br>1h = ProtMEnb: Protection mode enabled. When enabled, only privileged mode can access registers |

#### 6.6.1.8 INTC_IDLE Register (offset = 50h) [reset = 0h]

Register mask: FFFFFFFFh

Thanh ghi này kiểm soát clock auto-idle cho functional clock và input synchronisers.

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-2 | RESERVED | R | 0h | Write 0's for future compatibility<br>Reads returns 0 |
| 1 | Turbo | R/W | 0h | Input synchronizer clock auto-gating<br>0h = SyncFree: Input synchronizer clock is free running (default)<br>1h = SyncAuto: Input synchronizer clock is auto-gated based on interrupt input activity |
| 0 | FuncIdle | R/W | 0h | Functional clock auto-idle mode<br>0h = FuncAuto: Functional clock gating strategy is applied (default)<br>1h = FuncFree: Functional clock is free-running |

#### 6.6.1.9 INTC_IRQ_PRIORITY Register (offset = 60h) [reset = FFFFFFC0h]

Register mask: FFFFFFFFh

Thanh ghi này cung cấp currently active IRQ priority level.

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-7 | SpuriousIRQflag | R | 1FFFFFh | Spurious IRQ flag |
| 6-0 | IRQPriority | R | 40h | Current IRQ priority |

#### 6.6.1.10 INTC_FIQ_PRIORITY Register (offset = 64h) [reset = FFFFFFC0h]

Register mask: FFFFFFFFh

Thanh ghi này cung cấp currently active FIQ priority level.

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-7 | SpuriousFIQflag | R | 1FFFFFh | Spurious FIQ flag |
| 6-0 | FIQPriority | R | 40h | Current FIQ priority |

#### 6.6.1.11 INTC_THRESHOLD Register (offset = 68h) [reset = FFh]

Register mask: FFFFFFFFh

Thanh ghi này sets priority threshold.

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-8 | RESERVED | R | 0h | Reads returns 0 |
| 7-0 | PriorityThreshold | R/W | FFh | Priority threshold<br>Values used are 00h to 3Fh<br>Value FFh disables priority threshold |

#### 6.6.1.12 INTC_ITR0 Register (offset = 80h) [reset = 0h]

Register mask: 0h

Thanh ghi này hiển thị raw interrupt input status trước khi masking.

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | Itr | R | X | Interrupt status before masking |

#### 6.6.1.13 INTC_MIR0 Register (offset = 84h) [reset = FFFFFFFFh]

Register mask: FFFFFFFFh

Thanh ghi này chứa interrupt mask.

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | Mir | R/W | FFFFFFFFh | Interrupt mask |

#### 6.6.1.14 INTC_MIR_CLEAR0 Register (offset = 88h) [reset = 0h]

Register mask: 0h

Thanh ghi này được sử dụng để clear interrupt mask bits.

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | MirClear | W | X | Write 1 clears the mask bit to 0, reads return 0 |

#### 6.6.1.15 INTC_MIR_SET0 Register (offset = 8Ch) [reset = 0h]

Register mask: 0h

Thanh ghi này được sử dụng để set interrupt mask bits.

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | MirSet | W | X | Write 1 sets the mask bit to 1, reads return 0 |

#### 6.6.1.16 INTC_ISR_SET0 Register (offset = 90h) [reset = 0h]

Register mask: FFFFFFFFh

Thanh ghi này được sử dụng để set software interrupt bits. Nó cũng được sử dụng để đọc currently active software interrupts.

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | IsrSet | R/W | 0h | Reads returns the currently active software interrupts. Write 1 sets the software interrupt bits to 1 |

#### 6.6.1.17 INTC_ISR_CLEAR0 Register (offset = 94h) [reset = 0h]

Register mask: 0h

Thanh ghi này được sử dụng để clear software interrupt bits.

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | IsrClear | W | X | Write 1 clears the software interrupt bits to 0, reads return 0 |

#### 6.6.1.18 INTC_PENDING_IRQ0 Register (offset = 98h) [reset = 0h]

Register mask: FFFFFFFFh

Thanh ghi này chứa IRQ status sau khi masking.

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | PendingIRQ | R | 0h | IRQ status after masking |

#### 6.6.1.19 INTC_PENDING_FIQ0 Register (offset = 9Ch) [reset = 0h]

Register mask: FFFFFFFFh

Thanh ghi này chứa FIQ status sau khi masking.

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | PendingFIQ | R | 0h | FIQ status after masking |

#### 6.6.1.40 INTC_MIR_SET3 Register (offset = ECh) [reset = 0h]

Register mask: 0h

Thanh ghi này được sử dụng để set interrupt mask bits (interrupts 96-127).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | MirSet | W | X | Write 1 sets the mask bit to 1, reads return 0 |

#### 6.6.1.41 INTC_ISR_SET3 Register (offset = F0h) [reset = 0h]

Register mask: FFFFFFFFh

Thanh ghi này được sử dụng để set software interrupt bits (interrupts 96-127). Nó cũng được sử dụng để đọc currently active software interrupts.

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | IsrSet | R/W | 0h | Reads returns the currently active software interrupts. Write 1 sets the software interrupt bits to 1 |

#### 6.6.1.42 INTC_ISR_CLEAR3 Register (offset = F4h) [reset = 0h]

Register mask: 0h

Thanh ghi này được sử dụng để clear software interrupt bits (interrupts 96-127).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | IsrClear | W | X | Write 1 clears the software interrupt bits to 0, reads return 0 |

#### 6.6.1.43 INTC_PENDING_IRQ3 Register (offset = F8h) [reset = 0h]

Register mask: FFFFFFFFh

Thanh ghi này chứa IRQ status sau khi masking (interrupts 96-127).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | PendingIRQ | R | 0h | IRQ status after masking |

#### 6.6.1.44 INTC_PENDING_FIQ3 Register (offset = FCh) [reset = 0h]

Register mask: FFFFFFFFh

Thanh ghi này chứa FIQ status sau khi masking (interrupts 96-127).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | PendingFIQ | R | 0h | FIQ status after masking |

#### 6.6.1.45 INTC_ILR_0 to INTC_ILR_127 Registers (offset = 100h to 2FCh) [reset = 0h]

Register mask: FFFFFFFFh

Có 128 thanh ghi ILR (m = 0 đến 127), mỗi thanh ghi tương ứng với một interrupt line. Các thanh ghi này được sử dụng để cấu hình priority và routing (IRQ/FIQ) cho mỗi interrupt.

**Register Layout:**

Các thanh ghi ILR được địa chỉ tại:
- INTC_ILR_0: offset 100h
- INTC_ILR_1: offset 104h
- INTC_ILR_2: offset 108h
- ...
- INTC_ILR_127: offset 2FCh

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-8 | RESERVED | R | 0h | Write 0's for future compatibility<br>Reads returns 0 |
| 7-2 | Priority | R/W | 0h | Interrupt priority<br>Valid range: 0x00 (highest priority) to 0x3F (lowest priority) |
| 1 | RESERVED | R/W | 0h | - |
| 0 | FIQ/NIRQ | R/W | 0h | Interrupt IRQ FIQ mapping<br>0h = IntIRQ: Interrupt is routed to IRQ<br>1h = IntFIQ: Interrupt is routed to FIQ (this selection is reserved on GP devices) |

**Lưu ý quan trọng về ILR registers:**

1. **Priority Values:**
   - 0x00 = Highest priority
   - 0x3F = Lowest priority
   - Priority values từ 0x00 đến 0x3F được sử dụng
   - Các giá trị khác là reserved

2. **FIQ Routing:**
   - FIQ không khả dụng trên General Purpose (GP) devices
   - Chỉ nên sử dụng IRQ routing trên GP devices
   - Trên devices hỗ trợ FIQ, có thể route các high-priority interrupts đến FIQ

3. **Priority Grouping:**
   - Khi sử dụng priority threshold mechanism, cần phải group priorities cẩn thận
   - Tất cả FIQ priorities nên cao hơn tất cả IRQ priorities
   - Điều này đảm bảo FIQ luôn có priority cao hơn IRQ

4. **Default Configuration:**
   - Mặc định, tất cả interrupts được route đến IRQ
   - Mặc định, tất cả interrupts có priority 0x00 (highest)
   - Programmer cần reconfigure theo yêu cầu của application

**Lưu ý:**
- Các thanh ghi ITRn, MIRn, MIR_CLEARn, MIR_SETn, ISR_SETn, ISR_CLEARn, PENDING_IRQn, và PENDING_FIQn có 4 instances (n=0,1,2,3) để cover tất cả 128 interrupt lines
- Mỗi instance xử lý 32 interrupt lines
- Instance 0: Interrupts 0-31
- Instance 1: Interrupts 32-63
- Instance 2: Interrupts 64-95
- Instance 3: Interrupts 96-127

#### 6.6.1.21 INTC_ITR1 Register (offset = A0h) [reset = 0h]

Register mask: 0h

Thanh ghi này hiển thị raw interrupt input status trước khi masking (interrupts 32-63).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | Itr | R | X | Interrupt status before masking |

#### 6.6.1.22 INTC_MIR1 Register (offset = A4h) [reset = FFFFFFFFh]

Register mask: FFFFFFFFh

Thanh ghi này chứa interrupt mask (interrupts 32-63).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | Mir | R/W | FFFFFFFFh | Interrupt mask |

#### 6.6.1.23 INTC_MIR_CLEAR1 Register (offset = A8h) [reset = 0h]

Register mask: 0h

Thanh ghi này được sử dụng để clear interrupt mask bits (interrupts 32-63).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | MirClear | W | X | Write 1 clears the mask bit to 0, reads return 0 |

#### 6.6.1.24 INTC_MIR_SET1 Register (offset = ACh) [reset = 0h]

Register mask: 0h

Thanh ghi này được sử dụng để set interrupt mask bits (interrupts 32-63).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | MirSet | W | X | Write 1 sets the mask bit to 1, reads return 0 |

#### 6.6.1.25 INTC_ISR_SET1 Register (offset = B0h) [reset = 0h]

Register mask: FFFFFFFFh

Thanh ghi này được sử dụng để set software interrupt bits (interrupts 32-63). Nó cũng được sử dụng để đọc currently active software interrupts.

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | IsrSet | R/W | 0h | Reads returns the currently active software interrupts. Write 1 sets the software interrupt bits to 1 |

#### 6.6.1.26 INTC_ISR_CLEAR1 Register (offset = B4h) [reset = 0h]

Register mask: 0h

Thanh ghi này được sử dụng để clear software interrupt bits (interrupts 32-63).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | IsrClear | W | X | Write 1 clears the software interrupt bits to 0, reads return 0 |

#### 6.6.1.27 INTC_PENDING_IRQ1 Register (offset = B8h) [reset = 0h]

Register mask: FFFFFFFFh

Thanh ghi này chứa IRQ status sau khi masking (interrupts 32-63).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | PendingIRQ | R | 0h | IRQ status after masking |

#### 6.6.1.28 INTC_PENDING_FIQ1 Register (offset = BCh) [reset = 0h]

Register mask: FFFFFFFFh

Thanh ghi này chứa FIQ status sau khi masking (interrupts 32-63).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | PendingFIQ | R | 0h | FIQ status after masking |

#### 6.6.1.29 INTC_ITR2 Register (offset = C0h) [reset = 0h]

Register mask: 0h

Thanh ghi này hiển thị raw interrupt input status trước khi masking (interrupts 64-95).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | Itr | R | X | Interrupt status before masking |

#### 6.6.1.30 INTC_MIR2 Register (offset = C4h) [reset = FFFFFFFFh]

Register mask: FFFFFFFFh

Thanh ghi này chứa interrupt mask (interrupts 64-95).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | Mir | R/W | FFFFFFFFh | Interrupt mask |

#### 6.6.1.31 INTC_MIR_CLEAR2 Register (offset = C8h) [reset = 0h]

Register mask: 0h

Thanh ghi này được sử dụng để clear interrupt mask bits (interrupts 64-95).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | MirClear | W | X | Write 1 clears the mask bit to 0, reads return 0 |

#### 6.6.1.32 INTC_MIR_SET2 Register (offset = CCh) [reset = 0h]

Register mask: 0h

Thanh ghi này được sử dụng để set interrupt mask bits (interrupts 64-95).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | MirSet | W | X | Write 1 sets the mask bit to 1, reads return 0 |

#### 6.6.1.33 INTC_ISR_SET2 Register (offset = D0h) [reset = 0h]

Register mask: FFFFFFFFh

Thanh ghi này được sử dụng để set software interrupt bits (interrupts 64-95). Nó cũng được sử dụng để đọc currently active software interrupts.

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | IsrSet | R/W | 0h | Reads returns the currently active software interrupts. Write 1 sets the software interrupt bits to 1 |

#### 6.6.1.34 INTC_ISR_CLEAR2 Register (offset = D4h) [reset = 0h]

Register mask: 0h

Thanh ghi này được sử dụng để clear software interrupt bits (interrupts 64-95).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | IsrClear | W | X | Write 1 clears the software interrupt bits to 0, reads return 0 |

#### 6.6.1.35 INTC_PENDING_IRQ2 Register (offset = D8h) [reset = 0h]

Register mask: FFFFFFFFh

Thanh ghi này chứa IRQ status sau khi masking (interrupts 64-95).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | PendingIRQ | R | 0h | IRQ status after masking |

#### 6.6.1.36 INTC_PENDING_FIQ2 Register (offset = DCh) [reset = 0h]

Register mask: FFFFFFFFh

Thanh ghi này chứa FIQ status sau khi masking (interrupts 64-95).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | PendingFIQ | R | 0h | FIQ status after masking |

#### 6.6.1.37 INTC_ITR3 Register (offset = E0h) [reset = 0h]

Register mask: 0h

Thanh ghi này hiển thị raw interrupt input status trước khi masking (interrupts 96-127).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | Itr | R | X | Interrupt status before masking |

#### 6.6.1.38 INTC_MIR3 Register (offset = E4h) [reset = FFFFFFFFh]

Register mask: FFFFFFFFh

Thanh ghi này chứa interrupt mask (interrupts 96-127).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | Mir | R/W | FFFFFFFFh | Interrupt mask |

#### 6.6.1.39 INTC_MIR_CLEAR3 Register (offset = E8h) [reset = 0h]

Register mask: 0h

Thanh ghi này được sử dụng để clear interrupt mask bits (interrupts 96-127).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | MirClear | W | X | Write 1 clears the mask bit to 0, reads return 0 |

#### 6.6.1.40 INTC_MIR_SET3 Register (offset = ECh) [reset = 0h]

Register mask: 0h

Thanh ghi này được sử dụng để set interrupt mask bits (interrupts 96-127).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | MirSet | W | X | Write 1 sets the mask bit to 1, reads return 0 |

#### 6.6.1.41 INTC_ISR_SET3 Register (offset = F0h) [reset = 0h]

Register mask: FFFFFFFFh

Thanh ghi này được sử dụng để set software interrupt bits (interrupts 96-127). Nó cũng được sử dụng để đọc currently active software interrupts.

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | IsrSet | R/W | 0h | Reads returns the currently active software interrupts. Write 1 sets the software interrupt bits to 1 |

#### 6.6.1.42 INTC_ISR_CLEAR3 Register (offset = F4h) [reset = 0h]

Register mask: 0h

Thanh ghi này được sử dụng để clear software interrupt bits (interrupts 96-127).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | IsrClear | W | X | Write 1 clears the software interrupt bits to 0, reads return 0 |

#### 6.6.1.43 INTC_PENDING_IRQ3 Register (offset = F8h) [reset = 0h]

Register mask: FFFFFFFFh

Thanh ghi này chứa IRQ status sau khi masking (interrupts 96-127).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | PendingIRQ | R | 0h | IRQ status after masking |

#### 6.6.1.44 INTC_PENDING_FIQ3 Register (offset = FCh) [reset = 0h]

Register mask: FFFFFFFFh

Thanh ghi này chứa FIQ status sau khi masking (interrupts 96-127).

**Field Descriptions:**

| Bit | Field | Type | Reset | Description |
|-----|-------|------|-------|-------------|
| 31-0 | PendingFIQ | R | 0h | FIQ status after masking |

### 6.6.2 Bảng tổng hợp các thanh ghi INTC

| Offset | Acronym | Register Name | Section |
|--------|---------|---------------|---------|
| 0h | INTC_REVISION | Revision register | Section 6.6.1.1 |
| 10h | INTC_SYSCONFIG | System Configuration | Section 6.6.1.2 |
| 14h | INTC_SYSSTATUS | System Status | Section 6.6.1.3 |
| 40h | INTC_SIR_IRQ | Active IRQ number | Section 6.6.1.4 |
| 44h | INTC_SIR_FIQ | Active FIQ number | Section 6.6.1.5 |
| 48h | INTC_CONTROL | Control register | Section 6.6.1.6 |
| 4Ch | INTC_PROTECTION | Protection register | Section 6.6.1.7 |
| 50h | INTC_IDLE | Idle register | Section 6.6.1.8 |
| 60h | INTC_IRQ_PRIORITY | IRQ Priority | Section 6.6.1.9 |
| 64h | INTC_FIQ_PRIORITY | FIQ Priority | Section 6.6.1.10 |
| 68h | INTC_THRESHOLD | Threshold register | Section 6.6.1.11 |
| **Instance 0 (Interrupts 0-31)** |||
| 80h | INTC_ITR0 | Input Status 0 | Section 6.6.1.12 |
| 84h | INTC_MIR0 | Mask register 0 | Section 6.6.1.13 |
| 88h | INTC_MIR_CLEAR0 | Mask Clear 0 | Section 6.6.1.14 |
| 8Ch | INTC_MIR_SET0 | Mask Set 0 | Section 6.6.1.15 |
| 90h | INTC_ISR_SET0 | Software Interrupt Set 0 | Section 6.6.1.16 |
| 94h | INTC_ISR_CLEAR0 | Software Interrupt Clear 0 | Section 6.6.1.17 |
| 98h | INTC_PENDING_IRQ0 | Pending IRQ 0 | Section 6.6.1.18 |
| 9Ch | INTC_PENDING_FIQ0 | Pending FIQ 0 | Section 6.6.1.19 |
| **Instance 1 (Interrupts 32-63)** |||
| A0h | INTC_ITR1 | Input Status 1 | Section 6.6.1.21 |
| A4h | INTC_MIR1 | Mask register 1 | Section 6.6.1.22 |
| A8h | INTC_MIR_CLEAR1 | Mask Clear 1 | Section 6.6.1.23 |
| ACh | INTC_MIR_SET1 | Mask Set 1 | Section 6.6.1.24 |
| B0h | INTC_ISR_SET1 | Software Interrupt Set 1 | Section 6.6.1.25 |
| B4h | INTC_ISR_CLEAR1 | Software Interrupt Clear 1 | Section 6.6.1.26 |
| B8h | INTC_PENDING_IRQ1 | Pending IRQ 1 | Section 6.6.1.27 |
| BCh | INTC_PENDING_FIQ1 | Pending FIQ 1 | Section 6.6.1.28 |
| **Instance 2 (Interrupts 64-95)** |||
| C0h | INTC_ITR2 | Input Status 2 | Section 6.6.1.29 |
| C4h | INTC_MIR2 | Mask register 2 | Section 6.6.1.30 |
| C8h | INTC_MIR_CLEAR2 | Mask Clear 2 | Section 6.6.1.31 |
| CCh | INTC_MIR_SET2 | Mask Set 2 | Section 6.6.1.32 |
| D0h | INTC_ISR_SET2 | Software Interrupt Set 2 | Section 6.6.1.33 |
| D4h | INTC_ISR_CLEAR2 | Software Interrupt Clear 2 | Section 6.6.1.34 |
| D8h | INTC_PENDING_IRQ2 | Pending IRQ 2 | Section 6.6.1.35 |
| DCh | INTC_PENDING_FIQ2 | Pending FIQ 2 | Section 6.6.1.36 |
| **Instance 3 (Interrupts 96-127)** |||
| E0h | INTC_ITR3 | Input Status 3 | Section 6.6.1.37 |
| E4h | INTC_MIR3 | Mask register 3 | Section 6.6.1.38 |
| E8h | INTC_MIR_CLEAR3 | Mask Clear 3 | Section 6.6.1.39 |
| ECh | INTC_MIR_SET3 | Mask Set 3 | Section 6.6.1.40 |
| F0h | INTC_ISR_SET3 | Software Interrupt Set 3 | Section 6.6.1.41 |
| F4h | INTC_ISR_CLEAR3 | Software Interrupt Clear 3 | Section 6.6.1.42 |
| F8h | INTC_PENDING_IRQ3 | Pending IRQ 3 | Section 6.6.1.43 |
| FCh | INTC_PENDING_FIQ3 | Pending FIQ 3 | Section 6.6.1.44 |
| **Interrupt Priority Registers** |||
| 100h-2FCh | INTC_ILR_0 to INTC_ILR_127 | Interrupt Priority and Routing | Section 6.6.1.45 |

**Tổng số thanh ghi:**
- 11 thanh ghi control/status chính
- 32 thanh ghi cho 4 instances (8 registers x 4)
- 128 thanh ghi ILR (interrupt line registers)
- **Tổng cộng: 171 thanh ghi**

---

## Tóm tắt các điểm quan trọng

1. **Priority System:** 0x0 = cao nhất, 0x7F = thấp nhất
2. **Masking:** Individual masking qua MIRn registers, Priority masking qua THRESHOLD
3. **Processing:** IRQ/FIQ separation -> Priority sorting -> Handler execution
4. **Preemption:** Nested interrupts được hỗ trợ với proper threshold configuration
5. **Power Saving:** Auto-idle modes cho interface, functional và synchronizer clocks
6. **Spurious Handling:** Detection mechanism cho invalid interrupts trong sorting window
7. **128 Interrupt Sources:** Covering tất cả peripherals và subsystems của AM335x

---

## Ví dụ thực tế sử dụng INTC

### Example 1: Basic Interrupt Setup cho UART0

```c
// Define base addresses
#define INTC_BASE       0x48200000
#define UART0_IRQ       72

// Initialize INTC for UART0 interrupt
void setup_uart0_interrupt(void) {
    volatile uint32_t *intc_base = (uint32_t *)INTC_BASE;

    // 1. Enable interface clock auto-idle
    *(intc_base + 0x10/4) = 0x1;  // SYSCONFIG.AUTOIDLE = 1

    // 2. Set priority for UART0 (priority 10, route to IRQ)
    uint32_t ilr_offset = 0x100 + (UART0_IRQ * 4);
    *(intc_base + ilr_offset/4) = (10 << 2) | 0x0;  // Priority=10, IRQ

    // 3. Unmask UART0 interrupt (IRQ 72 is in MIR2)
    // MIR_CLEAR2 offset = 0xC8h, bit position = 72-64 = 8
    *(intc_base + 0xC8/4) = (1 << 8);  // Clear mask bit

    // 4. Set priority threshold to allow all
    *(intc_base + 0x68/4) = 0xFF;  // Disable threshold
}
```

### Example 2: Setup Multiple Interrupts với Priority

```c
// Setup GPIO0 (IRQ 96) and TIMER2 (IRQ 68) with different priorities
void setup_multiple_interrupts(void) {
    volatile uint32_t *intc_base = (uint32_t *)INTC_BASE;

    // GPIO0: High priority (5), route to IRQ
    *(intc_base + (0x100 + 96*4)/4) = (5 << 2) | 0x0;

    // TIMER2: Lower priority (15), route to IRQ
    *(intc_base + (0x100 + 68*4)/4) = (15 << 2) | 0x0;

    // Unmask GPIO0 (IRQ 96 is in MIR3, bit 0)
    *(intc_base + 0xE8/4) = (1 << 0);  // MIR_CLEAR3

    // Unmask TIMER2 (IRQ 68 is in MIR2, bit 4)
    *(intc_base + 0xC8/4) = (1 << 4);  // MIR_CLEAR2
}
```

### Example 3: Interrupt Handler với Preemption Support

```c
void irq_handler(void) {
    volatile uint32_t *intc_base = (uint32_t *)INTC_BASE;

    // Save current threshold
    uint32_t old_threshold = *(intc_base + 0x68/4);

    // Read active IRQ number
    uint32_t sir_irq = *(intc_base + 0x40/4);
    uint32_t active_irq = sir_irq & 0x7F;

    // Check spurious flag
    if (sir_irq & 0xFFFFFF80) {
        // Spurious interrupt, just acknowledge and return
        *(intc_base + 0x48/4) = 0x1;  // NewIRQAgr
        return;
    }

    // Read and set priority threshold for preemption
    uint32_t irq_priority = *(intc_base + 0x60/4) & 0x7F;
    *(intc_base + 0x68/4) = irq_priority;  // Set threshold

    // Acknowledge interrupt to allow preemption
    *(intc_base + 0x48/4) = 0x1;  // NewIRQAgr

    // Enable IRQs at ARM level for preemption
    __asm__ volatile("cpsie i");

    // Call specific interrupt handler
    switch(active_irq) {
        case 72:  // UART0
            uart0_irq_handler();
            break;
        case 96:  // GPIO0
            gpio0_irq_handler();
            break;
        // Add more cases as needed
    }

    // Disable IRQs before restoring threshold
    __asm__ volatile("cpsid i");

    // Restore original threshold
    *(intc_base + 0x68/4) = old_threshold;
}
```

### Example 4: Software Interrupt Trigger

```c
// Trigger software interrupt for testing
void trigger_software_interrupt(uint32_t irq_num) {
    volatile uint32_t *intc_base = (uint32_t *)INTC_BASE;

    // Determine which ISR_SET register (0-3) and bit position
    uint32_t reg_index = irq_num / 32;
    uint32_t bit_pos = irq_num % 32;

    // ISR_SET0=0x90, ISR_SET1=0xB0, ISR_SET2=0xD0, ISR_SET3=0xF0
    uint32_t isr_set_offsets[] = {0x90, 0xB0, 0xD0, 0xF0};

    // Set the software interrupt bit
    *(intc_base + isr_set_offsets[reg_index]/4) = (1 << bit_pos);
}

// Clear software interrupt
void clear_software_interrupt(uint32_t irq_num) {
    volatile uint32_t *intc_base = (uint32_t *)INTC_BASE;

    uint32_t reg_index = irq_num / 32;
    uint32_t bit_pos = irq_num % 32;

    // ISR_CLEAR0=0x94, ISR_CLEAR1=0xB4, ISR_CLEAR2=0xD4, ISR_CLEAR3=0xF4
    uint32_t isr_clear_offsets[] = {0x94, 0xB4, 0xD4, 0xF4};

    // Clear the software interrupt bit
    *(intc_base + isr_clear_offsets[reg_index]/4) = (1 << bit_pos);
}
```

### Example 5: Checking Pending Interrupts

```c
// Check if any interrupts are pending for a specific range
uint32_t check_pending_irqs(uint32_t start_irq, uint32_t end_irq) {
    volatile uint32_t *intc_base = (uint32_t *)INTC_BASE;

    // PENDING_IRQ0=0x98, 1=0xB8, 2=0xD8, 3=0xF8
    uint32_t pending_offsets[] = {0x98, 0xB8, 0xD8, 0xF8};

    uint32_t count = 0;

    for (uint32_t irq = start_irq; irq <= end_irq; irq++) {
        uint32_t reg_index = irq / 32;
        uint32_t bit_pos = irq % 32;

        uint32_t pending = *(intc_base + pending_offsets[reg_index]/4);

        if (pending & (1 << bit_pos)) {
            count++;
        }
    }

    return count;
}
```

### Example 6: Priority Threshold Configuration

```c
// Configure priority threshold to mask low-priority interrupts
void set_priority_threshold(uint8_t threshold) {
    volatile uint32_t *intc_base = (uint32_t *)INTC_BASE;

    // Valid threshold values: 0x00-0x3F, 0xFF to disable
    if (threshold <= 0x3F || threshold == 0xFF) {
        *(intc_base + 0x68/4) = threshold;
    }
}

// Example: Only allow high priority interrupts (0-9)
void allow_only_high_priority(void) {
    set_priority_threshold(9);  // Mask priorities 10-63
}

// Disable priority masking
void disable_priority_masking(void) {
    set_priority_threshold(0xFF);
}
```

### Example 7: Bulk Interrupt Mask Operations

```c
// Mask multiple interrupts at once
void mask_interrupts(uint32_t *irq_list, uint32_t count) {
    volatile uint32_t *intc_base = (uint32_t *)INTC_BASE;
    uint32_t mir_set_offsets[] = {0x8C, 0xAC, 0xCC, 0xEC};

    for (uint32_t i = 0; i < count; i++) {
        uint32_t irq = irq_list[i];
        uint32_t reg_index = irq / 32;
        uint32_t bit_pos = irq % 32;

        *(intc_base + mir_set_offsets[reg_index]/4) = (1 << bit_pos);
    }
}

// Unmask multiple interrupts at once
void unmask_interrupts(uint32_t *irq_list, uint32_t count) {
    volatile uint32_t *intc_base = (uint32_t *)INTC_BASE;
    uint32_t mir_clear_offsets[] = {0x88, 0xA8, 0xC8, 0xE8};

    for (uint32_t i = 0; i < count; i++) {
        uint32_t irq = irq_list[i];
        uint32_t reg_index = irq / 32;
        uint32_t bit_pos = irq % 32;

        *(intc_base + mir_clear_offsets[reg_index]/4) = (1 << bit_pos);
    }
}

// Example usage
void setup_peripheral_interrupts(void) {
    uint32_t irqs[] = {72, 73, 74, 96, 97, 98};  // UART0-2, GPIO0-2
    unmask_interrupts(irqs, 6);
}
```

### Example 8: Reading Interrupt Status

```c
// Get detailed interrupt status
typedef struct {
    uint32_t raw_status;      // Before masking
    uint32_t pending_irq;     // After masking (IRQ)
    uint32_t pending_fiq;     // After masking (FIQ)
    uint32_t active_irq;      // Currently active IRQ
    uint32_t active_priority; // Priority of active IRQ
} intc_status_t;

void get_intc_status(uint32_t irq_num, intc_status_t *status) {
    volatile uint32_t *intc_base = (uint32_t *)INTC_BASE;

    uint32_t reg_index = irq_num / 32;
    uint32_t bit_pos = irq_num % 32;

    // Read raw interrupt status (ITRn registers)
    uint32_t itr_offsets[] = {0x80, 0xA0, 0xC0, 0xE0};
    status->raw_status = *(intc_base + itr_offsets[reg_index]/4);

    // Read pending IRQ status
    uint32_t pending_irq_offsets[] = {0x98, 0xB8, 0xD8, 0xF8};
    status->pending_irq = *(intc_base + pending_irq_offsets[reg_index]/4);

    // Read pending FIQ status
    uint32_t pending_fiq_offsets[] = {0x9C, 0xBC, 0xDC, 0xFC};
    status->pending_fiq = *(intc_base + pending_fiq_offsets[reg_index]/4);

    // Read active IRQ number
    status->active_irq = *(intc_base + 0x40/4) & 0x7F;

    // Read active IRQ priority
    status->active_priority = *(intc_base + 0x60/4) & 0x7F;
}
```

---

## Debug Tips và Best Practices

### 1. Spurious Interrupt Handling
- Luôn kiểm tra spurious flag trong ISR
- Spurious interrupts có thể xảy ra khi mask thay đổi trong sorting window
- Acknowledge ngay và return nếu detect spurious

### 2. Priority Configuration
- Ưu tiên cao (0-9): Critical real-time interrupts
- Ưu tiên trung bình (10-31): Normal peripheral interrupts
- Ưu tiên thấp (32-63): Background/maintenance interrupts

### 3. Preemption Guidelines
- Chỉ enable preemption khi thực sự cần thiết
- Luôn save/restore threshold và ARM context
- Sử dụng Data Synchronization Barrier trước enable IRQ

### 4. Performance Optimization
- Enable TURBO mode (IDLE[1]=1) cho low-latency applications
- Sử dụng FuncIdle (IDLE[0]=0) cho power-critical applications
- Configure AutoIdle (SYSCONFIG[0]=1) cho OCP clock gating

### 5. Common Mistakes to Avoid
- Không acknowledge interrupt (NewIRQAgr)
- Thay đổi MIR trong interrupt sorting window
- Quên clear interrupt condition ở peripheral
- Không kiểm tra spurious flag
- Priority threshold quá cao, blocking critical interrupts

---

**Document Version:** 1.0
**Target Device:** Texas Instruments AM335x (ARM Cortex-A8)
**Created for:** AI Training and Embedded Systems Development
