/****************************************************************************

File Name:		  LCTSUPT.H

Company: 		  LC Technologies, Inc.
					  10363 Democracy Lane
                 Fairfax, VA 22030
					  (703) 385-8800

*****************************************************************************/
#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

#include <windows.h>   // HI THERE  // for COLORREF

/****************************************************************************/
#define HOLLOW_FILL 0
#define SOLID_FILL  1
#define NO_HATCH   -1
#define FALSE 0
#define TRUE  1

/* Font Functions:																			 */
int	lctFontInit(void);
int	lctFontLoad(int iFontNumber);
int	lctFontLoadSpecific(int iFontNumber, wchar_t *pchFontName,
								  int iDeciPtHeight, int iFontAttributes);
void	lct_font_unload(int iFontNumber);
int	lct_fontpixn(int iFontNumber, wchar_t *str, int dir, int len);
int	lct_fontheight(int iFontNumber, wchar_t *pchText);
int	lct_fontch(int iFontNumber, wchar_t ch, int tc, int tr, int color, int rot);
int   lct_fontsprt(int iFontNumber, wchar_t *pchText, int x, int y, int iDirection, int iColor, int iRotation);
void	lct_FontInfo(int iFontNumber, void *p);

/* Graphics Primitives: 																	 */
void	lct_dot(int x, int y,int color);
void  lct_plus(int x, int y, int iColor);
int	lct_readdot(int x, int y);
void	lct_line(int x1,int y1,int x2,int y2,int color);
void	lct_dashed_line(int x1,int y1,int x2,int y2,int color, int iLineStyle);
int	lct_rectangle(int x0,int y0,int x1,int y1,int color,int fill,int hatch);
int	lct_circle(int x, int y, int radius, int color, int fill);
void	lct_ReadGraphicsArea(int ulx,int uly,int lrx,int lry, char *array);
void	lct_WriteGraphicsArea(int ulx, int uly, char *array, int type);
BOOL  lct_RedrawScreen(int x1, int y1, int x2, int y2);

/* GDI+ related functions:                                                  */
int lctDrawImage(wchar_t *pchImage, int iX0, int iY0, int iX1, int iY1);
int lctGDIInit(void);
int lctGDIExit(void);

BOOL lctPlaySound(LPCTSTR pszSound, HMODULE hmod, DWORD fdwSound);

/****************************************************************************/
/* GRAPHICS IMAGE MAPPING STRUCTURE                                         */
struct _stImageMap
{
   // output window parameters
   int      iNHorzOutImagePix;   // dimensions of the output
   int      iNVertOutImagePix;   //   image
   int      iIStartOutWindow;    // start and end indices of
   int      iJStartOutWindow;    //   the output window within 
   int      iIEndOutWindow;      //   the output image 
   int      iJEndOutWindow;      // 
   // input window parameters
   int      iNHorzInImagePix;    // dimensions of the input  
   int      iNVertInImagePix;    //   image                  
   int      iIStartInWindow;     // start and end indices of 
   int      iJStartInWindow;     //   the input window within
   int      iIEndInWindow;       //   the input image        
   int      iJEndInWindow;       //                          
   // mapping coefficients
   int      iIShift;             // horz and vert shift offsets 
   int      iJShift;             //   for the image mapping
   float    fIGain;              // horz and vert scaling gains 
   float    fJGain;              //   for image mapping (out-pix per in-pix)
};

/****************************************************************************/
/* GRAPHICS IMAGE MAPPING FUNCTIONS                                         */
void SetImageMapStruct(
        struct _stImageMap *pstImageMap, // image mapping structure    // output
        int    iNHorzOutImagePix,      // dimensions of the output     // input
        int    iNVertOutImagePix,      //   image                      // input
        int    iIStartOutWindow,       // start and end indices of     // input
        int    iJStartOutWindow,       //   the output window within   // input
        int    iIEndOutWindow,         //   the output image           // input
        int    iJEndOutWindow,         //                              // input
        int    iNHorzInImagePix,       // dimensions of the input      // input
        int    iNVertInImagePix,       //   image                      // input
        int    iIStartInWindow,        // start and end indices of     // input
        int    iJStartInWindow,        //   the input window within    // input
        int    iIEndInWindow,          //   the input image            // input
        int    iJEndInWindow,          //                              // input
        BOOL   bPrint);                // flag to print the data       // input

void MapImagePoint(
        int   *piIout,                   // pixel coordinates of output // output
        int   *piJout,                   //   point                     // output
        float  fIin,                     // pixel coordinates of input  // input
        float  fJin,                     //   point                     // input
        struct _stImageMap *pstImageMap);// image mapping parameters    // input 

