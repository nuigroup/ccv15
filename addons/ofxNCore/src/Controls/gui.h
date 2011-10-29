/*
*  GUI.h
*
*
*  Created on 2/2/09.
*  Copyright 2009 NUI Group. All rights reserved.
*
*/

#ifndef GUI_CONTROLS_H
#define GUI_CONTROLS_H

#include "../Modules/ofxNCoreVision.h"
#include "../Camera/CameraUtils.h"

ofxNCoreVision  *appPtr;
CameraUtils		cameraCount;
//int MAIN_PANEL_SECOND_X = MAIN_PANEL_SECOND_X;

ofxNCoreVision* ofxNCoreVision::Instance()
{
	return appPtr;
}

void ofxNCoreVision::setupControls()
{
	appPtr = this;

	//panel border color
	controls->mGlobals->mBorderColor.r = 0;
	controls->mGlobals->mBorderColor.g = 0;
	controls->mGlobals->mBorderColor.b = 0;
	controls->mGlobals->mBorderColor.a = .3;
	//panel color
	controls->mGlobals->mCoverColor.r = 1;
	controls->mGlobals->mCoverColor.g = 1;
	controls->mGlobals->mCoverColor.b = 1;
	controls->mGlobals->mCoverColor.a = .4;
	//control outline color
	controls->mGlobals->mFrameColor.r = 0;
	controls->mGlobals->mFrameColor.g = 0;
	controls->mGlobals->mFrameColor.b = 0;
	controls->mGlobals->mFrameColor.a = .3;
	//text color
	controls->mGlobals->mTextColor.r = 0;
	controls->mGlobals->mTextColor.g = 0;
	controls->mGlobals->mTextColor.b = 0;
	controls->mGlobals->mTextColor.a = 1;
	//button color
	controls->mGlobals->mButtonColor.r = 1;
	controls->mGlobals->mButtonColor.g = 0;
	controls->mGlobals->mButtonColor.b = 0;
	controls->mGlobals->mButtonColor.a = .8;
	//slider tip color
	controls->mGlobals->mHandleColor.r = 0;
	controls->mGlobals->mHandleColor.g = 0;
	controls->mGlobals->mHandleColor.b = 0;
	//slider color
	controls->mGlobals->mSliderColor.r = 1;
	controls->mGlobals->mSliderColor.g = 0;
	controls->mGlobals->mSliderColor.b = 0;
	controls->mGlobals->mSliderColor.a = .8;

	this->addMainPanels();
	this->updateMainPanels();

}

void ofxNCoreVision::updateMainPanels()
{
	controls->forceUpdate(false);
	controls->activate(true);

	//do update while inactive

	/****************************
	* Set GUI values on startup
	****************************/
	controls->update(appPtr->propertiesPanel_flipV, kofxGui_Set_Bool, &appPtr->filter->bVerticalMirror, sizeof(bool));
	controls->update(appPtr->propertiesPanel_flipH, kofxGui_Set_Bool, &appPtr->filter->bHorizontalMirror, sizeof(bool));
	controls->update(appPtr->trackedPanel_outlines, kofxGui_Set_Bool, &appPtr->bDrawOutlines, sizeof(bool));
	controls->update(appPtr->trackedPanel_ids, kofxGui_Set_Bool, &appPtr->bShowLabels, sizeof(bool));
	controls->update(appPtr->trackedPanel_darkblobs, kofxGui_Set_Bool, &appPtr->filter->bTrackDark, sizeof(bool));
	//Source
	//controls->update(appPtr->sourcePanel_recording, kofxGui_Set_Bool, &(bool), sizeof(bool));
	//Calibration
	if (bcamera)
	{
		controls->update(appPtr->calibrationPanel_calibrate, kofxGui_Set_Bool, &appPtr->bCalibration, sizeof(bool));
		controls->update(appPtr->calibrationPanel_multicam, kofxGui_Set_Bool, &appPtr->bMultiCamsInterface, sizeof(bool));
	}
	controls->update(appPtr->generalSettingsPanel_fiducial_mode, kofxGui_Set_Bool, &appPtr->bFidtrackInterface, sizeof(bool));
	//Dynamic Background
	controls->update(appPtr->backgroundPanel_dynamic, kofxGui_Set_Bool, &appPtr->filter->bDynamicBG, sizeof(bool));
	//Smooth
	controls->update(appPtr->smoothPanel_use, kofxGui_Set_Bool, &appPtr->filter->bSmooth, sizeof(bool));
	controls->update(appPtr->smoothPanel_smooth, kofxGui_Set_Bool, &appPtr->filter->smooth, sizeof(float));
	//Highpass
	controls->update(appPtr->highpassPanel_use, kofxGui_Set_Bool, &appPtr->filter->bHighpass, sizeof(bool));
	controls->update(appPtr->highpassPanel_blur, kofxGui_Set_Bool, &appPtr->filter->highpassBlur, sizeof(float));
	controls->update(appPtr->highpassPanel_noise, kofxGui_Set_Bool, &appPtr->filter->highpassNoise, sizeof(float));
	//Amplify 
	controls->update(appPtr->amplifyPanel_use, kofxGui_Set_Bool, &appPtr->filter->bAmplify, sizeof(bool));
	controls->update(appPtr->amplifyPanel_amp, kofxGui_Set_Bool, &appPtr->filter->highpassAmp, sizeof(float));
	//Threshold

	// Normalize
	if (!bFidtrackInterface)
	{
		controls->update(appPtr->trackedPanel_threshold, kofxGui_Set_Bool, &appPtr->filter->threshold, sizeof(float));
		controls->update(appPtr->trackedPanel_thresholdDynamic, kofxGui_Set_Bool, &appPtr->filter->bDynamicTH, sizeof(bool));
		controls->update(appPtr->trackedPanel_min_movement, kofxGui_Set_Bool, &appPtr->tracker.MOVEMENT_FILTERING, sizeof(float));
	}
	else
	{
		controls->update(appPtr->trackedPanel_fid_tile_size, kofxGui_Set_Bool, &appPtr->filter_fiducial->fiducial_tile_size, sizeof(float));
		controls->update(appPtr->trackedPanel_fid_threshold, kofxGui_Set_Bool, &appPtr->filter_fiducial->fiducialThreshold, sizeof(float));
	}
	//Min Movement
	//Min Blob Size
	controls->update(appPtr->trackedPanel_min_blob_size, kofxGui_Set_Bool, &appPtr->MIN_BLOB_SIZE, sizeof(float));
	//Max Blob Size
	controls->update(appPtr->trackedPanel_max_blob_size, kofxGui_Set_Bool, &appPtr->MAX_BLOB_SIZE, sizeof(float));
	//Template Area
	if (contourFinder.bTrackObjects)
	{
		controls->update(appPtr->TemplatePanel_minArea, kofxGui_Set_Bool, &appPtr->minTempArea, sizeof(float));
		controls->update(appPtr->TemplatePanel_maxArea, kofxGui_Set_Bool, &appPtr->maxTempArea, sizeof(float));
	}
	//Background Learn Rate
	controls->update(appPtr->backgroundPanel_learn_rate, kofxGui_Set_Bool, &appPtr->backgroundLearnRate, sizeof(float));
	//Track Panel
	controls->update(appPtr->trackingPanel_trackFingers, kofxGui_Set_Bool, &appPtr->contourFinder.bTrackFingers, sizeof(bool));
	controls->update(appPtr->trackingPanel_trackObjects, kofxGui_Set_Bool, &appPtr->contourFinder.bTrackObjects, sizeof(bool));
	controls->update(appPtr->trackingPanel_trackFiducials, kofxGui_Set_Bool, &appPtr->contourFinder.bTrackFiducials, sizeof(bool));
	//Send TUIO
	controls->update(appPtr->optionPanel_tuio_osc, kofxGui_Set_Bool, &appPtr->myTUIO.bOSCMode, sizeof(bool));
	controls->update(appPtr->optionPanel_tuio_tcp, kofxGui_Set_Bool, &appPtr->myTUIO.bTCPMode, sizeof(bool));
	controls->update(appPtr->optionPanel_bin_tcp, kofxGui_Set_Bool, &appPtr->myTUIO.bBinaryMode, sizeof(bool));
	//controls->update(appPtr->optionPanel_win_hid, kofxGui_Set_Bool, &appPtr->bWinTouch, sizeof(bool));
	//TUIO Height Width
//	controls->update(appPtr->optionPanel_tuio_height_width, kofxGui_Set_Bool, &appPtr->myTUIO.bHeightWidth, sizeof(bool));

}

	// ---------------------------------- START TRACK VIEW-----------------------------------------------------------------------------------------
