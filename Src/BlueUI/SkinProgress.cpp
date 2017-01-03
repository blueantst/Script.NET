// SkinProgress.cpp : implementation file
//

/******************************************************************************/
/* NAME              : SkinProgress.cpp                                       */
/* PURPOSE           : Place a bitmapped progress bar into the status bar     */
/* CREATED_BY        : Kochise (kochise@caramail.com)                         */
/* CREATION_DATE     : 2003/05/02                                             */
/* MODIFIED_BY       : Kochise (kochise@caramail.com)                         */
/* MODIFICATION_DATE : 2003/05/18                                             */
/* LANGUAGE          : MFC Visual C++ 6                                       */
/* MPROC_TYPE        : Microsoft Windows compatible computer                  */
/******************************************************************************/

#include "stdafx.h"
#include "SkinProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkinProgress

// i_Input       : i_ = Input
// m_Member      : m_ = Member
// o_Output      : o_ = Output
// ...           : use your imagination

// aArray        : a  = Array    (array)
// bBool         : b  = Boolean  (boolean, true/false)
// cConstant     : c  = Constant (constant, whatever...)
// dDefine       : d  = Define   (simple define or defined value)
// nNumber       : n  = Number   (char, long, int, whatever...)
// oObject       : o  = Object   (C++ class)
// pPointer      : p  = Pointer  (typicaly a 32 bits ulong address)
// sStruct       : s  = Struct   (structure)
// uUnion        : u  = Union    (join two or more values of the same size under a common name)

// poRemoteClass : po = Pointer on Object
// cdConstantDef : cd = Constant Define, typicaly a constant defined value
// usUnionStruct : us = Union of Structures
// ...           : use your imagination

// o_psBitmap    : CBitmap::GetBitmap((BITMAP*) o_psBitmap); // s = BITMAP, p = *, o_ means it's an output

// <Value>
// [Function]

// TYPICAL LIFE TIME
// 
// CSkinProgress(...) -------> StepIt() ----------------------------> ~CSkinProgress()   USER
//   |                           |                                       |               SIDE
// ..|...........................|.......................................|.........................
//   |                           |                                       |
//   '-> Create(...)             |                                       '-> Clear()     OBJECT
//         |                     |                                                       SIDE
//         +-> GetStatusBar()    |
//         +-> SetRange(...)     |
//         +-> SetBitmap(...)    '----------->
//         '---------------------------------> ResizeTextPane() ------,
//         ,--------------------------------->   |                    |
//         |                               ,->   |                    |
//         |                               |     |                    |
//         |                               |     +-> GetStatusBar()   |
//         |                               |     +-> GetTimed(...)    |
//         |                               |     '-> CompactText(...) |
// ........|...............................|..........................|............................
//         |                               |                          |                  WINDOWS
//       OnSizing(...) -> OnSize(...)    OnEraseBkgnd(...)            '-> OnPaint()      SIDE

#ifndef dCSP_RESOURCE_BITMAP
/*** anAQUA_BITMAP ************************************************************/
/* Purpose : The default embedded progress bar image, if resource bitmap not  */
/*           used instead.                                                    */
/* Unit    : unsigned char                                                    */
/* Range   : [0-255] - LIMITED selection                                      */
/* List    : None                                                             */
/* Example : None                                                             */
/******************************************************************************/
unsigned char anAQUA_BITMAP[5760] =
{ // B,    G,    R,    A -> Set to 0xFF for CAlphaBitmap
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xAC, 0x9C, 0x9B, 0xFF, 0x9F, 0x60, 0x5A, 0xFF, 0x9E, 0x38, 0x2B, 0xFF,
  0xA2, 0x22, 0x00, 0xFF, 0xAA, 0x22, 0x00, 0xFF, 0xAB, 0x2B, 0x00, 0xFF, 0xAB, 0x2B, 0x00, 0xFF,
  0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF,
  0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF,
  0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF,
  0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF,
  0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF,
  0xAB, 0x2B, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF, 0xAB, 0x38, 0x00, 0xFF, 0xA4, 0x46, 0x27, 0xFF,
  0x9E, 0x65, 0x54, 0xFF, 0x99, 0x89, 0x86, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
  0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
  0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
  0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
  0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
  0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xA9, 0x8A, 0x86, 0xFF, 0x9F, 0x40, 0x2E, 0xFF,
  0xA5, 0x1C, 0x00, 0xFF, 0xB1, 0x2E, 0x00, 0xFF, 0xC8, 0x6D, 0x42, 0xFF, 0xD8, 0x91, 0x67, 0xFF,
  0xDB, 0x9D, 0x76, 0xFF, 0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF,
  0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF,
  0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF,
  0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF,
  0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF,
  0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF, 0xDB, 0x9D, 0x76, 0xFF, 0xD9, 0x94, 0x6D, 0xFF,
  0xC8, 0x6D, 0x42, 0xFF, 0xB1, 0x33, 0x00, 0xFF, 0xA5, 0x1C, 0x00, 0xFF, 0x97, 0x3D, 0x22, 0xFF,
  0x8E, 0x6F, 0x6B, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
  0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
  0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
  0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
  0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
  0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
  0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
  0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
  0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xB4, 0xA5, 0xA2, 0xFF,
  0xA1, 0x46, 0x2E, 0xFF, 0xAB, 0x2B, 0x00, 0xFF, 0xAF, 0x33, 0x00, 0xFF, 0xCE, 0x76, 0x42, 0xFF,
  0xE0, 0xAB, 0x81, 0xFF, 0xE5, 0xB5, 0x89, 0xFF, 0xE7, 0xBC, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF,
  0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF,
  0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF,
  0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF,
  0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF,
  0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF,
  0xE7, 0xBC, 0x95, 0xFF, 0xE5, 0xB5, 0x89, 0xFF, 0xE3, 0xAF, 0x88, 0xFF, 0xD0, 0x7F, 0x4C, 0xFF,
  0xB1, 0x33, 0x00, 0xFF, 0xAB, 0x2B, 0x00, 0xFF, 0x97, 0x3D, 0x22, 0xFF, 0x88, 0x78, 0x76, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xA6, 0x6E, 0x5A, 0xFF, 0xAF, 0x33, 0x00, 0xFF, 0xB8, 0x3D, 0x00, 0xFF,
  0xBB, 0x45, 0x00, 0xFF, 0xD4, 0x87, 0x44, 0xFF, 0xE4, 0xAD, 0x7B, 0xFF, 0xE5, 0xB3, 0x84, 0xFF,
  0xE5, 0xB5, 0x89, 0xFF, 0xE6, 0xB8, 0x8C, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF,
  0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF,
  0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF,
  0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF,
  0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF,
  0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE6, 0xB8, 0x8C, 0xFF, 0xE5, 0xB3, 0x84, 0xFF,
  0xE4, 0xAD, 0x7B, 0xFF, 0xD8, 0x8E, 0x4E, 0xFF, 0xBB, 0x45, 0x00, 0xFF, 0xB8, 0x3D, 0x00, 0xFF,
  0xB1, 0x33, 0x00, 0xFF, 0x89, 0x50, 0x3D, 0xFF, 0x99, 0x99, 0x99, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
  0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xA6, 0x50, 0x27, 0xFF,
  0xBB, 0x45, 0x00, 0xFF, 0xBF, 0x4E, 0x00, 0xFF, 0xC1, 0x54, 0x00, 0xFF, 0xD4, 0x7A, 0x2E, 0xFF,
  0xE1, 0xA2, 0x6A, 0xFF, 0xE5, 0xAC, 0x77, 0xFF, 0xE7, 0xB1, 0x7F, 0xFF, 0xE9, 0xB4, 0x85, 0xFF,
  0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF,
  0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF,
  0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF,
  0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF,
  0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF,
  0xE5, 0xB3, 0x84, 0xFF, 0xE4, 0xAD, 0x7B, 0xFF, 0xE1, 0xA6, 0x72, 0xFF, 0xD4, 0x80, 0x38, 0xFF,
  0xC1, 0x54, 0x00, 0xFF, 0xBF, 0x4E, 0x00, 0xFF, 0xBB, 0x45, 0x00, 0xFF, 0x9B, 0x46, 0x1C, 0xFF,
  0x82, 0x82, 0x82, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xB6, 0x4C, 0x00, 0xFF, 0xC1, 0x54, 0x00, 0xFF, 0xC5, 0x5C, 0x00, 0xFF,
  0xCA, 0x63, 0x15, 0xFF, 0xD4, 0x73, 0x22, 0xFF, 0xD9, 0x7D, 0x2E, 0xFF, 0xDA, 0x84, 0x3B, 0xFF,
  0xDF, 0x8A, 0x42, 0xFF, 0xE0, 0x8D, 0x48, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF,
  0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF,
  0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF,
  0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF,
  0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF,
  0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE0, 0x8D, 0x48, 0xFF, 0xDA, 0x84, 0x3B, 0xFF,
  0xD4, 0x7A, 0x2E, 0xFF, 0xCE, 0x6F, 0x22, 0xFF, 0xC5, 0x61, 0x15, 0xFF, 0xC5, 0x5C, 0x00, 0xFF,
  0xB8, 0x55, 0x00, 0xFF, 0xB6, 0x4C, 0x00, 0xFF, 0x73, 0x73, 0x73, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xB6, 0x4C, 0x00, 0xFF,
  0xC1, 0x54, 0x00, 0xFF, 0xC5, 0x5C, 0x00, 0xFF, 0xCA, 0x63, 0x15, 0xFF, 0xD4, 0x73, 0x22, 0xFF,
  0xD9, 0x7D, 0x2E, 0xFF, 0xDA, 0x84, 0x3B, 0xFF, 0xDF, 0x8A, 0x42, 0xFF, 0xE0, 0x8D, 0x48, 0xFF,
  0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF,
  0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF,
  0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF,
  0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF,
  0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF,
  0xE0, 0x8D, 0x48, 0xFF, 0xDA, 0x84, 0x3B, 0xFF, 0xD4, 0x7A, 0x2E, 0xFF, 0xCE, 0x6F, 0x22, 0xFF,
  0xC5, 0x61, 0x15, 0xFF, 0xC5, 0x5C, 0x00, 0xFF, 0xB8, 0x55, 0x00, 0xFF, 0xB6, 0x4C, 0x00, 0xFF,
  0x73, 0x73, 0x73, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xB5, 0xB5, 0xB5, 0xFF, 0xB8, 0x55, 0x00, 0xFF, 0xC3, 0x5F, 0x15, 0xFF, 0xCC, 0x6A, 0x1C, 0xFF,
  0xD4, 0x73, 0x22, 0xFF, 0xDE, 0x82, 0x35, 0xFF, 0xE3, 0x8C, 0x42, 0xFF, 0xEA, 0x96, 0x4D, 0xFF,
  0xEB, 0x9B, 0x55, 0xFF, 0xED, 0x9E, 0x59, 0xFF, 0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF,
  0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF,
  0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF,
  0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF,
  0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF,
  0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF, 0xED, 0x9E, 0x59, 0xFF, 0xE5, 0x94, 0x4D, 0xFF,
  0xDF, 0x8A, 0x42, 0xFF, 0xD4, 0x7C, 0x33, 0xFF, 0xCE, 0x72, 0x22, 0xFF, 0xCC, 0x6A, 0x1C, 0xFF,
  0xC3, 0x5F, 0x15, 0xFF, 0xB4, 0x54, 0x00, 0xFF, 0x67, 0x67, 0x67, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
  0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xA9, 0x62, 0x2B, 0xFF,
  0xCC, 0x6A, 0x1C, 0xFF, 0xD4, 0x73, 0x22, 0xFF, 0xDA, 0x7F, 0x35, 0xFF, 0xE4, 0x90, 0x46, 0xFF,
  0xF1, 0x9D, 0x54, 0xFF, 0xF8, 0xA7, 0x60, 0xFF, 0xFB, 0xAC, 0x66, 0xFF, 0xFE, 0xB2, 0x6D, 0xFF,
  0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF,
  0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF,
  0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF,
  0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF,
  0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF,
  0xFA, 0xAF, 0x6E, 0xFF, 0xEE, 0xA2, 0x5D, 0xFF, 0xE9, 0x96, 0x50, 0xFF, 0xDF, 0x8A, 0x42, 0xFF,
  0xD4, 0x7C, 0x33, 0xFF, 0xCE, 0x72, 0x22, 0xFF, 0xC7, 0x69, 0x1C, 0xFF, 0xA0, 0x59, 0x22, 0xFF,
  0x69, 0x69, 0x69, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xC3, 0xC3, 0xC3, 0xFF, 0x9C, 0x73, 0x50, 0xFF, 0xCE, 0x72, 0x22, 0xFF, 0xD4, 0x7C, 0x33, 0xFF,
  0xE1, 0x89, 0x3D, 0xFF, 0xF1, 0x9D, 0x54, 0xFF, 0xFB, 0xAC, 0x66, 0xFF, 0xFE, 0xB2, 0x6D, 0xFF,
  0xFF, 0xBC, 0x74, 0xFF, 0xFF, 0xC4, 0x7C, 0xFF, 0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF,
  0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF,
  0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF,
  0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF,
  0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF,
  0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC4, 0x7C, 0xFF, 0xFE, 0xB2, 0x6D, 0xFF,
  0xF5, 0xA5, 0x5E, 0xFF, 0xEA, 0x96, 0x4D, 0xFF, 0xDA, 0x84, 0x3B, 0xFF, 0xD4, 0x7C, 0x33, 0xFF,
  0xCE, 0x72, 0x22, 0xFF, 0x83, 0x5A, 0x38, 0xFF, 0x82, 0x82, 0x82, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0x9B, 0x91, 0x88, 0xFF,
  0xA3, 0x6A, 0x3B, 0xFF, 0xD4, 0x80, 0x38, 0xFF, 0xE4, 0x90, 0x46, 0xFF, 0xFB, 0xA6, 0x5A, 0xFF,
  0xFF, 0xB8, 0x6E, 0xFF, 0xFF, 0xC4, 0x7C, 0xFF, 0xFF, 0xD0, 0x87, 0xFF, 0xFF, 0xCD, 0x8D, 0xFF,
  0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF,
  0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF,
  0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF,
  0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF,
  0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF,
  0xFF, 0xCD, 0x8D, 0xFF, 0xFF, 0xC4, 0x7C, 0xFF, 0xFE, 0xB2, 0x6D, 0xFF, 0xF1, 0xA0, 0x57, 0xFF,
  0xE3, 0x8C, 0x42, 0xFF, 0xD4, 0x80, 0x38, 0xFF, 0x9E, 0x63, 0x33, 0xFF, 0x66, 0x5C, 0x54, 0xFF,
  0xA5, 0xA5, 0xA5, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
  0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
  0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
  0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
  0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0x8E, 0x7B, 0x6D, 0xFF, 0xA8, 0x6F, 0x40, 0xFF,
  0xE4, 0x90, 0x46, 0xFF, 0xF9, 0xA8, 0x5F, 0xFF, 0xFF, 0xBC, 0x74, 0xFF, 0xFF, 0xCC, 0x82, 0xFF,
  0xFF, 0xD5, 0x8C, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF,
  0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF,
  0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF,
  0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF,
  0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF,
  0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xCC, 0x82, 0xFF,
  0xFF, 0xBC, 0x74, 0xFF, 0xF5, 0xA5, 0x5E, 0xFF, 0xE4, 0x90, 0x46, 0xFF, 0xA3, 0x6A, 0x3B, 0xFF,
  0x70, 0x5D, 0x4E, 0xFF, 0x7F, 0x7F, 0x7F, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
  0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
  0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
  0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
  0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
  0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
  0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
  0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
  0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
  0xAC, 0xAC, 0xAC, 0xFF, 0x85, 0x7C, 0x74, 0xFF, 0x8C, 0x6A, 0x4C, 0xFF, 0xB8, 0x80, 0x50, 0xFF,
  0xEC, 0xA3, 0x66, 0xFF, 0xFF, 0xBD, 0x7A, 0xFF, 0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xCD, 0x8D, 0xFF,
  0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF,
  0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF,
  0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF,
  0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF,
  0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF,
  0xFF, 0xD5, 0x8C, 0xFF, 0xFF, 0xC4, 0x7C, 0xFF, 0xEC, 0xA3, 0x66, 0xFF, 0xB8, 0x80, 0x50, 0xFF,
  0x85, 0x63, 0x44, 0xFF, 0x6A, 0x60, 0x59, 0xFF, 0x7F, 0x7F, 0x7F, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xB5, 0xB5, 0xB5, 0xFF,
  0x91, 0x91, 0x91, 0xFF, 0x7F, 0x7F, 0x7F, 0xFF, 0x7F, 0x7F, 0x7F, 0xFF, 0x69, 0x69, 0x69, 0xFF,
  0x67, 0x67, 0x67, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF,
  0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF,
  0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF,
  0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF,
  0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF,
  0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF,
  0x67, 0x67, 0x67, 0xFF, 0x69, 0x69, 0x69, 0xFF, 0x73, 0x73, 0x73, 0xFF, 0x91, 0x91, 0x91, 0xFF,
  0xC3, 0xC3, 0xC3, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
  0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xB5, 0xB5, 0xB5, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF,
  0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF,
  0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF,
  0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF,
  0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF,
  0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF,
  0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF,
  0xBD, 0xBD, 0xBD, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
  0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
};

