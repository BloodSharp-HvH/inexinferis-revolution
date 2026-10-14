#include "screenshots.h"
#include "autoroute.h"
#include "players.h"
#include "opengl.h"
#include "aimbot.h"
#include "client.h"
#include "utils.h"
#include "cvar.h"

tglEnable pglEnable=NULL;
tglDisable pglDisable=NULL;
tglColor4f pglColor4f=NULL;
tglGetFloatv pglGetFloatv=NULL;
tglGetIntegerv pglGetIntegerv=NULL;
tglClearColor pglClearColor=NULL;
tglTexEnvi pglTexEnvi=NULL;
tglBlendFunc pglBlendFunc=NULL;
tglDepthRange pglDepthRange=NULL;
tglDepthFunc pglDepthFunc=NULL;
tglClear pglClear=NULL;
//tglVertex2f pglVertex2f=NULL;
tglVertex3f pglVertex3f=NULL;
tglVertex3fv pglVertex3fv=NULL;
tglBegin pglBegin=NULL;
tglEnd pglEnd=NULL;
tglFrustum pglFrustum=NULL;
tglReadBuffer pglReadBuffer=NULL;
tglReadPixels pglReadPixels=NULL;
tglPolygonMode pglPolygonMode=NULL;
tglLineWidth pglLineWidth=NULL;
tglColor3f pglColor3f=NULL;
tglViewport pglViewport=NULL;
tglPopMatrix pglPopMatrix=NULL;
tglGenTextures pglGenTextures=NULL;
tglBindTexture pglBindTexture=NULL;
tglTexImage2D pglTexImage2D=NULL;
tglTexParameteri pglTexParameteri=NULL;
tglTexCoord2f pglTexCoord2f=NULL;

//New OpenGL Functions
twglGetProcAddress pwglGetProcAddress=NULL;
tglBindBuffer pglBindBuffer=NULL;
tglMapBuffer pglMapBuffer=NULL;
tglUnmapBuffer pglUnmapBuffer=NULL;
tglGenBuffers pglGenBuffers=NULL;
tglBufferData pglBufferData=NULL;

HMODULE hOpengl32=NULL;

bool bSkyTex=false,bSmoke=false,bTex=false,oglSubtractive=false,oglChams=false;//,bFlash=false;
float oglChamsColor[3];

GLuint textureID=0;

BOOL WINAPI CheckFuncs(){
  static BOOL bCheckOK=FALSE;
  if(!bCheckOK)
    bCheckOK=(pglEnable&&pglDisable&&pglColor4f&&pglGetFloatv&&pglGetIntegerv&&pglClearColor&&pglTexEnvi&&pglBlendFunc&&pglDepthRange&&pglDepthFunc&&pglClear&&
      pglVertex3f&&pglVertex3fv&&pglBegin&&pglEnd&&pglFrustum&&pglReadBuffer&&pglReadPixels&&pglPolygonMode&&pglLineWidth&&pglColor3f);
  return bCheckOK;
}