void MapImageWindow(
        COLORREF acrOutImage[],        // output image                 // output
        COLORREF acrInImage[],         // input image                  // input
        struct _stImageMap *pstImageMap, // image mapping parameters   // input
        float    fColorCompression);   // compression factor for       // input
                                       //   scaling the output window 
                                       //   color range (unitless)
                                       //   values: 0.0 to 1.0
                                       //   0.0 => no compression, maintain full color range
                                       //   1.0 => full compression, output window is white

/****************************************************************************/
/* Graphics Mode Functions:																 */
void	lct_typedot(int i);
int	lct_GetXORMode(void);

/* Misc. Functions:																			 */
void	lct_sound(int frequency, unsigned int duration);
int	lct_syspause(int hours,int min,int sec,int hsec);
int	lct_lprtstat(int);
int	lct_sysdate(wchar_t *date, int iMaxDateStrLen, int format);
int	lct_systime(wchar_t *time, int iMaxTimeStrLen, int format);
int	lct_sysitime(int *hours, int *min, int *sec, int *hsec);
int	lct_copyfile(wchar_t *,wchar_t *);
int	lct_screendump(int iHorzDim, int iVertDim, int bFlipBlackToWhite);
void	lct_clearscreen(int color);
long	lct_MStimer(void);
int	lct_FindFirstFileName(wchar_t *wildcard, wchar_t *achFileName, int iMaxFileNameLen);
int	lct_FindNextFileName(wchar_t *achFileName, int iMaxFileNameLen);
int   lctScreenWidthPix(void);
int   lctScreenHeightPix(void);
//void  lctSetWindowHandle(HWND hwnd, HDC memdc, HDC hdc, wchar_t *sz, int iAppNameStringLen);
// above line commented out because of HWND reference.  Not all modules including this .h file define the Windows HWND type.

/****************************************************************************/
/* Serial port open and close:															 */
void *lct_serial_open(wchar_t *achPortName, long lPortSpeedBps, wchar_t *achErrorMessage, int iMsgStrLen);
int lct_serial_close(void *PortHandle);

/* Serial data read and write functions:												 */
int lct_serial_read_char(void *PortHandle);
int lct_serial_read_string(void *hfPortHandle, char *string, unsigned long ulMaxLen);
int lct_serial_read_buffer(void *hfPortHandle, char *string, unsigned long ulMaxLen);

int lct_serial_send_char(void *PortHandle, int chr);
int lct_serial_send_string(void *PortHandle, char *string);
int lct_serial_send_buffer(void *PortHandle, char *string, unsigned long uLen);

/* Hayes compatible modem control functions: 										 */
int lct_modem_hmdialrate(void *hfPortHandle, int rate);
int lct_modem_hmdialmode(void *hfPortHandle, unsigned dial_mode);
int lct_modem_hmdial(void *hfPortHandle, char *dial_buffer);
int lct_modem_hmanswer(void *hfPortHandle);
int lct_modem_hmhook(void *hfPortHandle, unsigned state);

/* Handshaking line control functions: 												 */
int lct_serial_set_dtr(void *hfPortHandle, unsigned state);
int lct_serial_set_rts(void *hfPortHandle, unsigned state);
int lct_serial_send_break(void *hfPortHandle, unsigned len);

/* Input and output buffer status functions: 										 */
int lct_serial_get_input_q_count(void *hfPortHandle);
int lct_serial_get_output_q_count(void *hfPortHandle);


/* Sockets */
void *lct_socket_open(int iServerClient, char *pszPortNumber,
                      wchar_t *pszRemoteAddress, wchar_t *achErrorMessage, int iMaxErrMsgStrLen);
int lct_socket_close(void *PortHandle, int *piSocketError);

/* Serial data read and write functions:												 */
int lct_socket_read_char(void *PortHandle, int *piSocketError);
int lct_socket_read_string(void *hfPortHandle, char *string, unsigned long ulMaxLen, int *piSocketError);
int lct_socket_read_buffer(void *hfPortHandle, char *string, unsigned long ulMaxLen, int *piSocketError);

int lct_socket_send_char(void *PortHandle, int chr, int *piSocketError);
int lct_socket_send_string(void *PortHandle, char *string, int *piSocketError);
int lct_socket_send_buffer(void *PortHandle, char *string, unsigned long uLen, int *piSocketError);

/* Input and output buffer status functions: 										 */
int lct_socket_get_input_q_count(void *hfPortHandle, int *piSocketError);

/****************************************************************************/
/* Functions used in Windows: 															 */
void EyegazeScreenUpdatesActive(int bActive);
int  lctNVisionSystems(void);

/****************************************************************************/
#ifdef __cplusplus
}			  /* Assume C declarations for C++ */
#endif  /* __cplusplus */
/****************************************************************************/

