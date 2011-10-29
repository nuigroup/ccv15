//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This library is part of CL NUI SDK
// It allows the use of Microsoft Kinect cameras in your own applications
//
// For updates and file downloads go to: http://codelaboratories.com/get/kinect
//
// Copyright 2010 (c) Code Laboratories, Inc.  All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <windows.h>

#define IMPORT(type) extern "C" __declspec(dllimport)## type __cdecl

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NUIDevice  API
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPORT(int) GetNUIDeviceCount();
IMPORT(PCHAR) GetNUIDeviceSerial(int index);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLNUIMotor  API
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Motor instance type
typedef void *CLNUIMotor;

// Library initialization
IMPORT(CLNUIMotor) CreateNUIMotor(PCHAR serial);
IMPORT(bool) DestroyNUIMotor(CLNUIMotor mot);

// Motor control
IMPORT(bool) SetNUIMotorPosition(CLNUIMotor mot, SHORT position);

// Get accelerometer data
IMPORT(bool) GetNUIMotorAccelerometer(CLNUIMotor mot, SHORT &x, SHORT &y, SHORT &z);

IMPORT(bool) SetNUIMotorLED(CLNUIMotor mot, BYTE value);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLNUICamera API
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Camera instance type
typedef void *CLNUICamera;

// Library initialization
IMPORT(CLNUICamera) CreateNUICamera(PCHAR serial);
IMPORT(bool) DestroyNUICamera(CLNUICamera cam);

// Camera capture control
IMPORT(bool) StartNUICamera(CLNUICamera cam);
IMPORT(bool) StopNUICamera(CLNUICamera cam);

// Camera video frame image data retrieval
IMPORT(bool) GetNUICameraColorFrameRAW(CLNUICamera cam, PBYTE pData, int waitTimeout = 2000);
IMPORT(bool) GetNUICameraColorFrameRGB24(CLNUICamera cam, PBYTE pData, int waitTimeout = 2000);
IMPORT(bool) GetNUICameraColorFrameRGB32(CLNUICamera cam, PDWORD pData, int waitTimeout = 2000);

// Camera depth frame image data retrieval
IMPORT(bool) GetNUICameraDepthFrameRAW(CLNUICamera cam, PUSHORT pData, int waitTimeout = 2000);
IMPORT(bool) GetNUICameraDepthFrameCorrected12(CLNUICamera cam, PUSHORT pData, int waitTimeout = 2000);
IMPORT(bool) GetNUICameraDepthFrameCorrected8(CLNUICamera cam, PBYTE pData, int waitTimeout = 2000);
IMPORT(bool) GetNUICameraDepthFrameRGB32(CLNUICamera cam, PDWORD pData, int waitTimeout = 2000);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
