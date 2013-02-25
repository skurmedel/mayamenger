#include <stdio.h>
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MFnPlugin.h>
#include <maya/MIOStream.h>
#include <maya/MGlobal.h>

#include "mengercmd.hpp"


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
    return MS::kSuccess;
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