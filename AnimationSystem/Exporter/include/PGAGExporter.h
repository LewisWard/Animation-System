// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 20/02/2015
// Maya plugin bases upon 'AnimExporter', 'AnimationBlending' by Rob Bateman and resources found at:
// http://nccastaff.bournemouth.ac.uk/jmacey/RobTheBloke/www/research/index.htm
// also the Maya API Reference Documentation : http://download.autodesk.com/us/maya/2009help/API/main.html
#pragma once 
#include <cstdint>
#include <maya/MPxFileTranslator.h>
#include <maya/MItDag.h>
#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MFileObject.h>
#include <maya/MDagPathArray.h>
#include <maya/MFnIkJoint.h>
#include <maya/MQuaternion.h>
#include <maya/MMatrix.h>
#include <maya/MAnimControl.h>
#include <maya/MFnSet.h>
#include <maya/MFnWeightGeometryFilter.h>
#include <maya/MSelectionList.h>
#include <maya/MItGeometry.h>
#include <maya/MFloatArray.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <vector>

//----------------------------------------------------------------------------------------------------------------------
/// \brief 3D vector
//----------------------------------------------------------------------------------------------------------------------
struct vec3
{
	float x;
	float y;
	float z;
};

//----------------------------------------------------------------------------------------------------------------------
/// \brief Quaternion
//----------------------------------------------------------------------------------------------------------------------
struct quat
{
	float x;
	float y;
	float z;
	float w;

	// negate all components
	void negate()
	{
		x = -x;
		y = -y;
		z = -z;
		w = -w;
	}
};

//----------------------------------------------------------------------------------------------------------------------
/// \brief Stores a single animation channel of positions and rotations
//----------------------------------------------------------------------------------------------------------------------
struct jointAnimation
{
	MDagPath path;
	MDagPath parentPath;
	std::vector<vec3> positions;
	std::vector<quat> rotations;

	// initialise the joint animation
	void init(MDagPath p)
	{
		parentPath = path = p;
		parentPath.pop();
	}

	// grab a frame of animation
	void extractHipsFrame(int frame, const MDagPath& trajectory)
	{
		// update the current time to the frame we want
		MAnimControl::setCurrentTime(MTime(frame, MTime::kPALFrame));

		// get matrix data
		MMatrix inclusiveMat = path.inclusiveMatrix();
		MMatrix inclusiveMatInverse = trajectory.inclusiveMatrixInverse();
		MMatrix combinedMat = inclusiveMat * inclusiveMatInverse;

		// store local space translation
		positions[frame].x = float(combinedMat[3][0]);
		positions[frame].y = float(combinedMat[3][1]);
		positions[frame].z = float(combinedMat[3][2]);

		// remove translation from matrix
		combinedMat[3][2] = combinedMat[3][1] = combinedMat[3][0] = 0;

		// convert the matrix to a quat
		MQuaternion rotation;
		rotation = combinedMat;

		// store local space rotation
		rotations[frame].x = float(rotation.x);
		rotations[frame].y = float(rotation.y);
		rotations[frame].z = float(rotation.z);
		rotations[frame].w = float(rotation.w);
	}

	/// grab a frame of animation
	void extractFrame(int frame)
	{
		// update the current time to the frame we want
		MAnimControl::setCurrentTime(MTime(frame, MTime::kPALFrame));

		MMatrix inclusiveMat = path.inclusiveMatrix();
		MMatrix inclusiveMatInverse = parentPath.inclusiveMatrixInverse();
		MMatrix combinedMat = inclusiveMat * inclusiveMatInverse;
		
		// store local space translation
		positions[frame].x = float(combinedMat[3][0]);
		positions[frame].y = float(combinedMat[3][1]);
		positions[frame].z = float(combinedMat[3][2]);
		
		// remove translation from matrix
		combinedMat[3][2] = combinedMat[3][1] = combinedMat[3][0] = 0;
		
		// convert the matrix to a quat 
		MQuaternion rotation;
		rotation = combinedMat;
		
		// store local space rotation
		rotations[frame].x = float(rotation.x);
		rotations[frame].y = float(rotation.y);
		rotations[frame].z = float(rotation.z);
		rotations[frame].w = float(rotation.w);
	}
};
//----------------------------------------------------------------------------------------------------------------------
/// \brief Stores all animation data for a single joint
//----------------------------------------------------------------------------------------------------------------------
struct animation
{
	animation(uint32_t frames, const MDagPathArray& paths) : m_framesNum(frames), m_jointsNum(paths.length())
	{
		// copy and store all the objects in teh scene
		m_bones = paths;

		// resize the vector
		m_animation.resize(m_jointsNum);

		// cycles the joints
		for (uint32_t i = 0; i < m_jointsNum; ++i)
		{
			// initialise the joint and resize its position/rotation vectors
			m_animation[i].init(paths[i]);
			m_animation[i].positions.resize(m_framesNum);
			m_animation[i].rotations.resize(m_framesNum);

			// identify the trajectory and hip joints
			if (paths[i].partialPathName() == "Hips")
			{
				hips = paths[i];
				hipsIndex = i;
			}
			else
			if (paths[i].partialPathName() == "Trajectory")
			{
				trajectory = paths[i];
				trajectoryIndex = i;
			}
		}
	}

