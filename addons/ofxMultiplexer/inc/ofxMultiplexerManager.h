/* ofxMulticamManager.h
*  
*  Created on 20/08/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/
#ifndef OFXMULTIPLEXERMANAGER_H_INCLUDED
#define OFXMULTIPLEXERMANAGER_H_INCLUDED

#include "ofxCameraBase.h"
#include "ofxCameraBaseSettings.h"
#include "ofxffmv.h"
#include "ofxPS3.h"
#include "ofxDShow.h"
#include "ofxKinect.h"
#include "ofxCMUCamera.h"
#include "ofxMultiplexer.h"
#include "ofxXMLSettings.h"
#include <vector>
#include "Filters\Filters.h"
#include "Calibration.h"


class ofxMultiplexerManager
{
public:
	ofxMultiplexerManager();
	~ofxMultiplexerManager();
	//multiplexer interaction logic
	void startMulticamManager();
	void startCalibration();
	void updateCalibrationStatus();
	void stopCalibration();
	void cancelCalibration();
	void resumeCaptureFromAllCameras();
	void pauseAllUnusedCameras();
	void applySettingsToMultiplexer();
	//setters
	void setInterleaveMode(bool isInterleaveMode);
	void setMultiplexer(ofxMultiplexer* multiplexer);
	void setCalibrator(Calibration* calibrator);
	void setProcessFilter(Filters* processFilter);
	void setMultiplexerSize(int width,int height);
	void setCameraGridSize(int width,int height);
	void setCalibratonGridSize(int width,int height);
	void addAllowdedCameraType(CAMERATYPE cameraType);
	void addCameraBaseToMultiplexerAtPosition(int position,ofxCameraBase* cameraBase);
	void removeCameraBaseFromMultiplexerAtPosition(int position);
	void removeAllCameraBasesFromMultiplexer();
	//getters
	ofxCameraBase* getCameraBase(int index);
	ofxCameraBase* getCameraBase(CAMERATYPE cameraType,GUID guid);
	ofxCameraBase* getCameraBaseAtPosition(int position);
	int getIndexOfCameraBase(ofxCameraBase* cameraBase);
	int getPositionOfCameraBase(ofxCameraBase* cameraBase);
	void getCameraGridSize(int* width,int* height);
	void getCalibrationGridSize(int* width,int* height);
	int getCameraBaseCount();
	bool getInterleaveMode();
	//XML settings logic
	void readSettingsFromXML(char* fileName="xml/multiplexer_settings.xml");
	void saveSettingsToXML(char* fileName="xml/multiplexer_settings.xml");
	void initializeCalibration();
private:
	void enumerateCameras();
private:
	ofxMultiplexer* cameraMultiplexer;
	Calibration* calibration;
	Filters* filters;
	int stitchedFrameWidth,stitchedFrameHeight;
	int cameraGridWidth,cameraGridHeight;
	int calibrationGridWidth,calibrationGridHeight;
	bool interleaveMode,needToUpdateBackground;
	std::vector<ofxCameraBase* > cameraBases;
	std::vector<ofxCameraBaseCalibration* > cameraBasesCalibration;
	std::vector<CAMERATYPE> allowdedCameraTypes;
	bool isMultiplexerNeedToUpdate;
};

#endif //OFXMULTIPLEXERMANAGER_H_INCLUDED
