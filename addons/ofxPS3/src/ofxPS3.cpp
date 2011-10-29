/*
*  ofxPS3.cpp
*  
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/
#include "ofxPS3.h"

LRESULT CALLBACK ofxPS3::WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
   static CREATESTRUCT   *cs;
   static HWND           hExposureWndSlider, hGainWndSlider, hHFlipSlider, hVFlipSlider, hHKeystoneSlider, hVKeystoneSlider, hXOffsetSlider,
	   hYOffsetSlider, hRotationSlider, hZoomSlider, hLensCorrection1Slider, hLensCorrection2Slider, hLensCorrection3Slider, hLensBrightnessSlider,
	   hExposureAutoCheckbox,hGainAutoCheckbox;
   static HWND			 hExposureWndBuddy, hGainWndBuddy, hHFlipBuddy, hVFlipBuddy, hHKeystoneBuddy, hVKeystoneBuddy, hXOffsetBuddy,
	   hYOffsetBuddy, hRotationBuddy, hZoomBuddy, hLensCorrection1Buddy, hLensCorrection2Buddy, hLensCorrection3Buddy, hLensBrightnessBuddy;
   ofxPS3* camera = (ofxPS3*) GetWindowLongPtr(hwnd,GWLP_USERDATA);
	if (camera!=NULL)
	{
		if (!camera->isSettedDefaultSettings)
		{
			camera->isSettedDefaultSettings = true;
			int firstValue, secondValue, minValue, maxValue;
			bool isEnabled,isAuto;
			camera->getCameraFeature(BASE_GAIN,&firstValue,&secondValue,&isAuto,&isEnabled,&minValue,&maxValue);
			SendMessage(GetDlgItem(hwnd, 1), TBM_SETPOS, TRUE, firstValue);
			camera->getCameraFeature(BASE_EXPOSURE,&firstValue,&secondValue,&isAuto,&isEnabled,&minValue,&maxValue);
			SendMessage(GetDlgItem(hwnd, 0), TBM_SETPOS, TRUE, firstValue);
			SendMessage(GetDlgItem(hwnd, 14), BM_SETCHECK , CLEyeGetCameraParameter(camera->ps3EyeCamera,CLEYE_AUTO_EXPOSURE), 0 );
			SendMessage(GetDlgItem(hwnd, 15), BM_SETCHECK , CLEyeGetCameraParameter(camera->ps3EyeCamera,CLEYE_AUTO_GAIN), 0  );
		   

			if (camera->cameraPixelMode)
			{
				camera->getCameraFeature(BASE_HFLIP,&firstValue,&secondValue,&isAuto,&isEnabled,&minValue,&maxValue);
				SendMessage(GetDlgItem(hwnd, 2), TBM_SETPOS, TRUE, firstValue);
				camera->getCameraFeature(BASE_VFLIP,&firstValue,&secondValue,&isAuto,&isEnabled,&minValue,&maxValue);
				SendMessage(GetDlgItem(hwnd, 3), TBM_SETPOS, TRUE, firstValue);
				camera->getCameraFeature(BASE_HKEYSTONE,&firstValue,&secondValue,&isAuto,&isEnabled,&minValue,&maxValue);
				SendMessage(GetDlgItem(hwnd, 4), TBM_SETPOS, TRUE, firstValue);
				camera->getCameraFeature(BASE_VKEYSTONE,&firstValue,&secondValue,&isAuto,&isEnabled,&minValue,&maxValue);
				SendMessage(GetDlgItem(hwnd, 5), TBM_SETPOS, TRUE, firstValue);
				camera->getCameraFeature(BASE_XOFFSET,&firstValue,&secondValue,&isAuto,&isEnabled,&minValue,&maxValue);
				SendMessage(GetDlgItem(hwnd, 6), TBM_SETPOS, TRUE, firstValue);
				camera->getCameraFeature(BASE_YOFFSET,&firstValue,&secondValue,&isAuto,&isEnabled,&minValue,&maxValue);
				SendMessage(GetDlgItem(hwnd, 7), TBM_SETPOS, TRUE, firstValue);
				camera->getCameraFeature(BASE_ROTATION,&firstValue,&secondValue,&isAuto,&isEnabled,&minValue,&maxValue);
				SendMessage(GetDlgItem(hwnd, 8), TBM_SETPOS, TRUE, firstValue);
				camera->getCameraFeature(BASE_ZOOM,&firstValue,&secondValue,&isAuto,&isEnabled,&minValue,&maxValue);
				SendMessage(GetDlgItem(hwnd, 9), TBM_SETPOS, TRUE, firstValue);
				camera->getCameraFeature(BASE_LENSCORRECTION1,&firstValue,&secondValue,&isAuto,&isEnabled,&minValue,&maxValue);
				SendMessage(GetDlgItem(hwnd, 10), TBM_SETPOS, TRUE, firstValue);
				camera->getCameraFeature(BASE_LENSCORRECTION2,&firstValue,&secondValue,&isAuto,&isEnabled,&minValue,&maxValue);
				SendMessage(GetDlgItem(hwnd, 11), TBM_SETPOS, TRUE, firstValue);
				camera->getCameraFeature(BASE_LENSCORRECTION3,&firstValue,&secondValue,&isAuto,&isEnabled,&minValue,&maxValue);
				SendMessage(GetDlgItem(hwnd, 12), TBM_SETPOS, TRUE, firstValue);
				camera->getCameraFeature(BASE_LENSBRIGHTNESS,&firstValue,&secondValue,&isAuto,&isEnabled,&minValue,&maxValue);
			}
		}
	}
   switch (iMsg) 
   {
   case WM_CREATE :
      cs = (LPCREATESTRUCT)lParam;
      hExposureWndSlider = CreateWindow (TRACKBAR_CLASS, "",
         WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE | TBS_BOTH | TBS_NOTICKS,
         220, 35, 225, 20,
         hwnd, (HMENU)0, cs->hInstance, NULL);
	  hGainWndSlider = CreateWindow (TRACKBAR_CLASS, "",
		 WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE | TBS_BOTH | TBS_NOTICKS,         
		 220, 60, 225, 20,
         hwnd, (HMENU)1, cs->hInstance, NULL);
	  hHFlipSlider = CreateWindow (TRACKBAR_CLASS, "",
		 WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE | TBS_BOTH | TBS_NOTICKS,         
		 220, 85, 225, 20,
         hwnd, (HMENU)2, cs->hInstance, NULL);
	  hVFlipSlider = CreateWindow (TRACKBAR_CLASS, "",
		 WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE | TBS_BOTH | TBS_NOTICKS,         
		 220, 110, 225, 20,
         hwnd, (HMENU)3, cs->hInstance, NULL);
	  hHKeystoneSlider = CreateWindow (TRACKBAR_CLASS, "",
		 WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE | TBS_BOTH | TBS_NOTICKS,         
		 220, 135, 225, 20,
         hwnd, (HMENU)4, cs->hInstance, NULL);
	  hVKeystoneSlider = CreateWindow (TRACKBAR_CLASS, "",
		 WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE | TBS_BOTH | TBS_NOTICKS,         
		 220, 160, 225, 20,
         hwnd, (HMENU)5, cs->hInstance, NULL);
	  hXOffsetSlider = CreateWindow (TRACKBAR_CLASS, "",
		 WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE | TBS_BOTH | TBS_NOTICKS,         
		 220, 185, 225, 20,
         hwnd, (HMENU)6, cs->hInstance, NULL);
	  hYOffsetSlider = CreateWindow (TRACKBAR_CLASS, "",
		 WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE | TBS_BOTH | TBS_NOTICKS,         
		 220, 210, 225, 20,
         hwnd, (HMENU)7, cs->hInstance, NULL);
	  hRotationSlider = CreateWindow (TRACKBAR_CLASS, "",
		 WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE | TBS_BOTH | TBS_NOTICKS,         
		 220, 235, 225, 20,
         hwnd, (HMENU)8, cs->hInstance, NULL);
	  hZoomSlider = CreateWindow (TRACKBAR_CLASS, "",
		 WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE | TBS_BOTH | TBS_NOTICKS,         
		 220, 260, 225, 20,
         hwnd, (HMENU)9, cs->hInstance, NULL);
	  hLensCorrection1Slider = CreateWindow (TRACKBAR_CLASS, "",
		 WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE | TBS_BOTH | TBS_NOTICKS,         
		 220, 285, 225, 20,
         hwnd, (HMENU)10, cs->hInstance, NULL);
	  hLensCorrection2Slider = CreateWindow (TRACKBAR_CLASS, "",
		 WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE | TBS_BOTH | TBS_NOTICKS,         
		 220, 310, 225, 20,
         hwnd, (HMENU)11, cs->hInstance, NULL);
	  hLensCorrection3Slider = CreateWindow (TRACKBAR_CLASS, "",
		 WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE | TBS_BOTH | TBS_NOTICKS,         
		 220, 335, 225, 20,
         hwnd, (HMENU)12, cs->hInstance, NULL);
	  hLensBrightnessSlider = CreateWindow (TRACKBAR_CLASS, "",
		 WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE | TBS_BOTH | TBS_NOTICKS,         
		 220, 360, 225, 20,
         hwnd, (HMENU)13, cs->hInstance, NULL);
	  hGainAutoCheckbox = CreateWindowEx (0, "Button", "Auto Gain",
                          WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
                          150, 5, 130, 25, hwnd, (HMENU)15, cs->hInstance, NULL);
	  hExposureAutoCheckbox = CreateWindowEx (0, "Button", "Auto Exposure",
                          WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
                          2, 5, 130, 25, hwnd, (HMENU)14, cs->hInstance, NULL);
	  hExposureWndBuddy = CreateWindow("STATIC", "Exposure:[0..255]", SS_LEFT | WS_CHILD | WS_VISIBLE, 
                                    10, 0, 215, 20, hwnd, NULL, cs->hInstance, NULL);
	  hGainWndBuddy = CreateWindow("STATIC", "Gain:[0..79]", SS_LEFT | WS_CHILD | WS_VISIBLE, 
                                    10, 0, 215, 20, hwnd, NULL, cs->hInstance, NULL);
	  hHFlipBuddy = CreateWindow("STATIC", "Horizontal flip:[0..1]", SS_LEFT | WS_CHILD | WS_VISIBLE, 
                                    10, 0, 215, 20, hwnd, NULL, cs->hInstance, NULL);
	  hVFlipBuddy = CreateWindow("STATIC", "Vertical flip:[0..1]", SS_LEFT | WS_CHILD | WS_VISIBLE, 
                                    10, 0, 215, 20, hwnd, NULL, cs->hInstance, NULL);
	  hHKeystoneBuddy = CreateWindow("STATIC", "Horizontal keystone:[-500..500]", SS_LEFT | WS_CHILD | WS_VISIBLE, 
                                    10, 0, 215, 20, hwnd, NULL, cs->hInstance, NULL);
	  hVKeystoneBuddy = CreateWindow("STATIC", "Vertical keystone:[-500..500]", SS_LEFT | WS_CHILD | WS_VISIBLE, 
                                    10, 0, 215, 20, hwnd, NULL, cs->hInstance, NULL);
	  hXOffsetBuddy = CreateWindow("STATIC", "Horizontal offset:[-500..500]", SS_LEFT | WS_CHILD | WS_VISIBLE, 
                                    10, 0, 215, 20, hwnd, NULL, cs->hInstance, NULL);
	  hYOffsetBuddy = CreateWindow("STATIC", "Vertical offset:[-500..500]", SS_LEFT | WS_CHILD | WS_VISIBLE, 
                                    10, 0, 215, 20, hwnd, NULL, cs->hInstance, NULL);
	  hRotationBuddy = CreateWindow("STATIC", "Rotation:[-500..500]", SS_LEFT | WS_CHILD | WS_VISIBLE, 
                                    10, 0, 215, 20, hwnd, NULL, cs->hInstance, NULL);
	  hZoomBuddy = CreateWindow("STATIC", "Zoom:[-500..500]", SS_LEFT | WS_CHILD | WS_VISIBLE, 
                                    10, 0, 215, 20, hwnd, NULL, cs->hInstance, NULL);
	  hLensCorrection1Buddy = CreateWindow("STATIC", "Lens correction 1:[-500..500]", SS_LEFT | WS_CHILD | WS_VISIBLE, 
                                    10, 0, 215, 20, hwnd, NULL, cs->hInstance, NULL);
	  hLensCorrection2Buddy = CreateWindow("STATIC", "Lens correction 2:[-500..500]", SS_LEFT | WS_CHILD | WS_VISIBLE, 
                                    10, 0, 215, 20, hwnd, NULL, cs->hInstance, NULL);
	  hLensCorrection3Buddy = CreateWindow("STATIC", "Lens correction 3:[-500..500]", SS_LEFT | WS_CHILD | WS_VISIBLE, 
                                    10, 0, 215, 20, hwnd, NULL, cs->hInstance, NULL);
	  hLensBrightnessBuddy = CreateWindow("STATIC", "Lens brightness:[-500..500]", SS_LEFT | WS_CHILD | WS_VISIBLE, 
                                    10, 0, 215, 20, hwnd, NULL, cs->hInstance, NULL);   

      SendMessage(hExposureWndSlider, TBM_SETBUDDY, (WPARAM)TRUE, (LPARAM)hExposureWndBuddy);
      SendMessage(hGainWndSlider, TBM_SETBUDDY, (WPARAM)TRUE, (LPARAM)hGainWndBuddy);
      SendMessage(hHFlipSlider, TBM_SETBUDDY, (WPARAM)TRUE, (LPARAM)hHFlipBuddy);
      SendMessage(hVFlipSlider, TBM_SETBUDDY, (WPARAM)TRUE, (LPARAM)hVFlipBuddy);
      SendMessage(hHKeystoneSlider, TBM_SETBUDDY, (WPARAM)TRUE, (LPARAM)hHKeystoneBuddy);
      SendMessage(hVKeystoneSlider, TBM_SETBUDDY, (WPARAM)TRUE, (LPARAM)hVKeystoneBuddy);
      SendMessage(hXOffsetSlider, TBM_SETBUDDY, (WPARAM)TRUE, (LPARAM)hXOffsetBuddy);
      SendMessage(hYOffsetSlider, TBM_SETBUDDY, (WPARAM)TRUE, (LPARAM)hYOffsetBuddy);
      SendMessage(hRotationSlider, TBM_SETBUDDY, (WPARAM)TRUE, (LPARAM)hRotationBuddy);
      SendMessage(hZoomSlider, TBM_SETBUDDY, (WPARAM)TRUE, (LPARAM)hZoomBuddy);
      SendMessage(hLensCorrection1Slider, TBM_SETBUDDY, (WPARAM)TRUE, (LPARAM)hLensCorrection1Buddy);
      SendMessage(hLensCorrection2Slider, TBM_SETBUDDY, (WPARAM)TRUE, (LPARAM)hLensCorrection2Buddy);
      SendMessage(hLensCorrection3Slider, TBM_SETBUDDY, (WPARAM)TRUE, (LPARAM)hLensCorrection3Buddy);
      SendMessage(hLensBrightnessSlider, TBM_SETBUDDY, (WPARAM)TRUE, (LPARAM)hLensBrightnessBuddy);

	  SendMessage(hExposureWndSlider, TBM_SETRANGE,  TRUE,  MAKELONG(0, 255)); 
	  SendMessage(hGainWndSlider, TBM_SETRANGE,  TRUE,  MAKELONG(0, 79)); 
	  SendMessage(hHFlipSlider, TBM_SETRANGE,  TRUE,  MAKELONG(0, 1)); 
	  SendMessage(hVFlipSlider, TBM_SETRANGE,  TRUE,  MAKELONG(0, 1)); 
	  SendMessage(hHKeystoneSlider, TBM_SETRANGE,  TRUE,  MAKELONG(-500, 500)); 
	  SendMessage(hVKeystoneSlider, TBM_SETRANGE,  TRUE,  MAKELONG(-500, 500)); 
	  SendMessage(hXOffsetSlider, TBM_SETRANGE,  TRUE,   MAKELONG(-500, 500)); 
	  SendMessage(hYOffsetSlider, TBM_SETRANGE,  TRUE,  MAKELONG(-500, 500)); 
	  SendMessage(hRotationSlider, TBM_SETRANGE,  TRUE,   MAKELONG(-500, 500)); 
	  SendMessage(hZoomSlider, TBM_SETRANGE,  TRUE,   MAKELONG(-500, 500)); 
	  SendMessage(hLensCorrection1Slider, TBM_SETRANGE,  TRUE,   MAKELONG(-500, 500)); 
	  SendMessage(hLensCorrection2Slider, TBM_SETRANGE,  TRUE,  MAKELONG(-500, 500)); 
	  SendMessage(hLensCorrection3Slider, TBM_SETRANGE,  TRUE,   MAKELONG(-500, 500)); 
	  SendMessage(hLensBrightnessSlider, TBM_SETRANGE,  TRUE,  MAKELONG(-500, 500)); 
      break;
   case WM_COMMAND :
	   switch(LOWORD(wParam))
	   {
		case 14:
			CLEyeSetCameraParameter(camera->ps3EyeCamera,CLEYE_AUTO_EXPOSURE,(int)SendMessage( GetDlgItem(hwnd, 14), BM_GETCHECK, 0, 0 ));
    		break;
		case 15:
			CLEyeSetCameraParameter(camera->ps3EyeCamera,CLEYE_AUTO_GAIN,(int)SendMessage( GetDlgItem(hwnd, 15), BM_GETCHECK, 0, 0 ));
			break;
	   }
	   break;
   case WM_HSCROLL :
	  switch(LOWORD(wParam))
      {
		 case TB_THUMBTRACK :
		 case SB_THUMBPOSITION:
		 case TB_ENDTRACK:
			 if (camera!=NULL)
			 {
			    if (CLEyeGetCameraParameter(camera->ps3EyeCamera,CLEYE_AUTO_EXPOSURE) == 0)
					CLEyeSetCameraParameter(camera->ps3EyeCamera,CLEYE_EXPOSURE,(int)SendMessage(GetDlgItem(hwnd, 0), TBM_GETPOS, 0, 0));
			    if (CLEyeGetCameraParameter(camera->ps3EyeCamera,CLEYE_AUTO_GAIN) == 0)
					CLEyeSetCameraParameter(camera->ps3EyeCamera,CLEYE_GAIN,(int)SendMessage(GetDlgItem(hwnd, 1), TBM_GETPOS, 0, 0));
				if (camera->cameraPixelMode)
				{
					CLEyeSetCameraParameter(camera->ps3EyeCamera,CLEYE_HFLIP,(int)SendMessage(GetDlgItem(hwnd, 2), TBM_GETPOS, 0, 0));
					CLEyeSetCameraParameter(camera->ps3EyeCamera,CLEYE_VFLIP,(int)SendMessage(GetDlgItem(hwnd, 3), TBM_GETPOS, 0, 0));
					CLEyeSetCameraParameter(camera->ps3EyeCamera,CLEYE_HKEYSTONE,(int)SendMessage(GetDlgItem(hwnd, 4), TBM_GETPOS, 0, 0));
					CLEyeSetCameraParameter(camera->ps3EyeCamera,CLEYE_VKEYSTONE,(int)SendMessage(GetDlgItem(hwnd, 5), TBM_GETPOS, 0, 0));
					CLEyeSetCameraParameter(camera->ps3EyeCamera,CLEYE_XOFFSET,(int)SendMessage(GetDlgItem(hwnd, 6), TBM_GETPOS, 0, 0));
					CLEyeSetCameraParameter(camera->ps3EyeCamera,CLEYE_YOFFSET,(int)SendMessage(GetDlgItem(hwnd, 7), TBM_GETPOS, 0, 0));
					CLEyeSetCameraParameter(camera->ps3EyeCamera,CLEYE_ROTATION,(int)SendMessage(GetDlgItem(hwnd, 8), TBM_GETPOS, 0, 0));
					CLEyeSetCameraParameter(camera->ps3EyeCamera,CLEYE_ZOOM,(int)SendMessage(GetDlgItem(hwnd, 9), TBM_GETPOS, 0, 0));
					CLEyeSetCameraParameter(camera->ps3EyeCamera,CLEYE_LENSCORRECTION1,(int)SendMessage(GetDlgItem(hwnd, 10), TBM_GETPOS, 0, 0));
					CLEyeSetCameraParameter(camera->ps3EyeCamera,CLEYE_LENSCORRECTION2,(int)SendMessage(GetDlgItem(hwnd, 11), TBM_GETPOS, 0, 0));
					CLEyeSetCameraParameter(camera->ps3EyeCamera,CLEYE_LENSCORRECTION3,(int)SendMessage(GetDlgItem(hwnd, 12), TBM_GETPOS, 0, 0));
					CLEyeSetCameraParameter(camera->ps3EyeCamera,CLEYE_LENSBRIGHTNESS,(int)SendMessage(GetDlgItem(hwnd, 13), TBM_GETPOS, 0, 0));
				}
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

void ofxPS3::StartSettingsDialog()
{
   HWND        hwnd;
   MSG         msg;
   WNDCLASS    wndclass;
   char        szAppName[64] = "PS3 Eye settings: ";
   strcat(szAppName,GUIDToString(guid).c_str());
   wndclass.style         = 0;
   wndclass.lpfnWndProc   = ofxPS3::WndProc;
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
	  0, 0, 465, cameraPixelMode ? 410 : 110,
      NULL, NULL, hInstance, 0);
   SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG_PTR)(this));
   while (GetMessage(&msg, NULL, 0, 0)) 
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }
}

DWORD WINAPI ofxPS3::SettingsThread(LPVOID instance)
{
	ofxPS3 *pThis = (ofxPS3*)instance;
	pThis->StartSettingsDialog();
	pThis->settingsGUIThread = NULL;
	pThis->isSettedDefaultSettings = false;
	return 0;
}

ofxPS3::ofxPS3() 
{
	cameraFrame = 0;
	ps3EyeCamera = NULL;
	settingsGUIThread = NULL;
	isSettedDefaultSettings = false;
}

ofxPS3::~ofxPS3() 
{
	deinitializeCamera();
}

void ofxPS3::callSettingsDialog()
{
	if (settingsGUIThread == NULL)
		settingsGUIThread = CreateThread(NULL, 0, &ofxPS3::SettingsThread, this, 0, 0);
}


void ofxPS3::getNewFrame(unsigned char* newFrame)
{
	CLEyeCameraGetFrame(ps3EyeCamera,(PBYTE)newFrame, 1000);
}

CAMERA_BASE_FEATURE* ofxPS3::getSupportedFeatures(int* featuresCount)
{
	CAMERA_BASE_FEATURE* features = NULL;
	if (cameraPixelMode)
	{
		*featuresCount = 15;
		features = (CAMERA_BASE_FEATURE*)malloc(*featuresCount * sizeof(CAMERA_BASE_FEATURE));
		features[0] = BASE_GAIN;
		features[1] = BASE_EXPOSURE;
		features[2] = BASE_FRAMERATE;
		features[3] = BASE_HFLIP;				
		features[4] = BASE_VFLIP;				
		features[5] = BASE_HKEYSTONE;			
		features[6] = BASE_VKEYSTONE;			
		features[7] = BASE_XOFFSET;				
		features[8] = BASE_YOFFSET;				
		features[9] = BASE_ROTATION;				
		features[10] = BASE_ZOOM;					
		features[11] = BASE_LENSCORRECTION1;		
		features[12] = BASE_LENSCORRECTION2;		
		features[13] = BASE_LENSCORRECTION3;		
		features[14] = BASE_LENSBRIGHTNESS;		
	}
	else
	{
		*featuresCount = 3;
		features = (CAMERA_BASE_FEATURE*)malloc(*featuresCount * sizeof(CAMERA_BASE_FEATURE));
		features[0] = BASE_GAIN;
		features[1] = BASE_EXPOSURE;
		features[2] = BASE_FRAMERATE;
	}
	return features;
}

void ofxPS3::cameraInitializationLogic()
{
	cameraType = PS3;
	int camNum = CLEyeGetCameraCount();
	for (int i=0;i<camNum;i++)
	{
		if (CLEyeGetCameraUUID(i) == guid)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
		return;
	ps3EyeCamera = CLEyeCreateCamera(guid, (depth == 1) ? (cameraPixelMode ? CLEYE_MONO_PROCESSED : CLEYE_MONO_RAW) :  (cameraPixelMode ? CLEYE_COLOR_PROCESSED : CLEYE_COLOR_RAW), width * height > 320 * 240 ? CLEYE_VGA : CLEYE_QVGA, framerate);
	Sleep(500);
	for (int i=0;i<cameraBaseSettings->propertyType.size();i++)
		setCameraFeature(cameraBaseSettings->propertyType[i],cameraBaseSettings->propertyFirstValue[i],cameraBaseSettings->propertySecondValue[i],cameraBaseSettings->isPropertyAuto[i],cameraBaseSettings->isPropertyOn[i]);
	CLEyeCameraStart(ps3EyeCamera);
}

void ofxPS3::setCameraType()
{
	cameraType = PS3;
	cameraTypeName = "PS3";
}

void ofxPS3::cameraDeinitializationLogic()
{
	CLEyeCameraStop(ps3EyeCamera);
	Sleep(50);
	CLEyeDestroyCamera(ps3EyeCamera);
	ps3EyeCamera = NULL;
}

void ofxPS3::setCameraFeature(CAMERA_BASE_FEATURE featureCode,int firstValue,int secondValue,bool isAuto,bool isEnabled)
{
	CLEyeCameraParameter deviceProperty = (CLEyeCameraParameter)0xFFFFFFFF;
	switch (featureCode)
	{
		case BASE_GAIN:
			CLEyeSetCameraParameter(ps3EyeCamera,CLEYE_AUTO_GAIN,isAuto);
			deviceProperty = CLEYE_GAIN;
			break;
		case BASE_EXPOSURE:
			CLEyeSetCameraParameter(ps3EyeCamera,CLEYE_AUTO_EXPOSURE,isAuto);
			deviceProperty = CLEYE_EXPOSURE;
			break;
		case BASE_FRAMERATE:
			framerate = firstValue;
			break;
		case BASE_HFLIP:
			if (cameraPixelMode)
				deviceProperty = CLEYE_HFLIP;
			break;
		case BASE_VFLIP:
			if (cameraPixelMode)
				deviceProperty = CLEYE_VFLIP;
			break;
		case BASE_HKEYSTONE:
			if (cameraPixelMode)
				deviceProperty = CLEYE_HKEYSTONE;
			break;
		case BASE_VKEYSTONE:
			if (cameraPixelMode)
				deviceProperty = CLEYE_VKEYSTONE;
			break;
		case BASE_XOFFSET:
			if (cameraPixelMode)
				deviceProperty = CLEYE_XOFFSET;
			break;
		case BASE_YOFFSET:
			if (cameraPixelMode)
				deviceProperty = CLEYE_YOFFSET;
			break;
		case BASE_ROTATION:
			if (cameraPixelMode)
				deviceProperty = CLEYE_ROTATION;
			break;
		case BASE_ZOOM:
			if (cameraPixelMode)
				deviceProperty = CLEYE_ZOOM;
			break;
		case BASE_LENSCORRECTION1:
			if (cameraPixelMode)
				deviceProperty = CLEYE_LENSCORRECTION1;
			break;
		case BASE_LENSCORRECTION2:
			if (cameraPixelMode)
				deviceProperty = CLEYE_LENSCORRECTION2;
			break;
		case BASE_LENSCORRECTION3:
			if (cameraPixelMode)
				deviceProperty = CLEYE_LENSCORRECTION3;
			break;
		case BASE_LENSBRIGHTNESS:
			if (cameraPixelMode)
				deviceProperty = CLEYE_LENSBRIGHTNESS;
			break;
	}
	if ((deviceProperty!=0xFFFFFFFF) && (ps3EyeCamera != NULL))
	{
		CLEyeSetCameraParameter(ps3EyeCamera,deviceProperty,firstValue);
	}
}

void ofxPS3::getCameraFeature(CAMERA_BASE_FEATURE featureCode,int* firstValue,int* secondValue, bool* isAuto, bool* isEnabled,int* minValue,int* maxValue)
{
	CLEyeCameraParameter deviceProperty = (CLEyeCameraParameter)0xFFFFFFFF;
	switch (featureCode)
	{
	case BASE_GAIN:
		deviceProperty = CLEYE_GAIN;
		*minValue = 0;
		*maxValue = 79;
		*isAuto = (ps3EyeCamera!= NULL) ? CLEyeGetCameraParameter(ps3EyeCamera,CLEYE_AUTO_GAIN) : false;
		break;
	case BASE_EXPOSURE:
		deviceProperty = CLEYE_EXPOSURE;
		*minValue = 0;
		*maxValue = 511;
		*isAuto = (ps3EyeCamera!= NULL) ? CLEyeGetCameraParameter(ps3EyeCamera,CLEYE_AUTO_EXPOSURE) : false;
		break;
	case BASE_FRAMERATE:
		*minValue = 15;
		*maxValue = 120;
		*isEnabled = true;
		*firstValue = framerate;
		*secondValue = 0;
		*isAuto = false;
		return;
	case BASE_HFLIP:
		if (cameraPixelMode)
			deviceProperty = CLEYE_HFLIP;
		*minValue = 0;
		*maxValue = 1;
		*isAuto = false;
		break;
	case BASE_VFLIP:
		if (cameraPixelMode)
			deviceProperty = CLEYE_VFLIP;
		*minValue = 0;
		*maxValue = 1;
		*isAuto = false;
		break;
	case BASE_HKEYSTONE:
		if (cameraPixelMode)
			deviceProperty = CLEYE_HKEYSTONE;
		*minValue = -500;
		*maxValue = 500;
		*isAuto = false;
		break;
	case BASE_VKEYSTONE:
		if (cameraPixelMode)
			deviceProperty = CLEYE_VKEYSTONE;
		*minValue = -500;
		*maxValue = 500;
		*isAuto = false;
		break;
	case BASE_XOFFSET:
		if (cameraPixelMode)
			deviceProperty = CLEYE_XOFFSET;
		*minValue = -500;
		*maxValue = 500;
		*isAuto = false;
		break;
	case BASE_YOFFSET:
		if (cameraPixelMode)
			deviceProperty = CLEYE_YOFFSET;
		*minValue = -500;
		*maxValue = 500;
		*isAuto = false;
		break;
	case BASE_ROTATION:
		if (cameraPixelMode)
			deviceProperty = CLEYE_ROTATION;
		*minValue = -500;
		*maxValue = 500;
		*isAuto = false;
		break;
	case BASE_ZOOM:
		if (cameraPixelMode)
			deviceProperty = CLEYE_ZOOM;
		*minValue = -500;
		*maxValue = 500;
		*isAuto = false;
		break;
	case BASE_LENSCORRECTION1:
		if (cameraPixelMode)
			deviceProperty = CLEYE_LENSCORRECTION1;
		*minValue = -500;
		*maxValue = 500;
		*isAuto = false;
		break;
	case BASE_LENSCORRECTION2:
		if (cameraPixelMode)
			deviceProperty = CLEYE_LENSCORRECTION2;
		*minValue = -500;
		*maxValue = 500;
		*isAuto = false;
		break;
	case BASE_LENSCORRECTION3:
		if (cameraPixelMode)
			deviceProperty = CLEYE_LENSCORRECTION3;
		*minValue = -500;
		*maxValue = 500;
		*isAuto = false;
		break;
	case BASE_LENSBRIGHTNESS:
		if (cameraPixelMode)
			deviceProperty = CLEYE_LENSBRIGHTNESS;
		*minValue = -500;
		*maxValue = 500;
		*isAuto = false;
		break;
	}
	if ((deviceProperty!=0xFFFFFFFF) && (ps3EyeCamera!=NULL))
	{
		int parm = CLEyeGetCameraParameter(ps3EyeCamera,deviceProperty);
		*isEnabled = true;
		*firstValue = parm;
		*secondValue = 0;
	}	
	else
	{
		*isEnabled = false;
	}	
}

int ofxPS3::getCameraBaseCount()
{
	return CLEyeGetCameraCount();
}

GUID* ofxPS3::getBaseCameraGuids(int* camCount)
{
	*camCount = CLEyeGetCameraCount();
	GUID* guids = (GUID*)malloc(*camCount * sizeof(GUID));
	for (int i=0;i<*camCount;i++)
	{
		guids[i] = CLEyeGetCameraUUID(i);
	}
	return guids;
}


