#include "stdio.h"
#include "string.h"
#include "inteldef.h"
#include "inteldis.h"
#include "windows.h"

//8位寄存器表
char *g_szRegByte[8] =
{
    "AL", "CL", "DL", "BL", "AH", "CH", "DH", "BH"
};

//16位寄存器表
char *g_szRegWord[8] =
{
    "AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI"
};

//32位寄存器表
char *g_szRegDword[8] =
{
    "EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI"
};

//段寄存器
char *g_szRegSegment[8] =
{
    "ES", "CS", "SS", "DS", "FS", "GS", "??", "??"
};

char *g_szRegAddr16[] =
{
    "[BX+SI]", "[SI+BX]", "[BX][SI]", "[SI][BX]",
    "[BX+DI]", "[DI+BX]", "[BX][DI]", "[DI][BX]",
    "[BP+SI]", "[SI+BP]", "[BP][SI]", "[SI][BP]",
    "[BP+DI]", "[DI+BP]", "[BP][DI]", "[DI][BP]",
    "[SI]", "[SI]", "[SI]", "[SI]",
    "[DI]", "[DI]", "[DI]", "[DI]",
    "[BP]", "[BP]", "[BP]", "[BP]",
    "[BX]", "[BX]", "[BX]", "[BX]"
};

LOCKREPXX g_szLockRepxx[] =
{
    { "LOCK ", 0xf0 },
    { "REP ", 0xf3 },
    { "REPZ ", 0xf3 },
    { "REPE ", 0xf3 },
    { "REPNE ", 0xf2 },
    { "REPNZ ", 0xf2 }
};

OPDATAWIDE g_szOpDataWide[] =
{
    { 1, "SHORT PTR" },
    { 2, "NEAR PTR" },
    { 4, "FAR PTR" },
    { 1, "BYTE PTR" },
    { 2, "WORD PTR" },
    { 4, "DWORD PTR" },
    { 6, "FWORD PTR" },
    { 8, "QWORD PTR" }
};

char *g_szPtr[] =
{
    NULL, "BYTE",
    "WORD", NULL,
    "DWORD", NULL,
    "FWORD", NULL,
    "QWORD", NULL,
    "TWORD"
};

//intel 指令表格
DIS_OPCODE_T Group0x80[] = //immediate group
{
    { R_M | Immediate_Ib, C_ADD, D__Eb, D__Ib, D__NONE, "ADD", }, //0x0
    { R_M | Immediate_Ib, C_OR, D__Eb, D__Ib, D__NONE, "OR", }, //0x1
    { R_M | Immediate_Ib, C_ADC, D__Eb, D__Ib, D__NONE, "ADC", }, //0x2
    { R_M | Immediate_Ib, C_SBB, D__Eb, D__Ib, D__NONE, "SBB", }, //0x3
    { R_M | Immediate_Ib, C_AND, D__Eb, D__Ib, D__NONE, "AND", }, //0x4
    { R_M | Immediate_Ib, C_SUB, D__Eb, D__Ib, D__NONE, "SUB", }, //0x5
    { R_M | Immediate_Ib, C_XOR, D__Eb, D__Ib, D__NONE, "XOR", }, //0x6
    { R_M | Immediate_Ib, C_CMP, D_rEb, D__Ib, D__NONE, "CMP", } //0x7
};

DIS_OPCODE_T Group0x81[] = //immediate group
{
    { R_M | Immediate_Iv, C_ADD, D__Ev, D__Iv, D__NONE, "ADD", }, //0x0
    { R_M | Immediate_Iv, C_OR, D__Ev, D__Iv, D__NONE, "OR", }, //0x1
    { R_M | Immediate_Iv, C_ADC, D__Ev, D__Iv, D__NONE, "ADC", }, //0x2
    { R_M | Immediate_Iv, C_SBB, D__Ev, D__Iv, D__NONE, "SBB", }, //0x3
    { R_M | Immediate_Iv, C_AND, D__Ev, D__Iv, D__NONE, "AND", }, //0x4
    { R_M | Immediate_Iv, C_SUB, D__Ev, D__Iv, D__NONE, "SUB", }, //0x5
    { R_M | Immediate_Iv, C_XOR, D__Ev, D__Iv, D__NONE, "XOR", }, //0x6
    { R_M | Immediate_Iv, C_CMP, D_rEv, D__Iv, D__NONE, "CMP", } //0x7

};

DIS_OPCODE_T Group0x82[] = //immediate group
{
    { R_M | Immediate_Ib, C_ADD, D__Eb, D__Ib, D__NONE, "ADD", }, //0x0
    { R_M | Immediate_Ib, C_OR, D__Eb, D__Ib, D__NONE, "OR", }, //0x1
    { R_M | Immediate_Ib, C_ADC, D__Eb, D__Ib, D__NONE, "ADC", }, //0x2
    { R_M | Immediate_Ib, C_SBB, D__Eb, D__Ib, D__NONE, "SBB", }, //0x3
    { R_M | Immediate_Ib, C_AND, D__Eb, D__Ib, D__NONE, "AND", }, //0x4
    { R_M | Immediate_Ib, C_SUB, D__Eb, D__Ib, D__NONE, "SUB", }, //0x5
    { R_M | Immediate_Ib, C_XOR, D__Eb, D__Ib, D__NONE, "XOR", }, //0x6
    { R_M | Immediate_Ib, C_CMP, D_rEb, D__Ib, D__NONE, "CMP", } //0x7

};

DIS_OPCODE_T Group0x83[] = //immediate group
{
    { R_M | Immediate_sIb, C_ADD, D__Ev, D_sIb, D__NONE, "ADD", }, //0x0
    { R_M | Immediate_sIb, C_OR, D__Ev, D_sIb, D__NONE, "OR", }, //0x1
    { R_M | Immediate_sIb, C_ADC, D__Ev, D_sIb, D__NONE, "ADC", }, //0x2
    { R_M | Immediate_sIb, C_SBB, D__Ev, D_sIb, D__NONE, "SBB", }, //0x3
    { R_M | Immediate_sIb, C_AND, D__Ev, D_sIb, D__NONE, "AND", }, //0x4
    { R_M | Immediate_sIb, C_SUB, D__Ev, D_sIb, D__NONE, "SUB", }, //0x5
    { R_M | Immediate_sIb, C_XOR, D__Ev, D_sIb, D__NONE, "XOR", }, //0x6
    { R_M | Immediate_sIb, C_CMP, D_rEv, D_sIb, D__NONE, "CMP", } //0x7

};

DIS_OPCODE_T Group0xc0[] = //shift group
{
    { R_M | Immediate_Ib, C_ROL, D__Eb, D__Ib, D__NONE, "ROL", }, //0x0
    { R_M | Immediate_Ib, C_ROR, D__Eb, D__Ib, D__NONE, "ROR", }, //0x1
    { R_M | Immediate_Ib, C_RCL, D__Eb, D__Ib, D__NONE, "RCL", }, //0x2
    { R_M | Immediate_Ib, C_RCR, D__Eb, D__Ib, D__NONE, "RCR", }, //0x3
    { R_M | Immediate_Ib, C_SHL, D__Eb, D__Ib, D__NONE, "SHL", }, //0x4
    { R_M | Immediate_Ib, C_SHR, D__Eb, D__Ib, D__NONE, "SHR", }, //0x5
    { R_M | Immediate_Ib, C_SAL, D__Eb, D__Ib, D__NONE, "SAL", }, //0x6
    { R_M | Immediate_Ib, C_SAR, D__Eb, D__Ib, D__NONE, "SAR", } //0x7

};

DIS_OPCODE_T Group0xc1[] = //shift group
{
    { R_M | Immediate_Ib, C_ROL, D__Ev, D__Ib, D__NONE, "ROL", }, //0x0
    { R_M | Immediate_Ib, C_ROR, D__Ev, D__Ib, D__NONE, "ROR", }, //0x1
    { R_M | Immediate_Ib, C_RCL, D__Ev, D__Ib, D__NONE, "RCL", }, //0x2
    { R_M | Immediate_Ib, C_RCR, D__Ev, D__Ib, D__NONE, "RCR", }, //0x3
    { R_M | Immediate_Ib, C_SHL, D__Ev, D__Ib, D__NONE, "SHL", }, //0x4
    { R_M | Immediate_Ib, C_SHR, D__Ev, D__Ib, D__NONE, "SHR", }, //0x5
    { R_M | Immediate_Ib, C_SAL, D__Ev, D__Ib, D__NONE, "SAL", }, //0x6
    { R_M | Immediate_Ib, C_SAR, D__Ev, D__Ib, D__NONE, "SAR", } //0x7

};

DIS_OPCODE_T Group0xc6[] = //mov group
{
    { R_M | Immediate_Ib, C_MOV, D_wEb, D__Ib, D__NONE, "MOV", }, //0x0
    { R_M, C_NULL }, //0x1
    { R_M, C_NULL }, //0x2
    { R_M, C_NULL }, //0x3
    { R_M, C_NULL }, //0x4
    { R_M, C_NULL }, //0x5
    { R_M, C_NULL }, //0x6
    { R_M, C_NULL }, //0x7

};

DIS_OPCODE_T Group0xc7[] = //mov group
{
    { R_M | Immediate_Iv, C_MOV, D_wEv, D__Iv, D__NONE, "MOV", }, //0x0
    { R_M, C_NULL }, //0x1
    { R_M, C_NULL }, //0x2
    { R_M, C_NULL }, //0x3
    { R_M, C_NULL }, //0x4
    { R_M, C_NULL }, //0x5
    { R_M, C_NULL }, //0x6
    { R_M, C_NULL }, //0x7

};

DIS_OPCODE_T Group0xd0[] = //shift group
{
    { R_M | Immediate_1, C_ROL, D__Eb, D__1, D__NONE, "ROL", }, //0x0
    { R_M | Immediate_1, C_ROR, D__Eb, D__1, D__NONE, "ROR", }, //0x1
    { R_M | Immediate_1, C_RCL, D__Eb, D__1, D__NONE, "RCL", }, //0x2
    { R_M | Immediate_1, C_RCR, D__Eb, D__1, D__NONE, "RCR", }, //0x3
    { R_M | Immediate_1, C_SHL, D__Eb, D__1, D__NONE, "SHL", }, //0x4
    { R_M | Immediate_1, C_SHR, D__Eb, D__1, D__NONE, "SHR", }, //0x5
    { R_M | Immediate_1, C_SAL, D__Eb, D__1, D__NONE, "SAL", }, //0x6
    { R_M | Immediate_1, C_SAR, D__Eb, D__1, D__NONE, "SAR", } //0x7

};

DIS_OPCODE_T Group0xd1[] = //shift group
{
    { R_M | Immediate_1, C_ROL, D__Ev, D__1, D__NONE, "ROL", }, //0x0
    { R_M | Immediate_1, C_ROR, D__Ev, D__1, D__NONE, "ROR", }, //0x1
    { R_M | Immediate_1, C_RCL, D__Ev, D__1, D__NONE, "RCL", }, //0x2
    { R_M | Immediate_1, C_RCR, D__Ev, D__1, D__NONE, "RCR", }, //0x3
    { R_M | Immediate_1, C_SHL, D__Ev, D__1, D__NONE, "SHL", }, //0x4
    { R_M | Immediate_1, C_SHR, D__Ev, D__1, D__NONE, "SHR", }, //0x5
    { R_M | Immediate_1, C_SAL, D__Ev, D__1, D__NONE, "SAL", }, //0x6
    { R_M | Immediate_1, C_SAR, D__Ev, D__1, D__NONE, "SAR", } //0x7

};

DIS_OPCODE_T Group0xd2[] = //shift group
{
    { R_M, C_ROL, D__Eb, D__CL, D__NONE, "ROL", }, //0x0
    { R_M, C_ROR, D__Eb, D__CL, D__NONE, "ROR", }, //0x1
    { R_M, C_RCL, D__Eb, D__CL, D__NONE, "RCL", }, //0x2
    { R_M, C_RCR, D__Eb, D__CL, D__NONE, "RCR", }, //0x3
    { R_M, C_SHL, D__Eb, D__CL, D__NONE, "SHL", }, //0x4
    { R_M, C_SHR, D__Eb, D__CL, D__NONE, "SHR", }, //0x5
    { R_M, C_SAL, D__Eb, D__CL, D__NONE, "SAL", }, //0x6
    { R_M, C_SAR, D__Eb, D__CL, D__NONE, "SAR", } //0x7

};

DIS_OPCODE_T Group0xd3[] = //shift group
{
    { R_M, C_ROL, D__Ev, D__CL, D__NONE, "ROL", }, //0x0
    { R_M, C_ROR, D__Ev, D__CL, D__NONE, "ROR", }, //0x1
    { R_M, C_RCL, D__Ev, D__CL, D__NONE, "RCL", }, //0x2
    { R_M, C_RCR, D__Ev, D__CL, D__NONE, "RCR", }, //0x3
    { R_M, C_SHL, D__Ev, D__CL, D__NONE, "SHL", }, //0x4
    { R_M, C_SHR, D__Ev, D__CL, D__NONE, "SHR", }, //0x5
    { R_M, C_SAL, D__Ev, D__CL, D__NONE, "SAL", }, //0x6
    { R_M, C_SAR, D__Ev, D__CL, D__NONE, "SAR", } //0x7

};

DIS_OPCODE_T Group0xf6[] = //test/not/neg/mul/div group
{
    { R_M | Immediate_Ib, C_TEST, D_rEb, D__Ib, D__NONE, "TEST", }, //0x0
    { R_M | Immediate_Ib, C_TEST, D_rEb, D__Ib, D__NONE, "TEST", }, //0x1
    { R_M, C_NOT, D__Eb, D__NONE, D__NONE, "NOT", }, //0x2
    { R_M, C_NEG, D__Eb, D__NONE, D__NONE, "NEG", }, //0x3
    { R_M, C_MUL, D_rEb, D__NONE, D__NONE, "MUL", }, //0x4
    { R_M, C_IMUL, D_rEb, D__NONE, D__NONE, "IMUL", }, //0x5
    { R_M, C_DIV, D_rEb, D__NONE, D__NONE, "DIV", }, //0x6
    { R_M, C_IDIV, D_rEb, D__NONE, D__NONE, "IDIV", }, //0x7

};

DIS_OPCODE_T Group0xf7[] = //test/not/neg/mul/div group
{
    { R_M | Immediate_Iv, C_TEST, D_rEv, D__Iv, D__NONE, "TEST", }, //0x0
    { R_M | Immediate_sIb, C_TEST, D_rEv, D_sIb, D__NONE, "TEST", }, //0x1
    { R_M, C_NOT, D__Ev, D__NONE, D__NONE, "NOT", }, //0x2
    { R_M, C_NEG, D__Ev, D__NONE, D__NONE, "NEG", }, //0x3
    { R_M, C_MUL, D_rEv, D__NONE, D__NONE, "MUL", }, //0x4
    { R_M, C_IMUL, D_rEv, D__NONE, D__NONE, "IMUL", }, //0x5
    { R_M, C_DIV, D_rEv, D__NONE, D__NONE, "DIV", }, //0x6
    { R_M, C_IDIV, D_rEv, D__NONE, D__NONE, "IDIV", } //0x7
};

DIS_OPCODE_T Group0xfe[] = //inc/dec group
{
    { R_M, C_INC, D__Eb, D__NONE, D__NONE, "INC", }, //0x0
    { R_M, C_DEC, D__Eb, D__NONE, D__NONE, "DEC", }, //0x1
    { R_M, C_NULL }, //0x2
    { R_M, C_NULL }, //0x3
    { R_M, C_NULL }, //0x4
    { R_M, C_NULL }, //0x5
    { R_M, C_NULL }, //0x6
    { R_M, C_NULL }, //0x7
};

DIS_OPCODE_T Group0xff[] = //inc/dec/间接转移指令/push group
{
    { R_M, C_INC, D__Ev, D__NONE, D__NONE, "INC", }, //0x0
    { R_M, C_DEC, D__Ev, D__NONE, D__NONE, "DEC", }, //0x1
    { R_M, C_CALL, D_rEv, D__NONE, D__NONE, "CALL", }, //0x2
    { R_M, C_CALL, D_rEp, D__NONE, D__NONE, "CALL", }, //0x3
    { R_M, C_JMP, D_rEv, D__NONE, D__NONE, "JMP", }, //0x4
    { R_M, C_JMP, D_rEp, D__NONE, D__NONE, "JMP", }, //0x5
    { R_M, C_PUSH, D_rEv, D__NONE, D__NONE, "PUSH", }, //0x6
    { R_M, C_NULL } //0x7
};

DIS_OPCODE_T Group0x8f[] = //pop group
{
    { R_M, C_POP, D_wEv, D__NONE, D__NONE, "POP", }, //0x0
    { R_M, C_NULL }, //0x1
    { R_M, C_NULL }, //0x2
    { R_M, C_NULL }, //0x3
    { R_M, C_NULL }, //0x4
    { R_M, C_NULL }, //0x5
    { R_M, C_NULL }, //0x6
    { R_M, C_NULL }, //0x7

};

DIS_OPCODE_T Group0xd8[] = //fpu group
{
    { R_M, C_FADD, D__single_real, D__NONE, D__NONE, "FADD", }, //0x0
    { R_M, C_FMUL, D__single_real, D__NONE, D__NONE, "FMUL", }, //0x1
    { R_M, C_FCOM, D__single_real, D__NONE, D__NONE, "FCOM", }, //0x2
    { R_M, C_FCOMP, D__single_real, D__NONE, D__NONE, "FCOMP", }, //0x3
    { R_M, C_FSUB, D__single_real, D__NONE, D__NONE, "FSUB", }, //0x4
    { R_M, C_FSUBR, D__single_real, D__NONE, D__NONE, "FSUBR", }, //0x5
    { R_M, C_FDIV, D__single_real, D__NONE, D__NONE, "FDIV", }, //0x6
    { R_M, C_FDIVR, D__single_real, D__NONE, D__NONE, "FDIVR", }, //0x7

    //双字节浮点指令
    { 0, C_FADD, D__st, D__st0, D__NONE, "FADD", }, //0xc0   
    { 0, C_FADD, D__st, D__st1, D__NONE, "FADD", }, //0xc1   
    { 0, C_FADD, D__st, D__st2, D__NONE, "FADD", }, //0xc2   
    { 0, C_FADD, D__st, D__st3, D__NONE, "FADD", }, //0xc3   
    { 0, C_FADD, D__st, D__st4, D__NONE, "FADD", }, //0xc4   
    { 0, C_FADD, D__st, D__st5, D__NONE, "FADD", }, //0xc5   
    { 0, C_FADD, D__st, D__st6, D__NONE, "FADD", }, //0xc6   
    { 0, C_FADD, D__st, D__st7, D__NONE, "FADD", }, //0xc7   

    { 0, C_FMUL, D__st, D__st0, D__NONE, "FMUL", }, //0xc8
    { 0, C_FMUL, D__st, D__st1, D__NONE, "FMUL", }, //0xc9
    { 0, C_FMUL, D__st, D__st2, D__NONE, "FMUL", }, //0xca
    { 0, C_FMUL, D__st, D__st3, D__NONE, "FMUL", }, //0xcb
    { 0, C_FMUL, D__st, D__st4, D__NONE, "FMUL", }, //0xcc
    { 0, C_FMUL, D__st, D__st5, D__NONE, "FMUL", }, //0xcd
    { 0, C_FMUL, D__st, D__st6, D__NONE, "FMUL", }, //0xce
    { 0, C_FMUL, D__st, D__st7, D__NONE, "FMUL", }, //0xcf

    { 0, C_FCOM, D__st, D__st0, D__NONE, "FCOM", }, //0xd0
    { 0, C_FCOM, D__st, D__st1, D__NONE, "FCOM", }, //0xd1
    { 0, C_FCOM, D__st, D__st2, D__NONE, "FCOM", }, //0xd2
    { 0, C_FCOM, D__st, D__st3, D__NONE, "FCOM", }, //0xd3
    { 0, C_FCOM, D__st, D__st4, D__NONE, "FCOM", }, //0xd4
    { 0, C_FCOM, D__st, D__st5, D__NONE, "FCOM", }, //0xd5
    { 0, C_FCOM, D__st, D__st6, D__NONE, "FCOM", }, //0xd6
    { 0, C_FCOM, D__st, D__st7, D__NONE, "FCOM", }, //0xd7

    { 0, C_FCOMP, D__st, D__st0, D__NONE, "FCOMP", }, //0xd8
    { 0, C_FCOMP, D__st, D__st1, D__NONE, "FCOMP", }, //0xd9
    { 0, C_FCOMP, D__st, D__st2, D__NONE, "FCOMP", }, //0xda
    { 0, C_FCOMP, D__st, D__st3, D__NONE, "FCOMP", }, //0xdb
    { 0, C_FCOMP, D__st, D__st4, D__NONE, "FCOMP", }, //0xdc
    { 0, C_FCOMP, D__st, D__st5, D__NONE, "FCOMP", }, //0xdd
    { 0, C_FCOMP, D__st, D__st6, D__NONE, "FCOMP", }, //0xde
    { 0, C_FCOMP, D__st, D__st7, D__NONE, "FCOMP", }, //0xdf

    { 0, C_FSUB, D__st, D__st0, D__NONE, "FSUB", }, //0xe0
    { 0, C_FSUB, D__st, D__st1, D__NONE, "FSUB", }, //0xe1
    { 0, C_FSUB, D__st, D__st2, D__NONE, "FSUB", }, //0xe2
    { 0, C_FSUB, D__st, D__st3, D__NONE, "FSUB", }, //0xe3
    { 0, C_FSUB, D__st, D__st4, D__NONE, "FSUB", }, //0xe4
    { 0, C_FSUB, D__st, D__st5, D__NONE, "FSUB", }, //0xe5
    { 0, C_FSUB, D__st, D__st6, D__NONE, "FSUB", }, //0xe6
    { 0, C_FSUB, D__st, D__st7, D__NONE, "FSUB", }, //0xe7

    { 0, C_FSUBR, D__st, D__st0, D__NONE, "FSUBR", }, //0xe8
    { 0, C_FSUBR, D__st, D__st1, D__NONE, "FSUBR", }, //0xe9
    { 0, C_FSUBR, D__st, D__st2, D__NONE, "FSUBR", }, //0xea
    { 0, C_FSUBR, D__st, D__st3, D__NONE, "FSUBR", }, //0xeb
    { 0, C_FSUBR, D__st, D__st4, D__NONE, "FSUBR", }, //0xec
    { 0, C_FSUBR, D__st, D__st5, D__NONE, "FSUBR", }, //0xed
    { 0, C_FSUBR, D__st, D__st6, D__NONE, "FSUBR", }, //0xee
    { 0, C_FSUBR, D__st, D__st7, D__NONE, "FSUBR", }, //0xef

    { 0, C_FDIV, D__st, D__st0, D__NONE, "FDIV", }, //0xf0
    { 0, C_FDIV, D__st, D__st1, D__NONE, "FDIV", }, //0xf1
    { 0, C_FDIV, D__st, D__st2, D__NONE, "FDIV", }, //0xf2
    { 0, C_FDIV, D__st, D__st3, D__NONE, "FDIV", }, //0xf3
    { 0, C_FDIV, D__st, D__st4, D__NONE, "FDIV", }, //0xf4
    { 0, C_FDIV, D__st, D__st5, D__NONE, "FDIV", }, //0xf5
    { 0, C_FDIV, D__st, D__st6, D__NONE, "FDIV", }, //0xf6
    { 0, C_FDIV, D__st, D__st7, D__NONE, "FDIV", }, //0xf7

    { 0, C_FDIVR, D__st, D__st0, D__NONE, "FDIVR", }, //0xf8
    { 0, C_FDIVR, D__st, D__st1, D__NONE, "FDIVR", }, //0xf9
    { 0, C_FDIVR, D__st, D__st2, D__NONE, "FDIVR", }, //0xfa
    { 0, C_FDIVR, D__st, D__st3, D__NONE, "FDIVR", }, //0xfb
    { 0, C_FDIVR, D__st, D__st4, D__NONE, "FDIVR", }, //0xfc
    { 0, C_FDIVR, D__st, D__st5, D__NONE, "FDIVR", }, //0xfd
    { 0, C_FDIVR, D__st, D__st6, D__NONE, "FDIVR", }, //0xfe
    { 0, C_FDIVR, D__st, D__st7, D__NONE, "FDIVR", } //0xff

};

