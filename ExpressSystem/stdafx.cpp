
// stdafx.cpp : 只包括标准包含文件的源文件
// ExpressSystem.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF")  
#import "c:/Program Files/Common Files/System/ADO/Msadox.dll"  rename_namespace("ADOCG") rename("EOF", "adoXEOF") rename("DataTypeEnum", "adoXDataTypeEnum")

