#include "mPch.h"
#include "MeshIntersectOriginalVertex.h"



	MeshIntersectOriginalVertex::MeshIntersectOriginalVertex(const Point3d* p_Point3d, const MeshVertexHandle& p_Vertex) :
		MeshIntersectVertex(p_Point3d),
		m_OriginalVertex(p_Vertex)
	{

	}

	Parameter2d MeshIntersectOriginalVertex::GetParameter2dAtTheTriangle(const MeshTriangleHandle& p_Triangle) const
	{
		//ͨ��ԭʼ������Լ������ε���Ϣ��ȡ��ά����
		return Parameter2d();
	}