DIS_OPCODE_T Group0xd9[] = //fpu group
{
    { R_M, C_FLD, D__single_real, D__NONE, D__NONE, "FLD", }, //0x0
    { R_M, C_NULL }, //0x1
    { R_M, C_FST, D__single_real, D__NONE, D__NONE, "FST", }, //0x2
    { R_M, C_FSTP, D__single_real, D__NONE, D__NONE, "FSTP", }, //0x3
    { R_M, C_FLDENV, D__14_28b, D__NONE, D__NONE, "FLDENV", }, //0x4
    { R_M, C_FLDCW, D__2b, D__NONE, D__NONE, "FLDCW", }, //0x5
    { R_M, C_FSTENV, D__14_28b, D__NONE, D__NONE, "FSTENV", }, //0x6
    { R_M, C_FSTCW, D__2b, D__NONE, D__NONE, "FSTCW", }, //0x7

    //双字节浮点指令
    { 0, C_FLD, D__st, D__st0, D__NONE, "FLD", }, //0xc0
    { 0, C_FLD, D__st, D__st1, D__NONE, "FLD", }, //0xc1
    { 0, C_FLD, D__st, D__st2, D__NONE, "FLD", }, //0xc2
    { 0, C_FLD, D__st, D__st3, D__NONE, "FLD", }, //0xc3
    { 0, C_FLD, D__st, D__st4, D__NONE, "FLD", }, //0xc4
    { 0, C_FLD, D__st, D__st5, D__NONE, "FLD", }, //0xc5
    { 0, C_FLD, D__st, D__st6, D__NONE, "FLD", }, //0xc6
    { 0, C_FLD, D__st, D__st7, D__NONE, "FLD", }, //0xc7

    { 0, C_FXCH, D__st, D__st0, D__NONE, "FXCH", }, //0xc8
    { 0, C_FXCH, D__st, D__st1, D__NONE, "FXCH", }, //0xc9
    { 0, C_FXCH, D__st, D__st2, D__NONE, "FXCH", }, //0xca
    { 0, C_FXCH, D__st, D__st3, D__NONE, "FXCH", }, //0xcb
    { 0, C_FXCH, D__st, D__st4, D__NONE, "FXCH", }, //0xcc
    { 0, C_FXCH, D__st, D__st5, D__NONE, "FXCH", }, //0xcd
    { 0, C_FXCH, D__st, D__st6, D__NONE, "FXCH", }, //0xce
    { 0, C_FXCH, D__st, D__st7, D__NONE, "FXCH", }, //0xcf

    { 0, C_FNOP, D__NONE, D__NONE, D__NONE, "FNOP", }, //0xd0
    { 0, C_NULL }, //0xd1
    { 0, C_NULL }, //0xd2
    { 0, C_NULL }, //0xd3
    { 0, C_NULL }, //0xd4
    { 0, C_NULL }, //0xd5
    { 0, C_NULL }, //0xd6
    { 0, C_NULL }, //0xd7

    { 0, C_NULL }, //0xd8
    { 0, C_NULL }, //0xd9
    { 0, C_NULL }, //0xda
    { 0, C_NULL }, //0xdb
    { 0, C_NULL }, //0xdc
    { 0, C_NULL }, //0xdd
    { 0, C_NULL }, //0xde
    { 0, C_NULL }, //0xdf

    { 0, C_FCHS, D__NONE, D__NONE, D__NONE, "FCHS", }, //0xe0
    { 0, C_FABS, D__NONE, D__NONE, D__NONE, "FABS", }, //0xe1
    { 0, C_NULL }, //0xe2
    { 0, C_NULL }, //0xe3
    { 0, C_FTST, D__NONE, D__NONE, D__NONE, "FTST", }, //0xe4
    { 0, C_FXAM, D__NONE, D__NONE, D__NONE, "FXAM", }, //0xe5
    { 0, C_NULL }, //0xe6
    { 0, C_NULL }, //0xe7

    { 0, C_FLD1, D__NONE, D__NONE, D__NONE, "FLD1", }, //0xe8
    { 0, C_FLD2T, D__NONE, D__NONE, D__NONE, "FLD2T", }, //0xe9
    { 0, C_FLDL2E, D__NONE, D__NONE, D__NONE, "FLDL2E", }, //0xea
    { 0, C_FLDP1, D__NONE, D__NONE, D__NONE, "FLDP1", }, //0xeb
    { 0, C_FLDLG2, D__NONE, D__NONE, D__NONE, "FLDLG2", }, //0xec
    { 0, C_FLDLN2, D__NONE, D__NONE, D__NONE, "FLDLN2", }, //0xed
    { 0, C_FLDZ, D__NONE, D__NONE, D__NONE, "FLDZ", }, //0xee
    { 0, C_NULL }, //0xef

    { 0, C_F2XM1, D__NONE, D__NONE, D__NONE, "F2XM1", }, //0xf0
    { 0, C_FYL2X, D__NONE, D__NONE, D__NONE, "FYL2X", }, //0xf1
    { 0, C_FPTAN, D__NONE, D__NONE, D__NONE, "FPTAN", }, //0xf2
    { 0, C_FPATAN, D__NONE, D__NONE, D__NONE, "FPATAN", }, //0xf3
    { 0, C_FXTRACT, D__NONE, D__NONE, D__NONE, "FXTRACT", }, //0xf4
    { 0, C_FPREM1, D__NONE, D__NONE, D__NONE, "FPREM1", }, //0xf5
    { 0, C_FDECSTP, D__NONE, D__NONE, D__NONE, "FDECSTP", }, //0xf6
    { 0, C_FINCSTP, D__NONE, D__NONE, D__NONE, "FINCSTP", }, //0xf7

    { 0, C_FPREM, D__NONE, D__NONE, D__NONE, "FPREM", }, //0xf8
    { 0, C_FYL2XP1, D__NONE, D__NONE, D__NONE, "FYL2XP1", }, //0xf9
    { 0, C_FSQRT, D__NONE, D__NONE, D__NONE, "FSQRT", }, //0xfa
    { 0, C_FSINCOS, D__NONE, D__NONE, D__NONE, "FSINCOS", }, //0xfb
    { 0, C_FRNDINT, D__NONE, D__NONE, D__NONE, "FRNDINT", }, //0xfc
    { 0, C_FSCALE, D__NONE, D__NONE, D__NONE, "FSCALE", }, //0xfd
    { 0, C_FSIN, D__NONE, D__NONE, D__NONE, "FSIN", }, //0xfe
    { 0, C_FCOS, D__NONE, D__NONE, D__NONE, "FCOS", } //0xff

};

DIS_OPCODE_T Group0xda[] = //fpu group
{
    { R_M, C_FIADD, D__dword_int, D__NONE, D__NONE, "FIADD", }, //0x0
    { R_M, C_FIMUL, D__dword_int, D__NONE, D__NONE, "FIMUL", }, //0x1
    { R_M, C_FICOM, D__dword_int, D__NONE, D__NONE, "FICOM", }, //0x2
    { R_M, C_FICOMP, D__dword_int, D__NONE, D__NONE, "FICOMP", }, //0x3
    { R_M, C_FISUB, D__dword_int, D__NONE, D__NONE, "FISUB", }, //0x4
    { R_M, C_FISUBR, D__dword_int, D__NONE, D__NONE, "FISUBR", }, //0x5
    { R_M, C_FIDIV, D__dword_int, D__NONE, D__NONE, "FIDIV", }, //0x6
    { R_M, C_FIDIVR, D__dword_int, D__NONE, D__NONE, "FIDIVR", }, //0x7

    //双字节浮点指令
    { 0, C_FCMOVB, D__st, D__st0, D__NONE, "FCMOVB", }, //0xc0
    { 0, C_FCMOVB, D__st, D__st1, D__NONE, "FCMOVB", }, //0xc1
    { 0, C_FCMOVB, D__st, D__st2, D__NONE, "FCMOVB", }, //0xc2
    { 0, C_FCMOVB, D__st, D__st3, D__NONE, "FCMOVB", }, //0xc3
    { 0, C_FCMOVB, D__st, D__st4, D__NONE, "FCMOVB", }, //0xc4
    { 0, C_FCMOVB, D__st, D__st5, D__NONE, "FCMOVB", }, //0xc5
    { 0, C_FCMOVB, D__st, D__st6, D__NONE, "FCMOVB", }, //0xc6
    { 0, C_FCMOVB, D__st, D__st7, D__NONE, "FCMOVB", }, //0xc7

    { 0, C_FCMOVE, D__st, D__st0, D__NONE, "FCMOVE", }, //0xc8
    { 0, C_FCMOVE, D__st, D__st1, D__NONE, "FCMOVE", }, //0xc9
    { 0, C_FCMOVE, D__st, D__st2, D__NONE, "FCMOVE", }, //0xca
    { 0, C_FCMOVE, D__st, D__st3, D__NONE, "FCMOVE", }, //0xcb
    { 0, C_FCMOVE, D__st, D__st4, D__NONE, "FCMOVE", }, //0xcc
    { 0, C_FCMOVE, D__st, D__st5, D__NONE, "FCMOVE", }, //0xcd
    { 0, C_FCMOVE, D__st, D__st6, D__NONE, "FCMOVE", }, //0xce
    { 0, C_FCMOVE, D__st, D__st7, D__NONE, "FCMOVE", }, //0xcf

    { 0, C_FCMOVBE, D__st, D__st0, D__NONE, "FCMOVBE", }, //0xd0
    { 0, C_FCMOVBE, D__st, D__st1, D__NONE, "FCMOVBE", }, //0xd1
    { 0, C_FCMOVBE, D__st, D__st2, D__NONE, "FCMOVBE", }, //0xd2
    { 0, C_FCMOVBE, D__st, D__st3, D__NONE, "FCMOVBE", }, //0xd3
    { 0, C_FCMOVBE, D__st, D__st4, D__NONE, "FCMOVBE", }, //0xd4
    { 0, C_FCMOVBE, D__st, D__st5, D__NONE, "FCMOVBE", }, //0xd5
    { 0, C_FCMOVBE, D__st, D__st6, D__NONE, "FCMOVBE", }, //0xd6
    { 0, C_FCMOVBE, D__st, D__st7, D__NONE, "FCMOVBE", }, //0xd7

    { 0, C_FCMOVU, D__st, D__st0, D__NONE, "FCMOVU", }, //0xd8
    { 0, C_FCMOVU, D__st, D__st1, D__NONE, "FCMOVU", }, //0xd9
    { 0, C_FCMOVU, D__st, D__st2, D__NONE, "FCMOVU", }, //0xda
    { 0, C_FCMOVU, D__st, D__st3, D__NONE, "FCMOVU", }, //0xdb
    { 0, C_FCMOVU, D__st, D__st4, D__NONE, "FCMOVU", }, //0xdc
    { 0, C_FCMOVU, D__st, D__st5, D__NONE, "FCMOVU", }, //0xdd
    { 0, C_FCMOVU, D__st, D__st6, D__NONE, "FCMOVU", }, //0xde
    { 0, C_FCMOVU, D__st, D__st7, D__NONE, "FCMOVU", }, //0xdf

    { 0, C_NULL }, //0xe0
    { 0, C_NULL }, //0xe1
    { 0, C_NULL }, //0xe2
    { 0, C_NULL }, //0xe3
    { 0, C_NULL }, //0xe4
    { 0, C_NULL }, //0xe5
    { 0, C_NULL }, //0xe6
    { 0, C_NULL }, //0xe7

    { 0, C_NULL }, //0xe8
    { 0, C_FUCOMPP, D__NONE, D__NONE, D__NONE, "FUCOMPP", }, //0xe9
    { 0, C_NULL }, //0xea
    { 0, C_NULL }, //0xeb
    { 0, C_NULL }, //0xec
    { 0, C_NULL }, //0xed
    { 0, C_NULL }, //0xee
    { 0, C_NULL }, //0xef

    { 0, C_NULL }, //0xf0
    { 0, C_NULL }, //0xf1
    { 0, C_NULL }, //0xf2
    { 0, C_NULL }, //0xf3
    { 0, C_NULL }, //0xf4
    { 0, C_NULL }, //0xf5
    { 0, C_NULL }, //0xf6
    { 0, C_NULL }, //0xf7

    { 0, C_NULL }, //0xf8
    { 0, C_NULL }, //0xf9
    { 0, C_NULL }, //0xfa
    { 0, C_NULL }, //0xfb
    { 0, C_NULL }, //0xfc
    { 0, C_NULL }, //0xfd
    { 0, C_NULL }, //0xfe
    { 0, C_NULL } //0xff

};

DIS_OPCODE_T Group0xdb[] = //fpu group
{
    { R_M, C_FILD, D__dword_int, D__NONE, D__NONE, "FILD", }, //0x0
    { R_M, C_NULL }, //0x1
    { R_M, C_FIST, D__dword_int, D__NONE, D__NONE, "FIST", }, //0x2
    { R_M, C_FISTP, D__dword_int, D__NONE, D__NONE, "FISTP", }, //0x3
    { R_M, C_NULL }, //0x4
    { R_M, C_FLD, D__extend_real, D__NONE, D__NONE, "FLD", }, //0x5
    { R_M, C_NULL }, //0x6
    { R_M, C_FSTP, D__extend_real, D__NONE, D__NONE, "FSTP", }, //0x7

    //双字节浮点指令
    { 0, C_FCMOVNB, D__st, D__st0, D__NONE, "FCMOVNB", }, //0xc0
    { 0, C_FCMOVNB, D__st, D__st1, D__NONE, "FCMOVNB", }, //0xc1
    { 0, C_FCMOVNB, D__st, D__st2, D__NONE, "FCMOVNB", }, //0xc2
    { 0, C_FCMOVNB, D__st, D__st3, D__NONE, "FCMOVNB", }, //0xc3
    { 0, C_FCMOVNB, D__st, D__st4, D__NONE, "FCMOVNB", }, //0xc4
    { 0, C_FCMOVNB, D__st, D__st5, D__NONE, "FCMOVNB", }, //0xc5
    { 0, C_FCMOVNB, D__st, D__st6, D__NONE, "FCMOVNB", }, //0xc6
    { 0, C_FCMOVNB, D__st, D__st7, D__NONE, "FCMOVNB", }, //0xc7

    { 0, C_FCMOVNE, D__st, D__st0, D__NONE, "FCMOVNE", }, //0xc8
    { 0, C_FCMOVNE, D__st, D__st1, D__NONE, "FCMOVNE", }, //0xc9
    { 0, C_FCMOVNE, D__st, D__st2, D__NONE, "FCMOVNE", }, //0xca
    { 0, C_FCMOVNE, D__st, D__st3, D__NONE, "FCMOVNE", }, //0xcb
    { 0, C_FCMOVNE, D__st, D__st4, D__NONE, "FCMOVNE", }, //0xcc
    { 0, C_FCMOVNE, D__st, D__st5, D__NONE, "FCMOVNE", }, //0xcd
    { 0, C_FCMOVNE, D__st, D__st6, D__NONE, "FCMOVNE", }, //0xce
    { 0, C_FCMOVNE, D__st, D__st7, D__NONE, "FCMOVNE", }, //0xcf

    { 0, C_FCMOVNBE, D__st, D__st0, D__NONE, "FCMOVNBE", }, //0xd0
    { 0, C_FCMOVNBE, D__st, D__st1, D__NONE, "FCMOVNBE", }, //0xd1
    { 0, C_FCMOVNBE, D__st, D__st2, D__NONE, "FCMOVNBE", }, //0xd2
    { 0, C_FCMOVNBE, D__st, D__st3, D__NONE, "FCMOVNBE", }, //0xd3
    { 0, C_FCMOVNBE, D__st, D__st4, D__NONE, "FCMOVNBE", }, //0xd4
    { 0, C_FCMOVNBE, D__st, D__st5, D__NONE, "FCMOVNBE", }, //0xd5
    { 0, C_FCMOVNBE, D__st, D__st6, D__NONE, "FCMOVNBE", }, //0xd6
    { 0, C_FCMOVNBE, D__st, D__st7, D__NONE, "FCMOVNBE", }, //0xd7
    { 0, C_FCMOVNU, D__st, D__st0, D__NONE, "FCMOVNU", }, //0xd8
    { 0, C_FCMOVNU, D__st, D__st1, D__NONE, "FCMOVNU", }, //0xd9
    { 0, C_FCMOVNU, D__st, D__st2, D__NONE, "FCMOVNU", }, //0xda
    { 0, C_FCMOVNU, D__st, D__st3, D__NONE, "FCMOVNU", }, //0xdb
    { 0, C_FCMOVNU, D__st, D__st4, D__NONE, "FCMOVNU", }, //0xdc
    { 0, C_FCMOVNU, D__st, D__st5, D__NONE, "FCMOVNU", }, //0xdd
    { 0, C_FCMOVNU, D__st, D__st6, D__NONE, "FCMOVNU", }, //0xde
    { 0, C_FCMOVNU, D__st, D__st7, D__NONE, "FCMOVNU", }, //0xdf

    { 0, C_NULL }, //0xe0
    { 0, C_NULL }, //0xe1
    { 0, C_FCLEX, D__NONE, D__NONE, D__NONE, "FCLEX", }, //0xe2
    { 0, C_FINIT, D__NONE, D__NONE, D__NONE, "FINIT", }, //0xe3
    { 0, C_NULL }, //0xe4
    { 0, C_NULL }, //0xe5
    { 0, C_NULL }, //0xe6
    { 0, C_NULL }, //0xe7

    { 0, C_FUCOMI, D__st, D__st0, D__NONE, "FUCOMI", }, //0xe8
    { 0, C_FUCOMI, D__st, D__st1, D__NONE, "FUCOMI", }, //0xe9
    { 0, C_FUCOMI, D__st, D__st2, D__NONE, "FUCOMI", }, //0xea
    { 0, C_FUCOMI, D__st, D__st3, D__NONE, "FUCOMI", }, //0xeb
    { 0, C_FUCOMI, D__st, D__st4, D__NONE, "FUCOMI", }, //0xec
    { 0, C_FUCOMI, D__st, D__st5, D__NONE, "FUCOMI", }, //0xed
    { 0, C_FUCOMI, D__st, D__st6, D__NONE, "FUCOMI", }, //0xee
    { 0, C_FUCOMI, D__st, D__st7, D__NONE, "FUCOMI", }, //0xef

    { 0, C_FCOMI, D__st, D__st0, D__NONE, "FCOMI", }, //0xf0
    { 0, C_FCOMI, D__st, D__st1, D__NONE, "FCOMI", }, //0xf1
    { 0, C_FCOMI, D__st, D__st2, D__NONE, "FCOMI", }, //0xf2
    { 0, C_FCOMI, D__st, D__st3, D__NONE, "FCOMI", }, //0xf3
    { 0, C_FCOMI, D__st, D__st4, D__NONE, "FCOMI", }, //0xf4
    { 0, C_FCOMI, D__st, D__st5, D__NONE, "FCOMI", }, //0xf5
    { 0, C_FCOMI, D__st, D__st6, D__NONE, "FCOMI", }, //0xf6
    { 0, C_FCOMI, D__st, D__st7, D__NONE, "FCOMI", }, //0xf7

    { 0, C_NULL }, //0xf8
    { 0, C_NULL }, //0xf9
    { 0, C_NULL }, //0xfa
    { 0, C_NULL }, //0xfb
    { 0, C_NULL }, //0xfc
    { 0, C_NULL }, //0xfd
    { 0, C_NULL }, //0xfe
    { 0, C_NULL } //0xff

};

DIS_OPCODE_T Group0xdc[] = //fpu group
{
    { R_M, C_FADD, D__double_real, D__NONE, D__NONE, "FADD", }, //0x0
    { R_M, C_FMUL, D__double_real, D__NONE, D__NONE, "FMUL", }, //0x1
    { R_M, C_FCOM, D__double_real, D__NONE, D__NONE, "FCOM", }, //0x2
    { R_M, C_FCOMP, D__double_real, D__NONE, D__NONE, "FCOMP", }, //0x3
    { R_M, C_FSUB, D__double_real, D__NONE, D__NONE, "FSUB", }, //0x4
    { R_M, C_FSUBR, D__double_real, D__NONE, D__NONE, "FSUBR", }, //0x5
    { R_M, C_FDIV, D__double_real, D__NONE, D__NONE, "FDIV", }, //0x6
    { R_M, C_FDIVR, D__double_real, D__NONE, D__NONE, "FDIVR", }, //0x7

    //双字节浮点指令
    { 0, C_FADD, D__st, D__st0, D__NONE, "FADD", }, //0xc0
    { 0, C_FADD, D__st, D__st1, D__NONE, "FADD", }, //0xc1
    { 0, C_FADD, D__st, D__st2, D__NONE, "FADD", }, //0xc2
    { 0, C_FADD, D__st, D__st3, D__NONE, "FADD", }, //0xc3
    { 0, C_FADD, D__st, D__st4, D__NONE, "FADD", }, //0xc4
    { 0, C_FADD, D__st, D__st5, D__NONE, "FADD", }, //0xc5
    { 0, C_FADD, D__st, D__st6, D__NONE, "FADD", }, //0xc6
    { 0, C_FADD, D__st, D__st7, D__NONE, "FADD", }, //0xc7

    { 0, C_FMUL, D__st, D__st0, D__NONE, "FMUL", }, //0xc8
    { 0, C_FMUL, D__st, D__st1, D__NONE, "FMUL", }, //0xc9
    { 0, C_FMUL, D__st, D__st2, D__NONE, "FMUL", }, //0xca
    { 0, C_FMUL, D__st, D__st3, D__NONE, "FMUL", }, //0xcb
    { 0, C_FMUL, D__st, D__st4, D__NONE, "FMUL", }, //0xcc
    { 0, C_FMUL, D__st, D__st5, D__NONE, "FMUL", }, //0xcd
    { 0, C_FMUL, D__st, D__st6, D__NONE, "FMUL", }, //0xce
    { 0, C_FMUL, D__st, D__st7, D__NONE, "FMUL", }, //0xcf

    { 0, C_NULL }, //0xd0
    { 0, C_NULL }, //0xd1
    { 0, C_NULL }, //0xd2
    { 0, C_NULL }, //0xd3
    { 0, C_NULL }, //0xd4
    { 0, C_NULL }, //0xd5
    { 0, C_NULL }, //0xd6
    { 0, C_NULL }, //0xd7

    { 0, C_NULL }, //0xd8
    { 0, C_NULL }, //0xd9
    { 0, C_NULL }, //0xda
    { 0, C_NULL }, //0xdb
    { 0, C_NULL }, //0xdc
    { 0, C_NULL }, //0xdd
    { 0, C_NULL }, //0xde
    { 0, C_NULL }, //0xdf

    { 0, C_FSUBR, D__st, D__st0, D__NONE, "FSUBR", }, //0xe0
    { 0, C_FSUBR, D__st, D__st1, D__NONE, "FSUBR", }, //0xe1
    { 0, C_FSUBR, D__st, D__st2, D__NONE, "FSUBR", }, //0xe2
    { 0, C_FSUBR, D__st, D__st3, D__NONE, "FSUBR", }, //0xe3
    { 0, C_FSUBR, D__st, D__st4, D__NONE, "FSUBR", }, //0xe4
    { 0, C_FSUBR, D__st, D__st5, D__NONE, "FSUBR", }, //0xe5
    { 0, C_FSUBR, D__st, D__st6, D__NONE, "FSUBR", }, //0xe6
    { 0, C_FSUBR, D__st, D__st7, D__NONE, "FSUBR", }, //0xe7

    { 0, C_FSUB, D__st, D__st0, D__NONE, "FSUB", }, //0xe8
    { 0, C_FSUB, D__st, D__st1, D__NONE, "FSUB", }, //0xe9
    { 0, C_FSUB, D__st, D__st2, D__NONE, "FSUB", }, //0xea
    { 0, C_FSUB, D__st, D__st3, D__NONE, "FSUB", }, //0xeb
    { 0, C_FSUB, D__st, D__st4, D__NONE, "FSUB", }, //0xec
    { 0, C_FSUB, D__st, D__st5, D__NONE, "FSUB", }, //0xed
    { 0, C_FSUB, D__st, D__st6, D__NONE, "FSUB", }, //0xee
    { 0, C_FSUB, D__st, D__st7, D__NONE, "FSUB", }, //0xef

    { 0, C_FDIVR, D__st, D__st0, D__NONE, "FDIVR", }, //0xf0
    { 0, C_FDIVR, D__st, D__st1, D__NONE, "FDIVR", }, //0xf1
    { 0, C_FDIVR, D__st, D__st2, D__NONE, "FDIVR", }, //0xf2
    { 0, C_FDIVR, D__st, D__st3, D__NONE, "FDIVR", }, //0xf3
    { 0, C_FDIVR, D__st, D__st4, D__NONE, "FDIVR", }, //0xf4
    { 0, C_FDIVR, D__st, D__st5, D__NONE, "FDIVR", }, //0xf5
    { 0, C_FDIVR, D__st, D__st6, D__NONE, "FDIVR", }, //0xf6
    { 0, C_FDIVR, D__st, D__st7, D__NONE, "FDIVR", }, //0xf7

    { 0, C_FDIV, D__st, D__st0, D__NONE, "FDIV", }, //0xf8
    { 0, C_FDIV, D__st, D__st1, D__NONE, "FDIV", }, //0xf9
    { 0, C_FDIV, D__st, D__st2, D__NONE, "FDIV", }, //0xfa
    { 0, C_FDIV, D__st, D__st3, D__NONE, "FDIV", }, //0xfb
    { 0, C_FDIV, D__st, D__st4, D__NONE, "FDIV", }, //0xfc
    { 0, C_FDIV, D__st, D__st5, D__NONE, "FDIV", }, //0xfd
    { 0, C_FDIV, D__st, D__st6, D__NONE, "FDIV", }, //0xfe
    { 0, C_FDIV, D__st, D__st7, D__NONE, "FDIV", } //0xff
};

