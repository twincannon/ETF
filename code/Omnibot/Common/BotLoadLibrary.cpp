#include "BotExports.h"

#if defined WIN32 || defined _WINDOWS || defined _WIN32

#define WIN32 1

#elif defined __linux__ || ((defined __MACH__) && (defined __APPLE__))

#define _stricmp strcasecmp

#else

#error "Unsupported Platform or Missing platform #defines";

#endif


void Omnibot_strncpy(char *dest, const char *source, int count)
{
	// Only doing this because some engines(HL2), think it a good idea to fuck up the 
	// defines of all basic string functions throughout the entire project.
	while (count && (*dest++ = *source++)!=0) /* copy string */
		count--;

	if (count) /* pad out with zeroes */
		while (--count)
			*dest++ = '\0';
}

////////////////////////////////////////////////////////////////////////////////

#ifdef GAMEDLL
//this is compiled into qagame_mp_x86.dll or qagame.mp.i386.so

#ifdef _WIN32
#pragma warning(disable:4530) //C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc
#pragma warning(disable:4706) //assignment within conditional expression
#endif // _WIN32

#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>

//////////////////////////////////////////////////////////////////////////

#ifndef CHECK_PRINTF_ARGS
#define CHECK_PRINTF_ARGS
#define CHECK_PARAM_VALID
#define CHECK_VALID_BYTES(parm)
#endif

//////////////////////////////////////////////////////////////////////////

bool					g_IsOmnibotLoaded = false;
Bot_EngineFuncs_t		g_BotFunctions = {0};
IEngineInterface		*g_InterfaceFunctions = 0;
std::string				g_OmnibotLibPath;

void Omnibot_Load_PrintMsg(const char *_msg);
void Omnibot_Load_PrintErr(const char *_msg);

bool IsOmnibotLoaded()
{
	return g_IsOmnibotLoaded;
}

const char *Omnibot_GetLibraryPath()
{
	return g_OmnibotLibPath.c_str();
}

//////////////////////////////////////////////////////////////////////////

static const char *BOTERRORS[BOT_NUM_ERRORS] = 
{
	"None",
	"Bot Library not found",
	"Unable to get Bot Functions from DLL",
	"Error Initializing the Bot",
	"Invalid Interface Functions",
	"Wrong Version",
	"Error Initializing File System",
};

const char *Omnibot_ErrorString(eomnibot_error err)
{
	return ((err >= BOT_ERROR_NONE) && (err < BOT_NUM_ERRORS)) ? BOTERRORS[err] : "";
}

const char *Omnibot_FixPath(const char *_path)
{
	const int iBufferSize = 512;
	static char pathstr[iBufferSize] = {0};
	Omnibot_strncpy(pathstr, _path, iBufferSize);
	pathstr[iBufferSize-1] = '\0';

	// unixify the path slashes
	char *pC = pathstr;
	while(*pC != '\0')
	{
		if(*pC == '\\')
			*pC = '/';
		++pC;
	}

	// trim any trailing slash
	while(*pC == '/' && pC > pathstr)
	{
		*pC = '\0';
		--pC;
	}
	return pathstr;
}

//////////////////////////////////////////////////////////////////////////

#ifdef WIN32
#define NOWINRES
#define NOSERVICE
#define NOMCX
#define NOIME
#include <stdio.h>
#include <windows.h>

#else

#include <stdarg.h>
#include <dlfcn.h>

#define GetProcAddress dlsym
#define FreeLibrary dlclose
#define HINSTANCE void*
#endif

//////////////////////////////////////////////////////////////////////////
// Utilities

const char *OB_VA( const char* _msg, ...)
{
	static int iCurrentBuffer = 0;
	const int iNumBuffers = 3;
	const int BUF_SIZE = 1024;
	struct BufferInstance
	{
		char buffer[BUF_SIZE];
	};
	static BufferInstance buffers[iNumBuffers];

	char *pNextBuffer = buffers[iCurrentBuffer].buffer;

	va_list list;
	va_start(list, _msg);
	vsnprintf(pNextBuffer, BUF_SIZE, _msg, list);
	pNextBuffer[BUF_SIZE - 1] = 0;
	va_end(list);

	iCurrentBuffer = (iCurrentBuffer+1)%iNumBuffers;
	return pNextBuffer;
}

//////////////////////////////////////////////////////////////////////////	
HINSTANCE g_BotLibrary = 0;

