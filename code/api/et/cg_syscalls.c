/*
===========================================================================

Wolfenstein: Enemy Territory GPL Source Code
Copyright (C) 1999-2010 id Software LLC, a ZeniMax Media company.

Enemy Territory Fortress
Copyright (C) 2000-2006 Quake III Fortress (Q3F) Development Team / Splash Damage Ltd.
Copyright (C) 2005-2018 Enemy Territory Fortress Development Team

This file is part of Enemy Territory Fortress (ETF).

ETF is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ETF is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ETF. If not, see <http://www.gnu.org/licenses/>.

In addition, the Wolfenstein: Enemy Territory GPL Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the ETF Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

// cg_syscalls.c

#include "../../cgame/cg_local.h"

static dllSyscall_t syscall = (dllSyscall_t)-1;

Q_EXPORT void dllEntry( dllSyscall_t syscallptr ) {
	syscall = syscallptr;
}

static int PASSFLOAT( float x ) {
	byteAlias_t fi;
	fi.f = x;
	return fi.i;
}

#include "../shared/cg_shared_syscalls.c"

void trap_PumpEventLoop( void ) {
	if( !cgs.initing ) {
		return;
	}
	SystemCall_NoArgs( CG_PUMPEVENTLOOP );
}


void	trap_Print( const char *fmt ) {
	SystemCall( CG_PRINT, fmt );
}

void NORETURN trap_Error( const char *fmt ) {
	SystemCall( CG_ERROR, fmt );
	exit( 1 );
}

int		trap_Milliseconds( void ) {
	return SystemCall_NoArgs( CG_MILLISECONDS );
}

void	trap_Cvar_Register( vmCvar_t *vmCvar, const char *varName, const char *defaultValue, int flags, int extflags ) {
	if ( cvar_notabcomplete && (extflags & EXT_CVAR_NOTABCOMPLETE) ) {
		flags |= cvar_notabcomplete;
	}
	if ( cvar_nodefault && (extflags & EXT_CVAR_NODEFAULT) ) {
		flags |= cvar_nodefault;
	}
	if ( cvar_archive_nd && (extflags & EXT_CVAR_ARCHIVE_ND) ) {
		flags |= cvar_archive_nd;
	}
	else if (!(flags & CVAR_ARCHIVE) && (extflags & EXT_CVAR_ARCHIVE_ND)) {
		// unsupported archive_nd so still add archive
		flags |= CVAR_ARCHIVE;
	}
	if ( cvar_developer && (extflags & EXT_CVAR_DEVELOPER) ) {
		flags |= cvar_developer;
	}

	SystemCall( CG_CVAR_REGISTER, vmCvar, varName, defaultValue, flags );
}

void	trap_Cvar_Update( vmCvar_t *vmCvar ) {
	SystemCall( CG_CVAR_UPDATE, vmCvar );
}

void	trap_Cvar_Set( const char *var_name, const char *value ) {
	SystemCall( CG_CVAR_SET, var_name, value );
}

void trap_Cvar_VariableStringBuffer( const char *var_name, char *buffer, int bufsize ) {
	SystemCall( CG_CVAR_VARIABLESTRINGBUFFER, var_name, buffer, bufsize );
}

void trap_Cvar_LatchedVariableStringBuffer( const char *var_name, char *buffer, int bufsize ) {
	SystemCall( CG_CVAR_LATCHEDVARIABLESTRINGBUFFER, var_name, buffer, bufsize );
}

int	trap_Argc( void ) {
	return SystemCall_NoArgs( CG_ARGC );
}

void	trap_Argv( int n, char *buffer, int bufferLength ) {
	SystemCall( CG_ARGV, n, buffer, bufferLength );
}

void	trap_Args( char *buffer, int bufferLength ) {
	SystemCall( CG_ARGS, buffer, bufferLength );
}

int		trap_FS_FOpenFile( const char *qpath, fileHandle_t *f, fsMode_t mode ) {
	return SystemCall( CG_FS_FOPENFILE, qpath, f, mode );
}

void	trap_FS_Read( void *buffer, int len, fileHandle_t f ) {
	SystemCall( CG_FS_READ, buffer, len, f );
}

void	trap_FS_Write( const void *buffer, int len, fileHandle_t f ) {
	SystemCall( CG_FS_WRITE, buffer, len, f );
}

void	trap_FS_FCloseFile( fileHandle_t f ) {
	SystemCall( CG_FS_FCLOSEFILE, f );
}

int trap_FS_GetFileList(  const char *path, const char *extension, char *listbuf, int bufsize ) {
	return SystemCall( CG_FS_GETFILELIST, path, extension, listbuf, bufsize );
}

int trap_FS_Delete( const char *filename ) {
	return SystemCall( CG_FS_DELETEFILE, filename);
}

void	trap_SendConsoleCommand( const char *text ) {
	SystemCall( CG_SENDCONSOLECOMMAND, text );
}

void	trap_AddCommand( const char *cmdName ) {
	SystemCall( CG_ADDCOMMAND, cmdName );
}

void	trap_RemoveCommand( const char *cmdName ) {
	SystemCall( CG_REMOVECOMMAND, cmdName );
}

void	trap_SendClientCommand( const char *s ) {
	SystemCall( CG_SENDCLIENTCOMMAND, s );
}

void	trap_UpdateScreen( void ) {
	SystemCall_NoArgs( CG_UPDATESCREEN );
}

/*void	trap_CM_LoadMap( const char *mapname ) {
	CG_DrawInformation();
	SystemCall( CG_CM_LOADMAP, mapname );
}*/

