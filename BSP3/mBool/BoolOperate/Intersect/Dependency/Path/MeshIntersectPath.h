#pragma once



	class MeshIntersectPath
	{
		friend class MeshIntersectResult;
		friend class MeshTriangleIntersectRecord;
		friend class CcwGraph;

	protected:
		MeshIntersectPath() {}
		virtual ~MeshIntersectPath() {}

	//protected:
	public: //������Ҫ����ʱpublic
		vector<const MeshIntersectVertex*> m_IntersectVertices;
		vector<PathSegment*> m_PathSegment;

	public:
		void AddVertex(const MeshIntersectVertex& p_Vertex);

	public:
		size_t Size() const { return m_IntersectVertices.size(); }

		const MeshIntersectVertex& Front() const { return  *(m_IntersectVertices[0]); } //begin ���ص��� iterator 
		const MeshIntersectVertex& Back() const { return  *(m_IntersectVertices.back()); }

	public:
		const MeshIntersectVertex& operator[](size_t p_Index) const;

		vector<PathSegment*>& GetPathSegments() { return m_PathSegment; }
		bool IsInPath(const MeshIntersectVertex* p_Vertex);

		void DeletePathSegment();
	};


