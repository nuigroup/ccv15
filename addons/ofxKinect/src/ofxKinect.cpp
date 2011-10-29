/*
*  ofxKinect.cpp
*  
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/

#include "ofxKinect.h"

LRESULT CALLBACK ofxKinect:: WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
   static CREATESTRUCT   *cs;

   static HWND           hNearBoundSlider;
   static HWND			 hFarBoundSlider;
   static HWND			 hModeSelectorSlider;
   static HWND			 hMotorPositionSlider;
   static HWND			 hLEDStatusSlider;

   static HWND			 hNearBoundBuddy;
   static HWND			 hFarBoundBuddy;
   static HWND			 hModeSelectorBuddy;
   static HWND			 hMotorPositionBuddy;
   static HWND			 hLEDStatusBuddy;

   ofxKinect* camera = (ofxKinect*) GetWindowLongPtr(hwnd,GWLP_USERDATA);
	if (camera!=NULL)
	{
		if (!camera->isSettedDefaultSettings)
		{
			camera->isSettedDefaultSettings = true;
			int firstValue, secondValue, minValue, maxValue;
			bool isEnabled,isAuto;
			camera->getCameraFeature(DEPTH_NEAR_BOUND,&firstValue,&secondValue,&isAuto,&isEnabled,&minValue,&maxValue);
			SendMessage(GetDlgItem(hwnd, 0), TBM_SETPOS, TRUE, firstValue);
			camera->getCameraFeature(DEPTH_FAR_BOUND,&firstValue,&secondValue,&isAuto,&isEnabled,&minValue,&maxValue);
			SendMessage(GetDlgItem(hwnd, 1), TBM_SETPOS, TRUE, firstValue);
			SendMessage(GetDlgItem(hwnd, 2), TBM_SETPOS, TRUE, camera->cameraPixelMode);
		//	camera->getCameraFeature(BASE_MOTOR_POSITION,&firstValue,&secondValue,&isAuto,&isEnabled,&minValue,&maxValue);
		//	SendMessage(GetDlgItem(hwnd, 3), TBM_SETPOS, TRUE, firstValue);
		//	camera->getCameraFeature(BASE_MOTOR_LED,&firstValue,&secondValue,&isAuto,&isEnabled,&minValue,&maxValue);
		//	SendMessage(GetDlgItem(hwnd, 4), TBM_SETPOS, TRUE, firstValue);
		}
	}
   switch (iMsg) 
   {
   case WM_CREATE :
      cs = (LPCREATESTRUCT)lParam;
      hNearBoundSlider = CreateWindow (TRACKBAR_CLASS, "",
         WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE | TBS_BOTH | TBS_NOTICKS,
         150, 15, 255, 20,
         hwnd, (HMENU)0, cs->hInstance, NULL);
	  hFarBoundSlider = CreateWindow (TRACKBAR_CLASS, "",
		 WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE | TBS_BOTH | TBS_NOTICKS,         
		 150, 40, 255, 20,
         hwnd, (HMENU)1, cs->hInstance, NULL);
	  hModeSelectorSlider = CreateWindow (TRACKBAR_CLASS, "",
		 WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE | TBS_BOTH | TBS_NOTICKS,         
		 150, 65, 255, 20,
         hwnd, (HMENU)2, cs->hInstance, NULL);
	  /*
	  hMotorPositionSlider = CreateWindow (TRACKBAR_CLASS, "",
		 WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE | TBS_BOTH | TBS_NOTICKS,         
		 130, 90, 255, 20,
         hwnd, (HMENU)3, cs->hInstance, NULL);
	  hLEDStatusSlider = CreateWindow (TRACKBAR_CLASS, "",
		 WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE | TBS_BOTH | TBS_NOTICKS,         
		 130, 115, 255, 20,
         hwnd, (HMENU)4, cs->hInstance, NULL);
	*/

	  hNearBoundBuddy = CreateWindow("STATIC", "Near bound:[0..2047]", SS_LEFT | WS_CHILD | WS_VISIBLE, 
                                    10, 0, 140, 20, hwnd, NULL, cs->hInstance, NULL);
	  hFarBoundBuddy = CreateWindow("STATIC", "Far bound:[0..2047]", SS_LEFT | WS_CHILD | WS_VISIBLE, 
                                    10, 0, 140, 20, hwnd, NULL, cs->hInstance, NULL);
	  hModeSelectorBuddy = CreateWindow("STATIC", "Mode:[0..2]", SS_LEFT | WS_CHILD | WS_VISIBLE, 
                                    10, 0, 140, 20, hwnd, NULL, cs->hInstance, NULL);
	  /*
	  hMotorPositionBuddy = CreateWindow("STATIC", "Motor position:[-8000..8000]", SS_LEFT | WS_CHILD | WS_VISIBLE, 
                                    10, 0, 115, 20, hwnd, NULL, cs->hInstance, NULL);
	  hLEDStatusBuddy = CreateWindow("STATIC", "LED status:[0..7]", SS_LEFT | WS_CHILD | WS_VISIBLE, 
                                    10, 0, 115, 20, hwnd, NULL, cs->hInstance, NULL);
*/
                                
      SendMessage(hNearBoundSlider, TBM_SETBUDDY, (WPARAM)TRUE, (LPARAM)hNearBoundBuddy);
      SendMessage(hFarBoundSlider, TBM_SETBUDDY, (WPARAM)TRUE, (LPARAM)hFarBoundBuddy);
      SendMessage(hModeSelectorSlider, TBM_SETBUDDY, (WPARAM)TRUE, (LPARAM)hModeSelectorBuddy);
  //    SendMessage(hMotorPositionSlider, TBM_SETBUDDY, (WPARAM)TRUE, (LPARAM)hMotorPositionBuddy);
  //    SendMessage(hLEDStatusSlider, TBM_SETBUDDY, (WPARAM)TRUE, (LPARAM)hLEDStatusBuddy);

	  SendMessage(hNearBoundSlider, TBM_SETRANGE,  TRUE,  MAKELONG(0, 0x7FF)); 
	  SendMessage(hFarBoundSlider, TBM_SETRANGE,  TRUE,  MAKELONG(0, 0x7FF)); 
	  SendMessage(hModeSelectorSlider, TBM_SETRANGE,  TRUE,  MAKELONG(0, 2)); 
	//  SendMessage(hFarBoundSlider, TBM_SETRANGE,  TRUE,  MAKELONG(-8000, 8000)); 
	//  SendMessage(hModeSelectorSlider, TBM_SETRANGE,  TRUE,  MAKELONG(0, 7)); 


      break;
   case WM_HSCROLL :
	  switch(LOWORD(wParam))
      {
		 case TB_THUMBTRACK :
		 case SB_THUMBPOSITION:
		 case TB_ENDTRACK:
			 if (camera!=NULL)
			 {
				camera->setCameraFeature(DEPTH_NEAR_BOUND,(int)SendMessage(GetDlgItem(hwnd, 0),TBM_GETPOS, 0, 0),0,false,true);
				camera->setCameraFeature(DEPTH_FAR_BOUND,(int)SendMessage(GetDlgItem(hwnd, 1),TBM_GETPOS, 0, 0),0,false,true);
				camera->cameraPixelMode = (int)SendMessage(GetDlgItem(hwnd, 2),TBM_GETPOS, 0, 0);
	//			camera->setCameraFeature(BASE_MOTOR_POSITION,(int)SendMessage(GetDlgItem(hwnd, 3),TBM_GETPOS, 0, 0),0,false,true);
	//			camera->setCameraFeature(BASE_MOTOR_LED,(int)SendMessage(GetDlgItem(hwnd, 4),TBM_GETPOS, 0, 0),0,false,true);
			 }
			 break; 
      }
      break;
   case WM_CLOSE :
      DestroyWindow(hwnd);
      break;
   case WM_DESTROY :
      PostQuitMessage(0);
      break;
   }
   return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void ofxKinect::StartSettingsDialog()
{
   HWND        hwnd;
   MSG         msg;
   WNDCLASS    wndclass;
   char        szAppName[64] = "Kinect settings: ";
   strcat(szAppName,GUIDToString(guid).c_str());
   wndclass.style         = 0;
   wndclass.lpfnWndProc   = ofxKinect::WndProc;
   wndclass.cbClsExtra    = 0;
   wndclass.cbWndExtra    = 0;
   HMODULE hInstance;
   GetModuleHandleEx(0,NULL,&hInstance);
   wndclass.hInstance     = hInstance;
   wndclass.hIcon         = LoadIcon(hInstance, szAppName);
   wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
   wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
   wndclass.lpszMenuName  = szAppName;
   wndclass.lpszClassName = szAppName;
   RegisterClass(&wndclass);

   InitCommonControls();

   hwnd = CreateWindow(szAppName,
      szAppName,
      DS_SETFONT | DS_MODALFRAME | DS_FIXEDSYS | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
      0, 0, 415, 120,
      NULL, NULL, hInstance, 0);
   SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG_PTR)(this));
   while (GetMessage(&msg, NULL, 0, 0)) 
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }
}

