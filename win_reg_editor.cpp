// QueryKey - Enumerates the subkeys of key and its associated values.
//     hKey - Key whose subkeys and values are to be enumerated.

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <Lmcons.h>


#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

void QueryKey(HKEY hKey)
{
    TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
    DWORD    cbName;                   // size of name string 
    //TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
    LPSTR    achClass = NULL;
    DWORD    cchClassName = MAX_PATH;  // size of class string 
    DWORD    cSubKeys = 0;               // number of subkeys 
    DWORD    cbMaxSubKey;              // longest subkey size 
    DWORD    cchMaxClass;              // longest class string 
    DWORD    cValues;              // number of values for key 
    DWORD    cchMaxValue;          // longest value name 
    DWORD    cbMaxValueData;       // longest value data 
    DWORD    cbSecurityDescriptor; // size of security descriptor 
    FILETIME ftLastWriteTime;      // last write time 

    DWORD i, retCode;

    TCHAR  achValue[MAX_VALUE_NAME];

    unsigned char achData[MAX_VALUE_NAME];

    DWORD cchValue = MAX_VALUE_NAME;

    DWORD achType = MAX_VALUE_NAME;

    // Get the class name and the value count. 
    retCode = RegQueryInfoKeyA(
        hKey,                    // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // size of class string 
        NULL,                    // reserved 
        &cSubKeys,               // number of subkeys 
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class string 
        &cValues,                // number of values for this key 
        &cchMaxValue,            // longest value name 
        &cbMaxValueData,         // longest value data 
        &cbSecurityDescriptor,   // security descriptor 
        &ftLastWriteTime);       // last write time 

    // Enumerate the subkeys, until RegEnumKeyEx fails.

    if (cSubKeys)
    {
        printf("\nNumber of subkeys: %d\n", cSubKeys);

        for (i = 0; i < cSubKeys; i++)
        {
            cbName = MAX_KEY_LENGTH;
            retCode = RegEnumKeyEx(hKey, i,
                achKey,
                &cbName,
                NULL,
                NULL,
                NULL,
                &ftLastWriteTime);
            if (retCode == ERROR_SUCCESS)
            {
                _tprintf(TEXT("(%d) %s\n"), i + 1, achKey);
                
            }
           
        }
    }
    // Enumerate the key values. 

    if (cValues)
    {
        printf("\nNumber of values: %d\n", cValues);

        for (i = 0, retCode = ERROR_SUCCESS; i < cValues; i++)
        {
            cchValue = MAX_VALUE_NAME;
            achValue[0] = '\0';
            achData[0] = '\0';
            retCode = RegEnumValueW(hKey, i,
                achValue,
                &cchValue,
                NULL,
                &achType,
                achData,
                &cchValue);
          

            if (retCode == ERROR_SUCCESS && achType == REG_SZ)
            {
                _tprintf(TEXT("(%d) (%s)\n"), i + 1, achValue);

                if (RegSetValueEx(
                    hKey,
                    TEXT("test"),
                    0,
                    REG_SZ,
                    (LPBYTE)("value_data"),
                    strlen("value_data") * sizeof(char)) == ERROR_SUCCESS) {
                    std::cout << "success" << std::endl;
                }

            }
        }
    }
    
}

int __cdecl _tmain()
{
    HKEY hTestKey;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
        TEXT("SOFTWARE\\Microsoft"),
        0,
        KEY_ALL_ACCESS,
        &hTestKey) == ERROR_SUCCESS
        )
    {

        TCHAR value[255] = { 0 };
        DWORD BufferSize = 512;
        // Get Reg key data
        if (RegGetValueW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft", L"test", RRF_RT_ANY, NULL, (PVOID)&value, &BufferSize) == ERROR_SUCCESS)
        {
            _tprintf(TEXT("The value is %s\n"), value);
        } 
        TCHAR name[6] = _T("test");
        TCHAR data[6] = _T("data");
        wchar_t test_data[6] = L"中文";
        // Set Reg key data
        if (RegSetKeyValueW(
            hTestKey,
            TEXT(""),
            TEXT("test"),
            REG_SZ,
            test_data,
            sizeof(TEXT("data"))) == ERROR_SUCCESS) {
            std::cout << "Successfully changed" << std::endl;
        }
        // iterate certain registry key and its subkey
        QueryKey(hTestKey);
    }

    RegCloseKey(hTestKey);
}