int		trap_CM_NumInlineModels( void ) {
	return SystemCall_NoArgs( CG_CM_NUMINLINEMODELS );
}

clipHandle_t trap_CM_InlineModel( int index ) {
	return SystemCall( CG_CM_INLINEMODEL, index );
}

clipHandle_t trap_CM_TempBoxModel( const vec3_t mins, const vec3_t maxs ) {
	return SystemCall( CG_CM_TEMPBOXMODEL, mins, maxs );
}

clipHandle_t trap_CM_TempCapsuleModel( const vec3_t mins, const vec3_t maxs ) {
	return SystemCall( CG_CM_TEMPCAPSULEMODEL, mins, maxs );
}

int		trap_CM_PointContents( const vec3_t p, clipHandle_t model ) {
	return SystemCall( CG_CM_POINTCONTENTS, p, model );
}

int		trap_CM_TransformedPointContents( const vec3_t p, clipHandle_t model, const vec3_t origin, const vec3_t angles ) {
	return SystemCall( CG_CM_TRANSFORMEDPOINTCONTENTS, p, model, origin, angles );
}

void	trap_CM_BoxTrace( trace_t *results, const vec3_t start, const vec3_t end,
						  const vec3_t mins, const vec3_t maxs,
						  clipHandle_t model, int brushmask ) {
	SystemCall( CG_CM_BOXTRACE, results, start, end, mins, maxs, model, brushmask );
}

void	trap_CM_TransformedBoxTrace( trace_t *results, const vec3_t start, const vec3_t end,
						  const vec3_t mins, const vec3_t maxs,
						  clipHandle_t model, int brushmask,
						  const vec3_t origin, const vec3_t angles ) {
	SystemCall( CG_CM_TRANSFORMEDBOXTRACE, results, start, end, mins, maxs, model, brushmask, origin, angles );
}

void	trap_CM_CapsuleTrace( trace_t *results, const vec3_t start, const vec3_t end,
						  const vec3_t mins, const vec3_t maxs,
						  clipHandle_t model, int brushmask ) {
	SystemCall( CG_CM_CAPSULETRACE, results, start, end, mins, maxs, model, brushmask );
}

void	trap_CM_TransformedCapsuleTrace( trace_t *results, const vec3_t start, const vec3_t end,
						  const vec3_t mins, const vec3_t maxs,
						  clipHandle_t model, int brushmask,
						  const vec3_t origin, const vec3_t angles ) {
	SystemCall( CG_CM_TRANSFORMEDCAPSULETRACE, results, start, end, mins, maxs, model, brushmask, origin, angles );
}

int		trap_CM_MarkFragments( int numPoints, const vec3_t *points, 
				const vec3_t projection,
				int maxPoints, vec3_t pointBuffer,
				int maxFragments, markFragment_t *fragmentBuffer ) {
	return SystemCall( CG_CM_MARKFRAGMENTS, numPoints, points, projection, maxPoints, pointBuffer, maxFragments, fragmentBuffer );
}

// ydnar
void		trap_R_ProjectDecal( qhandle_t hShader, int numPoints, vec3_t *points, vec4_t projection, vec4_t color, int lifeTime, int fadeTime )
{
	SystemCall( CG_R_PROJECTDECAL, hShader, numPoints, points, projection, color, lifeTime, fadeTime );
}

void		trap_R_ClearDecals( void )
{
	SystemCall_NoArgs( CG_R_CLEARDECALS );
}


