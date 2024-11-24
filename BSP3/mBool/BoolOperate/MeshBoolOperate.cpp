#include "mPch.h"
#include "BoolRely.h"
#include "MeshBoolOperate.h"


MeshBoolOperate::MeshBoolOperate(Mesh& p_MeshA, Mesh& p_MeshB, BSPConstructType p_TreeType, const Tolerance& p_Tolerence /*= Tolerance()*/)
	:m_OriginMeshA(p_MeshA), m_OriginMeshB(p_MeshB), m_Tolerance(p_Tolerence), m_TreeType(p_TreeType)
{
	m_IsInitialize = false;
	Initialize();
}


MeshBoolOperate::~MeshBoolOperate()
{

}

void MeshBoolOperate::Initialize()
{
	assert(false == m_IsInitialize);

	m_OriginMeshA.request_vertex_status();
	m_OriginMeshA.request_edge_status();
	m_OriginMeshA.request_face_status();

	m_OriginMeshB.request_vertex_status();
	m_OriginMeshB.request_edge_status();
	m_OriginMeshB.request_face_status();

	m_OriginMeshA.request_face_normals();
	m_OriginMeshB.request_face_normals();

	m_OriginMeshA.update_face_normals();
	m_OriginMeshB.update_face_normals();

	m_IsInitialize = true;
}

void MeshBoolOperate::MeshPreProcess()
{
	PreProcess prA({&m_OriginMeshA,&m_OriginMeshB},m_Tolerance);
	prA.run();

}

void MeshBoolOperate::GenerateIntersect()
{
	MeshIntersect mi(m_IntersectResult, m_OriginMeshA, m_OriginMeshB, m_TreeType, m_Tolerance);
	mi.Run();
}


void MeshBoolOperate::Clear()
{

}


bool MeshBoolOperate::Run(Mesh& r_Result, const MeshBoolOperateType p_Type)
{
	//ofstream of("C:\\Users\\NUOSEN\\Desktop\\work\\BSP\\bool-new3-d\\TestModel\\bool operate\\StageTime.txt", ios::app);

	//of << "========================="<< endl;
	//string s;
	//switch (m_TreeType)
	//{
	//case BSPConstructType::SDM:
	//	s = "SDM";
	//	break;
	//case BSPConstructType::SAH:
	//	s = "SAH";
	//	break;
	//case BSPConstructType::ObbMiddel:
	//	s = "Obb";
	//	break;
	//case BSPConstructType::Gravity_SPLIT:
	//	s = "Gravity";
	//	break;
	//default:
	//	break;
	//}
	//of << s << endl;

	auto t0 = std::chrono::steady_clock::now();

	MeshPreProcess();
	

	auto t1 = std::chrono::steady_clock::now();
	//of<< "Ԥ�����ʱ��" << std::chrono::duration<double, std::milli>(t1 - t0).count() << "����" << endl;

	GenerateIntersect();

	auto t2 = std::chrono::steady_clock::now();
	//of << "�󽻺�ʱ��" << std::chrono::duration<double, std::milli>(t2 - t1).count() << "����" << endl;

	GenerateRemesh();

	auto t3 = std::chrono::steady_clock::now();
	//of << "���ǻ���ʱ�� " << std::chrono::duration<double, std::milli>(t3 - t2).count() << "����" << endl;

	Classify();

	auto t4 = std::chrono::steady_clock::now();
	//of << "��Ƭ�����ʱ�� " << std::chrono::duration<double, std::milli>(t4 - t3).count() << "����" << endl;

	Combine(r_Result, p_Type);

	auto t5 = std::chrono::steady_clock::now();
	//of << "��Ƭ��Ϻ�ʱ�� " << std::chrono::duration<double, std::milli>(t5 - t4).count() << "����" << endl;

	//of << "�ܺ�ʱ�� " << std::chrono::duration<double, std::milli>(t5 - t1).count() << "����" << endl;

	//cout << "��ʱ�� " << std::chrono::duration<double, std::milli>(t5 - t1).count() << "����" << endl;
	
	//of << "=========================" << endl;
	//of.close();
	return false;
}

