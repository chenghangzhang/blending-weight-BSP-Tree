#include "mPch.h"
#include "Combine.h"


CombineOperate::CombineOperate(Mesh& p_MeshA, Mesh& p_MeshB, const MeshBoolOperateType p_Type, RemeshNewVertexResult& p_NewVertexs, ClassifyRecord& p_TriangleRecord, const Tolerance& p_Tolerence/* = Tolerance()*/):
	m_OriginalMeshA(p_MeshA), m_OriginalMeshB(p_MeshB), m_Type(p_Type), m_IntersectVertexs(p_NewVertexs),m_TriangleRecord(p_TriangleRecord), m_Tolerance(p_Tolerence)
{
	m_classifyTriangleA = m_TriangleRecord[&m_OriginalMeshA];
	m_classifyTriangleB = m_TriangleRecord[&m_OriginalMeshB];

	m_newVertexA = m_IntersectVertexs[&m_OriginalMeshA];
	m_newVertexB = m_IntersectVertexs[&m_OriginalMeshB];
}

CombineOperate::~CombineOperate()
{
}

void CombineOperate::GetMeshFrom2Models(Mesh& r_Mesh1, Mesh& r_Mesh2, bool needReverseNormal)
{
	//assert((m_OutputFacesFromMeshA.size() != 0) && (m_OutputFacesFromMeshB.size() != 0));

	r_Mesh1 = DataTranslate::TranslateFaceID2Mesh(m_OriginalMeshA, m_OutputFacesFromMeshA);

	if (m_Type == MeshBoolOperateType::Difference) {
		r_Mesh2 = DataTranslate::TranslateFaceID2Mesh(m_OriginalMeshB, m_OutputFacesFromMeshB, !needReverseNormal);
	}
	else {
		r_Mesh2 = DataTranslate::TranslateFaceID2Mesh(m_OriginalMeshB, m_OutputFacesFromMeshB);
	}
	
}

void CombineOperate::run(Mesh& r_Mesh)
{
	//���ݲ����������ͣ�ȷ����Щ��Ƭ��Ҫ������������������Ƭ�����µ�Mesh


	switch (m_Type) {
		case MeshBoolOperateType::Difference:
			ProcessDifference(r_Mesh);
			break;
		case MeshBoolOperateType::Intersection:
			ProcessIntersection(r_Mesh);
			break;
		case MeshBoolOperateType::Union:
			ProcessUnion(r_Mesh);
			break;
		default:

			break;

	}

}




void CombineOperate::ProcessDifference(Mesh& r_Mesh)
{
	//��ȡ����Mesh�еķ�����Ϣunordered_map<TRIANGLE, bool> m_TriangleInOutRecord;
	//�Ӷ���µ�����

	m_OutputFacesFromMeshA.clear();
	m_OutputFacesFromMeshB.clear();

	for (auto& itr : m_classifyTriangleA) {

		assert(itr.first >= 0);

		if (true == itr.second) {
			AddTriangle(r_Mesh, itr.first, m_OriginalMeshA);

			m_OutputFacesFromMeshA.insert(itr.first);
		}

	}

	for (auto& itr : m_classifyTriangleB) {

		assert(itr.first >= 0);

		if (false == itr.second) {
			AddTriangle(r_Mesh, itr.first, m_OriginalMeshB, true);

			m_OutputFacesFromMeshB.insert(itr.first);
		}

	}


}

void CombineOperate::ProcessIntersection(Mesh& r_Mesh)
{
	//��ȡ����Mesh�еķ�����Ϣunordered_map<TRIANGLE, bool> m_TriangleInOutRecord;
	//�Ӷ���µ�����
	m_OutputFacesFromMeshA.clear();
	m_OutputFacesFromMeshB.clear();


	for (auto& itr : m_classifyTriangleA) {

		//assert(itr.first != nullptr);
		if (false == itr.second) {
			AddTriangle(r_Mesh, itr.first, m_OriginalMeshA);

			m_OutputFacesFromMeshA.insert(itr.first);
		}

	}

	for (auto& itr : m_classifyTriangleB) {

		//assert(itr.first != nullptr);
		if (false == itr.second) {
			AddTriangle(r_Mesh, itr.first, m_OriginalMeshB);

			m_OutputFacesFromMeshB.insert(itr.first);
		}

	}
}

void CombineOperate::ProcessUnion(Mesh& r_Mesh)
{
	//��ȡ����Mesh�еķ�����Ϣunordered_map<TRIANGLE, bool> m_TriangleInOutRecord;
	//�Ӷ���µ�����
	m_OutputFacesFromMeshA.clear();
	m_OutputFacesFromMeshB.clear();

	for (auto& itr : m_classifyTriangleA) {

		//assert(itr.first != nullptr);

		if (true == itr.second) {
			AddTriangle(r_Mesh, itr.first, m_OriginalMeshA);

			m_OutputFacesFromMeshA.insert(itr.first);
		}
			
	}

	for (auto& itr : m_classifyTriangleB) {

		//assert(itr.first != nullptr);
		if (true == itr.second) {
			AddTriangle(r_Mesh, itr.first, m_OriginalMeshB);

			m_OutputFacesFromMeshB.insert(itr.first);
		}

	}

}

