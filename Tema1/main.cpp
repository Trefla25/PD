#include <windows.h>
#include <iostream>

using std::wcout;
using std::wcerr;
using std::endl;

void PrintServiceImagePath(HKEY parentKey, const wchar_t* serviceName) {
    HKEY serviceKey;
    if (RegOpenKeyEx(parentKey, serviceName, 0, KEY_READ, &serviceKey) == ERROR_SUCCESS) {
        wchar_t imagePath[1024];
        DWORD bufferSize = sizeof(imagePath);

        LONG result = RegQueryValueExW(serviceKey, L"ImagePath", nullptr, nullptr, reinterpret_cast<LPBYTE>(imagePath), &bufferSize);
        if (result == ERROR_SUCCESS) {
            wcout << L"    ImagePath: " << imagePath << endl;
        }
        else {
            wcout << L"    ImagePath: (missing)" << endl;
        }

        RegCloseKey(serviceKey);
    }
    else {
        wcout << L"    Unable to open service subkey" << endl;
    }
}

int main() {
    const wchar_t* servicesPath = L"SYSTEM\\CurrentControlSet\\Services";
    HKEY servicesKey;

    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, servicesPath, 0, KEY_READ, &servicesKey) != ERROR_SUCCESS) {
        wcerr << L"Failed to access registry path: " << servicesPath << endl;
        return 1;
    }

    wcout << L"Listing all services and their ImagePath (if available):" << endl;

    DWORD index = 0;
    wchar_t subKeyName[256];
    DWORD nameLength;

    while (true) {
        nameLength = static_cast<DWORD>(std::size(subKeyName)); // reset buffer size
        LONG result = RegEnumKeyExW(servicesKey, index, subKeyName, &nameLength, nullptr, nullptr, nullptr, nullptr);
        if (result != ERROR_SUCCESS)
            break;

        wcout << L"- Service: " << subKeyName << endl;
        PrintServiceImagePath(servicesKey, subKeyName);

        ++index;
    }

    RegCloseKey(servicesKey);
    return 0;
}