void WINAPI hglBegin(GLenum mode){
  GLfloat curcolor[4];
  if(cvar.active&&!cvar.takingss&&(!cvar.antiss||!gMe.IsFreeSpectator())&&CheckFuncs()){
    if(cvar.wallhack==2){
      if(mode==GL_POLYGON){
        pglDisable(GL_DEPTH_TEST);
        pglEnable(GL_BLEND);
        pglBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA_SATURATE);
        pglGetFloatv(GL_CURRENT_COLOR,curcolor);
        pglColor4f(curcolor[0],curcolor[1],curcolor[2],0.667f);
      }else
        if(mode==GL_TRIANGLES||mode==GL_TRIANGLE_STRIP||mode==GL_TRIANGLE_FAN){
          pglEnable(GL_DEPTH_TEST);
          pglDisable(GL_BLEND);
        }
    }
    if(mode!=GL_TRIANGLES&&mode!=GL_TRIANGLE_STRIP&&mode!=GL_TRIANGLE_FAN&&mode!=GL_QUADS){
      if(cvar.nightmode&&!gMe.zoom)
        pglTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_BLEND);
      if(cvar.whitewalls&&!gMe.zoom)
        pglTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
      if(cvar.fullbright&&bTex&&!gMe.zoom)
        pglDisable(GL_TEXTURE_2D);
    }else{
      if(cvar.nigger)
        pglTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_BLEND);
      if(cvar.lambert&&!cvar.glowshells&&mode==GL_TRIANGLE_STRIP)
        pglTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
    }
    switch((int)cvar.wireframe){
      case 1:
        if(mode==GL_POLYGON){
          pglPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
          pglLineWidth(1.0f);
          pglColor3f(1.0f,1.0f,1.0f);
        }else
          pglPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      break;
      case 2:
        if(mode==GL_POLYGON){
          pglPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
          pglLineWidth(2.0f);
          pglColor3f(1.0f,1.0f,1.0f);
        }else
          pglPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			case 3:
        if(mode==GL_POLYGON){
          pglPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
          pglLineWidth(4.0f);
          pglColor3f(1.0f,1.0f,1.0f);
        }else
          pglPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      break;
      default:break;
    }
    switch((int)cvar.wiremodels){
      case 1:
    		if(mode==GL_TRIANGLE_STRIP||mode==GL_TRIANGLE_FAN){
          pglPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
          pglLineWidth(1.0f);
        }else if(mode!=GL_POLYGON)
          pglPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      break;
      case 2:
        if(mode==GL_TRIANGLE_STRIP||mode==GL_TRIANGLE_FAN){
          pglPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
          pglLineWidth(3.0f);
        }else if(mode!=GL_POLYGON)
          pglPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      break;
			case 3:
        if(mode==GL_TRIANGLE_STRIP||mode==GL_TRIANGLE_FAN){
          pglPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
          pglLineWidth(5.0f);
        }else if(mode!=GL_POLYGON)
          pglPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      break;
      default:break;
    }
    if(mode==GL_QUADS){
      pglGetFloatv(GL_CURRENT_COLOR,curcolor);
      //fix laserbeam with no sky
      bSkyTex=!((curcolor[0]>0.0&&curcolor[1]==0.0&&curcolor[2]==0.0)||
                (curcolor[0]==0.0&&curcolor[1]>0.0&&curcolor[2]==0.0)||
                (curcolor[0]==0.0&&curcolor[1]==0.0&&curcolor[2]>0.0));
      //fix no smoke engine!
      bSmoke=(curcolor[0]==curcolor[1]&&curcolor[0]==curcolor[2]&&curcolor[0]!=0.0&&curcolor[0]!=1.0);
      //bFlash=(curcolor[0]>=1.0&&curcolor[1]>=1.0&&curcolor[2]>=1.0&&curcolor[3]>0.2);
    }else{
      bSkyTex=false;
      bSmoke=false;
    }
  }
  pglBegin(mode);
}

void WINAPI hglClear(GLbitfield mask){
  //don't clear cam view!
  if(cvar.active&&!cvar.takingss&&(!cvar.antiss||!gMe.IsFreeSpectator())&&
    (cvar.nosky||cvar.wallhack==2||cvar.wireframe)&&!drawingCam){
		if(mask&GL_DEPTH_BUFFER_BIT){
		  if(cvar.wireframe){
        if(pglClearColor)
          pglClearColor(0.1f,0.1f,0.1f,0.1f);
      }else{
        if(pglClearColor)
          pglClearColor(0.0f,0.0f,0.0f,0.0f);
      }
      mask|=GL_COLOR_BUFFER_BIT;
		}
	}
	pglClear(mask);
}

void WINAPI hglVertex3fv(const GLfloat *v){
  if(cvar.active&&!cvar.takingss&&(!cvar.antiss||!gMe.IsFreeSpectator())&&
     ((cvar.nosmoke&&bSmoke)||(bSkyTex&&(cvar.nosky||cvar.wallhack==2))))
    return;
  pglVertex3fv(v);
}

/*void WINAPI hglVertex2f(GLfloat x,GLfloat y){
  if(cvar.active&&!cvar.takingss&&cvar.noflash&&bFlash&&x==0.0&&y==0.0)
    pglColor4f(1.0f,1.0f,1.0f,0.1f);
  pglVertex2f(x,y);
}*/

void WINAPI hglEnable(GLenum mode){
  if(mode==GL_TEXTURE_2D)bTex=true;
  pglEnable(mode);
}

void WINAPI hglDisable(GLenum mode){
  if(mode==GL_TEXTURE_2D)bTex=false;
  pglDisable(mode);
}

//Cam zoom
void WINAPI hglFrustum(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble zNear,GLdouble zFar){
	if(cvar.active&&!cvar.takingss&&(!cvar.antiss||!gMe.IsFreeSpectator())&&
    drawingCam==1&&cvar.spycam==1){
		top=zNear*tan((50-cvar.camzoom*5)*M_PI/360);
		bottom=-top;
		left=bottom*4/3;
		right=top*4/3;
	}
  pglFrustum(left,right,bottom,top,zNear,zFar);
}

