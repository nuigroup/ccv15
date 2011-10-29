#ifndef CAMERAUTILS_H_
#define CAMERAUTILS_H_


#ifdef OF_VIDEO_CAPTURE_QUICKTIME
	#include "ofQtUtils.h"
#endif

#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	#include "videoInput.h"
#endif

#ifdef OF_VIDEO_CAPTURE_UNICAP
	#include "ofUCUtils.h"
#endif

#ifdef OF_VIDEO_CAPTURE_V4L
	#include "ofV4LUtils.h"
#endif


//--------------------------------------------------------------------
class CameraUtils {

	public: 

        CameraUtils(){}

		int getDeviceCount(){

			//---------------------------------
			#ifdef OF_VIDEO_CAPTURE_QUICKTIME
			//---------------------------------

				SGDeviceList deviceList;
				SGGetChannelDeviceList (gVideoChannel, sgDeviceListIncludeInputs, &deviceList);
				int numDevices = (*deviceList)->count;
				return numDevices;
			//---------------------------------
			#endif
			//---------------------------------
			#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
			//---------------------------------
				return VI.devicesFound;

			//---------------------------------
			#endif
			//---------------------------------
			#ifdef OF_VIDEO_CAPTURE_UNICAP
			//--------------------------------

				int status = STATUS_SUCCESS;
				int dev_count = 0;
				unicap_device_t devices[MAX_DEVICES];
				for (dev_count = 0; SUCCESS (status) && (dev_count < MAX_DEVICES); dev_count++) {
				   status = unicap_enumerate_devices (NULL, &devices[dev_count], dev_count);
				}
				return dev_count - 1;

   			//---------------------------------
			#endif
			//---------------------------------
		}

	protected:

		//--------------------------------- quicktime
		#ifdef OF_VIDEO_CAPTURE_QUICKTIME
			SGChannel 			gVideoChannel;
		#endif
		//--------------------------------- directshow
		#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
			videoInput			VI;
		#endif		
};
#endif