<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: GiboSMS - Win32 (WCE ARM) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP163.tmp" with contents
[
/nologo /W3 /D _WIN32_WCE=300 /D "WIN32_PLATFORM_WFSP=100" /D "ARM" /D "_ARM_" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /Fp"ARMRel/GiboSMS.pch" /YX /Fo"ARMRel/" /Oxs /MC /c 
"C:\Prog C++\Smartphone\GiboSMS\GiboSMS.cpp"
]
Creating command line "clarm.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP163.tmp" 
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP164.tmp" with contents
[
commctrl.lib coredll.lib aygshell.lib winsock.lib ole32.lib CellCore.lib /nologo /base:"0x00010000" /stack:0x10000,0x1000 /entry:"WinMainCRTStartup" /incremental:no /pdb:"ARMRel/GiboSMS.pdb" /nodefaultlib:"libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib /nodefaultlib:oldnames.lib" /out:"ARMRel/GiboSMS.exe" /subsystem:windowsce,3.00 /align:"4096" /MACHINE:ARM 
".\ARMRel\GiboSMS.obj"
".\ARMRel\Fce.obj"
".\ARMRel\Download.obj"
".\ARMRel\CHdc.obj"
".\ARMRel\CSVWalker.obj"
".\ARMRel\SetCharPtr.obj"
".\ARMRel\Setting.obj"
".\ARMRel\CPhonebook.obj"
".\ARMRel\XPhonebook.obj"
".\ARMRel\CFont.obj"
".\ARMRel\GiboSMS.res"
]
Creating command line "link.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP164.tmp"
<h3>Output Window</h3>
Compiling...
GiboSMS.cpp
Linking...
Signing C:\Prog C++\Smartphone\GiboSMS\ARMRel\GiboSMS.exe
Warning: This file is signed, but not timestamped.
Succeeded




<h3>Results</h3>
GiboSMS.exe - 0 error(s), 0 warning(s)
</pre>
</body>
</html>
