#include "detector.hpp"
#include <algorithm>
#include <string>
#include <windows.h>
#include <iostream>
#include <winreg.h>
#include <vector>

#include <psapi.h>
#include <imagehlp.h>

bool dirExists(const std::wstring& path) {
    DWORD attrs = GetFileAttributesW(path.c_str());
    return (attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY));
}

void ListDLLFunctions(std::string sADllName, std::vector<std::string>& slListOfDllFunctions)
{
    //https://stackoverflow.com/a/4354755
    DWORD *dNameRVAs(0);
    _IMAGE_EXPORT_DIRECTORY *ImageExportDirectory;
    unsigned long cDirSize;
    _LOADED_IMAGE LoadedImage;
    std::string sName;
    slListOfDllFunctions.clear();
    if (MapAndLoad(sADllName.c_str(), NULL, &LoadedImage, TRUE, TRUE))
    {
        ImageExportDirectory = (_IMAGE_EXPORT_DIRECTORY*)
            ImageDirectoryEntryToData(LoadedImage.MappedAddress,
            false, IMAGE_DIRECTORY_ENTRY_EXPORT, &cDirSize);
        if (ImageExportDirectory != NULL)
        {
            dNameRVAs = (DWORD *)ImageRvaToVa(LoadedImage.FileHeader, 
                LoadedImage.MappedAddress,
            ImageExportDirectory->AddressOfNames, NULL);
            for(size_t i = 0; i < ImageExportDirectory->NumberOfNames; i++)
            {
                sName = (char *)ImageRvaToVa(LoadedImage.FileHeader, 
                        LoadedImage.MappedAddress,
                       dNameRVAs[i], NULL);
             slListOfDllFunctions.push_back(sName);
            }
        }
        UnMapAndLoad(&LoadedImage);
    }
}


bool FileExists(const std::wstring& filePath) {
    DWORD attributes = GetFileAttributesW(filePath.c_str());
    if (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY)) {
        return true;
    }
    return false;
}

bool ProcessExists(const std::wstring& processName) {
    DWORD processes[1024];
    DWORD bytesReturned;
    if (!EnumProcesses(processes, sizeof(processes), &bytesReturned)) {
        std::cerr << "EnumProcesses failed. Error code: " << GetLastError() << std::endl;
        return false;
    }

    DWORD processCount = bytesReturned / sizeof(DWORD);

    for (DWORD i = 0; i < processCount; ++i) {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);
        if (hProcess) {
            WCHAR processFileName[MAX_PATH];
            if (GetModuleFileNameExW(hProcess, NULL, processFileName, MAX_PATH)) {
                std::wstring currentProcessName = processFileName;
                size_t pos = currentProcessName.find_last_of(L"\\/");
                if (pos != std::wstring::npos) {
                    currentProcessName = currentProcessName.substr(pos + 1);
                }

                if (_wcsicmp(currentProcessName.c_str(), processName.c_str()) == 0) {
                    CloseHandle(hProcess);
                    return true;
                }
            }
            CloseHandle(hProcess);
        }
    }

    return false;
}


bool ServiceExists(const std::wstring& serviceName) {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
    if (hSCManager == NULL) {
        std::cerr << "OpenSCManager failed. Error code: " << GetLastError() << std::endl;
        return false;
    }

    DWORD bytesNeeded = 0;
    DWORD servicesReturned = 0;
    DWORD resumeHandle = 0;
    std::vector<BYTE> buffer;

    // Get the required buffer size
    EnumServicesStatusExW(
        hSCManager,
        SC_ENUM_PROCESS_INFO,
        SERVICE_WIN32,
        SERVICE_STATE_ALL,
        NULL,
        0,
        &bytesNeeded,
        &servicesReturned,
        &resumeHandle,
        NULL
    );

    buffer.resize(bytesNeeded);

    // Get the service information
    if (!EnumServicesStatusExW(
        hSCManager,
        SC_ENUM_PROCESS_INFO,
        SERVICE_WIN32,
        SERVICE_STATE_ALL,
        buffer.data(),
        bytesNeeded,
        &bytesNeeded,
        &servicesReturned,
        &resumeHandle,
        NULL
    )) {
        std::cerr << "EnumServicesStatusExW failed. Error code: " << GetLastError() << std::endl;
        CloseServiceHandle(hSCManager);
        return false;
    }

    // Check if the specified service exists
    ENUM_SERVICE_STATUS_PROCESSW* services = reinterpret_cast<ENUM_SERVICE_STATUS_PROCESSW*>(buffer.data());
    for (DWORD i = 0; i < servicesReturned; ++i) {
        if (serviceName == services[i].lpServiceName) {
            CloseServiceHandle(hSCManager);
            return true;
        }
    }

    CloseServiceHandle(hSCManager);
    return false;
}

