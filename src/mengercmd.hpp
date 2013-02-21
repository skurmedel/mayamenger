#ifndef MENGER_CMD_HPP
#define MENGER_CMD_HPP

#define PLUGIN_CMD		"createMenger"
#define PLUGIN_NAME 	"MengerCommand"
#define PLUGIN_VER		"0.1"

#include <maya/MPxCommand.h>

class mengerCmd : public MPxCommand
{
    public:
        mengerCmd();
        virtual ~mengerCmd();
        MStatus doIt( const MArgList& );
        MStatus redoIt();
        MStatus undoIt();
        bool isUndoable() const;
        static void* creator();
};

#endif