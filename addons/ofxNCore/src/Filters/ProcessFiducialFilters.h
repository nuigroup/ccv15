/*
*  ProcessFilters.h
*  
*
*  Created on 2/2/09.
*  Copyright 2009 NUI Group. All rights reserved.
*
*/

#ifndef PROCESS_FIDUCIAL_FILTERS_H_
#define PROCESS_FIDUCIAL_FILTERS_H_

#include "Filters.h"

class ProcessFiducialFilters : public Filters {

  public:

    void allocate( int w, int h ) {

     	thresholder = new TiledBernsenThresholder();
		initialize_tiled_bernsen_thresholder(thresholder,w,h,fiducial_tile_size);
        camWidth = w;
        camHeight = h;
		//initialize learnrate
		fLearnRate = 0.0f;
        bMiniMode = false;

		exposureStartTime = ofGetElapsedTimeMillis();

        //CPU Setup
        grayImg.allocate(camWidth, camHeight);		//Gray Image
        grayDiff.allocate(camWidth, camHeight);		//Difference Image between Background and Source
        grayBg.allocate(camWidth, camHeight);		//Gray Image
   }

    void allocateGPU(){

    }

/****************************************************************
 *	CPU Filters
 ****************************************************************/
     void applyCPUFilters(CPUImageFilter& img)
	 {
        if(bVerticalMirror || bHorizontalMirror) img.mirror(bVerticalMirror, bHorizontalMirror);
		if (showProcessedFrame)
			if(!bMiniMode) grayImg = img; //for drawing
		/*
	    if((ofGetElapsedTimeMillis() - exposureStartTime) < CAMERA_EXPOSURE_TIME) 
			bLearnBakground = true;
		if (bLearnBakground)
		{
            grayBg = img;
			grayBg.flagImageChanged();
            bLearnBakground = false;
        }
		cvSub(img.getCvImage(), grayBg.getCvImage(), img.getCvImage());
		*/
		img.flagImageChanged();
		if (data==NULL)
			data = (unsigned char*)malloc(img.width*img.height*sizeof(unsigned char));
		tiled_bernsen_threshold(thresholder,data,img.getPixels(),1,img.width,img.height,fiducial_tile_size,fiducialThreshold);
		img.setFromPixels(data,img.width,img.height);
   		if (showProcessedFrame)
			if(!bMiniMode)	grayDiff = img; //for drawing
    }
	void updateSettings()
	{
		if (thresholder!=NULL)
			terminate_tiled_bernsen_thresholder(thresholder);
		initialize_tiled_bernsen_thresholder(thresholder,camWidth,camHeight,fiducial_tile_size);
	}

/****************************************************************
 *	GPU Filters
 ****************************************************************/
    void applyGPUFilters(){

    }

/****************************************************************
 *	Draw Filter Images
 ****************************************************************/
    void draw()
    {
        grayImg.draw(250, 25, 326, 246);
        grayDiff.draw(250+335, 25, 326, 246);
    }

    void drawGPU()
    {

    }
};
#endif//PROCESS_FIDUCIAL_FILTERS_H_