void CombineOperate::AddTriangle(Mesh& r_Mesh, FaceId p_Triangle, Mesh& p_SourceMesh, bool p_NeedDifferNormal /*= false*/)
{
	//const MeshVertex& mv0 = p_Triangle->VertexAt(0);

	array<FaceId, 3> newTriangle = {-1 , -1, -1};

	Mesh::FaceVertexCCWIter fv = p_SourceMesh.fv_ccwbegin(p_SourceMesh.face_handle(p_Triangle));
	if (!p_NeedDifferNormal) {
		
		for (int i = 0;i < 3;i++) {
			VertexId mv = (*fv++).idx();
			newTriangle[i] = AddVertex(r_Mesh, mv,p_SourceMesh);
		}
		
	}
	else {

		newTriangle[0] = AddVertex(r_Mesh, (*fv++).idx(), p_SourceMesh);
		newTriangle[2] = AddVertex(r_Mesh, (*fv++).idx(), p_SourceMesh);
		newTriangle[1] = AddVertex(r_Mesh, (*fv++).idx(), p_SourceMesh);
	}


	//assert(!IsContainerHas(newTriangle, nullptr))
	vector<Mesh::VertexHandle> face{
		r_Mesh.vertex_handle(newTriangle[0]) ,
		r_Mesh.vertex_handle(newTriangle[1]) ,
		r_Mesh.vertex_handle(newTriangle[2])
	};

	r_Mesh.add_face(face);
}

VertexId CombineOperate::AddVertex(Mesh& r_Mesh, VertexId p_Vertex, Mesh& p_SourceMesh)
{
	unordered_map<VertexId, VertexId>* addedVertex = nullptr;
	unordered_map<VertexId, VertexId>* addedVertexAnother = nullptr;

	RemeshNewVertexRecord* newVertexRecord = nullptr;
	RemeshNewVertexRecord* newVertexRecordAnother = nullptr;



	if ((&p_SourceMesh) == (&m_OriginalMeshA)) 
	{
		addedVertex = &m_AddedVertexA;
		addedVertexAnother = &m_AddedVertexB;

		newVertexRecord = &m_newVertexA;
		newVertexRecordAnother = &m_newVertexB;
	}
	else if((&p_SourceMesh) == (&m_OriginalMeshB))
	{
		addedVertex = &m_AddedVertexB;
		addedVertexAnother = &m_AddedVertexA;

		newVertexRecord = &m_newVertexB;
		newVertexRecordAnother = &m_newVertexA;
	}
	else {
		assert(false);
	}
	
	assert(nullptr != addedVertex);
	assert(nullptr != addedVertexAnother);
	assert(nullptr != newVertexRecord);
	assert(nullptr != newVertexRecordAnother);
	
	auto itrAdded = addedVertex->find(p_Vertex);

	//���ж��Ƿ�õ��Ѿ���ӵ�������
	if (addedVertex->end() == itrAdded) {
		//û�����

			RemeshNewVertexRecord::iterator itr = newVertexRecord->find(p_Vertex);

			//�ж��Ƿ������������󽻲������µĵ�
			if (newVertexRecord->end() == itr) {

				//���ǣ�ֱ�����
				Mesh::VertexHandle newVertex = r_Mesh.add_vertex(p_SourceMesh.point(p_SourceMesh.vertex_handle(p_Vertex)));
				(*addedVertex)[p_Vertex] = newVertex.idx();

				return newVertex.idx();

			}
			else {

				//�õ������������󽻲������µĵ�
				const Point3d* thisPoint = newVertexRecord->at(p_Vertex);
				//vector<MeshVertex*> samePositionVertex;

				//���Ҹõ��Ƿ��Ѿ����
				auto itrHere = addedVertex->find(p_Vertex);
				if (addedVertex->end() != itrHere) {
					return itrHere->second;
				}

				//�õ�δ���
				//��������λ����ͬ���󽻵��Ƿ��Ѿ���ӵ�������
				// 
				//�ڵ�ǰ����mesh�����в�����ͬλ�õ�
				for(auto& itr: *newVertexRecord){

					if ((itr.second == thisPoint) && (itr.first != p_Vertex)) {

						auto itrHere = addedVertex->find(itr.first);

						//��ͬλ�õ��Ѿ���ӣ�ֱ�ӷ���
						if (addedVertex->end() != itrHere) {
							return itrHere->second;
						}

					}

				}

				//����һMesh���������в�����ͬλ�õ�
				for (auto& itr : *newVertexRecordAnother) {

					//ֻ��ȷ���Ƿ�λ��һ��
					if (itr.second == thisPoint) {
						auto itrHere = addedVertexAnother->find(itr.first);

						//��ͬλ�õ��Ѿ���ӣ�ֱ�ӷ���
						if (addedVertexAnother->end() != itrHere) {
							return itrHere->second;
						}

					}

				}

				//��ͬλ�õ�û���Ѿ���ӵģ�ֱ�����
				Mesh::VertexHandle newVertex = r_Mesh.add_vertex(p_SourceMesh.point(p_SourceMesh.vertex_handle(p_Vertex)));
				(*addedVertex)[p_Vertex] = newVertex.idx();

				return newVertex.idx();
			}

	}
	else {
		return itrAdded->second;		
	}

	assert(false);
	return -1;
}