DIS_OPCODE_T Group0xdd[] = //fpu group
{
    { R_M, C_FLD, D__double_real, D__NONE, D__NONE, "FLD", }, //0x0
    { R_M, C_NULL }, //0x1
    { R_M, C_FST, D__double_real, D__NONE, D__NONE, "FST", }, //0x2
    { R_M, C_FSTP, D__double_real, D__NONE, D__NONE, "FSTP", }, //0x3
    { R_M, C_FRSTOR, D__98_108b, D__NONE, D__NONE, "FRSTOR", }, //0x4
    { R_M, C_NULL }, //0x5
    { R_M, C_FSAVE, D__98_108b, D__NONE, D__NONE, "FSAVE", }, //0x6
    { R_M, C_FSTSW, D__2b, D__NONE, D__NONE, "FSTSW", }, //0x7

    //双字节浮点指令
    { 0, C_FFREE, D__st0, D__NONE, D__NONE, "FFREE", }, //0xc0
    { 0, C_FFREE, D__st1, D__NONE, D__NONE, "FFREE", }, //0xc1
    { 0, C_FFREE, D__st2, D__NONE, D__NONE, "FFREE", }, //0xc2
    { 0, C_FFREE, D__st3, D__NONE, D__NONE, "FFREE", }, //0xc3
    { 0, C_FFREE, D__st4, D__NONE, D__NONE, "FFREE", }, //0xc4
    { 0, C_FFREE, D__st5, D__NONE, D__NONE, "FFREE", }, //0xc5
    { 0, C_FFREE, D__st6, D__NONE, D__NONE, "FFREE", }, //0xc6
    { 0, C_FFREE, D__st7, D__NONE, D__NONE, "FFREE", }, //0xc7

    { 0, C_NULL }, //0xc8
    { 0, C_NULL }, //0xc9
    { 0, C_NULL }, //0xca
    { 0, C_NULL }, //0xcb
    { 0, C_NULL }, //0xcc
    { 0, C_NULL }, //0xcd
    { 0, C_NULL }, //0xce
    { 0, C_NULL }, //0xcf

    { 0, C_FST, D__st0, D__NONE, D__NONE, "FST", }, //0xd0
    { 0, C_FST, D__st1, D__NONE, D__NONE, "FST", }, //0xd1
    { 0, C_FST, D__st2, D__NONE, D__NONE, "FST", }, //0xd2
    { 0, C_FST, D__st3, D__NONE, D__NONE, "FST", }, //0xd3
    { 0, C_FST, D__st4, D__NONE, D__NONE, "FST", }, //0xd4
    { 0, C_FST, D__st5, D__NONE, D__NONE, "FST", }, //0xd5
    { 0, C_FST, D__st6, D__NONE, D__NONE, "FST", }, //0xd6
    { 0, C_FST, D__st7, D__NONE, D__NONE, "FST", }, //0xd7

    { 0, C_FSTP, D__st0, D__NONE, D__NONE, "FSTP", }, //0xd8
    { 0, C_FSTP, D__st1, D__NONE, D__NONE, "FSTP", }, //0xd9
    { 0, C_FSTP, D__st2, D__NONE, D__NONE, "FSTP", }, //0xda
    { 0, C_FSTP, D__st3, D__NONE, D__NONE, "FSTP", }, //0xdb
    { 0, C_FSTP, D__st4, D__NONE, D__NONE, "FSTP", }, //0xdc
    { 0, C_FSTP, D__st5, D__NONE, D__NONE, "FSTP", }, //0xdd
    { 0, C_FSTP, D__st6, D__NONE, D__NONE, "FSTP", }, //0xde
    { 0, C_FSTP, D__st7, D__NONE, D__NONE, "FSTP", }, //0xdf

    { 0, C_FUCOM, D__st0, D__st, D__NONE, "FUCOM", }, //0xe0
    { 0, C_FUCOM, D__st1, D__st, D__NONE, "FUCOM", }, //0xe1
    { 0, C_FUCOM, D__st2, D__st, D__NONE, "FUCOM", }, //0xe2
    { 0, C_FUCOM, D__st3, D__st, D__NONE, "FUCOM", }, //0xe3
    { 0, C_FUCOM, D__st4, D__st, D__NONE, "FUCOM", }, //0xe4
    { 0, C_FUCOM, D__st5, D__st, D__NONE, "FUCOM", }, //0xe5
    { 0, C_FUCOM, D__st6, D__st, D__NONE, "FUCOM", }, //0xe6
    { 0, C_FUCOM, D__st7, D__st, D__NONE, "FUCOM", }, //0xe7

    { 0, C_FUCOMP, D__st0, D__NONE, D__NONE, "FUCOMP", }, //0xe8
    { 0, C_FUCOMP, D__st1, D__NONE, D__NONE, "FUCOMP", }, //0xe9
    { 0, C_FUCOMP, D__st2, D__NONE, D__NONE, "FUCOMP", }, //0xea
    { 0, C_FUCOMP, D__st3, D__NONE, D__NONE, "FUCOMP", }, //0xeb
    { 0, C_FUCOMP, D__st4, D__NONE, D__NONE, "FUCOMP", }, //0xec
    { 0, C_FUCOMP, D__st5, D__NONE, D__NONE, "FUCOMP", }, //0xed
    { 0, C_FUCOMP, D__st6, D__NONE, D__NONE, "FUCOMP", }, //0xee
    { 0, C_FUCOMP, D__st7, D__NONE, D__NONE, "FUCOMP", }, //0xef

    { 0, C_NULL }, //0xf0
    { 0, C_NULL }, //0xf1
    { 0, C_NULL }, //0xf2
    { 0, C_NULL }, //0xf3
    { 0, C_NULL }, //0xf4
    { 0, C_NULL }, //0xf5
    { 0, C_NULL }, //0xf6
    { 0, C_NULL }, //0xf7

    { 0, C_NULL }, //0xf8
    { 0, C_NULL }, //0xf9
    { 0, C_NULL }, //0xfa
    { 0, C_NULL }, //0xfb
    { 0, C_NULL }, //0xfc
    { 0, C_NULL }, //0xfd
    { 0, C_NULL }, //0xfe
    { 0, C_NULL } //0xff
};

DIS_OPCODE_T Group0xde[] = //fpu group
{
    { R_M, C_FIADD, D__word_int, D__NONE, D__NONE, "FIADD", }, //0x0
    { R_M, C_FIMUL, D__word_int, D__NONE, D__NONE, "FIMUL", }, //0x1
    { R_M, C_FICOM, D__word_int, D__NONE, D__NONE, "FICOM", }, //0x2
    { R_M, C_FICOMP, D__word_int, D__NONE, D__NONE, "FICOMP", }, //0x3
    { R_M, C_FISUB, D__word_int, D__NONE, D__NONE, "FISUB", }, //0x4
    { R_M, C_FISUBR, D__word_int, D__NONE, D__NONE, "FISUBR", }, //0x5
    { R_M, C_FIDIV, D__word_int, D__NONE, D__NONE, "FIDIV", }, //0x6
    { R_M, C_FIDIVR, D__word_int, D__NONE, D__NONE, "FIDIVR", }, //0x7

    //双字节浮点指令
    { 0, C_FADDP, D__st0, D__st, D__NONE, "FADDP", }, //0xc0
    { 0, C_FADDP, D__st1, D__st, D__NONE, "FADDP", }, //0xc1
    { 0, C_FADDP, D__st2, D__st, D__NONE, "FADDP", }, //0xc2
    { 0, C_FADDP, D__st3, D__st, D__NONE, "FADDP", }, //0xc3
    { 0, C_FADDP, D__st4, D__st, D__NONE, "FADDP", }, //0xc4
    { 0, C_FADDP, D__st5, D__st, D__NONE, "FADDP", }, //0xc5
    { 0, C_FADDP, D__st6, D__st, D__NONE, "FADDP", }, //0xc6
    { 0, C_FADDP, D__st7, D__st, D__NONE, "FADDP", }, //0xc7

    { 0, C_FMULP, D__st0, D__st, D__NONE, "FMULP", }, //0xc8
    { 0, C_FMULP, D__st1, D__st, D__NONE, "FMULP", }, //0xc9
    { 0, C_FMULP, D__st2, D__st, D__NONE, "FMULP", }, //0xca
    { 0, C_FMULP, D__st3, D__st, D__NONE, "FMULP", }, //0xcb
    { 0, C_FMULP, D__st4, D__st, D__NONE, "FMULP", }, //0xcc
    { 0, C_FMULP, D__st5, D__st, D__NONE, "FMULP", }, //0xcd
    { 0, C_FMULP, D__st6, D__st, D__NONE, "FMULP", }, //0xce
    { 0, C_FMULP, D__st7, D__st, D__NONE, "FMULP", }, //0xcf

    { 0, C_NULL }, //0xd0
    { 0, C_NULL }, //0xd1
    { 0, C_NULL }, //0xd2
    { 0, C_NULL }, //0xd3
    { 0, C_NULL }, //0xd4
    { 0, C_NULL }, //0xd5
    { 0, C_NULL }, //0xd6
    { 0, C_NULL }, //0xd7

    { 0, C_NULL }, //0xd8
    { 0, C_FCOMPP, D__NONE, D__NONE, D__NONE, "FCOMPP", }, //0xd9
    { 0, C_NULL }, //0xda
    { 0, C_NULL }, //0xdb
    { 0, C_NULL }, //0xdc
    { 0, C_NULL }, //0xdd
    { 0, C_NULL }, //0xde
    { 0, C_NULL }, //0xdf

    { 0, C_FSUBRP, D__st0, D__st, D__NONE, "FSUBRP", }, //0xe0
    { 0, C_FSUBRP, D__st1, D__st, D__NONE, "FSUBRP", }, //0xe1
    { 0, C_FSUBRP, D__st2, D__st, D__NONE, "FSUBRP", }, //0xe2
    { 0, C_FSUBRP, D__st3, D__st, D__NONE, "FSUBRP", }, //0xe3
    { 0, C_FSUBRP, D__st4, D__st, D__NONE, "FSUBRP", }, //0xe4
    { 0, C_FSUBRP, D__st5, D__st, D__NONE, "FSUBRP", }, //0xe5
    { 0, C_FSUBRP, D__st6, D__st, D__NONE, "FSUBRP", }, //0xe6
    { 0, C_FSUBRP, D__st7, D__st, D__NONE, "FSUBRP", }, //0xe7

    { 0, C_FSUBP, D__st0, D__st, D__NONE, "FSUBP", }, //0xe8
    { 0, C_FSUBP, D__st1, D__st, D__NONE, "FSUBP", }, //0xe9
    { 0, C_FSUBP, D__st2, D__st, D__NONE, "FSUBP", }, //0xea
    { 0, C_FSUBP, D__st3, D__st, D__NONE, "FSUBP", }, //0xeb
    { 0, C_FSUBP, D__st4, D__st, D__NONE, "FSUBP", }, //0xec
    { 0, C_FSUBP, D__st5, D__st, D__NONE, "FSUBP", }, //0xed
    { 0, C_FSUBP, D__st6, D__st, D__NONE, "FSUBP", }, //0xee
    { 0, C_FSUBP, D__st7, D__st, D__NONE, "FSUBP", }, //0xef

    { 0, C_FDIVRP, D__st0, D__st, D__NONE, "FDIVRP", }, //0xf0
    { 0, C_FDIVRP, D__st1, D__st, D__NONE, "FDIVRP", }, //0xf1
    { 0, C_FDIVRP, D__st2, D__st, D__NONE, "FDIVRP", }, //0xf2
    { 0, C_FDIVRP, D__st3, D__st, D__NONE, "FDIVRP", }, //0xf3
    { 0, C_FDIVRP, D__st4, D__st, D__NONE, "FDIVRP", }, //0xf4
    { 0, C_FDIVRP, D__st5, D__st, D__NONE, "FDIVRP", }, //0xf5
    { 0, C_FDIVRP, D__st6, D__st, D__NONE, "FDIVRP", }, //0xf6
    { 0, C_FDIVRP, D__st7, D__st, D__NONE, "FDIVRP", }, //0xf7

    { 0, C_FDIVP, D__st0, D__st, D__NONE, "FDIVP", }, //0xf8
    { 0, C_FDIVP, D__st1, D__st, D__NONE, "FDIVP", }, //0xf9
    { 0, C_FDIVP, D__st2, D__st, D__NONE, "FDIVP", }, //0xfa
    { 0, C_FDIVP, D__st3, D__st, D__NONE, "FDIVP", }, //0xfb
    { 0, C_FDIVP, D__st4, D__st, D__NONE, "FDIVP", }, //0xfc
    { 0, C_FDIVP, D__st5, D__st, D__NONE, "FDIVP", }, //0xfd
    { 0, C_FDIVP, D__st6, D__st, D__NONE, "FDIVP", }, //0xfe
    { 0, C_FDIVP, D__st7, D__st, D__NONE, "FDIVP", } //0xff
};

DIS_OPCODE_T Group0xdf[] = //fpu group
{
    //与内存数据有有关的浮点指令 长度跟指令(add D__Gb,D__Eb)一样
    { R_M, C_FILD, D__word_int, D__NONE, D__NONE, "FILD", }, //0x0
    { R_M, C_NULL }, //0x1
    { R_M, C_FIST, D__word_int, D__NONE, D__NONE, "FIST", }, //0x2
    { R_M, C_FISTP, D__word_int, D__NONE, D__NONE, "FISTP", }, //0x3
    { R_M, C_FBLD, D__packed_BCD, D__NONE, D__NONE, "FBLD", }, //0x4
    { R_M, C_FILD, D__qword_int, D__NONE, D__NONE, "FILD", }, //0x5
    { R_M, C_FBSTP, D__packed_BCD, D__NONE, D__NONE, "FBSTP", }, //0x6
    { R_M, C_FISTP, D__qword_int, D__NONE, D__NONE, "FISTP", }, //0x7

    //双字节浮点指令
    { 0, C_NULL }, //0xc0
    { 0, C_NULL }, //0xc1
    { 0, C_NULL }, //0xc2
    { 0, C_NULL }, //0xc3
    { 0, C_NULL }, //0xc4
    { 0, C_NULL }, //0xc5
    { 0, C_NULL }, //0xc6
    { 0, C_NULL }, //0xc7

    { 0, C_NULL }, //0xc8
    { 0, C_NULL }, //0xc9
    { 0, C_NULL }, //0xca
    { 0, C_NULL }, //0xcb
    { 0, C_NULL }, //0xcc
    { 0, C_NULL }, //0xcd
    { 0, C_NULL }, //0xce
    { 0, C_NULL }, //0xcf

    { 0, C_NULL }, //0xd0
    { 0, C_NULL }, //0xd1
    { 0, C_NULL }, //0xd2
    { 0, C_NULL }, //0xd3
    { 0, C_NULL }, //0xd4
    { 0, C_NULL }, //0xd5
    { 0, C_NULL }, //0xd6
    { 0, C_NULL }, //0xd7

    { 0, C_NULL }, //0xd8
    { 0, C_NULL }, //0xd9
    { 0, C_NULL }, //0xda
    { 0, C_NULL }, //0xdb
    { 0, C_NULL }, //0xdc
    { 0, C_NULL }, //0xdd
    { 0, C_NULL }, //0xde
    { 0, C_NULL }, //0xdf

    { 0, C_FSTSW, D__AX, D__NONE, D__NONE, "FSTSW" }, //0xe0
    { 0, C_NULL }, //0xe1
    { 0, C_NULL }, //0xe2
    { 0, C_NULL }, //0xe3
    { 0, C_NULL }, //0xe4
    { 0, C_NULL }, //0xe5
    { 0, C_NULL }, //0xe6
    { 0, C_NULL }, //0xe7

    { 0, C_FUCOMIP, D__st, D__st0, D__NONE, "FUCOMIP", }, //0xe8
    { 0, C_FUCOMIP, D__st, D__st1, D__NONE, "FUCOMIP", }, //0xe9
    { 0, C_FUCOMIP, D__st, D__st2, D__NONE, "FUCOMIP", }, //0xea
    { 0, C_FUCOMIP, D__st, D__st3, D__NONE, "FUCOMIP", }, //0xeb
    { 0, C_FUCOMIP, D__st, D__st4, D__NONE, "FUCOMIP", }, //0xec
    { 0, C_FUCOMIP, D__st, D__st5, D__NONE, "FUCOMIP", }, //0xed
    { 0, C_FUCOMIP, D__st, D__st6, D__NONE, "FUCOMIP", }, //0xee
    { 0, C_FUCOMIP, D__st, D__st7, D__NONE, "FUCOMIP", }, //0xef

    { 0, C_FCOMIP, D__st, D__st0, D__NONE, "FCOMIP", }, //0xf0
    { 0, C_FCOMIP, D__st, D__st1, D__NONE, "FCOMIP", }, //0xf1
    { 0, C_FCOMIP, D__st, D__st2, D__NONE, "FCOMIP", }, //0xf2
    { 0, C_FCOMIP, D__st, D__st3, D__NONE, "FCOMIP", }, //0xf3
    { 0, C_FCOMIP, D__st, D__st4, D__NONE, "FCOMIP", }, //0xf4
    { 0, C_FCOMIP, D__st, D__st5, D__NONE, "FCOMIP", }, //0xf5
    { 0, C_FCOMIP, D__st, D__st6, D__NONE, "FCOMIP", }, //0xf6
    { 0, C_FCOMIP, D__st, D__st7, D__NONE, "FCOMIP", }, //0xf7

    { 0, C_NULL }, //0xf8
    { 0, C_NULL }, //0xf9
    { 0, C_NULL }, //0xfa
    { 0, C_NULL }, //0xfb
    { 0, C_NULL }, //0xfc
    { 0, C_NULL }, //0xfd
    { 0, C_NULL }, //0xfe
    { 0, C_NULL } //0xff

};

DIS_OPCODE_T Group0x0f00[] = //CPU保护模式指令
{
    { R_M, C_SLDT, D_wEw, D__NONE, D__NONE, "SLDT", }, //0x0
    { R_M, C_STR, D_wEw, D__NONE, D__NONE, "STR", }, //0x1
    { R_M, C_LLDT, D_rEw, D__NONE, D__NONE, "LLDT", }, //0x2
    { R_M, C_LTR, D_rEw, D__NONE, D__NONE, "LTR", }, //0x3
    { R_M, C_VERR, D_rEw, D__NONE, D__NONE, "VERR", }, //0x4
    { R_M, C_VERW, D_rEw, D__NONE, D__NONE, "VERW", }, //0x5
    { R_M, C_NULL }, //0x6
    { R_M, C_NULL } //0x7

};

DIS_OPCODE_T Group0x0f01[] = //CPU保护模式指令
{
    { R_M, C_SGDT, D_wMs, D__NONE, D__NONE, "SGDT", }, //0x0
    { R_M, C_SIDT, D_wMs, D__NONE, D__NONE, "SIDT", }, //0x1
    { R_M, C_LGDT, D_rMs, D__NONE, D__NONE, "LGDT", }, //0x2
    { R_M, C_LIDT, D_rMs, D__NONE, D__NONE, "LIDT", }, //0x3
    { R_M, C_SMSW, D_wEw, D__NONE, D__NONE, "SMSW", }, //0x4
    { R_M, C_NULL }, //0x5
    { R_M, C_LMSW, D_rEw, D__NONE, D__NONE, "LMSW", }, //0x6
    { R_M, C_INVLPG, D__Mb, D__NONE, D__NONE, "INVLPG" } //0x7

};

DIS_OPCODE_T Group0x0f18[] =  //80386
{
    { R_M, C_PREFETCH, D__NTA, D__NONE, D__NONE, "prefetch" }, //0x0
    { R_M, C_PREFETCH, D__T0, D__NONE, D__NONE, "prefetch" }, //0x1
    { R_M, C_PREFETCH, D__T1, D__NONE, D__NONE, "prefetch" }, //0x2
    { R_M, C_PREFETCH, D__T2, D__NONE, D__NONE, "prefetch" }, //0x3
    { R_M, C_NULL, }, //0x4
    { R_M, C_NULL, }, //0x5
    { R_M, C_NULL, }, //0x6
    { R_M, C_NULL, }, //0x7

    //Group0x0f18_11b[] =  //80386
    { R_M, C_NULL, }, //0x0
    { R_M, C_NULL, }, //0x1
    { R_M, C_NULL, }, //0x2
    { R_M, C_NULL, }, //0x3
    { R_M, C_NULL, }, //0x4
    { R_M, C_NULL, }, //0x5
    { R_M, C_NULL, }, //0x6
    { R_M, C_NULL, } //0x7

};

DIS_OPCODE_T Group0x0f71[] =  //80386
{
    { R_M, C_NULL, }, //0x0
    { R_M, C_NULL, }, //0x1
    { R_M, C_NULL, }, //0x2
    { R_M, C_NULL, }, //0x3
    { R_M, C_NULL, }, //0x4
    { R_M, C_NULL, }, //0x5
    { R_M, C_NULL, }, //0x6
    { R_M, C_NULL, }, //0x7

    //Group0x0f71_11b[] =  //80386
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, }, //0x0
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, }, //0x1
    { R_M | Immediate_Ib, C_PSRLW, D__Pq, D__Ib, D__NONE, "psrlw", }, //0x2
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, }, //0x3
    { R_M | Immediate_Ib, C_PSRAW, D__Pq, D__Ib, D__NONE, "psraw", }, //0x4
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, }, //0x5
    { R_M | Immediate_Ib, C_PSLLW, D__Pq, D__Ib, D__NONE, "psllw", }, //0x6
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, } //0x7

};