//1.5 tint func!
void WINAPI hglBlendFunc(GLenum sfactor,GLenum dfactor){
  if(cvar.active&&!cvar.takingss&&(!cvar.antiss||!gMe.IsFreeSpectator())&&
    cvar.hudhealth&&dfactor==GL_ONE&&gMe.iHealth){
    GLfloat iRed = 1.0f;
    GLfloat iGreen = 1.0f;
    GLfloat iBlue = 1.0f;
    if(gMe.iHealth >= 60){
      iRed = 0.0f;
      iGreen = 1.0f;
      iBlue = 0.0f;
    }else if((gMe.iHealth >= 20) && (gMe.iHealth <= 59)){
      iRed = 1.0f;
      iGreen = 1.0f;
      iBlue = 0.2f;
    }else if(gMe.iHealth <= 19){
      iRed = 1.0f;
      iGreen = 0.0f;
      iBlue = 0.0f;
    }
    pglColor3f(iRed,iGreen,iBlue);
  }
	if(oglSubtractive)
    pglBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	else
    pglBlendFunc(sfactor,dfactor);
}

void WINAPI hglColor4f(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha){
  if(cvar.active&&!cvar.takingss&&(!cvar.antiss||!gMe.IsFreeSpectator())&&oglChams){
    red=oglChamsColor[0];
    green=oglChamsColor[1];
    blue=oglChamsColor[2];
    alpha=1.0;
  }
  pglColor4f(red,green,blue,alpha);
}

//SmallView
void WINAPI hglViewport(GLint x,GLint y,GLsizei width,GLsizei height){
	bool bSmall=(cvar.active&&!cvar.takingss&&cvar.smallview&&gMe.alive&&
    cvar.speedmode&&gMe.inspeed&&gAutoRoute.runninginroute&&!drawingCam);
	pglViewport(
		(bSmall?(width-200)/2:x),
		(bSmall?(height-150)/2:y),
		(bSmall?200:width),
		(bSmall?150:height)
	);
}

//UCP/SXE Screenshot
void WINAPI hglReadPixels(GLint x,GLint	y,GLsizei	width,GLsizei	height,GLenum format,GLenum type,GLvoid *data){
  if(cvar.active&&cvar.antiss)
    gScreenShots.CopyScreenShot(width,height,format,(PBYTE)data);
  else
    pglReadPixels(x,y,width,height,format,type,data);
}

void WINAPI glCreateTextureBuffer(GLint width,GLint height,GLvoid* data){
  static BOOL bInit=FALSE;
  if(!bInit){
    pglGenTextures(1,&textureID);
    bInit=TRUE;
  }
  if(pglBindTexture)
    pglBindTexture(GL_TEXTURE_2D,textureID);
  if(pglTexImage2D)
    pglTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_BGRA,GL_UNSIGNED_BYTE,data);
  if(pglTexParameteri){
    pglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    pglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  }
}

void WINAPI glDrawTexture(GLint x,GLint y,GLint width,GLint height){
  if(pglEnable)
    pglEnable(GL_TEXTURE_2D);
  if(pglBindTexture)
    pglBindTexture(GL_TEXTURE_2D,textureID);
  if(pglColor4f)
    pglColor4f(1.0f,1.0f,1.0f,0.1f);
  if(pglBegin)
    pglBegin(GL_QUADS);
  if(pglTexCoord2f&&pglVertex3f){
    pglTexCoord2f(1.0f,1.0f);
    pglVertex3f(x+width,y+height,1.0f);
    pglTexCoord2f(0.0f,1.0f);
    pglVertex3f(x,y+height,1.0f);
    pglTexCoord2f(0.0f,0.0f);
    pglVertex3f(x,y,1.0f);
    pglTexCoord2f(1.0f,0.0f);
    pglVertex3f(x+width,y,1.0f);
  }
  if(pglEnd)
    pglEnd();
  if(pglDisable)
    pglDisable(GL_TEXTURE_2D);
}

/*******************************************************************************\
 TIB HOOKING V3.0
\*******************************************************************************/

#include "Detours/detours.h"