	// extract the next frame of animation from maya 
	void extractFrame(int frame)
	{
		// cycle the joints
		for (uint32_t i = 0; i < m_jointsNum; ++i)
		{
			m_animation[i].extractFrame(frame);
		}

		// reparent the hips to the trajectory node
		// NOTE : if no joint is called Trajectory, the plugin will crash!!
		m_animation[hipsIndex].extractHipsFrame(frame, trajectory);

		// copy the trajectory into the root transform
		m_animation[0].positions[frame] = m_animation[trajectoryIndex].positions[frame];
		m_animation[0].rotations[frame] = m_animation[trajectoryIndex].rotations[frame];
	}
	
	// take all the objects/data from the scene before writing it to file
	void preWrite();

	// write the ASCII animation data to file
	bool write(const char* filename);

	struct vertexData
	{
		vec3 vertex;
		vec3 normal;
		float UV[2];
	};

private:
	uint32_t m_framesNum; ///< number of frames
	uint32_t m_jointsNum; ///< number of joints
	std::vector<jointAnimation> m_animation; ///< position and rotation of joints
	MDagPath trajectory; ///< the charaters trajectory (the floor under the character)
	MDagPath hips; ///< the charaters hips
	uint32_t hipsIndex; ///< index for the hips
	uint32_t trajectoryIndex; ///< index for the trajectory
	float cycle_diff[1]; ///< the number of different cycles
	MDagPathArray m_bones; ///< all mesh objects in the scene
	MStringArray m_transformNames; ///< stores names of joint clusters
	MFloatArray m_transformWeights; ///< stores weights of joint clusters
	std::vector<MIntArray> m_transformIndex; ///< stores indices of joint clusters
	std::vector<vertexData> m_vertexArray; ///< stores mesh data
	std::vector<uint32_t> m_vertexIndices; ///< stores indices for mesh data
	uint32_t m_jointClusters; ///< the number of joint clusters
};

//----------------------------------------------------------------------------------------------------------------------
/// \brief	Exporter inherits from MPxFileTranslator (base class for Maya API plugin exporters)
/// \note		most of the methods used within the class are overloading methods from the Maya API
//----------------------------------------------------------------------------------------------------------------------
class  PGAGExporter : public MPxFileTranslator
{
public:
	/// \brief	ctor
	PGAGExporter() : MPxFileTranslator() {}

	/// \brief	writes the scene to the file specified
	/// \param	MFileObject file to output (write) to
	/// \param	MString options for the exporter (not in use)
	/// \param	FileAccessMode writing the selected objects (not in use)
	virtual MStatus writer(const MFileObject& file, const MString& optionsString, FileAccessMode mode);

	/// \brief	bool returns true as this is the exporter
	virtual bool haveWriteMethod() const;

	/// \brief  returns the default extension for the exported files
	/// \return	MString extension
	virtual MString defaultExtension() const;

	/// \brief	returns the filter string for the file dialog 
	/// \return	MString filter string
	virtual MString filter() const;

	/// \brief	 gives to Maya to create a new instance of this plugin 
	static void* create()
	{
		return new PGAGExporter;
	}

protected:

	/// \brief	exports a mesh from maya 
	/// \prama	MString the filename
	void exportMeshes(MString filename);

private:
	animation* m_animation;
	MDagPathArray m_bones; ///< all mesh objects in the scene
};
