/*****************************************************************************

File Name:       EgWin.h

Company:         LC Technologies, Inc.
                 10363 Democracy Lane
                 Fairfax, VA 22030
                 (703) 385-8800

The Eyegaze functions run as a thread under Windows.  Prior to
creating the Eyegaze thread, the application program must set up an Eyegaze
control structure, stEgControl, and an Eyegaze data structure stEgData.
The application controls the Eyegaze image processing functions through the
stEgControl structure, and the Eyegaze functions pass the eyetracking data
to the application via the stEgData buffer.

*****************************************************************************/
#ifndef EGWIN_H_INCLUDED
#define EGWIN_H_INCLUDED
/****************************************************************************/
#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

#if defined WIN32
   #include <windows.h>
#endif

#include "lctypdef.h"            /* Commonly used data type abbreviations   */

/****************************************************************************/
/* The stEgControl structure contains control and status variables to       */
/* be used by the Eyegaze application program to setup and control the      */
/* Eyegaze image processing software, and to access the Eyegaze data.       */

struct _stEgControl
{
                                  /* CONTROL INPUTS FROM APPLICATION:       */

   struct _stEgData *pstEgData;   /* pointer to the Eyegaze data structure  */
                                  /*   where EgGetData() places the next    */
                                  /*   gazepoint data sample.               */
                                  /*   This memory is allocated by the      */
                                  /*   EgInit() function. The pointer to    */
                                  /*   data structure is returned to the    */
                                  /*   application.                         */
   int    iNDataSetsInRingBuffer; /* number of gazepoint data samples in    */
                                  /*   Eyegaze's internal ring buffer.      */
                                  /*   The application must set the ring-   */
                                  /*   buffer length in the stEgControl     */
                                  /*   structure before calling EgInit()    */
   BOOL   bTrackingActive;        /* flag controls whether eyetracking is   */
                                  /*   presently on (TRUE = 1) or off       */
                                  /*   (FALSE = 0). If the flag is on when  */
                                  /*   a new camera field finishes, the     */
                                  /*   Eyegaze thread processes the image   */
                                  /*   and puts the results in the data     */
                                  /*   ring buffer; if the flag is off,     */
                                  /*   the camera field is not processed.   */
                                  /*   The application may turn this        */
                                  /*   tracking flag on or off at any       */
                                  /*   time.                                */
                                  /* NOTE for non-Windows users: This BOOL  */
                                  /*    is a 4-byte integer                 */
   int    iScreenWidthPix;        /* Pixel dimensions of the full           */
   int    iScreenHeightPix;       /*   computer screen                      */
   BOOL   bEgCameraDisplayActive; /* Obsolete, not used.                    */
   int    iEyeImagesScreenPos;    /* Obsolete, not used.                    */
   int    iCommType;              /* Communication type:       Comp Config: */
                                  /*   EG_COMM_TYPE_LOCAL,       Single     */
                                  /*   EG_COMM_TYPE_SOCKET       Double     */
   wchar_t *pszCommName;          /* Pointer to IP address of server machine*/
                                  /*   used only in Double Computer Config  */
                                  /*   Note that this is a "wide character" */
   int    iVisionSelect;          /* Reserved - set to 0 (unused)           */

                                  /* OUTPUTS TO APPLICATION:                */

   int    iNPointsAvailable;      /* number of gazepoint data samples       */
                                  /*   presently available for the applica- */
                                  /*   tion to retrieve from Eyegaze's      */
                                  /*   internal ring buffer                 */
   int    iNBufferOverflow;       /* number of irretrievably missed gaze-   */
                                  /*   point data samples, i.e. the number  */
                                  /*   of valid data points at the tail of  */
                                  /*   the ring buffer that the application */
                                  /*   did not retrieve and that Eyegaze    */
                                  /*   overwrote since the application      */
                                  /*   last called EgGetData().             */
   int    iSamplePerSec;          /* Eyegaze image processing rate -        */
                                  /*   depends on the camera field rate:    */
                                  /*   RS_170               60 Hz           */
                                  /*   CCIR                 50 Hz           */
   float  fHorzPixPerMm;          /* Eyegaze monitor scale factors          */
   float  fVertPixPerMm;          /*   (pixel / millimeter)                 */
   void   *pvEgVideoBufferAddress;/* address of the video buffer containing */
                                  /*   the most recently processed camera   */
                                  /*   image field                          */