void	trap_S_RealStartSound( const vec3_t origin, int entityNum, int entchannel, sfxHandle_t sfx, const char* file, int line ) {
	// if the number returned is negative or above this number its bad
	// trying to detect where the random sound handles go wrong
	// 4096 is the size of the s_knownSfx array in th engine
	if(sfx < 0 || sfx >= 4096) {
		Com_Printf("^1Warning: trap_S_StartSound out of range handle %i\nFile: '%s' Line: %i", sfx, file, line);
	}
	SystemCall( CG_S_STARTSOUND, origin, entityNum, entchannel, sfx, 127 /* Gordon: default volume always for the moment*/ );
}

void	trap_S_StartSoundVControl( const vec3_t origin, int entityNum, int entchannel, sfxHandle_t sfx, int volume ) {
	SystemCall( CG_S_STARTSOUND, origin, entityNum, entchannel, sfx, volume );
}

//----(SA)	added
void	trap_S_StartSoundEx( const vec3_t origin, int entityNum, int entchannel, sfxHandle_t sfx, int flags ) {
	SystemCall( CG_S_STARTSOUNDEX, origin, entityNum, entchannel, sfx, flags, 127 /* Gordon: default volume always for the moment*/ );
}
//----(SA)	end

void	trap_S_StartSoundExVControl( const vec3_t origin, int entityNum, int entchannel, sfxHandle_t sfx, int flags, int volume ) {
	SystemCall( CG_S_STARTSOUNDEX, origin, entityNum, entchannel, sfx, flags, volume );
}

void	trap_S_RealStartLocalSound( sfxHandle_t sfx, int channelNum, const char* file, int line ) {
	// if the number returned is negative or above this number its bad
	// trying to detect where the random sound handles go wrong
	// 4096 is the size of the s_knownSfx array in th engine
	if(sfx < 0 || sfx >= 4096) {
		Com_Printf("^1Warning: trap_S_StartSound out of range handle %i\nFile: '%s' Line: %i", sfx, file, line);
	}
	SystemCall( CG_S_STARTLOCALSOUND, sfx, channelNum, 127 /* Gordon: default volume always for the moment*/ );
}

/*void	trap_S_ClearLoopingSounds( void ) {
	SystemCall_NoArgs( CG_S_CLEARLOOPINGSOUNDS );
}*/

void	trap_S_ClearSounds( qboolean killmusic ) {
	SystemCall( CG_S_CLEARSOUNDS, killmusic );
}

/*void	trap_S_AddLoopingSound( const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx, int volume, int soundTime ) {
	SystemCall( CG_S_ADDLOOPINGSOUND, origin, velocity, 1250, sfx, volume, soundTime );		// volume was previously removed from CG_S_ADDLOOPINGSOUND.  I added 'range'
}*/

/*void	trap_S_AddRealLoopingSound( const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx, int range, int volume, int soundTime ) {
	SystemCall( CG_S_ADDREALLOOPINGSOUND, origin, velocity, range, sfx, volume, soundTime );
}*/

void	trap_S_StopStreamingSound(int entityNum) {
	SystemCall( CG_S_STOPSTREAMINGSOUND, entityNum );
}

void	trap_S_UpdateEntityPosition( int entityNum, const vec3_t origin ) {
	SystemCall( CG_S_UPDATEENTITYPOSITION, entityNum, origin );
}

// Ridah, talking animations
int		trap_S_GetVoiceAmplitude( int entityNum ) {
	return SystemCall( CG_S_GETVOICEAMPLITUDE, entityNum );
}
// done.

void	trap_S_Respatialize( int entityNum, const vec3_t origin, vec3_t axis[3], int inwater ) {
	SystemCall( CG_S_RESPATIALIZE, entityNum, origin, axis, inwater );
}

/*sfxHandle_t	trap_S_RegisterSound( const char *sample, qboolean compressed ) {
	CG_DrawInformation();
	return SystemCall( CG_S_REGISTERSOUND, sample, compressed );
}*/

int trap_S_GetSoundLength(sfxHandle_t sfx)
{
	return SystemCall( CG_S_GETSOUNDLENGTH, sfx );
}

// ydnar: for timing looped sounds
int trap_S_GetCurrentSoundTime( void )
{
	return SystemCall_NoArgs( CG_S_GETCURRENTSOUNDTIME );
}

/*void	trap_S_StartBackgroundTrack( const char *intro, const char *loop, int fadeupTime ) {
	SystemCall( CG_S_STARTBACKGROUNDTRACK, intro, loop, fadeupTime );
}*/

void	trap_S_FadeBackgroundTrack( float targetvol, int time, int num){	// yes, i know.  fadebackground coming in, fadestreaming going out.  will have to see where functionality leads...
	SystemCall( CG_S_FADESTREAMINGSOUND, PASSFLOAT(targetvol), time, num);	// 'num' is '0' if it's music, '1' if it's "all streaming sounds"
}