DIS_OPCODE_T Group0x0f72[] =  //80386
{
    { R_M, C_NULL, }, //0x0
    { R_M, C_NULL, }, //0x1
    { R_M, C_NULL, }, //0x2
    { R_M, C_NULL, }, //0x3
    { R_M, C_NULL, }, //0x4
    { R_M, C_NULL, }, //0x5
    { R_M, C_NULL, }, //0x6
    { R_M, C_NULL, }, //0x7

    //Group0x0f72_11b[] =  //80386
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, }, //0x0
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, }, //0x1
    { R_M | Immediate_Ib, C_PSRLD, D__Pq, D__Ib, D__NONE, "psrld" }, //0x2
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, }, //0x3
    { R_M | Immediate_Ib, C_PSRAD, D__Pq, D__Ib, D__NONE, "psrad" }, //0x4
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, }, //0x5
    { R_M | Immediate_Ib, C_PSLLD, D__Pq, D__Ib, D__NONE, "pslld" }, //0x6
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, } //0x7

};

DIS_OPCODE_T Group0x0f73[] =  //80386
{
    { R_M, C_NULL, }, //0x0
    { R_M, C_NULL, }, //0x1
    { R_M, C_NULL, }, //0x2
    { R_M, C_NULL, }, //0x3
    { R_M, C_NULL, }, //0x4
    { R_M, C_NULL, }, //0x5
    { R_M, C_NULL, }, //0x6
    { R_M, C_NULL, }, //0x7

    //Group0x0f73_11b[] =  //80386
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, }, //0x0
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, }, //0x1
    { R_M | Immediate_Ib, C_PSRLQ, D__Pq, D__Ib, D__NONE, "psrlq" }, //0x2
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, }, //0x3
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, }, //0x4
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, }, //0x5
    { R_M | Immediate_Ib, C_PSLLQ, D__Pq, D__Ib, D__NONE, "psllq" }, //0x6
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, } //0x7

};

DIS_OPCODE_T Group0x0fae[] =  //80386
{
    { R_M, C_FXSAVE, D__NONE, D__NONE, D__NONE, "fxsave", }, //0x0
    { R_M, C_FXRSTOR, D__NONE, D__NONE, D__NONE, "fxrstor", }, //0x1
    { R_M, C_IDMXXCR, D__NONE, D__NONE, D__NONE, "idmxxcr", }, //0x2
    { R_M, C_STMXCSR, D__NONE, D__NONE, D__NONE, "stmxcsr", }, //0x3
    { R_M, C_NULL, }, //0x4
    { R_M, C_NULL, }, //0x5
    { R_M, C_NULL, }, //0x6
    { R_M, C_CLFLUSH, D__NONE, D__NONE, D__NONE, "clflush" }, //0x7

    //Group0x0fae_11b[] =  //80386
    { R_M, C_NULL, }, //0x0
    { R_M, C_NULL, }, //0x1
    { R_M, C_NULL, }, //0x2
    { R_M, C_NULL, }, //0x3
    { R_M, C_NULL, }, //0x4
    { R_M, C_LFENCE, D__NONE, D__NONE, D__NONE, "lfence" }, //0x5
    { R_M, C_MFENCE, D__NONE, D__NONE, D__NONE, "mfence" }, //0x6
    { R_M, C_SFENCE, D__NONE, D__NONE, D__NONE, "sfence" } //0x7

};

DIS_OPCODE_T Group0x0fba[] =  //80386
{
    { R_M, C_NULL, }, //0x0
    { R_M, C_NULL, }, //0x1
    { R_M, C_NULL, }, //0x2
    { R_M, C_NULL, }, //0x3
    { R_M | Immediate_Ib, C_BT, D_rEv, D__Ib, D__NONE, "BT", }, //0x4
    { R_M | Immediate_Ib, C_BTS, D__Ev, D__Ib, D__NONE, "BTS", }, //0x5
    { R_M | Immediate_Ib, C_BTR, D__Ev, D__Ib, D__NONE, "BTR", }, //0x6
    { R_M | Immediate_Ib, C_BTC, D__Ev, D__Ib, D__NONE, "BTC", } //0x7

};

DIS_OPCODE_T Group0x0fc7[] =  //80386
{
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, }, //0x00
    { R_M, C_CMPXCH8B, D__Mq, D__NONE, D__NONE, "CMPXCH8B", }, //0x1
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, }, //0x2
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, }, //0x3
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, }, //0x4
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, }, //0x5
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, }, //0x6
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, } //0x7

};

DIS_OPCODE_T Group0x0f[] =
{
    { R_M, C_GRP, D__NONE, D__NONE, D__NONE, (char*)Group0x0f00 }, //0x00
    { R_M, C_GRP, D__NONE, D__NONE, D__NONE, (char*)Group0x0f01 }, //0x01
    { R_M, C_LAR, D__Gv, D_rEv, D__NONE, "LAR", }, //0x02
    { R_M, C_LSL, D__Gv, D_rEv, D__NONE, "LSL", }, //0x03
    { R_M, C_NULL, }, //0x04
    { R_M, C_NULL, }, //0x05
    { 0, C_CLTS, D__NONE, D__NONE, D__NONE, "CLTS", }, //0x06
    { R_M, C_NULL, }, //0x07

    { 0, C_INVD, D__NONE, D__NONE, D__NONE, "INVD", }, //0x08
    { 0, C_WBINVD, D__NONE, D__NONE, D__NONE, "WBINVD", }, //0x09
    { R_M, C_NULL }, //0x0a
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, "2-byte Illegal Opcodes UD2 1B 1 Prefetch 1C" }, //0x0b
    { R_M, C_NULL, }, //0x0c
    { R_M, C_NULL, }, //0x0d
    { R_M, C_NULL, }, //0x0e
    { R_M, C_NULL, }, //0x0f


    { R_M, C_MOVUPS, D__Vps, D__Wps, D__NONE, "movups" }, //0x10
    { R_M, C_MOVUPS, D__Wps, D__Vps, D__NONE, "movups" }, //0x11
    { R_M, C_MOVLPS, D__Wq, D__Vq, D__NONE, "movlps" }, //0x12
    { R_M, C_MOVLPS, D__Vq, D__Wq, D__NONE, "movlps" }, //0x13
    { R_M, C_UNPCKLPS, D__Vps, D__Wq, D__NONE, "unpcklps" }, //0x14
    { R_M, C_UNPCKHPS, D__Vps, D__Wq, D__NONE, "unpckhps" }, //0x15
    { R_M, C_MOVHPS, D__Vq, D__Wq, D__NONE, "movhps" }, //0x16 
    { R_M, C_MOVHPS, D__Wq, D__Vq, D__NONE, "movhps" }, //0x17

    { R_M, C_GRP1, D__NONE, D__NONE, D__NONE, (char*)Group0x0f18 }, //0x18
    { R_M, C_NULL, }, //0x19
    { R_M, C_NULL, }, //0x1a
    { R_M, C_NULL, }, //0x1b
    { R_M, C_NULL, }, //0x1c
    { R_M, C_NULL, }, //0x1d
    { R_M, C_NULL, }, //0x1e
    { R_M, C_NULL, }, //0x1f

    { R_M, C_MOV, D__Rd, D__Cd, D__NONE, "MOV" }, //0x20
    { R_M, C_MOV, D__Rd, D__Dd, D__NONE, "MOV" }, //0x21
    { R_M, C_MOV, D__Cd, D__Rd, D__NONE, "MOV" }, //0x22
    { R_M, C_MOV, D__Dd, D__Rd, D__NONE, "MOV" }, //0x23
    { R_M, C_MOV, D__Rd, D__Td, D__NONE, "MOV" }, //0x24
    { R_M, C_NULL, }, //0x25
    { R_M, C_MOV, D__Td, D__Rd, D__NONE, "MOV" }, //0x26
    { R_M, C_NULL, }, //0x27

    { R_M, C_MOVAPS, D__Vps, D__Wps, D__NONE, "movaps" }, //0x28
    { R_M, C_MOVAPS, D__Wps, D__Vps, D__NONE, "movaps" }, //0x29
    { R_M, C_CVTPI2PS, D__Vps, D__Qq, D__NONE, "cvtpi2ps" }, //0x2a
    { R_M, C_MOVNTPS, D__Wps, D__Vps, D__NONE, "movntps" }, //0x2b
    { R_M, C_CVTTPS2PI, D__Qq, D__Wps, D__NONE, "cvttps2pi" }, //0x2c
    { R_M, C_CVTPS2PI, D__Qq, D__Wps, D__NONE, "cvtps2pi" }, //0x2d
    { R_M, C_UCOMISS, D__Vss, D__Wss, D__NONE, "ucomiss" }, //0x2e
    { R_M, C_COMISS, D__Vps, D__Wps, D__NONE, "comiss" }, //0x2f

    { 0, C_WRMSR, D__NONE, D__NONE, D__NONE, "WRMSR" }, //0x30
    { 0, C_RDTSC, D__NONE, D__NONE, D__NONE, "RDTSC", }, //0x31
    { 0, C_RDMSR, D__NONE, D__NONE, D__NONE, "RDMSR" }, //0x32
    { 0, C_RDPMC, D__NONE, D__NONE, D__NONE, "RDPMC" }, //0x33
    { 0, C_SYSENTER, D__NONE, D__NONE, D__NONE, "SYSENTER" }, //0x34
    { 0, C_SYSEXIT, D__NONE, D__NONE, D__NONE, "SYSEXIT" }, //0x35
    { R_M, C_NULL, }, //0x36
    { R_M, C_NULL, }, //0x37

    { R_M, C_NULL, }, //0x38
    { R_M, C_NULL, }, //0x39
    { R_M, C_NULL, }, //0x3a
    { R_M, C_NULL, }, //0x3b
    { R_M, C_NULL, }, //0x3c
    { R_M, C_NULL, }, //0x3d
    { R_M, C_NULL, }, //0x3e
    { R_M, C_NULL, }, //0x3f

    { R_M, C_CMOVtO, D__Gv, D_rEv, D__tO, "CMOVO", }, //0x40
    { R_M, C_CMOVfO, D__Gv, D_rEv, D__fO, "CMOVNO", }, //0x41
    { R_M, C_CMOVtC, D__Gv, D_rEv, D__tC, "CMOVC", }, //0x42
    { R_M, C_CMOVfC, D__Gv, D_rEv, D__fC, "CMOVNC", }, //0x43
    { R_M, C_CMOVtZ, D__Gv, D_rEv, D__tZ, "CMOVZ", }, //0x44
    { R_M, C_CMOVfZ, D__Gv, D_rEv, D__fZ, "CMOVNZ", }, //0x45
    { R_M, C_CMOVfA, D__Gv, D_rEv, D__fA, "CMOVNA", }, //0x46
    { R_M, C_CMOVtA, D__Gv, D_rEv, D__tA, "CMOVA", }, //0x47
    { R_M, C_CMOVtS, D__Gv, D_rEv, D__tS, "CMOVS", }, //0x48
    { R_M, C_CMOVfS, D__Gv, D_rEv, D__fS, "CMOVNS", }, //0x49
    { R_M, C_CMOVtP, D__Gv, D_rEv, D__tP, "CMOVP", }, //0x4a
    { R_M, C_CMOVfP, D__Gv, D_rEv, D__fP, "CMOVNP", }, //0x4b
    { R_M, C_CMOVtL, D__Gv, D_rEv, D__tL, "CMOVL", }, //0x4c
    { R_M, C_CMOVfL, D__Gv, D_rEv, D__fL, "CMOVNL", }, //0x4d
    { R_M, C_CMOVfG, D__Gv, D_rEv, D__fG, "CMOVNG", }, //0x4e
    { R_M, C_CMOVtG, D__Gv, D_rEv, D__tG, "CMOVG", }, //0x4f

    { R_M, C_MOVMSKPS, D__Ed, D__Vps, D__NONE, "movmskps" }, //0x50
    { R_M, C_SQRTPS, D__Vps, D__Wps, D__NONE, "sqrtps" }, //0x51
    { R_M, C_RSQRTPS, D__Vps, D__Wps, D__NONE, "rsqrtps" }, //0x52
    { R_M, C_RCPPS, D__Vps, D__Wps, D__NONE, "rcpps" }, //0x53
    { R_M, C_ANDPS, D__Vps, D__Wps, D__NONE, "andps" }, //0x54
    { R_M, C_ANDNPS, D__Vps, D__Wps, D__NONE, "andnps" }, //0x55
    { R_M, C_ORPS, D__Vps, D__Wps, D__NONE, "orps" }, //0x56
    { R_M, C_XORPS, D__Vps, D__Wps, D__NONE, "xorps" }, //0x57


    { R_M, C_ADDPS, D__Vps, D__Wps, D__NONE, "addps" }, //0x58
    { R_M, C_MULPS, D__Vps, D__Wps, D__NONE, "mulps" }, //0x59
    { R_M, C_CVTPS2PD, D__Vpd, D__Wps, D__NONE, "cvtps2pd" }, //0x5a
    { R_M, C_CVTDQ2PS, D__Vps, D__Wdq, D__NONE, "cvtdq2ps" }, //0x5b
    { R_M, C_SUBPS, D__Vps, D__Wps, D__NONE, "subps" }, //0x5c
    { R_M, C_MINPS, D__Vps, D__Wps, D__NONE, "minps" }, //0x5d
    { R_M, C_DIVPS, D__Vps, D__Wps, D__NONE, "divps" }, //0x5e
    { R_M, C_MAXPS, D__Vps, D__Wps, D__NONE, "maxps" }, //0x5f

    { R_M, C_PUNPCKLBW, D__Pq, D__Qd, D__NONE, "punpcklbw" }, //0x60
    { R_M, C_PUNPCKLWD, D__Pq, D__Qd, D__NONE, "punpcklwd" }, //0x61
    { R_M, C_PUNPCKLDQ, D__Pq, D__Qd, D__NONE, "punpckldq" }, //0x62
    { R_M, C_PACKSSWB, D__Pq, D__Qq, D__NONE, "packsswb" }, //0x63
    { R_M, C_PCMPGTB, D__Pq, D__Qq, D__NONE, "pcmpgtb" }, //0x64
    { R_M, C_PCMPGTW, D__Pq, D__Qq, D__NONE, "pcmpgtw" }, //0x65
    { R_M, C_PCMPGTD, D__Pq, D__Qq, D__NONE, "pcmpgtd" }, //0x66
    { R_M, C_PACKUSWB, D__Pq, D__Qq, D__NONE, "packuswb" }, //0x67

    { R_M, C_PUNPCKHBW, D__Pq, D__Qd, D__NONE, "punpckhbw" }, //0x68
    { R_M, C_PUNPCKHWD, D__Pq, D__Qd, D__NONE, "punpckhwd" }, //0x69
    { R_M, C_PUNPCKHDQ, D__Pq, D__Qd, D__NONE, "punpckhdq" }, //0x6a
    { R_M, C_PACKSSDW, D__Pq, D__Qd, D__NONE, "packssdw" }, //0x6b
    { R_M, C_PUNPCKLQDQ, D__Pq, D__Qd, D__NONE, "punpcklqdq" }, //0x6c
    { R_M, C_PUNPCKHQD, D__Pq, D__Qd, D__NONE, "punpckhqd" }, //0x6d
    { R_M, C_MOVD, D__Pd, D__Ed, D__NONE, "movd" }, //0x6e
    { R_M, C_MOVQ, D__Pq, D__Qq, D__NONE, "movq" }, //0x6f

    { R_M, C_PSHUFW, D__Pq, D__Qq, D__Ib, "pshufw" }, //0x70 
    { R_M, C_GRP1, D__Pq, D__Qq, D__NONE, (char*)Group0x0f71 }, //0x71
    { R_M, C_GRP1, D__Pq, D__Qq, D__NONE, (char*)Group0x0f72 }, //0x72
    { R_M, C_GRP1, D__Pq, D__Qq, D__NONE, (char*)Group0x0f73 }, //0x73
    { R_M, C_PCMPEQB, D__Pq, D__Qq, D__Ib, "pcmpeqb" }, //0x74
    { R_M, C_PCMPEQW, D__Pq, D__Qq, D__Ib, "pcmpeqw" }, //0x75
    { R_M, C_PCMPEQD, D__Pq, D__Qq, D__Ib, "pcmpeqd" }, //0x76
    { R_M, C_EMMS, D__NONE, D__NONE, D__NONE, "emms" }, //0x77

    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, "MMX UD" }, //0x78
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, "MMX UD" }, //0x79
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, "MMX UD" }, //0x7a
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, "MMX UD" }, //0x7b
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, "MMX UD" }, //0x7c
    { R_M, C_NULL, D__NONE, D__NONE, D__NONE, "MMX UD" }, //0x7d
    { R_M, C_MOVD, D__Ed, D__Pd, D__NONE, "movd" }, //0x7e
    { R_M, C_MOVQ, D__Qq, D__Pq, D__NONE, "movq" }, //0x7f

    { 0 | Immediate_Jv, C_JtO, D__Jv, D__tO, D__NONE, "JO", }, //0x80
    { 0 | Immediate_Jv, C_JfO, D__Jv, D__fO, D__NONE, "JNO", }, //0x81
    { 0 | Immediate_Jv, C_JtC, D__Jv, D__tC, D__NONE, "JC", }, //0x82
    { 0 | Immediate_Jv, C_JfC, D__Jv, D__fC, D__NONE, "JNC", }, //0x83
    { 0 | Immediate_Jv, C_JtZ, D__Jv, D__tZ, D__NONE, "JZ", }, //0x84
    { 0 | Immediate_Jv, C_JfZ, D__Jv, D__fZ, D__NONE, "JNZ", }, //0x85
    { 0 | Immediate_Jv, C_JfA, D__Jv, D__fA, D__NONE, "JNA", }, //0x86
    { 0 | Immediate_Jv, C_JtA, D__Jv, D__tA, D__NONE, "JA", }, //0x87
    { 0 | Immediate_Jv, C_JtS, D__Jv, D__tS, D__NONE, "JS", }, //0x88
    { 0 | Immediate_Jv, C_JfS, D__Jv, D__fS, D__NONE, "JNS", }, //0x89
    { 0 | Immediate_Jv, C_JtP, D__Jv, D__tP, D__NONE, "JP", }, //0x8a
    { 0 | Immediate_Jv, C_JfP, D__Jv, D__fP, D__NONE, "JNP", }, //0x8b
    { 0 | Immediate_Jv, C_JtL, D__Jv, D__tL, D__NONE, "JL", }, //0x8c
    { 0 | Immediate_Jv, C_JfL, D__Jv, D__fL, D__NONE, "JNL", }, //0x8d
    { 0 | Immediate_Jv, C_JfG, D__Jv, D__fG, D__NONE, "JNG", }, //0x8e
    { 0 | Immediate_Jv, C_JtG, D__Jv, D__tG, D__NONE, "JG", }, //0x8f

    { R_M, C_SETtO, D_wEb, D__tO, D__NONE, "SETO", }, //0x90
    { R_M, C_SETfO, D_wEb, D__fO, D__NONE, "SETNO", }, //0x91
    { R_M, C_SETtC, D_wEb, D__tC, D__NONE, "SETC", }, //0x92
    { R_M, C_SETfC, D_wEb, D__fC, D__NONE, "SETNC", }, //0x93
    { R_M, C_SETtZ, D_wEb, D__tZ, D__NONE, "SETZ", }, //0x94
    { R_M, C_SETfZ, D_wEb, D__fZ, D__NONE, "SETNZ", }, //0x95
    { R_M, C_SETfA, D_wEb, D__fA, D__NONE, "SETNA", }, //0x96
    { R_M, C_SETtA, D_wEb, D__tA, D__NONE, "SETA", }, //0x97
    { R_M, C_SETtS, D_wEb, D__tS, D__NONE, "SETS", }, //0x98
    { R_M, C_SETfS, D_wEb, D__fS, D__NONE, "SETNS", }, //0x99
    { R_M, C_SETtP, D_wEb, D__tP, D__NONE, "SETP", }, //0x9a
    { R_M, C_SETfP, D_wEb, D__fP, D__NONE, "SETNP", }, //0x9b
    { R_M, C_SETtL, D_wEb, D__tL, D__NONE, "SETL", }, //0x9c
    { R_M, C_SETfL, D_wEb, D__fL, D__NONE, "SETNL", }, //0x9d
    { R_M, C_SETfG, D_wEb, D__fG, D__NONE, "SETNG", }, //0x9e
    { R_M, C_SETtG, D_wEb, D__tG, D__NONE, "SETG", }, //0x9f

    { 0, C_PUSH, D__FS, D__NONE, D__NONE, "PUSH", }, //0xa0
    { 0, C_POP, D__FS, D__NONE, D__NONE, "POP", }, //0xa1
    { 0, C_CPUID, D__NONE, D__NONE, D__NONE, "CPUID", }, //0xa2
    { R_M, C_BT, D_rEv, D__Gv, D__NONE, "BT", }, //0xa3
    { R_M, C_SHLD, D__Ev, D__Gv, D__Ib, "SHLD", }, //0xa4
    { R_M, C_SHLD, D__Ev, D__Gv, D__CL, "SHLD", }, //0xa5
    { 0, C_NULL }, //0xa6
    { 0, C_NULL }, //0xa7

    { 0, C_PUSH, D__GS, D__NONE, D__NONE, "PUSP", }, //0xa8
    { 0, C_POP, D__GS, D__NONE, D__NONE, "POP", }, //0xa9
    { 0, C_RSM, D__NONE, D__NONE, D__NONE, "RSM", }, //0xaa
    { R_M, C_BTS, D__Ev, D__Gv, D__NONE, "BTS", }, //0xab
    { R_M, C_SHRD, D__Ev, D__Gv, D__Ib, "SHRD", }, //0xac
    { R_M, C_SHRD, D__Ev, D__Gv, D__CL, "SHRD", }, //0xad
    { R_M, C_GRP1, D__NONE, D__NONE, D__NONE, (char*)Group0x0fae }, //0xae
    { R_M, C_IMUL, D__Gv, D_rEv, D__NONE, "IMUL", }, //0xaf

    { R_M, C_CMPXCHG, D__Eb, D__Gb, D__NONE, "CMPXCHG", }, //0xb0
    { R_M, C_CMPXCHG, D__Ev, D__Gv, D__NONE, "CMPXCHG", }, //0xb1
    { R_M, C_LSS, D__Gv, D_rMp, D__NONE, "LSS", }, //0xb2
    { R_M, C_BTR, D__Ev, D__Gv, D__NONE, "BTR", }, //0xb3
    { R_M, C_LFS, D__Gv, D_rMp, D__NONE, "LFS", }, //0xb4
    { R_M, C_LGS, D__Gv, D_rMp, D__NONE, "LGS", }, //0xb5
    { R_M, C_MOVZX, D__Gv, D_rEb, D__NONE, "MOVZX", }, //0xb6
    { R_M, C_MOVZX, D__Gv, D_rEw, D__NONE, "MOVZX", }, //0xb7

    { R_M, C_NULL }, //0xb8
    { R_M, C_NULL }, //0xb9
    { R_M | Immediate_Ib, C_GRP, D__Ev, D__Ib, D__NONE, (char*)Group0x0fba }, //0xba
    { R_M, C_BTC, D__Ev, D__Gv, D__NONE, "BTC", }, //0xbb
    { R_M, C_BSF, D__Gv, D__Ev, D__NONE, "BSF", }, //0xbc
    { R_M, C_BSR, D__Gv, D__Ev, D__NONE, "BSR", }, //0xbd
    { R_M, C_MOVSX, D__Gv, D_rEb, D__NONE, "MOVSX", }, //0xbe
    { R_M, C_MOVSX, D__Gv, D_rEw, D__NONE, "MOVSX", }, //0xbf

    { R_M, C_XADD, D__Eb, D__Gb, D__NONE, "XADD", }, //0xc0
    { R_M, C_XADD, D__Ev, D__Gv, D__NONE, "XADD", }, //0xc1
    { R_M, C_CMPPS, D__Vps, D__Wps, D__Ib, "cmpps" }, //0xc2
    { R_M, C_MOVNTI, D__Ed, D__Gd, D__NONE, "movnti" }, //0xc3
    { R_M | Immediate_Ib, C_PINSRW, D__Pq, D__Ed, D__Ib, "pinsrw" }, //0xc4
    { R_M | Immediate_Ib, C_PEXTRW, D__Gd, D__Pq, D__Ib, "pextrw" }, //0xc5
    { R_M | Immediate_Ib, C_SHUFPS, D__Vps, D__Wps, D__Ib, "shufps" }, //0xc6
    { R_M, C_GRP, D__NONE, D__NONE, D__NONE, (char*)Group0x0fc7 }, //0xc7

    { 0, C_BSWAP, D__EAX, D__NONE, D__NONE, "BSWAP", }, //0xc8
    { 0, C_BSWAP, D__ECX, D__NONE, D__NONE, "BSWAP", }, //0xc9
    { 0, C_BSWAP, D__EDX, D__NONE, D__NONE, "BSWAP", }, //0xca
    { 0, C_BSWAP, D__EBX, D__NONE, D__NONE, "BSWAP", }, //0xcb
    { 0, C_BSWAP, D__ESP, D__NONE, D__NONE, "BSWAP", }, //0xcc
    { 0, C_BSWAP, D__EBP, D__NONE, D__NONE, "BSWAP", }, //0xcd
    { 0, C_BSWAP, D__ESI, D__NONE, D__NONE, "BSWAP", }, //0xce
    { 0, C_BSWAP, D__EDI, D__NONE, D__NONE, "BSWAP", }, //0xcf

    { R_M, C_NULL }, //0xd0
    { R_M, C_PSRLW, D__Pq, D__Qq, D__NONE, "psrlw" }, //0xd1
    { R_M, C_PSRLD, D__Pq, D__Qq, D__NONE, "psrld" }, //0xd2
    { R_M, C_PSRLQ, D__Pq, D__Qq, D__NONE, "psrlq" }, //0xd3
    { R_M, C_PADDQ, D__Pq, D__Qq, D__NONE, "paddq" }, //0xd4
    { R_M, C_PMULLW, D__Pq, D__Qq, D__NONE, "pmullw" }, //0xd5
    { R_M, C_MOVQ, D__Wq, D__Vq, D__NONE, "movq" }, //0xd6
    { R_M, C_PMOVMSKB, D__Gd, D__Pq, D__NONE, "pmovmskb" }, //0xd7

    { R_M, C_PSUBUSB, D__Pq, D__Qq, D__NONE, "psubusb" }, //0xd8
    { R_M, C_PSUNUSW, D__Pq, D__Qq, D__NONE, "psunusw" }, //0xd9
    { R_M, C_PMINUB, D__Pq, D__Qq, D__NONE, "pminub" }, //0xda
    { R_M, C_PAND, D__Pq, D__Qq, D__NONE, "pand" }, //0xdb
    { R_M, C_PADDUSB, D__Pq, D__Qq, D__NONE, "paddusb" }, //0xdc
    { R_M, C_PADDUSW, D__Pq, D__Qq, D__NONE, "paddusw" }, //0xdd
    { R_M, C_PMAXUB, D__Pq, D__Qq, D__NONE, "pmaxub" }, //0xde
    { R_M, C_PANDN, D__Pq, D__Qq, D__NONE, "pandn" }, //0xdf

    { R_M, C_PAVGB, D__Pq, D__Qq, D__NONE, "pavgb" }, //0xe0
    { R_M, C_PSRAW, D__Pq, D__Qq, D__NONE, "psraw" }, //0xe1
    { R_M, C_PSRAD, D__Pq, D__Qq, D__NONE, "psrad" }, //0xe2
    { R_M, C_PAVGW, D__Pq, D__Qq, D__NONE, "pavgw" }, //0xe3
    { R_M, C_PMULHUW, D__Pq, D__Qq, D__NONE, "pmulhuw" }, //0xe4
    { R_M, C_PMULHW, D__Pq, D__Qq, D__NONE, "pmulhw" }, //0xe5
    { 0, C_CVTPD2DQ, D__NONE, D__NONE, D__NONE, "cvtpd2dq" }, //0xe6
    { R_M, C_MOVNTQ, D__Wq, D__Vq, D__NONE, "movntq" }, //0xe7

    { R_M, C_PSUBSB, D__Pq, D__Qq, D__NONE, "psubsb" }, //0xe8
    { R_M, C_PSUBSW, D__Pq, D__Qq, D__NONE, "psubsw" }, //0xe9
    { R_M, C_PMINSW, D__Pq, D__Qq, D__NONE, "pminsw" }, //0xea
    { R_M, C_POR, D__Pq, D__Qq, D__NONE, "por" }, //0xeb
    { R_M, C_PADDSB, D__Pq, D__Qq, D__NONE, "paddsb" }, //0xec
    { R_M, C_PADDSW, D__Pq, D__Qq, D__NONE, "paddsw" }, //0xed
    { R_M, C_PMAXSW, D__Pq, D__Qq, D__NONE, "pmaxsw" }, //0xee
    { R_M, C_PXOR, D__Pq, D__Qq, D__NONE, "pxor" }, //0xef


    { R_M, C_NULL }, //0xf0
    { R_M, C_PSLLW, D__Pq, D__Qq, D__NONE, "psllw" }, //0xf1
    { R_M, C_PSLLD, D__Pq, D__Qq, D__NONE, "pslld" }, //0xf2
    { R_M, C_PSLLQ, D__Pq, D__Qq, D__NONE, "psllq" }, //0xf3
    { R_M, C_PMULUDQ, D__Pq, D__Qq, D__NONE, "pmuludq" }, //0xf4
    { R_M, C_PMADDWD, D__Pq, D__Qq, D__NONE, "pmaddwd" }, //0xf5
    { R_M, C_PSADBW, D__Pq, D__Qq, D__NONE, "psadbw" }, //0xf6
    { R_M, C_MASKMOVQ, D__Ppi, D__Qpi, D__NONE, "maskmovq" }, //0xf7

    { R_M, C_PSUBB, D__Pq, D__Qq, D__NONE, "psubb" }, //0xf8
    { R_M, C_PSUBW, D__Pq, D__Qq, D__NONE, "psubw" }, //0xf9
    { R_M, C_PSUBD, D__Pq, D__Qq, D__NONE, "psubd" }, //0xfa
    { R_M, C_PSUBD, D__Pq, D__Qq, D__NONE, "psubd" }, //0xfb
    { R_M, C_PADDB, D__Pq, D__Qq, D__NONE, "paddb" }, //0xfc
    { R_M, C_PADDW, D__Pq, D__Qq, D__NONE, "paddw" }, //0xfd
    { R_M, C_PADDD, D__Pq, D__Qq, D__NONE, "paddd" }, //0xfe
    { R_M, C_NULL }, //0xff

};