                                  /* INTERNAL EYEGAZE VARIABLE              */

   void   *hEyegaze;              /* Eyegaze handle -- used internally by   */
                                  /*   Eyegaze to keep track of which       */
                                  /*   vision subsubsystem is in use.       */
                                  /*   (not used by application)            */
};

/****************************************************************************/
/* The stEgData structure contains the results of the Eyegaze image         */
/* processing from a given field of video camera data.                      */

struct _stEgData
{
   BOOL   bGazeVectorFound;      /* flag indicating whether the image       */
                                 /*   processing software found the eye,    */
                                 /*   i.e. found a valid glint pupil vector */
                                 /*   (TRUE = 1, FALSE = 0)                 */
                                 /* NOTE for non-Windows users: This BOOL   */
                                 /*    is a 4-byte integer                  */
   int    iIGaze;                /* integer coordinates of the user         */
   int    iJGaze;                /*   gazepoint referenced to the           */
                                 /*   eyegaze display (pixels)              */
                                 /*   0,0 origin at upper left corner       */
                                 /*   iIGaze positive rightward             */
                                 /*   iJGaze positive downward              */
   float  fPupilRadiusMm;        /* actual pupil radius (mm)                */
   float  fXEyeballOffsetMm;     /* offset of the eyeball center from       */
   float  fYEyeballOffsetMm;     /*  the camera axis (mm)                   */
                                 /*  (as measured at the real eye, not on   */
                                 /*  the camera sensor)                     */
                                 /*  Notes on polarity:                     */
                                 /*  x positive: head moves to user's right */
                                 