void	trap_S_FadeAllSound( float targetvol, int time, qboolean stopsounds) {
	SystemCall( CG_S_FADEALLSOUNDS, PASSFLOAT(targetvol), time, stopsounds);
}

int	trap_S_StartStreamingSound( const char *intro, const char *loop, int entnum, int channel, int attenuation ) {
	return SystemCall( CG_S_STARTSTREAMINGSOUND, intro, loop, entnum, channel, attenuation );
}

/*void	trap_R_LoadWorldMap( const char *mapname ) {
	CG_DrawInformation();
	SystemCall( CG_R_LOADWORLDMAP, mapname );
}

qhandle_t trap_R_RegisterModel( const char *name ) {
	CG_DrawInformation();
	return SystemCall( CG_R_REGISTERMODEL, name );
}*/

//----(SA)	added
qboolean trap_R_GetSkinModel( qhandle_t skinid, const char *type, char *name ) {
	return SystemCall( CG_R_GETSKINMODEL, skinid, type, name);
}

qhandle_t trap_R_GetShaderFromModel( qhandle_t modelid, int surfnum, int withlightmap) {
	return SystemCall( CG_R_GETMODELSHADER, modelid, surfnum, withlightmap);
}
//----(SA)	end

/*qhandle_t trap_R_RegisterSkin( const char *name ) {
	CG_DrawInformation();
	return SystemCall( CG_R_REGISTERSKIN, name );
}

qhandle_t trap_R_RegisterShader( const char *name ) {
	CG_DrawInformation();
	return SystemCall( CG_R_REGISTERSHADER, name );
}

qhandle_t trap_R_RegisterShaderNoMip( const char *name ) {
	CG_DrawInformation();
	return SystemCall( CG_R_REGISTERSHADERNOMIP, name );
}

void trap_R_RegisterFont(const char *fontName, int pointSize, fontInfo_t *font) {
	SystemCall(CG_R_REGISTERFONT, fontName, pointSize, font );
}*/

void	trap_R_ClearScene( void ) {
	SystemCall_NoArgs( CG_R_CLEARSCENE );
}

void	trap_R_RealAddRefEntityToScene( const refEntity_t *re ) {
	SystemCall( CG_R_ADDREFENTITYTOSCENE, re );
}

void	trap_R_RealAddPolyToScene( qhandle_t hShader , int numVerts, const polyVert_t *verts ) {
	SystemCall( CG_R_ADDPOLYTOSCENE, hShader, numVerts, verts );
}

void	trap_R_RealAddPolyBufferToScene( polyBuffer_t* pPolyBuffer ) {
	SystemCall( CG_R_ADDPOLYBUFFERTOSCENE, pPolyBuffer );
}

// Ridah
void	trap_R_RealAddPolysToScene( qhandle_t hShader , int numVerts, const polyVert_t *verts, int numPolys ) {
	SystemCall( CG_R_ADDPOLYSTOSCENE, hShader, numVerts, verts, numPolys );
}
// done.

// ydnar: new dlight system
//%	void	trap_R_AddLightToScene( const vec3_t org, float intensity, float r, float g, float b, int overdraw ) {
//%		SystemCall( CG_R_ADDLIGHTTOSCENE, org, PASSFLOAT(intensity), PASSFLOAT(r), PASSFLOAT(g), PASSFLOAT(b), overdraw );
//%	}
/*void	trap_R_RealAddLightToScene( const vec3_t org, float radius, float intensity, float r, float g, float b, qhandle_t hShader, int flags )
{
	SystemCall( CG_R_ADDLIGHTTOSCENE, org, PASSFLOAT( radius ), PASSFLOAT( intensity ),
		PASSFLOAT( r ), PASSFLOAT( g ), PASSFLOAT( b ), hShader, flags );
}*/

//----(SA)	
void	trap_R_AddCoronaToScene( const vec3_t org, float r, float g, float b, float scale, int id, qboolean visible) {
	SystemCall( CG_R_ADDCORONATOSCENE, org, PASSFLOAT(r), PASSFLOAT(g), PASSFLOAT(b), PASSFLOAT(scale), id, visible);
}
//----(SA)	

//----(SA)
void	trap_R_SetFog( int fogvar, int var1, int var2, float r, float g, float b, float density ) {
	SystemCall( CG_R_SETFOG, fogvar, var1, var2, PASSFLOAT(r), PASSFLOAT(g), PASSFLOAT(b), PASSFLOAT(density) );
}
//----(SA)	

void	trap_R_SetGlobalFog( qboolean restore, int duration, float r, float g, float b, float depthForOpaque ) {
	SystemCall( CG_R_SETGLOBALFOG, restore, duration, PASSFLOAT(r), PASSFLOAT(g), PASSFLOAT(b), PASSFLOAT(depthForOpaque) );
}

