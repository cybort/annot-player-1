:: deploy-win.cmd
:: 11/5/2011
setlocal
cd /d d:/dev/build || exit /b 1

set VERSION=0.1.6.3
set APP=annot-player
set TARGET=Annot Stream
set ZIPFILE=%APP%-%VERSION%-win.zip

::set BASENAME=%APP%-%VERSION%-win
::rm -Rf "%BASENAME%" || exit 1
::mkdir "%BASENAME%" || exit 1
::cd "%BASENAME%" || exit 1

set MSVC_HOME=/Volumes/win/Windows/System32
set MSVC_DLLS=msvcp100.dll,msvcr100.dll

set MSVC90_HOME=/Volumes/win/Program Files/Microsoft Visual Studio 9.0
set MSVC90_REDIST=%MSVC90_HOME%/VC/redist/x86/Microsoft.VC90.CRT

set QT_HOME=/Volumes/win/qt/current
set QT_DLLS=QtCore4.dll,QtDeclarative4.dll,QtGui4.dll,QtNetwork4.dll,QtScript4.dll,QtSql4.dll,QtSvg4.dll,QtWebkit4.dll,QtXml4.dll,QtXmlPatterns4.dll,phonon4.dll
set QT_TRANSLATIONS=qt_ja.qm,qt_zh_CN.qm,qt_zh_TW.qm
set QT_PLUGINS=accessible bearer codecs graphicssystems iconengines imageformats sqldrivers

set ITH_HOME=/Volumes/win/dev/ith
::set ITH_DLLS=ITH.dll,ITH_engine.dll
set ITH_DLLS=IHF.dll,IHF_DLL.dll,ITH_Engine.dll

set OPENSSL_HOME=/Volumes/win/dev/openssl/1.0.0j
set OPENSSL_DLLS=libeay32.dll,ssleay32.dll

::set GPAC_HOME=/Volumes/win/dev/gpac
::set GPAC_DLLS=js32.dll,libgpac.dll,libeay32.dll,ssleay32.dll

::set MP4BOX_HOME=%GPAC_HOME%
::set MP4BOX_EXE=MP4Box.exe

::set MP4V2_HOME=/Volumes/win/dev/mp4v2
::set MP4V2_DLL=libmp4v2.dll

::set LUA_HOME=/Volumes/win/dev/lua
::set LUA_DLL=lua52.dll

set ZLIB_HOME=/Volumes/win/dev/zlib
set ZLIB_DLL=zlib1.dll

set VLC_HOME=/Volumes/win/Program Files/VideoLAN/VLC
set VLC_DLLS=libvlc.dll,libvlccore.dll
set VLC_DATA=plugins,lua,locale

set BUILD=/Volumes/local/dev/annot-build-desktop/build.win
set SOURCE=/Volumes/local/dev/annot

::set CURL_HOME=/Volumes/win/dev/curl
::set CURL_BIN=curl.exe

::set GZIP_HOME=/Volumes/win/dev/gzip
::set GZIP_BIN=gzip.exe

:: deploy into app dir

rm -Rf "%TARGET%"
mkdir "%TARGET%"
cd "%TARGET%" || exit /b 1

cp -v "%BUILD%/Annot Browser.exe" . || exit /b 1
cp -v "%BUILD%/Annot Downloader.exe" . || exit /b 1
cp -v "%BUILD%/Annot Player.exe" . || exit /b 1

cp -v "%SOURCE%/README" "Read Me.txt" || exit /b 1
unix2dos "Read Me.txt"

cp -v "%SOURCE%/ChangeLog" ChangeLog.txt || exit /b 1
unix2dos ChangeLog.txt

rm -Rf licenses
cp -R "%SOURCE%/licenses" Licenses
cp "%SOURCE%/COPYING" COPYING.txt || exit /b 1
unix2dos COPYING.txt

:: deploy modules
mkdir Data
cd Data || exit /b 1

mkdir translations
cp -v "%QT_HOME%"/translations/{%QT_TRANSLATIONS%} translations/ || exit /b 1