void OB_ShowLastError(const char *context)
{
#ifdef WIN32
	char *pMessage = 0;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		0,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&pMessage,
		0, 0 );

	if(pMessage)
	{
		// Strip Newlines
		int i = (int)strlen(pMessage)-1;
		while(pMessage[i] == '\n' || pMessage[i] == '\r')
			pMessage[i--] = 0;
	}

#else
	const char *pMessage = dlerror();
#endif

	if(!pMessage) pMessage = "<unknown error>";
	Omnibot_Load_PrintErr(OB_VA("%s Failed with Error: %s", context, pMessage));

#ifdef WIN32
	LocalFree(pMessage);
#endif
}

HINSTANCE Omnibot_LL(const char *file)
{
	//////////////////////////////////////////////////////////////////////////
	// Parse Variables
	// $(ProgramFiles)
	// $(OMNIBOT)

	//////////////////////////////////////////////////////////////////////////
	g_OmnibotLibPath = file;
#ifdef WIN32
	HINSTANCE hndl = LoadLibrary(g_OmnibotLibPath.c_str());
#else
	void *hndl = dlopen(g_OmnibotLibPath.c_str(), RTLD_NOW);
#endif
	if(!hndl)
		OB_ShowLastError("LoadLibrary");
	Omnibot_Load_PrintMsg(OB_VA("Looking for %s, %s", g_OmnibotLibPath.c_str(), hndl ? "found." : "not found"));
	return hndl;
}

eomnibot_error Omnibot_LoadLibrary(int version, const char *lib, const char *path)
{
	eomnibot_error r = BOT_ERROR_NONE;

#ifdef WIN32
#ifdef _WIN64
#define SUFFIX "_x64"
#else
#define SUFFIX
#endif
	g_BotLibrary = Omnibot_LL( OB_VA("%s\\%s" SUFFIX ".dll", path ? path : ".", lib) );
	if(g_BotLibrary == 0)
		g_BotLibrary = Omnibot_LL(OB_VA(".\\omni-bot\\%s" SUFFIX ".dll", lib));
	if(g_BotLibrary == 0)
		g_BotLibrary = Omnibot_LL(OB_VA("%s" SUFFIX ".dll", lib));

#else
#ifdef __APPLE__
#define SUFFIX "_mac"
#else
#ifdef __x86_64__
#define SUFFIX ".x86_64"
#elif defined __aarch64__
#define SUFFIX ".aarch64"
#else	
#define SUFFIX
#endif
#endif
	g_BotLibrary = Omnibot_LL(OB_VA("%s/%s" SUFFIX ".so", path ? path : ".", lib));
	if(!g_BotLibrary)
		g_BotLibrary = Omnibot_LL(OB_VA("./%s" SUFFIX ".so", lib));
	if(!g_BotLibrary)
	{
		char *homeDir = getenv("HOME");
		if(homeDir)
			g_BotLibrary = Omnibot_LL(OB_VA("%s/omni-bot/%s" SUFFIX ".so", homeDir, lib));
	}
	if(!g_BotLibrary)
		g_BotLibrary = Omnibot_LL(OB_VA("%s" SUFFIX ".so", lib));
#endif

	if(g_BotLibrary == 0)
	{
		g_OmnibotLibPath.clear();
		r = BOT_ERROR_CANTLOADDLL;
	}
	else
	{
		//Omnibot_Load_PrintMsg(OB_VA("Found Omni-bot: %s, Attempting to Initialize", g_OmnibotLibPath.c_str()));
		pfnGetFunctionsFromDLL pfnGetBotFuncs = 0;
		memset(&g_BotFunctions, 0, sizeof(g_BotFunctions));
		pfnGetBotFuncs = (pfnGetFunctionsFromDLL)GetProcAddress(g_BotLibrary, "ExportBotFunctionsFromDLL");
		if(pfnGetBotFuncs == 0)
		{
			r = BOT_ERROR_CANTGETBOTFUNCTIONS;
			OB_ShowLastError("GetProcAddress");
		} 
		else
		{
			r = pfnGetBotFuncs(&g_BotFunctions, sizeof(g_BotFunctions));
			if(r == BOT_ERROR_NONE)
			{
				r = g_BotFunctions.pfnInitialize(g_InterfaceFunctions, version);
			}
		}
		g_IsOmnibotLoaded = (r == BOT_ERROR_NONE);
		if(g_IsOmnibotLoaded)
		{
			//Omnibot_Load_PrintMsg("Omni-bot Loaded Successfully");
		}
		else
		{
			Omnibot_Load_PrintErr(OB_VA("Omni-bot Failed with Error: %s", Omnibot_ErrorString(r)));
			Omnibot_FreeLibrary();
		}
	}
	return r;
}