void ofxNCoreVision::addMainPanels() 
{
	appPtr = this;

	// ---------------------------------- START MAIN PANELS ----------------- 90*i------------------------------------------------------------------------
	// Views

	if (bcamera)
	{
		ofxGuiPanel* cPanel = controls->addPanel(appPtr->calibrationPanel, "Views", MAIN_PANEL_SECOND_X, MAIN_TOP_OFFSET - 10, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
		cPanel->addButton(appPtr->calibrationPanel_calibrate, "Calibration (c)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Trigger);
		cPanel->addButton(appPtr->calibrationPanel_multicam, "Setup/Track (TAB)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Trigger);
		cPanel->mObjWidth = 200;
		cPanel->mObjHeight = 75;
	}
	// Settings
	ofxGuiPanel* panel2 = controls->addPanel(appPtr->savePanel, "Settings", MAIN_PANEL_SECOND_X, 95 - (bcamera ? 0 : 80), OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
	panel2->addButton(appPtr->kParameter_SaveXml, "Save (s)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Trigger);
	panel2->addButton(appPtr->generalSettingsPanel_fiducial_mode, "Fiducial settings (m)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, bFidtrackInterface, kofxGui_Button_Switch);
	//panel2->addButton(appPtr->kParameter_SaveTemplateXml, "Save Templates", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Trigger);
	//panel2->addButton(appPtr->kParameter_LoadTemplateXml, "Load Templates", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Trigger);
	panel2->mObjWidth = 200;
	panel2->mObjHeight = 75;

	// Communications
	ofxGuiPanel* oPanel = controls->addPanel(appPtr->optionPanel, "Communication", MAIN_PANEL_SECOND_X, 175 - (bcamera ? 0 : 80), OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
	oPanel->addButton(appPtr->optionPanel_bin_tcp, "Binary TCP (n)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch);
	oPanel->addButton(appPtr->optionPanel_tuio_osc, "TUIO UDP (t)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch);
	oPanel->addButton(appPtr->optionPanel_tuio_tcp, "Flash XML (f)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch);
	oPanel->mObjWidth = 200;
	oPanel->mObjHeight = 95;

	// Templates


	// Transforms
	ofxGuiPanel* propPanel = controls->addPanel(appPtr->propertiesPanel, "Transforms", MAIN_PANEL_SECOND_X, 275 - (bcamera ? 0 : 80), 12, OFXGUI_PANEL_SPACING);
	propPanel->addButton(appPtr->propertiesPanel_flipV, "Flip Vertical (v)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch);
	propPanel->addButton(appPtr->propertiesPanel_flipH, "Flip Horizontal (h)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch);
	propPanel->mObjWidth = 200;
	propPanel->mObjHeight = 75;

	// Tracking
	ofxGuiPanel* trackingPanel = controls->addPanel(appPtr->trackingPanel, "Track", MAIN_PANEL_SECOND_X, 355 -(bcamera ? 0 : 80), OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
	trackingPanel->addButton(appPtr->trackingPanel_trackFingers, "Fingers (g)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch);
	trackingPanel->addButton(appPtr->trackingPanel_trackFiducials, "Fiducials (d)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch);
	trackingPanel->addButton(appPtr->trackingPanel_trackObjects, "Objects (j)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch);
	trackingPanel->mObjWidth = 200;
	trackingPanel->mObjHeight = 95;


	// ---------------------------------- START IO PANELS -----------------------------------------------------------------------------------------
	//Input Panel
	ofxGuiPanel* srcPanel = controls->addPanel(appPtr->sourcePanel, "Input", 251, 280, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
	srcPanel->addButton(appPtr->trackedPanel_outlines, "Outlines (o)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch);
	srcPanel->addButton(appPtr->trackedPanel_ids, "IDs (i)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch);
	srcPanel->addSlider(appPtr->trackedPanel_min_blob_size, "Min Blob Size", MAIN_FILTERS_Y, 13, 1.0f, 500.0f, MIN_BLOB_SIZE, kofxGui_Display_Int, 0);
	srcPanel->addSlider(appPtr->trackedPanel_max_blob_size, "Max Blob Size", MAIN_FILTERS_Y, 13, 1.0f, 1000.0f, MAX_BLOB_SIZE, kofxGui_Display_Int, 0);
	srcPanel->mObjWidth = 325;
	srcPanel->mObjHeight = 75;
	srcPanel->mObjects[0]->mObjX = 220;
	srcPanel->mObjects[0]->mObjY = 11;
	srcPanel->mObjects[1]->mObjX = 155;
	srcPanel->mObjects[1]->mObjY = 11;	
	srcPanel->mObjects[2]->mObjY = 32;
	srcPanel->mObjects[3]->mObjX = 165;
	srcPanel->mObjects[3]->mObjY = 32;
	srcPanel->adjustToNewContent(100, 0);

	//Output Panel
	ofxGuiPanel* trackPanel = controls->addPanel(appPtr->trackedPanel, "Output", 251+335, 280, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
	trackPanel->addButton(appPtr->trackedPanel_darkblobs, "Inverse (i)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch);
	if (!bFidtrackInterface)
	{
		trackPanel->addSlider(appPtr->trackedPanel_threshold, "Image Threshold", MAIN_FILTERS_Y, 13, 0.0f, 255.0f, filter->threshold, kofxGui_Display_Int, 0);
		trackPanel->addSlider(appPtr->trackedPanel_min_movement, "Movement Filtering", MAIN_FILTERS_Y, 13, 0.0f, 15.0f, tracker.MOVEMENT_FILTERING, kofxGui_Display_Int, 0);
	}
	else
	{
		trackPanel->addSlider(appPtr->trackedPanel_fid_threshold, "Image Threshold", MAIN_FILTERS_Y, 13, 0.0f, 255.0f, filter_fiducial->fiducialThreshold, kofxGui_Display_Int, 0);
		trackPanel->addSlider(appPtr->trackedPanel_fid_tile_size, "Tile size", MAIN_FILTERS_Y, 13, 8.0f, 64.0f, filter_fiducial->fiducial_tile_size, kofxGui_Display_Int, 0);
	}
	trackPanel->addButton(appPtr->trackedPanel_thresholdDynamic, "Dynamic Threshold", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch);

	trackPanel->mObjHeight = 75;
	trackPanel->mObjWidth = 325;
	trackPanel->mObjects[0]->mObjX = 230;
	trackPanel->mObjects[0]->mObjY = 11;
	trackPanel->mObjects[1]->mObjY = 32;
	trackPanel->mObjects[2]->mObjX = 165;
	trackPanel->mObjects[2]->mObjY = 32;
	trackPanel->mObjects[3]->mObjX = 100;
	trackPanel->mObjects[3]->mObjY = 11;
	trackPanel->adjustToNewContent(100, 0);

	// ---------------------------------- START FILTER PANELS-----------------------------------------------------------------------------------------
	//Background Subtract
	ofxGuiPanel* bkPanel1 = controls->addPanel(appPtr->backgroundPanel, "Background", MAIN_FILTERS_X+MAIN_FILTERS_W*0, 470, OFXGUI_PANEL_BORDER, 7);
	bkPanel1->addButton(backgroundPanel_remove, "Subtract BG (b)", 10, 10, kofxGui_Button_Off, kofxGui_Button_Trigger);
	bkPanel1->addButton(backgroundPanel_dynamic, "Dynamic Subtract", 10, 10, kofxGui_Button_Off, kofxGui_Button_Switch);
	bkPanel1->addSlider(appPtr->backgroundPanel_learn_rate, "Subtract Speed", MAIN_FILTERS_Z, 13, 1.0f, 2000.0f, backgroundLearnRate, kofxGui_Display_Int, 0);
	bkPanel1->mObjWidth = 128;
	bkPanel1->mObjHeight = 95;

	//Smooth Image
	ofxGuiPanel* sPanel = controls->addPanel(appPtr->smoothPanel, "Smooth", MAIN_FILTERS_X+MAIN_FILTERS_W*1, 470, OFXGUI_PANEL_BORDER, 7);
	sPanel->addButton(smoothPanel_use, "", 12, 12, kofxGui_Button_Off, kofxGui_Button_Switch);
	sPanel->addSlider(smoothPanel_smooth, "Smooth", MAIN_FILTERS_Z, 13, 0.0f, 15.0f, filter->smooth, kofxGui_Display_Int, 0);
	sPanel->mObjects[0]->mObjX = 105;
	sPanel->mObjects[0]->mObjY = 10;
	sPanel->mObjects[1]->mObjY = 30;
	sPanel->mObjWidth = 128;
	sPanel->mObjHeight = 95;

	//Highpass Image
	ofxGuiPanel* hpPanel = controls->addPanel(appPtr->highpassPanel, "Highpass", MAIN_FILTERS_X+MAIN_FILTERS_W*2,470, OFXGUI_PANEL_BORDER, 7);
	hpPanel->addButton(highpassPanel_use, "", 12, 12, kofxGui_Button_Off, kofxGui_Button_Switch);
	hpPanel->addSlider(highpassPanel_blur, "Blur", MAIN_FILTERS_Z, 13, 0.0f, 200.0f, filter->highpassBlur, kofxGui_Display_Int, 0);
	hpPanel->addSlider(highpassPanel_noise, "Noise", MAIN_FILTERS_Z, 13, 0.0f, 30.0f, filter->highpassNoise, kofxGui_Display_Int, 0);
	hpPanel->mObjects[0]->mObjX = 105;
	hpPanel->mObjects[0]->mObjY = 10;
	hpPanel->mObjects[1]->mObjY = 30;
	hpPanel->mObjects[2]->mObjY = 60;
	hpPanel->mObjWidth = 128;
	hpPanel->mObjHeight = 95;

	//Amplify Image
	ofxGuiPanel* ampPanel = controls->addPanel(appPtr->amplifyPanel, "Amplify", MAIN_FILTERS_X+MAIN_FILTERS_W*3,470, OFXGUI_PANEL_BORDER, 7);
	ampPanel->addButton(amplifyPanel_use, "", 12, 12, kofxGui_Button_Off, kofxGui_Button_Switch);
	ampPanel->addSlider(amplifyPanel_amp, "Amplify", MAIN_FILTERS_Z, 13, 0.0f, 300.0f, filter->highpassAmp, kofxGui_Display_Int, 0);
	ampPanel->mObjects[0]->mObjX = 105;
	ampPanel->mObjects[0]->mObjY = 10;
	ampPanel->mObjects[1]->mObjY = 30;
	ampPanel->mObjWidth = 128;
	ampPanel->mObjHeight = 95;

	if (contourFinder.bTrackObjects)
	{
		ofxGuiPanel* panel2x = controls->addPanel(appPtr->templatePanel, "Templates", MAIN_PANEL_SECOND_X, 455 - (bcamera ? 0 : 80), OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
		panel2x->addButton(appPtr->kParameter_SaveTemplateXml, "Save Template" , OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Trigger);
		panel2x->addButton(appPtr->kParameter_LoadTemplateXml, "Load Template", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Trigger);
		panel2x->addSlider(appPtr->TemplatePanel_minArea, "Min Area", MAIN_FILTERS_Y, 13, 0.0f, 2000.0f, minTempArea, kofxGui_Display_Int, 0);
		panel2x->addSlider(appPtr->TemplatePanel_maxArea, "Max Area", MAIN_FILTERS_Y, 13, 0.0f, 2000.0f, maxTempArea, kofxGui_Display_Int, 0);
		panel2x->mObjWidth = 200;
		panel2x->mObjHeight = 140;
		panel2x->mObjects[2]->mObjY = 75;
		panel2x->mObjects[3]->mObjY = 105;
		panel2x->adjustToNewContent(120, 0);
	}

//----------------------------------------------
	updateMainPanels();
//	controls->forceUpdate(false);
//	controls->activate(true);

}
	// ---------------------------------- START SETUP VIEW -----------------------------------------------------------------------------------------
void ofxNCoreVision::addMulticameraPanels()
{
	appPtr = this;

	// Views
	ofxGuiPanel* cPanel = controls->addPanel(appPtr->calibrationPanel, "Views", MAIN_PANEL_SECOND_X, MAIN_TOP_OFFSET - 10, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
	cPanel->addButton(appPtr->calibrationPanel_calibrate, "Calibration (c)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Trigger);
	cPanel->addButton(appPtr->calibrationPanel_tracker, "Setup/Track (TAB)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Trigger);
	cPanel->mObjWidth = 200;
	cPanel->mObjHeight = 75;

	// Settings
	ofxGuiPanel* dialogPanel = controls->addPanel(appPtr->generalSettingsPanel, "Settings", MAIN_PANEL_SECOND_X, 95, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
	dialogPanel->addButton(appPtr->generalSettingsPanel_apply_settings, "Save (s)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Trigger);
	dialogPanel->mObjHeight = 55;
	dialogPanel->mObjWidth = 200;

	// Camera Grid Size
	ofxGuiPanel* cameraGridPanel = controls->addPanel(appPtr->cameraGridSettingsPanel, "Camera Grid Size", MAIN_PANEL_SECOND_X, 155, 12, OFXGUI_PANEL_SPACING);
	int gWidth,gHeight;
	multiplexerManager->getCameraGridSize(&gWidth,&gHeight);
	cameraGridPanel->addSlider(appPtr->cameraGridSettingsPanel_x, "X Axis", MAIN_PANEL_SLIDER_WIDTH, MAIN_PANEL_SLIDER_HEIGHT, 1,16,gWidth,kofxGui_Display_Int,0);
	cameraGridPanel->addSlider(appPtr->cameraGridSettingsPanel_y, "Y Axis", MAIN_PANEL_SLIDER_WIDTH, MAIN_PANEL_SLIDER_HEIGHT, 1,16,gHeight,kofxGui_Display_Int,0);
	cameraGridPanel->addButton(appPtr->cameraGridSettingsPanel_interleave, "Interleave Mode", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, multiplexerManager->getInterleaveMode() ? 1 : 0, kofxGui_Button_Switch);
	cameraGridPanel->addButton(appPtr->generalSettingsPanel_reset_all, "Reset (r)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Trigger);
	cameraGridPanel->mObjWidth = 200;
	cameraGridPanel->mObjHeight = 145;

	// Calibration Grid Size
	ofxGuiPanel* calibrationGridPanel = controls->addPanel(appPtr->calibrationGridSettingsPanel, "Calibration Grid Size", MAIN_PANEL_SECOND_X, 305, 12, OFXGUI_PANEL_SPACING);
	multiplexerManager->getCalibrationGridSize(&gWidth,&gHeight);
	calibrationGridPanel->addSlider(appPtr->calibrationGridSettingsPanel_x, "X Axis", MAIN_PANEL_SLIDER_WIDTH, MAIN_PANEL_SLIDER_HEIGHT, 1,16,gWidth,kofxGui_Display_Int,0);
	calibrationGridPanel->addSlider(appPtr->calibrationGridSettingsPanel_y, "Y Axis", MAIN_PANEL_SLIDER_WIDTH, MAIN_PANEL_SLIDER_HEIGHT, 1,16,gHeight,kofxGui_Display_Int,0);
	calibrationGridPanel->mObjWidth = 200;
	calibrationGridPanel->mObjHeight = 100;

	// Camera Display Grid
	ofxGuiPanel* camerasDisplayPanel = controls->addPanel(appPtr->camerasDisplayPanel, "" , 250, 25, 0, OFXGUI_PANEL_SPACING );
	camerasDisplayPanel->isTransparent = true;
	multiplexerManager->getCameraGridSize(&gWidth,&gHeight);
	camsGrid = (ofxGuiGrid*)camerasDisplayPanel->addGrid( camerasDisplayPanel_grid, "", 662, 400, gWidth, gHeight, 0, 5, kofxGui_Grid_Display );
	camsGrid->setMultiplexerManager( multiplexerManager );
	camsGrid->setResetBtnId( camerasDisplayPanel_grid_reset );
	camsGrid->setSettingBtnId( camerasDisplayPanel_grid_setting );
	camsGrid->setShowResetBtn( true );
	camsGrid->setShowSettingBtn( true );
	camsGrid->enableDblClickMode( true );
	camerasDisplayPanel->mObjWidth = 662;	
	camerasDisplayPanel->mObjHeight = 400;

	// Camera Device List
	ofxGuiPanel* cameraListPanel = controls->addPanel(appPtr->devicesListPanel, "", 242, 410, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
	cameraListPanel->isTransparent = true;
	devGrid = (ofxGuiGrid*)cameraListPanel->addGrid( devicesListPanel_grid, "", 553, 110, 4, 1, 5, 5, kofxGui_Grid_List );
	devGrid->setMultiplexerManager( multiplexerManager );
	devGrid->setMode( kofxGui_Grid_Selectable, true );
	devGrid->setDrawSelectedText( true );
	cameraListPanel->addArrow( devicesListPanel_arrow_left, "", 40, 109, kofxGui_Arrow_Left );
	cameraListPanel->addArrow( devicesListPanel_arrow_right, "", 40, 109, kofxGui_Arrow_Right );
	cameraListPanel->mObjWidth = 700;
	cameraListPanel->mObjHeight = 190;
	cameraListPanel->mObjects[0]->mObjX = 60;	//! [0]: devices grid
	cameraListPanel->mObjects[0]->mObjY = 42;
	cameraListPanel->mObjects[1]->mObjX = 10;	//! [1]: left arrow
	cameraListPanel->mObjects[1]->mObjY = 42;
	cameraListPanel->mObjects[2]->mObjX = 623;//623;	//! [2]: right arrow
	cameraListPanel->mObjects[2]->mObjY = 42;
	cameraListPanel->adjustToNewContent( 500, 0 );

}

void ofxNCoreVision::setFiducialSettings(bool isFiducialsSettings)
{
	if (!bMultiCamsInterface)
	{
		bFidtrackInterface = isFiducialsSettings;
		controls->removePanel(this->trackedPanel);
		ofxGuiPanel* trackPanel = controls->addPanel(appPtr->trackedPanel, "Output", 251+335, 280, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
		trackPanel->addButton(appPtr->trackedPanel_darkblobs, "Inverse (i)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch);
		if (!bFidtrackInterface)
		{
			trackPanel->addSlider(appPtr->trackedPanel_threshold, "Image Threshold", MAIN_FILTERS_Y, 13, 0.0f, 255.0f, filter->threshold, kofxGui_Display_Int, 0);
			trackPanel->addSlider(appPtr->trackedPanel_min_movement, "Movement Filtering", MAIN_FILTERS_Y, 13, 0.0f, 15.0f, tracker.MOVEMENT_FILTERING, kofxGui_Display_Int, 0);
		}
		else
		{
			trackPanel->addSlider(appPtr->trackedPanel_fid_threshold, "Image Threshold", MAIN_FILTERS_Y, 13, 0.0f, 255.0f, filter_fiducial->fiducialThreshold, kofxGui_Display_Int, 0);
			trackPanel->addSlider(appPtr->trackedPanel_fid_tile_size, "Tile size", MAIN_FILTERS_Y, 13, 8.0f, 64.0f, filter_fiducial->fiducial_tile_size, kofxGui_Display_Int, 0);
		}
		trackPanel->addButton(appPtr->trackedPanel_thresholdDynamic, "Dynamic Threshold", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch);
		trackPanel->mObjHeight = 75;
		trackPanel->mObjWidth = 325;
		trackPanel->mObjects[0]->mObjX = 230;
		trackPanel->mObjects[0]->mObjY = 11;
		trackPanel->mObjects[1]->mObjY = 32;
		trackPanel->mObjects[2]->mObjX = 165;
		trackPanel->mObjects[2]->mObjY = 32;
		trackPanel->mObjects[3]->mObjX = 100;
		trackPanel->mObjects[3]->mObjY = 11;
		trackPanel->adjustToNewContent(100, 0);
		
		controls->forceUpdate(false);
		controls->activate(true);
		if (!bFidtrackInterface)
		{
			controls->update(appPtr->trackedPanel_threshold, kofxGui_Set_Bool, &appPtr->filter->threshold, sizeof(float));
			controls->update(appPtr->trackedPanel_thresholdDynamic, kofxGui_Set_Bool, &appPtr->filter->bDynamicTH, sizeof(bool));
			controls->update(appPtr->trackedPanel_min_movement, kofxGui_Set_Bool, &appPtr->tracker.MOVEMENT_FILTERING, sizeof(float));
		}
		else
		{
			controls->update(appPtr->trackedPanel_fid_tile_size, kofxGui_Set_Bool, &appPtr->filter_fiducial->fiducial_tile_size, sizeof(float));
			controls->update(appPtr->trackedPanel_fid_threshold, kofxGui_Set_Bool, &appPtr->filter_fiducial->fiducialThreshold, sizeof(float));
		}
	}
}



void ofxNCoreVision::removeMainPanels() 
{
	controls->removePanel( this->sourcePanel );
	controls->removePanel( this->TemplatePanel );
	controls->removePanel( this->trackedPanel );
	controls->removePanel( this->backgroundPanel );
	controls->removePanel( this->smoothPanel );
	controls->removePanel( this->highpassPanel );
	controls->removePanel( this->amplifyPanel );
    if (bcamera)
		controls->removePanel( this->calibrationPanel );
	controls->removePanel( this->propertiesPanel );
	controls->removePanel( this->optionPanel );
	controls->removePanel( this->savePanel );	
	if (contourFinder.bTrackObjects)
		controls->removePanel( this->templatePanel );
	controls->removePanel( this->trackingPanel );
}
void ofxNCoreVision::removeMulticameraPanels()
{
	controls->removePanel( this->calibrationPanel );
	controls->removePanel( this->cameraGridSettingsPanel );
	controls->removePanel( this->calibrationGridSettingsPanel );
	controls->removePanel( this->generalSettingsPanel );
	controls->removePanel( this->camerasDisplayPanel );
	controls->removePanel( this->devicesListPanel );
}

void ofxNCoreVision ::handleGui(int parameterId, int task, void* data, int length)
{
	printf( "DEBUG: foxNCoreVision::handleGui\n" );
	switch(parameterId)
	{
		case generalSettingsPanel_fiducial_mode:
			if ( length == sizeof(bool))
			{
				setFiducialSettings(*(bool*)data);
			}
			break;
		case trackedPanel_fid_threshold:
			if( length == sizeof(float) ) 
			{
				filter_fiducial->fiducialThreshold = (int)(*(float*)data);
			}
			break;
		case trackedPanel_fid_tile_size:
			if( length == sizeof(float) ) 
			{
				filter_fiducial->fiducial_tile_size = (int)(*(float*)data);
				filter_fiducial->updateSettings();
			}
			break;

		case cameraGridSettingsPanel_interleave:
			if ( length == sizeof( bool ))
			{
				multiplexerManager->setInterleaveMode(*(bool*)data);
			}
			break;
		case camerasDisplayPanel_grid:
			if ( length == sizeof(int) ) {
				if ( task == kofxGui_Set_Grid_Released ) {
					//! Refresh the grid control
					devGrid->setImages();
				} else if ( task == kofxGui_Set_Int ) {
					rawCamId = *(int*)data;
					//updateInfoPanel( rawCamId );
				}
			}
			break;
		case camerasDisplayPanel_grid_reset:
			if ( length == sizeof( bool ) && task == kofxGui_Set_Bool ) {
				if ( *(bool*)data ) 
				{
					int rawIndex = multiplexerManager->getIndexOfCameraBase(multiplexerManager->getCameraBaseAtPosition(camsGrid->getSelectedId()));
					multiplexerManager->removeCameraBaseFromMultiplexerAtPosition(camsGrid->getSelectedId());
					//if (rawIndex>=0)
					//	camsGrid->isSelected[rawIndex] = false;
					//utils->setSelected( utils->getRawId( utils->getCam( camsGrid->getSelectedId() ) ), true );
					//utils->setCam( camsGrid->getSelectedId(), NULL );
					devGrid->setImages();
					camsGrid->setImages();
					camsGrid->removeControls();
				}
			}
			break;
		case cameraGridSettingsPanel_x:
			if (bcamera)
			if( length == sizeof(float) ) 
			{
				int gWidth,gHeight;
				multiplexerManager->getCameraGridSize(&gWidth,&gHeight);
				if ( gWidth != (int)*(float*)data ) 
				{
					updateCameraGridSize( (int)(*(float*)data), gHeight );
					if ( multiplexerManager != NULL )
						multiplexerManager->removeAllCameraBasesFromMultiplexer();
					if ( camsGrid != NULL ) 
						camsGrid->resetAll();
					if ( devGrid != NULL )
						devGrid->setImages();
					if ( camsGrid != NULL ) 
						camsGrid->setImages();
				}
			}
			break;
		case cameraGridSettingsPanel_y:
			if (bcamera)
			if( length == sizeof(float) ) 
			{
				int gWidth,gHeight;
				multiplexerManager->getCameraGridSize(&gWidth,&gHeight);
				if ( gHeight != (int)*(float*)data ) 
				{
					updateCameraGridSize(gWidth, (int)(*(float*)data) );
					if ( multiplexerManager != NULL )
						multiplexerManager->removeAllCameraBasesFromMultiplexer();
					if ( camsGrid != NULL ) 
						camsGrid->resetAll();
					if ( devGrid != NULL )
						devGrid->setImages();
					if ( camsGrid != NULL ) 
						camsGrid->setImages();
				}
			}
			break;
		case calibrationGridSettingsPanel_x:
			if (bcamera) 
			if( length == sizeof(float) ) 
			{
				int gWidth,gHeight;
				multiplexerManager->getCalibrationGridSize(&gWidth,&gHeight);
				if ( gWidth != (int)*(float*)data )
				{
					updateCalibrationGridSize( (int)(*(float*)data), gHeight );
				}
			}
			break;
		case calibrationGridSettingsPanel_y:
			if (bcamera)
			if( length == sizeof(float) ) 
			{
				int gWidth,gHeight;
				multiplexerManager->getCalibrationGridSize(&gWidth,&gHeight);
				if ( gHeight != (int)*(float*)data ) 
				{
					updateCalibrationGridSize( gWidth, (int)(*(float*)data) );
				}
			}
			break;
		case generalSettingsPanel_reset_all:
			if ( length == sizeof( bool ) ) 
			{
				if ( *(bool*)data ) 
				{
					if ( multiplexerManager != NULL )
						multiplexerManager->removeAllCameraBasesFromMultiplexer();
					if ( camsGrid != NULL ) 
						camsGrid->resetAll();
					if ( devGrid != NULL )
						devGrid->setImages();
					if ( camsGrid != NULL ) 
						camsGrid->setImages();
				}
			}
			break;
		case generalSettingsPanel_apply_settings:
			if (bcamera)
				multiplexerManager->applySettingsToMultiplexer();
			break;
		case devicesListPanel_arrow_left:
			if ( length == sizeof(bool) ) 
			{
				if (*(bool*)data) 
					devGrid->previous();
			}
			break;
		case devicesListPanel_arrow_right:
			if ( length == sizeof(bool) ) 
			{
				if (*(bool*)data)
					devGrid->next();
			}
			break;
		case camerasDisplayPanel_grid_setting:
			if ( length == sizeof( bool ) && task == kofxGui_Set_Bool ) {
				if ( *(bool*)data ) 
				{
					ofxCameraBase* camera = multiplexerManager->getCameraBaseAtPosition(camsGrid->getSelectedId());
					if (camera!=NULL)
						camera->callSettingsDialog();
				}
			}
			break;
		case devicesListPanel_grid:
			if ( length == sizeof(int) ) 
			{
				if ( task == kofxGui_Set_Grid_Dragging ) 
				{
					int index = *(int*)data;
					//! the camera index must less than camera count
					if ( index < multiplexerManager->getCameraBaseCount() ) 
					{
						if ( bDraggingImage == false ) 
						{
							if (draggingImage!=NULL)
							{
								delete draggingImage;
								draggingImage = new ofxGuiImage();
							}
							bDraggingImage = true;
							unsigned int gWidth,gHeight;
							unsigned char gDepth,gPixel;
							ofxCameraBase* camera = multiplexerManager->getCameraBase( index );
							if (camera!=NULL)
							{
								draggingImage->init( dragging_image, "", 100, 100, devGrid->getGridWidth(), devGrid->getGridHeight() );
								draggingImage->setCamera(camera);
								draggingXOffset = devGrid->getDraggingXOffset();
								draggingYOffset = devGrid->getDraggingYOffset();
								draggingImage->draw();
								camsGrid->setDraggingRawIndex( index );
							}
						}
					} 
					else 
						bDraggingImage = false;
				} 
				else if ( task == kofxGui_Set_Int ) 
				{
					rawCamId = *(int*)data;
//					updateInfoPanel( rawCamId );

				}
			}
			break;
			//! Show previous camera.
		case trackedPanel_thresholdDynamic:
			if(length == sizeof(bool))
			{
				filter->bDynamicTH = *(bool*)data;
			}
			break;
		case calibrationPanel_multicam:
			if (bcamera)
				if (!bCalibration)
				{
					if(length == sizeof(bool))
						bMulticamDialog = *(bool*)data;
					switchMultiCamsGUI(true);
				}
			break;
		case calibrationPanel_tracker:
			if (bcamera)
				if (!bCalibration)
				{
					if(length == sizeof(bool))
						bMulticamDialog = *(bool*)data;
					switchMultiCamsGUI(false);
				}
			break;
		case calibrationPanel_calibrate:
			if (!bCalibration)
			{
				if (!bcamera)
					return;
				ofSetFrameRate(40);	//BUG : when framerate is high - calibration is not stable		
				bShowInterface = false;
				// Enter/Exit Calibration
				bCalibration = true;
				calib.calibrating = true;
				tracker.isCalibrating = true;
				startCalibrating = true;
				multiplexerManager->applySettingsToMultiplexer();
				multiplexerManager->initializeCalibration();
				if (bFullscreen == false) ofToggleFullscreen();
				bFullscreen = true;
			}
			break; 
		case trackedPanel_normalizing_on:
			if(length == sizeof(bool))
				filter->isNormalizingOn = *(bool*)data;
			break;
		//Source

		case propertiesPanel_flipH:
			if(length == sizeof(bool))
				filter->bHorizontalMirror = *(bool*)data;
				filter_fiducial->bHorizontalMirror = *(bool*)data;
			break;
		case propertiesPanel_flipV:
			if(length == sizeof(bool))
				filter->bVerticalMirror = *(bool*)data;
				filter_fiducial->bVerticalMirror = *(bool*)data;
			break;

		//Tracking Panel
		case trackingPanel_trackFingers:
			if(length == sizeof(bool))
				contourFinder.bTrackFingers=*(bool*)data;
			break;
		case trackingPanel_trackObjects:
			if(length == sizeof(bool))
			{
				if (contourFinder.bTrackObjects!=*(bool*)data)
				{
					if (*(bool*)data)
					{
						ofxGuiPanel* panel2x = controls->addPanel(appPtr->templatePanel, "Templates", MAIN_PANEL_SECOND_X, 455 - (bcamera ? 0 : 80), OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
						panel2x->addButton(appPtr->kParameter_SaveTemplateXml, "Save Template" , OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Trigger);
						panel2x->addButton(appPtr->kParameter_LoadTemplateXml, "Load Template", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Trigger);
						panel2x->addSlider(appPtr->TemplatePanel_minArea, "Min Area", MAIN_FILTERS_Y, 13, 0.0f, 2000.0f, minTempArea, kofxGui_Display_Int, 0);
						panel2x->addSlider(appPtr->TemplatePanel_maxArea, "Max Area", MAIN_FILTERS_Y, 13, 0.0f, 2000.0f, maxTempArea, kofxGui_Display_Int, 0);
						panel2x->mObjWidth = 200;
						panel2x->mObjHeight = 140;
						panel2x->mObjects[2]->mObjY = 75;
						panel2x->mObjects[3]->mObjY = 105;
						panel2x->adjustToNewContent(120, 0);
					}
					else
					{
						controls->removePanel( this->templatePanel );
					}
				}
				contourFinder.bTrackObjects=*(bool*)data;
				if(contourFinder.bTrackObjects)
				{
					templates.loadTemplateXml();
				}
				else
				{
					templates.saveTemplateXml();
				}
			}
			break;
		case trackingPanel_trackFiducials:
			if(length == sizeof(bool))
			{
				contourFinder.bTrackFiducials=*(bool*)data;
				bFidMode = *(bool*)data;
			}
			break;
		//Communication
		case optionPanel_win_hid:
			if(length == sizeof(bool))
				bWinTouch = *(bool*)data;
			break;
		case optionPanel_tuio_osc:
			if(length == sizeof(bool))
			{
				myTUIO.bOSCMode = *(bool*)data;
				bTUIOMode = myTUIO.bOSCMode;
				controls->update(appPtr->optionPanel_tuio_osc, kofxGui_Set_Bool, &appPtr->myTUIO.bOSCMode, sizeof(bool));
				if (*(bool*)data)
				{
					myTUIO.bTCPMode = false;
					myTUIO.bBinaryMode = false;
					controls->update(appPtr->optionPanel_tuio_tcp, kofxGui_Set_Bool, &appPtr->myTUIO.bTCPMode, sizeof(bool));
					controls->update(appPtr->optionPanel_bin_tcp, kofxGui_Set_Bool, &appPtr->myTUIO.bBinaryMode, sizeof(bool));
				}
			}
			break;
		case optionPanel_tuio_tcp:
			if(length == sizeof(bool))
			{
				myTUIO.bTCPMode = *(bool*)data;
				bTUIOMode = myTUIO.bTCPMode;
				controls->update(appPtr->optionPanel_tuio_tcp , kofxGui_Set_Bool, &appPtr->myTUIO.bTCPMode, sizeof(bool));
				if (*(bool*)data)
				{
					myTUIO.bOSCMode = false;
					myTUIO.bBinaryMode = false;
					controls->update(appPtr->optionPanel_tuio_osc, kofxGui_Set_Bool, &appPtr->myTUIO.bOSCMode, sizeof(bool));
					controls->update(appPtr->optionPanel_bin_tcp, kofxGui_Set_Bool, &appPtr->myTUIO.bBinaryMode, sizeof(bool));
				}
			}
			break;
		case optionPanel_bin_tcp:
			if(length == sizeof(bool))
			{
				myTUIO.bBinaryMode = *(bool*)data;
				bTUIOMode = myTUIO.bBinaryMode;
				controls->update(appPtr->optionPanel_bin_tcp , kofxGui_Set_Bool, &appPtr->myTUIO.bBinaryMode, sizeof(bool));
				if (*(bool*)data)
				{
					myTUIO.bOSCMode = false;
					myTUIO.bTCPMode = false;
					controls->update(appPtr->optionPanel_tuio_osc, kofxGui_Set_Bool, &appPtr->myTUIO.bOSCMode, sizeof(bool));
					controls->update(appPtr->optionPanel_tuio_tcp, kofxGui_Set_Bool, &appPtr->myTUIO.bTCPMode, sizeof(bool));
				}
			}
			//clear blobs
//				myTUIO.blobs.clear();
			break;
// 		case optionPanel_tuio_height_width:
// 			if(length == sizeof(bool))
// 				myTUIO.bHeightWidth = *(bool*)data;
// 			break;
		//Background
		case backgroundPanel_dynamic:
			if(length == sizeof(bool))
				filter->bDynamicBG = *(bool*)data;
				filter_fiducial->bDynamicBG = *(bool*)data;
			break;
		case backgroundPanel_remove:
			if(length == sizeof(bool))
				filter->bLearnBakground = *(bool*)data;
				filter_fiducial->bLearnBakground = *(bool*)data;
			break;
		case backgroundPanel_learn_rate:
			if(length == sizeof(float))
				backgroundLearnRate = *(float*)data;
			break;
		//Highpass
		case highpassPanel_use:
			if(length == sizeof(bool))
			{
				if(!bFidMode)
				{
					filter->bHighpass = *(bool*)data;
				}
				else
				{
					filter_fiducial->bHighpass = *(bool*)data;
				}
			}
			break;
		case highpassPanel_blur:
			if(length == sizeof(float))
			{
				if(!bFidMode)
				{
					filter->highpassBlur = *(float*)data;
				}
				else
				{
					filter_fiducial->highpassBlur = *(float*)data;
				}
			}
			break;
		case highpassPanel_noise:
			if(length == sizeof(float))
			{
				if(!bFidMode)
				{
					filter->highpassNoise = *(float*)data;
				}
				else
				{
					filter_fiducial->highpassNoise = *(float*)data;
				}
			}
			break;
		//Amplify
		case amplifyPanel_use:
			if(length == sizeof(bool))
			{
				if(!bFidMode)
				{
					filter->bAmplify = *(bool*)data;
				}
				else
				{
					filter_fiducial->bAmplify = *(bool*)data;
				}
			}
			break;
		case amplifyPanel_amp:
			if(length == sizeof(float))
			{
				if(!bFidMode)
				{
					filter->highpassAmp = *(float*)data;
				}
				else
				{
					filter_fiducial->highpassAmp = *(float*)data;
				}
			}
			break;
			
		case trackedPanel_low_normalizing:
			if(length == sizeof(float))
			{
				filter->normalizingLowLevel = *(float*)data;
			}
			break;
		case trackedPanel_high_normalizing:
			if(length == sizeof(float))
			{
				filter->normalizingHighLevel = *(float*)data;
			}
			break;
		case trackedPanel_darkblobs:
			if(length == sizeof(bool))
			{
				if(!bFidMode)
				{
					filter->bTrackDark = *(bool*)data;
				}
				else
				{
					filter_fiducial->bTrackDark = *(bool*)data;
				}
			}
			break;
		case trackedPanel_threshold:
			if(length == sizeof(float))
			{
				if(!bFidMode)
				{
					filter->threshold = *(float*)data;
				}
				else
				{
					filter_fiducial->threshold = *(float*)data;
				}
			}
			break;
		case trackedPanel_min_movement:
			if(length == sizeof(float))
				tracker.MOVEMENT_FILTERING = *(float*)data;
			break;
		case trackedPanel_min_blob_size:
			if(length == sizeof(float))
				MIN_BLOB_SIZE = *(float*)data;
			break;
		case trackedPanel_max_blob_size:
			if(length == sizeof(float))
				MAX_BLOB_SIZE = *(float*)data;
			break;
		case trackedPanel_outlines:
			if(length == sizeof(bool))
				bDrawOutlines = *(bool*)data;
			break;
		case trackedPanel_ids:
			if(length == sizeof(bool))
				bShowLabels = *(bool*)data;
			break;
		//smooth
		case smoothPanel_smooth:
			if(length == sizeof(float))
			{
				if(!bFidMode)
				{
					filter->smooth = *(float*)data;
				}
				else
				{
					filter_fiducial->smooth = *(float*)data;
				}
			}
			break;
		case smoothPanel_use:
			if(length == sizeof(bool))
			{
				if(!bFidMode)
				{
					filter->bSmooth = *(bool*)data;
				}
				else
				{
					filter_fiducial->bSmooth = *(bool*)data;
				}
			}
			break;
		//Template Area Sliders
		case TemplatePanel_minArea:
			if(length == sizeof(float))
			{
				minTempArea = *(float*)data;
				float smallArea = rect.height*rect.width-minTempArea; //The area of the small rectangle
				float _w = sqrt(smallArea*rect.width/rect.height); // Width of small rectangle, as the width and height
																   //will be proportional to the original rectangle
				float _h = sqrt(smallArea*rect.height/rect.width);
				minRect.x =rect.x + (rect.width - _w)/2 ;
				minRect.y = rect.y + (rect.height - _h)/2 ;
				minRect.width = _w ;
				minRect.height = _h ;
			}
			break;
		case TemplatePanel_maxArea:
			if(length == sizeof(float))
			{
				maxTempArea = *(float*)data;
				float bigArea = rect.height*rect.width+maxTempArea; //The area of the big rectangle
				float _w = sqrt(bigArea*rect.width/rect.height); // Width of big rectangle, as the width and height
																 //will be proportional to the original rectangle
				float _h = sqrt(bigArea*rect.height/rect.width);
				maxRect.x =rect.x - (_w - rect.width)/2 ;
				maxRect.y = rect.y - (_h - rect.height)/2 ;
				maxRect.width = _w ;
				maxRect.height = _h ;
			}
			break;
		//Save Settings
		case kParameter_SaveXml:
			if(length == sizeof(bool))
			{
				if(*(bool*)data)
				{
					controls->saveToXml(OFXGUI_XML);
					saveSettings();
				}
			}
			break;
		case kParameter_LoadTemplateXml:
			if(length == sizeof(bool))
			{
				if(*(bool*)data)
				{
					if(templates.loadTemplateXml())
					{
						printf("Templates Loaded\n");
					}
				}
			}
			break;
		case kParameter_SaveTemplateXml:
			if(length == sizeof(bool))
			{
				if(*(bool*)data)
				{
					templates.saveTemplateXml();
				}
			}
			break;
	}
}
#endif //GUI_CONTROLS_H

