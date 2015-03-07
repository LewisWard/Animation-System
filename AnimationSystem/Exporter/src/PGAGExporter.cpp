// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 20/02/2015
#include <fstream>
#include "PGAGExporter.h"
#include <maya/MItDag.h>
#include <maya/MFnDagNode.h>
#include <maya/MTypes.h>


bool animation::write(const char* filename)
{
	// store filename and add extension 
	std::string file = filename;
	
	// create the file and open it
	std::ofstream ouputFileStream(file);

	// write to file
	ouputFileStream << framesNum << std::endl;
	ouputFileStream << jointsNum << std::endl;

	for (uint32_t f = 0; f < framesNum; f++)
	{
		// get the frame
		extractFrame(f);

		// write to file
		for (uint32_t i = 0; i < m_animation.size(); i++)
		{
			// get the name of the parent path (i.e Hips etc.)
			MString parentPathName(m_animation[i].parentPath.partialPathName());

			uint32_t parentIndex = hipsIndex;

			// make sure there is a parent
			if (parentPathName.numChars())
			{
				for (uint32_t l = 0; l < m_animation.size(); l++)
				{
					if (parentPathName == m_animation[l].path.partialPathName())
					{
						parentIndex = l;
						break;
					}
				}
				// as it has a parent, add joint position to its parent (this could be done within the program, 
				// but is easier to perform here). MAKE SURE FREEZE TRANSFORMS HAS BEEN DONE FOR EXPORT!
				m_animation[i].positions[f].x += m_animation[parentIndex].positions[f].x;
				m_animation[i].positions[f].y += m_animation[parentIndex].positions[f].y;
				m_animation[i].positions[f].z += m_animation[parentIndex].positions[f].z;
			}

			// write results to file
			ouputFileStream << f << " "
											<< m_animation[i].positions[f].x << " "
											<< -m_animation[i].positions[f].y <<" " // need to invert as in the program Y is the otherway round
											<< m_animation[i].positions[f].z << " "
											<< m_animation[i].rotations[f].x << " "
											<< m_animation[i].rotations[f].y << " "
											<< m_animation[i].rotations[f].z << " "
											<< m_animation[i].rotations[f].w << std::endl;
		}
	}

	// close
	ouputFileStream.close();
	
	return true;
}

MStatus PGAGExporter::writer(const MFileObject& file, const MString& /*optionsString*/, FileAccessMode /*mode*/)
{
	// using an iterator, iterate over each transform in the scene
	MItDag it(MItDag::kDepthFirst, MFn::kDagNode);

	MDagPathArray bones;

	while (!it.isDone())
	{
		// grab a path for the node, this will allow for instancing
		MDagPath path;
		it.getPath(path);

		// attach the function set to the object
		MFnTransform fn(it.item());

		// initialise to get the node
		MFnDagNode dagNode(path);

		// check to see if this node is derived from a transform
		if (path.hasFn(MFn::kTransform))
		{
			// ignore and skip unwanted nodes
			if (dagNode.name() == "groundPlane_transform" ||
					dagNode.name() == "Manipulator1" ||
					dagNode.name() == "UniversalManip" ||
					dagNode.name() == "CubeCompass" ||
					dagNode.name() == "persp" ||
					dagNode.name() == "top" ||
					dagNode.name() == "front" ||
					dagNode.name() == "side")
			{
				// skip this node, and its children
				it.prune();
			}
			else
			{
			}
		}
		else
		{
			// make sure the node is an intermediate object (i.e not drawn but used for calculations)
			if (!dagNode.isIntermediateObject())
			{
				// mesh node
				if (path.hasFn(MFn::kMesh))
				{

				}
			}
		}

		if (path.hasFn(MFn::kJoint))
		{
			// store the mesh to be exported later
			bones.append(path);
		}

		// move iterator onto the next node in the scene 
		it.next();
	}

	// get start and end time
	MTime start = MAnimControl::animationStartTime();
	MTime end = MAnimControl::animationEndTime();

	// work out total number of frames
	int32_t totalFrames = (end.value() - start.value() + 1);

	// create new animation
	m_animation = new animation(totalFrames, bones);

	// export data
	//exportMeshes(file.fullName(), it.item());
	MStatus status = m_animation->write(file.fullName().asChar()) ? MS::kSuccess : MS::kFailure;

	// clean up
	delete m_animation;

	// return that export has been successful
	return MS::kSuccess;
}

bool PGAGExporter::haveWriteMethod() const
{
	// allows writing of files
	return true;
}

MString PGAGExporter::defaultExtension() const
{
	// file extension
	return "amesh";
}

MString PGAGExporter::filter() const
{
	// file extension
	return "*.amesh";
}
