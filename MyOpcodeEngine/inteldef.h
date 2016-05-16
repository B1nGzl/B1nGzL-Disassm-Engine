#pragma once
#include <windows.h>
//////////////////////////////////////////////////////////////////////////
#define MODE_16BIT				0
#define MODE_32BIT				1

#define sign_BYTE		0x80
#define sign_WORD		0x8000
#define sign_DWORD		0x80000000
#define shift_BYTE		0x07
#define shift_WORD		0x0f
#define shift_DWORD		0x1f
#define max_BYTE		0xff
#define max_WORD		0xffff
#define max_DWORD		0xffffffff
#define min_BYTE		0x7f
#define min_WORD		0x7fff
#define min_DWORD		0x7fffffff

#ifndef BX_BIG_ENDIAN
#define GetValue_b(p) (*(BYTE*)(p))
#define GetValue_w(p) (*(WORD*)(p))
#define GetValue_d(p) (*(DWORD*)(p))
#define SetValue_b(p,value) (*(BYTE*)(p) = (value)) 
#define SetValue_w(p,value) (*(WORD*)(p) = (value)) 
#define SetValue_d(p,value) (*(DWORD*)(p) = (value)) 
#endif

typedef char	BIT8S;
typedef short	BIT16S;
typedef int		BIT32S;

#define I_EAX	0
#define I_ECX	1
#define I_EDX	2
#define I_EBX	3
#define I_ESP	4
#define I_EBP	5
#define I_ESI	6
#define I_EDI	7

#define I_ES	0
#define I_CS	1
#define I_SS	2
#define I_DS	3
#define I_FS	4
#define I_GS	5
#define I_6S	6
#define I_7S	7

#define INTEL_GEG_REG_EAX	0
#define INTEL_GEG_REG_ECX	1
#define INTEL_GEG_REG_EDX	2
#define INTEL_GEG_REG_EBX	3
#define INTEL_GEG_REG_ESP	4
#define INTEL_GEG_REG_EBP	5
#define INTEL_GEG_REG_ESI	6
#define INTEL_GEG_REG_EDI	7

#define R_M		0x0200

typedef enum _IMMEDIATE___
{
    Immediate_,			// 8 bits regardless
    Immediate_1,		// shl eax,1
    Immediate_CL,		// shl eax,cl
    Immediate_Ib,       // 8 bits regardless
    Immediate_sIb,		// sign extend to OS size
    Immediate_zIb,		// zero extend to OS size
    Immediate_Iv,       // 16 or 32 depending on OS size
    Immediate_Iw,       // 16 bits regardless
    Immediate_IvIw,     // call_Ap
    Immediate_IwIb,     // enter_IwIb
    Immediate_O,        // mov_ALOb, mov_ObAL, mov_eAXOv, mov_OveAX
    Immediate_BrOff8,   // Relative branch offset byte
    Immediate_BrOff16,  // Relative branch offset word
    Immediate_BrOff32,  // Relative branch offset dword
    Immediate_Jb,	    // Relative branch offset byte
    Immediate_Jv,	    // Relative branch offset word/dword


    PREFIX_ES,		// 0x26 segment override prefix.
    PREFIX_CS,		// 0x2e segment override prefix.
    PREFIX_SS,		// 0x36 segment override prefix.
    PREFIX_DS,		// 0x3e segment override prefix.
    PREFIX_FS,		// 0x64 segment override prefix.
    PREFIX_GS,		// 0x65 segment override prefix.
    PREFIX_66,		// 0x66 Operand-size override prefix.
    PREFIX_67,		// 0x67 Address-size override prefix.
    PREFIX_F0,		// 0xf0 LOCK
    PREFIX_F2,		// 0xf2 REPNE/REPNZ (used only with string instructions).
    PREFIX_F3,		// 0xf3 REPE/REPZ (use only with string instructions).

    STRING_Move,	// movs stos lods ins outs (use only with string instructions).
    STRING_Comp,	// cmps scan (use only with string instructions).

    Extended_2byte, // pop cs (use only with two byte instructions).
    Extended_Group, // mov al,0x80 (use only with extent opcode byte instructions).
    Extended_Float, // fld [bx]  (use only with float exten byte instructions).
    Extended_Group1,// prefetch (use only with extent opcode byte instructions).

    OPCODE_UD,
};

