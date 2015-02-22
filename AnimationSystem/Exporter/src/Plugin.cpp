// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 20/02/2015
// a helpful resource : http://nccastaff.bournemouth.ac.uk/jmacey/RobTheBloke/www/research/maya/exp_plugin.htm
#include "PGAGExporter.h"
#include <maya/MFnPlugin.h>
#define MLL_EXPORT __declspec(dllexport)

MLL_EXPORT MStatus initializePlugin(MObject obj)
{
	// initialise the plugin
	MFnPlugin plugin(obj, "PGAG Animation Mesh Exporter", "1.0", "Any");

	// plugin only registers the file translator
	return plugin.registerFileTranslator("PGAG Mesh Exporter", 0, PGAGExporter::create);
}

MLL_EXPORT MStatus uninitializePlugin(MObject obj)
{
	// deregister the file translator
	MFnPlugin plugin(obj);
	return plugin.deregisterFileTranslator("PGAG Mesh Exporter");
}
