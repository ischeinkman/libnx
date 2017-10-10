.macro SVC_BEGIN name
	.section .text.\name, "ax", %progbits
	.global \name
	.type \name, %function
	.align 2
	.cfi_startproc
\name:
.endm

.macro SVC_END
	.cfi_endproc
.endm

SVC_BEGIN svcSetHeapSize
	str x0, [sp, #-16]!
	svc 0x1
	ldr x2, [sp], #16
	str x1, [x2]
	ret
SVC_END

SVC_BEGIN svcMapMemory
	svc 0x4
	ret
SVC_END

SVC_BEGIN svcUnmapMemory
	svc 0x5
	ret
SVC_END

SVC_BEGIN svcQueryMemory
	str x1, [sp, #-16]!
	svc 0x6
	ldr x2, [sp], #16
	str w1, [x2]
	ret
SVC_END

SVC_BEGIN svcExitProcess
	svc  0x7
	ret
SVC_END

SVC_BEGIN svcCreateThread
    str x0, [sp, #-16]!
	svc 0x8
    ldr x2, [sp]
    str w1, [x2]
    add sp, sp, #16
	ret
SVC_END

SVC_BEGIN svcStartThread
	svc  0x9
	ret
SVC_END

SVC_BEGIN svcExitThread
	svc  0xA
	ret
SVC_END

SVC_BEGIN svcSleepThread
	svc 0xB
	ret
SVC_END

SVC_BEGIN svcClearEvent
	svc 0x12
	ret
SVC_END

SVC_BEGIN svcCloseHandle
	svc 0x16
	ret
SVC_END

SVC_BEGIN svcCreateTransferMemory
	str x0, [sp, #-16]!
	svc 0x15
	ldr x2, [sp], #16
	str w1, [x2]
	ret
SVC_END

SVC_BEGIN svcWaitSynchronization
	str x0, [sp, #-16]!
	svc 0x18
	ldr x2, [sp], #16
	str w1, [x2]
	ret
SVC_END

SVC_BEGIN svcArbitrateLock
    svc 0x1a
SVC_END

SVC_BEGIN svcArbitrateUnlock
    svc 0x1b
SVC_END

SVC_BEGIN svcConnectToNamedPort
	str x0, [sp, #-16]!
	svc 0x1F
	ldr x2, [sp], #16
	str w1, [x2]
	ret
SVC_END

SVC_BEGIN svcSendSyncRequest
	svc 0x21
	ret
SVC_END

SVC_BEGIN svcBreak
	svc 0x26
	ret
SVC_END

SVC_BEGIN svcGetInfo
	str x0, [sp, #-16]!
	svc 0x29
	ldr x2, [sp], #16
	str x1, [x2]
	ret
SVC_END

SVC_BEGIN svcCreateSession
	stp x0, x1, [sp, #-16]!
	svc 0x40
	ldr x3, [sp], #8
	str w1, [x3]
	ldr x3, [sp], #8
	str w2, [x3]
	ret
SVC_END

SVC_BEGIN svcAcceptSession
	str x0, [sp, #-16]!
	svc 0x41
	ldr x2, [sp], #16
	str w1, [x2]
	ret
SVC_END

SVC_BEGIN svcReplyAndReceive
	str x0, [sp, #-16]!
	svc 0x43
	ldr x2, [sp], #16
	str w1, [x2]
	ret
SVC_END

SVC_BEGIN svcMapTransferMemory
	svc 0x51
	ret
SVC_END

SVC_BEGIN svcUnmapTransferMemory
	svc 0x52
	ret
SVC_END

SVC_BEGIN svcQueryPhysicalAddress
	str x0, [sp, #-16]!
	svc 0x54
	ldr x2, [sp], #16
	str x1, [x2]
	str x2, [x2, #8]
	str x3, [x2, #16]
	ret
SVC_END

SVC_BEGIN svcQueryIoMapping
	str x0, [sp, #-16]!
	svc 0x55
	ldr x2, [sp], #16
	str x1, [x2]
	ret
SVC_END

SVC_BEGIN svcCreateDeviceAddressSpace
	str x0, [sp, #-16]!
	svc 0x56
	ldr x2, [sp]
	str w1, [x2]
	add sp, sp, #0x10
	ret
SVC_END

SVC_BEGIN svcAttachDeviceAddressSpace
	svc 0x57
	ret
SVC_END

SVC_BEGIN svcDetachDeviceAddressSpace
	svc 0x58
	ret
SVC_END

SVC_BEGIN svcMapDeviceAddressSpaceAligned
	svc 0x5A
	ret
SVC_END

SVC_BEGIN svcUnmapDeviceAddressSpace
	svc 0x5C
	ret
SVC_END

SVC_BEGIN svcDebugActiveProcess
	str x0, [sp, #-16]!
	svc 0x60
	ldr x2, [sp], #16
	str w1, [x2]
	ret
SVC_END

SVC_BEGIN svcContinueDebugEvent
	svc 0x64
	ret
SVC_END

SVC_BEGIN svcQueryDebugProcessMemory
	str x1, [sp, #-16]!
	svc 0x69
	ldr x2, [sp], #16
	str w1, [x2]
	ret
SVC_END

SVC_BEGIN svcReadDebugProcessMemory
	svc 0x6A
	ret
SVC_END

SVC_BEGIN svcManageNamedPort
	str x0, [sp, #-16]!
	svc 0x71
	ldr x2, [sp], #16
	str w1, [x2]
	ret
SVC_END

SVC_BEGIN svcCallSecureMonitor
	str x0, [sp, #-16]!
	mov x8, x0
	ldp x0, x1, [x8]
	ldp x2, x3, [x8, #0x10]
	ldp x4, x5, [x8, #0x20]
	ldp x6, x7, [x8, #0x30]
	svc 0x7F
	ldr x8, [SP]
	stp x0, x1, [x8]
	stp x2, x3, [x8, #0x10]
	stp x4, x5, [x8, #0x20]
	stp x6, x7, [x8, #0x30]
	add sp, sp, #0x10
	ret
SVC_END
