#pragma once


class MeshBoolOperate {

protected: //�����������
	Mesh& m_OriginMeshA;
	Mesh& m_OriginMeshB;
	Tolerance m_Tolerance;

	
protected: //�м����
	//remesh·�����
	RemeshPathEdgeResult m_RemeshEdgeResult;

	//remesh������
	RemeshNewVertexRecord m_RemeshNewVertexRecord;
	RemeshNewVertexResult m_RemeshNewVertexResult;


	

protected: //���Ʊ���
	bool m_IsInitialize;

public:
	//����
	MeshIntersectMeshResult m_IntersectResult;

	//����
	//�����Ƭ�����ڻ�������
	ClassifyRecord m_TriangleRecord;

	BSPConstructType m_TreeType;

public:
	MeshBoolOperate(Mesh& p_MeshA, Mesh& p_MeshB, BSPConstructType p_TreeType, const Tolerance& p_Tolerence = Tolerance());
	~MeshBoolOperate();

protected: //���̺���
	void Initialize();
	void MeshPreProcess();
	void GenerateIntersect();
	void GenerateRemesh();
	void Classify();
	void Combine(Mesh& r_Result, const MeshBoolOperateType p_Type);
	

	void CombineTest(Mesh& r_Result, const MeshBoolOperateType p_Type, Mesh& r_MeshA, Mesh& r_MeshB, bool needReverseNormal);
private: //�������̺�����ϸ�����ܺ���
	//Initialize���̵ľ��幦�ܺ���

	//GenerateIntersect���̵ľ��幦�ܺ���

	//Remesh���̵ľ��幦�ܺ���

protected: //���ƺ���

	void Clear();

public:
	bool Run(Mesh& r_Result, const MeshBoolOperateType p_Type);

	bool RunTest(Mesh& r_Result, const MeshBoolOperateType p_Type, Mesh& r_MeshA, Mesh& r_MeshB, bool needReverseNormal, string file_head);
};