DWORD WINAPI ofxKinect::SettingsThread(LPVOID instance)
{
	ofxKinect *pThis = (ofxKinect*)instance;
	pThis->StartSettingsDialog();
	pThis->settingsGUIThread = NULL;
	pThis->isSettedDefaultSettings = false;
	return 0;
}

ofxKinect::ofxKinect()
{	
	kinectCamera = NULL;
	kinectMotor = NULL;
	kinectMotorLEDValue = 0;
	kinectMotorPositionValue = 0;
	nearBound = 0x0000;
	farBound = 0xFFFF;
	cameraType = KINECT;
	settingsGUIThread = NULL;
	isSettedDefaultSettings = false;
}
ofxKinect::~ofxKinect()
{
	deinitializeCamera();
}

void ofxKinect::callSettingsDialog()
{
	if (settingsGUIThread == NULL)
		settingsGUIThread = CreateThread(NULL, 0, &ofxKinect::SettingsThread, this, 0, 0);

}

CAMERA_BASE_FEATURE* ofxKinect::getSupportedFeatures(int* featuresCount)
{
	*featuresCount = 5;
	CAMERA_BASE_FEATURE* features = (CAMERA_BASE_FEATURE*)malloc(*featuresCount * sizeof(CAMERA_BASE_FEATURE));
	features[0] = BASE_FRAMERATE;
	features[1] = BASE_MOTOR_POSITION;
	features[2] = BASE_MOTOR_LED;
	features[3] = DEPTH_NEAR_BOUND;
	features[4] = DEPTH_FAR_BOUND;
	return features;
}