bool KeyExists(HKEY hKeyRoot, const std::wstring& subKey) {
    HKEY hKey;
    LONG result = RegOpenKeyExW(hKeyRoot, subKey.c_str(), 0, KEY_READ, &hKey);
    
    if (result == ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return true;
    } else {
        return false;
    }
}

Detect Detector::registryTest() {
    Detect detect;
    detect.name = "Registry";
    detect.score = 20;
    detect.detected = false;

    if (KeyExists(HKEY_LOCAL_MACHINE, L"Software\\Wow6432Node\\Wine")) {
        detect.detected = true;
        // return detect;
    }
    if (KeyExists(HKEY_CURRENT_USER, L"Software\\Wine")) {
        detect.detected = true;
        // return detect;
    }
    this->totalScore += detect.score;
    this->score += detect.detected ? detect.score : 0;
    return detect;
}

Detect Detector::drivesTest() {
    Detect detect;
    detect.name = "Drives";
    detect.score = 10;
    detect.detected = false;
    DWORD bufferSize = 1024; // Adjust as needed
    WCHAR buffer[1024];

    // Retrieve the drive strings
    DWORD result = GetLogicalDriveStringsW(bufferSize, buffer);

    if (result == 0) {
        std::cerr << "Failed to get logical drive strings. Error code: " << GetLastError() << std::endl;
        return detect;
    }

    // Print all drive strings
    WCHAR* drive = buffer;
    while (*drive) {
        // std::wcout << L"Drive: " << drive << std::endl;
        std::wstring driveStr(drive);
        if (driveStr[0] == L'Z') {
            std::wstring path = driveStr + L"bin";

            if (dirExists(path)) {
                detect.detected = true;
                break;
            }
        }
        drive += wcslen(drive) + 1;
    }
    this->totalScore += detect.score;
    this->score += detect.detected ? detect.score : 0;
    return detect;
}

Detect Detector::servicesTest() {
    Detect detect;
    detect.name = "Services";
    detect.score = 5;
    detect.detected = ServiceExists(L"Winedevice1");
    detect.score = 20;
    detect.detected = ServiceExists(L"Winedevice1");
    this->totalScore += detect.score;
    this->score += detect.detected ? detect.score : 0;
    return detect;
}

Detect Detector::dllExportTest() {
    // https://www.reddit.com/r/linux_gaming/comments/1f2jsgy/comment/lkajyvh/
    Detect detect;
    detect.name = "DLL Exports";
    detect.score = 30;
    detect.detected = false;

    std::vector<std::string> exports;

    ListDLLFunctions("ntdll.dll", exports);

    if (std::find(exports.begin(), exports.end(), "wine_get_version") != exports.end()) {
        detect.detected = true;
    }
    if (std::find(exports.begin(), exports.end(), "wine_get_host_version") != exports.end()) {
        detect.detected = true;
    }
    this->totalScore += detect.score;
    this->score += detect.detected ? detect.score : 0;
    return detect;
}

Detect Detector::muldivTest() {
    // https://www.reddit.com/r/linux_gaming/comments/1f2jsgy/comment/lkajyvh/ but it doesn't work
    Detect detect;
    detect.name = "MulDiv";
    detect.score = 20;
    detect.detected = MulDiv(1, 0x80000000, 0x80000000) != 2;
    // std::cout << MulDiv(1, 0x80000000, 0x80000000);
    this->totalScore += detect.score;
    this->score += detect.detected ? detect.score : 0;
    return detect;
}

Detect Detector::processTest() {
    Detect detect;
    detect.name = "Process";
    detect.score = 1;
    detect.detected = ProcessExists(L"winedevice.exe");
    this->totalScore += detect.score;
    this->score += detect.detected ? detect.score : 0;
    return detect;
}

Detect Detector::filesTest() {
    Detect detect;
    detect.name = "Files";
    detect.score = 1;
    detect.detected = FileExists(L"C:\\windows\\syswow64\\wineboot.exe");
    this->totalScore += detect.score;
    this->score += detect.detected ? detect.score : 0;
    return detect;
}