DIS_OPCODE_T g_szAllOpCode[] =
{
    { R_M, C_ADD, D__Eb, D__Gb, D__NONE, "ADD", }, //0x00
    { R_M, C_ADD, D__Ev, D__Gv, D__NONE, "ADD", }, //0x01
    { R_M, C_ADD, D__Gb, D_rEb, D__NONE, "ADD", }, //0x02
    { R_M, C_ADD, D__Gv, D_rEv, D__NONE, "ADD", }, //0x03
    { 0 | Immediate_Ib, C_ADD, D__AL, D__Ib, D__NONE, "ADD", }, //0x04
    { 0 | Immediate_Iv, C_ADD, D__eX, D__Iv, D__NONE, "ADD", }, //0x05
    { 0, C_PUSH, D__ES, D__NONE, D__NONE, "PUSH", }, //0x06
    { 0, C_POP, D__ES, D__NONE, D__NONE, "POP", }, //0x07

    { R_M, C_OR, D__Eb, D__Gb, D__NONE, "OR", }, //0x08
    { R_M, C_OR, D__Ev, D__Gv, D__NONE, "OR", }, //0x09
    { R_M, C_OR, D__Gb, D_rEb, D__NONE, "OR", }, //0x0a
    { R_M, C_OR, D__Gv, D_rEv, D__NONE, "OR", }, //0x0b
    { 0 | Immediate_Ib, C_OR, D__AL, D__Ib, D__NONE, "OR", }, //0x0c
    { 0 | Immediate_Iv, C_OR, D__eX, D__Iv, D__NONE, "OR", }, //0x0d
    { 0, C_PUSH, D__CS, D__NONE, D__NONE, "PUSH", }, //0x0e
    { 0, C_2BYTE, D__CS, D__NONE, D__NONE, (char*)Group0x0f }, //0x0f

    { R_M, C_ADC, D__Eb, D__Gb, D__NONE, "ADC", }, //0x10
    { R_M, C_ADC, D__Ev, D__Gv, D__NONE, "ADC", }, //0x11
    { R_M, C_ADC, D__Gb, D_rEb, D__NONE, "ADC", }, //0x12
    { R_M, C_ADC, D__Gv, D_rEv, D__NONE, "ADC", }, //0x13
    { 0 | Immediate_Ib, C_ADC, D__AL, D__Ib, D__NONE, "ADC", }, //0x14
    { 0 | Immediate_Iv, C_ADC, D__eX, D__Iv, D__NONE, "ADC", }, //0x15
    { 0, C_PUSH, D__SS, D__NONE, D__NONE, "PUSH", }, //0x16
    { 0, C_POP, D__SS, D__NONE, D__NONE, "POP", }, //0x17

    { R_M, C_SBB, D__Eb, D__Gb, D__NONE, "SBB", }, //0x18
    { R_M, C_SBB, D__Ev, D__Gv, D__NONE, "SBB", }, //0x19
    { R_M, C_SBB, D__Gb, D_rEb, D__NONE, "SBB", }, //0x1a
    { R_M, C_SBB, D__Gv, D_rEv, D__NONE, "SBB", }, //0x1b
    { 0 | Immediate_Ib, C_SBB, D__AL, D__Ib, D__NONE, "SBB", }, //0x1c
    { 0 | Immediate_Iv, C_SBB, D__eX, D__Iv, D__NONE, "SBB", }, //0x1d
    { 0, C_PUSH, D__DS, D__NONE, D__NONE, "PUSH", }, //0x1e
    { 0, C_POP, D__DS, D__NONE, D__NONE, "POP", }, //0x1f

    { R_M, C_AND, D__Eb, D__Gb, D__NONE, "AND", }, //0x20
    { R_M, C_AND, D__Ev, D__Gv, D__NONE, "AND", }, //0x21
    { R_M, C_AND, D__Gb, D_rEb, D__NONE, "AND", }, //0x22
    { R_M, C_AND, D__Gv, D_rEv, D__NONE, "AND", }, //0x23
    { 0 | Immediate_Ib, C_AND, D__AL, D__Ib, D__NONE, "AND", }, //0x24
    { 0 | Immediate_Iv, C_AND, D__eX, D__Iv, D__NONE, "AND", }, //0x25
    { 0, C_ES, D__NONE, D__NONE, D__NONE, "ES:" }, //0x26
    { 0, C_DAA, D__NONE, D__NONE, D__NONE, "DAA", }, //0x27

    { R_M, C_SUB, D__Eb, D__Gb, D__NONE, "SUB", }, //0x28
    { R_M, C_SUB, D__Ev, D__Gv, D__NONE, "SUB", }, //0x29
    { R_M, C_SUB, D__Gb, D_rEb, D__NONE, "SUB", }, //0x2a
    { R_M, C_SUB, D__Gv, D_rEv, D__NONE, "SUB", }, //0x2b
    { 0 | Immediate_Ib, C_SUB, D__AL, D__Ib, D__NONE, "SUB", }, //0x2c
    { 0 | Immediate_Iv, C_SUB, D__eX, D__Iv, D__NONE, "SUB", }, //0x2d
    { 0, C_CS, D__NONE, D__NONE, D__NONE, "CS:" }, //0x2e
    { 0, C_DAS, D__NONE, D__NONE, D__NONE, "DAS", }, //0x2f

    { R_M, C_XOR, D__Eb, D__Gb, D__NONE, "XOR", }, //0x30
    { R_M, C_XOR, D__Ev, D__Gv, D__NONE, "XOR", }, //0x31
    { R_M, C_XOR, D__Gb, D_rEb, D__NONE, "XOR", }, //0x32
    { R_M, C_XOR, D__Gv, D_rEv, D__NONE, "XOR", }, //0x33
    { 0 | Immediate_Ib, C_XOR, D__AL, D__Ib, D__NONE, "XOR", }, //0x34
    { 0 | Immediate_Iv, C_XOR, D__eX, D__Iv, D__NONE, "XOR", }, //0x35
    { 0, C_SS, D__NONE, D__NONE, D__NONE, "SS:" }, //0x36
    { 0, C_AAA, D__NONE, D__NONE, D__NONE, "AAA", }, //0x37

    { R_M, C_CMP, D_rEb, D__Gb, D__NONE, "CMP", }, //0x38
    { R_M, C_CMP, D_rEv, D__Gv, D__NONE, "CMP", }, //0x39
    { R_M, C_CMP, D__Gb, D_rEb, D__NONE, "CMP", }, //0x3a
    { R_M, C_CMP, D__Gv, D_rEv, D__NONE, "CMP", }, //0x3b
    { 0 | Immediate_Ib, C_CMP, D__AL, D__Ib, D__NONE, "CMP", }, //0x3c
    { 0 | Immediate_Iv, C_CMP, D__eX, D__Iv, D__NONE, "CMP", }, //0x3d
    { 0, C_DS, D__NONE, D__NONE, D__NONE, "DS:" }, //0x3e
    { 0, C_AAS, D__NONE, D__NONE, D__NONE, "AAS", }, //0x3f

    { 0, C_INC, D__eAX, D__NONE, D__NONE, "INC", }, //0x40
    { 0, C_INC, D__eCX, D__NONE, D__NONE, "INC", }, //0x41
    { 0, C_INC, D__eDX, D__NONE, D__NONE, "INC", }, //0x42
    { 0, C_INC, D__eBX, D__NONE, D__NONE, "INC", }, //0x43
    { 0, C_INC, D__eSP, D__NONE, D__NONE, "INC", }, //0x44
    { 0, C_INC, D__eBP, D__NONE, D__NONE, "INC", }, //0x45
    { 0, C_INC, D__eSI, D__NONE, D__NONE, "INC", }, //0x46
    { 0, C_INC, D__eDI, D__NONE, D__NONE, "INC", }, //0x47

    { 0, C_DEC, D__eAX, D__NONE, D__NONE, "DEC", }, //0x48
    { 0, C_DEC, D__eCX, D__NONE, D__NONE, "DEC", }, //0x49
    { 0, C_DEC, D__eDX, D__NONE, D__NONE, "DEC", }, //0x4a
    { 0, C_DEC, D__eBX, D__NONE, D__NONE, "DEC", }, //0x4b
    { 0, C_DEC, D__eSP, D__NONE, D__NONE, "DEC", }, //0x4c
    { 0, C_DEC, D__eBP, D__NONE, D__NONE, "DEC", }, //0x4d
    { 0, C_DEC, D__eSI, D__NONE, D__NONE, "DEC", }, //0x4e
    { 0, C_DEC, D__eDI, D__NONE, D__NONE, "DEC", }, //0x4f

    { 0, C_PUSH, D__eAX, D__NONE, D__NONE, "PUSH", }, //0x50
    { 0, C_PUSH, D__eCX, D__NONE, D__NONE, "PUSH", }, //0x51
    { 0, C_PUSH, D__eDX, D__NONE, D__NONE, "PUSH", }, //0x52
    { 0, C_PUSH, D__eBX, D__NONE, D__NONE, "PUSH", }, //0x53
    { 0, C_PUSH, D__eSP, D__NONE, D__NONE, "PUSH", }, //0x54
    { 0, C_PUSH, D__eBP, D__NONE, D__NONE, "PUSH", }, //0x55
    { 0, C_PUSH, D__eSI, D__NONE, D__NONE, "PUSH", }, //0x56
    { 0, C_PUSH, D__eDI, D__NONE, D__NONE, "PUSH", }, //0x57

    { 0, C_POP, D__eAX, D__NONE, D__NONE, "POP", }, //0x58
    { 0, C_POP, D__eCX, D__NONE, D__NONE, "POP", }, //0x59
    { 0, C_POP, D__eDX, D__NONE, D__NONE, "POP", }, //0x5a
    { 0, C_POP, D__eBX, D__NONE, D__NONE, "POP", }, //0x5b
    { 0, C_POP, D__eSP, D__NONE, D__NONE, "POP", }, //0x5c
    { 0, C_POP, D__eBP, D__NONE, D__NONE, "POP", }, //0x5d
    { 0, C_POP, D__eSI, D__NONE, D__NONE, "POP", }, //0x5e
    { 0, C_POP, D__eDI, D__NONE, D__NONE, "POP", }, //0x5f

    { 0, C_PUSHA, D__NONE, D__NONE, D__NONE, "PUSHA", }, //0x60
    { 0, C_POPA, D__NONE, D__NONE, D__NONE, "POPA", }, //0x61
    { R_M, C_BOUND, D__Gv, D_rMa, D__NONE, "BOUND", }, //0x62
    { R_M, C_ARPL, D__Ew, D__Gw, D__NONE, "ARPL", }, //0x63
    { 0, C_FS, D__NONE, D__NONE, D__NONE, "FS:" }, //0x64
    { 0, C_GS, D__NONE, D__NONE, D__NONE, "GS:" }, //0x65
    { 0, C_OPER }, //0x66
    { 0, C_ADDR }, //0x67

    { 0 | Immediate_Iv, C_PUSH, D__Iv, D__NONE, D__NONE, "PUSH", }, //0x68
    { R_M | Immediate_Iv, C_IMUL, D__Gv, D_rEv, D__Iv, "IMUL", }, //0x69
    { 0 | Immediate_sIb, C_PUSH, D_sIb, D__NONE, D__NONE, "PUSH", }, //0x6a
    { R_M | Immediate_sIb, C_IMUL, D__Gv, D_rEv, D_sIb, "IMUL", }, //0x6b
    { 0, C_INS, D__Yb, D__DX, D__NONE, "INS", }, //0x6c
    { 0, C_INS, D__Yv, D__DX, D__NONE, "INS", }, //0x6d
    { 0, C_OUTS, D__DX, D__Xb, D__NONE, "OUTS", }, //0x6e
    { 0, C_OUTS, D__DX, D__Xv, D__NONE, "OUTS", }, //0x6f

    { 0 | Immediate_Jb, C_JtO, D__Jb, D__tO, D__NONE, "JO", }, //0x70
    { 0 | Immediate_Jb, C_JfO, D__Jb, D__fO, D__NONE, "JNO", }, //0x71
    { 0 | Immediate_Jb, C_JtC, D__Jb, D__tC, D__NONE, "JC", }, //0x72
    { 0 | Immediate_Jb, C_JfC, D__Jb, D__fC, D__NONE, "JNC", }, //0x73
    { 0 | Immediate_Jb, C_JtZ, D__Jb, D__tZ, D__NONE, "JZ", }, //0x74
    { 0 | Immediate_Jb, C_JfZ, D__Jb, D__fZ, D__NONE, "JNZ", }, //0x75
    { 0 | Immediate_Jb, C_JfA, D__Jb, D__fA, D__NONE, "JNA", }, //0x76
    { 0 | Immediate_Jb, C_JtA, D__Jb, D__tA, D__NONE, "JA", }, //0x77
    { 0 | Immediate_Jb, C_JtS, D__Jb, D__tS, D__NONE, "JS", }, //0x78
    { 0 | Immediate_Jb, C_JfS, D__Jb, D__fS, D__NONE, "JNS", }, //0x79
    { 0 | Immediate_Jb, C_JtP, D__Jb, D__tP, D__NONE, "JP", }, //0x7a
    { 0 | Immediate_Jb, C_JfP, D__Jb, D__fP, D__NONE, "JNP", }, //0x7b
    { 0 | Immediate_Jb, C_JtL, D__Jb, D__tL, D__NONE, "JL", }, //0x7c
    { 0 | Immediate_Jb, C_JfL, D__Jb, D__fL, D__NONE, "JNL", }, //0x7d
    { 0 | Immediate_Jb, C_JfG, D__Jb, D__fG, D__NONE, "JNG", }, //0x7e
    { 0 | Immediate_Jb, C_JtG, D__Jb, D__tG, D__NONE, "JG", }, //0x7f

    { R_M | Immediate_Ib, C_GRP, D__Eb, D__Ib, D__NONE, (char*)Group0x80 }, //0x80
    { R_M | Immediate_Iv, C_GRP, D__Ev, D__Iv, D__NONE, (char*)Group0x81 }, //0x81
    { R_M | Immediate_Ib, C_GRP, D__Eb, D__Ib, D__NONE, (char*)Group0x82 }, //0x82
    { R_M | Immediate_sIb, C_GRP, D__Ev, D_sIb, D__NONE, (char*)Group0x83 }, //0x83
    { R_M, C_TEST, D_rEb, D__Gb, D__NONE, "TEST", }, //0x84
    { R_M, C_TEST, D_rEv, D__Gv, D__NONE, "TEST", }, //0x85
    { R_M, C_XCHG, D__Eb, D__Gb, D__NONE, "XCHG", }, //0x86
    { R_M, C_XCHG, D__Ev, D__Gv, D__NONE, "XCHG", }, //0x87

    { R_M, C_MOV, D_wEb, D__Gb, D__NONE, "MOV", }, //0x88
    { R_M, C_MOV, D_wEv, D__Gv, D__NONE, "MOV", }, //0x89
    { R_M, C_MOV, D__Gb, D_rEb, D__NONE, "MOV", }, //0x8a
    { R_M, C_MOV, D__Gv, D_rEv, D__NONE, "MOV", }, //0x8b
    { R_M, C_MOV, D_wEw, D__Sw, D__NONE, "MOV", }, //0x8c
    { R_M, C_LEA, D__Gv, D__M, D__NONE, "LEA", }, //0x8d
    { R_M, C_MOV, D__Sw, D_rEw, D__NONE, "MOV", }, //0x8e
    { R_M, C_GRP, D__Ev, D__NONE, D__NONE, (char*)Group0x8f }, //0x8f

    { 0, C_NOP, D__NONE, D__NONE, D__NONE, "NOP", }, //0x90
    { 0, C_XCHG, D__eCX, D__eAX, D__NONE, "XCHG", }, //0x91
    { 0, C_XCHG, D__eDX, D__eAX, D__NONE, "XCHG", }, //0x92
    { 0, C_XCHG, D__eBX, D__eAX, D__NONE, "XCHG", }, //0x93
    { 0, C_XCHG, D__eSP, D__eAX, D__NONE, "XCHG", }, //0x94
    { 0, C_XCHG, D__eBP, D__eAX, D__NONE, "XCHG", }, //0x95
    { 0, C_XCHG, D__eSI, D__eAX, D__NONE, "XCHG", }, //0x96
    { 0, C_XCHG, D__eDI, D__eAX, D__NONE, "XCHG", }, //0x97

    { 0, C_CBW, D__NONE, D__NONE, D__NONE, "CBW", }, //0x98
    { 0, C_CWD, D__NONE, D__NONE, D__NONE, "CWD", }, //0x99
    { 0 | Immediate_IvIw, C_CALL, D__Ap, D__NONE, D__NONE, "CALL", }, //0x9a
    { 0, C_WAIT, D__NONE, D__NONE, D__NONE, "WAIT", }, //0x9b
    { 0, C_PUSHF, D__Fv, D__NONE, D__NONE, "PUSHF", }, //0x9c
    { 0, C_POPF, D__Fv, D__NONE, D__NONE, "POPF", }, //0x9d
    { 0, C_SAHF, D__NONE, D__NONE, D__NONE, "SAHF", }, //0x9e
    { 0, C_LAHF, D__NONE, D__NONE, D__NONE, "LAHF", }, //0x9f

    { 0, C_MOV, D__AL, D_rOb, D__NONE, "MOV", }, //0xa0
    { 0, C_MOV, D__eX, D_rOv, D__NONE, "MOV", }, //0xa1
    { 0, C_MOV, D_wOb, D__AL, D__NONE, "MOV", }, //0xa2
    { 0, C_MOV, D_wOv, D__eX, D__NONE, "MOV", }, //0xa3
    { 0, C_MOVS, D_wYb, D_rXb, D__NONE, "MOVS", }, //0xa4
    { 0, C_MOVS, D_wYv, D_rXv, D__NONE, "MOVS", }, //0xa5
    { 0, C_CMPS, D_rYb, D_rXb, D__NONE, "CMPS", }, //0xa6
    { 0, C_CMPS, D_rYv, D_rXv, D__NONE, "CMPS", }, //0xa7

    { 0, C_TEST, D__AL, D__Ib, D__NONE, "TEST", }, //0xa8
    { 0, C_TEST, D__eX, D__Iv, D__NONE, "TEST", }, //0xa9
    { 0, C_STOS, D_wYb, D__AL, D__NONE, "STOS", }, //0xaa
    { 0, C_STOS, D_wYv, D__eX, D__NONE, "STOS", }, //0xab
    { 0, C_LODS, D__AL, D_rXb, D__NONE, "LODS", }, //0xac
    { 0, C_LODS, D__eX, D_rXv, D__NONE, "LODS", }, //0xad
    { 0, C_SCAS, D_rYb, D__AL, D__NONE, "SCAS", }, //0xae
    { 0, C_SCAS, D_rYv, D__eX, D__NONE, "SCAS", }, //0xaf

    { 0 | Immediate_Ib, C_MOV, D__AL, D__Ib, D__NONE, "MOV", }, //0xb0
    { 0 | Immediate_Ib, C_MOV, D__CL, D__Ib, D__NONE, "MOV", }, //0xb1
    { 0 | Immediate_Ib, C_MOV, D__DL, D__Ib, D__NONE, "MOV", }, //0xb2
    { 0 | Immediate_Ib, C_MOV, D__BL, D__Ib, D__NONE, "MOV", }, //0xb3
    { 0 | Immediate_Ib, C_MOV, D__AH, D__Ib, D__NONE, "MOV", }, //0xb4
    { 0 | Immediate_Ib, C_MOV, D__CH, D__Ib, D__NONE, "MOV", }, //0xb5
    { 0 | Immediate_Ib, C_MOV, D__DH, D__Ib, D__NONE, "MOV", }, //0xb6
    { 0 | Immediate_Ib, C_MOV, D__BH, D__Ib, D__NONE, "MOV", }, //0xb7

    { 0 | Immediate_Iv, C_MOV, D__eAX, D__Iv, D__NONE, "MOV", }, //0xb8
    { 0 | Immediate_Iv, C_MOV, D__eCX, D__Iv, D__NONE, "MOV", }, //0xb9
    { 0 | Immediate_Iv, C_MOV, D__eDX, D__Iv, D__NONE, "MOV", }, //0xba
    { 0 | Immediate_Iv, C_MOV, D__eBX, D__Iv, D__NONE, "MOV", }, //0xbb
    { 0 | Immediate_Iv, C_MOV, D__eSP, D__Iv, D__NONE, "MOV", }, //0xbc
    { 0 | Immediate_Iv, C_MOV, D__eBP, D__Iv, D__NONE, "MOV", }, //0xbd
    { 0 | Immediate_Iv, C_MOV, D__eSI, D__Iv, D__NONE, "MOV", }, //0xbe
    { 0 | Immediate_Iv, C_MOV, D__eDI, D__Iv, D__NONE, "MOV", }, //0xbf 

    { R_M | Immediate_Ib, C_GRP, D__Eb, D__Ib, D__NONE, (char*)Group0xc0 }, //0xc0
    { R_M | Immediate_Ib, C_GRP, D__Ev, D__Ib, D__NONE, (char*)Group0xc1 }, //0xc1
    { 0 | Immediate_Iw, C_RET, D__Iw, D__NONE, D__NONE, "RET", }, //0xc2
    { 0, C_RET, D__NONE, D__NONE, D__NONE, "RET", }, //0xc3
    { R_M, C_LES, D__Gv, D_rMp, D__NONE, "LES", }, //0xc4
    { R_M, C_LDS, D__Gv, D_rMp, D__NONE, "LDS", }, //0xc5
    { R_M, C_GRP, D__Eb, D__Ib, D__NONE, (char*)Group0xc6 }, //0xc6
    { R_M, C_GRP, D__Ev, D__Iv, D__NONE, (char*)Group0xc7 }, //0xc7

    { 0 | Immediate_IwIb, C_ENTER, D__Iw, D__Ib, D__NONE, "ENTER", }, //0xc8
    { 0, C_LEAVE, D__NONE, D__NONE, D__NONE, "LEAVE", }, //0xc9
    { 0 | Immediate_Iw, C_RETF, D__Iw, D__NONE, D__NONE, "RETF", }, //0xca
    { 0, C_RETF, D__NONE, D__NONE, D__NONE, "RETF", }, //0xcb
    { 0, C_INT3, D__NONE, D__NONE, D__NONE, "INT3", }, //0xcc
    { 0 | Immediate_Ib, C_INT, D__Ib, D__NONE, D__NONE, "INT", }, //0xcd
    { 0, C_INTO, D__NONE, D__NONE, D__NONE, "INTO", }, //0xce
    { 0, C_IRET, D__NONE, D__NONE, D__NONE, "IRET", }, //0xcf

    { R_M, C_GRP, D__Eb, D__1, D__NONE, (char*)Group0xd0 }, //0xd0
    { R_M, C_GRP, D__Ev, D__1, D__NONE, (char*)Group0xd1 }, //0xd1
    { R_M, C_GRP, D__Eb, D__CL, D__NONE, (char*)Group0xd2 }, //0xd2
    { R_M, C_GRP, D__Ev, D__CL, D__NONE, (char*)Group0xd3 }, //0xd3
    { 0 | Immediate_Ib, C_AAM, D__Ib, D__NONE, D__NONE, "AAM", }, //0xd4
    { 0 | Immediate_Ib, C_AAD, D__Ib, D__NONE, D__NONE, "AAD", }, //0xd5
    { 0, C_SALC, D__NONE, D__NONE, D__NONE, "SALC", }, //0xd6
    { 0, C_XLAT, D__NONE, D__NONE, D__NONE, "XLAT", }, //0xd7

    { R_M, C_ESC, D__NONE, D__NONE, D__NONE, (char*)Group0xd8, }, //0xd8
    { R_M, C_ESC, D__NONE, D__NONE, D__NONE, (char*)Group0xd9, }, //0xd9
    { R_M, C_ESC, D__NONE, D__NONE, D__NONE, (char*)Group0xda, }, //0xda
    { R_M, C_ESC, D__NONE, D__NONE, D__NONE, (char*)Group0xdb, }, //0xdb
    { R_M, C_ESC, D__NONE, D__NONE, D__NONE, (char*)Group0xdc, }, //0xdc
    { R_M, C_ESC, D__NONE, D__NONE, D__NONE, (char*)Group0xdd, }, //0xdd
    { R_M, C_ESC, D__NONE, D__NONE, D__NONE, (char*)Group0xde, }, //0xde
    { R_M, C_ESC, D__NONE, D__NONE, D__NONE, (char*)Group0xdf, }, //0xdf


    { 0 | Immediate_Jb, C_LOOPNZ, D__Jb, D__NONE, D__NONE, "LOOPNZ", }, //0xe0
    { 0 | Immediate_Jb, C_LOOPZ, D__Jb, D__NONE, D__NONE, "LOOPZ", }, //0xe1
    { 0 | Immediate_Jb, C_LOOP, D__Jb, D__NONE, D__NONE, "LOOP", }, //0xe2
    { 0 | Immediate_Jb, C_JCXZ, D__Jb, D__NONE, D__NONE, "JCXZ", }, //0xe3
    { 0 | Immediate_Ib, C_IN, D__AL, D__Ib, D__NONE, "IN", }, //0xe4
    { 0 | Immediate_Ib, C_IN, D__eX, D__Ib, D__NONE, "IN", }, //0xe5
    { 0 | Immediate_Ib, C_OUT, D__Ib, D__AL, D__NONE, "OUT", }, //0xe6
    { 0 | Immediate_Ib, C_OUT, D__Ib, D__eX, D__NONE, "OUT", }, //0xe7

    { 0 | Immediate_Jv, C_CALL, D__Jv, D__NONE, D__NONE, "CALL", }, //0xe8
    { 0 | Immediate_Jv, C_JMP, D__Jv, D__NONE, D__NONE, "JMP", }, //0xe9
    { 0 | Immediate_IvIw, C_JMP, D__Ap, D__NONE, D__NONE, "JMP", }, //0xea
    { 0 | Immediate_Jb, C_JMP, D__Jb, D__NONE, D__NONE, "JMP", }, //0xeb
    { 0, C_IN, D__AL, D__DX, D__NONE, "IN", }, //0xec
    { 0, C_IN, D__eX, D__DX, D__NONE, "IN", }, //0xed
    { 0, C_OUT, D__DX, D__AL, D__NONE, "OUT", }, //0xee
    { 0, C_OUT, D__DX, D__eX, D__NONE, "OUT", }, //0xef

    { 0, C_LOCK, D__NONE, D__NONE, D__NONE, "LOCK" }, //0xf0
    { 0, C_INT1, D__NONE, D__NONE, D__NONE, "INT1", }, //0xf1
    { 0, C_REPN, D__NONE, D__NONE, D__NONE, "REPNE" }, //0xf2
    { 0, C_REPZ, D__NONE, D__NONE, D__NONE, "REP" }, //0xf3
    { 0, C_HLT, D__NONE, D__NONE, D__NONE, "HLT", }, //0xf4
    { 0, C_CMC, D__NONE, D__NONE, D__NONE, "CMC", }, //0xf5
    { R_M, C_GRP, D__Eb, D__NONE, D__NONE, (char*)Group0xf6 }, //0xf6
    { R_M, C_GRP, D__Ev, D__NONE, D__NONE, (char*)Group0xf7 }, //0xf7
    { 0, C_CLC, D__NONE, D__NONE, D__NONE, "CLC", }, //0xf8
    { 0, C_STC, D__NONE, D__NONE, D__NONE, "STC", }, //0xf9
    { 0, C_CLI, D__NONE, D__NONE, D__NONE, "CLI", }, //0xfa
    { 0, C_STI, D__NONE, D__NONE, D__NONE, "STI", }, //0xfb
    { 0, C_CLD, D__NONE, D__NONE, D__NONE, "CLD", }, //0xfc
    { 0, C_STD, D__NONE, D__NONE, D__NONE, "STD", }, //0xfd
    { R_M, C_GRP, D__Eb, D__NONE, D__NONE, (char*)Group0xfe }, //0xfe
    { R_M, C_GRP, D__Ev, D__NONE, D__NONE, (char*)Group0xff },  //0xff

    { R_M, C_INS, D__Yb, D__DX, D__NONE, "INSB" },//0x6c,
    { R_M, C_INS, D__Yw, D__DX, D__NONE, "INSW" },//0x6d,
    { R_M, C_INS, D__Yd, D__DX, D__NONE, "INSD" },//0x6d,
    { R_M, C_OUTS, D__DX, D__Xb, D__NONE, "OUTSB" },//0x6e,
    { R_M, C_OUTS, D__DX, D__Xw, D__NONE, "OUTSW" },//0x6f,
    { R_M, C_OUTS, D__DX, D__Xd, D__NONE, "OUTSD" },//0x6f,

    { R_M, C_PUSHF, D__NONE, D__NONE, D__NONE, "PUSHF" },//0x9c,
    { R_M, C_PUSHF, D__NONE, D__NONE, D__NONE, "PUSHFD" },//0x9c,
    { R_M, C_POPF, D__NONE, D__NONE, D__NONE, "POPF" },//0x9d,
    { R_M, C_POPF, D__NONE, D__NONE, D__NONE, "POPFD" },//0x9d,
    { R_M, C_PUSHA, D__NONE, D__NONE, D__NONE, "PUSHA" },//0x60,
    { R_M, C_PUSHA, D__NONE, D__NONE, D__NONE, "PUSHAD" },//0x60,
    { R_M, C_POPA, D__NONE, D__NONE, D__NONE, "POPA" },//0x61,
    { R_M, C_POPA, D__NONE, D__NONE, D__NONE, "POPAD" },//0x61,

    { R_M, C_MOVS, D__Yb, D__NONE, D__NONE, "MOVSB" },//0xa4,
    { R_M, C_MOVS, D__Yw, D__NONE, D__NONE, "MOVSW" },//0xa5,
    { R_M, C_MOVS, D__Yd, D__NONE, D__NONE, "MOVSD" },//0xa5,
    { R_M, C_CMPS, D__Yb, D__NONE, D__NONE, "CMPSB" },//0xa6,
    { R_M, C_CMPS, D__Yw, D__NONE, D__NONE, "CMPSW" },//0xa7,
    { R_M, C_CMPS, D__Yd, D__NONE, D__NONE, "CMPSD" },//0xa7,
    { R_M, C_STOS, D__Yb, D__NONE, D__NONE, "STOSB" },//0xaa,
    { R_M, C_STOS, D__Yw, D__NONE, D__NONE, "STOSW" },//0xab,
    { R_M, C_STOS, D__Yd, D__NONE, D__NONE, "STOSD" },//0xab,
    { R_M, C_LODS, D__AL, D__Xb, D__NONE, "LODSB" },//0xac,
    { R_M, C_LODS, D__AX, D__Xw, D__NONE, "LODSW" },//0xad,
    { R_M, C_LODS, D__EAX, D__Xd, D__NONE, "LODSD" },//0xad,
    { R_M, C_SCAS, D__Yb, D__NONE, D__NONE, "SCASB" },//0xae,
    { R_M, C_SCAS, D__Yw, D__NONE, D__NONE, "SCASW" },//0xaf,
    { R_M, C_SCAS, D__Yd, D__NONE, D__NONE, "SCASD" },//0xaf,

    { R_M, C_XLAT, D__NONE, D__NONE, D__NONE, "XLAT" },//0xd7,
    { R_M, C_XLAT, D__NONE, D__NONE, D__NONE, "XLATB" },//0xd7,

    { R_M, C_REPN, D__NONE, D__NONE, D__NONE, "REPNZ" },//0xf2
    { R_M, C_REPN, D__NONE, D__NONE, D__NONE, "REPNE" },//0xf2
    { R_M, C_REPZ, D__NONE, D__NONE, D__NONE, "REP" },//0xf3
    { R_M, C_REPZ, D__NONE, D__NONE, D__NONE, "REPZ" },//0xf3
    { R_M, C_REPZ, D__NONE, D__NONE, D__NONE, "REPE" },//0xf3

    { R_M, C_JtO, D__Jb, D__NONE, D__NONE, "JO" },//0x70,
    { R_M, C_JfO, D__Jb, D__NONE, D__NONE, "JNO" },//0x71,
    { R_M, C_JtC, D__Jb, D__NONE, D__NONE, "JB" },//0x72,
    { R_M, C_JtC, D__Jb, D__NONE, D__NONE, "JC" },//0x72,
    { R_M, C_JtC, D__Jb, D__NONE, D__NONE, "JNAE" },//0x72,
    { R_M, C_JfC, D__Jb, D__NONE, D__NONE, "JAE" },//0x73,
    { R_M, C_JfC, D__Jb, D__NONE, D__NONE, "JNB" },//0x73,
    { R_M, C_JfC, D__Jb, D__NONE, D__NONE, "JNC" },//0x73,
    { R_M, C_JtZ, D__Jb, D__NONE, D__NONE, "JE" },//0x74,
    { R_M, C_JtZ, D__Jb, D__NONE, D__NONE, "JZ" },//0x74,
    { R_M, C_JfZ, D__Jb, D__NONE, D__NONE, "JNE" },//0x75,
    { R_M, C_JfZ, D__Jb, D__NONE, D__NONE, "JNZ" },//0x75,
    { R_M, C_JfA, D__Jb, D__NONE, D__NONE, "JBE" },//0x76,
    { R_M, C_JfA, D__Jb, D__NONE, D__NONE, "JNA" },//0x76,
    { R_M, C_JtA, D__Jb, D__NONE, D__NONE, "JA" },//0x77,
    { R_M, C_JtA, D__Jb, D__NONE, D__NONE, "JNBE" },//0x77,
    { R_M, C_JtS, D__Jb, D__NONE, D__NONE, "JS" },//0x78,
    { R_M, C_JfS, D__Jb, D__NONE, D__NONE, "JNS" },//0x79,
    { R_M, C_JtP, D__Jb, D__NONE, D__NONE, "JP" },//0x7a,
    { R_M, C_JtP, D__Jb, D__NONE, D__NONE, "JPE" },//0x7a,
    { R_M, C_JfP, D__Jb, D__NONE, D__NONE, "JNP" },//0x7b,
    { R_M, C_JfP, D__Jb, D__NONE, D__NONE, "JPO" },//0x7b,
    { R_M, C_JtL, D__Jb, D__NONE, D__NONE, "JL" },//0x7c,
    { R_M, C_JtL, D__Jb, D__NONE, D__NONE, "JNGE" },//0x7c,
    { R_M, C_JfL, D__Jb, D__NONE, D__NONE, "JGE" },//0x7d,
    { R_M, C_JfL, D__Jb, D__NONE, D__NONE, "JNL" },//0x7d,
    { R_M, C_JfG, D__Jb, D__NONE, D__NONE, "JLE" },//0x7e,
    { R_M, C_JfG, D__Jb, D__NONE, D__NONE, "JNG" },//0x7e,
    { R_M, C_JtG, D__Jb, D__NONE, D__NONE, "JG" },//0x7f,
    { R_M, C_JtG, D__Jb, D__NONE, D__NONE, "JNLE" },//0x7f,

    { R_M, C_INS, D__Xb, D__DX, D__NONE, "INSB" },//0x6c,
    { R_M, C_INS, D__Xw, D__DX, D__NONE, "INSW" },//0x6d,
    { R_M, C_INS, D__Xd, D__DX, D__NONE, "INSD" },//0x6d,
    { R_M, C_OUTS, D__DX, D__Xb, D__NONE, "OUTSB" },//0x6e,
    { R_M, C_OUTS, D__DX, D__Xw, D__NONE, "OUTSW" },//0x6f,
    { R_M, C_OUTS, D__DX, D__Xd, D__NONE, "OUTSD" },//0x6f,

    { R_M, C_MOVS, D__Xb, D__NONE, D__NONE, "MOVSB" },//0xa4,
    { R_M, C_MOVS, D__Xw, D__NONE, D__NONE, "MOVSW" },//0xa5,
    { R_M, C_MOVS, D__Xd, D__NONE, D__NONE, "MOVSD" },//0xa5,
    { R_M, C_CMPS, D__Xb, D__NONE, D__NONE, "CMPSB" },//0xa6,
    { R_M, C_CMPS, D__Xw, D__NONE, D__NONE, "CMPSW" },//0xa7,
    { R_M, C_CMPS, D__Xd, D__NONE, D__NONE, "CMPSD" },//0xa7,
    { R_M, C_STOS, D__Xb, D__NONE, D__NONE, "STOSB" },//0xaa,
    { R_M, C_STOS, D__Xw, D__NONE, D__NONE, "STOSW" },//0xab,
    { R_M, C_STOS, D__Xd, D__NONE, D__NONE, "STOSD" },//0xab,
    { R_M, C_LODS, D__Xb, D__NONE, D__NONE, "LODSB" },//0xac,
    { R_M, C_LODS, D__Xw, D__NONE, D__NONE, "LODSW" },//0xad,
    { R_M, C_LODS, D__Xd, D__NONE, D__NONE, "LODSD" },//0xad,
    { R_M, C_SCAS, D__Xb, D__NONE, D__NONE, "SCASB" },//0xae,
    { R_M, C_SCAS, D__Xw, D__NONE, D__NONE, "SCASW" },//0xaf,
    { R_M, C_SCAS, D__Xd, D__NONE, D__NONE, "SCASD" },//0xaf,

    { R_M, C_XLAT, D__NONE, D__NONE, D__NONE, "XLAT" },//0xd7,
    { R_M, C_XLAT, D__NONE, D__NONE, D__NONE, "XLATB" },//0xd7,

    { R_M, C_REPN, D__NONE, D__NONE, D__NONE, "REPNZ" },//0xf2
    { R_M, C_REPN, D__NONE, D__NONE, D__NONE, "REPNE" },//0xf2
    { R_M, C_REPZ, D__NONE, D__NONE, D__NONE, "REP" },//0xf3
    { R_M, C_REPZ, D__NONE, D__NONE, D__NONE, "REPZ" },//0xf3
    { R_M, C_REPZ, D__NONE, D__NONE, D__NONE, "REPE" },//0xf3

    { R_M, C_JtO, D__Jb, D__NONE, D__NONE, "JO" },//0x70,
    { R_M, C_JfO, D__Jb, D__NONE, D__NONE, "JNO" },//0x71,
    { R_M, C_JtC, D__Jb, D__NONE, D__NONE, "JB" },//0x72,
    { R_M, C_JtC, D__Jb, D__NONE, D__NONE, "JC" },//0x72,
    { R_M, C_JtC, D__Jb, D__NONE, D__NONE, "JNAE" },//0x72,
    { R_M, C_JfC, D__Jb, D__NONE, D__NONE, "JAE" },//0x73,
    { R_M, C_JfC, D__Jb, D__NONE, D__NONE, "JNB" },//0x73,
    { R_M, C_JfC, D__Jb, D__NONE, D__NONE, "JNC" },//0x73,
    { R_M, C_JtZ, D__Jb, D__NONE, D__NONE, "JE" },//0x74,
    { R_M, C_JtZ, D__Jb, D__NONE, D__NONE, "JZ" },//0x74,
    { R_M, C_JfZ, D__Jb, D__NONE, D__NONE, "JNE" },//0x75,
    { R_M, C_JfZ, D__Jb, D__NONE, D__NONE, "JNZ" },//0x75,
    { R_M, C_JfA, D__Jb, D__NONE, D__NONE, "JBE" },//0x76,
    { R_M, C_JfA, D__Jb, D__NONE, D__NONE, "JNA" },//0x76,
    { R_M, C_JtA, D__Jb, D__NONE, D__NONE, "JA" },//0x77,
    { R_M, C_JtA, D__Jb, D__NONE, D__NONE, "JNBE" },//0x77,
    { R_M, C_JtS, D__Jb, D__NONE, D__NONE, "JS" },//0x78,
    { R_M, C_JfS, D__Jb, D__NONE, D__NONE, "JNS" },//0x79,
    { R_M, C_JtP, D__Jb, D__NONE, D__NONE, "JP" },//0x7a,
    { R_M, C_JtP, D__Jb, D__NONE, D__NONE, "JPE" },//0x7a,
    { R_M, C_JfP, D__Jb, D__NONE, D__NONE, "JNP" },//0x7b,
    { R_M, C_JfP, D__Jb, D__NONE, D__NONE, "JPO" },//0x7b,
    { R_M, C_JtL, D__Jb, D__NONE, D__NONE, "JL" },//0x7c,
    { R_M, C_JtL, D__Jb, D__NONE, D__NONE, "JNGE" },//0x7c,
    { R_M, C_JfL, D__Jb, D__NONE, D__NONE, "JGE" },//0x7d,
    { R_M, C_JfL, D__Jb, D__NONE, D__NONE, "JNL" },//0x7d,
    { R_M, C_JfG, D__Jb, D__NONE, D__NONE, "JLE" },//0x7e,
    { R_M, C_JfG, D__Jb, D__NONE, D__NONE, "JNG" },//0x7e,
    { R_M, C_JtG, D__Jb, D__NONE, D__NONE, "JG" },//0x7f,
    { R_M, C_JtG, D__Jb, D__NONE, D__NONE, "JNLE" },//0x7f,

    { R_M, C_DBYTE, D_DBYTE, D__NONE, D__NONE, "DB" }, //db xx
    { R_M, C_DWORD, D_DWORD, D__NONE, D__NONE, "DW" }, //dw xx
    { R_M, C_DDWORD, D_DDWORD, D__NONE, D__NONE, "DD" }, //dd xx
    { R_M, C_DBYTE, D_STRING, D__NONE, D__NONE, "DB" }, //db xx

    { R_M, C_TAIL } //Tail
};
//////////////////////////////////////////////////////////////////////////