typedef enum _OPCODETYPE
{
    //----------------------------------------------
    //----------------------------------------------
    C_NULL,
    C_ES, C_CS, C_SS, C_DS, C_FS, C_GS, // 0x26 0x2e 0x36 0x3e 0x64 0x65 segment override
    // 0x2e 0x3e branch hint prefixes
    // 0x2e Branch not taken (used only with Jcc instructions).
    // 0x3e Branch taken (used only with Jcc instructions).
    //----------------------------------------------
    C_OPER, // 0x66 Operand-size override prefix.
    C_ADDR, // 0x67 Address-size override prefix.
    C_LOCK, // 0xf0 LOCK
    C_REPN, // 0xf2 REPNE/REPNZ (used only with string instructions).
    C_REPZ, // 0xf3 REPE/REPZ (use only with string instructions).

    //----------------------------------------------

    C_GRP, C_ESC, C_2BYTE, C_GRP1,
    //----------------------------------------------

    C_MOVS, C_STOS, C_LODS, C_INS, C_OUTS,
    C_CMPS, C_SCAS,
    C_IN, C_OUT,

    //----------------------------------------------
    C_LES, C_LCS, C_LSS, C_LDS, C_LFS, C_LGS,
    //----------------------------------------------
    C_LOOPNZ, C_LOOPZ, C_LOOP, C_JCXZ, C_JMP,
    //----------------------------------------------

    C_MOVUPS, C_MOVUSS, C_MOVLPS, C_UNPCKLPS, C_UNPCKHPS, C_MOVHLPS, C_MOVHPS,

    C_CVTPS2PS, C_CVTPS2SS, C_CVTTPS2PI,
    C_CVTTPS2SI, C_CVTPS2PI, C_CVTPS2SI,
    C_SQRTPS, C_SQRTSS,
    C_RSQRTPS, C_RSQRTSS,
    C_RCPPS, C_RCPSS,
    C_ADDPS, C_ADDSS,
    C_MULPS, C_MULSS,
    C_SUBPS, C_SUBSS,
    C_MINPS, C_MINSS,
    C_DIVPS, C_DIVSS,
    C_MAXPS, C_MAXSS,
    C_CMPPS, C_CMPSS,
    //----------------------------------------------
    //----------------------------------------------
    C_ADD, C_OR, C_ADC, C_SBB, C_AND, C_SUB, C_XOR, C_CMP,
    C_ROL, C_ROR, C_RCL, C_RCR, C_SHL, C_SHR, C_SAL, C_SAR,
    //----------------------------------------------
    //----------------------------------------------

    C_DAA, C_DAS, C_AAA, C_AAS,
    C_INC, C_DEC, C_PUSH, C_POP, C_PUSHA, C_POPA,
    C_BOUND, C_ARPL, C_IMUL,

    //----------------------------------------------
    //----------------------------------------------

    C_JtO,
    C_JfO,
    C_JtC,
    C_JfC,
    C_JtZ,
    C_JfZ,
    C_JfA,
    C_JtA,
    C_JtS,
    C_JfS,
    C_JtP,
    C_JfP,
    C_JtL,
    C_JfL,
    C_JfG,
    C_JtG,

    C_SETtO,
    C_SETfO,
    C_SETtC,
    C_SETfC,
    C_SETtZ,
    C_SETfZ,
    C_SETfA,
    C_SETtA,
    C_SETtS,
    C_SETfS,
    C_SETtP,
    C_SETfP,
    C_SETtL,
    C_SETfL,
    C_SETfG,
    C_SETtG,
    C_CMOVtO,
    C_CMOVfO,
    C_CMOVtC,
    C_CMOVfC,
    C_CMOVtZ,
    C_CMOVfZ,
    C_CMOVfA,
    C_CMOVtA,
    C_CMOVtS,
    C_CMOVfS,
    C_CMOVtP,
    C_CMOVfP,
    C_CMOVtL,
    C_CMOVfL,
    C_CMOVfG,
    C_CMOVtG,

    //----------------------------------------------
    //----------------------------------------------

    C_TEST, C_NOP, C_XCHG, C_MOV, C_LEA, C_CBW, C_CWD, C_CALL, C_WAIT,

    C_PUSHF, C_POPF, C_SAHF, C_LAHF,

    C_RET, C_ENTER, C_LEAVE, C_RETF, C_INT1, C_INT3, C_INT, C_INTO, C_IRET,

    C_AAM, C_AAD,

    C_SALC, C_XLAT,

    C_HLT, C_CMC, C_CLC, C_STC, C_CLI, C_STI, C_CLD, C_STD,

    C_NOT, C_NEG, C_MUL, C_DIV, C_IDIV,
    // One-byte opcode table completed.


    //FPU Instruction
    C_FSTCW, C_FSTENV, C_FCLEX, C_FINIT, C_FSAVE, C_FSTSW, C_FWAIT,


    C_FADD, C_FMUL, C_FCOM, C_FCOMP, C_FSUB, C_FSUBR, C_FDIV, C_FDIVR,

    C_FLD, C_FXCH, C_FNOP, C_FCHS, C_FABS, C_FTST,
    C_FLD1, C_FLDL2T, C_FLDL2E, C_FLDPI, C_FLDLG2, C_FLDLN2,

    C_FLDZ,
    C_F2XM1,
    C_FPTAN,
    C_FPATAN,
    C_FXTRACT,
    C_FPREM1,
    C_FDECSTP,

    C_FINCSTP,
    C_FYL2XP1,
    C_FSQRT,

    C_FSINCOS,
    C_FRNDINT,
    C_FSCALE,
    C_FSIN,
    C_FCOS,

    C_FLDCW,
    C_FLDENV,
    C_FNSTCW,
    C_FNSTENV,
    C_FST,
    C_FSTP,
    C_FCMOVB,
    C_FCMOVE,
    C_FCMOVBE,
    C_FCMOVU,
    C_FUCOMPP,

    C_FIADD, C_FICOM, C_FICOMP, C_FIDIV, C_FIDIVR, C_FIMUL, C_FISUB, C_FISUBR,

    C_FCMOVNB,
    C_FCMOVNE,
    C_FCMOVNBE,
    C_FCMOVNU,
    C_FNCLEX,
    C_FNINIT,
    C_FUCOMI,
    C_FCOMI,
    C_FILD,
    C_FIST,
    C_FISTP,

    C_FFREE,
    C_FUCOM,
    C_FUCOMP,

    C_FNSAVE,
    C_FNSTSW,
    C_FRSTOR,
    C_FADDP,
    C_FMULP,
    C_FCOMPP,
    C_FSUBRP,
    C_FSUBP,
    C_FDIVRP,
    C_FDIVP,

    C_FUCOMIP,
    C_FCOMIP,
    C_FBLD,
    C_FBSTP,

    C_FXAM,
    C_FLD2T,
    C_FLDP1,
    C_FYL2X,
    C_FPREM,

    //386 CPU Instruction
    //----------------------------------------------
    //----------------------------------------------
    C_SLDT, C_STR, C_LLDT, C_LTR, C_VERR, C_VERW,
    C_SGDT, C_SIDT, C_LGDT, C_LIDT, C_SMSW, C_LMSW,
    //----------------------------------------------
    //----------------------------------------------

    C_LAR,
    C_LSL,
    C_MOVZX,
    C_MOVSX,

    C_RSM,//0F AA RSM Resume operation of interrupted program
    //Resume from System Management Mode
    C_BT,
    C_BTR,
    C_BTS,
    C_BTC,
    C_BSF,
    C_BSR,
    C_SHLD,
    C_SHRD,
    C_CMPXCHG_XBTS,
    C_CMPXCHG_IBTS,

    C_CMPXCHG,
    C_XADD,
    C_BSWAP,//Byte Swap
    C_CLTS,
    C_CPUID, //0F A2 CPUID Returns processor identification and feature information
    //	to the EAX, EBX, ECX, and EDX registers, according to
    //	the input value entered initially in the EAX register

    C_MOVAPS,
    C_CVTPI2PS,
    C_MOVNTPS,
    C_UCOMISS,
    C_COMISS,

    C_MOVMSKPS,
    C_ANDPS,
    C_ANDNPS,
    C_ORPS,
    C_XORPS,

    C_MOVNTI,
    C_PINSRW,
    C_PEXTRW,
    C_SHUFPS,

    C_PSRLW,
    C_PSRLD,
    C_PSRLQ,
    C_PADDQ,
    C_PMULLW,
    C_MOVQ,
    C_PMOVMSKB,

    C_CVTPS2PD,
    C_CVTDQ2PS,

    C_PSHUFW,
    C_PCMPEQB,
    C_PCMPEQW,
    C_PCMPEQD,
    C_EMMS,


    C_PREFETCH,
    C_FXSAVE,
    C_FXRSTOR,
    C_IDMXXCR,
    C_STMXCSR,

    C_WRMSR, //0F 30 WRMSR Write the value in EDX:EAX to MSR specified by ECX
    C_RDTSC, //0F 31 RDTSC Read time-stamp counter into EDX:EAX
    C_RDMSR, //0F 32 RDMSR Load MSR specified by ECX into EDX:EAX
    C_RDPMC, //0F 33 RDPMC Read performance-monitoring counter specified by ECX	into EDX:EAX
    C_SYSENTER, //0F 34 Fast call to privilege level 0 system procedures
    C_SYSEXIT, //0F 35 Fast return to privilege level 3 user code

    C_INVD, //0F 08 INVD Flush internal caches; initiate flushing of external caches.
    C_WBINVD, //0F 09 WBINVD Write back and flush Internal caches; initiate writing-back
    //and flushing of external caches

    C_INVLPG,
    C_CMPXCH8B,


    C_PUNPCKLBW,
    C_PUNPCKLWD,
    C_PUNPCKLDQ,
    C_PACKSSWB,
    C_PCMPGTB,
    C_PCMPGTW,
    C_PCMPGTD,
    C_PACKUSWB,

    C_PUNPCKHBW,
    C_PUNPCKHWD,
    C_PUNPCKHDQ,
    C_PACKSSDW,
    C_PUNPCKLQDQ,
    C_PUNPCKHQD,
    C_MOVD,

    C_PSUBB,
    C_PSUBW,
    C_PSUBD,
    C_PADDB,
    C_PADDW,
    C_PADDD,

    C_PSUBUSB,
    C_PSUNUSW,
    C_PMINUB,
    C_PAND,
    C_PADDUSB,
    C_PADDUSW,
    C_PMAXUB,
    C_PANDN,

    C_PAVGB,
    C_PSRAW,
    C_PSRAD,
    C_PAVGW,
    C_PMULHUW,
    C_PMULHW,
    C_CVTPD2DQ,
    C_MOVNTQ,

    C_PSUBSB,
    C_PSUBSW,
    C_PMINSW,
    C_POR,
    C_PADDSB,
    C_PADDSW,
    C_PMAXSW,
    C_PXOR,

    C_PSLLW,
    C_PSLLD,
    C_PSLLQ,
    C_PMULUDQ,
    C_PMADDWD,
    C_PSADBW,
    C_MASKMOVQ,

    C_CLFLUSH,
    C_LFENCE,
    C_MFENCE,
    C_SFENCE,


    // Two-byte opcode table completed.
    //---------------------------------------
    C_DBYTE,
    C_DWORD,
    C_DDWORD,
    C_TAIL

};

