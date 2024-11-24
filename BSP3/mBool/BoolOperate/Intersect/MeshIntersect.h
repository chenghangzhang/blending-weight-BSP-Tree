#pragma once

class MeshIntersectResult;
class MeshIntersectAccelerator;

using MeshIntersectMeshResult = unordered_map<Mesh*, MeshIntersectResult>;

class MeshIntersect
	{
	public:
		MeshIntersect(MeshIntersectMeshResult& r_Result, Mesh& p_MeshA, Mesh& p_MeshB, BSPConstructType p_Type = BSPConstructType::WSDM, const Tolerance& p_Tolerence = Tolerance());
		virtual ~MeshIntersect();

	protected: //��������������
		//����
		Mesh& m_OriginMeshA;
		Mesh& m_OriginMeshB;
		Tolerance m_Tolerance;

		//��� 
		MeshIntersectMeshResult& m_IntersectResult;

	protected: //���Ʊ���
		bool m_IsInitialize;

		using IntersectCheckList = vector<pair<const MeshHalfEdgeHandle, const MeshTriangleHandle>>;

		IntersectCheckList m_IntersectCheckList;
		

	protected: //���ٽṹ
		MeshIntersectAccelerator* m_pAccelerator;

	protected: //���̺���
		void Initialize();
		void Reset();

		void GenerateIntersect();

	protected: //�������̺�����ϸ�����ܺ���
		//GenerateIntersect���̵ľ��幦�ܺ���
		void GenerateIntersectCheckList();
		void GenerateIntersect(const IntersectCheckList& p_IntersectCheckList);


		void GenerateIntersect(const MeshHalfEdgeHandle& p_Edge, const MeshTriangleHandle& p_Triangle); //����Ϊ�������κμ��ٽṹ�������ж�
		void GenerateIntersectPath();

	public:
		void SetAccelerator(MeshIntersectAccelerator::Type p_AccleratorType);
		bool Run();
		bool RunGenerateIntersectCheckList(); //��ʱ���� GenerateIntersectCheckList ����
	};

	//MeshIntersectResult* IntersectMesh(const Mesh& p_MeshA, const Mesh& p_MeshB, const Tolerance& p_Tolerance = Tolerance());

