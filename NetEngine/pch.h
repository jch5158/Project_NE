#pragma once

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#ifdef _DEBUG
#pragma comment(lib, "Debug\\NetCore.lib")
#else
#pragma comment(lib, "Release\\NetCore.lib")
#endif

#include "CorePch.h"
