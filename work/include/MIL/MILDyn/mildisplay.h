/************************************************************************/
/*
*
* Filename     :  MILDISPLAY.H
* Revision     :  10.00.2564
* Content      :  This file contains the defines necessary to use the
*                 Matrox Imaging Library display module
* Comments     :  
*
* Copyright © Matrox Electronic Systems Ltd., 1992-2014.
* All Rights Reserved
*************************************************************************/
#ifndef __MIL_DISPLAY_H__
#define __MIL_DISPLAY_H__

/************************************************************************/
/* MdispAlloc() for Windowed system                                     */
/************************************************************************/
#define M_MIL_DISPLAY_WND_CLASS_NAME MIL_TEXT("MIL Default Window")

// Maximum number of display that can be managed
#define M_MAX_VIDEO_DEVICE                            64L

/* DispNum */
#define M_DEV0                                        0L
#define M_DEV1                                        1L
#define M_DEV2                                        2L
#define M_DEV3                                        3L
#define M_DEV4                                        4L
#define M_DEV5                                        5L
#define M_DEV6                                        6L
#define M_DEV7                                        7L
#define M_DEV8                                        8L
#define M_DEV9                                        9L
#define M_DEV10                                       10L
#define M_DEV11                                       11L
#define M_DEV12                                       12L
#define M_DEV13                                       13L
#define M_DEV14                                       14L
#define M_DEV15                                       15L
#define M_DEV16                                       16L
#define M_DEV17                                       17L
#define M_DEV18                                       18L
#define M_DEV19                                       19L
#define M_DEV20                                       20L
#define M_DEV21                                       21L
#define M_DEV22                                       22L
#define M_DEV23                                       23L
#define M_DEV24                                       24L
#define M_DEV25                                       25L
#define M_DEV26                                       26L
#define M_DEV27                                       27L
#define M_DEV28                                       28L
#define M_DEV29                                       29L
#define M_DEV30                                       30L
#define M_DEV31                                       31L
#define M_DEV32                                       32L
#define M_DEV33                                       33L
#define M_DEV34                                       34L
#define M_DEV35                                       35L
#define M_DEV36                                       36L
#define M_DEV37                                       37L
#define M_DEV38                                       38L
#define M_DEV39                                       39L
#define M_DEV40                                       40L
#define M_DEV41                                       41L
#define M_DEV42                                       42L
#define M_DEV43                                       43L
#define M_DEV44                                       44L
#define M_DEV45                                       45L
#define M_DEV46                                       46L
#define M_DEV47                                       47L
#define M_DEV48                                       48L
#define M_DEV49                                       49L
#define M_DEV50                                       50L
#define M_DEV51                                       51L
#define M_DEV52                                       52L
#define M_DEV53                                       53L
#define M_DEV54                                       54L
#define M_DEV55                                       55L
#define M_DEV56                                       56L
#define M_DEV57                                       57L
#define M_DEV58                                       58L
#define M_DEV59                                       59L
#define M_DEV60                                       60L
#define M_DEV61                                       61L
#define M_DEV62                                       62L
#define M_DEV63                                       63L
#define M_CRTC0                                       0x00010000L
#define M_CRTC1                                       0x00020000L
#define M_CRTC2                                       0x00030000L
#define M_CRTC3                                       0x00040000L
#define M_CRTC4                                       0x00050000L
#define M_CRTC5                                       0x00060000L
#define M_CRTC6                                       0x00070000L
#define M_CRTC_MASK                                   0x00070000L
#define M_CRTC_OFFSET                                 (16)
#define M_CRTC_INDEX(a)                               (((a&M_CRTC_MASK)>>M_CRTC_OFFSET)-1)
#define M_DEVICE_NUMBER_MASK                          0x000003F
#define M_DEVICE_NUMBER(a)                            (a&M_DEVICE_NUMBER_MASK)
#define M_ALL_VALID_DISP_DEV_BITS                     (M_DEVICE_NUMBER_MASK | M_CRTC_MASK | M_MONITOR_BITS)
#define M_DISPLAY_NUMBER(devindex,crtcindex)          (devindex|((crtcindex+1)<<M_CRTC_OFFSET))
#define M_CENTER                                      0x00000080L
/* display positions values */
#define M_LEFT                                        0x00000100L
#define M_RIGHT                                       0x00000200L
#define M_TOP                                         0x00000400L
#define M_BOTTOM                                      0x00000800L
#define M_MONITOR_BITS                                0x00000F80
#define M_MONITOR_POSITION(p)                         ((M_NULL == (p&(~M_MONITOR_BITS)))? (p&M_MONITOR_BITS): M_NULL)
#define M_MOUSE_RESTRICTED                            0x00000001L

/* Init Flag */
#define M_REMOTE_DISPLAY                              0x00000001L
#define M_ASYNC_CUSTOM                                0x00000002L
#define M_DISPLAY_PROGRESSIVE                         0x00000004L
#define M_DISPLAY_INTERLACED                          0x00000008L
#define M_LOCAL_DISPLAY                               0x00000010L
//#define  AVAILABLE                                  0x00000020L
//#define  AVAILABLE                                  0x00000040L
//#define  AVAILABLE                                  0x00000080L
//#define  AVAILABLE                                  0x00000100L
#define M_NETWORK                                     0x00000200L
#define M_EXCLUSIVE                                   0x00000400L
//#define  AVAILABLE                                  0x00000800L
#define M_XDUAL_SCREEN                                0x00001000L
#define M_VIDEO_HD                                    0x00002000L
#define M_DUAL_SCREEN                                 0x00004000L
#define M_VIDEO_ENCODER                               0x00008000L
#define M_AUXILIARY                                   0x00010000L
//#define  AVAILABLE                                  0x00020000L
#define M_EXTERNAL                                    0x00040000L
#define M_VIDEO_HARDWARE_DEVICE                       0x00080000L
//#define  AVAILABLE                                  0x00100000L
#define M_IN_BUFFER_DISPLAY                           0x00200000L
//#define  AVAILABLE                                  0x00400000L
//#define  AVAILABLE                                  0x00800000L
#define M_WINDOWED                                    0x01000000L
//#define  AVAILABLE                                  0x02000000L
//#define  AVAILABLE                                  0x04000000L
#define M_MIL_WINDOW                                  0x08000000L
#define M_USER_WINDOW                                 0x10000000L
#define M_GDI_OVERLAY                                 0x20000000L
#define  M_FULL_SCREEN                                0x40000000L //M_LOCAL_DISPLAY_CONTROL
//NOT available (sign bit)                            0x80000000L
#define M_ALL_INIT_FLAGS                              0x4125F62FL //for basic-display
#define M_DOCUMENTED_INIT_FLAGS                       (M_WINDOWED|M_AUXILIARY|M_DUAL_SCREEN|M_VIDEO_ENCODER|M_EXCLUSIVE)
#define M_DISPLAY_LOCATION                            (M_LOCAL_DISPLAY|M_REMOTE_DISPLAY)
#define M_IS_EXCLUSIVE(InitFlag)                      (InitFlag&M_EXCLUSIVE)

/************************************************************************/
/* MdispInquire() / MdispControl() Types                                */
/************************************************************************/
#define M_LOCAL_DISPLAY_CONTROL                       0X40000000L
#define M_REMOTE_DISPLAY_CONTROL                      0X20000000L
#define M_FORCED_LOCAL_DISPLAY_CONTROL                0x10000000L
#define M_FORCED_REMOTE_DISPLAY_CONTROL               0x08000000L
#define M_POST_CONTROL                                0x04000000L

#define M_IS_POST_CONTROL(X)                          ((X)&M_POST_CONTROL)
#define M_ORIGINAL_DISPLAY_CONTROL(X)                 ((X)&~(M_FORCED_LOCAL_DISPLAY_CONTROL|M_FORCED_REMOTE_DISPLAY_CONTROL))
#define M_FORCE_ON_LOCAL_DISPLAY(X)                   ((X) | M_FORCED_LOCAL_DISPLAY_CONTROL)
#define M_FORCE_ON_REMOTE_DISPLAY(X)                  ((X) | M_FORCED_REMOTE_DISPLAY_CONTROL)
#define M_IS_FORCED_ON_LOCAL_DISPLAY(X)               (((X) & M_FORCED_LOCAL_DISPLAY_CONTROL))
#define M_IS_FORCED_ON_REMOTE_DISPLAY(X)              (((X) & M_FORCED_REMOTE_DISPLAY_CONTROL))

// Shared controls/inquires with other modules (not all listed)
#define M_SIZE_X                                      1536L //M_LOCAL_DISPLAY_CONTROL // 0x600
#define M_SIZE_Y                                      1537L //M_LOCAL_DISPLAY_CONTROL // 0x601
#define M_SIZE_Z                                      1538L //M_LOCAL_DISPLAY_CONTROL // 0x602
#define M_SIZE_BAND                                   1005L //M_LOCAL_DISPLAY_CONTROL
#define M_SIZE_BAND_LUT                               1006L //M_REMOTE_DISPLAY_CONTROL
#define M_SIZE_BIT                                    1007L //M_REMOTE_DISPLAY_CONTROL
#define M_TYPE                                        1008L //M_REMOTE_DISPLAY_CONTROL
#define M_NUMBER                                      1009L //M_LOCAL_DISPLAY_CONTROL
#define M_INIT_FLAG                                   1012L //M_LOCAL_DISPLAY_CONTROL
#define M_ATTRIBUTE                                   1013L //M_REMOTE_DISPLAY_CONTROL
#define M_SIGN                                        1014L //M_REMOTE_DISPLAY_CONTROL
#define M_THREAD_PRIORITY                             1019L //M_LOCAL_DISPLAY_CONTROL // Already defined in mil.h
#define M_NEED_UPDATE                                 1020L //M_REMOTE_DISPLAY_CONTROL
#define M_ENCODER_START                               1025L //M_LOCAL_DISPLAY_CONTROL
#define M_ENCODER                                     M_ENCODER_START
#define M_ENCODER_MODE                                1026L //M_LOCAL_DISPLAY_CONTROL
#define M_ENCODER_TYPE                                1027L //M_LOCAL_DISPLAY_CONTROL
#define M_ENCODER_SYNC_SOURCE                         1028L //M_LOCAL_DISPLAY_CONTROL
#define M_ENCODER_SYNC                                M_ENCODER_SYNC_SOURCE
#define M_ENCODER_PEDESTAL                            1029L //M_LOCAL_DISPLAY_CONTROL
#define M_ENCODER_FILTER                              1030L //M_LOCAL_DISPLAY_CONTROL
#define M_ENCODER_RGB_SYNC                            1031L //M_LOCAL_DISPLAY_CONTROL
#define M_ENCODER_INFO_MODE                           1032L //M_LOCAL_DISPLAY_CONTROL
#define M_ENCODER_RESET                               1033L //M_LOCAL_DISPLAY_CONTROL   //1034 reseved
#define M_ENCODER_SELECT_FIELD_ODD                    1037L //M_LOCAL_DISPLAY_CONTROL
#define M_ENCODER_SELECT_FIELD_EVEN                   1038L //M_LOCAL_DISPLAY_CONTROL
#define M_ENCODER_CONTRAST                            1039L //M_LOCAL_DISPLAY_CONTROL
#define M_ENCODER_BRIGHTNESS                          1040L //M_LOCAL_DISPLAY_CONTROL
#define M_ENCODER_SATURATION                          1041L //M_LOCAL_DISPLAY_CONTROL
#define M_ENCODER_HUE                                 1042L //M_LOCAL_DISPLAY_CONTROL
#define M_ENCODER_OVR_SHOW                            1043L //M_LOCAL_DISPLAY_CONTROL
#define M_ENCODER_OVR_LUT                             1044L //M_LOCAL_DISPLAY_CONTROL
#define M_ENCODER_OVR_BUF_ID                          1045L //M_LOCAL_DISPLAY_CONTROL

#define M_ENCODER_STATE                               M_ENCODER
#define M_SYNC_TYPE                                   M_ENCODER_RGB_SYNC
#define M_ENCODER_SYNC_LOCK                           M_ENCODER_SYNC_SOURCE
#define M_ENCODER_SYNC_FIELDS                         1047L //M_REMOTE_DISPLAY_CONTROL
#define M_ENCODER_CLOCK                               1048L //M_REMOTE_DISPLAY_CONTROL
#define M_ENCODER_END                                 M_ENCODER_CLOCK
#define M_Q_FACTOR                                    2564L //M_REMOTE_DISPLAY_CONTROL    // Already defined in mil.h
#define M_MODIFICATION_COUNT                          5010L //M_REMOTE_DISPLAY_CONTROL    // Already defined in mil.h

/************************************************************************/
/* MdispInquire() Types MIL_ID range start                              */
/************************************************************************/
#define M_DISP_INQUIRE_MIL_ID_START                   1100L
// Inquire value with MIL_ID type must be in that range : 1100-1199
#define M_OWNER_SYSTEM                                1101L //M_REMOTE_DISPLAY_CONTROL // Already define in mil.h
#define M_LUT_ID                                      1102L //M_REMOTE_DISPLAY_CONTROL // Already define in mil.h
#define M_SELECTED                                    1103L //M_REMOTE_DISPLAY_CONTROL
#define M_OVERLAY_LUT                                 1104L //M_REMOTE_DISPLAY_CONTROL
#define M_WINDOW_BUF_ID                               1105L //M_REMOTE_DISPLAY_CONTROL
#define M_OVERLAY_ID                                  1106L //M_REMOTE_DISPLAY_CONTROL
#define M_ASSOCIATED_LUT                              1110L //M_REMOTE_DISPLAY_CONTROL
#define M_AUXILIARY_BUF_ID                            1115L //M_LOCAL_DISPLAY_CONTROL
#define M_BUFFER_TO_RECYCLE                           1117L
#define M_COMPLEX_BUFFER                              1118L
#define M_MEGA_BUFFER_CHILD                           1119L
#define M_UNDERLAY_ID                                 1120L //M_LOCAL_DISPLAY_CONTROL
#define M_DISPLAY_SURFACE                             1121L //M_LOCAL_DISPLAY_CONTROL
#define M_PRIMARY_SURFACE_ID                          1122L //M_REMOTE_DISPLAY_CONTROL
#define M_USER_OVERLAY_ID                             1123L //M_REMOTE_DISPLAY_CONTROL
#define M_USER_BUFFER_SHARED_WITH_SCHEME              1124L
#define M_COMPRESSED_DISPLAY_SURFACE                  1125L //M_REMOTE_DISPLAY_CONTROL
#define M_ASYNCHRONOUS_DISPLAY_SURFACE                1126L //M_REMOTE_DISPLAY_CONTROL
#define M_DISP_INQUIRE_MIL_ID_END                     1199L
/************************************************************************/
/* MdispInquire() Types MIL_ID range end                                */
/************************************************************************/


#define M_PAN_X                                       3000L //M_LOCAL_DISPLAY_CONTROL
#define M_PAN_Y                                       3001L //M_LOCAL_DISPLAY_CONTROL
#define M_ZOOM_X                                      3002L //M_LOCAL_DISPLAY_CONTROL
#define M_ZOOM_Y                                      3003L //M_LOCAL_DISPLAY_CONTROL
#define M_HARDWARE_PAN                                3004L //M_LOCAL_DISPLAY_CONTROL
#define M_HARDWARE_ZOOM                               3005L //M_LOCAL_DISPLAY_CONTROL
#define M_KEY_MODE                                    3007L //M_REMOTE_DISPLAY_CONTROL
#define M_KEY_CONDITION                               3008L //M_REMOTE_DISPLAY_CONTROL
#define M_KEY_MASK                                    3009L //M_REMOTE_DISPLAY_CONTROL
#define M_KEY_SUPPORTED                               3011L //M_REMOTE_DISPLAY_CONTROL
#define M_VGA_BUF_ID                                  3012L //M_LOCAL_DISPLAY_CONTROL
#define M_TRANSPARENT_COLOR                           0x01000060L //M_LOCAL_DISPLAY_CONTROL
#define M_OVERLAY                                     M_OVR //(0x00000080) M_REMOTE_DISPLAY_CONTROL
#define M_INTERPOLATION_MODE                          3018L //M_LOCAL_DISPLAY_CONTROL
#define M_HOOK_OFFSET                                 3019L
#define M_FRAME_START_HANDLER_PTR                     3020L//needed for mil.dll? -applicationinfo
#define M_FRAME_START_HANDLER_USER_PTR                3021L//needed for mil.dll? -applicationinfo
#define M_OVERLAY_SHOW                                3023L //M_REMOTE_DISPLAY_CONTROL
#define M_DISPLAY_MODE                                3029L //M_LOCAL_DISPLAY_CONTROL
#define M_PIXEL_FORMAT                                3032L //M_LOCAL_DISPLAY_CONTROL
#define M_LUT_SUPPORTED                               3033L //M_REMOTE_DISPLAY_CONTROL
#define M_WINDOW_ZOOM                                 3051L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_RESIZE                               3052L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_OVERLAP                              3053L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_SCROLLBAR                            3054L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_PROTECT_AREA                         3056L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_TITLE_BAR                            3057L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_MENU_BAR                             3058L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_TITLE_BAR_CHANGE                     3059L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_MENU_BAR_CHANGE                      3060L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_MOVE                                 3061L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_SYSBUTTON                            3062L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_MINBUTTON                            3063L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_MAXBUTTON                            3064L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_PALETTE_NOCOLLAPSE                   3069L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_RANGE                                3072L //M_LOCAL_DISPLAY_CONTROL // Already defined in mil.h
//#define Available                                   3076L 
#define M_WINDOW_UPDATE_ON_PAINT                      3081L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_PAINT                                3083L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_INITIAL_POSITION_X                   3088L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_INITIAL_POSITION_Y                   3089L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_HANDLE                               3110L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_OFFSET_X                             3111L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_OFFSET_Y                             3112L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_SIZE_X                               3113L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_SIZE_Y                               3114L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_PAN_X                                3115L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_PAN_Y                                3116L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_ZOOM_X                               3117L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_ZOOM_Y                               3118L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_VISIBLE_OFFSET_X                     3141L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_VISIBLE_OFFSET_Y                     3142L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_VISIBLE_SIZE_X                       3145L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_VISIBLE_SIZE_Y                       3146L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_ACTIVE                               3149L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_ENABLE                               3150L //M_LOCAL_DISPLAY_CONTROL
#define M_PALETTE_HANDLE                              3151L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_THREAD_HANDLE                        3152L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_THREAD_ID                            3153L //M_LOCAL_DISPLAY_CONTROL
#define M_KEYBOARD_USE                                3155L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_SYNC_UPDATE                          3165L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_PAINT_MESSAGES                       3172L //M_LOCAL_DISPLAY_CONTROL
#define M_VIEW_MODE                                   3182L //M_REMOTE_DISPLAY_CONTROL
#define M_VIEW_BIT_SHIFT                              3183L //M_REMOTE_DISPLAY_CONTROL
#define M_NO_TEARING                                  3188L //M_LOCAL_DISPLAY_CONTROL
#define M_ZOOM_MAX_X                                  3190L //M_LOCAL_DISPLAY_CONTROL
#define M_ZOOM_MIN_X                                  3191L //M_LOCAL_DISPLAY_CONTROL
#define M_ZOOM_MAX_Y                                  3192L //M_LOCAL_DISPLAY_CONTROL
#define M_ZOOM_MIN_Y                                  3193L //M_LOCAL_DISPLAY_CONTROL
#define M_DUAL_ZOOM                                   3197L //M_LOCAL_DISPLAY_CONTROL
#define M_DUAL_ZOOM_SUPPORTED                         3198L //M_LOCAL_DISPLAY_CONTROL
#define M_UPDATE                                      3199L //M_REMOTE_DISPLAY_CONTROL
#define M_SYNCHRONOUS                                 0x00000001L
#define M_ASYNCHRONOUS                                0x00000002L
//      M_UPDATE+M_SYNCHRONOUS                        3200L //M_REMOTE_DISPLAY_CONTROL
//      M_UPDATE+M_ASYNCHRONOUS                       3201L //M_REMOTE_DISPLAY_CONTROL
#define M_WINDOW_VISIBLE                              3202L //M_LOCAL_DISPLAY_CONTROL