void Omnibot_FreeLibrary()
{
	if(g_BotLibrary)
	{
		FreeLibrary(g_BotLibrary);
		g_BotLibrary = 0;
	}
	memset(&g_BotFunctions, 0, sizeof(g_BotFunctions));

	delete g_InterfaceFunctions;
	g_InterfaceFunctions = 0;

	g_IsOmnibotLoaded = false;
}


#endif // GAMEDLL

//////////////////////////////////////////////////////////////////////////
KeyVals::KeyVals()
{
	Reset();
}
void KeyVals::Reset()
{
	memset(m_Key,0,sizeof(m_Key));
	memset(m_String,0,sizeof(m_String));
	memset(m_Value,0,sizeof(m_Value));
}
bool KeyVals::SetInt(const char *_key, int _val) 
{
	return SetKeyVal(_key,obUserData(_val)); 
}
bool KeyVals::SetFloat(const char *_key, float _val)
{
	return SetKeyVal(_key,obUserData(_val)); 
}
bool KeyVals::SetEntity(const char *_key, GameEntity _val) 
{
	return SetKeyVal(_key,obUserData(_val));
}
bool KeyVals::SetVector(const char *_key, float _x,float _y,float _z) 
{
	return SetKeyVal(_key,obUserData(_x,_y,_z)); 
}
bool KeyVals::SetVector(const char *_key, const float *_v)
{
	return SetKeyVal(_key,obUserData(_v[0],_v[1],_v[2])); 
}
bool KeyVals::SetString(const char *_key, const char *_value)
{
	_value = _value?_value:"";

	for(int a = 0; a < MaxArgs; ++a)
	{
		// look for the first null string
		if(m_String[a][0] == '\0')
		{
			Omnibot_strncpy(&m_String[a][0],_value,MaxStringLength-1);
			return SetKeyVal(_key,obUserData(&m_String[a][0]));
		}
	}
	assert(false);
	return false;
}
bool KeyVals::Set(const char *_key, const obUserData &_value)
{
	return SetKeyVal(_key,_value);
}
bool KeyVals::SetKeyVal(const char *_key, const obUserData &_ud)
{
	if(!_key)
		return false;

	int ifree = -1;
	for(int i = 0; i < MaxArgs; ++i)
	{
		if(ifree == -1 && m_Key[i][0]==0)
			ifree = i;
		if(!_stricmp(m_Key[i], _key))
		{
			m_Value[i] = _ud;
			return true;
		}
	}
	if(ifree != -1)
	{
		Omnibot_strncpy(&m_Key[ifree][0],_key,MaxArgLength-1);
		m_Value[ifree] = _ud;
		return true;
	}
	return false;
}

bool KeyVals::GetInt(const char *_key, int &_val) const
{
	obUserData d;
	if(GetKeyVal(_key,d))
	{
		_val = d.GetInt();
		return true;
	}
	return false;
}
bool KeyVals::GetFloat(const char *_key, float &_val) const
{
	obUserData d;
	if(GetKeyVal(_key,d))
	{
		_val = d.GetFloat();
		return true;
	}
	return false;
}
bool KeyVals::GetEntity(const char *_key, GameEntity &_val) const
{
	obUserData d;
	if(GetKeyVal(_key,d))
	{
		_val = d.GetEntity();
		return true;
	}
	return false;
}
bool KeyVals::GetVector(const char *_key, float &_x,float &_y,float &_z) const
{
	obUserData d;
	if(GetKeyVal(_key,d))
	{
		_x = d.GetVector()[0];
		_y = d.GetVector()[1];
		_z = d.GetVector()[2];
		return true;
	}
	return false;
}
bool KeyVals::GetVector(const char *_key, float *_v) const
{
	obUserData d;
	if(GetKeyVal(_key,d))
	{
		_v[0] = d.GetVector()[0];
		_v[1] = d.GetVector()[1];
		_v[2] = d.GetVector()[2];
		return true;
	}
	return false;
}
bool KeyVals::GetString(const char *_key, const char *&_val) const
{
	obUserData d;
	if(GetKeyVal(_key,d))
	{
		_val = d.GetString();
		return true;
	}
	return false;
}
bool KeyVals::GetKeyVal(const char *_key, obUserData &_ud) const
{
	for(int i = 0; i < MaxArgs; ++i)
	{
		if(!_stricmp(m_Key[i], _key))
		{
			_ud = m_Value[i];
			return true;
		}
	}
	return false;
}
void KeyVals::GetKV(int _index, const char *&_key, obUserData &ud) const
{
	_key = m_Key[_index];
	ud = m_Value[_index];
}
