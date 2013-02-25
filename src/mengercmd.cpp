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

mengerCmd::mengerCmd() 
{
    
}

mengerCmd::~mengerCmd() 
{
    
}

MStatus mengerCmd::doIt( const MArgList& ) 
{
    MFnMesh fnMesh;
    MObject result;

    MFloatPointArray points;

    points.append(MFloatPoint(-1.0, 1.0, 0.0));
    points.append(MFloatPoint(-1.0, -1.0, 0.0));
    points.append(MFloatPoint(1.0, -1.0, 0.0));
    points.append(MFloatPoint(1.0, 1.0, 0.0));

    int const face_cnt = 1;
    int pts_per_vtx[face_cnt] = { 4 };
    MIntArray faceCounts(pts_per_vtx, face_cnt);

    int face_connects[face_cnt * 4] = { 0, 1, 2, 3 };
    MIntArray faceConnects(face_connects, face_cnt * 4);

    MStatus stat;

    MObject newMesh = 
        fnMesh.create(
            /* numVertices */ 4, 
            /* numFaces */ face_cnt, 
            points, 
            faceCounts, 
            faceConnects, 
            MObject::kNullObj, 
            &stat);

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