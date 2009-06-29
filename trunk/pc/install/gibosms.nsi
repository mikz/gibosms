!define PRODUCT_NAME "Gibo SMS"
!define PRODUCT_VERSION "1.03"

!include "MUI.nsh"

;--------------------------------
;Configuration
	;General
	ShowInstDetails show
	ShowUninstDetails show
	XPStyle on

	InstType "Kompletní"
	InstType "Minimální"

  Name "${PRODUCT_NAME}"
	OutFile "GiboSMS_PC_${PRODUCT_VERSION}.exe"

	;The default installation directory
	InstallDir "$PROGRAMFILES\Gibo SMS"

	;;;!define MUI_TEXT_FINISH_RUN "Spustit Gibo SMS"

	;Set installer and uninstaller icon
	;Icon "..\res\GiboSMS.ico"
	;UninstallIcon "${NSISDIR}\Contrib\Graphics\Icons\nsis1-uninstall.ico"
	;!define MUI_ICON "..\res\GiboSMS.ico"
	;!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\arrow-uninstall.ico"
	
  !define TEMP $R0

;--------------------------------
;Variables

  Var MUI_TEMP
  Var STARTMENU_FOLDER

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY

  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\${PRODUCT_NAME}" 
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
  
  !insertmacro MUI_PAGE_STARTMENU Application $STARTMENU_FOLDER

  !insertmacro MUI_PAGE_INSTFILES

	!define MUI_FINISHPAGE
	!define MUI_FINISHPAGE_RUN "$INSTDIR\GiboSMS.exe"
	!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\nápovìda.html"
	!define MUI_ABORTWARNING

  !insertmacro MUI_PAGE_FINISH

  ;Uninstall         
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "Czech"

;--------------------------------
;Reserve Files

	;Things that need to be extracted on first (keep these lines before any File command!)
	;Only useful for BZIP2 compression

	ReserveFile "${NSISDIR}\Contrib\Graphics\Header\win.bmp"


;--------------------------------
; set sections
Section "GiboSMS.exe" SecMainProgram
	SectionIn RO

	;include required files
	SetOutPath "$INSTDIR"
	File "..\Release\GiboSMS.exe"
	File "..\res\nápovìda.html"
	
	;Store install folder
	WriteRegStr HKCU "Software\${PRODUCT_NAME}" "" $INSTDIR
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "DisplayName" "${PRODUCT_NAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "UninstallString" "$INSTDIR\Plugins\${PRODUCT_NAME}\Uninstall.exe"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "DisplayIcon" "$INSTDIR\Plugins\${PRODUCT_NAME}\Uninstall.exe"
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "NoModify" 1
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "NoRepair" 1

	!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
		;Create shortcuts
		CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
		CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Gibo SMS.lnk" "$INSTDIR\GiboSMS.exe"
		CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Nápovìda.lnk" "$INSTDIR\nápovìda.html"
		CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Odinstalovat.lnk" "$INSTDIR\Uninstall.exe"
	!insertmacro MUI_STARTMENU_WRITE_END

	;Create uninstaller
	WriteUninstaller "$INSTDIR\Uninstall.exe"
	AddSize 60 ; !!!!!!!!!!!! SIZE OF Uninstall.exe !!!!!!!!!!!!
SectionEnd

SubSection "Skiny" SecSkins
	Section "MDA Vario" SecSkinsMDAVario
		SectionIn 1

		;include required files
		SetOutPath "$INSTDIR\skins\MDA Vario"
		File "..\skins\MDA Vario\background.bmp"
		File "..\skins\MDA Vario\layout.ini"
	SectionEnd

	Section "Nokia E65" SecSkinsNokiaE65
		SectionIn 1

		;include required files
		SetOutPath "$INSTDIR\skins\Nokia E65"
		File "..\skins\Nokia E65\background.bmp"
		File "..\skins\Nokia E65\layout.ini"
	SectionEnd

	Section "Simple" SecSkinsSimple
		SectionIn 1

		;include required files
		SetOutPath "$INSTDIR\skins\Simple"
		File "..\skins\Simple\background.bmp"
		File "..\skins\Simple\layout.ini"
	SectionEnd

	Section "Simple - transparent" SecSkinsSimpleTransparent
		SectionIn 1

		;include required files
		SetOutPath "$INSTDIR\skins\Simple - transparent"
		File "..\skins\Simple - transparent\background.bmp"
		File "..\skins\Simple - transparent\layout.ini"
	SectionEnd