/************************************************************************
函数名字：BOOL OnDisassembly(INSTRUCTION_INFORMATION *pInstInfo, int &pCodeLen,
                             BYTE *pInbuff, DWORD dwEip, int nMode, DIS_CPU *DisCPU)
功    能：处理指令解析
参  数 1：pInstInfo    指令信息 - OUT
参  数 2：pCodeLen     指令长度 - OUT
参  数 3：pInbuff      需要解析的指令信息 - IN
参  数 4：dwEip        当前指令的虚拟地址
参  数 5：nMode        当前反汇编的模式
参  数 6：DisCPU       指令解析信息
返 回 值：解析成功返回TRUE/解析失败返回FALSE
************************************************************************/
BOOL OnDisassembly(INSTRUCTION_INFORMATION *pInstInfo, int &pCodeLen, 
                   BYTE *pInbuff, DWORD dwEip, int nMode, PDIS_CPU pTagDisCPU)
{
    // DIS_OPCODE_T 指令的类型和寻址方式
    PDIS_OPCODE_T pIntelOpcode = NULL;
    //DISP_INSTRUCTION_T  mod reg Sib IMm 信息
    DISP_INSTRUCTION_T disp_i = { 0 };
    DIS_CPU tagDisCPU = { 0 };
    unsigned Segment = -1;
    unsigned OpcodeType;

    if (pTagDisCPU == NULL)
    {
        pTagDisCPU = &tagDisCPU;
    }
    disp_i.os_32 = disp_i.as_32 = nMode; //BIT16,BIT32
    disp_i.lock = disp_i.rep_used = -1;

GET_OPCODE:
    if (pCodeLen >= 0x0f)
    {
        pCodeLen = 0;
        return FALSE;
    }

    disp_i.b1 = pInbuff[pCodeLen++];
    pIntelOpcode = g_szAllOpCode + disp_i.b1;

GET_OPATTR:
    //判断指令前缀以及类型
    OpcodeType = pIntelOpcode->OpcodeType;
    switch (OpcodeType)
    {
    case C_ES:case C_CS:case C_SS:
    case C_DS:case C_FS:case C_GS:
        Segment = OpcodeType - C_ES;
        goto GET_OPCODE;
    case C_OPER:
        if (nMode == MODE_32BIT)
        {
            disp_i.os_32 = MODE_16BIT;
        }
        else
        {
            disp_i.os_32 = MODE_32BIT;
        }
        goto GET_OPCODE;

    case C_ADDR:
        if (nMode == MODE_32BIT)
        {
            disp_i.as_32 = MODE_16BIT;
        }
        else
        {
            disp_i.as_32 = MODE_32BIT;
        }
        goto GET_OPCODE;
    case C_LOCK:
        disp_i.lock = 0xf0;
        goto GET_OPCODE;
    case C_REPN:
        disp_i.rep_used = 0xf2;
        goto GET_OPCODE;
    case C_REPZ:
        disp_i.rep_used = 0xf3;
        goto GET_OPCODE;

    case C_GRP:
        pIntelOpcode = (DIS_OPCODE_T*)pIntelOpcode->name 
            + ((pInbuff[pCodeLen] >> 3) & 0x07);
        break;
    case C_ESC:
        if (pInbuff[pCodeLen] < 0xc0)
        {
            pIntelOpcode = (DIS_OPCODE_T*)pIntelOpcode->name 
                + ((pInbuff[pCodeLen] >> 3) & 0x07);
        }
        else
        {
            pIntelOpcode = (DIS_OPCODE_T*)pIntelOpcode->name + 0x08 
                + pInbuff[pCodeLen++] - 0xc0;
        }
        break;

    case C_2BYTE:
        disp_i.b1 = pInbuff[pCodeLen++];
        pIntelOpcode = (DIS_OPCODE_T*)pIntelOpcode->name + disp_i.b1;
        goto GET_OPATTR;

    case C_GRP1:
        if (pInbuff[pCodeLen] < 0xc0)
        {
            pIntelOpcode = (DIS_OPCODE_T*)pIntelOpcode->name 
                + ((pInbuff[pCodeLen] >> 3) & 0x07);
        }
        else
        {
            pIntelOpcode = (DIS_OPCODE_T*)pIntelOpcode->name + 0x8 
                + ((pInbuff[pCodeLen] >> 3) & 0x07);
        }
        break;
    }

    if (pIntelOpcode->name == NULL)
    {
        pCodeLen = 0;
        return FALSE;
    }

    disp_i.attr = pIntelOpcode->Attr;
    disp_i.destination = pIntelOpcode->Opdata1;
    disp_i.source = pIntelOpcode->Opdata2;
    disp_i.other = pIntelOpcode->Opdata3;

    if ((disp_i.attr & R_M) == R_M)
    {
        if (disp_i.as_32 == MODE_32BIT)
        {
            pCodeLen += OnGetAddress32Dis(pTagDisCPU, pInbuff + pCodeLen, &disp_i);
        }
        else
        {
            pCodeLen += OnGetAddress16Dis(pTagDisCPU, pInbuff + pCodeLen, &disp_i);
        }
    }

    pCodeLen += OnProcessOpdata_immde(disp_i.destination, pInbuff + pCodeLen, &disp_i);
    pCodeLen += OnProcessOpdata_immde(disp_i.source, pInbuff + pCodeLen, &disp_i);
    pCodeLen += OnProcessOpdata_immde(disp_i.other, pInbuff + pCodeLen, &disp_i);

    if (pCodeLen >= 0x0f)
    {
        pCodeLen = 0;
        return FALSE;
    }

    disp_i.ilen = pCodeLen;

    //--------------------------------------------------------------------

    pInstInfo->OpCode = pIntelOpcode->OpcodeType;
    pInstInfo->Length = disp_i.ilen;
    pInstInfo->Repeat = disp_i.rep_used;
    pInstInfo->Lock = disp_i.lock;

    if (!OnProcessName_dis(pInstInfo, pIntelOpcode, disp_i.os_32, disp_i.as_32))
    {
        pCodeLen = 0;
        return FALSE;
    }

    int temp;

    temp = OnProcessOpdata_dis(pIntelOpcode->Opdata1, &pInstInfo->op[0], dwEip, &disp_i);
    if (temp == -1)
    {
        pCodeLen = 0;
        return FALSE;
    }

    temp = OnProcessOpdata_dis(pIntelOpcode->Opdata2, &pInstInfo->op[1], dwEip, &disp_i);
    if (temp == -1)
    {
        pCodeLen = 0;
        return FALSE;
    }

    temp = OnProcessOpdata_dis(pIntelOpcode->Opdata3, &pInstInfo->op[2], dwEip, &disp_i);
    if (temp == -1)
    {
        pCodeLen = 0;
        return FALSE;
    }

    if (disp_i.seg != Segment&&Segment != -1)
    {
        pInstInfo->SegmentPrefix = Segment;
        for (int i = 0; i < 3; i++)
        {
            if (pInstInfo->op[i].mode == mode_address)
            {
                pInstInfo->op[i].addr.line_address =
                    pInstInfo->op[i].addr.effect_address + pTagDisCPU->SEG_BASE[Segment];
            }
        }
    }
    else
    {
        pInstInfo->SegmentPrefix = -1;
        for (int i = 0; i < 3; i++)
        {
            if (pInstInfo->op[i].mode == mode_address)
            {
                pInstInfo->op[i].addr.line_address =
                    pInstInfo->op[i].addr.effect_address + pTagDisCPU->SEG_BASE[disp_i.seg];
            }
        }
    }


    if (pInstInfo->op[0].mode == mode_address&&pInstInfo->op[0].opersize != 0 &&
        (pInstInfo->op[1].mode == mode_immed || pInstInfo->op[1].mode == mode_invalid)
        )
    {
        pInstInfo->PtrType = pInstInfo->op[0].opersize;
    }
    else
    {
        pInstInfo->PtrType = -1;
    }

    if (pInstInfo->op[1].mode == mode_address&&pInstInfo->op[1].opersize != 0 && pInstInfo->op[1].opersize < pInstInfo->op[0].opersize)
    {
        pInstInfo->PtrType = pInstInfo->op[1].opersize;
    }
    else
    {
        pInstInfo->PtrType = -1;
    }

    if (pInstInfo->pAsmString != NULL)
    {
        OnPrintPasm(pInstInfo, pInstInfo->pAsmString);
    }

    return TRUE;
}

