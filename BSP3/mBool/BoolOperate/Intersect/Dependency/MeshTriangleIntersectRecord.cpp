#include "mPch.h"
#include "MeshTriangleIntersectRecord.h"



	MeshTriangleIntersectRecord::MeshTriangleIntersectRecord() :
		m_TerminalVertices(0),
		m_MiddleVertices(0),
		m_IntersectPaths(0),
		m_pVertexIntersectCountRecord(nullptr)
	{

	}

	MeshTriangleIntersectRecord::~MeshTriangleIntersectRecord()
	{
		if (nullptr != m_pVertexIntersectCountRecord)
		{
			delete m_pVertexIntersectCountRecord;
			m_pVertexIntersectCountRecord = nullptr;
		}
	}

	vector<MeshIntersectVertex*>& MeshTriangleIntersectRecord::GetMeshIntersectVertex()
	{

		vector<MeshIntersectVertex*>* mIVs = new vector<MeshIntersectVertex*>();
		for (auto& mTV : m_TerminalVertices) {
			mIVs->push_back(const_cast<MeshIntersectTerminalVertex*>(mTV));
		}



		for (auto& mMV : m_MiddleVertices) {
			mIVs->push_back(const_cast<MeshIntersectMiddleVertex*>(mMV));
		}

		//mIVs->insert(mIVs->end(), m_TerminalVertices.begin(), m_TerminalVertices.end());
		//mIVs->insert(mIVs->end(), m_MiddleVertices.begin(), m_MiddleVertices.end());

		return *mIVs;
		// 
		//vector<MeshIntersectVertex*> aa;
		//return aa;

		// TODO: �ڴ˴����� return ���
	}

	bool MeshTriangleIntersectRecord::IsValidPath(const MeshIntersectPath& p_Path)
	{
		if (p_Path.Size() <= 1)
		{
			return false;
		}

		const MeshIntersectVertex& pathBegin = p_Path.Front();
		const MeshIntersectVertex& pathEnd = p_Path.Back();

		//��·�����˶�Ϊ�˵�ʱ���Ǻ���·��
		if (pathBegin.IsTerminal() && pathEnd.IsTerminal())
		{
			return true;
		}

		return false;
	}

	bool MeshTriangleIntersectRecord::HasCoincidentTerminalVertex(const MeshIntersectTerminalVertex& p_Vertex, const Tolerance& p_Tolerance)
	{
		for (auto& pTv : m_TerminalVertices)
		{
			if ((*(pTv->m_Point3d)- *(p_Vertex.m_Point3d)).length() < p_Tolerance.m_Squared)
			{
				//��ͬλ�ö˵�
				
				if (pTv->GetIntersectTriangle(nullptr) == p_Vertex.GetIntersectTriangle(nullptr))
				{
					//�ཻ��������ͬ

					if ( (&pTv->GetHostEdge()) == (&p_Vertex.GetHostEdge()) ) {
						//����HostEdge��ͬ
						return true;
					}
				}
			}
		}

		//��ʱ�Ȳ����ظ���Ĵ����ȴ��������������������

		return false;
	}

	bool MeshTriangleIntersectRecord::HasCoincidentMiddleVertex(const MeshIntersectMiddleVertex& p_Vertex, const Tolerance& p_Tolerance)
	{
		for (auto& pMv : m_MiddleVertices)
		{
			if ((*(pMv->m_Point3d)- *(p_Vertex.m_Point3d)).length() < p_Tolerance.m_Squared)
			{
				//��ͬλ�õ�

				if (&pMv->GetHostTriangle() == &p_Vertex.GetHostTriangle())
				{
					//�ཻ��������ͬ

					if (&pMv->GetIntersectEdge() == &p_Vertex.GetIntersectEdge()) {
						//����HostEdge��ͬ
						return true;
					}
				}
			}
		}
		

		return false;
	}

	void MeshTriangleIntersectRecord::AddNewTerminalVertex(const MeshIntersectTerminalVertex* p_pVertex)
	{
		assert(nullptr != p_pVertex);
		
		//test have coincidence point
		bool hasSame = false;

		for (auto& tv : m_TerminalVertices) {
			if ((tv->GetHostEdge().first.idx() == p_pVertex->GetHostEdge().first.idx()) &&
				(tv->GetIntersectTriangle()->first.idx() == p_pVertex->GetIntersectTriangle()->first.idx())
				) {
				hasSame = true;
				break;
			}
		
		}

		if(!hasSame) m_TerminalVertices.push_back(p_pVertex);
		
	}

	MeshIntersectVertex* MeshTriangleIntersectRecord::AddMiddleVertex(const MeshIntersectMiddleVertex& p_Vertex, const Tolerance& p_Tolerance)
	{

		//test have coincidence point
		bool hasSame = false;

		for (auto& mv : m_MiddleVertices) {
			assert((mv->GetHostTriangle().first.idx() == p_Vertex.GetHostTriangle().first.idx()));

			const MeshHalfEdgeHandle& mie = mv->GetIntersectEdge();
			Mesh::HalfedgeHandle ie = mie.first;
			Mesh::HalfedgeHandle oppoIe = mie.second->opposite_halfedge_handle(ie);

			if ((ie.idx() == p_Vertex.GetIntersectEdge().first.idx()) || (oppoIe.idx() == p_Vertex.GetIntersectEdge().first.idx())) {
				hasSame = true;
				break;
			}

		}

		if (!hasSame) {
			auto* pNewVertex = new MeshIntersectMiddleVertex(p_Vertex);
			m_MiddleVertices.push_back(pNewVertex);

			return pNewVertex;
		}

		return nullptr;
	}

	vector<MeshIntersectPath*> MeshTriangleIntersectRecord::GeneratePaths()
	{
		m_IntersectPaths.clear();
		assert(nullptr == m_pVertexIntersectCountRecord);

		if (0 == m_TerminalVertices.size())
		{
			//�˵㿪ʼѰ�ң�ֻ�����е�������λ�������·����ͨ���˵������
			//������������ཻ��ֻ�����е�//�ݲ�����
			return m_IntersectPaths;
		}

		//for (auto& v : m_TerminalVertices) {
		//	cout << "Edge " << v->GetHostEdge().first.idx() << endl;
		//	cout << "Face " << v->GetIntersectTriangle()->first.idx() << endl;
		//}


		//��ʼ�����ж����ཻ����
		m_pVertexIntersectCountRecord = new VertexIntersectCountRecordMap;
		auto& intersectCountRecord = *m_pVertexIntersectCountRecord;

		//�ȷŶ˵㣬�������Ա�֤�� tsl::ordered_map �������ȴ���˵�
		for (auto& pTv : m_TerminalVertices)
		{
			assert(nullptr != pTv);
			assert(intersectCountRecord.find(pTv) == intersectCountRecord.end()); //û���ظ���
			intersectCountRecord[pTv] = pTv->GetIntersectCount();
		}

		for (auto& pMv : m_MiddleVertices)
		{
			assert(nullptr != pMv);
			assert(intersectCountRecord.find(pMv) == intersectCountRecord.end()); //û���ظ���
			intersectCountRecord[pMv] = pMv->GetIntersectCount();
		}

		while (0 != intersectCountRecord.size())
		{
			auto record = intersectCountRecord.begin(); // record Ϊ������ iterator
			const MeshIntersectVertex& v = *(record.key());
			size_t& intersectCount = record.value(); // record->second Ϊ����������������
			assert(0 < intersectCount);


			if (!v.IsTerminal()) {
				intersectCountRecord.erase(&v);
				continue;
			}

			const MeshTriangleHandle* pIntersectTriangle = v.GetIntersectTriangle();
			assert(nullptr != pIntersectTriangle);



			MeshIntersectPath& newPath = *(new MeshIntersectPath); //ò������÷����Ǻܹ淶
			newPath.AddVertex(v);
			intersectCount--; //��ǵ�ǰ·������

			GeneratePath(newPath, *pIntersectTriangle);

			//�жϵ�ǰ����·���Ƿ���Ч
			if (true == IsValidPath(newPath))
			{
				//��Ч·����ʼά�� intersectCountRecord ��Ϣ��������·��
				size_t newPathSize = newPath.Size();
				for (size_t i = 0; i < newPathSize; i++)
				{
					const auto& v = newPath[i];
					const size_t& c = intersectCountRecord[&v];
					if (0 == c)
					{
						intersectCountRecord.erase(&v);
					}
				}

				m_IntersectPaths.push_back(&newPath);
			}
			else
			{
				//��Ч·����ʼ�ظ� intersectCountRecord ��Ϣ��������·��
				newPath.DeletePathSegment();

				size_t newPathSize = newPath.Size();
				for (size_t i = 0; i < newPathSize; i++)
				{
					const auto& v = newPath[i];
					size_t& c = intersectCountRecord[&v];

					if (0 == c) // ���������Ŀǰ�޷ֲ�·�����������ÿ������ܷŵ� 1 ��·����
					{
						intersectCountRecord.erase(&v);
					}

					// c++; //�ظ���·���ݹ��������ʱ�Ƴ��� count
				}
			}
		}

		return m_IntersectPaths;
	}

	void MeshTriangleIntersectRecord::GeneratePath(MeshIntersectPath& r_Path, const MeshTriangleHandle& p_IntersectTriangle)
	{
		assert(nullptr != m_pVertexIntersectCountRecord);
		auto& intersectCountRecord = *m_pVertexIntersectCountRecord;
		assert(0 < intersectCountRecord.size());


		const MeshTriangleHandle* nextTriangle = nullptr;

		for (auto record = intersectCountRecord.begin(); record != intersectCountRecord.end(); record++) //record Ϊ������ iterator, ����֧��for(:) ����
		{
			const MeshIntersectVertex& v = *(record.key());
			size_t& intersectCount = record.value(); // record->second Ϊ����������������

			if (0 == intersectCount || &v == &(r_Path.Back())) //�����ϸ��ڵ��ж� �� ��ǰ�ڵ��ڵ�ǰ·���Ѿ�û���ཻ��
			{
				continue;
			}

			if (true == v.IsIntersectWithTriangle(p_IntersectTriangle.first))
			{
				r_Path.AddVertex(v);
				intersectCount--; //�����Ƿ񴴽��ɹ���Ҫ�� count �� 1

				const auto* pNextIntersectTriangle = v.GetIntersectTriangle(&p_IntersectTriangle);
				if (nullptr != pNextIntersectTriangle) //�����Լ����ݹ����·��
				{
					return GeneratePath(r_Path, *pNextIntersectTriangle);
				}
				else //����·���յ�, ���� 
				{
					return;
				}
			}
		}


		////�����м�ڵ� MiddleVertex
		//for (auto& pMv : m_MiddleVertices)
		//{
		//	if (pMv != &(r_Path.Back()))
		//	{
		//		//����㲻�غ�
		//		auto& intersectMvTriangles = pMv->GetIntersectEdge().GetHostTriangles();

		//		for (const auto& tri : intersectMvTriangles)
		//		{
		//			if (tri == &p_IntersectTriangle)
		//			{
		//				if (1 == intersectMvTriangles.size())
		//				{
		//					//��·��������Ƭ�߽�
		//					return false;
		//				}
		//				else
		//				{

		//					assert(2 == intersectMvTriangles.size());

		//					//������һ��������
		//					nextTriangle = (&p_IntersectTriangle == intersectMvTriangles[0]) ? intersectMvTriangles[1] : intersectMvTriangles[0];
		//					return GeneratePath(r_Path, *nextTriangle);
		//				}
		//			}
		//		}
		//	}
		//}

		//assert(nullptr == nextTriangle);

		////�����˵� TerminalVertex
		////û�������� Middle Vertex ,���� TerminalVertex
		//for (auto& pTv : m_TerminalVertices)
		//{
		//	if (&(pTv->GetIntersectTriangle()) == &p_IntersectTriangle)
		//	{
		//		r_Path.AddVertex(*pTv);
		//		//·����TerminalVertex����
		//		return true;
		//	}
		//}

		//ֻ��һ���ཻ�����γ�·��
		//�ݲ�����
		//return false;
	}