void ofxKinect::getNewFrame(unsigned char* newFrame)
{
	GetNUICameraDepthFrameRAW(kinectCamera,depthData);
	int size = width*height;
	float len = farBound - nearBound;
	switch (cameraPixelMode)
	{
	case 0:
		for (int i=0;i<size;i++)
			newFrame[i] = ((depthData[i] <= farBound) && (depthData[i] >= nearBound)) ? 255 : 0;
		break;
	case 1:
		for (int i=0;i<size;i++)
		{
			if (depthData[i] <= nearBound)
				newFrame[i] = 0;
			else if (depthData[i] >= farBound)
				newFrame[i] = 255;
			else 
				newFrame[i] = (len == 0) ? 0 : (unsigned char)(255.0f * ((depthData[i] - nearBound)/len));
		}
		break;
	case 2:
		for (int i=0;i<size;i++)
			newFrame[i] = ((depthData[i] <= farBound) && (depthData[i] >= nearBound)) ? (unsigned char)(255.0f * ((depthData[i] - nearBound)/len)) : 0;
	}
}

void ofxKinect::cameraInitializationLogic()
{
	int count = GetNUIDeviceCount();
	int selectedIndex = -1;
	for (int i=0;i<count;i++)
	{
		PCHAR serialDataString = GetNUIDeviceSerial(i);
		long serialDataInt = 0;
		for (int j=0;serialDataString[j]!='\0';j++)
		{
			serialDataInt *= 10;
			serialDataInt += (serialDataString[j] - '0');
		}
		if (serialDataInt == guid.Data1)
		{
			selectedIndex = i;
			break;
		}
	}
	PCHAR serial = GetNUIDeviceSerial(selectedIndex);
	kinectMotor = CreateNUIMotor(serial);
	kinectCamera = CreateNUICamera(serial);
	depthData = new USHORT[width*height];
	StartNUICamera(kinectCamera);
	for (int i=0;i<cameraBaseSettings->propertyType.size();i++)
		setCameraFeature(cameraBaseSettings->propertyType[i],cameraBaseSettings->propertyFirstValue[i],cameraBaseSettings->propertySecondValue[i],cameraBaseSettings->isPropertyAuto[i],cameraBaseSettings->isPropertyOn[i]);
}

