# WineDetector
Detect if user uses wine using various [methods](#methods)

*linux wine output:*\
![image](https://github.com/user-attachments/assets/a7c79718-ce42-44d2-a274-b4174d35b063)



*linux portproton output:*\
![image](https://github.com/user-attachments/assets/5c9d3635-74cf-4235-9633-1b4d66a87011)




*windows output (thanks [Belka](https://github.com/MiniBel04ka)):*\
![image](https://github.com/user-attachments/assets/4cfa880f-96e7-4a07-9d69-9925e03f01dd)\
todo: windows v0.3.0 screenshot


# Methods

### Drives test
If you open wine file manager you can find drive with letter Z: which have your linux fs on it
This checker [checks](https://github.com/strainxx/winedetector/blob/5d591557b91aa5261bd1031c05ebdedbf2e4932b/detector.cpp#L198-L211) if user have drive Z with folder bin on it

### Registry test
Wine has many register keys so [it's easy to detect it](https://github.com/strainxx/winedetector/blob/5d591557b91aa5261bd1031c05ebdedbf2e4932b/detector.cpp#L168-L175)

### Services test
You can [notice](https://github.com/strainxx/winedetector/blob/5d591557b91aa5261bd1031c05ebdedbf2e4932b/detector.cpp#L223) that wine creates services with name WinedeviceX\
![image](https://github.com/user-attachments/assets/a0e92af0-001c-4ebf-b33e-a3f704f9c3cb)

### Process test
Sometimes winedevice.exe can be found on wine (for me it works only in portproton)

### Files test
C:\\windows\\syswow64\\wineboot.exe - [it's all what you need to know](https://github.com/strainxx/winedetector/blob/5d591557b91aa5261bd1031c05ebdedbf2e4932b/detector.cpp#L277)

### DLL Exports Test
Thanks to [shavitush](https://www.reddit.com/r/linux_gaming/comments/1f2jsgy/comment/lkajyvh/) for this information about this

Some system dlls have suspicious exports:

### Legacy Api Test
https://www.hexacorn.com/blog/2016/03/27/detecting-wine-via-internal-and-legacy-apis/

* [ntdll.dll!wine_get_version](https://github.com/strainxx/winedetector/blob/5d591557b91aa5261bd1031c05ebdedbf2e4932b/detector.cpp#L240-L245)
* [ntdll.dll!wine_get_host_version](https://github.com/strainxx/winedetector/blob/5d591557b91aa5261bd1031c05ebdedbf2e4932b/detector.cpp#L240-L245)
* kernel32.dll!wine_get_unix_file_name
* .. and others
