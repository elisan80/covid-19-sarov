#pragma once
#include <string>

class Model;
class HumansMigration;

namespace translate
{
	/* ��������� ��� ��������� ������������� ��� SEIRD ������ �� ����������������� �����.
	*/
	struct SEIRDcfg
	{
		float alpha;
		float betta;
	};

	/*
		������� ������������ ������� ������ �� ���������� �������� � ���������� ���������
	*/
	int translate_data(Model* model, HumansMigration *phm);//����������� ���������������� ������

	int translate_step (Model* model, HumansMigration* phm, int day_idx); // ���������� ������ � ���������� ����������
	void ParseCmdLine(int argc, char* argv[], std::string& file_name1, std::string& file_name2, struct SEIRDcfg& seird_cfg);
};
