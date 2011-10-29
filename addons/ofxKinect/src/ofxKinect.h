/* ofxKinect.h
*  
*  Created on 31/05/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/
#ifndef OFXKINECT_H_INCLUDED
#define OFXKINECT_H_INCLUDED

#include "ofxCameraBase.h"
#include "CLNUIDevice.h"

class ofxKinect : ofxCameraBase
{
public:
    ofxKinect();
	~ofxKinect();
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
    CLNUICamera kinectCamera;
	CLNUIMotor kinectMotor;
	BYTE kinectMotorLEDValue;
	SHORT kinectMotorPositionValue;
	USHORT nearBound,farBound;
	PUSHORT depthData;
	HANDLE settingsGUIThread;
	bool isSettedDefaultSettings;
};
#endif // OFXKINECT_H_INCLUDED