#define M_AUXILIARY_KEEP_DISPLAY_ALIVE                3207L //M_LOCAL_DISPLAY_CONTROL
#define M_AUXILIARY_KEEP_UNDERLAY_ALIVE               3211L //M_LOCAL_DISPLAY_CONTROL
#define M_SELECT_VIDEO_SOURCE                         3210L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_TRANSPARENCY                         3212L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_TRANSPARENT_COLOR                    3213L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_ALPHA_BLENDING                       3214L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_ALPHA_VALUE                          3215L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_LAYERED                              3216L //M_LOCAL_DISPLAY_CONTROL
#define M_RESTRICTED_MOUSE_ACTIVATED                  3217L //M_LOCAL_DISPLAY_CONTROL
#define M_INTERACTIVE_MODE                            3218L //M_LOCAL_DISPLAY_CONTROL
#define M_MOUSE_USE                                   3219L //M_LOCAL_DISPLAY_CONTROL
#define M_SOMEONE_HOOKED_TO_ANNOTATIONS_DRAW          3220L //M_LOCAL_DISPLAY_CONTROL
#define M_REGION_OUTSIDE_SHOW                         3221L //M_LOCAL_DISPLAY_CONTROL
#define M_REGION_INSIDE_SHOW                          3222L //M_LOCAL_DISPLAY_CONTROL
#define M_REGION_OUTSIDE_COLOR                        3223L //M_LOCAL_DISPLAY_CONTROL
#define M_REGION_INSIDE_COLOR                         3224L //M_LOCAL_DISPLAY_CONTROL
#define M_UNIFIED_ZOOM                                3225L //M_LOCAL_DISPLAY_CONTROL
#define M_ACTUAL_SIZE                                 3226L //M_LOCAL_DISPLAY_CONTROL
#define M_HANDLE_MESSAGES                             3227L //M_LOCAL_DISPLAY_CONTROL
#define M_MOUSE_CURSOR_CHANGE                         3229L //M_LOCAL_DISPLAY_CONTROL
#define M_HANDLE_SET_CURSOR_MESSAGE                   M_MOUSE_CURSOR_CHANGE

/******************************************************************************/

#if OldDefinesSupport
#define M_KEY_COLOR                                   M_TRANSPARENT_COLOR
MIL_DEPRECATED(M_KEY_COLOR, 1000)
#define M_WINDOW_OVR_WRITE                            M_OVERLAY
MIL_DEPRECATED(M_WINDOW_OVR_WRITE, 1000)
#define M_WINDOW_OVR_BUF_ID                           M_OVERLAY_ID
MIL_DEPRECATED(M_WINDOW_OVR_BUF_ID, 1000)
#define M_WINDOW_OVR_SHOW                             M_OVERLAY_SHOW
MIL_DEPRECATED(M_WINDOW_OVR_SHOW, 1000)
#define M_WINDOW_OVR_LUT                              M_OVERLAY_LUT
MIL_DEPRECATED(M_WINDOW_OVR_LUT, 1000)
#define M_WINDOW_UPDATE                               M_UPDATE
MIL_DEPRECATED(M_WINDOW_UPDATE, 1000)
#define M_DISP_NUMBER                                 M_NUMBER
MIL_DEPRECATED(M_DISP_NUMBER, 1000)
#define M_DISP_FORMAT                                 M_FORMAT
MIL_DEPRECATED(M_DISP_FORMAT, 1000)
#define M_DISP_INIT_FLAG                              M_INIT_FLAG
MIL_DEPRECATED(M_DISP_INIT_FLAG, 1000)
#define M_DISP_PAN_X                                  M_PAN_X
MIL_DEPRECATED(M_DISP_PAN_X, 1000)
#define M_DISP_PAN_Y                                  M_PAN_Y
MIL_DEPRECATED(M_DISP_PAN_Y, 1000)
#define M_DISP_ZOOM_X                                 M_ZOOM_X
MIL_DEPRECATED(M_DISP_ZOOM_X, 1000)
#define M_DISP_ZOOM_Y                                 M_ZOOM_Y
MIL_DEPRECATED(M_DISP_ZOOM_Y, 1000)
#define M_DISP_HARDWARE_PAN                           M_HARDWARE_PAN
MIL_DEPRECATED(M_DISP_HARDWARE_PAN, 1000)
#define M_DISP_HARDWARE_ZOOM                          M_HARDWARE_ZOOM
MIL_DEPRECATED(M_DISP_HARDWARE_ZOOM, 1000)
#define M_DISP_KEY_MODE                               M_KEY_MODE
MIL_DEPRECATED(M_DISP_KEY_MODE, 1000)
#define M_DISP_KEY_CONDITION                          M_KEY_CONDITION
MIL_DEPRECATED(M_DISP_KEY_CONDITION, 1000)
#define M_DISP_KEY_MASK                               M_KEY_MASK
MIL_DEPRECATED(M_DISP_KEY_MASK, 1000)
#define M_DISP_KEY_COLOR                              M_TRANSPARENT_COLOR
MIL_DEPRECATED(M_DISP_KEY_COLOR, 1000)
#define M_DISP_MODE                                   M_DISPLAY_MODE
MIL_DEPRECATED(M_DISP_MODE, 1000)
#define M_DISP_THREAD_PRIORITY                        M_THREAD_PRIORITY
MIL_DEPRECATED(M_DISP_THREAD_PRIORITY, 1000)
#define M_DISP_INTERPOLATION_MODE                     M_INTERPOLATION_MODE
MIL_DEPRECATED(M_DISP_INTERPOLATION_MODE, 1000)
#define M_DISP_HOOK_OFFSET                            M_HOOK_OFFSET
MIL_DEPRECATED(M_DISP_HOOK_OFFSET, 1000)
#define M_DISP_KEY_SUPPORTED                          M_KEY_SUPPORTED
MIL_DEPRECATED(M_DISP_KEY_SUPPORTED, 1000)
#define M_DISP_WINDOW_ZOOM                            M_WINDOW_ZOOM
MIL_DEPRECATED(M_DISP_WINDOW_ZOOM, 1000)
#define M_DISP_WINDOW_RESIZE                          M_WINDOW_RESIZE
MIL_DEPRECATED(M_DISP_WINDOW_RESIZE, 1000)
#define M_DISP_WINDOW_OVERLAP                         M_WINDOW_OVERLAP
MIL_DEPRECATED(M_DISP_WINDOW_OVERLAP, 1000)
#define M_DISP_WINDOW_SCROLLBAR                       M_WINDOW_SCROLLBAR
MIL_DEPRECATED(M_DISP_WINDOW_SCROLLBAR, 1000)
#define M_DISP_WINDOW_UPDATE                          M_UPDATE
MIL_DEPRECATED(M_DISP_WINDOW_UPDATE, 1000)
#define M_DISP_WINDOW_PROTECT_AREA                    M_WINDOW_PROTECT_AREA
MIL_DEPRECATED(M_DISP_WINDOW_PROTECT_AREA, 1000)
#define M_DISP_WINDOW_TITLE_BAR                       M_WINDOW_TITLE_BAR
MIL_DEPRECATED(M_DISP_WINDOW_TITLE_BAR, 1000)
#define M_DISP_WINDOW_MENU_BAR                        M_WINDOW_MENU_BAR
MIL_DEPRECATED(M_DISP_WINDOW_MENU_BAR, 1000)
#define M_DISP_WINDOW_TITLE_BAR_CHANGE                M_WINDOW_TITLE_BAR_CHANGE
MIL_DEPRECATED(M_DISP_WINDOW_TITLE_BAR_CHANGE, 1000)
#define M_DISP_WINDOW_MENU_BAR_CHANGE                 M_WINDOW_MENU_BAR_CHANGE
MIL_DEPRECATED(M_DISP_WINDOW_MENU_BAR_CHANGE, 1000)
#define M_DISP_WINDOW_MOVE                            M_WINDOW_MOVE
MIL_DEPRECATED(M_DISP_WINDOW_MOVE, 1000)
#define M_DISP_WINDOW_SYSBUTTON                       M_WINDOW_SYSBUTTON
MIL_DEPRECATED(M_DISP_WINDOW_SYSBUTTON, 1000)
#define M_DISP_WINDOW_MINBUTTON                       M_WINDOW_MINBUTTON
MIL_DEPRECATED(M_DISP_WINDOW_MINBUTTON, 1000)
#define M_DISP_WINDOW_MAXBUTTON                       M_WINDOW_MAXBUTTON
MIL_DEPRECATED(M_DISP_WINDOW_MAXBUTTON, 1000)
#define M_DISP_WINDOW_PALETTE_NOCOLLAPSE              M_WINDOW_PALETTE_NOCOLLAPSE
MIL_DEPRECATED(M_DISP_WINDOW_PALETTE_NOCOLLAPSE, 1000)
#define M_DISP_WINDOW_UPDATE_WITH_MESSAGE             M_WINDOW_UPDATE_WITH_SEND_MESSAGE
MIL_DEPRECATED(M_DISP_WINDOW_UPDATE_WITH_MESSAGE, 1000)
#define M_DISP_WINDOW_PAINT                           M_WINDOW_PAINT
MIL_DEPRECATED(M_DISP_WINDOW_PAINT, 1000)
#define M_DISP_WINDOW_RANGE                           M_WINDOW_RANGE
MIL_DEPRECATED(M_DISP_WINDOW_RANGE, 1000)
#define M_DISP_WINDOW_HANDLE                          M_WINDOW_HANDLE
MIL_DEPRECATED(M_DISP_WINDOW_HANDLE, 1000)
#define M_DISP_WINDOW_OFFSET_X                        M_WINDOW_OFFSET_X
MIL_DEPRECATED(M_DISP_WINDOW_OFFSET_X, 1000)
#define M_DISP_WINDOW_OFFSET_Y                        M_WINDOW_OFFSET_Y
MIL_DEPRECATED(M_DISP_WINDOW_OFFSET_Y, 1000)
#define M_DISP_WINDOW_SIZE_X                          M_WINDOW_SIZE_X
MIL_DEPRECATED(M_DISP_WINDOW_SIZE_X, 1000)
#define M_DISP_WINDOW_SIZE_Y                          M_WINDOW_SIZE_Y
MIL_DEPRECATED(M_DISP_WINDOW_SIZE_Y, 1000)
#define M_DISP_WINDOW_PAN_X                           M_WINDOW_PAN_X
MIL_DEPRECATED(M_DISP_WINDOW_PAN_X, 1000)
#define M_DISP_WINDOW_PAN_Y                           M_WINDOW_PAN_Y
MIL_DEPRECATED(M_DISP_WINDOW_PAN_Y, 1000)
#define M_DISP_WINDOW_ZOOM_X                          M_WINDOW_ZOOM_X
MIL_DEPRECATED(M_DISP_WINDOW_ZOOM_X, 1000)
#define M_DISP_WINDOW_ZOOM_Y                          M_WINDOW_ZOOM_Y
MIL_DEPRECATED(M_DISP_WINDOW_ZOOM_Y, 1000)
#define M_DISP_WINDOW_TITLE_NAME                      M_WINDOW_TITLE_NAME
MIL_DEPRECATED(M_DISP_WINDOW_TITLE_NAME, 1000)
#define M_DISP_VISIBLE_OFFSET_X                       M_WINDOW_VISIBLE_OFFSET_X
MIL_DEPRECATED(M_DISP_VISIBLE_OFFSET_X, 1000)
#define M_DISP_VISIBLE_OFFSET_Y                       M_WINDOW_VISIBLE_OFFSET_Y
MIL_DEPRECATED(M_DISP_VISIBLE_OFFSET_Y, 1000)
#define M_DISP_VISIBLE_SIZE_X                         M_WINDOW_VISIBLE_SIZE_X
MIL_DEPRECATED(M_DISP_VISIBLE_SIZE_X, 1000)
#define M_DISP_VISIBLE_SIZE_Y                         M_WINDOW_VISIBLE_SIZE_Y
MIL_DEPRECATED(M_DISP_VISIBLE_SIZE_Y, 1000)
#define M_VISIBLE_OFFSET_X                            M_WINDOW_VISIBLE_OFFSET_X
MIL_DEPRECATED(M_VISIBLE_OFFSET_X, 1000)
#define M_VISIBLE_OFFSET_Y                            M_WINDOW_VISIBLE_OFFSET_Y
MIL_DEPRECATED(M_VISIBLE_OFFSET_Y, 1000)
#define M_VISIBLE_SIZE_X                              M_WINDOW_VISIBLE_SIZE_X
MIL_DEPRECATED(M_VISIBLE_SIZE_X, 1000)
#define M_VISIBLE_SIZE_Y                              M_WINDOW_VISIBLE_SIZE_Y
MIL_DEPRECATED(M_VISIBLE_SIZE_Y, 1000)
#define M_DISP_WINDOW_ACTIVE                          M_WINDOW_ACTIVE
MIL_DEPRECATED(M_DISP_WINDOW_ACTIVE, 1000)
#define M_DISP_WINDOW_ENABLE                          M_WINDOW_ENABLE
MIL_DEPRECATED(M_DISP_WINDOW_ENABLE, 1000)
#define M_DISP_PALETTE_HANDLE                         M_PALETTE_HANDLE
MIL_DEPRECATED(M_DISP_PALETTE_HANDLE, 1000)
#define M_DISP_WINDOW_THREAD_HANDLE                   M_WINDOW_THREAD_HANDLE
MIL_DEPRECATED(M_DISP_WINDOW_THREAD_HANDLE, 1000)
#define M_DISP_WINDOW_THREAD_ID                       M_WINDOW_THREAD_ID
MIL_DEPRECATED(M_DISP_WINDOW_THREAD_ID, 1000)
#define M_WINDOW_DIB_HEADER                           M_BITMAPINFO
MIL_DEPRECATED(M_WINDOW_DIB_HEADER, 1000)
#define M_WINDOW_UPDATE_WITH_SEND_MESSAGE             M_UPDATE_WITH_PAINT_MESSAGE
MIL_DEPRECATED(M_WINDOW_UPDATE_WITH_SEND_MESSAGE, 1000)
#define M_DDRAW_UNDERLAY_ID                           M_UNDERLAY_ID
MIL_DEPRECATED(M_DDRAW_UNDERLAY_ID, 1000)
#define M_DDRAW_UNDERLAY_SHOW                         M_UNDERLAY_SHOW
MIL_DEPRECATED(M_DDRAW_UNDERLAY_SHOW, 1000)
#define M_MAX_VGA_DEVICE                              M_MAX_VIDEO_DEVICE
MIL_DEPRECATED(M_MAX_VGA_DEVICE, 1000)
#define M_WINDOW_KEYBOARD_USE                         M_KEYBOARD_USE
MIL_DEPRECATED(M_WINDOW_KEYBOARD_USE, 1000)
#endif //OldDefinesSupport

// Display reserved area for Sys or Dev Inquire/Control
#define M_SYS_DISPLAY_START                           4501L
#define M_IS_IN_DISPLAY_SYS_RANGE(X)     (((M_STRIP_INTERMOD_BITS(X) >= M_SYS_DISPLAY_START) && ( M_STRIP_INTERMOD_BITS(X) <= M_SYS_DISPLAY_END)) || \
                                          ((M_STRIP_INTERMOD_BITS(X) >= M_SYS_DISPLAY_RANGE2_START) && (M_STRIP_INTERMOD_BITS(X) <= M_SYS_DISPLAY_RANGE2_END)))

#define M_MAIN_DDRAW_OBJECT                           M_SYS_DISPLAY_START+0L     //4501
#define M_DDRAW_AVAILABLE                             M_SYS_DISPLAY_START+2L     //4503
#define M_PRIMARY_SURFACE_FORMAT                      M_SYS_DISPLAY_START+9L     //4510
#define M_PRIMARY_SURFACE_SIZE_BITS                   M_SYS_DISPLAY_START+10L    //4511
#define M_PRIMARY_SURFACE_SIZE_X                      M_SYS_DISPLAY_START+11L    //4512
#define M_PRIMARY_SURFACE_SIZE_Y                      M_SYS_DISPLAY_START+12L    //4513
#define M_PRIMARY_SURFACE_PITCH_BYTE                  M_SYS_DISPLAY_START+13L    //4514
#define M_WINDOWED_DISPLAY_AVAILABLE                  M_SYS_DISPLAY_START+19L    //4520
#define M_UNDERLAY_LIVE_FORMAT_SUPPORTED              M_SYS_DISPLAY_START+26L    //4527
#define M_UNDERLAY_FORMAT_SUPPORTED                   M_SYS_DISPLAY_START+28L    //4529
#define M_UNDERLAY_SUPPORTED                          M_SYS_DISPLAY_START+29L    //4530
#define M_UNDERLAY_ZOOM_MAX                           M_SYS_DISPLAY_START+30L    //4531
#define M_UNDERLAY_ZOOM_MIN                           M_SYS_DISPLAY_START+31L    //4532

#define M_UNDERLAY_DOUBLE_RANGE_START                 M_SYS_DISPLAY_START+33L    //4534
#define M_UNDERLAY_DOUBLE_RANGE_END                   M_SYS_DISPLAY_START+40L    //4541

