// Lab2Pd.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <SetupAPI.h>
#include <initguid.h>
#include <devguid.h>

#pragma comment(lib, "SetupAPI.lib")

using namespace std;

void listConnectedDevices() {
    HDEVINFO deviceInfoSet;
    SP_DEVINFO_DATA deviceInfoData;
    DWORD index = 0;

    deviceInfoSet = SetupDiGetClassDevs(&GUID_DEVCLASS_USB, 0, 0, DIGCF_PRESENT); // obtinere lista doar dispozitive usb connectate

    if (deviceInfoSet == INVALID_HANDLE_VALUE)
    {
        cerr << "Eroare la obitnerea listei de dispozitive USB conectate\n";
        return;
    }

    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    cout << "Lista de dispozitive USB conectate acum: \n";

    while (SetupDiEnumDeviceInfo(deviceInfoSet, index, &deviceInfoData)) // enumera din set 1 cate 1 
    {
        char deviceName[100];
        DWORD size = 0;

        if (SetupDiGetDeviceInstanceIdA(deviceInfoSet, &deviceInfoData, deviceName, sizeof(deviceName), &size)) // obtine numele 
        {
            cout << "Device USB: " << deviceName << endl;
        }

        index++;
    }

    SetupDiDestroyDeviceInfoList(deviceInfoSet);
}

int main()
{
    listConnectedDevices();
    cout << "Hello World!\n";
}

