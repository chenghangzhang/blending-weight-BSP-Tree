#pragma once


class CombineOperate 
{
	
public:
		
	CombineOperate(Mesh& p_MeshA, Mesh& p_MeshB, const MeshBoolOperateType p_Type,
		RemeshNewVertexResult& p_NewVertexs, ClassifyRecord& p_TriangleRecord, const Tolerance& p_Tolerence = Tolerance());
	~CombineOperate();

public:
	//Ϊ���Խ���صؼӵ�

	void GetMeshFrom2Models(Mesh& r_Mesh1, Mesh& r_Mesh2, bool needReverseNormal);

	unordered_set<FaceId> m_OutputFacesFromMeshA;
	unordered_set<FaceId> m_OutputFacesFromMeshB;

protected:
	//�������

	//����
	Mesh& m_OriginalMeshA;
	Mesh& m_OriginalMeshB;

	const MeshBoolOperateType m_Type;

	//����ȥ��
	RemeshNewVertexResult& m_IntersectVertexs;
	RemeshNewVertexRecord m_newVertexA;
	RemeshNewVertexRecord m_newVertexB;

	//��Ƭ��ǽ��
	ClassifyRecord& m_TriangleRecord;
	unordered_map<FaceId, bool> m_classifyTriangleA;
	unordered_map<FaceId, bool> m_classifyTriangleB;

	Tolerance m_Tolerance;

	//���
	//Mesh m_rMesh;

	unordered_map<VertexId, VertexId> m_AddedVertexA;
	unordered_map<VertexId, VertexId> m_AddedVertexB;
protected:

	void ProcessDifference(Mesh& r_Mesh);
	void ProcessIntersection(Mesh& r_Mesh);
	void ProcessUnion(Mesh& r_Mesh);
	
	void AddTriangle(Mesh& r_Mesh, FaceId p_Triangle, Mesh& p_SourceMesh,bool p_NeedDifferNormal = false);
	VertexId AddVertex(Mesh& r_Mesh, VertexId p_Vertex, Mesh& p_SourceMesh);
public:
	void run(Mesh& r_Mesh);

};