#define M_VENDOR_ID                                   M_SYS_DISPLAY_START+48L    //4549
#define M_DEVICE_ID                                   M_SYS_DISPLAY_START+49L    //4550
#define M_DIRECTX_USED_FOR_AUXILIARY                  M_SYS_DISPLAY_START+50L    //4551
#define M_SYSTEM_INDEX_OF_VGA                         M_MATROX_VGA_BOARD_INDEX
#if (M_MIL_USE_UNICODE && !M_MIL_UNICODE_API && !M_COMPILING_MILDLL) 
#define M_SUPPORTED_DISPLAY_TYPE_STRING               M_SYS_DISPLAY_START+52L+M_CLIENT_ASCII_MODE    //4553
#else
#define M_SUPPORTED_DISPLAY_TYPE_STRING               M_SYS_DISPLAY_START+52L    //4553
#endif
#define M_NEED_LOCK_UNLOCK                            M_SYS_DISPLAY_START+54L    //4555
#define M_CAN_ALLOCATE_VIDEO_MEMORY                   M_SYS_DISPLAY_START+56L    //4557
#define M_SCAN_LINE_ORDERING_MODE                     M_SYS_DISPLAY_START+58L    //4559
#define M_REFRESH_RATE                                M_SYS_DISPLAY_START+59L    //4560
#if (M_MIL_USE_UNICODE && !M_MIL_UNICODE_API && !M_COMPILING_MILDLL) 
#define M_VIDEO_DEVICE_DESCRIPTION_STRING             M_SYS_DISPLAY_START+64L+M_CLIENT_ASCII_MODE    //4565
#else
#define M_VIDEO_DEVICE_DESCRIPTION_STRING             M_SYS_DISPLAY_START+64L    //4565 
#endif
#define M_SUB_SYSTEM_ID                               M_SYS_DISPLAY_START+66L    //4567
#define M_CAN_ALLOCATE_MONO8_IN_VIDEO_MEMORY          M_SYS_DISPLAY_START+75L    //4576
#define M_D3D_AVAILABLE                               M_SYS_DISPLAY_START+77L    //4578
#define M_MAIN_D3D9_OBJECT                            M_SYS_DISPLAY_START+78L    //4579
#define M_PART_OF_DESKTOP                             M_SYS_DISPLAY_START+79L    //4580
#define M_DISPLAY_TYPE_SUPPORTED                      M_SYS_DISPLAY_START+88L    //4589
#define M_CAN_ALLOCATE_CONTIGUOUS_VIDEO_MEMORY        M_SYS_DISPLAY_START+94L    //4595
#define M_REVISION                                    M_SYS_DISPLAY_START+95L    //4596
//#define M_XVMIL_ADAPTOR_PRESENT                       M_SYS_DISPLAY_START+96L    //4597

#define M_SYS_BUFFER_HANDLER_START                    M_SYS_DISPLAY_START+96L       // 4597
#define M_REGISTER_EXTERN_BUFFER_API_MODULE           M_SYS_BUFFER_HANDLER_START+0L // 4598
#define M_UNREGISTER_EXTERN_BUFFER_API_MODULE         M_SYS_BUFFER_HANDLER_START+1L // 4599
#define M_SYS_BUFFER_HANDLER_END                      M_SYS_DISPLAY_START+99L       // 4600
#define M_IS_IN_BUFFER_HANDLER_SYS_RANGE(X)           ((M_STRIP_INTERMOD_BITS(X) >= M_SYS_BUFFER_HANDLER_START) && (M_STRIP_INTERMOD_BITS(X) <= M_SYS_BUFFER_HANDLER_END))
////////////// MAXIMUM FOR THE FIRST DISPLAY RANGE /////////+99L    //4600
#define M_SYS_DISPLAY_END                             4600L

#define M_COMPRESSION_TYPE                            5045L //M_REMOTE_DISPLAY_CONTROL // already defined in mil.h
#define M_GRAB_BUFFERS                                5095L // internal use only
#define M_GRAB_BUFFERS_NO_LOCK                        5096L // internal use only
#define M_NUMBER_OF_GRAB_IN_PROGRESS                  5097L

#define M_APP_DIRECTX_RANGE_START                    (M_APP_INQUIRE_SYS_START+300L)

// Value reserved for AppInquire/AppControl to the hardware manager (300 to 349)
#define M_MTX0_CAPABLE_DIRECTX_OBJECT                (M_APP_DIRECTX_RANGE_START+0L)      // 300
#define M_MTX0_CAPABLE_DIRECTX_OBJECT_COUNT          (M_APP_DIRECTX_RANGE_START+1L)      // 301
#define M_SYSTEM_ASSOCIATED_TO_DX_OBJECT             (M_APP_DIRECTX_RANGE_START+2L)      // 302
#define M_DIRECTX_VERSION                            (M_APP_DIRECTX_RANGE_START+3L)      // 303
#define M_D3D_RESIZE_ALL_SWAP_CHAIN                  (M_APP_DIRECTX_RANGE_START+4L)      // 304
#define M_D3D_RELEASE_ALL_SWAP_CHAIN                 (M_APP_DIRECTX_RANGE_START+5L)      // 305
#define M_IS_ONE_SURFACE_8BITS                       (M_APP_DIRECTX_RANGE_START+6L)      // 306
#define M_IS_GPU_AVAILABLE                           (M_APP_DIRECTX_RANGE_START+7L)      // 307
#define M_NO_TEARING_ON_ALL_DEVICE                   (M_APP_DIRECTX_RANGE_START+8L)      // 308
#define M_IS_ONE_DEVICE_NEED_LOCK_UNLOCK             (M_APP_DIRECTX_RANGE_START+9L)      // 309                                        
#define M_SET_BACKGROUND_COLOR                       (M_APP_DIRECTX_RANGE_START+10L)     // 310
#define M_UPDATE_READ_WRITE_VIDEO_CAPS_SYSTEM        (M_APP_DIRECTX_RANGE_START+11L)     // 311
#define M_DIRECTX_SYNC                               (M_APP_DIRECTX_RANGE_START+12L)     // 312
//#define                                            (M_APP_DIRECTX_RANGE_START+13L)     // 313
//#define                                            (M_APP_DIRECTX_RANGE_START+14L)     // 314
//#define                                            (M_APP_DIRECTX_RANGE_START+15L)     // 315
//#define                                            (M_APP_DIRECTX_RANGE_START+16L)     // 316
//#define                                            (M_APP_DIRECTX_RANGE_START+17L)     // 317
//#define                                            (M_APP_DIRECTX_RANGE_START+18L)     // 318
//#define                                            (M_APP_DIRECTX_RANGE_START+19L)     // 319
#define M_DUAL_HEAD                                  (M_APP_DIRECTX_RANGE_START+20L)     // 320
#define M_XORG_ACCELERATION                          (M_APP_DIRECTX_RANGE_START+21L)     // 321
#define M_NB_OF_VIDEO_DEVICE                         (M_APP_DIRECTX_RANGE_START+22L)     // 322
#define M_NB_OF_VGA_DEVICE                           M_NB_OF_VIDEO_DEVICE
#define M_VGA_SYSTEM_FROM_PHYSICAL_ADDRESS           (M_APP_DIRECTX_RANGE_START+23L)     // 323
#define M_USE_UNDERLAY_IN_ALL_DISPLAY                (M_APP_DIRECTX_RANGE_START+24L)     // 324
#define M_USE_DDRAW_UNDERLAY                         M_USE_UNDERLAY_IN_ALL_DISPLAY
#define M_USE_UNDERLAY                               (M_APP_DIRECTX_RANGE_START+25L)     // 325 //This define is used as an AppControl/Inquire *AND* a DispControl/Inquire //M_LOCAL_DISPLAY_CONTROL
#define M_USE_DDRAW_UNDERLAY_SURFACE                 M_USE_UNDERLAY                      // This control is used in both directX and display dll (but use internally only for display DLL)
#define M_DESKTOP_INTERACTIVE                        (M_APP_DIRECTX_RANGE_START+26L)     // 326
#define M_STOP_INTERNALS_THREADS                     (M_APP_DIRECTX_RANGE_START+27L)     // 327
#define M_NB_OF_DESKTOP_VGA_DEVICE                   (M_APP_DIRECTX_RANGE_START+29L)     // 329
#define M_VIDEO_DEVICE_ID_FROM_SYSTEM_ID             (M_APP_DIRECTX_RANGE_START+30L)     // 330
#define M_CURRENT_DIRECTX_VERSION                    (M_APP_DIRECTX_RANGE_START+31L)     // 331
#define M_NB_OF_GRAPHIC_ADAPTER                      (M_APP_DIRECTX_RANGE_START+32L)     // 332
#define M_CONVERT_RECT_TO_VIDEO_DEVICE               (M_APP_DIRECTX_RANGE_START+33L)     // 333
#define M_GPU_NO_TEARING_AVAILABLE                   (M_APP_DIRECTX_RANGE_START+34L)     // 334
//#define                                            (M_APP_DIRECTX_RANGE_START+36L)     // 336
#define M_VIDEO_DEVICE_INDEX_FROM_DX_OBJ             (M_APP_DIRECTX_RANGE_START+37L)     // 337
#define M_NB_OF_PCI_VIDEO_DEVICE                     (M_APP_DIRECTX_RANGE_START+38L)     // 338
#define M_USE_BASIC_VGA_INFO                         (M_APP_DIRECTX_RANGE_START+39L)     // 339
#define M_FIRST_VIDEO_DEVICE_ON_DESKTOP              (M_APP_DIRECTX_RANGE_START+40L)     // 340
#define M_CONFIG_CHANGE_THREAD_PRIORITY              (M_APP_DIRECTX_RANGE_START+41L)     // 341
#define M_GPU_BILINEAR_SUPPORTED                     (M_APP_DIRECTX_RANGE_START+42L)     // 342
#define M_ENUM_PCI_VIDDEV_MEMORY_RANGE               (M_APP_DIRECTX_RANGE_START+43L)     // 343 
#define M_GENERATE_CAPS_FILE                         (M_APP_DIRECTX_RANGE_START+45L)     // 345
#define M_CAN_ALLOC_NON_PAGED_IN_VIDEO_MEMORY        (M_APP_DIRECTX_RANGE_START+46L)     // 346
#define M_DELETE_VIDEO_DEVICE_INDEX                  (M_APP_DIRECTX_RANGE_START+47L)     // 347
#define M_MED_SERIES_IS_PRESENT                      (M_APP_DIRECTX_RANGE_START+48L)     // 348
#define M_DESKTOP_FORMAT                             (M_APP_DIRECTX_RANGE_START+49L)     // 349
#define M_DESKTOP_SIZE_BIT                           (M_APP_DIRECTX_RANGE_START+50L)     // 350
#define M_IS_ONE_DISPLAY_8_BITS                      (M_APP_DIRECTX_RANGE_START+51L)     // 351
#define M_IS_ONE_DISPLAY_15_BITS                     (M_APP_DIRECTX_RANGE_START+52L)     // 352
#define M_IS_ONE_DISPLAY_16_BITS                     (M_APP_DIRECTX_RANGE_START+53L)     // 353
#define M_IS_ONE_DISPLAY_24_BITS                     (M_APP_DIRECTX_RANGE_START+54L)     // 354
#define M_IS_ONE_DISPLAY_32_BITS                     (M_APP_DIRECTX_RANGE_START+55L)     // 355
#define M_CLIENT_TO_MONITOR_RECT                     (M_APP_DIRECTX_RANGE_START+56L)     // 356
#define M_APP_DIRECTX_RANGE_END                      (M_APP_DIRECTX_RANGE_START+74L)

// Display reserved area for AppInquire/Control
/*Reserve values between M_APP_DISPLAY_START and M_APP_DISPLAY_END*/
#define M_APP_DISPLAY_START                          (M_APP_INQUIRE_SYS_START+375L)
// General purpose AppInquire/AppControl for MilDisplay (375 to 450)
#define M_APP_GENERAL_DISPLAY_START                  (M_APP_DISPLAY_START)             // 375
#define M_CAN_ALLOC_MTX0                             (M_APP_GENERAL_DISPLAY_START+0L)  // 375
#define M_DISPLAY_NUMBER_ALLOCATED                   (M_APP_GENERAL_DISPLAY_START+3L)  // 378
#define M_DEFAULT_3BITS_3BANDS_DISP_BUFFER_FORMAT    (M_APP_GENERAL_DISPLAY_START+4L)  // 379
#define M_LOCK_ALL_DISPLAYS                          (M_APP_GENERAL_DISPLAY_START+5L)  // 380

#if (M_MIL_USE_UNICODE && !M_MIL_UNICODE_API && !M_COMPILING_MILDLL) //Within ASCII client apps
   #define M_TRACE_INFORMATION                        M_TRACE_ASCII_INFORMATION
#else //Inside MIL and in Backward
   #define M_TRACE_INFORMATION                       (M_APP_GENERAL_DISPLAY_START+6L)  // 381
#endif

//#define                                            (M_APP_GENERAL_DISPLAY_START+7L)  // 382
//#define                                            (M_APP_GENERAL_DISPLAY_START+8L)  // 383
#define M_TRY_LOCK_ALL_DISPLAYS                      (M_APP_GENERAL_DISPLAY_START+9L)  // 384
#define M_KILL_SCHEME                                (M_APP_GENERAL_DISPLAY_START+10L) // 385
#define M_DISP_BUFFER_NUMBER_ALLOCATED               (M_APP_GENERAL_DISPLAY_START+11L) // 386
#define M_READY_TO_BE_FREED                          (M_APP_GENERAL_DISPLAY_START+12L) // 387
#define M_USER_TRACE_TEXT                            (M_APP_GENERAL_DISPLAY_START+15L) // 390
#define M_USER_TRACE_VALUE                           (M_APP_GENERAL_DISPLAY_START+16L) // 391
#define M_RESUSCITATE_SCHEME                         (M_APP_GENERAL_DISPLAY_START+17L) // 392
#define M_NB_EXTERN_BUFFER_API_MODULE                (M_APP_GENERAL_DISPLAY_START+18L) // 393
#define M_WANT_TO_HANDLE_AUXILIARY_UPDATE            (M_APP_GENERAL_DISPLAY_START+19L) // 394
//#define                                            (M_APP_GENERAL_DISPLAY_START+20L) // 395
//#define                                            (M_APP_GENERAL_DISPLAY_START+21L) // 396
#define M_TRACE_ASCII_INFORMATION                    (M_APP_GENERAL_DISPLAY_START+22L) // 397
//#define                                            (M_APP_GENERAL_DISPLAY_START+23L) // 398
#define M_DESKTOP_COMPOSITION                        (M_APP_GENERAL_DISPLAY_START+24L) // 399
//#define                                            (M_APP_GENERAL_DISPLAY_START+25L) // 400
//#define                                            (M_APP_GENERAL_DISPLAY_START+26L) // 401
#define M_EXCLUSIVE_ON_LAST_MONITOR                  (M_APP_GENERAL_DISPLAY_START+27L) // 402
#define M_MILDISPLAY_DLL_IN_DEBUG_MODE               (M_APP_GENERAL_DISPLAY_START+28L) // 403
//#define                                            (M_APP_GENERAL_DISPLAY_START+29L) // 404
//#define                                            (M_APP_GENERAL_DISPLAY_START+30L) // 405

// End of reserved value for display
#define M_APP_DISPLAY_END                            (M_APP_INQUIRE_SYS_START+450L)

#define M_IS_IN_DIRECTX_APP_RANGE(X)                 (((M_STRIP_INTERMOD_BITS(X) >= M_APP_DIRECTX_RANGE_START) && (M_STRIP_INTERMOD_BITS(X) <= M_APP_DIRECTX_RANGE_END))   || \
                                                      M_IS_IN_DIRECTX_APP_DX_HANDLER_RANGE(X) || \
                                                      M_IS_IN_VGA_DEVICE_SYSTEM_RANGE(X))

#define M_IS_IN_DISPLAY_APP_RANGE(X)                 (((M_STRIP_INTERMOD_BITS(X) >= M_APP_DISPLAY_START) && (M_STRIP_INTERMOD_BITS(X) <= M_APP_DISPLAY_END))         || \
                                                      M_IS_IN_APP_SCREEN_MANAGER_RANGE(X))

#define M_DISP_INQUIRE_SIZEOF_DOUBLE_START            6600L
// Inquire value with a size of double, must be in that range : 6600-6699
//#define                                             (M_DISP_INQUIRE_SIZEOF_DOUBLE_START+0L)  // 6600L //M_LOCAL_DISPLAY_CONTROL
#define M_SCALE_X                                     (M_DISP_INQUIRE_SIZEOF_DOUBLE_START+1L)  // 6601L //M_LOCAL_DISPLAY_CONTROL
#define M_SCALE_Y                                     (M_DISP_INQUIRE_SIZEOF_DOUBLE_START+2L)  // 6602L //M_LOCAL_DISPLAY_CONTROL
#define M_PAN_OFFSET_X                                (M_DISP_INQUIRE_SIZEOF_DOUBLE_START+3L)  // 6603L //M_LOCAL_DISPLAY_CONTROL
#define M_PAN_OFFSET_Y                                (M_DISP_INQUIRE_SIZEOF_DOUBLE_START+4L)  // 6604L //M_LOCAL_DISPLAY_CONTROL
#define M_ZOOM_FACTOR_X                               (M_DISP_INQUIRE_SIZEOF_DOUBLE_START+5L)  // 6605L //M_LOCAL_DISPLAY_CONTROL
#define M_ZOOM_FACTOR_Y                               (M_DISP_INQUIRE_SIZEOF_DOUBLE_START+6L)  // 6606L //M_LOCAL_DISPLAY_CONTROL
#define M_REAL_ZOOM_FACTOR_X                          (M_DISP_INQUIRE_SIZEOF_DOUBLE_START+7L)  // 6607L //M_LOCAL_DISPLAY_CONTROL
#define M_REAL_ZOOM_FACTOR_Y                          (M_DISP_INQUIRE_SIZEOF_DOUBLE_START+8L)  // 6608L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_MAXIMUM_ZOOM_FACTOR                  (M_DISP_INQUIRE_SIZEOF_DOUBLE_START+9L)  // 6609L //M_LOCAL_DISPLAY_CONTROL // obsolete
#define M_WINDOW_MINIMUM_ZOOM_FACTOR                  (M_DISP_INQUIRE_SIZEOF_DOUBLE_START+10L) // 6610L //M_LOCAL_DISPLAY_CONTROL // obsolete
#define M_WINDOW_ZOOM_FACTOR_X                        (M_DISP_INQUIRE_SIZEOF_DOUBLE_START+11L) // 6611L //M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_ZOOM_FACTOR_Y                        (M_DISP_INQUIRE_SIZEOF_DOUBLE_START+12L) // 6612L //M_LOCAL_DISPLAY_CONTROL
#define M_UPDATE_RATE                                 (M_DISP_INQUIRE_SIZEOF_DOUBLE_START+13L) // 6613L //M_LOCAL_DISPLAY_CONTROL
#define M_REAL_OFFSET_X                               (M_DISP_INQUIRE_SIZEOF_DOUBLE_START+14L) // 6614L //M_LOCAL_DISPLAY_CONTROL
#define M_REAL_OFFSET_Y                               (M_DISP_INQUIRE_SIZEOF_DOUBLE_START+15L) // 6615L //M_LOCAL_DISPLAY_CONTROL
#define M_INTERACTIVE_ZOOM_MAX                        (M_DISP_INQUIRE_SIZEOF_DOUBLE_START+16L) // 6616L //M_LOCAL_DISPLAY_CONTROL
#define M_INTERACTIVE_ZOOM_MIN                        (M_DISP_INQUIRE_SIZEOF_DOUBLE_START+17L) // 6617L //M_LOCAL_DISPLAY_CONTROL
#define M_INTERACTIVE_ZOOM_INCREMENT                  (M_DISP_INQUIRE_SIZEOF_DOUBLE_START+18L) // 6618L //M_LOCAL_DISPLAY_CONTROL
#define M_INTERACTIVE_ZOOM                            (M_DISP_INQUIRE_SIZEOF_DOUBLE_START+19L) // 6619L //M_LOCAL_DISPLAY_CONTROL
////////////// MAXIMUM VALUE FOR THE DISP INQUIRE SIZEOF DOUBLE range /////////////////////////// 6699L
#define M_DISP_INQUIRE_SIZEOF_DOUBLE_END              (M_DISP_INQUIRE_SIZEOF_DOUBLE_START+99L) // 6699L


