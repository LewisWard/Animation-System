// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 20/02/2015
#include <fstream>
#include "PGAGExporter.h"
#include <maya/MItDag.h>
#include <maya/MFnDagNode.h>
#include <maya/MTypes.h>
#include <maya/MFnMesh.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MPointArray.h>
#include <maya/MIntArray.h>
#include <maya/MFloatMatrix.h>
#include <maya/MMatrix.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>

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
				<< m_animation[i].positions[f].y << " " // need to invert as in the program Y is the otherway round
				<< m_animation[i].positions[f].z << " "
				<< m_animation[i].rotations[f].x << " "
				<< m_animation[i].rotations[f].y << " "
				<< m_animation[i].rotations[f].z << " "
				<< m_animation[i].rotations[f].w << std::endl;
		}
	}

	// joint cluster iterator
	MItDependencyNodes jointIT(MFn::kJointCluster);

	// go over all joint clusters
	while (!jointIT.isDone())
	{
		// attach function set
		MFnDependencyNode nodeCluster(jointIT.item());

		// get a plug to the matrix attribute
		MPlug plug = nodeCluster.findPlug("matrix");
		MPlugArray connections;

		// if the attribute is connected to a transform
		if (plug.connectedTo(connections, true, false))
		{
			// write to file
			MFnDependencyNode fnTransform(connections[0].node());
			ouputFileStream << fnTransform.name().asChar() << std::endl;
		}

		// create a weight filter
		MFnWeightGeometryFilter geoWeight(jointIT.item());

		// set the function to get the cluster members
		MFnSet set(geoWeight.deformerSet());

		// a selection list for objects within Maya
		MSelectionList objects;

		// get the objects in the cluster
		set.getMembers(objects, true);

		// cycle over the objects
		for (uint32_t i = 0; i < objects.length(); i++)
		{
			MDagPath skin;
			MObject elements;
			MFloatArray weights;

			// get path to the affected element
			objects.getDagPath(i, skin, elements);

			// get the vertex indices and weights for each point under influence
			// the weights should be 1, so can ignore them
			geoWeight.getWeights(skin, elements, weights);

			// set the function to the shape, in order to access it's name
			MFnDependencyNode shape(elements);

			// write shape name, and its number of points
			ouputFileStream << shape.name().asChar() << weights.length() << "\n";

			// output the vertex indices by iterating through the geometry components
			MItGeometry it(skin, elements);
			for (; !it.isDone(); it.next())
			{
				ouputFileStream << it.index() << "\n";
			}
		}

		// get next
		jointIT.next();
	}

	// cycle all objects
	for (uint32_t mesh = 0; mesh < m_bones.length(); ++mesh)
	{
		MFnMesh fn(m_bones[mesh]);

		MFloatPointArray vertices; ///< store the vertices
		MFloatVectorArray normals; ///< store the vertex normals
		MFloatArray uCoord, vCoord; ///< store the UV coordinates

		// setup a function set on the node
		MFnMesh meshFunc(m_bones[mesh]);

		// get the matrix
		MMatrix inclusiveMat(m_bones[mesh].inclusiveMatrix());

		// get the vertices/normals/UV's
		fn.getPoints(vertices);
		fn.getNormals(normals);
		fn.getUVs(uCoord, vCoord);
 
		// will be used to build up a map. As Maya uses several sets of indices it creates a small
		// problem, as OpenGL uses only one set of indices for vertex data! 
		struct mappingTriple
		{
			int32_t n;
			int32_t t;
			int32_t index;
		};

		// store vertex data
		struct vertexData
		{
			vec3 vertex;
			vec3 normal;
			float UV[2];
		};

		// an array of mapping that is the same size as the number of vertices in the mesh
		std::vector<std::vector<mappingTriple>> indexedMap;
		indexedMap.resize(vertices.length());

		// array of vertices
		std::vector<vertexData> verticesArray;
		verticesArray.reserve(vertices.length());

		// vertex indices
		std::vector<uint32_t> vIndices;
		vIndices.resize(vertices.length() * 2);
		uint32_t index = 0; ///< first index

		// need to split every polygon face into triangles and create a set of UV's for them
		std::vector<uint32_t> indicesFaceA;
		std::vector<uint32_t> indicesFaceB;

		// go over each polygon
		MItMeshPolygon polyIT(m_bones[mesh]);
		while (!polyIT.isDone())
		{
			// resize the indices 
			indicesFaceA.reserve(polyIT.polygonVertexCount());

			// construct each index
			for (uint32_t i = 0; i < polyIT.polygonVertexCount(); ++i)
			{
				// get the index
				int32_t vertexI = polyIT.vertexIndex(i);
				int32_t normalI = polyIT.normalIndex(i);
				int32_t uvI = polyIT.getUVIndex(i, uvI);

				// add vertex to array
				indicesFaceB.push_back(vertexI);

				// set to max value
				index = 0xFFFFFFFF;

				// cycle over to test if there is a match
				for (size_t j = 0; j < indexedMap[vertexI].size(); ++j)
				{
					if (normalI == indexedMap[vertexI][j].n && uvI == indexedMap[vertexI][j].t)
					{
						// found an existing index
						index = indexedMap[vertexI][j].index;
						break;
					}
				}

				// add vertex element if new
				if (index == 0xFFFFFFFF)
				{
					vertexData vData;
					MVector normal;

					// compute normal (normalize) and vertex position
					polyIT.getNormal(i, normal);
					MPoint point = polyIT.point(i) * inclusiveMat;
					normal = normal * inclusiveMat;
					normal.normalize();
					float uv[2];
					polyIT.getUV(i, uv);
					vData.normal.x = (float)normal.x;
					vData.normal.y = (float)normal.y;
					vData.normal.z = (float)normal.z;
					vData.vertex.x = (float)point.x;
					vData.vertex.y = (float)point.y;
					vData.vertex.z = (float)point.z;
					vData.UV[0] = uv[0];
					vData.UV[1] = uv[1];

					verticesArray.push_back(vData);

					mappingTriple id;
					id.n = normalI;
					id.t = uvI;
					id.index = (int32_t)(verticesArray.size() - 1);
					indexedMap[vertexI].push_back(id);

					indicesFaceA.push_back(id.index);
				}
				else
				{
					indicesFaceA.push_back(index);
				}
			}

			int tri_count = 0;
			polyIT.numTriangles(tri_count);

			// step through the triangles
			for (int32_t x = 0; x < tri_count; ++x)
			{
				MPointArray tri_points;
				MIntArray triVerts;
				polyIT.getTriangle(x, tri_points, triVerts);

				for (int32_t y = 0; y < 3; ++y)
				{
					int vertexTri = triVerts[y];
					for (size_t z = 0; z < indicesFaceB.size(); ++z)
					{
						if (vertexTri == indicesFaceB[z])
						{
							vIndices.push_back(indicesFaceA[z]);
						}
					}
				}
			}

			indicesFaceA.clear();
			indicesFaceB.clear();
			polyIT.next();
		}

		// make sure zero is not printed
		if (verticesArray.size() || vIndices.size())
		{
				ouputFileStream << verticesArray.size() << std::endl;
				ouputFileStream << vIndices.size() << std::endl;
		}
		for (size_t kk = 0; kk < verticesArray.size(); ++kk)
		{
			ouputFileStream << verticesArray[kk].vertex.x << " "
				<< verticesArray[kk].vertex.y << " "
				<< verticesArray[kk].vertex.z << " "
				<< verticesArray[kk].normal.x << " "
				<< verticesArray[kk].normal.y << " "
				<< verticesArray[kk].normal.z << " "
				<< verticesArray[kk].UV[0] << " "
				<< verticesArray[kk].UV[1] << std::endl;
		}
		for (size_t kk = 0; kk < vIndices.size(); kk += 3)
		{
			ouputFileStream << vIndices[kk] << " " << vIndices[kk + 1] << " " << vIndices[kk + 2] << std::endl;
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
					// store the mesh to be exported later
					bones.append(path);
				}
			}
		}

		if (path.hasFn(MFn::kJoint))
		{
			// store the mesh to be exported later
			bones.append(path);
		}


		if (path.hasFn(MFn::kJointCluster))
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