                                 /*  y positive: head moves up              */
   float  fFocusRangeImageTime;  /* distance from the camera sensor plane   */
                                 /*   to the camera focus plane, at the     */
                                 /*   time the camera captured the image    */
                                 /*   (mm)                                  */
   float  fFocusRangeOffsetMm;   /* range offset between the camera's true  */
                                 /*   focus plane and the corneal surface   */
                                 /*   of the eye, as measured from the      */
                                 /*   size and orientation of the corneal   */
                                 /*   reflection in the eye image -         */
                                 /*   at image time (mm)                    */
                                 /*   Polarity: A positive offset means     */
                                 /*   the eye is beyond the lens' focus     */
                                 /*   range, and the current focus range    */
                                 /*   must be increased to reduce the       */
                                 /*   error.                                */
   float  fLensExtOffsetMm;      /* distance that the lens extension would  */
                                 /*   have to be changed to bring the eye   */
                                 /*   into clear focus (millimeters)        */
                                 /*   (at image time)                       */
   ULONG  ulCameraFieldCount;    /* number of camera fields, i.e. 60ths of  */
                                 /*   a second, that have occurred since    */
                                 /*   the starting reference time (midnight */
                                 /*   January 1, this year)                 */
   double dGazeTimeSec;          /* The application time that the gazepoint */
                                 /*   was actually valid. (dGazeTimeSec     */
                                 /*   represents the original image-capture */
                                 /*   time, not the time that the gazepoint */
                                 /*   calculation was completed.)           */
   double dAppMarkTimeSec;       /* Pentium TSC counter value at the moment */
                                 /*   that the mark was incremented.        */
   int    iAppMarkCount;         /* Mark count used in logging functions.   */
   double dReportTimeSec;        /* The application time that Eyegaze       */
                                 /*   reported the gazepoint                */
};
/****************************************************************************/
struct _stEgData2
{
   BOOL   bGazeVectorFound;      /* flag indicating whether the image       */
                                 /*   processing software found the eye,    */
                                 /*   i.e. found a valid glint pupil vector */
                                 /*   (TRUE = 1, FALSE = 0)                 */
                                 /* NOTE for non-Windows users: This BOOL   */
                                 /*    is a 4-byte integer                  */
   int    iIGaze;                /* integer coordinates of the user         */
   int    iJGaze;                /*   gazepoint referenced to the           */
                                 /*   eyegaze display (pixels)              */
                                 /*   0,0 origin at upper left corner       */
                                 /*   iIGaze positive rightward             */
                                 /*   iJGaze positive downward              */
   float  fPupilRadiusMm;        /* actual pupil radius (mm)                */
   float  fXEyeballOffsetMm;     /* offset of the eyeball center from       */
   float  fYEyeballOffsetMm;     /*  the camera axis (mm)                   */
                                 /*  Notes on polarity:                     */
                                 /*  x positive: head moves to user's right */
                                 /*  y positive: head moves up              */
   float  fFocusRangeImageTime;  /* distance from the camera sensor plane   */
                                 /*   to the camera focus plane, at the     */
                                 /*   time the camera captured the image    */
                                 /*   (mm)                                  */
   float  fFocusRangeOffsetMm;   /* range offset between the camera's true  */
                                 /*   focus plane and the corneal surface   */
                                 /*   of the eye, as measured from the      */
                                 /*   size and orientation of the corneal   */
                                 /*   reflection in the eye image -         */
                                 /*   at image time (mm)                    */
                                 /*   Polarity: A positive offset means     */
                                 /*   the eye is beyond the lens' focus     */
                                 /*   range, and the current focus range    */
                                 /*   must be increased to reduce the       */
                                 /*   error.                                */
   float  fLensExtOffsetMm;      /* distance that the lens extension would  */
                                 /*   have to be changed to bring the eye   */
                                 /*   into clear focus (millimeters)        */
                                 /*   (at image time)                       */
   ULONG  ulCameraFieldCount;    /* number of camera fields, i.e. 60ths of  */
                                 /*   a second, that have occurred since    */
                                 /*   the starting reference time (midnight */
                                 /*   January 1, this year)                 */
   double dGazeTimeSec;          /* The application time that the gazepoint */
                                 /*   was actually valid. (dGazeTimeSec     */
                                 /*   represents the original image-capture */
                                 /*   time, not the time that the gazepoint */
                                 /*   calculation was completed.)           */
   double dAppMarkTimeSec;       /* Pentium TSC counter value at the moment */
                                 /*   that the mark was incremented.        */
   int    iAppMarkCount;         /* Mark count used in logging functions.   */
   double dReportTimeSec;        /* The application time that Eyegaze       */
                                 /*   reported the gazepoint                */

// NEW:
   void   *pvEgImageBufferAddress;/* address of the video buffer containing */
                                  /*   the image that produced this sample  */

/* - - - - - - - - - - - - Vergence Data - - - - - - - - - - - - - - - - - -*/

          // index 0 -> user's left eye
          // index 1 -> user's right eye

   BOOL   bGazeVergenceFound;       /* flag indicating whether a valid      */
                                    /*   binocular gaze vergence and a      */
                                    /*   corresponding 3D gazepoint were    */
                                    /*   measured                           */
   int    iVis;                     /* The vision system that produced the  */
                                    /*   most recent data.                  */

   // the following data are not valid if bGazeVergenceFound is not TRUE

   float  fTotDbeMm;                /* total distance between the two       */
                                    /*   eyes (mm)                          */
   float  fGazeVergenceAngleRad;    /* gaze vergence angle, i.e. the        */
                                    /*   angle between the two eyes'        */
                                    /*   gaze lines (rad)                   */

   float  fX3DGazePointWldMm;       /* millimeter location of the 3D        */
   float  fY3DGazePointWldMm;       /*   gazepoint within the World         */
   float  fZ3DGazePointWldMm;       /*   frame of reference (mm)            */
                                    /*   X positive right                   */
                                    /*   Y positive up                      */
                                    /*   Z positive toward user             */
                                    /*   X,Y = 0 at screen center           */
                                    /*   Z = 0 at 2D screen surface         */