void	trap_R_RenderScene( const refdef_t *fd ) {
	SystemCall( CG_R_RENDERSCENE, fd );
}

// Mad Doctor I, 11/4/2002.	
void	trap_R_SaveViewParms( void ) 
{
	SystemCall_NoArgs( CG_R_SAVEVIEWPARMS );
}

// Mad Doctor I, 11/4/2002.	
void	trap_R_RestoreViewParms( void ) 
{
	SystemCall_NoArgs( CG_R_RESTOREVIEWPARMS );
}

void	trap_R_SetColor( const float *rgba ) {
	SystemCall( CG_R_SETCOLOR, rgba );
}

void	trap_R_DrawStretchPic( float x, float y, float w, float h, 
							   float s1, float t1, float s2, float t2, qhandle_t hShader ) {
	SystemCall( CG_R_DRAWSTRETCHPIC, PASSFLOAT(x), PASSFLOAT(y), PASSFLOAT(w), PASSFLOAT(h), PASSFLOAT(s1), PASSFLOAT(t1), PASSFLOAT(s2), PASSFLOAT(t2), hShader );
}

void	trap_R_DrawRotatedPic( float x, float y, float w, float h, 
							   float s1, float t1, float s2, float t2, qhandle_t hShader, float angle ) {
	SystemCall( CG_R_DRAWROTATEDPIC, PASSFLOAT(x), PASSFLOAT(y), PASSFLOAT(w), PASSFLOAT(h), PASSFLOAT(s1), PASSFLOAT(t1), PASSFLOAT(s2), PASSFLOAT(t2), hShader, PASSFLOAT(angle) );
}

void	trap_R_DrawStretchPicGradient(	float x, float y, float w, float h, 
										float s1, float t1, float s2, float t2, qhandle_t hShader,
										const float *gradientColor, int gradientType ) {
	SystemCall( CG_R_DRAWSTRETCHPIC_GRADIENT, PASSFLOAT(x), PASSFLOAT(y), PASSFLOAT(w), PASSFLOAT(h), PASSFLOAT(s1), PASSFLOAT(t1), PASSFLOAT(s2), PASSFLOAT(t2), hShader, gradientColor, gradientType  );
}

void trap_R_Add2dPolys( polyVert_t* verts, int numverts, qhandle_t hShader ) {
	SystemCall( CG_R_DRAW2DPOLYS, verts, numverts, hShader );
}


void	trap_R_ModelBounds( clipHandle_t model, vec3_t mins, vec3_t maxs ) {
	SystemCall( CG_R_MODELBOUNDS, model, mins, maxs );
}

/*int		trap_R_LerpTag( orientation_t *tag, const refEntity_t *refent, const char *tagName, int startIndex ) {
	return SystemCall( CG_R_LERPTAG, tag, refent, tagName, startIndex );
}*/

void	trap_R_RemapShader( const char *oldShader, const char *newShader, const char *timeOffset ) {
	SystemCall( CG_R_REMAP_SHADER, oldShader, newShader, timeOffset );
}

void		trap_GetGlconfig( glconfig_t *glconfig ) {
	SystemCall( CG_GETGLCONFIG, glconfig );
}

void		trap_GetGameState( gameState_t *gamestate ) {
	SystemCall( CG_GETGAMESTATE, gamestate );
}

void trap_GetCurrentSnapshotNumber( int *snapshotNumber, int *serverTime ) {
	SystemCall( CG_GETCURRENTSNAPSHOTNUMBER, snapshotNumber, serverTime );
}

qboolean	trap_GetSnapshot( int snapshotNumber, snapshot_t *snapshot ) {
	return SystemCall( CG_GETSNAPSHOT, snapshotNumber, snapshot );
}

qboolean	trap_GetServerCommand( int serverCommandNumber ) {
	return SystemCall( CG_GETSERVERCOMMAND, serverCommandNumber );
}

int			trap_GetCurrentCmdNumber( void ) {
	return SystemCall_NoArgs( CG_GETCURRENTCMDNUMBER );
}

qboolean	trap_GetUserCmd( int cmdNumber, usercmd_t *ucmd ) {
	return SystemCall( CG_GETUSERCMD, cmdNumber, ucmd );
}

/*void		trap_SetUserCmdValue( int stateValue, int flags, float sensitivityScale, int mpIdentClient ) {
	SystemCall( CG_SETUSERCMDVALUE, stateValue, flags, PASSFLOAT(sensitivityScale), mpIdentClient );
}*/