SubSectionEnd


;--------------------------------
;Descriptions

	;Description
	LangString DESC_SecMainProgram ${LANG_CZECH} "Program Gibo SMS"
	LangString DESC_SecSkins ${LANG_CZECH} "Volitelné vzhledy programu"
	LangString DESC_SecSkinsMDAVario ${LANG_CZECH} "Skin MDA Vario"
	LangString DESC_SecSkinsSimple ${LANG_CZECH} "Skin Simple"
	LangString DESC_SecSkinsSimpleTransparent ${LANG_CZECH} "Skin Simple - transparent"

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	 !insertmacro MUI_DESCRIPTION_TEXT ${SecMainProgram} $(DESC_SecMainProgram)
	 !insertmacro MUI_DESCRIPTION_TEXT ${SecSkins} $(DESC_SecSkins)
	 !insertmacro MUI_DESCRIPTION_TEXT ${SecSkinsMDAVario} $(DESC_SecSkinsMDAVario)
	 !insertmacro MUI_DESCRIPTION_TEXT ${SecSkinsSimple} $(DESC_SecSkinsSimple)
	 !insertmacro MUI_DESCRIPTION_TEXT ${SecSkinsSimpleTransparent} $(DESC_SecSkinsSimpleTransparent)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END


;--------------------------------
;Uninstaller Section

Section "Uninstall"

	ReadRegStr ${TEMP} HKCU "Software\${PRODUCT_NAME}" ""
	StrCmp ${TEMP} "" skip1
		StrCpy $INSTDIR ${TEMP}
	skip1:
	
	Delete "$INSTDIR\skins\MDA Vario\background.bmp"
	Delete "$INSTDIR\skins\MDA Vario\layout.ini"
	RMDir "$INSTDIR\skins\MDA Vario\"
	Delete "$INSTDIR\skins\Simple\background.bmp"
	Delete "$INSTDIR\skins\Simple\layout.ini"
	RMDir "$INSTDIR\skins\Simple\"
	Delete "$INSTDIR\skins\Simple - transparent\background.bmp"
	Delete "$INSTDIR\skins\Simple - transparent\layout.ini"
	RMDir "$INSTDIR\skins\Simple - transparent\"

	RMDir "$INSTDIR\skins"
	
	Delete "$INSTDIR\GiboSMS.exe"
	Delete "$INSTDIR\nápovìda.html"
	Delete "$INSTDIR\Uninstall.exe"
	Delete "$INSTDIR\setting"
	Delete "$INSTDIR\phonebook.csv"
	RMDir "$INSTDIR"

  !insertmacro MUI_STARTMENU_GETFOLDER Application $MUI_TEMP

  Delete "$SMPROGRAMS\$MUI_TEMP\Odinstalovat.lnk"

  Delete "$SMPROGRAMS\$MUI_TEMP\Gibo SMS.lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\nápovìda.lnk"
  RMDir "$SMPROGRAMS\$MUI_TEMP"

  DeleteRegKey /ifempty HKCU "Software\${PRODUCT_NAME}"

	;Remove shortcut
	;;;ReadRegStr ${TEMP} "${MUI_STARTMENUPAGE_REGISTRY_ROOT}" "${MUI_STARTMENUPAGE_REGISTRY_KEY}" "${MUI_STARTMENUPAGE_REGISTRY_VALUENAME}"

	;;;StrCmp ${TEMP} "" "" skip2
		;;;StrCpy ${TEMP} "${PRODUCT_NAME}"
	;;;skip2:
		;;;Delete "$SMPROGRAMS\${TEMP}\Readme.lnk"
		;;;Delete "$SMPROGRAMS\${TEMP}\Uninstall.lnk"
		;;;Delete "$SMPROGRAMS\${TEMP}\Example song.lnk"
		;;;RMDir "$SMPROGRAMS\${TEMP}" ;Only if empty, so it won't delete other shortcuts

	;;;DeleteRegKey /ifempty HKCU "Software\${PRODUCT_NAME}"
	;;;DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"

	;Display the Finish header
	;;;!insertmacro MUI_UNFINISHHEADER
SectionEnd

;------ FUNCTIONS ------------
Function .onInit
	FindWindow ${TEMP} "Gibo SMS"
	IntCmp ${TEMP} 0 nothing
	SendMessage ${TEMP} ${WM_COMMAND} 32771 0
	nothing:
FunctionEnd