   float  fI3DGazePointPix;         /* pixel location of the 3D gaze        */
   float  fJ3DGazePointPix;         /*   point                              */
   float  fK3DGazePointPix;         /*   I(x) positive right                */
                                    /*   J(-y) positive downward            */
                                    /*   K(z) positive toward user          */
                                    /*   I,J = 0 at upper left corner       */
                                    /*   K = 0 at 2D screen surface         */

/* - - - - - - - - - - - - Head Position Data - - - - - - - - - - - - - - - */
   // See _stHeadFromFace and _stHeadFromEyes, lctigaze.h

/* - - - - - - - - - - - - Gaze Geometry Data - - - - - - - - - - - - - - - */
   // see stGazeGeom in lctigaze.h

/* - - - - - - - - - - - - Eyelid Data - - - - - - - - - - - - - - - - - - -*/
   // see eyelids structure in findlids.h


   /*
   vergence data (_stGazeVerg in Z)
   stHead, including bValidFlag, x, y, z, yaw, pitch, roll, velocities x 6
   stGazeGeom? gaze line yaw and gaze line pitch angles in world coordinate frame (Nonlinear Only)
   eyelid stuff (blinks)
   image data pointer or image structure pointer (z->itv->stImage  )
   see _stEgCalDat for individual eye data (from cal), stored in control structure?
   */

};
/****************************************************************************/
struct _stEyeImageInfo
{
   unsigned char *prgbEyeImage;  /* pointer to RGB eye image data           */
   BITMAPINFO    bmiEyeImage;    /*   bitmap information                    */
   int           iWidth;
   int           iHeight;
};
/****************************************************************************/
#define MAX_RES_CAL_POINTS  13 // do not alter this constant
#define CALIBRATION_9_POINT  0
#define CALIBRATION_5_POINT  1
#define CALIBRATION_13_POINT 2

struct _stEgCalParms    // Parameters controlling the Eyegaze Calibration Procedure
{
   //-------------- Calibration Presentation Parameters ------------------

   int   iCalBackgoundColor;            // background color of the calibration
                                        //   screen (24 bit RGB)
   int   iCalPointColor;                // color of the (static) calibration 
                                        //   points (24 bit RG
   int   iNCalPoints;                   // number of calibration points displayed 
                                        //   on the screen
   float afXCalPosWinPrcnt[MAX_RES_CAL_POINTS];
   float afYCalPosWinPrcnt[MAX_RES_CAL_POINTS];
   // x and y coordinates of the calibration- 
   //   points within the calibration window
   //   (percent of the calibration window size)
   //   e.g.
   //   0.05, 0.02 => point located at
   //     5% of window width from the left,
   //     2% of window height from the top
   //   0.95, 0.98 => point located at
   //     95% of window width from the left,
   //     98% of window height from the top
   BOOL  bAnimatedCalibrationFlag;      // flag indicating whether the calibration  
                                        //   procedure is to present animated 
                                        //   calibration points (T/F)
                                        //   True  => animated cal points
                                        //   False => static cal points
   int   iCalibrationAnimationType;     // if bAnimatedCalibrationFlag is TRUE, 
                                        //   this variable selects the animation  
                                        //   type: 
                                        //   ???
   float fCalCurDiam_WinPrcnt;          // diameter of the static calibration 
                                        //   cursor 
                                        //   (percent of calibration window width)
                                        //   e.g. 0.005 => 0.5% of the cal window width

                                        //---------- Calibration-Point Collection-Control Parameters ------------

                                        // Note: The calibration procedure leaves each calibration point on display until 
                                        //       a successful calibration fixation is obtained for the point.

   float fCalPointFixTime_Ms;           // time that the user's gaze must be fixated 
                                        //   (i.e. still) before the calibration 
                                        //   procedure accepts the calibration point 
                                        //   and moves on the next point 
                                        //   (milliseconds)
   float fGazeStillnessDist_WinPrcnt;   // level of gaze stillness required during 
                                        //   the cal-point fixation to accept a
                                        //   calibration point 
                                        //   (percent of the calibration window’s 
                                        //    horizontal dimension)  e.g.
                                        //   0.001 => 0.1% of the cal window width
   int   iMissedFramesAllowed;          // number of frames that the eyetracking 
                                        //   camera may fail to track the eye within 
                                        //   the required calibration-point fixation 
                                        //   period 
   float fMinCalPointTransitionTime_Ms; // minimum time from the acceptance of one 
                                        //   calibration point before the calibration 
                                        //   procedure begins looking for the next 
                                        //   calibration-point fixation (milliseconds)
   float fPrevCalAvoidDist_WinPrcnt;    // minimum distance that the gaze must move 
                                        //   from one calibration point before the  
                                        //   calibration procedure considers it for 
                                        //   the next 
                                        //   (percent of calibration window width)
                                        //   e.g. 0.05 => 5% of the cal window width