/************************************************************************
函数名字：int OnGetAddress32Dis(DIS_CPU * pDisCPU, BYTE *pbuff, 
                                DISP_INSTRUCTION_T *disp_i)
功    能：获取操作数偏移
参  数 1：pDisCPU  指令解析信息
参  数 2：pbuff    操作数的数据
参  数 3：disp_i   操作码的信息
返 回 值：返回操作数偏移
************************************************************************/
int OnGetAddress32Dis(DIS_CPU * pDisCPU, BYTE *pbuff, DISP_INSTRUCTION_T *disp_i)
{
    int r_m, reg, mod, base, index, scale;
    int length;
    DWORD base_index;

    r_m = ((DIS_MODR_M*)pbuff)->r_m;
    reg = ((DIS_MODR_M*)pbuff)->reg;
    mod = ((DIS_MODR_M*)pbuff)->mod;

    disp_i->rm = r_m;
    disp_i->nnn = reg;
    disp_i->mod = mod;

    disp_i->as_32 = MODE_32BIT;

    if (r_m == 4)
    {
        base = ((DIS_MODR_M*)pbuff)->base;
        index = ((DIS_MODR_M*)pbuff)->index;
        scale = ((DIS_MODR_M*)pbuff)->scale;

        pbuff += 2;

        disp_i->base = base;
        disp_i->index = index;
        disp_i->scale = scale;

        if (index == 4)
        {
            base_index = pDisCPU->gen_reg[base].erx;
            disp_i->index = -1;
        }
        else
        {
            base_index = pDisCPU->gen_reg[base].erx + (pDisCPU->gen_reg[index].erx << scale);
        }

        if ((base & 0xfe) == 4)
        {
            disp_i->seg = I_SS;
        }
        else
        {
            disp_i->seg = I_DS;
        }

        switch (mod)
        {
        case 0:

            if (base == 5)
            {
                disp_i->addr_displacement = GetValue_d(pbuff);
                disp_i->displacement_size = sizeof(DWORD);
                length = 0x06;

                if (index == 4)
                {
                    base_index = 0;
                }
                else
                {
                    base_index = (pDisCPU->gen_reg[index].erx << scale);
                }

                disp_i->seg = I_DS;
            }
            else
            {
                disp_i->addr_displacement = 0;
                disp_i->displacement_size = 0;
                length = 0x02;
            }

            break;

        case 1:

            disp_i->addr_displacement = (long)(*(char*)pbuff);
            disp_i->displacement_size = sizeof(BYTE);
            length = 0x03;
            break;

        case 2:

            disp_i->addr_displacement = GetValue_d(pbuff);
            disp_i->displacement_size = sizeof(DWORD);
            length = 0x06;
            break;

        default: //mod == 3
            disp_i->displacement_size = 0;
            length = 0x01;
        }
    }
    else
    {
        pbuff++;

        disp_i->base = r_m;
        disp_i->index = -1;
        disp_i->scale = 0;

        base_index = pDisCPU->gen_reg[r_m].erx;

        if (r_m == 5) disp_i->seg = I_SS;
        else disp_i->seg = I_DS;

        switch (mod)
        {
        case 0:

            if (r_m == 5)
            {
                disp_i->addr_displacement = GetValue_d(pbuff);
                disp_i->displacement_size = sizeof(DWORD);


                base_index = 0;
                disp_i->seg = I_DS;
                disp_i->base = -1;
            }
            else
            {
                disp_i->addr_displacement = 0;
                disp_i->displacement_size = 0;
            }

            break;

        case 1:

            disp_i->addr_displacement = (long)(*(char*)pbuff);
            disp_i->displacement_size = sizeof(BYTE);
            break;

        case 2:

            disp_i->addr_displacement = GetValue_d(pbuff);
            disp_i->displacement_size = sizeof(DWORD);

            break;

        default: //mod == 3

            disp_i->displacement_size = 0;
        }

        length = disp_i->displacement_size + 1;
    }

    disp_i->rm_addr = base_index + disp_i->addr_displacement;
    return length;
}

/************************************************************************
函数名字：int OnGetAddress16Dis(DIS_CPU * pDisCPU, BYTE *pbuff,
DISP_INSTRUCTION_T *disp_i)
功    能：获取操作数偏移
参  数 1：pDisCPU  指令解析信息
参  数 2：pbuff    操作数的数据
参  数 3：disp_i   操作码的信息
返 回 值：返回操作数偏移
************************************************************************/
int OnGetAddress16Dis(DIS_CPU * pDisCPU, BYTE *pbuff, DISP_INSTRUCTION_T *disp_i)
{
    int r_m, reg, mod;
    int length;
    DWORD base_index;

    r_m = ((DIS_MODR_M*)pbuff)->r_m;
    reg = ((DIS_MODR_M*)pbuff)->reg;
    mod = ((DIS_MODR_M*)pbuff)->mod;

    disp_i->rm = r_m;
    disp_i->nnn = reg;
    disp_i->mod = mod;

    pbuff++;

    disp_i->as_32 = MODE_16BIT;
    disp_i->scale = 0;

    switch (r_m)
    {
    case 0:

        base_index = pDisCPU->gen_reg[I_EBX].word.rx + pDisCPU->gen_reg[I_ESI].word.rx;
        disp_i->seg = I_DS;
        disp_i->base = INTEL_GEG_REG_EBX;
        disp_i->index = INTEL_GEG_REG_ESI;
        break;

    case 1:

        base_index = pDisCPU->gen_reg[I_EBX].word.rx + pDisCPU->gen_reg[I_EDI].word.rx;
        disp_i->seg = I_DS;
        disp_i->base = INTEL_GEG_REG_EBX;
        disp_i->index = INTEL_GEG_REG_EDI;
        break;

    case 2:

        base_index = pDisCPU->gen_reg[I_EBP].word.rx + pDisCPU->gen_reg[I_ESI].word.rx;
        disp_i->seg = I_SS;
        disp_i->base = INTEL_GEG_REG_EBP;
        disp_i->index = INTEL_GEG_REG_ESI;
        break;

    case 3:

        base_index = pDisCPU->gen_reg[I_EBP].word.rx + pDisCPU->gen_reg[I_EDI].word.rx;
        disp_i->seg = I_SS;
        disp_i->base = INTEL_GEG_REG_EBP;
        disp_i->index = INTEL_GEG_REG_EDI;
        break;

    case 4:

        base_index = pDisCPU->gen_reg[I_ESI].word.rx;
        disp_i->seg = I_DS;
        disp_i->base = INTEL_GEG_REG_ESI;
        disp_i->index = -1;
        break;

    case 5:

        base_index = pDisCPU->gen_reg[I_EDI].word.rx;
        disp_i->seg = I_DS;
        disp_i->base = INTEL_GEG_REG_EDI;
        disp_i->index = -1;
        break;

    case 6:

        base_index = pDisCPU->gen_reg[I_EBP].word.rx;
        disp_i->seg = I_SS;
        disp_i->base = INTEL_GEG_REG_EBP;
        disp_i->index = -1;
        break;

    case 7:

        base_index = pDisCPU->gen_reg[I_EBX].word.rx;
        disp_i->seg = I_DS;
        disp_i->base = INTEL_GEG_REG_EBX;
        disp_i->index = -1;
        break;
    };

    switch (mod)
    {
    case 0:

        if (r_m == 6)
        {
            disp_i->addr_displacement = GetValue_w(pbuff);
            disp_i->displacement_size = sizeof(WORD);


            base_index = 0;
            disp_i->seg = I_DS;
            disp_i->base = -1;
            disp_i->index = -1;
        }
        else
        {
            disp_i->addr_displacement = 0x00;
            disp_i->displacement_size = 0;
        }
        break;

    case 1:

        disp_i->addr_displacement = (long)(*(char*)pbuff);
        disp_i->displacement_size = sizeof(BYTE);
        break;

    case 2:

        disp_i->addr_displacement = GetValue_w(pbuff);
        disp_i->displacement_size = sizeof(WORD);
        break;

    default: //mod == 3

        disp_i->displacement_size = 0;
    }

    disp_i->rm_addr = (unsigned short)(base_index + disp_i->addr_displacement);
    //此有效地址为十六位

    length = disp_i->displacement_size + 1;

    return length;
}

/************************************************************************
函数名字：int OnProcessOpdata_immde(int nOpdata, BYTE *pInbuff, 
                                    DISP_INSTRUCTION_T *disp_i)
功    能：获取指令中立即数的数据以及长度
参  数 1：nOpdata 操作码信息中的数据
参  数 2：pInbuff 操作数的数据
参  数 3：disp_i  操作码的信息
返 回 值：指令立即数部分的长度
************************************************************************/
int OnProcessOpdata_immde(int nOpdata, BYTE *pInbuff, DISP_INSTRUCTION_T *disp_i)
{
    int nOpImmedLen = 0, SIZE_c = 0, SIZE_v = 0, SIZE_p = 0;

    if (disp_i->os_32 == MODE_32BIT)
    {
        SIZE_v = 4;
        SIZE_c = 2;
        SIZE_p = 6;
    }
    else
    {
        SIZE_v = 2;
        SIZE_c = 1;
        SIZE_p = 4;
    }

    switch (nOpdata)
    {
    case D__NONE:
        break;

    case D__1:
        disp_i->Ib = 1;
        break;

        //操作码后面的立即数
    case D__Ib:
    case D_zIb:
    case D_sIb:
        nOpImmedLen = SIZE_b;
        disp_i->Ib = GetValue_b(pInbuff);
        break;

    case D__Iw:
        nOpImmedLen = SIZE_w;
        disp_i->Iw = GetValue_w(pInbuff);
        break;
    case D__Iv:
        nOpImmedLen = SIZE_v;

        if (SIZE_v == SIZE_w)
        {
            disp_i->Iw = GetValue_w(pInbuff);
        }
        else
        {
            disp_i->Id = GetValue_d(pInbuff);
        }
        break;

        //处理跳转立即数
    case D__Jb:
        nOpImmedLen = SIZE_b;
        disp_i->Ib = GetValue_b(pInbuff);
        break;

    case D__Jv:
        nOpImmedLen = SIZE_v;

        if (SIZE_v == SIZE_w)
        {
            disp_i->Iw = GetValue_w(pInbuff);
        }
        else
        {
            disp_i->Id = GetValue_d(pInbuff);
        }

        break;

        // 存放没有mod/rm的指令，word类型或索引
    case D__Ob:
    case D_rOb:
    case D_wOb:
    case D__Ov:
    case D_rOv:
    case D_wOv:
        if (disp_i->as_32 == MODE_32BIT)
        {
            nOpImmedLen = 4;
            disp_i->Id = GetValue_d(pInbuff);
        }
        else
        {
            nOpImmedLen = 2;
            disp_i->Iw = GetValue_w(pInbuff);
        }
        break;

    case D__Ap:
        nOpImmedLen = SIZE_v + 2;

        if (SIZE_v == SIZE_w)
        {
            disp_i->Iw = GetValue_w(pInbuff);
            disp_i->Iw2 = GetValue_w(pInbuff + 2);
        }
        else
        {
            disp_i->Id = GetValue_d(pInbuff);
            disp_i->Iw2 = GetValue_w(pInbuff + 4);
        }

        break;

    default:
        nOpImmedLen = 0;
    }

    return nOpImmedLen;
}

/************************************************************************
函数名字：bool OnProcessName_dis(INSTRUCTION_INFORMATION* pInstInfo, 
                                 DIS_OPCODE_T *pIntelOpcode, int OperandSize, 
                                 int AddressSize)
功    能：格式化指令信息
参  数 1：pInstInfo 指令结构
参  数 2：pIntelOpcode OPCODE各部分类型
参  数 3：OperandSize  DISP_INSTRUCTION_T.os32
参  数 4：AddressSize  DISP_INSTRUCTION_T.as32
返 回 值：完成遍历返回true/条件不足返回false
************************************************************************/
bool OnProcessName_dis(INSTRUCTION_INFORMATION* pInstInfo, DIS_OPCODE_T *pIntelOpcode,
                     int OperandSize, int AddressSize)
{
    int opcodetype = 0, Opdata1 = 0, Opdata2 = 0;

    opcodetype = pIntelOpcode->OpcodeType;
    Opdata1 = pIntelOpcode->Opdata1;
    Opdata2 = pIntelOpcode->Opdata2;

    if (pInstInfo->Lock == 0xf0)
    {
        strcpy_s(pInstInfo->LockName, sizeof(pInstInfo->LockName), "LOCK");
    }
    else
    {
        pInstInfo->LockName[0] = '\0';
    }

    if (pInstInfo->Repeat == 0xf2)
    {
        strcpy_s(pInstInfo->RepeatName, sizeof(pInstInfo->RepeatName), "REPNZ");
    }
    else if (pInstInfo->Repeat == 0xf3)
    {
        strcpy_s(pInstInfo->RepeatName, sizeof(pInstInfo->RepeatName), "REPZ");
    }
    else
    {
        pInstInfo->RepeatName[0] = '\0';
    }

    if (opcodetype == C_NULL)
    {
        return false;
    }
    else
    {
        strcpy_s(pInstInfo->Name, sizeof(pInstInfo->Name), pIntelOpcode->name);
    }

    if (Opdata1 >= D__Xb&&Opdata1 <= D_wXb || Opdata1 >= D__Yb&&Opdata1 <= D_wYb ||
        Opdata2 >= D__Xb&&Opdata2 <= D_wXb || Opdata2 >= D__Yb&&Opdata2 <= D_wYb)
    {
        sprintf_s(pInstInfo->Name, "%sB", pIntelOpcode->name);
    }

    if (OperandSize == MODE_32BIT)
    {
        if (Opdata1 >= D__Xv&&Opdata1 <= D_wXv || Opdata1 >= D__Yv&&Opdata1 <= D_wYv ||
            Opdata2 >= D__Xv&&Opdata2 <= D_wXv || Opdata2 >= D__Yv&&Opdata2 <= D_wYv)
        {
            sprintf_s(pInstInfo->Name, "%sD", pIntelOpcode->name);
        }

        switch (opcodetype)
        {
        case C_CBW:
            strcpy_s(pInstInfo->Name, "CWDE");
            break;

        case C_CWD:
            strcpy_s(pInstInfo->Name, "CDQ");
            break;

        case C_PUSHA:
            strcpy_s(pInstInfo->Name, "PUSHAD");
            break;

        case C_POPA:
            strcpy_s(pInstInfo->Name, "POPAD");
            break;

        case C_PUSHF:
            strcpy_s(pInstInfo->Name, "PUSHFD");
            break;

        case C_POPF:
            strcpy_s(pInstInfo->Name, "POPFD");
            break;

        case C_JCXZ:
            strcpy_s(pInstInfo->Name, "JECXZ");
            break;

        case C_IRET:
            strcpy_s(pInstInfo->Name, "IRETD");
        };
    }
    else
    {
        if (Opdata1 >= D__Xv&&Opdata1 <= D_wXv || Opdata1 >= D__Yv&&Opdata1 <= D_wYv ||
            Opdata2 >= D__Xv&&Opdata2 <= D_wXv || Opdata2 >= D__Yv&&Opdata2 <= D_wYv)
        {
            sprintf_s(pInstInfo->Name, "%sW", pIntelOpcode->name);
        }
    }

    return true;
}