#define M_DISP_INQUIRE_SIZEOF_INT64_START             6700L
// Inquire value with a size of 64 bit, must be in that range : 6700-6799
#define M_EXTENDED_DISPLAY_SCHEME                     (M_DISP_INQUIRE_SIZEOF_INT64_START+0L) //(6700) M_LOCAL_DISPLAY_CONTROL

#define M_EXTENDED_INIT_FLAG                          6705L //M_LOCAL_DISPLAY_CONTROL
#define M_DISP_INQUIRE_SIZEOF_INT64_END               6799L

#define M_DISP_INQUIRE_UNKNOWN_SIZEOF_START           6800L
// Inquire value with unknown size of,
//            and require a user array, must be in that range : 6800-6899
#define M_DISP_INQUIRE_UNKNOWN_SIZEOF_END             6899L


#define M_DISP_INQUIRE_STRING_START                   7700L
   #if (M_MIL_USE_UNICODE && !M_MIL_UNICODE_API && !M_COMPILING_MILDLL)
      #define M_FORMAT                                (7701L|M_CLIENT_ASCII_MODE) //M_LOCAL_DISPLAY_CONTROL  // Already defined in mil.h, considered as a shared control
      #define M_TITLE                                 (7702L|M_CLIENT_ASCII_MODE) //M_LOCAL_DISPLAY_CONTROL
      #define M_WINDOW_TITLE_NAME                     M_TITLE
   #else
      #define M_FORMAT                                7701L //M_LOCAL_DISPLAY_CONTROL // Already defined in mil.h, considered as a shared control
      #define M_TITLE                                 7702L //M_LOCAL_DISPLAY_CONTROL
      #define M_WINDOW_TITLE_NAME                     M_TITLE
   #endif

   #define M_FORMAT_SIZE                              (M_STRING_SIZE + M_FORMAT) //M_LOCAL_DISPLAY_CONTROL // Already defined in mil.h, considered as a shared control
   #define M_TITLE_SIZE                               (M_STRING_SIZE + M_TITLE)  //M_LOCAL_DISPLAY_CONTROL
   #define M_WINDOW_TITLE_NAME_SIZE                   M_TITLE_SIZE

#define M_DISP_INQUIRE_STRING_END                     7799L




// MdispControl(M_REMAP_TYPE)
#define M_MULTI_BYTES                                    1L
#define M_AUTO_SCALE                                     2L
#define M_BIT_SHIFT                                      3L
#define M_LOGARITHMIC                                    4L
#define M_PIXEL_VALUES                                   5L
#define M_OPAQUE                                         0x01000058L
#define M_TRANSPARENT                                    0x01000059L
#define M_MED_SERIES                                     6L

// Mouse cursor types
//
// MdispControl(Id, M_SET_USER_CURSOR+[M_CURSOR_XYZ], (MIL_INT)hCursor)
//
#define M_CURSOR_NULL                  0L    // To hide the cursor
#define M_CURSOR_DEFAULT               1L    // Standard arrow
#define M_OS_CURSOR_ARROW              2L    // Standard arrow
#define M_OS_CURSOR_IBEAM              3L    // I-beam
#define M_OS_CURSOR_WAIT               4L    // Hourglass
#define M_OS_CURSOR_CROSS              5L    // Crosshair
#define M_OS_CURSOR_UPARROW            6L    // Vertical arrow
#define M_OS_CURSOR_SIZENWSE           7L    // Double-pointed arrow pointing northwest and southeast
#define M_OS_CURSOR_SIZENESW           8L    // Double-pointed arrow pointing northeast and southwest
#define M_OS_CURSOR_SIZEWE             9L    // Double-pointed arrow pointing west and east
#define M_OS_CURSOR_SIZENS             10L   // Double-pointed arrow pointing north and south
#define M_OS_CURSOR_SIZEALL            11L   // Four-pointed arrow pointing north, south, east, and west
#define M_OS_CURSOR_NO                 12L   // Slashed circle
#define M_OS_CURSOR_HAND               13L   // Pointing Hand
#define M_OS_CURSOR_APPSTARTING        14L   // Standard arrow and small hourglass
#define M_OS_CURSOR_HELP               15L   // Arrow and question mark
#define M_CURSOR_HAND_OPEN             16L   // Open hand
#define M_CURSOR_HAND_CLOSED           17L   // Closed hand
#define M_GRA_CURSOR_SIZENWSE          18L   // Double-pointed arrow pointing northwest and southeast 
#define M_GRA_CURSOR_SIZENESW          19L   // Double-pointed arrow pointing northeast and southwest
#define M_GRA_CURSOR_SIZEWE            20L   // Double-pointed arrow pointing west and east
#define M_GRA_CURSOR_SIZENS            21L   // Double-pointed arrow pointing north and south
#define M_GRA_CURSOR_SIZEALL           22L   // Four-pointed arrow pointing north, south, east, and west
#define M_GRA_CURSOR_SIZEALL_POINTER   23L   // Arrow and four-pointed arrow pointing north, south, east, and west
#define M_GRA_CURSOR_NO                24L   // Slashed circle
#define M_GRA_CURSOR_ROTATE            25L   // Circular arrow
#define M_GRA_CURSOR_IN_ROTATION       26L   // Quadruple circular arrows
#define M_GRA_CURSOR_HAND              27L   // Pointing Hand
// =======================================
#define M_NUMBER_OF_CURSORS            28L// *** Must be updated when adding new cursor ***
#define M_MAX_CURSOR                   63L
#define M_CURSOR_TYPE_MASK             0x0000003F

// 7872 to 7935
#define M_SET_USER_CURSOR                             7872L //(0x00001EC0) M_LOCAL_DISPLAY_CONTROL
/*Reserve next values      from                       (M_SET_USER_CURSOR+M_CURSOR_NULL)      //7872L */ //M_LOCAL_DISPLAY_CONTROL
/*                         to                         (M_SET_USER_CURSOR+M_MAX_CURSOR)       //7935L */ //M_LOCAL_DISPLAY_CONTROL

//Pseudo-ID to apply a control on all displays
#define M_ALL_DISPLAYS                                (M_PSEUDO_ID+3)

// Value to represent all scheme id. Used with M_RESUSCITATE_SCHEME control
#define M_ALL_DISPLAY_SCHEMES                         (0x80000000)

//Second range that is reserved for Display Sys or Dev Inquire/Control
/* Reserve next values     from    M_SYS_DISPLAY_RANGE2_START    8500L 
                           to      M_SYS_DISPLAY_RANGE2_END      8699L */
#define M_SYS_DISPLAY_RANGE2_START                    8500L 
//#define                                             (M_SYS_DISPLAY_RANGE2_START+0L)    //8500L
#define M_D3D_SUPPORTED_NO_TEARING_MODE               (M_SYS_DISPLAY_RANGE2_START+1L)    //8501L
#define M_NO_TEARING_DEVICE                           (M_SYS_DISPLAY_RANGE2_START+5L)    //8505L
#define M_NO_TEARING_ACTIVE_DEVICE                    (M_SYS_DISPLAY_RANGE2_START+6L)    //8506L
#define M_VGA_INFO_DISPLAY_RECT                       (M_SYS_DISPLAY_RANGE2_START+7L)    //8507L    // 
#define M_VGA_INFO_DISPLAY_DEVICE_NAME                (M_SYS_DISPLAY_RANGE2_START+10L)   //8510L    // 
#define M_CURRENT_MONITOR_RECT                        (M_SYS_DISPLAY_RANGE2_START+12L)   //8512L
#define M_CURRENT_MONITOR_WORK_RECT                   (M_SYS_DISPLAY_RANGE2_START+13L)   //8513L
#define M_VIDEO_DEVICE_SYNC                           (M_SYS_DISPLAY_RANGE2_START+14L)   //8514L
#define M_MAIN_D3D10_OBJECT                           (M_SYS_DISPLAY_RANGE2_START+19L)   //8519L
//#define                                             (M_SYS_DISPLAY_RANGE2_START+28L)   //8528L
//#define                                             (M_SYS_DISPLAY_RANGE2_START+29L)   //8529L
//#define                                             (M_SYS_DISPLAY_RANGE2_START+33L)   //8533L
#define M_IS_VIRTUAL_DEVICE                           (M_SYS_DISPLAY_RANGE2_START+37L)   //8537L
#define M_MAIN_D3D11_OBJECT                           (M_SYS_DISPLAY_RANGE2_START+39L)   //8539L
#define M_D3D_NO_TEARING                              (M_SYS_DISPLAY_RANGE2_START+43L)   //8543L
#define M_ALLOW_NONPAGED_BUFFERS_IN_VIDEO_MEMORY      (M_SYS_DISPLAY_RANGE2_START+48L)   //8548L
#define M_IS_VIDEO_DEVICE_ALIVE                       (M_SYS_DISPLAY_RANGE2_START+51L)   //8551L
//#define                                             (M_SYS_DISPLAY_RANGE2_START+52L)   //8552L
//...
//#define                                             (M_SYS_DISPLAY_RANGE2_START+99L)   //8599L
#define M_VGA_DEVICE_SYSTEM_START                     (M_SYS_DISPLAY_RANGE2_START+100L)  //8600L
#define M_VGA_DEVICE_SYSTEM                           (M_VGA_DEVICE_SYSTEM_START+0L)     //8600 to 8663
//8600L to 8663L reserved for M_VGA_DEVICE_SYSTEM
////////////// MAXIMUM FOR THE THIRD VGA DEVICE SYSTEM RANGE //////////////////////////////8663L
#define M_VGA_DEVICE_SYSTEM_END                       (M_VGA_DEVICE_SYSTEM_START+M_MAX_VIDEO_DEVICE-1) //8663L
//#define                                             (M_SYS_DISPLAY_RANGE2_START+164L)  //8664L
//...
//#define                                             (M_SYS_DISPLAY_RANGE2_START+199L)  //8699L
////////////// MAXIMUM FOR THE SECOND SYS_DISPLAY RANGE  //////////////////////////////////8699L
#define M_SYS_DISPLAY_RANGE2_END                      (M_SYS_DISPLAY_RANGE2_START+199L)  //8699L

// This range is an App inquire range not a DEV range
#define M_IS_IN_VGA_DEVICE_SYSTEM_RANGE(X)            ((M_STRIP_INTERMOD_BITS(X)>=M_VGA_DEVICE_SYSTEM_START) && \
                                                       (M_STRIP_INTERMOD_BITS(X)<=M_VGA_DEVICE_SYSTEM_END)     )  // 8600 to 8663


#define M_DIB_HANDLE                                  (M_BUF_DISPLAY_START+31L) // 9031

//Reserved values for Screen Manager 9200 - 9399
#define M_SCREEN_MANAGER_START                        9200L
//Reserved APP control/inquire values for Screen Manager 9200 - 9249
#define M_APP_SCREEN_MANAGER_START                    (M_SCREEN_MANAGER_START+0L)      // 9200L
#define M_PHYS_SHOW_TASKBAR                           (M_APP_SCREEN_MANAGER_START+0L)  // 9200L
#define M_BEST_DESKTOP_MONITOR                        (M_APP_SCREEN_MANAGER_START+1L)  // 9201L
#define M_BEST_DESKTOP_RECT                           (M_APP_SCREEN_MANAGER_START+2L)  // 9202L
#define M_BEST_EXCLUSIVE_MONITOR                      (M_APP_SCREEN_MANAGER_START+3L)  // 9203L
#define M_MULTI_MONITOR                               (M_APP_SCREEN_MANAGER_START+4L)  // 9204L
#define M_MONITORS_RECTS_LIST                         (M_APP_SCREEN_MANAGER_START+5L)  // 9205L
#define M_NEW_EXTERNAL_DISPLAY_ID                     (M_APP_SCREEN_MANAGER_START+6L)  // 9206L
#define M_DELETE_EXTERNAL_DISPLAY_ID                  (M_APP_SCREEN_MANAGER_START+7L)  // 9207L
#define M_SCREEN_ID_FROM_SYSTEM_ID                    (M_APP_SCREEN_MANAGER_START+8L)  // 9208L
#define M_SCREEN_MANAGER_SYSTEM_ID                    (M_APP_SCREEN_MANAGER_START+9L)  // 9209L
#define M_EXTERNAL_DISPLAY_NUMBER                     (M_APP_SCREEN_MANAGER_START+10L) // 9210L
#define M_NB_MONITORS_AVAILABLE                       (M_APP_SCREEN_MANAGER_START+13L) // 9213L
//available
////////////// MAXIMUM VALUE FOR THE APP control/inquire Screen Manager Range/////////// 9249L
#define M_APP_SCREEN_MANAGER_END                      (M_APP_SCREEN_MANAGER_START+49L)// 9249L

//Reserved DEV control/inquire values for dev Screen Manager 9250 - 9299
#define M_DEV_SCREEN_MANAGER_START                    (M_APP_SCREEN_MANAGER_END+1L)    // 9250L
#define M_DESKTOP_RESOLUTION                          (M_DEV_SCREEN_MANAGER_START+0L)  // 9250L
#define M_TEST_DESKTOP_RESOLUTION                     (M_DEV_SCREEN_MANAGER_START+1L)  // 9251L
#define M_DESKTOP_DEVICE_NAME                         (M_DEV_SCREEN_MANAGER_START+2L)  // 9252L
#define M_DESKTOP_DEVICE_NAME_SIZE                    (M_STRING_SIZE + M_DESKTOP_DEVICE_NAME)
#define M_EXCLUSIVE_MONITOR_RANK                      (M_DEV_SCREEN_MANAGER_START+4L)  // 9254L
#define M_IS_EXCLUSIVE_MONITOR_AVAILABLE              (M_DEV_SCREEN_MANAGER_START+5L)  // 9255L
#define M_EXTERN_DISPLAY_AVAILABLE                    (M_DEV_SCREEN_MANAGER_START+6L)  // 9256L
#define M_EXTERN_DISPLAY_SUPPORTED_FORMAT             (M_DEV_SCREEN_MANAGER_START+9L)  // 9259L
#define M_EXTERN_UNDERLAY_AVAILABLE                   (M_DEV_SCREEN_MANAGER_START+10L) // 9260L
#define M_IS_EXTERNAL_DISPLAY                         (M_DEV_SCREEN_MANAGER_START+14L) // 9264L
#define M_CURRENT_SCREEN_RECT                         (M_DEV_SCREEN_MANAGER_START+15L) // 9265L (M_CURRENT_MONITOR_RECT)
#define M_CURRENT_SCREEN_WORK_RECT                    (M_DEV_SCREEN_MANAGER_START+16L) // 9266L (M_CURRENT_MONITOR_WORK_RECT)
#define M_IS_SCREEN_FORBIDDEN                         (M_DEV_SCREEN_MANAGER_START+18L) // 9268L
#define M_IS_SCREEN_RECT_USABLE                       (M_DEV_SCREEN_MANAGER_START+19L) // 9269L
//available
////////////// MAXIMUM VALUE FOR THE APP control/inquire DEV Screen Manager Range/////// 9299L
#define M_DEV_SCREEN_MANAGER_END                      (M_DEV_SCREEN_MANAGER_START+49L)// 9299L
//Reserved but unused values for Screen Manager 9300 - 9374
#define M_SCREEN_MANAGER_END                          9374L

#define M_IS_IN_SCREEN_MANAGER_RANGE(X)               (((M_STRIP_INTERMOD_BITS(X)>=M_SCREEN_MANAGER_START) && (M_STRIP_INTERMOD_BITS(X)<=M_SCREEN_MANAGER_END)))                     // 9200 - 9399
#define M_IS_IN_APP_SCREEN_MANAGER_RANGE(X)           (((M_STRIP_INTERMOD_BITS(X)>=M_APP_SCREEN_MANAGER_START) && (M_STRIP_INTERMOD_BITS(X)<=M_APP_SCREEN_MANAGER_END)))             // 9200 - 9249
#define M_IS_IN_DEV_SCREEN_MANAGER_RANGE(X)           (((M_STRIP_INTERMOD_BITS(X)>=M_DEV_SCREEN_MANAGER_START) && (M_STRIP_INTERMOD_BITS(X)<=M_DEV_SCREEN_MANAGER_END)))             // 9250 - 9299

/*#define M_SYS_BUFFER_HANDLER_START                    9375L                         // 9375L
#define M_REGISTER_EXTERN_BUFFER_API_MODULE           M_SYS_BUFFER_HANDLER_START+0L // 9375L
#define M_UNREGISTER_EXTERN_BUFFER_API_MODULE         M_SYS_BUFFER_HANDLER_START+1L // 9376L
#define M_SYS_BUFFER_HANDLER_END                      9399L                         // 9399L
#define M_IS_IN_BUFFER_HANDLER_SYS_RANGE(X)           ((M_STRIP_INTERMOD_BITS(X) >= M_SYS_BUFFER_HANDLER_START) && (M_STRIP_INTERMOD_BITS(X) <= M_SYS_BUFFER_HANDLER_END))*/