                                        //----------- Calibration-Performance Acceptance Parameters ---------------

   float fMaxCalRmsError_WinPrcnt;      // maximum average calibration-point error 
                                        //   allowed for a calibration process to 
                                        //   be accepted 
                                        //   (percent of calibration window width)
                                        //   e.g. 0.01 => 1.0% of cal window width
   float fMaxCalPointError_WinPrcnt;    // maximum error for any individual calibration 
                                        //   point before a calibration is accepted
                                        //   (percent of calibration window width) 
                                        //   e.g. 0.02 => 2.0% of cal window width
   int   iMaxCalPointRetries;           // maximum number of attempts to retake 
                                        //   individual calibration points before 
                                        //   restarting the calibration procedure

                                        //-------------- Calibration-Results-Display Parameters -------------------

   BOOL  bShowAccuracyDisplay;          // flag controlling whether or not to display 
                                        //   the calibration accuracy at the end of
                                        //   calibration procedure (T/F)
   float fCalResultsDisplayTime_Sec;    // time that the calibration results 
                                        //   are displayed at the end of the 
                                        //   calibration procedure (seconds)
};
/****************************************************************************/
/* Function prototypes:                                                     */

// The following functions are part of the EgWin API and are made available
// via the lctigaze DLL.  Applications calling the functions within the DLL
// use the dllimport specification
#if defined BUILDDLL
#define LCTSPEC __declspec(dllexport) 
#else
#define LCTSPEC __declspec(dllimport) 
#endif

LCTSPEC int  EgInit(
        struct _stEgControl *pstEgControl);
                                 /* Initialize Eyegaze functions -          */
                                 /*   create eyetracking thread             */
LCTSPEC void EgCalibrate(
        struct _stEgControl *pstEgControl,
        HWND hwnd,
        int iCalAppType);        /* Perform the Eyegaze calibration         */
                                 /*   procedure                             */
LCTSPEC void EgCalibrate1(
        struct _stEgControl *pstEgControl,
        HWND hwnd,
        int iCalAppType);        /* Perform the Eyegaze calibration         */
                                 /*   procedure                             */
LCTSPEC void EgCalibrate2(
        struct _stEgControl *pstEgControl,
        int iCalAppType);        /* Perform the Eyegaze calibration         */
                                 /*   procedure                             */
LCTSPEC int  EgGetData(
        struct _stEgControl *pstEgControl);
                                 /* Retrieve data collected by eyegaze      */
                                 /*   image processing thread               */
                                 /*   EgGetData() returns the buffer index  */
                                 /*   of the last gazepoint sample          */
                                 /*   measured by Eyegaze                   */
LCTSPEC int  EgGetEvent(
        struct _stEgControl *pstEgControl,
        void *pv);
LCTSPEC int  EgGetVersion(void);         /* Returns the software version number     */
LCTSPEC int  EgExit(
       struct _stEgControl *pstEgControl);
                                 /* Shut down Eyegaze operation -           */
                                 /*   terminate eyetracking thread          */
LCTSPEC double EgGetApplicationStartTimeSec(void);
LCTSPEC int    EgReadCalibrationData(struct _stEgControl *pstEgControl);

/* Log file functions:                                                      */

LCTSPEC int  EgLogFileOpen(
        struct _stEgControl *pstEgControl,
        char                *pszFileName,
        char                *pszMode);
LCTSPEC void EgLogWriteColumnHeader(
        struct _stEgControl *pstEgControl);
LCTSPEC void EgLogAppendText(
        struct _stEgControl *pstEgControl,
        char                *pszText);
LCTSPEC void EgLogStart(
        struct _stEgControl *pstEgControl);
LCTSPEC void EgLogStop(
        struct _stEgControl *pstEgControl);
LCTSPEC unsigned int  EgLogMark(
        struct _stEgControl *pstEgControl);
