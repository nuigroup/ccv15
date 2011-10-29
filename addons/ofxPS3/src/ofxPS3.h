/*
*  ofxPS3.h
*  
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/
#ifndef OFXPS3_H
#define OFXPS3_H

#pragma comment(lib, "comctl32.lib")

#include "ofxCameraBase.h"
#include "CLEyeMulticam.h"
#include  <windows.h>
#include  <commctrl.h>

#define  ID_SPLITTER    1

class ofxPS3 : ofxCameraBase
{
public:
	ofxPS3();
	~ofxPS3();
	void setCameraFeature(CAMERA_BASE_FEATURE featureCode,int firstValue,int secondValue,bool isAuto,bool isEnabled);
	void getCameraFeature(CAMERA_BASE_FEATURE featureCode,int* firstValue,int* secondValue, bool* isAuto, bool* isEnabled,int* minValue,int* maxValue);
	int getCameraBaseCount();
	GUID* getBaseCameraGuids(int* camCount);
	CAMERA_BASE_FEATURE* getSupportedFeatures(int* featuresCount);
	void callSettingsDialog();
protected:
	void getNewFrame(unsigned char* newFrame);
	void cameraInitializationLogic();
	void cameraDeinitializationLogic();
	void setCameraType();
private:
	static DWORD WINAPI SettingsThread(LPVOID instance);
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	void StartSettingsDialog();
	CLEyeCameraInstance ps3EyeCamera;
	HANDLE settingsGUIThread;
	bool isSettedDefaultSettings;
};
#endif // OFXPS3_H_
