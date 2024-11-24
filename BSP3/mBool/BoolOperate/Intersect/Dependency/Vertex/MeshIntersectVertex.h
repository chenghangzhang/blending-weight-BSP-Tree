#pragma once



	class MeshIntersectVertex
	{

	public: //��ʱ public
		const Point3d* m_Point3d;
		const size_t m_IntersectCount = 1;

	public:
		MeshIntersectVertex(const Point3d* p_Point3d) : m_Point3d(p_Point3d) {};
		//virtual ~MeshIntersectVertex() = 0 {}
		virtual ~MeshIntersectVertex() = 0 {}

	public:
		size_t GetIntersectCount() const { return m_IntersectCount; }

	public: //�麯��
		virtual bool IsTerminal() const = 0; //Ŀǰֻ�� 2 ������ Terminal �� Middle��������ʱ�����ַ�ʽ�����������չ���Ըı䷽ʽ		
		virtual bool IsOrigin() const = 0;

		//virtual bool IsMiddle() const = 0;

		virtual const MeshVertexHandle* GetVertex() = 0;

		virtual bool IsIntersectWithTriangle(const Mesh::FaceHandle& p_Triangle) const = 0;
		virtual const MeshTriangleHandle* GetIntersectTriangle(const MeshTriangleHandle* p_pTriangle = nullptr) const = 0;

		virtual Parameter2d GetParameter2dAtTheTriangle(const MeshTriangleHandle& p_Triangle) const = 0;
	};