void ofxKinect::setCameraType()
{
	cameraType = KINECT;
	cameraTypeName = "KINECT";
}

void ofxKinect::cameraDeinitializationLogic()
{
	if (kinectMotor!=NULL)
	{
		DestroyNUIMotor(kinectMotor);
		kinectMotor = NULL;
	}
	if (kinectCamera!=NULL)
	{
		StopNUICamera(kinectCamera);
		Sleep(100);
		DestroyNUICamera(kinectCamera);
		kinectCamera = NULL;
	}
}

void ofxKinect::setCameraFeature(CAMERA_BASE_FEATURE featureCode,int firstValue,int secondValue,bool isAuto,bool isEnabled)
{
	if (kinectMotor == NULL)
		return;
	switch (featureCode)
	{
	case BASE_FRAMERATE:
		framerate = 30;
		break;
	case BASE_MOTOR_POSITION:
		SetNUIMotorPosition(kinectMotor,(SHORT)firstValue);
		kinectMotorPositionValue = (SHORT)firstValue;
		break;
	case BASE_MOTOR_LED:
		SetNUIMotorLED(kinectMotor,(BYTE)firstValue);
		kinectMotorLEDValue = (BYTE)firstValue;
		break;
	case DEPTH_NEAR_BOUND:
		nearBound = (USHORT)firstValue;
		break;
	case DEPTH_FAR_BOUND:
		farBound = (USHORT)firstValue;
		break;
	}
}

void ofxKinect::getCameraFeature(CAMERA_BASE_FEATURE featureCode,int* firstValue,int* secondValue, bool* isAuto, bool* isEnabled,int* minValue,int* maxValue)
{
	*firstValue = 0;
	*secondValue = 0;
	*isAuto = false;
	*isEnabled = false;
	*minValue = 0;
	*maxValue = 0;
	if (kinectMotor == NULL)
		return;
	switch (featureCode)
	{
	case BASE_FRAMERATE:
		*firstValue = framerate;
		*isEnabled = true;
		*secondValue = 0;
		*isAuto = false;
		*isEnabled = true;
		*minValue = framerate;
		*maxValue = framerate;
	case BASE_MOTOR_POSITION:
		*firstValue = kinectMotorPositionValue;
		*isEnabled = true;
		*secondValue = 0;
		*isAuto = false;
		*isEnabled = true;
		*minValue = -8000;
		*maxValue = 8000;
		break;
	case BASE_MOTOR_LED:
		*firstValue = kinectMotorLEDValue;
		*isEnabled = true;
		*secondValue = 0;
		*isAuto = false;
		*isEnabled = true;
		*minValue = 0;
		*maxValue = 7;
		break;
	case DEPTH_NEAR_BOUND:
		*firstValue = nearBound;
		*isEnabled = true;
		*secondValue = 0;
		*isAuto = false;
		*isEnabled = true;
		*minValue = 0x0000;
		*maxValue = 0x7FF;
		break;
	case DEPTH_FAR_BOUND:
		*firstValue = farBound;
		*isEnabled = true;
		*secondValue = 0;
		*isAuto = false;
		*isEnabled = true;
		*minValue = 0x0000;
		*maxValue = 0x7FF;
		break;
	}
}

int ofxKinect::getCameraBaseCount()
{
   return GetNUIDeviceCount();
}

GUID* ofxKinect::getBaseCameraGuids(int* camCount)
{
	*camCount = GetNUIDeviceCount();
	GUID* guids = (GUID*)malloc(*camCount * sizeof(GUID));
	for (int i=0;i<*camCount;i++)
	{
		GUID guid2;
		PCHAR serialDataString = GetNUIDeviceSerial(i);
		long serialDataInt = 0;
		for (int j=0;serialDataString[j]!='\0';j++)
		{
			serialDataInt *= 10;
			serialDataInt += (serialDataString[j] - '0');
		}
		guid2.Data1 = serialDataInt;
		guid2.Data2 = guid2.Data3 = 0;
		memset(guid2.Data4,0,8*sizeof(unsigned char));
		guids[i] = guid2;
	}
	return guids;
}

