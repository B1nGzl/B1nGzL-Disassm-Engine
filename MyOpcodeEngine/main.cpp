#include "main.h"


int g_nAddrOfEP = 0;

PBYTE GetFileOepBuf(char *pFileName)
{
    DWORD dwReadByte = 0;
    PBYTE pFileBuf = NULL;
    BOOL bRet = FALSE;
    STARTUPINFO tagStartupInfo = { 0 };
    PROCESS_INFORMATION tagProcessInfoRmation = { 0 };
    DEBUG_EVENT tagDebugEvent = { 0 };

    pFileBuf = (PBYTE)malloc(sizeof(BYTE) * FILEBUGSIZE);
    tagStartupInfo.cb = sizeof(STARTUPINFO);

    bRet = CreateProcessA(pFileName, NULL, NULL, NULL, FALSE,
        DEBUG_ONLY_THIS_PROCESS, NULL, NULL,
        &tagStartupInfo, &tagProcessInfoRmation);
    if (bRet == FALSE)
    {
        goto EXIT_FUN;
    }

    bRet = WaitForDebugEvent(&tagDebugEvent, INFINITE);
    if (bRet == 0)
    {
        goto EXIT_FUN;
    }

    g_nAddrOfEP = (int)tagDebugEvent.u.CreateProcessInfo.lpStartAddress;
    if (ReadProcessMemory(tagProcessInfoRmation.hProcess,
        (LPVOID)g_nAddrOfEP,
        pFileBuf, FILEBUGSIZE, &dwReadByte) && dwReadByte == FILEBUGSIZE)
    {
        return pFileBuf;
    }


EXIT_FUN:
    if (tagProcessInfoRmation.hThread != NULL)
    {
        CloseHandle(tagProcessInfoRmation.hThread);
    }

    if (tagProcessInfoRmation.hProcess != NULL)
    {
        CloseHandle(tagProcessInfoRmation.hProcess);
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    PBYTE pFileBuf = NULL;
    INSTRUCTION_INFORMATION tagInstInfo = { 0 };
    DIS_CPU tagDispCpu = { 0 };
    char szAsmString[0x80] = { 0 };
    BYTE szOpCode[0x10] = { 0 };
    int nCodeLen = 0, nShowCount = 0;

    //获取文件入口Buff
    pFileBuf = GetFileOepBuf(argv[1]);
    
    while (TRUE)
    {
        memset(szAsmString, 0, sizeof(szAsmString));
        memcpy(szOpCode, pFileBuf, sizeof(szOpCode));

        tagInstInfo.pAsmString = szAsmString;

        if (OnDisassembly(&tagInstInfo, nCodeLen, szOpCode,
            g_nAddrOfEP, MODE_32BIT, &tagDispCpu) == TRUE)
        {
            printf("%08X  %-30s   Length:%d\n", g_nAddrOfEP, szAsmString, nCodeLen);
        }
        pFileBuf = pFileBuf + nCodeLen;
        g_nAddrOfEP += nCodeLen;
        nCodeLen = 0;

        if (nShowCount == 20)
        {
            nShowCount = 0;
            system("pause");
        }
        nShowCount++;
    }
    return 0;
}