bool MeshBoolOperate::RunTest(Mesh& r_Result, const MeshBoolOperateType p_Type, Mesh& r_MeshA, Mesh& r_MeshB, bool needReverseNormal, string file_head) {

	ofstream of1("..//TestModel//batch bool//" + file_head + "Time.txt", ios::app);
	//ofstream of2("..//TestModel//batch bool//" + file_head + "RemeshTime.txt", ios::app);
	//ofstream of3("..//TestModel//batch bool//" + file_head + "ClassifyTime.txt", ios::app);
	//ofstream of4("..//TestModel//batch bool//" + file_head + "CombineTime.txt", ios::app);
	

	auto t0 = std::chrono::steady_clock::now();

	MeshPreProcess();

	auto t1 = std::chrono::steady_clock::now();
	//cout << "Ԥ�����ʱ��" << std::chrono::duration<double, std::milli>(t1 - t0).count() << "����" << endl;

	GenerateIntersect();

	auto t2 = std::chrono::steady_clock::now();
	//cout << "�󽻺�ʱ��" << std::chrono::duration<double, std::milli>(t2 - t1).count() << "����" << endl;
	of1 << std::chrono::duration<double, std::milli>(t2 - t1).count() << ",";

	GenerateRemesh();

	auto t3 = std::chrono::steady_clock::now();
	//cout << "���ǻ���ʱ�� " << std::chrono::duration<double, std::milli>(t3 - t2).count() << "����" << endl;
	of1 << std::chrono::duration<double, std::milli>(t3 - t2).count() << ",";
	Classify();

	auto t4 = std::chrono::steady_clock::now();
	////cout << "��Ƭ�����ʱ�� " << std::chrono::duration<double, std::milli>(t4 - t3).count() << "����" << endl;
	of1 << std::chrono::duration<double, std::milli>(t4 - t3).count() << ",";
	//Combine(r_Result, p_Type);
	CombineTest(r_Result, p_Type, r_MeshA, r_MeshB, needReverseNormal);

	auto t5 = std::chrono::steady_clock::now();


	//cout << "��Ƭ��Ϻ�ʱ�� " << std::chrono::duration<double, std::milli>(t5 - t4).count() << "����" << endl;
	of1 << std::chrono::duration<double, std::milli>(t5 - t4).count() << endl;

	of1.close();
	//of2.close();
	//of3.close();
	//of4.close();

	return false;

}


void MeshBoolOperate::GenerateRemesh()
{
	Mesh* ta = &m_OriginMeshA;
	Mesh* tb = &m_OriginMeshB;

	RemshOperate* roA = new CDTRemesh(m_OriginMeshA, m_IntersectResult[&m_OriginMeshA], m_RemeshNewVertexRecord);
	m_RemeshNewVertexResult[&m_OriginMeshA] = m_RemeshNewVertexRecord;
	m_RemeshNewVertexRecord.clear();

	RemshOperate* roB = new CDTRemesh(m_OriginMeshB, m_IntersectResult[&m_OriginMeshB], m_RemeshNewVertexRecord);
	m_RemeshNewVertexResult[&m_OriginMeshB] = m_RemeshNewVertexRecord;
	
	m_RemeshEdgeResult[&m_OriginMeshB] = roB->run();

	m_RemeshEdgeResult[&m_OriginMeshA] = roA->run();
	

	//delete roA;
	delete roB;

}

void MeshBoolOperate::Classify()
{
	ClassifyOperate cR({ &m_OriginMeshA,&m_OriginMeshB }, m_RemeshEdgeResult, m_TriangleRecord);
	cR.run();
}

void MeshBoolOperate::Combine(Mesh& r_Result, const MeshBoolOperateType p_Type)
{

	CombineOperate co(m_OriginMeshA, m_OriginMeshB, p_Type, m_RemeshNewVertexResult, m_TriangleRecord);
	co.run(r_Result);
	
}

void MeshBoolOperate::CombineTest(Mesh& r_Result, const MeshBoolOperateType p_Type, Mesh& r_MeshA, Mesh& r_MeshB, bool needReverseNormal)
{
	CombineOperate co(m_OriginMeshA, m_OriginMeshB, p_Type, m_RemeshNewVertexResult, m_TriangleRecord);
	co.run(r_Result);

	co.GetMeshFrom2Models(r_MeshA, r_MeshB, needReverseNormal);
}