//Reserved APP control/inquire values for Directx and Display 9400 - 9499
#define M_DUAL_DISP_DX_APP_RANGE_START                9400L
#define M_RESET_CRTC                                  (M_DUAL_DISP_DX_APP_RANGE_START+0L)  // 9400L
#define M_NEW_VIDEO_DEVICE_ADDED                      (M_DUAL_DISP_DX_APP_RANGE_START+6L)  // 9406L
#define M_VIDEO_DEVICE_REMOVED                        (M_DUAL_DISP_DX_APP_RANGE_START+7L)  // 9407L
//#define                                             (M_DUAL_DISP_DX_APP_RANGE_START+8L)  // 9408L
//...
//#define                                             (M_DUAL_DISP_DX_APP_RANGE_START+99L) // 9499L
////////////// MAXIMUM VALUE FOR THE APP control/inquire range for Directx and Display/////// 9499L
#define M_DUAL_DISP_DX_APP_RANGE_END                  (M_DUAL_DISP_DX_APP_RANGE_START+99L) // 9499L
#define M_IS_IN_DUAL_DISP_DX_APP_RANGE(X)             (((M_STRIP_INTERMOD_BITS(X) >= M_DUAL_DISP_DX_APP_RANGE_START) && (M_STRIP_INTERMOD_BITS(X) <= M_DUAL_DISP_DX_APP_RANGE_END)))     // 9400 - 9499

#define M_DIRECTX_APP_DX_HANDLER_START                9500L
#define M_DIRECTX_CAN_CREATE_OVER_HOST_MEMORY         (M_DIRECTX_APP_DX_HANDLER_START+0L)  // 9500L
#define M_MTX0_SERVICE                                (M_DIRECTX_APP_DX_HANDLER_START+1L)  // 9501L
#define M_DIRECTX_VIDEO_BUFFER_MAPPABLE               (M_DIRECTX_APP_DX_HANDLER_START+2L)  // 9502L
#define M_KEEP_POLLING_ON_MODE_CHANGED                (M_DIRECTX_APP_DX_HANDLER_START+3L)  // 9503L
// available
////////////// MAXIMUM VALUE FOR THE APP DX HANDLER range /////////////////////////////////// 9549L
#define M_DIRECTX_APP_DX_HANDLER_END                  (M_DIRECTX_APP_DX_HANDLER_START+49L) // 9549L
#define M_IS_IN_DIRECTX_APP_DX_HANDLER_RANGE(X)       ((M_STRIP_INTERMOD_BITS(X) >= M_DIRECTX_APP_DX_HANDLER_START) && (M_STRIP_INTERMOD_BITS(X) <= M_DIRECTX_APP_DX_HANDLER_END))       // 9500 - 9549

//Reserved values for DispControl
#define M_DISP_CONTROL_START                          10000L
#define M_NO_TEARING_SKIP_MODE                        (M_DISP_CONTROL_START+1L) //(10001) M_LOCAL_DISPLAY_CONTROL
#define M_NO_SKIP                                     (M_DISP_CONTROL_START+2L) //(10002) M_LOCAL_DISPLAY_CONTROL
#define M_SKIP_OLDEST                                 (M_DISP_CONTROL_START+3L) //(10003) M_LOCAL_DISPLAY_CONTROL
#define M_SKIP_NEWEST                                 (M_DISP_CONTROL_START+4L) //(10004) M_LOCAL_DISPLAY_CONTROL
#define M_DISPLAY_SCHEME                              (M_DISP_CONTROL_START+5L) //(10005) M_LOCAL_DISPLAY_CONTROL
#define M_OVERLAY_CLEAR                               (M_DISP_CONTROL_START+6L) //(10006) M_REMOTE_DISPLAY_CONTROL
#define M_FORCE_MONO_OVR                              (M_DISP_CONTROL_START+7L) //(10007) M_REMOTE_DISPLAY_CONTROL
#define M_D3D_VSYNC_INTERVAL_ONE                      (M_DISP_CONTROL_START+8L) //(10008) M_LOCAL_DISPLAY_CONTROL
#define M_D3D_VSYNC_INTERVAL_TWO                      (M_DISP_CONTROL_START+9L) //(10009) M_LOCAL_DISPLAY_CONTROL
#define M_D3D_VSYNC_INTERVAL_THREE                    (M_DISP_CONTROL_START+10L) //(10010) M_LOCAL_DISPLAY_CONTROL
#define M_D3D_VSYNC_INTERVAL_FOUR                     (M_DISP_CONTROL_START+11L) //(10011) M_LOCAL_DISPLAY_CONTROL
#define M_DIRECTX_VIDEO_DEVICE_ID                     (M_DISP_CONTROL_START+12L) //(10012) M_LOCAL_DISPLAY_CONTROL
#define M_VGA_INDEX                                   M_DIRECTX_VIDEO_DEVICE_ID
#define M_GDI_COMPATIBLE_OVERLAY                      (M_DISP_CONTROL_START+13L) //(10013) M_REMOTE_DISPLAY_CONTROL
#define M_HSCROLL_VISIBLE                             (M_DISP_CONTROL_START+14L) //(10014) M_LOCAL_DISPLAY_CONTROL
#define M_VSCROLL_VISIBLE                             (M_DISP_CONTROL_START+15L) //(10015) M_LOCAL_DISPLAY_CONTROL
#define M_SRC_UPDATE_REGION_OFFSET_X                  (M_DISP_CONTROL_START+16L) //(10016) M_LOCAL_DISPLAY_CONTROL
#define M_SRC_UPDATE_REGION_OFFSET_Y                  (M_DISP_CONTROL_START+17L) //(10017) M_LOCAL_DISPLAY_CONTROL
#define M_NEAREST_COLOR                               (M_DISP_CONTROL_START+18L) //(10018) M_LOCAL_DISPLAY_CONTROL
#define M_CENTER_DISPLAY                              (M_DISP_CONTROL_START+19L) //(10019) M_LOCAL_DISPLAY_CONTROL
#define M_USE_RESERVED_COLOR                          (M_DISP_CONTROL_START+20L) //(10020) M_LOCAL_DISPLAY_CONTROL
#define M_LOWER_RESERVED_COLOR_START                  (M_DISP_CONTROL_START+21L) //(10021) M_LOCAL_DISPLAY_CONTROL
#define M_LOWER_RESERVED_COLOR_END                    (M_DISP_CONTROL_START+22L) //(10022) M_LOCAL_DISPLAY_CONTROL
#define M_HIGHER_RESERVED_COLOR_START                 (M_DISP_CONTROL_START+23L) //(10023) M_LOCAL_DISPLAY_CONTROL
#define M_HIGHER_RESERVED_COLOR_END                   (M_DISP_CONTROL_START+24L) //(10024) M_LOCAL_DISPLAY_CONTROL
#define M_PREFERED_DISPLAY_FORMAT                     (M_DISP_CONTROL_START+25L) //(10025) M_LOCAL_DISPLAY_CONTROL
#define M_SAVE_INTERNAL_BUFFERS                       (M_DISP_CONTROL_START+26L) //(10026) M_LOCAL_DISPLAY_CONTROL
#define M_HARDWARE_LUT_AVAILABLE                      (M_DISP_CONTROL_START+27L) //(10027) M_LOCAL_DISPLAY_CONTROL
#define M_IS_IN_MEGA_BUFFER_MODE                      (M_DISP_CONTROL_START+28L) //(10028) M_LOCAL_DISPLAY_CONTROL
#define M_UPDATE_THREAD_PRIORITY                      (M_DISP_CONTROL_START+29L) //(10029) M_LOCAL_DISPLAY_CONTROL
#define M_VISIBLE_BUFFER_RECT_SIZE_X                  (M_DISP_CONTROL_START+30L) //(10030) M_LOCAL_DISPLAY_CONTROL
#define M_VISIBLE_BUFFER_RECT_SIZE_Y                  (M_DISP_CONTROL_START+31L) //(10031) M_LOCAL_DISPLAY_CONTROL
#define M_NO_TEARING_ACTIVE                           (M_DISP_CONTROL_START+32L) //(10032) M_LOCAL_DISPLAY_CONTROL
#define M_FORCE_DISPLAY_RESOLUTION                    (M_DISP_CONTROL_START+33L) //(10033) M_LOCAL_DISPLAY_CONTROL
#define M_USE_INTERNAL_BUFFER_FOR_GRAB_CONTINUOUS     (M_DISP_CONTROL_START+34L) //(10034) M_REMOTE_DISPLAY_CONTROL
#define M_NO_TEARING_MODE                             (M_DISP_CONTROL_START+35L) //(10035) M_REMOTE_DISPLAY_CONTROL
#define M_UNDERLAY_UPDATE_STATE                       (M_DISP_CONTROL_START+36L) //(10036) M_LOCAL_DISPLAY_CONTROL
#define M_USE_PRIMARY_SURFACE                         (M_DISP_CONTROL_START+37L) //(10037) M_LOCAL_DISPLAY_CONTROL
#define M_UPDATE_RATE_DIVIDER                         (M_DISP_CONTROL_START+38L) //(10038) M_REMOTE_DISPLAY_CONTROL
#define M_STOP_UPDATE                                 (M_DISP_CONTROL_START+39L) //(10039) M_REMOTE_DISPLAY_CONTROL
#define M_CAN_DO_HARDWARE_COMPOSITION                 (M_DISP_CONTROL_START+40L) //(10040) M_REMOTE_DISPLAY_CONTROL
#define M_MIN_MAX_OF_SELECTED_BUFFER_CHANGED          (M_DISP_CONTROL_START+41L) //(10041) M_REMOTE_DISPLAY_CONTROL
#define M_UPDATE_MEGA_BUFFER_CHILD_POSITION           (M_DISP_CONTROL_START+42L) //(10042) M_LOCAL_DISPLAY_CONTROL
#define M_UPDATE_DX_VERSION_ON_TITLEBAR               (M_DISP_CONTROL_START+43L) //(10043) M_LOCAL_DISPLAY_CONTROL
#define M_DEBUG_COLOR                                 (M_DISP_CONTROL_START+44L) //(10044) M_LOCAL_DISPLAY_CONTROL
#define M_LOSING_VIDEO_MEMORY                         (M_DISP_CONTROL_START+45L) //(10045) M_LOCAL_DISPLAY_CONTROL
#define M_VIDEO_MEMORY_RESTORED                       (M_DISP_CONTROL_START+46L) //(10046) M_LOCAL_DISPLAY_CONTROL
#define M_DISPLAY_SYNC                                (M_DISP_CONTROL_START+47L) //(10047) M_LOCAL_DISPLAY_CONTROL
#define M_DISPLAY_USAGE                               (M_DISP_CONTROL_START+48L) //(10048) M_REMOTE_DISPLAY_CONTROL
#define M_GENERATE_PENDING_HOOKS                      (M_DISP_CONTROL_START+49L) //(10049) M_LOCAL_DISPLAY_CONTROL
#define M_UPDATE_GRAB_TYPE                            (M_DISP_CONTROL_START+50L) //(10050) M_REMOTE_DISPLAY_CONTROL
#define M_GRAB_TYPE_ON_TITLE_BAR                      (M_DISP_CONTROL_START+51L) //(10051) M_LOCAL_DISPLAY_CONTROL
#define M_VIDEO_DEVICE_WAS_MODIFIED                   (M_DISP_CONTROL_START+52L) //(10052) M_LOCAL_DISPLAY_CONTROL
#define M_PAR_ENCODER_WAIT_FIELD                      (M_DISP_CONTROL_START+53L) //(10053) M_LOCAL_DISPLAY_CONTROL
#define M_USE_YCBCR_RANGE                             (M_DISP_CONTROL_START+54L) //(10054) M_REMOTE_DISPLAY_CONTROL
#define M_RESTRICT_CURSOR                             (M_DISP_CONTROL_START+55L) //(10055) M_LOCAL_DISPLAY_CONTROL
#define M_RESTRICT_FOCUS                              (M_DISP_CONTROL_START+56L) //(10056) M_LOCAL_DISPLAY_CONTROL
#define M_SHOW_TASKBAR                                (M_DISP_CONTROL_START+57L) //(10057) M_LOCAL_DISPLAY_CONTROL
#define M_EXCLUSIVE_MODE                              (M_DISP_CONTROL_START+58L) //(10058) M_LOCAL_DISPLAY_CONTROL
#define M_PRE_DISPSELECT_OPERATIONS                   (M_DISP_CONTROL_START+59L) //(10059) M_LOCAL_DISPLAY_CONTROL
#define M_POST_DISPSELECT_OPERATIONS                  (M_DISP_CONTROL_START+60L) //(10060) M_LOCAL_DISPLAY_CONTROL
#define M_PRE_DISPDESELECT_OPERATIONS                 (M_DISP_CONTROL_START+61L) //(10061) M_LOCAL_DISPLAY_CONTROL
#define M_POST_DISPDESELECT_OPERATIONS                (M_DISP_CONTROL_START+62L) //(10062) M_LOCAL_DISPLAY_CONTROL
#define M_PROPAGATE_CALIBRATION_TO_OVERLAY            (M_DISP_CONTROL_START+63L) //(10063) M_REMOTE_DISPLAY_CONTROL
#define M_SAVE_BASIC_SELECTED_BUFFER                  (M_DISP_CONTROL_START+64L) //(10064) M_LOCAL_DISPLAY_CONTROL
#define M_ACQUIRE_WINDOW_FOCUS                        (M_DISP_CONTROL_START+65L) //(10065) M_LOCAL_DISPLAY_CONTROL
#define M_BACKGROUND_COLOR                            (M_DISP_CONTROL_START+66L) //(10066) M_LOCAL_DISPLAY_CONTROL
#define M_BASIC_DISPLAY_SYSTEM_ID                     (M_DISP_CONTROL_START+67L) //(10067) M_LOCAL_DISPLAY_CONTROL

#define M_VGA_DEVICE_SYSTEM_ID                        (M_DISP_CONTROL_START+100L) //(10100) M_LOCAL_DISPLAY_CONTROL

#define M_DISPLAY_RESOLUTION                          (M_DISP_CONTROL_START+107L) //(10107) M_LOCAL_DISPLAY_CONTROL
#define M_DEFAULT_DISPLAY_RESOLUTION                  (M_DISP_CONTROL_START+108L) //(10108) M_LOCAL_DISPLAY_CONTROL
#define M_SAFE_MODE                                   (M_DISP_CONTROL_START+109L) //(10109) M_LOCAL_DISPLAY_CONTROL
#define M_USE_VIDEO_MEMORY_BUFFER                     (M_DISP_CONTROL_START+110L) //(10110) M_LOCAL_DISPLAY_CONTROL
#define M_SELECT_USER_BUFFER_DIRECTLY_ON_DISPLAY      (M_DISP_CONTROL_START+111L) //(10111) M_LOCAL_DISPLAY_CONTROL
#define M_DISPLAY_FORMAT                              (M_DISP_CONTROL_START+112L) //(10112) M_LOCAL_DISPLAY_CONTROL
#define M_DISPLAY_FORMAT_SUPPORTED                    (M_DISP_CONTROL_START+113L) //(10113) M_LOCAL_DISPLAY_CONTROL
#define M_DISPLAY_TYPE                                (M_DISP_CONTROL_START+114L) //(10114) M_LOCAL_DISPLAY_CONTROL
//                                                    (M_DISP_CONTROL_START+115L) //(10115) M_LOCAL_DISPLAY_CONTROL
#define M_USE_UNDERLAY_IN_DUALHEAD                    (M_DISP_CONTROL_START+116L) //(10116) M_LOCAL_DISPLAY_CONTROL
#define M_DISPLAY_SCHEME_ON_TITLEBAR                  (M_DISP_CONTROL_START+117L) //(10117) M_LOCAL_DISPLAY_CONTROL
#define M_UPDATE_SYNCHRONIZATION                      (M_DISP_CONTROL_START+118L) //(10118) M_LOCAL_DISPLAY_CONTROL
#define M_UNDERLAY_ALWAYS_ON_TOP                      (M_DISP_CONTROL_START+119L) //(10119) M_LOCAL_DISPLAY_CONTROL

#define M_USER_WINDOW_HANDLE                          (M_DISP_CONTROL_START+121L) //(10121) M_LOCAL_DISPLAY_CONTROL
#define M_USE_HOST_OVERLAY                            (M_DISP_CONTROL_START+122L) //(10122) M_REMOTE_DISPLAY_CONTROL
#define M_SELECTED_LINK_COPY                          (M_DISP_CONTROL_START+123L) //(10123) M_LOCAL_DISPLAY_CONTROL
#define M_HOOK_ON_ERROR                               (M_DISP_CONTROL_START+124L) //(10124) M_LOCAL_DISPLAY_CONTROL
#define M_USE_LIVE_UNDERLAY                           (M_DISP_CONTROL_START+125L) //(10125) M_LOCAL_DISPLAY_CONTROL
#define M_UPDATE_WITH_PAINT_MESSAGE                   (M_DISP_CONTROL_START+126L) //(10126) M_LOCAL_DISPLAY_CONTROL

#define M_DISPLAY_SCHEME_COUNT                        (M_DISP_CONTROL_START+131L) //(10131) M_LOCAL_DISPLAY_CONTROL
#define M_USE_DOUBLE_BUFFERING_SCHEME                 (M_DISP_CONTROL_START+132L) //(10132) M_LOCAL_DISPLAY_CONTROL
#define M_UPDATE_STATE                                (M_DISP_CONTROL_START+133L) //(10133) M_LOCAL_DISPLAY_CONTROL
#define M_FORCE_DISPLAY_SCHEME_CHANGE                 (M_DISP_CONTROL_START+134L) //(10134) M_LOCAL_DISPLAY_CONTROL
//#define                                             (M_DISP_CONTROL_START+135L) //(10135) M_LOCAL_DISPLAY_CONTROL

#define M_UNDERLAY_SHOW                               (M_DISP_CONTROL_START+137L) //(10137) M_LOCAL_DISPLAY_CONTROL
#define M_MINIMUM_DISPLAY_SIZE_X                      (M_DISP_CONTROL_START+138L) //(10138) M_LOCAL_DISPLAY_CONTROL
#define M_MINIMUM_DISPLAY_SIZE_Y                      (M_DISP_CONTROL_START+139L) //(10139) M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_SHOW                                 (M_DISP_CONTROL_START+140L) //(10140) M_LOCAL_DISPLAY_CONTROL
#define M_BUFFER_FORMAT_TO_SELECT_DIRECTLY            (M_DISP_CONTROL_START+141L) //(10141) M_LOCAL_DISPLAY_CONTROL

#define M_USE_USER_BUFFER_FOR_UNDERLAY                (M_DISP_CONTROL_START+144L) //(10144) M_REMOTE_DISPLAY_CONTROL
#define M_USE_CREATE_OVER_USER_BUFFER                 (M_DISP_CONTROL_START+145L) //(10145) M_REMOTE_DISPLAY_CONTROL
#define M_USE_FLICKER_FREE_BUFFER                     (M_DISP_CONTROL_START+146L) //(10146) M_REMOTE_DISPLAY_CONTROL
#define M_USE_CONVERT_TO_COLOR_DIB                    (M_DISP_CONTROL_START+147L) //(10147) M_REMOTE_DISPLAY_CONTROL
#define M_USE_SOFTWARE_COMPOSITION                    (M_DISP_CONTROL_START+148L) //(10148) M_REMOTE_DISPLAY_CONTROL
#define M_USE_HOST_ENTRY_BUFFER                       (M_DISP_CONTROL_START+149L) //(10149) M_REMOTE_DISPLAY_CONTROL
#define M_DISPLAY_SCHEME_BASE                         (M_DISP_CONTROL_START+150L) //(10150) M_REMOTE_DISPLAY_CONTROL