void		trap_SetClientLerpOrigin( float x, float y, float z ) {
	SystemCall( CG_SETCLIENTLERPORIGIN, PASSFLOAT(x), PASSFLOAT(y), PASSFLOAT(z) );
}

int trap_MemoryRemaining( void ) {
	return SystemCall_NoArgs( CG_MEMORY_REMAINING );
}

qboolean trap_loadCamera( int camNum, const char *name ) {
	return SystemCall( CG_LOADCAMERA, camNum, name );
}

void trap_startCamera(int camNum, int time) {
	SystemCall(CG_STARTCAMERA, camNum, time);
}

void trap_stopCamera(int camNum) {
	SystemCall(CG_STOPCAMERA, camNum);
}

qboolean trap_getCameraInfo( int camNum, int time, vec3_t *origin, vec3_t *angles, float *fov) {
	return SystemCall( CG_GETCAMERAINFO, camNum, time, origin, angles, fov );
}


qboolean trap_Key_IsDown( int keynum ) {
	return SystemCall( CG_KEY_ISDOWN, keynum );
}

int trap_Key_GetCatcher( void ) {
	return SystemCall_NoArgs( CG_KEY_GETCATCHER );
}

qboolean trap_Key_GetOverstrikeMode( void ) {
	return SystemCall_NoArgs( CG_KEY_GETOVERSTRIKEMODE );
}

void trap_Key_SetOverstrikeMode( qboolean state ) {
	SystemCall( CG_KEY_SETOVERSTRIKEMODE, state );
}

// binding MUST be lower case
void trap_Key_KeysForBinding( const char* binding, int* key1, int* key2 ) {
	SystemCall( CG_KEY_BINDINGTOKEYS, binding, key1, key2 );
}

void trap_Key_SetCatcher( int catcher ) {
	SystemCall( CG_KEY_SETCATCHER, catcher );
}

int trap_Key_GetKey( const char *binding ) {
	return SystemCall( CG_KEY_GETKEY, binding );
}


int trap_PC_AddGlobalDefine( const char *define ) {
	return SystemCall( CG_PC_ADD_GLOBAL_DEFINE, define );
}

int trap_PC_LoadSource( const char *filename ) {
	return SystemCall( CG_PC_LOAD_SOURCE, filename );
}

int trap_PC_FreeSource( int handle ) {
	return SystemCall( CG_PC_FREE_SOURCE, handle );
}

int trap_PC_ReadToken( int handle, pc_token_t *pc_token ) {
	return SystemCall( CG_PC_READ_TOKEN, handle, pc_token );
}

int trap_PC_SourceFileAndLine( int handle, char *filename, int *line ) {
	return SystemCall( CG_PC_SOURCE_FILE_AND_LINE, handle, filename, line );
}

int trap_PC_UnReadToken( int handle ) {
	return SystemCall( CG_PC_UNREAD_TOKEN, handle );
}

void	trap_S_StopBackgroundTrack( void ) {
	SystemCall_NoArgs( CG_S_STOPBACKGROUNDTRACK );
}

int trap_RealTime(qtime_t *qtime) {
	return SystemCall( CG_REAL_TIME, qtime );
}

void trap_SnapVector( float *v ) {
	SystemCall( CG_SNAPVECTOR, v );
}

// this returns a handle.  arg0 is the name in the format "idlogo.roq", set arg1 to NULL, alteredstates to qfalse (do not alter gamestate)
int trap_CIN_PlayCinematic( const char *arg0, int xpos, int ypos, int width, int height, int bits) {
  return SystemCall(CG_CIN_PLAYCINEMATIC, arg0, xpos, ypos, width, height, bits);
}
 
// stops playing the cinematic and ends it.  should always return FMV_EOF
// cinematics must be stopped in reverse order of when they are started
e_status trap_CIN_StopCinematic(int handle) {
  return SystemCall(CG_CIN_STOPCINEMATIC, handle);
}


// will run a frame of the cinematic but will not draw it.  Will return FMV_EOF if the end of the cinematic has been reached.
e_status trap_CIN_RunCinematic (int handle) {
  return SystemCall(CG_CIN_RUNCINEMATIC, handle);
}
 

// draws the current frame
void trap_CIN_DrawCinematic (int handle) {
  SystemCall(CG_CIN_DRAWCINEMATIC, handle);
}
 

// allows you to resize the animation dynamically
void trap_CIN_SetExtents (int handle, int x, int y, int w, int h) {
  SystemCall(CG_CIN_SETEXTENTS, handle, x, y, w, h);
}

qboolean trap_GetEntityToken( char *buffer, int bufferSize ) {
	return SystemCall( CG_GET_ENTITY_TOKEN, buffer, bufferSize );
}

