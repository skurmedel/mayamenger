#include <stdio.h>
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MFnPlugin.h>
#include <maya/MIOStream.h>
#include <maya/MGlobal.h>

#include <maya/MFnMesh.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatPointArray.h>

#include "mengercmd.hpp"
#include "mengerfun.hpp"

#ifdef WIN_64
#define FUN_EXPORT __declspec(dllexport)
#else
#define FUN_EXPORT
#endif 

static void makeCube(MStatus *stat)
{
    MFnMesh fnMesh;
    MObject result;

    MFloatPointArray points;

    int vtx_cnt = 0;
    for (int z = 0; z < 2; ++z)
    {
        for (int y = 0; y < 2; ++y)
        {
            for (int x = 0; x < 2; ++x)
            {
                points.append(MFloatPoint( (float) x, (float) y, (float) z ));
                vtx_cnt++;
            }
        }
    }
    cout << "vtx_cnt: " << vtx_cnt << endl;

    int const face_cnt = 6;
    int pts_per_face[face_cnt] = { 4, 4, 4, 4, 4, 4 };
    MIntArray faceCounts(pts_per_face, face_cnt);

    int face_connects[face_cnt * 4] =
    {  0, 1, 2, 3,
        4, 5, 6, 7,
        3, 2, 6, 5,
        0, 3, 5, 4,
        0, 4, 7, 1,
        1, 7, 6, 2  };
    MIntArray faceConnects(face_connects, face_cnt * 4);

    MObject newMesh = 
        fnMesh.create(
            /* numVertices */ vtx_cnt, 
            /* numFaces */ face_cnt, 
            points, 
            faceCounts, 
            faceConnects, 
            MObject::kNullObj, 
            stat);
}

mengerCmd::mengerCmd() 
{
    
}

mengerCmd::~mengerCmd() 
{
    
}

MStatus mengerCmd::doIt( const MArgList& ) 
{
    MStatus stat = MStatus::kSuccess;
    makeCube(&stat);

    if (stat == MStatus::kSuccess)
        MGlobal::displayInfo("YAY!");
    else
        MGlobal::displayInfo("Shit.");

    return stat;
}

MStatus mengerCmd::redoIt() 
{
    return MS::kSuccess;
}

MStatus mengerCmd::undoIt() 
{
    return MS::kSuccess;
}

bool mengerCmd::isUndoable() const 
{
    return true;
}

void* mengerCmd::creator() 
{
    return new mengerCmd();
}

#define INIT_INFO_TEXT PLUGIN_NAME " " PLUGIN_VER " registered."

FUN_EXPORT MStatus initializePlugin(MObject obj)
{
    MFnPlugin plugin(obj, PLUGIN_NAME, PLUGIN_VER, "Any");
    plugin.registerCommand(PLUGIN_CMD, mengerCmd::creator);
    
    MGlobal::displayInfo(INIT_INFO_TEXT);
    
    return MS::kSuccess;
}

FUN_EXPORT MStatus uninitializePlugin(MObject obj)
{
    MFnPlugin plugin( obj );

    plugin.deregisterCommand(PLUGIN_CMD);
    
    return MS::kSuccess;
}