LCTSPEC void EgLogFileClose(
        struct _stEgControl *pstEgControl);

LCTSPEC int EgGetCalibrationParms(struct _stEgControl *pstEgControl, struct _stEgCalParms *pstEgCalParms, int iSettingsType, int iCalRes);
LCTSPEC int EgSetCalibrationParms(struct _stEgControl *pstEgControl, struct _stEgCalParms *pstEgCalParms);

LCTSPEC void EgSetScreenDimensions(
        struct _stEgControl *pstEgControl,
        int    iEgMonWidthPix,
        int    iEgMonHeightPix,
        int    iEgMonHorzOffsetGdsPix,
        int    iEgMonVertOffsetGdsPix,
        int    iEgWindowWidthPix,
        int    iEgWindowHeightPix,
        int    iEgWinHorzOffsetGdsPix,
        int    iEgWinVertOffsetGdsPix);
LCTSPEC void EgInitScreenDimensions(
        int    iEgMonWidthPix,
        int    iEgMonHeightPix,
        int    iEgMonHorzOffsetGdsPix,
        int    iEgMonVertOffsetGdsPix,
        int    iEgWindowWidthPix,
        int    iEgWindowHeightPix,
        int    iEgWinHorzOffsetGdsPix, 
        int    iEgWinVertOffsetGdsPix);
LCTSPEC void EgUpdateScreenResolutions(
        int  iEgMonWidthPix,
        int  iEgMonHeightPix);
LCTSPEC void EgUpdateMonPixelOffsets(
        int  iEgMonHorzOffsetGdsPix,
        int  iEgMonVertOffsetGdsPix);
LCTSPEC void EgUpdateWindowParameters(
        int  iEgWindowWidthPix,
        int  iEgWindowHeightPix,
        int  iEgWinHorzOffsetGdsPix,
        int  iEgWinVertOffsetGdsPix);

/* Eye Image functions:                                                     */

/* Initialize the eye image display(s).                                     */
/* The argument is a pointer to an empty (uninitialized) _stEyeImageInfo    */
/* structure. The EgEyeImageInit function fills in the structure.           */
/* Returns the pointer to the EyeImage information structure.               */
LCTSPEC struct _stEyeImageInfo *EgEyeImageInit(
   struct _stEyeImageInfo *stEyeImageInfo,
   int                    iImageReductionFactor);

/* This function copies the eye image display to the screen                 */
LCTSPEC void  EgEyeImageDisplay(
   int   iVis,
   int   iX,
   int   iY,
   int   iWidth,
   int   iHeight,
   HDC   hdc);

/* Sets the display state for the eye images.  State can be TRUE or FALSE   */
LCTSPEC int   EgEyeImageDraw(int   iDisplayState);

LCTSPEC void  EgEyeImageExit(struct _stEyeImageDisp *pstEyeImageDisp);

/****************************************************************************/
/* EgCalCallback structures and prototypes for the Calibration Callback Function */

   struct _EgCalCallback
   {
      int  iXScreenPix;
      int  iYScreenPix;
      int  iStringLen;
      wchar_t achTextString[260];
      BOOL bFill;
      int  iDiameter;
      int  iColor;
   };

   /* This is the function to call to register the callback function in the application */
   LCTSPEC    BOOL bEgCalibrationRegisterCallback(
      struct _EgCalCallback *stEgCalCallback,
      BOOL(*bCalibrationCallback)(int iEventType, struct _EgCalCallback *stEgCalCallback));

/****************************************************************************/

#define EG_COMM_TYPE_LOCAL  0    /* Single computer configuration.          */
#define EG_COMM_TYPE_SOCKET 1    /* 2 computers, comm over TCP/IP.          */

#define EG_MESSAGE_TYPE_GAZEINFO       0
#define EG_MESSAGE_TYPE_MOUSEPOSITION  1
#define EG_MESSAGE_TYPE_MOUSEBUTTON    2
#define EG_MESSAGE_TYPE_KEYBD_COMMAND  3
#define EG_MESSAGE_TYPE_MOUSERELATIVE  4
#define EG_MESSAGE_TYPE_VERGENCE       5
#define EG_MESSAGE_TYPE_IMAGEDATA      81

