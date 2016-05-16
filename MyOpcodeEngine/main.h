#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "inteldef.h"
#include "inteldis.h"
//////////////////////////////////////////////////////////////////////////
#define FILEBUGSIZE 1000
//////////////////////////////////////////////////////////////////////////
PBYTE GetFileOepBuf(char *pFileName);
void StartDecode(PBYTE pFileBuf);