//----(SA)	added
// bring up a popup menu
extern void	Menus_OpenByName(const char *p);

//void trap_UI_Popup( const char *arg0) {
void trap_UI_Popup( int arg0 ) {
	SystemCall(CG_INGAME_POPUP, arg0);
}

void trap_UI_ClosePopup( const char *arg0) {
	SystemCall(CG_INGAME_CLOSEPOPUP, arg0);
}

void trap_Key_GetBindingBuf( int keynum, char *buf, int buflen ) {
	SystemCall( CG_KEY_GETBINDINGBUF, keynum, buf, buflen );
}

void trap_Key_SetBinding( int keynum, const char *binding ) {
	SystemCall( CG_KEY_SETBINDING, keynum, binding );
}

void trap_Key_KeynumToStringBuf( int keynum, char *buf, int buflen ) {
	SystemCall( CG_KEY_KEYNUMTOSTRINGBUF, keynum, buf, buflen );
}

#define	MAX_VA_STRING		32000

char* trap_TranslateString( const char *string ) {
	static char staticbuf[2][MAX_VA_STRING];
	static int bufcount = 0;
	char *buf;

	buf = staticbuf[bufcount++ % 2];

	SystemCall( CG_TRANSLATE_STRING, string, buf );

	return buf;
}
// -NERVE - SMF

// Media register functions
#ifdef _DEBUG
#define DEBUG_REGISTERPROFILE_INIT UNUSED_VAR int dbgTime = trap_Milliseconds();
#define DEBUG_REGISTERPROFILE_EXEC(f,n) if( developer.integer ) CG_Printf( BOX_PRINT_MODE_CHAT, "%s : loaded %s in %i msec\n", f, n, trap_Milliseconds()-dbgTime ); 
sfxHandle_t	trap_S_RealRegisterSound( const char *sample, qboolean compressed ) {
	sfxHandle_t snd;
	DEBUG_REGISTERPROFILE_INIT
//	CG_DrawInformation( qtrue );
	snd = SystemCall( CG_S_REGISTERSOUND, sample, qfalse /* compressed */ );
	if(!*sample) {
		Com_Printf("^1Warning: Null Sample filename\n");
	}
	if(snd == 0) {
		Com_Printf("^1Warning: Failed to load sound: %s\n", sample);
	}
	// if the number returned is negative or above this number its bad
	// trying to detect where the random sound handles go wrong
	// 4096 is the size of the s_knownSfx array in the engine
	if(snd < 0 || snd >= 4096) {
		Com_Printf("^1Warning: Failed to load sound: %s (out of range handle %i)\n", sample, snd);
	}
	DEBUG_REGISTERPROFILE_EXEC("trap_S_RealRegisterSound",sample)
//	trap_PumpEventLoop();
	return snd;
}

qhandle_t trap_R_RealRegisterModel( const char *name ) {
	qhandle_t handle;
	DEBUG_REGISTERPROFILE_INIT
//	CG_DrawInformation( qtrue );
	handle = SystemCall( CG_R_REGISTERMODEL, name );
	DEBUG_REGISTERPROFILE_EXEC("trap_R_RealRegisterModel",name)
//	trap_PumpEventLoop();
	return handle;
}

qhandle_t trap_R_RegisterSkin( const char *name ) {
	qhandle_t handle;
	DEBUG_REGISTERPROFILE_INIT
//	CG_DrawInformation( qtrue );
	handle = SystemCall( CG_R_REGISTERSKIN, name );
	DEBUG_REGISTERPROFILE_EXEC("trap_R_RegisterSkin",name)
//	trap_PumpEventLoop();
	return handle;
}

qhandle_t trap_R_RegisterShader( const char *name ) {
	qhandle_t handle;
	DEBUG_REGISTERPROFILE_INIT
//	CG_DrawInformation( qtrue );
	handle = SystemCall( CG_R_REGISTERSHADER, name );
	DEBUG_REGISTERPROFILE_EXEC("trap_R_RegisterShader",name)
//	trap_PumpEventLoop();
	return handle;
}

qhandle_t trap_R_RegisterShaderNoMip( const char *name ) {
	qhandle_t handle;
	DEBUG_REGISTERPROFILE_INIT
//	CG_DrawInformation( qtrue );
	handle = SystemCall( CG_R_REGISTERSHADERNOMIP, name );
//	trap_PumpEventLoop();
	return handle;
}

void trap_R_RegisterFont(const char *fontName, int pointSize, fontInfo_t *font) {
	DEBUG_REGISTERPROFILE_INIT
//	CG_DrawInformation( qtrue );
	SystemCall(CG_R_REGISTERFONT, fontName, pointSize, font );
	DEBUG_REGISTERPROFILE_EXEC("trap_R_RegisterFont",fontName)
//	trap_PumpEventLoop();
}