#define EG_MESSAGE_TYPE_CALIBRATE              10
#define EG_MESSAGE_TYPE_WORKSTATION_QUERY      11
#define EG_MESSAGE_TYPE_WORKSTATION_QUERY_2    26
#define EG_MESSAGE_TYPE_WORKSTATION_RESPONSE   12
#define EG_MESSAGE_TYPE_CLEAR_SCREEN           13
#define EG_MESSAGE_TYPE_SET_COLOR              14
#define EG_MESSAGE_TYPE_SET_DIAMETER           15
#define EG_MESSAGE_TYPE_DRAW_CIRCLE            16
#define EG_MESSAGE_TYPE_DRAW_CROSS             17
#define EG_MESSAGE_TYPE_DISPLAYTEXT            18
#define EG_MESSAGE_TYPE_CALIBRATION_COMPLETE   19
#define EG_MESSAGE_TYPE_CALIBRATION_ABORTED    20
#define EG_MESSAGE_TYPE_TRACKING_ACTIVE        22
#define EG_MESSAGE_TYPE_TRACKING_INACTIVE      23
#define EG_MESSAGE_TYPE_VOICE_ACTIVE           24
#define EG_MESSAGE_TYPE_VOICE_INACTIVE         25

#define EG_MESSAGE_TYPE_BEGIN_SENDING_DATA     30
#define EG_MESSAGE_TYPE_STOP_SENDING_DATA      31
#define EG_MESSAGE_TYPE_CLOSE_AND_RECYCLE      32
#define EG_MESSAGE_TYPE_FILE_OPEN              33
#define EG_MESSAGE_TYPE_FILE_WRITE_HEADER      34
#define EG_MESSAGE_TYPE_FILE_APPENDTEXT        35
#define EG_MESSAGE_TYPE_FILE_START_RECORDING   36
#define EG_MESSAGE_TYPE_FILE_STOP_RECORDING    37
#define EG_MESSAGE_TYPE_FILE_MARK_EVENT        38
#define EG_MESSAGE_TYPE_FILE_CLOSE             39
#define EG_MESSAGE_TYPE_CALIBRATE_ABORT        21
#define EG_MESSAGE_TYPE_BEGIN_SENDING_VERGENCE 40
#define EG_MESSAGE_TYPE_STOP_SENDING_VERGENCE  41
#define EG_MESSAGE_TYPE_READ_CALIBRATION       42

#define EG_EVENT_NONE              0
#define EG_EVENT_MOUSEPOSITION     1
#define EG_EVENT_MOUSERELATIVE     2
#define EG_EVENT_MOUSEBUTTON       3
#define EG_EVENT_KEYBOARD_COMMAND  4
#define EG_EVENT_UPDATE_EYE_IMAGE  5
#define EG_EVENT_TRACKING_ACTIVE   6
#define EG_EVENT_TRACKING_INACTIVE 7
#define EG_EVENT_VOICE_ACTIVE      8
#define EG_EVENT_VOICE_INACTIVE    9

#define EG_ERROR_EYEGAZE_ALREADY_INITIALIZED      9101
#define EG_ERROR_TRACKING_TERMINATED              9102
#define EG_ERROR_MEMORY_ALLOC_FAILED              9103
#define EG_ERROR_LCT_COMM_OPEN_FAILED             9104
#define EG_ERROR_INITIALIZING_TIMER               9105
                                              
/* Constants used in calls to EgCalibrate()                                 */
enum {EG_CALIBRATE_DISABILITY_APP,
      EG_CALIBRATE_NONDISABILITY_APP};

enum {CAL_KEY_COMMAND_ESCAPE,
      CAL_KEY_COMMAND_RESTART,
      CAL_KEY_COMMAND_SKIP,
      CAL_KEY_COMMAND_ACCEPT,
      CAL_KEY_COMMAND_RETRIEVE,
      CAL_KEY_COMMAND_SPACE};

/****************************************************************************/
#ifdef __cplusplus
}          /* Assume C declarations for C++ */
#endif  /* __cplusplus */
/****************************************************************************/
#endif // defined EGWIN_H_INCLUDED
/****************************************************************************/
