#pragma once
#include "inteldef.h"
//////////////////////////////////////////////////////////////////////////
//intel ָ��Ĳ��������͡�
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
    int Attr;			//Ѱַ��ʽ�ֶ�
    int OpcodeType;		//����������
    int Opdata1;		//����������
    int Opdata2;
    int Opdata3;
    char *name;			//ָ������
} DIS_OPCODE_T, *PDIS_OPCODE_T;

//�ڴ�Ѱַ����������ṹ
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
    unsigned displacement_size; //ƫ���ֽ�����1/2/4

    DWORD displacement;		//mov eax,[ebx+esi*4+8f] 8f
    DWORD effect_address;	//��Ч��ַ
    DWORD line_address;
} DIS_ADDRESS, *PDIS_ADDRESS;

//intel ����������ṹ
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
    unsigned attr;          //Ѱַ��ʽ
    unsigned b1;            //������
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


//intel ָ������ṹ
typedef struct  _INSTRUCTION_INFORMATION
{
    DWORD eip;		            //����ָ��������ַ 
    int CodeMode;	            //�����ģʽ
    int Length;		            //ָ���
    int	OpCode;		            //����ʶ
    char Name[0x10];			//����ı�
    int	 Lock;					//ǰ׺���η���ʶ
    char LockName[0x08];		//ǰ׺���η��ı�
    int	 Repeat;				//ѭ��ָ���ʶ
    char RepeatName[0x08];		//ѭ��ָ���ı�
    int SegmentPrefix;			//�ο�Խǰ׺
    int PtrType;				//��������ָʾ
    OPERAND_ITEM op[3];			//intel ָ�� ������� ��������
    BYTE *CodeBuff;			    //hex
    char *pAsmString;			//�������ָ��
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
//��������
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