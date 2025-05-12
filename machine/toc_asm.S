.global toc_go
.global toc_switch
.global toc_pop_parameter

        ;; Schaltet hart auf einen Task um, Rueckkehr zum
        ;; Aufrufer ist nicht vorgesehen
toc_go:
#ifdef TI_COMPILER
        mov     @r12, r1
#else
        mov     @r15, r1
#endif
        jmp     setupregs

        ;; Schaltet zwischen zwei Prozessen um
toc_switch:
        ;; vom Aufgerufenen zu sichernde Register auf den Stack legen
        .irp    reg,4,5,6,7,8,9,10,11
        push    r\reg
        .endr

        ;; alten Stackpointer sichern
#ifdef TI_COMPILER
        mov     r1, @r12
#else
        mov     r1, @r15
#endif

        ;; neuen Stackpointer lesen
#ifdef TI_COMPILER
        mov     @r13, r1
#else
        mov     @r14, r1
#endif

setupregs:
        ;; Register vom neuen Stack restaurieren
        .irp    reg,11,10,9,8,7,6,5,4
        pop     r\reg
        .endr

        ;; zum neuen Task zurueckkehren
        ret

        ;; einen Funktionsparameter vom Stack holen und in die Zielfunktion
        ;; springen (notwendig, da auf msp430 die ersten 4 Parameter in
        ;; Registern uebergeben werden)
toc_pop_parameter:
#ifdef TI_COMPILER
        pop     r12
#else
        pop     r15
#endif
        ret
