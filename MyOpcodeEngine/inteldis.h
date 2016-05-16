#pragma once
#include "inteldef.h"
//////////////////////////////////////////////////////////////////////////
//intel 指令的操作数类型　
#define mode_invalid	0
#define mode_address	1
#define mode_register	2
#define mode_segment	3
#define mode_immed		4
#define mode_near		5
#define mode_far		6

#define	RWFLG_UNKNOWN		0
#define RWFLG_READ			1
#define RWFLG_WRITE			2
#define	RWFLG_ACCESS		3

#define SIZE_a		8
#define SIZE_b		1
#define SIZE_d		4
#define SIZE_dq		8
#define SIZE_pi		8
#define SIZE_ps		8
#define SIZE_q		4
#define SIZE_s		6
#define SIZE_ss		16
#define SIZE_t		10
#define SIZE_w		2
//////////////////////////////////////////////////////////////////////////
typedef struct
{
    union
    {
        DWORD erx;
        struct
        {
            union
            {
                WORD rx;
                struct
                {
                    BYTE rl;
                    BYTE rh;
                } byte;
            };

            WORD word_filler;
        } word;
    };

} DIS_GEN_REG_T;

typedef struct _DIS_CPU
{
    DIS_GEN_REG_T gen_reg[0x08];
    DWORD SEG_BASE[0x06];

} DIS_CPU, *PDIS_CPU;

typedef struct _DIS_MODR_M
{
    unsigned r_m : 3;
    unsigned reg : 3;
    unsigned mod : 2;
    unsigned base : 3;
    unsigned index : 3;
    unsigned scale : 2;
    unsigned disp0 : 8;
    unsigned disp1 : 8;
    unsigned disp2 : 8;
    unsigned disp3 : 8;
} DIS_MODR_M, *PDIS_MODR_M;

typedef struct _DIS_OPCODE_T
{
    int Attr;			//寻址方式字段
    int OpcodeType;		//操作码类型
    int Opdata1;		//操作数类型
    int Opdata2;
    int Opdata3;
    char *name;			//指令名称
} DIS_OPCODE_T, *PDIS_OPCODE_T;

//内存寻址操作数描叙结构
typedef struct _DIS_ADDRESS
{
    unsigned address_size;	//[bx+si]/[ebx+edi]

    unsigned r_m;
    unsigned reg;
    unsigned mod;
    unsigned base;
    unsigned index;
    unsigned scale;
    unsigned segment;
    unsigned displacement_size; //偏移字节数　1/2/4

    DWORD displacement;		//mov eax,[ebx+esi*4+8f] 8f
    DWORD effect_address;	//有效地址
    DWORD line_address;
} DIS_ADDRESS, *PDIS_ADDRESS;

//intel 操作数描叙结构
typedef struct _OPERAND_ITEM
{
    int mode;
    int rwflag;
    int opersize;
    char string[0x20];

    union
    {
        DIS_ADDRESS addr;
        struct
        {
            int  reg_index;
        } reg;

        struct
        {
            int seg_index;
        } seg;

        struct
        {
            DWORD immed_value;

        } immed;

        struct
        {
            DWORD offset;
            DWORD label;

        } nearptr;

        struct
        {
            DWORD   offset;
            DWORD   segment;

        } farptr;


        struct
        {
            int count;
            BYTE buffer[0x20];

        } datadup;

        struct
        {
            char string[0x40];
            BOOL offset;
        } symbol;

        struct
        {
            DWORD nBytes;
        } align;
    };

} OPERAND_ITEM, *POPERAND_ITEM;

typedef struct _DISP_INSTRUCTION_T
{
    unsigned attr;          //寻址方式
    unsigned b1;            //操作码
    unsigned rep_used;
    unsigned lock;
    unsigned modrm;
    unsigned mod;
    unsigned nnn;
    unsigned rm;
    WORD displ16u;
    DWORD displ32u;
    unsigned seg;
    unsigned sib;
    unsigned scale;
    unsigned index;
    unsigned base;
    DWORD   addr_displacement;
    DWORD   rm_addr;
    DWORD   Id;
    WORD	Iw;
    BYTE    Ib;
    BYTE    Ib2;
    WORD	Iw2;
    unsigned ilen;
    unsigned modrm_length;
    unsigned displacement_size;
    unsigned os_32, as_32;
    unsigned flags_in, flags_out;
    unsigned destination, source, other;
} DISP_INSTRUCTION_T;


//intel 指令描叙结构
typedef struct  _INSTRUCTION_INFORMATION
{
    DWORD eip;		            //本条指令的虚拟地址 
    int CodeMode;	            //反汇编模式
    int Length;		            //指令长度
    int	OpCode;		            //汇编标识
    char Name[0x10];			//汇编文本
    int	 Lock;					//前缀修饰符标识
    char LockName[0x08];		//前缀修饰符文本
    int	 Repeat;				//循环指令标识
    char RepeatName[0x08];		//循环指令文本
    int SegmentPrefix;			//段跨越前缀
    int PtrType;				//操作类型指示
    OPERAND_ITEM op[3];			//intel 指令 中最多有 三个参数
    BYTE *CodeBuff;			    //hex
    char *pAsmString;			//完整汇编指令
} INSTRUCTION_INFORMATION, *PINSTRUCTION_INFORMATION;

//Opdata1, Opdata2, Opdata3
typedef struct _tagLOCKREPXX
{
    char* Name;
    BYTE  Code;
} LOCKREPXX, PLOCKREPXX;

//xxxptr
typedef struct _OPDATAWIDE
{
    int Size;
    char *Xptr;
}  OPDATAWIDE, POPDATAWIDE;
//////////////////////////////////////////////////////////////////////////
//函数声明
BOOL OnDisassembly(INSTRUCTION_INFORMATION *pInstInfo, int &pCodeLen,
    BYTE *pInbuff, DWORD dwEip, int nMode, PDIS_CPU pTagDisCPU);
int OnGetAddress32Dis(DIS_CPU * pDisCPU, BYTE *pbuff, DISP_INSTRUCTION_T *disp_i);
int OnGetAddress16Dis(DIS_CPU * pDisCPU, BYTE *pbuff, DISP_INSTRUCTION_T *disp_i);
int OnProcessOpdata_immde(int nOpdata, BYTE *pInbuff, DISP_INSTRUCTION_T *disp_i);
bool OnProcessName_dis(INSTRUCTION_INFORMATION* pInstInfo, DIS_OPCODE_T *pIntelOpcode,
    int OperandSize, int AddressSize);
int OnProcessOpdata_dis(int nOpdata, POPERAND_ITEM op, DWORD eip, DISP_INSTRUCTION_T *disp_i);
int OnPrintPasm(INSTRUCTION_INFORMATION *pInstInfo, char *pAsm);
void OnDispTranDis(DIS_ADDRESS * dis_a, DISP_INSTRUCTION_T *disp_i);
int OnPutStr(char *pBuf, char *s);