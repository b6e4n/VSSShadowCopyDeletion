# Revisiting of Conti shadow copies deletion 
After viewing a Conti ransomware code video explanation, I was wondering why ransomware group were still using cmd process to perform the actual deletion of shadow copies as it is easily detect by EDR or AV solutions via command line arguments.
So I wanted to know if it was possible to implement it in actual C++ and this Microsoft repository : https://github.com/microsoft/Windows-classic-samples (from which I largely took the code) shew me that yes, and this solution seems higly easier to write in comparison to their.
Ransomware devs don't look creative.

## Basic Conti ransomware shadow copy deletion
Located in main/locker/locker.cpp
```cpp
wsprintfW(CmdLine, OBFW(L"cmd.exe /c C:\\Windows\\System32\\wbem\\WMIC.exe shadowcopy where \"ID='%s'\" delete"), vtProp.bstrVal);
```

## Microsoft Repo inspiration
- Deletion of all Shadow Copies skeleton : https://github.com/microsoft/Windows-classic-samples/blob/main/Samples/Win7Samples/winbase/vss/vshadow/delete.cpp
- Initialization of the COM IVssEnumObject interface : https://github.com/microsoft/Windows-classic-samples/blob/main/Samples/Win7Samples/winbase/vss/vshadow/vssclient.cpp and https://learn.microsoft.com/en-us/cpp/cpp/how-to-create-and-use-ccomptr-and-ccomqiptr-instances?view=msvc-170
- VSS Interface Windows documentation :
  - https://learn.microsoft.com/en-us/windows/win32/learnwin32/what-is-a-com-interface-
  - https://learn.microsoft.com/en-us/windows/win32/api/vsbackup/nl-vsbackup-ivssbackupcomponents#methods
  - https://learn.microsoft.com/en-us/windows/win32/api/vss/ne-vss-vss_snapshot_context
  - https://learn.microsoft.com/en-us/windows/win32/api/vsbackup/nf-vsbackup-ivssbackupcomponents-query
  - https://learn.microsoft.com/en-us/windows/win32/api/vsbackup/nf-vsbackup-vssfreesnapshotproperties


## Conti ransomware code video review
Great malware reviews by Rad9800
https://youtu.be/SGbhqwXB-GU?si=ufk1UwYrmS-cOxJ2
