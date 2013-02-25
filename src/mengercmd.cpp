#include <stdio.h>
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MFnPlugin.h>
#include <maya/MIOStream.h>
#include <maya/MGlobal.h>

#include <maya/MFnMesh.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatVector.h>
#include <maya/MFloatPointArray.h>

#include "mengercmd.hpp"
#include "mengerfun.hpp"

#ifdef WIN_64
#define FUN_EXPORT __declspec(dllexport)
#else
#define FUN_EXPORT
#endif 

static void makeCube(float scale, MFloatVector pos, MStatus *stat)
{
    MFnMesh fnMesh;
    MObject result;

    MFloatPointArray points;

    int vtx_cnt = 8;
    #define mkpoint(x, y, z) points.append((MFloatPoint(x, y, z) * scale) + pos)
    
    mkpoint( 0.5,   0.5,   0.5);
    mkpoint(-0.5,  0.5,   0.5);
    mkpoint(-0.5, -0.5,   0.5);
    mkpoint( 0.5,  -0.5,  0.5);

    mkpoint( 0.5,   0.5,   -0.5);
    mkpoint(-0.5,  0.5,   -0.5);
    mkpoint(-0.5, -0.5,   -0.5);
    mkpoint( 0.5,  -0.5,  -0.5);

    #undef mkpoint

    int const face_cnt = 6;
    int pts_per_face[face_cnt] = { 4, 4, 4, 4, 4, 4 };
    MIntArray faceCounts(pts_per_face, face_cnt);

    int face_connects[face_cnt * 4] =
    {  0, 1, 2, 3,
        7, 6, 5, 4,
        3, 7, 4, 0,
        2, 1, 5, 6,
        0, 4, 5, 1,
        2, 6, 7, 3  };
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
    cube c;
    c.start.x = -10;
    c.start.y = -10;
    c.start.z = -10;
    c.end = c.start * -1;

    std::vector<cube> cubes;
    dice_err::val dice_status = 
        dice(c, 3, cubes);

    if (dice_status == dice_err::success)
    {
        MGlobal::displayInfo("YAY!");

        for (std::vector<cube>::iterator i = cubes.begin(); i != cubes.end(); ++i)
        {
            point3 diag = i->diagonal();
            float scale = diag.x;
            point3 pos = i->start + (diag * .5f);

            makeCube(scale, MFloatVector(pos.x, pos.y, pos.z), &stat);
        }
    }
    else
    {
        MGlobal::displayError("Shit.");
        switch (dice_status)
        {
            case dice_err::invalid_param_err:
                MGlobal::displayError("dice was called with wrong arguments.");
                break;
            case dice_err::bounds_err:
                MGlobal::displayError("dice was called with erronous bounds.");
                break;
            case dice_err::memory_err:
                MGlobal::displayError("dice had an memory error.");
                break;
            case dice_err::success: /* Stop warnings. */
                break;
        }
    }

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