/*** sAQUA_BITMAP *************************************************************/
/* Purpose : anAQUA_BITMAP's image header for CBitmap::CreateBitmapIndirect   */
/* Unit    : BITMAP                                                           */
/* Range   : structure list                                                   */
/* List    :  0 - int            bmType       :                               */
/*            1 - int            bmWidth      :                               */
/*            2 - int            bmHeight     :                               */
/*            3 - int            bmWidthBytes ;                               */
/*            4 - unsigned char  bmPlanes     ;                               */
/*            5 - unsigned char  bmBitsPixel  :                               */
/*            6 - unsigned char* bmBits       :                               */
/* Example : None                                                             */
/******************************************************************************/
BITMAP sAQUA_BITMAP =
{
  0,            // bmType
  90,           // bmWidth
  16,           // bmHeight
  360,          // bmWidthBytes
  1,            // bmPlanes
  32,           // bmBitsPixel
  anAQUA_BITMAP // bmBits
};
#endif // dCSP_RESOURCE_BITMAP

/*** anThreeDots **************************************************************/
/* Purpose : dots for reduced string                                          */
/* Unit    : char                                                             */
/* Range   : [0-127] - LIMITED selection                                      */
/* List    : None                                                             */
/* Example : None                                                             */
/******************************************************************************/
const char anThreeDots[] = "...";

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : CSkinProgress                                              */
/* Role          : Default constructor                                        */
/* Interface     : RETURN (direct value)                                      */
/*                   None                                                     */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   None                                                     */
/* Pre-condition : None                                                       */
/* Constraints   : Sets the default values to                                 */
/*                   i_poStrMessage = "Work in progress..."                   */
/*                   i_nSize        = 200          - 200 pixels FORCED WIDTH  */
/*                   i_nUpper       = 100          - from 1 to 100            */
/*                   i_nPane        = 0            - Share the text pane      */
/*                   i_nProgress    = cSPT_PERCENT - DISPLAY PERCENT          */
/*                   i_poBitmap     = NULL         - USE EMBEDDED BITMAP      */
/* Behavior      : A - Create the progress bar using default parameters       */
/*----------------------------------------------------------------------------*/
/* PROC CSkinProgress                                                         */
/*                                                                            */
/* A..... Create the progress bar using default parameters                    */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
CSkinProgress::CSkinProgress
( // Default constructor
  // Create a default progress bar with "Work in progress..." as text
)
{
// A..... Create the progress bar using default parameters
  Create("Work in progress...");
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : CSkinProgress                                              */
/* Role          : Default constructor with parameters                        */
/* Interface     : RETURN (direct value)                                      */
/*                   None                                                     */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_poStrMessage : The text message to display in pane 0   */
/*                   i_nSize        : Progress bar width in pixel IF pane 0   */
/*                   i_nUpper       : Upper limit to reach 100% (0 at 0%)     */
/*                   i_nPane        : Pane number for the progress bar        */
/*                   i_nProgress    : Progress completion message type        */
/*                   i_poBitmap     : Replacement progress bar bitmap         */
/* Pre-condition : None                                                       */
/* Constraints   : At least, <i_poStrMessage> must be provided                */
/* Behavior      : A - Create the progress bar using user's parameters        */
/*----------------------------------------------------------------------------*/
/* PROC CSkinProgress                                                         */
/*                                                                            */
/* A..... Create the progress bar using user's parameters                     */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
CSkinProgress::CSkinProgress
( // Default constructor with parameters
  LPCTSTR     i_poStrMessage, // Text to display
  int         i_nSize,        // = 200,         : Size of the progress bar if in pane 0
  int         i_nUpper,       // = 100,         : Default range from 0 to i_nUpper
  int         i_nPane,        // = 0,           : Pane number in which display the progress bar
  int         i_nProgress,    // = cSPT_PERCENT : Message type to add to the text
  CBitmap*    i_poBitmap      // = NULL         : Pointer to a user bitmap
)
{
// A..... Create the progress bar using user's parameters
  Create(i_poStrMessage, i_nSize, i_nUpper, i_nPane, i_nProgress, i_poBitmap);
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : ~CSkinProgress                                             */
/* Role          : Destructor, restore the inital states and used memory      */
/* Interface     : RETURN (direct value)                                      */
/*                   None                                                     */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   None                                                     */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Clear the progress bar                                 */
/*                 B - Delete the refresh timer                               */
/*                 C - Delete the base bitmap                                 */
/*                 C - Delete the client DC compatible base bitmap            */
/*                 D - Delete the stretched for image list bitmap             */
/*                 E - Delete the displayed progress bar bitmap               */
/*----------------------------------------------------------------------------*/
/* PROC ~CSkinProgress                                                        */
/*                                                                            */
/* A..... Clear the progress bar                                              */
/* B..... Delete the refresh timer                                            */
/* C..... Delete the <m_poBaseBmp> base bitmap                                */
/* D..... Delete the <m_poCompatibleBmp> client DC compatible base bitmap     */
/* E..... Delete the <m_poStretchBmp> stretched for image list bitmap         */
/* F..... Delete the <m_poProgressBmp> displayed progress bar bitmap          */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
CSkinProgress::~CSkinProgress
( // Destructor
)
{

  // Init

  // Process

// A..... Clear the progress bar
  Clear();

// B..... Delete the refresh timer
  if(IsWindow(this->m_hWnd))
  {
    KillTimer((UINT) this); // Use object's unique address as timer identifier
  }else{}

// C..... Delete the <m_poBaseBmp> base bitmap
  if(m_poBaseBmp != NULL)
  { // Delete the attached base bitmap
    delete m_poBaseBmp;
  }else{}

// D..... Delete the <m_poCompatibleBmp> client DC compatible base bitmap
  if(m_poCompatibleBmp != NULL)
  { // Delete the attached client DC compatible base bitmap
    delete m_poCompatibleBmp;
  }else{}

// E..... Delete the <m_poStretchBmp> stretched for image list bitmap
  if(m_poStretchBmp != NULL)
  { // Delete the attached stretched image list bitmap
    delete m_poStretchBmp;
  }else{}

// F..... Delete the <m_poProgressBmp> displayed progress bar bitmap
  if(m_poProgressBmp != NULL)
  { // Delete the attached progress bitmap
    delete m_poProgressBmp;
  }else{}
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : GetStatusBar                                               */
/* Role          : Get status bar's window handle                             */
/* Interface     : RETURN (direct value)                                      */
/*                   CStatusBar* : Window handle, else NULL if no status bar  */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   None                                                     */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Get the main window handle                             */
/*                 B - Retrieve the status bar handle                         */
/*----------------------------------------------------------------------------*/
/* PROC GetStatusBar                                                          */
/*                                                                            */
/* A..... Get the main window handle                                          */
/* B..... Retrieve the status bar handle                                      */
/* [IF main window is based on CFrameWnd]                                     */
/* : BA.... Return status bar's handle from CFrameWnd main window type        */
/* :[ELSE]                                                                    */
/* : BB.... Return status bar's handle from other main window type            */
/* [ENDIF]                                                                    */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
CXTPStatusBar* CSkinProgress::GetStatusBar
( // Get the pointer on the current status bar
)
{
  CWnd* poMainWnd;
  CWnd* poMessageBar;

  // Init

// A..... Get the main window handle
  poMainWnd = AfxGetMainWnd();

  // Process

// B..... Retrieve the status bar handle
  if(poMainWnd != NULL)
  {
    if(poMainWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
    { // If main window is a frame window, use normal methods
      poMessageBar = ((CFrameWnd*) poMainWnd)->GetMessageBar();
      
// BA.... Return status bar's handle from CFrameWnd main window type
      //return DYNAMIC_DOWNCAST(CStatusBar, poMessageBar);
	  return DYNAMIC_DOWNCAST(CXTPStatusBar, poMessageBar);
    }
    else
    { // otherwise traverse children to try and find the status bar
// BB.... Return status bar's handle from other main window type
      return DYNAMIC_DOWNCAST(CXTPStatusBar, poMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR));
    }
  }
  else
  {
    return NULL;
  }
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : Create                                                     */
/* Role          : Create the progress bar with basic parameters              */
/* Interface     : RETURN (direct value)                                      */
/*                   BOOL = false : Error during progress bar creation        */
/*                          true  : Progress bar created                      */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_poStrMessage : The text message to display in pane 0   */
/*                   i_nSize        : Progress bar width in pixel IF pane 0   */
/*                   i_nUpper       : Upper limit to reach 100% (0 at 0%)     */
/*                   i_nPane        : Pane number for the progress bar        */
/*                   i_nProgress    : Progress completion message type        */
/*                   i_poBitmap     : Replacement progress bar bitmap         */
/* Pre-condition : None                                                       */
/* Constraints   : Base of progress bar is fixed to 0, must use [SetRange] to */
/*                 change it...                                               */
/* Behavior      : A - Get status bar handler                                 */
/*                 B - Create a CStatic object upon the target pane           */
/*                 C - Set the progress bar values                            */
/*                 D - Load the bitmap                                        */
/*                 E - Get the creation date and time of the progress bar     */
/*                 F - Set the refresh timer                                  */
/*                 G - Resize the text and display the whole things           */
/*----------------------------------------------------------------------------*/
/* PROC Create                                                                */
/*                                                                            */
/* A..... Get status bar handler                                              */
/* B..... Create a CStatic object upon the target pane                        */
/* C..... Set the progress bar values                                         */
/* D..... Load the bitmap                                                     */
/* E..... Get the creation date and time of the progress bar                  */
/* F..... Set the refresh timer to 500 milliseconds                           */
/* G..... Resize the text and display the whole things                        */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
BOOL CSkinProgress::Create
( // The initialization routine
  LPCTSTR     i_poStrMessage, // Text to display
  int         i_nSize,        // = 200,         : Size of the progress bar if in pane 0
  int         i_nUpper,       // = 100,         : Default range from 0 to i_nUpper
  int         i_nPane,        // = 0,           : Pane number in which display the progress bar
  int         i_nProgress,    // = cSPT_PERCENT : Message type to add to the text
  CBitmap*    i_poBitmap      // = NULL         : Pointer to a user bitmap
)
{
  BOOL        bSuccess;
  CXTPStatusBar* poStatusBar;
  DWORD       nStyleEx;
  DWORD       nStyle;
  CRect       oRectPane;

  LPCSTR      poStrClass;
  UINT        nClassStyle;

  FILETIME    sStartTimeFile;

  // Init

  m_poBaseBmp       = NULL;
  m_poCompatibleBmp = NULL;
  m_poStretchBmp    = NULL;
  m_poProgressBmp   = NULL;

  bSuccess          = false;
// A..... Get status bar handler
  poStatusBar       = GetStatusBar();

  // Process

  if(poStatusBar != NULL)
  {
// B..... Create a CStatic object upon the target pane
    // Set the style for the custom progress bas
    nClassStyle = 0; // CS_CLASSDC | CS_HREDRAW | CS_VREDRAW;
    nStyleEx   = WS_EX_STATICEDGE;
    nStyle     = WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_CENTERIMAGE ;

    // Get CRect coordinates for requested status bar pane
    poStatusBar->GetItemRect(i_nPane, &oRectPane);

    // Create the progress bar
    poStrClass = AfxRegisterWndClass(nClassStyle, AfxGetApp()->LoadStandardCursor(IDC_WAIT));
    bSuccess   = CStatic::CreateEx(nStyleEx, poStrClass, "", nStyle, oRectPane, poStatusBar, 1);

    ASSERT(bSuccess);
    if(!bSuccess)
    {
      return false;
    }
    else
    {
// C..... Set the progress bar values
      // Set the progress text type
      m_nProgressText = i_nProgress;

      // Set range and step
      m_nLower = 0; // Set first parameters for the resampling
      ASSERT(i_nUpper > 0); // m_nLower is 0, m_nUpper CANNOT BE BEFORE m_nLower quite now
      m_nUpper = i_nUpper;
#ifdef dCSP_SLIDERBAR_METHOD
      m_nLeft = 0;
#endif // dCSP_SLIDERBAR_METHOD
      m_nRight   = 0;
      SetRange(0, i_nUpper, 1, false, false); // Resample range, and avoid display there !

      // Set and save additional values
      m_oStrMessage  = i_poStrMessage;
      SetSize(i_nSize, false); // Set the size, and avoid display there !
      m_nPane       = i_nPane;
      m_oStrPrevText = poStatusBar->GetPaneText(m_nPane); // Get the previous pane's text

#ifdef dCSP_VERTICAL_BAR
#ifndef dCSP_DISPLAY_STRETCH
      m_nVertical   = false; // *ALWAYS false IN CSkinProgress*
#else
      m_nVertical   = true;  // *FORCE* vertical for m_poStretchBmp analysis
#endif // dCSP_DISPLAY_STRETCH
#endif // dCSP_VERTICAL_BAR
      // Set the bitmap
// D..... Load the bitmap
      if(!SetBitmap(i_poBitmap, poStatusBar))
      {
        return false;
      }else{}

// E..... Get the creation date and time of the progress bar
//      GetSystemTime(&m_sStartTimeSystem);
      GetLocalTime(&m_sStartTimeSystem);
      SystemTimeToFileTime(&m_sStartTimeSystem, &sStartTimeFile);
      m_uStartTimeLarge.QuadPart   = sStartTimeFile.dwHighDateTime;
      m_uStartTimeLarge.QuadPart <<= 32;
      m_uStartTimeLarge.QuadPart  |= sStartTimeFile.dwLowDateTime;

// F..... Set the refresh timer to 500 milliseconds
#ifdef dCSP_TIMED_REDRAW
      SetTimer((UINT) this, 20, NULL); // Use object's unique address as timer identifier
#else
      SetTimer((UINT) this, 500, NULL); // Use object's unique address as timer identifier
#endif // dCSP_TIMED_REDRAW

// G..... Resize the text and display the whole things
      ResizeTextPane(); // Set the text in pane 0 and do the first drawing of the SkinProgress bar

      return true;
    }
  }
  else
  {
    return false;
  }
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : SetBitmap                                                  */
/* Role          : Create an image list for the progress bar painting         */
/* Interface     : RETURN (direct value)                                      */
/*                   BOOL = false : Error in image list creation              */
/*                          true  : Image list created and ready to use       */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_poBitmap    : Image to use, use default bitmap if NULL */
/*                   i_poStatusBar : Progress bar to target, current if NULL  */
/*                   i_bDisplay    : Display the changes                      */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Get status bar handle                                  */
/*                 B - Get the base bitmap for the image list creation        */
/*                 C - Create the stretched bitmap used for the image list    */
/*                 D - Create an image list compatible bitmap                 */
/*                 E - Create the image list                                  */
/*                 F - Display the new bitmap                                 */
/*----------------------------------------------------------------------------*/
/* PROC SetBitmap                                                             */
/*                                                                            */
/* A..... Get status bar handle                                               */
/* B..... Get the base bitmap for the <m_oBarImgLst> image list creation      */
/*   BA.... Create the base and the device compatible bitmaps                 */
/*   BB.... Load the <i_poBitmap> base bitmap                                 */
/*   BC.... Convert the <m_poBaseBmp> base bitmap in a compatible format      */
/* C..... Create the stretched bitmap used for the image list creation        */
/*   CA.... Get the pane dimension                                            */
/*   CB.... Create the bitmap, and delete the previous one if there was one   */
/*   CC.... Create a bitmap with mixed dimensions from the base and the pane  */
/*   CD.... Set the copy parameters for the stretching                        */
/* D..... Modify the base bitmap to create an image list compatible bitmap    */
/* E..... Create the <m_oBarImgLst> image list                                */
/*   EA.... Get the stretched bitmap size                                     */
/*   EB.... Calculate the size of every element of the image list             */
/*   EC.... Delete the previous image list the create a new one from scratch  */
/*   ED.... Create the new image list in 32 bits for maximun throughput       */
/*   EE.... Add the stretched bitmap in the image list                        */
/* F..... Display the new bitmap                                              */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
BOOL CSkinProgress::SetBitmap
( // Change of progress bar image
  CBitmap*    i_poBitmap,    // = NULL : Pointer to an existing bitmap
  CXTPStatusBar* i_poStatusBar, // = NULL : LEAVE THIS NULL
  BOOL        i_bDisplay     // = true : Display the changes
)
{ 
  BITMAP      sBmpSrc;
#ifdef dCSP_VERTICAL_BAR
  CBitmap     oBmpVertical;
#endif // dCSP_VERTICAL_BAR
  CBitmap*    poOldBitmap;
#ifdef dCSP_VERTICAL_BAR
  CBitmap*    poOldVertical;
#endif // dCSP_VERTICAL_BAR
  CBitmap*    poOldStretch;
  CRect       oRectBar;
  CDC         oDcBitmap;
#ifdef dCSP_VERTICAL_BAR
  CDC         oDcVertical;
#endif // dCSP_VERTICAL_BAR
  CDC         oDcStretch;

  // Init

  m_oRectPane.SetRect(0, 0, 0, 0); // Force redraw if the bitmap is changed while the SkinProgress object exist

// A..... Get status bar handle
  if(i_poStatusBar == NULL)
  { // If no specified StatusBar, get the default one
    i_poStatusBar = GetStatusBar();
  }else{}

  // Process

  if(i_poStatusBar != NULL)
  { // If any StatusBar (specified or default)
    CClientDC oDC(i_poStatusBar);

// B..... Get the <i_poBitmap> base bitmap for the <m_oBarImgLst> image list creation
// BA.... Create the base and the device compatible bitmaps
    if(m_poBaseBmp != NULL)
    { // *IF* m_poBaseBmp already exist
      delete m_poBaseBmp;
    }else{}
    m_poBaseBmp = new CBitmap; // BEWARE : DON'T INITIALIZE IT YET !

    if(m_poCompatibleBmp != NULL)
    { // *IF* m_poCompatibleBmp already exist
      delete m_poCompatibleBmp;
    }else{}
    m_poCompatibleBmp = new CBitmap; // BEWARE : DON'T INITIALIZE IT YET !

// BB.... Load the <i_poBitmap> base bitmap
    // Check if i_poBitmap is valid, otherwise use the resource's bitmap
    if(i_poBitmap == NULL)
    { // If no bitmap provided, takes the default bitmap
#ifndef dCSP_RESOURCE_BITMAP
      m_poBaseBmp->CreateBitmapIndirect(&sAQUA_BITMAP);
#else
      m_poBaseBmp->LoadBitmap(IDB_AQUABAR); // Defaut bitmap, might have to be changed !
#endif // dCSP_RESOURCE_BITMAP
    }
    else
    { // Copy the <i_poBitmap> input bitmap as the new <m_poBaseBmp> base bitmap
      CopyBitmap(m_poBaseBmp, i_poBitmap);
    }

// BC.... Convert the <m_poBaseBmp> base bitmap in a device compatible format
    ConvBitmap(m_poCompatibleBmp, m_poBaseBmp, &oDC);

    // Get bitmap width and height for the image list creation
    m_poCompatibleBmp->GetBitmap(&sBmpSrc);
    m_nBmpWidth  = sBmpSrc.bmWidth / cSPB_EnumElements; // cSPB_EnumElements == 9, the bitmap is actually composed of 9 parts
    m_nBmpHeight = sBmpSrc.bmHeight;

// C..... Create the <m_poStretchBmp> stretched bitmap used for the <m_oBarImgLst> image list creation
// CA.... Get the pane dimension (especialy its height value for the stretching)
    i_poStatusBar->GetItemRect(m_nPane, oRectBar);

// CB.... Create the bitmap, and if a previous one was already existing, delete it first
    if(m_poStretchBmp != NULL)
    { // *IF* m_poStretchBmp already exist
      delete m_poStretchBmp;
    }else{}
    m_poStretchBmp = new CBitmap;

// CC.... Create a bitmap with the base bitmap width, and the despination pane height minus 2
#ifdef dCSP_VERTICAL_BAR
    if(m_nVertical == false)
    { // Horizontal bar
#endif // dCSP_VERTICAL_BAR
#ifndef dCSP_CREATE_BITMAP_FILE
      m_poStretchBmp->CreateCompatibleBitmap(&oDC, sBmpSrc.bmWidth, oRectBar.Height() - 2); // Minus 2, with don't count the contour of the box
#else
      m_poStretchBmp->CreateCompatibleBitmap(&oDC, sBmpSrc.bmWidth, sBmpSrc.bmHeight); // BITMAP CREATION IN CURRENT VIDEO MODE !
#endif //dCSP_CREATE_BITMAP_FILE
#ifdef dCSP_VERTICAL_BAR
    }
    else
    { // Vertical bar
#ifndef dCSP_DISPLAY_STRETCH
      oBmpVertical.CreateCompatibleBitmap(&oDC, sBmpSrc.bmWidth, oRectBar.Width() - 2); // Minus 2, with don't count the contour of the box
      m_poStretchBmp->CreateCompatibleBitmap(&oDC, (oRectBar.Width() - 2) * cSPB_EnumElements, m_nBmpWidth); // Minus 2, with don't count the contour of the box
#else
      oBmpVertical.CreateCompatibleBitmap(&oDC, sBmpSrc.bmWidth, oRectBar.Height() - 2); // Minus 2, with don't count the contour of the box
      m_poStretchBmp->CreateCompatibleBitmap(&oDC, (oRectBar.Height() - 2) * cSPB_EnumElements, m_nBmpWidth); // Minus 2, with don't count the contour of the box
#endif //dCSP_DISPLAY_STRETCH
    }
#endif // dCSP_VERTICAL_BAR
   
// CD.... Set the copy parameters for the stretching
    // Set the DC
    oDcBitmap.CreateCompatibleDC(&oDC);
	  poOldBitmap  = oDcBitmap.SelectObject(m_poCompatibleBmp); // Source
    oDcStretch.CreateCompatibleDC(&oDC);
	  poOldStretch = oDcStretch.SelectObject(m_poStretchBmp); // Destination
#ifdef dCSP_VERTICAL_BAR
    if(m_nVertical == false)
    { // Horizontal bar
#endif // dCSP_VERTICAL_BAR
      oDcStretch.SetStretchBltMode(HALFTONE);
#ifdef dCSP_VERTICAL_BAR
    }
    else
    { // Vertical bar
      oDcVertical.CreateCompatibleDC(&oDC);
	    poOldVertical = oDcVertical.SelectObject(&oBmpVertical); // Destination
      oDcVertical.SetStretchBltMode(HALFTONE);
    }
#endif // dCSP_VERTICAL_BAR

// D..... Copy the <m_poCompatibleBmp> base bitmap and apply modifications to create an image list compatible bitmap
    // Copy the picture
#ifdef dCSP_VERTICAL_BAR
    if(m_nVertical == false)
    { // Horizontal bar
#endif // dCSP_VERTICAL_BAR
#ifndef dCSP_CREATE_BITMAP_FILE
      oDcStretch.StretchBlt(0, 0, sBmpSrc.bmWidth, oRectBar.Height() - 2, &oDcBitmap, 0, 0, sBmpSrc.bmWidth, sBmpSrc.bmHeight, SRCCOPY);
#else
      oDcStretch.BitBlt(0, 0, sBmpSrc.bmWidth, sBmpSrc.bmHeight, &oDcBitmap, 0, 0, SRCCOPY); // BITMAP CREATION IN CURRENT VIDEO MODE, PLEASE USE 32 BITS, AVOID MAPPED !
#endif //dCSP_CREATE_BITMAP_FILE
#ifdef dCSP_VERTICAL_BAR
    }
    else
    { // Vertical bar
#ifndef dCSP_DISPLAY_STRETCH
      oDcVertical.StretchBlt(0, 0, sBmpSrc.bmWidth, oRectBar.Width() - 2, &oDcBitmap, 0, 0, sBmpSrc.bmWidth, sBmpSrc.bmHeight, SRCCOPY);
#else
      oDcVertical.StretchBlt(0, 0, sBmpSrc.bmWidth, oRectBar.Height() - 2, &oDcBitmap, 0, 0, sBmpSrc.bmWidth, sBmpSrc.bmHeight, SRCCOPY);
#endif // dCSP_DISPLAY_STRETCH

      // Copy the stretched vertical picture into the m_poStretchBmp with part flip
#ifndef dCSP_DISPLAY_STRETCH
      for(int y = 0; y < oRectBar.Width() - 2; y += 1)
#else
      for(int y = 0; y < oRectBar.Height() - 2; y += 1)
#endif // dCSP_DISPLAY_STRETCH
      { // Scan each line
        for(int x = 0; x < sBmpSrc.bmWidth; x += 1)
        { // Scan every pixel of the line
#ifndef dCSP_DISPLAY_STRETCH
          oDcStretch.SetPixel(y + ((oRectBar.Width() - 2) * (x / m_nBmpWidth)), x % m_nBmpWidth, oDcVertical.GetPixel(x, y));
#else
          oDcStretch.SetPixel(y + ((oRectBar.Height() - 2) * (x / m_nBmpWidth)), x % m_nBmpWidth, oDcVertical.GetPixel(x, y));
#endif // dCSP_DISPLAY_STRETCH
        }
      }
    }
#endif // dCSP_VERTICAL_BAR

    // Release the DC
#ifdef dCSP_VERTICAL_BAR
    if(m_nVertical != false)
    { // Vertical bar
	    oDcVertical.SelectObject(poOldVertical);
      oDcVertical.DeleteDC();
    }else{}
#endif // dCSP_VERTICAL_BAR
    oDcStretch.SelectObject(poOldStretch);
    oDcStretch.DeleteDC();
    oDcBitmap.SelectObject(poOldBitmap);
    oDcBitmap.DeleteDC();

// E..... Create the <m_oBarImgLst> image list
// EA.... Get the stretched bitmap size
    // Get bitmap width and height for the image list creation
    m_poStretchBmp->GetBitmap(&sBmpSrc);

#ifdef dCSP_CREATE_BITMAP_FILE
    // Fast and dirty routine to save the stretched bitmap in C format, for the people who wants to recreate the AQUA_BITMAP array with their own default bitmap, instead to provide it through to the constructor
    FILE*         sFileHandle;
    unsigned char anColor[8192]; // MIGHT HAVE TO INCREASE THIS IF THE BITMAP IS TOO LARGE

    sFileHandle = fopen("C:\\aqua_bitmap.cpp", "wt"); // Change the path here
    if(sFileHandle != NULL)
    {
      int           nCount = sBmpSrc.bmWidthBytes * sBmpSrc.bmHeight;
      unsigned char nBufferHi;
      unsigned char nBufferLo;

      m_poStretchBmp->GetBitmapBits(nCount, &anColor);

      fprintf(sFileHandle, "unsigned char anAQUA_BITMAP[%d] =\n{\n ", nCount);
      for(int nPointer = 0; nPointer < nCount; nPointer += 1)
      {
        fputs(" 0x", sFileHandle);
        nBufferHi = anColor[nPointer];
        nBufferLo = nBufferHi;

        nBufferHi >>= 4;
        nBufferLo  &= 0x0F;

        if(nBufferHi < 10)
        {
          nBufferHi += '0';
        }
        else
        {
          nBufferHi += 'A' - 10;
        }

        if(nBufferLo < 10)
        {
          nBufferLo += '0';
        }
        else
        {
          nBufferLo += 'A' - 10;
        }

        fputc(nBufferHi, sFileHandle);
        fputc(nBufferLo, sFileHandle);

        fputs(",", sFileHandle);
        if((nPointer % 16) == 15)
        { // 16 pixels per line
          fputs("\n ", sFileHandle);
        }else{}
      }
      fputs("};\n\n", sFileHandle);

      fputs("BITMAP sAQUA_BITMAP =\n{\n", sFileHandle);
        fprintf(sFileHandle, "  %d,   // bmType\n",     sBmpSrc.bmType);
        fprintf(sFileHandle, "  %d,  // bmWidth\n",     sBmpSrc.bmWidth);
        fprintf(sFileHandle, "  %d,  // bmHeight\n",    sBmpSrc.bmHeight);
        fprintf(sFileHandle, "  %d, // bmWidthBytes\n", sBmpSrc.bmWidthBytes);
        fprintf(sFileHandle, "  %d,   // bmPlanes\n",   sBmpSrc.bmPlanes);
        fprintf(sFileHandle, "  %d,  // bmBitsPixel\n", sBmpSrc.bmBitsPixel);
        fputs("  anAQUA_BITMAP // bmBits\n", sFileHandle);
      fputs("};\n", sFileHandle);

      fclose(sFileHandle);
    }else{}
#endif //dCSP_CREATE_BITMAP_FILE

// EB.... Calculate the size of every element of the image list
    m_nBmpWidth  = sBmpSrc.bmWidth / cSPB_EnumElements; // cSPB_EnumElements == 5, the bitmap is yet composed of 5 parts
    m_nBmpHeight = sBmpSrc.bmHeight;

// EC.... Delete the previous image list the create a new one from scratch (useful when providing a new bitmap during SkinProgress life-time)
    // Image list creation
    m_oBarImgLst.DeleteImageList(); // Delete the previous ImageList and recreate it with the new bitmap
// ED.... Create the new image list in 32 bits to be sure it will accept any kind of bitmap in input (if 8 bits bitmap provided, will be converted in 32 bits)
    if(!m_oBarImgLst.Create(m_nBmpWidth, m_nBmpHeight, ILC_COLOR32, 0, 0)) // ILC_COLOR32 to be sure to accept any BitMap format passed through i_poBitmap
    {
      return false;
    }
    else
    {
// EE.... Add the stretched bitmap in the image list
      m_oBarImgLst.Add(m_poStretchBmp, RGB(0, 255, 255));

// F..... Display the new bitmap
      if(i_bDisplay != false)
      {
        ResizeTextPane();
      }else{}

      return true;
    }
  }
  else
  {
    return false;
  }
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : CopyBitmap                                                 */
/* Role          : Copy a bitmap                                              */
/* Interface     : RETURN (direct value)                                      */
/*                   BOOL = false : Error in copy                             */
/*                          true  : Image copied                              */
/*                 OUTPUT (pointer to value)                                  */
/*                   o_poBitmap    : Image where to copy                      */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_poBitmap    : Image to copy                            */
/* Pre-condition : Both output and input pointer must be provided !           */
/*                 Also, the output bitmap MUST BE CREATED, BUT NOT           */
/*                 INITIALIZED                                                */
/* Constraints   : Do not handle mapped bitmap yet...                         */
/* Behavior      : A - Create a remote data buffer to get the source data     */
/*                 B - Get the source data                                    */
/*                 C - Copy the bitmap                                        */
/*----------------------------------------------------------------------------*/
/* PROC CopyBitmap                                                            */
/*                                                                            */
/* A..... Create a remote data buffer to get the source data                  */
/* [IF the remote buffer is successfully created]                             */
/* : B..... Get the source data                                               */
/* : C..... Copy the bitmap                                                   */
/* [ENDIF]                                                                    */ 
/*----------------------------------------------------------------------------*/
/* As this STUPID [CBitmap::GetBitmap((BITMAP*) o_psBitmap)] returns NULL in  */
/* <o_psBitmap.bmBits>, we cannot reuse immediatly the returned BITMAP for a  */
/* [CBitmap::CreateBitmapIndirect((BITMAP*) i_psBitmap)], we have to get the  */
/* address of the data buffer by creating it by ourself, then get the datas   */
/* with [CBitmap::GetBitmapBits((DWORD) i_nDataCount, (LPVOID) i_panDataBmp)] */
/*--- END FUNCTION HEADER ----------------------------------------------------*/
BOOL CSkinProgress::CopyBitmap
( // Copy a bitmap
  CBitmap* o_poBitmap, // Pointer to an EXISTING but NOT INITIALIZED bitmap
  CBitmap* i_poBitmap  // Pointer to the source bitmap
)
{ 
  BITMAP         sBmpSrc;
  unsigned char* panDataBmp;
  DWORD          nDataCount;
  BOOL           bResult;

  // Init

  // Process

  if(
         (o_poBitmap == NULL)
      || (i_poBitmap == NULL)
    )
  {
    return false;
  }
  else
  {
// A..... Create a remote data buffer to get the source data
    i_poBitmap->GetBitmap(&sBmpSrc);
    nDataCount = sBmpSrc.bmWidthBytes * sBmpSrc.bmHeight;
    panDataBmp = new unsigned char[nDataCount];

    if(panDataBmp != NULL)
    {
// B..... Get the source data
      i_poBitmap->GetBitmapBits(nDataCount, panDataBmp);
      sBmpSrc.bmBits = panDataBmp;

// C..... Copy the bitmap
      bResult = o_poBitmap->CreateBitmapIndirect(&sBmpSrc);

      delete panDataBmp;

      return bResult;
    }
    else
    {
      return false;
    }
  }
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : ConvBitmap                                                 */
/* Role          : Convert a bitmap to a specified device context             */
/* Interface     : RETURN (direct value)                                      */
/*                   BOOL = false : Error in conversion                       */
/*                          true  : Image converted                           */
/*                 OUTPUT (pointer to value)                                  */
/*                   o_poBitmap    : Image where to copy/transform            */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_poBitmap    : Image to copy/transform                  */
/*                   i_poDC        : The DC to use for the conversion         */
/* Pre-condition : Both output and input pointer must be provided !           */
/*                 Also, the output bitmap MUST BE CREATED, BUT NOT           */
/*                 INITIALIZED                                                */
/* Constraints   : Do not handle mapped bitmap yet...                         */
/* Behavior      : A - Create a remote data buffer to get the source data     */
/*                 B - Get the target number of color                         */
/*                 C - Copy the bitmap                                        */
/*                 D - Get the source data                                    */
/*                 E - Initialize the bitmap the bitmap                       */
/*                 F - Convert the data                                       */
/*----------------------------------------------------------------------------*/
/* PROC CopyBitmap                                                            */
/*                                                                            */
/* A..... Create a remote data buffer to get the source data                  */
/* [IF the remote buffer is successfully created]                             */
/* : B..... Get the target number of color through a BITMAP structure         */
/* : [IF same color format]                                                   */
/* : : C..... Copy the bitmap                                                 */
/* : [ELSE]                                                                   */
/* : : D..... Get the source data                                             */
/* : : E..... Initialize the bitmap the bitmap                                */
/* : : F..... Convert the data                                                */
/* : [ENDIF]                                                                  */
/* [ENDIF]                                                                    */ 
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
BOOL CSkinProgress::ConvBitmap
( // Convert a bitmap to a specified device context
  CBitmap*    o_poBitmap, // Pointer to an EXISTING but NOT INITIALIZED bitmap
  CBitmap*    i_poBitmap, // Pointer to the source bitmap
  CDC*        i_poDC      // = NULL : Pointer to the DC to use for the conversion, if NULL use the current DC
)
{ 
  BITMAP         sBmpSrc;
  BITMAP         sBmpDst;
  BITMAPINFO     sBmpInfoSrc;
  CBitmap        oDstBmp;
  unsigned char* panDataBmp;
  DWORD          nDataCount;
  int            nResult;

  CBitmap*       poOldBitmap;
  CDC            oDcBitmap;

  // Init

  panDataBmp = NULL;

  // Process

  if(
         (o_poBitmap == NULL)
      || (i_poBitmap == NULL)
    )
  {
    return false;
  }
  else
  {
    if(i_poDC == NULL)
    { // If no target device context provided, use the current one
      CClientDC oDC(this);
      i_poDC = &oDC;
    }else{}

// A..... Create a remote data buffer to get the source data
    i_poBitmap->GetBitmap(&sBmpSrc);
    nDataCount = sBmpSrc.bmWidthBytes * sBmpSrc.bmHeight;
    panDataBmp = new unsigned char[nDataCount];

    if(panDataBmp != NULL)
    {
// B..... Get the target number of color through a BITMAP structure (not quite clean, I know it, sorry)
      oDstBmp.CreateCompatibleBitmap(i_poDC, sBmpSrc.bmWidth, sBmpSrc.bmHeight);
      oDstBmp.GetBitmap(&sBmpDst);

      if(sBmpSrc.bmBitsPixel == sBmpDst.bmBitsPixel)
      { // If it's the same bitmap format, just copy it instead to waste some time to convert the picture (what will fail :/ )
// C..... Copy the bitmap
        nResult = (int) CopyBitmap(o_poBitmap, i_poBitmap);
      }
      else
      {
// D..... Get the source data
        i_poBitmap->GetBitmapBits(nDataCount, panDataBmp);
        sBmpSrc.bmBits = panDataBmp;

// E..... Initialize the bitmap
        m_poCompatibleBmp->CreateCompatibleBitmap(i_poDC, sBmpSrc.bmWidth, sBmpSrc.bmHeight);

        oDcBitmap.CreateCompatibleDC(i_poDC);
	      poOldBitmap  = oDcBitmap.SelectObject(m_poCompatibleBmp); // Destination

        sBmpInfoSrc.bmiColors[0].rgbBlue      = 0;
        sBmpInfoSrc.bmiColors[0].rgbGreen     = 0;
        sBmpInfoSrc.bmiColors[0].rgbRed       = 0;
        sBmpInfoSrc.bmiColors[0].rgbReserved  = 0;

        sBmpInfoSrc.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
        sBmpInfoSrc.bmiHeader.biWidth         = sBmpSrc.bmWidth;
        sBmpInfoSrc.bmiHeader.biHeight        = 0 - sBmpSrc.bmHeight; // 0-HEIGHT : TOP-DOWN DIB, origin is upper/left corner
        sBmpInfoSrc.bmiHeader.biPlanes        = sBmpSrc.bmPlanes;
        sBmpInfoSrc.bmiHeader.biBitCount      = sBmpSrc.bmBitsPixel;
        sBmpInfoSrc.bmiHeader.biCompression   = BI_RGB;
        sBmpInfoSrc.bmiHeader.biSizeImage     = 0; // 0 : BI_RGB
        sBmpInfoSrc.bmiHeader.biXPelsPerMeter = 0;
        sBmpInfoSrc.bmiHeader.biYPelsPerMeter = 0;
        sBmpInfoSrc.bmiHeader.biClrUsed       = 0;
        sBmpInfoSrc.bmiHeader.biClrImportant  = 0;

// F..... Convert the data
        nResult = SetDIBitsToDevice(oDcBitmap, 0, 0, sBmpSrc.bmWidth, sBmpSrc.bmHeight, 0, 0, 0, sBmpSrc.bmHeight, panDataBmp, &sBmpInfoSrc, DIB_RGB_COLORS);

        oDcBitmap.SelectObject(poOldBitmap);
        oDcBitmap.DeleteDC();
      }

      delete panDataBmp;

      if(nResult != 0)
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    else
    {
      return false;
    }
  }
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : Clear                                                      */
/* Role          : Erase the progress                                         */
/* Interface     : RETURN (direct value)                                      */
/*                   None                                                     */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   None                                                     */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Set IDLE message in pane 0                             */
/*                 B - Set previous message in user pane                      */
/*                 C - Reset progress bar parameters                          */
/*                 D - Display the changes                                    */
/*----------------------------------------------------------------------------*/
/* PROC Clear                                                                 */
/*                                                                            */
/* A..... Set IDLE message in pane 0                                          */
/* B..... Set previous message in user pane                                   */
/* C..... Reset progress bar parameters                                       */
/* D..... Display the changes                                                 */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
void CSkinProgress::Clear
( // Deletion of the progress bar, restoration of the context
)
{
  CXTPStatusBar* poStatusBar;
  CString     oStrText;

  //

  if(IsWindow(GetSafeHwnd()))
  {
    ModifyStyle(WS_VISIBLE, 0); // Clear the CStatic

    poStatusBar = GetStatusBar();
    if(poStatusBar != NULL)
    {
// A..... Set IDLE message in pane 0
      m_oStrMessage.LoadString(AFX_IDS_IDLEMESSAGE); // Get the IDLE_MESSAGE and place 顃 in the status bar 
      poStatusBar->SetPaneText(0, m_oStrMessage);

// B..... Set previous message in user pane
      if(m_nPane != 0)
      { // If not the text pane, restore previous text in the pane
        poStatusBar->SetPaneText(m_nPane, m_oStrPrevText);
      }else{}

// C..... Reset progress bar parameters
      m_nProgressText = cSPT_NONE; // No information
      SetPos(m_nLower); // Force the progress bar to be cleared with the IDLE_MESSAGE
#ifdef dCSP_TIMED_REDRAW
      ResizeTextPane(); // Force to clear the progress bar in timed redraw mode
#endif // dCSP_TIMED_REDRAW

// D..... Display the changes
      poStatusBar->UpdateWindow();
    }else{}
  }else{}
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : SetText                                                    */
/* Role          : Change the text into pane 0                                */
/* Interface     : RETURN (direct value)                                      */
/*                   BOOL = false : NEVER                                     */
/*                          true  : ALWAYS                                    */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_poStrMessage : New text to display in pane 0           */
/*                   i_bDisplay    : Display the changes                      */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Set the new text                                       */
/*                 B - Display the new text                                   */
/*----------------------------------------------------------------------------*/
/* PROC SetText                                                               */
/*                                                                            */
/* A..... Set the new text                                                    */
/* B..... Display the changes                                                 */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
BOOL CSkinProgress::SetText
( // Set the new text
  LPCTSTR i_poStrMessage, // New text to display      
  BOOL    i_bDisplay      // = true : Display the changes
)
{ 

  // Init

// A..... Set the new text
  m_oStrMessage = i_poStrMessage;

  // Process

// B..... Display the changes
#ifndef dCSP_TIMED_REDRAW
  if(i_bDisplay != false)
  {
    ResizeTextPane();
  }else{}
#endif // dCSP_TIMED_REDRAW

  // Return

  return true;
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : SetSize                                                    */
/* Role          : Change the size of the progress bar if in pane 0           */
/* Interface     : RETURN (direct value)                                      */
/*                   BOOL = false : NEVER                                     */
/*                          true  : ALWAYS                                    */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_nSize    : The new size of the progress bar in pane 0  */
/*                   i_bDisplay : Display the changes                         */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Set the new size of the progress bar                   */
/*                 B - Display the resized progress bar                       */
/*----------------------------------------------------------------------------*/
/* PROC SetSize                                                               */
/*                                                                            */
/* A..... Set the new size of the progress bar                                */
/* B..... Display the resized progress bar                                    */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
BOOL CSkinProgress::SetSize
( // Set size of the progress bar in pane 0 
  int  i_nSize,   // Set the size of the progress bar
  BOOL i_bDisplay // = true : Display the changes
)
{

  // Init

// A..... Set the new size of the progress bar
  if(i_nSize > 0)
  { // Due to pane anti-disclosure system and progress bar contour, must readapt the size
    i_nSize += 5;
  }
  else if(i_nSize < 0)
  {
    i_nSize -= 2;
  }else{}

  m_nSize = i_nSize;

  // Process

// B..... Display the resized progress bar
#ifndef dCSP_TIMED_REDRAW
  if(i_bDisplay != false)
  {
    ResizeTextPane();
  }else{}
#endif // dCSP_TIMED_REDRAW

  // Return

  return true;
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : SetRange                                                   */
/* Role          : None                                                       */
/* Interface     : RETURN (direct value)                                      */
/*                   BOOL = false : Error while trying to change the range    */
/*                          true  : New range set                             */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_nLower    : Minimum value possible                     */
/*                   i_nUpper    : Maximum value possible                     */
/*                   i_nStep     : Step increment                             */
/*                   i_bResamble : Resample the current position              */
/*                   i_bDisplay  : Display the changes                        */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Resample the postion                                   */
/*                 B - Set the new range                                      */
/*                 C - Display the new ranged progress bar                    */
/*----------------------------------------------------------------------------*/
/* PROC SetRange                                                              */
/*                                                                            */
/* A..... Resample the postion from the previous range to the new one         */
/* B..... Set the new range values                                            */
/* C..... Display the new ranged progress bar                                 */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
BOOL CSkinProgress::SetRange
( // Set the new range
  int  i_nLower,    // Minimum limit
  int  i_nUpper,    // Maximum limit
  int  i_nStep,     // = 1     : Step increment
  BOOL i_bResamble, // = false : Resample the current position from the new Lower and Upper values
  BOOL i_bDisplay   // = true  : Display the changes
)
{

  // Init

  // Process + Return

  if(IsWindow(GetSafeHwnd()))
  {
    ASSERT((-0x7FFFFFFF <= i_nLower) && (i_nLower <= 0x7FFFFFFF));
    ASSERT((-0x7FFFFFFF <= i_nUpper) && (i_nUpper <= 0x7FFFFFFF));
    ASSERT(i_nLower < i_nUpper); // Seems to be stupid, but avoid zero divide problem in percent calculation below

// A..... Resample the postion from the previous range to the new one
    if(i_bResamble != false)
    { // Resample positions to readapt them to the new range
#ifdef dCSP_SLIDERBAR_METHOD
      // Recalculate start
      m_nLeft     =   (
                           ( 
                               (m_nLeft - m_nLower) // Old start
                             * (i_nUpper - i_nLower) // New range
                           )
                         / (m_nUpper - m_nLower)     // Old range
                       )
                     + i_nLower;                     // New start 
      m_nPrevLeft = m_nLeft;
#endif // dCSP_SLIDERBAR_METHOD

      // Recalculate end
      m_nRight       =   (
                           ( 
                               (m_nRight - m_nLower)   // Old end
                             * (i_nUpper - i_nLower) // New range
                           )
                         / (m_nUpper - m_nLower)     // Old range
                       )
                     + i_nLower;                     // New end 
      m_nPrevRight   = m_nRight;
    }else{}

// B..... Set the new range values
    // Sets new parameters for good
    m_nLower = i_nLower;
    m_nUpper = i_nUpper;
    m_nStep  = i_nStep;

// C..... Display the new ranged progress bar
#ifndef dCSP_TIMED_REDRAW
    if(i_bDisplay != false)
    {
      ResizeTextPane();
    }else{}
#endif // dCSP_TIMED_REDRAW

    return true;
  }
  else
  {
    return false;
  }
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : SetPos                                                     */
/* Role          : None                                                       */
/* Interface     : RETURN (direct value)                                      */
/*                   int : Previous <m_nRight> position, 0 if error           */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_nPos     : Select the new position                     */
/*                   i_bDisplay : Display the changes                         */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Jump to a specified position                           */
/*                 B - Display the changes                                    */
/*                 C - Return the previous position                           */
/*----------------------------------------------------------------------------*/
/* PROC SetPos                                                                */
/*                                                                            */
/* A..... Jump to a specified <i_nEnd> position                               */
/* B..... Display the changes                                                 */
/* C..... Return the previous <m_nRight> position                             */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
int CSkinProgress::SetPos
( // Set m_nRight value
  int  i_nPos,    // Set a new current position
  BOOL i_bDisplay // = true : Display the changes
)
{

  // Init

  // Process + Return

  if(IsWindow(GetSafeHwnd()))
  {
    ASSERT((-0x7FFFFFFF <= i_nPos) && (i_nPos <= 0x7FFFFFFF));

    if(i_nPos > m_nUpper)
    { // Avoid problems
      i_nPos = m_nUpper;
    }else{}

    m_nPrevRight = m_nRight;
// A..... Jump to a specified <i_nEnd> position
    m_nRight     = i_nPos;

// B..... Display the changes
#ifndef dCSP_TIMED_REDRAW
    if(i_bDisplay != false)
    {
      ResizeTextPane();
    }else{}
#endif // dCSP_TIMED_REDRAW

// C..... Return the previous <m_nRight> position
    return m_nPrevRight;
  }
  else
  {
    return 0;
  }
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : OffsetPos                                                  */
/* Role          : None                                                       */
/* Interface     : RETURN (direct value)                                      */
/*                   int : Previous <m_nRight> position, 0 if error           */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_nOffset  : Offset the current position                 */
/*                   i_bDisplay : Display the changes                         */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Add an offset to the current position                  */
/*                 B - Display the changes                                    */
/*                 C - Return the previous position                           */
/*----------------------------------------------------------------------------*/
/* PROC OffsetPos                                                             */
/*                                                                            */
/* A..... Add a <i_nEnd> offset to the current <m_nRight> position            */
/* B..... Display the changes                                                 */
/* C..... Return the previous <m_nRight> position                             */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
int CSkinProgress::OffsetPos
( // Forward of nOffset value
  int  i_nOffset, // Add the offset to the current position (can be negative)          
  BOOL i_bDisplay // = true : Display the changes
)
{ 

  // Init

  // Process + Return

  if(IsWindow(GetSafeHwnd()))
  {
    m_nPrevRight  = m_nRight;
// A..... Add a <i_nEnd> offset to the current <m_nRight> position
    m_nRight     += i_nOffset;

    if(m_nRight > m_nUpper)
    { // Avoid problems
      m_nRight = m_nUpper;
    }else{}

// B..... Display the changes
#ifndef dCSP_TIMED_REDRAW
    if(i_bDisplay != false)
    {
      ResizeTextPane();
    }else{}
#endif // dCSP_TIMED_REDRAW

// C..... Return the previous <m_nRight> position
    return m_nPrevRight;
  }
  else
  {
    return 0;
  }
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : SetStep                                                    */
/* Role          : None                                                       */
/* Interface     : RETURN (direct value)                                      */
/*                   int : Previous <m_nStep> setting, 0 if error             */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_nStep : Set the new step increment                     */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Set the step increment value                           */
/*                 B - Return the previous step increment value               */
/*----------------------------------------------------------------------------*/
/* PROC SetStep                                                               */
/*                                                                            */
/* A..... Set the <m_nStep> step increment value                              */
/* C..... Return the previous <m_nStep> step increment value                  */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
int CSkinProgress::SetStep
( // Set m_nStep value
  int i_nStep // Set the step increment
)             
{ 
  int PrevStep;

  // Init

  // Process + Return

  if(IsWindow(GetSafeHwnd()))
  {
    PrevStep  = m_nStep;
// A..... Set the <m_nStep> step increment value
    m_nStep   = i_nStep;

// B..... Return the previous <m_nStep> step increment value
    return PrevStep;
  }
  else
  {
    return 0;
  }
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : StepIt                                                     */
/* Role          : None                                                       */
/* Interface     : RETURN (direct value)                                      */
/*                   int : Previous <m_nRight> position, 0 if error           */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_bDisplay    : Display the changes                      */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Step the position                                      */
/*                 B - Display the changes                                    */
/*                 C - Return the previous position                           */
/*----------------------------------------------------------------------------*/
/* PROC StepIt                                                                */
/*                                                                            */
/* A..... Step the <m_nRight> position from the <m_nStep> increment value     */
/* B..... Display the changes                                                 */
/* C..... Return the previous <m_nRight> position                             */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
int CSkinProgress::StepIt
( // Forward of m_nStep value
  BOOL i_bDisplay // = true : Display the changes
) 
{ 

  // Init

  // Process + Return

  if(IsWindow(GetSafeHwnd()))
  {
    m_nPrevRight  = m_nRight;
// A..... Step the <m_nRight> position from the <m_nStep> step increment value
    m_nRight     += m_nStep;

    if(m_nRight > m_nUpper)
    { // Avoid problems
      m_nRight = m_nUpper;
    }else{}

// B..... Display the changes
#ifndef dCSP_TIMED_REDRAW
    if(i_bDisplay != false)
    {
    ResizeTextPane();
    }else{}
#endif // dCSP_TIMED_REDRAW

// C..... Return the previous <m_nRight> position
    return m_nPrevRight;
  }
  else
  {
    return 0;
  }
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : ResizeTextPane                                             */
/* Role          : None                                                       */
/* Interface     : RETURN (direct value)                                      */
/*                   BOOL = false : Error while trying to resize the text     */
/*                          true  : Process completed without error           */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   None                                                     */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Get optional message length                            */
/*                 B - Calculate progress bar size                            */
/*                 C - Compact the text in the remaining space                */
/*                 D - Create the complete information message                */
/*                 E - Modify the progress bar size according of its size     */
/*                 F - Resize the progress bar                                */
/*                 G - Display the complete information message               */
/*----------------------------------------------------------------------------*/
/* PROC ResizeTextPane                                                        */
/*                                                                            */
/* A..... Get optional message length                                         */
/* [IF there is a completion status report to add]                            */
/* : AA.... Get the length of the cSPT_PERCENT message at least               */
/* : [IF there is a timed report to provide]                                  */
/* : : AB.... Get the length of the cSPT_TIMED message also                   */
/* : [ENDIF]                                                                  */
/* [ENDIF]                                                                    */
/*   AC.... Get the length of the text to display                             */
/* B..... Calculate progress bar size                                         */
/* [IF the progress bar is in pane 0 AND have a given size (fixed or not)]    */
/* : [IF the size if fixed]                                                   */
/* : : BA.... Calculate the remaining size (minus the progress bar size)      */
/* : [ELSE]                                                                   */
/* : : BB.... Calculate the fixed size of the text to use                     */
/* : [ENDIF]                                                                  */ 
/* [ELSE]                                                                     */
/* : BC.... Use whole width of pane 0 for the text                            */
/* [ENDIF]                                                                    */
/* C..... Compact the text in the remaining space                             */
/* [IF there is no report to add to the text]                                 */
/* : CA.... Compact the text on just its length                               */
/* [ELIF there is a cSPT_PERCENT report to add to the text]                   */
/* : CB.... Compact the text on its length minus the len of cSPT_PERCENT      */
/* [ELIF there is a cSPT_TIMED report to add to the text]                     */
/* : CC.... Compact the text on its length minus the len of cSPT_TIMED        */
/* [ELIF the report have to be automaticaly selected on the remaining space]  */
/* : CD.... Compact the text according to the most usable remaining place     */
/* :   CDA... Try to compact the text with the cSPT_TIMED message at first    */
/* :   [IF the text was compacted, even a ltlle, there is no enough place]    */
/* :   : CDB... Reset the text                                                */
/* :   : CDC... Calculate the len of the "..." displayed after cSPT_PERCENT   */
/* :   : CDD... Compact the text on its length minus cSPT_PERCENT and "..."   */
/* :   : CDE... Indicate the message used is cRTP_PERCENT                     */
/* :   [ELSE]                                                                 */
/* :   : CDF... Indicate the message used is cSPT_TIMED                       */
/* :   [ENDIF]                                                                */
/* [ELSE]                                                                     */
/* : CE.... Compact the text according to just its length                     */
/* [ENDIF]                                                                    */
/* [IF there is nothing remaining from the original text]                     */
/* : CF.... Indicate the text is completly compacted                          */
/* [ENDIF]                                                                    */
/* D..... Create the complete information message                             */
/* [IF there is a cSPT_PERCENT report to add to the text]                     */
/* : DA.... Add the cSPT_PERCENT message                                      */
/* [ELIF there is a cSPT_TIMED report to add to the text]                     */
/* : DB.... Add the cSPT_TIMED message                                        */
/* [ELIF the report have to be automaticaly selected on the remaining space]  */
/* : [IF there was just enough place for cSPT_PERCENT]                        */
/* : : DC.... Add the cSPT_PERCENT message                                    */
/* : [ELSE]                                                                   */
/* : : DD.... Add the cSPT_TIMED message                                      */
/* : [ENDIF]                                                                  */ 
/* [ENDIF]                                                                    */ 
/* E..... Modify the progress bar size according of its size                  */
/* [IF packed text too large AND size not fixed OR wrapping blocked]          */
/* : EA.... Get the length of the complete text plus its margin               */
/* [ENDIF]                                                                    */ 
/* [IF text larger than the text pane]                                        */
/* : EB.... Get the maximum length of the text pane                           */
/* [ENDIF]                                                                    */ 
/*   EC.... Move the left side of the progress bar to the right of the text   */
/*   ED.... Set the new progress bar rectangle                                */
/* [IF progress bar changed AND not closed]                                   */
/* : F..... Resize the progress bar                                           */
/* [ENDIF]                                                                    */ 
/* G..... Display the complete information message                            */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
#define cRTP_BLOCK   (1 << 0)
#define cRTP_PERCENT (1 << 1)
#define cRTP_TIMED   (1 << 2)
BOOL CSkinProgress::ResizeTextPane
( // Resize the text pane and the progress bar
) 
{
  CXTPStatusBar* poStatusBar;
  CFont*      pOldFont;

  CString     oStrMessage;    // Working buffer for m_oStrMessage
  CString     oStrPercent;    // Percent of completion level
  CString     oStrTimed;      // Timed completion level

  int         nLenMessage;    // Len of m_oStrMessage in pixel
  int         nLenPercent;    // Len of oStrPercent in pixel
  int         nLenTimed;      // Len of oStrTimed in pixel
  int         nLenMargin;     // Len of 2*' ' in pixel

  int         nLenText;       // Allowed len of the text (used to wrap it)
  int         nLenPane;       // 
  int         nLenBlock;      // Flags for computation (text wrapping)

  CRect       oRectText;      // Rect of pane 0 -> oRectPane at the end of the process if m_nPane == 0
  CRect       oRectPane;      // Rect of pane m_nPane, can be also pane 0

  // Init

  ModifyStyle(0, WS_VISIBLE); // Force redraw
  InvalidateRect(NULL, false);
  nLenBlock = 0;

  // Process and Return

  if(IsWindow(GetSafeHwnd()))
  {
    poStatusBar = GetStatusBar();
    if(poStatusBar == NULL)
    {
      return false;
    }
    else
    {
      // Get StatusBar's DC
      CClientDC oDC(poStatusBar);
      pOldFont = oDC.SelectObject(poStatusBar->GetFont());

// A..... Get optional message length
      if(m_nProgressText != cSPT_NONE)
      { // Calculate progress text
// AA.... Get the length of the cSPT_PERCENT message
        oStrPercent.Format("%d%%", ((m_nRight - m_nLower) * 100) / (m_nUpper - m_nLower));
        oStrPercent = " (" + oStrPercent + ")";
        nLenPercent = oDC.GetTextExtent(oStrPercent).cx; // Length of Percent

        if(m_nProgressText >= cSPT_TIMED)
        {
// AB.... Get the length of the cSPT_TIMED message
          GetTimed(&oStrTimed);
          nLenTimed = oDC.GetTextExtent(oStrTimed).cx; // Length of Timed
        }else{}
      }else{}

// AC.... Get the length of the text to display
      oStrMessage = m_oStrMessage;
      nLenMessage = oDC.GetTextExtent(oStrMessage).cx; // Length of Message
      nLenMargin  = oDC.GetTextExtent(_T(" ")).cx * 2; // Text margin

      if(IsWindowVisible())
      { // Redraw the window text

// B..... Calculate progress bar size 
        // Now calculate the rectangle in which we will draw the progress bar
        poStatusBar->GetItemRect(0,       oRectText);
        poStatusBar->GetItemRect(m_nPane, oRectPane);

        if(
               (m_nPane == 0)
            && (m_nSize != 0)
          )
        { // If the text pane is shared with the progress bar, calculate how much space the text takes up
          if(m_nSize > 0)
          { // Force progress bar size
// BA.... Calculate the remaining size of the text once removed the fixed size of the progress bar
            nLenPane = oRectText.Width() - (m_nSize - 3); // Minus 3 to keep the same spacing with the others ways just below, to resize the text
            nLenText = nLenPane; // Use the remaining space left by the resized progress bar
          }
          else
          { // Resize the progress bar if the text is too long
// BB.... Calculate the size of the text to use (here the whole width of pane 0)
            nLenPane = oRectText.Width() + m_nSize; // *BEWARE* : m_nSize < 0 -> Get the optimal width of the progress bar before resizing it if necessary
            nLenText = oRectText.Width() - 3; // Use the whole space of pane 0, minus 3 to avoid a complete pane 0 disclosure leading to a windows bug
          }
        }
        else
        { // Resize the text if it is too long (even in pane 0 if m_nSize == 0)
// BC.... Use whole width of pane 0 for the text, and what remains for the progress bar
          nLenPane = 0; // Full length for the progress bar, might be resized for pane 0
          nLenText = oRectText.Width() - 3; // Use the whole space of pane 0, minus 3 to avoid a complete pane 0 disclosure leading to a windows bug
        }

// C..... Compact the text in the remaining space
        // Compact the text
        switch(m_nProgressText)
        {
          case cSPT_NONE :
// CA.... Compact the text according to just its length
            CompactText(&oStrMessage, &oDC, nLenText, nLenMargin);
            break;
          case cSPT_PERCENT :
// CB.... Compact the text according to its length minus the length of the cSPT_PERCENT message
            CompactText(&oStrMessage, &oDC, nLenText, nLenMargin + nLenPercent);
            break;
          case cSPT_TIMED :
// CC.... Compact the text according to its length minus the length of the cSPT_TIMED message
            CompactText(&oStrMessage, &oDC, nLenText, nLenMargin + nLenTimed);
            break;
          case cSPT_AUTOSIZE :
// CD.... Compact the text according to the most usable remaining place between the text and the progress bar
// CDA... Try to compact the text with the cSPT_TIMED message at first
            if(CompactText(&oStrMessage, &oDC, nLenText, nLenMargin + nLenTimed))
            { // If the message was compacted, try with Percent
// CDB... Reset the text
              oStrMessage = m_oStrMessage; // Restore the message

// CDC... Calculate the length of the three dots that will be displayed after the cSPT_PERCENT message
              // In case of Timed wrapping, use Percent instead, but add "..." to show that the Timed information was wrapped
              oStrPercent += "...";
              nLenPercent = oDC.GetTextExtent(oStrPercent).cx; // Length of Percent

// CDD... Compact the text according to its length minus the length of the cSPT_PERCENT message and the added three dots
              // Compact the Percent + "..." information
              CompactText(&oStrMessage, &oDC, nLenText, nLenMargin + nLenPercent);
// CDE... Indicate the message used is cRTP_PERCENT
              nLenBlock |= cRTP_PERCENT;
            }
            else
            { // There was enough place to add the Timed information
// CDF... Indicate the message used is cSPT_TIMED
              nLenBlock |= cRTP_TIMED;
            }
            break;
          default :
// CE.... Compact the text according to just its length
            CompactText(&oStrMessage, &oDC, nLenText, nLenMargin);
        }

        // Block the text wrapping if there is nothing more to wrap
        if(oStrMessage == "...")
        {
// CF.... Indicate the text is completly compacted, 
          nLenBlock |= cRTP_BLOCK;
        }else{}

// D..... Create the complete information message with the user text and the completion report
        // Add the information
        switch(m_nProgressText)
        {
          case cSPT_NONE :
            break;
          case cSPT_PERCENT :
// DA.... Add the cSPT_PERCENT message
            oStrMessage += oStrPercent;
            break;
          case cSPT_TIMED :
// DB.... Add the cSPT_TIMED message
            oStrMessage += oStrTimed;
            break;
          case cSPT_AUTOSIZE :
            if((nLenBlock & cRTP_PERCENT) != 0)
            { // If the message was compacted, try with Percent
// DC.... Add the cSPT_PERCENT message
              oStrMessage += oStrPercent;
            }
            else
            { // There was enough place to add the Timed information
// DD.... Add the cSPT_TIMED message
              oStrMessage += oStrTimed;
            }
            break;
          default :
            break;
        }

// E..... Modify the progress bar size according of its size
        if(m_nPane == 0)
        { // If the text pane is shared with the progress bar, calculate how much space the text takes up to
          nLenMessage = oDC.GetTextExtent(oStrMessage).cx; // Length of the compacted Message

          if(
                 (
                      (
                          (nLenMessage + nLenMargin) // Lenght of Message + Margin
                        > (nLenPane)                // Lenght of remaining space for text
                      )
                   && (m_nSize <= 0) // Resize of the progress bar if the text is longer, also used for m_nSize == 0
                 )
              || ((nLenBlock & cRTP_BLOCK) != 0) // Avoid paning if the text is wrapped to the max (when only "...'+information remains)
            )
          { // *IF* the packed text is larger than the space left by the resized progress bar *AND* the resizing of the progress bar is allowed *OR* the wrapping is blocked
// EA.... Get the length of the complete text plus its margin
            nLenPane = nLenMessage + nLenMargin;
          }else{}

          if(nLenPane > oRectText.Width())
          { // Make sure the length will stay positive
// EB.... Get the maximum length of the text pane
            nLenPane = oRectText.Width();
          }else{}

// EC.... Move the left side of the progress bar to the right of the text
          // Move left edge of the progress bar to the right, to leave some place for the text
          oRectText.left += nLenPane;

// ED.... Set the new progress bar rectangle
          // This is the pane to display
          oRectPane = oRectText;
        }else{}

        // If the current position is not the same that the drawed one, just change it
        if(
               (oRectPane != m_oRectPane)
            && ( // Avoid pane disclosure, so that you can reopen the window and the pane won't stay closed
                    (oRectPane.Width()  > 2)
                 && (oRectPane.Height() > 2)
               )
          )
        {
// F..... Resize the progress bar
          MoveWindow(&oRectPane);
        }else{}

// G..... Display the complete information message
        // Force text in pane 0
        poStatusBar->SetPaneText(0, oStrMessage);
        poStatusBar->UpdateWindow();
      }else{}

      // Release DC
      oDC.SelectObject(pOldFont);

      return true;
    }
  }
  else
  {
    return false;
  }
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : CompactText                                                */
/* Role          : None                                                       */
/* Interface     : RETURN (direct value)                                      */
/*                   BOOL = false : The text was not compacted                */
/*                          true  : Compaction applied successfully           */
/*                 OUTPUT (pointer to value)                                  */
/*                   io_poString  : Packed and three-doted text               */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   io_poString  : Text to compact                           */
/*                   i_poDC       : Target DC                                 */
/*                   i_nMaxWidth  : Width in pixel the text should fits in    */
/*                   i_nAddWidth  : Additional width for some purposes        */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Verify there is something to do                        */
/*                 B - Compact the string                                     */
/*                 C - Add the three dots at the end of the string            */
/*----------------------------------------------------------------------------*/
/* PROC CompactText                                                           */
/*                                                                            */
/* A..... Verify there is something to do                                     */
/* B..... Compact the string as long as it does'nt fit in the desired space   */
/* C..... Add the three dots at the end of the string                         */
/*----------------------------------------------------------------------------*/
/* Quite easy and fun to use... No trailing blank characters !                */
/*--- END FUNCTION HEADER ----------------------------------------------------*/
BOOL CSkinProgress::CompactText
(
  CString* io_poString, 
  CDC*     i_poDC,
  int      i_nMaxWidth,
  int      i_nAddWidth
)
{
  int nLenString;
  int nLenDots;
  
  // Init

  nLenString = io_poString->GetLength();

  // Process and Return

// A..... Verify there is something to do
  if(
         (nLenString == 0)
      || (i_poDC->GetTextExtent(*io_poString, nLenString).cx + i_nAddWidth <= i_nMaxWidth)
    )
  {
    return false;
  }
  else
  {
    nLenDots = i_poDC->GetTextExtent(anThreeDots, sizeof(anThreeDots)).cx;

// B..... Compact the string as long as it does'nt fit in the desired space
    do
    {
      *io_poString = io_poString->Left(io_poString->GetLength() - 1); // BEWARE : Modify the original object
      io_poString->TrimRight();               // Kill spaces 
      nLenString  = io_poString->GetLength(); // Get the real length after trim
    }
    while( // As long as something remains *AND* the stuff is bigger than the allowed space
              (nLenString > 0)
           && (i_poDC->GetTextExtent(*io_poString, nLenString).cx + nLenDots + i_nAddWidth > i_nMaxWidth)
         );

// C..... Add the three dots at the end of the string
    // Return the 'three doted' line
    *io_poString += anThreeDots;

    return true;
  }
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : GetTimed                                                   */
/* Role          : Get a complete and accurate timed text message             */
/* Interface     : RETURN (direct value)                                      */
/*                   BOOL = false : NEVER                                     */
/*                          true  : ALWAYS                                    */
/*                 OUTPUT (pointer to value)                                  */
/*                   o_poTimed : Timed message                                */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   None                                                     */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Get current date and time                              */
/*                 B - Add start date in timed message if necessary           */
/*                 C - Add start time int timed message                       */
/*                 D - Add the time elapsed from start                        */
/*                 E - Add the current progress completion status             */
/*                 F - Add the remaining progress completion                  */
/*                 G - Get the remaining time before end                      */
/*                 H - Display the expected date and time of the end          */
/*----------------------------------------------------------------------------*/
/* PROC GetTimed                                                              */
/*                                                                            */
/* A..... Get current date and time                                           */
/* B..... Add start date in timed message if necessary                        */
/* C..... Add start time int timed message                                    */
/* D..... Add the time elapsed from start                                     */
/* E..... Add the current progress completion status                          */
/* F..... Add the remaining progress completion                               */
/* G..... Get the remaining time before end                                   */
/* H..... Display the expected date and time of the end                       */
/*----------------------------------------------------------------------------*/
/* I will in the next update recode a bit this mess because if the progress   */
/* is not linear, the end time is far from being accurate. You might see that */
/* When grabing the bottom-right corner of the main window to resize it. It   */
/* block the progress, but the timer continue to elapse, so the end time      */
/* grows fast. Then, once you release the corner, the progress restart, but   */
/* then the time to finish is quite short so the remaining time decrease very */
/* fast, instead to readapt to show a 1:1 linear timer till the end.          */
/*--- END FUNCTION HEADER ----------------------------------------------------*/
BOOL CSkinProgress::GetTimed
(
  CString* o_poTimed
)
{
  CString        oStrTempo;
  unsigned long  nTempo;

  SYSTEMTIME     sCurrentTimeSystem;
  SYSTEMTIME     sTempoTimeSystem;
  FILETIME       sTempoTimeFile;
  ULARGE_INTEGER uTempoTimeLarge;
  ULARGE_INTEGER uDeltaTimeLarge;

  // Init

// A..... Get current date and time
//  GetSystemTime(&nCurrentTime);
  GetLocalTime(&sTempoTimeSystem);
  sCurrentTimeSystem = sTempoTimeSystem; // For the Date of End and Time of End
  SystemTimeToFileTime(&sTempoTimeSystem, &sTempoTimeFile);
  uTempoTimeLarge.QuadPart = sTempoTimeFile.dwHighDateTime;
  uTempoTimeLarge.QuadPart <<= 32;
  uTempoTimeLarge.QuadPart |= sTempoTimeFile.dwLowDateTime;

  // Process

  *o_poTimed = " (";

// B..... Add start date in timed message if necessary
  // Date of Start
  if(
         (sTempoTimeSystem.wYear  != m_sStartTimeSystem.wYear)
      && (sTempoTimeSystem.wMonth != m_sStartTimeSystem.wMonth)
      && (sTempoTimeSystem.wDay   != m_sStartTimeSystem.wDay)
    )
  { // *IF* Date of Start is different from Current Date
    oStrTempo.Format("%04d/%02d/%02d/%02d ", m_sStartTimeSystem.wYear, m_sStartTimeSystem.wMonth, m_sStartTimeSystem.wDay, m_sStartTimeSystem.wDayOfWeek + 1);
    *o_poTimed += oStrTempo;
  }else{}

// C..... Add start time in timed message
  // Time of Start
  oStrTempo.Format("%02d:%02d:%02d - ", m_sStartTimeSystem.wHour, m_sStartTimeSystem.wMinute, m_sStartTimeSystem.wSecond);
  *o_poTimed += oStrTempo;

// D..... Add the time elapsed from start
  // Calculate time from start
  uDeltaTimeLarge.QuadPart      = uTempoTimeLarge.QuadPart - m_uStartTimeLarge.QuadPart;
  uTempoTimeLarge.QuadPart      = uDeltaTimeLarge.QuadPart; // Keep uDeltaTimeLarge safe
  sTempoTimeFile.dwLowDateTime  = (unsigned long) uTempoTimeLarge.QuadPart;
  uTempoTimeLarge.QuadPart    >>= 32;
  sTempoTimeFile.dwHighDateTime = (unsigned long) uTempoTimeLarge.QuadPart;
  FileTimeToSystemTime(&sTempoTimeFile, &sTempoTimeSystem);
  sTempoTimeSystem.wYear       -= 1600; // Starts from 1601

  // Day from start
  if(
         (sTempoTimeSystem.wYear  != 1)
      || (sTempoTimeSystem.wMonth != 1)
      || (sTempoTimeSystem.wDay   != 1)
    )
  { // If not the same day, calculate the number of days elapsed from start
    uTempoTimeLarge.QuadPart  = uDeltaTimeLarge.QuadPart;
    uTempoTimeLarge.QuadPart /= 864000000000; // Number of days given in nanoseconds
    nTempo                    = (unsigned long) uTempoTimeLarge.QuadPart;

    if(nTempo != 0)
    { // If there is some days elapsed
      oStrTempo.Format("%d:", nTempo);
      *o_poTimed += oStrTempo;
    }else{}
  }else{}

  // Time from start
  oStrTempo.Format("%02d:%02d:%02d - ", sTempoTimeSystem.wHour, sTempoTimeSystem.wMinute, sTempoTimeSystem.wSecond);
  *o_poTimed += oStrTempo;

// E..... Add the current progress completion status
  // Elapsed percent
#ifndef dCSP_SLIDERBAR_METHOD
  nTempo = ((m_nRight - m_nLower) * 25600) / (m_nUpper - m_nLower); // 100% *256 to keep a pseudo 8-bits fixed point value (0.00390625 - 1/256 - resolution)
#else
  nTempo = ((m_nRight - m_nLeft) * 25600) / (m_nUpper - m_nLower); // 100% *256 to keep a pseudo 8-bits fixed point value (0.00390625 - 1/256 - resolution)
#endif // dCSP_SLIDERBAR_METHOD
  oStrTempo.Format("%d%% / ", nTempo >> 8); // '>> 8' == '/ 256'
  *o_poTimed += oStrTempo;

// F..... Add the remaining progress completion
  // Remaining percent
  oStrTempo.Format("%d%% - ", 100 - (nTempo >> 8));
  *o_poTimed += oStrTempo;

// G..... Get the remaining time before end
  // Remaining time
  if(nTempo == 0)
  { // Avoid zero divide
    uTempoTimeLarge.QuadPart    = 0; // No more time to wait
  }
  else
  {
    uTempoTimeLarge.QuadPart    = ((uDeltaTimeLarge.QuadPart * 25600) / nTempo) - uDeltaTimeLarge.QuadPart; // The remaining time, 100% - Elasped (100% = Elapsed / Percent)
  }
  sTempoTimeFile.dwLowDateTime  = (unsigned long) uTempoTimeLarge.QuadPart;
  uTempoTimeLarge.QuadPart    >>= 32;
  sTempoTimeFile.dwHighDateTime = (unsigned long) uTempoTimeLarge.QuadPart;
  FileTimeToSystemTime(&sTempoTimeFile, &sTempoTimeSystem);
  sTempoTimeSystem.wYear       -= 1600;

  // Day to end
  if(
         (sTempoTimeSystem.wYear  != 1)
      || (sTempoTimeSystem.wMonth != 1)
      || (sTempoTimeSystem.wDay   != 1)
    )
  { // If not the same day, calculate the number of days elapsed from start
    uTempoTimeLarge.QuadPart  = uDeltaTimeLarge.QuadPart;
    uTempoTimeLarge.QuadPart /= 864000000000; // Number of days
    nTempo                    = (unsigned long) uTempoTimeLarge.QuadPart;

    if(nTempo != 0)
    { // If there is some days elapsed
      oStrTempo.Format("%d:", nTempo);
      *o_poTimed += oStrTempo;
    }else{}
  }else{}

  // Time to end
  oStrTempo.Format("%02d:%02d:%02d - ", sTempoTimeSystem.wHour, sTempoTimeSystem.wMinute, sTempoTimeSystem.wSecond);
  *o_poTimed += oStrTempo;

// H..... Display the expected date and time of the end
  // Date of End
  if(nTempo == 0)
  { // Avoid zero divide
    uTempoTimeLarge.QuadPart    = m_uStartTimeLarge.QuadPart + uDeltaTimeLarge.QuadPart; // Current time, Delta = Tempo - Start, see above
  }
  else
  {
    uTempoTimeLarge.QuadPart    = m_uStartTimeLarge.QuadPart + ((uDeltaTimeLarge.QuadPart * 25600) / nTempo); // Start + 100% (100% = Elapsed / Percent)
  }
  sTempoTimeFile.dwLowDateTime  = (unsigned long) uTempoTimeLarge.QuadPart;
  uTempoTimeLarge.QuadPart    >>= 32;
  sTempoTimeFile.dwHighDateTime = (unsigned long) uTempoTimeLarge.QuadPart;
  FileTimeToSystemTime(&sTempoTimeFile, &sTempoTimeSystem);

  if(
         (sTempoTimeSystem.wYear  != sCurrentTimeSystem.wYear)
      && (sTempoTimeSystem.wMonth != sCurrentTimeSystem.wMonth)
      && (sTempoTimeSystem.wDay   != sCurrentTimeSystem.wDay)
    )
  {
    oStrTempo.Format("%04d/%02d/%02d/%02d ", sTempoTimeSystem.wYear, sTempoTimeSystem.wMonth, sTempoTimeSystem.wDay, sTempoTimeSystem.wDayOfWeek + 1);
    *o_poTimed += oStrTempo;
  }else{}

  // Time of End
  oStrTempo.Format("%02d:%02d:%02d)", sTempoTimeSystem.wHour, sTempoTimeSystem.wMinute, sTempoTimeSystem.wSecond);
  *o_poTimed += oStrTempo;

  // Return

  return true;
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : OnEraseBkgnd                                               */
/* Role          : Erase the background (with a filled rect) before [OnPaint] */
/* Interface     : RETURN (direct value)                                      */
/*                   None                                                     */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_poDC : current DC                                      */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Erase the background                                   */
/*                 B - Resize and display the text                            */
/*----------------------------------------------------------------------------*/
/* PROC OnEraseBkgnd                                                          */
/*                                                                            */
/* A..... Erase the background                                                */
/* B..... Resize and display the text                                         */
/*----------------------------------------------------------------------------*/
/* This function is called prior to [OnSizing] because we cannot over-ride    */
/* the status bar [OnSizing] function from here. And we also cannot do it for */
/* the main window also, which is the parent window of the status bar. So, in */
/* order to resize the text in real-time while it is moved/resized, just call */
/* the [ResizeTextPane] function when the status bar is asked to be erased.   */
/*--- END FUNCTION HEADER ----------------------------------------------------*/
BOOL CSkinProgress::OnEraseBkgnd
(
  CDC* i_poDC
)
{
  BOOL bResult;

  // Init

// A..... Erase the background
  bResult = CStatic::OnEraseBkgnd(i_poDC);

  // Process

// B..... Resize and display the text
  // 删除此处的调用，否则会导致异常(xtreme 12.0版本开始)
  //ResizeTextPane();

  // Return

  return bResult;//
}

BEGIN_MESSAGE_MAP(CSkinProgress, CStatic)
	//{{AFX_MSG_MAP(CSkinProgress)
  ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinProgress message handlers

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : OnPaint                                                    */
/* Role          : Repaint the progress bar according to its new size         */
/* Interface     : RETURN (direct value)                                      */
/*                   None                                                     */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   None                                                     */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Calculate the elements' position                       */
/*                 B - Draw the progress bar                                  */
/*                 C - Display the progress bar                               */
/*----------------------------------------------------------------------------*/
/* PROC OnPaint                                                               */
/*                                                                            */
/* A..... Calculate the elements' position                                    */
/*   AA.... Get the position in pixel of the end of the progress bar          */
/*   AB.... Get the length in pixel of the prgress bar                        */
/* [IF the lenght of the progress bar exceed the end position]                */
/* : AC... Reset the length of the progress bar to its maximum size           */
/* [ENDIF]                                                                    */ 
/* B..... Draw the progress bar                                               */
/* [IF the progress bar size is different from the previous time]             */
/* : BA.... Redraw the complete bar from scratch                              */
/* : [IF the progress bar was existing]                                       */
/* : : BAA... Delete the previous progress bar bitmap                         */
/* : [ENDIF]                                                                  */ 
/* : BAB... Create the new progress bar bitmap with the new dimension         */ 
/* [ENDIF]                                                                    */ 
/* [IF the progress bar size is different from the previous time]             */
/* : BB.... Start the complete redraw of the progress bar from the end        */
/* :   BBA... Draw the background element                                     */
/* :     BBAA.. Draw the background element at the most right position        */
/* :     [UNTIL the right end of the progress bar is not reached]             */
/* :     : BBAB.. Draw the background element until the right end of the bar  */
/* :     [NEXT]                                                               */
/* :   BBB... Save the previous position of the end of the progress bar       */
/* :   BBC... Draw the bar element                                            */
/* :     BBCA.. Reset the drawing position on a base of an element's width    */
/* :     [UNTIL the beginning is not reached]                                 */
/* :     :  BBCB.. Draw the bar element until the left end of the bar         */
/* :     [NEXT]                                                               */
/* :   BBD... Draw the center element                                         */
/* :   [IF not under the start element]                                       */
/* :   : BBE... Draw the before-left element                                  */
/* :   [ENDIF]                                                                */
/* :   [IF not under the end element]                                         */
/* :   :  BBF... Draw the after-right element                                 */
/* :   [ENDIF]                                                                */
/* :   BBG... Draw the start element                                          */
/* :   BBH... Draw the end element                                            */
/* :   BBI... Draw the left element of the bar                                */
/* :   BBJ... Draw the right element of the bar                               */
/* [ELSE]                                                                     */
/* : BC.... Modify the moved elements without redrawing everything            */
/* :   BCA... Start to update the progress bar from the previous position     */
/* :   [IF going backward (Oh my God, could it be possible ?)]                */
/* :   : BCB... Going backward                                                */
/* :   :   BCBA.. Draw the background to replace the end of the progress bar  */
/* :   :   BCBB.. Reset the drawing position on a base of an element's width  */
/* :   :   [UNTIL the right end of the progress bar is not reached]           */
/* :   :   : BCBC.. Draw the background until the right end of the bar        */
/* :   :   [NEXT]                                                             */
/* :   [ELSE]                                                                 */
/* :   : BCC... Going forward                                                 */
/* :   :   BCCA.. Draw the bar element to replace the end of the progress bar */
/* :   :   BCCB.. Reset the drawing position on a base of an element's width  */
/* :   :   [UNTIL the right end of the progress bar is not reached]           */
/* :   :   : BCCC.. Draw the progress bar until the right end of the bar      */
/* :   :   [NEXT]                                                             */
/* :   [ENDIF]                                                                */
/* :   BCD... Draw the center element                                         */
/* :   [IF moved AND not under the start element]                             */
/* :   : BCE... Draw the before-left element                                  */
/* :   [ENDIF]                                                                */
/* :   [IF moved AND not under the end element]                               */
/* :   :  BCF... Draw the after-right element                                 */
/* :   [ENDIF]                                                                */
/* :   [IF modifed by BEFORE or LEFT or CENTER]                               */
/* :   : BCG... Draw the start element                                        */
/* :   [ENDIF]                                                                */
/* :   [IF modifed by CENTER or RIGHT or AFTER]                               */
/* :   : BCH... Draw the end element                                          */
/* :   [ENDIF]                                                                */
/* :   [IF moved]                                                             */
/* :   : BCI... Draw the left element of the bar                              */
/* :   [ENDIF]                                                                */
/* :   [IF moved]                                                             */
/* :   : BCJ... Draw the right element of the bar                             */
/* :   [ENDIF]                                                                */
/* [ENDIF]                                                                    */ 
/* C..... Display the progress bar                                            */
/*----------------------------------------------------------------------------*/
/* Specific details about the dCSP_SLIDERBAR_METHOD or the dCSP_VERTICAL_BAR  */
/* are not provided because beyon the scope of the class. The code is also    */
/* fully functionnal, but is not yet intented to be used. It will surely be   */
/* a near update. But right now, anyone who wants to understand how slider    */
/* bars and/or vertical bars works just have to understand first how the      */
/* progress bars works. Hope there is enough comments for this purpose ;p     */
/*--- END FUNCTION HEADER ----------------------------------------------------*/
void CSkinProgress::OnPaint
( // On redraw event
) 
{
	CPaintDC oDC(this);   // Device context for painting - Do not call CStatic::OnPaint() for painting messages

  CDC      oDcProgress; // CompatibleDC
  CBitmap* poOldBitmap; // oDC's previous bitmap

  CRect    oRectPane;
//  CRect    rcStatusBar;

#ifdef dCSP_SLIDERBAR_METHOD
  int      nLeftPos;    // Pos of the left end of the bar
#endif // dCSP_SLIDERBAR_METHOD
  int      nRightPos;   // Pos of the right end of the bar
  int      nEndPos;     // Pos of the end of the progress bar
  int      nCurrentPos; // Pos of the current calculated position

  // Init

  GetClientRect(&oRectPane); // the CStatic currently being repaint
//  GetParent()->GetClientRect(&rcStatusBar); // For old test purpose, just stay here to recall me the syntax
//  GetParent()->ClientToScreen(&rcStatusBar);
//  ScreenToClient(&rcStatusBar);

  // Bitmap process

  // nEndPos ------------------------------------------------------,
  // nRightPos ------------------------------,                     |
  // nLeftPos ------------,                  | m_nBmpWidth --,-,   |
  //                      |<------ bar ----->|               | |   |
  // ProgressBar = [ : : :(: : : : : : : : : ) : : : : : : : : : : : ]
  //               | |                                           | |
  //               | '- 0% ---------- nRightPos ---------- 100% -+-'
  //               '- 0% ---------- nLeftPos ------------- 100% -'

// A..... Calculate the elements' position
// AA.... Get the position in pixel of the end of the progress bar
  nEndPos = oRectPane.Width() - m_nBmpWidth; // Position of the cSPB_RIGHT element
// AB.... Get the length in pixel of the prgress bar
  nRightPos = (( (m_nRight   - m_nLower) * (oRectPane.Width() - (2 * m_nBmpWidth)) ) / (m_nUpper - m_nLower)) + m_nBmpWidth; // Length of the progress bar, from cSPB_LEFT to cSPB_RIGHT
#ifdef dCSP_SLIDERBAR_METHOD
  nLeftPos  = (( (m_nLeft - m_nLower) * (oRectPane.Width() - (2 * m_nBmpWidth)) ) / (m_nUpper - m_nLower)); // Position of the cSPB_LEFT element
#endif // dCSP_SLIDERBAR_METHOD

  if(nRightPos > nEndPos)
  { // Cannot be be bigger than the bar itselves
// AC... Reset the length of the progress bar to its maximum size
    nRightPos = nEndPos;
  }else{}

#ifdef dCSP_SLIDERBAR_METHOD
  if(nLeftPos > nRightPos)
  { // Cannot be be bigger than the bar itselves
    nLeftPos = nRightPos;
  }else{}
#endif // dCSP_SLIDERBAR_METHOD

  // Process

// B..... Draw the progress bar
  if( // If the SIZE (don't mind the position) of the ProgressBar has changed
         (oRectPane.Width()  != m_oRectPane.Width())
      || (oRectPane.Height() != m_oRectPane.Height())
    )
  { // If the ProgressBar has changed
// BA.... Redraw the complete bar from scratch
    if(m_poProgressBmp != NULL)
    { // If the ProgressBitmap already exists, delete it and create a new one with the new dimension of the pane
// BAA... Delete the previous progress bar bitmap
      delete m_poProgressBmp;
    }else{}

// BAB... Create the new progress bar bitmap with the new dimension
    m_poProgressBmp = new CBitmap;
    m_poProgressBmp->CreateCompatibleBitmap(&oDC, oRectPane.Width(), oRectPane.Height()); // *ALWAYS* use '&oDC', *NEVER* '&oDcProgress' otherwise you'll get a monochrom image
  }else{}

  oDcProgress.CreateCompatibleDC(&oDC);
	poOldBitmap = oDcProgress.SelectObject(m_poProgressBmp);

  if( // If the SIZE (don't mind the position) of the ProgressBar has changed
         (oRectPane.Width()  != m_oRectPane.Width())
      || (oRectPane.Height() != m_oRectPane.Height())
    )
  { // If the ProgressBar has changed, redraw it completly
// BB.... Start the complete redraw of the progress bar from the end
#ifdef dCSP_VERTICAL_BAR
    if(m_nVertical == false)
    { // Horizontal bar
#endif // dCSP_VERTICAL_BAR
      // Background
// BBA... Draw the background element
// BBAA.. Draw the background element from the most right position
      nCurrentPos = nEndPos - (nEndPos % m_nBmpWidth);
      for(; nCurrentPos > nRightPos; nCurrentPos -= m_nBmpWidth)
      {
// BBAB.. Draw the background element until the right end of the bar
        m_oBarImgLst.Draw(&oDcProgress, cSPB_BACKGROUND, CPoint(nCurrentPos, 0), ILD_NORMAL);
      }

// BBB... Save the previous position of the end of the progress bar
      // Position of the end of the bar
      m_nPrevRightAff = nRightPos;

// BBC... Draw the bar element
      // Bar
// BBCA.. Reset the drawing position on a base of the image list element's width
      nCurrentPos -= (nCurrentPos % m_nBmpWidth);
#ifndef dCSP_SLIDERBAR_METHOD
      for(; nCurrentPos >= m_nBmpWidth; nCurrentPos -= m_nBmpWidth) // For m_nLeft-less progress bar routine
#else
      for(; nCurrentPos >= nLeftPos; nCurrentPos -= m_nBmpWidth)
#endif // dCSP_SLIDERBAR_METHOD
      {
// BBCB.. Draw the bar element until the left end of the bar
        m_oBarImgLst.Draw(&oDcProgress, cSPB_BAR, CPoint(nCurrentPos, 0), ILD_NORMAL);
      }

#ifdef dCSP_SLIDERBAR_METHOD
      // Position of the beginning of the bar
      m_nPrevLeftAff = nLeftPos;

      // Background
      nCurrentPos -= (nCurrentPos % m_nBmpWidth);
      for(; nCurrentPos >= m_nBmpWidth; nCurrentPos -= m_nBmpWidth)
      {
        m_oBarImgLst.Draw(&oDcProgress, cSPB_BACKGROUND, CPoint(nCurrentPos, 0), ILD_NORMAL);
      }
#endif // dCSP_SLIDERBAR_METHOD

// BBD... Draw the center element
      // Center
#ifndef dCSP_SLIDERBAR_METHOD
      m_oBarImgLst.Draw(&oDcProgress, cSPB_CENTER, CPoint(nRightPos / 2, 0), ILD_NORMAL);
#else
      m_oBarImgLst.Draw(&oDcProgress, cSPB_CENTER, CPoint((nLeftPos + nRightPos) / 2, 0), ILD_NORMAL);
#endif // dCSP_SLIDERBAR_METHOD

// BBE... Draw the before-left element
      // Before
#ifdef dCSP_SLIDERBAR_METHOD
      if(nLeftPos > m_nBmpWidth)
      {
        m_oBarImgLst.Draw(&oDcProgress, cSPB_BEFORE, CPoint(nLeftPos - m_nBmpWidth, 0), ILD_NORMAL);
      }else{}
#endif // dCSP_SLIDERBAR_METHOD

// BBF... Draw the after-right element
      // After
      if(nRightPos < (nEndPos - m_nBmpWidth))
      {
        m_oBarImgLst.Draw(&oDcProgress, cSPB_AFTER, CPoint(nRightPos + m_nBmpWidth, 0), ILD_NORMAL);
      }else{}

// BBG... Draw the start element
      // Start
#ifdef dCSP_SLIDERBAR_METHOD
      m_oBarImgLst.Draw(&oDcProgress, cSPB_START, CPoint(0, 0), ILD_NORMAL);
#endif // dCSP_SLIDERBAR_METHOD

// BBH... Draw the end element
      // End
      m_oBarImgLst.Draw(&oDcProgress, cSPB_END, CPoint(nEndPos, 0), ILD_NORMAL);

// BBI... Draw the left element of the bar
      // Left
#ifndef dCSP_SLIDERBAR_METHOD
      m_oBarImgLst.Draw(&oDcProgress, cSPB_LEFT, CPoint(0, 0), ILD_NORMAL); // For m_nLeft-less progress bar routine
#else
      m_oBarImgLst.Draw(&oDcProgress, cSPB_LEFT, CPoint(nLeftPos, 0), ILD_NORMAL);
#endif // dCSP_SLIDERBAR_METHOD

// BBJ... Draw the right element of the bar
      // Right
      m_oBarImgLst.Draw(&oDcProgress, cSPB_RIGHT, CPoint(nRightPos, 0), ILD_NORMAL);
#ifdef dCSP_VERTICAL_BAR
    }
    else
    { // Vertical bar
      // Background
      nCurrentPos = nEndPos - (nEndPos % m_nBmpHeight);
      for(; nCurrentPos > nRightPos; nCurrentPos -= m_nBmpHeight)
      {
        m_oBarImgLst.Draw(&oDcProgress, cSPB_BACKGROUND, CPoint(0, nCurrentPos), ILD_NORMAL);
      }

      // Position of the end of the bar
      m_nPrevRightAff = nRightPos;

      // Bar
      nCurrentPos -= (nCurrentPos % m_nBmpHeight);
#ifndef dCSP_SLIDERBAR_METHOD
      for(; nCurrentPos >= m_nBmpHeight; nCurrentPos -= m_nBmpHeight) // For m_nLeft-less progress bar routine
#else
      for(; nCurrentPos >= nLeftPos; nCurrentPos -= m_nBmpHeight)
#endif // dCSP_SLIDERBAR_METHOD
      {
        m_oBarImgLst.Draw(&oDcProgress, cSPB_BAR, CPoint(0, nCurrentPos), ILD_NORMAL);
      }

#ifdef dCSP_SLIDERBAR_METHOD
      // Position of the beginning of the bar
      m_nPrevLeftAff = nLeftPos;

      // Background
      nCurrentPos -= (nCurrentPos % m_nBmpHeight);
      for(; nCurrentPos >= 0; nCurrentPos -= m_nBmpHeight)
      {
        m_oBarImgLst.Draw(&oDcProgress, cSPB_BACKGROUND, CPoint(0, nCurrentPos), ILD_NORMAL);
      }
#endif // dCSP_SLIDERBAR_METHOD

      // Center
#ifndef dCSP_SLIDERBAR_METHOD
      m_oBarImgLst.Draw(&oDcProgress, cSPB_CENTER, CPoint(0, nRightPos / 2), ILD_NORMAL);
#else
      m_oBarImgLst.Draw(&oDcProgress, cSPB_CENTER, CPoint(0, (nLeftPos + nRightPos) / 2), ILD_NORMAL);
#endif // dCSP_SLIDERBAR_METHOD

      // Before
#ifdef dCSP_SLIDERBAR_METHOD
      if(nLeftPos > m_nBmpHeight)
      {
        m_oBarImgLst.Draw(&oDcProgress, cSPB_BEFORE, CPoint(0, nLeftPos - m_nBmpHeight), ILD_NORMAL);
      }else{}
#endif // dCSP_SLIDERBAR_METHOD

      // After
      if(nRightPos < (nEndPos - m_nBmpHeight))
      {
        m_oBarImgLst.Draw(&oDcProgress, cSPB_AFTER, CPoint(0, nRightPos + m_nBmpHeight), ILD_NORMAL);
      }else{}

      // Start
#ifdef dCSP_SLIDERBAR_METHOD
      m_oBarImgLst.Draw(&oDcProgress, cSPB_START, CPoint(0, 0), ILD_NORMAL);
#endif // dCSP_SLIDERBAR_METHOD

      // End
      m_oBarImgLst.Draw(&oDcProgress, cSPB_END, CPoint(0, nEndPos), ILD_NORMAL);

      // Top
#ifndef dCSP_SLIDERBAR_METHOD
      m_oBarImgLst.Draw(&oDcProgress, cSPB_TOP, CPoint(0, 0), ILD_NORMAL); // For m_nLeft-less progress bar routine
#else
      m_oBarImgLst.Draw(&oDcProgress, cSPB_TOP, CPoint(0, nLeftPos), ILD_NORMAL);
#endif // dCSP_SLIDERBAR_METHOD

      // Bottom
      m_oBarImgLst.Draw(&oDcProgress, cSPB_BOTTOM, CPoint(0, nRightPos), ILD_NORMAL);
    }
#endif // dCSP_VERTICAL_BAR

    m_oRectPane = oRectPane;
  }
  else
  {
// BC.... Modify the moved elements without redrawing the complete progress bar
    if(nRightPos != m_nPrevRightAff)
    {
// BCA... Start to update the progress bar from the previous position
      nCurrentPos = m_nPrevRightAff;

#ifdef dCSP_VERTICAL_BAR
      if(m_nVertical == false)
      { // Horizontal bar
#endif // dCSP_VERTICAL_BAR
        if(nRightPos < m_nPrevRightAff)
        { // If going backward, draw BACKGROUND and last with RIGHT
// BCB... Going backward
// BCBA.. Draw the background element to replace the end of the progress bar
          m_oBarImgLst.Draw(&oDcProgress, cSPB_BACKGROUND, CPoint(nCurrentPos, 0), ILD_NORMAL);
// BCBB.. Reset the drawing position on a base of the image list element's width
          nCurrentPos -= (nCurrentPos % m_nBmpWidth);
          for(; nCurrentPos > nRightPos; nCurrentPos -= m_nBmpWidth)
          {
// BCBC.. Draw the background element until the right end of the bar
            m_oBarImgLst.Draw(&oDcProgress, cSPB_BACKGROUND, CPoint(nCurrentPos, 0), ILD_NORMAL);
          }
        }
        else
        { // If going forward, draw BAR and last with RIGHT
// BCC... Going forward
// BCCA.. Draw the progress bar element to replace the end of the progress bar
          m_oBarImgLst.Draw(&oDcProgress, cSPB_BAR, CPoint(nCurrentPos, 0), ILD_NORMAL);
// BCCB.. Reset the drawing position on a base of the image list element's width
          nCurrentPos += m_nBmpWidth;
          nCurrentPos -= (nCurrentPos % m_nBmpWidth);
          for(; nCurrentPos < nRightPos; nCurrentPos += m_nBmpWidth)
          {
// BCCC.. Draw the progress bar element until the right end of the bar
            m_oBarImgLst.Draw(&oDcProgress, cSPB_BAR, CPoint(nCurrentPos, 0), ILD_NORMAL);
          }
        }
#ifdef dCSP_VERTICAL_BAR
      }
      else
      { // Vertical bar
        if(nRightPos < m_nPrevRightAff)
        { // If going backward, draw BACKGROUND and last with BOTTOM
          m_oBarImgLst.Draw(&oDcProgress, cSPB_BACKGROUND, CPoint(0, nCurrentPos), ILD_NORMAL);
          nCurrentPos -= (nCurrentPos % m_nBmpHeight);
          for(; nCurrentPos > nRightPos; nCurrentPos -= m_nBmpHeight)
          {
            m_oBarImgLst.Draw(&oDcProgress, cSPB_BACKGROUND, CPoint(0, nCurrentPos), ILD_NORMAL);
          }
        }
        else
        { // If going forward, draw BAR and last with BOTTOM
          m_oBarImgLst.Draw(&oDcProgress, cSPB_BAR, CPoint(0, nCurrentPos), ILD_NORMAL);
          nCurrentPos += m_nBmpHeight;
          nCurrentPos -= (nCurrentPos % m_nBmpHeight);
          for(; nCurrentPos < nRightPos; nCurrentPos += m_nBmpHeight)
          {
            m_oBarImgLst.Draw(&oDcProgress, cSPB_BAR, CPoint(0, nCurrentPos), ILD_NORMAL);
          }
        }
      }
#endif // dCSP_VERTICAL_BAR
    }else{}

#ifdef dCSP_SLIDERBAR_METHOD
    if(nLeftPos != m_nPrevLeftAff)
    {
      nCurrentPos = m_nPrevLeftAff;

#ifdef dCSP_VERTICAL_BAR
      if(m_nVertical == false)
      { // Horizontal bar
#endif // dCSP_VERTICAL_BAR
        if(nLeftPos < m_nPrevLeftAff)
        { // If going backward, draw BAR and last with LEFT
          m_oBarImgLst.Draw(&oDcProgress, cSPB_BAR, CPoint(nCurrentPos, 0), ILD_NORMAL);
          nCurrentPos -= (nCurrentPos % m_nBmpWidth);
          for(; nCurrentPos > nLeftPos; nCurrentPos -= m_nBmpWidth)
          {
            m_oBarImgLst.Draw(&oDcProgress, cSPB_BAR, CPoint(nCurrentPos, 0), ILD_NORMAL);
          }
        }
        else
        { // If going forward, draw BACKGROUND and last with LEFT
          m_oBarImgLst.Draw(&oDcProgress, cSPB_BACKGROUND, CPoint(nCurrentPos, 0), ILD_NORMAL);
          nCurrentPos += m_nBmpWidth;
          nCurrentPos -= (nCurrentPos % m_nBmpWidth);
          for(; nCurrentPos < nLeftPos; nCurrentPos += m_nBmpWidth)
          {
            m_oBarImgLst.Draw(&oDcProgress, cSPB_BACKGROUND, CPoint(nCurrentPos, 0), ILD_NORMAL);
          }
        }
#ifdef dCSP_VERTICAL_BAR
      }
      else
      { // Vertical bar
        if(nLeftPos < m_nPrevLeftAff)
        { // If going backward, draw BAR and last with TOP
          m_oBarImgLst.Draw(&oDcProgress, cSPB_BAR, CPoint(0, nCurrentPos), ILD_NORMAL);
          nCurrentPos -= (nCurrentPos % m_nBmpHeight);
          for(; nCurrentPos > nLeftPos; nCurrentPos -= m_nBmpHeight)
          {
            m_oBarImgLst.Draw(&oDcProgress, cSPB_BAR, CPoint(0, nCurrentPos), ILD_NORMAL);
          }
        }
        else
        { // If going forward, draw BACKGROUND and last with TOP
          m_oBarImgLst.Draw(&oDcProgress, cSPB_BACKGROUND, CPoint(0, nCurrentPos), ILD_NORMAL);
          nCurrentPos += m_nBmpHeight;
          nCurrentPos -= (nCurrentPos % m_nBmpHeight);
          for(; nCurrentPos < nLeftPos; nCurrentPos += m_nBmpHeight)
          {
            m_oBarImgLst.Draw(&oDcProgress, cSPB_BACKGROUND, CPoint(0, nCurrentPos), ILD_NORMAL);
          }
        }
      }
#endif // dCSP_VERTICAL_BAR
    }else{}
#endif // dCSP_SLIDERBAR_METHOD

// BCD... Draw the center element
    if(
           (nRightPos != m_nPrevRightAff)
#ifdef dCSP_SLIDERBAR_METHOD
        || (nLeftPos != m_nPrevLeftAff)
#endif // dCSP_SLIDERBAR_METHOD
      )
    {
#ifdef dCSP_VERTICAL_BAR
      if(m_nVertical == false)
      { // Horizontal bar
#endif // dCSP_VERTICAL_BAR
#ifndef dCSP_SLIDERBAR_METHOD
        m_oBarImgLst.Draw(&oDcProgress, cSPB_CENTER, CPoint(nRightPos / 2, 0), ILD_NORMAL);
#else
        m_oBarImgLst.Draw(&oDcProgress, cSPB_CENTER, CPoint((nLeftPos + nRightPos) / 2, 0), ILD_NORMAL);
#endif // dCSP_SLIDERBAR_METHOD
#ifdef dCSP_VERTICAL_BAR
      }
      else
      { // Vertical bar
#ifndef dCSP_SLIDERBAR_METHOD
        m_oBarImgLst.Draw(&oDcProgress, cSPB_CENTER, CPoint(0, nRightPos / 2), ILD_NORMAL);
#else
        m_oBarImgLst.Draw(&oDcProgress, cSPB_CENTER, CPoint(0, (nLeftPos + nRightPos) / 2), ILD_NORMAL);
#endif // dCSP_SLIDERBAR_METHOD
      }
#endif // dCSP_VERTICAL_BAR
    }

// BCE... Draw the before-left element
    // Before
#ifdef dCSP_SLIDERBAR_METHOD
    if(nLeftPos != m_nPrevLeftAff)
    {
#ifdef dCSP_VERTICAL_BAR
      if(m_nVertical == false)
      { // Horizontal bar
#endif // dCSP_VERTICAL_BAR
        if(nLeftPos > m_nBmpWidth)
        {
          m_oBarImgLst.Draw(&oDcProgress, cSPB_BEFORE, CPoint(nLeftPos - m_nBmpWidth, 0), ILD_NORMAL);
        }else{}
#ifdef dCSP_VERTICAL_BAR
      }
      else
      { // Vertical bar
        if(nLeftPos > m_nBmpHeight)
        {
          m_oBarImgLst.Draw(&oDcProgress, cSPB_BEFORE, CPoint(0, nLeftPos - m_nBmpHeight), ILD_NORMAL);
        }else{}
      }
#endif // dCSP_VERTICAL_BAR
    }else{}
#endif // dCSP_SLIDERBAR_METHOD

// BCF... Draw the after-right element
    // After
    if(nRightPos != m_nPrevRightAff)
    {
#ifdef dCSP_VERTICAL_BAR
      if(m_nVertical == false)
      { // Horizontal bar
#endif // dCSP_VERTICAL_BAR
        if(nRightPos < (nEndPos - m_nBmpWidth))
        {
          m_oBarImgLst.Draw(&oDcProgress, cSPB_AFTER, CPoint(nRightPos + m_nBmpWidth, 0), ILD_NORMAL);
        }else{}
#ifdef dCSP_VERTICAL_BAR
      }
      else
      { // Vertical bar
        if(nRightPos < (nEndPos - m_nBmpHeight))
        {
          m_oBarImgLst.Draw(&oDcProgress, cSPB_AFTER, CPoint(0, nRightPos + m_nBmpHeight), ILD_NORMAL);
        }else{}
      }
#endif // dCSP_VERTICAL_BAR
    }else{}

// BCG... Draw the start element
    // Start
#ifdef dCSP_SLIDERBAR_METHOD
    if(nLeftPos != m_nPrevLeftAff)
#else
    if(nRightPos != m_nPrevRightAff)
#endif // dCSP_SLIDERBAR_METHOD
    {
#ifdef dCSP_VERTICAL_BAR
      if(m_nVertical == false)
      { // Horizontal bar
#endif // dCSP_VERTICAL_BAR
#ifdef dCSP_SLIDERBAR_METHOD
        if(nLeftPos < (2 * m_nBmpWidth))
#else
        if(nRightPos < (2 * m_nBmpWidth))
#endif // dCSP_SLIDERBAR_METHOD
        {
          m_oBarImgLst.Draw(&oDcProgress, cSPB_START, CPoint(0, 0), ILD_NORMAL);
        }else{}
#ifdef dCSP_VERTICAL_BAR
      }
      else
      { // Vertical bar
#ifdef dCSP_SLIDERBAR_METHOD
        if(nLeftPos < (2 * m_nBmpHeight))
#else
        if(nRightPos < (2 * m_nBmpWidth))
#endif // dCSP_SLIDERBAR_METHOD
        {
          m_oBarImgLst.Draw(&oDcProgress, cSPB_START, CPoint(0, 0), ILD_NORMAL);
        }else{}
      }
#endif // dCSP_VERTICAL_BAR
    }else{}

// BCH... Draw the end element
    // End
    if(nRightPos != m_nPrevRightAff)
    {
#ifdef dCSP_VERTICAL_BAR
      if(m_nVertical == false)
      { // Horizontal bar
#endif // dCSP_VERTICAL_BAR
        if(nRightPos > (nEndPos - (2 * m_nBmpWidth)))
        {
          m_oBarImgLst.Draw(&oDcProgress, cSPB_END, CPoint(nEndPos, 0), ILD_NORMAL);
        }else{}
#ifdef dCSP_VERTICAL_BAR
      }
      else
      { // Vertical bar
        if(nRightPos > (nEndPos - (2 * m_nBmpHeight)))
        {
          m_oBarImgLst.Draw(&oDcProgress, cSPB_END, CPoint(0, nEndPos), ILD_NORMAL);
        }else{}
      }
#endif // dCSP_VERTICAL_BAR
    }else{}

// BCI... Draw the left element of the bar
#ifdef dCSP_SLIDERBAR_METHOD
    if(nLeftPos != m_nPrevLeftAff)
    {
      // Position of the start of the bar
      m_nPrevLeftAff = nLeftPos;

#ifdef dCSP_VERTICAL_BAR
      if(m_nVertical == false)
      { // Horizontal bar
#endif // dCSP_VERTICAL_BAR
        // Left
        m_oBarImgLst.Draw(&oDcProgress, cSPB_LEFT, CPoint(nLeftPos, 0), ILD_NORMAL);
#ifdef dCSP_VERTICAL_BAR
      }
      else
      { // Vertical bar
        // Top
        m_oBarImgLst.Draw(&oDcProgress, cSPB_TOP, CPoint(0, nLeftPos), ILD_NORMAL);
      }
#endif // dCSP_VERTICAL_BAR
    }else{}
#else
#ifdef dCSP_VERTICAL_BAR
      if(m_nVertical == false)
      { // Horizontal bar
#endif // dCSP_VERTICAL_BAR
        // Left
        if(nRightPos < (2 * m_nBmpWidth))
        {
          m_oBarImgLst.Draw(&oDcProgress, cSPB_LEFT, CPoint(0, 0), ILD_NORMAL);
        }else{}
#ifdef dCSP_VERTICAL_BAR
      }
      else
      { // Vertical bar
        // Top
        if(nRightPos < (2 * m_nBmpHeight))
        {
          m_oBarImgLst.Draw(&oDcProgress, cSPB_TOP, CPoint(0, 0), ILD_NORMAL);
        }else{}
      }
#endif // dCSP_VERTICAL_BAR
#endif // dCSP_SLIDERBAR_METHOD

// BCJ... Draw the right element of the bar
    if(nRightPos != m_nPrevRightAff)
    {
      // Position of the end of the bar
      m_nPrevRightAff = nRightPos;

#ifdef dCSP_VERTICAL_BAR
      if(m_nVertical == false)
      { // Horizontal bar
#endif // dCSP_VERTICAL_BAR
        // Right
        m_oBarImgLst.Draw(&oDcProgress, cSPB_RIGHT, CPoint(nRightPos, 0), ILD_NORMAL);
#ifdef dCSP_VERTICAL_BAR
      }
      else
      { // Vertical bar
        // Bottom
        m_oBarImgLst.Draw(&oDcProgress, cSPB_BOTTOM, CPoint(0, nRightPos), ILD_NORMAL);
      }
#endif // dCSP_VERTICAL_BAR
    }else{}
  }

// C..... Display the progress bar
  // Copy the progress bitmap each time the object have to be refreshed
#ifndef dCSP_DISPLAY_STRETCH
  oDC.BitBlt(0, 0, oRectPane.Width(), oRectPane.Height(), &oDcProgress, 0, 0, SRCCOPY);
#else
  CDC      oDcStretch;   // CompatibleDC
  CBitmap* poOldStretch; // oDC's previous bitmap
  BITMAP   bmStretch;

  oDcStretch.CreateCompatibleDC(&oDC);
	poOldStretch = oDcStretch.SelectObject(m_poStretchBmp);

  m_poStretchBmp->GetBitmap(&bmStretch);
  oDC.BitBlt(0, 0, bmStretch.bmWidth, bmStretch.bmHeight, &oDcStretch, 0, 0, SRCCOPY);

  oDcStretch.SelectObject(poOldStretch);
  oDcStretch.DeleteDC();
#endif // dCSP_DISPLAY_STRETCH

  // Release the DC
  oDcProgress.SelectObject(poOldBitmap);
  oDcProgress.DeleteDC();
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : OnSizing                                                   */
/* Role          : Before things are resized                                  */
/* Interface     : RETURN (direct value)                                      */
/*                   None                                                     */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   fwSide : Edge of window to be moved                      */
/*                   pRect  : New rectangle                                   */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Resize the object                                      */
/*                 B - Resize the text                                        */
/*----------------------------------------------------------------------------*/
/* PROC OnSizing                                                              */
/*                                                                            */
/* A..... Resize the object                                                   */
/* B..... Resize the text                                                     */
/*----------------------------------------------------------------------------*/
/* In normal cases, this is NEVER called                                      */
/*--- END FUNCTION HEADER ----------------------------------------------------*/
void CSkinProgress::OnSizing
(
  UINT   fwSide, // Edge of window to be moved
  LPRECT pRect   // New rectangle
)
{
// A..... Resize the object
	CStatic::OnSizing(fwSide, pRect);

  // Process

// B..... Resize the text
  ResizeTextPane(); // Recalculate text layout on sizing the window
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : OnSize                                                     */
/* Role          : Once things were resized                                   */
/* Interface     : RETURN (direct value)                                      */
/*                   None                                                     */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   nType : Type of resizing requested                       */
/*                   cx    : Width                                            */
/*                   cy    : Height                                           */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Transmit the new dimensions                            */
/*----------------------------------------------------------------------------*/
/* PROC OnSize                                                                */
/*                                                                            */
/* A..... Transmit the new dimensions                                         */
/*----------------------------------------------------------------------------*/
/* Just over-ridden, ready to use for various purposes ;)                     */
/*--- END FUNCTION HEADER ----------------------------------------------------*/
void CSkinProgress::OnSize
(
  UINT nType, // Type of resizing requested 
  int  cx,    // Width
  int  cy     // Height
)
{
// A..... Transmit the new dimensions
	CStatic::OnSize(nType, cx, cy);
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : OnTimer                                                    */
/* Role          : The object timer has elapsed                               */
/* Interface     : RETURN (direct value)                                      */
/*                   None                                                     */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   nIDEvent : The Timer handle that elapsed                 */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Refresh the text and the progress bar                  */
/*                 B - Transmit the Timer handle to the parent                */
/*----------------------------------------------------------------------------*/
/* PROC OnTimer                                                               */
/*                                                                            */
/* A..... Refresh the text and the progress bar                               */
/* B..... Transmit the Timer handle to the parent                             */
/*----------------------------------------------------------------------------*/
/* Just used to refresh the timed message when things are going too slow...   */
/* Also used when on time redrawing the progress bar, to limit to 50 refresh  */
/* per second...                                                              */
/*--- END FUNCTION HEADER ----------------------------------------------------*/
void CSkinProgress::OnTimer
( // On a Timer event
  UINT nIDEvent
)
{
  if(nIDEvent == (UINT) this) // Use object's unique address as timer identifier
  {
// A..... Refresh the text and the progress bar
    ResizeTextPane(); // Refresh the whole stuff, each 500 ms or 20 ms if in dCSP_TIMED_REDRAW mode
  }else{}

// B..... Transmit the Timer handle to the parent	
	CStatic::OnTimer(nIDEvent);
}