#define M_USE_DISPLAY_SIZE_BUFFER                     (M_DISP_CONTROL_START+153L) //(10153) M_REMOTE_DISPLAY_CONTROL
#define M_USE_MEGA_BUFFER                             (M_DISP_CONTROL_START+154L) //(10154) M_REMOTE_DISPLAY_CONTROL
#define M_UPDATE_TRANSFER_TYPE                        (M_DISP_CONTROL_START+155L) //(10155) M_REMOTE_DISPLAY_CONTROL
#define M_SURFACE_LUT_FLAG                            (M_DISP_CONTROL_START+156L) //(10156) M_REMOTE_DISPLAY_CONTROL

#define M_DEFAULT_MEGA_DISPLAY_SIZE_X                 (M_DISP_CONTROL_START+158L) //(10158) M_LOCAL_DISPLAY_CONTROL
#define M_DEFAULT_MEGA_DISPLAY_SIZE_Y                 (M_DISP_CONTROL_START+159L) //(10159) M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_INITIAL_SIZE_X                       (M_DISP_CONTROL_START+160L) //(10160) M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_INITIAL_SIZE_Y                       (M_DISP_CONTROL_START+161L) //(10161) M_LOCAL_DISPLAY_CONTROL
#define M_PREFER_COLOR_DISPLAY_RESOLUTION             (M_DISP_CONTROL_START+162L) //(10162) M_LOCAL_DISPLAY_CONTROL
#define M_UPDATE_ON_OVERLAY_MODIFIED_INTERNAL         (M_DISP_CONTROL_START+164L) //(10164) M_REMOTE_DISPLAY_CONTROL
#define M_USE_GIGACOLOR_SCHEME                        (M_DISP_CONTROL_START+165L) //(10165) M_LOCAL_DISPLAY_CONTROL
#define M_MEGA_BUFFER_CHILD_OFFSET_X                  (M_DISP_CONTROL_START+166L) //(10166) M_LOCAL_DISPLAY_CONTROL
#define M_MEGA_BUFFER_CHILD_OFFSET_Y                  (M_DISP_CONTROL_START+167L) //(10167) M_LOCAL_DISPLAY_CONTROL
#define M_ZOOMED_DESTINATION_OUT_DESKTOP              (M_DISP_CONTROL_START+168L) //(10168) M_LOCAL_DISPLAY_CONTROL
#define M_VALIDATE_DISPLAY                            (M_DISP_CONTROL_START+169L) //(10169) M_LOCAL_DISPLAY_CONTROL
#define M_HOOKS                                       (M_DISP_CONTROL_START+170L) //(10170) M_LOCAL_DISPLAY_CONTROL
#define M_HOOK_ON_OUT_OF_MONITOR                      (M_DISP_CONTROL_START+171L) //(10171) M_LOCAL_DISPLAY_CONTROL
#define M_HOOK_ON_ZOOM_CHANGE                         (M_DISP_CONTROL_START+172L) //(10172) M_LOCAL_DISPLAY_CONTROL
#define M_HOOK_ON_MB_CHILD_RESIZE                     (M_DISP_CONTROL_START+173L) //(10173) M_LOCAL_DISPLAY_CONTROL
#define M_DISPLACE_WINDOWS                            (M_DISP_CONTROL_START+174L) //(10174) M_LOCAL_DISPLAY_CONTROL
#define M_UPDATE_TITLE                                (M_DISP_CONTROL_START+175L) //(10175) M_LOCAL_DISPLAY_CONTROL
#define M_RESET_WINDOW_SIZE                           (M_DISP_CONTROL_START+176L) //(10176) M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_CURRENTLY_MAXIMIZED                  (M_DISP_CONTROL_START+177L) //(10177) M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_CURRENTLY_MINIMIZED                  (M_DISP_CONTROL_START+178L) //(10178) M_LOCAL_DISPLAY_CONTROL
#define M_MEGA_BUFFER_LOW_LIMIT                       (M_DISP_CONTROL_START+179L) //(10179) M_LOCAL_DISPLAY_CONTROL
#define M_MAXIMUM_VISIBLE_DEST_SIZE_X                 (M_DISP_CONTROL_START+180L) //(10180) M_LOCAL_DISPLAY_CONTROL
#define M_MAXIMUM_VISIBLE_DEST_SIZE_Y                 (M_DISP_CONTROL_START+181L) //(10181) M_LOCAL_DISPLAY_CONTROL
#define M_DO_SELECT                                   (M_DISP_CONTROL_START+182L) //(10182) M_LOCAL_DISPLAY_CONTROL
#define M_RESTRICT_WINDOWS                            (M_DISP_CONTROL_START+183L) //(10183) M_LOCAL_DISPLAY_CONTROL
#define M_NUMBER_OF_BASIC_IMPLEMENTATIONS             (M_DISP_CONTROL_START+185L) //(10185) M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_CURRENTLY_VISIBLE                    (M_DISP_CONTROL_START+187L) //(10187) M_LOCAL_DISPLAY_CONTROL
#define M_VIRTUAL_GRAB                                (M_DISP_CONTROL_START+188L) //(10188) M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_RESIZE_ON_ZOOM                       (M_DISP_CONTROL_START+189L) //(10189) M_LOCAL_DISPLAY_CONTROL
#define M_USE_NO_VIDEO_OVERLAY_BUFFER                 (M_DISP_CONTROL_START+191L) //(10191) M_LOCAL_DISPLAY_CONTROL
#define M_USE_YUV16_UYVY_SCHEME                       (M_DISP_CONTROL_START+192L) //(10192_
#define M_FILL_COLOR                                  (M_DISP_CONTROL_START+193L) //(10193) M_LOCAL_DISPLAY_CONTROL
#define M_CURRENTLY_CENTERED_X                        (M_DISP_CONTROL_START+194L) //(10194) M_LOCAL_DISPLAY_CONTROL
#define M_XDISPLAY                                    (M_DISP_CONTROL_START+195L) //(10195)
#define M_WINDOW_TASKBAR                              (M_DISP_CONTROL_START+196L) //(10196) M_LOCAL_DISPLAY_CONTROL
#define M_SHOW_WINDOW_IN_TASKBAR                      M_WINDOW_TASKBAR
#define M_WINDOW_TASKBAR_ON_FULLSCREEN                (M_DISP_CONTROL_START+197L) //(10197) M_LOCAL_DISPLAY_CONTROL
#define M_CURRENTLY_CENTERED_Y                        (M_DISP_CONTROL_START+198L) //(10198) M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_ROI_BUTTONS                          (M_DISP_CONTROL_START+200L) //(10200) M_LOCAL_DISPLAY_CONTROL
#define M_ROI_AUTO_RESET                              (M_DISP_CONTROL_START+201L) //(10201) M_LOCAL_DISPLAY_CONTROL
#define M_COPY_TO_CLIPBOARD                           (M_DISP_CONTROL_START+202L) //(10202) M_LOCAL_DISPLAY_CONTROL
#if (M_MIL_USE_UNICODE && !M_MIL_UNICODE_API && !M_COMPILING_MILDLL)
#define M_SAVE_TO_FILE                                ((M_DISP_CONTROL_START+203L)|M_CLIENT_ASCII_MODE)  // M_LOCAL_DISPLAY_CONTROL
#else
#define M_SAVE_TO_FILE                                (M_DISP_CONTROL_START+203L) //(10203) M_LOCAL_DISPLAY_CONTROL
#endif
#define M_COPY_DISPLAY_TO_ID                          (M_DISP_CONTROL_START+204L) //(10204) M_LOCAL_DISPLAY_CONTROL
#define M_SAVE_TO_BUFFER                              M_COPY_DISPLAY_TO_ID
#define M_UPDATE_RATE_MAX                             (M_DISP_CONTROL_START+205L) //(10205) M_REMOTE_DISPLAY_CONTROL
#define M_ASYNC_UPDATE                                (M_DISP_CONTROL_START+206L) //(10206) M_REMOTE_DISPLAY_CONTROL
#define M_MEGA_BUFFER_LOW_LIMIT_X                     (M_DISP_CONTROL_START+207L) //(10207) M_LOCAL_DISPLAY_CONTROL
#define M_COMMON_DIRECTX_VIDEO_DEVICE_ID              (M_DISP_CONTROL_START+208L) //(10208) M_LOCAL_DISPLAY_CONTROL
#define M_COMMON_VGA_INDEX                            M_COMMON_DIRECTX_VIDEO_DEVICE_ID
#define M_FAST_SELECT                                 (M_DISP_CONTROL_START+209L) //(10209) M_REMOTE_DISPLAY_CONTROL
#define M_FAST_UNDERLAY_MODE                          (M_DISP_CONTROL_START+210L) //(10210) M_LOCAL_DISPLAY_CONTROL
#define M_CUSTOM_SCHEME_CHANGING                      (M_DISP_CONTROL_START+211L) //(10211) M_REMOTE_DISPLAY_CONTROL
#define M_DISPLAY_FRAME_RATE_ON_TITLEBAR              (M_DISP_CONTROL_START+212L) //(10212) M_LOCAL_DISPLAY_CONTROL
#define M_SCREEN_RECT                                 (M_DISP_CONTROL_START+213L) //(10213) M_LOCAL_DISPLAY_CONTROL
#define M_DISPLAY_ZOOM_FACTOR_ON_TITLEBAR             (M_DISP_CONTROL_START+214L) //(10214) M_LOCAL_DISPLAY_CONTROL

#define M_ROI_DISPCONTROL_START                       (M_DISP_CONTROL_START+500L) // 10500
#define M_ROI_DEFINE                                  (M_DISP_CONTROL_START+500L) //(10500) M_LOCAL_DISPLAY_CONTROL
#define M_ROI_SHOW                                    (M_DISP_CONTROL_START+501L) //(10501) M_LOCAL_DISPLAY_CONTROL
#define M_ROI_SUPPORT                                 (M_DISP_CONTROL_START+502L) //(10502) M_LOCAL_DISPLAY_CONTROL
#define M_ROI_LINE_COLOR                              (M_DISP_CONTROL_START+503L) //(10503) M_LOCAL_DISPLAY_CONTROL
#define M_ROI_RESET                                   (M_DISP_CONTROL_START+504L) //(10504) M_LOCAL_DISPLAY_CONTROL
#define M_ROI_RECT_DISPLAY                            (M_DISP_CONTROL_START+505L) //(10505) M_LOCAL_DISPLAY_CONTROL
#define M_ROI_RECT_BUFFER                             (M_DISP_CONTROL_START+506L) //(10506) M_LOCAL_DISPLAY_CONTROL
#define M_ROI_SET_POINT                               (M_DISP_CONTROL_START+507L) //(10507) M_LOCAL_DISPLAY_CONTROL
#define M_ROI_LINE_WIDTH                              (M_DISP_CONTROL_START+508L) //(10508) M_LOCAL_DISPLAY_CONTROL
#define M_ROI_HANDLE_COLOR                            (M_DISP_CONTROL_START+509L) //(10509) M_LOCAL_DISPLAY_CONTROL
#define M_ROI_HANDLE_SIZE                             (M_DISP_CONTROL_START+510L) //(10510) M_LOCAL_DISPLAY_CONTROL
#define M_ROI_DISPLAY_OFFSET_X                        (M_DISP_CONTROL_START+511L) //(10511) M_LOCAL_DISPLAY_CONTROL
#define M_ROI_DISPLAY_OFFSET_Y                        (M_DISP_CONTROL_START+512L) //(10512) M_LOCAL_DISPLAY_CONTROL
#define M_ROI_DISPLAY_SIZE_X                          (M_DISP_CONTROL_START+513L) //(10513) M_LOCAL_DISPLAY_CONTROL
#define M_ROI_DISPLAY_SIZE_Y                          (M_DISP_CONTROL_START+514L) //(10514) M_LOCAL_DISPLAY_CONTROL
#define M_ROI_BUFFER_OFFSET_X                         (M_DISP_CONTROL_START+515L) //(10515) M_LOCAL_DISPLAY_CONTROL
#define M_ROI_BUFFER_OFFSET_Y                         (M_DISP_CONTROL_START+516L) //(10516) M_LOCAL_DISPLAY_CONTROL
#define M_ROI_BUFFER_SIZE_X                           (M_DISP_CONTROL_START+517L) //(10517) M_LOCAL_DISPLAY_CONTROL
#define M_ROI_BUFFER_SIZE_Y                           (M_DISP_CONTROL_START+518L) //(10518) M_LOCAL_DISPLAY_CONTROL
#define M_ROI_SHAPE                                   (M_DISP_CONTROL_START+519L) //(10519) M_LOCAL_DISPLAY_CONTROL
#define M_ROI_MINIMUM_SIZE                            (M_DISP_CONTROL_START+520L) //(10521) M_LOCAL_DISPLAY_CONTROL
#define M_ROI_DISPCONTROL_END                         (M_DISP_CONTROL_START+550L) // 10550
#define M_IS_IN_ROI_DISPLAY_RANGE(X)                  ((M_ORIGINAL_DISPLAY_CONTROL(M_STRIP_INTERMOD_BITS(X))>=M_ROI_DISPCONTROL_START) && (M_ORIGINAL_DISPLAY_CONTROL(M_STRIP_INTERMOD_BITS(X))<=M_ROI_DISPCONTROL_END))
#define M_CONTEXTUAL_MENU                             (M_DISP_CONTROL_START+551L) //(10551) M_LOCAL_DISPLAY_CONTROL
#define M_DISPLAY_IS_INTERNALLY_ALLOCATED             (M_DISP_CONTROL_START+553L) //(10553) M_LOCAL_DISPLAY_CONTROL
#define M_HOOK_ON_DEVICE_LOSING                       (M_DISP_CONTROL_START+554L) //(10554) 
//#define Available                                   (M_DISP_CONTROL_START+555L) //(10555) M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_ANNOTATIONS                          (M_DISP_CONTROL_START+556L) //(10556) M_LOCAL_DISPLAY_CONTROL  
#define M_SCALE_DISPLAY                               (M_DISP_CONTROL_START+557L) //(10557) M_LOCAL_DISPLAY_CONTROL
#define M_WINDOW_ANNOTATIONS_STATE                    (M_DISP_CONTROL_START+558L) //(10558) M_LOCAL_DISPLAY_CONTROL
#define M_NO_TEARING_ALLOWED                          (M_DISP_CONTROL_START+559L) //(10559) M_LOCAL_DISPLAY_CONTROL
#define M_ZOOM_ACCURACY                               (M_DISP_CONTROL_START+560L) //(10560) M_LOCAL_DISPLAY_CONTROL
#define M_USE_EXTERNAL_NO_TEARING                     (M_DISP_CONTROL_START+561L) //(10561) M_LOCAL_DISPLAY_CONTROL
#define M_ASSOCIATED_GRAPHIC_LIST_ID                  (M_DISP_CONTROL_START+562L) //(10562) M_LOCAL_DISPLAY_CONTROL
#define M_DISPLAY_GRAPHIC_LIST                        M_ASSOCIATED_GRAPHIC_LIST_ID
#define M_ZOOM_PAN_HANDLED_BY_SCHEME                  (M_DISP_CONTROL_START+563L) //(10563) M_LOCAL_DISPLAY_CONTROL
#define M_UPDATE_GRAPHIC_LIST                         (M_DISP_CONTROL_START+564L) //(10564) M_LOCAL_DISPLAY_CONTROL
#define M_MEGA_BUFFER_SIZE_X_TO_USE                   (M_DISP_CONTROL_START+565L) //(10565) M_LOCAL_DISPLAY_CONTROL
#define M_MEGA_BUFFER_SIZE_Y_TO_USE                   (M_DISP_CONTROL_START+566L) //(10566) M_LOCAL_DISPLAY_CONTROL
#define M_GRAPHIC_LIST_INTERACTIVE                    (M_DISP_CONTROL_START+567L) //(10567) M_LOCAL_DISPLAY_CONTROL
#define M_USE_GDI_TO_APPLY_LUT                        (M_DISP_CONTROL_START+568L) //(10568) M_LOCAL_DISPLAY_CONTROL //available for customers
#define M_GRAPHIC_LIST_BOUNDING_RECT                  (M_DISP_CONTROL_START+570L) //(10570) M_LOCAL_DISPLAY_CONTROL
#define M_DMIL_REALLOC_SURFACE                        (M_DISP_CONTROL_START+571L) //(10571) M_REMOTE_DISPLAY_CONTROL
#define M_ASSOCIATED_REMOTE_DISPLAY_ID                (M_DISP_CONTROL_START+572L) //(10572) M_LOCAL_DISPLAY_CONTROL
#define M_FILL_DISPLAY                                (M_DISP_CONTROL_START+574L) //(10574) M_LOCAL_DISPLAY_CONTROL
#define M_GRAPHIC_LIST_LIMITED_TO_BUFFER_AREA         (M_DISP_CONTROL_START+575L) //(10575) M_LOCAL_DISPLAY_CONTROL
#define M_GRAPHIC_LIST_RELATIVE_TO_ANCESTOR           (M_DISP_CONTROL_START+576L) //(10576) M_LOCAL_DISPLAY_CONTROL


#define M_DISP_CONTROL_DIRECT_STOP                    11000L

#define M_APP_BH_DISPLAY_START                        12000L
#define M_DESCRIPTOR_LOCK                             (M_APP_BH_DISPLAY_START+00L)     // 12000
#define M_APP_BH_DISPLAY_END                          12099L
#define M_IS_IN_DISPLAY_APP_BH_RANGE(X)               ((M_STRIP_INTERMOD_BITS(X) >= M_APP_BH_DISPLAY_START) && (M_STRIP_INTERMOD_BITS(X) <= M_APP_BH_DISPLAY_END))


#define M_SCALE                                       0x00008010L
#define M_CUSTOM_DISPLAY                              M_IN_BUFFER_DISPLAY
#define M_CUSTOM_DISPLAY_BUFFER                       M_DISPLAY_SURFACE

/************************************************************************/
/* MdispControl() / MdispInquire() Values                               */
/************************************************************************/

#define M_FULL_SIZE                      0L
#define M_NORMAL_SIZE                    1L

#define M_BENCHMARK_IN_DEBUG_ON          0x00000001L
#define M_BENCHMARK_IN_DEBUG_OFF         0
#define M_BENCHMARK_IN_DEBUG_TRACE       0x00000002L
#define M_BENCHMARK_IN_DEBUG_NOTRACE     0
#define M_BENCHMARK_IN_DEBUG_ALLSIZE     0x00000004L
#define M_BENCHMARK_IN_DEBUG_CSTSIZE     0

