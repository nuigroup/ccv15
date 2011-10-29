/*
*  ofxMulticamManager.cpp
*  
*
*  Created on 20/08/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/

#include "ofxMultiplexerManager.h"

ofxMultiplexerManager::ofxMultiplexerManager()
{
	cameraMultiplexer = NULL;
	calibration = NULL;
	filters = NULL;
	stitchedFrameWidth = 640;
	stitchedFrameHeight = 480;
	interleaveMode = false;
	isMultiplexerNeedToUpdate = true;
}
ofxMultiplexerManager::~ofxMultiplexerManager()
{
	saveSettingsToXML();
	for (int i=0;i<cameraBases.size();i++)
		delete cameraBases[i];
}
void ofxMultiplexerManager::getCameraGridSize(int* width,int* height)
{
	*width = cameraGridWidth;
	*height = cameraGridHeight;
}

void ofxMultiplexerManager::getCalibrationGridSize(int* width,int* height)
{
	*width = calibrationGridWidth;
	*height = calibrationGridHeight;
}

void ofxMultiplexerManager::setCameraGridSize(int width,int height)
{
	if (cameraGridWidth != width)
		isMultiplexerNeedToUpdate = true;
	cameraGridWidth = width;
	if (cameraGridHeight != height)
		isMultiplexerNeedToUpdate = true;
	cameraGridHeight = height;
}
void ofxMultiplexerManager::setCalibratonGridSize(int width,int height)
{
	if (calibrationGridWidth != width)
		isMultiplexerNeedToUpdate = true;
	calibrationGridWidth = width;
	if (calibrationGridHeight != height)
		isMultiplexerNeedToUpdate = true;
	calibrationGridHeight = height;
}

void ofxMultiplexerManager::setMultiplexerSize(int width,int height)
{
	if (stitchedFrameWidth!= width)
		isMultiplexerNeedToUpdate = true;
	stitchedFrameWidth = width;
	if (stitchedFrameHeight!=height)
		isMultiplexerNeedToUpdate = true;
	stitchedFrameHeight = height;
}
void ofxMultiplexerManager::setMultiplexer(ofxMultiplexer* multiplexer)
{
	cameraMultiplexer = multiplexer;
}
void ofxMultiplexerManager::setCalibrator(Calibration* calibrator)
{
	calibration = calibrator;
}
void ofxMultiplexerManager::setProcessFilter(Filters* processFilter)
{
	filters = processFilter;
}

void ofxMultiplexerManager::removeCameraBaseFromMultiplexerAtPosition(int index)
{
	for (int i=0;i<cameraBasesCalibration.size();i++)
	{
		if (cameraBasesCalibration[i]->index == index)
		{
			cameraBasesCalibration.erase(cameraBasesCalibration.begin() + i);
			isMultiplexerNeedToUpdate = true;
			return;
		}
	}
}

void ofxMultiplexerManager::removeAllCameraBasesFromMultiplexer()
{
	cameraBasesCalibration.clear();
	isMultiplexerNeedToUpdate = true;
}

int ofxMultiplexerManager::getIndexOfCameraBase(ofxCameraBase* cameraBase)
{
	if (cameraBase == NULL)
		return -1;
	for (int i=0;i<cameraBases.size();i++)
	{
		if ((cameraBases[i]->getCameraGUID() == cameraBase->getCameraGUID()) && (cameraBases[i]->getBaseCameraType() == cameraBase->getBaseCameraType()))
		{
			return i;
		}
	}
	return -1;
}

int ofxMultiplexerManager::getPositionOfCameraBase(ofxCameraBase* cameraBase)
{
	if (cameraBase == NULL)
		return -1;
	for (int i=0;i<cameraBasesCalibration.size();i++)
	{
		if ((cameraBasesCalibration[i]->camera->getCameraGUID() == cameraBase->getCameraGUID()) &&
			(cameraBasesCalibration[i]->camera->getBaseCameraType() == cameraBase->getBaseCameraType()))
			return cameraBasesCalibration[i]->index;
	}
	return -1;
}

void ofxMultiplexerManager::applySettingsToMultiplexer()
{
	if (!isMultiplexerNeedToUpdate)
		return;
	isMultiplexerNeedToUpdate = false;
	if ((cameraBasesCalibration.size() == 0) && (cameraBases.size() > 0))
	{
		ofxCameraBaseCalibration* newCameraBaseCalibration = new ofxCameraBaseCalibration();
		newCameraBaseCalibration->camera = cameraBases[0];
		newCameraBaseCalibration->index = 0;
		int calibrationPointWidth = calibrationGridWidth + 1;
		int calibrationPointHeight = calibrationGridHeight + 1;
		float cameraCellWidth = 1.0f / calibrationGridWidth;
		float cameraCellHeight = 1.0f / calibrationGridHeight;
		for (int i = 0;i<calibrationPointWidth*calibrationPointHeight;i++)
		{
			vector2df newPoint;
			newPoint.X = cameraCellWidth * (i%(calibrationGridWidth+1));
			newPoint.Y = cameraCellHeight * (i/(calibrationGridWidth+1));
			newCameraBaseCalibration->calibrationPoints.push_back(newPoint);
		}
		cameraBasesCalibration.push_back(newCameraBaseCalibration);
		cameraGridWidth = 1;
		cameraGridHeight = 1;
	}
	cameraMultiplexer->setCalibrationGridSize(calibrationGridWidth,calibrationGridHeight);
	cameraMultiplexer->setCameraGridSize(cameraGridWidth,cameraGridHeight);
	cameraMultiplexer->setStitchedFrameSize(stitchedFrameWidth,stitchedFrameHeight);
	cameraMultiplexer->setInterleaveMode(interleaveMode);
	cameraMultiplexer->clearAllCameraBase();
	for (int i=0;i<cameraBasesCalibration.size();i++)
		cameraMultiplexer->addCameraBase(cameraBasesCalibration[i]);
	cameraMultiplexer->initializeMultiplexer();
}


void ofxMultiplexerManager::startMulticamManager()
{
	enumerateCameras();
	readSettingsFromXML();
	applySettingsToMultiplexer();

}

void ofxMultiplexerManager::initializeCalibration()
{
	calibration->setup(stitchedFrameWidth,stitchedFrameHeight,cameraGridWidth,cameraGridHeight,calibrationGridWidth,calibrationGridHeight,interleaveMode);
}

void ofxMultiplexerManager::startCalibration()
{
	for (int i=0;i<cameraBases.size();i++)
		cameraBases[i]->pauseCamera();
	cameraMultiplexer->setCalibrationGridSize(calibrationGridWidth,calibrationGridHeight);
	cameraMultiplexer->setCameraGridSize(cameraGridWidth,cameraGridHeight);
	cameraMultiplexer->setStitchedFrameSize(stitchedFrameWidth,stitchedFrameHeight);
	cameraMultiplexer->clearAllCameraBase();
	for (int i=0;i<cameraBasesCalibration.size();i++)
	{
		cameraBasesCalibration[i]->calibrationPoints.clear();
		cameraMultiplexer->addCameraBase(cameraBasesCalibration[i]);
	}
	cameraMultiplexer->initializeMultiplexer();
	cameraMultiplexer->pauseStreamingFromAllCameras();
	cameraMultiplexer->startStreamingFromCamera(0);
	cameraMultiplexer->updateStitchedFrame();
	cameraMultiplexer->setIsCalibrationMode(true);
	needToUpdateBackground = true;
}

void ofxMultiplexerManager::cancelCalibration()
{
	if (calibration->calibrate.bCalibrating)
	{
		for (int i=0;i<cameraBasesCalibration.size();i++)
		{
			cameraBasesCalibration[i]->calibrationPoints.clear();
			int calibrationPointWidth = calibrationGridWidth + 1;
			int calibrationPointHeight = calibrationGridHeight + 1;
			float cameraCellWidth = 1.0f / calibrationGridWidth;
			float cameraCellHeight = 1.0f / calibrationGridHeight;
			for (int j = 0;j<calibrationPointWidth*calibrationPointHeight;j++)
			{
				vector2df newPoint;
				newPoint.X = cameraCellWidth * (j%(calibrationGridWidth+1));
				newPoint.Y = cameraCellHeight * (j/(calibrationGridWidth+1));
				cameraBasesCalibration[i]->calibrationPoints.push_back(newPoint);
			}
		}
		stopCalibration();
	}
	else
	{
		cameraMultiplexer->initializeMultiplexer();
		cameraMultiplexer->startStreamingFromAllCameras();
		cameraMultiplexer->updateStitchedFrame();
	}
	filters->bLearnBakground = true;
}

void ofxMultiplexerManager::updateCalibrationStatus()
{
	if (needToUpdateBackground)
	{
		if (calibration->calibrate.selectedCameraIndex > 0)
		{
			cameraMultiplexer->pauseStreamingFromAllCameras();
			cameraMultiplexer->startStreamingFromCamera(calibration->calibrate.selectedCameraIndex);
			cameraMultiplexer->updateStitchedFrame();
		}
		filters->bLearnBakground = true;
		needToUpdateBackground = false;
	}

	if (calibration->calibrate.bNextCamera)
	{
		cameraMultiplexer->setCalibrationPointsToCamera(calibration->calibrate.selectedCameraIndex,calibration->calibrate.cameraPoints);
		calibration->calibrate.nextCameraCalibration();
		if (calibration->calibrate.selectedCameraIndex == 0)
		{
			stopCalibration();
		}
		needToUpdateBackground = true;
	}
}
void ofxMultiplexerManager::stopCalibration()
{
	cameraMultiplexer->initializeMultiplexer();
	cameraMultiplexer->startStreamingFromAllCameras();
	cameraMultiplexer->updateStitchedFrame();
	cameraMultiplexer->setIsCalibrationMode(false);
	filters->bLearnBakground = true;

	//fid filter
	calibration->calibrate.bCalibrating = false;
	calibration->calibrating = true;//WTF???
	calibration->calibrate.bGoToNextStep = false;
	calibration->calibrate.bNextCamera = false;
	isMultiplexerNeedToUpdate = true;
	this->applySettingsToMultiplexer();
}
void ofxMultiplexerManager::resumeCaptureFromAllCameras()
{
	for (int i=0;i<cameraBases.size();i++)
		cameraBases[i]->resumeCamera();
}
void ofxMultiplexerManager::pauseAllUnusedCameras()
{
	for (int i=0;i<cameraBases.size();i++)
	{
		bool isUsed = false;
		for (int j=0;j<cameraBasesCalibration.size();j++)
		{
			if ((cameraBasesCalibration[j]->camera->getBaseCameraType() == cameraBases[i]->getBaseCameraType()) &&
			(cameraBasesCalibration[j]->camera->getBaseCameraGuid() == cameraBases[i]->getBaseCameraGuid()))
			{
				isUsed = true;
				break;
			}
		}
		if (isUsed)
			cameraBases[i]->resumeCamera();
		else
			cameraBases[i]->pauseCamera();
	}
}

void ofxMultiplexerManager::addAllowdedCameraType(CAMERATYPE cameraType)
{
	bool isNew = true;
	for (int i=0;i<allowdedCameraTypes.size();i++)
		if (allowdedCameraTypes[i] == cameraType)
		{
			isNew = false;
			break;
		}
	if (isNew)
		allowdedCameraTypes.push_back(cameraType);
}
void ofxMultiplexerManager::readSettingsFromXML(char* fileName)
{
	ofxXmlSettings* xmlSettings = new ofxXmlSettings();
	if (xmlSettings->loadFile(fileName))
	{
		cameraGridWidth			= xmlSettings->getValue("MULTIPLEXER:CAMERAGRID:WIDTH", 1);
		cameraGridHeight		= xmlSettings->getValue("MULTIPLEXER:CAMERAGRID:HEIGHT", 1);
		calibrationGridWidth	= xmlSettings->getValue("MULTIPLEXER:CALIBRATIONGRID:WIDTH", 4);
		calibrationGridHeight	= xmlSettings->getValue("MULTIPLEXER:CALIBRATIONGRID:HEIGHT", 3);
		interleaveMode			= xmlSettings->getValue("MULTIPLEXER:INTERLEAVE", 0);
		xmlSettings->pushTag("MULTIPLEXER", 0);
		xmlSettings->pushTag("CAMERAS", 0);
		int numCamerasTags = xmlSettings->getNumTags("CAMERA");
		//if (numCamerasTags==cameraGridWidth*cameraGridHeight)
		{
			for (int i=0;i<numCamerasTags;i++)
			{
				ofxCameraBaseCalibration* cameraBaseCalibration = new ofxCameraBaseCalibration();
				cameraBaseCalibration->camera = NULL;
				xmlSettings->pushTag("CAMERA", i);
				std::string cameraTypeName = xmlSettings->getValue("TYPE", "NONE");
				GUID newGuid = StringToGUID(xmlSettings->getValue("GUID", "{00000000-0000-0000-0000-000000000000}"));
				for (int j=0;j<cameraBases.size();j++)
				{
					if ((cameraTypeName == cameraBases[j]->getBaseCameraTypeName()) && (newGuid == cameraBases[j]->getBaseCameraGuid()))
					{
						cameraBaseCalibration->camera = cameraBases[j];
					}
				}
				cameraBaseCalibration->index = 	xmlSettings->getValue("INDEX", i);
				xmlSettings->pushTag("POINTARRAY", 0);
				int numPointTags = xmlSettings->getNumTags("POINT");

				int calibrationPointWidth = calibrationGridWidth + 1;
				int calibrationPointHeight = calibrationGridHeight + 1;
			
				float cameraCellWidth = 1.0f / calibrationGridWidth;
				float cameraCellHeight = 1.0f / calibrationGridHeight;
				
				for (int j = 0;j<calibrationPointWidth*calibrationPointHeight;j++)
				{
					if (numPointTags==calibrationPointWidth*calibrationPointHeight)
						xmlSettings->pushTag("POINT", j);
					vector2df newPoint;
					newPoint.X = xmlSettings->getValue("X",cameraCellWidth * (j%(calibrationGridWidth+1)));
					newPoint.Y = xmlSettings->getValue("Y",cameraCellHeight * (j/(calibrationGridWidth+1)));
					cameraBaseCalibration->calibrationPoints.push_back(newPoint);
					if (numPointTags==calibrationPointWidth*calibrationPointHeight)
						xmlSettings->popTag();
				}
				xmlSettings->popTag();

				xmlSettings->popTag();
				if (cameraBaseCalibration->camera != NULL)
					cameraBasesCalibration.push_back(cameraBaseCalibration);
			}

		}
	}
	xmlSettings->popTag();
	xmlSettings->popTag();
	delete xmlSettings;
}
void ofxMultiplexerManager::saveSettingsToXML(char* fileName)
{
	ofxXmlSettings*	xmlSettings = new ofxXmlSettings();
	if (xmlSettings->loadFile(fileName))
	{
		xmlSettings->setValue("MULTIPLEXER","",0);
		xmlSettings->pushTag("MULTIPLEXER", 0);
		xmlSettings->setValue("CAMERAGRID:WIDTH",cameraGridWidth);
		xmlSettings->setValue("CAMERAGRID:HEIGHT",cameraGridHeight);
		xmlSettings->setValue("CALIBRATIONGRID:WIDTH",calibrationGridWidth);
		xmlSettings->setValue("CALIBRATIONGRID:HEIGHT",calibrationGridHeight);
		xmlSettings->setValue("INTERLEAVE",interleaveMode);
		xmlSettings->setValue("CAMERAS","",0);
		xmlSettings->pushTag("CAMERAS", 0);
		//if (cameraGridWidth*cameraGridHeight == cameraBasesCalibration.size())
		{
			for (int i=0;i<cameraBasesCalibration.size();i++)
			{
				xmlSettings->setValue("CAMERA","",i);
				xmlSettings->pushTag("CAMERA", i);
				if (cameraBasesCalibration[i]->camera != NULL)
				{
					xmlSettings->setValue("GUID", GUIDToString(cameraBasesCalibration[i]->camera->getBaseCameraGuid()));
					xmlSettings->setValue("TYPE",  cameraBasesCalibration[i]->camera->getBaseCameraTypeName());
					xmlSettings->setValue("INDEX", cameraBasesCalibration[i]->index);
					xmlSettings->setValue("POINTARRAY","",0);
					xmlSettings->pushTag("POINTARRAY", 0);
					int numPointTags = cameraBasesCalibration[i]->calibrationPoints.size();
					for (int j = 0;j<numPointTags;j++)
					{
						xmlSettings->setValue("POINT","",j);
 						xmlSettings->pushTag("POINT", j);
						xmlSettings->setValue("X",cameraBasesCalibration[i]->calibrationPoints[j].X);
						xmlSettings->setValue("Y",cameraBasesCalibration[i]->calibrationPoints[j].Y);
						xmlSettings->popTag();
					}
					xmlSettings->popTag();
				}
				xmlSettings->popTag();
			}
		}
		xmlSettings->popTag();
		xmlSettings->popTag();
	}
	xmlSettings->saveFile(fileName);
	delete xmlSettings;
	xmlSettings = NULL;
}

void ofxMultiplexerManager::addCameraBaseToMultiplexerAtPosition(int position,ofxCameraBase* cameraBase)
{
	if (cameraBase == NULL)
		return;
	bool isFound = false;
	for (int i=0;i<cameraBases.size();i++)
	{
		if ((cameraBases[i]->getCameraGUID() == cameraBase->getBaseCameraGuid()) && (cameraBases[i]->getBaseCameraType() == cameraBase->getBaseCameraType()))
		{
			isFound = true;
			break;
		}
	}
	if (!isFound)
		return;
	ofxCameraBaseCalibration* newCameraBaseCalibration = new ofxCameraBaseCalibration();
	newCameraBaseCalibration->camera = cameraBase;
	newCameraBaseCalibration->index = position;
	int calibrationPointWidth = calibrationGridWidth + 1;
	int calibrationPointHeight = calibrationGridHeight + 1;
	float cameraCellWidth = 1.0f / calibrationGridWidth;
	float cameraCellHeight = 1.0f / calibrationGridHeight;
	for (int i = 0;i<calibrationPointWidth*calibrationPointHeight;i++)
	{
		vector2df newPoint;
		newPoint.X = cameraCellWidth * (i%(calibrationGridWidth+1));
		newPoint.Y = cameraCellHeight * (i/(calibrationGridWidth+1));
		newCameraBaseCalibration->calibrationPoints.push_back(newPoint);
	}
	vector<int> indexes;
	for (int i=cameraBasesCalibration.size()-1;i>=0;i--)
	{
		if (cameraBasesCalibration[i]->index == position)
			indexes.push_back(i);
	}
	for (int i=0;i<indexes.size();i++)
	{
		cameraBasesCalibration.erase(cameraBasesCalibration.begin()+indexes[i]);
	}
	cameraBasesCalibration.push_back(newCameraBaseCalibration);
	isMultiplexerNeedToUpdate = true;
}

ofxCameraBase* ofxMultiplexerManager::getCameraBase(int index)
{
	if (index>=cameraBases.size())
		return NULL;
	return cameraBases[index];

}

ofxCameraBase* ofxMultiplexerManager::getCameraBaseAtPosition(int position)
{
	for (int i=0;i<cameraBasesCalibration.size();i++)
	{
		if (cameraBasesCalibration[i]->index == position)
			return cameraBasesCalibration[i]->camera;
	}
	return NULL;
}

ofxCameraBase* ofxMultiplexerManager::getCameraBase(CAMERATYPE cameraType,GUID guid)
{
	for (int i=0;i<cameraBases.size();i++)
	{
		if ((cameraBases[i]->getBaseCameraType() == cameraType) && (guid == cameraBases[i]->getBaseCameraGuid()))
			return cameraBases[i];
	}
	return NULL;
}
int ofxMultiplexerManager::getCameraBaseCount()
{
	return cameraBases.size();
}

void ofxMultiplexerManager::enumerateCameras()
{
	for (int i=0;i<allowdedCameraTypes.size();i++)
	{
		if (allowdedCameraTypes[i] == PS3)
		{
			ofxCameraBase* cam = (ofxCameraBase*)(new ofxPS3());
			int cameraCount = 0;
			GUID* cameraGUIDs = cam->getBaseCameraGuids(&cameraCount);
			for (int j=0;j<cameraCount;j++)
			{
				ofxCameraBase* newCam = (ofxCameraBase*)(new ofxPS3());
				newCam->initializeWithGUID(cameraGUIDs[j]);
				cameraBases.push_back(newCam);
			}
			delete cam;
		}
		if (allowdedCameraTypes[i] == CMU)
		{
			ofxCameraBase* cam = (ofxCameraBase*)(new ofxCMUCamera());
			int cameraCount = 0;
			GUID* cameraGUIDs = cam->getBaseCameraGuids(&cameraCount);
			for (int j=0;j<cameraCount;j++)
			{
				ofxCameraBase* newCam = (ofxCameraBase*)(new ofxCMUCamera());
				newCam->initializeWithGUID(cameraGUIDs[j]);
				cameraBases.push_back(newCam);
			}
			delete cam;
		}
		if (allowdedCameraTypes[i] == FFMV)
		{
			ofxCameraBase* cam = (ofxCameraBase*)(new ofxffmv());
			int cameraCount = 0;
			GUID* cameraGUIDs = cam->getBaseCameraGuids(&cameraCount);
			for (int j=0;j<cameraCount;j++)
			{
				ofxCameraBase* newCam = (ofxCameraBase*)(new ofxffmv());
				newCam->initializeWithGUID(cameraGUIDs[j]);
				cameraBases.push_back(newCam);
			}
			delete cam;
		}
		if (allowdedCameraTypes[i] == KINECT)
		{
			ofxCameraBase* cam = (ofxCameraBase*)(new ofxKinect());
			int cameraCount = 0;
			GUID* cameraGUIDs = cam->getBaseCameraGuids(&cameraCount);
			for (int j=0;j<cameraCount;j++)
			{
				ofxCameraBase* newCam = (ofxCameraBase*)(new ofxKinect());
				newCam->initializeWithGUID(cameraGUIDs[j]);
				cameraBases.push_back(newCam);
			}
			delete cam;
		}
		if (allowdedCameraTypes[i] == DIRECTSHOW)
		{
			ofxCameraBase* cam = (ofxCameraBase*)(new ofxDShow());
			int cameraCount = 0;
			GUID* cameraGUIDs = cam->getBaseCameraGuids(&cameraCount);
			for (int j=0;j<cameraCount;j++)
			{
				ofxCameraBase* newCam = (ofxCameraBase*)(new ofxDShow());
				newCam->initializeWithGUID(cameraGUIDs[j]);
				cameraBases.push_back(newCam);
			}
			delete cam;
		}
	}
	for (int i=0;i<cameraBases.size();i++)
	{
		cameraBases[i]->startCamera();
		cameraBases[i]->pauseCamera();
	}
}

void ofxMultiplexerManager::setInterleaveMode(bool isInterleaveMode)
{
	if (isInterleaveMode!=interleaveMode)
		isMultiplexerNeedToUpdate = true;
	interleaveMode = isInterleaveMode;
}

bool ofxMultiplexerManager::getInterleaveMode()
{
	return interleaveMode;
}