BOOL WINAPI HookOpenGl(){

	HMODULE hOpengl32 = GetModuleHandle("opengl32.dll");

	if (!hOpengl32)
		return FALSE;

    //Update redirect call
    pglEnd = (tglEnd) GetProcAddress(hOpengl32, "glEnd");
    pglColor3f = (tglColor3f) GetProcAddress(hOpengl32, "glColor3f");
    pglVertex3f = (tglVertex3f) GetProcAddress(hOpengl32, "glVertex3f");
    pglTexCoord2f = (tglTexCoord2f) GetProcAddress(hOpengl32, "glTexCoord2f");
    pglBindTexture = (tglBindTexture) GetProcAddress(hOpengl32, "glBindTexture");
    
    pglGetFloatv = (tglGetFloatv) GetProcAddress(hOpengl32, "glGetFloatv");
    pglGetIntegerv = (tglGetIntegerv) GetProcAddress(hOpengl32, "glGetIntegerv");
    pglClearColor = (tglClearColor) GetProcAddress(hOpengl32, "glClearColor");
    pglTexEnvi = (tglTexEnvi) GetProcAddress(hOpengl32, "glTexEnvi");
    pglDepthRange = (tglDepthRange) GetProcAddress(hOpengl32, "glDepthRange");
    pglDepthFunc = (tglDepthFunc) GetProcAddress(hOpengl32, "glDepthFunc");
    pglReadBuffer = (tglReadBuffer) GetProcAddress(hOpengl32, "glReadBuffer");
    pglLineWidth = (tglLineWidth) GetProcAddress(hOpengl32, "glLineWidth");
    pglPolygonMode = (tglPolygonMode) GetProcAddress(hOpengl32, "glPolygonMode");
    pglGenTextures = (tglGenTextures) GetProcAddress(hOpengl32, "glGenTextures");
    pglTexImage2D = (tglTexImage2D) GetProcAddress(hOpengl32, "glTexImage2D");
    pglTexParameteri = (tglTexParameteri) GetProcAddress(hOpengl32, "glTexParameteri");

    //Update TIB Hook
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    pglBegin = (tglBegin) GetProcAddress(hOpengl32, "glBegin");
    DetourAttach(&(PVOID&)pglBegin, hglBegin);
    pglEnable = (tglEnable) GetProcAddress(hOpengl32, "glEnable");
    DetourAttach(&(PVOID&)pglEnable, hglEnable);
    pglDisable = (tglDisable) GetProcAddress(hOpengl32, "glDisable");
    DetourAttach(&(PVOID&)pglDisable, hglDisable);
    pglVertex3fv = (tglVertex3fv) GetProcAddress(hOpengl32, "glVertex3fv");
    DetourAttach(&(PVOID&)pglVertex3fv, hglVertex3fv);
    pglColor4f = (tglColor4f) GetProcAddress(hOpengl32, "glColor4f");
    DetourAttach(&(PVOID&)pglColor4f, hglColor4f);

    pglFrustum = (tglFrustum) GetProcAddress(hOpengl32, "glFrustum");
    DetourAttach(&(PVOID&)pglFrustum, hglFrustum);
    pglClear = (tglClear) GetProcAddress(hOpengl32, "glClear");
    DetourAttach(&(PVOID&)pglClear, hglClear);
    pglBlendFunc = (tglBlendFunc) GetProcAddress(hOpengl32, "glBlendFunc");
    DetourAttach(&(PVOID&)pglBlendFunc, hglBlendFunc);
    pglReadPixels = (tglReadPixels) GetProcAddress(hOpengl32, "glReadPixels");
    DetourAttach(&(PVOID&)pglReadPixels, hglReadPixels);
    pglViewport = (tglViewport) GetProcAddress(hOpengl32, "glViewport");
    DetourAttach(&(PVOID&)pglViewport, hglViewport);

    LONG error = DetourTransactionCommit();

    if (error != NO_ERROR) {
        return FALSE;
    }

    //New OpenGL functions...
    if (!pwglGetProcAddress) {
        pwglGetProcAddress = (twglGetProcAddress)GetProcAddress(hOpengl32, "wglGetProcAddress");
    }

    if (pwglGetProcAddress) {
        if (!pglGenBuffers) {
            pglGenBuffers = (tglGenBuffers)pwglGetProcAddress("glGenBuffers");
        }
        if (!pglBufferData) {
            pglBufferData = (tglBufferData)pwglGetProcAddress("glBufferData");
        }
        if (!pglBindBuffer) {
            pglBindBuffer = (tglBindBuffer)pwglGetProcAddress("glBindBuffer");
        }
        if (!pglMapBuffer) {
            pglMapBuffer = (tglMapBuffer)pwglGetProcAddress("glMapBuffer");
        }
        if (!pglUnmapBuffer) {
            pglUnmapBuffer = (tglUnmapBuffer)pwglGetProcAddress("glUnmapBuffer");
        }
    }
    
    return TRUE;
}
