::x64 native tools command prompt ‚©‚ç‚â‚ç‚È‚¢‚Æ,dumpbin /EXPORTS libŠm”F‚ÅŠÖ”‚Ìæ“ª‚É _ ‚ª‚Â‚¢‚Ä(_lua_open)ƒŠƒ“ƒN‚Å‚«‚È‚©‚Á‚½
if defined VCINSTALLDIR (
call "%VCINSTALLDIR%\vcvarsall.bat" x64)
cl /MD /O2 /c /DLUA_BUILD_AS_DLL *.c
ren lua.obj lua.o
ren luac.obj luac.o
link /DLL /IMPLIB:lua5.4.0.lib /OUT:lua5.4.0.dll *.obj 
link /OUT:lua.exe lua.o lua5.4.0.lib 
lib /OUT:lua5.4.0-static.lib *.obj
link /OUT:luac.exe luac.o lua5.4.0-static.lib
del *.obj *.o
