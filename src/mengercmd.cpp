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

#ifdef WIN64
#define FUN_EXPORT __declspec(dllexport)
#else
#define FUN_EXPORT
#endif 

static void addCube(
    float scale, 
    MFloatVector pos,
    int index_offset,
    MFloatPointArray &points,
    MIntArray &faceCounts,
    MIntArray &faceConnects)
{
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

    for (int i = 0; i < 6; i++)
        faceCounts.append(4);

    int face_connects[6 * 4] =
    {  
        0, 1, 2, 3,
        7, 6, 5, 4,
        3, 7, 4, 0,
        2, 1, 5, 6,
        0, 4, 5, 1,
        2, 6, 7, 3  
    };
    for (int i = 0; i < 6 * 4; i++)
    {
        faceConnects.append(face_connects[i] + index_offset);
    }
}

static void makeCubes(std::vector<cube> &cubes, MStatus *stat)
{
    MFnMesh fnMesh;
    MObject result;

    MFloatPointArray points;
    MIntArray faceCounts;
    MIntArray faceConnects;

    int index_offset = 0;
    for (std::vector<cube>::iterator cit = cubes.begin(); cit != cubes.end(); ++cit)
    {
        point3 diag = cit->diagonal();
        float scale = diag.x;
        point3 pos = cit->start + (diag * .5f);

        MFloatVector mpos(pos.x, pos.y, pos.z);

        addCube(scale, mpos, index_offset * (8), points, faceCounts, faceConnects);
        index_offset += 1;
    }

    unsigned int vtx_cnt = points.length();
    unsigned int face_cnt = faceCounts.length();

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

static void makeCube(int index_offset, float scale, MFloatVector pos, MStatus *stat)
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
        dice(c, 4, cubes);

    if (dice_status == dice_err::success)
    {
        MGlobal::displayInfo("YAY!");

        makeCubes(cubes, &stat);
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