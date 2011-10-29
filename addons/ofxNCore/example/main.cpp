#include "ofMain.h"
#include "testApp.h"

#include <stdio.h>
#include <stdlib.h>

#define LOGIT FALSE

int main( )
{

    //get rid of the console
    #ifdef TARGET_WIN32
 //       FreeConsole();
    #endif

#if LOGIT

     /* alright first we need to get time and date so our logs can be ordered*/
    char dateStr [9];
    char timeStr [9];

    /*time stuff */
    time_t rawtime;
    struct tm * timeinfo;
    char fileName [80];

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    strftime (fileName,80,"logs/log_%B_%d_%y_%H_%M_%S.txt",timeinfo);

    FILE *stream ;
    if((stream = freopen(fileName, "w", stdout)) == NULL)
       exit(-1);

	ofSetupOpenGL(900,900, OF_WINDOW);
	ofRunApp(new testApp());

    stream = freopen("CON", "w", stdout);
#else
	ofSetupOpenGL(900,900, OF_WINDOW);
	ofRunApp(new testApp());
#endif

}