#define M_DISPLAY_SCAN_LINE_START        0x00000000L
#define M_DISPLAY_SCAN_LINE_END                   -1

#define M_SLAVE                          0L
#define M_MASTER                         1L

#define M_TRANSPARENT_COLOR              0x01000060L //M_LOCAL_DISPLAY_CONTROL
#define M_TEST_UPDATE_IF_ASYNCHRONOUS    0x00000010L

//For M_NO_TEARING dispControl
#define M_ENABLE                                      -9997L
#define M_FULL_UPDATE                                 -9998L
#define M_DISABLE                                     -9999L
#define M_DEFAULT                                     0x10000000L
#define M_GRAB_CONTINUOUS_ONLY                        0x00100000L
#define M_BASIC                                       0x00200000L
#define M_ADVANCED                                    0x00400000L
#define M_NO_SKIP                                     (M_DISP_CONTROL_START+2L) //(10002) M_LOCAL_DISPLAY_CONTROL
#define M_SKIP_OLDEST                                 (M_DISP_CONTROL_START+3L) //(10003) M_LOCAL_DISPLAY_CONTROL
#define M_SKIP_NEWEST                                 (M_DISP_CONTROL_START+4L) //(10004) 
#define M_NO_TEARING_SKIP_MODE_MASK                   0x000FFFFFL //M_LOCAL_DISPLAY_CONTROL
#define M_NO_TEARING_MODE_MASK                        0x00F00000L
#define M_CURRENT_RESOLUTION                          MIL_TEXT("M_CURRENT_RESOLUTION")
#define M_RESET_RESOLUTION                            MIL_TEXT("M_RESET_RESOLUTION")
#define M_RESTORE_RESOLUTION                          MIL_TEXT("M_RESTORE_RESOLUTION")

#define BIN_TO_MIL(b)                                 ( ((M_DEFAULT==b)||(M_ENABLE==b)||(M_DISABLE==b)||(M_FULL_UPDATE==b))?(b):(b?M_ENABLE:M_DISABLE) )

#define M_ON_SELECT                      1L
#define M_ON_DESELECT                    2L

// For the control M_GRAB_BUFFERS
#define M_DRIVER_ERROR_PENDING                        0x40000000L

// For the M_STOP_UPDATE control/inquire
#define M_WHEN_MINIMIZED                              0x00000001L
#define M_WHEN_NOT_VISIBLE                            0x00000002L

// For M_VIDEO_DEVICE_LOST control/inquire
#define M_SIGNAL_OFF                                  0x00000000L
#define M_CLEAR                                       0x00000001L
#define M_NO_CLEAR                                    0x00000002L

// For M_ROI_DEFINE control
#define M_START                                       0x00000002L
#define M_STOP                                        0x00000004L
#define M_RESET                                       9L
#define M_RECTANGULAR                                 0x00000010L

// For M_VGA_INDEX inquire
#define M_INVALID_VGA_INDEX                           M_INVALID

// For M_CAN_ALLOCATE_CONTIGUOUS_VIDEO_MEMORY inquire
#define M_CONTIGUOUS                    0x00000001L
#define M_START_IN_VIDEO_MEMORY         0x00000010L
#define M_END_IN_VIDEO_MEMORY           0x00000020L
#define M_START_IN_HOST_MEMORY          0x00000040L
#define M_END_IN_HOST_MEMORY            0x00000080L
#define M_NO_PHYSICAL_ADDRESS           0x00000100L
#define M_IN_VIDEO_MEMORY(x)               ((x&M_START_IN_VIDEO_MEMORY) && (x&M_END_IN_VIDEO_MEMORY))
#define M_IN_HOST_MEMORY(x)                ((x&M_START_IN_HOST_MEMORY) && (x&M_END_IN_HOST_MEMORY))
#define M_CONTIGUOUS_AND_IN_VIDEO_MEMORY(x)((x&M_CONTIGUOUS) && M_IN_VIDEO_MEMORY(x))

// For M_ZOOM_ACCURACY
#define M_MEDIUM                                2L
#define M_HIGH                                  3L


// For M_INTERACTIVE_MODE
//M_LEGACY
#define M_MOUSE_DRIVEN                          0x00000002L

// For M_UPDATE_RATE_MAX
#define M_SCREEN_REFRESH_RATE                   0x01000000L
#define M_MAX_REFRESH_RATE                      0x02000000L


/************************************************************************/
/* MdispLut()                                                           */
#define M_PSEUDO                                       M_PSEUDO_LUT //(M_PSEUDO_ID+2)


/************************************************************************/
/* MdispHook()                                                          */
/************************************************************************/
#define M_ASYNC_HOOK                  0x08000000L

/* Defines for hook to modification to bitmap and window */
#define M_NOT_MODIFIED                0 /* No changed at all              */
#define M_MODIFIED_LUT                1 /* Disp lut is changed            */
#define M_MODIFIED_DIB                2 /* Disp buffer data is changed    */
#define M_MODIFIED_ZOOM               3 /* Disp is zoomed                 */
#define M_MODIFIED_PAN                4 /* Disp is panned                 */
#define M_MODIFIED_DIB_CREATION       5 /* Disp receives a new buffer ID  */
#define M_MODIFIED_DIB_DESTRUCTION    6 /* Disp receives a buffer ID 0    */
#define M_MODIFIED_WINDOW_CREATION    7 /* Wnd is created                 */
#define M_MODIFIED_WINDOW_DESTRUCTION 8 /* Wnd is destroyed               */
#define M_MODIFIED_WINDOW_LOCATION    9 /* Wnd size is changed            */
#define M_MODIFIED_WINDOW_OVERLAP    11 /* Wnd overlap is changed         */
#define M_MODIFIED_WINDOW_ICONIZED   12 /* Wnd is changed to iconic state */
#define M_MODIFIED_WINDOW_ZOOM       13 /* Wnd is zoomed                  */
#define M_MODIFIED_WINDOW_PAN        14 /* Wnd is panned                  */
#define M_MODIFIED_WINDOW_MENU       15 /* Wnd menu pulled-down           */
#define M_MODIFIED_WINDOW_PAINT      16 /* Wnd is painted with image      */
#define M_MODIFIED_WINDOW_ACTIVE     17 /* Wnd activation state changed   */
#define M_MODIFIED_WINDOW_ENABLE     18 /* Wnd enable state changed       */
#define M_MODIFIED_WINDOW_CLIP_LIST  19 /* Wnd clip list changed          */

/* M_MODIFIED_WINDOW_MENU modification hook defines */
#define M_MODIFIED_SYS_MENU            0x00100L
#define M_MODIFIED_APP_MENU            0x00200L
#define M_MODIFIED_USER_APP_MENU       0x10000L
#define M_MODIFIED_RESTORE_MENUITEM    0x00001L
#define M_MODIFIED_MOVE_MENUITEM       0x00002L
#define M_MODIFIED_SIZE_MENUITEM       0x00003L
#define M_MODIFIED_MINIMIZE_MENUITEM   0x00004L
#define M_MODIFIED_MAXIMIZE_MENUITEM   0x00005L
#define M_MODIFIED_CLOSE_MENUITEM      0x00006L
#define M_MODIFIED_TASKLIST_MENUITEM   0x00007L
#define M_MODIFIED_MENUBAR_MENUITEM    0x00008L
#define M_MODIFIED_TITLEOFF_MENUITEM   0x00009L
#define M_MODIFIED_ZOOMIN_MENUITEM     0x0000AL
#define M_MODIFIED_ZOOMOUT_MENUITEM    0x0000BL
#define M_MODIFIED_NOZOOM_MENUITEM     0x0000CL

/* M_MODIFIED_WINDOW_ACTIVE modification hook defines */
/* M_MODIFIED_WINDOW_ENABLE modification hook defines */
#define M_MODIFIED_STATE_FROM_WINDOW   0
#define M_MODIFIED_STATE_FROM_PARENT   0x00000010L
#define M_MODIFIED_OFF                 0
#define M_MODIFIED_ON                  0x00000001L

/* M_MODIFIED_WINDOW_CLIP_LIST modification hook defines */
/* M_MODIFIED_WINDOW_CLIP_LIST modification hook defines */
#define M_MODIFIED_ACCESS_RECTANGULAR_OFF 0
#define M_MODIFIED_ACCESS_RECTANGULAR_ON  0x00000001L
#define M_MODIFIED_ACCESS_OFF             0
#define M_MODIFIED_ACCESS_ON              0x00000002L
#define M_MODIFIED_ACCESS_COMMAND_PROMPT  0x00000004L

/* Type of event to hook for a display */
#define M_HOOK_AFTER                0x10000000L          /* For hook after modification      */
#define M_HOOK_BEFORE               0x20000000L          /* For hook before modification     */
#define M_LOCAL_HOOK_RANGE1_START   M_HOOK_MODIFIED_DIB  /* Start of first range of local hooks */
#define M_HOOK_MODIFIED_DIB         1L                                                 /* For buffer bitmap modification   */
#define M_HOOK_MODIFIED_WINDOW      2L                                                 /* For disp window modification     */
#define M_HOOK_MESSAGE_LOOP         4L                                                 /* For disp window modification     */
#define M_LOCAL_HOOK_RANGE1_END     M_HOOK_MESSAGE_LOOP  /* End of first range of local hooks */
#define M_FRAME_START               9L                   /* For disp frame start             */
#define M_PRE_GRAB_BUFFER_CHANGE    10L                  /* For hook before the display scheme change when it imply the change of the grab buffers */
#define M_POST_GRAB_BUFFER_CHANGE   11L                  /* For hook after the display scheme change when it imply the change of the grab buffers  */
#define M_PRE_SCHEME_CHANGE         M_PRE_GRAB_BUFFER_CHANGE   /* Old Name */
#define M_POST_SCHEME_CHANGE        M_POST_GRAB_BUFFER_CHANGE  /* Old Name */
#define M_LOCAL_HOOK_RANGE2_START   (M_BASIC_DISPLAY_EVENT & ~M_ASYNC_HOOK)   /* Start of second range of local hooks */
#define M_BASIC_DISPLAY_EVENT       (12L | M_ASYNC_HOOK)                               /* For hook when disp window is displayed in another VGA,or between, or unsupported zoom or more (new MIL8) */
#define M_SURFACE_LOST              (14L | M_ASYNC_HOOK)                               /* For hook when DirectX surface is lost (new event for MIL8.0)     */
#define M_LOCAL_HOOK_RANGE2_END     (M_SURFACE_LOST & ~M_ASYNC_HOOK) /* End of second range of local hooks */
#define M_INVALIDATE_DISPLAY        (16L | M_ASYNC_HOOK) /* For hook when a full internal update is needed (new event for MIL8.0)     */
#define M_LOCAL_HOOK_RANGE3_START   M_WINDOW_CLOSE_BEFORE   /* Start of third range of local hooks */
#define M_WINDOW_CLOSE_BEFORE       17L
#define M_WINDOW_CLOSE_AFTER        18L
#define M_WINDOW_MINIMIZE_BEFORE    19L
#define M_WINDOW_MINIMIZE_AFTER     20L
#define M_WINDOW_MAXIMIZE_BEFORE    21L
#define M_WINDOW_MAXIMIZE_AFTER     22L
#define M_WINDOW_RESTORE_BEFORE     23L
#define M_WINDOW_RESTORE_AFTER      24L
#define M_WINDOW_POSCHANGE_BEFORE   25L
#define M_WINDOW_POSCHANGE_AFTER    26L
#define M_WINDOW_ZOOM_BEFORE        27L
#define M_WINDOW_ZOOM_AFTER         28L
#define M_WINDOW_SCROLL_BEFORE      29L
#define M_WINDOW_SCROLL_AFTER       30L
#define M_WINDOW_PAINT_BEFORE       31L
#define M_WINDOW_PAINT_AFTER        32L
#define M_WINDOW_ACTIVE_BEFORE      33L
#define M_WINDOW_ACTIVE_AFTER       34L
#define M_WINDOW_ENABLE_BEFORE      35L
#define M_WINDOW_ENABLE_AFTER       36L
#define M_WINDOW_SHOW_BEFORE        37L
#define M_WINDOW_SHOW_AFTER         38L
#define M_MODIFIED_DISPLAY          39L
#define M_LOCAL_HOOK_RANGE3_END     M_MODIFIED_DISPLAY   /* End of third range of local hooks */
#define M_DISPLAY_SCHEME_CHANGED    40L
#define M_PRE_OVERLAY_ID_CHANGE     41L
#define M_POST_OVERLAY_ID_CHANGE    42L
#define M_LOCAL_HOOK_RANGE4_START   M_WINDOW_VISIBLE_CHANGE /* Start of fourth range of local hooks */
#define M_WINDOW_VISIBLE_CHANGE     43L
#define M_LOCAL_HOOK_RANGE4_END     M_WINDOW_VISIBLE_CHANGE /* End of fourth range of local hooks */
#define M_PRIMARY_SURFACE_ID_CHANGED 44L
#define M_LOCAL_HOOK_RANGE5_START   M_DEVICE_LOSING   /* Start of fifth range of local hooks */
#define M_DEVICE_LOSING             45L
#define M_DEVICE_LOST               46L
#define M_DEVICE_RESTORING          47L
#define M_DEVICE_RESTORED           48L
#define M_ROI_CHANGE                49L
#define M_ROI_CHANGE_END            50L
#define M_ROI_HANDLE_RELEASE        51L
#define M_VIDEO_DEVICE_MODIFIED_BEFORE 52L
#define M_VIDEO_DEVICE_MODIFIED_AFTER  53L
#define M_DEVICE_LEAVEVT            54L
#define M_DEVICE_ENTERVT            55L
#define M_MOUSE_LEFT_BUTTON_DOWN    56L
#define M_MOUSE_RIGHT_BUTTON_DOWN   57L
#define M_MOUSE_LEFT_BUTTON_UP      58L
#define M_MOUSE_RIGHT_BUTTON_UP     59L
#define M_ANNOTATIONS_DRAW          60L
#define M_LOCAL_HOOK_RANGE5_END     M_ANNOTATIONS_DRAW   /* End of fifth of local hooks */
#define M_WINDOW_ACTION             61L
#define M_WINDOW_RESIZED            62L
// available                        63L
#define M_LOCAL_HOOK_RANGE6_START   M_MOUSE_MOVE /* Start of sixth range of local hooks */
#define M_MOUSE_MOVE                64L
#define M_MOUSE_WHEEL               65L
#define M_MOUSE_MIDDLE_BUTTON_DOWN  66L
#define M_MOUSE_MIDDLE_BUTTON_UP    67L
#define M_KEY_DOWN                  68L
#define M_KEY_UP                    69L
// available                        70L
#define M_MOUSE_LEFT_DOUBLE_CLICK   71L
#define M_KEY_CHAR                  72L
#define M_SET_CURSOR                73L
#define M_WINDOW_SIZING             74L
#define M_CUSTOM_MESSAGE_SERVER     75L
#define M_CUSTOM_MESSAGE_CLIENT     76L
#define M_GRA_INTERACTIVE_ENABLED   77L
#define M_GRA_INTERACTIVE_DISABLED  78L
//#define RESERVED                  79 to 99L
#define M_LOCAL_HOOK_RANGE6_END     M_SET_CURSOR   /* End of sixth of local hooks */

#define M_IS_LOCAL_HOOK(HookType)   (((((HookType) & ~(M_HOOK_AFTER | M_HOOK_BEFORE)) >= M_LOCAL_HOOK_RANGE1_START) &&  \
                                      (((HookType) & ~(M_HOOK_AFTER | M_HOOK_BEFORE)) <= M_LOCAL_HOOK_RANGE1_END)) ||   \
                                     ((((HookType) & ~(M_HOOK_AFTER | M_HOOK_BEFORE)) >= M_LOCAL_HOOK_RANGE2_START) &&  \
                                      (((HookType) & ~(M_HOOK_AFTER | M_HOOK_BEFORE)) <= M_LOCAL_HOOK_RANGE2_END)) ||   \
                                     ((((HookType) & ~(M_HOOK_AFTER | M_HOOK_BEFORE)) >= M_LOCAL_HOOK_RANGE3_START) &&  \
                                      (((HookType) & ~(M_HOOK_AFTER | M_HOOK_BEFORE)) <= M_LOCAL_HOOK_RANGE3_END)) ||   \
                                     ((((HookType) & ~(M_HOOK_AFTER | M_HOOK_BEFORE)) >= M_LOCAL_HOOK_RANGE4_START) &&  \
                                      (((HookType) & ~(M_HOOK_AFTER | M_HOOK_BEFORE)) <= M_LOCAL_HOOK_RANGE4_END)) ||   \
                                      ((((HookType) & ~(M_HOOK_AFTER | M_HOOK_BEFORE)) >= M_LOCAL_HOOK_RANGE5_START) &&  \
                                     ((((HookType) & ~(M_HOOK_AFTER | M_HOOK_BEFORE)) <= M_LOCAL_HOOK_RANGE5_END)) ||   \
                                     ((((HookType) & ~(M_HOOK_AFTER | M_HOOK_BEFORE)) >= M_LOCAL_HOOK_RANGE6_START) &&  \
                                      (((HookType) & ~(M_HOOK_AFTER | M_HOOK_BEFORE)) <= M_LOCAL_HOOK_RANGE6_END))))

//Old names
#define M_WINDOW_LBUTTONDOWN        M_MOUSE_LEFT_BUTTON_DOWN
#define M_WINDOW_RBUTTONDOWN        M_MOUSE_RIGHT_BUTTON_DOWN
#define M_WINDOW_LBUTTONUP          M_MOUSE_LEFT_BUTTON_UP
#define M_WINDOW_RBUTTONUP          M_MOUSE_RIGHT_BUTTON_UP
#define M_PRE_SCHEME_CHANGE         M_PRE_GRAB_BUFFER_CHANGE 
#define M_POST_SCHEME_CHANGE        M_POST_GRAB_BUFFER_CHANGE

#if OldDefinesSupport
#define M_ROI_ANCHOR_COLOR    M_ROI_HANDLE_COLOR
MIL_DEPRECATED(M_ROI_ANCHOR_COLOR, 1000)
#define M_ROI_ANCHOR_SIZE     M_ROI_HANDLE_SIZE
MIL_DEPRECATED(M_ROI_ANCHOR_SIZE, 1000)
#define M_ROI_ANCHOR_RELEASE  M_ROI_HANDLE_RELEASE
MIL_DEPRECATED(M_ROI_ANCHOR_RELEASE, 1000)
#define M_WINDOW_ROI          M_WINDOW_ROI_BUTTONS
MIL_DEPRECATED(M_WINDOW_ROI, 1000)
#endif