/************************************************************************
函数名字：int OnProcessOpdata_dis(int nOpdata, POPERAND_ITEM op, 
                                  DWORD eip, DISP_INSTRUCTION_T *disp_i)
功    能：格式化指令信息
参  数 1：nOpdata 操作码信息中的数据
参  数 2：op      操作数描叙结构
参  数 3：eip     虚拟地址
参  数 4：disp_i  操作码的信息
返 回 值：操作数长度
************************************************************************/
int OnProcessOpdata_dis(int nOpdata, POPERAND_ITEM op, DWORD eip, DISP_INSTRUCTION_T *disp_i)
{
    int OperandSize = disp_i->os_32;
    int AddressSize = disp_i->as_32;
    int k = 0, nOpImmedLen = 0, SIZE_c = 0, SIZE_v = 0, SIZE_p = 0;

    if (OperandSize == MODE_32BIT)
    {
        SIZE_v = 4;
        SIZE_c = 2;
        SIZE_p = 6;
    }
    else
    {
        SIZE_v = 2;
        SIZE_c = 1;
        SIZE_p = 4;
    }

    nOpImmedLen = 0;
    op->mode = mode_invalid;
    op->opersize = 0;

    switch (nOpdata)
    {
    case D__NONE:
        break;

    case D__1:
        op->mode = mode_immed;
        op->opersize = 1;
        op->immed.immed_value = 1;
        strcpy_s(op->string, "1");
        break;

        // Immediate data. The operand value is encoded in subsequent bytes of the instruction.
    case D__Ib:
        op->mode = mode_immed;
        op->opersize = SIZE_b;
        nOpImmedLen = SIZE_b;
        op->immed.immed_value = disp_i->Ib;
        sprintf_s(op->string, "%02X", op->immed.immed_value);
        break;

    case D_zIb:
        op->mode = mode_immed;
        op->opersize = SIZE_b;
        nOpImmedLen = SIZE_b;
        op->immed.immed_value = disp_i->Ib;
        op->immed.immed_value;
        sprintf_s(op->string, "%02X", (BYTE)op->immed.immed_value);
        break;

    case D_sIb:
        op->mode = mode_immed;
        op->opersize = SIZE_b;
        nOpImmedLen = SIZE_b;
        op->immed.immed_value = disp_i->Ib;

        k = op->immed.immed_value;
        if (k >= sign_BYTE)
        {
            sprintf_s(op->string, "%02X", (BYTE)(0 - k));
        }
        else
        {
            sprintf_s(op->string, "%02X", (BYTE)k);
        }
        break;

    case D__Iw:
        op->mode = mode_immed;
        op->opersize = SIZE_w;
        nOpImmedLen = SIZE_w;
        op->immed.immed_value = disp_i->Iw;
        sprintf_s(op->string, "%04X", op->immed.immed_value);
        break;

    case D__Iv:
        op->mode = mode_immed;
        op->opersize = SIZE_v;
        nOpImmedLen = SIZE_v;
        if (SIZE_v == SIZE_w)
        {
            op->immed.immed_value = disp_i->Iw;
            sprintf_s(op->string, "%04X", op->immed.immed_value);
        }
        else
        {
            op->immed.immed_value = disp_i->Id;
            sprintf_s(op->string, "%08X", op->immed.immed_value);
        }
        break;

        
    case D__Jb:
        op->mode = mode_near;
        op->opersize = SIZE_b;
        op->nearptr.offset = disp_i->Ib;
        nOpImmedLen = 1;

        k = eip + disp_i->ilen + (long)(char)op->nearptr.offset;

        op->nearptr.label = (DWORD)k;

        if (OperandSize == MODE_32BIT)
        {
            sprintf_s(op->string, "%08X", k);
        }
        else
        {
            sprintf_s(op->string, "%04X", (WORD)k);
        }
        break;

    case D__Jv:
        op->mode = mode_near;
        op->opersize = SIZE_v;

        if (op->opersize == SIZE_d)
        {
            op->nearptr.offset = disp_i->Id;
        }
        else
        {
            op->nearptr.offset = disp_i->Iw;
        }

        nOpImmedLen = SIZE_v;

        k = eip + disp_i->ilen + op->nearptr.offset;

        op->nearptr.label = (DWORD)k;

        if (OperandSize == MODE_32BIT)
        {
            sprintf_s(op->string, "%08X", k);
        }
        else
        {
            sprintf_s(op->string, "%04X", (WORD)k);
        }
        break;

    case D__Ob:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ | RWFLG_WRITE;
            op->opersize = SIZE_b;
        }
    case D_rOb:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ;
            op->opersize = SIZE_b;
        }
    case D_wOb:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_WRITE;
            op->opersize = SIZE_b;
        }
    case D__Ov:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ | RWFLG_WRITE;
            op->opersize = SIZE_v;
        }
    case D_rOv:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ;
            op->opersize = SIZE_v;
        }
    case D_wOv:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_WRITE;
            op->opersize = SIZE_v;
        }

        disp_i->base = -1;
        disp_i->index = -1;
        disp_i->scale = 0;
        disp_i->seg = I_DS;

        if (AddressSize == MODE_32BIT)
        {
            disp_i->as_32 = MODE_32BIT;
            disp_i->displacement_size = 4;
            disp_i->addr_displacement = disp_i->Id;
            nOpImmedLen = 4;
        }
        else
        {
            disp_i->as_32 = MODE_16BIT;
            disp_i->displacement_size = 2;
            disp_i->addr_displacement = disp_i->Iw;
            nOpImmedLen = 2;
        }

        disp_i->rm_addr = disp_i->addr_displacement;

        op->mode = mode_address;
        OnDispTranDis(&op->addr, disp_i);
        break;

    case D__Ap:

        op->mode = mode_far;
        if (OperandSize == MODE_32BIT)
        {
            op->farptr.offset = disp_i->Id;
            op->farptr.segment = disp_i->Iw2;
        }
        else
        {
            op->farptr.offset = disp_i->Iw;
            op->farptr.segment = disp_i->Iw2;
        }

        op->opersize = SIZE_p;
        nOpImmedLen = SIZE_p;

        if (OperandSize == MODE_32BIT)
        {
            sprintf_s(op->string, "%04X:%08X", op->farptr.segment, op->farptr.offset);
        }
        else
        {
            sprintf_s(op->string, "%04X:%04X", op->farptr.segment, op->farptr.offset);
        }
        break;

    case D__Cd:
        op->mode = mode_register;
        op->opersize = SIZE_d;
        op->reg.reg_index = disp_i->nnn;
        sprintf_s(op->string, "CR%x", disp_i->nnn);
        if (op->reg.reg_index > 0x03 || op->reg.reg_index == 0x01)
        {
            op->mode = mode_invalid;
            return 0;
        }
        break;

    case D__Dd:
        op->mode = mode_register;
        op->opersize = SIZE_d;
        op->reg.reg_index = disp_i->nnn;

        sprintf_s(op->string, "DR%x", disp_i->nnn);
        break;

    case D__Fv:
        break;

    case D__Gb:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ | RWFLG_WRITE;
            op->opersize = SIZE_b;
        }
    case D_rGb:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ;
            op->opersize = SIZE_b;
        }
    case D_wGb:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_WRITE;
            op->opersize = SIZE_b;
        }

        op->mode = mode_register;
        op->reg.reg_index = disp_i->nnn;
        strcpy_s(op->string, g_szRegByte[disp_i->nnn]);
        break;
    case D__Gw:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ | RWFLG_WRITE;
            op->opersize = SIZE_w;
        }
    case D_rGw:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ;
            op->opersize = SIZE_w;
        }
    case D_wGw:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_WRITE;
            op->opersize = SIZE_w;
        }

        op->mode = mode_register;
        op->reg.reg_index = disp_i->nnn;
        strcpy_s(op->string, g_szRegWord[disp_i->nnn]);
        break;

    case D__Gv:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ | RWFLG_WRITE;
            op->opersize = SIZE_v;
        }
    case D_rGv:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ;
            op->opersize = SIZE_v;
        }
    case D_wGv:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_WRITE;
            op->opersize = SIZE_v;
        }

        op->mode = mode_register;
        op->reg.reg_index = disp_i->nnn;
        if (op->opersize == SIZE_w)
        {
            strcpy_s(op->string, g_szRegWord[disp_i->nnn]);
        }
        else
        {
            strcpy_s(op->string, g_szRegDword[disp_i->nnn]);
        }
        break;

    case D__Gd:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ | RWFLG_WRITE;
            op->opersize = SIZE_d;
        }
    case D_rGd:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ | RWFLG_WRITE;
            op->opersize = SIZE_d;
        }
    case D_wGd:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ | RWFLG_WRITE;
            op->opersize = SIZE_d;
        }

        op->mode = mode_register;
        op->reg.reg_index = disp_i->nnn;
        strcpy_s(op->string, g_szRegDword[disp_i->nnn]);
        break;

        // The reg field of the ModR/M byte selects a packed quadword MMX?technology reg-ister.
    case D__Pd:		
        if (op->opersize == 0)
        {
            op->opersize = SIZE_d;
        }
    case D__Pq:		
        if (op->opersize == 0)
        {
            op->opersize = SIZE_q;
        }
    case D__Ppi:	
        if (op->opersize == 0)
        {
            op->opersize = SIZE_pi;
        }
        op->mode = mode_register;
        op->reg.reg_index = disp_i->nnn;
        sprintf_s(op->string, "MM%x", disp_i->nnn);
        break;

    case D__Rd:
        if (disp_i->mod != 3)
        {
            return -1;
        }

        op->mode = mode_register;
        op->opersize = SIZE_d;
        op->reg.reg_index = disp_i->rm;

        strcpy_s(op->string, g_szRegDword[disp_i->rm]);
        break;

    case D__Sw:
        op->mode = mode_segment;
        op->opersize = SIZE_w;
        op->seg.seg_index = disp_i->nnn;
        strcpy_s(op->string, g_szRegSegment[disp_i->nnn]);
        break;

    case D__NTA:
        break;
    case D__T0:
    case D__T1:
    case D__T2:

        op->mode = mode_register;
        op->opersize = SIZE_d;
        op->reg.reg_index = nOpdata - D__T0;
        sprintf_s(op->string, "T%x", nOpdata - D__T0);
        break;

        // The reg field of the ModR/M byte selects a packed SIMD floating-point register.
    case D__Vq:		
        if (op->opersize == 0)
        {
            op->opersize = SIZE_q;
        }
    case D__Vps:	
        if (op->opersize == 0)
        {
            op->opersize = SIZE_ps;
        }
    case D__Vss:	
        if (op->opersize == 0)
        {
            op->opersize = SIZE_ss;
        }

        op->mode = mode_register;
        op->reg.reg_index = disp_i->nnn;

        sprintf_s(op->string, "XMM%x", disp_i->nnn);
        break;

    case D__Xb:
    case D_rXb:
    case D_wXb:
    case D__Xw:
    case D_rXw:
    case D_wXw:
    case D__Xv:
    case D_rXv:
    case D_wXv:
    case D__Xd:
    case D_rXd:
    case D_wXd:
        nOpImmedLen = 0;
        op->mode = mode_invalid;
        break;

    case D__Yb:
    case D_rYb:
    case D_wYb:
    case D__Yw:
    case D_rYw:
    case D_wYw:
    case D__Yv:
    case D_rYv:
    case D_wYv:
    case D__Yd:
    case D_rYd:
    case D_wYd:
        nOpImmedLen = 0;
        op->mode = mode_invalid;
        break;
	
    case D__AL:
    case D__CL:
    case D__DL:
    case D__BL:
    case D__AH:
    case D__CH:
    case D__DH:
    case D__BH:
        op->mode = mode_register;
        op->opersize = SIZE_b;
        op->reg.reg_index = nOpdata - D__AL;
        strcpy_s(op->string, g_szRegByte[nOpdata - D__AL]);
        break;

    case D__AX:
    case D__CX:
    case D__DX:
    case D__BX:
    case D__SP:
    case D__BP:
    case D__SI:
    case D__DI:
        op->mode = mode_register;
        op->opersize = SIZE_w;
        op->reg.reg_index = nOpdata - D__AX;
        strcpy_s(op->string, g_szRegWord[nOpdata - D__AX]);
        break;

    case D__eAX:
    case D__eCX:
    case D__eDX:
    case D__eBX:
    case D__eSP:
    case D__eBP:
    case D__eSI:
    case D__eDI:
        op->mode = mode_register;
        op->opersize = SIZE_v;
        op->reg.reg_index = nOpdata - D__eAX;

        if (op->opersize == SIZE_w)
        {
            strcpy_s(op->string, g_szRegWord[nOpdata - D__eAX]);
        }
        else
        {
            strcpy_s(op->string, g_szRegDword[nOpdata - D__eAX]);
        }
        break;

    case D__EAX:
    case D__ECX:
    case D__EDX:
    case D__EBX:
    case D__ESP:
    case D__EBP:
    case D__ESI:
    case D__EDI:
        op->mode = mode_register;
        op->opersize = SIZE_d;
        op->reg.reg_index = nOpdata - D__EAX;
        strcpy_s(op->string, g_szRegDword[nOpdata - D__EAX]);
        break;

    case D__ES:
    case D__CS:
    case D__SS:
    case D__DS:
    case D__FS:
    case D__GS:
        op->mode = mode_segment;
        op->opersize = SIZE_w;
        op->reg.reg_index = nOpdata - D__ES;
        strcpy_s(op->string, g_szRegSegment[nOpdata - D__ES]);
        break;

    case D__st:
        op->mode = mode_register;
        op->opersize = SIZE_t;
        op->reg.reg_index = 0;
        strcpy_s(op->string, "ST");
        break;
    case D__st0:
    case D__st1:
    case D__st2:
    case D__st3:
    case D__st4:
    case D__st5:
    case D__st6:
    case D__st7:
        op->mode = mode_register;
        op->opersize = SIZE_t;
        op->reg.reg_index = nOpdata - D__st0;
        sprintf_s(op->string, "ST(%x)", nOpdata - D__st0);
        break;

    case D__Eb:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ | RWFLG_WRITE;
            op->opersize = SIZE_b;
        }
    case D_rEb:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ;
            op->opersize = SIZE_b;
        }
    case D_wEb:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_WRITE;
            op->opersize = SIZE_b;
        }
    case D__Ew:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ | RWFLG_WRITE;
            op->opersize = SIZE_w;
        }
    case D_rEw:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ;
            op->opersize = SIZE_w;
        }
    case D_wEw:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_WRITE;
            op->opersize = SIZE_w;
        }
    case D__Ev:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ | RWFLG_WRITE;
            op->opersize = SIZE_v;
        }
    case D_rEv:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ;
            op->opersize = SIZE_v;
        }
    case D_wEv:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_WRITE;
            op->opersize = SIZE_v;
        }
    case D__Ed:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ | RWFLG_WRITE;
            op->opersize = SIZE_d;
        }
    case D_rEd:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ;
            op->opersize = SIZE_d;
        }
    case D_wEd:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_WRITE;
            op->opersize = SIZE_d;
        }
    case D__Ep:
    case D_rEp:
        if (op->opersize == 0)
        {
            op->rwflag = 1;
            op->opersize = SIZE_p;
        }

        if (disp_i->mod == 3)
        {
            if (nOpdata == D__Ep || nOpdata == D_rEp)
            {
                return -1;
            }
            else
            {
                op->mode = mode_register;
            }
            k = disp_i->rm;
            op->reg.reg_index = k;
            if (op->opersize == SIZE_b)
            {
                strcpy_s(op->string, g_szRegByte[k]);
            }
            else if (op->opersize == SIZE_w)
            {
                strcpy_s(op->string, g_szRegWord[k]);
            }
            else
            {
                strcpy_s(op->string, g_szRegDword[k]);
            }
        }
        else
        {
            op->mode = mode_address;
            OnDispTranDis(&op->addr, disp_i);
        }
        break;

    case D__M:
        if (op->opersize == 0)
        {
            op->rwflag = NULL; //read|write
            op->opersize = -1;
        }
    case D__Ma:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ | RWFLG_WRITE; //read|write
            op->opersize = SIZE_a;
        }
    case D_rMa:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ;
            op->opersize = SIZE_a;
        }
    case D__Mb:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ | RWFLG_WRITE;
            op->opersize = SIZE_b;
        }
    case D__Mp:
    case D_rMp:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ;
            op->opersize = SIZE_p;
        }
    case D__Ms:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ | RWFLG_WRITE;
            op->opersize = SIZE_s;
        }
    case D_rMs:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ;
            op->opersize = SIZE_s;
        }
    case D_wMs:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_WRITE;
            op->opersize = SIZE_s;
        }
    case D__Mq:
        if (op->opersize == 0)
        {
            op->rwflag = RWFLG_READ | RWFLG_WRITE;
            op->opersize = SIZE_q;
        }

        if (disp_i->mod == 3)
        {
            return -1;
        }
        else
        {
            if (op->opersize == -1) op->opersize = 0;
            op->mode = mode_address;
            OnDispTranDis(&op->addr, disp_i);
        }
        break;

    case D__Qd:		
        if (op->opersize == 0)
        {
            op->opersize = SIZE_d;
        }
    case D__Qq:		
        if (op->opersize == 0)
        {
            op->opersize = SIZE_q;
        }
    case D__Qpi:	
        if (op->opersize == 0)
        {
            op->opersize = SIZE_pi;
        }

        if (disp_i->mod == 3)
        {
            op->mode = mode_register;
            op->reg.reg_index = disp_i->rm;
            sprintf_s(op->string, "MM%x", disp_i->rm);
        }
        else
        {
            op->mode = mode_address;
            OnDispTranDis(&op->addr, disp_i);
        }
        break;

    case D__Wq:		
        if (op->opersize == 0)
        {
            op->opersize = SIZE_q;
        }
    case D__Wps:	
        if (op->opersize == 0)
        {
            op->opersize = SIZE_ps;
        }
    case D__Wss:	
        if (op->opersize == 0)
        {
            op->opersize = SIZE_ss;
        }

        if (disp_i->mod == 3)
        {
            op->mode = mode_register;
            op->reg.reg_index = disp_i->rm;
            sprintf_s(op->string, "XMM%x", disp_i->rm);
        }
        else
        {
            op->mode = mode_address;
            OnDispTranDis(&op->addr, disp_i);
        }
        break;

    case D__word_int:
        if (op->opersize == 0)
        {
            op->opersize = SIZE_w;
        }
    case D__dword_int:
        if (op->opersize == 0)
        {
            op->opersize = SIZE_d;
        }
    case D__qword_int:
        if (op->opersize == 0)
        {
            op->opersize = SIZE_q;
        }
    case D__single_real:
        if (op->opersize == 0)
        {
            op->opersize = SIZE_d;
        }
    case D__double_real:
        if (op->opersize == 0)
        {
            op->opersize = SIZE_q;
        }
    case D__extend_real:
        if (op->opersize == 0)
        {
            op->opersize = SIZE_t;
        }
    case D__packed_BCD:
        if (op->opersize == 0)
        {
            op->opersize = SIZE_t;
        }
    case D__2b:
        if (op->opersize == 0)
        {
            op->opersize = SIZE_w;
        }
    case D__14_28b:
        if (op->opersize == 0)
        {
            op->opersize = SIZE_t;
        }
    case D__98_108b:
        if (op->opersize == 0)
        {
            op->opersize = SIZE_t;
        }

        if (disp_i->mod == 3)
        {
            return -1;
        }
        else
        {
            op->mode = mode_address;
            OnDispTranDis(&op->addr, disp_i);
        }
        break;

    default:
        nOpImmedLen = 0;
        op->mode = mode_invalid;
        op->opersize = 0;
    }

    return nOpImmedLen;
}

/************************************************************************
函数名字：int OnPrintPasm(INSTRUCTION_INFORMATION *pInstInfo, char *pAsm)
功    能：拼接指令文本和统计长度
参  数 1：pInstInfo  指令结构
参  数 2：pAsm       指令的文本
返 回 值：指令文本长度
************************************************************************/
int OnPrintPasm(INSTRUCTION_INFORMATION *pInstInfo, char *pAsm)
{
    int i = 0, length = 0;
    char szFormat[0x20] = { 0 };
    char **preg = NULL;

    for (i = 0; i <= 0x50; i++)
    {
        pAsm[i] = 0x20;
    }
    pAsm[i] = '\0';

    if (pInstInfo->Lock != -1)
    {
        i = OnPutStr(pAsm + length, pInstInfo->LockName);
        if (i)
        {
            length += i + 1;
        }
    }

    if (pInstInfo->Repeat != -1)
    {
        i = OnPutStr(pAsm + length, pInstInfo->RepeatName);
        if (i)
        {
            length += i + 1;
        }
    }

    length += OnPutStr(pAsm + length, pInstInfo->Name);

    OPERAND_ITEM *op = pInstInfo->op;

    for (i = 0; i < 3 && op->mode != mode_invalid; i++, op++)
    {
        if (i == 0)
        {
            if (length < 0)
            {
                length = 0;
            }
            else
            {
                length++;
            }
        }

        if (i > 0)
        {
            length += OnPutStr(pAsm + length, ",");
        }

        if (op->mode == mode_address)
        {
            if (pInstInfo->PtrType != -1 && pInstInfo->PtrType != 0)
            {
                length += OnPutStr(pAsm + length, g_szPtr[pInstInfo->PtrType]);
                length += OnPutStr(pAsm + length, " PTR ");
            }

            if (pInstInfo->SegmentPrefix != -1)
            {
                length += OnPutStr(pAsm + length, g_szRegSegment[pInstInfo->SegmentPrefix]);
                length += OnPutStr(pAsm + length, ":");
            }

            if (op->addr.address_size == MODE_32BIT)
            {
                preg = g_szRegDword;
            }
            else
            {
                preg = g_szRegWord;
            }

            length += OnPutStr(pAsm + length, "[");
            if (op->addr.base != -1)
            {
                length += OnPutStr(pAsm + length, preg[op->addr.base]);
            }

            if (op->addr.index != -1)
            {
                if (op->addr.base != -1)
                {
                    length += OnPutStr(pAsm + length, "+");
                }
                length += OnPutStr(pAsm + length, preg[op->addr.index]);
                if (op->addr.scale > 0)
                {
                    sprintf_s(szFormat, "*%1x", 1 << op->addr.scale);
                    length += OnPutStr(pAsm + length, szFormat);
                }
            }

            if (op->addr.displacement_size >= 2 
                && (op->addr.base != -1 || op->addr.index != -1))
            {
                length += OnPutStr(pAsm + length, "+");
            }

            if (op->addr.displacement_size == 1)
            {
                if (op->addr.displacement >= sign_BYTE)
                {
                    sprintf_s(szFormat, "-%02X", (BYTE)(0 - op->addr.displacement));
                }
                else
                {
                    sprintf_s(szFormat, "+%02X", (BYTE)op->addr.displacement);
                }
                length += OnPutStr(pAsm + length, szFormat);
            }
            else if (op->addr.displacement_size == 2)
            {
                sprintf_s(szFormat, "%04X", op->addr.displacement);

                length += OnPutStr(pAsm + length, szFormat);
            }
            else if (op->addr.displacement_size == 4)
            {
                sprintf_s(szFormat, "%08X", op->addr.displacement);

                length += OnPutStr(pAsm + length, szFormat);
            }

            length += OnPutStr(pAsm + length, "]");
        }
        else
        {
            length += OnPutStr(pAsm + length, op->string);
        }
    }
    pAsm[length] = '\0';

    return length;
}

/************************************************************************
函数名字：void OnDispTranDis(DIS_ADDRESS * dis_a, DISP_INSTRUCTION_T *disp_i)
功    能：操作码信息给操作数
参  数 1：dis_a  操作数的信息
参  数 2：disp_i 操作码的信息
返 回 值：无
************************************************************************/
void OnDispTranDis(DIS_ADDRESS * dis_a, DISP_INSTRUCTION_T *disp_i)
{
    dis_a->mod = disp_i->mod;
    dis_a->reg = disp_i->nnn;
    dis_a->r_m = disp_i->rm;
    dis_a->base = disp_i->base;
    dis_a->index = disp_i->index;
    dis_a->scale = disp_i->scale;
    dis_a->segment = disp_i->seg;
    dis_a->address_size = disp_i->as_32;
    dis_a->displacement_size = disp_i->displacement_size;
    dis_a->displacement = disp_i->addr_displacement;
    dis_a->effect_address = disp_i->rm_addr;

    return;
}

/************************************************************************
函数名字：int OnPutStr(char *pBuf, char *s)
功    能：拷贝文本
参  数 1：pBuf   目标Buf
参  数 2：s      源Buf
返 回 值：长度
************************************************************************/
int OnPutStr(char *pBuf, char *s)
{
    int i = 0;
    for (i = 0; s[i] != '\0'; i++)
    {
        pBuf[i] = s[i];
    }
    return i;
}