for %%i in (%QT_PLUGINS%) do (
  mkdir %%i
  cp -v "%QT_HOME%"/plugins/%%i/*4.dll %%i/ || exit /b 1
  rm -f %%i/*d4.dll
)

rm -f sqldrivers/{qsqlodbc4.dll,qsqlpsql4.dll}
::rm -f graphicssystems/qglgraphicssystem4.dll

cp -v "%QT_HOME%"/bin/{%QT_DLLS%} . || exit /b 1

cp -v "%MSVC_HOME%"/{%MSVC_DLLS%} . || exit /b 1
cp -Rv "%MSVC90_REDIST%" . || exit /b 1
cp -v "%ITH_HOME%"/bin/{%ITH_DLLS%} . || exit /b 1
cp -v "%OPENSSL_HOME%"/{%OPENSSL_DLLS%} . || exit /b 1
::cp -v "%GPAC_HOME%"/bin/{%GPAC_DLLS%} . || exit /b 1
::cp -v "%MP4BOX_HOME%"/bin/%MP4BOX_EXE% . || exit /b 1
::cp -v "%MP4V2_HOME%"/bin/%MP4V2_DLL% . || exit /b 1
::cp -v "%LUA_HOME%"/bin/%LUA_DLL% . || exit /b 1
cp -v "%ZLIB_HOME%"/bin/%ZLIB_DLL% . || exit /b 1

::cp -v "%CURL_HOME%"/bin/%CURL_BIN% . || exit /b 1
::cp -v "%GZIP_HOME%"/bin/%GZIP_BIN% . || exit /b 1

cp -Rv "%VLC_HOME%"/{%VLC_DATA%} . || exit /b 1

::rm -Rfv plugins || exit /b 1
::mkdir plugins || exit /b 1
::cp -Rv "%VLC_HOME%"/plugins/*/*.dll plugins/

rm -fv plugins/*.dat*

::cp -v "%BUILD%"/*.{exe,dll} . || exit /b 1
cp -v "%BUILD%"/*.{exe,dll} .

rm -fv "Annot Player.exe" "Annot Browser.exe" "Annot Downloader.exe"
rm -fv "annot-tester.exe"
rm -fv hook.dll
rm -fv webbrowser.dll

:: compile lua

::luac -s -o luascript.luac "%SOURCE%"/module/mrlresolver/lua/luascript.lua  || exit 1
::cp -Rv "%SOURCE%"/module/mrlresolver/lua/luascript .  || exit 1
::pushd luascript || exit 1
::for %%d in (lib sitelist .) do (
::  pushd %%d || exit 1
::  for %%i in (*.lua) do luac -s -o %%ic %%i && rm -f %%i
::  popd
::)
::popd
set LUA_PATH=lua/luascript
mkdir "%LUA_PATH%" || exit 1
cp "%SOURCE%"/module/luaresolver/lua/luascript.lua "%LUA_PATH%"/ || exit 1
cp -v "%SOURCE%"/module/luaresolver/lua/luascript/*.lua "%LUA_PATH%"/  || exit 1
cp -v "%SOURCE%"/module/luaresolver/lua/luascript/*/*.lua "%LUA_PATH%"/  || exit 1

:: doc
cp -Rv "%SOURCE%"/module/qtext/doc . || exit 1
cp -Rv "%SOURCE%"/module/qtext/images . || exit 1

:: jsf
cp -Rv "%SOURCE%"/module/annotcloud/jsf . || exit 1

:: images
cp -Rv "%SOURCE%"/project/player/avatars . || exit 1

cd ..

:: desktop.ini
cp -v "%SOURCE%"/project/common/share/apps.ico icon.ico || exit 1
cp -v "%SOURCE%"/project/common/share/desktop.ini.txt desktop.ini || exit 1

:: repair permissions

chown -R jichi .
chmod -R 755 .

attrib +h Data
attrib +h Licenses

:: See: http://msdn.microsoft.com/en-us/library/windows/desktop/cc144102(v=vs.85).aspx
attrib +h icon.ico
attrib +h +s desktop.ini
attrib +r .

:: archive
::call "Delete Caches.cmd"

cd ..

rm -f "%ZIPFILE%"
zip -9r "%ZIPFILE%" "%TARGET%" > nul

:: EOF