// Flag passed via the EventId of the hook
#define M_INVALIDATE_OVERLAY     (0x00000001)
#define M_INVALIDATE_UNDERLAY    (0x00000002)
#define M_INVALIDATE_ALL_DISPLAY (M_INVALIDATE_OVERLAY|M_INVALIDATE_UNDERLAY)

/************************************************************************/
/* MdispGetHookInfo() Info types                                        */
/************************************************************************/
#define M_DISPLAY                0x00200000L
#define M_MOUSE_POSITION_X       1L
#define M_MOUSE_POSITION_Y       2L
#define M_ANNOTATIONS_DC         3L
#define M_ANNOTATIONS_PIXMAP     M_ANNOTATIONS_DC // Pixmap for Linux 
#define M_MOUSE_POSITION_BUFFER_X  4L
#define M_MOUSE_POSITION_BUFFER_Y  5L
#define M_USER_FLAG                6L
#define M_COMBINATION_KEYS         7L
#define M_EVENT_VALUE              8L

// documented aliases for M_EVENT_VALUE
#define M_MOUSE_WHEEL_VALUE   M_EVENT_VALUE
#define M_KEY_VALUE           M_EVENT_VALUE

// aliases for some infos, for easier documentation of M_CUSTOM_MESSAGE_CLIENT/SERVER
#define M_EVENT_VALUE_1          M_MOUSE_POSITION_X
#define M_EVENT_VALUE_2          M_MOUSE_POSITION_Y
#define M_EVENT_VALUE_3          M_EVENT_VALUE
#define M_EVENT_VALUE_4          M_COMBINATION_KEYS

/************************************************************************/
/* MdispMessage flags                                                   */
/************************************************************************/

#define M_KEY_SHIFT              0x00010000
#define M_KEY_CTRL               0x00020000
#define M_KEY_ALT                0x00040000
#define M_MOUSE_LEFT_BUTTON      0x00080000
#define M_MOUSE_MIDDLE_BUTTON    0x00100000
#define M_MOUSE_RIGHT_BUTTON     0x00200000

#if M_MIL_USE_WINDOWS
#define M_COMBINATION_KEYS_FROM_WPARAM(X) (((X&MK_SHIFT)    ?  M_KEY_SHIFT          : M_NULL)   | \
                                           ((X&MK_CONTROL)  ?  M_KEY_CTRL           : M_NULL)   | \
                                           ((X&MK_LBUTTON)  ?  M_MOUSE_LEFT_BUTTON  : M_NULL)   | \
                                           ((X&MK_MBUTTON)  ?  M_MOUSE_MIDDLE_BUTTON: M_NULL)   | \
                                           ((X&MK_RBUTTON)  ?  M_MOUSE_RIGHT_BUTTON : M_NULL))

#define M_COMBINATION_KEYS_FROM_KEY_STATE()  (((GetKeyState(VK_SHIFT)&0x8000)    ?  M_KEY_SHIFT : M_NULL)   | \
                                              ((GetKeyState(VK_CONTROL)&0x8000)  ?  M_KEY_CTRL  : M_NULL)   | \
                                              ((GetKeyState(VK_MENU)&0x8000)     ?  M_KEY_ALT   : M_NULL))
#define M_KEY_SPACE              VK_SPACE
#define M_KEY_PAGEUP             VK_PRIOR 
#define M_KEY_PAGEDOWN           VK_NEXT
#define M_KEY_END                VK_END
#define M_KEY_HOME               VK_HOME
#define M_KEY_ARROW_LEFT         VK_LEFT
#define M_KEY_ARROW_UP           VK_UP
#define M_KEY_ARROW_RIGHT        VK_RIGHT
#define M_KEY_ARROW_DOWN         VK_DOWN
#define M_KEY_DELETE             VK_DELETE
#define M_KEY_ADD                VK_ADD
#define M_KEY_SUBSTRACT          VK_SUBTRACT
#else
#define M_COMBINATION_KEYS_FROM_WPARAM(X) (((X&ShiftMask)    ?  M_KEY_SHIFT          : M_NULL)   | \
                                           ((X&ControlMask)  ?  M_KEY_CTRL           : M_NULL)   | \
                                           ((X&Button1Mask)  ?  M_MOUSE_LEFT_BUTTON  : M_NULL)   | \
                                           ((X&Button2Mask)  ?  M_MOUSE_MIDDLE_BUTTON: M_NULL)   | \
                                           ((X&Button3Mask)  ?  M_MOUSE_RIGHT_BUTTON : M_NULL))
#define M_KEY_SPACE              0x00000020
#define M_KEY_PAGEUP             0x00000021
#define M_KEY_PAGEDOWN           0x00000022
#define M_KEY_END                0x00000023
#define M_KEY_HOME               0x00000024
#define M_KEY_ARROW_LEFT         0x00000025
#define M_KEY_ARROW_UP           0x00000026
#define M_KEY_ARROW_RIGHT        0x00000027
#define M_KEY_ARROW_DOWN         0x00000028
#define M_KEY_DELETE             0x0000002E
#define M_KEY_ADD                0x0000006B
#define M_KEY_SUBSTRACT          0x0000006D
#endif



#if M_MIL_USE_WINDOWS
#define M_WHEEL_DELTA            WHEEL_DELTA
#define M_MOUSE_WHEEL_DELTA_FROM_WPARAM(X)  (GET_WHEEL_DELTA_WPARAM(X)/M_WHEEL_DELTA)
#else
#define M_WHEEL_DELTA            120 
#define M_MOUSE_WHEEL_DELTA_FROM_WPARAM(X)   ((X<0)?-1:1)
#endif


/************************************************************************/
/* MdispOverlayKey()                                                    */
/************************************************************************/
#define M_KEY_ON_COLOR                                1L
#define M_KEY_OFF                                     2L
#define M_KEY_ALWAYS                                  3L


      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* FUNCTION PROTOTYPES */

      /* -------------------------------------------------------------- */

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
   {
#endif

typedef MIL_HOOK_FUNCTION_PTR MDISPHOOKFCTPTR;
typedef MIL_HOOK_FUNCTION_PTR MIL_DISP_HOOK_FUNCTION_PTR;

void MFTYPE    MdispHookFunction    (MIL_ID DisplayId,
                                     MIL_INT HookType,
                                     MIL_DISP_HOOK_FUNCTION_PTR HookHandlerPtr,
                                     void *UserDataPtr);


#if M_MIL_USE_UNICODE
MIL_ID MFTYPE  MdispAllocA          (MIL_ID SystemId,
                                     MIL_INT DispNum,
                                     const char *DispFormat,
                                     MIL_INT64 InitFlag,
                                     MIL_ID *DisplayIdPtr);
MIL_ID MFTYPE  MdispAllocW          (MIL_ID SystemId,
                                     MIL_INT DispNum,
                                     MIL_CONST_TEXT_PTR DispFormat,
                                     MIL_INT64 InitFlag,
                                     MIL_ID *DisplayIdPtr);
#if M_MIL_UNICODE_API
#define MdispAlloc MdispAllocW
#else
#define MdispAlloc MdispAllocA
#endif
#else

MIL_ID MFTYPE  MdispAlloc           (MIL_ID SystemId,
                                     MIL_INT DispNum,
                                     MIL_CONST_TEXT_PTR DispFormat,
                                     MIL_INT64 InitFlag,
                                     MIL_ID *DisplayIdPtr);

#endif


void MFTYPE    MdispFree            (MIL_ID DisplayId);

void MFTYPE    MdispSelect          (MIL_ID DisplayId,
                                     MIL_ID ImageBufId);

#define MdispDeselect(DisplayId, ImageId) MdispSelect(DisplayId, M_NULL)

MIL_INT MFTYPE MdispInquire         (MIL_ID DisplayId,
                                     MIL_INT64 InquireType,
                                     void* UserVarPtr);

void MFTYPE    MdispPan             (MIL_ID DisplayId,
                                     MIL_DOUBLE XOffset,
                                     MIL_DOUBLE YOffset);
void MFTYPE    MdispZoom            (MIL_ID DisplayId,
                                     MIL_DOUBLE XFactor,
                                     MIL_DOUBLE YFactor);

void MFTYPE    MdispLut             (MIL_ID DisplayId,
                                     MIL_ID LutBufId);

void MFTYPE    MdispMessage(         MIL_ID  DisplayId, 
                                     MIL_INT EventType, 
                                     MIL_INT MousePositionX, 
                                     MIL_INT MousePositionY, 
                                     MIL_INT EventValue, 
                                     MIL_INT CombinationKeys,
                                     MIL_INT UserValue);

#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs

void MFTYPE    MdispControlInt64        (MIL_ID DisplayId,
                                         MIL_INT64 ControlType,
                                         MIL_INT64 ControlValue);

void MFTYPE    MdispControlDouble       (MIL_ID DisplayId,
                                         MIL_INT64 ControlType,
                                         MIL_DOUBLE ControlValue);
#else
// Prototypes for 32 bits OSs

#define MdispControlDouble MdispControl
#define MdispControlInt64  MdispControl
void MFTYPE       MdispControl         (MIL_ID DisplayId,
                                        MIL_INT64 ControlType,
                                        MIL_DOUBLE ControlValue);
#endif


void MFTYPE    MdispUpdate         (MIL_ID DisplayId,
                                    MIL_INT OffsetX,
                                    MIL_INT OffsetY,
                                    MIL_INT SizeX,
                                    MIL_INT SizeY,
                                    MIL_INT64 ControlFlag);

MIL_INT MFTYPE    MdispGetHookInfo    (MIL_ID EventId,
                                       MIL_INT64 InfoType,
                                       void* UserVarPtr);

void MFTYPE    MdispSelectWindow    (MIL_ID DisplayId,
                                     MIL_ID ImageBufId,
                                     MIL_WINDOW_HANDLE ClientWindowHandle);


/* C++ directive if needed */
#ifdef __cplusplus
}
#endif


#if M_MIL_USE_64BIT
#ifdef __cplusplus

//////////////////////////////////////////////////////////////
// MdispControl function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_MIL_USE_LINUX
inline void MdispControl(MIL_ID DisplayId,
                         MIL_INT64 ControlType,
                         int ControlValue)
   {
   MdispControlInt64(DisplayId, ControlType, ControlValue);
   };
#endif
inline void MdispControl(MIL_ID DisplayId,
                         MIL_INT64 ControlType,
                         MIL_INT32 ControlValue)
   {
   MdispControlInt64(DisplayId, ControlType, ControlValue);
   };

inline void MdispControl(MIL_ID DisplayId,
                         MIL_INT64 ControlType,
                         MIL_INT64 ControlValue)
   {
   MdispControlInt64(DisplayId, ControlType, ControlValue);
   };

inline void MdispControl(MIL_ID DisplayId,
                         MIL_INT64 ControlType,
                         MIL_DOUBLE ControlValue)
   {
   MdispControlDouble(DisplayId, ControlType, ControlValue);
   };

#else // __cplusplus

#define MdispControl MdispControlInt64

#endif // __cplusplus
#endif // M_MIL_USE_64BIT

//////////////////////////////////////////////////////////////
// SafeType functions.
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// See milos.h for explanation about these functions.
//////////////////////////////////////////////////////////////

// M_MIL_USE_SAFE_TYPE defined in milos.h.
#if M_MIL_USE_SAFE_TYPE

// ----------------------------------------------------------
// MdispInquire

inline MIL_INT MFTYPE MdispInquireUnsafe  (MIL_ID DisplayId, MIL_INT64 InquireType, void*                 ResultPtr);
inline MIL_INT MFTYPE MdispInquireSafeType(MIL_ID DisplayId, MIL_INT64 InquireType, int                   ResultPtr);
inline MIL_INT MFTYPE MdispInquireSafeType(MIL_ID DisplayId, MIL_INT64 InquireType, MIL_TEXT_CHAR*        ResultPtr);
inline MIL_INT MFTYPE MdispInquireSafeType(MIL_ID DisplayId, MIL_INT64 InquireType, MIL_INT*              ResultPtr);
inline MIL_INT MFTYPE MdispInquireSafeType(MIL_ID DisplayId, MIL_INT64 InquireType, MIL_DOUBLE*           ResultPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MdispInquireSafeType(MIL_ID DisplayId, MIL_INT64 InquireType, MIL_UINT*             ResultPtr);
#endif
#if   M_MIL_USE_WINDOWS
inline MIL_INT MFTYPE MdispInquireSafeType(MIL_ID DisplayId, MIL_INT64 InquireType, MIL_WINDOW_HANDLE*    ResultPtr);
#endif
#if !M_MIL_USE_64BIT
inline MIL_INT MFTYPE MdispInquireSafeType(MIL_ID DisplayId, MIL_INT64 InquireType, MIL_INT64*            ResultPtr);
#endif

inline MIL_INT64 MdispInquireRequiredType(MIL_INT64 InquireType)
   {
   if (M_IN_DISP_INQUIRE_MIL_ID_RANGE(InquireType))
      return M_TYPE_MIL_ID;

   if (M_IN_DISP_INQUIRE_DOUBLE_RANGE(InquireType))
      return M_TYPE_DOUBLE;

   if (M_IN_DISP_INQUIRE_STRING_RANGE(InquireType))
      return M_TYPE_TEXT_CHAR;

   if (M_IN_DISP_INQUIRE_MIL_INT64_RANGE(InquireType))
      return M_TYPE_MIL_INT64;

   return M_TYPE_MIL_INT;
   }

inline MIL_INT MFTYPE MdispInquireSafeType(MIL_ID DisplayId, MIL_INT64 InquireType, int UserVarPtr)
   {
   if (UserVarPtr || M_DISP_INQUIRE_MUST_HAVE_USER_PTR(InquireType))
      SafeTypeError(MIL_TEXT("MdispInquire"));

   return MdispInquire(DisplayId, InquireType, NULL);
   }

inline MIL_INT MFTYPE MdispInquireExecute (MIL_ID DisplayId, MIL_INT64 InquireType, void* ValuePtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MdispInquireRequiredType(InquireType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MdispInquire"));

   return MdispInquire(DisplayId, InquireType, ValuePtr);
   }

inline MIL_INT MFTYPE MdispInquireUnsafe  (MIL_ID DisplayId, MIL_INT64 InquireType, void*          UserVarPtr) {return MdispInquire       (DisplayId, InquireType, UserVarPtr                  );}
inline MIL_INT MFTYPE MdispInquireSafeType(MIL_ID DisplayId, MIL_INT64 InquireType, MIL_TEXT_CHAR* UserVarPtr) {return MdispInquireExecute(DisplayId, InquireType, UserVarPtr, M_TYPE_TEXT_CHAR);}
inline MIL_INT MFTYPE MdispInquireSafeType(MIL_ID DisplayId, MIL_INT64 InquireType, MIL_INT*       UserVarPtr) {return MdispInquireExecute(DisplayId, InquireType, UserVarPtr, M_TYPE_MIL_INT  );}
inline MIL_INT MFTYPE MdispInquireSafeType(MIL_ID DisplayId, MIL_INT64 InquireType, MIL_DOUBLE*    UserVarPtr) {return MdispInquireExecute(DisplayId, InquireType, UserVarPtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MdispInquireSafeType(MIL_ID DisplayId, MIL_INT64 InquireType, MIL_UINT*      UserVarPtr) {return MdispInquireExecute(DisplayId, InquireType, UserVarPtr, M_TYPE_MIL_INT  );}
#endif
#if !M_MIL_USE_64BIT
inline MIL_INT MFTYPE MdispInquireSafeType(MIL_ID DisplayId, MIL_INT64 InquireType, MIL_INT64*     UserVarPtr) {return MdispInquireExecute(DisplayId, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
#endif

#if   M_MIL_USE_WINDOWS
inline MIL_INT MFTYPE MdispInquireSafeType(MIL_ID DisplayId, MIL_INT64 InquireType, MIL_WINDOW_HANDLE* ResultPtr)
   {
   if (InquireType != M_WINDOW_HANDLE)
      SafeTypeError(MIL_TEXT("MdispInquire"));

   return MdispInquire(DisplayId, InquireType, ResultPtr);
   }
#endif

// ----------------------------------------------------------
// MdispGetHookInfo

inline MIL_INT MFTYPE MdispGetHookInfoUnsafe  (MIL_ID EventId, MIL_INT64 InfoType, void            *UserVarPtr);
inline MIL_INT MFTYPE MdispGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, int             *UserVarPtr);
inline MIL_INT MFTYPE MdispGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_INT         *UserVarPtr);
inline MIL_INT MFTYPE MdispGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_DC_HANDLE   *UserVarPtr);

//safe type checking 
inline MIL_INT64 MdispGetHookInfoRequiredType(MIL_INT64 InfoType)
   {
   //check if InfoType requires a double
   if ((M_MOUSE_POSITION_BUFFER_X == InfoType) || (M_MOUSE_POSITION_BUFFER_Y == InfoType))
      {
      return M_TYPE_MIL_DOUBLE;
      }

   return M_TYPE_MIL_INT;
   }

inline MIL_INT MFTYPE MdispGetHookInfoSafeTypeExecute (MIL_ID EventId, MIL_INT64 InfoType, void* UserVarPtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MdispGetHookInfoRequiredType(InfoType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MdispGetHookInfo"));

   return MdispGetHookInfo(EventId, InfoType, UserVarPtr);
   }

inline MIL_INT MFTYPE MdispGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, int UserVarPtr)
   {
   if (UserVarPtr)
      SafeTypeError(MIL_TEXT("MdispGetHookInfo"));

   return MdispGetHookInfo(EventId, InfoType, NULL);
   }

inline MIL_INT MFTYPE MdispGetHookInfoUnsafe  (MIL_ID EventId, MIL_INT64 InfoType, void            *UserVarPtr) {return MdispGetHookInfo               (EventId, InfoType, UserVarPtr)                  ;}
inline MIL_INT MFTYPE MdispGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_INT         *UserVarPtr) {return MdispGetHookInfoSafeTypeExecute(EventId, InfoType, UserVarPtr, M_TYPE_MIL_INT  );}
inline MIL_INT MFTYPE MdispGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_DOUBLE      *UserVarPtr) {return MdispGetHookInfoSafeTypeExecute(EventId, InfoType, UserVarPtr, M_TYPE_DOUBLE   );}

inline MIL_INT MFTYPE MdispGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_DC_HANDLE   *UserVarPtr)
      {
      if (InfoType != M_ANNOTATIONS_DC)
         SafeTypeError(MIL_TEXT("MdispHookFunction"));

      return MdispGetHookInfo(EventId, InfoType, UserVarPtr);
      }

#define MdispInquire         MdispInquireSafeType
#define MdispGetHookInfo     MdispGetHookInfoSafeType

#else // #if M_MIL_USE_SAFE_TYPE

#define MdispGetHookInfoUnsafe     MdispGetHookInfo
#define MdispInquireUnsafe         MdispInquire

#endif
#endif //__MIL_DISPLAY_H__
