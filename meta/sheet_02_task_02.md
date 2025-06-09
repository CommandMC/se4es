> [!NOTE]
> This file is best viewed as Markdown (using GitHub formatting)

## 1. CFG-Reconstruction

### Block "init":
```c++
int b = a >> 2;
```

### Block "loop":
```c++
for(int i = 0; i < a; ++i)
```

### Block "loopcontent":
```c++
a -= b;
```

### Block "ret":
```c++
return a + b;
```

## 2. Value analysis:
### "init":
```
a ∈ [0, 42]
b ∈ [0, 10]
```

### "loop"
```
a ∈ [0, 42]
b ∈ [0, 10]
i ∈ [0, 41]
```

### "loopcontent"
> [!NOTE]
> This disregards b assignment logic and just assumes it can always range from 0 to 10
```
a ∈ [-9, 42]
b ∈ [0, 10]
i ∈ [0, 41]
```

> [!NOTE]
> Not sure how to continue without control flow analysis here, since the value ranges depend on how many times the loop executes. The exercise kind of interrupted here to work on control flow analysis first

### "ret"
> [!NOTE]
> The range for `a` was calculated using a small test application, running through the function and collecting possible values. In case this was not the intended solution: Since "ret" does not change a, the same range as specified in "loop"/"loopcontent" apply.
```
a ∈ [0, 3]
b ∈ [0, 10]
```

## 3. Control flow analysis

### Lower bound:
```
a = 0
b = 0
```
Loop executes 0 times

### Upper bound:
> [!NOTE]
> This situation can't actually happen (since the value of b depends on a), but it's guaranteed to give us a worst case
```
a = 42
b = 0
```
Loop executes 42 times

## 4. µ-architectural analysis

```
a -> r13
b -> r15
i -> r14
```
r12 is
1. function argument (initial a)
2. return value

### "init":
```
    800e:       0d 4c           mov     r12,    r13     ; 1 cycle  (MSP430 Format I instr, Rn to Rm)
    8010:       0f 4c           mov     r12,    r15     ; 1 cycle  (MSP430 Format I instr, Rn to Rm)
    8012:       5f 05           rram    #2,     r15     ; 2 cycles (MSP430X Format II instr)
```
### Total cycles for this block: 4

---

### "loop"
```
    ; Make sure we have to enter the loop at all (a != 0)
    8014:       4c 43           clr.b   r12             ; 2 cycles (MSP430 Format I instr, #N to Rm, emulated as MOV.b #0,r12)
    8016:       0c 9d           cmp     r13,    r12     ; 1 cycle  (MSP430 Format I instr, Rn to Rm)
    8018:       07 34           jge     $+16            ; 2 cycles ("All jump instructions [...] take two CPU cycles to execute)
    ; Initialize i to 0
    801a:       4e 43           clr.b   r14             ; 2 cycles (MSP430 Format I instr, #N to Rm, emulated as MOV.b #0,r14)
```
### Total cycles for this block: 7

---

### "loopcontent"
```
    ; Optimization here: Store the old value of a in the return value register, if we end up quitting out of the loop in this iteration, just return (simplifies 'a - b + b' to just 'a')
    801c:       0c 4d           mov     r13,    r12     ; 1 cycle  (MSP430 Format I instr, Rn to Rm)
    ; The actual operation (a -= b)
    801e:       0d 8f           sub     r15,    r13     ; 1 cycle  (MSP430 Format I instr, Rn to Rm)
    ; increment i
    8020:       1e 53           inc     r14             ; 2 cycles (MSP430 Format I instr, #N to Rm, emulated as ADD.b #1,r14)
    ; return to start of loop if i < a
    8022:       0e 9d           cmp     r13,    r14     ; 1 cycle  (MSP430 Format I instr, Rn to Rm)
    8024:       fb 3b           jl      $-8             ; 2 cycles ("All jump instructions [...] take two CPU cycles to execute)  Jumps back to loop start
```
### Total cycles for this block: 7

---

### "ret"
"ret" has two possible "forms", depending on whether we executed the loop at all  
Case where loop was entered:
```
    ; Right after `jl $-8` in loopcontent
    ; continuation from optimization in loopcontent: Just return r12 (the value of a before subtracting b)
    8026:       30 41           ret                     ; 4 cycles (MSP430 Format I instr, @Rn+ to SP, emulated as MOV @SP+,PC)
```
Case where loop wasn't entered:
```
    ; At 0x8028, target of `jge $+16` in loop init
    ; Copy a to r12, add b, then return
    8028:       0c 4d           mov     r13,    r12     ; 1 cycle  (MSP430 Format I instr, Rn to Rm)
    802a:       0c 5f           add     r15,    r12     ; 1 cycle  (MSP430 Format I instr, Rn to Rm)
    802c:       30 41           ret                     ; 4 cycles (MSP430 Format I instr, @Rn+ to SP, emulated as MOV @SP+,PC)
```
### We will split this block into two blocks, "ret4c" and "ret6c" (taking 4 and 6 cycles respectively)

## 5. Global bound analysis

> [!NOTE]
> Due to the code re-ordering and optimizations done by the compiler in step 4, we will rename the block "loop" to "skip_loop", and "loopcontent" to "loop", to better match the examples presented in the lecture/exercise.  
> Additionally, we split the "loop" block into 3 smaller blocks, which clarifies their intent: (1) incrementing i, (2) performing the subtraction and (3) deciding whether to jump back. In case this is not intended/allowed, an alternative graph is provided
```
┌───────────┐
│    init   │
└───────────┘
      ║
      🮦
┌───────────┐   ┌───────────┐
│ skip_loop?│══🮥│    ret6   │
└───────────┘   └───────────┘
      ║
      🮦
┌───────────┐
│   inc_i   │🮤═╗
└───────────┘  ║
      ║        ║
      🮦        ║
┌───────────┐  ║
│    sub    │  ║
└───────────┘  ║
      ║        ║
      🮦        ║
┌───────────┐  ║
│ finished? │══╝
└───────────┘
      ║
      🮦
┌───────────┐
│    ret4   │
└───────────┘
```
Alternative graph without split `loop` block:
```
┌───────────┐
│    init   │
└───────────┘
      ║
      🮦
┌───────────┐   ┌───────────┐
│ skip_loop?│══🮥│    ret6   │
└───────────┘   └───────────┘
      ║
      🮦
┌───────────┐══╗
│   loop?   │  ║
└───────────┘🮤═╝
      ║
      🮦
┌───────────┐
│    ret4   │
└───────────┘

```

### ILP:
```
init = skip_loop = 1
ret4 + ret6 = 1
inc_i = sub = finished
LB <= inc_i <= UB
all >= 0

Goal function: init * 4 + skip_loop * 7 + inc_i * 1 + sub * 1 + finished * 5 + ret4 * 4 + ret6 * 6
```
Again without split `loop`:
```
init = skip_loop = 1
ret4 + ret6 = 1
LB <= loop <= UB
all >= 0

Goal function: init * 4 + skip_loop * 7 + loop * 7 + ret4 * 4 + ret6 * 6
```