void	trap_CM_LoadMap( const char *mapname ) {
	DEBUG_REGISTERPROFILE_INIT
//	CG_DrawInformation( qtrue );
	SystemCall( CG_CM_LOADMAP, mapname );
	DEBUG_REGISTERPROFILE_EXEC("trap_CM_LoadMap",mapname)
//	trap_PumpEventLoop();
}

void	trap_R_LoadWorldMap( const char *mapname ) {
	DEBUG_REGISTERPROFILE_INIT
//	CG_DrawInformation( qtrue );
	SystemCall( CG_R_LOADWORLDMAP, mapname );
	DEBUG_REGISTERPROFILE_EXEC("trap_R_LoadWorldMap",mapname)
//	trap_PumpEventLoop();
}
#else
sfxHandle_t	trap_S_RealRegisterSound( const char *sample, qboolean compressed ) {
//	CG_DrawInformation( qtrue );
//	trap_PumpEventLoop();
	return SystemCall( CG_S_REGISTERSOUND, sample, qfalse /* compressed */ );
}

qhandle_t trap_R_RealRegisterModel( const char *name ) {
//	CG_DrawInformation( qtrue );
//	trap_PumpEventLoop();
	return SystemCall( CG_R_REGISTERMODEL, name );
}

qhandle_t trap_R_RegisterSkin( const char *name ) {
//	CG_DrawInformation( qtrue );
//	trap_PumpEventLoop();
	return SystemCall( CG_R_REGISTERSKIN, name );
}

qhandle_t trap_R_RegisterShader( const char *name ) {
//	CG_DrawInformation( qtrue );
//	trap_PumpEventLoop();
	return SystemCall( CG_R_REGISTERSHADER, name );
}

qhandle_t trap_R_RegisterShaderNoMip( const char *name ) {
//	CG_DrawInformation( qtrue );
//	trap_PumpEventLoop();
	return SystemCall( CG_R_REGISTERSHADERNOMIP, name );
}

void trap_R_RegisterFont(const char *fontName, int pointSize, fontInfo_t *font) {
//	CG_DrawInformation( qtrue );
//	trap_PumpEventLoop();
	SystemCall(CG_R_REGISTERFONT, fontName, pointSize, font );
}

void	trap_CM_LoadMap( const char *mapname ) {
//	CG_DrawInformation( qtrue );
//	trap_PumpEventLoop();
	SystemCall( CG_CM_LOADMAP, mapname );
}

void	trap_R_LoadWorldMap( const char *mapname ) {
//	CG_DrawInformation( qtrue );
//	trap_PumpEventLoop();
	SystemCall( CG_R_LOADWORLDMAP, mapname );
}
#endif // _DEBUG

qboolean trap_R_inPVS( const vec3_t p1, const vec3_t p2 ) {
	return SystemCall( CG_R_INPVS, p1, p2 );
}

void trap_GetHunkData( int* hunkused, int* hunkexpected ) {
	SystemCall( CG_GETHUNKDATA, hunkused, hunkexpected );
}

//zinx - binary message channel
void trap_SendMessage( char *buf, int buflen ) {
	SystemCall( CG_SENDMESSAGE, buf, buflen );
}

messageStatus_t trap_MessageStatus( void ) {
	return SystemCall_NoArgs( CG_MESSAGESTATUS );
}

//bani - dynamic shaders
qboolean trap_R_LoadDynamicShader( const char *shadername, const char *shadertext ) {
	return SystemCall( CG_R_LOADDYNAMICSHADER, shadername, shadertext );
}

// fretn - render to texture
void trap_R_RenderToTexture( int textureid, int x, int y, int w, int h ) {
	SystemCall( CG_R_RENDERTOTEXTURE, textureid, x, y, w, h );
}

int trap_R_GetTextureId( const char *name ) {
	return SystemCall( CG_R_GETTEXTUREID, name );
}

// bani - sync rendering
void trap_R_Finish( void ) {
	SystemCall_NoArgs( CG_R_FINISH );
}

// extension interface

qboolean trap_GetValue( char *value, int valueSize, const char *key ) {
	return SystemCall( dll_com_trapGetValue, value, valueSize, key );
}

void trap_R_AddRefEntityToScene2( const refEntity_t *re ) {
	SystemCall( dll_trap_R_AddRefEntityToScene2, re );
}

void trap_R_AddLinearLightToScene( const vec3_t start, const vec3_t end, float intensity, float r, float g, float b ) {
	SystemCall( dll_trap_R_AddLinearLightToScene, start, end, intensity, r, g, b );
}