typedef enum
{
    D__NONE,

    // The reg field of the ModR/M byte selects a control register (for example,
    // MOV (0F20, 0F22)).
    D__Cd,

    // The reg field of the ModR/M byte selects a debug register (for example,
    // MOV (0F21,0F23)).
    D__Dd,

    // The reg field of the ModR/M byte selects a test register (for example,
    // MOV (0F24,0F26)).
    D__Td,

    // EFLAGS Register.
    D__Fv,

    // The reg field of the ModR/M byte selects a general register (for example, AX (000)).	
    D__Gb, D_rGb, D_wGb,
    D__Gw, D_rGw, D_wGw,
    D__Gv, D_rGv, D_wGv,
    D__Gd, D_rGd, D_wGd,

    // Immediate data. The operand value is encoded in subsequent bytes of the instruction.
    D__Ib, D_zIb, D_sIb, D__Iw, D__Id, D__Iv, D__1,
    // The instruction contains a relative offset to be added to the instruction pointer register
    // (for example, JMP (0E9), LOOP).
    D__Jb, D__Jv,

    // The instruction has no ModR/M byte; the offset of the operand is coded as a word or
    // double word (depending on address size attribute) in the instruction. No base register,
    // index register, or scaling factor can be applied (for example, MOV (A0-A3)).
    D__Ob, D_rOb, D_wOb,
    D__Ov, D_rOv, D_wOv,

    // Direct address. The instruction has no ModR/M byte; the address of the operand is en-coded
    // in the instruction; and no base register, index register, or scaling factor can be
    // applied (for example, far JMP (EA)).	
    D__Ap,


    // The reg field of the ModR/M byte selects a packed quadword MMX?technology reg-ister.
    D__Pd, D__Pq, D__Ppi,

    // The mod field of the ModR/M byte may refer only to a general register (for example,
    // MOV (0F20-0F24, 0F26)).
    D__Rd,

    // The reg field of the ModR/M byte selects a segment register (for example, MOV
    //(8C,8E)).
    D__Sw,

    //unkown opdata
    D__NTA,

    // The reg field of the ModR/M byte selects a test register (for example, MOV
    // (0F24,0F26)).
    D__T0, D__T1, D__T2, D__T3, D__T4, D__T5, D__T6, D__T7,

    // The reg field of the ModR/M byte selects a packed SIMD floating-point register.
    D__Vq, D__Vps, D__Vss,

    // Memory addressed by the DS:SI register pair (for example, MOVS, CMPS, OUTS, or
    // LODS).
    D__Xb, D_rXb, D_wXb,
    D__Xw, D_rXw, D_wXw,
    D__Xv, D_rXv, D_wXv,
    D__Xd, D_rXd, D_wXd,

    // Memory addressed by the ES:DI register pair (for example, MOVS, CMPS, INS,
    //STOS, or SCAS).
    D__Yb, D_rYb, D_wYb,
    D__Yw, D_rYw, D_wYw,
    D__Yv, D_rYv, D_wYv,
    D__Yd, D_rYd, D_wYd,


    // When an operand is a specific register encoded in the opcode, the register is identified by its
    // name (for example, AX, CL, or ESI). The name of the register indicates whether the register is
    // 32, 16, or 8 bits wide. A register identifier of the form eXX is used when the width of the register
    // depends on the operand-size attribute. For example, eAX indicates that the AX register is used
    // when the operand-size attribute is 16, and the EAX register is used when the operand-size at-tribute
    // is 32.	

    D__AL, D__CL, D__DL, D__BL, D__AH, D__CH, D__DH, D__BH,

    D__AX, D__CX, D__DX, D__BX, D__SP, D__BP, D__SI, D__DI,

    D__eAX, D__eCX, D__eDX, D__eBX, D__eSP, D__eBP, D__eSI, D__eDI,
#define D__eX	D__eAX

    D__EAX, D__ECX, D__EDX, D__EBX, D__ESP, D__EBP, D__ESI, D__EDI,

    D__ES, D__CS, D__SS, D__DS, D__FS, D__GS,

    D__st,
    D__st0, D__st1, D__st2, D__st3, D__st4, D__st5, D__st6, D__st7,


    // A ModR/M byte follows the opcode and specifies the operand. The operand is either a
    // general-purpose register or a memory address. If it is a memory address, the address is
    // computed from a segment register and any of the following values: a base register, an
    // index register, a scaling factor, a displacement.		
    D__Eb, D_rEb, D_wEb,
    D__Ew, D_rEw, D_wEw,
    D__Ev, D_rEv, D_wEv,
    D__Ed, D_rEd, D_wEd,
    D__Ep, D_rEp,

    // The ModR/M byte may refer only to memory (for example, LEA,BOUND, LES, LDS, LSS,
    // LFS, LGS, CMPXCHG8B).
    D__M,
    D__Ma, D_rMa,
    D__Mb,
    D__Mp, D_rMp,
    D__Ms, D_rMs, D_wMs,
    D__Mq,


    // A ModR/M byte follows the opcode and specifies the operand. The operand is either
    // an MMX?technology register or a memory address. If it is a memory address, the ad-dress
    // is computed from a segment register and any of the following values: a base reg-ister,
    // an index register, a scaling factor, and a displacement.
    D__Qd, D__Qq, D__Qpi,

    // An ModR/M byte follows the opcode and specifies the operand. The operand is either
    // a SIMD floating-point register or a memory address. If it is a memory address, the ad-dress
    // is computed from a segment register and any of the following values: a base reg-ister,
    // an index register, a scaling factor, and a displacement
    D__Wq, D__Wps, D__Wss,

    D__Vpd, D__Wdq,


    //浮点数据类型
    D__word_int,
    D__dword_int,
    D__qword_int,
    D__single_real,
    D__double_real,
    D__extend_real,
    D__packed_BCD,
    D__2b,
    D__14_28b,
    D__98_108b,

    //为虚拟执行而补充的操作数类型
    D__tO, D__fO, D__tC, D__fC, D__tZ, D__fZ, D__fA, D__tA,
    D__tS, D__fS, D__tP, D__fP, D__tL, D__fL, D__fG, D__tG,
    D__LOOPNZ, D__LOOPZ, D__LOOP, D__JCXZ, D__JMP,

    //为正汇编而补充的操作数类型
    D_NEAR, D_MFAR,  // The ModR/M byte may refer only to memory or word register for call and jmp
    D_DBYTE, //正汇编命令 db
    D_DWORD, //正汇编命令 dw 
    D_DDWORD,//正汇编命令 dd 
    D_STRING,//正汇编命令 db "string" 
};