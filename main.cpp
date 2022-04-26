#include <QCoreApplication>
#include <QDebug>

#include "UnrealLiveLinkCInterfaceAPI.h"
#ifdef WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <time.h>
#include <math.h>

/* number of circles to perform */
#define CIRCLES 300

/* size in Unreal units of the circle on the X-Y plane */
#define CIRCLE_RADIUS 200

/* number of steps to perform the circles */
#define STEP_COUNT 100000

/* Z axis location -- number of units above origin */
#define HEIGHT 100

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);
    qDebug() << "Hello world";

    int i;
    int rc;
    time_t t;
    double angle = 0.0;
    double worldTime = 0.0;

#ifdef WIN32
    qDebug() << "WIN32";
    const wchar_t *sharedObj =L"UnrealLiveLinkCInterface.dll";
//    const char * sharedObj = "UnrealLiveLinkCInterface.dll";
#else
    const char * sharedObj = "UnrealLiveLinkCInterface.so";
    struct timespec ts;

    ts.tv_sec = 0;
    ts.tv_nsec = 16 * 1000000;
#endif

    rc = UnrealLiveLink_Load(sharedObj, "CirclingTransform");
    qDebug()<<"Error" <<rc;
    if (rc != UNREAL_LIVE_LINK_OK)
    {
        printf("error: unable to load %s\n", sharedObj);
        return 1;
    }

    printf("Starting...\n");
    t = time(NULL);

    /* set up transform live link role */
    UnrealLiveLink_SetTransformStructure("circle", 0);

    /* initialize the per frame transform structure */
    struct UnrealLiveLink_Transform xform;
    UnrealLiveLink_InitTransform(&xform);
    xform.translation[2] = HEIGHT;

    /* calculate the radian step */
    const double step = 2.0 * M_PI * CIRCLES / STEP_COUNT;

    /* loop STEP_COUNT times sending data every 16ms (not exact) */
    for (i = 0; i < STEP_COUNT; i++)
    {
        xform.translation[0] = (float)sin(angle) * CIRCLE_RADIUS;
        xform.translation[1] = (float)cos(angle) * CIRCLE_RADIUS;

        UnrealLiveLink_UpdateTransformFrame("circle", worldTime, 0, 0, &xform);

        angle += step;

        /* sleep 16ms */
#ifdef WIN32
        Sleep(16);
#else
        nanosleep(&ts, &ts);
#endif

        worldTime += 16.0;
    }

    printf("Done. Took %lld seconds.\n", time(NULL) - t);
    qDebug() << "Try to unload";
    UnrealLiveLink_Unload();
    qDebug() << "Get here";
//    return
//    return a.exec();
}
